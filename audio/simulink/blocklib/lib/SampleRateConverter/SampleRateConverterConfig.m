classdef SampleRateConverterConfig < BtcBaseClass
% SampleRateConverterConfig Build-time configuration class for the SampleRateConverter block
% Usage:
%    aConfig = SampleRateConverterConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    Upfactor  = 12
    Downfactor  = 13
    OutFrameSize  = -1
    
    % Filter Design Parameters:    
    
    RateSelector RateSelectorEnum = 'InputSampleRate' % Known Sample Rate
    Fs                  = 48000     % Sample Rate
    FPassband           = 16500     % Passband Frequency
    FHearing            = 19000     % Hearing Threshold
    PassbandRipple      = 60        % Passband Ripple (dB)
    StopbandAttenuation = 60        % Stopband Attenuation (dB)
  end
  methods
    %----------------------------------------------------------------------
    function this = SampleRateConverterConfig()
        % SampleRateConverterConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

  end
end
