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

tsp_payloads = '''
<root version="1.0">
   <REQ id="TSP_ReadRequest">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="uint32_T" id="offset" width="1"/>
      <Signal charOffset="12" dtypeStr="uint32_T" id="byteCount" width="1"/>
   </REQ>
   <REQ id="TSP_ReadResponse">
      <Signal charOffset="0" dtypeStr="uint32_T" id="msgId" width="1"/>
      <Signal charOffset="4" dtypeStr="uint32_T" id="caller_handle" width="1"/>
      <Signal charOffset="8" dtypeStr="int32_T" id="statusCode" width="1"/>
      <Signal charOffset="12" dtypeStr="uint32_T" id="byteCount" width="1"/>
      <Signal charOffset="16" dtypeStr="uint8_T" id="data" width="N"/>
   </REQ>
</root>
'''

class tsp_signal:
    def __init__(self, signal):
        self.index = signal.get('charOffset')
        self.name = signal.get('id')
        self.type = signal.get('dtypeStr')
        self.width = signal.get('width')

    def __str__(self):
        return "<Signal Index:%s Name:%s Type:%s Width:%s>" % (self.index, self.name, self.type, self.width)

    def __repr__(self):
        return self.__str__()

class tsp_REQ:
    def __init__(self, req):
        self.signals = []
        for signal in req.findall('Signal'):
            self.signals.append(tsp_signal(signal))
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

class tsp_RSP:
    def __init__(self, rsp):
        self.signals = []
        for signal in rsp.findall('Signal'):
            self.signals.append(tsp_signal(signal))
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

class tsp_info(object):
    '''
    Python class storing TSP data from a model
    '''
    def __init__(self, tsp_info_xml_filename):
        ''' Initialize data
        '''
        self.tsps = {}
        self.reqs = {}
        self.rsps = {}
        self._filename = tsp_info_xml_filename
        self._parse_xml_file()
        self._parse_tsp_payloads()

    def _parse_tsp_payloads(self):
        ''' Parses tsp payloads
        '''
        root = ElementTree.fromstring(tsp_payloads)
        for req in root.findall('REQ'):
            name = req.get('id')
            self.reqs[name] = tsp_REQ(req)

        for rsp in root.findall('RSP'):
            name = rsp.get('id')
            self.rsps[name] = tsp_RSP(req)

    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for tsp in root.findall('TSP'):
            name = tsp.get('id')
            self.tsps[name] = {}

            # Get the read token
            read_token = tsp.get('blindReadToken')
            read_token = read_token.upper()
            if (read_token.startswith('0X')) and (len(read_token) == 10):
                read_token = read_token[2:]
            else:
                read_token = None
            self.tsps[name]['readToken'] = read_token

            if 'width' in tsp.attrib:
                width = tsp.get('width')
                self.tsps[name]['width'] = width
            else:
                self.tsps[name]['width'] = None

            if 'dataType' in tsp.attrib:
                data_type = tsp.get('dataType')
                self.tsps[name]['dataType'] = data_type
            else:
                self.tsps[name]['dataType'] = None


    def print_tsp_info(self):
        for name,token in self.tsps.items():
            print ( "%65s, %10s" % (name, token))

    def get_tsp_ids(self):
        return self.tsps.keys()

    def get_tsp_id(self, token):
        token = token.upper()
        for tsp_id in self.get_tsp_ids():
            if token == self.tsps[tsp_id]['readToken']:
                return tsp_id
        return None

    def get_data_type(self, tsp_id):
        return self.tsps[tsp_id]['dataType']

    def get_read_token(self, tsp_id):
        return self.tsps[tsp_id]['readToken']

    def get_req_signals(self, payload_id):
        return self.reqs[payload_id].get_signals()

    def get_rsp_signals(self, payload_id):
        return self.rsps[payload_id].get_signals()

    def get_total_char_count(self, tsp_id):
        datatype = self.tsps[tsp_id]['dataType']
        width = int ( self.tsps[tsp_id]['width'] )

        if ( (datatype == 'uint32_T') or (datatype == 'uint32') or (datatype == 'uint32_t') \
             or (datatype == 'int32_T') or (datatype == 'int32') or (datatype == 'int32_t') \
             or (datatype == 'real32_T') or (datatype == 'single') or (datatype == 'float') ):
            return 4*width
        elif ( (datatype == 'uint16_T') or (datatype == 'uint16') or (datatype == 'uint16_t') \
             or (datatype == 'int16_T') or (datatype == 'int16') or (datatype == 'int16_t') ):
            return 2*width
        elif ( (datatype == 'uint8_T') or (datatype == 'uint8') or (datatype == 'uint8_t') \
             or (datatype == 'int8_T') or (datatype == 'int8') or (datatype == 'int8_t') ):
            return width
        else:
            return None


