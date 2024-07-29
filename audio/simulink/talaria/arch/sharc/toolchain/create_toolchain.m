% Helper script for creating/updating custom toolchain

%   Copyright 2019 Bose Corporation
% The toolchain rarely changes, so by default the .mat file containing
% the toolchain info is checked in and needs to be manually updated if the 
% toolchain info is changed. Set update_toolchain_info to 1 to update the
% .mat file when running this script
update_toolchain_info = 0;

if (update_toolchain_info == 1)
    disp('--- Registering VDSP Toolchain ---');
    cd('registry')
    info = get_vdsp_ToolChainInfo();
    tc = vdsp_tc(info.ToolChainName);
    save vdsp_tc tc;
    if (tc.validate ~= 1)
        error('Toolchain validation failed');
    end
    cd('..')
end

% Simulink Project should have 'registry' in the project path
% installToolchains() will call sl_refresh_customizations to pull
% in changes

% Set the evironment variable if necessary
default_vdsp_dir = 'C:\Program Files\Analog Devices\VisualDSP 5.0';
vdsp_dir = getenv('VDSP_DIR');
if ~exist(vdsp_dir, 'dir')
    disp('--- Setting VDSP environment variable ---');
    setenv('VDSP_DIR', default_vdsp_dir);
end

% Set the VDSP_LDF_DIR env var used by ert.tlc builds
thisPath = fileparts(mfilename('fullpath'));
setenv('VDSP_LDF_DIR', fullfile(fileparts(thisPath),'build'));