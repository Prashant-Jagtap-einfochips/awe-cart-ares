classdef EnergyLimiterConfig < BtcBaseClass
% EnergyLimiterConfig Build-time configuration class for the EnergyLimiter block
% Usage:
%    aConfig = EnergyLimiterConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    EnableGainLimiter                      = 1.0 
    EnergyAdjustmentFactorInDb             = 0 
    EnergyMeasurementTimeConstantInSeconds = 0.4
    GainChangeStepInDb                     = 0.5
    GainDecreaseDurationInSeconds          = 0.5  
    GainIncreaseDurationInSeconds          = 0.5
    MaxChannels                            = 8
    MaxEnergyLevelInDb                     = -3
    MaxGainReductionInDb                   = 6
    
    % Missing - previously pulled from base workspace. legacy wrapper still
    % explicitly evalutes these in base workspace to pass in
    FrameSize
    Fs
    CurrentGainVal = 1.0
  end
  methods
    %----------------------------------------------------------------------
    function this = EnergyLimiterConfig()
        % EnergyLimiterConfig constructor: assign component property objects
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
