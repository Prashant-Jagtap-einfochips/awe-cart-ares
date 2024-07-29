function y = db20(x)
%DB20  Linear quantity to decibels
%   Converts x to dB assuming that x represents volts.
%   y=20*log10(abs(x));
%
% See Also: DB10, UNDB20

%   Copyright 2018-2019 Bose Corporation
% Bill Berardi
% $Id: db20.m 11813 2012-07-11 17:31:25Z rs7109 $

y = 20/log(10)*log(abs(x));
