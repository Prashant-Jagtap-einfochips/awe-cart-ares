#*********************************************************************************
#  Global Definitions
#
#*********************************************************************************
from xml.dom.minidom import parse
import os
import sys
import getopt

__usage__ = """ \

Usage:
  SizeC6678.py [-h] -t toolspath -e exe_file -l size_log

Description:
   - Runs ofd6x on the specified EXE "exe_file"
   - Parses the output xml file for code, data and bss
   - Logs the output into specified "size_log"

Assumptions:


Options:
  -h,   --help  Display usage information (this message)
  -t,   --tools Path containing ofd6x 
  -e,   --exe   EXE File to be loaded
  -l,   --log   Log file to be created

"""

g_textsection = ['.text','vectors']
g_datasection = ['.const','.data','.rodata','.fardata','.neardata','.switch','.pinit','.oj_ext_data']
g_bsssection = ['.bss','.far','.cio','.csldata']
g_ignoresection = ['.args','.sysmem','.sysstack','.stack','.ioport','.cinit',
    '$build.attributes','.debug_frame','.debug_abbrev','.debug_info','.debug_line',
    '.debug_pubnames','.debug_aranges','.debug_str','.debug_pubtypes',
    '.c6xabi.attributes','.symtab','.TI.section.flags','.strtab','.shstrtab']


#*********************************************************************************
# Function: log_size(shList, logPath, sexePath)
# Description:
#     1. Given a list containing the name and size of the output sections, groups
#        them into three categories (code, data and bsz)
#     2. Write the totals in each category and the overall total to the specified
#        log file.
#
#*********************************************************************************
def isInSection(s, sec_list):
    if s in sec_list:
        return True
    for sec in sec_list:
        if sec in s:
            return True
    return False
    
def log_size(shList, logPath, sexePath):

    bsz_size = 0
    code_size = 0
    data_size = 0
    
    for sh in shList:
        sh0 = sh[0]
        if sh0 == '':
            continue
        if isInSection(sh0, g_textsection):
            code_size = code_size + int(sh[1],16)
        elif isInSection(sh0, g_datasection):
            data_size = data_size + int(sh[1],16)
        elif isInSection(sh0, g_bsssection):
            bsz_size = bsz_size + int(sh[1],16)
        elif isInSection(sh0, g_ignoresection):
            pass
        else:
            print ('Warning: unknown section '+ sh0)
            
    total_size = code_size + data_size + bsz_size;
    
    logFile = open(logPath, 'w')
    logFile.write('   text\t   data\t    bss\t    dec\t    hex\tfilename\n')
    logFile.write('%(#)7d\t' % {"#":code_size})
    logFile.write('%(#)7d\t' % {"#":data_size})
    logFile.write('%(#)7d\t' % {"#":bsz_size})
    logFile.write('%(#)7d\t' % {"#":total_size})
    logFile.write('%(#)7x\t' % {"#":total_size})        
    logFile.write(sexePath+'\n')
    logFile.close()

#*********************************************************************************
# Function: parse_ofd6x(ofd6x, sexePath)
# Description:
#     1. Runs ofd6x on the supplied exe and generates a xml output
#     2. Parses the xml output. Goes down the xml tree until section header
#        information is found.
#     3. Creates a list containing the name and size of the output sections
#
#*********************************************************************************
def is_sectiontype(shel, type):
    nl = shel.getElementsByTagName(type)
    if nl.length == 0:
        return 0
    if nl[0].firstChild.data == 'true':
        return 1
    return 0
def parse_ofd6x(ofd6x, sexePath):   
    tmpfile = 'a.xml'   
    cmd = '"'+ofd6x+'"' + ' -x -o=' + tmpfile + ' ' + sexePath
    os.system(cmd)
    
    if not os.path.isfile(tmpfile):
        print ('Error: Unsuccessful ofd6x run')
        sys.exit()
    
    try:
        elfDOM = parse(tmpfile)
    except (IOError):
        print ('Error: ' + tmpfile + ' not found')
        sys.exit(2)     
        
    try:
        os.remove(tmpfile)
    except:
        print ('Warning: ' + tmpfile + ' could not be removed')
    
    ofdDOM = elfDOM.getElementsByTagName("ofd")[0]
    object_fileDOM = ofdDOM.getElementsByTagName("object_file")[0]
    elf_DOM = object_fileDOM.getElementsByTagName("elf")[0]
    section_tableDOM = elf_DOM.getElementsByTagName("section_table")[0]
    shels = section_tableDOM.getElementsByTagName("section")
    
    shList = []
    for shel in shels:
        if shel.parentNode != section_tableDOM:
            continue
        section_size = shel.getElementsByTagName("sh_size")[0].firstChild.data
        if int(section_size,16) == 0:
            continue        
        section_name = shel.getElementsByTagName("sh_name_string")[0].firstChild.data
        tsh = []
        tsh.append(section_name)
        tsh.append(section_size)
        shList.append(tsh)
    return shList
#*********************************************************************************
# Function: verify_paths(aexePath, logPath)
# Description: Verifies the following:
#      1. Correctness of the exe path (aexePath)
#      2. Existence of the directory for the logfile
#
#*********************************************************************************
def verify_paths(aexePath, logPath):
    if (aexePath == None) or (logPath == None):
        usage()
    
    logDir = os.path.dirname(logPath)
    #check for valid aexePath and logDir
    if (not os.path.isfile(aexePath)):
        print ('Error: ' + aexePath + ' could not be found')
        sys.exit()
    if (not os.path.isdir(logDir)):
        print ('Error: ' + logDir + ' could not be found')
        sys.exit()
    
#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()
    
#*********************************************************************************
# Function: dsp_memory_usage
#
#*********************************************************************************
def dsp_memory_usage(tools_path, exe_file, size_log, debug=False):
    ofd6x = os.path.join(tools_path, 'ofd6x.exe')
    shList = parse_ofd6x(ofd6x, exe_file)
    log_size(shList, size_log, exe_file)

#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    aexePath = None
    logPath = None
    toolsPath = None
    ofd6x = 'ofd6x'
    
    try:
        opts, args = getopt.getopt(argv, "ht:e:l:", ["help", "tools=", "exe=", "log="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-t", "--tools"):
            toolsPath = arg;
        elif opt in ("-e", "--exe"):
            sexePath = arg;
            aexePath = os.path.abspath(arg)
        elif opt in ("-l", "--log"):
            logPath = os.path.abspath(arg)
    if toolsPath == None:
        ofd6x = 'ofd6x.exe'
    else:
        ofd6x = os.path.join(toolsPath, 'ofd6x.exe')
           
    verify_paths(aexePath, logPath)
    shList = parse_ofd6x(ofd6x, sexePath)
    log_size(shList, logPath, sexePath)
    
    sys.exit()
#*********************************************************************************
if __name__ == "__main__":
    main()