function [coeffs, advancesamp, blocksize] = SampleRateConverter_init(up, down, rate_selector, fs, Fpass, Fhearing, passband_ripple_dB, stopband_atten_dB)

% Sample rates and resample factors
if rate_selector == 1
    fs1 = fs;
    fsmid = fs1 * up;
    fs2 = fsmid / down;
elseif rate_selector == 2
    fs2 = fs;
    fsmid = fs2 * down;
    fs1 = fsmid / up;
else
    error('Incorrect sample rate inputs')
end

fs_min = min(fs1,fs2);

if Fpass <= 0
    Fpass = 0.8*fs_min/2;
end

if Fhearing <= 0
    Fhearing = fs_min/2;
end

if passband_ripple_dB <= 0
    passband_ripple_dB = 40;
end

if stopband_atten_dB <= 0
    stopband_atten_dB = 40;
end

Fhearing = min(Fhearing,fs_min/2);

ripple = 10^(-passband_ripple_dB/20);
atten = 10^(-stopband_atten_dB/20);

% Filter Coefficients
Fstop = fs2 / 2 + (fs2 / 2 - Fhearing);
[order, Wc, bta] = kaiserord([Fpass Fstop], [1 0], [ripple atten], fsmid);
filt_coef = fir1(order, Wc, 'low', kaiser(order + 1, bta), 'noscale');

% Zero-pad the filter until it is an integer multiple of the upfactor
blocksize = ceil(numel(filt_coef) / up);
nFilt = blocksize * up;
coeffs = reshape([filt_coef(:); zeros(nFilt - numel(filt_coef), 1)], ...
    up, blocksize).' * down;
coeffs = flipud(coeffs);

% Polyphase implementation parameters
curidx = 1;
index = [];
advancesamp = [];
while ((curidx ~= 1) || isempty(index))
    index = [index; curidx];
    curidx = curidx + down;
    curadvance = 0;
    while (curidx > up)
        curadvance = curadvance + 1;
        curidx = curidx - up;
    end
    advancesamp = [advancesamp; curadvance];
end
advancesamp = int32(advancesamp);

coeffs = (coeffs(:, index));
coeffs = coeffs(:);

end