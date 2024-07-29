classdef RefChMixerConfig < BtcBaseClass
% RefChMixerConfig Build-time configuration class for the RefChMixer block
% Usage:
%    aConfig = RefChMixerConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultMixingGainsFader  % = mixing_gains_fader
    DefaultMixingGainsNormal  % = mixing_gains_normal
    DefaultTuneFadePos  % = tune_fade_pos
    IncludeFade  = 1
  end
  methods
    %----------------------------------------------------------------------
    function this = RefChMixerConfig()
        % RefChMixerConfig constructor: assign component property objects
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
