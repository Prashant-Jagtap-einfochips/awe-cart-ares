function audioOut = AllpassFilter(audioIn,p)
% MATH:
%   The 2nd-order APF is realized through 2 cascaded 1st-order APF's:
%
%   W(z)/X(z) = (a1+z^-1)/(1+a1*z^-1)
%   Y(z)/W(z) = (a2+z^-1)/(1+a2*z^-1)
%
%   w[n] = a1*x[n] + x[n-1] - a1*w[n-1]
%   y[n] = a2*w[n] + w[n-1] - a2*y[n-1]
%
%   In Matlab implementation, the above equations are realized by calling
%   the built-in "filter" function, with associated state updates.
%   
%   In C implementation, we also use the cascaded 1st-order sections.
%   The filter topology is transposed direct form II:
%
%   
%                |\                w[n]         |\
% x[n] ----->+---| >-- + ------->+--------->+---| >-- + ------->+---> y[n]
%           |    |/    ^         |         |    |/    ^         | 
%           |     a1   |         |         |     a2   |         | 
%           |         tn1        |         |         vn1        | 
%           |         ---        |         |         ---        | 
%           |        |   |       |         |        |   |       | 
%           |         ---  -a1   |         |         ---  -a2   | 
%           v          |     /|  v         v          |     /|  v 
%           +----------+----< |--+         +----------+----< |--+ 
%                            \|                             \|    
%
%
%   This form minimizes the amount state and also keeps the state
%   reasonably bounded (provided that input and output are bounded). 
%   We can rewrite the equations as coupled difference equations
%   based on the above topology.
% 
%   Compute the output based on current input and states:
%
%   w[n] = a1*x[n] + tn1
%   y[n] = a2*w[n] + vn1
%   
%   Then update states for the next iteration:
%
%   tn1  = x[n] - a1*w[n];
%   vn1  = w[n] - a2*y[n];

%   Copyright 2019 Bose Corporation
persistent filterStates;

% figure out the frame size and channel count
[frame_size, channel_count] = size(audioIn);

if (isempty(filterStates))
    filterStates = zeros(1,2*channel_count,'single');
end

a1 = p(1);
a2 = p(2);
audioOut = coder.nullcopy(audioIn);
stateIdx = 1;

for chanIdx = 1:channel_count
    tn1 = filterStates(stateIdx); stateIdx = stateIdx+1;
    vn1 = filterStates(stateIdx); stateIdx = stateIdx-1;
    for n = 1:frame_size
        wn = audioIn(n,chanIdx)*a1 + tn1;
        yout = wn*a2 + vn1;
        
        tn1 = audioIn(n,chanIdx) - (wn*a1);
        vn1 = wn - (yout*a2);
        audioOut(n,chanIdx) = yout;
    end
    filterStates(stateIdx) = tn1; stateIdx = stateIdx+1;
    filterStates(stateIdx) = vn1; stateIdx = stateIdx+1;
end
