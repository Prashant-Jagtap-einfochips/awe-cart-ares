function partition_generate_and_align(model_name, pg_parameters)
    partition_and_generate(model_name, pg_parameters);

    [status, cmdout] = system("python --version");
    [status3, cmdout3] = system("python3 --version");
    hexagon_sdk_root = get_hexagon_sdk_root();
    [pycparser_install_cmd, alignment_script_call_cmd] = get_commands_strs('Model', status, cmdout, status3, cmdout3, hexagon_sdk_root);
    pycparser_status = system(pycparser_install_cmd);
    alignment_status = system(alignment_script_call_cmd);
    if ~ (pycparser_status || alignment_status)
        disp("Float types alignment done!")
    end
end