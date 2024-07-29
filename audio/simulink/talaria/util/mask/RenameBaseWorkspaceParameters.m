function RenameBaseWorkspaceParameters(blockPath,directiveFileName,className,okToModifyRootPath)
% RenameBaseWorkspaceParameters accelerates the effort involved in
% migrating a system from using build-time parameters stored in the base
% workspace to using properties defined in a "Config" classdef defined in a
% mask workspace. This script replaces use of base workspace parameters
% in block dialogs and MATLAB Function Block scripts. It also creates the
% classdef definition file with the properties and their default values.
% However, this script does not create the masks with "Config" parameters.
% Usage:
%    RenameBaseWorkspaceParameters(simulinkBlockPath,directiveFileName,className,[okToModifyRootPath])
%
% Parameters:
%   simulinkBlockPath : path of block containing the content to be renamed
%   directiveFileName : name of text file with directives in this format:
%      ClassdefPropertyName = BASE_WS_NAME = default-value;
% 
%      ClassdefPropertyName is the desired name of the new classdef
%      property which should be in Pascal case (i.e. like camelCase except
%      FirstIsCapital.)
%
%      BASE_WS_NAME is the current base workspace variable name that will
%      be replaced.
%
%      default-value is a MATLAB expression which will be directly copied
%      in an automatically generated classdef as the default value for the
%      ClassdefParameterName above.
%
%   className : Name of the new classdef to be defined with the new
%   properties with ClassdefParameterName(s) and default-values. If this
%   file already exists, IT IS DESTROYED!!!!
%
%   okToModifyRootPath : optional parameter defaults to current folder and
%   specifies the filesystem root under which diagram and library
%   modification is allowed. Any blocks that live in *.slx files somewhere
%   not contained under this folder will not be modified.
%
% NOTE THAT THIS SCRIPT *DOES* DIRECTLY EDIT SIMULINK MODELS AND LIBARIES!
% HOWEVER, AS IT DOES NOT ALSO "SAVE" THESE MODIFICATIONS, YOU CAN JUST
% CLOSE THE MODIFIED FILE(S) WITHOUT SAVING with "bdclose all".
% EXCEPT THE CLASSDEF FILE GETS FULLY CLOBBERED

% Copyright 2020 Bose Corporation

global oldNames;
global newNames;
global defaultValues;
global diffReport;
global modifiedLibraries;
global paramToModelMap;
global modifiedBlocks;

diffReport = "";
modifiedLibraries = {};
paramToModelMap = containers.Map;
modifiedBlocks = {};

if (nargin < 4)
    okToModifyRootPath = pwd;
end

fprintf('Only allowing modificaton of diagrams under [%s]\n', ...
    okToModifyRootPath);

% Get the diagram name that contains the blockpath & ensure it's loaded
fprintf('Opening path...\n');
modelName = strtok(blockPath,'/');
load_system(modelName);
load_system(blockPath);
modelObj = get_param(modelName,'Object');
if (strcmp('on',modelObj.Dirty))
    error('Path [%s] already has modifications. Save or close first', ...
        blockPath)
end

% For libraries only, unlock it!
if (modelObj.isLibrary)
    if (strcmp('on',modelObj.Lock))
        modelObj.Lock = 'off';
    end
end

% ----- Get the user input for renaming
goodToGo = DoGetInput(directiveFileName);
if (false == goodToGo)
    return;
end

% ----- Perform the renaming on all the sub-blocks under the mask
DoReplaceSubBlocks(blockPath,okToModifyRootPath);

% ----- Create a classdef with the new properties
DoCreateClassdef(className);
% assignin('base','Config',feval(className));

% Go ahead and ensure that the model window shows up now with the block
for i=1:numel(modifiedLibraries)
    open_system(modifiedLibraries{i});
end

open_system(modelName);
hilite_system(blockPath,'none');

% ----- Dump out and display the difference report
WriteReportFile(strrep(blockPath,'/','_'), className);

fprintf('\nDone.\n');

end

%--------------------------------------------------------------------------
function theString = CellArrayToString(cellArray)
    strCell = string(cellArray);
    elements = "'" + strCell + "'";
    list = join(elements,',');
    theString = sprintf('{%s}',list);
end

