import time, os, threading
import sys
import getopt
import struct
from talaria.rtc_payloads import *
from talaria.top_info import *
from talaria.tsp_info import *
from talaria.translation_engine import *
from talaria.interpretation_engine import *
from OJTSI.baf_msg_info import *

is_py2 = sys.version[0] == '2'

if not is_py2:
    import codecs

__usage__ = """ \

Usage:
  ConvertResponse.py [-h] -m model_name -w work_folder -b baf_xml_file -i input_file -o output_file

Description:

Options:
  -h,   --help   Display usage information (this message)
  -m,   --model  Model name
  -w,   --work   Talaria work folder (Simulink Cache and Code Generation Folder) containing
                 * <Model>_InterpretationEngine(32/64)_win64.dll
                 * <Model>_TranslationEngine(32/64)_win64.dll
                 * <Model>_RTC_Payloads.xml
                 * <Model>_TOP_Tokens.xml
  -b,   --baf    Message_Id.xml file
  -i,   --in     Input hex string response file
  -o,   --out    Output human readable response file
"""
def decode_hex(h):
    if is_py2:
        return h.decode('hex')
    else:
        return codecs.decode(h, 'hex')

#
# https://docs.python.org/3/library/platform.html#cross-platform
#
def is_64bits():
    return (sys.maxsize > 2**32)
#
# https://stackoverflow.com/questions/23624212/how-to-convert-a-float-into-hex
#
def float_to_hex(f):
    mystr = hex(struct.unpack('<I', struct.pack('<f', f))[0])
    return mystr[2:].upper()

# def double_to_hex(f):
#    return hex(struct.unpack('<Q', struct.pack('<d', f))[0])

def hex_to_float(h):
    d = struct.unpack('>i', decode_hex(h))[0]
    f = struct.unpack('<f', struct.pack('<i', d))[0]
    return '%.9g' % f

def hex_to_int32(h):
    d = struct.unpack('>i', decode_hex(h))[0]
    return '%d' % d

def int32_to_hex(d):
    if d < 0:
        mystr = hex((d + (1 << 32)) % (1 << 32))
        mystr = mystr[2:len(mystr)-1].upper()
    else:
        mystr = '%8.8X' % d
        return mystr

def swap_endian_hex32(hex32_str):
    return hex32_str[6:8]+hex32_str[4:6]+hex32_str[2:4]+hex32_str[0:2]

def hexbytes_to_hex32(hexbytes):
    return hexbytes[3]+hexbytes[2]+hexbytes[1]+hexbytes[0]

class HexToPayload:
    def __init__(self, type, val):
        self.type = type
        self.val = val

    def __str__(self):
        if (self.type == 'uint32_T') or (self.type == 'uint32') or (self.type == 'uint32_t'):
            return '%d' % int(self.val, 16)
        elif (self.type == 'int32_T') or (self.type == 'int32') or (self.type == 'int32_t'):
            return hex_to_int32(self.val)
        elif (self.type == 'real32_T') or (self.type == 'single') or (self.type == 'float'):
            return hex_to_float(self.val)

    def __repr__(self):
        return self.__str__()

class HexBytesToPayload:
    def __init__(self, type, vals):
        self.type = type
        self.val = hexbytes_to_hex32(vals)

    def __str__(self):
        if (self.type == 'uint32_T') or (self.type == 'uint32') or (self.type == 'uint32_t'):
            return '%d' % int(self.val, 16)
        elif (self.type == 'int32_T') or (self.type == 'int32') or (self.type == 'int32_t'):
            return hex_to_int32(self.val)
        elif (self.type == 'real32_T') or (self.type == 'single') or (self.type == 'float'):
            return hex_to_float(self.val)

    def __repr__(self):
        return self.__str__()

