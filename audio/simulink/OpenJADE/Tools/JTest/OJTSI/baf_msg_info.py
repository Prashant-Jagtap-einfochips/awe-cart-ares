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
from collections import OrderedDict
import struct
import sys

is_py2 = sys.version[0] == '2'

if not is_py2:
    import codecs

def decode_hex(h):
    if is_py2:
        return h.decode('hex')
    else:
        return codecs.decode(h, 'hex')
    
def hexbytes_to_hex32(hexbytes):
    return hexbytes[3]+hexbytes[2]+hexbytes[1]+hexbytes[0]

def hex_to_int32(h):
    return struct.unpack('>i', decode_hex(h))[0]
    
def hexbytes_to_uint32(hexbytes):
    hex32 = hexbytes[3]+hexbytes[2]+hexbytes[1]+hexbytes[0]
    return struct.unpack('>I', decode_hex(hex32))[0]

def hexbytes_to_float32(hexbytes):
    hex32 = hexbytes[3]+hexbytes[2]+hexbytes[1]+hexbytes[0]
    d = struct.unpack('>i', decode_hex(hex32))[0]
    return struct.unpack('<f', struct.pack('<i', d))[0]

class BAF_Error(object):
    def __init__(self, baf_error):
        self.id = baf_error.get('id')
        self.value = int(baf_error.get('Value'), 10)

    def __str__(self):
        mystr = '%s' % (self.id)
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
class BAF_Logger(object):
    def __init__(self, baf_logger):
        self.id = baf_logger.get('id')
        token = baf_logger.get('Token').upper()
        if (token.startswith('0X')) and (len(token) == 10):
            token = token[2:]
        else:
            return None
        self.token = token
        self.value = int(self.token, 16)

    def parse_payload(self, payload):
        count = len(payload)
        if self.id == 'LOG_TYPE_CAPTURE':
            idx = 0
            mystr = ''
            while idx < count:
                mystr = mystr + ' - %.3f\n' % (hexbytes_to_float32(payload[idx+0:idx+4]))
                idx = idx + 4
            return mystr
        elif (self.id == 'LOG_TYPE_PRINT') or (self.id == 'LOG_TYPE_EXEC'):
            idx = 0
            mystr = ''
            while idx < count:
                mystr = mystr + chr(int(payload[idx+0],16)) + chr(int(payload[idx+1],16))
                mystr = mystr + ' ' + payload[idx+2] + ' ' + payload[idx+3]
                mystr = mystr + ' ' + hexbytes_to_hex32(payload[idx+4:idx+8]) + '\n'
                idx = idx + 8
            return mystr
        return None
        
    def __str__(self):
        mystr = 'Logger ' + 'ID %s Token %s\n' % (self.id, self.token)
        return mystr
        
    def __repr__(self):
        return self.__str__()

class BAF_Signal(object):
    def __init__(self, baf_signal):
        self.id = baf_signal.get('id')
        self.bit_offset = int(baf_signal.get('bit_offset'), 10)
        self.bit_width = int(baf_signal.get('bit_width'), 10)

    def __str__(self):
        mystr = 'Signal ' + 'ID %s Bit_Offset %d Bit_Width %d\n' % (self.id, self.bit_offset, self.bit_width)
        return mystr
        
    def __repr__(self):
        return self.__str__()

