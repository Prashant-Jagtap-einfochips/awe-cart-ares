#!/usr/bin/env python
"""
createAudioGraphFiles.py contains logic for generation of files pertaining to various components of an audio graph.
"""

import os
import sys
import json
from components import Feature
from components import Process
from components import Port
from components import Edge
from components import Buffer
import xml.etree.ElementTree as Et
from collections import defaultdict

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
        os.mkdir(basedir)
    with open(basedir + os.path.sep + filename, "w") as f:
        f.write(data)


def merge_two_dicts(first, second):
    """
    Merges two dictionaries
    :param first: the first dict
    :param second: the second dict
    :return: the merged dict
    """
    z = first.copy()   # start with x's keys and values
    z.update(second)    # modifies z with y's keys and values & returns None
    return z


def get_features(xml, params):
    """
    Parses XML to warp relevant attributes to Features
    :param xml: the audio graph XML
    :param params: the parameters to expect in the XML for each feature
    :return: a list of features in XML and a dictionary mapping feature Ids to features
    """
    feature_list = []
    feature_id_dict = {}
    tree = Et.fromstring(xml)
    feature_nodes = tree.findall('root/JModule')
    for feature_node in feature_nodes:
        feature = Feature()
        param_values = {}
        for param in params:
            param_values[param] = feature_node.get(param)
        feature.params = param_values
        feature.id = feature_node.get("id")
        feature.label = feature_node.get("label")
        feature_list.append(feature)
        feature_id_dict[feature.id] = feature
    return feature_list, feature_id_dict


def get_processes(xml, params):
    """
    Parses XML to wrap relevant attributes to Processes
    :param xml: the audio graph XML
    :param params: the parameters to expect in the XML for each process
    :return: a list of processes in XML and a dictionary mapping process Ids to processes,
    dictionaries mapping processes to the input and output ports in them and a set of different call intervals
    """
    process_list = []
    process_id_dict = {}
    process_out_port_dict = {}
    process_in_port_dict = {}
    different_call_intervals = set()
    tree = Et.fromstring(xml)
    process_nodes = tree.findall('root/Process')
    for process_node in process_nodes:
        process = Process()
        param_values = {}
        for param in params:
            param_values[param] = process_node.get(param)
        process.params = param_values
        process.id = process_node.get("id")
        process.label = process_node.get("label2")
        child_node = process_node.findall("mxCell")
        process.parent = child_node[0].get("parent")
        process_list.append(process)
        process_id_dict[process.id] = process
        process_in_port_dict[process] = list()
        process_out_port_dict[process] = list()
        different_call_intervals.add(int(param_values["call_interval"]))
    return process_list, process_id_dict, process_in_port_dict, process_out_port_dict, different_call_intervals


def get_ports(xml, params, process_id_dict, process_in_port_dict, process_out_port_dict):
    """
    Parses XML to wrap relevant attributes to Ports
    :param xml: the audio graph XML
    :param params:  the parameters to expect in the XML for each port
    :param process_id_dict: a dictionary mapping processes to their id
    :param process_in_port_dict: a dictionary mapping input ports to processes
    :param process_out_port_dict: a dictionary mapping output ports to processes
    :return: a list of input ports, a dictionary mapping input port ids with input ports,
    a dictionary mapping processes with list of input ports, a list of output ports,
    a dictionary mapping output port ids with output ports, dictionary mapping processes with list of output ports
    """
    tree = Et.fromstring(xml)
    in_ports = tree.findall('root/Port_In')
    out_ports = tree.findall('root/Port_Out')
    return \
        unpack_port_nodes(in_ports, params, process_id_dict, process_in_port_dict), \
        unpack_port_nodes(out_ports, params, process_id_dict, process_out_port_dict)


