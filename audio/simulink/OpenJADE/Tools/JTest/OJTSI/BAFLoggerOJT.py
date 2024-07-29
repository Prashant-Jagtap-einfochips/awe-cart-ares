import os
import getopt
import sys
import struct
import copy
import numpy as np
from collections import OrderedDict
import xml.etree.ElementTree as et
import shutil
import subprocess
import json

__usage__ = """ \

Usage:
  BAFLoggerOJT.py [-h] -b baf_logger -l heap_log_buffer

Description:
   - Parses baf_logger.bin and heap_log_buffer.bin

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -b,   --baf   baf_logger
  -l,   --log   heap_log_buffer

"""

class MapSymbol:
    def __init__(self, name, demangled, address, size):
        self.name = name
        self.demangled = demangled
        self.address = address
        self.size = str(int(size,16))

class MapFile:
    def __init__(self, map_file, map_type=None):
        self.map_type = map_type

        if self.map_type == 'cces':
            xml_tree = et.parse(map_file)
            self.xml_root = xml_tree.getroot()

        self.map_file = map_file

    def get_symbol(self, map_symbol_name):
        map_symbol = None

        if self.map_type == 'cces':
            ms = self.xml_root.findall(".//SYMBOL[@name='" + map_symbol_name + "']")
            if len(ms) > 0:
                if ms[0][0].tag == 'DEMANGLED_NAME':
                    demangled = ms[0][0].text
                else:
                    demangled = map_symbol_name
                map_symbol = MapSymbol(map_symbol_name,
                                    demangled,
                                    ms[0].get('address'),
                                    ms[0].get('size'))
            else:
                map_symbol = None
                print ( 'Warning: map_symbol ' + map_symbol_name + ' could not be found in ' + self.map_file)
        elif self.map_type == 'gnu':
            map_lines = []
            with open(self.map_file, 'r') as map_f:
                map_lines = map_f.readlines()

            start_addr = None
            numBytes = None
            num_lines = len(map_lines)
            for i in range(0, num_lines):
                # For compiler flag -fdata-sections, checking for newline and hex address is more accurate
                if (map_symbol_name+'\n' in map_lines[i]) and (map_lines[i].find('0x') != -1):
                    pos = map_lines[i].find('0x')
                    start_addr = map_lines[i][pos:(pos+10)] # 10 characters in 32-bit hex

                    end_addr_line = i
                    pos = -1
                    while (pos == -1):
                        end_addr_line = end_addr_line+1
                        pos = map_lines[end_addr_line].find('0x')

                    end_addr = map_lines[end_addr_line][pos:(pos+10)]

                    numBytes = hex(int(end_addr,16) - int(start_addr,16))

            if (start_addr != None) and (numBytes != None):
                map_symbol = MapSymbol(map_symbol_name,
                                        map_symbol_name, # TODO - May need to be replaced
                                        start_addr,
                                        numBytes)
            else:
                map_symbol = None
                print ( 'Warning: map_symbol ' + map_symbol_name + ' could not be found in ' + self.map_file)
        elif self.map_type == 'hexagon':
            # TODO - Do not need actual address for now.
            map_symbol = MapSymbol(map_symbol_name,
                                    map_symbol_name,
                                    0xDEADBEEF,
                                    '0x0')
        else:
            print('Error: map_type cannot be determined.')

        return map_symbol

    def symbol_from_address(self, symbol_address, symbol_filter):
        if self.map_type == 'cces':
            if symbol_filter == None:
                ms = self.xml_root.findall(".//SYMBOL")
            else:
                ms = [el for el in self.xml_root.findall(".//SYMBOL") if symbol_filter in el.get('name')]

            sym_addr = int(symbol_address, 16)
            for el in ms:
                eladdr = int(el.get('address'), 16)
                elsize = int(el.get('size'), 16)
                if (sym_addr >= eladdr) and ( sym_addr < (eladdr + elsize)):
                    map_symbol_name = el.get('name')
                    dm = el.findall('./DEMANGLED_NAME')
                    if len(dm) > 0:
                        demangled = dm[0].text
                    else:
                        demangled = map_symbol_name
                    map_symbol = MapSymbol(map_symbol_name,
                                demangled,
                                el.get('address'),
                                el.get('size'))
                    return map_symbol

            print ( 'Error: symbol_address ' + symbol_address + ' could not be found in ' + self.map_file)
            return None
        elif self.map_type == 'gnu':
            print('TBD')
            return None
        else:
            print('Error: map_type cannot be determined.')
            return None


