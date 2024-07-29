classdef tsp_parser < handle
%tsp_parser Loads the generated TSP_Tokens.xml file and enables
%conversion between binary-packed payloads and string representations of
%TSP payloads
    
%   Copyright 2022 Bose Corporation
    properties
        ModelName
        XmlFilePath
        MapFPathToTspInfoStruct containers.Map
        MapBlindReadTokenToFPath containers.Map
        MapBlindWriteTokenToFPath containers.Map
        MapAddTokenToFPath containers.Map
        MapSubTokenToFPath containers.Map
        MapRetrieveTokenToFPath containers.Map
        MapCaptureTokenToCoreIndex containers.Map
        MapClearTokenToCoreIndex containers.Map
        MapCoreIndexToCoreInfoStruct containers.Map
        % TODO: what about capture and clear tokens?
    end
    properties (Access=protected)
        FileSuffix = '_TSP_Tokens.xml'
        XmlDoc
    end
    methods
        %------------------------------------------------------------------
        function this = tsp_parser(modelName,fileSuffix)
            % tsp_parser Constructs a TSP parser object.
            % Given a model name, the parser attempts to find the generated
            % XML file to describe the tokens. Payload definitions are
            % manually known and described within this class
            this.ModelName = modelName;
            if (nargin >= 2)
                this.FileSuffix = fileSuffix;
            end
            this.XmlFilePath = payload_parser.GetXmlFilePath(modelName,this.FileSuffix);
            if (isempty(this.XmlFilePath))
                error('Could not find TSP XML for %s',modelName);
            end
            
            this.XmlDoc = xmlread(this.XmlFilePath);
            
            % Load the maps:
            this.MapFPathToTspInfoStruct = containers.Map;
            this.MapBlindReadTokenToFPath = containers.Map;
            this.MapBlindWriteTokenToFPath = containers.Map;
            this.MapAddTokenToFPath = containers.Map;
            this.MapSubTokenToFPath = containers.Map;
            this.MapRetrieveTokenToFPath = containers.Map;
            this.MapCaptureTokenToCoreIndex = containers.Map;
            this.MapClearTokenToCoreIndex = containers.Map;
            this.MapCoreIndexToCoreInfoStruct = containers.Map;
            
            % Acquire all the information about Target State Probes
            elemsTSP = this.XmlDoc.getElementsByTagName('TSP');
            for i = 0:elemsTSP.getLength-1
                % Transfer all the attributes into a stuct
                tspInfo = struct;
                anItem = elemsTSP.item(i);
                theAttributes = anItem.getAttributes();
                for count=1:theAttributes.getLength
                     attrib = theAttributes.item(count-1);
                     name = char(attrib.getName);
                     value = char(attrib.getValue);
                     if (ismember(name,{'width'}))
                         % Store some values as numeric
                         tspInfo.(name) = eval(value);
                     else
                         % Leave the others as strings
                         tspInfo.(name) = value;
                     end
                end
                % Add to maps
                this.MapFPathToTspInfoStruct(tspInfo.id) = tspInfo;
                this.MapBlindReadTokenToFPath(tspInfo.blindReadToken) = tspInfo.id;
                this.MapBlindWriteTokenToFPath(tspInfo.blindWriteToken) = tspInfo.id;
                this.MapAddTokenToFPath(tspInfo.addToken) = tspInfo.id;
                this.MapSubTokenToFPath(tspInfo.subToken) = tspInfo.id;
                this.MapRetrieveTokenToFPath(tspInfo.retrieveToken) = tspInfo.id;
            end

            % Acquire all the information about per-core triggered capture
            elemsCore = this.XmlDoc.getElementsByTagName('Core');
            for i = 0:elemsCore.getLength-1
                % Transfer all the attributes into a stuct
                tspInfo = struct;
                anItem = elemsCore.item(i);
                theAttributes = anItem.getAttributes();
                for count=1:theAttributes.getLength
                     attrib = theAttributes.item(count-1);
                     name = char(attrib.getName);
                     value = char(attrib.getValue);
                     if (ismember(name,{'width'}))
                         % Store some values as numeric
                         tspInfo.(name) = eval(value);
                     else
                         % Leave the others as strings
                         tspInfo.(name) = value;
                     end
                end
                this.MapCaptureTokenToCoreIndex(tspInfo.captureToken) = tspInfo.coreIndex;
                this.MapClearTokenToCoreIndex(tspInfo.clearToken) = tspInfo.coreIndex;
                this.MapCoreIndexToCoreInfoStruct(tspInfo.coreIndex) = tspInfo;
            end            
        end
        %------------------------------------------------------------------
        function readToken = GetBlindReadToken(this, featurePath)
            % GetReadToken returns string hex token for the specified
            % TSP featurePath string.
            mapStruct = this.MapFPathToTspInfoStruct(featurePath);
            readToken = mapStruct.blindReadToken;
        end
        %------------------------------------------------------------------
        function readToken = GetBlindWriteToken(this, featurePath)
            % GetReadToken returns string hex token for the specified
            % TSP featurePath string.
            mapStruct = this.MapFPathToTspInfoStruct(featurePath);
            readToken = mapStruct.blindWriteToken;
        end
        %------------------------------------------------------------------
        function readToken = GetAddToken(this, featurePath)
            % GetReadToken returns string hex token for the specified
            % TSP featurePath string.
            mapStruct = this.MapFPathToTspInfoStruct(featurePath);
            readToken = mapStruct.addToken;
        end
        %------------------------------------------------------------------
        function readToken = GetSubToken(this, featurePath)
            % GetReadToken returns string hex token for the specified
            % TSP featurePath string.
            mapStruct = this.MapFPathToTspInfoStruct(featurePath);
            readToken = mapStruct.subToken;
        end
        %------------------------------------------------------------------
        function readToken = GetRetrieveToken(this, featurePath)
            % GetReadToken returns string hex token for the specified
            % TSP featurePath string.
            mapStruct = this.MapFPathToTspInfoStruct(featurePath);
            readToken = mapStruct.retrieveToken;
        end
        %------------------------------------------------------------------
        function captureToken = GetCaptureToken(this, coreIndex)
            % GetCaptureToken returns string hex token for the specified
            % TSP triggered capture.
            % GetCaptureToken(this,1);
            coreIndexStr = num2str(coreIndex);
            mapStruct = this.MapCoreIndexToCoreInfoStruct(coreIndexStr);
            captureToken = mapStruct.captureToken;
        end
        %------------------------------------------------------------------
        function clearToken = GetClearToken(this, coreIndex)
            % GetClearToken returns string hex token for the specified
            % TSP triggered capture list clear.
            % GetClearToken(this,1);
            coreIndexStr = num2str(coreIndex);
            mapStruct = this.MapCoreIndexToCoreInfoStruct(coreIndexStr);
            clearToken = mapStruct.clearToken;
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspBlindRead(this, featurePath, charOffset, charCount)
            % PackTspBlindRead packs a binary TSP blindread
            % request 
            % Usage:
            %    binaryPayload = PackTspBlindRead(this, featurePath, charOffset, charCount);
            %
            % See also: ext_port_model.HandleAsdIdRequest
            tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);

            if (nargin < 3)
                % By default, start at the beginning
                charOffset = 0;
            end
            if (nargin < 4)
                % By default, grab the whole thing!
                charCount = tspInfoStruct.width * dtBytes;
            end

            signalsList = "msgId callerHandle charOffset charCount";
            signalValueList = [
                string(tspInfoStruct.blindReadToken) % msgId
                "0" % callerHandle
                string(charOffset) % charOffset
                string(charCount) % charCount                
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = [tspInfoStruct.id ' Blind Read'];
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspBlindWrite(this, featurePath, charOffset, charCount, strValues)
            % PackTspBlindWrite packs a binary TSP blindwrite
            % request 
            % Usage:
            %    binaryPayload = PackTspBlindWrite(this, featurePath, charOffset, charCount, strValues);
            %
            % See also: ext_port_model.HandleAsdIdRequest
            tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);

            if (nargin < 3)
                % By default, start at the beginning
                charOffset = 0;
            end
            if (nargin < 4)
                % By default, grab the whole thing!
                charCount = tspInfoStruct.width * dtBytes;
            end

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
            signalsList = "msgId callerHandle charOffset charCount data";
            signalValueList = [
                string(tspInfoStruct.blindWriteToken) % msgId
                "0" % callerHandle
                string(charOffset) % charOffset
                string(charCount) % charCount
                strValues         % data
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;

            % Use a payload_parser specStruct to help pack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = [tspInfoStruct.id ' Blind Write'];

            for i=1:numel(signalNames)
                specStruct.sigName = signalNames(i);
                if ("data" == signalNames(i))
                    % data has special handling
                    specStruct.dataType = tspInfoStruct.dataType;
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
        function binaryPayload = PackTspListAdd(this, featurePath, captureCount, skipInterval)
            % PackTspListAdd packs a binary TSP list add
            % request 
            % Usage:
            %    binaryPayload = PackTspListAdd(this, featurePath, captureCount, skipInterval);
            %
            % See also: ext_port_model.HandleAsdIdRequest
            tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);

            if (nargin < 3)
                % By default, just capture one time
                captureCount = 1;
            end
            if (nargin < 4)
                % By default, don't skip anything
                skipInterval = 0;
            end

            signalsList = "msgId callerHandle captureCount skipInterval";
            signalValueList = [
                string(tspInfoStruct.addToken) % msgId
                "0" % callerHandle
                string(captureCount) % charOffset
                string(skipInterval) % charCount                
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = [tspInfoStruct.id ' List Add'];
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspListSub(this, featurePath)
            % PackTspListSub packs a binary TSP list subtraction
            % request 
            % Usage:
            %    binaryPayload = PackTspListSub(this, featurePath);
            %
            % See also: ext_port_model.HandleAsdIdRequest
            tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);

            signalsList = "msgId callerHandle";
            signalValueList = [
                string(tspInfoStruct.subToken) % msgId
                "0" % callerHandle
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = [tspInfoStruct.id ' List Subtract'];
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspRetrieve(this, featurePath,captureIndex,charOffset,charCount)
            % PackTspListSub packs a binary TSP captured data retrieval
            % request 
            % Usage:
            %    binaryPayload = PackTspRetrieve(this, featurePath,captureIndex,charOffset,charCount);
            %
            % See also: ext_port_model.HandleAsdIdRequest
            tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);

            if (nargin < 3)
                % By default, retrieve the first one
                captureIndex = 0;
            end
            if (nargin < 4)
                % By default, start at the beginning
                charOffset = 0;
            end
            if (nargin < 5)
                % By default, retrieve the whole thing for this index
                charCount = tspInfoStruct.width * dtBytes;
            end

            signalsList = "msgId callerHandle captureIndex charOffset charCount";
            signalValueList = [
                string(tspInfoStruct.retrieveToken) % msgId
                "0" % callerHandle
                num2str(captureIndex) % captureIndex
                num2str(charOffset) % charOffset
                num2str(charCount) % charCount
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = [tspInfoStruct.id ' Capture Retrieval'];
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspCapture(this, coreIndex)
            % PackTspCapture creates a binary payload to trigger a capture
            % on the core indicated.
            % Usage:
            %   binaryPayload = PackTspCapture(this,coreIndex);
            % e.g. reqBin = obj.PackTspCapture(1);
            captureToken = this.GetCaptureToken(coreIndex);
            signalsList = "msgId callerHandle";
            signalValueList = [
                captureToken % msgId
                "0" % callerHandle
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = sprintf('Trigger capture on core %d',coreIndex);
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
        end
        %------------------------------------------------------------------
        function binaryPayload = PackTspClear(this, coreIndex)
            % PackTspClear creates a binary payload to clear thecapture
            % list on the core indicated.
            % Usage:
            %   binaryPayload = PackTspClear(this,coreIndex);
            % e.g. reqBin = obj.PackTspClear(1);
            captureToken = this.GetClearToken(coreIndex);
            signalsList = "msgId callerHandle";
            signalValueList = [
                captureToken % msgId
                "0" % callerHandle
                ];
            signalNames = signalsList.split(' ');
            srcStrArray = signalValueList;
            featurePath = sprintf('Clear capture list on core %d',coreIndex);
            binaryPayload = PackHelper(featurePath,signalNames,srcStrArray);
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
            % If not provided, isRequest is assumed false
            % Usage:
            %   theStruct = objUnpackPayload(this,binaryPayload, isRequest)

            if (nargin < 3)
                isRequest = false;
            end
            
            % Retrieve the token from the payload
            token = typecast(binaryPayload(1:4),'uint32');
            tokenStr = sprintf('0x%08x',token);
            theStruct.CalcToken = tokenStr;

            % Assume the payload might have typed data
            hasData = true;

            % Determine which of the 4 types of payloads to unpack
            % (TODO does not yet recognize trigger-capture or clear-list)
            if (this.MapBlindReadTokenToFPath.isKey(tokenStr))
                % This is a BLIND READ
                featurePath = this.MapBlindReadTokenToFPath(tokenStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP Blind Read Request';
                    signalsList = "msgId callerHandle charOffset charCount";
                else
                    theStruct.CalcPayloadType = 'TSP Blind Read Response';
                    signalsList = "msgId callerHandle statusCode charCount data";
                end
            elseif (this.MapBlindWriteTokenToFPath.isKey(tokenStr))
                % This is a BLIND WRITE
                featurePath = this.MapBlindWriteTokenToFPath(tokenStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP Blind Write Request';
                    signalsList = "msgId callerHandle charOffset charCount data";
                else
                    theStruct.CalcPayloadType = 'TSP Blind Write Response';
                    signalsList = "tspId callerHandle statusCode";
                end
            elseif (this.MapAddTokenToFPath.isKey(tokenStr))
                % This is a LIST ADD
                featurePath = this.MapAddTokenToFPath(tokenStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP List Add Request';
                    signalsList = "msgId callerHandle captureCount skipInterval";
                else
                    theStruct.CalcPayloadType = 'TSP List Add Response';
                    signalsList = "msgId callerHandle statusCode";
                end
            elseif (this.MapSubTokenToFPath.isKey(tokenStr))
                % This is a LIST SUB
                featurePath = this.MapSubTokenToFPath(tokenStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP List Subtract Request';
                    signalsList = "msgId callerHandle";
                else
                    theStruct.CalcPayloadType = 'TSP List Subract Response';
                    signalsList = "msgId callerHandle statusCode";
                end
            elseif (this.MapRetrieveTokenToFPath.isKey(tokenStr))
                % This is a RETRIEVE
                featurePath = this.MapRetrieveTokenToFPath(tokenStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP Retrieve Request';
                    signalsList = "msgId callerHandle captureIndex charOffset charCount";
                else
                    theStruct.CalcPayloadType = 'TSP Retrieve Response';
                    signalsList = "msgId callerHandle statusCode charCount data";
                end
            elseif (this.MapCaptureTokenToCoreIndex.isKey(tokenStr))
                % This is a Capture
                hasData = false;
                coreIndexStr = this.MapCaptureTokenToCoreIndex(tokenStr);
                featurePath = sprintf('Capture on core %s',coreIndexStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP Capture Request';
                    signalsList = "msgId callerHandle";
                else
                    % Capture response assumed, but will automatically
                    % adjust to become notification based on discovered
                    % statusCode
                    theStruct.CalcPayloadType = 'TSP Capture Response';
                    signalsList = "msgId callerHandle statusCode";
                end
            elseif (this.MapClearTokenToCoreIndex.isKey(tokenStr))
                % This is a Clear of the list
                hasData = false;
                coreIndexStr = this.MapClearTokenToCoreIndex(tokenStr);
                featurePath = sprintf('Capture on core %s',coreIndexStr);
                if (isRequest)
                    theStruct.CalcPayloadType = 'TSP Clear List Request';
                    signalsList = "msgId callerHandle";
                else
                    theStruct.CalcPayloadType = 'TSP Clear List Response';
                    signalsList = "msgId callerHandle statusCode";
                end
            else
                error('Token [%s] is not recognized as a TSP token for [%s]', ...
                    tokenStr,this.ModelName);
            end

            % Understand TSP's datatype and size in count of uint8s
            theStruct.CalcFeaturePath = featurePath;
            if (hasData)
                tspInfoStruct = this.MapFPathToTspInfoStruct(featurePath);
                dtBytes = payload_parser.GetDataTypeBytes(tspInfoStruct.dataType);
            end

            % Use a payload_parser specStruct to help unpack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = featurePath;
            specStruct.binaryPayload = binaryPayload;
           
            % Pull out all signals using XML doc as payload specification
            signalNames = signalsList.split;
            i = 1;
            while i <= numel(signalNames)
                specStruct.sigName = char(signalNames(i));
                if (hasData && "data" == signalNames(i))
                    % the data payload element is variable size and type
                    specStruct.dataType = tspInfoStruct.dataType;
                    specStruct.width = theStruct.charCount / dtBytes;
                    % Include additional "calculated" information in result
                    theStruct.CalcTspDatatype = tspInfoStruct.dataType;
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
                    if (payload_parser.GetNotificationCode == theStruct.statusCode)
                        % This is a notification! Change payload format:
                        theStruct.CalcPayloadType = 'TSP Capture Complete Notification';
                        signalsList = "msgId callerHandle statusCode totalCaptureCount";
                        signalNames = signalsList.split;
                    elseif (0 ~= theStruct.statusCode)
                        % This is a failure status response. There are no
                        % more expected contents for the payload
                        break;
                    end
                end
                i = i+1;
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

function binaryPayload = PackHelper(featurePath,signalNames,srcStrArray)
    % Use a payload_parser specStruct to help pack the request
    specStruct = payload_parser.InitSpecStruct();
    specStruct.featurePath = featurePath;

    for i=1:numel(signalNames)
        specStruct.sigName = signalNames(i);
        specStruct.dataType = GetSigDataType(specStruct.sigName);
        specStruct.width = 1;
        [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray,specStruct);
    end
    binaryPayload = specStruct.binaryPayload;
end
