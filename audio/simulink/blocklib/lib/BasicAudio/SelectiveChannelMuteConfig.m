classdef SelectiveChannelMuteConfig < BtcBaseClass
% SelectiveChannelMuteConfig Build-time configuration class for the SelectiveChannelMute block
% Usage:
%    aConfig = SelectiveChannelMuteConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    InitialMuteState  = 0
    NumChannelsIn  % = sysin_channel_count

    DefaultRampTimeMsec = 100
    DefaultMuteBits = 0
    DefaultRampRateDbPerSec = 0
  end
  properties (Dependent)
      Rgainy RgainyMultiTriggerConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = SelectiveChannelMuteConfig()
        % SelectiveChannelMuteConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

    %----------------------------------------------------------------------
    function value = get.Rgainy(this)
        % Setup each channel to have its own ramper
        value = RgainyMultiTriggerConfig;
        value.NumRampers = this.NumChannelsIn;
        value.Chan2RamperMap = 1:this.NumChannelsIn;
        value.InitialLinearGains = (false == this.InitialMuteState);
    end
  end
end
