classdef SelectiveDeleteProtect < handle
% SelectiveDeleteProtect destroys or obfuscates files and folders including
% removal from Simulink project. Only one project supported (no refs).
% Usage:
%    SelectiveDeleteProtect(containingPath, contentPath, configurationString)
%     *containingPath: absolute path of folder with "parent" project
%        having references to the contentPath project as well as any
%        other dependencies that project requires to operate. This
%        folder can be the same as the content path if there are no
%        dependencies.
%     *contentPath: absolute path of folder containing project and
%        files to be protect/deleted.
%     *configurationString: protect/delete directive filesystem
%        patterns with +,-,= prefixes indicating how to delete/protect
% configurationString matching prefix directives:
%   '+' to protect (p-code .m files, delete .c files)
%   '-' to remove (i.e. blindly delete regardless of file type)
%   '=' do not touch - takes precidence over the above directives (not '!')
%   '!' to remove based on filepath, only '>' or '<' provide exceptions
%   '<' exception to filepath delete- protect instead
%   '>' exception to filepath delete- leave untouched instead
%   '%' ignored comment line
% After constructing the object, invoke PerformDelete to launch the action.

%   Copyright 2023 Bose Corporation
properties
    PreserveHelp = true;
end
properties (SetAccess=protected)
    
    CfileCount  % Count of how many C files were deleted
    MfileCount  % Count of how many M files were P-coded
    RemoveCount % Count of how many other files were deleted

    StatusIteratorCount = 0

    ProtectList
    RemoveList
    ExceptionList
    PathRemoveList
    PathProtectInsteadList
    PathUntouchedInsteadList

    ContainingPath string % folder containing "parent" project with references to both the content project as well as other dependencies
    ContentPath string % folder containing project and files to delete/protect
    ConfigurationString string % string of directive patterns with +, -, =
    MyScriptFolder string % when non-blank, contains path to this class

    CSource     % Paths of removed C files
    MFile       % Paths of p-coded M files
    Exceptions  % Paths of files untouched due to "=" directive
    Failure     % Paths of files that could not be removed due to error
    RemovedFiles string % Paths of files removed from project
    AddedFiles   string % Paths of files added to project
    RemovedFromProjectPath string % Paths removed from project path that were not themselves a part of the project
end

methods
%--------------------------------------------------------------------------
function this = SelectiveDeleteProtect(containingPath, contentPath, configurationString)
    % Contruct the object - ensure the SimulinkProject is opened as root
    % Usage:
    %    SelectiveDeleteProtect(containingPath, contentPath, configurationString)
    %     *containingPath: absolute path of folder with "parent" project
    %        having references to the contentPath project as well as any
    %        other dependencies that project requires to operate. This
    %        folder can be the same as the content path if there are no
    %        dependencies.
    %     *contentPath: absolute path of folder containing project and
    %        files to be protect/deleted.
    %     *configurationString: protect/delete directive filesystem
    %        patterns with +,-,= prefixes indicating how to delete/protect

    % Convert whatever was given for the paths into absolute paths
    containingDir = dir(containingPath);
    if (isempty(containingDir))
        error('Provided containing project path [%s] was not found',containingPath);
    end
    contentDir = dir(contentPath);
    if (isempty(contentDir))
        error('Provided content project path [%s] was not found',contentPath);
    end
    this.ContainingPath = containingDir(1).folder;
    this.ContentPath = contentDir(1).folder;
    this.ConfigurationString = configurationString;

    % To perform selective delete and protect, the current Simulink project
    % may be changed, impacting the MATLAB path. To ensure that this
    % current classdef file continues to be used, remember its path
    this.MyScriptFolder = fileparts(mfilename('fullpath'));
end
%--------------------------------------------------------------------------
function rootProject = EnsureProjectOpen(this,projectRootFolder)
    % Opens specified project if it's not already the root project
    rootProject = matlab.project.rootProject;
    if (false == isempty(rootProject) && rootProject.RootFolder == projectRootFolder)
        % It's already loaded!
        fprintf('Project [%s] already loaded\n',projectRootFolder);
    else
        fprintf('Opening project [%s]\n',projectRootFolder);
        rootProject = openProject(projectRootFolder);
    end
    % Regardless of what happened above - ensure this file is found first
    % on MATLAB path
    addpath(this.MyScriptFolder);