class BAFRequest:
    def __init__(self, ojtsi, baf_token):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)

    def __str__(self):
        comment = '#REQ: baf_payload '
        mystr = self.baf_payload
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class RTCRequest:
    def __init__(self, ojtsi, baf_token, rtc_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.rtc_token = rtc_token
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(self.rtc_token)
        self.signals = self.ojtsi.rtc_payloads.get_req_signals(self.rtc_id)
        self.rtc_items = [x.strip() for x in payload]

        idx = 0
        for signal in self.signals:
            if signal.index != '0':
                cnt = 0
                while cnt < int(signal.width):
                    self.rtc_items[idx] = str(HexToPayload(signal.type, self.rtc_items[idx]))
                    idx = idx + 1
                    cnt = cnt + 1

    def __str__(self):
        comment = '#REQ: baf_payload'
        comment = comment + ' ' + 'rtc_id'
        for signal in self.signals:
            if signal.index != '0':
                comment = comment + ' ' + signal.name
        mystr = self.baf_payload
        mystr = mystr + ' ' + self.rtc_id
        mystr = mystr + ' ' + ' '.join(self.rtc_items)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TOPWriteRequest:
    def __init__(self, ojtsi, baf_token, top_write_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.top_write_token = top_write_token
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.top_id = self.ojtsi.top_info.get_top_id(self.top_write_token)
        self.datatype = self.ojtsi.top_info.get_data_type(self.top_id)

        self.top_items = [x.strip() for x in payload]
        self.caller_handle = str(HexToPayload('uint32_T', self.top_items[0]))
        self.offset = str(HexToPayload('uint32_T', self.top_items[1]))
        self.byte_count = str(HexToPayload('uint32_T', self.top_items[2]))
        self.data = [str(HexToPayload(self.datatype, x)) for x in self.top_items[3:]]

    def __str__(self):
        comment = '#REQ: baf_payload'
        comment = comment + ' ' + 'top_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byte_count'
        comment = comment + ' ' + 'data'

        mystr = self.baf_payload
        mystr = mystr + ' TOP_WRITE_' + self.top_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        mystr = mystr + ' ' + ' '.join(self.data)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TOPReadRequest:
    def __init__(self, ojtsi, baf_token, top_read_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.top_read_token = top_read_token
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.top_id = self.ojtsi.top_info.get_top_id(self.top_read_token)

        self.top_items = [x.strip() for x in payload]
        self.caller_handle = str(HexToPayload('uint32_T', self.top_items[0]))
        self.offset = str(HexToPayload('uint32_T', self.top_items[1]))
        self.byte_count = str(HexToPayload('uint32_T', self.top_items[2]))

    def __str__(self):
        comment = '#REQ: baf_payload'
        comment = comment + ' ' + 'top_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byte_count'

        mystr = self.baf_payload
        mystr = mystr + ' TOP_READ_' + self.top_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TSPReadRequest:
    def __init__(self, ojtsi, baf_token, tsp_read_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.tsp_read_token = tsp_read_token
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.tsp_id = self.ojtsi.tsp_info.get_tsp_id(self.tsp_read_token)

        self.tsp_items = [x.strip() for x in payload]
        self.caller_handle = str(HexToPayload('uint32_T', self.tsp_items[0]))
        self.offset = str(HexToPayload('uint32_T', self.tsp_items[1]))
        self.byte_count = str(HexToPayload('uint32_T', self.tsp_items[2]))

    def __str__(self):
        comment = '#REQ: baf_payload'
        comment = comment + ' ' + 'tsp_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byte_count'

        mystr = self.baf_payload
        mystr = mystr + ' TSP_READ_' + self.tsp_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class BAFResponse:
    def __init__(self, ojtsi, baf_token, baf_status, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        if payload == None:
            self.payload = None
        else:
            self.payload = self.ojtsi.baf_msg_info.parse_payload(self.baf_token, payload)

    def __str__(self):
        comment = '#RSP: baf_payload baf_status'
        mystr = self.baf_payload + ' '
        if self.baf_status != None:
            mystr = mystr + self.baf_status
        if self.payload != None:
            mystr = mystr + '\n' + self.payload
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class RTCResponse:
    def __init__(self, ojtsi, baf_token, baf_status, rtc_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.rtc_token = rtc_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(self.rtc_token)
        self.signals = self.ojtsi.rtc_payloads.get_rsp_signals(self.rtc_id)
        self.rtc_bytes = [x.strip() for x in payload]

        idx = 0
        self.rtc_items = []
        for signal in self.signals:
            if signal.index != '0':
                cnt = 0
                while cnt < int(signal.width):
                    self.rtc_items.append( str(HexBytesToPayload(signal.type, self.rtc_bytes[idx:idx+4])) )
                    idx = idx + 4
                    cnt = cnt + 1
                    if idx >= len(self.rtc_bytes):
                        return

    def __str__(self):
        comment = '#RSP: baf_payload baf_status'
        comment = comment + ' ' + 'rtc_id'
        for signal in self.signals:
            if signal.index != '0':
                comment = comment + ' ' + signal.name
        mystr = self.baf_payload + ' ' + self.baf_status
        mystr = mystr + ' ' + self.rtc_id
        mystr = mystr + ' ' + ' '.join(self.rtc_items)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class NTFResponse:
    def __init__(self, ojtsi, baf_token, baf_status, rtc_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.rtc_token = rtc_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(self.rtc_token)
        self.signals = self.ojtsi.rtc_payloads.get_ntf_signals(self.rtc_id)
        self.rtc_bytes = [x.strip() for x in payload]

        idx = 0
        self.rtc_items = []
        for signal in self.signals:
            if signal.index != '0':
                cnt = 0
                while cnt < int(signal.width):
                    self.rtc_items.append( str(HexBytesToPayload(signal.type, self.rtc_bytes[idx:idx+4])) )
                    idx = idx + 4
                    cnt = cnt + 1

    def __str__(self):
        comment = '#NTF: baf_payload baf_status'
        comment = comment + ' ' + 'rtc_id'
        for signal in self.signals:
            if signal.index != '0':
                comment = comment + ' ' + signal.name
        mystr = self.baf_payload + ' ' + self.baf_status
        mystr = mystr + ' ' + self.rtc_id
        mystr = mystr + ' ' + ' '.join(self.rtc_items)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TOPWriteResponse:
    def __init__(self, ojtsi, baf_token, baf_status, top_write_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.top_write_token = top_write_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.top_id = self.ojtsi.top_info.get_top_id(self.top_write_token)

        self.top_bytes = [x.strip() for x in payload]
        self.caller_handle = str(HexBytesToPayload('uint32_T', self.top_bytes[4*0:4*0+4]))
        self.statusCode    = str(HexBytesToPayload('int32_T',  self.top_bytes[4*1:4*1+4]))

    def __str__(self):
        comment = '#RSP: baf_payload baf_status'
        comment = comment + ' ' + 'top_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'statusCode'

        mystr = self.baf_payload + ' ' + self.baf_status
        mystr = mystr + ' TOP_WRITE_' + self.top_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.statusCode
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TOPReadResponse:
    def __init__(self, ojtsi, baf_token, baf_status, top_read_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.top_read_token = top_read_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.top_id = self.ojtsi.top_info.get_top_id(self.top_read_token)
        self.datatype = self.ojtsi.top_info.get_data_type(self.top_id)

        self.top_bytes = [x.strip() for x in payload]
        self.caller_handle = str(HexBytesToPayload('uint32_T', self.top_bytes[4*0:4*0+4]))
        self.statusCode    = str(HexBytesToPayload('int32_T',  self.top_bytes[4*1:4*1+4]))
        self.data_bytes = self.top_bytes[4*2:]
        idx = 0
        self.data_items = []
        while idx < len(self.data_bytes):
            self.data_items.append( str(HexBytesToPayload(self.datatype,  self.data_bytes[idx:idx+4])) )
            idx = idx + 4

    def __str__(self):
        comment = '#RSP: baf_payload baf_status'
        comment = comment + ' ' + 'top_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'statusCode'
        comment = comment + ' ' + 'data'

        mystr = self.baf_payload + ' ' + self.baf_status
        mystr = mystr + ' TOP_READ_' + self.top_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.statusCode
        mystr = mystr + ' ' + ' '.join(self.data_items)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class TSPReadResponse:
    def __init__(self, ojtsi, baf_token, baf_status, tsp_read_token, payload):
        self.ojtsi = ojtsi
        self.baf_token = baf_token
        self.tsp_read_token = tsp_read_token
        self.baf_status = self.ojtsi.baf_msg_info.deconstruct_status(baf_status)
        self.baf_payload = self.ojtsi.baf_msg_info.deconstruct_message(self.baf_token)
        self.tsp_id = self.ojtsi.tsp_info.get_tsp_id(self.tsp_read_token)
        self.datatype = self.ojtsi.tsp_info.get_data_type(self.tsp_id)

        self.tsp_bytes = [x.strip() for x in payload]
        self.caller_handle = str(HexBytesToPayload('uint32_T', self.tsp_bytes[4*0:4*0+4]))
        self.statusCode    = str(HexBytesToPayload('int32_T',  self.tsp_bytes[4*1:4*1+4]))
        self.charCount     = str(HexBytesToPayload('uint32_T', self.tsp_bytes[4*2:4*2+4]))
        self.data_bytes = self.tsp_bytes[4*3:]
        idx = 0
        self.data_items = []
        while idx < len(self.data_bytes):
            self.data_items.append( str(HexBytesToPayload(self.datatype,  self.data_bytes[idx:idx+4])) )
            idx = idx + 4

    def __str__(self):
        comment = '#RSP: baf_payload baf_status'
        comment = comment + ' ' + 'top_id'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'statusCode'
        comment = comment + ' ' + 'charCount'
        comment = comment + ' ' + 'data'

        mystr = self.baf_payload + ' ' + self.baf_status
        mystr = mystr + ' TSP_READ_' + self.tsp_id
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.statusCode
        mystr = mystr + ' ' + self.charCount
        mystr = mystr + ' ' + ' '.join(self.data_items)
        return (comment + '\n' + mystr + '\n')

    def __repr__(self):
        return self.__str__()

class OJTSI:
    def __init__(self, model_name):
        self.model_name = model_name

    def load_baf_xml(self, baf_xml):
        self.baf_msg_info = baf_msg_info(baf_xml)

    def load_rtcs(self, rtc_payloads_xml):
        self.rtc_payloads = rtc_payloads(rtc_payloads_xml)

    def load_tops(self, top_tokens_xml):
        self.top_info = top_info(top_tokens_xml)

    def load_tsps(self, tsp_tokens_xml):
        self.tsp_info = tsp_info(tsp_tokens_xml)

    def load_tie(self, tie_dll):
        self.tie = interpretation_engine(tie_dll)

    def load_tte(self, tte_dll):
        self.tte = translation_engine(tte_dll)

class ConvertResponse:
    def __init__(self, ojtsi, input_file, output_file):
        self.ojtsi = ojtsi
        self.input_file = input_file
        self.output_file = output_file
        self.linecount = 0

    def _isValidLine(self, line):
        if len(line) == 0:
            return False
        if line.startswith('#'):
            return False
        if line.startswith('REQ:'):
            return True
        if line.startswith('RSP:'):
            return True
        if line.startswith('NTF:'):
            return True
        return False

    def _classify(self, line):
        if (self._isValidLine(line) == False):
            return None

        self.linecount = self.linecount + 1
        #print '%d %s' % (self.linecount, line)
        id, data = [x.strip() for x in line.split(':')]
        if (id == None) or (data == None):
            return None
        items = [x.strip() for x in data.split()]
        if id == 'REQ':
            if len(items) == 1:
                return BAFRequest(self.ojtsi, items[0])

            if len(items) > 2:
                rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(items[1])
                if rtc_id != None:
                    return RTCRequest(self.ojtsi, items[0], items[1], items[2:])

                top_id = self.ojtsi.top_info.get_top_id(items[1])
                if top_id != None:
                    write_token = self.ojtsi.top_info.get_write_token(top_id)
                    if write_token == items[1]:
                        return TOPWriteRequest(self.ojtsi, items[0], items[1], items[2:])
                    read_token = self.ojtsi.top_info.get_read_token(top_id)
                    if read_token == items[1]:
                        return TOPReadRequest(self.ojtsi, items[0], items[1], items[2:])

                tsp_id = self.ojtsi.tsp_info.get_tsp_id(items[1])
                if tsp_id != None:
                    read_token = self.ojtsi.tsp_info.get_read_token(tsp_id)
                    if read_token == items[1]:
                        return TSPReadRequest(self.ojtsi, items[0], items[1], items[2:])

        if id == 'RSP':
            baf_token = int(items[0], 16) & 0xFF
            sys_start_token = int(self.ojtsi.baf_msg_info.get_token('SYS_START'), 16) &  0xFF
            if baf_token >= sys_start_token:
                baf_id = self.ojtsi.baf_msg_info.get_id(items[0])
                if baf_id != None:
                    return BAFResponse(self.ojtsi, items[0], items[1], items[2:])
            if len(items) > 6:
                token = hexbytes_to_hex32(items[2:6])

                rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(token)
                if rtc_id != None:
                    return RTCResponse(self.ojtsi, items[0], items[1], token, items[6:])

                top_id = self.ojtsi.top_info.get_top_id(token)
                if top_id != None:
                    write_token = self.ojtsi.top_info.get_write_token(top_id)
                    if write_token == token:
                        return TOPWriteResponse(self.ojtsi, items[0], items[1], token, items[6:])
                    read_token = self.ojtsi.top_info.get_read_token(top_id)
                    if read_token == token:
                        return TOPReadResponse(self.ojtsi, items[0], items[1], token, items[6:])

                tsp_id = self.ojtsi.tsp_info.get_tsp_id(token)
                if tsp_id != None:
                    read_token = self.ojtsi.tsp_info.get_read_token(tsp_id)
                    if read_token == token:
                        return TSPReadResponse(self.ojtsi, items[0], items[1], token, items[6:])

            if len(items) == 2:
                baf_id = self.ojtsi.baf_msg_info.get_id(items[0])
                if baf_id != None:
                    return BAFResponse(self.ojtsi, items[0], items[1], None)


        if id == 'NTF':
            if len(items) > 6:
                token = hexbytes_to_hex32(items[2:6])

                rtc_id = self.ojtsi.rtc_payloads.get_rtc_id(token)
                if rtc_id != None:
                    return NTFResponse(self.ojtsi, items[0], items[1], token, items[6:])

    def run(self):
        with open(self.input_file, 'r') as in_fp:
            with open(self.output_file, 'w') as out_fp:
                line = in_fp.readline()
                while line:
                    out_obj = self._classify( line.strip() )
                    if out_obj != None:
                        out_fp.write('#' + line)
                        out_fp.write(str(out_obj) + '\n')
                    line = in_fp.readline()

#*********************************************************************************
# scan_work_folder(work_folder)
#*********************************************************************************
def scan_work_folder(ojtsi, work_folder):
    model_name = ojtsi.model_name

    rtc_payloads_xml = os.path.join(work_folder, model_name + '_RTC_Payloads.xml')
    top_tokens_xml = os.path.join(work_folder, model_name + '_TOP_Tokens.xml')
    if is_64bits():
        tie_dll = os.path.join(work_folder, model_name + '_InterpretationEngine64_win64.dll')
        tte_dll = os.path.join(work_folder, model_name + '_TranslationEngine64_win64.dll')
    else:
        tie_dll = os.path.join(work_folder, model_name + '_InterpretationEngine32_win64.dll')
        tte_dll = os.path.join(work_folder, model_name + '_TranslationEngine32_win64.dll')

    if not os.path.isfile(rtc_payloads_xml):
        print ('Error: <Model>_RTC_Payloads.xml ' + rtc_payloads_xml + ' could not be found')
        return False

    if not os.path.isfile(top_tokens_xml):
        print ('Error: <Model>_TOP_Tokens.xml ' + top_tokens_xml + ' could not be found')
        return False

    if not os.path.isfile(tsp_tokens_xml):
        print ('Error: <Model>_TSP_Tokens.xml ' + tsp_tokens_xml + ' could not be found')
        return False

    if not os.path.isfile(tie_dll):
        print ('Error: <Model>_InterpretationEngine(32/64)_win64.dll ' + tie_dll + ' could not be found')
        return False

    if not os.path.isfile(tte_dll):
        print ('Error: <Model>_TranslationEngine(32/64)_win64.dll ' + tte_dll + ' could not be found')
        return False

    ojtsi.load_rtcs(rtc_payloads_xml)
    ojtsi.load_tops(top_tokens_xml)
    ojtsi.load_tsps(tsp_tokens_xml)
    ojtsi.load_tie(tie_dll)
    ojtsi.load_tte(tte_dll)
    return True


#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    model_name = None
    input_file = None
    output_file = None
    work_folder = None
    baf_xml = None

    try:
        opts, args = getopt.getopt(argv, 'hm:w:b:i:o:', ['help','model=','work=','baf=','in=','out='])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-m', '--model'):
            model_name = arg
        elif opt in ('-w', '--work'):
            work_folder = os.path.abspath(arg)
            if not os.path.isdir(work_folder):
                print ('Error: work folder ' + work_folder + ' could not be found')
                sys.exit()
        elif opt in ('-b', '--baf'):
            baf_xml = os.path.abspath(arg)
            if not os.path.isfile(baf_xml):
                print ('Error: Message_Id.xml file ' + baf_xml + ' could not be found')
                sys.exit()
        elif opt in ('-i', '--in'):
            input_file = os.path.abspath(arg)
            if not os.path.isfile(input_file):
                print ('Error: config file ' + input_file + ' could not be found')
                sys.exit()
        elif opt in ('-o', '--out'):
            output_file = os.path.abspath(arg)
            control_file_path = os.path.dirname(output_file)
            if not os.path.isdir(control_file_path):
                print ('Error: ' + control_file_path + ' could not be found')
                sys.exit()

    if (model_name == None) or (input_file == None) or (output_file == None) or (baf_xml == None):
        usage()
        sys.exit()

    ojtsi = OJTSI(model_name)

    ojtsi.load_baf_xml(baf_xml)

    if (work_folder == None):
        print ('Warning: Talaria work folder unspecified. Only BAF commands will be converted')
    else:
        if scan_work_folder(ojtsi, work_folder) == False:
            sys.exit()

    return ConvertResponse(ojtsi, input_file, output_file)

def DecompileResponse (model_name, work_folder, baf_xml, input_file, output_file) :
    ojtsi = OJTSI(model_name)
    ojtsi.load_baf_xml(baf_xml)

    if scan_work_folder(ojtsi, work_folder) == False:
        sys.exit()

    ojtsi.load_corrections_xml(corrections_xml=None)

    conv = ConvertResponse(ojtsi=ojtsi, input_file=input_file, output_file=output_file)
    conv.run()

def DecompileResponse2 (baf_xml, rtc_payloads_xml=None, top_tokens_xml=None, tsp_tokens_xml=None, tie_dll=None, tte_dll=None, input_file=None, output_file=None) :
    ojtsi = OJTSI('Model')
    if not os.path.isfile(baf_xml):
        print ( 'Error: Message.xml ' + baf_xml + ' could not be found' )
        sys.exit()

    if (rtc_payloads_xml != None) and (not os.path.isfile(rtc_payloads_xml)):
        print ( 'Warning: <Model>_RTC_Payloads.xml ' + rtc_payloads_xml + ' could not be found')
        rtc_payloads_xml = None

    if (top_tokens_xml != None) and (not os.path.isfile(top_tokens_xml)):
        print ( 'Warning: <Model>_TOP_Tokens.xml ' + top_tokens_xml + ' could not be found')
        top_tokens_xml = None

    if (tsp_tokens_xml != None) and (not os.path.isfile(tsp_tokens_xml)):
        print ( 'Warning: <Model>_TSP_Tokens.xml ' + tsp_tokens_xml + ' could not be found')
        tsp_tokens_xml = None

    if (tie_dll != None) and (not os.path.isfile(tie_dll)):
        print ( 'Warning: <Model>_InterpretationEngine(32/64)_win64.dll ' + tie_dll + ' could not be found')
        tie_dll = None

    if (tte_dll != None) and (not os.path.isfile(tte_dll)):
        print ( 'Warning: <Model>_TranslationEngine(32/64)_win64.dll ' + tte_dll + ' could not be found')
        tte_dll = None

    ojtsi.load_baf_xml(baf_xml)
    if rtc_payloads_xml != None:
        ojtsi.load_rtcs(rtc_payloads_xml)
    if top_tokens_xml != None:
        ojtsi.load_tops(top_tokens_xml)
    if tsp_tokens_xml != None:
        ojtsi.load_tsps(tsp_tokens_xml)
    if tie_dll != None:
        ojtsi.load_tie(tie_dll)
    if tte_dll != None:
        ojtsi.load_tte(tte_dll)

    conv = ConvertResponse(ojtsi=ojtsi, input_file=input_file, output_file=output_file)
    conv.run()
#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    ojtsi_convert = parse_args(argv)
    ojtsi_convert.run()

#*********************************************************************************
if __name__ == '__main__':
    main()
