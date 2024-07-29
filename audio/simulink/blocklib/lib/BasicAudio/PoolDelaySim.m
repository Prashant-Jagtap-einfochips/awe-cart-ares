function audioOut = PoolDelaySim(audioIn, delayTime, totalDelayBufferSize)

    % Determine frame size and channel count
    [frameSize, numChannels] = size(audioIn);

    % Convert variables to integers
    frameSize = int32(frameSize);
    delayTime = int32(delayTime);
    totalStateLen = int32(totalDelayBufferSize);
    audioOut = coder.nullcopy(audioIn);

    % Create local fast delay buffer for simulation that retains its state
    % across function calls
    persistent fastDelayBuffer;
    if isempty(fastDelayBuffer)
        fastDelayBuffer = zeros(1, totalDelayBufferSize, 'single');
    end
    
    % Keep track of delay index across function calls
    persistent delayIdx;
    if isempty(delayIdx)
        delayIdx = int32(0);
    end

    %----------------------------------------------------------------------
    % Process all channel delays:
    %       Note that circindex function is used here as it will map to 
    %       the optimized SHARC+ built-in function for code generation. 
    %----------------------------------------------------------------------

    delayBufferIdx = delayIdx; 

    % Output: Transfer each channel's delay state data to its output frame 
    %         based on whether delay was less than or greater/equal to a frame.
    %         Delays of less than a frame first copy number of delay samples
    %         for the channel to the audio output frame and then copy the
    %         remaining samples (frame size - delay) from the audio input frame
    %         to the audio output frame. Delays of a frame or greater copy the 
    %         entire frame from the delay buffer to the audio output.
    for ch = 1:numChannels
        
        % Delay for current channel
        delay = delayTime(ch);
        
        % Number of samples into the delay buffer
        partialDelay = min(frameSize, delay);

        % Initial offset into delay buffer
        srcIdx = circindex(delayBufferIdx, int32(1), totalStateLen);
        
        % Fill first part of output buffer from delay buffer
        for sampleIdx = 1:partialDelay
            audioOut(sampleIdx, ch) = fastDelayBuffer(srcIdx + 1);
            srcIdx = circindex(srcIdx, int32(1), totalStateLen);
        end

        % Fill the rest from the input buffer
        for  sampleIdx = partialDelay+1:frameSize
            audioOut(sampleIdx, ch) = audioIn(sampleIdx - partialDelay, ch);
        end

        % Advance buffer index for next channel
        delayBufferIdx = circindex(delayBufferIdx, delay, totalStateLen);

    end

    % Reset index for all channels to prepare for input frame processing
    delayBufferIdx = circindex(delayIdx, frameSize, totalStateLen);
    delayIdx = delayBufferIdx;

    % Input: Update each channel's delay state data with its input frame 
    %        based on whether delay was less than or greater/equal to a frame.
    %        Delays of less than a frame copy remaining audio input samples
    %        not copied to the audio output frame (delay samples) to the delay
    %        buffer. Delays of a frame or greater copy the entire audio input 
    %        frame to the delay buffer.
    for ch = 1:numChannels

        % Delay for current channel
        delay = delayTime(ch);
        
        % Number of samples into the delay buffer
        partialDelay = min(frameSize, delay);
        
        % Calculate index with offset
        destIdx = circindex(delayBufferIdx, (delay - partialDelay + 1), ...
                            totalStateLen);

        % Fill output buffer from delay buffer
        for sampleIdx = (frameSize - partialDelay + 1):frameSize
            fastDelayBuffer(destIdx + 1) = audioIn(sampleIdx, ch);
            destIdx = circindex(destIdx, int32(1), totalStateLen);
        end
        
        % Advance buffer index for next channel
        delayBufferIdx = circindex(delayBufferIdx, delay, totalStateLen);
    
    end

end  % PoolDelaySim()


