import os
import getopt
import sys
import struct
import xml.etree.ElementTree as et
import subprocess
import shutil

import execgraph.BAFLogger
import execgraph.OJExecLog

from execgraph.ExecGraphPlatform import ExecGraphPlatform

__usage__ = """ \

Usage:
  CCESLogger.py [-h] -x platform_xml -d dxe_file [-p preload_dxe_file] -o work_dir

Description:
   - For a given platform, uses CCES_Runner to load the
     supplied <testname>.dxe on to the platform.
     The <testname>.dxe is ran until breakpoint or termination.
     When the <testname>.dxe halts, dumps memory corresponding to
     BAF_logger and heap_log_buffer.
   - Memory dumps of BAF_logger and heap_log_buffer are
     placed in the baf_logger.bin and heap_log_buffer.bin files in the work_dir folder.
   - <testname>.run.log file is also kept in the work_dir folder
   - Memory dumps are analyzed to print the MIPS report.
     Following files are kept in the work_dir folder
     <testname>.mipsreport.csv
     <testname>.timeobjects.csv

Assumptions:
   - It is assumed <testcase>.map.xml is also present in the path containing <testname>.dxe

Options:
  -h,   --help   Display usage information (this message)
  -x,   --xml    Platform XML file
  -d,   --dxe    dxe_file
  -p,   --pre    preload_dxe_file
  -w,   --work   work_dir

"""

g_known_platforms = ['JTestHIL', 'JTestPIL', 'JTestRTA', 'VegaAlpine', 'Spetter', 'Melville']
g_known_boards = ['gul_sim', 'gul_ezkit', 'alpine_board', 'spetter_board', 'melville_board']

class MapSymbol:
    def __init__(self, name, address, size):
        self.name = name
        self.address = address
        self.size = str(int(size,16))

class MapFile:
    def __init__(self, map_file):
        xml_tree = et.parse(map_file)
        self.xml_root = xml_tree.getroot()
        self.map_file = map_file

    def get_symbol(self, map_symbol_name):
        ms = self.xml_root.findall(".//SYMBOL[@name='" + map_symbol_name + "']")
        if len(ms) > 0:
            map_symbol = MapSymbol(map_symbol_name,
                                   ms[0].get('address'),
                                   ms[0].get('size'))
        else:
            map_symbol = None
            print ( 'Error: map_symbol ' + map_symbol_name + ' could not be found in ' + self.map_file)
        return map_symbol

class DxeClass:
    def __init__(self, platform, index, dxe_file, preload_file):
        self.platform = platform
        self.index = index
        self.valid = True

        self.dxe_file = dxe_file
        self.map_file = dxe_file.replace('.dxe','.map.xml') if dxe_file.find('.dxe') != -1 else False
        self.preload_file = preload_file

        if not os.path.isfile(self.dxe_file):
            print ( 'Error: dxe_file ' + self.dxe_file + ' could not be found' )
            self.valid = False
            return

        if self.map_file and not os.path.isfile(self.map_file):
            print ( 'Error: map_file ' + self.map_file + ' could not be found' )
            self.valid = False
            return

        if not os.path.isdir(self.preload_file):
            if not os.path.isfile(self.preload_file):
                print ( 'Error: preload_file ' + self.preload_file + ' could not be found' )
                self.valid = False
                return

        map_file_obj = MapFile(self.map_file) if self.map_file else False

        if self.platform['type'] in ['JTestPIL']:
            if not self.config_baf_logger(map_file_obj,'BAF_logger.','baf_logger.bin'):
                self.valid = False
                return

            if not self.config_heap_log_buffer(map_file_obj,'heap_log_buffer.','heap_log_buffer.bin'):
                self.valid = False
                return

        elif self.platform['type'] in ['VegaAlpine']:
            if not self.config_baf_logger(map_file_obj,'BAF_logger.','baf_logger_'+self.index+'.bin'):
                self.valid = False
                return

            if not self.config_heap_log_buffer(map_file_obj,'heap_baf_log.','heap_log_buffer_'+self.index+'.bin'):
                self.valid = False
                return

        elif self.platform['type'] in ['Spetter']:
            if map_file_obj:
                if not self.config_baf_logger(map_file_obj,'BAF_logger.','baf_logger_'+self.index+'.bin'):
                    self.valid = False
                    return

                if not self.config_heap_log_buffer(map_file_obj,'heap_log_buffer.','heap_log_buffer_'+self.index+'.bin'):
                    self.valid = False
                    return

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

    def isValid(self):
        return self.valid

