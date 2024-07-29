classdef AnnSteeringConfig < BtcBaseClass
% AnnSteeringConfig Build-time configuration class for the AnnSteering block
% Usage:
%    aConfig = AnnSteeringConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize
    InitialRampingRateRtc
    InitialRampingTimeRtc
    InitialTargetGainRtc
    MinGainDb
    NumAnnIn
    NumAnnOut
    Prevgainvalues
    SnapThresh
  end
  methods
    %----------------------------------------------------------------------
    function this = AnnSteeringConfig()
        % AnnSteeringConfig constructor: assign component property objects
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
