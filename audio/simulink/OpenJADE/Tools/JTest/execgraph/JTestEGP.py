#*********************************************************************************
# This script is JTestRTA and JTestHIL class structure
#
#*********************************************************************************

import os
import sys
import getopt
import struct
import xml.etree.ElementTree as et
from xml.etree.ElementTree import XMLParser

from OJTSI.baf_msg_info import *
from execgraph.BAFLogger import BAF_logger
from execgraph.BAFLogger import Logger_dump

is_py2 = sys.version[0] == '2'

class BAFMsg:
    def __init__(self, baf_xml):
        self.baf_msg_info = baf_msg_info(baf_xml)

    def swap_endian_hex32(self, hex32_str):
        return hex32_str[6:8]+hex32_str[4:6]+hex32_str[2:4]+hex32_str[0:2]

    def convert_response(self, ojx):
        if not ojx.startswith('RSP:'):
            return None

        id, data = [x.strip() for x in ojx.split(':')]

        if (id == None) or (data == None):
            return None

        items = [x.strip() for x in data.split()]

        if id == 'RSP':
            baf_token = int(items[0], 16) & 0xFF
            sys_start_token = int(self.baf_msg_info.get_token('SYS_START'), 16) &  0xFF
            if baf_token >= sys_start_token:
                baf_id = self.baf_msg_info.get_id(items[0])
                if baf_id != None:
                    baf_token = items[0]
                    baf_status = items[1]
                    payload = items[2:]

                    baf_status = self.baf_msg_info.deconstruct_status(baf_status)
                    baf_payload = self.baf_msg_info.deconstruct_message(baf_token)

                    lines = []
                    lines.append( baf_payload + ' ' + baf_status )

                    if len(payload) > 0:
                        payload = self.baf_msg_info.parse_payload(baf_token, payload)
                        payload = payload.split('\n')
                        for str in payload:
                            lines.append(str)

                    return lines
        return None

    def convert_request(self, ojt):
        baf_items = [x.strip() for x in ojt.lstrip('[').rstrip(']').split()]
        baf_id = baf_items[0]
        baf_payload = baf_items[1:]

        ojx = []

        if baf_id == 'SYS_LOG_PULL':
            total_request = int(baf_payload[1])
            max_per_request = 32
            running_total = 0
            while running_total < total_request:
                current_count = total_request - running_total
                if current_count > max_per_request:
                    current_count = max_per_request
                baf_payload[1] = str(current_count)
                baf_token = self.baf_msg_info.construct_message(baf_id, baf_payload)
                ojx.append(self.swap_endian_hex32(baf_token))
                running_total = running_total + current_count
        else:
            baf_token = self.baf_msg_info.construct_message(baf_id, baf_payload)
            ojx.append(self.swap_endian_hex32(baf_token))

        return ojx


