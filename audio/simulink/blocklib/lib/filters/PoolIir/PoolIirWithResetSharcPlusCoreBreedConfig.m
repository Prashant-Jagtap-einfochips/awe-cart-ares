classdef PoolIirWithResetSharcPlusCoreBreedConfig < BtcBreedBaseClass
    % PoolIirWithResetSharcPlusCoreBreedConfig Build-time configuration class for the PooliirSharcPlusCore breed
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(SetObservable, GetObservable)
        CodeWorkMemorySize              % State memory used for core implementation
    end

    properties(SetAccess = private)
        PostOverlayRequired     = 0
        CodeWorkMemoryInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolIirWithResetSharcPlusCoreBreedConfig(parentObj)
            %.PoolIirWithResetSharcPlusCoreBreedConfig constructor
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

            % Be friendly about the CRL this breed expects
            this.EnsureCodeReplacementLibrary(blockHandle,'SHARC+');

            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end
end
