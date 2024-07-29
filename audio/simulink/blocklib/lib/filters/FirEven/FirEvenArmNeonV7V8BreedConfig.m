classdef FirEvenArmNeonV7V8BreedConfig < FirEvenHexagonBreedConfig
% FirEvenArmNeonV7V8BreedConfig Build-time configuration class for the FirUpHexagon variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
% For Documentation see FirEvenHexagonBreedConfig

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % PropertyName size class {validation functions} = DefaultValue

  end
  methods
    %----------------------------------------------------------------------
    function this = FirEvenArmNeonV7V8BreedConfig(constructingParentObj)
        this@FirEvenHexagonBreedConfig(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.

        BreedValidate@FirEvenHexagonBreedConfig(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.

        this.ConfigureDataStoreMemory("CodeWorkMemory", blockHandle);
        this.ConfigureDataStoreMemory("States", blockHandle);
        this.ConfigureSignalObject([char(this.Parent.Alias) 'FirEvenCoeffs'], blockHandle,'Initialization/SignalBreak');
        this.ConfigureSignalObject([char(this.Parent.Alias) 'FirEvenInput'], blockHandle,'AudioIn');


        % Be friendly about the CRL this variant expects
        this.EnsureCodeReplacementLibrary(blockHandle,'ARM');
        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end

  end
end