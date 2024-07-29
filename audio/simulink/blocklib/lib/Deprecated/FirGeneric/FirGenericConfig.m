classdef FirGenericConfig < BtcBaseClassWithVariants
% FirGenericConfig Build-time configuration class for the FirGeneric block
% Usage:
%    aConfig = FirGenericConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
      NumOutputs = 3
      NumInputs = 3
      FrameSize = 32
      NumTaps = 8       % NumTaps needs to be a multiple of 4 for hexagon Variant
      MaxAvgCoeffLength = 8
      Alias = 'Default'
  end
  properties(Dependent, SetAccess=private)
    StateBufferSizeSimulation
  end

  methods
    %----------------------------------------------------------------------
    function this = FirGenericConfig()
        % FirGenericConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

        % Specify the variant enumeration

        this@BtcBaseClassWithVariants('FirGenericVariantEnum');
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSizeSimulation(this)
        val = (this.NumTaps + this.FrameSize)*this.NumOutputs;
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@BtcBaseClassWithVariants(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        DoMaskInit@BtcBaseClass(this, blockHandle);
        warning([getfullname(blockHandle) ' uses a the deprecated [Blocklib FirGeneric] block. Use this updated block instead [Blocklib Fir].']);

    end
  end
end
