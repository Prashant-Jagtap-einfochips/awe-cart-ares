classdef SawtoothConfig < SignalGeneratorConfig
% SawtoothConfig Build-time configuration class for the Sawtooth block
% Usage:
%    aConfig = SawtoothConfig
%

%   Copyright 2022 Bose Corporation

  properties

  end
  methods
    %----------------------------------------------------------------------
    function this = SawtoothConfig()
        % SawtoothConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.DefaultTime         = 1; % = default_time
        this.DefaultTuneTopTime  = 0; % = tuneTop_time
        this.FrameSizeSamples    = 32; % = sys_defs.frameSize_samples
        this.FsHz                = 44100; % = sys_defs.Fs_Hz
        this.NumChannel          = 1; % = nch
        this.SawtoothModeOn      = 5; % = sawtooth_mode_ON
        this.StartFreqHz         = 240; % = start_frequencies
        this.StartPhaseRad       = 0; % = start_phase

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

    end

  end
end
