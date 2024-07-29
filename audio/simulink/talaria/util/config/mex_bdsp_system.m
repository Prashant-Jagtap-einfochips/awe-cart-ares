function mex_bdsp_system(sys, name)
%Creates xmodel from BDSP system SYS, runs BDSP code generation and 
%  compiles it using mex for use inSimulink S-function

%   Copyright 2018-2019 Bose Corporation
% Set some paths
code_gen_folder = Simulink.fileGenControl('get', 'CodeGenFolder');
bdsp_work_folder = fullfile(code_gen_folder, 'bdsp');

[current_path, ~] = fileparts(mfilename('fullpath'));
talaria_root = fullfile(current_path, '..', '..');
blink_lib_path = fullfile(talaria_root, 'lib', 'config', 'blink');

% Run code generation of BDSP system
disp('### Generating code from BDSP system');
build_bdsp_system(sys, name, bdsp_work_folder);

% mex'ify the system
disp('### Runing mex on the generated code');
build_mex(name, bdsp_work_folder);

% Build .dlb for integrating generated code
template_makefile = fullfile(blink_lib_path, 'Makefile');
bdsp_code_gen_folder = [bdsp_work_folder '\' name '_visual_DSP'];
copyfile(template_makefile, bdsp_code_gen_folder);
gmake = fullfile(matlabroot, 'bin', 'win64', 'gmake');
make_cmd = ['"' gmake '" -C ' bdsp_code_gen_folder ' LIB_NAME=' name];
[status, cmdout] = system(make_cmd);
if (status ~= 0)
    error(['Could not build profiler dxe: ' cmdout]);
end

% Add .tlc file by copying template and replacing the name at the top
disp('### Creating .tlc file for the system');

tlc_template = fullfile(blink_lib_path, 'blink.tlc');
bdsp_tlc = fullfile(code_gen_folder, [name  '.tlc']);

fid_template = fopen(tlc_template, 'r');
contents = fread(fid_template);
contents = char(contents.');
fclose(fid_template);

contents = strrep(contents, '<<__REPLACE_WITH_SYSTEM_NAME__>>', name);
fid_bdsp = fopen(bdsp_tlc, 'w');
fwrite(fid_bdsp, contents);
fclose(fid_bdsp);

end




