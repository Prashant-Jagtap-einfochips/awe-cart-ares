classdef RifftWolaHexagonBreedConfig < BtcBreedBaseClass
% RifftWolaHexagonBreedConfig Build-time configuration class for the RifftWolaHexagon breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties(Dependent, SetAccess = private)
    % PropertyName size class {validation functions} = DefaultValue
    BufferAlias
  end
  properties(GetObservable, SetObservable, Dependent, Hidden, SetAccess = private)
    DsmTwiddle1InitValue (:,1) single
    DsmTwiddle2InitValue (:,1) single
    DsmRifftOutputTmpInitValue (:,1) single
  end
  methods
    %----------------------------------------------------------------------
    function this = RifftWolaHexagonBreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function value = get.BufferAlias(this)
        value = [char(this.Parent.Alias) 'RifftBuffer'];
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
        BreedValidate@BtcBreedBaseClass(this);
        if (this.Parent.FftSize ~= 256 && this.Parent.FftSize ~= 128)
            error("This breed of Rifft does not support FftSize of %d", this.Parent.FftSize);
        end
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        
        % Set up storage classes for working memory
        this.ConfigureDataStoreMemory('Twiddle1',  blockHandle);
        this.ConfigureDataStoreMemory('Twiddle2',  blockHandle);
        this.ConfigureDataStoreMemory('RifftOutputTmp',  blockHandle);

        % Be friendly about the CRL this breed expects
        this.EnsureCodeReplacementLibrary(blockHandle,'Qualcomm Hexagon DSP code replacement library');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------  
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['RifftWola/' ...
            char(this.Parent.Breed) '/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);

        % Configure DSM Storage Class
        identifier = [this.BufferAlias DataStoreMemoryName];
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FFTBuffer', ...
                                'Identifier', identifier, ...
                                'MemorySection','AttributeAligned8');
    end
    %----------------------------------------------------------------------
    function val = get.DsmRifftOutputTmpInitValue(this)
        val = single(complex(zeros(this.Parent.FftSize,1)));
    end
    %----------------------------------------------------------------------
    function val = get.DsmTwiddle1InitValue(this)
        val = single(complex(zeros(3*this.Parent.FftSize/8,1)));
    end
    %----------------------------------------------------------------------
    function val = get.DsmTwiddle2InitValue(this)
        val = single(complex(zeros(this.Parent.FftSize/2,1)));
    end
    %----------------------------------------------------------------------
  end
end
