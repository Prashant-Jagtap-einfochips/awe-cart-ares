classdef SignalGeneratorConfig < BtcBaseClass
% LogSweepConfig Build-time configuration class for the LogSweep block
% Usage:
%    aConfig = SignalGeneratorConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    %% Common
    FrameSizeSamples    = 32 % = sys_defs.frameSize_samples
    FsHz                = 44100 % = sys_defs.Fs_Hz
    % If blank, it gets automatically calculated based on SineTableLength
    SineTable           % = sys_defs.sineTable
    SineTableLength     = 1024 % = sys_defs.sineTableLength
    StartFreqHz         = 50 % = start_freq_Hz
    StartPhaseRad       = 0 % = start_phase_rad

    %% Common to LogSweep, LinearSweep, OldSweep, WhiteNoiseGen
    NumChannel          = 1 % = Nch
    
    %% Common to LogSweep, LinearSweep, OldSweep
    EndFreqHz           = 1000 % = end_freq_Hz
    
    %% Common to LogSweep, LinearSweep
    SweepTimeSec        = 0.1 % = sweep_time_sec
    
    %% LogSweep
    LogSweepOn          = 1 % = log_sweep_ON
    
    %% LinearSweep
    LinearSweepOn       = 2;  % = linear_sweep_ON_phaseDetector
    
    %% OldSweep
    Steps               = 5 % = steps
    CyclesPerStepc      = 10 % = cycles_per_step
    OldSweepOn          = 3 % = old_sweep_ON

    %% MultiToneGen
    MultitoneChannelMap = 3  % = multitone_channel_map
    MultitoneOn         = 6 % = multitone_ON
    NumMultitoneChannels = 1 % = Num_Multitone_channels
    NumTones            = 3 % = NumTones
    ToneFreqHz          = [100 1000 10000]  % = Tone_freq_Hz
    ToneGains           % = ToneGains
    
    %% Sawtooth
    DefaultTime         = 1 % = default_time
    DefaultTuneTopTime  = 0 % = tuneTop_time
    SawtoothModeOn      = 5 % = sawtooth_mode_ON
    
    %% WhiteNoiseGen
    RandomNoise         = 0.5 % = random_noise
    RandomNoiseOn       = 4  % = random_noise_ON
  end
  methods
    %----------------------------------------------------------------------
    function this = SignalGeneratorConfig()
        % SignalGeneratorConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
        
        if (isempty(this.SineTable))
            end_time = ( this.SineTableLength - 1 )/this.SineTableLength;
            inc_time = (1/this.SineTableLength);
            time_values = [start_time : inc_time : end_time];
            this.SineTable  = 0.99 * sin( 2 * pi * time_values );
        end
        
        if (isempty(this.ToneGains))
            this.ToneGains = ones(1,this.NumTones);
        end

    end

  end
end