# From Log.h
#    typedef struct Log
#    {
#       uint8_t msg[2];
#       uint8_t sender_id;
#       uint8_t arg_byte;
#       uint32_t arg_word;
#    }  Log_record;
#
#    typedef union {
#      Log_record record;
#      float flt[2];
#      uint32_t word[2];
#    } Log;

class Log:
    #
    # Format is
    # (member_name, [offset, data_size, data_type, value])
    # data_type int, addr, bool
    #
    log_template = OrderedDict([
        ('record_msg_0',     [0,  1, 'uint',   None]),
        ('record_msg_1',     [1,  1, 'uint',   None]),
        ('record_sender_id', [2,  1, 'uint',   None]),
        ('record_arg_byte',  [3,  1, 'uint',   None]),
        ('record_arg_word',  [4,  4, 'uint',   None]),
        ('flt_0',            [0,  4, 'float',  None]),
        ('flt_1',            [4,  4, 'float',  None]),
        ('word_0',           [0,  4, 'uint',   None]),
        ('word_1',           [4,  4, 'uint',   None])
    ])
    sizeof_Log = 8

    def __init__(self, buffer):
        if len(buffer) != self.sizeof_Log:
            self.log = None
            return

        self.log = copy.deepcopy(self.log_template)

        buffer_idx = 0
        for member_id in self.log.keys():
            offset    = self.log[member_id][0]
            data_size = self.log[member_id][1]
            data_type = self.log[member_id][2]
            if data_size == 4:
                if data_type == 'uint':
                    self.log[member_id][3] = struct.unpack('<I', buffer[buffer_idx+offset: buffer_idx+offset+data_size])[0]
                elif data_type == 'float':
                    self.log[member_id][3] = struct.unpack('<f', buffer[buffer_idx+offset: buffer_idx+offset+data_size])[0]
            elif data_size == 1:
                self.log[member_id][3] = buffer[buffer_idx+offset]

    def is_Valid(self):
        return (self.log != None)

    def get_member(self, member_id):
        return self.log[member_id][3]

    def __str__(self):
        mystr = ''
        for k,v in self.log.items():
            v3 = 'None' if (v[3] == None) else hex(v[3])
            mystr += f'{k}: {v[0]}, {v[1]}, {v[2]}, {v3}\n'
        return mystr

    def __repr__(self):
        return self.__str__()
#
# From Heap.h
#    typedef struct Heap
#    {
#       uint32_t size;
#       void* top;
#       void* base;
#    } Heap;
#
# From Logger.h
#    typedef struct Logger
#    {
#       Heap heap;        /* 12 */
#       Log* pull;        /* 04 */
#       Log* start;       /* 04 */
#       Log* end;         /* 04 */
#       Log* current;     /* 04 */
#       bool wrapped;     /* 01 */
#       bool circular;    /* 01 */
#       bool enabled;     /* 01 */
#    } Logger;
#
#    typedef enum
#    {
#       LOG_TYPE_PRINT  ,
#       LOG_TYPE_CAPTURE,
#       LOG_TYPE_EXEC   ,
#    } Logger_Type;
#
# From BAF.c
#       for ( uint32_t logger_id = 0; logger_id < FW_NUM_LOG; logger_id++) {
#           Heap_init(&(BAF_logger[logger_id].heap),
#                        (uint8_t*)this_private->resource.memory_log_pointer + logger_id*heap_size,
#                         heap_size);
#          Logger_init((Logger_Type)logger_id, true);
#       }



