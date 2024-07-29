classdef NonEntGainCalcConfig < BtcBaseClass
% NonEntGainCalcConfig Build-time configuration class for the NonEntGainCalc block
% Usage:
%    aConfig = NonEntGainCalcConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    Fd  % = Fs/L
    FrameSize  % = L
    RampCoef  = 0
    
    DefaultBoostOverride  % = boostOverRide
    DefaultBoostOverrideFlag  % = boostOverRideFlag
    DefaultMaxBoost  % = maxBoost
    DefaultMaxSpl  % = maxSPL
    DefaultMinVncSignal  % = minVNCSignal
    DefaultNoiseOverride  % = NoiseOverRide
    DefaultNoiseOverrideFlag  % = NoiseOverRideFlag
    DefaultTau single = 0.05
    DefaultVncSlope  % = VNCSlope
    DefaultVncThresh  % = VNCThresh
    DefaultVncEnable = 1
  end
  methods
    %----------------------------------------------------------------------
    function this = NonEntGainCalcConfig()
        % NonEntGainCalcConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end
