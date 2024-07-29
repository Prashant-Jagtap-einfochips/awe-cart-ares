function [num,den] = firstOrderButter(FsLpf, Fc)
%
%  Equations derived from RESEARCH PAPERS FACULTY OF MATERIALS SCIENCE
%  AND TECHNOLOGY IN TRNAVA SLOVAK UNIVERSITY OF TECHNOLOGY IN BRATISLAVA
%  2019, Volume 27, Number 45 DOI 10.2478/rput-2019-0030
%  DIGITAL IMPLEMENTATION OF BUTTERWORTH FIRST–ORDER FILTER TYPE IIR
%
%   FsLpf is samplerate of filter
%   Fc is cutoff freq of filter
%
Wc = 2*pi*Fc;  % Cutoff freq in radians
T = 1/FsLpf;  % Sample time of filter
%
% Calculate denominator coeffs
%
b1 = 1;
b2 = (2 * tan (Wc*T/2) - 2) / (2 + 2*tan (Wc*T/2));
%
% Calculate numerator coeffs 
%
a1  = 2*tan(Wc*T/2) / (2 + 2*tan(Wc*T/2));
a2 = a1;
%
% Return coeffs
%
num = [a1 a2];
den = [b1 b2];
end
