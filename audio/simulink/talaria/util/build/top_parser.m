classdef top_parser < handle
%top_parser Loads the generated TOP_Tokens.xml file and enables
%conversion between binary-packed payloads and string representations of
%TOP payloads
    
%   Copyright 2022 Bose Corporation
    properties
        ModelName
        XmlFilePath
        MapFPathToTopInfoStruct containers.Map
        MapTokenToFPath containers.Map
    end
    properties (Access=protected)
        FileSuffix = '_TOP_Tokens.xml'
        XmlDoc
    end
    methods
        %------------------------------------------------------------------
        function this = top_parser(modelName,fileSuffix)
            % top_parser Constructs a TOP parser object.
            % Given a model name, the parser attempts to find the generated
            % XML file to describe the tokens. Payload definitions are
            % manually known and described within this class
            this.ModelName = modelName;
            if (nargin >= 2)
                this.FileSuffix = fileSuffix;
            end
            this.XmlFilePath = payload_parser.GetXmlFilePath(modelName,this.FileSuffix);
            if (isempty(this.XmlFilePath))
                error('Could not find TOP XML for %s',modelName);
            end
            
            this.XmlDoc = xmlread(this.XmlFilePath);
            
            % Load the maps:
            this.MapFPathToTopInfoStruct = containers.Map;
            this.MapTokenToFPath = containers.Map;
            
            elemsTOP = this.XmlDoc.getElementsByTagName('TOP');
            for i = 0:elemsTOP.getLength-1
                % Transfer all the attributes into a struct
                topInfo = struct;
                anItem = elemsTOP.item(i);
                theAttributes = anItem.getAttributes();
                for count=1:theAttributes.getLength
                    attrib = theAttributes.item(count-1);
                    name = char(attrib.getName);
                    value = char(attrib.getValue);
                    if (isempty(value))
                        % Empty values are just empty
                        topInfo.(name) = [];
                    elseif (ismember(name,{'width','isComplex','coreIndex'}))
                        % Evaluate these into numeric values
                        topInfo.(name) = eval(value);
                    else
                        % Keep everything else as char
                        topInfo.(name) = value;
                    end
                end
                % Add to maps
                this.MapFPathToTopInfoStruct(topInfo.id) = topInfo;
                this.MapTokenToFPath(topInfo.writeToken) = topInfo.id;
                this.MapTokenToFPath(topInfo.readToken) = topInfo.id;
            end
        end
        %------------------------------------------------------------------
        function readToken = GetReadToken(this, featurePath)
            % GetReadToken returns string hex readToken for the specified
            % TOP featurePath string.
            % Usage:
            %    readToken = obj.GetReadToken(featurePath);
            mapStruct = this.MapFPathToTopInfoStruct(featurePath);
            readToken = mapStruct.readToken;
        end
        %------------------------------------------------------------------
        function writeToken = GetWriteToken(this, featurePath)
            % GetWriteToken returns string hex writeToken for the specified
            % TOP featurePath string.
            % Usage:
            %    writeToken = obj.GetWriteToken(featurePath);
            mapStruct = this.MapFPathToTopInfoStruct(featurePath);
            writeToken = mapStruct.writeToken;
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTopWriteFromEngineString(this, translationEngineString)
            % PackTopWriteFromEngineString packs a binary TOP write
            % request directly from a string generated by the translation
            % engine. These strings contain featurepath name, datatatype,
            % and a space-delimited list of values all in one string to set
            % the entire TOP.
            % Usage:
            %    binaryPayload = obj.PackTopWriteFromEngineString(this, translationEngineString);
            %
            % See also: ext_port_model.HandleAsdIdRequest, PackTopWriteFromString, ext_port_model.SetTOP

            translationEngineString = string(translationEngineString);
            if (numel(translationEngineString) > 1)
                error('Translation engine string appears to contain more than one translation. Send one at a time');
            end
            strParts = translationEngineString.split();
            % Extra validation: Make sure the datatype matches
            featurePath = strParts(1);
            dataTypeStr = strParts(2);
            strValues = strParts(3:end);
            topInfoStruct = this.MapFPathToTopInfoStruct(featurePath);
            if (false == strcmp(topInfoStruct.dataType,dataTypeStr))
                error('Engine string datatype [%s] does not match TOP datatype from XML definition [%s]', ...
                    topInfoStruct.dataType,dataTypeStr);
            end
            % Forward to the more flexible payload packing routine that
            % includes the ability to use offsets (although no need to use
            % one here)
            binaryPayload = this.PackTopWriteFromString(featurePath,strValues);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTopWriteFromString(this, featurePath, strValues, charOffset)
            % PackTopWrite Packs a TOP write request payload given
            % featurePath and string data (space-delimited or string array)
            % separately specifying featurePath, values, and charOffset.
            % Usage:
            %   binaryPayload = obj.PackTopWrite(featurePath, strValues, charOffset);
            % 
            % strValues can be a single space-delimited or a string array.
            % If not provided, charOffset is assumed to be 0.
            %
            % Example for a TOP of type single with write token 0x01000000:
            %    obj.PackTopWriteFromString('MyVector','1 2 3 4');
            %
            % See also: ext_port_model.HandleAsdIdRequest, PackTopWriteFromEngineString, ext_port_model.SetTOP

            if (nargin < 4)
                charOffset = 0;
            end

            % Retrieve specifications for this TOP.
            topInfoStruct = this.MapFPathToTopInfoStruct(featurePath);

            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(topInfoStruct.dataType);

            % Split the strValues if they are not already
            if (false == isstring(strValues))
                strValues = string(strValues);
            end
            if (numel(strValues) <= 1)
                strValues = strValues.split;
            end

            % Calculate how much data is to be set in uint8s
            widthToSet = numel(strValues);            
            charsToSet = widthToSet * dtBytes; % / (topInfoStruct.isComplex+1);

            % Arrange payload specs and content
            signalsList = "topId callerHandle offset byteCount data";
            signalValueList = [
                string(topInfoStruct.writeToken)    % topId
                "0"                         % callerHandle
                string(charOffset)         % charOffset
                string(charsToSet)         % byteCount
                strValues                   % data
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
    
            % Use a payload_parser specStruct to help pack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = topInfoStruct.id;

            for i=1:numel(signalNames)
                specStruct.sigName = signalNames(i);
                if ("data" == signalNames(i))
                    % data has special handling
                    specStruct.dataType = topInfoStruct.dataType;
                    specStruct.width = widthToSet;
                else
                    specStruct.dataType = GetSigDataType(specStruct.sigName);
                    specStruct.width = 1;
                end
                [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray,specStruct);
            end
            binaryPayload = specStruct.binaryPayload;
        end

        %------------------------------------------------------------------
        function binaryPayload = PackTopRead(this, featurePath, charOffset, byteCount)
            % PackTopRead Packs a TOP read request payload given
            % featurePath, offset, and byteCount
            % Usage:
            %   binaryPayload = obj.PackTopRead(this, featurePath, charOffset, byteCount);
            %
            % See also: ext_port_model.HandleAsdIdRequest, ext_port_model.GetTOP 

            if (nargin < 3)
                charOffset = 0;
            end

            % Retrieve specifications for this TOP,
            % and understand datatype's size in count of uint8s
            topInfoStruct = this.MapFPathToTopInfoStruct(featurePath);
            dtBytes = payload_parser.GetDataTypeBytes(topInfoStruct.dataType);

            % If no byteCount was given - read it all
            if (nargin < 4)
                byteCount = dtBytes * topInfoStruct.width - charOffset;
            end
            
            % Arrange payload specs and content
            signalsList = "topId callerHandle offset byteCount";
            signalValueList = [
                string(topInfoStruct.readToken)    % topId
                "0"                         % callerHandle
                string(charOffset)         % charOffset
                string(byteCount)         % byteCount
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
    
            % Use a payload_parser specStruct to help pack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = topInfoStruct.id;

            for i=1:numel(signalNames)
                specStruct.sigName = signalNames(i);
                specStruct.dataType = GetSigDataType(specStruct.sigName);
                specStruct.width = 1;
                [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray,specStruct);
            end
            binaryPayload = specStruct.binaryPayload;
        end

        %------------------------------------------------------------------
        function theStruct = UnpackRequest(this, binaryPayload)
            theStruct = this.UnpackPayload(binaryPayload,true);
        end
        %------------------------------------------------------------------
        function theStruct = UnpackResponse(this, binaryPayload)
            theStruct = this.UnpackPayload(binaryPayload,false);
        end
        %------------------------------------------------------------------
        function theStruct = UnpackPayload(this, binaryPayload, isRequest)
            % UnpackPayload Returns a MATLAB struct given a binary payload
            % and the knowledge of whether to unpack a request or response.
            % If not provided, isRequest is assumed false;
            % Usage:
            %   theStruct = obj.UnpackPayload(binaryPayload, isRequest)

            if (nargin < 3)
                isRequest = false;
            end
            
            % Retrieve the token from the payload and lookup feature path
            token = typecast(binaryPayload(1:4),'uint32');
            tokenStr = sprintf('0x%08x',token);
            featurePath = this.MapTokenToFPath(tokenStr);
            theStruct.CalcFeaturePath = featurePath;
            theStruct.CalcToken = tokenStr;

            % Understand TOP's datatype and size in count of uint8s
            topInfoStruct = this.MapFPathToTopInfoStruct(featurePath);
            dtBytes = payload_parser.GetDataTypeBytes(topInfoStruct.dataType);

            % Determine which of the 4 types of payloads to unpack
            if (bitand(token,0x80000000))
                % This is a READ
                if (isRequest)
                    % Read Request
                    theStruct.CalcPayloadType = 'TOP Read Request';
                    signalsList = "topId callerHandle offset byteCount";
                else
                    % Read Response
                    theStruct.CalcPayloadType = 'TOP Read Response';
                    signalsList = "topId callerHandle statusCode byteCount data";
                end
            else
                % This is a WRITE
                if (isRequest)
                    % Write request
                    theStruct.CalcPayloadType = 'TOP Write Request';
                    signalsList = "topId callerHandle offset byteCount data";
                else
                    % Write response
                    theStruct.CalcPayloadType = 'TOP Write Response';
                    signalsList = "topId callerHandle statusCode";
                end
            end

            % Use a payload_parser specStruct to help unpack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = featurePath;
            specStruct.binaryPayload = binaryPayload;
           
            % Pull out all signals using XML doc as payload specification
            signalNames = signalsList.split;
            for i=1:numel(signalNames)
                specStruct.sigName = char(signalNames(i));
                if ("data" == signalNames(i))
                    % the data payload element is variable size and type
                    specStruct.dataType = topInfoStruct.dataType;
                    specStruct.width = theStruct.byteCount / dtBytes;
                    % Include additional "calculated" information in result
                    theStruct.CalcTopDatatype = topInfoStruct.dataType;
                    theStruct.CalcWidth = specStruct.width;
                else                
                    specStruct.width = 1;
                    specStruct.dataType = GetSigDataType(specStruct.sigName);
                end
                [theStruct,specStruct] = payload_parser.UnpackPayloadElement(theStruct,specStruct);
                % If a status code has just been unpacked, also include
                % it's string translation:
                if ("statusCode" == signalNames(i))
                    theStruct.CalcStatusCodeStr = payload_parser.GetStatusCodeStr(theStruct.statusCode);
                end
            end

            % Diagnostic information
            unparsedCount = numel(binaryPayload) - specStruct.charItr;
            if (0 < unparsedCount)
                theStruct.CalcUnparsedBytes = unparsedCount;
                warning('[%d] additional bytes in payload that were not parsed', ...
                    unparsedCount);
            end
        end
    end
end

% Local helper function (e.g. like private static, but without hassle)
function dataType = GetSigDataType(sigName)
    switch(sigName)
        case 'statusCode'
            dataType = 'int32';
        otherwise
            dataType = 'uint32';
    end
end


