classdef DelayConfig < BtcBaseClass
% DelayConfig Build-time configuration class for the Delay block
% Usage:
%    aConfig = DelayConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize  = 32
    MaxDelayTime  = 512
    NumChannels  = 2
    % BufferAlias when non-blank updates code generation settings to use
    % the provided name for the buffer memory variable in C language
    BufferAlias  = []
    % HeaderFilename with non-blank directs the code generation to place
    % the the buffer memory variable in the provided filename. The
    % BufferAlias must also be specified to avoid a configuration error
    HeaderFilename  = []
  end
  properties (Dependent)
      DlgInitialState
  end
  methods
    %----------------------------------------------------------------------
    function this = DelayConfig()
        % DelayConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

    function value = get.DlgInitialState(this)
        value =  zeros((this.MaxDelayTime+this.FrameSize)*this.NumChannels,1);
    end
  end
end
