function dostat(folder)
% dostat outputs the SVN status showing modified files
% Usage:
%   dostat(folder)
% Examples:
%   dostat
%   dostat blocklib

if (exist('folder','var'))
    folder = sprintf('./%s',folder);
else
    folder = '.';
end
statCmd = sprintf('jsvn stat %s',folder);
output = evalc(statCmd);

% Remove distracting lines about externals. Note that SVN adds a single
% whitespace character to the first line. Just enjoy it.
searchExp = '(^X\s.*\n)|(^\?\s.*\n)|(^Performing status.*\n)|(^\s*\n)';
cleaned = regexprep(output,searchExp,'','lineanchors','dotexceptnewline');
fprintf('%s\n',cleaned);