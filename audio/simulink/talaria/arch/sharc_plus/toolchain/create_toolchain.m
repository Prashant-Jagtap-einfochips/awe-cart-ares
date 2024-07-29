% Helper script for creating/updating custom toolchain

%   Copyright 2019 Bose Corporation
% The toolchain rarely changes, so by default the .mat file containing
% the toolchain info is checked in and needs to be manually updated if the 
% toolchain info is changed. Set update_toolchain_info to 1 to update the
% .mat file when running this script
update_toolchain_info = 0;

if (update_toolchain_info == 1)
    disp('--- Registering CCES Toolchain ---');
    cd('registry')
    info = get_sc58x_ToolChainInfo();
    tc = adsp_sc58x_tc(info.ToolChainName);
    save adsp_sc58x_tc tc;
    if (tc.validate ~= 1)
        error('Toolchain validation failed');
    end
    cd('..')
end

% Simulink Project should have 'registry' in the project path
% installToolchains() will call sl_refresh_customizations to pull
% in changes

% Set the evironment variable if necessary
default_cces_dir = 'C:\Analog Devices\CrossCore Embedded Studio 2.6.0';
cces_dir = getenv('CCES_2_6');
if ~exist(cces_dir, 'dir')
    disp('--- Setting CCES 2.6.0 environment variable ---');
    setenv('CCES_2_6', default_cces_dir);
end

% Set the CCES_LDF_DIR env var used by ert.tlc builds
thisPath = fileparts(mfilename('fullpath'));
setenv('CCES_LDF_DIR', fullfile(fileparts(thisPath),'build'));