#
#  This class represents BAF_logger in GUL memory
#
class BAF_logger:
    #
    # Format is
    # (member_name, [offset, data_size, data_type, value])
    # data_type int, addr, bool
    #
    logger_template_ILP32 = OrderedDict([
        ('heap_size', [0,  4, 'uint', None]),
        ('heap_top',  [4,  4, 'addr', None]),
        ('heap_base', [8,  4, 'addr', None]),
        ('pull',      [12, 4, 'addr', None]),
        ('start',     [16, 4, 'addr', None]),
        ('end',       [20, 4, 'addr', None]),
        ('current',   [24, 4, 'addr', None]),
        ('wrapped',   [28, 1, 'bool', None]),
        ('circular',  [29, 1, 'bool', None]),
        ('enabled',   [30, 1, 'bool', None])
    ])
    sizeof_Heap_ILP32 = 12
    sizeof_Logger_ILP32 = 32

    logger_template_LP64 = OrderedDict([
        ('heap_size', [0,  4, 'uint', None]),
        ('heap_top',  [8,  8, 'addr', None]),
        ('heap_base', [16, 8, 'addr', None]),
        ('pull',      [24, 8, 'addr', None]),
        ('start',     [32, 8, 'addr', None]),
        ('end',       [40, 8, 'addr', None]),
        ('current',   [48, 8, 'addr', None]),
        ('wrapped',   [56, 1, 'bool', None]),
        ('circular',  [57, 1, 'bool', None]),
        ('enabled',   [58, 1, 'bool', None])
    ])
    sizeof_Heap_LP64 = 24
    sizeof_Logger_LP64 = 64


    logger_types = [ 'LOG_TYPE_PRINT',
                     'LOG_TYPE_CAPTURE',
                     'LOG_TYPE_EXEC']

    def __init__(self, buffer, arch='ILP32', debug=False):
        self.debug = debug
        if arch == 'LP64':
            self.logger_template = self.logger_template_LP64
            self.sizeof_Logger = self.sizeof_Logger_LP64
            self.sizeof_Heap = self.sizeof_Heap_LP64
        else:
            self.logger_template = self.logger_template_ILP32
            self.sizeof_Logger = self.sizeof_Logger_ILP32
            self.sizeof_Heap = self.sizeof_Heap_ILP32

        if len(buffer) != len(self.logger_types) * self.sizeof_Logger:
            self.baf_logger = None
            return

        self.baf_logger = OrderedDict()
        for logger_id in self.logger_types:
            self.baf_logger[logger_id] = copy.deepcopy(self.logger_template)

        buffer_idx = 0
        for logger_id in self.logger_types:
            logger = self.baf_logger[logger_id]
            for member_id in logger:
                offset    = logger[member_id][0]
                data_size = logger[member_id][1]
                data_type = logger[member_id][2]
                if data_size == 8:
                    logger[member_id][3] = struct.unpack('<Q', buffer[buffer_idx+offset: buffer_idx+offset+data_size])[0]
                elif data_size == 4:
                    logger[member_id][3] = struct.unpack('<I', buffer[buffer_idx+offset: buffer_idx+offset+data_size])[0]
                elif data_size == 1:
                    logger[member_id][3] = buffer[buffer_idx+offset]

                if self.debug:
                    print ( member_id + ': ' + hex(logger[member_id][3]) )

            buffer_idx = buffer_idx + self.sizeof_Logger

        self.valid = True

    def __str__(self):
        mystr = ''
        for logger_id in self.logger_types:
            mystr += f'{logger_id}\n'
            for k,v in self.baf_logger[logger_id].items():
                v3 = 'None' if (v[3] == None) else hex(v[3])
                mystr += f'\t{k}: {v[0]}, {v[1]}, {v[2]}, {v3}\n'
        return mystr

    def __repr__(self):
        return self.__str__()

    def is_Valid(self):
        return (self.baf_logger != None)

    def get_logger(self, logger_id):
        return self.baf_logger[logger_id]

    def get_logger_ids(self):
        return self.logger_types

    def get_member_ids(self):
        return self.logger_template.keys()

    def get_logger_member(self, logger_id, member_id):
        return self.baf_logger[logger_id][member_id][3]

    def get_member_offset(self, member_id):
        return self.logger_template[member_id][0]

    def get_member_size(self, member_id):
        return self.logger_template[member_id][1]

    def get_pull_offset(self, logger_id):
        return (self.baf_logger[logger_id]['pull'][3] - self.baf_logger[logger_id]['start'][3])

    def get_end_offset(self, logger_id):
        return (self.baf_logger[logger_id]['end'][3] - self.baf_logger[logger_id]['start'][3])

    def get_current_offset(self, logger_id):
        return (self.baf_logger[logger_id]['current'][3] - self.baf_logger[logger_id]['start'][3])

    def get_buffer_start(self, logger_id):
            return (self.baf_logger[logger_id]['start'][3] - self.baf_logger['LOG_TYPE_PRINT']['start'][3])

    def get_buffer_len(self, logger_id):
        return (self.baf_logger[logger_id]['end'][3] - self.baf_logger[logger_id]['start'][3])

    def is_wrapped(self, logger_id):
        return (self.baf_logger[logger_id]['wrapped'][3] == 1)

    def is_circular(self, logger_id):
        return (self.baf_logger[logger_id]['circular'][3] == 1)

    def is_enabled(self, logger_id):
        return (self.baf_logger[logger_id]['enabled'][3] == 1)

