classdef InportBuildTimeConfig <  BtcBaseClass
% InportBTConfig Class definition to contain build-time configuration for
% InportBT. The InportBT is just a Simulink inport block with signal
% attributes driven by build-time configuration.
% Usage:
%    aConfig = InportBTConfig
%

%   Copyright 2020 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB

    DataType = 'single'
    FrameSamples = 32
    ChannelCount = 2
    SampleRateInHertz = 44100
  end

  properties (Dependent)
      Dimensions
      SampleTime
  end
  methods
      function value = get.Dimensions(this)
          value = [this.FrameSamples, this.ChannelCount];
      end
      function value = get.SampleTime(this)
          value = this.FrameSamples / this.SampleRateInHertz;
      end
  end
  
end