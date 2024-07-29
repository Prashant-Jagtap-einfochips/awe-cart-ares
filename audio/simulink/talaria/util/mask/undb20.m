function y=undb20(x)
%UNDB20  Decibels to linear quantity
%   Converts x from DB to absolute magnitude assuming that x is in volts.
%   y = 10^(x/20)
%
% See also: DB20, UNDB10

%   Copyright 2018-2019 Bose Corporation
% Created : 9/7/1994 Bill Berardi
%$Id: undb20.m 11244 2011-10-31 18:51:23Z rg20270 $

y = exp(x * log(10)/20);
