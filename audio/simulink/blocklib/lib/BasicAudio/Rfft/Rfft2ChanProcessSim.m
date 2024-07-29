function [X1,X2] = Rfft2ChanProcessSim(x1, x2, fftSize)
% Computes the Real FFT of a Two Channel Signal. 
% This is the Generic C Implementation.
% Computation of Two Real FFTs is done using a single complex FFT.
% Channel 1 is loaded into the real part and Channel 2 into imag.
% Complex FFT is computed.
% FFT of channel 1 is extracted as the conjugate symmetric part 
% FFT of channel 3 is extracted as the conjugate anti-symmetric part .
% Inputs:
%     x1 - purely real channel 1 
%     x2 - purely real channel 2
%     fftSize - length of x1 and x2. Also the size of FFT/
% Outputs:
%     X1 - FFT of x1
%     X2 - FFT of x2
% -------------------------------------------------------------------------

% Output size equal to number of independent FFT coefficients
half_spectrum_size = (fftSize/2) + 1;

% Compute Complex FFT
X = fft(complex(x1, x2));

% Temporary variable for calculating outputs
Z = conj(circshift(flip(X),1));

% % Nullcopy copies type, size, and complexity to output
X1 = complex(single(zeros(half_spectrum_size, 1)));
X2 = complex(single(zeros(half_spectrum_size, 1)));

% Apply FFT properties of symmetry to extract FFT of even and imag channel
X1(1:half_spectrum_size) = (X(1:half_spectrum_size) + Z(1:half_spectrum_size))/2;
X2(1:half_spectrum_size) = (X(1:half_spectrum_size) - Z(1:half_spectrum_size))/2j;


end