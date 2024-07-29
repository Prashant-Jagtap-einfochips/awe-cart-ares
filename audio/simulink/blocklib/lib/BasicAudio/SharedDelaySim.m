function audioOut = SharedDelaySim(audioIn, delayChain, inputMapping, ...
                                   totalDelayBufferSize)

% Determine frame size, input channel count, and output channel count
[frameSize,delayChain,inputMapping,totalStateLen,numInputs,...
    numOutputs] = SharedDelaySim1(audioIn, delayChain,inputMapping,...
    totalDelayBufferSize);
% Initialize audio outputs
audioOut = zeros(frameSize, numOutputs, 'single');
% Create local delay buffer that retains its state across function calls
persistent delayBuffer;
if isempty(delayBuffer)
    delayBuffer = zeros(1, totalDelayBufferSize, 'single');
end

% Keep track of delay index across function calls
persistent delayIdx;
if isempty(delayIdx)
    delayIdx = int32(0);
end

% Persistent variables maintained across all function calls.Determine the
% number of output taps per input channel and create output mapping that
% contains output channel/delay index for each input channel's output taps 
% in sequential order. Also determine maximum input delay for each input.
persistent outputTapsPerInput;
persistent outputMapping;
persistent maxInputDelay;
if isempty(maxInputDelay)
    outputTapsPerInput = zeros(1, numInputs, 'int32');
    outputMapping = zeros(1, numOutputs, 'int32');
    maxInputDelay = zeros(1, numInputs, 'int32');
    startIndex = int32(1);
    for i = 1:numInputs
        outputTapsPerInput(i) = sum(inputMapping == (i - 1));
        endIndex = startIndex + outputTapsPerInput(i) - 1;
        tapIndex = find(inputMapping == (i-1));
        outputMapping(startIndex:endIndex) = tapIndex;
        maxInputDelay(i) = max(delayChain(tapIndex));
        startIndex = endIndex + 1;
    end
    
    % Verify delay buffer size given is sufficient. Note this simulation
    % only requires a buffer size of sum(maxInputDelay); however the
    % delay buffer used for code generation must be (numInputs * frameSize)
    % larger to account for write before read DMA implementation.
    if (totalStateLen - numInputs * frameSize) < sum(maxInputDelay)
        error("Mask value 'Total Allotted Delay' is not large " + ...
            "enough for input 'DelayChain' specified.");
    end
end

% Process all channel delays:
[totalStateLen,outputTapsPerInput,outputMapping,~,...
    audioOut,delayBufferIdx,delayIdx] = SharedDelaySim2(delayIdx,...
    numInputs,totalStateLen,outputTapsPerInput,outputMapping,delayChain,...
    frameSize,maxInputDelay,delayBuffer,audioIn,audioOut);

% Input: Update each input's delay state data with its input frame
delayBuffer = SharedDelaySim3(numInputs,maxInputDelay,...
    frameSize,delayBufferIdx,totalStateLen,audioIn,delayBuffer);

