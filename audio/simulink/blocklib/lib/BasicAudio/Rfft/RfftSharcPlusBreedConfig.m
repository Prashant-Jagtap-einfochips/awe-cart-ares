classdef RfftSharcPlusBreedConfig < BtcBreedBaseClass
% RfftSharcPlusBreedConfig Build-time configuration class for the RfftSharcPlus breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation
  properties(Dependent, SetAccess = private)
    % PropertyName size class {validation functions} = DefaultValue
    BufferAlias
  end
  properties(SetObservable, GetObservable, Dependent, Hidden, SetAccess = private)
    DsmDataRealInitValue (:,1) single {mustBeReal}
    DsmDataImagInitValue (:,1) single {mustBeReal}
    DsmTempRealInitValue (:,1) single {mustBeReal}
    DsmTempImagInitValue (:,1) single {mustBeReal}
    Input1Name
    Input2Name
  end
  properties(SetObservable, GetObservable, Hidden, SetAccess = private)
    DsmTwiddleRealInitValue uint32 = 0
    DsmTwiddleImagInitValue uint32 = 0
  end
  methods
    %----------------------------------------------------------------------
    function this = RfftSharcPlusBreedConfig(constructingParentObj)
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
            error("This variant of Rfft does not support FftSize of %d", this.Parent.FftSize);
        end
        BreedValidate@BtcBreedBaseClass(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        
        % Set up storage classes for working memory
        this.ConfigureDataStoreMemory('TempReal', blockHandle, 'Default');
        this.ConfigureDataStoreMemory('TempImag', blockHandle, 'Default');
        if (this.Parent.NumChannels == 2)
            % Create Signal Object for Inputs
            this.ConfigureRfft2ChanInputSignalObject(this.Input1Name, blockHandle, "SelectorChan1", 'FFTMemL1DmWordAlign');
            this.ConfigureRfft2ChanInputSignalObject(this.Input2Name, blockHandle, "SelectorChan2", 'FFTMemL1PmWordAlign');
        else
            this.ConfigureDataStoreMemory('DataReal', blockHandle, ['FFTMemL1DmWordAlign' num2str(this.Parent.FftSize)]);
            this.ConfigureDataStoreMemory('DataImag', blockHandle, ['FFTMemL1PmWordAlign' num2str(this.Parent.FftSize)]);
        end
        % Be friendly about the CRL this breed expects
        this.EnsureCodeReplacementLibrary(blockHandle, 'SHARC+ code replacement library');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end

    %----------------------------------------------------------------------  
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh, MemorySection)
        % Get DSM block Path
        theDsm = ['Rfft/'...
            char(this.Parent.Breed) '/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);

        % Configure DSM Storage Class
        identifier = [this.BufferAlias DataStoreMemoryName];
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FFTBuffer', ...
                                'Identifier', identifier, ...
                                'MemorySection', MemorySection);
    end
    %----------------------------------------------------------------------
    function ConfigureRfft2ChanInputSignalObject(this, InputSignalName, blockHandle, InportName, MemorySectionPrefix)
        InputBlock = MaskUtil.SubBlockName(blockHandle, ...
            ['Rfft/SharcPlus2Chan' InportName]);
        CscUtil.SetOutportCsc(InputBlock, 1, 'Blocklib','FFTBuffer', ...
            'MemorySection', [MemorySectionPrefix num2str(this.Parent.FftSize)], ...
            'Identifier', InputSignalName);
    end
    %----------------------------------------------------------------------  
    function val = get.DsmDataRealInitValue(this)
        val = zeros(this.Parent.FftSize,1);
    end
    %----------------------------------------------------------------------  
    function val = get.DsmDataImagInitValue(this)
        val = zeros(this.Parent.FftSize,1);
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
