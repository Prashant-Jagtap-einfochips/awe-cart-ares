function [ coeffs ] = sos2pooliir( sos )
%SOS2POOLIIR Converts SOS stages for a given channel to pooliir coeffs

%   Copyright 2018-2019 Bose Corporation
% Preparation
num_stages = size(sos, 1);
ts = 1:4:4*num_stages;
coeffs = zeros(num_stages * 4 + 1,1);

% First element is the overall gain of the SOS
coeffs(1,1) = prod(sos(:,1));

% Error out for poorly-formed sos coeffs
if ((coeffs(1,1) == 0) && (num_stages > 1))
    TranslateError('Poorly-formed SOS: zero b0 for multi-stage results in a zero EQ.');
end

% the remaining coeffs are compacted forms of the biquads
% a2 is next, stagger for each stage
coeffs(1+ts, 1) = sos(:,6);

% a1 is next, stagger for each stage
coeffs(2+ts,1) = sos(:,5);  

% b2 then b1 are next, normalized by the stage gain
if( sos(:,1) == 0 )
    coeffs(3+ts,1) = 0;
    coeffs(4+ts,1) = 0;
else
    coeffs(3+ts,1) = sos(:,3)./sos(:,1);  % b2/b0
    coeffs(4+ts,1) = sos(:,2)./sos(:,1);  % b1/b0
end

end