# From Logger.c
#   void Logger_dump(Logger_Type id)
#   {
#   	Logger* this = &BAF_logger[id];
#   	Log* log;
#
#   	BAF_printf("LOG%d %d %d %d %08X\n", id, this->enabled, this->circular, this->wrapped, this->current - this->start);
#
#   	if(this->start)
#      {
#         if ((id == LOG_TYPE_EXEC) || (id == LOG_TYPE_PRINT))
#         {
#            if ( this->circular && this->wrapped ){
#               for ( log = this->current; log < this->end-1; log++ ) {
#                  BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
#                                 log->record.sender_id, log->record.arg_byte, log->record.arg_word );
#               }
#            }
#            for ( log = this->start; log < this->current; log++) {
#               BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
#                                 log->record.sender_id, log->record.arg_byte, log->record.arg_word );
#            }
#         }
#         else if (id == LOG_TYPE_CAPTURE)
#         {
#            /* Circular mode is not supported */
#            for ( log = this->start; log < this->current; log++) {
#               BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
#                                 log->record.sender_id, log->record.arg_byte, log->record.arg_word );
#               if ( log->record.msg[0] == 'C' ) {  // capture log
#                   uint32_t size = log->record.arg_word;
#                 uint32_t i;
#                 log++;
#                 for ( i = 0 ; i < size; i+=sizeof(Log)/sizeof(float), log++ )	{
#                    BAF_printf(" - %.3f\n", log->flt[0]);
#                    BAF_printf(" - %.3f\n", log->flt[1]);
#                 }
#                 log--;
#              }
#              else if ( log->record.msg[0] == 'S' ) {  // stats log
#                 uint32_t size = log->record.arg_word;
#                 uint32_t i;
#                 log++;
#                 for ( i = 0 ; i < size; i+=sizeof(Log)/sizeof(uint32_t), log++ )	{
#                    BAF_printf(" - %08x\n", log->word[0]);
#                    BAF_printf(" - %08x\n", log->word[1]);
#                 }
#                 log--;
#              }
#           }
#        }
#  	}
#  }

