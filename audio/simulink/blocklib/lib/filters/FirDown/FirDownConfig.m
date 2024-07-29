classdef FirDownConfig < BtcBaseClassWithBreeds
% FirDownConfig Build-time configuration class for the FirDown block
% Block Documentation For FirDown -------------------------------------------
% Downsampling followed by an Fir filter. The block is implemented using a 
% polyphase Fir. This block expects input "CoeffsTarget" to be translated 
% to the correct format for each hardware target. The same 
% Fir filter is applied to each channel.
% 
% Inputs:
% -AudioIn: [FrameSize x  NumChannels] The input audio signal
% -CoeffsTarget: [NumCoeffs x 1] 1-D array of the Fir filter coefficients. 
%   These must be translated into the format expected by the hardware 
%   target. For Simulink and Simuation Breeds, the CoeffsTarget should be
%   equal to the CoeffsRaw.
% -CoeffsRaw: [NumCoeffs x 1] 1-D array of the Fir filter coefficients used
%   for simulation. For filter h[n] of length N, 
%   CoeffsRaw = [ h[0]; h[1]; ...... h[N-1] ]
% 
% Outputs:
% -AudioOut: [FrameSize*FactorDown x NumChannels] The output audio after 
%   upsampling and filtering.
%--------------------------------------------------------------------------

  properties (Constant,Hidden)
    BreedClassMap = dictionary( ...
        AsdHwEnum.Simulink, "BtcBreedBaseClass", ...             % Simulink Fir Interpolation Block
        AsdHwEnum.Generic, "BtcBreedBaseClass", ...  
        AsdHwEnum.SharcPlusCore, "BtcBreedBaseClass", ... 
        AsdHwEnum.SharcPlusAccel, "BtcBreedBaseClass", ... 
        AsdHwEnum.Hexagon, "BtcBreedBaseClass", ... 
        AsdHwEnum.ArmNeonV7V8, "BtcBreedBaseClass"); 
  end
  properties (GetObservable,SetObservable)
      NumCoeffs     (1,1) {mustBePositive, mustBeInteger} = 96   % Length of Fir Filter
      DownFactor    (1,1) {mustBePositive, mustBeInteger} = 4    % Downsampling Factor
      Alias         string = "";      
      NumChannels   {mustBeInteger} = 2
      FrameSizeIn   {mustBeInteger} = 32
  end
  properties (GetObservable,SetObservable, Hidden)
      Fir FirConfig
  end
  methods
    %----------------------------------------------------------------------
    function this = FirDownConfig()
        % FirDownConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.Fir = FirConfig();
        this.Default(@this.Fir.Alias, @() this.Alias);
        this.Lock(@this.Fir.NumOutputs, @() this.DownFactor*this.NumChannels);
        this.Lock(@this.Fir.NumInputs, @() this.DownFactor*this.NumChannels);
        this.Lock(@this.Fir.FrameSize, @() this.FrameSizeIn/this.DownFactor);
        this.Lock(@this.Fir.MaxAvgCoeffLength, @() this.GetMaxAvgCoeffLength());
        this.Lock(@this.Fir.BreedSelectionGoals, @() this.Breed);
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@BtcBaseClassWithBreeds(this);
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        DoMaskInit@BtcBaseClassWithBreeds(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = GetMaxAvgCoeffLength(this)
        val = ceil(this.NumCoeffs/this.DownFactor);
    end
    %----------------------------------------------------------------------
  end
end
