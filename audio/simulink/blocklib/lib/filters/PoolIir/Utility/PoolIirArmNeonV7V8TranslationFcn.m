function [PoolNumStages, PoolCoeffs] = PoolIirArmNeonV7V8TranslationFcn(rawSOS, numStages, maxNumStages)
% Shares Translation Fcn with Hexagon

% ARM expects biquad coefficiients in following order:
%   [b0, b1, a1/a0, b2, a2/a0] where a0 is assumed to be 1. 
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].

NumCoeffPerBiquad = 5;
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
    
for i = 1:NumChannels
    
    ChannelNumStages = NumStages(i);   % Size of current SOS
    assert(ChannelNumStages <= MaxNumStages);
    if (ChannelNumStages == 0)
        continue;
    end

    ChannelSOS = RawSOS(SOSIndex+(1:ChannelNumStages),:);
    SOSIndex = SOSIndex + ChannelNumStages;
    
    % decompose SOS
    % [b0, b1, a1/a0, b2, a2/a0] ARM biquad coefficiients 
    ts = (1:NumCoeffPerBiquad:(NumCoeffPerBiquad*ChannelNumStages)) - 1;
    ChannelCoeffs(ts + 1) =  ChannelSOS(:, 1); % b0
    ChannelCoeffs(ts + 2) =  ChannelSOS(:, 2); % b1
    ChannelCoeffs(ts + 3) =  ChannelSOS(:, 5) ./ ChannelSOS(:, 4); % a1/a0
    ChannelCoeffs(ts + 4) =  ChannelSOS(:, 3); % b2
    ChannelCoeffs(ts + 5) =  ChannelSOS(:, 6) ./ ChannelSOS(:, 4); % a2/a0
    
    % length of Current Channel Coeffs
    ChannelCoeffsLength = NumCoeffPerBiquad*ChannelNumStages;
    
    % Offset into coefficient array for the current channel
    offset = NumCoeffPerBiquad*sum(PoolNumStages(1:i-1));    
    PoolNumStages(i) = int32(ChannelNumStages);
    % Store the coefficients
    PoolCoeffs(offset+(1:ChannelCoeffsLength)) = single(ChannelCoeffs(1:ChannelCoeffsLength)); 
end