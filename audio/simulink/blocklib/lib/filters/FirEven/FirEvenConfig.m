classdef FirEvenConfig < BtcBaseClassWithBreeds
% FirEvenConfig Build-time configuration class for the FirEven block
% Usage:
%    aConfig = FirEvenConfig
%

%   Copyright 2023 Bose Corporation
  properties (Constant,Hidden)
    BreedClassMap = dictionary( ...
        AsdHwEnum.Generic, "BtcBreedBaseClass", ... 
        AsdHwEnum.Hexagon, "FirEvenHexagonBreedConfig", ... 
        AsdHwEnum.ArmNeonV7V8, "FirEvenArmNeonV7V8BreedConfig", ... 
        AsdHwEnum.SharcPlus, "BtcBreedBaseClass"); 
  end
  properties (GetObservable,SetObservable)
      Alias             string = "";
      MaxNumOfThreads   {mustBeInteger} = 1;
      NumOutputs        {mustBeInteger} = 2
      NumInputs         {mustBeInteger} = 2
      FrameSize         {mustBeInteger}= 32
      NumChannels       {mustBeInteger}= 2
      MaxAvgCoeffLength {mustBeInteger} = 32 
  end
  methods
    %----------------------------------------------------------------------
    function this = FirEvenConfig(optionalBreed)
        % FirEvenConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

        if (exist('optionalBreed','var'))
            % Perform direct assignment so provided string must make sense
            this.Breed = optionalBreed;
        end
        this.Lock(@this.NumInputs, @this.NumChannels);
        this.Lock(@this.NumOutputs, @this.NumChannels);

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        Validate@BtcBaseClassWithBreeds(this);
        if any(this.Breed == [AsdHwEnum.Hexagon, AsdHwEnum.ArmNeonV7V8]) && (mod(this.MaxAvgCoeffLength,4) ~= 0)
            error("FirEven is selected for %s Breed and property [MaxAvgCoeffLength] has an invalid value of %d. This property must be a multiple of 4.", string(this.Breed), this.MaxAvgCoeffLength);
        end
        if any(this.Breed == [AsdHwEnum.Hexagon, AsdHwEnum.ArmNeonV7V8]) && (this.MaxAvgCoeffLength < this.FrameSize)
            error("FirEven is selected for %s Breed and property [MaxAvgCoeffLength] has an invalid value of %d. This property must be a greater than property [FrameSize] of value %d.", string(this.Breed), this.MaxAvgCoeffLength, this.FrameSize);
        end
    end
    %----------------------------------------------------------------------
    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.
        DoMaskInit@BtcBaseClassWithBreeds(this,blockHandle);
    end
  end
end
