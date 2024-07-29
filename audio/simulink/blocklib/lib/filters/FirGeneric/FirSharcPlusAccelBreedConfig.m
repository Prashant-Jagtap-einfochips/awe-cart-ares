classdef FirSharcPlusAccelBreedConfig < BtcBreedBaseClass
% FirSharcPlusAccelBreedConfig Build-time configuration class for the FirSharcPlusAccel variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties
    % PropertyName size class {validation functions} = DefaultValue

  end

  properties (Dependent, SetAccess = private)
    CodeWorkMemorySize
    StateBufferSize
  end

  methods
    %----------------------------------------------------------------------
    function this = FirSharcPlusAccelBreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
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
        
        % Set Custom Storage Class options for data stores and input buffer
        blockDataStores = {'CodeWorkMemory', 'StateBuffer'};
        blockMemorySections = {'MemL1', 'MemSegment'};
        variantBlock = {'Fir/TargetSpecific/'};
        variantBlockPath = MaskUtil.SubBlockName(blockHandle, variantBlock);
        variantBlockHandle = getSimulinkBlockHandle(variantBlockPath);
        FirAccelMask(variantBlockHandle, blockDataStores, blockMemorySections, this.Parent.Alias);

        % Be friendly about the CRL this variant expects
        this.EnsureCodeReplacementLibrary(blockHandle,'SHARC+');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = get.CodeWorkMemorySize(this)
        % Accelerator CRL code work memory size is equal to:
        %             sizeof(accelWorkMemory_t) + variableDataSize;
        % with variable data size dependent on number inputs, number outputs
        % and frame size.  NOTE: this must be updated to match the CRL code
        % if any changes are made to the accelWorkMemory_t structure.
        val = 156 + (24 + this.Parent.NumInputs*48 + this.Parent.NumOutputs*(80 + 4*this.Parent.FrameSize));
        val = val / 4;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize - 1)*this.Parent.NumInputs;
    end
    %----------------------------------------------------------------------
  end
end