class JTestRTAEGP:
    initcmds_ojt = [
        # Disable all logs at the start
        '[SYS_LOG_DISABLE LOG_TYPE_PRINT]',
        '[SYS_LOG_DISABLE LOG_TYPE_CAPTURE]',
        '[SYS_LOG_DISABLE LOG_TYPE_EXEC]',
        # Put exec log in circular mode
        '[SYS_LOG_CIRCULAR LOG_TYPE_EXEC]',
        # Enable exec log
        '[SYS_LOG_ENABLE LOG_TYPE_EXEC]']

    pullcmds_ojt = [
        # Disable and pull exec log
        '[SYS_LOG_DISABLE LOG_TYPE_EXEC]',
        '[SYS_LOG_PULL LOG_TYPE_EXEC 400]',
        # Re-enable exec log
        '[SYS_LOG_ENABLE LOG_TYPE_EXEC]']

    def __init__(self, config_file):
        self.valid = False
        self.baf_msg = None
        self.initcmds_ojx = []
        self.pullcmds_ojx = []
        self.clock_rate = None
        self.sample_rate = None
        self.frame_size = None

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        xml_node = xml_root.find('BAF_XML')
        if xml_node == None:
            print ('Error: BAF_XML node could not be found in test config xml ' + config_file)
            return

        baf_xml = os.path.normpath(os.path.join(os.path.dirname(config_file), xml_node.get('file')))
        if not os.path.isfile(baf_xml):
            print ('Error: baf xml ' + baf_xml + ' could not be found')
            return
        self.baf_msg = BAFMsg(baf_xml)

        xml_node = xml_root.find('PROCESSOR')
        if xml_node == None:
            print ('Warning: PROCESSOR node could not be found in test config xml ' + config_file)
        else:
            self.clock_rate = int(xml_node.get('clock_rate'))

        xml_node = xml_root.find('AUDIO_PARAMS')
        if xml_node == None:
            print ('Warning: AUDIO_PARAMS node could not be found in test config xml ' + config_file)
        else:
            self.sample_rate = int(xml_node.get('sample_rate'))
            self.frame_size = int(xml_node.get('frame_size'))

        for ojt in self.initcmds_ojt:
            ojxs = self.baf_msg.convert_request(ojt)
            for ojx in ojxs:
                ojx = ojx+'\n'
                if not is_py2:
                    ojx = ojx.encode()
                self.initcmds_ojx.append(ojx)

        for ojt in self.pullcmds_ojt:
            ojxs = self.baf_msg.convert_request(ojt)
            for ojx in ojxs:
                ojx = ojx+'\n'
                if not is_py2:
                    ojx = ojx.encode()
                self.pullcmds_ojx.append(ojx)

        self.valid = True

    def isValid(self):
        return (self.valid)

    def get_frame_size(self):
        return self.frame_size

    def get_sample_rate(self):
        return self.sample_rate

    def get_clock_rate(self):
        return self.clock_rate

    def get_ExecLog_InitCmds(self):
        if self.valid:
            return self.initcmds_ojx
        else:
            return None

    def get_ExecLog_PullCmds(self):
        if self.valid:
            return self.pullcmds_ojx
        else:
            return None

    def parse_ExecLog(self, exec_log):
        if self.valid:
            ojxs = []
            for ojx in exec_log:
                if not is_py2:
                    ojx = ojx.decode('utf-8')
                lines = self.baf_msg.convert_response(ojx.strip())
                if lines != None:
                    for line in lines:
                        ojxs.append(line)
            return ojxs
        else:
            return None

    def is_prompt(self, buf):
        if self.valid:
            if not is_py2:
                buf = buf.decode('utf-8')
            if (buf == '>\n') or (buf == '?\n'):
                return True
            else:
                return False
        else:
            return None


