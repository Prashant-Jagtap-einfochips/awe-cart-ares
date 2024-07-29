#!/usr/bin/env python
###############################################################################
#
# BOSE CORPORATION
# COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
# This program may not be reproduced, in whole or in part in any
# form or any means whatsoever without the written permission of:
# BOSE CORPORATION
# The Mountain
# Framingham, MA 01701-9168
#
################################################################################

import xml.etree.ElementTree as ElementTree

class TOPInfo(object):
    '''
    Python class storing TOP data from a model
    '''
    def __init__(self, top_info_xml_filename):
        ''' Initialize data
        '''
        self.tops = {}
        self._filename = top_info_xml_filename
        self._parse_xml_file()
        
    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for top in root.findall('TOP'):
            # Create the top dictionary. Here is the format
            #   {name: {writeToken: value,
            #           readToken: value,
            #           ... }
            name = top.get('id')
            self.tops[name] = {}

            # Get the write token
            write_token = top.get('writeToken')
            self.tops[name]['writeToken'] = write_token

            # Get the read token
            read_token = top.get('readToken')
            self.tops[name]['readToken'] = read_token

            # Other fields may not be present based on addressing mode, so treat as optional

            if 'width' in top.attrib:
                width = top.get('width')
                self.tops[name]['width'] = width

            if 'dataType' in top.attrib:
                data_type = top.get('dataType')
                self.tops[name]['dataType'] = data_type


    def print_top_info(self):
        for name,token in self.tops.items():
            print "%65s, %10s" % (name, token)
