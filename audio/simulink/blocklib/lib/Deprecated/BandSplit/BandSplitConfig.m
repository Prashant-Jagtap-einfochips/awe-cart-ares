classdef BandSplitConfig < BtcBaseClass
% BandsplitConfig Build-time configuration class for the Bandsplit block
% Usage:
%    aConfig = BandsplitConfig
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
        SampleRate = 44100
        FrameSize = 32
        ChannelCount = 4

        %LPF parameters
        CrossoverFrequency = 3000
        InterpolatedFilter
        DecimationFactor = 4
        Fireven
        Upfir
        BandSeparationDelay = 100
		
        Ifir IfirConfig
        FirEven FirEvenConfig
  end
  
methods
    %----------------------------------------------------------------------
    function this = BandSplitConfig()
        % BandsplitConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.FirEven = FirEvenConfig;
		this.FirEven.FrameSize = this.FrameSize;
        this.FirEven.MaxAvgCoeffLength = 1024; 
        this.FirEven.NumChannels = this.ChannelCount;

        this.Ifir =  IfirConfig;
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok

        %.If there are no cross-property interactions, then this entire
        %.method can be deleted!

        %.End with a call to our base class to continue validation checks
        %.including any violations of property locking.
        Validate@BtcBaseClass(this);
    end
end
end
