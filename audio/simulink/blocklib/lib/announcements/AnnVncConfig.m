classdef AnnVncConfig < BtcBaseClass
% AnnVncConfig Build-time configuration class for the AnnVnc block
% Usage:
%    aConfig = AnnVncConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    FrameSize
    LowPassSamplingHz  = 48000
    LowPassSmoothingHz  = 20
  end
  properties (SetAccess=private)
      % These properties are calculated by butterworth filter at Validate
    Den
    Num
    ZInit
  end
  methods
    %----------------------------------------------------------------------
    function this = AnnVncConfig()
        % AnnVncConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % Calculate butterworth filter
        [this.Num, this.Den]= butter(1,2*this.LowPassSmoothingHz/this.LowPassSamplingHz);
        this.ZInit=(this.Num(2)-this.Den(2));
    end
  end
end
