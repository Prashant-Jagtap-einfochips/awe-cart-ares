classdef LimiterConfig < BtcBaseClass
% LimiterConfig Build-time configuration class for the Limiter block
% Usage:
%    aConfig = LimiterConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    NumChannels     {mustBeInteger, mustBePositive}     = 16
    DefaultAttackDb
    DefaultAttackLowDb
    DefaultDecayDB
    DefaultDecayLowDb
    DefaultMaxAttackDb
    DefaultMaxAttackLowDb
    DefaultThreshold  = [0.1        0.015         0.2        0.25         0.3        0.35         0.4        0.45         0.5         0.6        0.65         0.7        0.75         0.8         0.85 0.9]
    DefaultThresholdLow  = [0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5         0.5]

  end
  methods
    %----------------------------------------------------------------------
    function this = LimiterConfig()
        % LimiterConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.Default(@this.DefaultAttackDb, @() 10000 * ones(1, this.NumChannels));
        this.Default(@this.DefaultAttackLowDb, @() 10000 * ones(1, this.NumChannels));
        this.Default(@this.DefaultDecayDB, @() 100 * ones(1, this.NumChannels));
        this.Default(@this.DefaultDecayLowDb, @() 100 * ones(1, this.NumChannels));
        this.Default(@this.DefaultMaxAttackDb, @() 10 * ones(1, this.NumChannels));
        this.Default(@this.DefaultMaxAttackLowDb, @() 10 * ones(1, this.NumChannels));
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end
