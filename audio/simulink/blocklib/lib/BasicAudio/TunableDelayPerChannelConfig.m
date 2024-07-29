classdef TunableDelayPerChannelConfig < DelayPerChannelConfig
% TunableDelayPerChannelConfig Build-time configuration class for the TunableDelayPerChannel block
% Usage:
%    aConfig = TunableDelayPerChannelConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultDelayAmount  = 0
    DefaultDelayType DelayTypeEnum = 'InMilliseconds'
  end
  methods
    %----------------------------------------------------------------------
    function this = TunableDelayPerChannelConfig()
        % TunableDelayPerChannelConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
       Validate@DelayPerChannelConfig(this);
    end

  end
end
