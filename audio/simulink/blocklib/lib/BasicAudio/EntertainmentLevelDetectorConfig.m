classdef EntertainmentLevelDetectorConfig < BtcBaseClass
% EntertainmentLevelDetectorConfig Build-time configuration class for the EntertainmentLevelDetector block
% Usage:
%    aConfig = EntertainmentLevelDetectorConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
      % Default values for TuneVars. These are adjusted by Fs/FrameSize
      % during Validate to provide the derived values actually used.
      % Unfortunately, the official TuneVar values are adjusted during
      % build-time configuration (in the legacy script, test, and in this
      % class) instead of placing the adjustments into tuning translations.
      % This current design should be refactored - but that would change
      % existing tunings and tool integrations.
    DefaultFastDecayDbSec  single  = 5.0 % db/sec 10
    DefaultSlowDecayDbSec  single	= 0.1 % db/sec
    DefaultHoldTimeSec     single  = 3.0 % sec 4.0
    DefaultHoldMargin      single  = 6.0 % db (same as ld_hold_margin)
    DefaultMinVolDpSpl     single  = -80.0 % dBSPL
    DefaultMaxVolDbSpl     single  = 0.0 % dBSPL

    % THIS MIGHT NOT BE USED...
    MaxSpl          = 123 % dBSPL 

    % these came originally from sys_defs
    Fs = 44100
    FrameSize = 32
  end
 
  properties (SetAccess=private)
      % These are computed during Validate from the properties above
    DlgHoldTime    single    %= ceil(DefaultHoldTimeSec * DlgDecRate);
    DlgSlowDecay   single    %= -abs(DefaultSlowDecayDbSec / DlgDecRate);
    DlgFastDecay   single    %= -abs(DefaultFastDecayDbSec / DlgDecRate);
    DlgDecRate     single    %= sys_defs.Fs / sys_defs.FrameSize;
    
    % Alignment of FeaturePath & ConfigPath causes the below names which
    % are backwards-compatible with the FeaturePaths found in the design.
    % These, of course, do not meet convention
    level_detect_preemphasis_lpf_ PoolIirExternalTranslationConfig
    level_detect_preemphasis_hpf_ PoolIirExternalTranslationConfig    
    
% [DELETEME Legacy] To keep using discrete parameters in legacy Pooliir mask
%     DlgBsLpCoeffs      %= [pooliirLpCoeffs; pooliirLpCoeffs];
%     DlgBsHpCoeffs      %= [pooliirHpCoeffs; pooliirHpCoeffs];
%     DlgBsNumStages     %= [numSOSStages; numSOSStages];
%     DlgBsMaxNumStages  %= sum(DlgBsNumStages);
  end

  methods
    %----------------------------------------------------------------------
    function this = EntertainmentLevelDetectorConfig()
        % EntertainmentLevelDetectorConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        this.DlgDecRate = this.Fs / this.FrameSize;
        
        % Calculate derivative settings - note that in the legacy interface
        % these are actually the settings used, not the undecimated ones
        this.DlgHoldTime  = ceil(this.DefaultHoldTimeSec * this.DlgDecRate);
        this.DlgSlowDecay = -abs(this.DefaultSlowDecayDbSec / this.DlgDecRate);
        this.DlgFastDecay = -abs(this.DefaultFastDecayDbSec / this.DlgDecRate);
        
        % Band splitter
        order = 5;
        fc = 100;

        wc = fc / (this.Fs / 2);
        numSOSStages = ceil(order / 2);

        [z, p, k] = butter(order, wc);
        sosLp = zp2sos(z, p, k);
        pooliirLpCoeffs = sos2pooliir(sosLp);

        [z, p, k] = butter(order, wc, 'high');
        sosHp = zp2sos(z, p, k);
        pooliirHpCoeffs = sos2pooliir(sosHp);
        
        % Pool allocation
        numStages = [numSOSStages; numSOSStages];
        maxNumStages = sum(numStages);
        
        % Configure low pass
        lowPass = PoolIirExternalTranslationConfig;
        lowPass.MaxNumStages = maxNumStages;
        lowPass.DefaultNumStages = numStages;
        lowPass.DefaultSosCoefficients = [pooliirLpCoeffs; pooliirLpCoeffs];
        
        % Configure high pass
        highPass = PoolIirExternalTranslationConfig;
        highPass.MaxNumStages = maxNumStages;
        highPass.DefaultNumStages = numStages;
        highPass.DefaultSosCoefficients = [pooliirHpCoeffs; pooliirHpCoeffs];
        
        % Drop the filter configurations into their component properties
        this.level_detect_preemphasis_lpf_ = lowPass;
        this.level_detect_preemphasis_hpf_ = highPass;    
        
% [DELETEME Legacy] To keep using discrete parameters in legacy Pooliir mask
%         this.DlgBsLpCoeffs = [pooliirLpCoeffs; pooliirLpCoeffs];
%         this.DlgBsHpCoeffs = [pooliirHpCoeffs; pooliirHpCoeffs];
%         this.DlgBsNumStages = [numSOSStages; numSOSStages];
%         this.DlgBsMaxNumStages = sum(this.DlgBsNumStages);
    end
    
    % These are exposed to support the *legacy* interface which used
    % scripts to adjust the "intended" properties above. The legacy test
    % cases hack in design-time values into the test model anyway - but
    % leaving these apis in respects the legacy design. These are used in
    % the legacy mask block which now acquires the "LD" struct variable via
    % added mask parameters.
    % -------- Note that the Fs and FrameSize must be set first before
    % using these. They reverse the logic to compute the non-computed human
    % values.
    function LegacyHoldTime(this,value)  %newval= ceil(DefaultHoldTimeSec * DlgDecRate);
        % since there was a ceil involved, this back-computation may not
        % match exactly the default value fed in to the legacy init script
        decRate = this.Fs / this.FrameSize;
        this.DefaultHoldTimeSec = value/decRate;
    end
    function LegacySlowDecay(this,value)  %newval= -abs(DefaultSlowDecayDbSec / DlgDecRate);
        decRate = this.Fs / this.FrameSize;
        this.DefaultSlowDecayDbSec = abs(value * decRate);
    end
    function LegacyFastDecay(this,value)   %newval= -abs(DefaultFastDecayDbSec / DlgDecRate);
        decRate = this.Fs / this.FrameSize;
        this.DefaultFastDecayDbSec = abs(value * decRate);
    end
  end
end
