import sys
import os



LEVEL1 = 1 # print nothing
LEVEL2 = 2 # print error
LEVEL3 = 3 # print warning
LEVEL4 = 4 # print general information
LEVEL5 = 5 # print all debug information

PRINT_COMMANDLINE = 1
PRINT_FILE = 2


JAG_ERROR = 'ERROR'
JAG_WARNING = 'WARNING'
JAG_GENERAL = 'GENERAL'
JAG_DEBUG = 'DEBUG'



debuglevel = LEVEL2
debugtype = PRINT_COMMANDLINE

def Jlog_init(debug_level = LEVEL2, debug_type = PRINT_COMMANDLINE):
    global debuglevel, debugtype
    debuglevel = debug_level
    debugtype = debug_type

def Jlog_print(msgtype=None, msg=None):
    if msgtype == JAG_ERROR and debuglevel >= LEVEL2:
        Jlog_output(msgtype + ': ' + msg)
    elif msgtype == JAG_WARNING and debuglevel >= LEVEL3:
        Jlog_output(msgtype + ': ' + msg)
    elif msgtype == JAG_GENERAL and debuglevel >= LEVEL4:
        Jlog_output(msgtype + ':' + msg)
    elif msgtype == JAG_DEBUG and debuglevel >= LEVEL5:
        Jlog_output(msgtype + ':' + msg)

def Jlog_output(msg=None):
    print(msg)

def Jlog_deinit():
    pass
