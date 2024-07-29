classdef RgainzConfig < BtcBaseClass
% RgainzConfig Build-time configuration class for the Rgainz block
% Usage:
%    aConfig = RgainzConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize  % = frameSize_samples
    InitialLinearGains  % = gain
    NumChannels  % = sysin_channel_count
    NumRampers  % = sysin_channel_count
    SampleRateInHertz  % = Fs_Hz

  end
  methods
    %----------------------------------------------------------------------
    function this = RgainzConfig()
        % RgainzConfig constructor: assign component property objects
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
