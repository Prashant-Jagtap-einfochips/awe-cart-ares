function CreateTextFile
% CreateTextFile Create file for write access, returning error if fail
%    Normally, fopen does not emit an error upon failure. This does.
%    Use fclose to close the returned file handle
%  Usage:
%     fid = CreateTextFile(newFilePath);
