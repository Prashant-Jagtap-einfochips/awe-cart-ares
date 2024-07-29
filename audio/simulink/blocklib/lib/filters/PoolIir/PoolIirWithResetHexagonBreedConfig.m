classdef PoolIirWithResetHexagonBreedConfig < BtcBreedBaseClass
% PoolIirWithResetHexagonBreedConfig Build-time configuration class for the PooliirHexagon variant
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation
  properties(GetObservable, SetObservable, SetAccess = private)
      PostOverlayRequired     = 1
  end
  properties(GetObservable, SetObservable)
      DsmStatesInitVal  (:,1)
      MaxNumOfThreads = 0;
  end
  methods
    %----------------------------------------------------------------------
    function this = PoolIirWithResetHexagonBreedConfig(parentObj)
        this@BtcBreedBaseClass(parentObj);
        this.Lock(@this.DsmStatesInitVal, ...
            @() zeros(2 * parentObj.MaxNumStages*parentObj.NumChannels + 16 + 3 * this.MaxNumOfThreads + this.MaxNumOfThreads * 7 + 1, 1));
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

        % Be friendly about the CRL this breed expects
        %this.EnsureCodeReplacementLibrary(blockHandle,'Hexagon');
        
        this.ConfigureDataStoreMemory('states', blockHandle);
        
        % Input needs to be aligned by 8 bytes
        this.ConfigureSignalObject('PoolIirAudioIn', blockHandle, "AudioIn");
        % Output needs to be aligned by 8 bytes
        this.ConfigureSignalObject('PoolIirAudioOut', blockHandle, "PoolIirProcess");
        % Coeffs need to be aligned by 8 bytes
        this.ConfigureSignalObject('PoolIirCoeffs', blockHandle, "Initialization/SignalBreak/SignalBreak/SignalBreakFcn");

        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function ConfigureDataStoreMemory(this, DataStoreMemoryName, blkh)
        % Get DSM block Path
        theDsm = ['PoolIirWithReset/TargetSpecific/' char(DataStoreMemoryName)];
        DSMBlockPath = MaskUtil.SubBlockName(blkh,theDsm);
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blkh,[char(this.Parent.Alias) char(DataStoreMemoryName)]);

        % Configure DSM Storage Class
        CscUtil.SetDatastoreCsc(DSMBlockPath, 'Blocklib', 'PooliirAccelStorage', ...
                                'MemorySection','AttributeAligned8',...
                                'Identifier', Identifier);
    end
    %----------------------------------------------------------------------
    function ConfigureSignalObject(this, SignalName, blockHandle, InportName)
        Identifier = MaskUtil.GetIdentifierBasedOnSID(blockHandle,[char(this.Parent.Alias) char(SignalName)]);
        InputBlock = MaskUtil.SubBlockName(blockHandle, ...
            ['PoolIirWithReset/TargetSpecific' InportName]);
        CscUtil.SetOutportCsc(InputBlock, 1,'Blocklib','PooliirAccelStorage', ...
            'MemorySection', 'AttributeAligned8',...
            'Identifier', Identifier);
    end 
    %----------------------------------------------------------------------
  end
end
