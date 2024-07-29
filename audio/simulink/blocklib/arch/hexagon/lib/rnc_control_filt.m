function y  = rnc_control_filt(ref_in,ControlCoefficients,rnc_adaptive_filter_length, ...
    rnc_number_of_accel_channels, rnc_number_of_spkrs, FilteredErrorDelay)
% If code gen'ing, error out if this is not replaced

% coder.replace('-warnifnoreplacement')

y = zeros(1,rnc_number_of_spkrs,'like',ref_in);

for m=1:rnc_number_of_spkrs
    for k=1:rnc_number_of_accel_channels
        for i=1:rnc_adaptive_filter_length
            % Use only the last 'rnc_adaptive_filter_length' samples from 
            % total 'rnc_adaptive_filter_length + FilteredErrorDelay-1'
            % buffered ref_in
            y(1,m) = y(1,m) + ControlCoefficients(i,k,m).*ref_in(i+FilteredErrorDelay-1,k);
        end
    end
    % Generate anti-noise speaker signal
    y(1,m) = -y(1,m);

end