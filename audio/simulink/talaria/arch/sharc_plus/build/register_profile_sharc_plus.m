function [toolChainName,profileFunctionHandle] = register_profile_sharc_plus()
% Register profile_model support for this architecture
%   returns the toolchain name and profiling function

info = get_sc58x_ToolChainInfo();
toolChainName = info.ToolChainName;
profileFunctionHandle = @profile_model_in_cces;