def Logger_dump(baf_logger, logger_id, buffer):
    baf_printf = []
    id = list(baf_logger.get_logger_ids()).index(logger_id)
    this_enabled = baf_logger.get_logger_member(logger_id, 'enabled')
    this_circular = baf_logger.get_logger_member(logger_id, 'circular')
    this_wrapped = baf_logger.get_logger_member(logger_id, 'wrapped')
    this_current = baf_logger.get_current_offset(logger_id)
    this_end = baf_logger.get_end_offset(logger_id) - Log.sizeof_Log
    this_start = 0
    sizeof_capture = 4
    capture_inc = Log.sizeof_Log/sizeof_capture

    baf_printf.append( 'LOG%d %d %d %d %08X' % (id, this_enabled, this_circular, this_wrapped, int(this_current/Log.sizeof_Log)) )

    if baf_logger.get_logger_member(logger_id, 'start') == 0:
        return baf_printf

    if (logger_id == 'LOG_TYPE_EXEC') or (logger_id == 'LOG_TYPE_PRINT'):
        if baf_logger.is_circular(logger_id) and baf_logger.is_wrapped(logger_id):
            log_curr = this_current
            while log_curr < this_end:
                log_obj = Log( buffer[log_curr:log_curr+Log.sizeof_Log] )
                baf_printf.append( '%c%c %02X %02X %08X' % ( log_obj.get_member('record_msg_0'),
                                                            log_obj.get_member('record_msg_1'),
                                                            log_obj.get_member('record_sender_id'),
                                                            log_obj.get_member('record_arg_byte'),
                                                            log_obj.get_member('record_arg_word') ) )
                log_curr = log_curr + Log.sizeof_Log

        log_curr = this_start
        while log_curr < this_current:
            log_obj = Log( buffer[log_curr:log_curr+Log.sizeof_Log] )
            baf_printf.append( '%c%c %02X %02X %08X' % ( log_obj.get_member('record_msg_0'),
                                                        log_obj.get_member('record_msg_1'),
                                                        log_obj.get_member('record_sender_id'),
                                                        log_obj.get_member('record_arg_byte'),
                                                        log_obj.get_member('record_arg_word') ) )
            log_curr = log_curr + Log.sizeof_Log
    if (logger_id == 'LOG_TYPE_CAPTURE'):
        log_curr = this_start
        while log_curr < this_current:
            log_obj = Log( buffer[log_curr:log_curr+Log.sizeof_Log] )
            baf_printf.append( '%c%c %02X %02X %08X' % ( log_obj.get_member('record_msg_0'),
                                                        log_obj.get_member('record_msg_1'),
                                                        log_obj.get_member('record_sender_id'),
                                                        log_obj.get_member('record_arg_byte'),
                                                        log_obj.get_member('record_arg_word') ) )
            if log_obj.get_member('record_msg_0') == ord('C'):
                capture_size = log_obj.get_member('record_arg_word')
                log_curr = log_curr + Log.sizeof_Log
                capture_index = 0
                while capture_index < capture_size:
                    log_obj = Log( buffer[log_curr:log_curr+Log.sizeof_Log] )
                    baf_printf.append( ' - %.3f' % ( log_obj.get_member('flt_0')) )
                    baf_printf.append( ' - %.3f' % ( log_obj.get_member('flt_1')) )
                    log_curr = log_curr + Log.sizeof_Log
                    capture_index = capture_index + capture_inc
                log_curr = log_curr - Log.sizeof_Log
            if log_obj.get_member('record_msg_0') == ord('S'):
                capture_size = log_obj.get_member('record_arg_word')
                log_curr = log_curr + Log.sizeof_Log
                capture_index = 0
                while capture_index < capture_size:
                    log_obj = Log( buffer[log_curr:log_curr+Log.sizeof_Log] )
                    baf_printf.append( ' - %08X' % ( log_obj.get_member('word_0')) )
                    baf_printf.append( ' - %08X' % ( log_obj.get_member('word_1')) )
                    log_curr = log_curr + Log.sizeof_Log
                    capture_index = capture_index + capture_inc
                log_curr = log_curr - Log.sizeof_Log
            log_curr = log_curr + Log.sizeof_Log
    return baf_printf



