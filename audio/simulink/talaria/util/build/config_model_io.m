function [  ] = config_model_io( directive )
%CONFIG_MODEL_IO Configures the input/output of a model
%   Configuration the input and output of a model in preparation of a build

%   Copyright 2018-2019 Bose Corporation
% NOTE: Directive argument is a temporary workaround for figuring out how
% ports are to be named for framework integration.  In one case, only
% inputs are to be named and the directive should be 'input-only'

if nargin < 1
    directive = '';
end

input_port_suffix = '_sys_in';
output_port_suffix = '_sys_out';

hSystem = get_param(bdroot, 'handle');

inports = find_system(hSystem,'SearchDepth', '1', 'BlockType', 'Inport');
if (length(inports) ~= 1) %#ok<*ISMT>
    error('Currently too dumb to handle more than one inport');
end
% find the line attached to the input port
hInport = get(inports, 'PortHandles');
hPort = hInport.Outport; % *shrug*
hInputLine = get(hPort, 'Line');

% Rename and set storage class
set(hInputLine,'Name', strcat(bdroot, input_port_suffix));
set(hInputLine,'StorageClass', 'ExportedGlobal');
    
if ~strcmp(directive, 'input-only')
    outports = find_system(hSystem,'SearchDepth', '1', 'BlockType', 'Outport');
    if (length(outports) ~= 1)
        error('Currently too dumb to handle more than one outport');
    end
    % find the line attached to the output port
    hOutport = get(outports, 'PortHandles');
    hPort = hOutport.Inport; % *shrug*
    hOutputLine = get(hPort, 'Line');
    
    % Rename and set storage class
    set(hOutputLine,'Name', strcat(bdroot, output_port_suffix));
    set(hOutputLine,'StorageClass', 'ExportedGlobal');
end
end

