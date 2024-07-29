classdef ChimeSystemConfig < ScalarChimeControlConfig
% ChimeSystemConfig Build-time configuration class for the ChimeSystem block
% Usage:
%    aConfig = ChimeSystemConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    ChimeGroupCount  % = chimeConfig.scalarchime.chime_group_count
    ControlState  % = chimeConfig.wplay.controlState

    %Gain  % = chimeConfig.rgainz.gain
%     IncludeEqForExternalChime  % = include_eq_for_external_chime
%     IncludeExternalChimeFlag  % =  chimeConfig.ExternalChime.include_external_chime_flag
%     InitialLinearGain  % = chimeConfig.ExternalChime.TargetGainVal
    MinVol  % = chimeConfig.wplay.minVol
%     NIn  % = chimeConfig.chimeRouter.In
    NumExternalChime  % = chimeConfig.ExternalChime.num_external_chime
%     OutputIndex  % = chimeConfig.chimeRouter.output_indxs
    PlayControl  % = chimeConfig.wplay.playControl
    SpkrRouting  % = chimeConfig.scalarchime.spkrRouting
    SysinChannelCount  % = chimeConfig.rgainz.sysin_channel_count
%     Table  % = chimeConfig.wplay.table
    TableIndex  % = chimeConfig.wplay.tableIndex
    
    % Properties previously coming directly from base workspace
    IncludeAudioPilotForChime = 0 % "include_audiopilot_for_chime"
    
    % Configurable Components
    ExternalChimeProcess ExternalChimeProcessConfig
    WavePlay WavePlayConfig
    AnnVnc AnnVncConfig
	Len
	ExternalChimeCancel
  end
  
  % Less-configurable components
  properties (Dependent)
    NonEntGainCalc NonEntGainCalcConfig
  end      
  
  methods
    %----------------------------------------------------------------------
    function this = ChimeSystemConfig()
        % ChimeSystemConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        
        % ----- ExternalChimesProcess
        this.ExternalChimeProcess = ExternalChimeProcessConfig;
        this.LockPropertyAsAlias('ExternalChimeProcess.FrameSize','FrameSizeSamples');
        % Unknown why this was locked into place, but honoring that design:
        this.LockPropertyValue('ExternalChimeProcess.NumExtChimes', 1);
        
        % ----- WavePlay
        this.WavePlay = WavePlayConfig;
        this.LockPropertyAsAlias('WavePlay.Fs','FsHz');
        this.LockPropertyAsAlias('WavePlay.FrameSize','FrameSizeSamples');

        % ----- AnnVnc
        this.AnnVnc = AnnVncConfig;
        this.LockPropertyAsAlias('AnnVnc.FrameSize','FrameSizeSamples');
        this.LockPropertyAsAlias('AnnVnc.LowPassSamplingHz', 'FsHz');
        %(so there's really only the smoothing frequency that can be set)
        
        

    end
    %----------------------------------------------------------------------
    function [ok, errCell] = Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        [ok, errCell] = Validate@BtcBaseClass(this);
    end

    %----------------------------------------------------------------------
    function value = get.NonEntGainCalc(this)
        % Unsure why these values were directly locked into the canvas
        % design of the ChimeSystem block, but honoring that choice below.
        % Switch NonEntGainCalc to a normal property to regain its ability
        % to be configured at build time.
        value = NonEntGainCalcConfig;
        value.DefaultVncThresh = 0;
        value.DefaultVncSlope = 0.4;
        value.DefaultMaxBoost = 18;
        value.DefaultMaxSpl = 120;
        value.DefaultMinVncSignal = 40;
        value.Fd = this.FsHz / this.FrameSizeSamples;
        value.FrameSize = this.FrameSizeSamples;
        % Overrides:
        value.DefaultNoiseOverrideFlag = 0;
        value.DefaultNoiseOverride = 0;
        value.DefaultBoostOverrideFlag = 0;
        value.DefaultBoostOverride = 0;
    end       
  end
end
