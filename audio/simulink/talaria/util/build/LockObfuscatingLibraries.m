classdef LockObfuscatingLibraries < handle
% LockObfuscatingLibraries locks libraries in preparation for obfuscation.
% Intended to be run on an HCI project in advance of attempting an
% obfuscated release, this function tries to find libraries that the
% release will attempt to obfuscate and ensures (modifying if necessary)
% these libraries have their link locks on.
% Usage:
%   LockObfuscatingLibraries(makeChanges,moduleFolder,featureDiagramGlobs)
% Examples:
%   LockObfuscatingLibraries(true,"module","lib/**/*.slx");

properties (Access=protected)
    % When true, MakeChanges will lock any unlocked diagrams
    MakeChanges = false;

    % Release content folder (in SVN and local) with content to release.
    % This folder is present in both HCI and obfuscated sandboxes
    HciContentSubFolder = 'module'
    
    % Feature diagram filename patterns using MATLAB's "dir". Matching
    % diagrams will be obfuscated and included in the release. Specify
    % empty to disable all block diagram obfuscation and *.slx elimination.
    % Example for any slx files in a "lib" folder: "**/lib/*.slx"
    % Example for a specific slx file: "**/ThisOne.slx"
    FeatureDiagramGlobs string = "**/lib/*.slx"

    % Names of diagrams that have been checked for release
    CheckedDiagramNameList string

    % Filesystem paths of self-modifying diagrams found during obfuscation
    % which could not be collapsed/absorbed and so must be separately
    % obfuscated and included
    SelfModifyingLibraryList string

    ActuallyChangedList string
    ShouldBeChangedList string
    AlreadyGoodList string
end

methods

%--------------------------------------------------------------------------
function this = LockObfuscatingLibraries(makeChanges,moduleFolder,featureDiagramGlobs)

    % Receive any inputs
    if exist("makeChanges","var")
        this.MakeChanges = makeChanges;
    end
    if (exist("moduleFolder","var") && false == isempty(moduleFolder))
        this.HciContentSubFolder = moduleFolder;
    end
    if (exist("featureDiagramGlobs","var") && false == isempty(featureDiagramGlobs))
        this.FeatureDiagramGlobs = featureDiagramGlobs;
    end

    % Do the job
    this.DoFeatureDiagramLocking();


    fprintf('-------------------------------------------------\n');
    if (false == isempty(this.AlreadyGoodList))
        fprintf('Diagrams already correctly locked (%d):\n', ...
            numel(this.AlreadyGoodList));
        ind = "   " + this.AlreadyGoodList;
        fprintf("%s\n",ind.join(newline));
    end
    if (false == isempty(this.ActuallyChangedList))
        fprintf('Fixed diagrams that are locked now (%d):\n', ...
            numel(this.ActuallyChangedList));
        ind = "   " + this.ActuallyChangedList;
        fprintf("%s\n",ind.join(newline));
    end
    if (false == isempty(this.ShouldBeChangedList))
        fprintf('Diagrams which need to be fixed (%d):\n', ...
            numel(this.ShouldBeChangedList));
        ind = "   " + this.ShouldBeChangedList;
        fprintf("%s\n",ind.join(newline));
    end
    fprintf('-------------------------------------------------\n');
end
%--------------------------------------------------------------------------
function DoFeatureDiagramLocking(this)
    % DoFeatureDiagramLocking searches the FeatureDiagramGlobs and
    % locks the diagrams (if enabled). Recurses down into self-modifying libraries.

    fileList = string([]);

    rootFolder = fullfile(pwd,this.HciContentSubFolder);

    % Loop through the globs and aggregate a list of feature digrams
    for i=1:numel(this.FeatureDiagramGlobs)
        aGlob = fullfile(rootFolder,this.FeatureDiagramGlobs(i));
        dirList = dir(aGlob);
        if (0 == numel(dirList))
            error(['Unabled to find any matches for FeatureDiagramGlob [%s] ' ...
                'under [%s].'], ...
                this.FeatureDiagramGlobs(i),rootFolder);
        end
        for j=1:numel(dirList)
            aPath = fullfile(dirList(j).folder,dirList(j).name);
            [~,~,extension] = fileparts(aPath);
            if (".slx" ~= extension)
                fprintf('ignoring non-diagram [%s]\n',dirList(j).name);
            else
                fileList(end+1) = aPath; %#ok<AGROW>
            end
        end
    end
    diagramsToObfuscate = unique(fileList);

    % Now that we have the list of diagrams to obfuscate... get to work
    for aDiagram = diagramsToObfuscate
        this.LockLibraryDiagram(aDiagram,rootFolder);
    end

    % Obfuscating the diagrams above may have generated a list of
    % self-modifying libraries that must also now get obfuscation.
    % Keep going until ObfuscatedDiagramNameList also contains everything 
    % in SelfModifyingLibraryList
    allDone = false;
    while (false == allDone)
        selfModIsObfuscated = ismember(this.SelfModifyingLibraryList, ...
                                       this.CheckedDiagramNameList);
        toDo = this.SelfModifyingLibraryList(~selfModIsObfuscated);
        for i=1:numel(toDo)
            aDiagram = toDo(i);
            this.LockLibraryDiagram(aDiagram,rootFolder);
        end
        allDone = isempty(toDo);
    end
