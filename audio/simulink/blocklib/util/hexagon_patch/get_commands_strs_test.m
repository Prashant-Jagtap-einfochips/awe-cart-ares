function tests = get_commands_strs_test
    tests = functiontests(localfunctions);
end


function test_python_ver_ok_python3_ver_ok(testCase)
    status = 0;
    cmdout = "Python 3.7.0";
    status3 = 0;
    cmdout3 = "Python 3.7.0";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root);
    assert (pycparser_install_cmd == "python -m pip install pycparser");
end


function test_python_ver_ok_python3_non_exist(testCase)
    status = 0;
    cmdout = "Python 3.7.0";
    status3 = 1;
    cmdout3 = "";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root);
    assert (pycparser_install_cmd == "python -m pip install pycparser");
end


function test_python_non_exist_python3_non_exist(testCase)
    status = 1;
    cmdout = "";
    status3 = 1;
    cmdout3 = "";
    hexagon_sdk_root = get_hexagon_sdk_root();
    verifyError(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "PYTHON:NOT:INSTALLED");
end


function test_python_non_exist_python3_ver_wrong(testCase)
    status = 1;
    cmdout = "";
    status3 = 0;
    cmdout3 = "Python 3.6.0";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = verifyWarning(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "PYTHON:WRONG_VERSION");
    assert (pycparser_install_cmd == "python3 -m pip install pycparser");
end


function test_python_ver_wrong_python3_non_exist(testCase)
    status = 0;
    cmdout = "Python 3.6.0";
    status3 = 1;
    cmdout3 = "";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = verifyWarning(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "PYTHON:WRONG_VERSION");
    assert (pycparser_install_cmd == "python -m pip install pycparser");
end


function test_python_ver_ok_python3_ver_wrong(testCase)
    status = 0;
    cmdout = "Python 3.7.0";
    status3 = 0;
    cmdout3 = "Python 3.6.0";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root);
    assert (pycparser_install_cmd == "python -m pip install pycparser");
end


function test_python_ver_wrong_python3_ver_wrong_but_python3_newer(testCase)
    status = 0;
    cmdout = "Python 3.5.0";
    status3 = 0;
    cmdout3 = "Python 3.6.0";
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, ~] = verifyWarning(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "PYTHON:WRONG_VERSION");
    assert (pycparser_install_cmd == "python3 -m pip install pycparser");
end


function test_python_ver_too_old_python3_non_exist(testCase)
    status = 0;
    cmdout = "Python 2.7.0";
    status3 = 1;
    cmdout3 = "";
    hexagon_sdk_root = get_hexagon_sdk_root();
    verifyError(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "PYTHON:WRONG_VERSION");
end


function test_non_existing_hexagon_sdk_root(testCase)
    status = 0;
    cmdout = "Python 3.7.0";
    status3 = 0;
    cmdout3 = "Python 3.7.0";
    hexagon_sdk_root = "non_existing_dir";
    verifyError(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "FOLDER:NOT:EXISTS");
end


function test_non_existing_hexagon_sdk_root_version(testCase)
    status = 0;
    cmdout = "Python 3.7.0";
    status3 = 0;
    cmdout3 = "Python 3.7.0";
    this_script_path = matlab.desktop.editor.getActiveFilename;
    parts = strsplit(this_script_path, filesep);
    hexagon_sdk_root = append(strjoin(parts(1:end-1), filesep), "/hexagon_sdk_root_version_mock");
    verifyError(testCase, @() get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root), "VERSION:NOT:EXISTS");
end