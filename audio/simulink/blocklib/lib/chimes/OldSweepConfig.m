classdef OldSweepConfig < SignalGeneratorConfig
% OldSweepConfig Build-time configuration class for the OldSweep block
% Usage:
%    aConfig = OldSweepConfig
%

%   Copyright 2022 Bose Corporation

  properties

  end
  methods
    %----------------------------------------------------------------------
    function this = OldSweepConfig()
        % OldSweepConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.CyclesPerStepc     = 10; % = cycles_per_step
        this.EndFreqHz          = 1000; % = end_freq_Hz
        this.FrameSizeSamples   = 32; % = sys_defs.frameSize_samples
        this.FsHz               = 44100; % = sys_defs.Fs_Hz
        this.NumChannel         = 1; % = Nch
        this.OldSweepOn         = 3; % = old_sweep_ON
        %this.SineTable  % = sys_defs.sineTable
        this.SineTableLength    = 1024; % = sys_defs.sineTableLength
        this.StartFreqHz        = 100; % = start_freq_Hz
        this.StartPhaseRad      = 0; % = start_phase_rad
        this.Steps              = 5; % = steps
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

    end

  end
end
