"""
ojcu.py is the main driver script to generate JModule and Audio graph files from XMLs and Database Schema
"""

import os
import sys
#import argparse

__author__ = "cd1043538 | Desai, Chirayu Ketan"
__copyright__ = "COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED"


#def check_arg(args=None):
#    """
#    Function to Parse parameters
#    :param args: None
#    :return: argument values for Parameter Schema, Audio Graph XML path, Jmodule paths, an optional default module
#    and an optional output directory path
#    """
#    parser = argparse.ArgumentParser(description='Master Script for Creating Audio Graph Components')
#    parser.add_argument('-dp', '--dependency_path', help='Path were the files on which this script depends reside', required=True)
#    parser.add_argument('-ps', '--param_schema', help='Path to parameter schema JSON', required=True)
#    parser.add_argument('-ag', '--audio_graph', help='Path to audio graph XML', required=True)
#    parser.add_argument('-jm', '--jmodules', help='Path to one or more JModule XMLs', nargs='+', required=True)
#    parser.add_argument('-dm', '--default_module', help="Optional Default JModule for Execution")
#    parser.add_argument('-od', '--output_directory', help="Optional Output Directory Path")
#
#    results = parser.parse_args(args)
#    return results.param_schema, results.audio_graph, results.jmodules, results.default_module, results.output_directory, results.dependency_path

def read_xml(param_schema, audio_graph, jmodules, default_module, output_directory, dependency_path):
    """
    Driver Function to generate JModule and Audio graph files from XMLs and Database Schema
    """
    print('running read_xml')

    #param_schema, audio_graph, jmodules, default_module, output_directory, dependency_path = check_arg(sys.argv[1:])
    feature_desc_script = os.sep.join([dependency_path, "createDescriptionFilesCL.py"])
    audio_graph_desc_script = os.sep.join([dependency_path, "createAudioGraphFiles.py"])
    graph_xml_path = audio_graph
    schema_json_path = param_schema
    default_module = default_module if default_module is not None else "None"


    output_directory = output_directory if output_directory is not None else "."
    os.system("python " + audio_graph_desc_script + " " + graph_xml_path + " " + schema_json_path
              + " " + default_module + " " + output_directory)

    for arg in jmodules:
        os.system("python " + feature_desc_script + " " + arg + " " + schema_json_path + " " + output_directory + " " + dependency_path)


#if __name__ == '__main__':
#    main()
