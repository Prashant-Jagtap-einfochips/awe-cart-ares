function [PoolNumStages, PoolCoeffs] = PoolIirHexagonTranslationFcn(rawSOS, numStages, maxNumStages)

% Hexagon flt_iir_cas_bq.S expects biquad coefficiients in following order:
%   [-a1/a0 b1/b0 -a2/a0 b2/b0] where a0 is assumed to be 1.
%   Overall gain b0/a0 is appended at the end of PoolCoeffs per channel. 
%
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].

NumCoeffPerBiquad = 4;
RawSOS        = rawSOS.Value;
NumStages     = int32(numStages.Value);
NumChannels   = numel(NumStages);
MaxNumStages  = maxNumStages; 
PoolNumStages = int32(zeros(NumChannels,1));
PoolCoeffs    = single(zeros(MaxNumStages * NumCoeffPerBiquad, 1));
ChannelCoeffs = zeros(MaxNumStages * NumCoeffPerBiquad, 1);
SOSIndex      = int32(0);

% Verify that the SOS does not violate the maximum number of stages
% allocated for the pool
if sum(NumStages) > MaxNumStages
    TranslateError('Too many second order sections');
end

OverallGainPerCh = ones(NumChannels, 1);
for i = 1:NumChannels
    
    ChannelNumStages = NumStages(i);   % Size of current SOS
    assert(ChannelNumStages <= MaxNumStages);
    if (ChannelNumStages == 0)
        continue;
    end

    ChannelSOS = RawSOS(SOSIndex+(1:ChannelNumStages),:);
    SOSIndex = SOSIndex + ChannelNumStages;
    
    % decompose SOS    
    OverallGainPerCh(i) = prod(ChannelSOS(:, 1)) / prod(ChannelSOS(:, 4));
    
    % If a0 == 0
    if any(ChannelSOS(:, 4) == 0 )
        TranslateError('Sos a0 coefficients must not be zero');
    end

    % [a1 b1/b0 a2 b2/b0] Hexagon assembly code 
    ts = (1:NumCoeffPerBiquad:(NumCoeffPerBiquad*ChannelNumStages)) - 1;
    
    % Check for zero channel gain. If zero, then likely a divide by zero
    % may occur during translation. Zero Overall gain means channel is
    % muted. Set Channel Coeffs to zero.
    if OverallGainPerCh(i) == 0
        ChannelCoeffs(ts + 2) = 0;  %  b1/b0
        ChannelCoeffs(ts + 4) = 0;  %  b2/b0
    else
        ChannelCoeffs(ts + 2) =  ChannelSOS(:, 2) ./ ChannelSOS(:, 1);  %  b1/b0
        ChannelCoeffs(ts + 4) =  ChannelSOS(:, 3) ./ ChannelSOS(:, 1);  %  b2/b0
    end
    ChannelCoeffs(ts + 1) = -ChannelSOS(:, 5) ./ ChannelSOS(:, 4);  % -a1/a0
    ChannelCoeffs(ts + 3) = -ChannelSOS(:, 6) ./ ChannelSOS(:, 4);  % -a2/a0


    % length of Current Channel Coeffs
    ChannelCoeffsLength = NumCoeffPerBiquad*ChannelNumStages;
    
    % Offset into coefficient array for the current channel
    offset = NumCoeffPerBiquad*sum(PoolNumStages(1:i-1));    
    PoolNumStages(i) = int32(ChannelNumStages);

    % Store the coefficients
    PoolCoeffs(offset+(1:ChannelCoeffsLength)) = single(ChannelCoeffs(1:ChannelCoeffsLength));   
end

PoolCoeffs = [PoolCoeffs; OverallGainPerCh];
