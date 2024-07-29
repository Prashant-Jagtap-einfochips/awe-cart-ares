%---------------------------------------------------------------------
%
%  COPYRIGHT BOSE CORPORATION ALL RIGHTS RESERVED
%  This program may not be reproduced, in whole or in part in any
%  form or any means whatsoever without the written permission of:
%
%     BOSE CORPORATION
%     The Mountain
%     Framingham, MA 01701-9168
%
%---------------------------------------------------------------------
%
function saveGeneratedCodePathsOJT(artifactInfo)

% The TargetModelBuildInfo structure array is based on Simulink's
% RTW.getBuildDir which is old enough that it uses char vectors instead of
% strings. Extract the BuildDirectory values as strings. String arrays are
% easier to manipulate than a cell array of char vectors.
buildFolders = string({artifactInfo.TargetModelBuildInfo.BuildDirectory});

% Append the name of the MATLAB formatted binary file (MAT-file) that
% contains information about the generated code for building. See... string
% arrays are easier to use:
thePaths = fullfile(buildFolders,'buildinfo.mat');

% For each target model buildinfo, printf out the full file list
for i=1:numel(thePaths)
    % Load the MATLAB file into a variable
    x = load(thePaths(i));

    x.buildInfo.updateFileSeparator('/');

    % Dump the full file list from the variable's "buildInfo" element
    fullFileList = string(transpose(x.buildInfo.getFullFileList()));
    %fprintf('-----\nFile list for %s:\n-----\n',x.buildInfo.ComponentName);
    %fprintf('%s\n',fullFileList);
    %fprintf('\n');

    incpaths   = string(transpose(x.buildInfo.getIncludePaths(true)));
    %fprintf('-----\nInclude paths list for %s:\n-----\n',x.buildInfo.ComponentName);
    %fprintf('%s\n',incpaths);
    %fprintf('\n');

    buildInfo=[];
    buildInfo.ComponentName = x.buildInfo.ComponentName;
    buildInfo.MatlabRoot = replace(matlabroot, filesep, '/');
    buildInfo.FullFileList = fullFileList;
    buildInfo.IncludePaths = incpaths;
    buildInfoStr = jsonencode(buildInfo, PrettyPrint=true);

    buildInfoJson = fullfile(buildFolders(i),'buildInfoOJT.json');
    fileID = fopen(buildInfoJson, 'w');
    fprintf(fileID, buildInfoStr);
    fclose(fileID);
end
