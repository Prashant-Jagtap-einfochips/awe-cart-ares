function [Y, StateFinal] = FirFilter(Coeff, Input, StateInitial, OneSampleDelay)
%----------------------------------------------------------------------
% Textbook Implementation of FIR Filter. Coefficients are Flipped.
% Coeff:            Fir Coefficients h[n] (flipped)
% Input:            Input signal x[n]
% InitialState:     Initial State of the Fir Delay Lines
% LowLatency:       (Optional Input) bool 
%----------------------------------------------------------------------
if nargin < 4
    OneSampleDelay = true;
end
CoeffLength = numel(Coeff);
FrameSize = size(Input,1);

% 1) update state buffer with the current input
BufferPerOutput = [StateInitial; Input];
% 2) multiply and accumulated sum
Y = single(zeros(FrameSize,1));
for k = 1 : FrameSize
    tmp = single(0);
    for m = 1 : CoeffLength
        tmp = tmp + (Coeff(m) * BufferPerOutput(k+m-OneSampleDelay));
    end
    Y(k) = tmp;
end
% 3) shift/update state
StateFinal = BufferPerOutput(FrameSize+1 : end); 
%----------------------------------------------------------------------
end