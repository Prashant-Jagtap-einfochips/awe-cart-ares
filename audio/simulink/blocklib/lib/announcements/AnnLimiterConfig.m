classdef AnnLimiterConfig < BtcBaseClass
% AnnLimConfig Build-time configuration class for the AnnLim block
% Usage:
%    aConfig = AnnLimConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultAttackDb  % = 10e3 * ones(1,sys_defs.number_of_output_audio_channels)
    DefaultDecayDb  % = 100 * ones(1,sys_defs.number_of_output_audio_channels)
    DefaultMaxAttack  % = 10 * ones(1,sys_defs.number_of_output_audio_channels)
    DefaultThreshold  % = 0.9 * ones(1,sys_defs.number_of_output_audio_channels)
  end
  methods
    %----------------------------------------------------------------------
    function this = AnnLimiterConfig()
        % AnnLimConfig constructor: assign component property objects
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