def unpack_port_nodes(ports, params, process_id_dict, process_port_dict):
    """
    Wraps appropriate attributes in ports nodes from XML to Ports
    :param ports: the port nodes from audio graph XML
    :param params: the parameters to expect in the XML for each port
    :param process_id_dict: a dictionary mapping process ids with processes
    :param process_port_dict: a dictionary mapping processes with ports
    :return: a list of ports, a dictionary mapping port ids with ports,
    a dictionary mapping processes with list of ports
    """
    port_list = []
    port_id_dict = {}
    for port_node in ports:
        port = Port()
        param_values = {}
        for param in params:
            param_values[param] = port_node.get(param)
        port.params = param_values
        port.label = port_node.get("label")
        port.id = port_node.get("id")
        port.parent_process = port_node.get("parent_process")
        child_nodes = port_node.findall("mxCell")
        for child_node in child_nodes:
            port.parent = child_node.get("parent")
        port_list.append(port)
        process = process_id_dict[port.parent]
        l1 = process_port_dict[process]
        l1.append(port)
        process_port_dict[process] = l1
        port_id_dict[port.id] = port
    return port_list, port_id_dict, process_port_dict


def get_edges(xml, port_id_dict):
    """
    Parses XML to wrap relevant attributes to Edges
    :param xml: the audio graph XML
    :param port_id_dict: a dictionary mapping port ids with ports
    :return: a list of edges, a dictionary mapping ports with source edges,
    a dictionary mapping ports with target edges
    """
    edge_list = []
    tree = Et.fromstring(xml)
    edges = tree.findall('root/mxCell')
    edges = edges[2:]
    source_edge_dict = defaultdict(list)
    target_edge_dict = defaultdict(list)
    for edge_node in edges:
        edge = Edge()
        edge.source = edge_node.get('source')
        edge.target = edge_node.get('target')
        edge_list.append(edge)
        source_edge_dict[port_id_dict[edge.source]].append(edge)
        source_edge_dict[port_id_dict[edge.target]].append(edge)
    return edge_list, source_edge_dict, target_edge_dict


def create_feature_list_string(feature_list):
    """
    Generates a string representation of JModule list file
    :param feature_list: a list of Features
    :return: a string representation of JModule list file
    """
    s = "/*******************************************************************************\r\n" \
        + "*  BOSE CORPORATION\r\n" + "*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED\r\n" \
        + "*  This program may not be reproduced, in whole or in part in any\r\n" \
        + "*  form or any means whatsoever without the written permission of:\r\n" \
        + "*     BOSE CORPORATION\r\n" + "*     The Mountain\r\n" \
        + "*     Framingham, MA 01701-9168\r\n" \
        + "*******************************************************************************/\r\n" \
        + "/****************************************************\r\n" + " * OpenJADE" \
        + " ****************************************************/\r\n" + "\r\n" \
        + "#include \"JModule_List.h\"\r\n" + "\r\n" \
        + "JModule* JModule_List_get_feature(JModule_List* feature_list, JModule_Id feature_id)\r\n" \
        + "{\r\n" + "	uint32_t i;\r\n" + "	\r\n" \
        + "	for ( i = 0; i < feature_list->num_items; i++ ) {\r\n" \
        + "	    if ( feature_list->item[i]->id == feature_id ) {\r\n" \
        + "	        return feature_list->item[i];\r\n" + "	    }\r\n" + "	}\r\n" \
        + "	return NULL; // not found\r\n" + "} \r\n" + "\r\n"
    feature_set = set(feature_list)
    for feature in feature_set:
        s = s + "extern JModule JModule_" + feature.label.split(",")[0] + "_obj;\r\n"
    s = s + "JModule_List JModule_List_obj = {\r\n"
    s = s + ".num_items = " + str(len(feature_set)) + "," + "\r\n"
    s = s + ".item = {" + "\r\n"
    for feature in feature_set:
        s = s + "&JModule_" + feature.label.split(",")[0] + "_obj," + "\r\n"
    s = s[:-3]
    s = s + "}};\r\n"
    return s


