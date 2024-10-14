% ----------------------------------------------------------------------
% Postbuild
% Since the target module array size could change during its prebuild
% the check for array index exceeding the target variable array size is
% done in this postBuild
% ----------------------------------------------------------------------

function M = control_postbuild(M, TSYS)

% Sanity check
if nargin < 3
    return;
end

M = new_set_pointer_variables(TSYS, M);

ptrExpr = [M.constructorArgument{1}.value '.coeff'];
hierName = find_hierarchy_name(M, ptrExpr);

% Check for if varName is an array element
ind = find(hierName == '[');
if (isempty(ind))
    return;
end

arrayIndex = str2num(hierName(ind+1:end-1));
varName = hierName(1:ind-1);

try
    O = get_object(TSYS, varName);
catch err
    error('AWE:modVarErrorModNotFound', 'Unable to find variable %s in the system - %s', varName, err.message);
end

arraySize = prod(O.size);

if (arrayIndex >= arraySize)
    error('AWE:modVarInvalidArrayIndex', '%s index %d exceeds size of array %d', ptrExpr, arrayIndex, arraySize);
end

return;