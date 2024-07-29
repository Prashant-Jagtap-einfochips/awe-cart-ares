function [Output] = PoolIirHexagonSimFcn(Input, NumChannels, NumStages, CoeffsTarget, StatesIn)

% Hexagon flt_iir_cas_bq.S expects biquad coefficiients in following order:
%   [-a1/a0 b1/b0 -a2/a0 b2/b0] where a0 is assumed to be 1. Overall gain b0/a0 is
%   appended at the end of PoolCoeffs per channel.  
%
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].

persistent States;
if isempty(States)
    States = zeros(sum(NumStages),2,'single');
end

% extract the overall gain per channel  
OverallGainPerCh = CoeffsTarget(end-NumChannels+1:end);
NumCoeffPerBiquad = 4;
tmp = CoeffsTarget;
CoeffsTarget = [];

% remove the overall gain per channel appended  
CoeffsTarget = tmp(1:end-NumChannels);

CoeffsHexagon = reshape(CoeffsTarget, ...
                        NumCoeffPerBiquad, ...
                        numel(CoeffsTarget)/NumCoeffPerBiquad)';

% Reconstruct standard 6-by-NumStages SOS coefficients
CoeffsMatlab = zeros(height(CoeffsHexagon), 6,'single');
CoeffsMatlab(:, 1) =  single(1.0);             % b0
CoeffsMatlab(:, 5) = -CoeffsHexagon(:, 1);     % a1
CoeffsMatlab(:, 2) =  CoeffsHexagon(:, 2);     % b1
CoeffsMatlab(:, 4) =  single(1.0);             % a0
CoeffsMatlab(:, 3) =  CoeffsHexagon(:, 4);     % b2
CoeffsMatlab(:, 6) = -CoeffsHexagon(:, 3);     % a2 
Output = zeros(size(Input),'single');

for ch = 1: NumChannels
    EndIdx = sum(NumStages(1:ch));
    StartIdx = 1 + EndIdx - NumStages(ch);

    % Re-apply the overall gain per channel to the input 
    [FiltOut, FiltStates] = SosFilter(Input(:,ch) * OverallGainPerCh(ch),...
                                      CoeffsMatlab(StartIdx:EndIdx,:), ...
                                      States(StartIdx:EndIdx,:));
    Output(:,ch) = FiltOut;
    States(StartIdx:EndIdx,:) = FiltStates;
end
end