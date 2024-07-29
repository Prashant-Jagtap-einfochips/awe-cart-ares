function [PoolNumStages, PoolCoeffs] = poolIirTranslation(rawSOS, numStages, maxNumStages, Breed)
% Perform platform-based translation for poolIir block
%
% This functions offers the following outputs:
%   PoolNumStages: Number of active stages for each channel
%   PoolCoeffs   : SOS coefficients in a format accepted by CRL (can be
%                  different for different platform CRLs)
%                  Hexagon (qhdsp_iir_af_bose.S) and ARM format - [b0, b1, a1, b2, a2]        [NumStages x 5, 1]
%                  Hexagon (flt_iir_cas_bq.S) DF-II format [-a1/a0 b1/b0 -a2/a0 b2/b0] plus gain per channel.
%                                             where a0 is assumed to be 1.0. [TotNumStages x 4 + NumChannel, 1]
%                  SHARC plus processors - [k, b1, b2, a1, a2]          [NumStages x 4, 1]                  
%
% This function expects the following inputs:
%   rawSOS      : Unprocessed SOS coefficients in the order - [b0, b1, b2, a0, a1, a2]
%                 Dimensions - [NumStages x NumChannels, 6]
%   numStages   : Number of active stages for each channel
%                 Dimensions - [1 x NumChannels]
%   maxNumStages: Maximum permissible number of stages for each channel
%                 Dimensions - [1 x NumChannels]
%   Breed:        Platform Breed - AsdHwEnum Type
% 
% If rawSOS and numStages are not being supplied from a TuneVar directly,
% please change it to TuneVar output type as shown below:
% rawSOS = struct('Value', rawSosCoeffs);

switch (Breed)
    case {AsdHwEnum.Generic, AsdHwEnum.SharcPlusCore}
        % These Breeds order biquad coefficients in a compacted form
        [PoolNumStages, PoolCoeffs] = pooliirTranslationFcn(rawSOS, numStages, maxNumStages);
    case {AsdHwEnum.SharcPlusAccelGul, AsdHwEnum.SharcPlusAccelGxp}
        % These Breeds order biquad coefficients for the ADI accelerator
        [PoolNumStages, PoolCoeffs] = pooliirAccelTranslationFcn(rawSOS, numStages, maxNumStages);
    case{AsdHwEnum.Hexagon}
        % Hexagon specific order for biquad coefficients 
        [PoolNumStages, PoolCoeffs] = PoolIirHexagonTranslationFcn(rawSOS, numStages, maxNumStages);
    case{AsdHwEnum.ArmNeonV7V8}
        [PoolNumStages, PoolCoeffs] = PoolIirArmNeonV7V8TranslationFcn(rawSOS, numStages, maxNumStages);
    otherwise
        % This is a build-time error, not a translation error. The
        % selected build-time Breed is not supported for translation
        error('Invalid implementation type for pool IIR: [%s]', Breed);
end

end

