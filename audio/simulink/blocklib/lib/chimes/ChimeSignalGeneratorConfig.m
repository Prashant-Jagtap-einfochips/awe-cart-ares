classdef ChimeSignalGeneratorConfig < BtcBaseClass
% ChimeSignalGeneratorConfig Build-time configuration class for the ChimesSignalGenerator block
% Usage:
%    aConfig = ChimesSignalGeneratorConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    CyclesPerStep  % = chimeConfig.sigGen.cycles_per_step
    EndFreqHz  % = chimeConfig.sigGen.end_freq_Hz
    FrameSizeSamples  % = frameSize_samples
    FsHz  % = Fs_Hz
    RandomNoise  % = chimeConfig.sigGen.random_noise
    SelectMode  % = chimeConfig.sigGen.select_mode
    SineTableLength  % = sineTableLength
    StartFreqHz  % = chimeConfig.sigGen.start_freq_Hz
    StartPhaseRad  % = chimeConfig.sigGen.start_phase_rad
    SweepTimeSec  % = chimeConfig.sigGen.sweep_time_sec
    ToneGains  % = chimeConfig.sigGen.tone_gains
    
    % Added manually in BTC conversion - sine table used to come from
    % "other places"
    SineTableOffset = 1
    SineTable
  end
  methods
    %----------------------------------------------------------------------
    function this = ChimeSignalGeneratorConfig()
        % ChimesSignalGeneratorConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
        if (isempty(this.SineTable))
            this.SineTable = ...
                0.99999*sin(2*pi*((0:this.SineTableLength-1)/ ...
                this.SineTableLength - this.SineTableOffset/this.SineTableLength))';
        end
    end

  end
end
