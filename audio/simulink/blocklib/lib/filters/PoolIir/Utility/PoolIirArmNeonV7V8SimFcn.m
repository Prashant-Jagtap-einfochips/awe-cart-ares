function [Output] = PoolIirArmNeonV7V8SimFcn(Input, NumChannels, NumStages, CoeffsTarget, StatesIn)
% ARM expects biquad coefficiients in following order:
%   [b0, b1, a1/a0, b2, a2/a0] where a0 is assumed to be 1. 
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].

persistent States;
if isempty(States)
    States = zeros(sum(NumStages),2,'single');
end

NumCoeffPerBiquad = 5;

CoeffsARM = reshape(CoeffsTarget, ...
                        NumCoeffPerBiquad, ...
                        numel(CoeffsTarget)/NumCoeffPerBiquad)';

% Reconstruct standard 6-by-NumStages SOS coefficients
CoeffsMatlab = zeros(height(CoeffsARM), 6,'single');
CoeffsMatlab(:, 1) =  CoeffsARM(:,1);     % b0
CoeffsMatlab(:, 5) =  CoeffsARM(:,3);     % a1
CoeffsMatlab(:, 2) =  CoeffsARM(:,2);     % b1
CoeffsMatlab(:, 4) =  single(1.0);        % a0
CoeffsMatlab(:, 3) =  CoeffsARM(:,4);     % b2
CoeffsMatlab(:, 6) =  CoeffsARM(:,5);     % a2 
Output = zeros(size(Input),'single');

for ch = 1: NumChannels
    EndIdx = sum(NumStages(1:ch));
    StartIdx = 1 + EndIdx - NumStages(ch);

    % Re-apply the overall gain per channel to the input 
    [FiltOut, FiltStates] = SosFilter(Input(:,ch),CoeffsMatlab(StartIdx:EndIdx,:), ...
                                      States(StartIdx:EndIdx,:));
    Output(:,ch) = FiltOut;
    States(StartIdx:EndIdx,:) = FiltStates;
end
end