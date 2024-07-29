classdef PoolDelayL2MemoryBreedConfig < BtcBreedBaseClass
    % PoolIirWithResetGenericBreedConfig Build-time configuration class for the PooliirGeneric variant
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(GetObservable, SetObservable)
          CodeWorkMemorySize               % Code work memory size required for breed
          TotalDelayBufferSize             % Size of delay buffer required
          TotalFastDelayBufferSize         % Size of fast delay buffer required
    end

    properties (SetAccess=private) 
        DelayFrame  = 1
        DelayFrameLess  = 1
        DelayMultiFrame  = 1
        CodeWorkMemoryInitVal 
        DelayBufferInitVal    
        FastDelayBufferInitVal 
    end


    methods
        %----------------------------------------------------------------------
        function this = PoolDelayL2MemoryBreedConfig(parentObj)
            %.PoolDelayL2MemoryBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            this.GetDmaMemorySize();
        end
        %----------------------------------------------------------------------
        function BreedValidate(this)
            % BreedValidate validates cross-property interactions. Our parent
            % object calls us during block mask initialization.
            BreedValidate@BtcBreedBaseClass(this);
        end
        %----------------------------------------------------------------------
        function BreedDoMaskInit(this,blockHandle)
            % BreedDoMaskInit performs automation of code generation settings
            % and other canvas interactions for a variant subsystem. Our parent
            % object calls us during block mask initialization.
            % Data stores in the PoolDelay block and their Custom Storage Class 
            BreedBlock = {'PoolDelay/L2Memory'};
            blockDataStores = {'codeWorkMemory', 'delayBuffer', 'fastDelayBuffer'};
            % memory sections
            blockMemorySections = {'MemL1', 'MemL2Uncached', 'MemPm'};
            for i = 1:1:length(blockDataStores)
                % Get the data store buffer name
                dataStoreName = blockDataStores{i};
                dataStoreName = [BreedBlock{1} '/' dataStoreName];
                % Get block handle of the data store
                bufferBlockPath = MaskUtil.SubBlockName(blockHandle, dataStoreName);
                bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

                % Create unique alias for data store name from buffer block
                % path, removing spaces and replacing all non-alphanumeric
                % characters with underscores
                bufferAlias = replace(bufferBlockPath, ' ', '');
                bufferAlias = regexprep(bufferAlias, '\W', '_');
                bufferAlias = erase(bufferAlias, '_PoolDelay_L2Memory');
                if -1 ~= bufferBlockHandle

                    % Set the header name for this top level model
                    fullHeaderFilename = ...
                        [getfullname(bdroot(bufferBlockHandle)) '_PoolDelayStorage.h'];
                    % Set definition file name for this top level model
                    fullDefinitionFilename = ...
                        [getfullname(bdroot(bufferBlockHandle)) '_PoolDelayStorage.c'];
                    CscUtil.SetDatastoreCsc(bufferBlockHandle,'Blocklib','PoolDelayStorage', ...
                        'Identifier', bufferAlias, ...
                        'HeaderFile', fullHeaderFilename, ...
                        'DefinitionFile', fullDefinitionFilename);

                    % Get Coder Info of the data store signal object used in the 
                    % Pool Delay block
                    theStateSignalObject = get(bufferBlockHandle, 'StateSignalObject');
                    theCoderInfo = get(theStateSignalObject, 'CoderInfo');
                    %L1 and L2 Memory implementations:  Set to designated section 
                    theCoderInfo.CustomAttributes.MemorySection = blockMemorySections{i};
                end

            end
            this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);
            this.FastDelayBufferInitVal = zeros(1, this.TotalFastDelayBufferSize);
            this.DelayBufferInitVal = zeros(1, this.TotalDelayBufferSize);
            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end

    methods
        function GetDmaMemorySize(this)
            % L2 Memory DMA Implementation

            % Set delay buffer sizes. Total delay memory is split between
            % the delay buffer and fast delay buffer, with only 1 frame of
            % samples, or the delay (delay < this.FrameSize) for each  
            % channel stored in the fast delay buffer. Get the delay buffer
            % sizes and DMA TCB memory required for the worst case Room 
            % Mode Down Load given the specified delay categories and 
            % this.MaxTotalDelay.
            delayConfig = [this.DelayFrameLess, this.DelayFrame, ...
                           this.DelayMultiFrame];
            [this.TotalFastDelayBufferSize, this.TotalDelayBufferSize, ...
             maxTcbMemSize, maxChanMapSize] = ...
                    PoolDelayDmaMemorySizes(delayConfig, ...
                                            this.Parent.MaxTotalDelay, ...
                                            this.Parent.NumChannels, ...
                                            this.Parent.FrameSize);

            % Make sure circular delay buffer size is at least as large as
            % the frame size. Note this is required to ensure the SHARC 
            % built-in circindex() function works correctly in the CRL 
            % code.
            this.TotalDelayBufferSize = max(this.Parent.FrameSize, ...
                                            this.TotalDelayBufferSize);

            % CRL code work memory size in bytes is equal to:
            %             sizeof(dmaWorkMemory_t) + variableDataSize;
            % with variable data size dependent on number channels and
            % number DMA TCB blocks.  NOTE: This must be updated to match
            % the CRL code if any changes are made to the dmaWorkMemory_t 
            % structure.
            this.CodeWorkMemorySize = (156/4) + maxTcbMemSize + ...
                                      maxChanMapSize; % Size in words
        end
    end
end
