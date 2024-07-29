function FixMfbVectorOutputs(folderNameCell,fixIt)
%FixMfbVectorOutputs checks MATLAB Function Blocks do not output 1D data
%   Starting with 2021b, MFBs now have a VectorOutputs1D property which
%   enables them to *not* output 1-dimensional vectors (which Simulink does
%   not like as much). The default is to turn this off for new MFBs, but
%   for any MFBs we've already got, there's this function
% Examples:
%   FixMfbVectorOutputs('module');
%   FixMfbVectorOutputs('module',true);

% Copyright 2021 Bose Corporation

if (nargin < 2)
    fixIt = false;
end
if (false == iscell(folderNameCell))
    folderNameCell = {folderNameCell};
end

if (fixIt)
    fprintf('Finding and fixing any MATLAB Function Blocks with old-style 1D output\n');
else
    fprintf('Finding MATLAB Function Blocks with old-style 1D output, pass 2nd argument of true to fix these\n');
end

% Acquire a list of all *.slx files in the folders provided.
dirExpressions = cellfun(@(x)fullfile('.',x,'**','*.slx'), ...
    folderNameCell,'UniformOutput',false);
theDirsCell = cellfun(@(x)dir(x),dirExpressions,'UniformOutput',false);
allFileStruct = vertcat(theDirsCell{:});
modelNames = extractBefore({allFileStruct.name},'.')';

% Acquire the StateFlow root. MATLAB Function Blocks are StateFlow charts
sf = sfroot();

% For each model, find MFBs and repair as needed
for i=1:numel(modelNames)
    aModelName = modelNames{i};
    if (exist(aModelName) ~= 4)
        fprintf('Ignoring [%s] which is not found on path\n',aModelName);
        continue;
    end
    fprintf('Checking diagram [%s]...\n',aModelName);
    load_system(aModelName);

    modelMFBs = sf.find('-isa','Stateflow.EMChart','-regexp','Path',['^' aModelName '/']);

    numMFBs = numel(modelMFBs);
    for b=1:numMFBs
        anMFB = modelMFBs(b);
        CheckAndFix(anMFB, fixIt);
    end

end

fprintf('Done.\n');

end

%--------------------------------------------------------------------------
function CheckAndFix(anMFB, fixIt)

if (0 == anMFB.VectorOutputs1D)
    % No problems here. Move on
    return;
end

% Otherwise... tell the world of this offense
aBlock = anMFB.Path;

% First check library link status
includeDisabledLinks = true;
[ancestorLibRef,ancestorBlock,isUnresolved] = MaskUtil.FindAncestorLibRef(aBlock,includeDisabledLinks);
if (isempty(ancestorLibRef))
    theRealBlock = aBlock;
else
    % Build source block path relative from libRef
    relativePath = extractAfter(aBlock,numel(ancestorBlock));
    theRealBlock = [ancestorLibRef,relativePath];
end

% Announce what we know
fprintf('  Block [%s] outputs 1D',MaskUtil.GetBlockLinkString(aBlock));
if (false == isempty(ancestorBlock))
    fprintf(' (reference of [%s])', ...
        MaskUtil.GetBlockLinkString(theRealBlock));
    if (isUnresolved)
        fprintf(' (via a DISABLED link)');
    end
end

% Take action, if bold empowered to do so
if (fixIt)
    % unlock libraries if any
    diagramName = fileparts(theRealBlock);
    load_system(diagramName);
    diagramName = bdroot(theRealBlock);
    if (bdIsLibrary(diagramName) && "on" == get_param(diagramName,'Lock'))
        % UNLOCK!
        set_param(diagramName,'Lock','off');
    end
    % FIX!
    anMFB.VectorOutputs1D = 0;
    fprintf('... FIXED\n');
    % Open the system so user can choose to save it
    open_system(diagramName);
else
    fprintf('... unchanged.\n');
end

end