#!/usr/bin/env python
################################################################################
#
#  BOSE CORPORATION
#  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
#  This program may not be reproduced, in whole or in part in any
#  form or any means whatsoever without the written permission of:
#     BOSE CORPORATION
#     The Mountain
#     Framingham, MA 01701-9168
#
################################################################################

import fnmatch
import getopt
import glob
import json
import os
import re
import sys

from collections import defaultdict
from collections import OrderedDict
from pathlib import Path

from clang.cindex import Config, CursorKind, Index, StorageClass, TypeKind

from code_patcher import CodePatcher

CONFIG_FILE_NAME = 'code_tagger_config.json'

__usage__ = f""" \

Usage:
  code_tagger.py [-h] -c code_tagger_config.json

where:

Description:

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -c,   --cfg   Configuration file directory containing {CONFIG_FILE_NAME}
  -f,   --file  Configuration file
  -i,   --inc   Directories to search for header files, separated by semicolon
  -l,   --ldf   LDF file directory
  -w,   --work  Work directory to hold various json files
  -s,   --src   Source directories, separated by semicolon

"""

class Configuration:
    def __init__(self, config_json_file, src_directories):
        self._config_json_file = config_json_file
        self._src_directories = src_directories

        self._feature = ''
        self._enabled = False
        self._source_files = []
        self._step_1_func = ''
        self._step_2_func = ''
        self._step_3_func = ''
        self._step_4_func = ''
        self._step_5_func = ''
        self._step_6_func = ''
        self._step_7_func = ''
        self._step_8_func = ''
        self._step_9_func = ''
        self._excluded_files = []
        self._excluded_symbols = []
        self._explicit_symbol_map = []
        self._probe_points = []

        self._read_config()

    def _add_src_file(self, src_file):
        if src_file.split('.')[-1].lower() == 'c':
            src_file_abs_path = os.path.abspath(src_file)

            if os.path.isfile(src_file_abs_path):
                if src_file_abs_path not in self._source_files:
                    self._source_files.append(src_file_abs_path)
        else:
            print(f'Warning: source file {src_file} not a C file')

    def _read_config(self):
        with open(self._config_json_file, 'r', encoding='utf-8') as config_json_fp:
            config = json.load(config_json_fp)
            mandatory_config_keys = {'FEATURE', 'ENABLE', 'SOURCE_FILES', 'STEP_1_FUNC', 'STEP_2_FUNC', 'STEP_3_FUNC', 'STEP_4_FUNC', 'STEP_5_FUNC', 'STEP_6_FUNC', 'STEP_7_FUNC', 'STEP_8_FUNC', 'STEP_9_FUNC'}

            if config.keys() < mandatory_config_keys:
                print(f'Error: {", ".join(mandatory_config_keys.keys())} are mandatory keys in the configuration file')
                sys.exit(1)

            self._feature = config['FEATURE'].title()
            self._enabled = config['ENABLE']

            for src_dir in self._src_directories:
                for src in config['SOURCE_FILES']:
                    if '*' not in src:
                        self._add_src_file(os.path.join(src_dir, src))
                    else:
                        src_files = glob.glob(os.path.join(src_dir, src))

                        for src_file in src_files:
                            self._add_src_file(src_file)

            for src_file in self._source_files:
                if os.path.isfile(f'{src_file}.orig'):
                    print(f'Error: {src_file}.orig existence prevents further processing')
                    sys.exit(1)

            self._step_1_func = config['STEP_1_FUNC']
            self._step_2_func = config['STEP_2_FUNC']
            self._step_3_func = config['STEP_3_FUNC']
            self._step_4_func = config['STEP_4_FUNC']
            self._step_5_func = config['STEP_5_FUNC']
            self._step_6_func = config['STEP_6_FUNC']
            self._step_7_func = config['STEP_7_FUNC']
            self._step_8_func = config['STEP_8_FUNC']
            self._step_9_func = config['STEP_9_FUNC']

            if 'EXCLUDED_FILES' in config:
                self._excluded_files = config['EXCLUDED_FILES']

            if 'EXCLUDED_SYMBOLS' in config:
                self._excluded_symbols = config['EXCLUDED_SYMBOLS']

            if 'EXPLICIT_SYMBOL_MAP' in config:
                symbol_mappings = config['EXPLICIT_SYMBOL_MAP']

                for symbol, mappings in symbol_mappings.items():
                    self._explicit_symbol_map.append([symbol, mappings])

            if 'PROBE_POINTS' in config:
                self._probe_points = config['PROBE_POINTS']

    @property
    def feature(self):
        """ Feature for which code is parsed and tagged """
        return self._feature

    @property
    def enabled(self):
        """ Indicates wether code parsing and tagging is enabled for the feature """
        return self._enabled

    @property
    def source_files(self):
        """ List of source files to parse and tag """
        return self._source_files

    @property
    def step_1_func(self):
        """ Step 1 function name """
        return self._step_1_func

    @property
    def step_2_func(self):
        """ Step 2 function name """
        return self._step_2_func

    @property
    def step_3_func(self):
        """ Step 3 function name """
        return self._step_3_func

    @property
    def step_4_func(self):
        """ Step 4 function name """
        return self._step_4_func

    @property
    def step_5_func(self):
        """ Step 5 function name """
        return self._step_5_func

    @property
    def step_6_func(self):
        """ Step 6 function name """
        return self._step_6_func

    @property
    def step_7_func(self):
        """ Step 7 function name """
        return self._step_7_func

    @property
    def step_8_func(self):
        """ Step 8 function name """
        return self._step_8_func

    @property
    def step_9_func(self):
        """ Step 9 function name """
        return self._step_9_func

    @property
    def excluded_files(self):
        """ List of files to exclude from parsing and tagging """
        return self._excluded_files

    @property
    def excluded_symbols(self):
        """ List of symbols to exclude from parsing and tagging """
        return self._excluded_symbols

    @property
    def explicit_symbol_map(self):
        """ List of explicit symbol mappings """
        return self._explicit_symbol_map

    @property
    def probe_points(self):
        """ List of probe point functions """
        return self._probe_points

    def is_node_excluded(self, node):
        """ Checks wether the specified node is excluded """
        if not node.extent.start.file:
            return False

        for file in self._excluded_files:
            if fnmatch.fnmatch(node.extent.start.file.name, file):
                return True

        for symbol in self._excluded_symbols:
            if not re.match(symbol, node.spelling) is None:
                return True

        return False

