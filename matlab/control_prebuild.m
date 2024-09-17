function M = control_prebuild(M, TSYS)
% ----------------------------------------------------------------------
% Prebuild
% This function will check if the module to control exists in the system
% ----------------------------------------------------------------------

% make_standardmodulepack will call this with no arguments
if nargin < 3
    return;
end

% Validate modVar by throwing an error if any issues found
% This can probably be optimized to find the module and not a module variable
modVar = [M.constructorArgument{1}.value '.coeff'];
[targetMod, varName] = verify_mod_var(TSYS, M, modVar);

return;