def create_feature_id_string(feature_list):
    """
    Generates a string representation of JModule id file
    :param feature_list: a list of Features
    :return: a string representation of JModule id file
    """
    s = "/*******************************************************************************\r\n" \
        + "*  BOSE CORPORATION\r\n" + "*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED\r\n" \
        + "*  This program may not be reproduced, in whole or in part in any\r\n" \
        + "*  form or any means whatsoever without the written permission of:\r\n" \
        + "*     BOSE CORPORATION\r\n" + "*     The Mountain\r\n" \
        + "*     Framingham, MA 01701-9168\r\n" \
        + "*******************************************************************************/\r\n" \
        + "/****************************************************\r\n" + " * \r\n" + " * OpenJADE\r\n" \
        + " *                      \r\n" + " ****************************************************/\r\n" \
        + "\r\n" + "#if !defined(BC_JModule_Id__INCLUDED_)\r\n" + "#define BC_JModule_Id__INCLUDED_\r\n" \
        + "\r\n" + "\r\n" + "#ifdef __cplusplus\r\n" + "extern \"C\" {\r\n" + "#endif\r\n" + "\r\n" \
        + "\r\n" + "/**\r\n" + " * @ingroup JModule_Id\r\n" + " *\r\n" + " * \r\n" \
        + " * @version 1.0\r\n" + " */\r\n" + "typedef enum\r\n" + "{\r\n"
    feature_set = set(feature_list)
    for feature in feature_set:
        s = s + "JMODULE_" + feature.label.split(",")[0].upper() + ",\r\n"
    s = s + "JMODULE_NUM_JMODULE } JModule_Id;" + "\r\n" + "#ifdef __cplusplus\r\n" + "}\r\n" \
        + "#endif\r\n" + "\r\n" + "\r\n" + "#endif /*!defined(BC_JModule_Id__INCLUDED_)*/\r\n" + ""
    return s


def create_audio_graph(different_call_intervals):
    """
    Generates a String representation of audio graph file
    :param different_call_intervals: a set of different call intervals
    :return: a String representation of audio graph file
    """
    s = "#include \"Audio_Graph.h\"" + "\r\n" + "extern Module_List Module_List_obj;" \
        + "\r\n" + "extern Connection_List Connection_List_obj;" + "\r\n" \
        + "extern Port_List Port_List_obj;" + "\r\n"
    for i in range(len(different_call_intervals)):
        s = s + "extern Execution_List Execution_List_obj" + str(i) + ";" + "\r\n"
    s = s + "Audio_Graph Audio_Graph_obj = {" + "\r\n" + ".modules = &Module_List_obj," \
        + "\r\n" + ".connections = &Connection_List_obj," + "\r\n" \
        + ".ports = &Port_List_obj," + "\r\n" + ".num_exec = " + str(len(different_call_intervals)) \
        + "," + "\r\n" + ".execution = {\r\n"
    for i in range(len(different_call_intervals)):
        s = s + "&Execution_List_obj" + str(i) + "," + "\r\n"
    s = s[:-3]
    s = s + "}};\r\n"
    return s


def create_module_list_string(feature_list):
    """
    Generates a string representation of module list file
    :param feature_list: a list of Features
    :return: a string representation of module list file
    """
    s = "#include \"Module_List.h\"" + "\r\n" \
        + "#include \"JModule_Id.h\"" + "\r\n" + "Module_List Module_List_obj = {" \
        + "\r\n" + ".num_items = " + str(len(feature_list)) + "," + "\r\n" + "{\r\n"
    for feature in feature_list:
        s = s + "JModule_Bitfield_module(JMODULE_" + feature.label.split(",")[0].upper() + ", " \
            + "CONTEXT_" + feature.label.split(",")[1] + ")," + "\r\n"
    s = s[:-3]
    s = s + "}};\r\n"
    return s


