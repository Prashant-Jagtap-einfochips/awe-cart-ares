function set_up_project()
%set_up_project  Configure the environment for this project
%
%   Set up the environment for the current project. This function is set to
%   Run at Startup.

%   Copyright 2019 Bose Corporation

installToolchains();
registerTargetProcessors();

%TODO: remove this dead code
cces_2_6 = getenv('CCES_2_6');
if ~exist(cces_2_6, 'dir')
    cces_dir = getenv('CCES_DIR');
    if exist(cces_dir, 'dir')
        warning('Environment variable CCES_DIR will be obsolete. Please use CCES_2_6, CCES_2_9 and CCES_2_10 instead.');
        setenv('CCES_2_6', getenv('CCES_DIR'));
    end
end

% Touch timestamps for all *.p files to avoid warnings (*.p newer than *.m)
TouchPfiles();

end
