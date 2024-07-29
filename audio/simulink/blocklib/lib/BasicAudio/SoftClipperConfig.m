classdef SoftClipperConfig < BtcBaseClass
% SoftClipperConfig Build-time configuration class for the SoftClipper block
% Usage:
%    aConfig = SoftClipperConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultXmin  = 0.65
    DefaultXminLow  = 0.65
    DefaultYmax  = 1.0
    DefaultYmaxLow  = 1.0
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

  end
  methods
    %----------------------------------------------------------------------
    function this = SoftClipperConfig()
        % SoftClipperConfig constructor: assign component property objects
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