end    
%--------------------------------------------------------------------------
function PerformDelete(this)
% Performs protection/deletion on DestinationPath

    this.CfileCount = 0;
    this.MfileCount = 0;
    this.RemoveCount = 0;

    % Announce intentions with help preservation
    if (this.PreserveHelp)
        fprintf('   MATLAB class/function help will be preserved.\n');
    else
        fprintf('   Help will *not* be preserved.\n');
    end

    % Read in the configuration
    configLines = splitlines(this.ConfigurationString);
    
    % Sort the configuration out into "protected" vs "removed" names
    this.ProtectList = {};
    this.RemoveList = {};
    this.PathRemoveList = {};
    this.PathProtectInsteadList = {};
    this.PathUntouchedInsteadList = {};
    for i=1:numel(configLines)
        aLine = configLines{i};
        if (isempty(aLine))
            % Ignore empty lines
            continue;
        end
        if (aLine(1)) == '-'
            this.RemoveList{end+1} = ['^' regexptranslate('wildcard',aLine(2:end)) '$'];
            fprintf('   Removing: %s\n',this.RemoveList{end});
        elseif (aLine(1)) == '+'
            this.ProtectList{end+1} = ['^' regexptranslate('wildcard',aLine(2:end)) '$'];
            fprintf('   Protecting: %s\n',this.ProtectList{end});
        elseif (aLine(1)) == '='
            this.ExceptionList{end+1} = ['^' regexptranslate('wildcard',aLine(2:end)) '$'];
            fprintf('   Except: %s\n',this.ExceptionList{end});
        elseif (aLine(1)) == '%'
            % This is a comment line, ignore it!
        elseif (aLine(1)) == '!'
            this.PathRemoveList{end+1} = ['\<' regexptranslate('wildcard',aLine(2:end)) '\>'];
            fprintf('   Removing paths: %s\n',this.PathRemoveList{end});
        elseif (aLine(1)) == '<'
            this.PathProtectInsteadList{end+1} = ['\<' regexptranslate('wildcard',aLine(2:end)) '\>'];
            fprintf('   Protecting inside removal paths: %s\n',this.PathProtectInsteadList{end});
        elseif (aLine(1)) == '>'
            this.PathUntouchedInsteadList{end+1} = ['\<' regexptranslate('wildcard',aLine(2:end)) '\>'];
            fprintf('   Leaving untouched inside removal paths: %s\n',this.PathUntouchedInsteadList{end});
        else
            % if the line is not recognized, then error now so there are no
            % unexpected silent failures - directives must be understood!
            error('   IGNORING Unrecognized Delete/Protect directive [%s]. Missing -,+,=,! prefix',aLine);
        end
    end
    
    % Track activity in a log
    this.CSource = {};
    this.MFile = {};
    this.Exceptions = {};
    this.Failure = {};
   
    try
        % Bothersome warnings. Turn them off:
        initialWarnState = warning('query'); % Remember warning states
        warning('off','MATLAB:RMDIR:RemovedFromPath');
        warning('off','MATLAB:pfileOlderThanMfile');
        
        % Load containing project. It's paths are needed to understand the
        % content project and extract help text
        this.EnsureProjectOpen(this.ContainingPath);

        % Recursively clean out protected files
        fprintf('Removing/protecting files...\n');
        this.CleanDir(this.ContentPath,false,false);

        % If any files are intended to be added or removed from the content
        % project, perform those changes now
        this.UpdateProjectAddRemove();
    catch ME
        % Turn warnings back on (if they were on before)
        warning(initialWarnState);
        % Now continue with the error
        rethrow(ME);
    end
    % All done!
    % Turn warnings back on (if they were on before)
    warning(initialWarnState);
    fprintf('\n');
    fprintf('Deleted %d [*.c] files\n', this.CfileCount);
    fprintf('Replaced %d [*.m] files with [*.p] files\n', this.MfileCount);
    fprintf('Removed %d files due to location or name\n', this.RemoveCount);
