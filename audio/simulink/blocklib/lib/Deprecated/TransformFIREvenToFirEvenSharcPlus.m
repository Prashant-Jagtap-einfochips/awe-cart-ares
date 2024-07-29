function outData = TransformFIREvenToFirEvenSharcPlus(inData)
    % Switch ot use the new FirEven block with the SharcPlus variant
    outData.NewBlockPath = '';
    outData.NewInstanceData = inData.InstanceData;

    outData.NewInstanceData(end+1).Name = 'Config';
    outData.NewInstanceData(end).Value = "FirEvenConfig('SharcPlus')";
end