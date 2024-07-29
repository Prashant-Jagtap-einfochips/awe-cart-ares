function [PoolNumStages, PoolCoeffs] = pooliirTranslationFcn(rawSOS, numStages, maxNumStages)

RawSOS = rawSOS.Value;
NumStages = int32(numStages.Value);
NumChannels = length(NumStages);

MaxNumStages = maxNumStages; 
PoolNumStages = int32(zeros(NumChannels,1));
PoolCoeffs = single(zeros(MaxNumStages * 4 + NumChannels,1));
ChannelSOS = single(zeros(MaxNumStages, 6));
SOSIndex = int32(0);

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
    
    ChannelSOS(1:ChannelNumStages,:) = RawSOS(SOSIndex+(1:ChannelNumStages),:);
    SOSIndex = SOSIndex + ChannelNumStages;

    if size(ChannelSOS,2) ~= 6
        TranslateError('SOS matrix must have 6 columns');
    end

    % decompose SOS
    ChannelCoeffs = sos2pooliir(ChannelSOS(1:ChannelNumStages, :));

    % length of Cuurent Channel Coeffs
    
    ChannelCoeffsLength = 1+4*ChannelNumStages;
    
    % Offset into coefficient array for the current channel
    offset = (i-1)+4*sum(PoolNumStages(1:i-1));
    
    PoolNumStages(i) = int32(ChannelNumStages);

    % Store the coefficients
    PoolCoeffs(offset+(1:ChannelCoeffsLength)) = single(ChannelCoeffs(1:ChannelCoeffsLength));   
end



