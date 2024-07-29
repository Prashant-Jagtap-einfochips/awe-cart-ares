function Output = FirEvenProcessSim(Input, NumChannels, ActiveCoeffSet, Coeffs)
% FirEvenProcessGeneric Computes FirEven for Generic C code and during
% simulation
%--------------------------------------------------------------------------
a = 1;
b = Coeffs;
[taps, ~] = size(b);

persistent zi;
if isempty(zi)
    zi = single(zeros(taps - 1, NumChannels));
end

Output = single(zeros(size(Input)));
for i = 1:NumChannels
    % Get input, filter coeffs and state for this channel
    u0 = Input(:, i);
    filterIdx = ActiveCoeffSet(i) + 1; % add 1 for Matlab indexing
    coeff = b(:, filterIdx);
    state = zi(:, i);
    
    [y0, state] = FirFilter(coeff, u0, state);
    Output(:, i) = y0;

    % Save state
    zi(:, i) = state;
end
end