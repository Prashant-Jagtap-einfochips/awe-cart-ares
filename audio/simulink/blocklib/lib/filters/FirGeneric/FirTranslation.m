function [CoeffsTarget, CoeffsTargetLengths] = FirTranslation(CoeffsRaw, CoeffsRawLengths, Breed, NumFilters, MaxAvgCoeffLength, FrameSize, IsCoeffsRawFlipped, AlwaysFlip)
% Translates Raw Filter Coefficients into target specific format. Will do
% coefficient flipping and padding if necessary.
% Inputs:
%   CoeffsRaw: [1d array] All filter coefficients stored consecutively in a
%       1d array. Filter lengths may vary between filters. IE: For N number 
%       of filter, CoeffsRaw stores the i-th filter hi(n) as
%       CoeffsRaw = [h1(1:end); h2(1:end); ... hN(1:end)]
%       filter_i[0] = CoeffsRaw(CoeffsMapping(i))
%   CoeffsRawLengths: [1d array] Array of lengths for each filter. The length
%       of filter i is given by CoeffsLengths(i)
%   Breed: [AsdHwEnum Type] Specifies the target type for
%       translation
%   FrameSize: [Integer]
%   IsCoeffsRawFlipped [logical]
%   TotalPaddedLength [Integer]
% Output:
%   CoeffsTarget: [1d array] All translated filter coefficients after
%       translation into target specific format.
%   CoeffsTargetLengths [1d array] Modified CoeffsLengths, depends on Breed
%
% Breed Specific Requirements:
%   All Breeds:
%       -Filter Coefficients must be flipped
%   For Hexagon and ArmNeonV7V8
%       -Filter lengths must all be equal. 
%       -Filter length must be multiple of 4. (Use zero-padding)
%       -Filter lengths must be greater than the FrameSize. (Use zero-padding)
%       -Filter length requirements can be met by zero-padding filters to ensure
%           compliance
%--------------------------------------------------------------------------

arguments
    CoeffsRaw 
    CoeffsRawLengths 
    Breed AsdHwEnum = AsdHwEnum.Generic                         % Specified Hardware Target
    NumFilters  {mustBeInteger} = numel(CoeffsRawLengths)       % Number of Filters specified
    MaxAvgCoeffLength {mustBeInteger} = max(CoeffsRawLengths)   % Max number of coeffs divided by NumFilters
    FrameSize (1,1) {mustBeInteger} = 8                         % FrameSize of the signal to be filtered
    IsCoeffsRawFlipped logical = false                          % If true, the CoeffsRaw are in flipped format
    AlwaysFlip logical = false                                  % If true, flip coefficients regardless of Breed

end

% Check NumFilters
if NumFilters ~= numel(CoeffsRawLengths)
    TranslateError('NumFilters should be equal to number of elements in CoeffsRawLengths');
end

TotalPaddedLength = NumFilters*MaxAvgCoeffLength;

%--------------------------------------------------------------------------
% Generate CoeffsLengthsTarget 
%--------------------------------------------------------------------------
CoeffsTargetLengths = GetCoeffsTargetLengths(CoeffsRawLengths, Breed, FrameSize);

%--------------------------------------------------------------------------
% Generate CoeffsTarget
%--------------------------------------------------------------------------
if (TotalPaddedLength) < sum(CoeffsTargetLengths)
    TranslateError('TotalPaddedLength must be greater than or equal to sum(CoeffsTargetLengths)');
end
CoeffsTarget = zeros(TotalPaddedLength,1,class(CoeffsRaw));

% The following code assumes that CoeffsLengthsTarget is greater than
% CoeffsLengthsRaw. Checking that this assumption is true.
if any(CoeffsRawLengths > CoeffsTargetLengths)
    TranslateError('Assumption that CoeffsLengthsRaw is less than or equal to CoeffsLengthsTarget is broken. Can not continue with Translation');
