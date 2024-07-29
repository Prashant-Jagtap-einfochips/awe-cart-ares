classdef PoolIirWithResetArmNeonV7V8BreedConfig < BtcBreedBaseClass
% PoolIirWithResetArmNeonV7V8BreedConfig Build-time configuration class for the PoolIirArmNeonV7V8 variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties(GetObservable, SetObservable, SetAccess = private)
      PostOverlayRequired     = 1
  end
  properties(GetObservable, SetObservable)
      DsmStatesInitVal  (:,1)
  end
  properties(GetObservable, SetObservable, Hidden)
      % Ignore this property. Only exists to allow ARM and Hexagon variant
      % to share the same canvas space.
      MaxNumOfThreads = 0
  end
  methods
    %----------------------------------------------------------------------
    function this = PoolIirWithResetArmNeonV7V8BreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
        this.Lock(@this.DsmStatesInitVal, ...
            @() zeros(2*constructingParentObj.MaxNumStages*constructingParentObj.NumChannels + 16, 1));
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
        this.EnsureCodeReplacementLibrary(blockHandle,'ARM');
        
        this.ConfigureDataStoreMemory('states', blockHandle);

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['PoolIirWithReset/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);

        % Configure DSM Storage Class
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'PooliirAccelStorage', ...
                                'MemorySection','AttributeAligned8',...
                                'Identifier', Identifier);
    end
    %----------------------------------------------------------------------

  end
end
