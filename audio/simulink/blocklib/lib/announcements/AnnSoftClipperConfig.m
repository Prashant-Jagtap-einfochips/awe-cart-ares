classdef AnnSoftClipperConfig < BtcBaseClass
% AnnSoftClipperConfig Build-time configuration class for the AnnSoftClipper block
% Usage:
%    aConfig = AnnSoftClipperConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    MaskXMin  = 0.65
    MaskYMax  = 1
  end
  methods
    %----------------------------------------------------------------------
    function this = AnnSoftClipperConfig()
        % AnnSoftClipperConfig constructor: assign component property objects
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
