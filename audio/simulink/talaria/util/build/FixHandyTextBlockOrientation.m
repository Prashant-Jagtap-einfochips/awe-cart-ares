function FixHandyTextBlockOrientation(folderNameCell,fixIt)
%FIXHANDYTEXTBLOCKORIENTATION Fixes HandyTextFile block orientation
% Usage:
%   FixHandyTextBlockOrientation(folderNameCell,fixIt)
%
% Examples:
%   FixHandyTextBlockOrientation('module');
%   FixHandyTextBlockOrientation('module',true);

% Copyright 2021 Bose Corporation

if (nargin < 2)
    fixIt = false;
end
if (false == iscell(folderNameCell))
    folderNameCell = {folderNameCell};
end

if (fixIt)
    fprintf('Finding and fixing any HandyTextFile blocks with incorrect orientation\n');
else
    fprintf('Finding HandyTextFile blocks with incorrect orientation, pass 2nd argument of true to fix these\n');
end

% Acquire a list of all *.slx files in the folders provided.
dirExpressions = cellfun(@(x)fullfile('.',x,'**','*.slx'), ...
    folderNameCell,'UniformOutput',false);
theDirsCell = cellfun(@(x)dir(x),dirExpressions,'UniformOutput',false);
allFileStruct = vertcat(theDirsCell{:});
modelNames = extractBefore({allFileStruct.name},'.')';

% For each model, find the HandyTextFile blocks and repair as needed
for i=1:numel(modelNames)
    aModelName = modelNames{i};
    if (exist(aModelName) ~= 4)
        fprintf('Ignoring [%s] which is not found on path\n',aModelName);
        continue;
    end
    fprintf('Checking diagram [%s]...\n',aModelName);
    load_system(aModelName);
    F = Simulink.FindOptions("FollowLinks",true);
    blocks = Simulink.findBlocks(bdroot,'MaskType',HandyTextFileMask.MaskType,F);

    % For each found block, figure out what to do
    for b=1:numel(blocks)
        aBlock = blocks{b};
        FixOrientation(aBlock,fixIt);
    end
end

if (numel(modelNames) == 0)
    fprintf('No models found.\n');
end

fprintf('Done.');
if (fixIt)
    fprintf('Review changes in any opened diagrams and save as appropriate!\n');
else
    fprintf('\n');
end

end

function FixOrientation(aBlock,fixIt)
% FixOrientation discovers if provided block is located in a resolved link
% or not and takes action as necessary to correct orientation of the
% "source" block (i.e. follows the link if appropriate)

includeDisabledLinks = true;
[ancestorLibRef,ancestorBlock,isUnresolved] = MaskUtil.FindAncestorLibRef(aBlock,includeDisabledLinks);

if (isempty(ancestorLibRef))
    theRealBlock = aBlock;
else
    % Build source block path relative from libRef
    relativePath = extractAfter(aBlock,numel(ancestorBlock));
    theRealBlock = [ancestorLibRef,relativePath];
end

% Get the orientation and do something if it's not "right" (is that a pun?)
currentOrientation = get_param(aBlock,'Orientation');
if ("right" ~= currentOrientation)
    fprintf('  Block [%s] incorrect [%s]',...
        MaskUtil.GetBlockLinkString(aBlock),currentOrientation);
    if (false == isempty(ancestorBlock))
        fprintf(' (reference of [%s])', ...
            MaskUtil.GetBlockLinkString(theRealBlock));
        if (isUnresolved)
            fprintf(' (via a DISABLED link)');
        end
    end
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
       set_param(theRealBlock,'Orientation','right');
        fprintf('... FIXED\n');
        % Open the system so user can choose to save it
        open_system(diagramName);
    else
        fprintf('... unchanged.\n');
    end    
end
end