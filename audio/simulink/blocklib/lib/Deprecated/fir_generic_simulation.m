function y = fir_generic_simulation(u, coeffs, states, filterLengths, ...
                inputMapping, filterMapping, frameSize, numInputs, numOutputs)

y = single(zeros(frameSize,numOutputs));
numCoeffSets = numel(filterLengths);

% Matlab value objects are passes by value, so 'states' is only used to
% trigger CRL. Another buffer is created here with the same size to
% actually save the states between function calls.
persistent states2;
if isempty(states2)
    states2 = single(zeros(size(states)));
end

% Calculate filter offsets from filter lengths
persistent filterOffsets;
if isempty(filterOffsets)
    filterOffsets = uint32(ones(numCoeffSets,1));
    currIndex = uint32(1);
    for i = 2:numCoeffSets
        currIndex = filterLengths(i-1) + currIndex;
        filterOffsets(i,1) = currIndex;
    end
end

% Calculate state offsets from filter lengths and filter mapping. In Matlab
% state has size [filer length - 1], not [filter length + frame size - 1],
% so the stateOffsets here is different than the one derived in CRL.
persistent stateOffsets;
if isempty(stateOffsets)
    stateOffsets = uint32(ones(numInputs,1));
    currIndex = uint32(1);

    for i = 2:numInputs
        % Find first coeff set applied to last input, the rest is assumed
        % to have the same length anyway. 
        prevIndex = i-1;
        allChannelIdx = find(inputMapping == (prevIndex-1)); % find channel indices
        if ~isempty(allChannelIdx)
            % Get first channel index
            firstChannelIdx = allChannelIdx(1); 
            
            % Get coeff index corresponding to this channel
            % add 1 for Matlab indexing
            firstCoeffIdx = filterMapping(firstChannelIdx) + 1; 
            
            % State buffer length in Matlab is filter length - 1
            prevStateBufferLength = filterLengths(firstCoeffIdx) - 1;
            
            % Current state offset is previous state buffer length +
            % previous offset
            currIndex = prevStateBufferLength + currIndex;
        end
        stateOffsets(i,1) = currIndex;
    end

end

inputBufferUpdated = zeros(1,numInputs);  % flags to indicate an input buffer has been updated
statesTemp = states2; % temp state buffer
for i = 1:numOutputs
    % Get input 
    inputIdx = inputMapping(i)+1;  % add 1 for Matlab indexing
    input = u(:, inputIdx);      

    % Get filter coefficients
    coeffIdx = filterMapping(i)+1; % add 1 for Matlab indexing
    coeffOffset = filterOffsets(coeffIdx); % offsets already defined in Matlab as one-based
    coeffLength = filterLengths(coeffIdx);
    coeff = coeffs(coeffOffset: (coeffOffset + coeffLength - 1));
    coeff = reshape(coeff, [], 1); % always reshape to column vector
    coeff = flipud(coeff);         % flip to work with Matlab filter

    % Get state pointer
    stateOffset = stateOffsets(inputIdx);
    state = states2(stateOffset : stateOffset+coeffLength-2);

    % Call filter function
    [y0, state] = filter(coeff,1,input,state);
    y(:,i) = y0;

    % Update temp state vector, only filterLength-1 samples are used 
    if (inputBufferUpdated(inputIdx) ~= 1)
        statesTemp(stateOffset:stateOffset+coeffLength-2) = state; 
        inputBufferUpdated(inputIdx) = 1;
    end    
end

% Update state buffer 
states2 = statesTemp;
end