#*********************************************************************************
# GenExecClass
#*********************************************************************************
class GenExecClass:
    def __init__(self, index=None, exec_file=None, file_ext=None, map_ext=None, map_type=None, preload_file=None, baf_logger=None, heap_log_buffer=None, isAudioExec=True):
        self.index = index
        self.valid = True
        self.isAudioExec = isAudioExec

        # TODO - Should decide what the "generic" case is, current assumption is GNU format
        # Alternate is to blank of map_ext or map_type being None
        if file_ext == None:
            self.file_ext = '.elf'

        self.exec_file = exec_file
        if self.isAudioExec:
            if map_ext == None:
                self.map_file = exec_file.replace(file_ext,'.map')
                self.map_type = 'gnu'
            else:
                self.map_file = exec_file.replace(file_ext, map_ext)
                self.map_type = map_type

        self.preload_file = preload_file
        if heap_log_buffer == None:
            self.heap_log_buffer = 'heap_log_buffer'
        else:
            self.heap_log_buffer = heap_log_buffer

        if baf_logger == None:
            self.baf_logger = 'BAF_logger'
        else:
            self.baf_logger = baf_logger

        if not os.path.isfile(self.exec_file):
            print ( 'Error: exec_file ' + self.exec_file + ' could not be found' )
            self.valid = False
            return

        if self.isAudioExec:
            if not os.path.isfile(self.map_file):
                print ( 'Error: map_file ' + self.map_file + ' could not be found' )
                self.valid = False
                return

        if self.preload_file != None:
            if not os.path.isfile(self.preload_file):
                print ( 'Error: preload_file ' + self.preload_file + ' could not be found' )
                self.valid = False
                return

        if self.isAudioExec:
            map_file_obj = MapFile(self.map_file, self.map_type)

            if self.index == None:
                if not self.config_baf_logger(map_file_obj,self.baf_logger,'baf_logger.bin'):
                    self.valid = False
                    return

                if not self.config_heap_log_buffer(map_file_obj,self.heap_log_buffer,'heap_log_buffer.bin'):
                    self.valid = False
                    return
            else:
                if not self.config_baf_logger(map_file_obj,self.baf_logger,'baf_logger_'+self.index+'.bin'):
                    self.valid = False
                    return

                if not self.config_heap_log_buffer(map_file_obj,self.heap_log_buffer,'heap_log_buffer_'+self.index+'.bin'):
                    self.valid = False
                    return

            self.map_file_obj = map_file_obj
        else:
            self.map_file_obj = None
            self.map_file = None

    def isValid(self):
        return self.valid

    def symbol_from_address(self, symbol_address, symbol_filter):
        if self.valid:
            return self.map_file_obj.symbol_from_address(symbol_address, symbol_filter)
        else:
            return None

    def get_symbol(self, map_symbol_name):
        if self.valid:
            return self.map_file_obj.get_symbol(map_symbol_name)

    def config_baf_logger(self, map_file_obj, symbol_name, file_name):
        self.baf_logger_symbol = map_file_obj.get_symbol(symbol_name)
        if self.baf_logger_symbol == None:
            return False
        self.baf_logger_file = file_name
        return True

    def config_heap_log_buffer(self, map_file_obj, symbol_name, file_name):
        self.heap_log_buffer_symbol = map_file_obj.get_symbol(symbol_name)
        if self.heap_log_buffer_symbol == None:
            return False
        self.heap_log_buffer_file = file_name
        return True

    def process_logger_dumps(self, work_dir, work_prefix, arch='ILP32', debug=False, log_dir=None):
        if self.index == None:
            work_prefix = work_prefix + '.'
        else:
            work_prefix = work_prefix + '_' + self.index + '.'

        if log_dir != None:
            baf_logger_path = os.path.join(work_dir, work_prefix+'baf_logger.bin')
            shutil.move(log_dir+self.baf_logger_file, baf_logger_path)

            heap_log_buffer_path = os.path.join(work_dir, work_prefix+'heap_log_buffer.bin')
            shutil.move(log_dir+self.heap_log_buffer_file, heap_log_buffer_path)
        else:
            baf_logger_path = os.path.join(work_dir, work_prefix+'baf_logger.bin')
            shutil.move(self.baf_logger_file, baf_logger_path)

            heap_log_buffer_path = os.path.join(work_dir, work_prefix+'heap_log_buffer.bin')
            shutil.move(self.heap_log_buffer_file, heap_log_buffer_path)

        if not os.path.isfile(baf_logger_path):
            print ( 'Error: baf_logger.bin ' + baf_logger_path + ' could not be found' )
            return False

        if not os.path.isfile(heap_log_buffer_path):
            print ( 'Error: heap_log_buffer.bin ' + heap_log_buffer_path + ' could not be found' )
            return False

        baf_logger_bytes = None
        with open(baf_logger_path, "rb") as f:
            baf_logger_bytes = bytearray(f.read())

        heap_log_buffer_bytes = None
        with open(heap_log_buffer_path, "rb") as f:
            heap_log_buffer_bytes = bytearray(f.read())

        bl = BAF_logger(baf_logger_bytes, arch=arch, debug=debug)
        self.baf_logger_path = baf_logger_path
        self.heap_log_buffer_path = heap_log_buffer_path

        buffer_bytes = {}
        for logger_id in bl.get_logger_ids():
            buffer_start = bl.get_buffer_start(logger_id)
            buffer_len   = bl.get_buffer_len(logger_id)
            buffer_bytes[logger_id] = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
            #print ( '%d %d %d' % (buffer_start, buffer_len, len(buffer_bytes[logger_id])) )
            baf_printf = Logger_dump(bl, logger_id, buffer_bytes[logger_id])
            logger_suffix = None
            if (logger_id == 'LOG_TYPE_PRINT'):
                logger_suffix = 'print.log'
            elif (logger_id == 'LOG_TYPE_CAPTURE'):
                logger_suffix = 'capture.log'
            elif (logger_id == 'LOG_TYPE_EXEC'):
                logger_suffix = 'exec.log'

            if logger_suffix:
                logger_file_path = os.path.join(work_dir, work_prefix+logger_suffix)
                with open(logger_file_path, 'w') as f:
                    for x in baf_printf:
                        f.write(x)
                        f.write('\n')


