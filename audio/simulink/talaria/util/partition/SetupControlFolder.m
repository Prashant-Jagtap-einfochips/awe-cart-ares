function SetupControlFolder
%  SetupControlFolder Ensures model "Control" code generation folder exists
%  and copies several useful header files in from code generated for an
%  embedded signal processing target.
%  USAGE:
%     To create the folder only:
%     controlFolder = SetupControlFolder(anyModelPath)
%          anyModelPath can be the model name on path, or full tagged model
% 
%     To create the folder and copy generated files:
%     controlFolder = SetupControlFolder(anyModelPath,modelPaths)
%          modelPaths is a cell array of at least one generated target model
