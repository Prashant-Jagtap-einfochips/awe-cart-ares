function CleanMyPathPlease(actuallyDoIt,savedNameStringArray)
% This function removes paths that are not part of a loaded project or else
% do not contain one of the specified strings to save. By default, the
% following path-saving strings are used:["MATLAB","mingw","OneDrive"]
% Usage:
%   CleanMyPathPlease(actuallyDoIt,savedNameStringArray)
% Examples:
%   CleanMyPathPlease
%   CleanMyPathPlease(true)
%   CleanMyPathPlease(false,["MATLAB","MyToolBox"]


%Example paths we'd like to keep:
% C:\MinGW64\mingw64-6.3.0
% C:\ProgramData\MATLAB\SupportPackages\R2022b\3P.instrset\mingw_w64.instrset
% ?C:\Users\kl13806\OneDrive - Bose Corporation\Documents\MATLAB

% Determine whether changes are to be made
if (false == exist('actuallyDoIt','var'))
    actuallyDoIt = false;
end

% Acquire saved names or use default
if (false == exist('savedNameStringArray','var'))
    savedNameStringArray = ["MATLAB","mingw","OneDrive"];
else
    savedNameStringArray = string(savedNameStringArray);
end

% Announce what's happening
fprintf('Analyzing MATLAB path for items not containing:\n')
for i=1:numel(savedNameStringArray)
    fprintf('   %s\n', savedNameStringArray(i));
end

% If a project is open, just automatically ignore everything that's from
% the project's path so people don't have to shut down projects. However,
% the path will not be able to be saved - it has all the project items!
rootProject = matlab.project.currentProject;
savePatterns = savedNameStringArray;
if (false == isempty(rootProject))
    % There's a project open!
    % Apparently the root project's "code gen" folder is also on path?
    fprintf('   & root project code folder: %s\n', rootProject.SimulinkCodeGenFolder);
    fprintf('   & root project cache folder: %s\n', rootProject.SimulinkCacheFolder);
    savePatterns = horzcat(savePatterns,rootProject.SimulinkCodeGenFolder);
    savePatterns = horzcat(savePatterns,rootProject.SimulinkCacheFolder);
    
    % Add its project path to those to save
    fprintf('   & the project path of %s\n',rootProject.Name);
    projPath = rootProject.ProjectPath;
    absolutePaths = [projPath.File];
    savePatterns = horzcat(savePatterns,absolutePaths);

    % Acquire all its referenced projects and do the same:
    allProjects = rootProject.listAllProjectReferences();
    for p=1:numel(allProjects)
        aProject = allProjects(p).Project;
        fprintf('   & the project path of %s\n',aProject.Name);
        projPath = aProject.ProjectPath;
        absolutePaths = [projPath.File];
        savePatterns = horzcat(savePatterns,absolutePaths);
    end
end

% for Windows operating systems, ignore case
ignoreCase = ispc;

% Get the path
thePath = string(path);
paths = thePath.split(';');

% Find folders containing our savePatterns
isToSave = contains(paths,savePatterns,"IgnoreCase",ignoreCase);

pathsToRemove = paths(~isToSave);

if (isempty(pathsToRemove))
    fprintf('!! All clean! There are no paths identified to remove!\n');
else
    fprintf('-------------------\nFound these paths to remove:\n-------------------\n');
    disp(paths(~isToSave));
    
    if (actuallyDoIt)
        disp('Removing above paths.')
        % Get rid of them
        paths(~isToSave) = [];
        
        % Join remaining folders back into a path variable:
        newPath = paths.join(';');
        
        % Set the new path!
        path(newPath);
    else
        fprintf('-------------------\n');
        fprintf('Call %s(true) to remove the above\n',mfilename);
    end
end
