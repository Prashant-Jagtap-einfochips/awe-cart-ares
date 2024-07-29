classdef NonEntVncAppliedConfig < BtcBaseClass
% NonEntVncAppliedConfig Build-time configuration class for the NonEntVncApplied block
% Usage:
%    aConfig = NonEntVncAppliedConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize = 32 % = L
    
    % TuneVar defaults
    DefaultTau = 0.05
  end
  methods
    %----------------------------------------------------------------------
    function this = NonEntVncAppliedConfig()
        % NonEntVncAppliedConfig constructor: assign component property objects
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
