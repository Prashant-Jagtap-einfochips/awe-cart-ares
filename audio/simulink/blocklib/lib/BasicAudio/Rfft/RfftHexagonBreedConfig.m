classdef RfftHexagonBreedConfig < BtcBreedBaseClass
% RfftHexagonBreedConfig Build-time configuration class for the RfftHexagon breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties
    % PropertyName size class {validation functions} = DefaultValue
  end
  properties(SetAccess = private)
      BufferAlias
      InputName
  end
  properties(GetObservable, SetObservable, Dependent, Hidden, SetAccess = private)
    DsmTwiddle1InitValue (:,1) single
    DsmTwiddle2InitValue (:,1) single
    DsmRfftOutputTmpInitValue (:,1) single
  end
  methods
    %----------------------------------------------------------------------
    function this = RfftHexagonBreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function value = get.BufferAlias(this)
        value = [char(this.Parent.Alias) 'RfftBuffer'];
    end
    %----------------------------------------------------------------------
    function value = get.InputName(this)
        value = [char(this.Parent.Alias) 'RfftInput'];
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
        this.ConfigureDataStoreMemory('Twiddle1', blockHandle);
        this.ConfigureDataStoreMemory('Twiddle2', blockHandle);
        this.ConfigureDataStoreMemory('RfftOutputTmp', blockHandle);

        % Be friendly about the CRL this breed expects
        this.EnsureCodeReplacementLibrary(blockHandle, 'Qualcomm Hexagon DSP code replacement library')

        % Create Signal Object for Inputs
        this.ConfigureInputSignalObject(this.InputName, blockHandle, "AudioIn");

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
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
                                'Identifier', identifier, ...
                                'MemorySection','AttributeAligned8');
    end
    %----------------------------------------------------------------------
    function ConfigureInputSignalObject(this, InputSignalName, blockHandle, InportName)
        InputBlock = MaskUtil.SubBlockName(blockHandle, ...
            ['Rfft' char(this.Parent.Breed) InportName]);
        CscUtil.SetOutportCsc(InputBlock, 1, 'Blocklib','FFTBuffer', ...
            'MemorySection', ['AttributeAligned' num2str(this.Parent.FftSize*4)], ...
            'Identifier', InputSignalName);
    end    
    %----------------------------------------------------------------------
    function val = get.DsmRfftOutputTmpInitValue(this)
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


