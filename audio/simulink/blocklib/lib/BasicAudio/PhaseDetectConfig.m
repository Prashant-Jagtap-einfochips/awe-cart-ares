classdef PhaseDetectConfig < BtcBaseClass
% PhaseDetectConfig Build-time configuration class for the PhaseDetector block
% Usage:
%    aConfig = PhaseDetectConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    NumMicIn = 2 % = PhaseDetectConfig.numMicIn
    FrameSize = 32 % previously found in base workspace
    
    % Initial values for DSMs, destroyed by receipt of RTC message.
    % InitialStartCount represents a control message (RTC) correlation
    % duration in frames at start/reset.
    InitialStartCount = 1000
    % Initial values for DSMs, destroyed by receipt of RTC message.
    % InitialFrameCount represents start count plus control message (RTC)
    % delay in frames, asserted at start/reset
    InitialFrameCount = 1050
  end
  methods
    %----------------------------------------------------------------------
    function this = PhaseDetectConfig()
        % PhaseDetectConfig constructor: assign component property objects
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
