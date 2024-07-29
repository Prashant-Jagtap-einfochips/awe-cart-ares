classdef TunableDelayConfig < DelayConfig
% TunableDelayConfig Build-time configuration class for the TunableDelay block
% Usage:
%    aConfig = TunableDelayConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultDelayType DelayTypeEnum = 'InMilliseconds'
    DefaultDelayAmount = 0
  end
  methods
    %----------------------------------------------------------------------
    function this = TunableDelayConfig()
        % TunableDelayConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@DelayConfig(this);
    end

  end
end
