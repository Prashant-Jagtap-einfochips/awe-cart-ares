classdef FirGenericArmNeonV7V8VariantConfig < BtcVariant
% FirGenericArmNeonV7V8VariantConfig Build-time configuration class for the FirGenericArmCortexa variant
% Usage:
%    <automatically allocated by setting the "Variant" property of the
%    containing object derived from BtcBaseClassWithVariants
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % PropertyName size class {validation functions} = DefaultValue
    CodeWorkMemorySize
    StateBufferSize
  end
  methods
    %----------------------------------------------------------------------
    function this = FirGenericArmNeonV7V8VariantConfig(constructingParentObj)
        this@BtcVariant(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function VariantValidate(this)
        % VariantValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
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
        expectedCRL = 'ARM Neon V7/V8 code replacement library';
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
    	% For CodeWorkMemorySize computation, see fir_init.c in /lib/arch/arm/arc/fir_armv7v8.c
        val = 156 + (24 + this.Parent.NumInputs*8 + this.Parent.NumOutputs*(80 + 4*this.Parent.FrameSize));
        val = val / 4;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = (this.Parent.NumTaps + this.Parent.FrameSize)*this.Parent.NumOutputs;
    end
    %----------------------------------------------------------------------
  end
end