end
%--------------------------------------------------------------------------
function CleanDir(this, entityName, protectAll, removeAll)
% CleanDir recursively clears a directory of protected files

    dirList = dir(entityName);
    entityCount = numel(dirList);
    for i=1:entityCount
        entityName = dirList(i).name;
        % If the name is nothing but dots, skip it
        if (entityName == '.')
            continue;
        end
        
        % Display some indicator of progress
        fprintf('.');
        this.StatusIteratorCount = this.StatusIteratorCount + 1;
        if (this.StatusIteratorCount > 40)
            this.StatusIteratorCount = 0;
            fprintf('\n');
        end

        fullPath = fullfile(dirList(i).folder,entityName);
        removeByPath = this.IsInList(fullPath,this.PathRemoveList);
        
        % Apply either path-based matching, or file-based matching
        if (removeByPath)
            % Path-based matching (we got a hit on the path)
            protectInstead = this.IsInList(fullPath,this.PathProtectInsteadList);
            untouchedInstead = this.IsInList(fullPath,this.PathUntouchedInsteadList);
            if (protectInstead)
                % Protection exception. Protect instead of delete.
                removeThis = false;
                protectThis = true;
            elseif (untouchedInstead)
                % Complete exception. Leave untouched
                this.Exceptions{end+1} = fullPath;
                removeThis = false;
                protectThis = false;
            elseif  dirList(i).isdir
                % Entire folders do not get removed in path delete, so do
                % not remove this entity - instead look within
                removeThis = false;
                protectThis = false;
            else
                % no escaping this one - it's a path removal
                removeThis = true;
                protectThis = false;
            end
        else
            % Filename-based matching
            protectThis = protectAll || this.IsInList(entityName,this.ProtectList);
            removeThis = removeAll || this.IsInList(entityName,this.RemoveList);
            % Apply exceptions to filename-based protection
            if (protectThis)
                if this.IsInList(entityName,this.ExceptionList)
                    protectThis = false;
                    this.Exceptions{end+1} = fullPath;
                end
            end
            % Apply exceptions to filename-based removal
            if (removeThis)
                if this.IsInList(entityName,this.ExceptionList)
                    removeThis = false;
                    this.Exceptions{end+1} = fullPath;
                end
            end
        end
        
        if (dirList(i).isdir)
            % This entity is a directory: recurse down into it
            this.CleanDir(fullPath, protectThis, removeThis);
            if (removeThis)
                this.ScheduleFileForDestruction(fullPath);
            end
        % Removal takes precidence over protecting. Check removal first:
        elseif (removeThis)
            % Remove this entity
            this.ScheduleFileForDestruction(fullPath);
            this.RemoveCount = this.RemoveCount + 1;
        elseif (protectThis)
            % Ensure this file is protected
            [~,~,ext] = fileparts(fullPath);
            if (strcmp(ext,'.c'))
                % C source files are deleted
                this.ScheduleFileForDestruction(fullPath);
                this.CfileCount = this.CfileCount+1;
                this.CSource{end+1} = fullPath;
            elseif (strcmp(ext,'.m'))
                % M files are obfuscated into P files
                pcode(fullPath,'-inplace');
                replacementFile = strrep(fullPath,'.m','.p');
                this.AddedFiles(end+1) = replacementFile;
                if (this.PreserveHelp)
                    theHelp = this.ExtractHelp(fullPath);
                else
                    theHelp = '';
                end
                if (false == isempty(regexp(theHelp,'^\s*$', 'once')))
                    % Save a help file since *.p files have none
                    fid = fopen(fullPath,'wt');
                    fprintf(fid,'%s\n',theHelp);
                    fclose(fid);
                else
                    % No help, so destroy file
                    this.ScheduleFileForDestruction(fullPath);
                end
                this.MfileCount = this.MfileCount+1;
                this.MFile{end+1} = fullPath;
            end
        end
    end
end
%--------------------------------------------------------------------------
function ScheduleFileForDestruction(this,destFullPath)
    % Adds file to a list of files to be removed later
    % Files must exist at the time they are removed from a project.
    % Files can only be removed from the currently active project.
    % The project undergoing protect/delete may have dependencies on other
    % projects, but those other projects are not referenced. Instead, the
    % containing "parent" project must reference them (since they are
    % potentially shared for lots of sub-projects). In this case, the
    % ability to exact help from M files requires MATLAB knows about all
    % the base classes possibily defined in these dependency projects.
    % Therefore - the *containing* project must be active instead of the
    % *content* project where files are deleted.
    if (isempty(destFullPath))
        % No file to delete - we are done here
        return;
    end

    % Add this file to the list. It will be deleted later
    % (see UpdateProjectAddRemove)
    this.RemovedFiles(end+1) = destFullPath;
