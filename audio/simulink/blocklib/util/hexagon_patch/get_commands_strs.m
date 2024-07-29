function [pycparser_install_cmd, alignment_script_call_cmd] = get_commands_strs(model_name, status, cmdout, status3, cmdout3, hexagon_sdk_root)
    % find a version for Hexagon SDK
    if ~isfolder(hexagon_sdk_root)
        error("FOLDER:NOT:EXISTS", "Hexagon_SDK folder does not exist: %s. Float types alignment will not be executed.\n", hexagon_sdk_root);
    end
    versions = dir(fullfile(hexagon_sdk_root));
    versions = regexpi({versions.name}, "[0-9]\.[0-9]\.[0-9]\.[0-9]", "match");
    versions = versions(~cellfun('isempty',versions));
    if isempty(versions)
        error("VERSION:NOT:EXISTS", "There is no any version for Hexagon_SDK installed in: %s.\n", hexagon_sdk_root);
    end
    version = versions{1};
    hexagon_sdk_root = append(hexagon_sdk_root, version);
    
    % find proper python call - 'python' or 'python3' - and check is Python
    % properly instaled
    python_call = find_python_call(status, cmdout, status3, cmdout3);
    
    % install pycparser
    pycparser_install_cmd = append(python_call, "-m pip install pycparser");
    
    % run float types alignment script written in Python
    this_script_path = mfilename('fullpath');
    parts = strsplit(this_script_path, filesep);
    python_script_path = append(strjoin(parts(1:end-1), filesep), "/float_types_alignment.py");
    alignment_script_call_cmd = append(python_call, python_script_path, " -model_name ", model_name, " -hexagon_sdk_root ", hexagon_sdk_root);
end


function [python_ver, python_ver_num, python_major_ver, python_minor_ver] = extract_python_version(cmdout)
    python_split = strsplit(cmdout);
    python_ver = python_split{2};
    python_ver_split = strsplit(python_ver, ".");
    python_ver_num = str2double(append(python_ver_split{1}, python_ver_split{2}, python_ver_split{3}));
    python_major_ver = str2double(python_ver_split{1});
    python_minor_ver = str2double(python_ver_split{2});
end


function python_version_too_old(python_ver)
    python_ver_split = strsplit(python_ver, ".");
    python_major_ver = str2double(python_ver_split{1});
    if python_major_ver > 2
        msg = append("Your Python version ", python_ver, " may be too old! Please install Python >=3.7.0 to have supported version for float types alignment.\n");
        warning("PYTHON:WRONG_VERSION", msg);
    else
        msg = append("Your Python version ", python_ver, " is too old! Please install Python >=3.7.0 to have supported version for float types alignment.\n");
        error("PYTHON:WRONG_VERSION", msg);
    end
end


function python_call = find_python_call(status, cmdout, status3, cmdout3)
    python_call_options = {{status, cmdout, "python "}, {status3, cmdout3, "python3 "}};
    python_vers = {{"0", 0}, {"0", 0}};

    for i = 1:length(python_call_options)
        curr_status = python_call_options{i}{1};
        curr_cmdout = python_call_options{i}{2};
        curr_python_call = python_call_options{i}{3};

        if ~curr_status
            [python_ver, python_ver_num, python_major_ver, python_minor_ver] = extract_python_version(curr_cmdout);
            if python_major_ver < 3 || python_major_ver == 3 && python_minor_ver < 7
                python_vers{i}{1} = python_ver;
                python_vers{i}{2} = python_ver_num;
            else
                python_call = curr_python_call;
                return;
            end
        end
    end

    python_ver_num = python_vers{1}{2};
    python3_ver_num = python_vers{2}{2};
    if python_ver_num + python3_ver_num == 0
        error("PYTHON:NOT:INSTALLED", "Python seems to not be installed! You need Python >=3.7 for float types alignment to be done.\n")
    elseif python_ver_num >= python3_ver_num
        python_version_too_old(python_vers{1}{1});
        python_call = python_call_options{1}{3};
    else
        python_version_too_old(python_vers{2}{1});
        python_call = python_call_options{2}{3};
    end
end