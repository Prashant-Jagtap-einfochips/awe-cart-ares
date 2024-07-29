function outData = TransformAnnDelayNames(inData)
    % Rename a parameter
    outData.NewBlockPath = '';
    outData.NewInstanceData = inData.InstanceData;
    oldNames = {'maxdelay','num_input_chs_var'};
    newNames = {'maxTotalDelay','numChannels'};
    for i=1:numel(oldNames)
        oldName = oldNames{i};
        newName = newNames{i};
        matches = strcmp(oldName,{outData.NewInstanceData.Name});
        index = find(matches);
        if (false == isempty(index))
            outData.NewInstanceData(index).Name = newName;
        end
    end
end