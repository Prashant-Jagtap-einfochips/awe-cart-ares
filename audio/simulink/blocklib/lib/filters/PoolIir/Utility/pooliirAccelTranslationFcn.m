function [PoolNumStages, PoolCoeffs] = pooliirAccelTranslationFcn(rawSOS, numStages, maxNumStages)

% Accelerator expects biquad coefficiients in following order:
%   [b0, b1, -a1, b2, -a2, dk2, dk1]
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].

RawSOS        = rawSOS.Value;
NumStages     = int32(numStages.Value);
NumChannels   = numel(NumStages);
MaxNumStages  = maxNumStages; 
PoolNumStages = int32(zeros(NumChannels,1));
PoolCoeffs    = single(zeros(MaxNumStages * 7, 1));
ChannelCoeffs = zeros(MaxNumStages * 7, 1);
SOSIndex      = int32(0);

% Verify that the SOS does not violate the maximum number of stages
% allocated for the pool
if sum(NumStages) > MaxNumStages
    TranslateError('Too many second order sections');
end
    
for i = 1:NumChannels
    
    ChannelNumStages = NumStages(i);   % Size of current SOS
    assert(ChannelNumStages <= MaxNumStages);
    if (ChannelNumStages == 0)
        continue;
    end

    ChannelSOS = RawSOS(SOSIndex+(1:ChannelNumStages),:);
    SOSIndex = SOSIndex + ChannelNumStages;
    
    % decompose SOS
    ts = (1:7:(7*ChannelNumStages)) - 1;
    ChannelCoeffs(ts + 1) =  ChannelSOS(:, 1); % b0
    ChannelCoeffs(ts + 2) =  ChannelSOS(:, 2); % b1
    ChannelCoeffs(ts + 3) = -1 * ChannelSOS(:, 5) ./ ChannelSOS(:, 4); % -a1/a0
    ChannelCoeffs(ts + 4) =  ChannelSOS(:, 3); % b2
    ChannelCoeffs(ts + 5) = -1 * ChannelSOS(:, 6) ./ ChannelSOS(:, 4); % -a2/a0
    ChannelCoeffs(ts + 6) =  0; % dk2
    ChannelCoeffs(ts + 7) =  0; % dk1
    
    % length of Current Channel Coeffs
    ChannelCoeffsLength = 7*ChannelNumStages;
    
    % Offset into coefficient array for the current channel
    offset = 7*sum(PoolNumStages(1:i-1));
    
    PoolNumStages(i) = int32(ChannelNumStages);

    % Store the coefficients
    PoolCoeffs(offset+(1:ChannelCoeffsLength)) = single(ChannelCoeffs(1:ChannelCoeffsLength));   
end



