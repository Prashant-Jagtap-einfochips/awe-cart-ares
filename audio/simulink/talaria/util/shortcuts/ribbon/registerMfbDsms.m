function registerMfbDsms(blockPath)
% registerMfbDsms registers DataStoreMemory scope for MATLAB Function Block
% variables declared as global.
% Usage:
%    registerMfbDsms(blockPath)

%   Copyright 2022 Bose Corporation

    % Use the diagnostic viewer for status messages
    myStage = sldiagviewer.createStage('TalariaAutomation','ModelName',bdroot(blockPath));

    % find the Embedded MATLAB Function Block object
    sf = sfroot();
    sfBlock = sf.find('Path',blockPath,'-isa','Stateflow.EMChart');
    if isempty(sfBlock)
        error('The currently selected block ''%s'' does not appear to be an Embedded MATLAB Function Block.', ...
            blockPath);
    end

    % ... otherwise, let's register those datastores!
    % First, find the declares "global" variables in the script
    searchExp = '^\s*global\s*(\w*)';
    tokens = regexp(sfBlock.script,searchExp,'lineanchors','tokens');
    dstoreName = vertcat(tokens{:});
    
    % Now iterate over the block's datastore registrations removing or
    % correcting any names that match our script-avowed list of dstores
    dataElement = sfBlock.getChildren();
    numElems = numel(dataElement);
    dstoreScope = 'Data Store Memory';
    numAdded = 0;
    numCorrected = 0;
    numRemoved = 0;
    numAllSet = 0;
    warningString = '';
    for i=1:numElems
        anElem = dataElement(i);
        % Check if the name matches a declared global of the MATLAB func
        idx = find(strcmp(dstoreName,anElem.Name));
        if (false == isempty(idx))
            % Yes- it's a matching name. Remove from our list
            theBlockName = dstoreName{idx};
            dstoreName(idx) = [];
            % Yes- the name matches, so make sure it *IS* a dstore
            if (strcmp(anElem.Scope,dstoreScope))
                numAllSet = numAllSet + 1;
            else
                % Yikes! The name matches, but it's not a dstore. Fix it!
                anElem.Scope = dstoreScope;
                numCorrected = numCorrected + 1;
            end
            % Now check on the datastore memory block itself
            blockH = FindDatastoreBlock(theBlockName, blockPath);
            if (isempty(blockH))
                warningString = sprintf('%sCould not find DataStoreMemory block for [%s]\n', ...
                    warningString,theBlockName);
            elseif (strcmp('Auto',get_param(blockH,'StateStorageClass')))
                warningString = sprintf('%sMemory block [%s] is of "Auto" storage class.\n', ...
                    warningString,theBlockName);
            end
        else
            % No- the name does not match, so make sure it's NOT a dstore
            if (strcmp(anElem.Scope,dstoreScope))
                % A registered dstore that's not in the script. Delete it!
                anElem.delete;
                numRemoved = numRemoved + 1;
            end
            % otherwise who cares- the name does not match and it's not a
            % dstore... so it's an input,output,parameter, etc...
        end
    end
    
    % Finally, iterate over any remaining dstore names that were not found
    % yet and add them in
    numToAdd = numel(dstoreName);
    for i=1:numToAdd
        newElem = Stateflow.Data(sfBlock);
        newElem.Name = dstoreName{i};
        newElem.Scope = dstoreScope;
        numAdded = numAdded + 1;
    end
    
    dlgMsg = sprintf([ ...
        'Adjusting block ''%s''\n' ...
        'Succesfully Datastore registrations:\n' ...
        '   %d Datastore registrations added\n' ...
        '   %d Datastore registrations removed\n' ...
        '   %d Registrations found and corrected to be Datastores\n' ...
        '   %d Registrations already all set and left alone!'], ...
        getfullname(blockPath),numAdded, numRemoved, numCorrected, numAllSet);
    sldiagviewer.reportInfo(dlgMsg,'Component','MATLAB Function Block');
    if (false == isempty(warningString))
            sldiagviewer.reportWarning(warningString,'Component','MATLAB Function Block');
    end
end

function blockH = FindDatastoreBlock(dstoreName, referenceBlock)
    system = get_param(referenceBlock,'Parent');
    blockH = [];
    F = Simulink.FindOptions("FollowLinks",true,"SearchDepth",1);
    while (false == isempty(system) && isempty(blockH))
        blockH = Simulink.findBlocksOfType(system,'DataStoreMemory','DataStoreName',dstoreName,F);
        system = get_param(system,'Parent');
    end    
end