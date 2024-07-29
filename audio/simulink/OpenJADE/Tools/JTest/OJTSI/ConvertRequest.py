import time, os, threading
import math
import sys
import getopt
import struct
from talaria.rtc_payloads import *
from talaria.top_info import *
from talaria.tsp_info import *
from talaria.translation_engine import *
from talaria.interpretation_engine import *
from OJTSI.baf_msg_info import *
import xml.etree.ElementTree as ElementTree

__usage__ = """ \

Usage:
  ConvertRequest.py [-h] -m model_name -w work_folder -b baf_xml_file -c corrections_xml
                    -f frame_size -s sample_rate
                    -i input_file -o output_file

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
  -t,   --base_time_step Base Time Step: pass as string e.g. '(32/48000)/12'
  -c,   --corrections <Model>_RTC_Corrections.xml file
  -f,   --frame_size  Frame Size  (default 32)
  -s,   --sample_rate Sample Rate (default 48000)
  -i,   --in     Input human readable request file
  -o,   --out    Output hex string request file
"""


#/** @defgroup ASDID Message IDs
# *  _____  _               _                 _     _                   _
# * |  __ \(_)             | |       /\      | |   | |                 (_)
# * | |  | |_ _ __ ___  ___| |_     /  \   __| | __| |_ __ ___  ___ ___ _ _ __   __ _
# * | |  | | | '__/ _ \/ __| __|   / /\ \ / _` |/ _` | '__/ _ \/ __/ __| | '_ \ / _` |
# * | |__| | | | |  __/ (__| |_   / ____ \ (_| | (_| | | |  __/\__ \__ \ | | | | (_| |
# * |_____/|_|_|  \___|\___|\__| /_/    \_\__,_|\__,_|_|  \___||___/___/_|_| |_|\__, |
# *                                                                              __/ |
# *                                                                             |___/
# * ASDID Message ID High Level Parsing
# * =========================
# * The top 8 bits of RTC and TOP message IDs share a common format "ASDID"
# * The ASDID_... macros provide quick parsing and decoding of this format:
# *
# *   Shared ASDID Message ID Layout:
#   ~~~{.c}
# *     1+3 bits+ 4 bits +                 +                                   +
# *    31|30 28 | 27  24 |        +        |        +        +        +        |
# *    +-+------------------------+--------+--------+--------+--------+--------+
# *    |R| core | group  |                                                     |
# *    +-+------+--------+-----------------------------------------------------+
# *
# *    R      : 1 bit value "reserved" (usage varies based on group)
# *
# *    core   : 3 bit value, ASDID_CORE
# *             [0-7] = Index of DSP core that handles this message
# *
# *    group  : 4 bit value, ASDID_GROUP
# *             [0]    = RTC message (Control messaging )
# *             [1-15] = TOP message (Room mode data for a feature)
# *
# #define ASDID_CORE(id)                 (((id) >> 28) & 0x7UL)    // processor core
#

def ASDID_CORE(token_str):
    token =  int(token_str, base=16)
    core_id = (((token) >> 28) & 7)
    return str(core_id)

#
# https://docs.python.org/3/library/platform.html#cross-platform
#
def is_64bits():
    return (sys.maxsize > 2**32)
#
# https://stackoverflow.com/questions/23624212/how-to-convert-a-float-into-hex
#
def float_to_hex(f):
    d = struct.unpack('<I', struct.pack('<f', f))[0]
    mystr = '%8.8X' % d
    return mystr

# def double_to_hex(f):
#    return hex(struct.unpack('<Q', struct.pack('<d', f))[0])

# def hex_to_int32(h):
#    return struct.unpack('>i', h.decode('hex'))[0]

def int32_to_hex(d):
    if d < 0:
        mystr = hex((d + (1 << 32)) % (1 << 32))
        mystr = mystr[2:len(mystr)-1].upper()
    else:
        mystr = '%8.8X' % d
        return mystr

