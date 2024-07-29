classdef PoolIirWithResetGenericBreedConfig < BtcBreedBaseClass
    % PoolIirWithResetGenericBreedConfig Build-time configuration class for the PooliirGeneric variant
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(GetObservable, SetObservable)
        CodeWorkMemorySize                  % State memory used for generic implementation
    end

    properties(SetAccess = private)
        PostOverlayRequired     = 1
        CodeWorkMemoryInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolIirWithResetGenericBreedConfig(parentObj)
            %.PoolIirWithResetGenericBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            this.Lock(@this.CodeWorkMemorySize,@()2*this.Parent.MaxNumStages);
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
            this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);
            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end
end
