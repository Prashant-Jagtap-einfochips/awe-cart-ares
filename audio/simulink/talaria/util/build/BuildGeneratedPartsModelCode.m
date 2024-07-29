function buildDirectory = BuildGeneratedPartsModelCode(modelName,cleanFirst)
% BuildGeneratedPartsModelCode loads specified model from the parts folder
% and executes the generated <modelname>.bat file to build the code.
% Usage:
%    BuildGeneratedPartsModelCode(modelName,[cleanFirst]);
%
% * modelName is the name of the generated model e.g. 'ExampleModel_Target'
% * cleanFirst is an optional boolean to "make clean" before building. By
%   default, cleanFirst is false;

%   Copyright 2023 Bose Corporation

if (false ==  exist('cleanFirst','var'))
    cleanFirst = false;
end

fprintf('----------------------------------------------------------------\n');
fprintf('Building the code generated for partitioned model [%s]:\n',modelName);

% Load model from parts folder to ask about its build directories
LoadPartsModel(modelName);

% Construct an absolute filesystem path to the presumed Windows *.bat file
% generated to perform the build:
buildDirs = RTW.getBuildDir(modelName);
makeCmd = fullfile(buildDirs.BuildDirectory,[modelName '.bat']);

% Remember the current directory to enable return to it afterwards
startDir = pwd;
ME = [];

% Perform the build, cleaning first if specified
try
    cd(buildDirs.BuildDirectory);
    if (cleanFirst)
        system([makeCmd ' clean']);
    end
    system(makeCmd);
catch ME    
    % This error will be rethrown below after restoring pwd
end

% Wrap it up: go back to the initial filesystem directory and rethrow any
% errors encountered during the attempted build
cd(startDir);
if (false == isempty(ME))
    rethrow(ME);
end

buildDirectory = buildDirs.BuildDirectory;

end