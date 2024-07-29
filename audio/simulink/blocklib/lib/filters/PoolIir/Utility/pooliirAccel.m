function y = pooliirAccel(x, frameSize, numChannels, numStages, coeffs)
%
% Simulation of the IIR Accelerator with one frame delay

% Accelerator expects biquad coefficiients in following order:
%   [b0, b1, -a1, b2, -a2, dk2, dk1]
% In Matlab, coefficients are ordered as:
%   [b0, b1, b2, a0, a1, a2].
%
% The mappings from SHARC biquads to Matlab coefficients are:
%    Bs = [1, 2, 4]
%    As = [3, 5]
%    Ds = [6, 7]

% State buffer for holding two states per stage
persistent stateBuffer;
if isempty(stateBuffer)
    stateMemSize = 2 * sum(numStages);

    % Temporary work around for unbounded buffer size error during code 
    % generation of this file when running Talaria partition_and_generate() and
    % building generated <model_name>_Target.slx models. Note that this file is
    % only for simulation and should never be used for code generation.
    % Maximum Accelerator State Memory Size = 
    %                       (2 states/SOS) * (64 biquad max/chan) * (128 chan)
    if stateMemSize > (2 * 64 * 128)
        error('Exceeded maximum size in code generation work around');
    end

    stateBuffer = zeros(1,stateMemSize,'single');
end

% Initialize variables
persistent delayedOut;    % IIR accelerator frame delayed output
if isempty(delayedOut)
    delayedOut = zeros(frameSize,numChannels,'single');
end

% Introduce a one frame delay to simulate accelerator behavior
y = delayedOut;

stateIdx = uint32(1);
coeffIdx = uint32(0);
y0 = zeros(frameSize,1,'single');

% Outer loop over all channels                 
for cha = 1:numChannels
    
    % Number of SOS stages for this channel
    channelNumStages = numStages(cha);
    
    % Input signal for this channel
    x0 = x(:, cha);
    for stage = 1:channelNumStages            
      
      % Coeffs for this stage
      coeffOffset = coeffIdx + ((stage-1) * 7);
      b0 = coeffs(coeffOffset + 1);
      b1 = coeffs(coeffOffset + 2);
      b2 = coeffs(coeffOffset + 4);
      a1 = -1 * coeffs(coeffOffset + 3);
      a2 = -1 * coeffs(coeffOffset + 5);
      b = [b0; b1; b2];      
      a = [1; a1; a2]; 
      
      % State for this stage
      state = stateBuffer(stateIdx : stateIdx+1);      
      
      % Compute output samples and state
      [y0, state] = filter(b, a, x0, state);
      
      % Cascade stages
      x0 = y0;
      
      % Save state
      stateBuffer(stateIdx : stateIdx+1) = state;
      stateIdx = stateIdx + 2;
    end
    
    % Output signal for this channel
    delayedOut(:, cha) = y0;
    coeffIdx = coeffIdx + channelNumStages * 7;
end
   
end


