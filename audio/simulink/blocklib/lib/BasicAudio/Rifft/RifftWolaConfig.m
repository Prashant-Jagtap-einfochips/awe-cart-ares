classdef RifftWolaConfig < BtcBaseClassWithBreeds
% RifftWola----------------------------------------------------------------
% This block computes the Inverse Fourier Transform using an optimized IFFT
% for complex half-spectra, followed by in-place windowing and overlap-add. 
% 
% Inputs: 
%   X[nL,k]: [(FftSize/2 +1) x NumChannels] Multi-Channel complex half-
%       spectra
%   win: [FftSize, 1] Window Coefficients
% Outputs:
%   x[n]: [FftSize, NumChannels] Real valued time domain output signal
%
% The SharcPlus and Hexagon breed only supports 128-point or 256-point 
% IFFT.
% The SharcPlus breed only supports even number of channels
%--------------------------------------------------------------------------
%

%   Copyright 2022 Bose Corporation
  properties (Constant,Hidden)
    BreedClassMap = dictionary( ...
        AsdHwEnum.Generic, "BtcBreedBaseClass", ... 
        AsdHwEnum.SharcPlus, "RifftWolaSharcPlusBreedConfig", ... 
        AsdHwEnum.Hexagon, "RifftWolaHexagonBreedConfig", ... 
        AsdHwEnum.ArmNeonV7V8, "RifftWolaArmNeonV7V8BreedConfig"); 
  end
  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
    
    FftSize     {mustBeInteger} = 256
    NumOverlap  {mustBeInteger} = 128    % Amount of overlap between frames in samples
    Alias = 'Default'

  end

  methods
    %----------------------------------------------------------------------
    function this = RifftWolaConfig()
        % RifftWindowOverlapAddConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values

    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Validate called from mask initialization to check settings ok
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClassWithBreeds(this);

        % If there are no cross-property interactions, then this entire
        % method can be deleted!
    end

  end
end
