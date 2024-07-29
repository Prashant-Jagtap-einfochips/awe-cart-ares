function resultsStruct = BtcUpgradeTuneVars(rootFolder,writeChangesToFile)
%BtcUpgradeTuneVars Removes the aborted "Get" mechanism
%   Provide a root folder in which to search for files ending in .slx
% This function will attempt to upgrade the diagrams it finds to remove
% TuneVar references to Get, and remove property conversions performed on
% canvas (recording these is PartialFixList.
% Make sure to review the PartiallyFixed list in the results struct as
% these TuneVars have been stripped of some transforms that must be added
% to the Config classdefs to reconstruct the same behavior as before.
% Usage:
%    resultsStruct = BtcUpgradeTuneVars(rootFolder,writeChangesToFile);
%
% ... then dig through resultsStruct, especially PartialFixList
%
% See also: BtcUpgradeObservers

if false == exist('writeChangesToFile','var')
    writeChangesToFile = false;
end

% Search for potential TuneVar-containing SLX files
filesystemSearchExpr = sprintf('%s/**/*.slx',rootFolder);
foundFiles = dir(filesystemSearchExpr);

% Loop through files and discover what they contain, attempting to upgrade
% as appears appropriate

ignoreList = string.empty; % these TuneVar are not recognized as BTC
unGetList = string.empty; % these TuneVar were already using "Get"
modifiedModels = string.empty;
partialFixList = string.empty;

for i=1:numel(foundFiles)
    aDirEntry = foundFiles(i);
    aFilename = fullfile(aDirEntry.folder,aDirEntry.name);

    fprintf('   %s...\n',aFilename);
    model = load_system(aFilename);

    % Search for all TuneVars, all variants, under masks, but not links
    F = Simulink.FindOptions("LookUnderMasks","All");
    tuneVarBlocks = Simulink.findBlocks(model,'MaskType',TuneVarMask.MaskType,F);
    tuneTopBlocks = Simulink.findBlocks(model,'MaskType',TuneTOP_Mask.MaskType,F);
    blocks = vertcat(tuneVarBlocks,tuneTopBlocks);
    numBlocks = numel(blocks);

    foundCount = 0;

    % Loop through each TuneVarblock in this model, checking its default
    for b=1:numBlocks
        aBlock = getfullname(blocks(b));
        aBlockSafeName = strrep(aBlock,newline,'');
        defaultValue = get_param(aBlock,'MaskValue');
        % Search for "Config.Variables.Here"
        eligiblePattern = '(?<=^Config\.)(?<VarName>.*)';
        result = regexp(defaultValue,eligiblePattern,"names");

        if (isempty(result) || contains(defaultValue,'(:)'))
            % Could not find the "expected" BTC pattern, but search more
            % loosely to see if the expected pattern is hidden inside other
            % transforms (that break automation)
            % Pull out just Config.Variable from anywhere it might be found

            % But first undo any "Get" stuff
            unGetDefault = defaultValue ;
            removePattern = '(?<Removed1>Config\.Get\(@)(?<VarName>[^\)]*)(?<Removed2>\))';
            removeResult = regexp(defaultValue,removePattern,"names");
            if (false == isempty(removeResult))
                oldpiece = [removeResult.Removed1 removeResult.VarName removeResult.Removed2];
                unGetDefault = strrep(defaultValue,oldpiece,removeResult.VarName);
            end

            partialPattern = '(?<Config>(?<!@)Config)\.(?<variable>(\w*\.?)*)';
            partialResult = regexp(unGetDefault,partialPattern,"names");
            if (isempty(partialResult))
                % No Config stuff found. This TuneVar is beyond help
                ignoreList(end+1) = sprintf('%s = %s',aBlockSafeName,defaultValue);
            elseif (writeChangesToFile)
                % Found Config stuff in the TuneVar value, but it's trapped
                % inside on-canvas mutations e.g. casting etc.
                foundCount  = foundCount + 1;
                newVal = sprintf('Config.%s',partialResult.variable);
                % Hack in an exception case found in blocklib (using enums)
                if (contains(unGetDefault,"int32("))
                    % Just put pack in the casting for now
                    newVal = sprintf('int32(%s)',newVal);
                end
                % Also strip out the (:) found in blocklib - should be a
                % property dimension restriction instead of a transform on
                % the value after the fact.
                newVal = strrep(newVal,'(:)','');
                FixTuneVar(aBlock,newVal);
                modifiedModels(end+1) = bdroot(aBlock);
                fprintf('Partially Fixed %s = %s\n',aBlockSafeName,newVal);
                % Discover classname where rest of fix must happen
                parentMask = MaskUtil.FindParentMask(aBlock);
                configParam = parentMask.getParameter('Config');
                % Record the partial fix information to help somebody
                % continue the fix maually after this is done:
                partialFixList(end+1) = sprintf('class[%s] Originally[%s] Now[%s] block[%s]', ...
                    configParam.Prompt,defaultValue,newVal,aBlockSafeName);
            else
                fprintf('Could partially fix %s %s\n',aBlockSafeName,defaultValue);
            end
        else
            if (contains(result.VarName,"Get("))
                % This one has been "upgraded" to Get. Reverse course!
                removePattern = '(?<Removed>Config\.Get\(@)(?<VarName>[^\)]*)\)';
                removeResult = regexp(defaultValue,removePattern,"names");
                newVal = removeResult.VarName;
                if (writeChangesToFile)
                    FixTuneVar(aBlock,newVal);
                    modifiedModels(end+1) = bdroot(aBlock);
                    foundCount  = foundCount + 1;
                    unGetList(end+1) = sprintf('%s: %s ==> %s',aBlockSafeName,defaultValue,newVal);
                    fprintf('Un-Get Fixed %s = %s\n',aBlockSafeName,newVal);
                else
                    fprintf('Could un-Get %s = %s\n',aBlockSafeName,newVal);
                end
            else
                % Got one that should be left alone!
            end
        end
    end
    if (0 == foundCount)
        close_system(aFilename);
    end
end

modifiedModels = unique(modifiedModels);

% Now save all the changed models
for s=1:numel(modifiedModels)
    aModel = modifiedModels(s);
    fprintf('saving %s\n',aModel);
    save_system(aModel);
    close_system(aModel);
end

resultsStruct.IgnoredList = ignoreList';
resultsStruct.UnGetList = unGetList';
resultsStruct.ModifiedModels = modifiedModels';
resultsStruct.PartialFixList = partialFixList';
end

function FixTuneVar(aBlock,newVal)
    modelName= bdroot(aBlock);
    libObj = get_param(modelName,'Object');
    libObj.Lock = 'off';
    set_param(aBlock,'MaskValue',newVal);
end
