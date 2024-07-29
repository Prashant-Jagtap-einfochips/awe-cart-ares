classdef FirUpHexagonBreedConfig < FirUpGenericBreedConfig
% FirUpHexagonBreedConfig Build-time configuration class for the FirUpHexagon variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
% For Documentation see FirUpGenericBreedConfig

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % PropertyName size class {validation functions} = DefaultValue

  end
  methods
    %----------------------------------------------------------------------
    function this = FirUpHexagonBreedConfig(constructingParentObj)
        this@FirUpGenericBreedConfig(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.

        BreedValidate@FirUpGenericBreedConfig(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.

        BreedDoMaskInit@FirUpGenericBreedConfig(this,blockHandle);
    end

  end
end
