classdef PoolIirWithResetSharcPlusAccelGxpBreedConfig < BtcBreedBaseClass
% This is a just a placeholder of SharcPlusAccelGxpBreedConfig. This file
% needs to be updated whenever the block is merged to blocklib by Zeus team.

    % PoolIirWithResetSharcPlusAccelGxpBreedConfig Build-time configuration class for the PooliirGXPAccel breed
    % Usage:
    %    <automatically allocated by setting the "Breed" property of the
    %    containing object derived from BtcBaseClassWithBreeds
    %

    %   Copyright 2023 Bose Corporation

    properties(GetObservable, SetObservable)
        CodeWorkMemorySize              % Size of code work memory required for the breed
        WorkloadEstimate                % Estimate of HW workload for the accelerator task
        AccelDeviceId uint32    = 0     % IIR Accel device ID: 0, 1, 2, 3, 4, 5, 6, or 7
    end

    properties(SetAccess = private)
        CoreId uint32           = []
        PostOverlayRequired     = 1
        CacheLineLen            = 64    % ADI GXP size in bytes
        CodeWorkMemoryInitVal
    end

    methods
        %----------------------------------------------------------------------
        function this = PoolIirWithResetSharcPlusAccelGxpBreedConfig(parentObj)
            %.PoolIirWithResetSharcPlusAccelGxpBreedConfig constructor
            this@BtcBreedBaseClass(parentObj);
            this.Lock(@this.CodeWorkMemorySize,@()this.GetCodeWorkMemorySize);
            this.Lock(@this.WorkloadEstimate,@()this.GetWorkloadEstimate);
        end
        %----------------------------------------------------------------------
        function BreedValidate(this)
            % BreedValidate validates cross-property interactions. Our parent
            % object calls us during block mask initialization.
            BreedValidate@BtcBreedBaseClass(this);
        end
        %----------------------------------------------------------------------
        function BreedDoMaskInit(this,blockHandle)
            % BreedDoMaskInit performs automation of code generation settings
            % and other canvas interactions for a variant subsystem. Our parent
            % object calls us during block mask initialization.
            % Create alias for code work memory buffer for each instance of model
            blockDataStore = {'codeWorkMemory'};
            variantBlock = {'PoolIirWithReset/SharcPlusAccelGxp'};
            variantBlockPath = MaskUtil.SubBlockName(blockHandle, variantBlock);
            variantBlockHandle = getSimulinkBlockHandle(variantBlockPath);
            PooliirAccelMask(variantBlockHandle, blockDataStore);

            this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);

            % Discover and create accelerator device mapping to DSP core
            unitMapPropertyPath = ["Config" "BreedObj" "AccelDeviceId"];
            workloadEstimate = this.WorkloadEstimate;
            [~, this.CoreId] = ProcessUnitMap.AssignBlockToUnit(blockHandle, ...
                unitMapPropertyPath, 'iir', workloadEstimate);

            % Be friendly about the CRL this breed expects
            this.EnsureCodeReplacementLibrary(blockHandle,'SHARC+ Griffin Lite XP (GLXP)');

            BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
        end
        %----------------------------------------------------------------------
    end

    methods
        function SetAccelDeviceId(this, deviceId)
            this.AccelDeviceId = deviceId;
        end
        function value = GetCodeWorkMemorySize(this)
            % Accelerator CRL code work memory size is equal to:
            %             sizeof(iirAccelWorkMemory_t) + variableDataSize;
            % with variable data size dependent on number channels and
            % frame size.  NOTE: this must be updated to match the CRL code
            % if any changes are made to the accelWorkMemory_t structure.
            bufferSize = 68 + this.Parent.NumChannels * (64 + this.Parent.FrameSize * 4);
            variableDataSize = TargetCacheRoundUpSize(bufferSize, ...
                1, ...
                this.CacheLineLen); % bytes
            memSize = 64 + variableDataSize; % bytes
            value = memSize / 4; % words (single)
        end

        function value = GetWorkloadEstimate(this)
            % Estimate of HW accelerator MIPS equivalent usage for this
            % accelerator task

            % SJD: Add calculation.  This could also include a core MIPS
            %      estimate.  Will need sample rate property.
            value = 0;  % HW MIPS equivalent
        end
    end
end
