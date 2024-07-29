classdef WhiteNoiseGenConfig < SignalGeneratorConfig
% WhiteNoiseGenConfig Build-time configuration class for the WhiteNoiseGen block
% Usage:
%    aConfig = WhiteNoiseGenConfig
%

%   Copyright 2022 Bose Corporation

  properties

  end
  methods
    %----------------------------------------------------------------------
    function this = WhiteNoiseGenConfig()
        % WhiteNoiseGenConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

        this.FrameSizeSamples    = 32; % = sys_defs.frameSize_samples
        this.FsHz                = 44100; % = sys_defs.Fs_Hz
        this.NumChannel          = 1; % = nch
        this.RandomNoise         = 0.5; % = random_noise
        this.RandomNoiseOn       = 4;  % = random_noise_ON
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

    end

  end
end