def create_port_list_string(port_id_dict, process_in_port_dict, process_out_port_dict, feature_id_dict):
    """
    Generates a string representation of port list file
    :param port_id_dict: a dictionary mapping port ids with ports
    :param process_in_port_dict: a dictionary mapping processes with lists of input ports
    :param process_out_port_dict: a dictionary mapping processes with lists of output ports
    :param feature_id_dict: a dictionary mapping feature ids with features
    :return: a string representation of port list file
    """
    s = "#include \"Port_List.h\"" + "\r\n" + \
        "#include \"JModule_Id.h\"" + "\r\n" + "Port_List Port_List_obj = {" \
        + "\r\n" + ".num_items = " + str(len(port_id_dict)) + "," + "\r\n" + ".item = {\r\n"
    for key, value in process_in_port_dict.items():
        port_list = value
        process = key
        feature = feature_id_dict[process.parent]
        for i in range(len(port_list)):
            s = s + " { JModule_Bitfield_port (PORT_DIRECTION_IN,PORT_" + str(i) + "," + process.label + ", JMODULE_" \
                + feature.label.split(",")[0].upper() + ",   CONTEXT_" + feature.label.split(",")[1] \
                + "), BUFFER_" + port_list[i].buffer + "},\r\n"
    for key, value in process_out_port_dict.items():
        port_list = value
        process = key
        feature = feature_id_dict[process.parent]
        for i in range(len(port_list)):
            s = s + " { JModule_Bitfield_port (PORT_DIRECTION_OUT,PORT_" + str(i) + "," + process.label + ", JMODULE_" \
                + feature.label.split(",")[0].upper() + ",   CONTEXT_" + feature.label.split(",")[1] \
                + "), BUFFER_" + port_list[i].buffer + "},\r\n"
    s = s[:-3]
    s = s + "}};\r\n"
    return s


def calculate_buffer_size(port_params):
    """
    Calculates the required buffer size for port based on parameter values
    :param port_params: a dictionary containing various port parameters
    :return: the size of the required buffer
    """
    return int(port_params["num_chan"]) * int(port_params["frame_size"]) * int(port_params["sample_rate"])


def get_port_index(port, port_list):
    """
    Gets the index of the port from list
    :param port: the port to look for
    :param port_list: the list of ports to search in
    :return: the index of the port from list
    """
    if port in port_list:
        return port_list.index(port)
    return -1


def set_default_exec_feature(exec_list, feature_label):
    """
    Changes the execution list to have default JModule at the start
    :param exec_list: the original execution list
    :param feature_label: the default module to have at start of list
    :return: modified execution list
    """
    name, context = feature_label.split(",")
    name = "JMODULE_" + name.upper()
    context = "CONTEXT_" + context
    for string in exec_list:
        if name and context in string:
            exec_list.insert(1, exec_list.pop(exec_list.index(string)))
    return exec_list


def create_connection_list_string(edge_list, port_id_dict, process_id_dict,
                                  process_in_port_dict, process_out_port_dict, feature_id_dict):
    """
    Generates a string representation of the connection list file
    :param edge_list: a list of edges
    :param port_id_dict: a dictionary mapping port ids with ports
    :param process_id_dict: a dictionary mapping process ids with processes
    :param process_in_port_dict: a dictionary mapping processes with lists of input ports
    :param process_out_port_dict: a dictionary mapping processes with lists of output ports
    :param feature_id_dict: a dictionary mapping feature ids with features
    :return: a string representation of the connection list file
    """
    s = "#include \"Connection_List.h\"\r\n" \
        + "#include \"JModule_Bitfield.h\"\r\n" + "#include \"JModule_Id.h\"\r\n" \
        + "#include \"Port_Direction.h\"\r\n" + "\r\n" + "\r\n" \
        + "Connection_Item* Connection_List_get_connection" \
        + "(Connection_List* connection_list, uint32_t connection_id)\r\n" \
        + "{\r\n" + "	return  &connection_list->item[connection_id];\r\n" + "} \r\n" + "\r\n" + "\r\n" \
        + "Connection_List Connection_List_obj = {\r\n"
    s = s + ".num_items =" + str(len(edge_list)) + "," + "\r\n" + ".item = {" + "\r\n"
    for edge in edge_list:
        out_port = port_id_dict[edge.source]
        in_port = port_id_dict[edge.target]
        process_of_out_port = process_id_dict[out_port.parent]
        process_of_in_port = process_id_dict[in_port.parent]

        out_port_index = get_port_index(out_port, process_out_port_dict[process_of_out_port])
        in_port_index = get_port_index(in_port, process_in_port_dict[process_of_in_port])

        s = s + " { JModule_Bitfield_port ( PORT_DIRECTION_OUT, PORT_" + str(out_port_index) + "," \
            + process_of_out_port.label.upper() + "," + "JMODULE_" \
            + feature_id_dict[process_of_out_port.parent].label.split(",")[0].upper() + "," \
            + "CONTEXT_" + feature_id_dict[process_of_out_port.parent].label.split(",")[1].upper() + ")," + "\r\n" \
            + " JModule_Bitfield_port ( PORT_DIRECTION_IN, PORT_" + str(in_port_index) + "," \
            + process_of_in_port.label.upper() + "," + "JMODULE_" \
            + feature_id_dict[process_of_in_port.parent].label.split(",")[0].upper() + "," \
            + "CONTEXT_" + feature_id_dict[process_of_in_port.parent].label.split(",")[1].upper() \
            + ")},\r\n"
    s = s + "}};\r\n"
    return s


