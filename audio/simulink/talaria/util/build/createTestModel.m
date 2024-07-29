%--------------------------------------------------------------------------
% Function: createTestModel
%
% This function will create a generic C version of the target model
%
% Inputs:
%   modelName : The name of the current root level system (i.e. model)
%   tagModelPath : Path to the tagged model
%
% Outputs:
%   None
%

%   Copyright 2018-2019 Bose Corporation
%--------------------------------------------------------------------------
function testModel = createTestModel(srcModelName)

    % Load source model
    srcModelHandle = load_system(srcModelName);

    newModel = [srcModelName,'Test'];
    copyModel(srcModelName,newModel);

    % Need to keep the TLCOptions through the config set change
    tlcOptions = get_param(newModel,'TLCOptions');
    Simulink.BlockDiagram.loadActiveConfigSet(newModel, 'ConfigSetTestModel.m');
    set_param(newModel,'TLCOptions',tlcOptions);

    % Save model and exit.
    save_system(newModel);
    testModel = get_param(newModel,'FileName');
    disp(' Done creating test model!');

end

