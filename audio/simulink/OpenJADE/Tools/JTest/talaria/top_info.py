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

top_payloads = '''
<root version="1.0">
   <REQ id="TOP_WriteRequest">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="uint32_T" id="offset" width="1"/>
      <Signal charOffset="12" dtypeStr="uint32_T" id="byteCount" width="1"/>
      <Signal charOffset="16" dtypeStr="uint8_T" id="data" width="N"/>
   </REQ>
   <REQ id="TOP_ReadRequest">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="uint32_T" id="offset" width="1"/>
      <Signal charOffset="12" dtypeStr="uint32_T" id="byteCount" width="1"/>
   </REQ>
   <RSP id="TOP_WriteResponse">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="int32_T" id="statusCode" width="1"/>
   </RSP>
   <RSP id="TOP_ReadResponse">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="int32_T" id="statusCode" width="1"/>
      <Signal charOffset="12" dtypeStr="uint8_T" id="data" width="N"/>
   </RSP>
</root>
'''

class top_signal:
    def __init__(self, signal):
        self.index = signal.get('charOffset')
        self.name = signal.get('id')
        self.type = signal.get('dtypeStr')
        self.width = signal.get('width')

    def __str__(self):
        return "<Signal Index:%s Name:%s Type:%s Width:%s>" % (self.index, self.name, self.type, self.width)

    def __repr__(self):
        return self.__str__()

class top_REQ:
    def __init__(self, req):
        self.signals = []
        for signal in req.findall('Signal'):
            self.signals.append(top_signal(signal))
        self.signals.sort(key=lambda x: int(x.index))

    def __str__(self):
        mystr = '\tREQ\n'
        for signal in self.signals:
            mystr = mystr + '\t\t' + signal.__str__() + '\n'
        return mystr

    def __repr__(self):
        return self.__str__()

    def get_signals(self):
        return self.signals

class top_RSP:
    def __init__(self, rsp):
        self.signals = []
        for signal in rsp.findall('Signal'):
            self.signals.append(top_signal(signal))
        self.signals.sort(key=lambda x: int(x.index))

    def __str__(self):
        mystr = '\tRSP\n'
        for signal in self.signals:
            mystr = mystr + '\t\t' + signal.__str__() + '\n'
        return mystr

    def __repr__(self):
        return self.__str__()

    def get_signals(self):
        return self.signals

class top_info(object):
    '''
    Python class storing TOP data from a model
    '''
    def __init__(self, top_info_xml_filename):
        ''' Initialize data
        '''
        self.tops = {}
        self.reqs = {}
        self.rsps = {}
        self._filename = top_info_xml_filename
        self._parse_xml_file()
        self._parse_top_payloads()

    def _parse_top_payloads(self):
        ''' Parses top payloads
        '''
        root = ElementTree.fromstring(top_payloads)
        for req in root.findall('REQ'):
            name = req.get('id')
            self.reqs[name] = top_REQ(req)

        for rsp in root.findall('RSP'):
            name = rsp.get('id')
            self.rsps[name] = top_RSP(req)

    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for top in root.findall('TOP'):
            name = top.get('id')
            self.tops[name] = {}

            # Get the write token
            write_token = top.get('writeToken')
            write_token = write_token.upper()
            if (write_token.startswith('0X')) and (len(write_token) == 10):
                write_token = write_token[2:]
            else:
                write_token = None

            self.tops[name]['writeToken'] = write_token

            # Get the read token
            read_token = top.get('readToken')
            read_token = read_token.upper()
            if (read_token.startswith('0X')) and (len(read_token) == 10):
                read_token = read_token[2:]
            else:
                read_token = None
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
            print ( "%65s, %10s" % (name, token))

    def get_top_ids(self):
        return self.tops.keys()

    def get_top_id(self, token):
        token = token.upper()
        for top_id in self.get_top_ids():
            if token == self.tops[top_id]['readToken']:
                return top_id
            if token == self.tops[top_id]['writeToken']:
                return top_id
        return None

    def get_data_type(self, top_id):
        return self.tops[top_id]['dataType']

    def get_read_token(self, top_id):
        return self.tops[top_id]['readToken']

    def get_write_token(self, top_id):
        return self.tops[top_id]['writeToken']

    def get_req_signals(self, payload_id):
        return self.reqs[payload_id].get_signals()

    def get_rsp_signals(self, payload_id):
        return self.rsps[payload_id].get_signals()