class ParsedContent:
    def __init__(self):
        self._bss_data = {}
        self._data = {}
        self._const_data = {}
        self._functions = {}
        self._call_graph = {}

    @property
    def bss_data(self):
        """ Parsed BSS data (global variables and local static variables) """
        return self._bss_data

    @property
    def data(self):
        """ Parsed data (global variables and local static variables) """
        return self._data

    @property
    def const_data(self):
        """ Parsed constant data (global variables and local static variables) """
        return self._const_data

    @property
    def functions(self):
        """ Parsed functions """
        return self._functions

    @property
    def call_graph(self):
        """ Parsed call graphs """
        return self._call_graph

    def get_call_graph(self, func_name, visited, print_call=False, depth=0):
        """ Retrieves call graph of a given function name """
        for functions in self._call_graph.values():
            if func_name in functions:
                for called_func in functions[func_name]:
                    if print_call is True:
                        print(f'{"  " * depth} {called_func}')

                    if called_func in visited:
                        continue

                    visited.append(f'{called_func}()')
                    self.get_call_graph(called_func, visited, print_call, depth + 1)

class CodeParser:
    def __init__(self, config, inc_directories):
        self._config = config
        self._inc_directories = inc_directories

        self._source_files = config.source_files

        self._src_translation_units = {}

        self._parsed_content = ParsedContent()
        self._bss_data = self._parsed_content.bss_data
        self._data = self._parsed_content.data
        self._const_data = self._parsed_content.const_data
        self._functions = self._parsed_content.functions
        self._call_graph = self._parsed_content.call_graph

        if config.enabled:
            self._parse_source_code()

    def _check_parse_errors(self, translation_unit):
        parse_error = False

        for diag in translation_unit.diagnostics:
            if diag.severity in (diag.Error, diag.Fatal):
                print(f'Severity: {diag.severity}')
                print(f'Location: {diag.location}')
                print(f'Spelling: {diag.spelling}')
                print(f'Ranges:   {list(diag.ranges)}')
                print(f'FixIts:   {list(diag.fixits)}')
                parse_error = True

        if parse_error:
            sys.exit(1)

    def _is_const_data(self, cursor):
        for token in cursor.get_tokens():
            if token.spelling == 'const' and cursor.type.kind != TypeKind.POINTER:
                return True

        return False

    def _is_bss_data(self, cursor):
        for token in cursor.get_tokens():
            if token.spelling == '=':
                return False

        return True

    def _is_forward_decl(self, cursor):
        return cursor.get_definition() is None

    def _store_data(self, src_file, cursor, parent):
        if not self._config.is_node_excluded(cursor):
            if not self._is_const_data(cursor):
                if self._is_bss_data(cursor):
                    self._bss_data[src_file][cursor.spelling] = {}
                    self._bss_data[src_file][cursor.spelling]['start'] = cursor.extent.start
                    self._bss_data[src_file][cursor.spelling]['parent'] = parent.spelling if parent is not None else ''
                else:
                    self._data[src_file][cursor.spelling] = {}
                    self._data[src_file][cursor.spelling]['start'] = cursor.extent.start
                    self._data[src_file][cursor.spelling]['parent'] = parent.spelling if parent is not None else ''
            else:
                self._const_data[src_file][cursor.spelling] = {}
                self._const_data[src_file][cursor.spelling]['start'] = cursor.extent.start
                self._const_data[src_file][cursor.spelling]['parent'] = parent.spelling if parent is not None else ''

    def _process_cursor(self, src_file, cursor, current_func=None):
        if cursor.kind == CursorKind.VAR_DECL:
            if current_func is None:
                if (cursor.lexical_parent.kind == CursorKind.TRANSLATION_UNIT
                    and cursor.storage_class != StorageClass.EXTERN):
                    # Global variable
                    self._store_data(src_file, cursor, None)
            else:
                if cursor.storage_class == StorageClass.STATIC:
                    # Local static variable
                    self._store_data(src_file, cursor, current_func)

        if (cursor.kind == CursorKind.FUNCTION_DECL
            and not self._is_forward_decl(cursor)
            and not self._config.is_node_excluded(cursor)):
            current_func = cursor
            self._functions[src_file][current_func.spelling] = cursor.extent.start

        if (not current_func is None
            and cursor.kind == CursorKind.CALL_EXPR
            and cursor.referenced
            and not self._config.is_node_excluded(cursor.referenced)
            and cursor.referenced.spelling not in self._call_graph[src_file][current_func.spelling]):
            self._call_graph[src_file][current_func.spelling].append(cursor.referenced.spelling)

        for child in cursor.get_children():
            self._process_cursor(src_file, child, current_func)

    def _parse_source_code(self):
        index = Index.create()
        arguments = ['-Ddm=__attribute__((annotate("dm")))', '-Dpm=__attribute__((annotate("pm")))']

        if len(self._inc_directories) > 0:
            for inc_dir in self._inc_directories:
                arguments.append(f'-I"{inc_dir}"')

        for src_file in self._source_files:
            if os.path.isfile(f'{src_file}.orig'):
                continue

            print(f'Info: Parsing source file {src_file} ...\n')
            self._bss_data[src_file] = {}
            self._data[src_file] = {}
            self._const_data[src_file] = {}
            self._functions[src_file] = {}
            self._call_graph[src_file] = defaultdict(list)

            tu_src = index.parse(src_file, arguments)

            if not tu_src:
                print(f'Error: unable to parse source file {src_file}')
                sys.exit(1)

            self._src_translation_units[src_file] = tu_src
            self._check_parse_errors(tu_src)
            self._process_cursor(src_file, tu_src.cursor)

    @property
    def parsed_content(self):
        """ Parsed content """
        return self._parsed_content

