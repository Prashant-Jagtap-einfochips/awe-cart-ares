function [toolChainName,profileFunctionHandle] = register_profile_gul()
% Register profile_model support for this architecture
%   returns the toolchain name and profiling function

info = get_adsp_2156x_ToolChainInfo();
toolChainName = info.ToolChainName;
profileFunctionHandle = @profile_model_gul_in_cces;
