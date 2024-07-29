classdef MuteControlConfig < MuteRamperConfig
% MuteControlConfig Build-time configuration class for the MuteControl block
% Inherits from MuteRamperConfig to maintain single inheritance up through
% full mute block (which has both control and ramper).
% Usage:
%    aConfig = MuteControlConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
      DefaultRampTimeMsec = 100
  end
  methods
    %----------------------------------------------------------------------
    function this = MuteControlConfig()
        % MuteControlConfig constructor: assign component property objects
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
