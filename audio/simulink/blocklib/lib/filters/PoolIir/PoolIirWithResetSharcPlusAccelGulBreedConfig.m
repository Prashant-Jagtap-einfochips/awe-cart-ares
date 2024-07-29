classdef PoolIirWithResetSharcPlusAccelGulBreedConfig < BtcBreedBaseClass
    % PoolIirWithResetSharcPlusAccelGulBreedConfig Build-time configuration class for the PooliirGulAccel breed
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties (GetObservable, SetObservable)
        RmdlShutdown    uint32  = 0
        CodeWorkMemorySize              % Size of code work memory required for breed
    end

    properties(SetAccess = private)
        PostOverlayRequired     = 1
        CacheLineLen            = 64    % ADI GUL size in bytes
        CodeWorkMemoryInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolIirWithResetSharcPlusAccelGulBreedConfig(parentObj)
            %.PoolIirWithResetSharcPlusAccelGulBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            this.Lock(@this.CodeWorkMemorySize,@()this.GetCodeWorkMemorySize);
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

            % Create alias for code work memory buffer for each instance of model
            blockDataStore = {'codeWorkMemory'};
            variantBlock = {'PoolIirWithReset/SharcPlusAccelGul'};
            variantBlockPath = MaskUtil.SubBlockName(blockHandle, variantBlock);
            variantBlockHandle = getSimulinkBlockHandle(variantBlockPath);
            PooliirAccelMask(variantBlockHandle, blockDataStore);

            this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);

            % Be friendly about the CRL this variant expects
            this.EnsureCodeReplacementLibrary(blockHandle,'SHARC+');

            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end

    methods
        function value = GetCodeWorkMemorySize(this)
            % Accelerator CRL code work memory size is equal to:
            %             sizeof(iirAccelWorkMemory_t) + variableDataSize;
            % with variable data size dependent on number of channels and
            % frame size.  NOTE: this must be updated to match the CRL code
            % if any changes are made to the accelWorkMemory_t structure.
            bufferSize = 68 + this.Parent.NumChannels * (64 + this.Parent.FrameSize * 4);
            variableDataSize = TargetCacheRoundUpSize(bufferSize, ...
                1, ...
                this.CacheLineLen); % bytes
            memSize = 64 + variableDataSize; % bytes
            value = memSize / 4; % words (single)
        end
    end
end
