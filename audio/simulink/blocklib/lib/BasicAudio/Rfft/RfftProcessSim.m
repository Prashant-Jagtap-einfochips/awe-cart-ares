%% Function for simulation
function y = RfftProcessSim(u, fftSize)
% Calculates the FFT of a purely real input signal. Uses an optimized 2 for
% 1 Rfft algorithm. 
% Inputs:
%       u: input of size (fftSize x numChannels)
%       fftSize: Size of FFT
% Output:
%       y: array of size (fftSize/2 + 1 x numChannels) number of unique FFT
%           coefficients of the input
%--------------------------------------------------------------------------


half_spectrum_size = (fftSize/2) + 1;
numChannels = size(u,2);
numChannelPairs = floor(numChannels/2);
extraChannel = numChannels - numChannelPairs*2;


% Initialize output
y = single(complex(zeros(half_spectrum_size,size(u,2))));

% Calculate 2 Channel Real FFT for every channel pair
for pairIdx = 1: numChannelPairs
    chan1Idx = pairIdx*2-1;
    chan2Idx = chan1Idx + 1;
    [X1, X2] = Rfft2ChanProcessSim(u(:,chan1Idx), u(:,chan2Idx), fftSize);
    y(:,chan1Idx)   = X1(:);
    y(:,chan2Idx)   = X2(:);
end

% if numChannels is odd, there is an extra channel, Calculate RFFT normally
if extraChannel == 1
    lastFFT = fft(u(:,end));
    y(:,end) = lastFFT(1:half_spectrum_size);
end

end  