end  % SharedDelaySim()

    function [frameSize,delayChain,inputMapping,totalStateLen,...
            numInputs,numOutputs] = SharedDelaySim1(audioIn, delayChain,...
            inputMapping,totalDelayBufferSize)
        
        [frameSize, numInputs] = size(audioIn);
        numOutputs = numel(delayChain);
        
        % Convert variables to integers
        frameSize = int32(frameSize);
        delayChain = int32(delayChain);
        inputMapping = int32(inputMapping);
        totalStateLen = int32(totalDelayBufferSize);
        
        % Validate input mapping
        if (min(inputMapping) ~= 0) || (max(inputMapping) >= numInputs)
            error("Input mapping elements must be in the range of " + ...
                "(0 <= element < numInputs) and there must be at least one input.");
        end
        
        % Validate delay chain elements
        if (min(delayChain) < 0)
            error("Delay chain elements must be greater than or equal to zero.");
        end
        
    end% SharedDelaySim1()

    function [totalStateLen,outputTapsPerInput,outputMapping,delayBuffer,...
            audioOut,delayBufferIdx,delayIdx] = SharedDelaySim2(delayIdx,...
            numInputs,totalStateLen,outputTapsPerInput,outputMapping,delayChain,...
            frameSize,maxInputDelay,delayBuffer,audioIn,audioOut)
        %----------------------------------------------------------------------
        % Process all channel delays:
        %       Note that circindex function is used here as it will map to
        %       the optimized SHARC+ built-in function in the event this
        %       simulation is ever used for code generation.
        %----------------------------------------------------------------------
        delayBufferIdx = delayIdx;
        delayChainIdx = int32(1);
        audioOutputIdx = int32(1);
        % Output: Transfer each output tap's delay state data to its output frame
        %         based on whether delay was less than or greater/equal to a frame.
        %         Delays of less than a frame first copy number of delay samples
        %         for the channel to the audio output frame and then copy the
        %         remaining samples (frame size - delay) from the audio input frame
        %         to the audio output frame. Delays of a frame or greater copy the
        %         entire frame from the delay buffer to the audio output.
        for input = 1:numInputs
            % Initial offset into delay buffer for current input channel
            inputChannelIdx = circindex(delayBufferIdx, int32(1), totalStateLen);
            % Output delay taps for this input channel
            numOutputTaps = outputTapsPerInput(input);
            
            for output = 1:numOutputTaps
                % Mapped delay and output index
                mappedIndex = outputMapping(audioOutputIdx);
                % Delay for current channel
                delay = delayChain(mappedIndex);
                delayChainIdx = delayChainIdx + 1;
                % Number of samples into the delay buffer
                partialDelay = min(frameSize, delay);
                % Initial offset into delay buffer for this input channel
                srcIdx = circindex(inputChannelIdx, maxInputDelay(input) - delay, ...
                    totalStateLen);
                
                % Fill first part of output buffer from delay buffer
                for sampleIdx = 1:partialDelay
                    audioOut(sampleIdx, mappedIndex) = delayBuffer(srcIdx + 1);
                    srcIdx = circindex(srcIdx, int32(1), totalStateLen);
                end
                
                % Fill the rest from the input buffer
                for  sampleIdx = partialDelay+1:frameSize
                    audioOut(sampleIdx, mappedIndex) = ...
                        audioIn(sampleIdx - partialDelay, input);
                end
                % Advance audio output to next channel
                audioOutputIdx = audioOutputIdx + 1;
            end
            % Advance buffer index for next channel
            delayBufferIdx = circindex(delayBufferIdx, maxInputDelay(input), ...
                totalStateLen);
        end
        
        % Reset index for all channels to prepare for input frame processing
        delayBufferIdx = circindex(delayIdx, frameSize, totalStateLen);
        delayIdx = delayBufferIdx;
        
    end% SharedDelaySim2()

    function delayBuffer = SharedDelaySim3(numInputs,maxInputDelay,...
            frameSize,delayBufferIdx,totalStateLen,audioIn,delayBuffer)
        % Input: Update each input's delay state data with its input frame
        % based on whether the maximum output tap delay for the each input
        % was less than or greater/equal to a frame. Maximum delays of
        % less than a frame copy remaining audio input samples not copied
        % to the audio output frame (delay samples) to the delay buffer.
        % Maximum delays of a frame or greater copy the entire audio input
        % frame to the delay buffer.
        for input = 1:numInputs
            % Maximum output tap delay for this input
            delay =  maxInputDelay(input);
            
            % Number of samples into the delay buffer
            partialDelay = min(frameSize, delay);
            
            % Calculate index with offset
            destIdx = circindex(delayBufferIdx, (delay - partialDelay + 1), ...
                totalStateLen);
            
            % Fill delay buffer from audio input
            for sampleIdx = (frameSize - partialDelay + 1):frameSize
                delayBuffer(destIdx + 1) = audioIn(sampleIdx, input);
                destIdx = circindex(destIdx, int32(1), totalStateLen);
            end
            
            % Advance buffer index for next channel
            delayBufferIdx = circindex(delayBufferIdx, maxInputDelay(input), ...
                totalStateLen);
        end
        
    end% SharedDelaySim3()