class CodeTagger():
    def __init__(self, config, parsed_content, ldf_dir, work_dir):
        self._feature = config.feature
        self._step_1_func = config.step_1_func
        self._step_2_func = config.step_2_func
        self._step_3_func = config.step_3_func
        self._step_4_func = config.step_4_func
        self._step_5_func = config.step_5_func
        self._step_6_func = config.step_6_func
        self._step_7_func = config.step_7_func
        self._step_8_func = config.step_8_func
        self._step_9_func = config.step_9_func
        self._parsed_data = parsed_content
        self._bss_data = parsed_content.bss_data
        self._data = parsed_content.data
        self._const_data = parsed_content.const_data
        self._functions = parsed_content.functions
        self._call_graph = parsed_content.call_graph
        self._ldf_dir = ldf_dir
        self._work_dir = work_dir

        self._src_patcher = CodePatcher()
        self._tag_map_json_data = OrderedDict()

        self._SECTION_PREFIX = f'bose{self._feature}'
        self._PRAGMA_PREFIX = f'#pragma section("{self._SECTION_PREFIX}'
        self._PRAGMA_SUFFIX = '")\n'
        self._PRAGMA_BSS_SUFFIX = '", ZERO_INIT)\n'
        self._SYMBOL_NAME_PREFIX = 'Symbol_'

        if config.enabled:
            self._tag_bss_data()
            self._tag_data()
            self._tag_const_data()
            self._tag_functions()
            self._src_patcher.apply()
            self._create_tag_map_json_file()
            self._create_probe_points_json_file(config)

        self._create_mem_map_header_files(config)

    def _remove_common_list_elements(self, lst_1, lst_2):
        for elem in lst_1[:]:
            if elem in lst_2:
                lst_1.remove(elem)
                lst_2.remove(elem)

    def _replace_list_elem(self, lst, old, new):
        for i, value in enumerate(lst):
            if value == old:
                lst[i] = new

    def _insert_section_pragma(self, src_file, section_type_str, symbol_name, symbol_start):
        pragma = f'{self._PRAGMA_PREFIX}{section_type_str}_{symbol_name}'
        pragma = f'{pragma}{self._PRAGMA_SUFFIX if "bsz" not in section_type_str.lower() else self._PRAGMA_BSS_SUFFIX}'

        # Restore any indentation the line containing the symbol might have had
        pragma = pragma.ljust(symbol_start.column + len(pragma) - 1, ' ')

        self._src_patcher.insert(src_file, symbol_start, pragma)

    def _tag_bss_data(self):
        section_type_str = 'BszData'
        bss_data_symbol_counter = 0

        for src_file, bss_data in self._bss_data.items():
            src_file_name = os.path.basename(src_file)
            self._tag_map_json_data[src_file_name] = OrderedDict()
            self._tag_map_json_data[src_file_name]['BSS_DATA'] = OrderedDict()

            for bss_data_name, bss_data_info in bss_data.items():
                symbol_name = f'{self._SYMBOL_NAME_PREFIX}{bss_data_symbol_counter}'

                self._insert_section_pragma(src_file, section_type_str, symbol_name, bss_data_info['start'])
                bss_data_symbol_counter += 1

                data_key = bss_data_name if bss_data_info['parent'] == '' else f'{bss_data_info["parent"]}().{bss_data_name}'
                self._tag_map_json_data[src_file_name]['BSS_DATA'][data_key] = f'{self._SECTION_PREFIX}{section_type_str}_{symbol_name}'

    def _tag_data(self):
        section_type_str = 'Data'
        data_symbol_counter = 0

        for src_file, data in self._data.items():
            src_file_name = os.path.basename(src_file)

            if src_file_name not in self._tag_map_json_data:
                self._tag_map_json_data[src_file_name] = OrderedDict()

            self._tag_map_json_data[src_file_name]['DATA'] = OrderedDict()

            for data_name, data_info in data.items():
                symbol_name = f'{self._SYMBOL_NAME_PREFIX}{data_symbol_counter}'

                self._insert_section_pragma(src_file, section_type_str, symbol_name, data_info['start'])
                data_symbol_counter += 1

                data_key = data_name if data_info['parent'] == '' else f'{data_info["parent"]}().{data_name}'
                self._tag_map_json_data[src_file_name]['DATA'][data_key] = f'{self._SECTION_PREFIX}{section_type_str}_{symbol_name}'

    def _tag_const_data(self):
        section_type_str = 'ConstData'
        const_data_symbol_counter = 0

        for src_file, const_data in self._const_data.items():
            src_file_name = os.path.basename(src_file)

            if src_file_name not in self._tag_map_json_data:
                self._tag_map_json_data[src_file_name] = OrderedDict()

            self._tag_map_json_data[src_file_name]['CONST_DATA'] = OrderedDict()

            for const_data_name, const_data_info in const_data.items():
                symbol_name = f'{self._SYMBOL_NAME_PREFIX}{const_data_symbol_counter}'

                self._insert_section_pragma(src_file, section_type_str, symbol_name, const_data_info['start'])
                const_data_symbol_counter += 1

                data_key = const_data_name if const_data_info['parent'] == '' else f'{const_data_info["parent"]}().{const_data_name}'
                self._tag_map_json_data[src_file_name]['CONST_DATA'][data_key] = f'{self._SECTION_PREFIX}{section_type_str}_{symbol_name}'

    def _tag_functions(self):
        section_type_str = 'Code'
        code_symbol_counter = 0

        for src_file, functions in self._functions.items():
            src_file_name = os.path.basename(src_file)

            if src_file_name not in self._tag_map_json_data:
                self._tag_map_json_data[src_file_name] = OrderedDict()

            self._tag_map_json_data[src_file_name]['CODE'] = OrderedDict()

            for func_name, func_location in functions.items():
                symbol_name = f'{self._SYMBOL_NAME_PREFIX}{code_symbol_counter}'

                self._insert_section_pragma(src_file, section_type_str, symbol_name, func_location)
                code_symbol_counter += 1

                self._tag_map_json_data[src_file_name]['CODE'][f'{func_name}()'] = f'{self._SECTION_PREFIX}{section_type_str}_{symbol_name}'

    def _create_tag_map_json_file(self):
        # Serialize JSON data
        tag_map_json_object = json.dumps(self._tag_map_json_data, indent=4, sort_keys=False)
        tag_map_json_file = os.path.join(self._work_dir, f'{self._feature.lower()}_tag_map.json')
        with open(tag_map_json_file, 'w', encoding='utf-8') as tag_map_json_fp:
            tag_map_json_fp.write(tag_map_json_object)

    def _get_probe_points(self, config):
        probe_points = {}
        if config.probe_points:
            macro_val = "0x00FE"
            count = 0

            for fun in config.probe_points:
                probe_val = [macro_val + f'{count:02x}'.upper() + "00", macro_val + f'{count:02x}'.upper() + "01"]
                count = count + 1
                probe_points[fun] = probe_val

        return probe_points

    def _create_probe_points_json_file(self, config):
        if config.probe_points:
            json_data = {}
            json_data["enable"] = True
            count = 0
            func_data = {}

            for func in config.probe_points:
                func_data[func] = {"process_id":count}
                count = count + 1

            json_data["processes"] = func_data

            # Serialize JSON data
            tag_map_json_object = json.dumps(json_data, indent=4, sort_keys=False)
            tag_map_json_file = os.path.join(self._work_dir, 'probe_points.json')
            with open(tag_map_json_file, 'w', encoding='utf-8') as tag_map_json_fp:
                tag_map_json_fp.write(tag_map_json_object)

    def tag_explict_symbol_map(self, config):
        labels = ["DATA", "CODE", "CONST", "BSZ"]
        symbol_table = []

        for symbol, maps in config.explicit_symbol_map:
            ex_symbol_data = list()

            for label in labels:
                if label in symbol.upper():
                    if label == "BSZ":
                        label = "BSS_DATA"

                    if label == "CONST":
                        label = "CONST_DATA"

                    for map in maps:
                        flag = False
                        if label == "CODE":
                            map = f'{map}()'
                        for sections in self._tag_map_json_data.values():
                            if flag:
                                break
                            for section, value in sections[label].items():
                                if section == map:
                                    ex_symbol_data.append(value)
                                    flag = True
                                    break

            symbol_table.append(ex_symbol_data)

        return symbol_table

    def _create_mem_map_header_files(self, config):
        called_functions_step_1 = []
        called_functions_step_2 = []
        called_functions_step_3 = []
        called_functions_step_4 = []
        called_functions_step_5 = []
        called_functions_step_6 = []
        called_functions_step_7 = []
        called_functions_step_8 = []
        called_functions_step_9 = []
        bss_data = []
        data = []
        const_data = []
        code = []
        feat_mem_map_file_name=f'bose_{self._feature.lower()}_custom_mem_map.h'
        feat_mem_map_file=os.path.join(self._ldf_dir, f'{feat_mem_map_file_name}')
        feat_mem_map_inc_guard=f'BOSE_{self._feature.upper()}_CUSTOM_MEM_MAP_H'
        mem_map_file=os.path.join(self._ldf_dir, 'custom_mem_map.h')

        if len(self._step_1_func) > 0:
            called_functions_step_1.append(f'{self._step_1_func}()')
            self._parsed_data.get_call_graph(self._step_1_func, called_functions_step_1)

        if len(self._step_2_func) > 0:
            called_functions_step_2.append(f'{self._step_2_func}()')
            self._parsed_data.get_call_graph(self._step_2_func, called_functions_step_2)

        if len(self._step_3_func) > 0:
            called_functions_step_3.append(f'{self._step_3_func}()')
            self._parsed_data.get_call_graph(self._step_3_func, called_functions_step_3)

        if len(self._step_4_func) > 0:
            called_functions_step_4.append(f'{self._step_4_func}()')
            self._parsed_data.get_call_graph(self._step_4_func, called_functions_step_4)

        if len(self._step_5_func) > 0:
            called_functions_step_5.append(f'{self._step_5_func}()')
            self._parsed_data.get_call_graph(self._step_5_func, called_functions_step_5)

        if len(self._step_6_func) > 0:
            called_functions_step_6.append(f'{self._step_6_func}()')
            self._parsed_data.get_call_graph(self._step_6_func, called_functions_step_6)

        if len(self._step_7_func) > 0:
            called_functions_step_7.append(f'{self._step_7_func}()')
            self._parsed_data.get_call_graph(self._step_7_func, called_functions_step_7)

        if len(self._step_8_func) > 0:
            called_functions_step_8.append(f'{self._step_8_func}()')
            self._parsed_data.get_call_graph(self._step_8_func, called_functions_step_8)

        if len(self._step_9_func) > 0:
            called_functions_step_9.append(f'{self._step_9_func}()')
            self._parsed_data.get_call_graph(self._step_9_func, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_1, called_functions_step_2)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_3)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_4)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_5)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_6)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_1, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_2, called_functions_step_3)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_4)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_5)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_6)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_2, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_3, called_functions_step_4)
        self._remove_common_list_elements(called_functions_step_3, called_functions_step_5)
        self._remove_common_list_elements(called_functions_step_3, called_functions_step_6)
        self._remove_common_list_elements(called_functions_step_3, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_3, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_3, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_4, called_functions_step_5)
        self._remove_common_list_elements(called_functions_step_4, called_functions_step_6)
        self._remove_common_list_elements(called_functions_step_4, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_4, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_4, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_5, called_functions_step_6)
        self._remove_common_list_elements(called_functions_step_5, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_5, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_5, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_6, called_functions_step_7)
        self._remove_common_list_elements(called_functions_step_6, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_6, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_7, called_functions_step_8)
        self._remove_common_list_elements(called_functions_step_7, called_functions_step_9)

        self._remove_common_list_elements(called_functions_step_8, called_functions_step_9)

        # Replace function names with obfuscated function names
        for sections in self._tag_map_json_data.values():
            for func_name, obfuscated_func_name in sections['CODE'].items():
                if func_name in called_functions_step_1:
                    self._replace_list_elem(called_functions_step_1, func_name, obfuscated_func_name)

                if func_name in called_functions_step_2:
                    self._replace_list_elem(called_functions_step_2, func_name, obfuscated_func_name)

                if func_name in called_functions_step_3:
                    self._replace_list_elem(called_functions_step_3, func_name, obfuscated_func_name)

                if func_name in called_functions_step_4:
                    self._replace_list_elem(called_functions_step_4, func_name, obfuscated_func_name)

                if func_name in called_functions_step_5:
                    self._replace_list_elem(called_functions_step_5, func_name, obfuscated_func_name)

                if func_name in called_functions_step_6:
                    self._replace_list_elem(called_functions_step_6, func_name, obfuscated_func_name)

                if func_name in called_functions_step_7:
                    self._replace_list_elem(called_functions_step_7, func_name, obfuscated_func_name)

                if func_name in called_functions_step_8:
                    self._replace_list_elem(called_functions_step_8, func_name, obfuscated_func_name)

                if func_name in called_functions_step_9:
                    self._replace_list_elem(called_functions_step_9, func_name, obfuscated_func_name)

        for sections in self._tag_map_json_data.values():
            bss_data += list(sections['BSS_DATA'].values())
            data += list(sections['DATA'].values())
            const_data += list(sections['CONST_DATA'].values())

            for obfuscated_func_name in sections['CODE'].values():
                if (not obfuscated_func_name in called_functions_step_1
                    and not obfuscated_func_name in called_functions_step_2
                    and not obfuscated_func_name in called_functions_step_3
                    and not obfuscated_func_name in called_functions_step_4
                    and not obfuscated_func_name in called_functions_step_5
                    and not obfuscated_func_name in called_functions_step_6
                    and not obfuscated_func_name in called_functions_step_7
                    and not obfuscated_func_name in called_functions_step_8
                    and not obfuscated_func_name in called_functions_step_9):
                    code.append(obfuscated_func_name)

        if config.explicit_symbol_map:
            explicit_symbol_mapping_table = self.tag_explict_symbol_map(config)

        # Remove duplicate symbols
        if config.explicit_symbol_map:
            count = 0

            while count < len(explicit_symbol_mapping_table):
                if len(explicit_symbol_mapping_table[count]) > 0:
                    data = [x for x in data if x not in explicit_symbol_mapping_table[count]]
                    code = [x for x in code if x not in explicit_symbol_mapping_table[count]]
                    bss_data = [x for x in bss_data if x not in explicit_symbol_mapping_table[count]]
                    const_data = [x for x in const_data if x not in explicit_symbol_mapping_table[count]]

                count += 1

        with open(feat_mem_map_file, 'w', encoding='utf-8') as feat_mem_map_fp:
            feat_mem_map_fp.write('/*\n')
            feat_mem_map_fp.write(' * BOSE CORPORATION\n')
            feat_mem_map_fp.write(' * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED\n')
            feat_mem_map_fp.write(' * This program may not be reproduced, in whole or in part in any\n')
            feat_mem_map_fp.write(' * form or any means whatsoever without the written permission of:\n')
            feat_mem_map_fp.write(' *    BOSE CORPORATION\n')
            feat_mem_map_fp.write(' *    The Mountain\n')
            feat_mem_map_fp.write(' *    Framingham, MA 01701-9168\n')
            feat_mem_map_fp.write(' */\n\n')
            feat_mem_map_fp.write(f'#ifndef {feat_mem_map_inc_guard}\n')
            feat_mem_map_fp.write(f'#define {feat_mem_map_inc_guard}')

            if len(bss_data) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}BszData \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(bss_data))

            if len(data) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}Data \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(data))

            if len(const_data) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}ConstData \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(const_data))

            if len(code) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}Code \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(code))

            if len(called_functions_step_1) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep1 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_1))

            if len(called_functions_step_2) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep2 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_2))

            if len(called_functions_step_3) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep3 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_3))

            if len(called_functions_step_4) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep4 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_4))

            if len(called_functions_step_5) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep5 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_5))

            if len(called_functions_step_6) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep6 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_6))

            if len(called_functions_step_7) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep7 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_7))

            if len(called_functions_step_8) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep8 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_8))

            if len(called_functions_step_9) > 0:
                feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}CodeStep9 \\\n  ')
                feat_mem_map_fp.write(' \\\n  '.join(called_functions_step_9))

            if config.explicit_symbol_map:
                count = 0

                for symbol, maps in config.explicit_symbol_map:
                    if len(explicit_symbol_mapping_table[count]) > 0:
                        feat_mem_map_fp.write(f'\n\n#define {self._SECTION_PREFIX}{symbol} \\\n  ')
                        feat_mem_map_fp.write(' \\\n  '.join(explicit_symbol_mapping_table[count]))

                    count += 1

            feat_mem_map_fp.write(f'\n\n#endif /* {feat_mem_map_inc_guard} */\n')

        if os.path.isfile(feat_mem_map_file):
            mem_map_open_mode = 'r+' if os.path.isfile(mem_map_file) else 'a+'

            with open(mem_map_file, mem_map_open_mode, encoding='utf-8') as mem_map_fp:
                content = mem_map_fp.read()

                if not feat_mem_map_file_name in content:
                    mem_map_fp.write(f'#include "{feat_mem_map_file_name}"\n')

