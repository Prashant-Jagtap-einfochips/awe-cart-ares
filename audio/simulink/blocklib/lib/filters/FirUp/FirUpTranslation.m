function [CoeffsTarget] = FirUpTranslation(CoeffsRaw, UpFactor, Breed, FrameSize, MaxFilterLength)
% Translates Raw FirUp Filter Coefficients into Target Compatible formats
% Inputs:
%   CoeffsRaw: [1D array] Raw Coefficients, also the Fir Coeffs for
%       Simulation
%   UpFactor: [int] Upsampling Factor
%   Breed: [AsdHwEnum Type] (Optional) Breed Type associated
%       with target.
%   FrameSize [int]
%   
% Outputs:
%   CoeffsTarget: [1D array] Coefficients in Target Specific Format
%
%
% Translation Summary -----------------------------------------------------
% -Polyphase: Reorders Coefficients by Polyphase Subfilter, instead of a
%   standard format [h[0], h[1], h[3] ... ]. All coefficients of the first
%   polyphase subfilter come first, followed by coefficients of the second
%   polyphase subfilter, followed by the next...
%   
% -Hexagon:
%   -Zero Padding to increase size of Coefficients in order to be a
%   multiple of UpFactor*4.
%   -Reorder Coefficients by polyphase subfilter. (See Polyphase)
% ArmNeonV7V8:
%   -Same as hexagon
%--------------------------------------------------------------------------

arguments
    CoeffsRaw
    UpFactor {mustBeInteger} = 4
    Breed AsdHwEnum = AsdHwEnum.Simulink
    FrameSize {mustBeInteger} = 8
    MaxFilterLength {mustBeInteger} = 0
end



if (Breed == AsdHwEnum.Simulink)
    CoeffsTarget = CoeffsRaw;
else
    % CoeffsTarget = [<SubFilter1> <SubFilter2> ... <SubFilter[D]> ]
    % Example for UpFactor = 4
    % CoeffsTarget = [     h0, h4, h8,  ... 
    %                      h1, h5, h9,  ... 
    %                      h2, h6, h10, ... 
    %                      h3, h7, h11, ... ]

    % Calculate default SubFilter length
    SubFilterLength = ceil(numel(CoeffsRaw)/UpFactor);

    % Pad SubFilterLength depending on Breed
    if (Breed == AsdHwEnum.Hexagon) || (Breed == AsdHwEnum.ArmNeonV7V8)
        % Force SubFilterLength to be greater than FrameSize
        SubFilterLength = max(SubFilterLength, FrameSize);
        % Force SubFilterLength to be a Multiple of Four
        SubFilterLength = GetNextMultiple(SubFilterLength, 4);
    end
    
    % Pad CoeffsRaw
    CoeffsRaw = PadArray(CoeffsRaw,SubFilterLength*UpFactor);
    CoeffsTarget = zeros(SubFilterLength*UpFactor,1,class(CoeffsRaw));

    % Loop through each each subfilter and assign its coefficients
    for k = 0: UpFactor-1
        SubFilterIndices = 1 + (0:SubFilterLength-1)*UpFactor + k;
        TargetCoeffIndicies = SubFilterLength*k + (1:SubFilterLength);
        CoeffsTarget(TargetCoeffIndicies) = CoeffsRaw(SubFilterIndices);
        CoeffsTarget(TargetCoeffIndicies) = FirTranslation(CoeffsTarget(TargetCoeffIndicies), SubFilterLength, Breed, 1, SubFilterLength, FrameSize, false);                
    end
end
end



function Padded = PadArray(Arr, Length)
% Zero Padd the array up to a length specified by [Length]
    if Length <= numel(Arr)
        Padded = Arr;
    else
        Padded = [Arr(:); zeros(Length - numel(Arr), 1, class(Arr))];
    end
end
function NextMult = GetNextMultiple(Num, Factor)
% Return the Smallest Multiple of [Factor] that is greater than [Num]
    NextMult = Num + Factor - mod(Num,Factor);
end