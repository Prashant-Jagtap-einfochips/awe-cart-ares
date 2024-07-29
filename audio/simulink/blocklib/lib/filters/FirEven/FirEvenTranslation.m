function FirEvenCoeffsTarget = FirEvenTranslation(FirEvenCoeffs, Breed, MaxNumFilters, MaxAvgCoeffLength, FrameSize, IsCoeffsRawFlipped, AlwaysFlip)
arguments
    FirEvenCoeffs (:,:)
    Breed AsdHwEnum = AsdHwEnum.Generic
    MaxNumFilters {mustBeInteger} = size(FirEvenCoeffs,2);
    MaxAvgCoeffLength {mustBeInteger} = size(FirEvenCoeffs,1);
    FrameSize {mustBeInteger} = 8;
    IsCoeffsRawFlipped logical = false;
    AlwaysFlip logical = false
end

CoeffsRawLengths = MaxAvgCoeffLength*ones(MaxNumFilters,1);

% Temporary buffer of max coeff size, stored with coeffs transposed
CoeffsTemp = zeros(MaxAvgCoeffLength, MaxNumFilters);
CoeffsTemp(1:size(FirEvenCoeffs,1),1:size(FirEvenCoeffs,2)) = FirEvenCoeffs;
FirEvenCoeffsTarget = reshape(  FirTranslation(CoeffsTemp(:), CoeffsRawLengths, Breed, MaxNumFilters, MaxAvgCoeffLength, FrameSize, IsCoeffsRawFlipped, AlwaysFlip),...
                                [MaxAvgCoeffLength, MaxNumFilters]);

end