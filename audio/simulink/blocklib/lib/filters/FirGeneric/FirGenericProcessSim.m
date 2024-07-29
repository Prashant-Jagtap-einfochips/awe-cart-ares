function y = FirGenericProcessSim(Input, Coeffs, StateSize, FilterLengths, ...
                InputMapping, FilterMapping, FrameSize, NumOutputs, Breed)
arguments
    Input single
    Coeffs single
    StateSize (1,1) {mustBeInteger}
    FilterLengths {mustBeInteger}
    InputMapping {mustBeInteger}
    FilterMapping {mustBeInteger}
    FrameSize (1,1) {mustBeInteger}
    NumOutputs (1,1) {mustBeInteger}
    Breed AsdHwEnum = AsdHwEnum.Generic
end

% This function Assumes Coefficients are already flipped, for specified
% Breed, if coefficients are not already flipped, flip them now.
if IsFirCoeffsFlipped(Breed) == false
    Coeffs = FlipCoefficients(Coeffs,FilterLengths);
else
    % No Flipping required
end

y = zeros(FrameSize,NumOutputs,class(Input));
numCoeffSets = numel(FilterLengths);

% Matlab value objects are passes by value, so 'states' is only used to
% trigger CRL. Another buffer is created here with the same size to
% actually save the states between function calls.
persistent states2;
if isempty(states2)
    states2 = single(zeros(StateSize,1));
end

% Calculate filter offsets from filter lengths
persistent filterOffsets;
if isempty(filterOffsets)
    filterOffsets = uint32(ones(numCoeffSets,1));
    currIndex = uint32(1);
    for OutputIndex = 2:numCoeffSets
        currIndex = FilterLengths(OutputIndex-1) + currIndex;
        filterOffsets(OutputIndex,1) = currIndex;
    end
end

% Calculate state offsets from filter lengths and filter mapping. In Matlab
% state has size [fitler length - 1], not [filter length + frame size - 1],
% so the stateOffsets here is different than the one derived in CRL.
persistent stateOffsets;
if isempty(stateOffsets)
    % Initiate State Buffer Offset Array
    stateOffsets = uint32(ones(NumOutputs,1));
    % Get the index of the filter for all but the last output
    FilterIndices = FilterMapping(1:NumOutputs-1);
    % Get the filter length for each active filter
    ActiveFilterLengths = FilterLengths(FilterIndices + 1);
    % Get state size from filter length
    StateSizes = ActiveFilterLengths-1;
    % stateOffset(filter[i+1]) = stateOffset(filter[i]) + Len(filter[i])-1
    stateOffsets(2:NumOutputs,1) = uint32(cumsum(StateSizes) + 1);
end

statesTemp = states2; % temp state buffer
for OutputIndex = 1:NumOutputs
    % Get input 
    inputIdx = InputMapping(OutputIndex)+1;  % add 1 for Matlab indexing
    CurrentInput = Input(:, inputIdx);      

    % Get filter coefficients
    coeffIdx = FilterMapping(OutputIndex)+1; % add 1 for Matlab indexing
    coeffOffset = filterOffsets(coeffIdx); % offsets already defined in Matlab as one-based
    coeffLength = FilterLengths(coeffIdx);
    coeff = Coeffs(coeffOffset: (coeffOffset + coeffLength - 1));

    % Get state pointer
    stateOffset = stateOffsets(OutputIndex);
    state = states2(stateOffset : stateOffset+coeffLength-2);

    [y0, state] = FirFilter(coeff, CurrentInput, state);
    y(:, OutputIndex) = y0;
    
    % Update Temp state vector
    statesTemp(stateOffset:stateOffset+coeffLength-2) = state;

end

% Update state buffer 
states2 = statesTemp;
end

function FlippedCoeffs = FlipCoefficients(Coeffs, CoeffsLengths)
FlippedCoeffs = zeros(size(Coeffs),class(Coeffs));
CoeffsOffset = 1;
for idx = 1:numel(CoeffsLengths)
    Length = double(CoeffsLengths(idx));
    FlippedCoeffs(CoeffsOffset: CoeffsOffset + Length - 1) = flip(Coeffs(CoeffsOffset : CoeffsOffset + Length - 1));
    CoeffsOffset = CoeffsOffset + Length;

    if(CoeffsOffset >= numel(Coeffs))
         CoeffsOffset = 1;  
    end
end
end