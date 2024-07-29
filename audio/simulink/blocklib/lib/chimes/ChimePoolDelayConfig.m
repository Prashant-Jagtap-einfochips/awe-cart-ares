classdef ChimePoolDelayConfig < BtcBaseClass
% ChimePoolDelayConfig Build-time configuration class for the ChimePoolDelay block
% Usage:
%    aConfig = ChimePoolDelayConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    DefaultDelayChain % = chimeConfig.pooldelay.delay_chain
	MaxDelay = 4000 % = chimeConfig.pooldelay.max_delay
    DefaultMaxDelay = 4000 % = chimeConfig.pooldelay.defaultmax_delay
    InitialEnableDelay  = 1
    NumChannels = 1
    NOutputDelay = 0
  end
  methods
    %----------------------------------------------------------------------
    function this = ChimePoolDelayConfig()
        % ChimePoolDelayConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
        
        if (isempty(this.DefaultDelayChain))
            this.DefaultDelayChain = 4.*ones(1,this.NumChannels) + this.NOutputDelay;   
        end
        
        if (numel(this.DefaultDelayChain) ~= this.NumChannels)
            error('DelayChain dimensions [%d] must match channels [%d]',...
                numel(this.DefaultDelayChain),this.NumChannels);
        end
    end

  end
end