end
%--------------------------------------------------------------------------
function LockLibraryDiagram(this,modelPath,rootFolder)
% Attempts to mirror the logic of ObfuscationRelease in terms of finding
% self-modifying libraries that also would need to be locked. Performs the
% locking if enabled.

fprintf('Loading %s for analysis...\n',modelPath);

% First close any existing open model that has the same name. The
% architecture-specific CRL workflows often reuse the same model name
% "assembly_crl" and there may be other situations where *.slx files share
% the same name: close the name first before loading from a specific path:
[~,outputModelName] = fileparts(modelPath);
close_system(outputModelName,0);

load_system(modelPath);

% Ensure that this library has its linked locked - since it is being
% obfuscated, we must ensure that the original HCI version would never have
% its link broken, thereby copying all its HCI secrets into the containing
% diagram.
if ("on" ~= get_param(outputModelName,'LockLinksToLibrary'))
    if (this.MakeChanges)
        fprintf('Changing [%s]...\n',modelPath);
        obj = this.RefreshAndUnlock(outputModelName);
        obj.LockLinksToLibrary = 'on';
        save_system(modelPath);
        this.ActuallyChangedList = vertcat(this.ActuallyChangedList,modelPath);
    else
        fprintf('Model [%s] must be updated.\n', ...
            modelPath);
        this.ShouldBeChangedList = vertcat(this.ShouldBeChangedList,modelPath);
    end
else
    this.AlreadyGoodList = vertcat(this.AlreadyGoodList,modelPath);
end
this.CheckedDiagramNameList = vertcat(this.CheckedDiagramNameList,modelPath);

% Dig through the library links one level at a time to differentiate links
% into 3 categories:
% 1-External libraries that are not found in the rootFolder
% 2-Self-modifying library that must be checked and searched separately
% 3-Absorbed libraries that will consumed in the obfuscation process
F = Simulink.FindOptions("FollowLinks",true,"RegExp",true);
collapseLevel = 1;
searchBlocks = outputModelName;
while (false == isempty(searchBlocks))
    % Get resolved links
    fprintf('Analyzing %s link collapse level %d...\n',outputModelName,collapseLevel);
    if (collapseLevel > 1)
        % Eliminate any blocks that are not subsystems
        isSubsystem = "Subsystem" == get_param(searchBlocks,'BlockType');
        searchBlocks = searchBlocks(isSubsystem);
    end
    collapseLevel = collapseLevel+1;
    linkHs = Simulink.findBlocks(searchBlocks,'LinkStatus','^resolved',F);
    linkBlocks = getfullname(linkHs);

    % narrow down to just the top-level links
    linkBlocks = RemoveSubpaths(linkBlocks);

    % Retrieve the library paths that are being linked
    refBlocks = string(get_param(linkBlocks,"ReferenceBlock"));

    % Determine whether the reference blocks are external to this project
    refDiagrams = fileparts(refBlocks);
    load_system(refDiagrams);
    rootNames = cellfun(@bdroot,refBlocks,'UniformOutput',false);
    fileNames = get_param(rootNames,"FileName");
    isExternal = (0 == contains(fileNames,rootFolder));

    % Remove external blocks that are outside the scope of our study
    refBlocks(isExternal) = [];
    fileNames(isExternal) = [];

    % Check for self-modifyable blocks
    isSelfMod = ("on" == get_param(refBlocks,'MaskSelfModifiable'));
    selfModLibFiles = fileNames(isSelfMod);
    if (false == isempty(selfModLibFiles))
        this.SelfModifyingLibraryList = vertcat(this.SelfModifyingLibraryList,selfModLibFiles);
        this.SelfModifyingLibraryList = unique(this.SelfModifyingLibraryList);
    end
    
    % Continue searching for those links which would get absorbed
    isCollapse = ~isSelfMod;
    searchBlocks = unique(refBlocks(isCollapse));
end

end

%--------------------------------------------------------------------------
function modelObj = RefreshAndUnlock(this,outputModelName)
    % Internal use method to ensure library has freshly resolved links and
    % is unlocked... ready for modification
    modelObj = get_param(outputModelName,'Object');
    try
        modelObj.refreshModelBlocks;
    catch ME
        % Uh... some feature libraries have dead links in them?
        % Don't let that stop us now. Just dump the error and continue
        MaskUtil.ReportError(ME.message);
        MaskUtil.ReportError('Some libraries may not be updated');
    end
    % ... and if the model is a library, unlock it!
    if (modelObj.isLibrary && this.MakeChanges)
        modelObj.Lock = 'off';
    end
end

end %methods
end % classdef