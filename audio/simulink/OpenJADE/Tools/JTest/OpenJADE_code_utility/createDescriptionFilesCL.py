#!/usr/bin/env python
"""
createDescriptionFilesCL.py contains the logic for deriving JModule and its description and adapter files
"""

import os
import sys
import json
import xml.etree.ElementTree as Et

__author__ = "cd1043538 | Desai, Chirayu Ketan"
__copyright__ = "COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED"


def read_feature_xml(path):
    """
    Reads the file at the given path
    :param path: the path to the file
    :return: the content of the file as a string
    """
    with open(path, 'r') as f:
        data = f.read()
    return data


def read_param_schema(path):
    """
    Reads the json file at the given path
    :param path: the path to the file
    :return: the content of the file as a json object
    """
    with open(path, 'r') as f:
        data = json.load(f)
    return data


def read_template_file(path):
    """
    Reads the file at the given path
    :param path: the path to the file
    :return: the content of the file as a string
    """
    return read_feature_xml(path)


def custom_replace(data, mapping):
    """
    Replace old values in data with new values
    :param data: the data in which values need to be replaced
    :param mapping: a mapping of which values need to be replaced by which
    :return: the data with replaced values
    """
    for old, new in mapping.items():
        data = data.replace(old, new)
    return data


def create_file(basedir, filename, data):
    """
    Creates a file and writes data to it
    :param filename: the name of the file
    :param basedir: the directory to create file in
    :param data: the data to be written in file
    """
    if not os.path.exists(basedir):
        os.makedirs(basedir)
    with open(os.path.join(basedir, filename), "w") as f:
        f.write(data)


def create_feature_desc(xml, params):
    """
    Parses the Feature XML to create feature description
    :param xml: the Feature XML as a string
    :param params: the Feature parameters
    :return: the Feature Description String and the Feature Name
    """
    tree = Et.fromstring(xml)
    feat_desc = ""
    features = tree.findall('root/JModule')
    feature_name = ""

    for feature in features:
        feature_name = feature.get('label')
        feat_desc = feat_desc + "extern void JModule_Adapt_" + feature_name + "_create();\n"
        feat_desc = feat_desc + "JModule JModule_" + feature_name + "_obj = {\n"
        feat_desc = feat_desc + ".id = JMODULE_" + feature_name.upper() + ",\n"
        feat_desc = feat_desc + ".processes = &JModule_" + feature_name + "_processes,\n"
        feat_desc = feat_desc + ".create = JModule_Adapt_" + feature_name + "_create,\n"
        feat_desc = feat_desc + ".param = &JModule_" + feature_name + "_Param_obj,\n"
        feat_desc = feat_desc + ".state = &JModule_" + feature_name + "_State_obj,\n"

        for param in params:
            if not param.endswith("id") and not param.endswith("processes") \
                    and not param.endswith("create") and not param.endswith("param") and not param.endswith("state") and not param.endswith("init") and not param.endswith("command") and not param.endswith("register_notify")and feature.get(param) != "":
                feat_desc = feat_desc + "." + param + " = " + feature.get(param) + ",\n"

        feat_desc = feat_desc[:-2] + "\n};\n"

    return feat_desc, feature_name


def has_input_ports(tree, process):
    """
    Checks if process has input ports
    :param tree: the XML tree for process
    :param process: the name of process
    :return: True if process has input ports
    """
    in_ports = tree.findall('root/Port_In')
    for port in in_ports:
        if process == port.get("parent_process"):
            return True
    return False


def has_output_ports(tree, process):
    """
    Checks if process has output ports
    :param tree: the XML tree for process
    :param process: the name of process
    :return: True if process has output ports
    """
    out_ports = tree.findall('root/Port_Out')
    for port in out_ports:
        if process == port.get("parent_process"):
            return True
    return False


