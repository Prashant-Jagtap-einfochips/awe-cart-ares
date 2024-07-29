function outData = TransformPooliirAccelToPoolIirTunableSharcPlusAccelGul(inData)
    % Switch to use the new PoolIirTunable block with the SharcPlusAccelGul variant
    outData.NewBlockPath = '';
    outData.NewInstanceData = [];
    
    % Get linked block instance data
    instanceData = inData.InstanceData;
    
    % Iterate over the list of instance data of and copy over the param
    % values being passed to linked block
    UnidentifiedParams = [];
    for i = 1:length(instanceData)
        switch(instanceData(i).Name)
            case 'variantSelect'
                Variant = instanceData(i).Value;
            case 'maxNumStages'
                MaxNumStages = instanceData(i).Value;
            case 'numStages'
                NumStages = instanceData(i).Value;
            case 'sosCoefficients'
                SosCoefficients = instanceData(i).Value;
            case 'frameSize'
                FrameSize = instanceData(i).Value;
            case 'numChannels'
                NumChannels = instanceData(i).Value;
            case 'FeatureName'
                FeatureName = instanceData(i).Value;
            otherwise
                UnidentifiedParams = [UnidentifiedParams instanceData(i)];
        end
    end

    % Return new instance data
    outData.NewInstanceData.Name = 'FeatureName';
    outData.NewInstanceData.Value = FeatureName;

    outData.NewInstanceData(end+1).Name = 'Config';
    outData.NewInstanceData(end).Value = sprintf("ConvertLegacyPoolIirAccelToVariant(""%s"", %s, %s, %s, %s, %s)", Variant, FrameSize, NumChannels, MaxNumStages, NumStages, SosCoefficients);

end