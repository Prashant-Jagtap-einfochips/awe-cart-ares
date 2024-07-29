function RifftMaskInit(gcbh, fftSize, bufferAlias, generateGenericCode)

% Check if fftSize is supported
if fftSize ~= 256 && fftSize ~= 128
    error('The current supported FFT sizes are 256 and 128 only.');
end

% Parallel lists. memSectionNames{i} is memory section for workingBufferNames{i}
workingBufferNames = {'data_r', 'data_i', 'temp_r', 'temp_i'};
memSectionNames = {'FFTMemL1DmWordAlign', 'FFTMemL1PmWordAlign', 'FFTMemL1DmWordAlign', 'FFTMemL1PmWordAlign'};

for i = 1:1:length(workingBufferNames)
    
    currentBufferName = workingBufferNames{i};
    
    % Get block handle
    bufferBlockPath = MaskUtil.SubBlockName(gcbh, currentBufferName);
    bufferBlockHandle = getSimulinkBlockHandle(bufferBlockPath);

    if -1 ~= bufferBlockHandle   
        if (generateGenericCode == 1)
            % Set alias
            identifier = ['IFFT_' bufferAlias '_' currentBufferName];
            CscUtil.SetDatastoreCsc(bufferBlockHandle, 'Blocklib', 'FFTBuffer', 'Identifier', identifier);

            cm = CscUtil.GetMappingObj(bufferBlockHandle);
            cm.setDataStore(bufferBlockHandle, 'MemorySection', [memSectionNames{i} num2str(fftSize)]);
        elseif (generateGenericCode == 2)
            CscUtil.SetDatastoreCsc(bufferBlockHandle, 'Simulink', 'Auto');
        end
    end
end
end