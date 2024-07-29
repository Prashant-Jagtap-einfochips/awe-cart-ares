function buildProject = CreateBuildProject(projectFolder,appProjectFolder,codeFolder,cacheFolder)
%CREATEBUILDPROJECT Creates Simulink project to perform code generation
%   Designed for building the same design model with different build-time
%   configuration settings such as differing target processor, optimization
%   goals, or even channel counts.
% Usage:
%    buildProject = CreateBuildProject(projectFolder,appProjectFolder,[codeFolder],[cacheFolder]);
% 
% projectFolder (required): filesystem path where the project will reside
% appProjectFolder (required): filesystem path of existing model content project 
% codeFolder (optional): sub-folder of projectFolder to place generated
%    code. If not provided, "code" is the default folder.
% cacheFolder (options): sub-folder of projectFolder to place cache files.
%    If no provided, "cache" is the default folder.

%   Copyright 2022 Bose Corporation

% Handle default values
if (false == exist('codeFolder','var'))
    codeFolder = 'code';
end
if (false == exist('cacheFolder','var'))
    cacheFolder = 'cache';
end

% Attempt to acquire a full absolute path for the appProjectFolder because
% once the new project is created, the current filesystem path is uncertain
fullPathToApp = GetFullFileSystemPath(appProjectFolder);

% Delete new project if it already exists
if (isfolder(projectFolder))
    fprintf('Deleting any existing project in [%s]...\n',projectFolder);
    try
        % Try to get current project... but there might not be one!
        curProjObj = currentProject;
        if (curProjObj.isLoaded)
            fprintf('Closing project [%s].\n',curProjObj.Name);
            curProjObj.close;
        end
    catch
    end
    try
        % Try to delete the project in that folder... but there might not
        % be one!
        matlab.project.deleteProject(projectFolder);
    catch ME
        fprintf('Tried to delete existing project, but %s\n',ME.message)
    end
end

% Create the new project
fprintf('Creating new project in [%s]...\n',projectFolder);
buildProject = matlab.project.createProject( ...
    'Folder', projectFolder, ...
    'Name', 'build');

% Setup project details
[~,dirName] = fileparts(projectFolder);
buildProject.Name = sprintf('%s Build Project',dirName);
buildProject.SimulinkCacheFolder = cacheFolder;
buildProject.SimulinkCodeGenFolder = codeFolder;

% Add reference to the application design project
fprintf('Adding reference to [%s]...\n',fullPathToApp);
buildProject.addReference(fullPathToApp,"relative");

% Remove SVN "add" of this new project. Start at the new project's root
% folder and work back until folders are no longer in "add" status.
folderToRemove = buildProject.RootFolder;
stillRemoving = contains(buildProject.SourceControlIntegration,"SVN");
while (stillRemoving)
    % jsvn does not support "info --show-item" :-( so just get it all:
    theOutput = evalc("jsvn info " + folderToRemove);
    if (contains(theOutput,"Schedule: add"))
        % This folder is scheduled to add to version control. Change that.
        evalc("jsvn remove --keep-local " + folderToRemove);
        fprintf("Eliminated SVN add of %s\n",folderToRemove);
        % Proceed with the containing folder (one step higher in path)
        folderToRemove = fileparts(folderToRemove);
    elseif (contains(theOutput,"Schedule: normal"))
        % Reached the end - back to normal working copies. All done.
        stillRemoving = false;
    elseif (contains(theOutput,"E155007"))
        % Encountered somewhat expected error "... is not a working copy"
        stillRemoving = false;
    else
        error("Failed to remove SVN additions with error:\n %s\n",theOutput);
    end
end

fprintf('Done!\n');
end

%--------------------------------------------------------------------------
function fullPath = GetFullFileSystemPath(aPath)
% Acquire the full folder path - no longer relative
    theDir = dir(aPath);
    if (isempty(theDir))
        error('Unable to find project folder [%s]',aPath);
    end
    % otherwise, the first entry is the "." self folder
    fullPath = theDir(1).folder;
end
