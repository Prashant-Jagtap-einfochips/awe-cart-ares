function [zd,pd,kd] = mybilinear(z,p,k,fs)
% Simple bilinear transform for zero-pole-gain form inputs
% to support code gen
% Copied from Matlab built-in bilinear.m

%   Copyright 2019 Bose Corporation
[mn,nn] = size(z);
[md,nd] = size(p);
if (nd == 1 && nn < 2) && nargout ~= 4
    if mn > md
        error(message('signal:bilinear:InvalidRange'))
    end
    fs = 2*signal.internal.sigcasttofloat(fs,'double','bilinear','',...
        'allownumeric');
    z = single(z);
    p = single(p);
    k = single(k);
    
    z = z(isfinite(z));	 % Strip infinities from zeros
    pd = single((1+p/fs)./(1-p/fs)); % Do bilinear transformation
    zd = single((1+z/fs)./(1-z/fs));
    % real(kd) or just kd?
    kd = single((k*prod(fs-z)./prod(fs-p)));
    zd = [zd;-ones(length(pd)-length(zd),1)];  % Add extra zeros at -1
else
    error(message('function only takes inputs in pole-zero-gain form'))
end
