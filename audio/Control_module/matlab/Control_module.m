function M= Control_module(NAME, MODULE)
% M = Control_module(NAME)
% Copies input pin data to the output pin.
%
% Arguments:
% NAME - name of the module
% MODULE - name of module to control
%

% AudioWeaverModule [This tag makes it appear under awe_help]

if (nargin < 2) || isempty(MODULE)
    MODULE = 'Mod';
end

M=awe_module('Control', 'Copies input wire to output wire');

% Adds "module" field to module properties window in Designer
add_argument(M, 'module', 'string', MODULE, 'const', 'Module to Control');

if (nargin == 0)
 return;
end

M.name=NAME;
M.defaultName='Control';

M.preBuildFunc = @control_prebuild;
M.postBuildFunc = @control_postbuild;

% This will point to the module instance we want to control
% This concept can be extednded to include multiple modules
add_pointer(M, 'modPtr', 'void *', 'parameter', 'Points to the module to set', 1);
M.modPtr.ptrExpr = ['\' MODULE];

% This will point to a specific variable in the module
% This is used here for error checking and not in the control code 
add_pointer(M, 'varPtr', 'void *', 'parameter', 'Points to the variable to set within the module instance structure', 1);
M.varPtr.ptrExpr = ['\' MODULE '.coeff']; %coeff is a known parameter

% % Add input and output pins with no data or size restrictions
%PT=new_pin_type([], [], [], '*32', []);
%add_pin(M, 'input', 'in', 'Input signal', PT);
%add_pin(M, 'output', 'out', 'Output signal', PT);

% ----------------------------------------------------------------------
% Add module variables
% ----------------------------------------------------------------------

add_array(M, 'coeff', 'float', zeros(11,1), 'parameter', 'Coefficent Data');
M.coeff.range=[0 1];
M.coeff.units='linear';

% ----------------------------------------------------------------------
% Code generation details
% ----------------------------------------------------------------------
awe_addcodemarker(M, 'processFunction', 'Insert:\InnerControl_Process.c');
awe_addcodemarker(M, 'setFunction', 'Insert:\InnerControl_Set.c');
awe_addcodemarker(M, 'getFunction', 'Insert:\InnerControl_Get.c');
awe_addcodemarker(M, 'constructorFunction', 'Insert:\InnerControl_Construct.c');
awe_addcodemarker(M, 'discussion', {'Copies the input to the output. '});
awe_addcodemarker(M, 'hFileInclude', '#include "ModTestAudio.h"');

% ----------------------------------------------------------------------
% Add the inspector information
% ----------------------------------------------------------------------

M.coeff.guiInfo.controlType='grid';
add_control(M, '.coeff');

% ----------------------------------------------------------------------
% Module browser information
% ----------------------------------------------------------------------
M.moduleBrowser.path = 'Third Party';
M.moduleBrowser.image = '../images/Control.bmp';
M.moduleBrowser.searchTags = 'pass thru through copy';
return;


% ----------------------------------------------------------------------
% Prebuild
% This function will check if the module to control exists in the system
% ----------------------------------------------------------------------

function M = control_prebuild(M, ~, TSYS)

% make_standardmodulepack will call this with no arguments
if nargin < 3
    return;
end

% Validate modVar by throwing an error if any issues found
% This can probably be optimized to find the module and not a module variable
modVar = [M.constructorArgument{1}.value '.coeff'];
[targetMod, varName] = verify_mod_var(TSYS, M, modVar);

return;

% ----------------------------------------------------------------------
% Postbuild
% Since the target module array size could change during its prebuild
% the check for array index exceeding the target variable array size is
% done in this postBuild
% ----------------------------------------------------------------------

function M = control_postbuild(M, ~, TSYS)

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