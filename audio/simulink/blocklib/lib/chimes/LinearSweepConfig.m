classdef LinearSweepConfig < SignalGeneratorConfig
% LinearSweepConfig Build-time configuration class for the LinearSweep block
% Usage:
%    aConfig = LinearSweepConfig
%

%   Copyright 2022 Bose Corporation

  properties


  end
  methods
    %----------------------------------------------------------------------
    function this = LinearSweepConfig()
        % LinearSweepConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.EndFreqHz           = [1200 1200];  % = end_freq_Hz_phaseDetector
        this.FrameSizeSamples    = 32;  % = sys_defs.frameSize_samples
        this.FsHz                = 44100;  % = sys_defs.Fs_Hz
        this.LinearSweepOn       = 2;  % = linear_sweep_ON_phaseDetector
        this.NumChannel          = 2;  % = Numch
        % If blank, it gets automatically calculated based on SineTableLength
        % this.SineTable           % = sys_defs.sineTable
        this.SineTableLength     = 1024;  % = sys_defs.sineTableLength
        this.StartFreqHz         = [1200 1200]; % = start_freq_Hz_phaseDetector
        this.StartPhaseRad       = [0 pi/2]; % = start_phase_rad_LinearSweep_phaseDetector
        this.SweepTimeSec        = [0.25 0.25]; % = sweep_time_sec_phaseDetector
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@SignalGeneratorConfig(this);

    end

  end
end
