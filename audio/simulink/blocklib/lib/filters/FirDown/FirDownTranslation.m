function [CoeffsTarget] = FirDownTranslation(CoeffsRaw, DownFactor, Breed, FrameSize)
% Translates Raw FirDown Filter Coefficients into Target Compatible formats
% Inputs:
%   CoeffsRaw: [1D array] Raw Coefficients, also the Fir Coeffs for
%       Simulation
%   DownFactor: [int] Downsampling Factor
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
    DownFactor {mustBeInteger} = 4
    Breed AsdHwEnum = AsdHwEnum.Simulink
    FrameSize {mustBeInteger} = 8
end

% Reuse the FirUpTranslation function
[CoeffsTarget] = FirUpTranslation(CoeffsRaw, DownFactor, Breed, FrameSize);

end