classdef RfftSharcPlus2ChanBreedConfig < BtcBreedBaseClass
% RfftSharcPlus2ChanBreedConfig Build-time configuration class for the Rfft2ChanSharcPlus breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties
    % PropertyName size class {validation functions} = DefaultValue
    
  end
  properties(SetObservable, GetObservable, Dependent, SetAccess = private)
    DsmTempRealInitValue (:,1) single {mustBeReal}
    DsmTempImagInitValue (:,1) single {mustBeReal}
  end
    properties(SetObservable, GetObservable, SetAccess = private)
    DsmTwiddleRealInitValue uint32 = 0
    DsmTwiddleImagInitValue uint32 = 0
  end
  properties(Dependent, SetObservable, GetObservable, SetAccess=private)
    BufferAlias
    Input1Name
    Input2Name
  end
  methods
    %----------------------------------------------------------------------
    function this = RfftSharcPlus2ChanBreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
        
    end
    %----------------------------------------------------------------------
    function value = get.BufferAlias(this)
        value = [char(this.Parent.Alias) 'RfftBuffer'];
    end
    %----------------------------------------------------------------------
    function value = get.Input1Name(this)
        value = [char(this.Parent.Alias) 'RfftInput1'];
    end
    %----------------------------------------------------------------------
    function value = get.Input2Name(this)
        value = [char(this.Parent.Alias) 'RfftInput2'];
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.

        if (this.Parent.FftSize ~= 256 && this.Parent.FftSize ~= 128)
            error("This breed of Rfft does not support FftSize of %d", this.Parent.FftSize);
        end
        BreedValidate@BtcBreedBaseClass(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        
        % Set up storage classes for working memory
        this.ConfigureDataStoreMemory('TempReal', blockHandle);
        this.ConfigureDataStoreMemory('TempImag', blockHandle);

        % Be friendly about the CRL this breed expects
        this.EnsureCodeReplacementLibrary(blockHandle, 'SHARC+ code replacement library');

        % Create Signal Object for Inputs
        this.ConfigureInputSignalObject(this.Input1Name, blockHandle, "SelectorChan1", 'FFTMemL1DmWordAlign');
        this.ConfigureInputSignalObject(this.Input2Name, blockHandle, "SelectorChan2", 'FFTMemL1PmWordAlign');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureInputSignalObject(this, InputSignalName, blockHandle, InportName, MemorySectionPrefix)
        InputBlock = MaskUtil.SubBlockName(blockHandle, ...
            ['Rfft' char(this.Parent.Breed) InportName]);
        CscUtil.SetOutportCsc(InputBlock, 1, 'Blocklib','FFTBuffer', ...
            'MemorySection', [MemorySectionPrefix num2str(this.Parent.FftSize)], ...
            'Identifier', InputSignalName);
    end
    %----------------------------------------------------------------------  
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['Rfft/' ...
            char(this.Parent.Breed) '/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);

        % Configure DSM Storage Class
        identifier = [this.BufferAlias DataStoreMemoryName];
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FFTBuffer', ...
                                'Identifier', identifier);
    end
    %----------------------------------------------------------------------
    function val = get.DsmTempRealInitValue(this)
        val = zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------  
    function val = get.DsmTempImagInitValue(this)
        val = zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------
  end
end
