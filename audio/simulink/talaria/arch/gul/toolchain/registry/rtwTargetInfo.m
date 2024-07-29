function rtwTargetInfo(tr)
%RTWTARGETINFO Registration file for custom toolchains.

%   Copyright 2019 Bose Corporation
% Copyright 2012-2013 The MathWorks, Inc.

tr.registerTargetInfo(@loc_createToolchain);

end

% -------------------------------------------------------------------------
% Create the ToolchainInfoRegistry entries
% -------------------------------------------------------------------------
function config = loc_createToolchain

info = get_adsp_2156x_ToolChainInfo();

config(1)                       = coder.make.ToolchainInfoRegistry;
config(1).Name                  = info.ToolChainName;
config(1).FileName              = fullfile(fileparts(mfilename('fullpath')), 'adsp_2156x_tc.mat');
config(1).TargetHWDeviceType    = {'*'};
config(1).Platform              =  {computer('arch')};

end