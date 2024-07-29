classdef PoolDelayConfig < BtcBaseClassWithBreeds
% PoolDelayConfig Build-time configuration class for the PoolDelay block
% Usage:
%    aConfig = PoolDelayConfig
%
%   Copyright 2023 Bose Corporation

    properties (Constant,Hidden)
        BreedClassMap = dictionary( ...
            PoolDelayBreedEnum.Generic, "PoolDelayGenericBreedConfig", ... 
            PoolDelayBreedEnum.L1Memory, "PoolDelayL1MemoryBreedConfig", ... 
            PoolDelayBreedEnum.L2Memory, "PoolDelayL2MemoryBreedConfig"); 
    end

    properties(GetObservable, SetObservable)
        FrameSize  = 32
        MaxTotalDelay  = 512
        NumChannels  = 3
        Alias           = 'Default'
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolDelayConfig()
        % PoolDelayConfig constructor: assign component property objects
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