def create_process_desc(xml, params, feature_name):
    """
    Parses the XML to create feature description
    :param xml: the XML file contents as a string
    :param params: Process parameters
    :param feature_name: the name of the Feature
    :return: the Process description as a string, and the number of processes
    """
    tree = Et.fromstring(xml)
    process_desc = ""
    processes = tree.findall('root/Process')
    num_processes = len(processes)
    process_desc = process_desc + "static Processes JModule_" + feature_name + "_processes = {\n"
    process_desc = process_desc + ".num_items = " + str(num_processes) + ",\n" + "{{\n"

    for counter, process in enumerate(processes):
        if has_input_ports(tree, process.get("label")):
            process_desc = process_desc + ".ports_in = &JModule_" + feature_name + "_" \
                           + process.get("label").lower() + "_port_in,\n"

        else:
            process_desc = process_desc + ".ports_in = NULL,\n"

        if has_output_ports(tree, process.get("label")):
            process_desc = process_desc + ".ports_out = &JModule_" + feature_name + "_" \
                           + process.get("label").lower() + "_port_out,\n"

        else:
            process_desc = process_desc + ".ports_out = NULL,\n"

        for param in params:
            if not param.endswith("id") and not param.endswith("processes") and not param.endswith("create") \
                    and process.get(param) != "":
                process_desc = process_desc + "." + param + " = " + process.get(param) + ",\n"

        process_desc = process_desc[:-2] + "\n},\n{\n"
    process_desc = process_desc[:-4] + "\n}};\n"
    return process_desc, num_processes


def create_port_in_desc(xml, params, feature_name):
    """
    Creates a description for input ports
    :param xml: the XML file contents as a string
    :param params: Port parameters
    :param feature_name: the name of the Feature
    :return: the Input Port description as a string
    """
    tree = Et.fromstring(xml)
    port_in_desc = ""
    in_ports = tree.findall('root/Port_In')
    processes = tree.findall('root/Process')

    for process_counter, process in enumerate(processes):
        local = ""
        counter = 0
        for temp, port in enumerate(in_ports):
            if process.get("label") == port.get("parent_process"):
                if counter == 0:
                    port_in_desc = port_in_desc + "static Ports JModule_" + feature_name + "_" + port.get(
                        "parent_process").lower() + "_port_in = {" + "\n"

                if counter != 0:
                    local = local + "}," + "\n" + "{" + "\n"
                counter = counter + 1
                for param in params:
                    if port.get(param) != "" and port.get(param) is not None:
                        local = local + "." + param + " = " + port.get(param) + ",\n"

        if not counter == 0:
            port_in_desc = port_in_desc + ".num_items = " + str(counter) + ",\n" + "{{\n"
            port_in_desc = port_in_desc + local
            port_in_desc = port_in_desc + "}}};" + "\n"
    return port_in_desc


def create_port_out_desc(xml, params, feature_name):
    """
    Creates a description for output ports
    :param xml: the XML file contents as a string
    :param params: Port parameters
    :param feature_name: the name of the Feature
    :return: the output Port description as a string
    """
    tree = Et.fromstring(xml)
    port_out_desc = ""
    out_ports = tree.findall('root/Port_Out')
    processes = tree.findall('root/Process')

    for process_counter, process in enumerate(processes):
        local = ""
        counter = 0
        for temp, port in enumerate(out_ports):
            if process.get("label") == port.get("parent_process"):
                if counter == 0:
                    port_out_desc = port_out_desc + "static Ports JModule_" + feature_name + "_" + port.get(
                        "parent_process").lower() + "_port_out = {" + "\n"

                if counter != 0:
                    local = local + "}," + "\n" + "{" + "\n"
                counter = counter + 1
                for param in params:
                    if port.get(param) != "" and port.get(param) is not None:
                        local = local + "." + param + " = " + port.get(param) + ",\n"

        if not counter == 0:
            port_out_desc = port_out_desc + ".num_items = " + str(counter) + ",\n" + "{{\n"
            port_out_desc = port_out_desc + local
            port_out_desc = port_out_desc + "}}};" + "\n"
    return port_out_desc


def create_adapter_processes(xml, num_processes, adapter_attr):
    """
    Creates the Functions and strings for processes to be put in adapter
    :param adapter_attr: the attributes in JModule XML that hold adapter data
    :param xml: the JModule XML
    :param num_processes: the number of processes in the Feature
    :return: Functions and strings for processes to be put in adapter
    """
    tree = Et.fromstring(xml)
    features = tree.findall('root/JModule')
    adapter_data = ""
    for feature in features:
        for attr in adapter_attr:
            if attr in feature.attrib:
                adapter_data = adapter_data + feature.get(attr) + "\n\n"

    if "".join(adapter_data.split()) == "":
        function_template = "static void JModule_Adapt_process#P_INDEX#(JModule* this, Process* process, " \
                            "Process_Context* process_ctx, uint32_t thread_level)\r\n{\r\n\tPorts " \
                            "*port_in = process->ports_in;\r\n\tPorts_Context *port_in_ctx = process_ctx->ports_in;" \
                            "\r\n\tPorts_Context *port_out_ctx = process_ctx->ports_out;\r\n\tuint32_t i;\r\n\t" \
                            "float * src = port_in_ctx->item[0].payload;\r\n\t" \
                            "float * dst = port_out_ctx->item[0].payload;\r\n\t" \
                            "for(i = 0; i<port_in->port[0].frame_size * port_in->port[0].num_chan ; i++ )" \
                            "{ // passthru example\r\n\t\tdst[i] = src[i];\r\n\t}\r\n}\r\n"
        process_exec_string = "\tthis->processes->process[#P_INDEX#].exec = JModule_Adapt_process#P_INDEX#;"

        ret_funcs = ""
        ret_strings = ""
        for num in range(num_processes):
            ret_funcs = ret_funcs + function_template.replace("#P_INDEX#", str(num)) + "\r\n"
            ret_strings = ret_strings + process_exec_string.replace("#P_INDEX#", str(num)) + "\r\n"
        return ret_funcs, ret_strings
    else:
        return "", adapter_data


