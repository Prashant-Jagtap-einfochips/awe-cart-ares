classdef LogSweepConfig < SignalGeneratorConfig
% LogSweepConfig Build-time configuration class for the LogSweep block
% Usage:
%    aConfig = LogSweepConfig
%

%   Copyright 2022 Bose Corporation

  properties

  end
  methods
    %----------------------------------------------------------------------
    function this = LogSweepConfig()
        % LogSweepConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.EndFreqHz           = 1000; % = end_freq_Hz
        this.FrameSizeSamples    = 32; % = sys_defs.frameSize_samples
        this.FsHz                = 44100; % = sys_defs.Fs_Hz
        this.LogSweepOn          = 1; % = log_sweep_ON
        this.NumChannel          = 1; % = Nch
        % If blank, it gets automatically calculated based on SineTableLength
        % this.SineTable           % = sys_defs.sineTable
        this.SineTableLength     = 1024; % = sys_defs.sineTableLength
        this.StartFreqHz         = 50; % = start_freq_Hz
        this.StartPhaseRad       = 0; % = start_phase_rad
        this.SweepTimeSec        = 0.1; % = sweep_time_sec
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@SignalGeneratorConfig(this);

    end

  end
end
