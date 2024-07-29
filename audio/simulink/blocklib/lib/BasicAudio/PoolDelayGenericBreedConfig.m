classdef PoolDelayGenericBreedConfig < BtcBreedBaseClass
    % PoolDelayGenericBreedConfig Build-time configuration class for the PooliirGeneric variant
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(GetObservable, SetObservable)
        TotalFastDelayBufferSize           % Size of fast delay buffer required
    end

    properties(SetAccess = private)
        FastDelayBufferInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolDelayGenericBreedConfig(parentObj)
            %.PoolDelayGenericBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            this.Lock(@this.TotalFastDelayBufferSize,@()this.Parent.MaxTotalDelay);
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
            blockDataStore = {'PoolDelay/Generic/fastDelayBuffer'};
            % Get block handle of the data store
            bufferBlockPath = MaskUtil.SubBlockName(blockHandle, blockDataStore);
            bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);
            if -1 ~= bufferBlockHandle
                % Set the Custom Storage Class memory section
                CscUtil.SetDatastoreCsc(bufferBlockHandle,'Blocklib','Auto');
            end

            this.FastDelayBufferInitVal = zeros(1, this.TotalFastDelayBufferSize);
            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end
end
