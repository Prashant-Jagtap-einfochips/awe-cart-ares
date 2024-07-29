classdef BandSplitterConfig < BtcBaseClass
% Blocklib BandSplitter ---------------------------------------------------
% This block splits the input signal into low frequency band and high
% frequency bands in such a way that allows for perfect reconstruction of
% the original signal. The low frequency output is also decimated while the
% high frequency remains at the full-rate. 
% 
% Inputs:
%   AudioIn: [FrameSize x NumChannels] Input audio to be processed
%   LowFreqOut: [Framesize/DecimationFactor x NumChannels] Decimated &
%       low-passed output
%   HighFreqOut: [FrameSize x NumChannels] High-passed and delayed output.
%
% Defaults Configuration: 
%   DecimationFactor = 4
%   Decimation Filter with passband of 3 kHz, stopband at 5512.5 Hz, and
%   less than -100 dB ripple in passband.
%
% Automate Filter Design
%   BandSplitterConfig has a class method that can automate calculation of
%   coefficients given ripple in dB and freqeuncy of passband edge. To use, 
%   >> coeffs = BandSplitterCfg.CalculateCoefficients(FreqPass, RippleDb)
%   >> coeffs = BandSplitterCfg.CalculateCoefficients(3000, 100)   
%--------------------------------------------------------------------------

  properties (GetObservable,SetObservable)
      FirUp             FirUpBtcConfig
      FirDown           FirDownBtcConfig
      NumChannels       {mustBeInteger} = 2
      FrameSizeIn       {mustBeInteger} = 32
      DecimationFactor  {mustBeInteger} = 4
      SamplingRate      = 44100
      CoeffsCascaded    (:,1)
  end
  properties (GetObservable, SetObservable, Dependent)
      Delay {mustBeInteger}
  end
  methods
    %----------------------------------------------------------------------
    function this = BandSplitterConfig()
        % BandSplitterConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        
        % FirDown
        this.FirDown = FirDownBtcConfig();
        this.FirDown.BreedSelectionGoals = "Simulink";
        this.Lock(@this.FirDown.DownFactor, @()this.DecimationFactor); 
        this.Lock(@this.FirDown.FrameSizeIn, @() this.FrameSizeIn);
        this.Lock(@this.FirDown.NumChannels, @ this.NumChannels);
        this.Default(@this.FirDown.CoeffsRaw, @() this.CalculateCoefficients(3000, 100));
        
        % FirUp
        this.FirUp = FirUpBtcConfig();
        this.FirUp.BreedSelectionGoals = "Simulink";
        this.Lock(@this.FirUp.UpFactor, @()this.DecimationFactor);
        this.Lock(@this.FirUp.FrameSizeIn, @() this.FrameSizeIn/this.DecimationFactor);
        this.Lock(@this.FirUp.NumChannels, @ this.NumChannels);
        this.Default(@this.FirUp.CoeffsRaw, @()this.DecimationFactor*this.FirDown.CoeffsRaw);
        this.FirUp.Alias = "BandSplitterFirUp";

        % Get coefficeints for cascaded interpolation and decimation fitler 
        this.Lock(@this.CoeffsCascaded, @() conv(this.FirDown.CoeffsRaw, this.FirUp.CoeffsRaw));
        
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@BtcBaseClass(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        DoMaskInit@BtcBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = get.Delay(this)
        NormalizedStopBandFreq = 1/2/this.DecimationFactor;
        GrpDelayResponse = double(grpdelay(this.CoeffsCascaded, 1, linspace(0,NormalizedStopBandFreq,1024)));
        GrpDelayAvg = round(mean(GrpDelayResponse));

        % Check if GroupDelay is integer
        if rms(GrpDelayResponse - GrpDelayAvg)^2 > 1E-5
            warning("Non-integer sample delay calculated for cascaded interpolation and decimation filters in BandSplitter block. Check for invalid coefficients. FirUp coefficients and FirDown Coefficients must both be odd length or both be even length.");
        end

        val = round(GrpDelayAvg);
        
        % Add Delay for FirUp
        if this.FirUp.Breed == AsdHwEnum.SharcPlusAccel
            val = val + this.FirUp.FrameSize*this.DecimationFactor;
        end

        % Add Delay for FirDown
        if this.FirDown.Breed == AsdHwEnum.SharcPlusAccel
            val = val + this.FirDown.FrameSize;
        end
    end
    %----------------------------------------------------------------------
    function Coeffs = CalculateCoefficients(this, FreqPass, RippleDb)
        % Specify filter properties
        Ripple = db2mag(-RippleDb);
        FreqStop = this.SamplingRate/2/this.DecimationFactor; 
        if FreqPass > FreqStop
            disp(['Passband frequency of ' num2str(FreqPass) ' must not be greater than stopband frequency of ' num2str(FreqStop)]);
            FreqPass = FreqStop - this.SamplingRate/10/2;
            disp(['Passband frequnecy has been reset to ' num2str(FreqPass) ', a value less than the stopband frequency']);
        end

        % Deign Filter Properties
        [FirOrder,FirWn,FirBeta,FirFiltType] =      ...
            kaiserord( [FreqPass FreqStop],         ... % band edge frequencies
                       [1 0],                       ... % amplitude in bands - LPF
                       [Ripple Ripple],             ... % ripple
                       this.SamplingRate); 

        % Get filter Coeffs
        Coeffs = fir1(  FirOrder,                           ... % filter order
                        FirWn,                              ... % cut-off frequency
                        FirFiltType,                        ... % filter type
                        kaiser(FirOrder+1,FirBeta),         ... % kaiser window with length (n+1) and
                                                            ... % shape parameter bta   
                        'noscale');
    end
    %----------------------------------------------------------------------
  end
end