class JTestHILEGP:
    dump_exec_ojt = '[SYS_LOG_DUMP LOG_TYPE_EXEC]'

    def __init__(self, config_file):
        self.valid = False
        self.baf_msg = None
        self.clock_rate = None
        self.sample_rate = None
        self.frame_size = None
        self.dump_exec_ojx = None
        if is_py2:
            self.exec_log = ''
        else:
            self.exec_log = b''

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        xml_node = xml_root.find('BAF_XML')
        if xml_node == None:
            print ('Error: BAF_XML node could not be found in test config xml ' + config_file)
            return

        baf_xml = os.path.normpath(os.path.join(os.path.dirname(config_file), xml_node.get('file')))
        if not os.path.isfile(baf_xml):
            print ('Error: baf xml ' + baf_xml + ' could not be found')
            return
        self.baf_msg = BAFMsg(baf_xml)

        xml_node = xml_root.find('PROCESSOR')
        if xml_node == None:
            print ('Warning: PROCESSOR node could not be found in test config xml ' + config_file)
        else:
            self.clock_rate = int(xml_node.get('clock_rate'))

        xml_node = xml_root.find('AUDIO_PARAMS')
        if xml_node == None:
            print ('Warning: AUDIO_PARAMS node could not be found in test config xml ' + config_file)
        else:
            self.sample_rate = int(xml_node.get('sample_rate'))
            self.frame_size = int(xml_node.get('frame_size'))

        self.dump_exec_ojx = self.baf_msg.convert_request(self.dump_exec_ojt)
        if len(self.dump_exec_ojx) == 1:
            dump_exec_ojx = self.baf_msg.swap_endian_hex32(self.dump_exec_ojx[0])
            dump_exec_req = 'REQ: ' + dump_exec_ojx
            dump_exec_rsp = 'RSP: ' + dump_exec_ojx + ' 00000000'
            if is_py2:
                self.dump_exec_req = dump_exec_req
                self.dump_exec_rsp = dump_exec_rsp
            else:
                self.dump_exec_req = dump_exec_req.encode('utf-8')
                self.dump_exec_rsp = dump_exec_rsp.encode('utf-8')
            self.valid = True

    def isValid(self):
        return (self.valid)

    def get_frame_size(self):
        return self.frame_size

    def get_sample_rate(self):
        return self.sample_rate

    def get_clock_rate(self):
        return self.clock_rate

    def parse_ExecLog(self, exec_log):
        if self.valid:
            if is_py2:
                empty_str = ''
                newline = '\n'
            else:
                empty_str = b''
                newline = b'\n'

            exec_log = self.exec_log + empty_str.join(exec_log)

            start_idx = exec_log.find(self.dump_exec_req)
            if start_idx == -1:
                self.exec_log = empty_str
                exec_log = []
            else:
                stop_idx = exec_log.find(self.dump_exec_rsp, start_idx)
                if stop_idx == -1:
                    self.exec_log = exec_log[start_idx:]
                    exec_log = []
                else:
                    self.exec_log = exec_log[stop_idx:]
                    if is_py2:
                        exec_log = [ x.strip() for x in exec_log[start_idx:stop_idx].split(newline) ]
                    else:
                        exec_log = [ x.decode('utf-8').strip() for x in exec_log[start_idx:stop_idx].split(newline) ]
            return exec_log
        else:
            return None

class JTestPILEGP:
    def __init__(self, config_file):
        self.valid = False
        self.baf_msg = None
        self.clock_rate = None
        self.sample_rate = None
        self.frame_size = None

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        #xml_node = xml_root.find('BAF_XML')
        #if xml_node == None:
        #    print ('Error: BAF_XML node could not be found in test config xml ' + config_file)
        #    return
        #
        #baf_xml = os.path.normpath(os.path.join(os.path.dirname(config_file), xml_node.get('file')))
        #if not os.path.isfile(baf_xml):
        #    print ('Error: baf xml ' + baf_xml + ' could not be found')
        #    return
        #self.baf_msg = BAFMsg(baf_xml)

        xml_node = xml_root.find('PROCESSOR')
        if xml_node == None:
            print ('Warning: PROCESSOR node could not be found in test config xml ' + config_file)
        else:
            self.clock_rate = int(xml_node.get('clock_rate'))

        xml_node = xml_root.find('AUDIO_PARAMS')
        if xml_node == None:
            print ('Warning: AUDIO_PARAMS node could not be found in test config xml ' + config_file)
        else:
            self.sample_rate = int(xml_node.get('sample_rate'))
            self.frame_size = int(xml_node.get('frame_size'))
        self.valid = True

    def isValid(self):
        return (self.valid)

    def get_frame_size(self):
        return self.frame_size

    def get_sample_rate(self):
        return self.sample_rate

    def get_clock_rate(self):
        return self.clock_rate

    def parse_ExecLog(self, buffer_bytes):
        if not self.valid:
            return None

        bl = BAF_logger(buffer_bytes['BAF_logger'], debug=False)

        logger_id = 'LOG_TYPE_EXEC'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        exec_log_buffer = buffer_bytes['heap_log_buffer'][buffer_start : buffer_start + buffer_len]
        exec_log = Logger_dump(bl, logger_id, exec_log_buffer)
        return exec_log