def main():
    """
    Contains variables and execution flow
    """
    if len(sys.argv) < 3:
        print("Not Enough arguments")
        print("Usage:> python createDescriptionFilesCL.py <Feature XML Path> <Parameter Schema Json Path>")
        sys.exit()

    feature_xml_path = sys.argv[1]
    schema_json_path = sys.argv[2]
    output_directory = sys.argv[3] + os.path.sep
    dependency_path = sys.argv[4]
    schema_keys = ["jModuleParams", "processParams", "portParams"]
    adapter_h_template_path = os.sep.join([dependency_path, "JModule_Adapt_.h"])
    adapter_default_c_template_path = os.sep.join([dependency_path, "JModule_Adapt_Default.c"])
    adapter_c_template_path = os.sep.join([dependency_path, "JModule_Adapt_.c"])
    desc_h_template = os.sep.join([dependency_path, "JModule_Desc_.h"])
    desc_c_template = os.sep.join([dependency_path, "JModule_Desc_.c"])
    replace_mapping = {}
    adapter_attributes = ["m1", "m2", "m3", "m4", "m7", "m5", "m6"]
    feat_xml = read_feature_xml(feature_xml_path)
    schema = read_param_schema(schema_json_path)

    feat_desc_g, feat_name = create_feature_desc(feat_xml, schema[schema_keys[0]].keys())
    proc_desc, num_proc = create_process_desc(feat_xml, schema[schema_keys[1]].keys(), feat_name)

    in_port_desc = create_port_in_desc(feat_xml, schema[schema_keys[2]].keys(), feat_name)
    out_port_desc = create_port_out_desc(feat_xml, schema[schema_keys[2]].keys(), feat_name)

    adapter_functions, adapter_strings = create_adapter_processes(feat_xml, num_proc, adapter_attributes)

    replace_mapping["$$JModule$$"] = feat_name
    replace_mapping["$$JMODULE$$"] = feat_name.upper()
    replace_mapping["$$NUM_PROCESSES$$"] = str(num_proc)
    replace_mapping["$$PORTS_AND_PROCESSES$$"] = in_port_desc + "\n" + out_port_desc + "\n" + proc_desc
    replace_mapping["$$FEATURE_DESC$$"] = "\n" + feat_desc_g
    if adapter_functions == "":
        chosen_adapter_template = adapter_c_template_path
        replace_mapping["$$ADAPTER_DATA$$"] = adapter_strings
    else:
        chosen_adapter_template = adapter_default_c_template_path
        replace_mapping["$$ADAPTER_PROCESSES$$"] = adapter_functions
        replace_mapping["$$ADAPTER_STRINGS$$"] = adapter_strings

    create_file(os.path.join(output_directory, "JModule_Lib", feat_name),
                "JModule_Adapt_" + feat_name + ".h",
                custom_replace(read_template_file(adapter_h_template_path), replace_mapping))
    create_file(os.path.join(output_directory, "JModule_Lib", feat_name),
                "JModule_Adapt_" + feat_name + ".c",
                custom_replace(read_template_file(chosen_adapter_template), replace_mapping))
    create_file(os.path.join(output_directory, "JModule_Lib", feat_name),
                "JModule_Desc_" + feat_name + ".h",
                custom_replace(read_template_file(desc_h_template), replace_mapping))
    create_file(os.path.join(output_directory, "JModule_Lib", feat_name),
                "JModule_Desc_" + feat_name + ".c",
                custom_replace(read_template_file(desc_c_template), replace_mapping))


if __name__ == '__main__':
    main()
