% --------------------------------------------------------------------
% Rounds up buffer size to a multiple of the the target processor 
% cache line length in order to prevent any adjacent data from 
% sharing the same cache line
%
%    Argurments: 
%       bufSize = size of buffer in units of data type size
%       dataTypeSize = target size of data type in bytes
%       cacheLineLength = target processor cache line length in bytes
%
%    Outputs:
%       Size of buffer in units of data type size
% --------------------------------------------------------------------
function y = TargetCacheRoundUpSize( bufSize, dataTypeSize, cacheLineLength )
    y = (bufSize * dataTypeSize) + (cacheLineLength - 1);
    y = floor(y / cacheLineLength);
    y = (y * cacheLineLength) + (dataTypeSize - 1); 
    y = floor(y / dataTypeSize);
end 