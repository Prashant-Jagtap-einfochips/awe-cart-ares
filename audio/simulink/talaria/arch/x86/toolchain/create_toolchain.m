% Helper script for creating/updating custom toolchain

%   Copyright 2019 Bose Corporation
% The toolchain rarely changes, so by default the .mat file containing
% the toolchain info is checked in and needs to be manually updated if the 
% toolchain info is changed. Set update_toolchain_info to 1 to update the
% .mat file when running this script
update_toolchain_info = 0;

if (update_toolchain_info == 1)
    disp('--- Registering mingw toolchain ---');
    cd('registry')
    info = get_mingw_ToolChainInfo();
    tc = mingw_tc(info.ToolChainName);
    save mingw_tc tc;
    if (tc.validate ~= 1)
        error('Toolchain validation failed');
    end
    cd('..')
end
