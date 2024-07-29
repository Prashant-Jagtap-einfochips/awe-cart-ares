#*********************************************************************************
#  Global Definitions
#
#*********************************************************************************

# libraries
import os
import getopt
import shutil
import sys

import xmlparse.ParseMemXML
from xmlparse.MemoryUsageToHtml import *

__usage__ = """ \

Usage:
  SizeSharcBinary.py [-h] [-x xslt_file] -m map_xml_file -l size_log [-g classifier_group]

Description:
   - Parses the output xml file for code, data and bsz
   - Logs the output into specified "size_log" with .size.log extension.
   - Also outputs into .size.xml file
   - And copies xslt_file (if specified) to same folder as size_log file.


Assumptions:
    Log file to be created must have .size.log extension


Options:
  -h,   --help  Display usage information (this message)
  -x,   --xslt  .xsl file associated with out xml file
  -m,   --map   .map.xml file to be parsed
  -l,   --log   Log file to be created
  -g,   --group classifier group to be used
  -f,   --filelist folder path for filelist

"""



#*********************************************************************************
# Function: verify_paths(mapPath, logPath)
# Description: Verifies the following:
#      1. Correctness of the map file path (mapPath)
#      2. Existence of the directory for the logfile
#
#*********************************************************************************
def verify_paths(mapPath, logPath):
    if (mapPath == None) or (logPath == None):
        print ('Error: .map.xml file and output log file needs to be specified')
        return False

    if (not os.path.isfile(mapPath)):
        print ('Error: .map.xml file ' + mapPath + ' could not be found')
        return False

    logDir = os.path.dirname(logPath)
    if (not os.path.isdir(logDir)):
        print ('Error: output log dir ' + logDir + ' could not be found')
        return False

    return True

#*********************************************************************************
# Function: dsp_memory_usage
#
#*********************************************************************************
def my_int_str(f):
    return str(int(f))

def dsp_memory_usage(xml_path, log_path, xsl_file=None, classifierGroup = 'JTest', debug=False, fl_path=None, classifiers=None):

    if not verify_paths(xml_path, log_path):
        return False

    root_imgs = xmlparse.ParseMemXML.parseMemXML(xml_path, classifierGroup = classifierGroup, debug = debug, fl_path = fl_path, classifiers=classifiers)

    with open(log_path, 'w') as fp:
        for root_img in root_imgs:
            img = root_img.get_img()
            img_memory_totals = img.memory_breakdown()
            for memory_block in img_memory_totals.keys():
                fp.write(memory_block + '\n')
                mb_memory_totals = img_memory_totals[memory_block]
                for memory_section in mb_memory_totals.keys():
                    fp.write('\t' + memory_section)
                    ms_memory_totals = mb_memory_totals[memory_section]
                    fp.write('\tlength: ' + my_int_str(ms_memory_totals['length']) + '\n')
                    if ms_memory_totals['filler'] != 0:
                        fp.write('\t\tfiller\ttotal: ' + my_int_str(ms_memory_totals['filler']) + '\n')

                    for memory_group in ms_memory_totals.keys():
                        if memory_group in ['length','filler'] :
                            continue
                        if 'total' in ms_memory_totals[memory_group].keys():
                            fp.write('\t\t' + memory_group)
                            fp.write('\ttotal: ' + my_int_str(ms_memory_totals[memory_group]['total']) + '\n')
                        else:
                            fp.write('\t\t' + memory_group + '\n')

                        for file_group in ms_memory_totals[memory_group].keys():
                            if file_group == 'total':
                                continue
                            fp.write('\t\t\t' + file_group + ': ' + my_int_str(ms_memory_totals[memory_group][file_group]) + '\n')


    with open(log_path.replace('.size.log','.size.xml'), 'w') as fp:
        fp.write('<?xml version="1.0" encoding="utf-8"?>\n')
        fp.write('<?xml-stylesheet type="text/xsl" href="ojtsi_size.xsl"?>\n')
        fp.write('<root version="1.0">\n')

        for root_img in root_imgs:
            img = root_img.get_img()
            img_memory_totals = img.memory_breakdown()
            for memory_block in img_memory_totals.keys():
                fp.write('<MEMORY_BLOCK id="' + memory_block + '">\n')
                mb_memory_totals = img_memory_totals[memory_block]
                for memory_section in mb_memory_totals.keys():
                    fp.write('\t<MEMORY_SECTION id="' + memory_section + '"')
                    ms_memory_totals = mb_memory_totals[memory_section]
                    fp.write(' length="' + my_int_str(ms_memory_totals['length']) + '">\n')
                    #if ms_memory_totals['filler'] != 0:
                    #    fp.write('\t\t<MEMORY id="filler" total="' + my_int_str(ms_memory_totals['filler']) + '">\n')
                    #    fp.write('\t\t</MEMORY>\n')
                    for memory_group in ms_memory_totals.keys():
                        if memory_group in ['length','filler'] :
                            continue
                        if 'total' in ms_memory_totals[memory_group].keys():
                            fp.write('\t\t<MEMORY id="' + memory_group + '"')
                            fp.write(' total="' + my_int_str(ms_memory_totals[memory_group]['total']) + '">\n')
                        else:
                            fp.write('\t\t<MEMORY id="' + memory_group + '">\n')

                        for file_group in ms_memory_totals[memory_group].keys():
                            if file_group == 'total':
                                continue
                            fp.write('\t\t\t<CONTRIBUTOR id="' + file_group + '" size="' + my_int_str(ms_memory_totals[memory_group][file_group]) + '"/>\n')
                        fp.write('\t\t</MEMORY>\n')
                    fp.write('\t</MEMORY_SECTION>\n')
                fp.write('</MEMORY_BLOCK>\n')
        fp.write('</root>\n\n')

        if (xsl_file != None):
            if xsl_file == 'default':
                xsl_path = os.path.dirname(os.path.realpath(__file__))
                xsl_file = os.path.abspath(os.path.join(xsl_path, 'ojtsi_size.xsl'))
            if (not os.path.isfile(xsl_file)):
                print ('Error: xsl_file ' + xsl_file + ' could not be found')
                return False

            log_dir = os.path.dirname(log_path)
            shutil.copy(xsl_file, log_dir)
    return True

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
    mapPath = None
    logPath = None
    flPath = None
    xsl_file = None
    classifierGroup = 'JTest'

    try:
        opts, args = getopt.getopt(argv, "hx:m:l:g:f:", ["help", "xslt=", "map=", "log=", "group=", "filelist="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-x", "--xslt"):
            if arg == 'default':
                xsl_file = arg
            else:
                xsl_file = os.path.abspath(arg)
        elif opt in ("-m", "--map"):
            mapPath = os.path.abspath(arg)
        elif opt in ("-l", "--log"):
            logPath = os.path.abspath(arg)
        elif opt in ("-g", "--group"):
            classifierGroup = arg
        elif opt in ("-f", "--filelist"):
            flPath = os.path.abspath(arg)
    if dsp_memory_usage(mapPath, logPath, xsl_file=xsl_file, classifierGroup=classifierGroup, fl_path = flPath) == False:
        usage()
    else:
        GenerateHtmlMemoryUsage(logPath)

#*********************************************************************************
if __name__ == "__main__":
    main()