end
%--------------------------------------------------------------------------
function UpdateProjectAddRemove(this)
    % If any files are to be added/removed, open the content project and
    % make the changes
    if (isempty(this.AddedFiles) && isempty(this.RemovedFiles))
        % Nothing was added or removed! Just leave... nothing to do here.
        return;
    end

    % Update the content project with additions and removals
    contentProject = this.EnsureProjectOpen(this.ContentPath);

    % Handle additions
    addCount = numel(this.AddedFiles);
    for i=1:addCount
        aFile = this.AddedFiles(i);
        contentProject.addFile(aFile);
    end

    % Handle removals - more complex, so called in separate method
    subCount = numel(this.RemovedFiles);
    for i=1:subCount
        aFile = this.RemovedFiles(i);
        this.DestroyFile(aFile,contentProject);
    end
end
%--------------------------------------------------------------------------
function DestroyFile(this,destFullPath,project)
    % Actually destroy the file. The Content project must already be open!
    removeStr = regexptranslate('escape',project.RootFolder);
    projectRelativePath = regexprep(destFullPath,removeStr,'','ignorecase');

    % Remove from the project
    if (~isempty(project.findFile(projectRelativePath)))
        % Filesystem path was found in project. Remove it now. If this is a
        % file that is on the project path, SimulinkProject will
        % automatically remove as necessary from the project path also.
        project.removeFile(destFullPath);
    else
        % Not found in project, so SCM adjustments must be made manually
    end
    
    % Remove from projectpath and perform actual deletion
    if (isfolder(destFullPath))
        % Check if this about-to-delete folder is on the project path.
        % CleanDir operates in depth-first pattern, so exact match of the
        % folder makes sense. Note that the removeFile call above will also
        % clean the path, but it's only called for folders that are in the
        % project. This checks for folders that are on the path but *not*
        % in the project:
        ProjectPath = [project.ProjectPath.File];
        if (ismember(destFullPath,ProjectPath))
            % This folder is mentioned in the path! This match only happens
            % when a folder is not part of the project but is still on the
            % project path.
            project.removePath(destFullPath);
            this.RemovedFromProjectPath(end+1) = destFullPath;
        end
        success = rmdir(destFullPath,'s');
        if (~success)
            fprintf('Could not remove %s!!\n',destFullPath);
            this.Failure{end+1} = sprintf( ...
                'Could not remove [%s]', destFullPath);
        end
    else
        % for files, just delete them
        delete(destFullPath);
    end
end
%--------------------------------------------------------------------------
function isMember = IsInList(~,entityName,aList)
    % return true if entityName is a member of aList (regex expressions)
    isMember = false;
    count = numel(aList);
    for ex=1:count
        if (regexp(entityName,aList{ex},'once'))
            isMember = true;
            break;
        end
    end
end
%--------------------------------------------------------------------------
function [theHelp] = ExtractHelp(~,inFileName)
%ExtractHelp Returns "help file" text file as a string array
%   Works for both functions and classdefs. Intended for use with pcode-
%   just write the returned string to replace the original *.m file
%   afterwards.

theHelp = "";

[~, name, ~] = fileparts(inFileName);

try
    % Try to extract a classdef "meta.method"
    mco = eval(['?' name]);
