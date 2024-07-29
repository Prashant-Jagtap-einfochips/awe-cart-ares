classdef ExternalChimeProcessConfig < BtcBaseClass
% ExternalChimeProcessConfig Build-time configuration class for the ExternalChimeProcess block
% Usage:
%    aConfig = ExternalChimeProcessConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    ExternalChimeCancel  = 0
    FrameSize  = 32
    NumExtChimes  = 1
    
    % Added properties that were previously stolen (not in mask)
    TargetGain = 224 %chimeConfig.ExternalChime.TargetGainVal: Whoa, seems high
    IncludeEq = 0 % include_eq_for_external_chime
    
    % Configurable components
    PoolDelay ChimePoolDelayConfig
    Router RouterConfig
    %chimeConfig.PoolIIR.maxNumStages_OutEq,chimeConfig.PoolIIR.numStages_OutEq,chimeConfig.PoolIIR.coeffs_OutEq
    PoolIir PoolIirTunableConfig
  end
  properties (Dependent)
    Volume AnnVolumeConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = ExternalChimeProcessConfig()
        % ExternalChimeProcessConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.PoolDelay = ChimePoolDelayConfig;
        this.Router = RouterConfig;
        this.PoolIir = PoolIirTunableConfig;
        this.LockPropertyAsAlias('PoolDelay.NumChannels','NumExtChimes');
        this.LockPropertyAsAlias('Router.NumIn','NumExtChimes');
        this.LockPropertyAsAlias('Router.NumOut','NumExtChimes');
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

    %----------------------------------------------------------------------
    function value = get.Volume(this)
        value = AnnVolumeConfig;
        value.InitialLinearGain = this.TargetGain;
        value.NumInputChannels = this.NumExtChimes;
    end
    %----------------------------------------------------------------------
  end
end
