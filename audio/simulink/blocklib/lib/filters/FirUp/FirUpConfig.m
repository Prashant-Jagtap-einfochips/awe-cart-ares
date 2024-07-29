classdef FirUpConfig < BtcBaseClassWithBreeds
% Block Documentation For FirUp -------------------------------------------
% Upsampling followed by an Fir filter. The block is implemented using a 
% polyphase Fir. Fir filter length must be a multiple of the upsampling 
% factor. This block expects the the input "CoeffsTarget" to be translated 
% to the correct format for each hardware target. A seperate input 
% "CoeffsRaw" is the coefficients used in the simulation format. The same 
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
% -AudioOut: [FrameSize*FactorUp x NumChannels] The output audio after 
%   upsampling and filtering.
%--------------------------------------------------------------------------

  properties (Constant,Hidden)
    BreedClassMap = dictionary( ...
        AsdHwEnum.Simulink, "BtcBreedBaseClass", ...             % Simulink Fir Interpolation Block
        AsdHwEnum.Generic, "FirUpGenericBreedConfig", ...        % Can be configured to use FirGeneric CRL or FirUp CRL
        AsdHwEnum.Hexagon, "FirUpHexagonBreedConfig", ...        % Can be configured to use FirGeneric CRL or FirUp CRL
        AsdHwEnum.SharcPlusCore, "BtcBreedBaseClass", ... 
        AsdHwEnum.SharcPlusAccel, "BtcBreedBaseClass", ... 
        AsdHwEnum.ArmNeonV7V8, "BtcBreedBaseClass"); 
  end
  properties (GetObservable,SetObservable)
      NumCoeffs         (1,1) {mustBePositive, mustBeInteger} = 96     % Length of Fir Filter
      UpFactor          (1,1) {mustBePositive, mustBeInteger} = 4      % Upsampling Factor
      Alias             string = "" % Unique Alias for each instance of the block
      UseWithFirGeneric logical % Uses FirUp CRL instead of FirGeneric CRL if Breed is Generic or Hexagon. Other Breeds still use WithFir.
      NumChannels       {mustBeInteger} = 2
      FrameSizeIn       {mustBeInteger} = 8
  end
  properties (GetObservable,SetObservable, Hidden)
      Fir FirConfig
  end

  methods
    %----------------------------------------------------------------------
    function this = FirUpConfig()
        % FirUpConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
        this.Default(@this.UseWithFirGeneric, @() true);

        this.Fir = FirConfig();
        this.Default(@this.Fir.Alias, @() this.Alias);
        this.Lock(@this.Fir.NumOutputs, @() this.UpFactor*this.NumChannels);
        this.Lock(@this.Fir.NumInputs, @() this.NumChannels);
        this.Lock(@this.Fir.FrameSize, @() this.FrameSizeIn);
        this.Lock(@this.Fir.MaxAvgCoeffLength, @() this.GetMaxAvgCoeffLength());
        this.Lock(@this.Fir.BreedSelectionGoals, @() this.Breed);
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@BtcBaseClassWithBreeds(this);
        if this.UseWithFirGeneric == false
            if not(this.Breed == AsdHwEnum.Generic) && not(this.Breed == AsdHwEnum.Hexagon)
                error("FirUp Breed [%s] is not supported without FirGeneric. Property [UseWithFirGeneric] must be true.", string(this.Breed))
            end
        end
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        DoMaskInit@BtcBaseClassWithBreeds(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = GetMaxAvgCoeffLength(this)
        val = ceil(this.NumCoeffs/this.UpFactor);
    end
    %----------------------------------------------------------------------
  end
end
 