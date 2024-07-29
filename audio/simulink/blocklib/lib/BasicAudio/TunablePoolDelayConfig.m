classdef TunablePoolDelayConfig < BtcBaseClass
% TunablePoolDelayConfig Build-time configuration class for the TunablePoolDelay block
% Usage:
%    aConfig = TunablePoolDelayConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    %.Properties use this syntax:
    %.   PropertyName size class {validation functions} = DefaultValue
    %.About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
    delayType = 1 % = 1 delay in samples; = 0 delay in ms
    enableDelay = 1 % = 1 enable delay; = 0 bypass;
    FrameSize = 32
    NumChannels = 1
    delayChains 
    MaxTotalDelay = 512
    PostOverlayRequired = 1
  end

  properties (SetAccess=protected)
    poolDelay PoolDelayConfig
  end

  methods
    %----------------------------------------------------------------------
    function this = TunablePoolDelayConfig()
        % TunablePoolDelayConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.poolDelay = PoolDelayConfig;
        this.Lock(@this.poolDelay.FrameSize, @()this.FrameSize);
        this.Lock(@this.poolDelay.NumChannels, @()this.NumChannels);
        this.Lock(@this.poolDelay.MaxTotalDelay, @()this.MaxTotalDelay);
        this.delayChains = zeros(1,this.NumChannels);
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok

        %.If there are no cross-property interactions, then this entire
        %.method can be deleted!

        %.End with a call to our base class to continue validation checks
        %.including any violations of property locking.
        Validate@BtcBaseClass(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        %.If there are no CscUtil code generation settings or other canvas
        %.interactions, this entire method can be deleted!
        %.Avoid making design-time changes - find another way if possible.
        %.(e.g. avoid calling: set_param, add_block, replace_block).
        %.If there's an error, just call the error function!

        %.Call base class in case it has any work to perform:
        DoMaskInit@BtcBaseClass(this,blockHandle);
    end

  end
end
