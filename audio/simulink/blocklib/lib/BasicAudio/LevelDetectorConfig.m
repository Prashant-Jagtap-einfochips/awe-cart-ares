classdef LevelDetectorConfig < BtcBaseClass
% LevelDetectorConfig Build-time configuration class for the LevelDetector block
% Usage:
%    aConfig = LevelDetectorConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    AnnTriggerThreshold  = single(0.5)
    SuperFastDecayMultiplier  = single(3.0)
  end
  methods
    %----------------------------------------------------------------------
    function this = LevelDetectorConfig()
        % LevelDetectorConfig constructor: assign component property objects
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