%--------------------------------------------------------------------------
function [newValue, isChanged, aBlock] = PerformReplace(aBlock, origValue, description, indexes, doNotUseConfigObject)
    global oldNames;
    global newNames;
    global diffReport;
    global paramToModelMap;
    % If no indexes specified, do them all by default
    if (nargin < 4)
        indexes = 1:numel(oldNames);
    end
    if (nargin < 5)
        doNotUseConfigObject = false;
    end
    isChanged = false;
    newValue = origValue;
    oldDiff = origValue;
    newDiff = origValue;
    matchedIdx = [];
    incrementalValue = origValue;
    for changeIdx=indexes
        % MATLAB Function Block parameters must be root members of the data
        % source (e.g. mask workspace). BTConfig.Expand could help.
        if (doNotUseConfigObject)
            theNewName = newNames{changeIdx};
        else
            theNewName = sprintf('Config.%s',newNames{changeIdx});
        end
        % Find the parameter, but replace 3 different ways. "Normal" for
        % the diagram itself, and "OldDiff / NewDiff" for the report.
        nameFindExp = ['\<' oldNames{changeIdx} '\>'];
        replaceNormal = theNewName;
        replaceOldDiff = ['<span style="background-color:yellow;">' ...
            oldNames{changeIdx} '</span>'];
        replaceNewDiff = ['<span style="background-color:yellow;">' ...
            theNewName '</span>'];
        
        % First check for "error" of new name already present in old value
        newNameFindExp = ['\<' theNewName '\>'];
        replaceErrNewInOld = ['<span style="background-color:#ff9999;">' ...
            theNewName '</span>'];
        maybeBad  = regexprep(oldDiff,newNameFindExp,replaceErrNewInOld);
        if (false == strcmp(maybeBad,oldDiff))
            % Found a new name in the old value - show in report as if
            % changed, but might not be (depends on other replacements).
            isChanged = true;
            oldDiff = maybeBad;
        end            
        
        newValue = regexprep(newValue,nameFindExp,replaceNormal);
        oldDiff  = regexprep(oldDiff,nameFindExp,replaceOldDiff);
        newDiff  = regexprep(newDiff,nameFindExp,replaceNewDiff);
        oldDiff  = strrep(oldDiff,newline,'<br>');
        newDiff  = strrep(newDiff,newline,'<br>');
        
        if (false == strcmp(incrementalValue, newValue))
            % Detected a change for this parameter. Register for the diff
            % report's tabulation of which diagrams use this property
            matchedIdx(end+1) = changeIdx;
            incrementalValue = newValue;
        end
    end
    
    % If something changed, add to report
    if (false == strcmp(newValue, origValue) || isChanged)
        isChanged = true;
        % Redirect library links- this time in prep for actual modify
        libReference = '';
        refPath = GetBlockToModify(aBlock);
        if (false == strcmp(refPath,aBlock))
            libReference = sprintf('<br>Lib-block: <em>%s</em>', ...
                refPath);
        end
        % Tabulate which diagrams reference each parameter
        diagram = bdroot(refPath);
        for i=1:numel(matchedIdx)
            paramName = newNames{matchedIdx(i)};
            if (paramToModelMap.isKey(paramName))
                % Update
                diagramsThatUseParam = paramToModelMap(paramName);
            else
                % Create
                diagramsThatUseParam = {};
            end
            if (false == ismember(diagram,diagramsThatUseParam))
                diagramsThatUseParam{end+1} = diagram;
                paramToModelMap(paramName) = sort(diagramsThatUseParam);
            end
        end
        
        diffReport = diffReport + ...
            '<tr><td colspan="2" bgcolor="#eeeeee">' + ...
            'Block: [' + aBlock + '] ' + ...
            description + ...
            libReference + ...
            '</td></tr>' + ...
            ['<tr><td>' oldDiff '</td><td>' newDiff '</td></tr>'] + ...
            '<tr height="10px"></tr>';
        aBlock = refPath;
    end
end