def consume_stack(stack, visited, port_requirement_dict, process_out_port_dict, feature_id_dict,
                  source_edge_dict, buffer_dict, buffer_list, port_id_dict, count, exec_list, pbs):
    """
    Works its way through a stack of processes to allocate buffers and generate an execution list

    :param stack: a stack containing processes
    :param visited: a dictionary keeping track of visited i.e. consumed processes
    :param port_requirement_dict: a dictionary mapping ports with their IO requirements
    :param process_out_port_dict: a dictionary mapping processes with lists of output ports
    :param feature_id_dict: a dictionary mapping feature ids with features
    :param source_edge_dict: a dictionary mapping ports with source edges
    :param buffer_dict:  a dictionary keeping track of buffer mappings
    :param buffer_list: a list of buffers
    :param port_id_dict:  a dictionary mapping port ids with ports
    :param count: an index
    :param exec_list: the string containing execution list representation
    :param pbs: previous buffer size

    :return: a modified stack, a dictionary keeping track of visited i.e. consumed processes,
    a modified dictionary mapping ports with their IO requirements,
    a modified dictionary keeping track of buffer mappings, a list of buffers, a dictionary mapping port ids with ports,
    count index, the string containing execution list representation
    """
    while stack:
        process = stack.pop()
        out_port_list = process_out_port_dict[process]
        for port in out_port_list:
            edge_list = source_edge_dict[port]
            for edge in edge_list:
                port_params = port.params
                temp_buf_size = calculate_buffer_size(port_params)
                buffer_size = temp_buf_size if temp_buf_size > 0 else pbs
                pbs = buffer_size
                buffer = None
                if buffer_size not in buffer_dict:
                    buffer = Buffer(str(len(buffer_list)), False)
                    buffer_list.append(buffer)
                    temp_list = list()
                    temp_list.append(buffer)
                    buffer_dict[buffer_size] = temp_list
                else:
                    buf_list = buffer_dict[buffer_size]
                    for buf in buf_list:
                        if buf.free:
                            buffer = buf
                            break
                    if buffer is None:
                        buffer = Buffer(str(len(buffer_list)), False)
                        buffer_list.append(buffer)
                        temp_list = list()
                        temp_list.append(buffer)
                        buffer_dict[buffer_size] = temp_list
                port_id_dict[edge.source].buffer = buffer.name
                port_id_dict[edge.target].buffer = buffer.name
                input_required = port_requirement_dict[port_id_dict[edge.target]]
                port_requirement_dict[port_id_dict[edge.target]] = input_required - 1
        visited[process] = True
        count += 1
        exec_str = ""
        exec_str = exec_str + "JModule_Bitfield_process (" + process.label + ", JMODULE_"
        exec_str = exec_str + feature_id_dict[process.parent].label.split(",")[0].upper() + ", CONTEXT_"
        exec_str = exec_str + feature_id_dict[process.parent].label.split(",")[1] + ")," + "\r\n"
        exec_list.append(exec_str)
    return stack, visited, port_requirement_dict, buffer_dict, buffer_list, port_id_dict, count, exec_list, pbs


