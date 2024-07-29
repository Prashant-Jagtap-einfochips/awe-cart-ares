function y = copysignf(x_mag, x_sign)
%COPYSIGNF Copies sign of x_sign and applies to x_mag

%   Copyright 2018-2019 Bose Corporation
coder.replace('-warnifnoreplacement');
y = single(0);
y = sign(x_sign) * abs(x_mag);
end

