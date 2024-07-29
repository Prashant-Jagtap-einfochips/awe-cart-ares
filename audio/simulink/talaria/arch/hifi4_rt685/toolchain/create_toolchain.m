% Helper script for creating/updating custom toolchain

%   Copyright 2019 Bose Corporation
% The toolchain rarely changes, so by default the .mat file containing
% the toolchain info is checked in and needs to be manually updated if the 
% toolchain info is changed. Set update_toolchain_info to 1 to update the
% .mat file when running this script
update_toolchain_info = 0;

if (update_toolchain_info == 1)
    disp('--- Registering HiFi4 RT685 Toolchain ---');
    cd('registry')
    info = get_hifi4_rt685_ToolChainInfo();
    tc = hifi4_rt685_tc(info.ToolChainName);
    save hifi4_rt685_tc tc;
    if (tc.validate ~= 1)
        error('Toolchain validation failed');
    end
    cd('..')
end

% Simulink Project should have 'registry' in the project path
% installToolchains() will call sl_refresh_customizations to pull
% in changes

% Set the evironment variable if necessary
default_xtensa_dir = 'C:\usr\xtensa\XtDevTools\install\tools\RI-2020.5-win32\XtensaTools\bin';
xtensa_dir = getenv('XTENSA_DIR');
if ~exist(xtensa_dir, 'dir')
    disp('--- Setting Xtensa environment variable ---');
    setenv('XTENSA_DIR', default_xtensa_dir);
end
