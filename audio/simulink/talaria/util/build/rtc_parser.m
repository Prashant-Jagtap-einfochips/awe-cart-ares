classdef rtc_parser < handle
%rtc_parser Loads the generated RTC_Payloads.xml file and enables
%conversion between binary-packed payloads and string representations of
%RTC payloads
    
%   Copyright 2020 Bose Corporation
    properties
        ModelName
        XmlFilePath
        MapFPathToToken containers.Map
        MapTokenToFPath containers.Map
    end
    properties (Access=protected)
        FileSuffix = '_RTC_Payloads.xml'
        XmlDoc
        % FeaturePath-to-XML element maps
        ReqMap containers.Map
        RspMap containers.Map
        NtfMap containers.Map
    end
    
    methods
        function this = rtc_parser(modelName,fileSuffix)
            % rtc_parser Constructs an RTC parser object.
            % Given a model name, the parser attempts to find the generated
            % {model-name}_RTC_Payloads.XML file to describe the payloads.
            this.ModelName = modelName;
            if (nargin >= 2)
                this.FileSuffix = fileSuffix;
            end
            this.XmlFilePath = payload_parser.GetXmlFilePath(modelName,this.FileSuffix);
            if (isempty(this.XmlFilePath))
                error('Could not find RTC XML for %s',modelName);
            end
            
            this.XmlDoc = xmlread(this.XmlFilePath);
            
            % Load the maps:
            this.MapFPathToToken = containers.Map;
            this.MapTokenToFPath = containers.Map;
            this.ReqMap = containers.Map;
            this.RspMap = containers.Map;
            this.NtfMap = containers.Map;
            
            elemsRTC = this.XmlDoc.getElementsByTagName('RTC');
            for i = 0:elemsRTC.getLength-1
                anItem = elemsRTC.item(i);
                featurePath = char(anItem.getAttribute('id'));
                token = char(anItem.getAttribute('msgId'));
                sizesItem = anItem.getElementsByTagName('PayloadSizes').item(0);
                mapStruct = struct;
                mapStruct.token = token;
                mapStruct.reqSize = eval(sizesItem.getAttribute('REQ'));
                mapStruct.rspSize = eval(sizesItem.getAttribute('RSP'));
                mapStruct.ntfSize = eval(sizesItem.getAttribute('NTF'));
                this.MapFPathToToken(featurePath) = mapStruct;
                % This is the only map that keys off something other than a
                % feature path
                this.MapTokenToFPath(token) = featurePath;
            end
            
            elems = this.XmlDoc.getElementsByTagName('REQ');
            for i = 0:elems.getLength-1
                anItem = elems.item(i);
                featurePath = char(anItem.getAttribute('id'));
                this.ReqMap(featurePath) = anItem.getElementsByTagName('Signal');
            end
            elems = this.XmlDoc.getElementsByTagName('RSP');
            for i = 0:elems.getLength-1
                anItem = elems.item(i);
                featurePath = char(anItem.getAttribute('id'));
                this.RspMap(featurePath) = anItem.getElementsByTagName('Signal');
            end
            elems = this.XmlDoc.getElementsByTagName('NTF');
            for i = 0:elems.getLength-1
                anItem = elems.item(i);
                featurePath = char(anItem.getAttribute('id'));
                this.NtfMap(featurePath) = anItem.getElementsByTagName('Signal');
            end
        end

        function binaryPayload = PackRequest(this, requestString)
            % PackRequest Returns a packed binary RTC payload given a
            % string expression of the request.
            % Usage:
            %    binaryReqPayload = obj.PackRequest(requestString);
            % Example:
            %    req = obj.PackRequest('RumbleGainSet 1234 120 0 2000 0')

            % Break up the request string into words, recognize the first
            % one as a feature path name, and use map to retrieve the token
            srcStrArray = strsplit(string(requestString),' ');
            featurePath = srcStrArray(1);
            mapStruct = this.MapFPathToToken(featurePath);

            % Use a payload_parser specStruct to help pack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = featurePath;
            specStruct.binaryPayload = zeros(1,mapStruct.reqSize,'uint8');

            % Replace the featurepath name with token
            srcStrArray(1) = mapStruct.token;

            % Get all the request signals
            reqSigs = this.ReqMap(featurePath);
            
            % Pack all the signals into the payload
            for i=0:reqSigs.getLength()-1
                aSig = reqSigs.item(i);
                specStruct.sigName = char(aSig.getAttribute('id'));
                specStruct.dataType = payload_parser.C99TypeToMatlabTypName(char(aSig.getAttribute('dtypeStr')));
                specStruct.width = eval(aSig.getAttribute('width'));
                [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray,specStruct);
            end

            % Return the packed binary payload
            binaryPayload = specStruct.binaryPayload;
        end
        
        function theStruct = UnpackResponse(this, binaryPayload)
            % UnpackResponse Returns a MATLAB struct representing the
            % supplied binary payload interpretted as an RTC response.
            % Usage:
            %    theStruct = UnpackResponse(this, binaryPayload)
            theStruct = this.UnpackPayload(binaryPayload,this.RspMap);
            theStruct.CalcPayloadType = 'RTC Response';
        end
            
        function theStruct = UnpackRequest(this, binaryPayload)
            % UnpackRequest Returns a MATLAB struct representing the
            % supplied binary payload interpretted as an RTC request.
            % Usage:
            %    theStruct = UnpackRequest(this, binaryPayload)
            theStruct = this.UnpackPayload(binaryPayload,this.ReqMap);
            theStruct.CalcPayloadType = 'RTC Request';
        end
               
        function theStruct = UnpackNotification(this, binaryPayload)
            % UnpackNotification Returns a MATLAB struct representing the
            % supplied binary payload interpretted as an RTC notification.
            % Usage:
            %    theStruct = UnpackNotification(this, binaryPayload)
            theStruct = this.UnpackPayload(binaryPayload,this.NtfMap);
            theStruct.CalcPayloadType = 'RTC Notification';
        end
    end

    % Internal use methods not exposed in the public interface
    methods (Access=protected)
        function theStruct = UnpackPayload(this, binaryPayload, payloadSpec)
            % UnpackPayload Returns a MATLAB struct given a  specification
            % from one of the the payload maps.
            % Usage:
            %   theStruct = obj.UnpackPayload(binaryRspPayload, payloadSpec)
            
            % Retrieve the token from the payload and lookup feature path
            token = typecast(binaryPayload(1:4),'uint32');
            tokenStr = sprintf('0x%08x',token);
            featurePath = this.MapTokenToFPath(tokenStr);
            theStruct.CalcFeaturePath = featurePath;
            theStruct.CalcToken = tokenStr;

            % Use a payload_parser specStruct to help unpack the request
            specStruct = payload_parser.InitSpecStruct();
            specStruct.featurePath = featurePath;
            specStruct.binaryPayload = binaryPayload;
           
            % Pull out all signals using XML doc as payload specification
            theSigs = payloadSpec(featurePath);
            for i=0:theSigs.getLength()-1
                aSig = theSigs.item(i);
                specStruct.sigName = char(aSig.getAttribute('id'));
                specStruct.width = eval(aSig.getAttribute('width'));
                specStruct.dataType = payload_parser.C99TypeToMatlabTypName(char(aSig.getAttribute('dtypeStr')));
                [theStruct,specStruct] = payload_parser.UnpackPayloadElement(theStruct,specStruct);
                if ("status_code" == specStruct.sigName)
                    theStruct.CalcStatusCodeStr = payload_parser.GetStatusCodeStr(theStruct.status_code);
                    if (theStruct.status_code < 0)
                        % The payload represents an error condition! In
                        % that case, the payload should end here
                        if (numel(specStruct.binaryPayload) < specStruct.charItr)
                            return;
                        end
                    end
                end
            end
        end        
    end
    
end



