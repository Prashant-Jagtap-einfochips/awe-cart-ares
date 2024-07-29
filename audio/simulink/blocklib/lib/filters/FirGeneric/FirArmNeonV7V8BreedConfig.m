classdef FirArmNeonV7V8BreedConfig < BtcBreedBaseClass
% FirArmNeonV7V8BreedConfig Build-time configuration class for the FirArmNeonV7V8 variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable)
    % PropertyName size class {validation functions} = DefaultValue
    CodeWorkMemorySize
    StateBufferSize
  end
  methods
    %----------------------------------------------------------------------
    function this = FirArmNeonV7V8BreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
        
	    if mod(this.Parent.MaxAvgCoeffLength, 4) ~= 0
            error("Property [MaxAvgCoeffLength] of Fir block is not a multiple of 4. Arm " + ...
                "variant requires that the number of taps is a multiple of 4. Generated C code will not be correct.");
        end

        if mod(this.Parent.MaxAvgCoeffLength, 4)  ~= 0
            error("Property [MaxAvgCoeffLength] of Fir block is not a multiple of 4. Arm " + ...
                "variant requires that the number of taps is a multiple of 4. Generated C code will not be correct.");
        end
        BreedValidate@BtcBreedBaseClass(this);

    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        this.ConfigureDataStoreMemory("StateBuffer", blockHandle);
        this.ConfigureDataStoreMemory("CodeWorkMemory", blockHandle);

        this.EnsureCodeReplacementLibrary(blockHandle,'ARM Neon');

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['Fir/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        DSMBlockHandle = getSimulinkBlockHandle(DSMBlockPath);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);

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
        val = (this.Parent.MaxAvgCoeffLength + this.Parent.FrameSize)*this.Parent.NumOutputs;
    end
    %----------------------------------------------------------------------
  end
end
