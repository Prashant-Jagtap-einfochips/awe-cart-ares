function outData = TransformAnnRouterNames(inData)
    % Rename a parameter
    outData.NewBlockPath = '';
    outData.NewInstanceData = inData.InstanceData;
    outData.NewInstanceData(3:end+1) = outData.NewInstanceData(2:end);
    outData.NewInstanceData(3).Name = 'nOutRouterMask';
    oldNames = {'nIn','output_Index'};
    newNames = {'nInRouterMask','outputMapDefaultRouterMask'};
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