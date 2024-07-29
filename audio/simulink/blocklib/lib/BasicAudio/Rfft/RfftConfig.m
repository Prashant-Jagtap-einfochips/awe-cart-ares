classdef RfftConfig < BtcBaseClassWithBreeds
% Blocklib Rfft------------------------------------------------------------
% This block computes the Fourier Transform using an optimized FFT for real
% multi-channel inputs.
% Inputs:
%   x[n]: [FftSize, NumChannels] Real valued time domain input signal
% Outputs: 
%   X[nL,k]: [(FftSize/2 +1) x NumChannels] Multi-Channel complex half-
%       spectra of the input
% The SharcPlus and Hexagon breed currently only supports an even number 
% of channels and an FFT size of 128 or 256.
%--------------------------------------------------------------------------

    %   Copyright 2022 Bose Corporation
    properties (Constant,Hidden)
        BreedClassMap = dictionary( ...
            AsdHwEnum.Generic, "BtcBreedBaseClass", ... 
            AsdHwEnum.SharcPlus, "RfftSharcPlusBreedConfig", ... 
            AsdHwEnum.Hexagon, "RfftHexagonBreedConfig", ... 
            AsdHwEnum.ArmNeonV7V8, "RfftArmNeonV7V8BreedConfig"); 
    end
    properties (GetObservable,SetObservable)
        % Properties use this syntax:
        %    PropertyName size class {validation functions} = DefaultValue
        % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
        
        FftSize {mustBeInteger, mustBePositive} = 256
        Alias = 'Default'
        NumChannels {mustBeInteger} = 2
    end
    methods
        %----------------------------------------------------------------------
        function this = RfftConfig(Breed)
            % RfftConfig constructor: assign component property objects
            % and adjust component property defaults and/or lock values
                
            if exist('Breed','var')
                this.SelectBreed(Breed);
            end
        end
        %----------------------------------------------------------------------
        function Validate(this)
            % Validate called from mask initialization to check settings ok
            % Call base class and check for any cross-property interactions
            Validate@BtcBaseClassWithBreeds(this);
        
            % If there are no cross-property interactions, then this entire
            % method can be deleted!
        end
        %----------------------------------------------------------------------
    end
end
