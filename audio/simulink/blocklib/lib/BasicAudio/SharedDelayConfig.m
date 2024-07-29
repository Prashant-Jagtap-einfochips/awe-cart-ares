classdef SharedDelayConfig < BtcBaseClass
% SharedDelayConfig Build-time configuration class for the SharedDelay block
% Usage:
%    aConfig = SharedDelayConfig
%

%   Copyright 2021 Bose Corporation

  properties (GetObservable,SetObservable)
    % Properties use this syntax:
    %    PropertyName size class {validation functions} = DefaultValue
    % About Build-Time Configuration: https://wiki.bose.com/x/dqPoB
    FrameSize  = 32
    MaxTotalDelay  = 512
    NumInputs  = 3
    NumOutputs = 3
    CodeWorkMemorySize        % Code work memory size required for variant
    TotalDelayBufferSize      % Size of delay buffer required
    TotalFastDelayBufferSize  % Size of fast delay buffer required
  end
  properties (SetAccess=private) 
    DelayFrame  = 1
    DelayFrameLess  = 1
    DelayMultiFrame  = 1
    CodeWorkMemoryInitVal
	DelayBufferInitVal
	FastDelayBufferInitVal
  end
  %------------------------------------------------------------------------
  % Public Methods
  %------------------------------------------------------------------------
  methods
    %----------------------------------------------------------------------
    function this = SharedDelayConfig()
        % SharedDelayConfig constructor: assign component property objects
        % and adjust component property defaults and/or lock values
    end
    %----------------------------------------------------------------------
    function Validate(this)
        % Setup any calculated properties before validating/locking aliases 
        % There are three implementation types:

        % L2 Memory DMA Implementation

        % Set delay buffer sizes. Total delay memory is split between
        % the delay buffer and fast delay buffer, with only 1 frame of
        % samples, or the delay (delay < this.FrameSize) for each  
        % channel stored in the fast delay buffer. Get the delay buffer
        % sizes and DMA TCB memory required for the worst case Room 
        % Mode Down Load given the specified delay categories and 
        % this.MaxTotalDelay.
        delayConfig = [this.DelayFrameLess, this.DelayFrame, ...
                       this.DelayMultiFrame];
        [this.TotalFastDelayBufferSize, this.TotalDelayBufferSize, ...
         maxTcbMemSize, maxChanMapSize] = ...
            PoolDelayDmaMemorySizes(delayConfig, ...
                                    this.MaxTotalDelay, ...
                                    this.NumOutputs, ...
                                    this.FrameSize);

        % Make sure circular delay buffer size is at least as large as
        % the frame size. Note this is required to ensure the SHARC 
        % built-in circindex() function works correctly in the CRL 
        % code.
        this.TotalDelayBufferSize = max(this.FrameSize, ...
                                        this.TotalDelayBufferSize);

        % CRL code work memory size in bytes is equal to:
        %             sizeof(dmaWorkMemory_t) + variableDataSize;
        % with variable data size dependent on number channels and
        % number DMA TCB blocks.  NOTE: This must be updated to match
        % the CRL code if any changes are made to the dmaWorkMemory_t 
        % structure.
        this.CodeWorkMemorySize = (156/4) + maxTcbMemSize + ...
                                   maxChanMapSize; % Size in words
        % Validate called from mask initialization to check settings ok        
        % Call base class and check for any cross-property interactions
        Validate@BtcBaseClass(this);
    end

    function DoMaskInit(this,blockHandle)
        % DoMaskInit called from mask initialization to perform automation
        % of code generation settings and other canvas interactions.

        %.If there are no CscUtil code generation settings or other canvas
        %.interactions, this entire method can be deleted!
        %.Avoid making design-time changes - find another way if possible.
        %.(e.g. avoid calling: set_param, add_block, replace_block).
        %.If there's an error, just call the error function!
        % Create alias for delay buffer for each instance of model
        SharedDelayMask(blockHandle);

        this.CodeWorkMemoryInitVal = zeros(1, this.CodeWorkMemorySize);
        this.DelayBufferInitVal = zeros(1, this.TotalDelayBufferSize);
        this.FastDelayBufferInitVal = zeros(1, this.TotalFastDelayBufferSize);
    
        %.Call base class in case it has any work to perform:
        DoMaskInit@BtcBaseClass(this,blockHandle);
    end
  end
end
