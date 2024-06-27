#!/usr/bin/env python

import os
import getopt
import sys

__usage__ = """ \

Usage:
  <script> -t tsf_file

Description:

Assumptions:

Options:
  -h,   --help   Display usage information (this message)
  -t,   --tsf    Default locations:
"""

class AweSymbol(object):
    def __init__(self, core_id, symbol_name, class_name, object_id):
        self.core_id = core_id
        self.symbol_name = symbol_name
        self.class_name = class_name
        self.object_id = object_id

class AresTsf(object):
    ''' Read TSF file '''
    def __init__(self, tsf_file=None, argv=None):
        if argv is not None:
            tsf_file = self._parse_args(argv)
            self._tsf_file = "C:\\Users\\110468\\Downloads\\audioWeaver\\CustomModule-ARES-Set1\\audio\\AWEDesigner\\ControlAudio_release.tsf"
            self.tsf_symbols = {}
            self._parse_tsf_file()

    def _usage(self):
        sys.stdout.write(__usage__)
        sys.exit(1)

    def _parse_args(self, argv):
        app_parser = None
        tsf_file = None

        try:
            opts, args = getopt.getopt(argv, 'ha:t:', ['help', 'tsf='])
        except getopt.GetoptError:
            self._usage()

        for opt, arg in opts:
            if opt in ('-h', '--help'):
                self._usage()
            elif opt in ('-t', '--tsf'):
                tsf_file = os.path.abspath(arg)

                if not os.path.isfile(tsf_file):
                    print (f'Error: TSF file {tsf_file} could not be found')
                    sys.exit(1)

        return tsf_file

    def _classify_tsf_line(self, tsf_line):
        tsf_items = tsf_line.split(',')

        if len(tsf_items) != 5:
            return None, None

        core_id, tsf_cmd, symbol_name, class_name, object_id = tsf_items

        if (tsf_cmd != 'add_symbol_id'):
            return None, None
        else:
            return symbol_name, AweSymbol(core_id, symbol_name, class_name, object_id)

    def _parse_tsf_file(self):
        with open(self._tsf_file, 'r', encoding='utf-8') as tsf_file:
            while True:
                line = tsf_file.readline()

                if not line:
                    break

                key, value = self._classify_tsf_line(line.strip())

                if (key is not None) and (value is not None):
                    self.tsf_symbols[key] = value

    def keys(self):
        return self.tsf_symbols.keys()

    def has_key(self, key):
        if key in self.tsf_symbols:
            return True

        return False

    def get_symbol(self, key):
        if key in self.tsf_symbols:
            return self.tsf_symbols[key]

        return None

    def get_core_id(self, key):
        if key in self.tsf_symbols:
            return self.tsf_symbols[key].core_id

        return None

    def get_symbol_name(self, key):
        if key in self.tsf_symbols:
            return self.tsf_symbols[key].symbol_name

        return None

    def get_class_name(self, key):
        if key in self.tsf_symbols:
            return self.tsf_symbols[key].class_name

        return None

    def get_object_id(self, key):
        if key in self.tsf_symbols:
            return self.tsf_symbols[key].object_id

        return None
