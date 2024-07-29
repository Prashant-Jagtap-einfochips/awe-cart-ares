classdef ScalarChimeControlConfig < ChimeControlConfig
% ScalarChimeControlConfig Build-time configuration class for the ScalarChimeControl block
% Usage:
%    aConfig = ScalarChimeControlConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    Gain  % = gain
    SysInChannelCount  % = sysin_channel_count

% The following properties were present in the legacy mask, but appeared to
% have no references... i.e. unused
%     NumExternalChime  % = num_external_chime
%     SpkrRouting  % = spkrRouting
  end
  properties (Dependent)
      Rgainz RgainzConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = ScalarChimeControlConfig()
        % ScalarChimeControlConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@ChimeControlConfig(this);
    end

    %----------------------------------------------------------------------
    function value = get.Rgainz(this)
        value = RgainzConfig;
        value.NumRampers = this.SysInChannelCount;
        value.InitialLinearGains = this.Gain;
        value.NumChannels = this.SysInChannelCount;
        value.SampleRateInHertz = this.FsHz;
        value.FrameSize = this.FrameSizeSamples;
    end
  end
end
