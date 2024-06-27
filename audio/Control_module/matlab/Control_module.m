function M= Control_module(NAME)
% M = Control_module(NAME)
% Copies input pin data to the output pin.
%
% Arguments:
% NAME - name of the module
%

% AudioWeaverModule [This tag makes it appear under awe_help]

M=awe_module('Control', 'Copies input wire to output wire');
if (nargin == 0)
 return;
end

M.name=NAME;
M.defaultName='Control';

% % Add input and output pins with no data or size restrictions
PT=new_pin_type([], [], [], '*32', []);
add_pin(M, 'input', 'in', 'Input signal', PT);
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
