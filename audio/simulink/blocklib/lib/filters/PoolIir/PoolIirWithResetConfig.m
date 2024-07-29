classdef PoolIirWithResetConfig < BtcBaseClassWithBreeds
    % Blocklib PoolIir-----------------------------------------------------
    %   Inputs:
    %       AudioIn         [FrameSize x NumChannels] Input Signal
    %       NumStages       [NumChannels x 1] Number of Sos Stages per channel
    %       CoeffsTarget    [CoeffLength(Target) x 1] Total set of 
    %                           coefficients translated to target specific 
    %                           format from RawSos    
    %----------------------------------------------------------------------
    %   Copyright 2023 Bose Corporation

    properties (Constant,Hidden)
        BreedClassMap = dictionary( ...
            AsdHwEnum.Generic, "PoolIirWithResetGenericBreedConfig", ... 
            AsdHwEnum.SharcPlusCore, "PoolIirWithResetSharcPlusCoreBreedConfig", ... 
            AsdHwEnum.SharcPlusAccelGul, "PoolIirWithResetSharcPlusAccelGulBreedConfig", ... 
            AsdHwEnum.SharcPlusAccelGxp, "PoolIirWithResetSharcPlusAccelGxpBreedConfig", ... 
            AsdHwEnum.Hexagon, "PoolIirWithResetHexagonBreedConfig", ... 
            AsdHwEnum.ArmNeonV7V8, "PoolIirWithResetArmNeonV7V8BreedConfig"); 
    end
    properties(GetObservable, SetObservable)
        FrameSize       {mustBeInteger} = 32
        MaxNumStages    {mustBeInteger} = 80
        NumChannels     {mustBeInteger} = 4
        Alias           string = ""
    end
    methods
        %----------------------------------------------------------------------
        function this = PoolIirWithResetConfig()
            % PoolIirWithResetConfig constructor: assign component property objects
            % and adjust component property defaults and/or lock values

        end
        %----------------------------------------------------------------------
        function Validate(this)
            % Validate called from mask initialization to check settings ok

            %.End with a call to our base class to continue validation checks
            %.including any violations of property locking.
            Validate@BtcBaseClassWithBreeds(this);
        end
        %----------------------------------------------------------------------
        function DoMaskInit(this,blockHandle)
            % DoMaskInit called from mask initialization to perform automation
            % of code generation settings and other canvas interactions.

            %.Call base class in case it has any work to perform:
            DoMaskInit@BtcBaseClassWithBreeds(this,blockHandle);
        end

    end
end
