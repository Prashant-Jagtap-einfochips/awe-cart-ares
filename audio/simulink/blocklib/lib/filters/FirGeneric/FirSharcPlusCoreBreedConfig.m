classdef FirSharcPlusCoreBreedConfig < BtcBreedBaseClass
% FirSharcPlusCoreBreedConfig Build-time configuration class for the FirSharcPlusCore variant
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
    function this = FirSharcPlusCoreBreedConfig(constructingParentObj)
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
        
        % Be friendly about the CRL this variant expects
        this.EnsureCodeReplacementLibrary(blockHandle,'SHARC+');

        % Assign Unique Identifier to StateBuffer
        this.ConfigureDataStoreMemory(blockHandle, 'StateBuffer');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = get.CodeWorkMemorySize(this)
        % 6 variable size arrays (for input, coeffs, states, 
        % stateSharingEnabled, taps array per output channel,
        % and mapped audio input offsets) 
        % + 10 struct members
        val = this.Parent.NumOutputs * 6 + 10;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize)*this.Parent.NumInputs;
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, blkh, DataStoreMemoryName)
        % Get DSM block Path
        theDsm = ['Fir/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);
        % Configure DSM Storage Class
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FirGenericStorage', ...
                                'Identifier', Identifier);
    end
    %----------------------------------------------------------------------
  end
end
