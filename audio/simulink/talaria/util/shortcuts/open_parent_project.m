function open_parent_project()
% open_parent_project Opens the Simulink Project in our parent folder

%   Copyright 2019 Bose Corporation
project = currentProject;
projectRoot = project.RootFolder;

openProject(fullfile(projectRoot, '..'));
