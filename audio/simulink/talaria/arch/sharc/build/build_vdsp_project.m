function [] = build_vdsp_project(model_name)
% build_vdsp_project: builds the generated code in VDSP simulator 
%

%   Copyright 2019 Bose Corporation
project = currentProject;
projectRoot = project.RootFolder;
[current_path, ~] = fileparts(mfilename('fullpath'));

clean_up_afterwards = 0;
run_manually = 0;

% Specify source file(s)
main_src = strcat('..\src\test.c');
source_files = {main_src};

% VDSP config
target_name = 'ADSP-214xx Family Simulator';
platform_name = 'ADSP-214xx Simulator';
processor_name = 'ADSP-21489';
build_cfg = 'Debug';

% Project template file
project_template = fullfile(current_path, 'template.dpj');

% Invoke VDSP
app = actxserver('VisualDSP.ADspApplication');

% app properties
app.Visible = 1;
app.Interactive = 0;
app.CurrentDirectory = current_path;

% Disconnect active sessions, if any
if (~isempty(app.ActiveSession))
    app.DisconnectSession();
end

% Derive session name
session_name = strcat([model_name '_profile_session']);

% Project file (file location is relative to MATLAB pwd)
project_file = strcat(current_path, '\', model_name, '.dpj');

% Create a new session
app.CreateSession(session_name, ...
                  target_name,  ...
                  platform_name, ...
                  processor_name);
              
session = app.ActiveSession;              

if (isempty(session))
    error('could not create a new session');
end

% Add project from template
copyfile(project_template, project_file);

% Project file (file location relative to VDSP object's CurrentDirectory
project_file = strcat(model_name, '.dpj');
app.ProjectList.Close;
project_list = app.ProjectList.AddProject(project_file);

if(isempty(project_list))
    error('Could not open project %s', project_file);
end

% Get active project
active_project = app.ProjectList.ActiveProject;
active_project.Name = model_name;

% Add source files
for i=1:numel(source_files)
    active_project.AddFile(source_files{i}, '');
end

% Add code gen directory to include path
code_gen_path = Simulink.fileGenControl('get', 'CodeGenFolder');
include_switch = ['-I' code_gen_path '\' model_name '_ert_shrlib_rtw'];
compiler = active_project.ToolList.Item('Compiler');
compiler.AddSwitch('Debug', include_switch,'');

% Add header file location to include path
% include_switch = '-I../../lib/component';
% compiler = active_project.ToolList.Item('Compiler');
% compiler.AddSwitch('Debug', include_switch,'');

% Add simulink model library
lib_name = [model_name '_win64.dlb'];
lib = fullfile(code_gen_path, lib_name);
active_project.AddFile(lib,'');

if run_manually
    % Running manually, useful for debugging
    disp(['Breaking out of ' mfilename ' to run VDSP manually']);
    keyboard
end

% Build project
try 
    active_project.UpdateDependencies(1);
    active_project.BuildAll(1);
catch exp
    build_window_text = app.OutputWindow.GetText(1);
    error(build_window_text);
    throw(exp);
end

% Load program
exe_file = fullfile(build_cfg, strcat(model_name, '.dxe'));
processor = session.ProcessorList.Item(processor_name);
processor.Reset(1);
processor.LoadProgram(exe_file);
processor.Run(1);

% app.OutputWindow.ActiveTab = 0; % 0 = console, 1 = build
text = app.OutputWindow.GetText(0);
disp(text);

app.ProjectList.Close;
app.DisconnectSession();

% Clean up auto-generated files
if clean_up_afterwards
    vdsp_dpj = strcat(project_path, '\', project_name, '.dpj');
    vdsp_mak = strcat(project_path, '\', project_name, '.mak');
    vdsp_pcf = strcat(project_path, '\', project_name, '.pcf');
    delete(vdsp_dpj, vdsp_mak, vdsp_pcf);
    rmdir(strcat(project_path, '\Debug'), 's');
end

return

