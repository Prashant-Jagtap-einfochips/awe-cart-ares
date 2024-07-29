classdef FirGenericSharcPlusCoreVariantConfig < BtcVariant
% FirGenericSharcPlusCoreVariantConfig Build-time configuration class for the FirGenericSharcPlusCore variant
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
    function this = FirGenericSharcPlusCoreVariantConfig(constructingParentObj)
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
        % 6 variable size arrays (for input, coeffs, states, 
        % stateSharingEnabled, taps array per output channel,
        % and mapped audio input offsets) 
        % + 10 struct members
        val = this.Parent.NumOutputs * 6 + 10;
    end
    %----------------------------------------------------------------------
    function val = get.StateBufferSize(this)
        val = (this.Parent.NumTaps + this.Parent.FrameSize - 1)*this.Parent.NumOutputs;
    end
    %----------------------------------------------------------------------
  end
end
