function [OutputFinal, StateFinal] = SosFilter(InputInitial, SosFilter, InitialState)
% Implements single channel IIR with cascaded second order stages (SOS)
% Inputs
%   -InputInitial [FrameSize x 1] 1-D Input signal
%   -SosFilter [NumStages x 6] SOS Coefficients [b0, b1, b2, a0, a1, a2]
%       for each Stage
%   -InitialState [NumStages x 2] Buffer to store states. Two states per
%       stage
%
% Outputs:
%   -OutputFinal [FrameSize x 1] The final output after the applying the
%       SOS filters
%   -StateFinal [NumStages x 2] Final state buffer. To be used in
%       processing of the next frame.
%--------------------------------------------------------------------------


% Input buffers for each Sos Stage
InputForSosStage = single(InputInitial);
% Output buffers for each Sos Stage
OutputForSosStage = zeros(size(InputInitial),'single');

% States = [v0[n-1], v0[n-2]; 
%           V1[n-1], V1[n-2] ....]
States = InitialState;
NumStages = height(SosFilter);

for FilterIdx = 1:NumStages
    % Get Coefficients
    % [b0, b1, b2, a0, a1, a2].
    b0 = SosFilter(FilterIdx,1);
    b1 = SosFilter(FilterIdx,2);
    b2 = SosFilter(FilterIdx,3);    
    a0 = SosFilter(FilterIdx,4);
    a1 = SosFilter(FilterIdx,5);
    a2 = SosFilter(FilterIdx,6);

    % Reset Output Buffer
    OutputForSosStage(:) = 0;       

    % Calculate Output for this Stages
    for n = 1:numel(OutputForSosStage)
        
        % Calculate Output With Transposed Direct Form II
        % y[n] = (b0*x[n] + s1[n-1])/a0
        OutputForSosStage(n) = (b0*InputForSosStage(n) + States(FilterIdx,1))/a0;
        
        % Update States for S1[n] 
        % S1[n] = S2[n-1] + b1*x1[n] - a1*y[n]
        States(FilterIdx,1) = States(FilterIdx,2) + b1*InputForSosStage(n) - a1*OutputForSosStage(n);
        
        % Update States for S2[n] 
        % S2[n] = b2*x[n] - a2*y[n]
        States(FilterIdx,2) = b2*InputForSosStage(n) - a2*OutputForSosStage(n);



    end
    % Output Becomes Input for the next stage
    InputForSosStage = OutputForSosStage;
end
OutputFinal = OutputForSosStage;
StateFinal = States;
end
