function [toolChainName,profileFunctionHandle] = register_profile_sharc_plus_glxp()
% Register profile_model support for this architecture
%   returns the toolchain name and profiling function

info = get_sc59x_ToolChainInfo();
toolChainName = info.ToolChainName;
profileFunctionHandle = @profile_model_glxp_in_cces;
