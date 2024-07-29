classdef MicDiagConfig < BtcBaseClass
% MicDiagConfig Build-time configuration class for the MicDiag block
% Usage:
%    aConfig = MicDiagConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize  % = framesize
    NumMicIn  % = numMicIn
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
  end
  methods
    %----------------------------------------------------------------------
    function this = MicDiagConfig()
        % MicDiagConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
    end
  end
end
