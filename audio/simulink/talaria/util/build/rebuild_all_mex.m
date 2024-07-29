function [builtPaths] = rebuild_all_mex(rootFolder,mexOpt,ignoreList)
%REBUILD_ALL_MEX searches for all *.mexw64 files and rebuilds them
% Usage: rebuild_all_mex(rootFolder,mexOpts,ignoreList)
%
% If rootFolder is provided, the filesearch starts there instead of the
% current directory.
%
% If mexOpt is provded, it is added to the mex commands (e.g. '-g' will
% build with debug symbols)
%
% If ignoreList provided, (cell array of partial names), these will be
% ignored (e.g. 'TOP' would not build TOP_MEX.c which needs -R2018a option)
% Examples:
%   rebuild_all_mex('.','','TOP')
%   rebuild_all_mex('module/lib/top','-R2018a')

%   Copyright 2018-2019 Bose Corporation
% If no folder specified, use current
if (nargin < 1)
    rootFolder = cd;
end
if (nargin < 2)
    mexOpt = '';
end
if (nargin < 3)
    ignoreList = [];
elseif isempty(ignoreList) == false && iscell(ignoreList) == false
    ignoreList = {ignoreList};
end

FileList = dir(fullfile(rootFolder,'**','*.mexw64'));

numFiles = numel(FileList);
for i=1:numFiles
    entry = FileList(i);
    % reconstitute the full path of the mex file
    fullMexPath = fullfile(entry.folder,entry.name);
    if (false == isempty(ignoreList) && contains(entry.name,ignoreList))
        fprintf('---------------------\nSKIPPING/IGNORING [%s]\n', ...
            fullMexPath);
        continue;
    end
    % change from a *.mexw64 to a *.c source
    srcPath = regexprep(fullMexPath,'\.mexw64$','\.c');
    % build it
    fprintf('---------------------\n');
    fprintf('%s ... ',entry.name);
    mex(srcPath,'-outdir',entry.folder,mexOpt);
end


end

