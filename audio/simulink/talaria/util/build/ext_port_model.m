classdef ext_port_model < handle
%ext_port_model Loads a model with Talaria ExtPorts into MATLAB.
%   Given the name of the original model, this routine seaches the code
%   generation folder for the dll. Class methods include query and access
%   to Model-External-In and Model-External-Out buffers as well as
%   synchronous processing calls. Additionally a low-level entrypoint for
%   asynchronous communication is provided for TOP,RTC,and TSP payloads.
    
%   Copyright 2018-2019 Bose Corporation
    properties
        ModelName
        DllName
    end
    
    methods (Static)
        function dllName = GetDllName(modelName)
            % Acquire the code generation folder and search it for a
            % matching DLL
            codeGenPath = Simulink.fileGenControl('get','CodeGenFolder');
            searchBase = fullfile(codeGenPath,modelName);
            searchString = [searchBase '*.dll'];
            namesStruct = dir(searchString);
            dllCandidates = string({namesStruct(:).name});
            [~,winnerIdx] = min(dllCandidates.strlength);
            [~,dllName] = fileparts(dllCandidates(winnerIdx).char);
        end
        function UnloadDll(modelName)
            dllName = ext_port_model.GetDllName(modelName);
            if (false == isempty(dllName) && libisloaded(dllName))
                fprintf('%s unloaded\n',dllName);
                unloadlibrary(dllName);
            end
        end
    end
    
    methods
        function this = ext_port_model(modelName)
            this.ModelName = modelName;
            
            this.DllName = ext_port_model.GetDllName(modelName);
            if (isempty(this.DllName))
                error('Could not find DLL for %s',modelName);
            end
            
            % Now find the folder where this DLL's code headers are found
            codeGenPath = Simulink.fileGenControl('get','CodeGenFolder');
            folderName = [modelName '_ert_shrlib_rtw'];
            folderPath = fullfile(codeGenPath,folderName);
            dllHeader = fullfile(folderPath,[modelName '_TalariaAPI.h']);
            
            % Try to load the library
            [notfound,warnings] = loadlibrary(this.DllName,dllHeader);
           
            % Automatically initialize the model
            this.Initialize();
        end

        function Initialize(this)
            % Cold start the entire model:
            calllib(this.DllName,[this.ModelName '_initialize']);
        end

        % ----- Access to model INPUT buffers (sending data into model)
        function [count] = GetInBufferCount(this)
            % GetInBufferCount Returns the number of input ports available
            % in the model
            count = calllib(this.DllName,'GetInBufferCount');
        end
        function [bufferNumber] = GetInBufferNumber(this, bufferName)
            % GetInBufferNumber Searches inports based on the provided name
            % and returns the number of the matching port, or "-1" if no
            % match was found.
            % Usage: bufferNumber = obj.GetInBufferNumber('buffername')
            bufferNumber = calllib(this.DllName,'GetInBufferNumber',bufferName);
        end
        function [bufSpec] = GetInBufferSpec(this, bufferNumber)
            % GetInBufferSpec Returns a structure of details about the
            % buffer specified by number
            % Usage: bufSpec = obj.GetInBufferSpace(bufferNumber);
            bufSpec = calllib(this.DllName,'GetInBufferSpec',bufferNumber,struct);
            if (isempty(bufSpec.buffer_ptr))
                error('InBuffer #%d not found',bufferNumber);
            end
            bufSpec.buffer_ptr.setdatatype([bufSpec.datatype_name 'Ptr'], ...
                bufSpec.frame_samples,bufSpec.channel_count);
        end
        function SetInBufferData(this, bufferNumber, data)
            % SetInBufferData Writes provided data into the model buffer
            % number specified. Use GetInBufferSpec to find out what
            % datatype and dimensions are expected.
            % Usage: obj.SetInBufferData,bufferNumber,data)
            
            % Get the buffer spec and verify provided data matches
            [bufSpec] = this.GetInBufferSpec(bufferNumber);
            dataDims = size(data);
            bufDims = [bufSpec.frame_samples, bufSpec.channel_count];
            if (false == isequal(dataDims,bufDims))
                error('Dimensions of provided data are %s. Expected %s', ...
                    mat2str(dataDims), mat2str(bufDims));
            end
            if (false == strcmp(class(data),bufSpec.datatype_name))
                error('Datatype of provided data is [%s]. Expected [%s]', ...
                    class(data),bufSpec.datatype_name);
            end
            % Go ahead and deliver the data to the model
            dataBytes = typecast(data,'uint8');
            retCode = calllib(this.DllName,'SetInBufferData',bufferNumber,dataBytes);
        end
        
        % ----- Access to model OUTPUT buffers (reading data out of model)
        function [count] = GetOutBufferCount(this)
            % GetOutBufferCount Returns the number of output ports available
            % in the model
            count = calllib(this.DllName,'GetOutBufferCount');
        end
        function [bufferNumber] = GetOutBufferNumber(this, bufferName)
            % GetOutBufferNumber Searches outports based on the provided name
            % and returns the number of the matching port, or "-1" if no
            % match was found.
            % Usage: bufferNumber = obj.GetOutBufferNumber('buffername')
            bufferNumber = calllib(this.DllName,'GetOutBufferNumber',bufferName);
        end
        function [bufSpec] = GetOutBufferSpec(this, bufferNumber)
            % GetOutBufferSpec Returns a structure of details about the
            % buffer specified by number
            % Usage: bufSpec = obj.GetOutBufferSpace(bufferNumber);
            bufSpec = calllib(this.DllName,'GetOutBufferSpec',bufferNumber,struct);
            if (isempty(bufSpec.buffer_ptr))
                error('OutBuffer #%d not found',bufferNumber);
            end
            bufSpec.buffer_ptr.setdatatype([bufSpec.datatype_name 'Ptr'], ...
                bufSpec.frame_samples,bufSpec.channel_count)
        end
        function data = GetOutBufferData(this, bufferNumber)
            % GetOutBufferData Reads buffer data from the model buffer
            % number specified.
            % Usage: data = obj.GetOutBufferData,bufferNumber)

            % Get the buffer spec and verify provided data matches
            bufSpec = this.GetOutBufferSpec(bufferNumber);
            % Retrieve the data from the model
            buffer = calllib(this.DllName,'GetOutBufferData',bufferNumber);
            buffer.setdatatype([bufSpec.datatype_name 'Ptr'], ...
                bufSpec.frame_samples,bufSpec.channel_count);
            data = buffer.Value;
        end    
        
        % ----- Synchronous processing
        function [count] = GetSyncTaskCount(this)
            % GetSyncTaskCount Returns the number of synchronous tasks
            % contained by the model.
            count = calllib(this.DllName,'GetSyncTaskCount');
        end
        function [theFlags] = UpdateStepFlags(this)
            % UpdateStepFlags Returns a boolean array indicating whether
            % the model expects the corresponding synchronous task to run
            % during the current base frame. Array index 1 corresponds to
            % step task 0 and so forth.
            % MATLAB array indexes are 1-based while ProcessStep task
            % indexes are 0-based.
            flags = libpointer('uint8Ptr',zeros(1,this.GetSyncTaskCount()));
            theFlags = calllib(this.DllName,'UpdateStepFlags',flags);
        end
        function ProcessStep(this, stepNumber)
            % ProcessStep Executes the indicated synchronous processing
            % task and returns upon completion.
            % Usage: obj.ProcessStep(stepNumber)
            calllib(this.DllName,'ProcessStep',stepNumber);
        end
        function DoBaseRate(this)
            % DoBaseRate Processes all rate groups as appropriate
            % Usage:
            %   first set all input buffers via SetInBufferData
            %   next execute processing via DoBaseRate
            %   finally get all output buffers vis GetOutBufferData
            theFlags = this.UpdateStepFlags();
            for i=1:numel(theFlags)
                if (theFlags(i))
                    TID = i-1;
                    this.ProcessStep(TID);
                    fprintf('Process TID %d\n',TID);
                end
            end
        end
        
        % ----- Asynchronous communication
        function responseBuffer = HandleAsdIdRequest(this, requestBuffer)
            % HandleAsdIdRequest Interfaces asynchronous request and
            % response payloads with the model. The request buffer is
            % expected to be an array of uint8 formatted to exactly match
            % the payload format expected by the Talaria blocks (RTC, TSP,
            % or TOP).
            % Note that a 4K response buffer is automatically provided
            
            % Prepare the request buffer
            reqBuf = typecast(requestBuffer,'uint8');
            reqBufSize = numel(reqBuf);
            % Prepare the response buffer
            rspBuf = libpointer('uint8Ptr',zeros(1,4096,'uint8'));
            rspBufSize = numel(rspBuf.Value);
            [retVal,~,rspBuf] = calllib(this.DllName, ...
                [this.ModelName '_HandleAsdIdRequest'], ...
                reqBuf, reqBufSize, rspBuf, rspBufSize);
            % Format the response pointer to size indicated by retVal
            rspBuf.setdatatype('uint8Ptr',1,retVal);
            if (retVal == 0)
                % No response was given
                responseBuffer = [];
            else
                responseBuffer = rspBuf.Value;
            end
        end

        function notificationBuffer = ServiceNotifications(this)
            % HandleAsdIdRequest Interfaces asynchronous request and
            % response payloads with the model. The request buffer is
            % expected to be an array of uint8 formatted to exactly match
            % the payload format expected by the Talaria blocks (RTC, TSP,
            % or TOP).
            % Note that a 4K notification buffer is automatically provided
            
            % Prepare the notification buffer
            ntfBuf = libpointer('uint8Ptr',zeros(1,4096,'uint8'));
            ntfBufSize = numel(ntfBuf.Value);
            [retVal,ntfBuf] = calllib(this.DllName, ...
                [this.ModelName '_ServiceNotifications'], ...
                ntfBuf, ntfBufSize);
            % Format the notification pointer to size indicated by retVal
            if (retVal > 0)
                % We've got a notification!
                ntfBuf.setdatatype('uint8Ptr',1,retVal);
                notificationBuffer = ntfBuf.Value;
            elseif (retVal < 0)
                % Error- the provided buffer is too small!
                error('Notification waiting, but requires %d bytes instead of only %d', ...
                    -retVal, ntfBufSize);
            else
                % retVal must be 0. This means nothing to see here
                notificationBuffer = [];
            end
        end

        function statusString = SetTOP(this, topWriteToken, charOffset, data)
            % SetTOP Sets TOP data and returns status code (0 means OK).
            % This method is simple/low-level accepting numeric token and
            % numeric data already in the expected format.
            % Usage:
            %   statusString = obj.SetTOP(topWriteToken, charOffset, data);   
            %
            % Example for a TOP of type single with write token 0x01000000:
            %    obj.SetTOP(0x01000000,0,single(12345))
            %
            % See also: top_parser.GetWriteToken, top_parser.PackTopWriteFromString

            % Build a TOP write request payload as an array of uint8 chars
            % typedef struct
            % {
              % uint32_t topId;
              % uint32_t callerHandle;
              % uint32_t offset;
              % uint32_t byteCount;
              % uint8_t data[];
            % }
            % TOP_WriteRequest_type;
            topWriteToken = uint32(topWriteToken);
            if (bitand(topWriteToken, 0x80000000))
                error('Invalid TOP write token. MSB must not be set');
            end
            
            uint8Data = typecast(data,'uint8');
            uint8TopId = typecast(topWriteToken,'uint8');
            uint8CallerHandle = typecast(uint32(0),'uint8');
            uint8Offset = typecast(uint32(charOffset),'uint8');
            % Automatically calculate the byteCount of what was provided:
            uint8ByteCount = typecast(uint32(numel(uint8Data)),'uint8');
            requestPayload = [uint8TopId uint8CallerHandle uint8Offset uint8ByteCount ...
                uint8Data];
            rspBuf = this.HandleAsdIdRequest(requestPayload);
            rspToken = dec2hex(typecast(rspBuf(1:4),'uint32'));
            rspCallerHandle = typecast(rspBuf(5:8),'uint32');
            statusCode = typecast(rspBuf(9:12),'int32');
            statusString = payload_parser.GetStatusCodeStr(statusCode);
        end
        
        function [dataBytes, statusString] = GetTOP(this, topReadToken, charOffset, charCount)
            % SetTOP Gets TOP data and returns status code (0 means OK)
            % Usage:
            %   [dataBytes,statusString] = obj.GetTOP(topWriteToken, charOffset, charCount);
            % 
            % Example for TOP of datatype 'single' with read token 0x81000000:
            %    typecast(obj.GetTOP(0x81000000,0,4),'single')
            %
            % See also: top_parser.GetReadToken, top_parser.PackTopRead

            % Build a TOP read request payload as an array of uint8 chars
            % typedef struct
            % {
              % uint32_t topId;
              % uint32_t callerHandle;
              % uint32_t offset;
              % uint32_t byteCount;
            % }
            % TOP_ReadRequest_type;
            topReadToken = uint32(topReadToken);
            if (0 == bitand(topReadToken, 0x80000000))
                error('Invalid TOP read token. Must have MSB set');
            end
            uint8TopId = typecast(topReadToken,'uint8');
            uint8CallerHandle = typecast(uint32(0),'uint8');
            uint8Offset = typecast(uint32(charOffset),'uint8');
            % Automatically calculate the byteCount of what was provided:
            uint8ByteCount = typecast(uint32(charCount),'uint8');
            requestPayload = [uint8TopId uint8CallerHandle uint8Offset uint8ByteCount];
            rspBuf = this.HandleAsdIdRequest(requestPayload);
            %rspToken = typecast(rspBuf(1:4),'uint32');
            %rspCallerHandle = typecast(rspBuf(5:8),'uint32');
            statusCode = typecast(rspBuf(9:12),'int32');
            statusString = payload_parser.GetStatusCodeStr(statusCode);
            dataBytes = rspBuf(17:end);
        end
        
        % Make sure to unload the library when object dies
        function delete(this)
            if (false == isempty(this.DllName) && libisloaded(this.DllName))
                unloadlibrary(this.DllName);
            end
        end
        
    end
end



