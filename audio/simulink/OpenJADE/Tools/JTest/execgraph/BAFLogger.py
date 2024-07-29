import os
import getopt
import sys
import struct
import copy
from collections import OrderedDict

__usage__ = """ \

Usage:
  BAFLogger.py [-h] -b baf_logger -l heap_log_buffer

Description:
   - Parses baf_logger.bin and heap_log_buffer.bin

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -b,   --baf   baf_logger
  -l,   --log   heap_log_buffer

"""

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
    logger_template = OrderedDict([
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
    sizeof_Heap = 12
    sizeof_Logger = 32

    logger_types = [ 'LOG_TYPE_PRINT',
                     'LOG_TYPE_CAPTURE',
                     'LOG_TYPE_EXEC']

    def __init__(self, buffer, debug=False):
        self.debug = debug

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
                if data_size == 4:
                    logger[member_id][3] = struct.unpack('<I', buffer[buffer_idx+offset: buffer_idx+offset+data_size])[0]
                elif data_size == 1:
                    logger[member_id][3] = buffer[buffer_idx+offset]

                if self.debug:
                    print ( member_id + ': ' + hex(logger[member_id][3]) )

            buffer_idx = buffer_idx + self.sizeof_Logger

        self.valid = True

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
#    void Logger_dump(Logger_Type id)
#    {
#       Logger* this = &BAF_logger[id];
#       Log* log;
#
#       BAF_printf("LOG%d %d %d %d %08X\n", id, this->enabled, this->circular, this->wrapped, this->current - this->start);
#       if(this->start) {
#           if ( this->circular && this->wrapped ){
#               for ( log = this->current; log < this->end-1; log++ ) {
#                   BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
#                                       log->record.sender_id, log->record.arg_byte, log->record.arg_word );
#               }
#           }
#           for ( log = this->start; log < this->current; log++) {
#               BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
#                                       log->record.sender_id, log->record.arg_byte, log->record.arg_word );
#               if ( log->record.msg[0] == 'C' ) {  // capture log
#                   uint32_t size = log->record.arg_word;
#                   log++;
#                   for ( uint32_t i = 0 ; i < size; i+=sizeof(Log)/sizeof(float), log++ )  {
#                       BAF_printf(" - %.3f\n", log->flt[0]);
#                       BAF_printf(" - %.3f\n", log->flt[1]);
#                   }
#                   log--;
#               }
#           }
#       }
#    }
def Logger_dump(baf_logger, logger_id, buffer):
    baf_printf = []
    id = list(baf_logger.get_logger_ids()).index(logger_id)
    this_enabled = baf_logger.get_logger_member(logger_id, 'enabled')
    this_circular = baf_logger.get_logger_member(logger_id, 'circular')
    this_wrapped = baf_logger.get_logger_member(logger_id, 'wrapped')
    this_current = baf_logger.get_current_offset(logger_id)
    this_end = baf_logger.get_end_offset(logger_id) - Log.sizeof_Log
    this_start = 0
    sizeof_float = 4
    capture_inc = Log.sizeof_Log/sizeof_float

    baf_printf.append( 'LOG%d %d %d %d %08X' % (id, this_enabled, this_circular, this_wrapped, int(this_current/Log.sizeof_Log)) )

    if baf_logger.get_logger_member(logger_id, 'start') == 0:
        return baf_printf

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
        log_curr = log_curr + Log.sizeof_Log
    return baf_printf
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