%--------------------------------------------------------------------------
function readyToModifyPath = GetBlockToModify(aBlock,findOnly)
% Find the block that should be modifed. Could be directly in model, or a
% link inside a link inside a link...
% Also - if a block is ALREADY modified, return empty here to indicate that
% the modifications have already been done. This happens because library
% blocks are visited multiple times (once for each instance of the library
% block used in the model)
    global modifiedLibraries;
    global modifiedBlocks;
    
    if nargin < 2
        findOnly = false;
    end
    
    % Discover if parent is involved in link. If so, we must unlock its
    % library and redirect aBlock path to it's parent's library's instance
    readyToModifyPath = aBlock;
    containingDiagram = bdroot(readyToModifyPath);
    blockName = get_param(aBlock,'Name');
    parent = get_param(aBlock,'Parent');
    grandParent = get_param(parent,'Parent');
    if (grandParent ~= "")
        % Parent block might be a link
        parentRef = get_param(parent,'ReferenceBlock');
        if ("none" ~= get_param(parent,'LinkStatus'))
            % Redirect to library version of block
            load_system(parentRef);
            containingDiagram = bdroot(parentRef);
            readyToModifyPath = MaskUtil.SubBlockName(parentRef,blockName);
        end
    end
    % Now we have a ready to modify path. Just unlock it!
    if (bdIsLibrary(containingDiagram))
        libObj = get_param(containingDiagram,'Object');
        if (findOnly == false && strcmp('on',libObj.Lock))
            libObj.Lock = 'off';
            modifiedLibraries{end+1} = containingDiagram;
        end
    end
    
    % Now that we have a block, check if it has already been visited
    % Bail out if this block has already been modified
    if (ismember(readyToModifyPath,modifiedBlocks))
        % already got this one
        readyToModifyPath = [];
    elseif (findOnly == false)
        % this block is really going to be modified. Never do it again.
        modifiedBlocks{end+1} = readyToModifyPath;
    end
    
end

