classdef payload_parser < handle
%payload_parser contains shared methods to pack/unpack binary payloads
    
%   Copyright 2022 Bose Corporation
    methods (Static)
        %------------------------------------------------------------------
        function XmlFilePath = GetXmlFilePath(modelName,fileSuffix)
            % Acquire the code generation folder and search it for the XML
            codeGenPath = Simulink.fileGenControl('get','CodeGenFolder');
            searchBase = fullfile(codeGenPath,modelName);
            searchString = [searchBase fileSuffix];
            namesStruct = dir(searchString);
            % Pick the one with the shortest filename
            dllCandidates = string({namesStruct(:).name});
            if (isempty(dllCandidates))
                error('Unable to find XML file for model [%s]',modelName);
            end
            [~,winnerIdx] = min(dllCandidates.strlength);
            fileName = namesStruct(winnerIdx).name;
            folder = namesStruct(winnerIdx).folder;
            XmlFilePath = fullfile(folder,fileName);
        end
        %------------------------------------------------------------------
        function matlabTypeName = C99TypeToMatlabTypName(C99TypeName)
            switch (C99TypeName)
                case 'int32_t'; matlabTypeName = 'int32';
                case 'uint32_t'; matlabTypeName = 'uint32';
                case 'float'; matlabTypeName = 'single';
                case 'double'; matlabTypeName = 'double';
                case 'int8_t'; matlabTypeName = 'int8';
                case 'uint8_t'; matlabTypeName = 'uint8';
                otherwise
                    error('Unsupported type name [%s]',C99TypeName);
            end
        end
        %------------------------------------------------------------------
        function specStruct = InitSpecStruct()
            % Sets up specStruct with expected fields
            specStruct = struct;
            specStruct.charItr = 1;
            specStruct.binaryPayload = uint8([]);
            specStruct.featurePath = '';
            specStruct.sigName = '';
            specStruct.dataType = '';
            specStruct.width = 0;
        end
        %------------------------------------------------------------------
        function [dtBytes,aZero] = GetDataTypeBytes(dataTypeName)
            % GetDataTypeBytes returns size of datatype in bytes and and
            % example zero
            % Usage:
            %    [dtBytes,aZero] = payload_parser.GetDataTypeBytes(dataTypeName);
            aZero = feval(dataTypeName,0); %#ok<NASGU> 
            zeroSpec = whos("aZero");
            dtBytes = zeroSpec.bytes;
        end
        %------------------------------------------------------------------
        function ntfStatus = GetNotificationCode()
            % GetNoficiationCode returns the status code present in all
            % notification payloads (e.g. "1").
            ntfStatus = int32(0x00000001);
        end
        %------------------------------------------------------------------
        function str = GetStatusCodeStr(statusCode)
            % GetStatusCodeStr Returns a string-version of several common
            % status codes (from ASDID.h) given a numeric value.
            switch(statusCode)
                case (0)
                    str='OK';
                case (-1)
                    str='RSP_BUFFER_SMALL';
                case (-2)
                    str='REQ_BUFFER_SMALL';
                case (-1000)
                    str='TOP_BAD_GROUP_ID';
                case (-1001)
                    str='TOP_BAD_BANK_ID';
                case (-1002)
                    str='TOP_OUT_OF_RANGE';
                case (-2000)
                    str='RTC_UNKNOWN_SERVICE';
                case (-3000)
                    str='TSP_BAD_STATE_ID';
                case (-3001)
                    str='TSP_CAPTURE_OVERFLOW';
                case (-3002)
                    str='ASDSTATUS_TSP_CAPTURE_ALL_OFF';
                case (-3003)
                    str='ASDSTATUS_TSP_CAPTURE_THIS_OFF';
                case (-3004)
                    str = 'ASDSTATUS_TSP_WRITE_IMPOSSIBLE';
                case (-4000)
                    str='VALUE_NOT_AVAILABLE';
                case (-4001)
                    str='NO_COMPAT_CODES';
                case (0x00000001)
                    str='RTC_NOTIFICATION';
                otherwise
                    code = typecast(int32(statusCode),'uint32');
                    str=sprintf('unknown code %d (0x%s)', ...
                        statusCode, pad(dec2hex(code),8,'left','0'));
            end
        end
        %------------------------------------------------------------------
        function [unpackStruct,specStruct] = UnpackPayloadElement(unpackStruct, specStruct)
            % UnpackPayloadElement Adds to provided unpackStruct based on
            % definitions and data in the provided specStruct. Both the
            % structs must be passed in/out as they change with each call.
            % Usage:
            %   [unpackStruct,specStruct] = payload_parser.UnpackPayloadElement(unpackStruct, specStruct)
            %
            % unpackStruct(*) is just a MATLAB struct
            % specStruct is struct with the following member elements:
            %  .charItr(*) : starting at 1, tracks binary location
            %  .binaryPayload : the payload to unpack (uint8 vector)
            %  .featurePath : feature path name used for error message
            %  .sigName : name of the element to unpack (char/string)
            %  .dataType : MATLAB data type name (char/string)
            %  .width : count of provided dataType to unpack (numeric)
            % (*) = this element is updated and must pass in/out
            
            % Understand datatype's size in count of uint8s
            [dtBytes,aZero] = payload_parser.GetDataTypeBytes(specStruct.dataType);

            % Prepare a native value of correct size and type
            nativeValue = repmat(aZero,1,specStruct.width);

            % Fill in the native value, parsing from binary payload
            for j=1:specStruct.width
                % Check that the data is present
                if (numel(specStruct.binaryPayload) < specStruct.charItr+dtBytes-1)
                    error('Supplied payload is too short. Ran out of data in %s %s element %d of %d', ...
                        specStruct.featurePath,specStruct.sigName,j,specStruct.width);
                end
                % Extract, cast, and move parsing iterator
                valueUint8s = specStruct.binaryPayload(specStruct.charItr:specStruct.charItr+dtBytes-1);
                nativeValue(j) = typecast(valueUint8s,specStruct.dataType);
                specStruct.charItr = specStruct.charItr + dtBytes;
            end
            unpackStruct.(specStruct.sigName) = nativeValue;
        end
        %------------------------------------------------------------------
        function [srcStrArray,specStruct] = PackPayloadElement(srcStrArray, specStruct)
            % PackPayloadElement Adds to binary payload based on
            % definitions and data in the provided specStruct. Values are
            % consumed and removed from provided srcStrArray which is
            % returned.
            % Usage:
            %   [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray, specStruct)
            %
            % srcStrArray string array of source values to consume
            % specStruct is struct with the following member elements:
            %  .charItr(*) : starting at 1, tracks binary location
            %  .binaryPayload(*) : the payload being packed (uint8 vector)
            %  .featurePath : feature path name used for error message
            %  .sigName : name of the element to unpack (char/string)
            %  .dataType : MATLAB data type name (char/string)
            %  .width : count of provided dataType to unpack (numeric)
            % (*) = this element is updated and must pass in/out
            
            % Understand datatype's size in count of uint8s
            dtBytes = payload_parser.GetDataTypeBytes(specStruct.dataType);

            % Track position in the srcStrArray
            wordItr = 1;

            % Fill in the native value, consuming from srcStrArray
            typeObj = numerictype(specStruct.dataType);
            for j=1:specStruct.width
                % Check that the data is present
                if (numel(srcStrArray) < wordItr)
                    error('Missing value #%d in [%s] element [%s]', ...
                        j, specStruct.featurePath, specStruct.sigName);
                end

                % Grab, evaluate to numeric, cast to type, then uint8
                valueStr = srcStrArray(wordItr);
                valueNum = eval(valueStr);
                valueNativeObj = fi(valueNum,typeObj);
                valueUint8s = typecast(valueNativeObj.storedInteger,'uint8');
                wordItr = wordItr + 1;

                % Pack it
                specStruct.binaryPayload(specStruct.charItr:specStruct.charItr+dtBytes-1) = valueUint8s;
                specStruct.charItr = specStruct.charItr + dtBytes;
            end

            % Consume the words from the srcStrArray
            srcStrArray(1:wordItr-1) = [];
        end
        %------------------------------------------------------------------
        function flattened = FlattenData(data, isComplex)
            % If the data is complex
            if (isComplex)
                % Pre-flatten it
                data = reshape(data,1,numel(data));
                % Make sure it's complex
                data = complex(data);
                % Extract the real and imaginary parts
                dataReal = real(data);
                dataImg = imag(data);
                interleave = [dataReal;dataImg];
                data = transpose(interleave(:));
                % Width is just numeric - double for complex numbers
                widthToSet = widthToSet * 2;
            end
            flattened = reshape(data,1,numel(data));
        end
    end
end



