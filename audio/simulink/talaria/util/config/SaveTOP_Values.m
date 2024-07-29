function SaveTOP_Values(model)
% This script is intended to save AudioPilot3 and other works that use
% an early version of TOP block that holds a value in its mask. Run this
% script while using the old TOP library to first extract and dump the
% TOP values into a base workspace map - then save the map, update the
% TOP library (i.e. all of Talaria) and run the other script.

%   Copyright 2018-2019 Bose Corporation
modelName = bdroot(model);

% Find all the TOPs in the model
fprintf('Finding all TOPS in %s...\n',modelName);
allTOPs = find_system(modelName, ...
            'LookUnderMasks', 'on', ...
            'FollowLinks','on', ...
            'MaskType', TOP_Mask.MaskType);

fprintf('Found %d TOPs.\n',numel(allTOPs));


            mapName = [modelName '_ValueSaveFromOldTOP'];
            try
                theMap = evalin('base',mapName);
            catch
                theMap = containers.Map;
                assignin('base',mapName,theMap);
            end

for i = 1:numel(allTOPs)

    aBlock = allTOPs{i};
    fprintf('Converting TOP #%d: %s...\n',i,MaskUtil.GetBlockLinkString(aBlock));

    theValue = get_param(aBlock,'Value');
    
    theMap(aBlock) = theValue;
    
end
