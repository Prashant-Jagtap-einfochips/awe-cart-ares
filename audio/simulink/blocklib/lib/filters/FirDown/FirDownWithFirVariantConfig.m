classdef FirDownWithFirVariantConfig < BtcVariant
% FirDownWithFirVariantConfig Build-time configuration class for the FirDownWithFir variant
% Usage:
%    <automatically allocated by setting the "Variant" property of the
%    containing object derived from BtcBaseClassWithVariants
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % PropertyName size class {validation functions} = DefaultValue
    Fir FirConfig
    NumChannels {mustBeInteger} = 2
    FrameSizeIn {mustBeInteger} = 32
  end
  methods
    %----------------------------------------------------------------------
    function this = FirDownWithFirVariantConfig(constructingParentObj)
        this@BtcVariant(constructingParentObj);
        this.Fir = FirConfig();
        this.Default(@this.Fir.Alias, @() ([char(constructingParentObj.Alias), 'Fir']));
        this.Lock(@this.Fir.NumOutputs, @() constructingParentObj.DownFactor*this.NumChannels);
        this.Lock(@this.Fir.NumInputs, @() constructingParentObj.DownFactor*this.NumChannels);
        this.Lock(@this.Fir.FrameSize, @() this.FrameSizeIn/constructingParentObj.DownFactor);
        this.Lock(@this.Fir.MaxAvgCoeffLength, @() this.GetMaxAvgCoeffLength());
    end
    %----------------------------------------------------------------------
    function VariantValidate(this)
        % VariantValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
        VariantValidate@BtcVariant(this);
    end
    %----------------------------------------------------------------------
    function VariantDoMaskInit(this,blockHandle)
        % VariantDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        VariantDoMaskInit@BtcVariant(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = GetMaxAvgCoeffLength(this)
        val = ceil(this.Parent.NumCoeffs/this.Parent.DownFactor);
    end
    %----------------------------------------------------------------------
  end
end
