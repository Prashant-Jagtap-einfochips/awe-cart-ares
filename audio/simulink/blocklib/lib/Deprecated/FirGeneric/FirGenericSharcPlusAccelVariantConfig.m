classdef FirGenericSharcPlusAccelVariantConfig < BtcVariant
% FirGenericSharcPlusAccelVariantConfig Build-time configuration class for the FirGenericSharcPlusAccel variant
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
    function this = FirGenericSharcPlusAccelVariantConfig(constructingParentObj)
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
        
        % Be friendly about the CRL this variant expects
        containingModel = getfullname(bdroot(blockHandle));
        selectedCRL = get_param(containingModel,'CodeReplacementLibrary');
        expectedCRL = 'SHARC+ code replacement library';
        if (false == contains(selectedCRL, expectedCRL))
            warning('Expected CRL [%s], but instead found [%s]', ...
                expectedCRL,selectedCRL);
        end
        VariantDoMaskInit@BtcVariant(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = get.CodeWorkMemorySize(this)
        % Accelerator CRL code work memory size is equal to:
        %             sizeof(accelWorkMemory_t) + variableDataSize;
        % with variable data size dependent on number inputs, number outputs
        % and frame size.  NOTE: this must be updated to match the CRL code
        % if any changes are made to the accelWorkMemory_t structure.
        val = 156 + (24 + this.Parent.NumInputs*48 + this.Parent.NumOutputs*(80 + 4*this.Parent.FrameSize));
        val = val / 4;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = (this.Parent.NumTaps + this.Parent.FrameSize - 1)*this.Parent.NumInputs;
    end
    %----------------------------------------------------------------------
  end
end
