function FirGenericMask(blkh, implementation)

model = bdroot(blkh);
if bdIsLibrary(model)
    return;
end

% Data stores in the FIR Generic block and their Custom Storage Class 
% memory sections
blockDataStores = {'codeWorkMemory', 'stateBuffer'};
blockMemorySections = {'MemL1', 'MemSegment'};

for i = 1 : 1: numel(blockDataStores)
    
    % Get the data store buffer name
    dataStore = blockDataStores{i};

    % Get block handle of the data store
    bufferBlockPath = MaskUtil.SubBlockName(blkh, dataStore);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    % Create unique alias for data store name from buffer block
    % path, removing spaces and replacing all non-alphanumeric
    % characters with underscores
    bufferAlias = replace(bufferBlockPath, ' ', '');
    bufferAlias = regexprep(bufferAlias, '\W', '_');
    
    if -1 ~= bufferBlockHandle

        % Get Coder Info of the data store signal object used in the 
        % FIR Generic block
        CscUtil.SetDatastoreCsc(bufferBlockHandle, ...
            'Blocklib', 'FirGenericStorage', ...
            'Identifier', bufferAlias);

        % Set the Custom Storage Class memory section
        if 3 == implementation
            % Generic implementation:  Set to default to eliminate memory 
            % section pragma mapping and alignment
            mem_section = 'Default';
        else
            % Core and Accelerator Memory implementations:  Set to 
            % designated section 
            mem_section = blockMemorySections{i};
            
        end
        cm =  CscUtil.GetMappingObj(bufferBlockHandle);
        cm.setDataStore(bufferBlockHandle, 'MemorySection', mem_section);

        % Set the header name for this top level model
        fullHeaderFilename = ...
               [getfullname(bdroot(bufferBlockHandle)) '_FirGenericStorage.h'];
        cm.setDataStore(bufferBlockHandle, 'HeaderFile', fullHeaderFilename);

        % Set definition file name for this top level model
        fullDefinitionFilename = ...
               [getfullname(bdroot(bufferBlockHandle)) '_FirGenericStorage.c'];
        cm.setDataStore(bufferBlockHandle, 'DefinitionFile', fullDefinitionFilename);
    end

end

%% Generate and assign unique alias for fir generic input buffer
% Get block handle of the corresponding inport
inBufferBlockPath = MaskUtil.SubBlockName(blkh, 'Audio In');
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

