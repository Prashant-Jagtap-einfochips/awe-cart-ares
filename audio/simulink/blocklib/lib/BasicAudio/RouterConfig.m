classdef RouterConfig < BtcBaseClass
% RouterConfig Build-time configuration class for the Router block
% Usage:
%    aConfig = RouterConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    % Enter a value of 0 to "zero out" the specific channel, non-specified
    % output channels will default to zero
    DefaultOutputMap  % = outputMap. 
    NumIn = 1 % = nChanIn
    NumOut = 1 % = nChanOut
  end
  methods
    %----------------------------------------------------------------------
    function this = RouterConfig()
        % RouterConfig constructor: assign component property objects
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