def swap_endian_hex32(hex32_str):
    return hex32_str[6:8]+hex32_str[4:6]+hex32_str[2:4]+hex32_str[0:2]

class PayloadToHex:
    def __init__(self, type, val):
        self.type = type
        self.val = val

    def __str__(self):
        if (self.type == 'uint32_T') or (self.type == 'uint32') or (self.type == 'uint32_t'):
            return '%8.8X' % int(self.val)
        elif (self.type == 'int32_T') or (self.type == 'int32') or (self.type == 'int32_t'):
            return int32_to_hex( int(self.val) )
        elif (self.type == 'real32_T') or (self.type == 'single') or (self.type == 'float'):
            return float_to_hex( float(self.val) )

    def __repr__(self):
        return self.__str__()

class ConfigLine(object):
    def __init__(self, ojtsi, baf_id, baf_payload, timestamp, payload):
        self.baf_id = baf_id
        self.baf_payload = baf_payload
        self.timestamp = timestamp
        self.payload = payload
        if baf_payload == None:
            self.baf_token = ojtsi.baf_msg_info.get_token(baf_id)
        else:
            self.baf_token = ojtsi.baf_msg_info.construct_message(baf_id, baf_payload)

    def get_baf_token(self):
        return self.baf_token

    def get_payload(self):
        return self.payload

    def get_baf_payload(self):
        return self.baf_payload

    def get_timestamp(self):
        return self.timestamp

class BAFLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, timestamp, payload):
        super(BAFLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)

    def get_control_line(self):
        comment = '# ' + self.baf_id + ': time '
        comment = comment + ' ' + 'baf_token'
        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])
        return (comment + '\n' + mystr + '\n')

class RAWLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, timestamp, payload):
        super(RAWLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)
        self.raw_items = payload.lstrip('[').rstrip(']').split()

    def get_control_line(self):
        comment = '# ' + self.baf_id + ': time '
        comment = comment + ' ' + 'baf_token'
        comment = comment + ' ' + 'raw_items'
        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = mystr + ' ' + ' '.join(self.raw_items)
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])
        return (comment + '\n' + mystr + '\n')

class RTCLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, rtc_id, timestamp, payload):
        super(RTCLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)
        self.rtc_id = rtc_id
        self.signals = ojtsi.rtc_payloads.get_req_signals(rtc_id)

        self.rtc_token = ojtsi.rtc_payloads.get_rtc_token(rtc_id)
        self.rtc_items = payload.lstrip('[').rstrip(']').split()

        idx = 0
        for signal in self.signals:
            if signal.index != '0':
                cnt = 0
                while cnt < int(signal.width):
                    st = PayloadToHex(signal.type, self.rtc_items[idx])
                    self.rtc_items[idx] = str(st)
                    idx = idx + 1
                    cnt = cnt + 1

    def get_control_line(self):
        comment = '# ' + self.rtc_id + ': time '
        comment = comment + ' ' + 'baf_token'
        comment = comment + ' ' + 'rtc_token'
        for signal in self.signals:
            if signal.index != '0':
                comment = comment + ' ' + signal.name
        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = mystr + ' ' + self.rtc_token
        mystr = mystr + ' ' + ' '.join(self.rtc_items)
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])
        return (comment + '\n' + mystr + '\n')

class TOPWriteLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, top_write_id, timestamp, datatype, payload):
        super(TOPWriteLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)
        self.top_write_id = top_write_id

        self.top_write_token = ojtsi.top_info.get_write_token(top_write_id)

        self.datatype = datatype

        self.caller_handle = str(PayloadToHex('uint32', payload[0]))
        self.offset = str(PayloadToHex('uint32', payload[1]))
        self.byte_count = str(PayloadToHex('uint32', payload[2]))
        self.data = [str(PayloadToHex(datatype, x.strip())) for x in payload[3:]]

    def get_control_line(self):
        comment = '# ' + self.top_write_id + ': time '
        comment = comment + ' ' + 'baf_token'
        comment = comment + ' ' + 'top_write_token'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byteCount'
        comment = comment + ' ' + 'data'

        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = mystr + ' ' + self.top_write_token
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])
        if (self.datatype == 'int32') or (self.datatype == 'uint32') or (self.datatype == 'single'):
            datastr = ' '.join([swap_endian_hex32(x) for x in self.data])
        else:
            print ('Error: TOPWriteLine unknown data type ' + datatype)

        return (comment + '\n' + mystr + ' ' + datastr + '\n')

class TOPReadLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, top_read_id, timestamp, datatype, payload):
        super(TOPReadLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)
        self.top_read_id = top_read_id

        self.top_read_token = ojtsi.top_info.get_read_token(top_read_id)

        self.datatype = datatype

        self.caller_handle = str(PayloadToHex('uint32', payload[0]))
        self.offset = str(PayloadToHex('uint32', payload[1]))
        self.byte_count = str(PayloadToHex('uint32', payload[2]))

    def get_control_line(self):
        comment = '# ' + self.top_read_id + ': time '
        comment = comment + ' ' + 'baf_token'
        comment = comment + ' ' + 'top_read_token'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byteCount'

        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = mystr + ' ' + self.top_read_token
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])

        return (comment + '\n' + mystr + '\n')

class TSPLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, tsp_id, timestamp, datatype, payload):
        super(TSPLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)
        self.tsp_id = tsp_id

        self.tsp_token = ojtsi.tsp_info.get_read_token(tsp_id)

        self.datatype = datatype

        caller_handle = payload[0]
        char_offset = payload[1] if (len(payload) > 1) and (payload[1] != '\x00') else '0'
        char_count = payload[2] if (len(payload) > 2) and (payload[2] != '\x00') else str(ojtsi.tsp_info.get_total_char_count(tsp_id))

        self.caller_handle = str(PayloadToHex('uint32', caller_handle))
        self.offset = str(PayloadToHex('uint32', char_offset))
        self.byte_count = str(PayloadToHex('uint32', char_count))

    def get_control_line(self):
        comment = '# ' + self.tsp_id + ': time '
        comment = comment + ' ' + 'baf_token'
        comment = comment + ' ' + 'tsp_token'
        comment = comment + ' ' + 'caller_handle'
        comment = comment + ' ' + 'offset'
        comment = comment + ' ' + 'byteCount'

        mystr =  '%8.8X' % int(self.timestamp)
        mystr = mystr + ' ' + self.baf_token
        mystr = mystr + ' ' + self.tsp_token
        mystr = mystr + ' ' + self.caller_handle
        mystr = mystr + ' ' + self.offset
        mystr = mystr + ' ' + self.byte_count
        mystr = ' '.join([swap_endian_hex32(x) for x in mystr.split()])

        return (comment + '\n' + mystr + '\n')

class rtc_corrections:
    def __init__(self, corrections_xml=None):
        self.corrections = {}
        if corrections_xml != None:
            tree = ElementTree.parse(corrections_xml)
            root = tree.getroot()
            for rtc in root.findall('RTC'):
                self.corrections[rtc.get('id')] = rtc.find('Correction').text

    def getCorrection(self, rtc_id):
        if rtc_id in self.corrections.keys():
            correction_text = self.corrections[rtc_id].strip()
            if correction_text == '1':
                return 1
        return 0

    def __str__(self):
        mystr = ''
        for x in self.corrections.keys():
            mystr = mystr + ' ' + '<RTC ID:%s Correction:%s>' % (x, self.corrections[x])
        return mystr

    def __repr__(self):
        return self.__str__()

class TuneVarLine(ConfigLine):
    def __init__(self, ojtsi, baf_id, baf_payload, tunevar_id, timestamp, caller_handle, payload):
        super(TuneVarLine,self).__init__(ojtsi, baf_id, baf_payload, timestamp, payload)

        self.tunevar_id = tunevar_id
        self.timestamp = timestamp
        self.caller_handle = caller_handle
        self.payload = payload