#*********************************************************************************
# DxeClass
#*********************************************************************************
class DxeClass(GenExecClass):
    def __init__(self, index=None, dxe_file=None, file_ext='.dxe', map_ext='.map.xml', map_type='cces', preload_file=None, baf_logger='BAF_logger.', heap_log_buffer='heap_log_buffer.', isAudioDxe=True, genpgo=False):
        super().__init__(index=index, exec_file=dxe_file, file_ext=file_ext, map_ext=map_ext, map_type=map_type, preload_file=preload_file, baf_logger=baf_logger, heap_log_buffer=heap_log_buffer, isAudioExec=isAudioDxe)
        self.dxe_file = dxe_file
        if self.isAudioExec:
            if genpgo:
                self.config_pgo_loggers(self.map_file_obj)

    def config_pgo_loggers(self, map_file_obj):
        self.baf_pgo_symbol = map_file_obj.get_symbol('data_baf_pgo.')
        self.baf_pgo_file = 'Baf_pgo.pgo'
        self.baf_pgt_symbol = map_file_obj.get_symbol('data_baf_pgt.')
        self.baf_pgt_file = 'Baf_pgo.pgt'
        self.ram_files_symbol = map_file_obj.get_symbol('g_ram_files.')
        self.ram_files_file = 'RamFiles.bin'

    def get_accel_processes(self, work_dir, work_prefix, debug=False):
        if self.index == None:
            work_prefix = work_prefix + '.'
        else:
            work_prefix = work_prefix + '_' + self.index + '.'
        logger_suffix = 'print.log'
        logger_file_path = os.path.join(work_dir, work_prefix+logger_suffix)
        accel_suffix = 'accel.json'
        accel_file_path = os.path.join(work_dir, work_prefix+accel_suffix)

        thread_ids = {}
        with open(logger_file_path, 'r') as fp:
            line = fp.readline()
            while line:
                line = line.strip()
                if (len(line) > 0) and (line.startswith('Id')):
                    lns = line.split()
                    if (len(lns) == 4):
                        thread_id = lns[1]
                        process_id = lns[2]
                        tcb_address =  lns[3]
                        symbol = self.symbol_from_address(tcb_address, '_codeWorkMemory')
                        if symbol != None:
                            accel_process = {'symbol_name' :  symbol.name,
                               'demangled' : symbol.demangled,
                               'start_address': symbol.address,
                               'tcb_address': tcb_address,
                               'size': symbol.size,
                               'thread_id': thread_id,
                               'process_id': process_id }

                            if thread_id not in thread_ids:
                                thread_ids[thread_id] = {}

                            thread_ids[thread_id][process_id] = accel_process
                line = fp.readline()

        json_object = json.dumps(thread_ids, indent=4, sort_keys=True)
        with open(accel_file_path, 'w') as fp:
            fp.write(json_object)

    def process_pgo_dumps(self, work_dir, work_prefix, toolspath, pgo_file, debug=False):
        dxe_folder = os.path.dirname(self.exec_file)

        dtype = np.dtype('uint32')
        with open(self.ram_files_file, 'rb') as f:
            numpy_data = np.fromfile(f,dtype)
        pgo_file_len = numpy_data[5*5+3]
        pgt_file_len = numpy_data[6*5+3]

        if self.index == None:
            work_prefix = work_prefix + '.'
        else:
            work_prefix = work_prefix + '_' + self.index + '.'

        ram_files_path = os.path.join(work_dir, work_prefix+'RamFiles.bin')
        shutil.move(self.ram_files_file, ram_files_path)

        with open(self.baf_pgo_file, 'rb') as f:
            pgo_raw = f.read(pgo_file_len)

        pgo_file_path = os.path.join(work_dir, 'Baf_pgo.pgo')
        with open(pgo_file_path, 'wb') as f:
            f.write(pgo_raw)
        shutil.copy(pgo_file_path, dxe_folder)

        pgo_file_path = os.path.join(work_dir, 'Baf_pgo.bin')
        shutil.move(self.baf_pgo_file, pgo_file_path)

        with open(self.baf_pgt_file, 'rb') as f:
            pgt_raw = f.read(pgt_file_len)

        pgt_file_path = os.path.join(work_dir, 'Baf_pgo.pgt')
        with open(pgt_file_path, 'wb') as f:
            f.write(pgt_raw)
        shutil.copy(pgt_file_path, dxe_folder)

        pgt_file_path = os.path.join(work_dir, 'Baf_pgt.bin')
        shutil.move(self.baf_pgt_file, pgt_file_path)

        run_cmd = toolspath + 'SHARC/etc/pgo.exe Baf_pgo.pgt'
        print (run_cmd)
        try:
            result = subprocess.check_output(run_cmd,
                                 cwd=dxe_folder,
                                 stderr=subprocess.STDOUT,
                                 universal_newlines=True)
        except subprocess.CalledProcessError as e:
            print ("command '{}' return with error (code {}): {}".format(e.cmd, e.returncode, e.output))
        else:
            print( result )

        os.makedirs(os.path.dirname(pgo_file), exist_ok = True)
        pgo_file_path = os.path.join(dxe_folder, 'Baf_pgo.pgo')
        shutil.copy(pgo_file_path, pgo_file)


