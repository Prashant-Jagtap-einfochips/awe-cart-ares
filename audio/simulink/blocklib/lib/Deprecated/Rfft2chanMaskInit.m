function Rfft2chanMaskInit(gcbh, fftSize, bufferAlias, generateGenericCode)

if fftSize ~= 256 && fftSize ~= 128
    error('The current supported FFT sizes are 256 and 128 only.');
end

% Parallel lists. memSectionNames{i} is memory section for workingBufferNames{i}
workingBufferNames = {'temp_r', 'temp_i'};
memSectionNames = {'FFTMemL1DmWordAlign', 'FFTMemL1PmWordAlign'};

for i = 1:1:length(workingBufferNames)
    
    currentBufferName = workingBufferNames{i};
    
    % Get block handle
    bufferBlockPath = MaskUtil.SubBlockName(gcbh, currentBufferName);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    if -1 ~= bufferBlockHandle   
        % Get Coder Info of the signal object
        theStateSignalObject = get(bufferBlockHandle, 'StateSignalObject');
        theCoderInfo = get(theStateSignalObject,'CoderInfo');

        if (generateGenericCode == 1)
            % Set alias
            identifier = ['FFT_' bufferAlias '_' currentBufferName];
            CscUtil.SetDatastoreCsc(bufferBlockPath, 'Blocklib', 'FFTBuffer', 'Identifier', identifier);

            cm = CscUtil.GetMappingObj(bufferBlockHandle);
            cm.setDataStore(bufferBlockHandle, 'MemorySection', [memSectionNames{i} num2str(fftSize)]);
        elseif (generateGenericCode == 2)
            theCoderInfo.Alias = '';
            theCoderInfo.StorageClass = 'Auto';
        end
    end

end
end