class TuneVarGroup(object):
    def __init__(self, ojtsi, timestamp, lines, filter_core=None):
        self.ojtsi = ojtsi
        self.lines = lines
        self.filter_core = filter_core

        self.top_lines = []
        self.tops = []

        ojtsi.tte.Reset()
        for line in lines:
            ojtsi.tte.SetTuneVar(line.tunevar_id, line.payload)
            baf_id = line.baf_id
            baf_payload = line.baf_payload
        tops = ojtsi.tte.RunTranslations()
        if ojtsi.tte.GetTranslationErrorState() == 0:
            if len(tops) > 0:
                caller_handle = int(self.lines[0].caller_handle)
            for top in tops:
                items = top.split()
                top_write_id = items[0]
                datatype = items[1]
                if self.filter_core == None:
                    self.tops.append(top)
                else:
                    top_write_line = TOPWriteLine(ojtsi, baf_id, baf_payload, top_write_id, timestamp, datatype, items[2:])
                    if ASDID_CORE(top_write_line.top_write_token) == self.filter_core:
                        self.tops.append(top)
                    else:
                        continue

                if (datatype == 'int32') or (datatype == 'uint32') or (datatype == 'single'):
                    max_items = 20
                    total = len(items[2:])
                    idx = 2
                    if total <= max_items:
                        curr_cnt = total
                    else:
                        curr_cnt = max_items
                    offset = 0
                    while curr_cnt > 0:
                        byteCount = curr_cnt * 4

                        payload = [ str(caller_handle), str(offset), str(byteCount)]
                        payload.extend( items[idx:idx+curr_cnt] )
                        self.top_lines.append(TOPWriteLine(ojtsi, baf_id, baf_payload, top_write_id, timestamp, datatype, payload))

                        caller_handle = caller_handle + 1
                        offset = offset + byteCount
                        idx = idx + curr_cnt
                        total = total - curr_cnt
                        if total <= max_items:
                            curr_cnt = total
                        else:
                            curr_cnt = max_items
                else:
                    print ('Error: TuneVarGroup unknown data type ' + datatype)
                    continue


    def get_control_lines(self):
        mystr = ''
        for top in self.tops:
            if len(top) > 240:
                top = top[:240] + ' ...'
            comment = '# ' + top + '\n'
            mystr = mystr + comment
        for top_line in self.top_lines:
            mystr = mystr + top_line.get_control_line() + '\n'
        return mystr

class OJTSI:
    def __init__(self, model_name):
        self.model_name = model_name

    def load_baf_xml(self, baf_xml):
        self.baf_msg_info = baf_msg_info(baf_xml)

    def load_corrections_xml(self, corrections_xml):
        self.rtc_corrections_info = rtc_corrections(corrections_xml)

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


