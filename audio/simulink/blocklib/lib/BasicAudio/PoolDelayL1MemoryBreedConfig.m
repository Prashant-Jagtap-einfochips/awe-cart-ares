classdef PoolDelayL1MemoryBreedConfig < BtcBreedBaseClass
    % PoolDelayL1MemoryBreedConfig Build-time configuration class for the PooliirGeneric variant
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(GetObservable, SetObservable)
        CodeWorkMemorySize                  % Code work memory size required for breed
        TotalFastDelayBufferSize            % Size of fast delay buffer required
    end

    properties(SetAccess = private)
        PostOverlayRequired     = 1
        CodeWorkMemoryInitVal
        FastDelayBufferInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolDelayL1MemoryBreedConfig(parentObj)
            %.PoolDelayL1MemoryBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            % Fast delay buffer (L1) is used for core implementation.
            this.Lock(@this.TotalFastDelayBufferSize,@()this.Parent.MaxTotalDelay);
            % CRL code work memory size in bytes is equal to:
            %             sizeof(coreWorkMemory_t);
            % No variable data memory is used.  NOTE: This must be updated
            % to match the CRL code if any changes are made to the 
            % coreWorkMemory_t structure,36 bytes.
            % Work Memory Size = 36/4 = 9 words.
            this.Default(@this.CodeWorkMemorySize, @()36/4);
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
            BreedBlock = {'PoolDelay/L1Memory'};
            blockDataStores = {'codeWorkMemory', 'fastDelayBuffer'};
            % memory sections
            blockMemorySections = {'MemL1', 'MemPm'};
            for i = 1:1:length(blockDataStores)
                % Get the data store buffer name
                dataStoreName = blockDataStores{i};
                dataStoreName = [BreedBlock{1} '/' dataStoreName];
                % Get block handle of the data store
                bufferBlockPath = MaskUtil.SubBlockName(gcbh, dataStoreName);
                bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

                % Create unique alias for data store name from buffer block
                % path, removing spaces and replacing all non-alphanumeric
                % characters with underscores
                bufferAlias = replace(bufferBlockPath, ' ', '');
                bufferAlias = regexprep(bufferAlias, '\W', '_');
                bufferAlias = erase(bufferAlias, '_PoolDelay_L1Memory');
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
                    % L1 Memory implementations:  Set to designated section 
                    theCoderInfo.CustomAttributes.MemorySection = blockMemorySections{i};
                end
            end
            this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);
            this.FastDelayBufferInitVal = zeros(1, this.TotalFastDelayBufferSize);
            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end
end
