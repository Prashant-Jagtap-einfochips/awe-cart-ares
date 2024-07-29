function FirAccelMask(blkh, blockDataStores, blockMemorySections, alias)

model = bdroot(blkh);
if bdIsLibrary(model)
    return;
end

for i = 1 : 1: numel(blockDataStores)
    
    % Get the data store buffer name
    dataStore = blockDataStores{i};

    % Get block handle of the data store
    bufferBlockPath = MaskUtil.SubBlockName(blkh, dataStore);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    % Create unique alias for data store name
    bufferAlias = [char(alias) char(dataStore)];
    
    if -1 ~= bufferBlockHandle

        % Set the header name for this top level model
        fullHeaderFilename = ...
            [getfullname(bdroot(bufferBlockHandle)) '_FirGenericStorage.h'];

        % Set definition file name for this top level model
        fullDefinitionFilename = ...
            [getfullname(bdroot(bufferBlockHandle)) '_FirGenericStorage.c'];

        % Set the Custom Storage Class memory section
        mem_section = blockMemorySections{i};

        % Get Coder Info of the data store signal object used in the
        % FIR Generic block
        CscUtil.SetDatastoreCsc(bufferBlockHandle, ...
            'Blocklib', 'FirGenericStorage', ...
            'Identifier', bufferAlias, ...
            'MemorySection', mem_section, ...
            'HeaderFile', fullHeaderFilename, ...
            'DefinitionFile', fullDefinitionFilename);

    end

end

%% Generate and assign unique alias for fir generic input buffer
% Get block handle of the corresponding inport
inBufferBlockPath = MaskUtil.SubBlockName(blkh, 'AudioIn');
bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

if -1 ~= bufferBlockHandle
    % Create a unique alias for the signal using 32 bit encoding scheme
    signalBufferAlias = sprintf('firGenericInputBuffer%s', crc32(inBufferBlockPath));
    
    % Get port handle for the specified inport
    CscUtil.SetOutportCsc( ...
        inBufferBlockPath, 1, ...
        'Blocklib', 'AccelBuffer', ...
        'Identifier', signalBufferAlias);
end

end