catch ME
    % Unknown cause on a "fresh" instance of MATLAB, the above line
    % RELIABLY fails with 'MATLAB:subscripting:classHasNoPropertyOrMethod'
    % while processing FeaturePathUtil, but because FeaturePathMask is
    % somehow messed up in a cache somewhere.
    %   'Invalid default value for property 'MaskName' in class 'FeaturePathUtil':
    %   The class FeaturePathMask has no Constant property or Static method named 'MaskType'.'
    % Trying to instantiate a FeaturePathMask, we see more symptoms of fail
    %   FeaturePathMask
    %   Error using FeaturePathMask
    %   The class 'FeaturePathMask' contains a parse error,
    %   cannot be found on MATLAB's search path, or is shadowed
    %   by another file with the same name.
    % Our Workaround: find the class that "has no" and clear it
    expr = 'The class (\w+) has no';
    hasNoClassTokens = regexp(ME.message,expr,'tokens');
    hasNoClass = hasNoClassTokens{1}{1};
    warning('Clearing [%s] as workaround', hasNoClass);
    eval(['clear ' hasNoClass]);
    % (try again)
    mco = eval(['?' name]);
end

if (isempty(mco))
    % Not a classs: just do function help
    emptyHelp = string([name ' is a function.']);
    aHelp = help(name);
    lines = string(split(aHelp,newline));
    if (lines(1) ~= emptyHelp && ...
        lines(1) ~= "" && ...
        lines{1}(1) ~= '-')
        theHelp = sprintf("function %s",name);
        lines(end) = [];
        commentHelp = "%" + lines;
        theHelp = vertcat(theHelp,commentHelp);
    end
else
    % We've got a class
    emptyHelp = [name ' is a class.'];
    lines = split(help(name),newline);
    theHelp = sprintf("classdef %s",name);
    gotHelp = false;
    % Add class-level help
    if (false == isempty(lines{1}) && ...
        false == strcmp(emptyHelp,lines{1}))
        gotHelp = true;
        commentHelp = "%" + string(lines);
        theHelp = vertcat(theHelp,commentHelp);
    end
    % Handle enumeration classes and normal classes differently
    if (mco.Enumeration)
        enumerationList = strjoin({mco.EnumerationMemberList.Name},',');
        commentHelp = "% EnumerationMemberList: " + enumerationList;
        theHelp = vertcat(theHelp,commentHelp);
    else
        % Loop through properties for help
        propertyHelp = [];
        for i=1:numel(mco.PropertyList)
            aProperty = mco.PropertyList(i);
            access = aProperty.GetAccess;
            if (false == ischar(access) || ...
                false == contains(access,'public'))
                % Skip properties that cannot be read by public
                continue;
            end
            propName = [name '.' aProperty.Name];
            emptyHelp = sprintf("%s is a property.\n",propName);
            aHelp = string(help(propName));
            if (isempty(regexp(aHelp,emptyHelp, 'once')))
                gotHelp = true;
                lines = string(split(aHelp,newline));
                % remove the last line which is always empty
                lines(end) = [];
                commentHelp = "  %" + lines;
                propertyHelp = vertcat(propertyHelp,commentHelp,aProperty.Name); %#ok<AGROW> 
            end
        end
        % only add the properties block if there's something in there:
        if (false == isempty(propertyHelp))
            theHelp = vertcat(theHelp,"properties");
            theHelp = vertcat(theHelp,propertyHelp);
            theHelp = vertcat(theHelp,"end");
        end
        % Loop through methods for help
        theHelp = vertcat(theHelp,"methods");
        numMethods = numel(mco.MethodList);
        for i = 1:numMethods
            aMethod = mco.MethodList(i);
            methodAccess = aMethod.Access;
            if (false == ischar(methodAccess) || ...
                false == contains(methodAccess,'public'))
                % Skip methods that are not simple public access
                continue;
            end
            if (false == strcmp(aMethod.DefiningClass.Name,name))
                % Skip inherited methods also
                continue;
            end
            funcName = [name '.' aMethod.Name];
            emptyHelp = sprintf("%s is a function.",funcName);
            aHelp = string(split(help(funcName),newline));
            if (emptyHelp ~= aHelp(1) && "empty" ~= aMethod.Name)
                gotHelp = true;
                lines = string(split(aHelp,newline));
                % remove the last line which is always empty
                lines(end) = [];
                commentHelp = "   %" + lines;
                theHelp = vertcat(theHelp,"function "+aMethod.Name, ...
                    commentHelp,"end"); %#ok<AGROW> 
            end
        end
        theHelp = vertcat(theHelp,"end");
    end
    if (gotHelp)
        theHelp = vertcat(theHelp,"end");
    else
        theHelp = "";
    end
end
end



end % methods
end % classdef