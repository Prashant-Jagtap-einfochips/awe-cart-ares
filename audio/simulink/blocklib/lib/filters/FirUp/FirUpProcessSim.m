function Output = FirUpProcessSim(Input, Coeffs, UpFactor, CoeffsInPolyphaseFormat, Breed)
% Upsample Input signal followed by Fir Filter. Implemented using
% Polyphase Fir Filter
% Inputs:
%   Input:      [FrameSize x NumChannels] Lower Rate Input Signal
%   Coeffs:     [1d array of size CoeffLength] Fir Filter Coefficients
%   UpFactor:   [int] Upsampling Factor
%   CoeffsInPolyphaseFormat: [logical] Coefficients not expected in
%       polyphase format, if it is, turn on this flag so that code can
%       translate coeffs back into standard format.

%--------------------------------------------------------------------------

arguments
    Input
    Coeffs
    UpFactor {mustBeInteger} = 4
    CoeffsInPolyphaseFormat logical = false
    Breed AsdHwEnum = AsdHwEnum.Generic
end

[FrameSize, NumChannels] = size(Input);
Output = single(zeros(FrameSize*UpFactor, NumChannels));
CoeffLength = numel(Coeffs);
MaxSubFilterLength = ceil(CoeffLength/UpFactor);

% Padding to ensure that all Subfilters are the same length
PaddedCoeffLength = MaxSubFilterLength*UpFactor;
PaddedCoeffs = [Coeffs(:); zeros(PaddedCoeffLength - CoeffLength,1)];

% Translate from Polyphase format to standard
if CoeffsInPolyphaseFormat == true
    PaddedCoeffs = TranslateCoeffsPolyphaseToStandard(PaddedCoeffs, UpFactor);
end

persistent States 
if isempty(States)
    States = single(zeros(PaddedCoeffLength, NumChannels));
end
for ch = 1 : NumChannels
    for k = 0: UpFactor - 1
        % Get indices for kth Subfilter
        SubFilterIndices = 1 + (0:MaxSubFilterLength-1)*UpFactor + k;
        % Get Coeffs of k-th Subfilter
        SubFilterCoeffs = PaddedCoeffs(SubFilterIndices);
        % Coefficient Format for these Breed are not Flipped, but
        % simulation fcn expects flipped coeffs. Flip coeffs before running
        % FirFilter.
        if IsFirCoeffsFlipped(Breed) == false
            SubFilterCoeffs = flipud(SubFilterCoeffs);
        end
        % Compute Fir
        [SubFilterOutput, SubFiltersStates] = FirFilter(SubFilterCoeffs, Input(:,ch), States(SubFilterIndices, ch), false);
        % Save States
        States(SubFilterIndices,ch) = SubFiltersStates(:);
        % Upsample, Delay and Accumulate
        OutputIndices = ((1 + k):UpFactor:(FrameSize*UpFactor)); 
        Output(OutputIndices , ch) = Output(OutputIndices , ch) + SubFilterOutput(:);
    end
end
end

function CoeffsStandard = TranslateCoeffsPolyphaseToStandard(CoeffsPolyphase, UpFactor)
    % Undoes Coefficient Translation. Returns coefficients in a standard
    % format used in simulation.
    
    CoeffsStandard = zeros(size(CoeffsPolyphase),class(CoeffsPolyphase));
    % Undo Polyphase reordering
    MaxSubFilterLength = numel(CoeffsPolyphase)/UpFactor;
    for k = 0: UpFactor - 1
        SubFilterIndices = 1 + (0:MaxSubFilterLength-1)*UpFactor + k;
        CoeffsStandard(SubFilterIndices) = CoeffsPolyphase(k*MaxSubFilterLength + (1:MaxSubFilterLength));
    end
end

