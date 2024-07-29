classdef InLineRmsConfig < BtcBaseClass
% InLineRmsConfig Build-time configuration class for the InLineRms block
% Usage:
%    aConfig = InLineRmsConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize  % = framesize
    InitialEnable  % Initial state of Enable RTC signal
    NumIn  % = numIn
    
    % Set boundary conditions for time window tuning
    % Time window duration is specified in milli seconds, and is in the range
    % of 1 milli second to 1000 milli seconds. 
    TimeWindowBounds = [1,1000]
    
    % TuneVar Defaults
    DefaultTimeWindowMs = 100
  end
  
  properties (Dependent=true)
      DefaultChannelEnable
  end
  
  methods
    %----------------------------------------------------------------------
    function this = InLineRmsConfig()
        % InLineRmsConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    
    function value = get.DefaultChannelEnable(this)
        value = ones(1,this.NumIn);
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end