class ConvertRequest:
    def __init__(self, ojtsi, input_file, output_file, frame_size=None, sample_rate=None, base_time_step=None, filter_core=None):
        self.ojtsi = ojtsi
        self.input_file = input_file
        self.output_file = output_file
        self.filter_core = filter_core

        self.frame_size = None
        self.sample_rate = None
        self.base_time_step = None
        if base_time_step == None:
            if frame_size is None:
                self.frame_size = 32
            else:
                self.frame_size = frame_size
            if sample_rate is None:
                self.sample_rate = 48000
            else:
                self.sample_rate = sample_rate
            print ('Info: using frame_size %d sample_rate %d Hz' % (self.frame_size, self.sample_rate))
        else:
            if isinstance(base_time_step, float):
                self.base_time_step = base_time_step
            else:
                self.base_time_step = eval(base_time_step)
            print ('Info: using base_time_step %1.16e sec' % (self.base_time_step))

    def convertTime2Frame(self, time, adjust=0):
        if self.base_time_step == None:
            return str(int(round((float(time)*self.sample_rate/self.frame_size)-0.01)) + adjust)
        else:
            return str(int(round((float(time)/self.base_time_step)-0.01)) + adjust)

    def run(self):
        BAF_lines = {}
        RAW_lines = {}
        RTC_lines = {}
        TOPRead_lines = {}
        TOPWrite_lines = {}
        TuneVar_lines = {}
        TSP_lines = {}

        timestamps = []
        with open(self.input_file, 'r') as fp:
            line = fp.readline()
            while line:
                line = line.strip()
                if (len(line) != 0) and (not line.startswith('#')):
                    items = [x.strip() for x in line.split(',')]
                    timestamp = None
                    if items[0].startswith('['):
                        baf_items = [x.strip() for x in items[0].lstrip('[').rstrip(']').split()]
                        baf_id = baf_items[0]
                        baf_payload = baf_items[1:]
                    else:
                        baf_id = items[0]
                        baf_payload = None

                    if items[1] == 'registerBAF':
                        timestamp = self.convertTime2Frame(items[2])
                        payload = None
                        if timestamp not in BAF_lines:
                            BAF_lines[timestamp] = []
                        if baf_id == 'SYS_NUM_FRAMES':
                            baf_payload[0] = self.convertTime2Frame(baf_payload[0],0)
                        if baf_id == 'SYS_LOG_PULL':
                            total_request = int(baf_payload[1])
                            max_per_request = 32
                            running_total = 0
                            while running_total < total_request:
                                current_count = total_request - running_total
                                if current_count > max_per_request:
                                    current_count = max_per_request
                                baf_payload[1] = str(current_count)
                                running_total = running_total + current_count
                                BAF_lines[timestamp].append(BAFLine(self.ojtsi, baf_id, baf_payload, timestamp, payload))

                        else:
                            BAF_lines[timestamp].append(BAFLine(self.ojtsi, baf_id, baf_payload, timestamp, payload))
                    elif items[1] == 'registerRAW':
                        timestamp = self.convertTime2Frame(items[2])
                        payload = items[3]
                        if timestamp not in RAW_lines:
                            RAW_lines[timestamp] = []
                        RAW_lines[timestamp].append(RAWLine(self.ojtsi, baf_id, baf_payload, timestamp, payload))
                    elif items[1] == 'registerRTC':
                        rtc_id = items[2]
                        timestamp = self.convertTime2Frame(items[3], adjust=self.ojtsi.rtc_corrections_info.getCorrection(rtc_id))
                        payload = items[4]
                        if timestamp not in RTC_lines:
                            RTC_lines[timestamp] = []
                        RTC_lines[timestamp].append(RTCLine(self.ojtsi, baf_id, baf_payload, rtc_id, timestamp, payload))
                    elif items[1] == 'registerTuneVar':
                        tunevar_id = items[2]
                        timestamp = self.convertTime2Frame(items[3])
                        caller_handle = items[4]
                        payload = items[5].lstrip('[').rstrip(']')
                        if timestamp not in TuneVar_lines:
                            TuneVar_lines[timestamp] = []
                        TuneVar_lines[timestamp].append(TuneVarLine(self.ojtsi, baf_id, baf_payload, tunevar_id, timestamp, caller_handle, payload))
                    elif items[1] == 'registerTOPWrite':
                        top_write_id = items[2]
                        timestamp = self.convertTime2Frame(items[3])
                        datatype = self.ojtsi.top_info.get_data_type(top_write_id)
                        payload = [x.strip() for x in items[4].lstrip('[').rstrip(']').split()]
                        if timestamp not in TOPWrite_lines:
                            TOPWrite_lines[timestamp] = []
                        TOPWrite_lines[timestamp].append(TOPWriteLine(self.ojtsi, baf_id, baf_payload, top_write_id, timestamp, datatype, payload))
                    elif items[1] == 'registerTOPRead':
                        top_read_id = items[2]
                        timestamp = self.convertTime2Frame(items[3])
                        datatype = self.ojtsi.top_info.get_data_type(top_read_id)
                        payload = [x.strip() for x in items[4].lstrip('[').rstrip(']').split()]
                        if timestamp not in TOPRead_lines:
                            TOPRead_lines[timestamp] = []
                        TOPRead_lines[timestamp].append(TOPReadLine(self.ojtsi, baf_id, baf_payload, top_read_id, timestamp, datatype, payload))
                    elif items[1] == 'registerOjtTSP':
                        tsp_id = items[2]
                        timestamp = self.convertTime2Frame(items[3])
                        datatype = self.ojtsi.tsp_info.get_data_type(tsp_id)
                        payload = items[4:]
                        if timestamp not in TSP_lines:
                            TSP_lines[timestamp] = []
                        TSP_lines[timestamp].append(TSPLine(self.ojtsi, baf_id, baf_payload, tsp_id, timestamp, datatype, payload))


                    if (timestamp != None) and (timestamp not in timestamps):
                        timestamps.append(timestamp)
                line = fp.readline()


        TuneVar_timestamps = list(TuneVar_lines.keys())
        TuneVar_timestamps.sort(key=lambda x: int(x))
        TuneVar_groups = {}
        for timestamp in TuneVar_timestamps:
            TuneVar_groups[timestamp] = TuneVarGroup(self.ojtsi, timestamp, TuneVar_lines[timestamp], filter_core=self.filter_core)

        timestamps.sort(key=lambda x: int(x))
        with open(self.output_file, 'w') as fp:
            for timestamp in timestamps:
                if timestamp in BAF_lines:
                    for baf_line in BAF_lines[timestamp]:
                        fp.write(baf_line.get_control_line() + '\n')

                if timestamp in RAW_lines:
                    for raw_line in RAW_lines[timestamp]:
                        fp.write(raw_line.get_control_line() + '\n')

                if timestamp in RTC_lines:
                    for rtc_line in RTC_lines[timestamp]:
                        if self.filter_core == None:
                            fp.write(rtc_line.get_control_line() + '\n')
                        else:
                            if ASDID_CORE(rtc_line.rtc_token) == self.filter_core:
                                fp.write(rtc_line.get_control_line() + '\n')

                if timestamp in TOPWrite_lines:
                    for top_write_line in TOPWrite_lines[timestamp]:
                        if self.filter_core == None:
                            fp.write(top_write_line.get_control_line() + '\n')
                        else:
                            print (top_write_line.top_write_token)
                            if ASDID_CORE(top_write_line.top_write_token) == self.filter_core:
                                fp.write(top_write_line.get_control_line() + '\n')

                if timestamp in TOPRead_lines:
                    for top_read_line in TOPRead_lines[timestamp]:
                        if self.filter_core == None:
                            fp.write(top_read_line.get_control_line() + '\n')
                        else:
                            print(top_read_line.top_read_token)
                            if ASDID_CORE(top_read_line.top_read_token) == self.filter_core:
                                fp.write(top_read_line.get_control_line() + '\n')

                if timestamp in TSP_lines:
                    for tsp_line in TSP_lines[timestamp]:
                        if self.filter_core == None:
                            fp.write(tsp_line.get_control_line() + '\n')
                        else:
                            print(tsp_line.tsp_token)
                            if ASDID_CORE(tsp_line.tsp_token) == self.filter_core:
                                fp.write(tsp_line.get_control_line() + '\n')

                if timestamp in TuneVar_groups:
                    fp.write( TuneVar_groups[timestamp].get_control_lines() + '\n')

                fp.write('\n')

