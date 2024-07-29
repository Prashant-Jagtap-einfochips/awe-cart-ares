classdef RfftArmNeonV7V8BreedConfig < BtcBreedBaseClass
% RfftArmNeonV7V8BreedConfig Build-time configuration class for the RfftArmNeonV7V8 breed
% Usage:
%    <automatically allocated by setting the "Breed" property of the
%    containing object derived from BtcBaseClassWithBreeds
%

%   Copyright 2023 Bose Corporation

  properties (GetObservable,SetObservable, Dependent)
    DsmCodeWorkMemoryInitValue (:,1) uint32
    BufferAlias
  end
  methods
    %----------------------------------------------------------------------
    function this = RfftArmNeonV7V8BreedConfig(constructingParentObj)
        this@BtcBreedBaseClass(constructingParentObj);
    end
    %----------------------------------------------------------------------
    function BreedValidate(this)
        % BreedValidate validates cross-property interactions. Our parent
        % object calls us during block mask initialization.
        if (mod(this.Parent.FftSize,4) ~= 0)
            error("ArmNeonV7V8 Breed must have FftSize that is a multiple of 4");
        end
        if (this.Parent.FftSize < 15)
            error("ArmNeonV7V8 Breed must have FftSize greater than 15");
        end
        BreedValidate@BtcBreedBaseClass(this);
    end
    %----------------------------------------------------------------------
    function BreedDoMaskInit(this,blockHandle)
        % BreedDoMaskInit performs automation of code generation settings
        % and other canvas interactions for a variant subsystem. Our parent
        % object calls us during block mask initialization.
        this.ConfigureDataStoreMemory('CodeWorkMemory',  blockHandle);
        this.EnsureCodeReplacementLibrary(blockHandle,"ARM Neon V7/V8 code replacement library");
        BreedDoMaskInit@BtcBreedBaseClass(this,blockHandle);
    end
    %----------------------------------------------------------------------
    function val = get.DsmCodeWorkMemoryInitValue(this)
        % ne10_uint32_t memneeded = 
        % + sizeof (ne10_fft_r2c_state_float32_t)           /*(40 bytes)*/
        % + sizeof (ne10_int32_t) * (NE10_MAXFACTORS * 2)   /* factors (256 bytes) */
        % + sizeof (ne10_fft_cpx_float32_t) * ncfft         /* twiddle (8*N/2 bytes) */
        % + sizeof (ne10_fft_cpx_float32_t) * (ncfft / 2)   /* super twiddles (8*N/2/2 bytes)*/
        % + sizeof (ne10_fft_cpx_float32_t) * nfft          /* buffer (8*N bytes)*/
        % + NE10_FFT_BYTE_ALIGNMENT;                        /* 64-bit alignment (8 bytes) */
        %
        % typedef struct ~40 bytes
        % {
        %     ne10_fft_cpx_float32_t *buffer;           // 8 byte
        %     ne10_int32_t ncfft;                       // 4 byte
        %     ne10_int32_t *factors;                    // 8 byte
        %     ne10_fft_cpx_float32_t *twiddles;         // 8 byte
        %     ne10_fft_cpx_float32_t *super_twiddles;   // 8 byte
        % 
        % } ne10_fft_r2c_state_float32_t;
        MemorySizeBytes =   40 + ... 
                            4*32*2 + ...
                            8*this.Parent.FftSize/2 + ...
                            8*this.Parent.FftSize/2/2 + ...
                            8*this.Parent.FftSize + ...
                            8;
        % Force to be multiple of 8 for Neon SIMD
        % Convert from Num Bytes to uint32
        CodeWorkMemorySize = ceil(MemorySizeBytes/4);
        CodeWorkMemorySize = CodeWorkMemorySize + rem(CodeWorkMemorySize,8);

        val = uint32(zeros(CodeWorkMemorySize,1));          
     
    end
    %----------------------------------------------------------------------
    function val = get.BufferAlias(this)
        val = [char(this.Parent.Alias) 'RfftBuffer'];
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
                                'MemorySection','Default');
    end
    %----------------------------------------------------------------------
  end
end
