function [maxFastBufSize, maxBufSize, maxTcbMemSize, maxChanMapSize] = ... 
                PoolDelayDmaMemorySizes(delayConfig, maxDelay, numChans, frameSize)
% Determines the worst case delay buffer and EMDMA descriptor (TCB) memory
% required to handle mask delay configuration input
%
% Input Parameters:
%   delayConfig:    Delays cases selected [1 x 3]
%   maxDelay:       Maximum delay (sum of all channel delays) in samples (scaler)
%   numChannels:    Number of channels (scaler)
%   frameSize:      Frame size (scaler)
%
% Outputs:
%   maxFastBufSize  Maximum fast delay buffer size in samples (scaler)
%   maxBufSize      Maximum slow delay buffer size in samples (scaler)
%   maxTcbMemSize   Maximum DMA TCB memory size in 32 bit words (scaler)
%   maxChanMapSize  Maximum TCB channel map size in 32 bit words (scaler)
%          
    
    % Convert vector to logical vector
    delayConfig = logical(delayConfig);
    
    % Delay buffer sizes and number TCBs required for each delay case
    fastBufSize = [0 0 0];
    slowBufSize = [0 0 0];
    numTcb = [0 0 0];
  
    % Delay Case 1: (delay <= frameSize)
    if delayConfig(1)
        fastBufSize(1) = min((numChans * frameSize), maxDelay);
        slowBufSize(1) = 0;
        numTcb(1) = numChans;
    end
  
    % Delay Case 2: (frameSize < delay < (2 * frameSize))
    if delayConfig(2)
        fastBufSize(2) = min((numChans * frameSize), maxDelay);
        slowBufSize(2) = min((maxDelay - fastBufSize(2)), ...
                             (numChans * (frameSize -1)));
        numTcb(2) = 3 * numChans;
    end
  
    % Delay Case 3: (delay >= (2 * frameSize))
    if delayConfig(3)
        fastBufSize(3) = min((numChans * frameSize), maxDelay);
        slowBufSize(3) = (maxDelay - fastBufSize(3));
        numTcb(3) = 2 * numChans;
    end

    % Make sure total allotted delay is compatible with other mask
    % parameter settings
    if (delayConfig(1) && ~delayConfig(2) && ~delayConfig(3) && (maxDelay > fastBufSize(1))) || ...
       (delayConfig(2) && ~delayConfig(3) && (maxDelay > (fastBufSize(2) + slowBufSize(2)))) || ...
       (delayConfig(2) && slowBufSize(2) == 0) || ...
       (delayConfig(3) && slowBufSize(3) == 0)
        error("'Total Allotted Delay' specified is not valid with other mask parameter settings");
    end     

    % Determine the maximum delay buffer sizes required in samples for delay
    % configurations selected
    maxFastBufSize = max(fastBufSize(:));
    maxBufSize = max(slowBufSize(:));
  
    % Determine the maximum TCB and channel map memory required in words
    maxTcbMemSize = 9 * max(numTcb(:));
    maxChanMapSize = 2 * numChans;
  
end

