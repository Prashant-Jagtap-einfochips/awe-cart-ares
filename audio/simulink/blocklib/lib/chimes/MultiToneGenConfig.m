classdef MultiToneGenConfig < SignalGeneratorConfig
% MultiToneGenConfig Build-time configuration class for the MultiToneGen block
% Usage:
%    aConfig = MultiToneGenConfig
%

%   Copyright 2022 Bose Corporation

  properties

  end
  methods
    %----------------------------------------------------------------------
    function this = MultiToneGenConfig()
        % MultiToneGenConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.FrameSizeSamples       = 32;  % = sys_defs.frameSize_samples
        this.FsHz                   = 44100;  % = sys_defs.Fs_Hz
        this.MultitoneChannelMap    = 3;  % = multitone_channel_map
        this.MultitoneOn            = 6;  % = multitone_ON
        this.NumMultitoneChannels   = 1;  % = Num_Multitone_channels
        this.NumTones               = 3;  % = NumTones
        %this.SineTable;  % = sineTable
        this.SineTableLength        = 1024;  % = sys_defs.sineTableLength
        this.StartPhaseRad          = [0 pi/2 pi ];  % = start_phase_rad
        this.ToneFreqHz             = [100 1000 10000];  % = Tone_freq_Hz
        %this.ToneGains              % = ToneGains
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

    end

  end
end
