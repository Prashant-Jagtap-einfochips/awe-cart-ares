function mips = run_vdsp_project(model_name, model_code_gen_folder, profile_run_count)
% run_vdsp_project: runs a dxe in VDSP simulator 
%

%   Copyright 2019 Bose Corporation
[current_path, ~] = fileparts(mfilename('fullpath'));


% VDSP config
target_name = 'ADSP-214xx Family Simulator';
platform_name = 'ADSP-214xx Simulator';
processor_name = 'ADSP-21489';

% Invoke VDSP
app = actxserver('VisualDSP.ADspApplication');
app.Visible = 1;
app.Interactive = 0;
app.CurrentDirectory = current_path;

% Disconnect active sessions, if any
if (~isempty(app.ActiveSession))
    app.DisconnectSession();
end

% Derive session name
session_name = strcat([model_name '_profile_session']);

% Create a new session
app.CreateSession(session_name, ...
                  target_name,  ...
                  platform_name, ...
                  processor_name);
              
session = app.ActiveSession;              

% Load program
exe_file = fullfile(model_code_gen_folder, strcat(model_name, '.dxe'));
processor = session.ProcessorList.Item(processor_name);
processor.Reset(1);
processor.LoadProgram(exe_file);
processor.Run(1);

% app.OutputWindow.ActiveTab = 0; % 0 = console, 1 = build
text = app.OutputWindow.GetText(0);

% Parse the output (and remove newline char)
disp(strtrim(text));

% Get total mips from output (by taking the numbers after 'mips:')
if (profile_run_count == 1)
    mips_per_step_fcn = regexp(text, '(?<=mips: )(\S+)', 'match');
else
    mips_per_step_fcn = regexp(text, '(?<=max mips: )(\S+)', 'match');
end
mips = sum(str2double(mips_per_step_fcn));
fprintf('\nTotal mips: %f\n', mips);

app.ProjectList.Close;
app.DisconnectSession();

return

