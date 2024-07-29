function index = circindex(index, increment, length)
% Circular buffer indexing function to support CRL for optimized built-in 
% routine on SHARC platforms.
%
% Note: all input arguments need to be int32 to trigger CRL.
    
% If code gen'ing, generate a warning if this is not replaced with circindex.c
coder.replace('-warnifnoreplacement');
index = mod((index + increment), length); 

end