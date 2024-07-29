%---------------------------------------------------------------------
%
%  COPYRIGHT BOSE CORPORATION ALL RIGHTS RESERVED
%  This program may not be reproduced, in whole or in part in any
%  form or any means whatsoever without the written permission of:
%
%     BOSE CORPORATION
%     The Mountain
%     Framingham, MA 01701-9168
%
%---------------------------------------------------------------------
%

function build_model_cmdlineOJT(prj_name, model_name, build_options, default_config, build_proj_path, done_log, test_case, test_method, test_path)
%
% prj_name -- Name of development project. Just the project name. The current folder must be prj_path before the function is called
% model_name -- Name of the model to be built. The path of the model must be setup by the project prj_name
% build_proj_path -- Folder to keep build project (a temporary project referencing development project)
% done_log -- Name of file to be created for sucessful build
% build_options -- Either a options string or a Matlab .m file with a function returning a full options object
% default_config -- In case a options string is passed in build_options, the default_config to be used by all partitions
%
% test_case -- Matlab class inheriting TalariaTestOJT class
% test_method -- Specified method of test_case class
% test_path -- absolute path for generated test vectors
%

minArgs=6;
maxArgs=9;
narginchk(minArgs,maxArgs);

build_and_test = false;
if nargin > minArgs
    build_and_test = true;
end

currentFolder = pwd;

try
    devProj = openProject(prj_name);
catch
    error('Failed to open project [%s]',prj_name);
end

buildProject = CreateBuildProject(build_proj_path, prj_name);
buildProjSVN = contains(buildProject.SourceControlIntegration, 'SVN');
buildProjSVN = false;

[done_log_path, done_log_name] = fileparts(done_log);
err_log = fullfile(done_log_path, 'error.log');

if (buildProjSVN)
    svn_info_cmd = ['jsvn info --xml ' build_proj_path];
    svn_info_str = evalc(svn_info_cmd);
    %
    % Check for SVN Error E155007 Not a working copy
    %
    if (contains(svn_info_str, 'E155007') == 0)
        svn_info_xml = [build_proj_path filesep 'svn_info.xml'];
        fid = fopen(svn_info_xml,'Wt');
        fwrite(fid,svn_info_str);
        fclose(fid);

        %
        % Basic idea is start at the working copy root
        % move up a part of the path at time
        % first time you get a path that is scheduled to be added
        % run svn remove command
        %
        svn_info = readstruct(svn_info_xml);
        cd(svn_info.entry.wc_info.wcroot_abspath);
        wc_root = pwd;
        svn_parts = strsplit(extractAfter(buildProject.RootFolder,[wc_root filesep]), filesep);
        svn_path = wc_root;
        for k=1:length(svn_parts)
            svn_path = append(svn_path, filesep, svn_parts{k});
            svn_info_cmd = ['jsvn info ' svn_path];
            svn_info_str = evalc(svn_info_cmd);
            %disp(svn_info_str)
            if contains(svn_info_str, 'Schedule: add')
                fprintf('Info: removing from SVN source control integration %s\n', svn_path);
                svn_remove_cmd = append('jsvn remove --keep-local ', svn_path);
                svn_remove_str = evalc(svn_remove_cmd);
                break;
            end
        end
    end
end

cd (buildProject.RootFolder);

try
    if exist('PartitionGenerateOptions','class') ~= 0
        pgo_file = build_options;
        if isfile(pgo_file)
            [pgo_path, pgo_name] = fileparts(pgo_file);
            addpath(pgo_path);
            partition_generate_options = eval(pgo_name);
            rmpath(pgo_path);
            if (isa(partition_generate_options,'PartitionGenerateOptions'))
                fprintf('Info: Full options objects is passed in %s\n', build_options);
            else
                error('Error: Unrecognized build_options [%s]', build_options)
            end
        else
            fprintf('Info: Assume options string - %s\n', build_options);
            partition_generate_options = PartitionGenerateOptions(build_options);

            [config_path, config_name] = fileparts(default_config);
            oldpath = path;
            if ismember(config_path, strsplit(path, pathsep))
                rmpath(config_path);
                addpath(config_path);
                partition_generate_options.CoreTagToConfigSetMap('default')= eval(config_name);
            else
                addpath(config_path);
                partition_generate_options.CoreTagToConfigSetMap('default')= eval(config_name);
                rmpath(config_path);
            end
            path(oldpath);
        end

        if build_and_test
            test_case_func = str2func(test_case);
            mkdir(test_path);
            run(test_case_func('',test_path,partition_generate_options),test_method);
        else
            if (string(talariaVersion) < "V2200BL")
                partition_and_generate(model_name, partition_generate_options);
            else
                artifactInfo = partition_and_generate(model_name, partition_generate_options);
                saveGeneratedCodePathsOJT(artifactInfo);
            end
        end

        fclose(fopen(done_log,'w'));
    else
        err_msg = 'Error: Unsupported MATLAB installation and/or Talaria Release\n';
        fid_err=fopen(err_log,'w');
        fprintf(fid_err, err_msg);
        fclose(fid_err);
        error(err_msg);
    end
catch ME
    % In case of errors in the script, save relevant information
    % If called from a script, save the error stack
    fid_err=fopen(err_log,'w');

    adj_message = strrep(ME.message,'\','\\');
    fprintf(fid_err, ['Error: ',adj_message,'\n\n']);
    fprintf(fid_err, 'Stack: \n');
    for cnt=1:size(ME.stack,1)
        fprintf(fid_err, ['File: ',strrep(ME.stack(cnt).file,'\','\\'),'\n']);
        fprintf(fid_err, ['Name: ',ME.stack(cnt).name,'\n']);
        fprintf(fid_err, ['Line: ',num2str(ME.stack(cnt).line),'\n\n']);
    end

    fclose(fid_err);

    rethrow(ME);
end

cd(currentFolder);