class BAF_msg(object):
    def __init__(self, baf_msg):
        self.id = baf_msg.get('id')
        token = baf_msg.get('Token')
        token = token.upper()
        if (token.startswith('0X')) and (len(token) == 10):
            token = token[2:]
        else:
            return None
        self.token = token
        
        self.baf_signals = {}
        self.bit_offsets = []
        
        for baf_signal in baf_msg.findall('Signal'):
            bit_offset = baf_signal.get('bit_offset')
            bit_offset = int(bit_offset, 10)
            self.bit_offsets.append(bit_offset)
            self.baf_signals[bit_offset] = BAF_Signal(baf_signal)
            
        self.bit_offsets.sort()

    def get_signal(self, id):
        for bit_offset in self.bit_offsets:
            signal = self.baf_signals[bit_offset]
            if id == signal.id:
                return signal
                
    def construct_message(self, payloads, loggers):
        message = 0
        bit_pos = 0
        idx = 0
        
        while bit_pos < 32:
            baf_signal = self.baf_signals[bit_pos]
            name = baf_signal.id
            
            bit_offset = baf_signal.bit_offset
            bit_width = baf_signal.bit_width
            bit_mask = (1 << bit_width) - 1
            
            if name == 'MsgID':
                v = int(self.token, 16)
            elif name == 'Reserved':
                v = 0
            elif name == 'LoggerID':
                payload = payloads[idx]
                v = loggers[payload].value
            else:
                payload = payloads[idx]
                v = int(payload, 10)
                
            message = message | ((v & bit_mask) << bit_offset) 
            bit_pos = bit_pos + bit_width
            idx = idx + 1
        return ('%8.8X' % message)
            
    def deconstruct_message_dict(self, message, loggers):
        d = struct.unpack('>I', decode_hex(message))[0]
        payloads = OrderedDict()
        bit_pos = 0
        
        while bit_pos < 32:
            baf_signal = self.baf_signals[bit_pos]
            
            name = baf_signal.id
            bit_offset = baf_signal.bit_offset
            bit_width = baf_signal.bit_width
            bit_mask = (1 << bit_width) - 1
            v = (d >> bit_offset) & bit_mask
            item = None
            
            if name == 'MsgID':
                item = self.id
            elif name == 'Reserved':
                pass
            elif name == 'LoggerID':
                for k in loggers.keys():
                    if loggers[k].value == v:
                        item = loggers[k].id
            else:
                item = '%d' % v
            if item != None:
                payloads[name] = item
            bit_pos = bit_pos + bit_width
        
        return payloads
            
    def deconstruct_message(self, message, loggers):
        payloads = [v for k,v in self.deconstruct_message_dict(message, loggers).items()]
        return '[' + ' '.join(payloads) + ']'
        
    def __str__(self):
        mystr = 'Message ' + 'ID %s Token %s\n' % (self.id, self.token)
        for baf_signal in self.baf_signals:
            mystr = mystr + str(baf_signal)
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
class baf_msg_info(object):
    '''
    Python class storing BAF MSG ID
    '''
    def __init__(self, baf_msg_xml_filename):
        ''' Initialize data
        '''
        self.baf_msgs = {}
        self.baf_loggers = {}
        self.baf_errors = {}
        self._filename = baf_msg_xml_filename
        self._parse_xml_file()
        
    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for baf_msg in root.findall('MESSAGE'):
            name = baf_msg.get('id')
            self.baf_msgs[name] = BAF_msg(baf_msg)

        for baf_logger in root.findall('LOGGER'):
            name = baf_logger.get('id')
            self.baf_loggers[name] = BAF_Logger(baf_logger)
            
        for baf_error in root.findall('ERROR'):
            name = baf_error.get('id')
            self.baf_errors[name] = BAF_Error(baf_error)
            
    def __str__(self):
        for name in self.baf_msgs.keys():
            mystr = mystr + str(self.baf_msgs[name])
        return mystr
        
    def __repr__(self):
        return self.__str__()

    def get_msg_ids(self):
        return self.baf_msgs.keys()
        
    def get_token(self, id):
        if id in self.baf_msgs:
            return self.baf_msgs[id].token
        else:
            return None

    def get_id(self, token):
        token= '%8.8X' % (int(token, 16) & 0xFF) 
        for id in self.get_msg_ids():
            if token == self.get_token(id):
                return id
        return None

    def parse_payload(self, message, payload):
        id = self.get_id(message)
        if id != None:
            message_dict = self.baf_msgs[id].deconstruct_message_dict(message, self.baf_loggers)
            if message_dict['MsgID'] == 'SYS_LOG_PULL':
                logger_id = message_dict['LoggerID']
                count = int(message_dict['Count'], 10)
                if len(payload) != 8*count:
                    print ('Error: SYS_LOG_PULL length expected %d got %d\n' % ((8*count), len(payload)))
                else:
                    return self.baf_loggers[logger_id].parse_payload(payload)
        return None
    
    def construct_message(self, id, payload):
        items = [id]
        items.extend(payload)
        for name in self.baf_msgs.keys():
            if name == id:
                return self.baf_msgs[id].construct_message(items, self.baf_loggers)
        return None
        
    def deconstruct_message(self, message):
        id = self.get_id(message)
        if id != None:
            return self.baf_msgs[id].deconstruct_message(message, self.baf_loggers)
        return None
        
    def deconstruct_status(self, baf_status):
        baf_value = hex_to_int32(baf_status)
        if baf_value <= 0:
            for k,v in self.baf_errors.items():
                if v.value == baf_value:
                    return v.id
        else:
            return '%d' % baf_value