#*******************************************************************************
# Function: usage()
#*******************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()

#*******************************************************************************
# Function: main()
#*******************************************************************************
def main():
    config_json_file = None
    inc_directories = []
    ldf_dir = None
    work_dir = None
    src_directories = []
    config = None
    libclang_dyn_lib_file = os.path.join(os.getenv('CONDA_PREFIX'), 'Library/bin/libclang-13.dll')

    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hc:f:i:l:w:s:', ['help', 'cfg=', 'file=', 'inc=', 'ldf=', 'work=', 'src='])
    except getopt.GetoptError:
        usage()

    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
        elif opt in ('-c', '--cfg'):
            if os.path.isdir(os.path.abspath(arg)):
                config_json_file = os.path.abspath(os.path.join(arg, CONFIG_FILE_NAME))
                if not os.path.isfile(config_json_file):
                    print(f'Error: configuration file {config_json_file} could not be found')
                    sys.exit(1)
        elif opt in ('-f', '--file'):
            config_json_file = os.path.abspath(arg)
            if not os.path.isfile(config_json_file):
                print(f'Error: configuration file {config_json_file} could not be found')
                sys.exit(1)
        elif opt in ('-i', '--inc'):
            if ';' in arg:
                inc_directories = arg.split(';')
            else:
                inc_directories.append(arg)

            for inc_dir in inc_directories:
                inc_dir_abs_path = os.path.abspath(inc_dir)

                if os.path.isdir(inc_dir_abs_path):
                    inc_directories = list(map(lambda d, id=inc_dir, id_ap=inc_dir_abs_path: d.replace(id, id_ap), inc_directories))
                else:
                    inc_directories.remove(inc_dir)
        elif opt in ('-l', '--ldf'):
            ldf_dir = os.path.abspath(arg)

            if not os.path.isdir(ldf_dir):
                print(f'Error: specified directory {ldf_dir} doesn\'t exits')
                sys.exit(1)

            if not list(Path(ldf_dir).rglob("*.ldf")):
                print(f'Error: specified directory {ldf_dir} doesn\'t contain an LDF file')
                sys.exit(1)
        elif opt in ('-w', '--work'):
            work_dir = os.path.abspath(arg)

            if not os.path.isdir(work_dir):
                print(f'Error: specified directory {work_dir} doesn\'t exits')
                sys.exit(1)

        elif opt in ('-s', '--src'):
            if ';' in arg:
                src_directories = arg.split(';')
            else:
                src_directories.append(arg)

            for src_dir in src_directories:
                src_dir_abs_path = os.path.abspath(src_dir)

                if os.path.isdir(src_dir_abs_path):
                    src_directories = list(map(lambda d, sd=src_dir, sd_ap=src_dir_abs_path: d.replace(sd, sd_ap), src_directories))
                else:
                    src_directories.remove(src_dir)

    if (config_json_file is None) or (ldf_dir is None) or (work_dir is None) or (len(src_directories) == 0):
        usage()

    if not os.path.isfile(libclang_dyn_lib_file):
        print('Error: unable to locate libclang dynamic library')
        sys.exit(1)

    Config.set_library_file(libclang_dyn_lib_file)
    config = Configuration(config_json_file, src_directories)
    src_parser = CodeParser(config, inc_directories)
    CodeTagger(config, src_parser.parsed_content, ldf_dir, work_dir)

#*******************************************************************************
if __name__ == '__main__':
    main()
