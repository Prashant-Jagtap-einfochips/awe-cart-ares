function SharedDelayMask(blkh)

% Data stores in the SharedDelay block and their Custom Storage Class 
% memory sections
blockDataStores = {'codeWorkMemory', 'delayBuffer', 'fastDmaBuffer'};

for i = 1:1:length(blockDataStores)
    
    % Get the data store buffer name
    dataStoreName = blockDataStores{i};

    % Get block handle of the data store
    bufferBlockPath = MaskUtil.SubBlockName(blkh, dataStoreName);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    % Create unique alias for data store name from buffer block
    % path, removing spaces and replacing all non-alphanumeric
    % characters with underscores
    bufferAlias = replace(bufferBlockPath, ' ', '');
    bufferAlias = regexprep(bufferAlias, '\W', '_');
    
    if -1 ~= bufferBlockHandle

        % Get Coder Info of the data store signal object used in the 
        % Shared Delay block
        theStateSignalObject = get(bufferBlockHandle, 'StateSignalObject');
        theCoderInfo = get(theStateSignalObject, 'CoderInfo');
        
        % Set the alias for this instance of the data store
        theCoderInfo.Alias = bufferAlias;

        % Set the header name for this top level model
        fullHeaderFilename = ...
                [getfullname(bdroot(bufferBlockHandle)) '_SharedDelayStorage.h'];
        theCoderInfo.CustomAttributes.HeaderFile = fullHeaderFilename;

        % Set definition file name for this top level model
        fullDefinitionFilename = ...
                [getfullname(bdroot(bufferBlockHandle)) '_SharedDelayStorage.c'];
        theCoderInfo.CustomAttributes.DefinitionFile = fullDefinitionFilename;

    end

end

