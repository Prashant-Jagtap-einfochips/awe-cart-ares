classdef FirGenericHexagonVariantConfig < BtcVariant
% FirGenericHexagonVariantConfig Build-time configuration class for the FirGenericHexagon variant
% Usage:
%    <automatically allocated by setting the "Variant" property of the
%    containing object derived from BtcBaseClassWithVariants
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
    function this = FirGenericHexagonVariantConfig(constructingParentObj)
        this@BtcVariant(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function VariantValidate(this)
        % VariantValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
        if mod(this.Parent.NumTaps, 4)  ~= 0
            error("Property [NumTaps] of FirGeneric block is not a multiple of 4. Hexagon " + ...
                "variant requires that the number of taps is a multiple of 4. Generated C code will not be correct.");
        end

        VariantValidate@BtcVariant(this);
    end
    %----------------------------------------------------------------------
    function VariantDoMaskInit(this,blockHandle)
        % VariantDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        this.ConfigureDataStoreMemory("States", blockHandle);
        this.ConfigureDataStoreMemory("CodeWorkMemory", blockHandle);

        % Be friendly about the CRL this variant expects
        containingModel = getfullname(bdroot(blockHandle));
        selectedCRL = get_param(containingModel,'CodeReplacementLibrary');
        expectedCRL = 'Qualcomm Hexagon DSP code replacement library';
        if (false == contains(selectedCRL, expectedCRL))
            warning('Expected CRL [%s], but instead found [%s]', ...
                expectedCRL,selectedCRL);
        end

        VariantDoMaskInit@BtcVariant(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['FirGeneric/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        DSMBlockHandle = getSimulinkBlockHandle(DSMBlockPath);
        Identifier = [char(this.Parent.Alias) char(DataStoreMemoryName)];

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
    function val = get.CodeWorkMemorySize(this)
        val = 156 + (24 + this.Parent.NumInputs*8 + this.Parent.NumOutputs*(80 + 4*this.Parent.FrameSize));
        val = val / 4;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        % qhdsp_fir_af_bose.S
        %val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize - 1)*this.Parent.NumOutputs;
        
        % flt_bkfir.S
        val = (this.Parent.MaxAvgCoeffLength + 2*(this.Parent.FrameSize) - 1)*this.Parent.NumOutputs;
    end
    %----------------------------------------------------------------------
  end
end
