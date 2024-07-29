function [mips, num_kB] = profile_model_in_cces(model_name, profile_run_count)

% Get some directory info
code_gen_folder = Simulink.fileGenControl('get', 'CodeGenFolder');
model_code_gen_folder = fullfile(code_gen_folder, [model_name '_ert_shrlib_rtw']);

%   Copyright 2019 Bose Corporation
% Build a dxe for profiling
num_bytes = build_profiler(model_name, model_code_gen_folder, profile_run_count);

% Run the dxe in the simulator
mips = run_simulator(model_name, model_code_gen_folder, profile_run_count);

% Output the number of bytes used
num_kB = round(num_bytes / 2^10);
disp(['Memory usage: ' num2str(num_kB) 'kB']);
return

function num_bytes = build_profiler(model_name, code_gen_folder, profile_run_count)
% Used to store the memory footprint without the model included
reset_baseline = 0;

% Copy over makefile and profiler src
src_file = fullfile(fileparts(mfilename('fullpath')), 'test.c');
ldf_file = fullfile(getenv('CCES_LDF_DIR'), 'test.ldf');
make_file = fullfile(fileparts(mfilename('fullpath')), 'Makefile');

copyfile(src_file, code_gen_folder);
copyfile(ldf_file, code_gen_folder);
copyfile(make_file, code_gen_folder);

% Create profile config header file
outputAggregatedFileName = ['ProfileConfig.h'];
defprotectName = upper(strrep(outputAggregatedFileName,'.','_'));
bannerLines = {
'/*'
' * BOSE CORPORATION'
' * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED'
' * This program may not be reproduced, in whole or in part in any'
' * form or any means whatsoever without the written permission of:'
' *    BOSE CORPORATION'
' *    The Mountain'
' *    Framingham, MA 01701-9168'
' *'
[' * File: ' outputAggregatedFileName]
' *'
' * Combined RTC message ID and payload definitions from all cores'
' *'
' */'
''
['#ifndef ' defprotectName '_']
['#define ' defprotectName '_']
''
''};
bannerStr = strjoin(bannerLines,newline);
outputAggregatedFilePath = fullfile(code_gen_folder,outputAggregatedFileName);
fid = fopen(outputAggregatedFilePath,'wt');
fprintf(fid, bannerStr);
fprintf(fid,'#define  MODEL_PROFILE_RUN_COUNT  %u\n', profile_run_count);
fprintf(fid,'\n#endif\n\n');
fclose(fid);

% Run gmake from system command (cd to directory?)
gmake = fullfile(matlabroot, 'bin', 'win64', 'gmake');
make_cmd = ['"' gmake '" -C ' code_gen_folder ' MODEL_NAME=' model_name];
if reset_baseline
    make_cmd = [make_cmd ' EXTRA_CFLAGS=-DCOMPILE_WITHOUT_MODEL'];
end
[status, cmdout] = system(make_cmd);
if (status ~= 0)
    error(['Could not build profiler dxe: ' cmdout]);
end

% Get the memory breakdown
xml_file = fullfile(code_gen_folder, [model_name '.xml']);
baseline = fullfile(fileparts(mfilename('fullpath')), 'profiling_memory_baseline.mat');
[~, num_bytes] = memory_breakdown(xml_file, baseline, reset_baseline);
return

function mips = run_simulator(model_name, code_gen_folder, profile_run_count)
% Set some variables associated with the call to the simulator
cces_dir = getenv('CCES_2_6');
sim = ['"' cces_dir '\CCES_Runner.exe"'];

processor = 'ADSP-SC584';
target = '"ChipFactory Simulator"';
platform = '"ADSP-SC584 Cycle-Accurate-Sim"';
profiler_exe = fullfile(code_gen_folder, [model_name '.dxe']);

sim_cmd = [sim ' --processor ' processor ' --target ' target];
sim_cmd = [sim_cmd ' --platform ' platform ' --core "0,' profiler_exe '"'];
[status, cmdout] = system(sim_cmd);
if (status ~= 0)
    error(['Could not run profiler: ' cmdout 'Call command: ' sim_cmd]);
end

% Parse the output (and remove newline char)
disp(strtrim(cmdout));

% Get total mips from output (by taking the numbers after 'mips:')
if (profile_run_count == 1)
    mips_per_step_fcn = regexp(cmdout, '(?<=mips: )(\S+)', 'match');
else
    mips_per_step_fcn = regexp(cmdout, '(?<=max mips: )(\S+)', 'match');
end
mips = sum(str2double(mips_per_step_fcn));
fprintf('\nTotal mips: %f\n', mips);
return
