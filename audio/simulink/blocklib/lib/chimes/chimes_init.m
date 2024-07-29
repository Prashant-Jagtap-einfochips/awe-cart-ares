% -----------------------------------------------------------------------------------------------
% BOSE CORPORATION
% COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
% This program may not be reproduced, in whole or in part in any
% form or any means whatsoever without the written permission of:
% BOSE CORPORATION
% The Mountain
% Framingham, MA 01701-9168
% -----------------------------------------------------------------------------------------------
% Filename: chimes_init.m
% Description: Chimes parameters init file
% -----------------------------------------------------------------------------------------------

% assigns sys_defs model parameters
% Pull in build time configuration parameters
% sys_defs = auto_gen_sys_defs();
sys_defs.sineTableLength            = sys_defs.sin_lookup_table_length;
sys_defs.SampleRateSelect           = 1; 
sys_defs.Fs_Hz                      = sys_defs.Fs;
sys_defs.frameSize_samples          = sys_defs.frame_size_in_samples;
%% For BAC_externalChime command;
sys_defs.nInternalBeepsGongs        = 4;
%% External Chimes
NumInternalChimes = 4;
NumExternalChimes = 2;
NumChimesMixerOut = NumExternalChimes;
post_out_channels = NumChimesMixerOut;
%% Audio Pilot/VNC 
include_audiopilot_for_chime    = 0;

% External Mixer
fMixerMatrixExtChimes = single(eye(NumChimesMixerOut,NumExternalChimes));

% External Chimes Delay
ext_chimes_max_delay_per_ch_in_samples = 128;
ext_chimes_max_delay_in_samples = NumChimesMixerOut * ext_chimes_max_delay_per_ch_in_samples;
ext_chimes_init_delay = zeros(1,NumExternalChimes); 

% External Chimes EQ
ext_chimes_max_num_EQ_stages_per_ch = 6;
ext_chimes_max_num_EQ_stages = NumChimesMixerOut * ext_chimes_max_num_EQ_stages_per_ch;
ext_chimes_rawSOS = zeros(ext_chimes_max_num_EQ_stages, 6);
ext_chime_numStages = ones(1, NumChimesMixerOut) * ext_chimes_max_num_EQ_stages_per_ch;
sosIdx = 0;
for k = 1 : NumChimesMixerOut
    % set to pass-thru SOS coefficients 
    ext_chimes_rawSOS(sosIdx+1 : sosIdx+ext_chime_numStages(k), :) = ...
        repmat([1 0 0 1 0 0], ext_chimes_max_num_EQ_stages_per_ch, 1);
    sosIdx = sosIdx + ext_chime_numStages(k);
end

%% Internal Chimes
sys_defs.nInternalBeepsGongs = 4;
sys_defs.Fs_Hz = sys_defs.Fs;
sys_defs.frameSize_samples = sys_defs.frame_size_in_samples;
sys_defs.sineTableLength = sys_defs.sin_lookup_table_length;
%sys_defs.sineTable = sys_defs_chimes.sineTable;
start_time                          = 0;
end_time                            = ( sys_defs.sineTableLength - 1 )/sys_defs.sineTableLength;
inc_time                            = (1/sys_defs.sineTableLength);
time_values                         = [ start_time : inc_time : end_time ];
sineTable                           = 0.99 * sin( 2 * pi * time_values );
sys_defs.sineTable                  = sineTable;
tuneTop_time = 0;
default_time = 1;
Nch = 1;
nch=Nch;

% Linear Sweep
linear_sweep_ON = 2;
start_freq_Hz = 50;
end_freq_Hz = 1000;
sweep_time_sec = 0.25;
start_phase_rad_LinearSweep = 0;

% Multitone
multitone_ON = 6;
multitone_channel_map = 3;
Num_Multitone_channels = 1;
NumTones = 3;
Tone_freq_Hz = [440 550 750];
start_phase_rad = [0 pi/2 pi];
ToneGains = [1 1 1];

% Log Sweep
log_sweep_ON = 1;
start_freq_Hz_LogSweep = 50;
end_freq_Hz_LogSweep = 1000;
sweep_time_sec_LogSweep = 0.1;
start_phase_rad_LogSweep = 0;