def main():
    """
    The main caller of all functions and logic paths for creation of audio graph and its components
    """
    if len(sys.argv) < 3:
        print("Not Enough arguments")
        print("Usage:> python createAudioGraphFiles.py <Audio Graph XML Path> <Parameter Schema Json Path>")
        sys.exit()

    graph_xml_path = sys.argv[1]
    schema_json_path = sys.argv[2]
    default_module = sys.argv[3]
    output_directory = sys.argv[4] + os.path.sep
    schema_keys = ["jModuleParams", "processParams", "portParams"]
    graph_xml = read_feature_xml(graph_xml_path)
    schema = read_param_schema(schema_json_path)
    feature_list, feature_id_dict = get_features(graph_xml, schema[schema_keys[0]].keys())
    process_list, process_id_dict, process_in_port_dict, process_out_port_dict, different_call_intervals \
        = get_processes(graph_xml, schema[schema_keys[1]].keys())
    in_p, out_p = get_ports(graph_xml, schema[schema_keys[2]].keys(),
                            process_id_dict, process_in_port_dict, process_out_port_dict)
    in_port_list, in_port_id_dict, process_in_port_dict = in_p
    out_port_list, out_port_id_dict, process_out_port_dict = out_p
    port_id_dict = merge_two_dicts(in_port_id_dict, out_port_id_dict)
    buffer_dict = dict()
    buffer_list = list()

    object_count = 0
    execution_list_final = "#include \"Execution_List.h\"" + "\r\n" + "#include \"JModule_Id.h\"" + "\r\n"
    visited = dict()

    for process in process_list:
        visited[process] = False
    edge_list, source_edge_dict, target_edge_dict = get_edges(graph_xml, port_id_dict)
    port_requirement_dict = dict()

    for port in in_port_list:
        port_requirement_dict[port] = 0

    for edge in edge_list:
        port = port_id_dict[edge.target]
        inputs = port_requirement_dict[port]
        port_requirement_dict[port] = inputs + 1

    stack = list()
    pbs = 0
    for call_interval in different_call_intervals:
        count = 0
        tmp_e_lst = []
        while True:
            # update
            t_tup = consume_stack(
                stack, visited, port_requirement_dict, process_out_port_dict,
                feature_id_dict, source_edge_dict, buffer_dict, buffer_list,
                port_id_dict, count, tmp_e_lst, pbs)
            stack, visited, port_requirement_dict, buffer_dict, buffer_list, port_id_dict, count, tmp_e_lst, pbs = t_tup
            # for buffer in buffer_list:
            #     buffer.free = True
            for process in process_list:
                in_ports = process_in_port_dict[process]
                acc = 0
                for port in in_ports:
                    acc = acc + port_requirement_dict[port]
                if acc == 0 and not visited[process] and call_interval == int(process.params["call_interval"]):
                    stack.append(process)
            if not stack:
                break
        object_count = object_count + 1
        execution_list_final = execution_list_final + "Execution_List Execution_List_obj" + str(
            object_count - 1) + " = {" + "\r\n" + ".num_items = " + str(count) + "," + "\r\n"
        if default_module != "None":
            tmp_e_lst = set_default_exec_feature(tmp_e_lst, default_module)
        execution_list_final = execution_list_final + "{" + "\r\n" + "".join(tmp_e_lst)[
                                                                     :-3] + "\r\n" + "}" + "\r\n" + "};" + "\r\n"

    create_file(output_directory + os.path.sep + "Audio_Graph", "Execution_List.c", execution_list_final)
    create_file(output_directory + os.path.sep + "Audio_Graph", "Connection_List.c", create_connection_list_string(
        edge_list, port_id_dict, process_id_dict, process_in_port_dict, process_out_port_dict, feature_id_dict))
    create_file(output_directory + os.path.sep + "Audio_Graph", "Module_List.c",
                create_module_list_string(feature_list))
    create_file(output_directory + os.path.sep + "Audio_Graph", "Port_List.c", create_port_list_string(
        port_id_dict, process_in_port_dict, process_out_port_dict, feature_id_dict))
    create_file(output_directory + os.path.sep + "Audio_Graph", "Audio_Graph.c",
                create_audio_graph(different_call_intervals))
    create_file(output_directory + os.path.sep + "JModule_Lib", "JModule_Id.h",
                create_feature_id_string(feature_list))
    create_file(output_directory + os.path.sep + "JModule_Lib", "JModule_List.c",
                create_feature_list_string(feature_list))


if __name__ == '__main__':
    main()
