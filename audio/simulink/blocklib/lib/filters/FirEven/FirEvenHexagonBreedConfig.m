classdef FirEvenHexagonBreedConfig < BtcBreedBaseClass
% FirEvenHexagonBreedConfig Build-time configuration class for the FirEvenHexagon variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties(GetObservable, SetObservable)
    % PropertyName size class {validation functions} = DefaultValue
    Alias = "Default"
    MaxNumOfThreads 
    StateBufferSize
  end
  methods
    %----------------------------------------------------------------------
    function this = FirEvenHexagonBreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
        this.MaxNumOfThreads=constructingParentObj.MaxNumOfThreads;
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
        this.ConfigureDataStoreMemory("CodeWorkMemory", blockHandle);
        this.ConfigureDataStoreMemory("States", blockHandle);
        this.ConfigureSignalObject([char(this.Parent.Alias) 'FirEvenCoeffs'], blockHandle,'Initialization/SignalBreak');
        this.ConfigureSignalObject([char(this.Parent.Alias) 'FirEvenInput'], blockHandle,'AudioIn');


        % Be friendly about the CRL this variant expects
        this.EnsureCodeReplacementLibrary(blockHandle,'Hexagon');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['FirEven/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);

        % Configure DSM Storage Class
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'FirStorage',...
                                'MemorySection', 'AttributeAligned8',...
                                'Identifier', Identifier);

    end
    %----------------------------------------------------------------------
    function ConfigureSignalObject(this, SignalName, blockHandle, PortName)
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blockHandle,[char(this.Parent.Alias) char(SignalName)]);
        InputBlock = MaskUtil.SubBlockName(blockHandle, ...
            ['FirEven/TargetSpecific/' PortName]);
        CscUtil.SetOutportCsc(InputBlock, 1,'Blocklib','FirGenericStorage',...
            'MemorySection', 'AttributeAligned8',...
            'Identifier', Identifier);
    end  
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        % qhdsp_fir_af_bose.S
        %val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize - 1)*this.Parent.NumOutputs;
        
        % flt_bkfir.S
        val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize*2 + 2) * this.Parent.NumChannels * 2;
    end
  end
end
