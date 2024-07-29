classdef RifftWolaSharcPlusBreedConfig < BtcBreedBaseClass
% RifftWolaSharcPlusBreedConfig Build-time configuration class for the RifftWolaSharcPlus breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties(Dependent, SetAccess=private)
    % PropertyName size class {validation functions} = DefaultValue
    BufferAlias
  end
  properties(GetObservable, SetObservable, Hidden, SetAccess = private)
      DsmTwiddleRealInitValue uint32 = 0
      DsmTwiddleImagInitValue uint32 = 0
  end
  properties(GetObservable, SetObservable, Dependent, Hidden, SetAccess = private)
      DsmDataImagInitValue (:,1) single
      DsmDataRealInitValue (:,1) single
      DsmTempImagInitValue (:,1) single
      DsmTempRealInitValue (:,1) single
  end
  methods
    %----------------------------------------------------------------------
    function this = RifftWolaSharcPlusBreedConfig(constructingParentObj)
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
        if this.Parent.FftSize ~= 256 && this.Parent.FftSize ~= 128
            error('The current supported FFT sizes are 256 and 128 only.');
        end
        BreedValidate@BtcBreedBaseClass(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        % Set up storage classes for working memory
        this.ConfigureDataStoreMemory('TempReal', blockHandle, ['FFTMemL1DmWordAlign' num2str(this.Parent.FftSize)]);
        this.ConfigureDataStoreMemory('TempImag', blockHandle, ['FFTMemL1PmWordAlign' num2str(this.Parent.FftSize)]);
        this.ConfigureDataStoreMemory('DataReal', blockHandle, ['FFTMemL1DmWordAlign' num2str(this.Parent.FftSize)]);
        this.ConfigureDataStoreMemory('DataImag', blockHandle, ['FFTMemL1PmWordAlign' num2str(this.Parent.FftSize)]);
        
	    % Be friendly about the CRL this breed expects
        this.EnsureCodeReplacementLibrary(blockHandle, 'SHARC+ code replacement library');
        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------  
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh, MemorySection)
        % Get DSM block Path
        theDsm = ['RifftWola/' ...
            char(this.Parent.Breed) '/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);

        % Configure DSM Storage Class
        identifier = [this.BufferAlias DataStoreMemoryName];
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FFTBuffer', ...
                                'Identifier', identifier, ...
                                'MemorySection', MemorySection);
    end
    %----------------------------------------------------------------------
    function val = get.DsmDataImagInitValue(this)
        val =  zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------
    function val = get.DsmDataRealInitValue(this)
        val =  zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------
    function val = get.DsmTempImagInitValue(this)
        val =  zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------
    function val = get.DsmTempRealInitValue(this)
        val =  zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------

  end
end
