classdef FirDownBtcConfig < FirDownConfig
% Block Documentation for FirDownBtc-----------------------------------------
% Downsampling followed by an Fir filter. The block is implemented using a 
% polyphase Fir. Fir filter length must be a multiple of the upsampling 
% factor. This block is a wrapper around the FirDown (DSP only) block. The
% wrapper handes translations of the coefficients to each specific hardware
% target depending on the selected variant. The translations are handled in
% the BTC. 
%
% Inputs:
%   AudioIn: [FrameSize x NumChannels] Input signal 
% Outputs:
%   AudioOut: [FrameSize/DownFactor x NumChannels] Downsampled and filtered
%       output signal
%
% Properties:
%   CoeffsRaw: [NumCoeffs x 1] Coefficients in standard format [h[0], h[1] ...]
%   DownFactor: Down-Sampling Factor
%
%--------------------------------------------------------------------------

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
        CoeffsRaw                   (:,1) single
  end
  properties (GetObservable,SetObservable,Dependent)
        CoeffsTarget                (:,1) single
  end
  methods
    %----------------------------------------------------------------------
    function this = FirDownBtcConfig(varargin)
        % FirDownBtcConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        if(nargin == 0)
            this.Default(@this.CoeffsRaw, @() designMultirateFIR(1,this.DownFactor,0.1));
        else
            FirDownCoeff = varargin{1};
            this.Default(@this.CoeffsRaw, @() FirDownCoeff);
        end
        this.Lock(@this.NumCoeffs, @() numel(this.CoeffsTarget));

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@FirDownConfig(this);

    end
    %----------------------------------------------------------------------
    function val = get.CoeffsTarget(this)
        val = FirDownTranslation(this.CoeffsRaw, this.DownFactor, this.Breed);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        % MaskInit Depending on which blockType uses this Config
        blockType = get_param(blockHandle, 'MaskType');
        if (blockType == "Blocklib FirDown")
            DoMaskInit@FirDownConfig(this,blockHandle);
        elseif (blockType == "Blocklib FirDownBtc")
            DoMaskInit@BtcBaseClassWithBreeds(this,blockHandle);
        else
            error("Unsupported Block %s" , blockType);
        end
    end
    %----------------------------------------------------------------------
  end
end