#*********************************************************************************
# scan_work_folder(work_folder)
#*********************************************************************************
def scan_work_folder(ojtsi, work_folder):
    model_name = ojtsi.model_name

    rtc_payloads_xml = os.path.join(work_folder, model_name + '_RTC_Payloads.xml')
    top_tokens_xml = os.path.join(work_folder, model_name + '_TOP_Tokens.xml')
    tsp_tokens_xml = os.path.join(work_folder, model_name + '_TSP_Tokens.xml')

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
    corrections_xml = None
    work_folder = None
    frame_size = None
    sample_rate = None
    base_time_step = None
    baf_xml = None

    try:
        opts, args = getopt.getopt(argv, 'hm:w:b:c:f:s:i:o:',
                                        ['help','model=','work=','baf=','corrections=',
                                         'frame_size=','sample_rate=', 'base_time_step=',
                                         'in=','out='])
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
                print( 'Error: work folder ' + work_folder + ' could not be found')
                sys.exit()
        elif opt in ('-b', '--baf'):
            baf_xml = os.path.abspath(arg)
            if not os.path.isfile(baf_xml):
                print( 'Error: Message_Id.xml file ' + baf_xml + ' could not be found')
                sys.exit()
        elif opt in ('-c', '--corrections'):
            corrections_xml = os.path.abspath(arg)
            if not os.path.isfile(corrections_xml):
                print( 'Error: <Model>_RTC_Corrections.xml file ' + corrections_xml + ' could not be found')
                sys.exit()
        elif opt in ('-t', '--base_time_step'):
            base_time_step = arg
        elif opt in ('-f', '--frame_size'):
            frame_size = int(arg)
        elif opt in ('-s', '--sample_rate'):
            sample_rate = int(arg)
        elif opt in ('-i', '--in'):
            input_file = os.path.abspath(arg)
            if not os.path.isfile(input_file):
                print( 'Error: config file ' + input_file + ' could not be found')
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

    ojtsi.load_corrections_xml(corrections_xml)

    if (work_folder == None):
        print( 'Warning: Talaria work folder unspecified. Only BAF commands will be converted')
    else:
        if scan_work_folder(ojtsi, work_folder) == False:
            sys.exit()

    return ConvertRequest(ojtsi=ojtsi, input_file=input_file, output_file=output_file, frame_size=frame_size, sample_rate=sample_rate, base_time_step=base_time_step)

