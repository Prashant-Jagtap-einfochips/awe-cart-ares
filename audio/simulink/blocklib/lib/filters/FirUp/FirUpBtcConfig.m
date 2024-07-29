classdef FirUpBtcConfig < FirUpConfig
% Block Documentation for FirUpBtc-----------------------------------------
% Upsampling followed by an Fir filter. The block is implemented using a 
% polyphase Fir. Fir filter length must be a multiple of the upsampling 
% factor. This block is a wrapper around the FirUp (DSP only) block. The
% wrapper handes translations of the coefficients to each specific hardware
% target depending on the selected variant. The translations are handled in
% the BTC.
%
% Inputs:
%   AudioIn: [FrameSize x NumChannels] Input signal 
% Outputs:
%   AudioOut: [FrameSize*UpFactor x NumChannels] Upsampled and filtered
%       output signal
%
% Properties:
%   CoeffsRaw: [NumCoeffs x 1] Coefficients in standard format [h[0], h[1] ...]
%   UpFactor: Up-Sampling Factor
%
%--------------------------------------------------------------------------

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
      CoeffsRaw (:,1) single
  end
  properties (GetObservable,SetObservable,Dependent)
      CoeffsTarget (:,1) single
  end
  methods
    %----------------------------------------------------------------------
    function this = FirUpBtcConfig(varargin)
        % FirUpBtcConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        if(nargin == 0)
            this.Default(@this.CoeffsRaw, @() designMultirateFIR(this.UpFactor,1,0.1));
        else
            FirUpCoeff = varargin{1};
            this.Default(@this.CoeffsRaw, @() FirUpCoeff);
        end
        this.Lock(@this.NumCoeffs, @() numel(this.CoeffsTarget));

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@FirUpConfig(this);

    end
    %----------------------------------------------------------------------
    function val = get.CoeffsTarget(this)
        val = FirUpTranslation(this.CoeffsRaw, this.UpFactor, this.Breed, this.FrameSizeIn);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        % MaskInit Depending on which blockType uses this Config
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib FirUp")
            DoMaskInit@FirUpConfig(this,blockHandle);
        elseif (blockType == "Blocklib FirUpBtc")
            DoMaskInit@BtcBaseClass(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end
    end
    %----------------------------------------------------------------------
  end
end
