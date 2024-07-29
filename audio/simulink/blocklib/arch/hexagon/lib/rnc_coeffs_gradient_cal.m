function ControlCoefficients = rnc_coeffs_gradient_cal(data_in, ControlCoefficients, step_in, speakerth, ...
                               number_accel_channels, adaptive_filter_len)
    
% If code gen'ing, error out if this is not replaced
coder.replace('-warnifnoreplacement')

for kk=1:number_accel_channels
    ControlCoefficients(1:adaptive_filter_len,kk,speakerth) = ...
        ControlCoefficients(1:adaptive_filter_len,kk,speakerth) + step_in*data_in(:,kk);
end


end
