function IsFlipped = IsFirCoeffsFlipped(Breed)
% Returns true or false if the Fir CRL coefficients for a specific Breed
% requires flipping. If flipping requirement changes for FIR, simply change
% this file and this will update behavior of FirUp, FirGeneric and FirEven
% blocks. 
% For Hexagon, two different libraries are used that have different
% requirements for flipping. To ensure that flipping requirements are
% consistent within Breeds. The CRL wrapper code for blk_fir function will
% unflip the coefficients.
%   -qhdsp_fir_af: Library Requires Coeffs to be Flipped.
%   -blk_fir: Library requires Coeffs Not to be Flipped. CRL wrapper will
%       undo flipping.
if any(Breed == [])
    IsFlipped = false;
elseif any(Breed == [AsdHwEnum.Hexagon, AsdHwEnum.Generic, AsdHwEnum.SharcPlus, AsdHwEnum.SharcPlusCore, AsdHwEnum.SharcPlusAccel, AsdHwEnum.ArmNeonV7V8])
    IsFlipped = true;
else
    error("%s is not a supported Breed for Fir Generic.", string(Breed) );
end

end

