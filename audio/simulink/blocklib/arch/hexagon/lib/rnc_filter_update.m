function e_prime = rnc_filter_update(e, y, H1, H2, ...
    speakerth, rnc_wiener_filter_length, rnc_number_of_spkrs, rnc_number_of_mics)
% If code gen'ing, error out if this is not replaced
coder.replace('-warnifnoreplacement')

e_prime = single(0);

%Filtered Error from Roof Mic signals
for l=1:rnc_number_of_mics
    for j=1:rnc_wiener_filter_length
        e_prime = e_prime + H1(j,l+(speakerth-1)*rnc_number_of_mics) .* e(j,l);
    end
end

%Filtered Error from Secondary Source signals
for minput=1:rnc_number_of_spkrs
    for j=1:rnc_wiener_filter_length
        e_prime = e_prime + H2(j,minput+(speakerth-1)*rnc_number_of_spkrs) .* y(j,minput);
    end
end

end

