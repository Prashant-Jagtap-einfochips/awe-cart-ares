function M= Control_module(NAME, MODULE)
% M = Control_module(NAME)
% Copies input pin data to the output pin.
%
% Arguments:
% NAME - name of the module
% MODULE - name of module to control
%

% AudioWeaverModule [This tag makes it appear under awe_help]
CONTROL_MAX_PAYLOAD_SIZE = 256

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

% Set to interpreted, this prevents breakpoints but allows usage with Standard license
M.isInterpreted = 1

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

add_array(M, 'CONTROL_REQUEST', 'uint', zeros(CONTROL_MAX_PAYLOAD_SIZE,1), 'parameter', 'Control Message Request');
add_array(M, 'CONTROL_RESPONSE', 'uint', zeros(CONTROL_MAX_PAYLOAD_SIZE,1), 'parameter', 'Control Message Response');

% ----------------------------------------------------------------------
% Code generation details
% ----------------------------------------------------------------------
awe_addcodemarker(M, 'processFunction', 'Insert:\InnerControl_Process.c');
awe_addcodemarker(M, 'setFunction', 'Insert:\InnerControl_Set.c');
awe_addcodemarker(M, 'getFunction', 'Insert:\InnerControl_Get.c');
awe_addcodemarker(M, 'constructorFunction', 'Insert:\InnerControl_Construct.c');
awe_addcodemarker(M, 'discussion', {'Copies the input to the output. '});
awe_addcodemarker(M, 'hFileInclude', '#include "ModTestAudio.h"');

awe_addcodemarker(M, 'hFileDefine', strcat("#define CONTROL_MAX_PAYLOAD_SIZE " + "256U"));


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

