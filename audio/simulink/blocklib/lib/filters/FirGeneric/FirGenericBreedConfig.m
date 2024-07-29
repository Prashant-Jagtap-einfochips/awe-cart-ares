classdef FirGenericBreedConfig < BtcBreedBaseClass
% FirGenericBreedConfig Build-time configuration class for the FirGeneric variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties
    % PropertyName size class {validation functions} = DefaultValue

  end
  properties (GetObservable, SetObservable, Dependent, SetAccess = private)
      StateBufferSize
  end
  methods
    %----------------------------------------------------------------------
    function this = FirGenericBreedConfig(constructingParentObj)
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
        
        this.ConfigureDataStoreMemory('StateBuffer',blockHandle);
        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);

    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['Fir/' ...
            char(this.Parent.Breed) '/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        DSMBlockHandle = getSimulinkBlockHandle(DSMBlockPath);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);
        FullHeaderFilename = ...
               [getfullname(bdroot(DSMBlockHandle)) '_FirGenericStorage.h'];
        FullDefinitionFilename = ...
               [getfullname(bdroot(DSMBlockHandle)) '_FirGenericStorage.c'];
        % Configure DSM Storage Class
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FirGenericStorage', ...
                                'MemorySection','Default',...
                                'HeaderFile', FullHeaderFilename, ...
                                'DefinitionFile', FullDefinitionFilename, ...
                                'Identifier', Identifier);
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = this.Parent.StateBufferSizeSimulation;
    end
    %----------------------------------------------------------------------
  end
end
