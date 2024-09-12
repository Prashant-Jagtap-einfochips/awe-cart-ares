function M= TestAudio_module(NAME)
% M = TestAudio_module(NAME)
% Copies input pin data to the output pin.
%
% Arguments:
% NAME - name of the module
%

% AudioWeaverModule [This tag makes it appear under awe_help]
CONTROL_MAX_PAYLOAD_SIZE = 256


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

add_array(M, 'CONTROL_REQUEST', 'uint', zeros(CONTROL_MAX_PAYLOAD_SIZE,1), 'parameter', 'Control Message Request');
add_array(M, 'CONTROL_RESPONSE', 'uint', zeros(CONTROL_MAX_PAYLOAD_SIZE,1), 'parameter', 'Control Message Response');
add_variable(M, 'STATE', 'int', 1, 'parameter', 'Toggles AudioPP Active/Bypass', 1);

% Log Control
add_variable(M, 'logCTRL', 'int', 1, 'parameter', 'Debug Start/Stop', 1);

% BAF pointer
add_pointer(M, 'pHandle', 'void *', 'state', 'Instance handle', 1);
% valid CONTROL_RESPONSE
add_variable(M, 'validCtrlRsp', 'int', 0, 'state', 'valid control response');
% algorithm state
add_variable(M, 'algoState', 'int', 0, 'state', 'algorithm state active/bypass');

add_variable(M, 'gain', 'int', 1, 'parameter', 'Linear gain');
M.gain.range=[0 10];
M.gain.units='linear';

add_array(M, 'coeff', 'float', zeros(11,1), 'parameter', 'Coefficent Data');
M.coeff.range=[0 1];
M.coeff.units='linear';

% ----------------------------------------------------------------------
% Code generation details
% ----------------------------------------------------------------------
awe_addcodemarker(M, 'processFunction', 'Insert:\InnerTestAudio_Process.c');
awe_addcodemarker(M, 'setFunction', 'Insert:\InnerTestAudio_Set.c');
awe_addcodemarker(M, 'getFunction', 'Insert:\InnerTestAudio_Get.c');
awe_addcodemarker(M, 'constructorFunction', 'Insert:\InnerTestAudio_Construct.c');

awe_addcodemarker(M, 'discussion', {'Copies the input to the output. '});

awe_addcodemarker(M, 'srcFileInclude', '#include "BAF.h"');
awe_addcodemarker(M, 'srcFileInclude', '#include "BAF_Message_Id.h"');

awe_addcodemarker(M, 'srcFileInclude', '#define CONTROL_RESPONSE_INVALID 0U');
awe_addcodemarker(M, 'srcFileInclude', '#define CONTROL_RESPONSE_VALID   1U');
awe_addcodemarker(M, 'srcFileInclude', '#define AUDIOPP_STATE_BYPASS     0U');
awe_addcodemarker(M, 'srcFileInclude', '#define AUDIOPP_STATE_ACTIVE     1U');

awe_addcodemarker(M, 'hFileDefine', strcat("#define NUM_CHANNELS_IN " + "2U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define NUM_CHANNELS_OUT " + "2U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define SAMPLERATE " + "48000U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define BLOCKSIZE " + "32U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define BASE_TASKID " + "0U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define EXT_SCHEDULER " + "1U"));
awe_addcodemarker(M, 'hFileDefine', strcat("#define CONTROL_MAX_PAYLOAD_SIZE " + "256U"));
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
