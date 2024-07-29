function y = pooliirCore(x, frameSize, numChannels, numStages, coeffs, states2)
% If code gen'ing, generate warning if this is not replaced with pooliir.asm
coder.replace('-warnifnoreplacement');

% We have to use a different state buffer than the one passed in.  Matlab
% value objects are passed by value, not by reference, so the saving of
% states here does not work.  The state buffer is still necessary to pass
% allocate in the calling function and pass here in order to trigger CRL.
% This double buffering is annoying, however only happens during
% simulation, not code generation.
% num_states = (length(coeffs) - numChannels) / 2;
% persistent states;
% if isempty(states)
%     states = single(zeros(1, num_states));
% end

global codeWorkMemory; % states buffer
stateIdx = 1;
coeffIdx = 1;

frameMinus1  = frameSize-1;      % number of samples minus one
y     = coder.nullcopy(x); % output matrix

% OUTER LOOP over all channels                 
for cha = 1:numChannels

    % FIRST STAGE
    % prologue: get coefficients, states and input vector 
    % and perform some computations
    
    w1 = codeWorkMemory(stateIdx);                          stateIdx = stateIdx+1; 
                        gain = coeffs(coeffIdx);    coeffIdx = coeffIdx+1;
    w2 = codeWorkMemory(stateIdx);                          stateIdx = stateIdx-1; 
                        a2   = coeffs(coeffIdx);    coeffIdx = coeffIdx+1;
    xx  = x(1,cha);     a1   = coeffs(coeffIdx);    coeffIdx = coeffIdx+1; 
    xx  = xx*gain;      b2   = coeffs(coeffIdx);    coeffIdx = coeffIdx+1;
    yy  = w1+xx;        b1   = coeffs(coeffIdx);    coeffIdx = coeffIdx+1; 
    w1  = b1*xx;

    % main loop: 5 ops/sample
    for i = 1:frameMinus1
        r  = a1*yy;      w1 = w1+w2;        y(i,cha) = yy;
        q  = b2*xx;      w1 = w1-r;         xx = x(i+1,cha);
        xx = xx*gain;
        w2 = a2*yy;      yy = w1+xx;  
        w1 = b1*xx;      w2 = q-w2; 
    end
    
    % epilogue: drain pipe and write states
    r  = a1*yy;     w1 = w1+w2;             y(frameMinus1+1,cha) = yy;
    q  = b2*xx;     w1 = w1-r; 
    w2 = a2*yy;     codeWorkMemory(stateIdx) = w1;  stateIdx = stateIdx+1;
    w2 = q-w2;      codeWorkMemory(stateIdx) = w2;  stateIdx = stateIdx+1;              
    % NOTE: asm implementation needs to exit here if it's only
    % one stage, NOTE: all subsequent stages work in-place
      
    for st = 2:numStages(cha)
        % prologue: get coefficients, states and input vector, 
        % and perform some computations
        w1 = codeWorkMemory(stateIdx);                      stateIdx = stateIdx+1; 
                            a2 = coeffs(coeffIdx);  coeffIdx = coeffIdx+1;
        w2 = codeWorkMemory(stateIdx);                      stateIdx = stateIdx-1; 
                            a1 = coeffs(coeffIdx);  coeffIdx = coeffIdx+1;
        xx = y(1,cha);      b2 = coeffs(coeffIdx);  coeffIdx = coeffIdx+1;
        yy = w1+xx;         b1 = coeffs(coeffIdx);  coeffIdx = coeffIdx+1; 
        w1 = b1*xx;
        
        % main loop: 4 ops/sample
        for i = 1:frameMinus1
            r  = a1*yy;     w1 = w1+w2;         y(i,cha) = yy;
            q  = b2*xx;     w1 = w1-r ;         xx = y(i+1,cha);
            w2 = a2*yy;     yy = w1+xx; 
            w1 = b1*xx;     w2 = q-w2; 
        end
        
        % epilogue: drain pipe and write states
        r  = a1*yy;     w1 = w1+w2;             y(frameMinus1+1,cha) = yy;
        q  = b2*xx;     w1 = w1-r; 
        w2 = a2*yy;     codeWorkMemory(stateIdx) = w1;  stateIdx = stateIdx+1;
        w2 = q-w2;      codeWorkMemory(stateIdx) = w2;  stateIdx = stateIdx+1;
        
    end
end

end

