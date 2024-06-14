function M= TestAudio_module(NAME)
% M = TestAudio_module(NAME)
% Copies input pin data to the output pin.
%
% Arguments:
% NAME - name of the module
%

% AudioWeaverModule [This tag makes it appear under awe_help]

M=awe_module('TestAudio', 'Copies input wire to output wire');
if (nargin == 0)
 return;
end

M.name=NAME;
M.defaultName='TestAudio';

% Add input and output pins with no data or size restrictions
PT=new_pin_type([], [], [], '*32', []);
add_pin(M, 'input', 'in', 'Input signal', PT);
add_pin(M, 'output', 'out', 'Output signal', PT);

% ----------------------------------------------------------------------
% Add module variables
% ----------------------------------------------------------------------

add_variable(M, 'gain', 'int', 1, 'parameter', 'Linear gain');
M.gain.range=[0 10];
M.gain.units='linear';

add_array(M, 'coeff', 'float', [0; 0.1; 0.2; 0.3; 0.4; 0.5; 0.6; 0.7; 0.8; 0.9; 1], 'parameter', 'Coefficent Data');
M.coeff.range=[0 1];
M.coeff.units='linear';

% ----------------------------------------------------------------------
% Code generation details
% ----------------------------------------------------------------------
awe_addcodemarker(M, 'processFunction', 'Insert:\InnerTestAudio_Process.c');
awe_addcodemarker(M, 'discussion', {'Copies the input to the output. '});

% ----------------------------------------------------------------------
% Add the inspector information
% ----------------------------------------------------------------------

M.gain.guiInfo.controlType='slider';
add_control(M, '.gain');

M.coeff.guiInfo.controlType='grid';
add_control(M, '.coeff');

% ----------------------------------------------------------------------
% Module browser information
% ----------------------------------------------------------------------
M.moduleBrowser.path = 'Third Party';
M.moduleBrowser.image = '../images/TestAudio.bmp';
M.moduleBrowser.searchTags = 'pass thru through copy';
return;
