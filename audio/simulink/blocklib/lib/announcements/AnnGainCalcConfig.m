classdef AnnGainCalcConfig < BtcBaseClass
% AnnGainCalcConfig Build-time configuration class for the AnnGainCalc block
% Usage:
%    aConfig = AnnGainCalcConfig
%

%   Copyright 2022 Bose Corporation

  properties (GetObservable,SetObservable)
    Fd  % Decimated rate, i.e. Fs/L  e.g. 44100/32
    MinVncSignal  = 40
    NoiseOverride  = 0
    NoiseOverrideFlag  = 0
    RampTau  = 0.1
    VncBoostGain  = 1
    VncEnable  = 1
    
    % Validate will use NumAnnIn to auto-fill several other properties if
    % they are left empty.
    NumAnnIn = 1
    VncSlope  % If left empty, Validate will fill = 0.4.*ones(1,NumAnnIn)
    VncThresh  % If left empty, Validate will fill = zeros(1,NumAnnIn)
    BoostOverride  % If left empty, Validate will fill = zeros(1,NumAnnIn)
    BoostOverrideFlag  % If left empty, Validate will fill = zeros(1,NumAnnIn)
    MaxBoost  % If left empty, Validate will fill = 18.*ones(1,NumAnnIn)
    MaxSpl  % If left empty, Validate will fill = 120.*ones(1,NumAnnIn)

  end
  properties (Dependent)
      RampCoef
      RampWc
      RampSinCoef
      RampCosCoef      
  end

   
  methods
    %----------------------------------------------------------------------
    function this = AnnGainCalcConfig()
        % AnnGainCalcConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);

        % TODO: It seems likely that there are cross-property interactions
        % that should be checked here - for example the dimensions. If
        % something is not right however, perhaps the model will simply
        % return errors by itself.
    end

    %----------------------------------------------------------------------
    % These properties can be set, but also will automatically fill if left
    % empty... so set to [] to get auto-values, otherwise use normally
    function value = get.VncSlope(this)
        value = this.FillBlank(this.VncSlope, 0.4.*ones(1,this.NumAnnIn));
    end
    function value = get.VncThresh(this)
        value = this.FillBlank(this.VncThresh, zeros(1,this.NumAnnIn));
    end
    function value = get.BoostOverride(this)
        value = this.FillBlank(this.BoostOverride, zeros(1,this.NumAnnIn));
    end
    function value = get.BoostOverrideFlag(this)
        value = this.FillBlank(this.BoostOverrideFlag, zeros(1,this.NumAnnIn));
    end
    function value = get.MaxBoost(this)
        value = this.FillBlank(this.MaxBoost, 18.*ones(1,this.NumAnnIn));
    end
    function value = get.MaxSpl(this)
        value = this.FillBlank(this.MaxSpl, 120.*ones(1,this.NumAnnIn));
    end
    %----------------------------------------------------------------------
    % These Dependent properties cannot be set: calculated only
    function value = get.RampCoef(this)
        value = (1-this.RampSinCoef)/this.RampCosCoef;
    end
    function value = get.RampWc(this)
        value = 1/(this.RampTau*this.Fd);
    end
    function value = get.RampSinCoef(this)
        value = sin(this.RampWc);
    end
    function value = get.RampCosCoef(this)
        value = cos(this.RampWc);
    end
    
  end
  methods (Static, Access=private)
      function value = FillBlank(inValue,fillValue)
          % FillBlank method makes code more readable by placing the
          % property-name and the auto-fill value all on one line.
          if (isempty(inValue))
              value = fillValue;
          else
              value = inValue;
          end
      end
  end
end