#*********************************************************************************
# HiFiElfClass
#*********************************************************************************
class HiFiElfClass(GenExecClass):
    def __init__(self, index=None, exec_file=None, file_ext='.elf', map_ext='.map', map_type='gnu', baf_logger='BAF_logger', heap_log_buffer='heap_log_buffer', isAudioExec=True):
        super().__init__(index=index, exec_file=exec_file, file_ext=file_ext, map_ext=map_ext, map_type=map_type, baf_logger=baf_logger, heap_log_buffer=heap_log_buffer, isAudioExec=isAudioExec)

    def create_jlink_read_script(self, script_name, log_dir, audio_dir):
        audioOut_symbol = self.get_symbol('data_out_binary')
        baf_logger_filename = log_dir+'/baf_logger.bin'
        heap_log_buffer_filename = log_dir+'/heap_log_buffer.bin'
        with open(script_name, 'w') as jlink_cmd_f:
            jlink_cmd_f.write('Device MIMXRT685S_M33\nLE\nSelectInterface 1\nSpeed auto\nConnect\n')
            jlink_cmd_f.write('SaveBin {} {} {}\n'.format(baf_logger_filename, self.baf_logger_symbol.address, hex(int(self.baf_logger_symbol.size))))
            jlink_cmd_f.write('SaveBin {} {} {}\n'.format(heap_log_buffer_filename, self.heap_log_buffer_symbol.address, hex(int(self.heap_log_buffer_symbol.size))))
            jlink_cmd_f.write('SaveBin {} {} {}\n'.format(audio_dir+'/audioOut.bin', audioOut_symbol.address, hex(int(audioOut_symbol.size))))
            jlink_cmd_f.write('Exit\n')


#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()


#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    baf_logger = None
    heap_log_buffer = None

    try:
        opts, args = getopt.getopt(argv, "hb:l:", ["help", "baf=", "log="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-b", "--baf"):
            baf_logger = os.path.abspath(arg)
        elif opt in ("-l", "--log"):
            heap_log_buffer = os.path.abspath(arg)

    if (baf_logger == None) or (heap_log_buffer == None):
        usage()
        sys.exit()

    if not os.path.isfile(baf_logger):
        print ( 'Error: baf_logger.bin ' + baf_logger + ' could not be found' )
        sys.exit()

    if not os.path.isfile(heap_log_buffer):
        print ( 'Error: heap_log_buffer.bin ' + heap_log_buffer + ' could not be found' )
        sys.exit()

    baf_logger_bytes = None
    with open(baf_logger, "rb") as f:
        baf_logger_bytes = bytearray(f.read())

    heap_log_buffer_bytes = None
    with open(heap_log_buffer, "rb") as f:
        heap_log_buffer_bytes = bytearray(f.read())

    bl = BAF_logger(baf_logger_bytes, debug=True)

    buffer_bytes = {}
    for logger_id in bl.get_logger_ids():
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        buffer_bytes[logger_id] = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
        print ( '%d %d %d' % (buffer_start, buffer_len, len(buffer_bytes[logger_id])) )
        baf_printf = Logger_dump(bl, logger_id, buffer_bytes[logger_id])
        for x in baf_printf:
            print ( x )

#*********************************************************************************
if __name__ == "__main__":
    main()

