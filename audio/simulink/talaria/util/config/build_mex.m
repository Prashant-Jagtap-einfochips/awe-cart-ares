function build_mex(name, bdsp_work_folder)
%

%   Copyright 2018-2019 Bose Corporation
bdsp_code_gen_folder = [bdsp_work_folder '\' name];

% Set the general options
options = {...
%     '-v', ...     % uncomment for verbose output (good for debugging)
    '-outdir work', ...
    ['-output ' name], ...
    '-compatibleArrayDims'};
opts = strjoin(options);

%   Copyright 2018 Bose Corporation
% Set the include paths
include_paths = {...
    bdsp_code_gen_folder, ...
    [bdsp_code_gen_folder '\bdspinclude'], ...
    'bdsp\src\generic', ...
    'bdsp\inc'};
include_path_opts = strjoin(strcat('-I', include_paths));

% Set the defines
defines = {...
    ['S_FUNCTION_NAME=' name]};
define_opts = strjoin(strcat('-D', defines));

% Define the files to include
core_bdsp_src_files = {...
    'bdsp\src\generic\xmodel.c', ...
    'bdsp\src\xmodels\xfile\xfileDriver.c', ...
    'bdsp\src\generic\bdsp_config_record.c', ...
    'bdsp\src\generic\blkutil.c'};

bdsp_codegen_files = {...
    [bdsp_code_gen_folder '\xmodelgendef.c']};

bdsp_process_files = get_src_file_list(bdsp_code_gen_folder);

all_files = ['lib\config\blink\blink.c ' ...
    core_bdsp_src_files bdsp_codegen_files bdsp_process_files];
src_file_opts = strjoin(all_files);

% Compile the whole command and run it
mex_cmd = ['mex ' opts ' ' include_path_opts ' ' define_opts ' ' src_file_opts];
eval(mex_cmd);

end

function src = get_src_file_list(code_gen_folder)
src_list_file = [code_gen_folder '\auto_gen_C_src_list.txt'];

fid = fopen(src_list_file);
contents = textscan(fid, '%s');
fclose(fid);

src = strcat('bdsp\src\', contents{1}');
end