% Old-fashioned Sweep
old_sweep_ON = 3;
start_freq_Hz_OldSweep = 100;
end_freq_Hz_OldSweep = 1000;
steps = 5;
start_phase_rad_OldSweep = 0;
cycles_per_step = 10;

% Sawtooth
sawtooth_mode_ON = 5;
start_frequencies = 240;
start_phase = 0;

% White Noise
        random_noise_ON=4;
        random_noise =0.5;
        chimeConfig.sigGen.Nch          =4;
        chimeConfig.sigGen.select_mode =[1 2 3 4];
       
        chimeConfig.sigGen.SigNch                         =2;
        chimeConfig.sigGen.start_freq_Hz                  = [500 600 700 800 900 1000 100 100];  
        chimeConfig.sigGen.end_freq_Hz                    = [5000 6000 7000 8000 9000 100000 1000 1000]; 
        chimeConfig.sigGen.sweep_time_sec                 = [0.1 0.1 0.25 0.25 0 0 0.1 0.1];
        chimeConfig.sigGen.start_phase_rad                = [0 0 0 0 0 0 0 0];
        chimeConfig.sigGen.cycles_per_step                = [5 5 5 5 5 5 5 5];
        chimeConfig.sigGen.tone_gains                     = [1 1 1 1 1 1 1 1];
        chimeConfig.sigGen.random_noise                  =  [0.5 0.5 0.5 0.5 0.5 0.5 0.5 0.5];
        chimeConfig.sigGen.num_signals_per_mode             = [2];
        chimeConfig.sigGen.num_channels                        = length(chimeConfig.sigGen.num_signals_per_mode);
        chimeConfig.sigGen.NumTones                            = sum(chimeConfig.sigGen.num_signals_per_mode );

        if(~isempty(RTC_SIM_OBJ))
        %if (~exist(chimeConfig.RTC_SIM_OBJ, 'var') || (~chimeConfig.RTC_SIM_OBJ.REQ_list.Count))
        %    RTC_SIM_OBJ = RTC_SimData();
        end
        
        chimeConfig.scalarchime.num_internal_chime          = chimeConfig.sigGen.Nch;
        chimeConfig.scalarchime.OffsetIntChime = 1;
        chimeConfig.scalarchime.SelectNumIntChimes = chimeConfig.scalarchime.OffsetIntChime:chimeConfig.scalarchime.num_internal_chime;
        chimeConfig.scalarchime.num_click_clack                         = 1;
        
        chimeConfig.scalarchime.nch = length(chimeConfig.sigGen.select_mode);
        chimeConfig.scalarchime.num_inp_chs                         = chimeConfig.scalarchime.nch;
        % assigns state count
        chimeConfig.scalarchime.NSTATES                             = 6;           
        chimeConfig.scalarchime.decrementVals                       = ones(1,chimeConfig.scalarchime.NSTATES*chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.decrementVals(1:chimeConfig.scalarchime.NSTATES:end)        = 0;
        % assigns counter reset values for each state
        chimeConfig.scalarchime.count                               = zeros(1,chimeConfig.scalarchime.NSTATES*chimeConfig.scalarchime.nch);
        % assigns counter decrement for each state
        chimeConfig.scalarchime.countDecrement                      = chimeConfig.scalarchime.decrementVals;
        % assigns state machine variables
        chimeConfig.scalarchime.state                               = zeros(1,chimeConfig.scalarchime.nch);
        %assigns counter for each states
        chimeConfig.scalarchime.countState                          = zeros(1,chimeConfig.scalarchime.NSTATES*chimeConfig.scalarchime.nch); 

        
        %% State machine
% 
%                      State:    idle  rise  fall  check  wait    check
%                                                  pulse  for     group
%                                                  count  period  count
%                                 0     1     2     3      4       5
%%  
        chimeConfig.scalarchime.nextState_expired    =           [0     2     3     4      5       0]; %Next state when counter has expired
        chimeConfig.scalarchime.nextState_notExpired =           [0     1     2     1      4       1]; %Next state when counter has not expired
        chimeConfig.scalarchime.outputState          =           [1     1     1     0      1       0]; %Flag for "state is an output state"

        % assigns six states
        chimeConfig.scalarchime.IDLE                                = 0;
        chimeConfig.scalarchime.RISE                                = 1;
        chimeConfig.scalarchime.FALL                                = 2;
        chimeConfig.scalarchime.CHECK_PULSE_COUNT                   = 3;
        chimeConfig.scalarchime.WAIT_FOR_PERIOD                     = 4;
        chimeConfig.scalarchime.CHECK_GROUP_COUNT                   = 5;
        chimeConfig.scalarchime.CHIMECONTROL_NSTATES                = chimeConfig.scalarchime.NSTATES;
        
        
        chimeConfig.scalarchime.volume = [224 224 224 224];
		chimeConfig.scalarchime.riseTime = 5*[0.002 0.002 0.002 0.002];
		chimeConfig.scalarchime.onTime  = [0.030 0.030 0.030 0.030] ;
		chimeConfig.scalarchime.fallTime = [0.045 0.045 0.045 0.045 ];
		chimeConfig.scalarchime.offTime  = [0.500 0.500 0.500 0.500];
		chimeConfig.scalarchime.numPulses = [3 3 3 3 ];
        chimeConfig.scalarchime.period                              = 1*ones(1,chimeConfig.scalarchime.nch);
        
        % assigns initial values for chime variables
        chimeConfig.scalarchime.startOfFinalChime                   = 0*ones(1,chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.endOfFinalChime                     = 0*ones(1,chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.chimeEndNotification                = 1*ones(1,chimeConfig.scalarchime.nch);  %% boolean value
        chimeConfig.scalarchime.newProfile                          = 0;
        chimeConfig.scalarchime.chimeRouting                        = 0;
		
		chimeConfig.scalarchime.num_chime_count =8;
		chimeConfig.scalarchime.chime_group_count=[2 2 2 2];
		chimeConfig.scalarchime.spkrRouting= [0 0 0 0 ];
		chimeConfig.scalarchime.include_internal_chime_flag =[1 1 1 1 ];
		chimeConfig.scalarchime.chimeCancel = 1;
		chimeConfig.scalarchime.chimeType = [2 2 2 2];
        chimeConfig.scalarchime.chimePlayPending            = 0*ones(1, chimeConfig.scalarchime.nch); 
        chimeConfig.scalarchime.remainingCount              = 0;
        chimeConfig.scalarchime.chimeEndNotification        = 0*ones(1, chimeConfig.scalarchime.nch);  %% boolean value
        chimeConfig.scalarchime.FW_AppendHostNotification   = -1*ones(1,chimeConfig.scalarchime.nch);  %% by default -1, 0-startOfFinalChimeNotify, 1-endOfFinalChimeNotify
        
        
        i=[1:chimeConfig.scalarchime.nch];
        chimeConfig.scalarchime.chime_input_index=i;
        %% For click clack signal
        chimeConfig.scalarchime.include_internal_click_clack=0; %% Include click clock if set to 1

        %% For threshold value of numPulse of chime
        chimeConfig.scalarchime.thresholdNumPulse           = 1;

		        
        %RTC_SIM_OBJ.AddREQ('ChimesSignalGeneratorSet',0,...
        %      [0001,chimeConfig.sigGen.select_mode,chimeConfig.sigGen.start_freq_Hz,chimeConfig.sigGen.end_freq_Hz,chimeConfig.sigGen.sweep_time_sec,chimeConfig.sigGen.start_phase_rad,chimeConfig.sigGen.tone_gains,chimeConfig.sigGen.random_noise,chimeConfig.sigGen.cycles_per_step]);
        %RTC_SIM_OBJ.AddREQ('ChimesControlPulseConfig',0,...
        %      [0001,chimeConfig.scalarchime.volume,chimeConfig.scalarchime.riseTime, chimeConfig.scalarchime.onTime, chimeConfig.scalarchime.fallTime, chimeConfig.scalarchime.offTime, chimeConfig.scalarchime.numPulses]);

        %RTC_SIM_OBJ.AddREQ('ChimesControlStatePlayback',0,...
        %      [0001, chimeConfig.scalarchime.playback_flag,chimeConfig.scalarchime.forever,chimeConfig.scalarchime.immediately, chimeConfig.scalarchime.chimeEndNotification]);
          
        %RTC_SIM_OBJ.AddREQ('ChimesControlChimesFeature',0,...
        %      [0001, chimeConfig.scalarchime.num_chime_count, chimeConfig.scalarchime.chime_group_count, chimeConfig.scalarchime.spkrRouting, chimeConfig.scalarchime.include_internal_chime_flag, chimeConfig.scalarchime.chimeCancel, chimeConfig.scalarchime.chimeType]);
     
        %% RTC Parameters for External chime configuration	 
		chimeConfig.ExternalChime.externalChimeCancel                 = 0; 
        %RTC_SIM_OBJ.AddREQ('ChimesExtChimeConfig',0,[0001,chimeConfig.ExternalChime.externalChimeCancel]);
		
	%% RTC Parameters for Waveplay chime configuration
        %% Note: Click signal is used solely for testing purpose, user may feed any
        %        known test signal like "Welcome" ,"Good Bye" etc.
        load click.mat;
        load clack.mat;
        table1Len = length(click);
		table2Len = length(clack);
		table3Len = length(clack);
		table4Len = length(clack);
        MaxtableLen=1120; %MaxtableLen=tableLen+padLen;
		selectclickclack=1; %Select either 1- click, 2-clack, 3- table3, 4 -table4
		TotalPeriod = 25;   % In milli seconds
		clicksignal =single(zeros(MaxtableLen,1));
		clacksignal =single(zeros(MaxtableLen,1));
		Table3 =single(zeros(MaxtableLen,1));
		Table4 =single(zeros(MaxtableLen,1));
		clicksignal(1:table1Len,:)= click;
		clacksignal(1:table2Len,:)= clack;
		Table3(1:table3Len,:)= clack;
		Table4(1:table4Len,:)= clack;
		p.table1 = clicksignal;  % Wave table
		p.table2 = clacksignal;  % Wave table
		p.table3 = Table3;  % Wave table
		p.table4 = Table4;  % Wave table

        
        %% Note : Maximum Table length number has been chosen based on use case of click and clack
        %         signals, typically click signal is of 1079 values, and clack signal is of
        %         1095 values, we round off to the nearest number in multiple of frame size.

        tableSig = click;  

        chimeConfig.wplay.len                 = MaxtableLen;
		chimeConfig.wplay.clickwavtable       = tableSig;
		chimeConfig.wplay.table = chimeConfig.wplay.clickwavtable;
        chimeConfig.wplay.frameSize         = sys_defs.frameSize_samples;  
        chimeConfig.wplay.tableLength       = chimeConfig.wplay.len;
		chimeConfig.wplay.tableIndex        = 0;        % reset pointer to start of the table,
        chimeConfig.wplay.playControl       = 1;        % Volume and switch to start wave play
        chimeConfig.wplay.controlState      = 0;        % Old state of playControl  
        chimeConfig.wplay.minVol            = 0.00001;  % -100 dB
		%RTC_SIM_OBJ.AddREQ('ChimesWaveplay',0,[0001,chimeConfig.wplay.playControl]);		

        % STEP2:       
        chimeConfig.scalarchime.immediately     = 0*ones(1,chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.forever         = 1*ones(1,chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.playback_flag   = 1*ones(1,chimeConfig.scalarchime.nch);
        chimeConfig.scalarchime.chimeEndNotification= 1*ones(1,chimeConfig.scalarchime.nch);
      
       %RTC_SIM_OBJ.AddREQ('ChimesControlPulseConfig',0.001,...
       %     [0002,chimeConfig.scalarchime.volume,chimeConfig.scalarchime.riseTime, chimeConfig.scalarchime.onTime, chimeConfig.scalarchime.fallTime, chimeConfig.scalarchime.offTime, chimeConfig.scalarchime.numPulses]);
        
       %RTC_SIM_OBJ.AddREQ('ChimesControlStatePlayback',0.001,...
       %     [0002, chimeConfig.scalarchime.playback_flag,chimeConfig.scalarchime.forever,chimeConfig.scalarchime.immediately, chimeConfig.scalarchime.chimeEndNotification]);
       
       %RTC_SIM_OBJ.AddREQ('ChimesControlChimesFeature',0.001,...
       %     [0002,chimeConfig.scalarchime.num_chime_count, chimeConfig.scalarchime.chime_group_count, chimeConfig.scalarchime.spkrRouting, chimeConfig.scalarchime.include_internal_chime_flag, chimeConfig.scalarchime.chimeCancel, chimeConfig.scalarchime.chimeType]);
       
       %% External ChimeVolume Parameter Settings    
        chimeConfig.ExternalChime.num_external_chime =1;
        chimeConfig.ExternalChime.include_external_chime_flag = 1*ones(1, chimeConfig.ExternalChime.num_external_chime);        
        %% For external Chime playing
        include_eq_for_external_chime = 0;
        chimeConfig.ExternalChime.TargetGainVal =224.*ones(1,chimeConfig.ExternalChime.num_external_chime);
        chimeConfig.ExternalChime.TargetGainVal = chimeConfig.ExternalChime.TargetGainVal(1:chimeConfig.ExternalChime.num_external_chime);
        chimeConfig.ExternalChime.Ramp_rate_db = 0.*ones(1,chimeConfig.ExternalChime.num_external_chime);
        chimeConfig.ExternalChime.lpfTauVal= 1.*ones(1,chimeConfig.ExternalChime.num_external_chime);
        %RTC_SIM_OBJ.AddREQ('ChimesVolumeSet',0,[0001,chimeConfig.ExternalChime.TargetGainVal,chimeConfig.ExternalChime.Ramp_rate_db,chimeConfig.ExternalChime.lpfTauVal]);
        
        %% ChimeRouter Parameter Settings        
        chimeConfig.chimeRouter.In=chimeConfig.ExternalChime.num_external_chime;						%No of inputs
        chimeConfig.chimeRouter.output_indxs =[1:chimeConfig.chimeRouter.In];				%Index of Router outputs
        %L=FrameSize;
        
        %% Rgainz Parameter Settings    
        chimeConfig.rgainz.sysin_channel_count = chimeConfig.scalarchime.nch;
        chimeConfig.rgainz.gain=1*ones(1,chimeConfig.rgainz.sysin_channel_count);
        chimeConfig.rgainz.sample_rate_in_hertz= sys_defs.Fs_Hz;
        sample_rate_in_hertz = sys_defs.Fs_Hz;
        chimeConfig.rgainz.frame_size= sys_defs.frameSize_samples;
        frame_size = sys_defs.frameSize_samples;
        sysin_channel_count = chimeConfig.rgainz.sysin_channel_count;
        gain=chimeConfig.rgainz.gain;
        GainLibrary_data_rgainz;        
        %%  annProcIn specific configurations
        Fa=1000*ones(1,chimeConfig.chimeRouter.In);
        Pa=0*ones(1,chimeConfig.chimeRouter.In);
        Aa=1:chimeConfig.chimeRouter.In;
        
        %%  External Chime PoolIIR for multi channels
        chimeConfig.PoolIIR.ChimeIn                 = chimeConfig.ExternalChime.num_external_chime;
        chimeConfig.PoolIIR.subsystem_output        = chimeConfig.ExternalChime.num_external_chime;
        chimeConfig.PoolIIR.maxStagesPerChannel_o   = 1;
        chimeConfig.PoolIIR.maxNumStages_OutEq      = chimeConfig.PoolIIR.subsystem_output*chimeConfig.PoolIIR.maxStagesPerChannel_o;
        chimeConfig.PoolIIR.numStages_OutEq         = ones(1,chimeConfig.PoolIIR.subsystem_output);
        chimeConfig.PoolIIR.coeffs_OutEq            = [];

        chimeConfig.PoolIIR.PASSTHROUGH             = 1;
        chimeConfig.PoolIIR.coeffs_InEq             = [];

        if(chimeConfig.PoolIIR.PASSTHROUGH)
            for i = 1:chimeConfig.PoolIIR.ChimeIn
                chimeConfig.PoolIIR.sos{i}          = [1 0 0 1 0 0];
                chimeConfig.PoolIIR.coeffs_OutEq    = [chimeConfig.PoolIIR.coeffs_OutEq; (chimeConfig.PoolIIR.sos{i})];
            end
        else    
            % designs some octave filters and program into pooliir object
            chimeConfig.PoolIIR.maxStagesPerChannel = 3;
            chimeConfig.PoolIIR.maxNumStages_InEq   = chimeConfig.PoolIIR.ChimeIn*chimeConfig.PoolIIR.maxStagesPerChannel;
            chimeConfig.PoolIIR.f0                  = 1000;    
            chimeConfig.PoolIIR.numStages_InEq      = 2.*ones(1,chimeConfig.PoolIIR.ChimeIn);
            chimeConfig.PoolIIR.ripple_db           = 1;
            chimeConfig.PoolIIR.stop_atten_db       = 80;   

            for i = 1:chimeConfig.PoolIIR.ChimeIn
                [Z,P,K]                             = ellip( chimeConfig.PoolIIR.numStages_InEq(i), chimeConfig.PoolIIR.ripple_db, chimeConfig.PoolIIR.stop_atten_db, chimeConfig.PoolIIR.f0*[1/sqrt(2) sqrt(2)]*2/sys_defs.Fs_Hz);
                chimeConfig.PoolIIR.sos{i}          = zp2sos(Z,P,K);
                chimeConfig.PoolIIR.coeffs_InEq     = [chimeConfig.PoolIIR.coeffs_InEq; (chimeConfig.PoolIIR.sos{i})];
                chimeConfig.PoolIIR.f0              = chimeConfig.PoolIIR.f0 + 200;
            end  
        end
        
        %% External Chime Processing Pool Delay
        chimeConfig.pooldelay.nOutputDelay = 0;
        chimeConfig.pooldelay.delay_chain             = 4.*ones(1,chimeConfig.ExternalChime.num_external_chime) + chimeConfig.pooldelay.nOutputDelay;   
        chimeConfig.pooldelay.max_delay               = 4000;
        
        %RTC_SIM_OBJ.AddREQ('ChimesRouterSet',0,[0001,chimeConfig.chimeRouter.output_indxs]);
         %% GainCalcInit
        chimeConfig.nch = chimeConfig.scalarchime.nch + chimeConfig.ExternalChime.num_external_chime; 
        chimeConfig.GainCalc.ChimeIn             =chimeConfig.nch;
        chimeConfig.GainCalc.noise               = 0*ones(1,chimeConfig.nch);
        chimeConfig.GainCalc.vncEnable			= 1;
        chimeConfig.GainCalc.vncBoostGain		= chimeConfig.GainCalc.vncEnable;
        chimeConfig.GainCalc.NoiseOverRideFlag 	= 0;
        chimeConfig.GainCalc.NoiseOverRide 		= 0;
        chimeConfig.GainCalc.boostOverRide 		= zeros(1,chimeConfig.GainCalc.ChimeIn);  
        chimeConfig.GainCalc.boostOverRideFlag 	= zeros(1,chimeConfig.GainCalc.ChimeIn);
        chimeConfig.GainCalc.VNCThresh           = zeros(1,chimeConfig.GainCalc.ChimeIn);
        chimeConfig.GainCalc.VNCSlope            = 0.4.*ones(1,chimeConfig.GainCalc.ChimeIn);
        chimeConfig.GainCalc.maxBoost            = 18*ones(1,chimeConfig.GainCalc.ChimeIn);
        chimeConfig.GainCalc.maxSPL              = 120*ones(1,chimeConfig.GainCalc.ChimeIn);
        chimeConfig.GainCalc.minVNCSignal        = 40;
        chimeConfig.GainCalc.rampTau             = 0.1;
        chimeConfig.GainCalc.Fd                  = sys_defs.Fs_Hz/sys_defs.frameSize_samples;
        chimeConfig.GainCalc.rampWc              = 1/(chimeConfig.GainCalc.rampTau*chimeConfig.GainCalc.Fd);
        chimeConfig.GainCalc.rampSinCoef         = sin(chimeConfig.GainCalc.rampWc);
        chimeConfig.GainCalc.rampCosCoef         = cos(chimeConfig.GainCalc.rampWc);
        chimeConfig.GainCalc.rampCoef            = (1-chimeConfig.GainCalc.rampSinCoef)/chimeConfig.GainCalc.rampCosCoef;
        %RTC_SIM_OBJ.AddREQ('ChimesVNCNoiseSig',0,[0001,chimeConfig.GainCalc.noise]);
        
        %% VNC Initialization
        chimeConfig.VNC.fsig                = 20;
        chimeConfig.VNC.Fs2                 = sys_defs.Fs_Hz;
        [chimeConfig.VNC.num ,chimeConfig.VNC.den]          = butter(1,2*chimeConfig.VNC.fsig/chimeConfig.VNC.Fs2);
        chimeConfig.VNC.zinit               = (chimeConfig.VNC.num(2) - chimeConfig.VNC.den(2));