end

% Allocate temp coeff Buffer. Allocate memory equal to Longest CoeffLength
% Coeff = zeros(MaxCoeffLength, 1, class(CoeffsRaw));

% Hexagon doesn't require flipping.
if AlwaysFlip == true
    DoFlip = true;
elseif IsFirCoeffsFlipped(Breed) == false
    DoFlip = false;
else
    DoFlip = true;
end

% If Coefficients are already flipped on the input, no need to flip.
if IsCoeffsRawFlipped == true
    DoFlip = not(DoFlip);
end

% Initialize Counters at 1
CoeffsRawStartingIdx = 1;
CoeffsTargetStartingIdx = 1;
% Loop through each filter then flip and copy values into output
% array.
for FilterIdx = 1:NumFilters
    CoeffsTargetEndingIdx = CoeffsTargetStartingIdx + double(CoeffsTargetLengths(FilterIdx))-1;
    % Copy over values to CoeffsTarget and do flipping if necessary
    if DoFlip == true
        % Put CoeffsRaw Coefficients into Coeff Buffer [[---CoeffsRaw ---] 0 0 0 0 ... 0 0 0]
        % Copy (a section of Coeff Buffer) and Flip Into CoeffsTarget [0 0 0 0 [---Flipped CoeffsRaw---]]
        CoeffsTarget(CoeffsTargetEndingIdx - CoeffsRawLengths(FilterIdx) + 1: CoeffsTargetEndingIdx) = flip(CoeffsRaw(CoeffsRawStartingIdx: CoeffsRawStartingIdx + CoeffsRawLengths(FilterIdx) - 1));
    else
        % Put CoeffsRaw Coefficients into Coeff Buffer [0 0 0 0 [---CoeffsRaw (Flipped)---] ... 0 0 0]
        % Copy (a section of Coeff Buffer) Into CoeffsTarget without flipping [0 0 0 0 [---CoeffsRaw (Flipped)---]]
        CoeffsTarget(CoeffsTargetEndingIdx - CoeffsRawLengths(FilterIdx) + 1: CoeffsTargetEndingIdx) = CoeffsRaw(CoeffsRawStartingIdx: CoeffsRawStartingIdx + CoeffsRawLengths(FilterIdx) - 1);
    end
    % Increment Starting Index
    CoeffsTargetStartingIdx = CoeffsTargetStartingIdx + double(CoeffsTargetLengths(FilterIdx));
    CoeffsRawStartingIdx = CoeffsRawStartingIdx + double(CoeffsRawLengths(FilterIdx));
end
end

function CoeffsTargetLengths = GetCoeffsTargetLengths(CoeffsRawLengths, Breed, FrameSize)
%--------------------------------------------------------------------------
% Generate CoeffsLengthsTarget 
%--------------------------------------------------------------------------

% Get Minimum CoeffsLengths
if any(Breed == [AsdHwEnum.Hexagon, AsdHwEnum.ArmNeonV7V8])
    MinimumCoeffsLengths = FrameSize;
else
    MinimumCoeffsLengths = 0;
end 

% For these Breeds, CoeffsLengths must be a multiple of 4
if any(Breed == [AsdHwEnum.Hexagon, AsdHwEnum.ArmNeonV7V8])
    CoeffLengthsMultiple = 4;
else
    CoeffLengthsMultiple = 1;
end

% Apply Requirements on CoeffsLengths: Require CoeffsLengths to be multiple
% a number and have a minimum value
CoeffsTargetLengths = CoeffLengthsMultiple*ceil(max(CoeffsRawLengths, MinimumCoeffsLengths)/CoeffLengthsMultiple);

% For These Breeds, CoeffsLengths are always equal
if any(Breed == [AsdHwEnum.Hexagon, AsdHwEnum.ArmNeonV7V8])
    CoeffsTargetLengths(:) = max(CoeffsTargetLengths); 
end
end

