classdef MuteRamperConfig < BtcBaseClass
% MuteRamperConfig Build-time configuration class for the MuteRamper block
% Usage:
%    aConfig = MuteRamperConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    InitialMuteState  = 1
    SamplesPerFrame  = 32
  end
  properties (Constant)
      SilentGainLinear = 10^(-126.5/20)
      SnapThreshold = single(10^(-9))
  end
  properties (Dependent)
      DlgInitialGainLinear
  end
  methods
    %----------------------------------------------------------------------
    function this = MuteRamperConfig()
        % MuteRamperConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end
    
    %----------------------------------------------------------------------
    function value = get.DlgInitialGainLinear(this)
        if (this.InitialMuteState)
            value = this.SilentGainLinear;
        else
            value = 1;
        end
    end
  end
end