def CompileRequest (model_name, work_folder, baf_xml, input_file, output_file) :
    ojtsi = OJTSI(model_name)
    ojtsi.load_baf_xml(baf_xml)

    if scan_work_folder(ojtsi, work_folder) == False:
        sys.exit()

    ojtsi.load_corrections_xml(corrections_xml=None)

    conv = ConvertRequest(ojtsi=ojtsi, input_file=input_file, output_file=output_file, frame_size=None, sample_rate=None, base_time_step=None )
    conv.run()

def CompileRequest2 (frame_size=None, sample_rate=None, base_time_step=None, baf_xml=None, rtc_payloads_xml=None, top_tokens_xml=None,  tsp_tokens_xml=None, tie_dll=None, tte_dll=None, input_file=None, output_file=None, filter_core=None) :
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
    ojtsi.load_corrections_xml(corrections_xml=None)
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

    conv = ConvertRequest(ojtsi=ojtsi, input_file=input_file, output_file=output_file, frame_size=frame_size, sample_rate=sample_rate, base_time_step=base_time_step, filter_core=filter_core)
    conv.run()


def CompileRequest3 (frame_size=None, sample_rate=None, base_time_step=None, baf_xml=None, corrections_xml=None, rtc_payloads_xml=None, top_tokens_xml=None, tsp_tokens_xml=None, tie_dll=None, tte_dll=None, input_file=None, output_file=None, filter_core=None) :
    ojtsi = OJTSI('Model')
    if not os.path.isfile(baf_xml):
        print ( 'Error: Message.xml ' + baf_xml + ' could not be found' )
        sys.exit()

    if (corrections_xml != None) and (not os.path.isfile(corrections_xml)):
        print ( 'Warning: <Model>_RTC_Corrections.xml ' + corrections_xml + ' could not be found' )
        corrections_xml = None

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
    if corrections_xml != None:
        ojtsi.load_corrections_xml(corrections_xml)
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

    conv = ConvertRequest(ojtsi=ojtsi, input_file=input_file, output_file=output_file, frame_size=frame_size, sample_rate=sample_rate, base_time_step=base_time_step, filter_core=filter_core)
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
