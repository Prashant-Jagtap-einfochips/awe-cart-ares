function outData = TransformAnnOutputDelayNames(inData)
    % Rename a parameter

    outData.NewBlockPath = '';
    outData.NewInstanceData = inData.InstanceData;
    oldNames = {'delay_value','maxdelay'};
    newNames = {'delaychain_var','maxTotalDelay'};
    for i=1:numel(oldNames)
        oldName = oldNames{i};
        newName = newNames{i};
        matches = strcmp(oldName,{outData.NewInstanceData.Name});
        index = find(matches);
        if (false == isempty(index))
            outData.NewInstanceData(index).Name = newName;
        end
    end
    
    [ParameterNames{1:length(outData.NewInstanceData)}] = outData.NewInstanceData.Name;
    if (~ismember('numChannels',ParameterNames))
        % numChannels parameter is not present in old link.
        outData.NewInstanceData(end+1).Name = 'numChannels';
        outData.NewInstanceData(end).Value = 'num_input_chs';
    end
end