class CCESRunner:
    gul_toolspath = 'C:/Analog Devices/CrossCore Embedded Studio 2.9.1/'
    gul_preload   = gul_toolspath + 'SHARC/ldr/ezkit21569_preload.dxe'

    sc584_toolspath = 'C:/Analog Devices/CrossCore Embedded Studio 2.6.0/'
    sc584_preload   = sc584_toolspath + 'SHARC/ldr/ezkitSC584_preload_core0_v01'

    cces_runner   = 'CCES_Runner.exe'

    def __init__(self, platform, dxe_file, preload_file, work_dir):
        self.valid = True
        self.dxe_file = dxe_file
        self.preload_file = preload_file

        self.platform = platform
        if not self.verify_platform():
            self.valid = False
            return

        self.dxe_objs = []

        if self.platform['board'] == 'gul_ezkit':
            self.toolspath = self.gul_toolspath
            dxe_obj = DxeClass(self.platform, '0', dxe_file[0], self.gul_preload)
            if dxe_obj.isValid():
                self.dxe_objs.append(dxe_obj)
            else:
                self.valid = False
                return

        elif self.platform['board'] == 'alpine_board':
            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                dxe_obj = DxeClass(self.platform, str(index), d, p)
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return

        elif self.platform['board'] == 'melville_board':
            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                dxe_obj = DxeClass(self.platform, str(index), d, p)
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return

        elif self.platform['board'] == 'spetter_board':
            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                dxe_obj = DxeClass(self.platform, str(index), d, p)
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return

        elif self.platform['board'] == 'gul_sim':
            self.toolspath = self.gul_toolspath
            dxe_obj = DxeClass(self.platform, dxe_file[0], None)
            if dxe_obj.isValid():
                self.dxe_objs.append(dxe_obj)
            else:
                self.valid = False
                return

        if not self.verify_toolspath():
            self.valid = False
            return

        self.work_dir = work_dir
        if not self.verify_workdir():
            self.valid = False
            return

        self.run_log_path = os.path.join(self.work_dir, 'cceslogger.run.log')

        if not self.config_runcmd():
            self.valid = False
            return

        run_cmd = self.run_cmd

    def isValid(self):
        return self.valid

    def verify_platform(self):
        platform_type = self.platform['type']
        platform_board = self.platform['board']
        if not platform_board in g_known_boards:
            print ('Error: unknown PLATFORM board ' + platform_board)
            print ('Info: Known Platform boards\t' +  ' '.join(g_known_boards))
            return False

        if not platform_type in g_known_platforms:
            print ('Error: unknown PLATFORM type ' + platform_type)
            print ('Info: Known Platform types\t' +  ' '.join(g_known_platforms))
            return False

        return True

    def verify_workdir(self):
        if not os.path.isdir(self.work_dir):
            print ( 'Error: work_dir ' + self.work_dir + ' could not be found' )
            return False
        return True

    def verify_toolspath(self):
        if not os.path.isdir(self.toolspath):
            print ( 'Error: toolspath ' + self.toolspath + ' could not be found' )
            return False

        cces_runner = self.toolspath + self.cces_runner

        if not os.path.isfile(cces_runner):
            print ( 'Error:  cces_runner ' + cces_runner + ' could not be found' )
            return False

        return True

    def config_runcmd(self):
        status = True

        if self.platform['board'] == 'gul_sim':
            dxe_obj = self.dxe_objs[0]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --processor ADSP-21569'
                        ' --target "ChipFactory Simulator"'
                        ' --platform "ADSP-21569 Functional-Sim"')
            run_cmd += ' --core "0,'+dxe_obj.dxe_file+'"'
            if self.platform['type'] == 'JTestPIL':
                run_cmd += ' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"'
                run_cmd += ' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"'
        elif self.platform['board'] == 'gul_ezkit':
            dxe_obj = self.dxe_objs[0]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --processor ADSP-21569'
                        ' --target "Emulation Debug Target"'
                        ' --platform "ADSP-21569 via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"'
            if self.platform['type'] == 'JTestPIL':
                run_cmd += ' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"'
                run_cmd += ' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"'
            elif self.platform['type'] in ['JTestHIL','JTestRTA']:
                run_cmd += ' -b "0,main"'
        elif self.platform['board'] == 'alpine_board':
            dxe_obj_0 = self.dxe_objs[0]
            dxe_obj_1 = self.dxe_objs[1]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --timeout 5000'
                        ' --processor ADSP-21565'
                        ' --target "Emulation Debug Target"'
                        ' --platform "Vega via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|SYMBOLS_ONLY|NO_RESET_BEFORE_LOAD|RUN_AFTER_LOAD|'+dxe_obj_0.dxe_file+'"'
            run_cmd += ' --core "1,EMU|KIT|CLEAR_SYMBOLS|SYMBOLS_ONLY|NO_RESET_BEFORE_LOAD|RUN_AFTER_LOAD|'+dxe_obj_1.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.baf_logger_file+',binary:le,'+dxe_obj_0.baf_logger_symbol.name+','+dxe_obj_0.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.heap_log_buffer_file+',binary:le,'+dxe_obj_0.heap_log_buffer_symbol.name+','+dxe_obj_0.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.baf_logger_file+',binary:le,'+dxe_obj_1.baf_logger_symbol.name+','+dxe_obj_1.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.heap_log_buffer_file+',binary:le,'+dxe_obj_1.heap_log_buffer_symbol.name+','+dxe_obj_1.heap_log_buffer_symbol.size+',1"'
        elif self.platform['board'] == 'melville_board':
            dxe_obj_0 = self.dxe_objs[0]
            dxe_obj_1 = self.dxe_objs[1]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --timeout 5000'
                        ' --processor ADSP-21569'
                        ' --target "Emulation Debug Target"'
                        ' --platform "Melville via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|SYMBOLS_ONLY|NO_RESET_BEFORE_LOAD|RUN_AFTER_LOAD|'+dxe_obj_0.dxe_file+'"'
            run_cmd += ' --core "1,EMU|KIT|CLEAR_SYMBOLS|SYMBOLS_ONLY|NO_RESET_BEFORE_LOAD|RUN_AFTER_LOAD|'+dxe_obj_1.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.baf_logger_file+',binary:le,'+dxe_obj_0.baf_logger_symbol.name+','+dxe_obj_0.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.heap_log_buffer_file+',binary:le,'+dxe_obj_0.heap_log_buffer_symbol.name+','+dxe_obj_0.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.baf_logger_file+',binary:le,'+dxe_obj_1.baf_logger_symbol.name+','+dxe_obj_1.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.heap_log_buffer_file+',binary:le,'+dxe_obj_1.heap_log_buffer_symbol.name+','+dxe_obj_1.heap_log_buffer_symbol.size+',1"'
        elif self.platform['board'] == 'spetter_board':
            dxe_obj_0 = self.dxe_objs[0]
            dxe_obj_1 = self.dxe_objs[1]
            dxe_obj_2 = self.dxe_objs[2]
            dxe_obj_3 = self.dxe_objs[3]
            dxe_obj_4 = self.dxe_objs[4]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' -v2'
                        ' --timeout 10000'
                        ' --processor ADSP-SC584'
                        ' --target "Emulation Debug Target"'
                        ' --platform "Mule Board via ICE-2000"')
            run_cmd += ' --core "0,EMU|ANY|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_0.preload_file+','+dxe_obj_0.dxe_file+'"'
            run_cmd += ' --core "1,EMU|ANY|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_1.dxe_file+'"'
            run_cmd += ' --core "2,EMU|ANY|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_2.preload_file+','+dxe_obj_2.dxe_file+'"'
            run_cmd += ' --core "3,EMU|ANY|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_3.dxe_file+'"'
            run_cmd += ' --core "4,EMU|ANY|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_4.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.baf_logger_file+',binary:le,'+dxe_obj_0.baf_logger_symbol.name+','+dxe_obj_0.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.heap_log_buffer_file+',binary:le,'+dxe_obj_0.heap_log_buffer_symbol.name+','+dxe_obj_0.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.baf_logger_file+',binary:le,'+dxe_obj_1.baf_logger_symbol.name+','+dxe_obj_1.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.heap_log_buffer_file+',binary:le,'+dxe_obj_1.heap_log_buffer_symbol.name+','+dxe_obj_1.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "3,out,'+dxe_obj_3.baf_logger_file+',binary:le,'+dxe_obj_3.baf_logger_symbol.name+','+dxe_obj_3.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "3,out,'+dxe_obj_3.heap_log_buffer_file+',binary:le,'+dxe_obj_3.heap_log_buffer_symbol.name+','+dxe_obj_3.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "4,out,'+dxe_obj_4.baf_logger_file+',binary:le,'+dxe_obj_4.baf_logger_symbol.name+','+dxe_obj_4.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "4,out,'+dxe_obj_4.heap_log_buffer_file+',binary:le,'+dxe_obj_4.heap_log_buffer_symbol.name+','+dxe_obj_4.heap_log_buffer_symbol.size+',1"'
        else:
            status = False

        self.run_cmd = run_cmd
        return status

    def get_baf_logger_bytes(self, dxe_obj):
        baf_logger_path = os.path.join(self.work_dir, dxe_obj.baf_logger_file)
        shutil.move(dxe_obj.baf_logger_file, baf_logger_path)

        baf_logger_bytes = None
        with open(baf_logger_path, "rb") as f:
            baf_logger_bytes = bytearray(f.read())
        return baf_logger_bytes

    def get_heap_log_buffer_bytes(self, dxe_obj):
        heap_log_buffer_path = os.path.join(self.work_dir, dxe_obj.heap_log_buffer_file)
        shutil.move(dxe_obj.heap_log_buffer_file, heap_log_buffer_path)

        heap_log_buffer_bytes = None
        with open(heap_log_buffer_path, "rb") as f:
            heap_log_buffer_bytes = bytearray(f.read())
        return heap_log_buffer_bytes

    def run(self):
        print ('Info:' + self.run_cmd)
        print ("Info: Load 'n Run: " + ';'.join(self.dxe_file) + " ...")
        result = subprocess.check_output(self.run_cmd, stderr=subprocess.STDOUT, universal_newlines=True)
        print( result )
        print ("Info: Load 'n Run Done")

        with open(self.run_log_path, 'w') as f:
            f.write(result)

        if self.platform['type'] == 'JTestPIL':
            dxe_obj = self.dxe_objs[0]
            baf_logger_bytes = self.get_baf_logger_bytes(dxe_obj)
            heap_log_buffer_bytes = self.get_heap_log_buffer_bytes(dxe_obj)

            buffer_bytes = {'BAF_logger': baf_logger_bytes,
                            'heap_log_buffer': heap_log_buffer_bytes }

            return [buffer_bytes]

        elif self.platform['type'] in ['VegaAlpine']:
            buffer_bytes_list = []
            for dxe_obj in self.dxe_objs:
                baf_logger_bytes = self.get_baf_logger_bytes(dxe_obj)
                heap_log_buffer_bytes = self.get_heap_log_buffer_bytes(dxe_obj)

                buffer_bytes = {'BAF_logger': baf_logger_bytes,
                                'heap_log_buffer': heap_log_buffer_bytes }
                buffer_bytes_list.append(buffer_bytes)
            return buffer_bytes_list

        elif self.platform['type'] in ['Spetter']:
            buffer_bytes_list = []
            for dxe_obj in self.dxe_objs:
                dxe_file = dxe_obj.dxe_file
                if dxe_file.find('.dxe') != -1: #skip ARM
                    baf_logger_bytes = self.get_baf_logger_bytes(dxe_obj)
                    heap_log_buffer_bytes = self.get_heap_log_buffer_bytes(dxe_obj)

                    buffer_bytes = {'BAF_logger': baf_logger_bytes,
                                    'heap_log_buffer': heap_log_buffer_bytes }
                    buffer_bytes_list.append(buffer_bytes)
            return buffer_bytes_list

        return None