% NOTE: Function not currently used.  It provides exact buffer sizing for 
% delay cases specified. Delay cases can be worst case for RMDL.
function [maxFastBufSize, maxBufSize, maxTcbMemSize, maxChanMapSize] = ... 
                            PoolDelaySizeMemory(rmdlDelays, frameSize)
    % Determines the worst case Room Mode Download delay buffer and EMDMA 
    % descriptor (TCB) memory required to handle all RMDL delay groups 
    % for Pool Delay EMDMA variant using both L2 (slow) and L1 (fast) memory
    %
    % Input Parameters:
    %   rmdlDelays:     Delays for all RMDL groups [numDelayGroups x maxNumChannels]
    %                   All channel delay groups less than maxNumChannels must be
    %                   padded with zeros.
    %   frameSize:      Frame size (scaler)
    %
    % Outputs:
    %   maxFastBufSize  Maximum fast delay buffer size in samples (scaler)
    %   maxBufSize      Maximum slow delay buffer size in samples (scaler)
    %   maxTcbMemSize   Maximum DMA TCB memory size in 32 bit words (scaler)
    %   maxChanMapSize  Maximum TCB channel map size in 32 bit words (scaler)
    %

    % Get the number of different RMDL delay groups and maximum number of 
    % channels
    [numDelayGroups, maxNumChans] = size(rmdlDelays);

    % Maximum delay and fast delay buffer sizes in samples
    maxBufSize = 0;
    maxFastBufSize = 0;
    
    % Maximum number DMA descriptors (TCBs) required and memory size in
    % words
    maxNumTcbRequired = 0;
    maxTcbMemSize = 0;
    
    % Number of channel within each delay case for each delay group
    % Different delay cases for DMA transfer handling:
    %       Case 1:  (0 < delay <= frameSize)
    %       Case 2:  (frameSize < delay < (2 * frameSize))
    %       Case 3:  (delay >= (2 * frameSize))
    delayCase = zeros(numDelayGroups, 3);
    
    % Total pool delay for all channels in each delay group
    totalPoolDelay = zeros(numDelayGroups, 1);
    
    % Total delay for frame or less case for each group
    totalDelayCase1 = zeros(numDelayGroups, 1);
    
    for group = 1:numDelayGroups
        
        % Determine the total delay requiring delay buffer memory for this
        % group. A delay of zero requires no delay buffer memory.
        totalPoolDelay(group) = sum(rmdlDelays(group, :));
        
        % Determine channel delays within each delay case for this group
        for i = 1:maxNumChans
            
            % Get channel delay for this group
            delay = rmdlDelays(group, i);
            
            if ((delay > 0) && (delay <= frameSize))
                
                % Total case 1 delay for this group
                totalDelayCase1(group) = totalDelayCase1(group) + delay;
                
                % Number of channels within this group with case 1 delay
                delayCase(group, 1) = delayCase(group, 1) + 1;
                
            elseif ((delay > frameSize) && (delay < (2 * frameSize)))
                
                % Number of channels within this group with case 2 delay
                delayCase(group, 2) = delayCase(group, 2) + 1;
                
            elseif (delay >= (2 * frameSize))
                
                % Number of channels within this group with case 3 delay
                delayCase(group, 3) = delayCase(group, 3) + 1;
                
            end
            
        end
        
        % Determine the fast buffer size in samples for this delay group.
        % The fast delay buffer holds a frame size of delay for each
        % channel with delays greater than or equal to the frame size and 
        % the size of the delay for delays less than a frame. 
        fastBufferSize = ((delayCase(group, 2) + delayCase(group, 3)) * ...
            frameSize) + totalDelayCase1(group);
                      
        % Determine the maximum fast delay buffer size              
        maxFastBufSize = max(fastBufferSize, maxFastBufSize);
        
        % Determine the delay buffer size for this delay group. The slow
        % delay buffer holds the remaining delay.
        delayBufferSize = totalPoolDelay(group) - fastBufferSize;
                      
        % Determine the maximum delay buffer size              
        maxBufSize = max(delayBufferSize, maxBufSize);
        
        % Determine the number of descriptors (TCBs) required for this 
        % group for DMA transfers. Each delay case requires these delays:
        %       Case 1:  1 TCB 
        %       Case 2:  3 TCBs
        %       Case 3:  2 TCBs
        numTcbRequired = delayCase(group, 1) + (3 * delayCase(group, 2)) + ...
            (2 * delayCase(group, 3));
        
        % Determine the maximum number of TCBs required
        maxNumTcbRequired = max(numTcbRequired, maxNumTcbRequired);
        
    end
        
    % Determine the maximum TCB memory required in words
    maxTcbMemSize = 9 * maxNumTcbRequired;

    % Determine the maximum TCB channel map memory required in words
    maxChanMapSize = 2 * maxNumChans; 
    
end