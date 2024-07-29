function installToolchains()
%INSTALLTOOLCHAINS Installs all custom toolchains in the arch folder

%   Copyright 2018-2019 Bose Corporation
% Make sure to be in top level directory
thisDir = pwd;
project = currentProject;
rootDir = project.RootFolder;
cd(rootDir);

% Get a list of all architecture directories
archDirs = dir('arch');
for i = 1:length(archDirs)
    if ~strcmp(archDirs(i).name, '.') && ~strcmp(archDirs(i).name, '..')
        toolchainDir = ['arch\' archDirs(i).name '\toolchain'];
        if exist(toolchainDir, 'dir')
            cd(toolchainDir);
            create_toolchain;
            cd(rootDir);
        end
    end
end

% Go back to original directory
cd(thisDir);

% Get Simulink to pull in the new toolchains
sl_refresh_customizations;

end