#*********************************************************************************
# Function: _cces_logger(platform_xml, dxe_file, preload_file, work_dir)
# Description: Unit testing function
#
#*********************************************************************************
def _cces_logger(platform_xml, dxe_file, preload_file, work_dir, debug=False):
    exec_platform = ExecGraphPlatform(platform_xml)

    if not exec_platform.isValid():
        print ('Error: failure parsing platform config xml ' + platform_xml)
        return None

    platform_board = exec_platform.get_platform_board()
    platform_type = exec_platform.get_platform_type()

    cr_obj = CCESRunner({'type':platform_type, 'board':platform_board}, dxe_file, preload_file, work_dir)
    if cr_obj.isValid():
        buffer_bytes_list = cr_obj.run()
    else:
        return None

    for buffer_bytes in buffer_bytes_list:
        oj_exec_log = exec_platform.parse_ExecLog(buffer_bytes)
        if debug:
            for x in oj_exec_log:
                print ( x )

        oj_exec_obj = execgraph.OJExecLog.OJExecLog(frame_size=exec_platform.get_frame_size(),
                            sample_rate=exec_platform.get_sample_rate(),
                            clock_rate=exec_platform.get_clock_rate(),
                            input_lines=oj_exec_log)
        oj_exec_obj.run()

        mips_report_path = os.path.join(work_dir, 'cceslogger.mipsreport.csv')
        mips_report_df = oj_exec_obj.get_complete_mips_report_df()
        with open(mips_report_path, "w") as f:
            f.write(mips_report_df.to_csv(line_terminator='\n'))
        print(mips_report_df)

        time_objects_path = os.path.join(work_dir, 'cceslogger.timeobjects.csv')
        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        with open(time_objects_path, "w") as f:
            f.write(time_objects_df.to_csv(line_terminator='\n'))
        print(time_objects_df)

#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    dxe_file = None
    work_dir = None
    plat_xml = None
    preload_file = None

    try:
        opts, args = getopt.getopt(argv, "hx:d:p:w:", ["help", "xml=", "dxe=", "pre=", "work="])
    except getopt.GetoptError:
        usage()
        return
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            return
        elif opt in ("-x", "--xml"):
            plat_xml = os.path.abspath(arg)
        elif opt in ("-d", "--dxe"):
            dxe_file = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-p", "--pre"):
            preload_file = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-w", "--work"):
            work_dir = os.path.abspath(arg)

    if (plat_xml == None) or (dxe_file == None) or (work_dir == None):
        usage()
        return

    _cces_logger(plat_xml, dxe_file, preload_file, work_dir)

#*********************************************************************************
if __name__ == "__main__":
    main()



