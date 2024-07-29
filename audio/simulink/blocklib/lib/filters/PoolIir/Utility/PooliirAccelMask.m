function PooliirAccelMask(blkh, blockDataStores)

model = bdroot(blkh);
if bdIsLibrary(model)
    return;
end

% Data stores in the Pool IIR block

for i = 1 : 1: numel(blockDataStores)

    % Get the data store buffer name
    dataStore = blockDataStores{i};

    % Get block handle of the data store
    bufferBlockPath = MaskUtil.SubBlockName(blkh, dataStore);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    % Create unique alias for data store name from buffer block
    % path, removing sub-block name and spaces, and replacing all
    % non-alphanumeric characters with underscores
    bufferAlias = erase(bufferBlockPath, '/Pooliir Function');
    bufferAlias = replace(bufferAlias, ' ', '');
    bufferAlias = regexprep(bufferAlias, '\W', '_');

    if -1 ~= bufferBlockHandle

        % Set the header name for this top level model
        fullHeaderFilename = ...
            [getfullname(bdroot(bufferBlockHandle)) '_PooliirAccelStorage.h'];

        % Set definition file name for this top level model
        fullDefinitionFilename = ...
            [getfullname(bdroot(bufferBlockHandle)) '_PooliirAccelStorage.c'];

        % Obtain Coder Info for the data store signal object used in the
        % Pool IIR block
        CscUtil.SetDatastoreCsc(bufferBlockHandle,'Blocklib','PooliirAccelStorage', ...
            'Identifier', bufferAlias, ...
            'HeaderFile', fullHeaderFilename, ...
            'DefinitionFile', fullDefinitionFilename);

    end

end

%% Generate and assign unique alias for pooliir input buffer
% Get block handle of the corresponding inport
inBufferBlockPath = MaskUtil.SubBlockName(blkh, 'AudioIn');
bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

if -1 ~= bufferBlockHandle
    % Create a unique alias for the signal using 32 bit encoding scheme
    signalBufferAlias = sprintf('poolIIRInputBuffer%s', crc32(inBufferBlockPath));

    % Get port handle for the specified inport
    CscUtil.SetOutportCsc( ...
        inBufferBlockPath, 1, ...
        'Blocklib', 'AccelBuffer', ...
        'Identifier', signalBufferAlias);
end

end