%--------------------------------------------------------------------------
function WriteReportFile(fileName,className)
    global oldNames;
    global newNames;
    global defaultValues;
    global diffReport;
    global modifiedLibraries;
    global paramToModelMap;

    reportPath = fullfile(tempdir,['Diff_' fileName '.html']);
    fidReport = fopen(reportPath,'w+');

    % Doc header
    fprintf(fidReport, ...
    ['<!DOCTYPE html><html><head><style>td{border: 1px solid #CCCCCC; }' ...
    '</style></head><body style="font-family:monospace;">' ...
    '<h2>Rename Report: Workspace to Config Object</h2>']);

    % Mask changes summary
    fprintf(fidReport,'<table><tr><td><b>Base Workspace Name</b></td>');
    fprintf(fidReport,'<td><b>New classdef Property Name</b></td>');
    fprintf(fidReport,'<td><b>Default Value</b></td>');
    fprintf(fidReport,'<td><b>Invoked in Diagrams</b></td></tr>');
    for nIdx=1:numel(oldNames)
        fprintf(fidReport,'<tr><td style="border:none;">%s</td>', ...
            oldNames{nIdx});
        fprintf(fidReport,'<td style="border:none;">%s</td>', ...
            newNames{nIdx});
        fprintf(fidReport,'<td style="border:none;">%s</td>', ...
            defaultValues{nIdx});
        if (paramToModelMap.isKey(newNames{nIdx}))
            diagrams = strjoin(paramToModelMap(newNames{nIdx}),', ');
        else
            diagrams = '';
        end
        fprintf(fidReport,'<td style="border:none;">%s</td></tr>', ...
            diagrams);
    end
    fprintf(fidReport, '</tr></table>\n');
    
    % New classdef
    % Cheap & cheerful HTML encoder:
    htmlEntFind = {'&','"','''','<','>','|',newline};
    htmlEntRep  = {'&amp;','&quot;','&apos;','&lt;','&gt;','&brvbar;',['<br>' newline]};
    classdefFilePath = sprintf('%s.m',className);
    classdefText = fileread(classdefFilePath);
    for i=1:numel(htmlEntFind)
        classdefText = strrep(classdefText,htmlEntFind{i},htmlEntRep{i});
    end
    fprintf(fidReport,'<h3>New classdef created</h3>\n');
    fprintf(fidReport,'<div style="border: 1px solid #CCCCCC;">%s</div>\n',classdefText);

    % Modified libraries
    if (numel(modifiedLibraries) > 0)
        fprintf(fidReport,'<h3>Additional Modified Libraries</h3>\n');
        for i=1:numel(modifiedLibraries)
            fprintf(fidReport,'%s<br>',modifiedLibraries{i});
        end
    end
    
    % Differences content
    fprintf(fidReport, ...
    ['<table style="width:100%%">' ...
    '<tr><th>Original</th><th>Renamed</th></tr>']);
    fprintf(fidReport,'%s',diffReport);

    % Doc ending
    fprintf(fidReport,'\n</table></body></html>\n');
    fclose(fidReport);
    
    winopen(reportPath);

end

%--------------------------------------------------------------------------
function goodToGo = DoGetInput(directiveFileName)
    global oldNames;
    global newNames;
    global defaultValues;
    
    goodToGo = true;
    
    % Acquire the directives
    directiveText = fileread(directiveFileName);
    
    % Parse out the 3 elements of each parameter declaration
    %     ClassdefParameterName = BASE_WS_NAME = default-value;
    theRegEx = '\s*(\w*)\s*=\s*(\w*)\s*=\s*([^;]*)[;]';
    matchedTokens = regexp(directiveText,theRegEx,'tokens','lineanchors');
    
    % Loop through the matches and create the three parallel arrays
    oldNames = {};
    newNames = {};
    defaultValues = {};
    errCell = {};
    for i=1:numel(matchedTokens)
        thisSet = matchedTokens{i};
        
        newNames{end+1} = thisSet{1};
        oldNames{end+1} = thisSet{2};
        defaultValues{end+1} = thisSet{3};
    end
    
    % Rules for bending the rules:
    % 1: oldNames can be used only once in the directives. How can the same
    % old name map to more than one new name!
    [C,~,IC] = unique(oldNames);
    N = histcounts(IC,numel(C));
    repeaters = C(N>1);
    if (false == isempty(repeaters))
        goodToGo = false;
        fprintf('Old workspace names specified more than once in directives:\n[%s]\n', ...
            strjoin(repeaters,', '));
    end
    % 2: New names can show up more than once, but only if the default
    % values match in all cases
    [C,~,IC] = unique(newNames);
    N = histcounts(IC,numel(C));
    repeaters = C(N>1);
    % For each repeater, ensure defaults match. If so, empty out the
    % default value for any repeats- leaving only the first default.
    for i=1:numel(repeaters)
        aName = repeaters{i};
        matches = ismember(newNames,aName);
        defaults = defaultValues(matches);
        if (numel(unique(defaults)) > 1)
            fprintf('New property [%s] specified with multiple defaults [%s]\n', ...
                aName, strjoin(defaults,', '));
            goodToGo = false;
        else
            % Same default value specified multiple times. Just empty
            % out all but first as an indicator for the classdef maker
            % Generate index values so we can chop off the first one
            matchIdx = find(matches);
            matchIdx (1) = [];
            % and then remove defaults at all the remaining index values
            defaultValues(matchIdx) = {[]};
        end
    end
    
    if (goodToGo == false)
        fprintf('Enable to continue. Aborted.\n');
    end
end

%--------------------------------------------------------------------------
function DoReplaceSubBlocks(blockPath,okToModifyRootPath)

    % OK - get to work. Find every block inside and replace names
    allBlocks = find_system(blockPath, ...
        'LookUnderMasks','all','FollowLinks','on','MatchFilter',@Simulink.match.allVariants);
    numBlocks = numel(allBlocks);
    
    % Find the root path (i.e. filesystem place where block lives)
    rootModel = bdroot(blockPath);
    rootPath = fileparts(get_param(rootModel,'FileName'));    
    
    fprintf('Analyzing %d blocks...\n', numBlocks);
    
    % For each block, search all dialog parameters and do replace
    for blockIdx=1:numBlocks
        % Block loop
        if (0 == mod(blockIdx,10))
            fprintf('.');
        end
        if (0 == mod(blockIdx,1000))
            fprintf('\n');
        end
        aBlock = allBlocks{blockIdx};
        if (strcmp(aBlock,blockPath))
            % skip the masked block itself
            continue;
        end

        % Check if block is even elgible for modification
        wouldModBlock = GetBlockToModify(aBlock,true);
        if (isempty(wouldModBlock))
            % Already visited this block. Move on.
            continue;
        end
        wouldModel = strtok(wouldModBlock,'/');
        load_system(wouldModel);
        wouldPath = get_param(wouldModel,'FileName');
        if (false == okToModifyRootPath && contains(wouldPath,rootPath))
            % Block is not found somewhere we want to change. Move on.
            continue;
        end
        
        % Special MATLAB Function Block parameter handling: Attempt to
        % perform rename operations inside the MATLAB function script
        isMFB = strcmp('SubSystem',get_param(aBlock,'BlockType')) && ...
            strcmp('MATLAB Function',get_param(aBlock,'SFBlockType'));
        if (isMFB)
            SearchMatlabFunctionBlockParameters(aBlock);
        end
        
        % Block Dialog Parameters
        dialogParams = get_param(aBlock,'DialogParameters');
        if (false == isstruct(dialogParams))
            % No parameters. Move on.
            continue;
        end
        paramNames = fields(dialogParams);
        numParams = numel(paramNames);
        relativeBlockPath = regexprep(aBlock,['^' blockPath '/'],'');
        aBlockMask = get_param(aBlock,'MaskObject');
        
        % For each parameter, search/replace for new names
        for blockParamIdx=1:numParams
            % Block's parameter loop
            paramName = paramNames{blockParamIdx};
            
            % Check if this is a user mask with an evaluated parameter
            if (isempty(aBlockMask))
                evalDesc = '<unknown if evaluated>';
            else
                theMaskParam = aBlockMask.getParameter(paramName);
                if isempty(theMaskParam)
                    evalDesc = '<unknown if evaluated>';
                elseif (strcmp(theMaskParam.Evaluate,'on'))
                    evalDesc = '<b>Evaluated</b>';
                else
                    evalDesc = 'Not-Evaluated (red highlights probably ok)';
                end
            end
            
            blockParam = dialogParams.(paramName);
            isExpr = strcmp('string',blockParam.Type);
            if (false == isExpr)
                % This parameter isn't even a string. Move on.
                continue;
            end
            if (any(ismember(blockParam.Attributes,'read-only')))
                % This parameter is read-only. Move on.
                continue;
            end
            
            oldValue = get_param(aBlock,paramName);
            % For each new name, perform the search/replace (iteratively)
            description = sprintf('Parameter:[%s]        %s', ...
                paramName,evalDesc);
            [newValue,isChanged,aBlock] = PerformReplace(aBlock,oldValue,description);
            if (isChanged)
                % OK - Make the change!
                MaskUtil.SetPromotedParameter(aBlock,paramName,newValue);
            end
        end % parameter loop
    end % block loop
end

%--------------------------------------------------------------------------
function SearchMatlabFunctionBlockParameters(aBlock)
    global oldNames;
    global newNames;

    sfBlock = [];
    
    path = aBlock;
    % Get from inside a library linked block!
    reference = get_param(path,'ReferenceBlock');
    if (~isempty(reference))
        path = reference;
    end

    sf = sfroot();
    sfBlock = sf.find('Path',path,'-isa','Stateflow.EMChart');
    
    if isempty(sfBlock)
        % we failed to find the block. Give up.
        return;
    end
    
    % Check if each argument is a parameter that is found in oldNames. If
    % so, add to search/replace indexes for this block's script:
    args = sfBlock.getChildren();
    numArgs = numel(args);
    foundIndexes = [];
    for i=1:numArgs
        arg = args(i);
        if (strcmp('Parameter',arg.Scope))
            [~,foundIdx] = ismember(arg.Name,oldNames);
            if (foundIdx > 0)
                % This parameter is being renamed! Add to replacement list
                foundIndexes(end+1) = foundIdx;
            end
        end
    end
    
    % Get outta here if no names were found
    if (isempty(foundIndexes))
        return;
    end
    
    description = sprintf('"%s"', 'Script inside MATLAB Function Block');
    [newScript,isChanged,newBlock] = PerformReplace(path,sfBlock.Script, ...
        description,foundIndexes,true);
    
    if (isChanged)
        % OK - Make the change!
        modBlock = sf.find('Path',newBlock,'-isa','Stateflow.EMChart');
        modBlock.Script = newScript;
    end
    
end


%--------------------------------------------------------------------------
function DoCreateClassdef(className)
    global newNames;
    global defaultValues;
    global paramToModelMap;
    
   
    fileHeaderLines = {
        sprintf('classdef %s',className)
        sprintf('%% %s provides build-time configuration settings',className)
        ''
        '% Copyright 2020 Bose Corporation'
        ''
        'properties'
        };
    fileFooterLines = {
        ''
        'end % Properties'
        'end'
        };
    
    propertyLines = {};
    for i=1:numel(newNames)
        if (paramToModelMap.isKey(newNames{i}) && false == isempty(defaultValues{i}))
            propertyLines{end+1} = sprintf('    %s = %s;', ...
                newNames{i},defaultValues{i});
        else
            % No used or empty default.
        end
    end
    
    % Build the file text
    allLines = vertcat(fileHeaderLines, propertyLines.', fileFooterLines);
    theText = strjoin(allLines, newline);
    
    % For now, just dump the new file in the current directory :-(
    filePath = sprintf('%s.m',className);
    fid = fopen(filePath,'w+');
    fprintf(fid,'%s',theText);
    fclose(fid);
end