class VegaAlpineEGP:
    def __init__(self, config_file):
        self.valid = False
        self.baf_msg = None
        self.clock_rate = None
        self.sample_rate = None
        self.frame_size = None

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        #xml_node = xml_root.find('BAF_XML')
        #if xml_node == None:
        #    print ('Error: BAF_XML node could not be found in test config xml ' + config_file)
        #    return
        #
        #baf_xml = os.path.normpath(os.path.join(os.path.dirname(config_file), xml_node.get('file')))
        #if not os.path.isfile(baf_xml):
        #    print ('Error: baf xml ' + baf_xml + ' could not be found')
        #    return
        #self.baf_msg = BAFMsg(baf_xml)

        xml_node = xml_root.find('PROCESSOR')
        if xml_node == None:
            print ('Warning: PROCESSOR node could not be found in test config xml ' + config_file)
        else:
            self.clock_rate = int(xml_node.get('clock_rate'))

        xml_node = xml_root.find('AUDIO_PARAMS')
        if xml_node == None:
            print ('Warning: AUDIO_PARAMS node could not be found in test config xml ' + config_file)
        else:
            self.sample_rate = int(xml_node.get('sample_rate'))
            self.frame_size = int(xml_node.get('frame_size'))
        self.valid = True

    def isValid(self):
        return (self.valid)

    def get_frame_size(self):
        return self.frame_size

    def get_sample_rate(self):
        return self.sample_rate

    def get_clock_rate(self):
        return self.clock_rate

    def parse_ExecLog(self, buffer_bytes):
        if not self.valid:
            return None

        bl = BAF_logger(buffer_bytes['BAF_logger'], debug=False)

        logger_id = 'LOG_TYPE_EXEC'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        exec_log_buffer = buffer_bytes['heap_log_buffer'][buffer_start : buffer_start + buffer_len]
        exec_log = Logger_dump(bl, logger_id, exec_log_buffer)
        return exec_log


class SpetterEGP:
    def __init__(self, config_file):
        self.valid = False
        self.baf_msg = None
        self.clock_rate = None
        self.sample_rate = None
        self.frame_size = None

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        #xml_node = xml_root.find('BAF_XML')
        #if xml_node == None:
        #    print ('Error: BAF_XML node could not be found in test config xml ' + config_file)
        #    return
        #
        #baf_xml = os.path.normpath(os.path.join(os.path.dirname(config_file), xml_node.get('file')))
        #if not os.path.isfile(baf_xml):
        #    print ('Error: baf xml ' + baf_xml + ' could not be found')
        #    return
        #self.baf_msg = BAFMsg(baf_xml)

        xml_node = xml_root.find('PROCESSOR')
        if xml_node == None:
            print ('Warning: PROCESSOR node could not be found in test config xml ' + config_file)
        else:
            self.clock_rate = int(xml_node.get('clock_rate'))

        xml_node = xml_root.find('AUDIO_PARAMS')
        if xml_node == None:
            print ('Warning: AUDIO_PARAMS node could not be found in test config xml ' + config_file)
        else:
            self.sample_rate = int(xml_node.get('sample_rate'))
            self.frame_size = int(xml_node.get('frame_size'))
        self.valid = True

    def isValid(self):
        return (self.valid)

    def get_frame_size(self):
        return self.frame_size

    def get_sample_rate(self):
        return self.sample_rate

    def get_clock_rate(self):
        return self.clock_rate

    def parse_ExecLog(self, buffer_bytes):
        if not self.valid:
            return None

        bl = BAF_logger(buffer_bytes['BAF_logger'], debug=False)

        logger_id = 'LOG_TYPE_EXEC'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        exec_log_buffer = buffer_bytes['heap_log_buffer'][buffer_start : buffer_start + buffer_len]
        exec_log = Logger_dump(bl, logger_id, exec_log_buffer)
        return exec_log


