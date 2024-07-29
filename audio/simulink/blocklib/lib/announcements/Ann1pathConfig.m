classdef Ann1pathConfig < BtcBaseClass
% Ann1pathConfig Build-time configuration class for the Ann1path block
% Usage:
%    aConfig = Ann1pathConfig
%

%   Copyright 2023 Bose Corporation

properties (GetObservable,SetObservable)
    %.Properties use this syntax:
    %.   PropertyName size class {validation functions} = DefaultValue
    %.About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    AnnInEQ PoolIirTunableConfig
    AnnVolume AnnVolumeConfig
    AnnGainCalc AnnGainCalcConfig
    AnnVnc AnnVncConfig
    AnnSteering AnnSteeringConfig
    AnnRouter RouterConfig
    AnnDelay TunableDelayPerChannelConfig
    AnnOutEQ PoolIirTunableConfig
    AnnMute SelectiveChannelMuteConfig
    AnnOutputRouter  AsdRouterConfig
    AnnLimiter  AnnLimiterConfig
    AnnSoftClipper AnnSoftClipperConfig
    FrameSize =32;
    Fd = 1500;
    aux_select=[1 2];
	
    NumInChans = 1; % Number of Ann input channels
    NumOutChans = 2; % Number of Ann output channels
	
	% Ann Input path Configuration
	AnnInputSelect = 1;%1:NumInChans(based on configurable number of Inputs
	
	% In EQ
    maxNumInEqStages = 8;
	
	% Out EQ
    maxNumOutEqStages = 26;
	
	% Volume
    InitialLinearGain = 1; % = ones(1,num_inp_chs)
	
	% Steering
	NumSteerChans = 6;
	MinGainDb = -100;
    InitialRampingRateRtc = 300;
    InitialRampingTimeRtc = 100;
    InitialTargetGainRtc = 1;
    Prevgainvalues = 1;
    SnapThresh = 1.0e-04;

    % Delay
    DefaultDelayAmount = 0;
    DefaultDelayType DelayTypeEnum = 'InSamples'

    % Mute
	NumChannelsIn =2 % = sysin_channel_count
    InitialMuteState  = 0
    DefaultRampTimeMsec = 100
    DefaultMuteBits = 0
    DefaultRampRateDbPerSec = 0

    %ASD Router
    EnableAux = [0] 
    AuxGain = 0 
    NumAux = 0 
    MainGain = 1

    %Limiter
    DefaultAttackDb  = 10e3 
    DefaultDecayDb   = 100 
    DefaultMaxAttack = 10 
    DefaultThreshold = 0.9 

end
  methods
    %----------------------------------------------------------------------
    function this = Ann1pathConfig()
        % Ann1pathConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

        %.Delete these examples:
        %.  this.LeftConfig = SubBlockConfig;
        %.  this.UpdatePropertyDefault('ComponentProperty.SubProp', value);
        %.  this.LockPropertyValue('InheritedProperty', forcedValue);
        %.  this.LockPropertyAsAlias('Comp.FrameSize', 'FrameSize');
        this.AnnInEQ =  PoolIirTunableConfig;
        this.Default(@this.AnnInEQ.NumChannels, @() this.NumInChans);
        this.Default(@this.AnnInEQ.MaxNumStages, @() this.maxNumInEqStages);

        this.AnnVolume = AnnVolumeConfig; 
        this.Default(@this.AnnVolume.NumInputChannels, @() this.NumInChans);
        this.Default(@this.AnnVolume.InitialLinearGain, @() this.InitialLinearGain * ones(1, this.NumInChans));

        this.AnnGainCalc = AnnGainCalcConfig; 
        this.Default(@this.AnnGainCalc.Fd, @ this.Fd);
        this.Default(@this.AnnGainCalc.NumAnnIn, @ this.NumInChans);

        this.AnnVnc = AnnVncConfig;
        this.Default(@this.AnnVnc.FrameSize, @ this.FrameSize);

        this.AnnSteering = AnnSteeringConfig; 
        this.Default(@this.AnnSteering.FrameSize, @() this.FrameSize);
        this.Default(@this.AnnSteering.MinGainDb, @() this.MinGainDb);
        this.Default(@this.AnnSteering.NumAnnIn, @() this.NumInChans);
        this.Default(@this.AnnSteering.NumAnnOut, @() this.NumSteerChans);
        this.Default(@this.AnnSteering.InitialRampingRateRtc, @() this.InitialRampingRateRtc * ones(1, this.NumSteerChans*this.NumInChans));
        this.Default(@this.AnnSteering.InitialRampingTimeRtc, @() this.InitialRampingTimeRtc * ones(1, this.NumSteerChans*this.NumInChans));
        this.Default(@this.AnnSteering.InitialTargetGainRtc, @() this.InitialTargetGainRtc * ones(1, this.NumSteerChans*this.NumInChans));
        this.Default(@this.AnnSteering.Prevgainvalues, @() this.Prevgainvalues * ones(this.NumSteerChans,this.NumInChans));
        this.Default(@this.AnnSteering.SnapThresh, @() this.SnapThresh * ones(this.NumSteerChans,this.NumInChans));

        this.AnnRouter = RouterConfig; 
        this.Default(@this.AnnRouter.NumIn, @() this.NumSteerChans);
        this.Default(@this.AnnRouter.NumOut, @() this.NumOutChans);
        this.Default(@this.AnnRouter.DefaultOutputMap, @() 1:this.NumOutChans);

        this.AnnDelay = TunableDelayPerChannelConfig;  
        this.Default(@this.AnnDelay.DefaultDelayAmount, @() this.DefaultDelayAmount * ones(1, this.NumOutChans));
        this.Default(@this.AnnDelay.DefaultDelayType, @() this.DefaultDelayType);
        this.Default(@this.AnnDelay.NumChannels, @() this.NumOutChans);

        this.AnnOutEQ = PoolIirTunableConfig; 
        this.Default(@this.AnnOutEQ.NumChannels, @() this.NumOutChans);
        this.Default(@this.AnnOutEQ.MaxNumStages, @() this.maxNumOutEqStages);


        this.AnnMute = SelectiveChannelMuteConfig;
        this.Default(@this.AnnMute.InitialMuteState, @() this.InitialMuteState);
        this.Default(@this.AnnMute.NumChannelsIn, @() this.NumOutChans);
        this.Default(@this.AnnMute.DefaultRampTimeMsec, @() this.DefaultRampTimeMsec);
        this.Default(@this.AnnMute.DefaultMuteBits, @() this.DefaultMuteBits);
        this.Default(@this.AnnMute.DefaultRampRateDbPerSec, @() this.DefaultRampRateDbPerSec);

        this.AnnOutputRouter  = AsdRouterConfig;
        this.Default(@this.AnnOutputRouter.FrameSize, @() this.FrameSize);
        this.Default(@this.AnnOutputRouter.EnableAux, @() this.EnableAux);
        this.Default(@this.AnnOutputRouter.NumAux, @() this.NumAux);
        this.Default(@this.AnnOutputRouter.NumMainCh, @() this.NumOutChans);
        this.Default(@this.AnnOutputRouter.NumChanOut, @() this.NumOutChans);
        this.Default(@this.AnnOutputRouter.MainSelect, @() 1:this.NumOutChans);
        this.Default(@this.AnnOutputRouter.MainGain, @() this.MainGain * ones(1, this.NumOutChans));
        this.Default(@this.AnnOutputRouter.AuxGain, @() this.AuxGain * ones(1, this.EnableAux));

        this.AnnLimiter  = AnnLimiterConfig;
        this.Default(@this.AnnLimiter.DefaultAttackDb, @() this.DefaultAttackDb * ones(1, this.NumOutChans));
        this.Default(@this.AnnLimiter.DefaultDecayDb, @() this.DefaultDecayDb * ones(1, this.NumOutChans));
        this.Default(@this.AnnLimiter.DefaultMaxAttack, @() this.DefaultMaxAttack * ones(1, this.NumOutChans));
        this.Default(@this.AnnLimiter.DefaultThreshold, @() this.DefaultThreshold * ones(1, this.NumOutChans));

        this.AnnSoftClipper = AnnSoftClipperConfig;

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
