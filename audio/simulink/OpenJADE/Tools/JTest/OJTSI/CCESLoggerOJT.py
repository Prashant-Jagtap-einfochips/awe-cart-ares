import os
import getopt
import sys
import struct
import subprocess
import shutil
import json

__usage__ = """ \

Usage:
  CCESLoggerOJT.py [-h] -b board -j json_file -d dxe_files [-p preload_files] -w work_dir

Description:

Assumptions:

Options:
  -h,   --help   Display usage information (this message)
  -b,   --board  Board
  -j,   --json   execgraph.json file with graph parameters
  -d,   --dxe    dxe_files
  -p,   --pre    preload files
  -w,   --work   work_dir

  if there are more than one execgraph.json, dxe or preload files supply a semicolon separated list
  if there is no preload file associated with a dxe file specify 'null'

"""
import OJTSI.BAFLoggerOJT
import OJTSI.PlotExecGraphOJT

g_known_boards = ['gul_sim', 'gul_ezkit', 'gul_ezkit_rta', 'alpine_board', 'spetter_board', 'melville_board']

class CCESRunner:
    gul_toolspath = 'C:/Analog Devices/CrossCore Embedded Studio 2.9.1/'
    gul_preload   = gul_toolspath + 'SHARC/ldr/ezkit21569_preload.dxe'

    sc584_toolspath = 'C:/Analog Devices/CrossCore Embedded Studio 2.6.0/'
    sc584_preload   = sc584_toolspath + 'SHARC/ldr/ezkitSC584_preload_core0_v01'

    cces_runner   = 'CCES_Runner.exe'

    def __init__(self, platform_board, dxe_file, preload_file, work_dir):
        self.valid = True
        self.dxe_file = dxe_file
        self.preload_file = preload_file

        self.platform_board = platform_board
        if not platform_board in g_known_boards:
            print ('Error: unknown PLATFORM board' + platform_board)
            print ('Info: Known Platform boards\t' +  ' '.join(g_known_boards))
            self.valid = False
            return False

        self.dxe_objs = []

        if self.platform_board in ['gul_ezkit','gul_ezkit_rta']:
            self.toolspath = self.gul_toolspath
            dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(index=None, dxe_file=dxe_file[0], preload_file=self.gul_preload, heap_log_buffer='heap_log_buffer.')
            if dxe_obj.isValid():
                self.dxe_objs.append(dxe_obj)
            else:
                self.valid = False
                return

        elif self.platform_board == 'alpine_board':
            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(index=str(index), dxe_file=d, preload_file=p, heap_log_buffer='heap_baf_log.')
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return
        elif self.platform_board == 'melville_board':
            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                #dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(index=str(index), dxe_file=d, preload_file=p, heap_log_buffer='heap_baf_log.')
                dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(index=str(index), dxe_file=d, preload_file=p, heap_log_buffer='heap_log_buffer.')
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return
        elif self.platform_board == 'spetter_board':
            if len(dxe_file) != 5:
                print('Error: spetter_board expects five executables')
                self.valid = False
                return

            self.toolspath = self.gul_toolspath
            index = 0
            for d,p in zip(dxe_file, preload_file):
                if os.path.isdir(p) or (os.path.basename(p) == 'null'):
                    p = None
                isAudioDxe = False if (index == 2) else True
                dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(index=str(index), dxe_file=d, preload_file=p, heap_log_buffer='heap_log_buffer.', isAudioDxe=isAudioDxe)
                index += 1
                if dxe_obj.isValid():
                    self.dxe_objs.append(dxe_obj)
                else:
                    self.valid = False
                    return
        elif self.platform_board == 'gul_sim':
            self.toolspath = self.gul_toolspath
            dxe_obj = OJTSI.BAFLoggerOJT.DxeClass(dxe_file=dxe_file[0], heap_log_buffer='heap_log_buffer.')
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

        if self.platform_board == 'gul_sim':
            dxe_obj = self.dxe_objs[0]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --processor ADSP-21569'
                        ' --target "ChipFactory Simulator"'
                        ' --platform "ADSP-21569 Functional-Sim"')
            run_cmd += ' --core "0,'+dxe_obj.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"'
        elif self.platform_board in ['gul_ezkit','gul_ezkit_rta']:
            dxe_obj = self.dxe_objs[0]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            if self.platform_board == 'gul_ezkit_rta':
                run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --timeout 30000'
                        ' --processor ADSP-21569'
                        ' --target "Emulation Debug Target"'
                        ' --platform "ADSP-21569 via ICE-2000"')
            else:
                run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --processor ADSP-21569'
                        ' --target "Emulation Debug Target"'
                        ' --platform "ADSP-21569 via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"'
        elif self.platform_board == 'alpine_board':
            dxe_obj_0 = self.dxe_objs[0]
            dxe_obj_1 = self.dxe_objs[1]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --timeout 30000'
                        ' --processor ADSP-21565'
                        ' --target "Emulation Debug Target"'
                        ' --platform "Vega via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_0.preload_file+','+dxe_obj_0.dxe_file+'"'
            run_cmd += ' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_1.preload_file+','+dxe_obj_1.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.baf_logger_file+',binary:le,'+dxe_obj_0.baf_logger_symbol.name+','+dxe_obj_0.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.heap_log_buffer_file+',binary:le,'+dxe_obj_0.heap_log_buffer_symbol.name+','+dxe_obj_0.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.baf_logger_file+',binary:le,'+dxe_obj_1.baf_logger_symbol.name+','+dxe_obj_1.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.heap_log_buffer_file+',binary:le,'+dxe_obj_1.heap_log_buffer_symbol.name+','+dxe_obj_1.heap_log_buffer_symbol.size+',1"'
        elif self.platform_board == 'melville_board':
            dxe_obj_0 = self.dxe_objs[0]
            dxe_obj_1 = self.dxe_objs[1]
            run_cmd = self.toolspath
            run_cmd += self.cces_runner
            run_cmd += (' --cceshome "' + self.toolspath + '"'
                        ' --timeout 30000'
                        ' --processor ADSP-21569'
                        ' --target "Emulation Debug Target"'
                        ' --platform "Melville via ICE-2000"')
            run_cmd += ' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_0.preload_file+','+dxe_obj_0.dxe_file+'"'
            run_cmd += ' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj_1.preload_file+','+dxe_obj_1.dxe_file+'"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.baf_logger_file+',binary:le,'+dxe_obj_0.baf_logger_symbol.name+','+dxe_obj_0.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "0,out,'+dxe_obj_0.heap_log_buffer_file+',binary:le,'+dxe_obj_0.heap_log_buffer_symbol.name+','+dxe_obj_0.heap_log_buffer_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.baf_logger_file+',binary:le,'+dxe_obj_1.baf_logger_symbol.name+','+dxe_obj_1.baf_logger_symbol.size+',1"'
            run_cmd += ' --datafile "1,out,'+dxe_obj_1.heap_log_buffer_file+',binary:le,'+dxe_obj_1.heap_log_buffer_symbol.name+','+dxe_obj_1.heap_log_buffer_symbol.size+',1"'

        elif self.platform_board == 'spetter_board':
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

    def run(self):
        print ('Info:' + self.run_cmd)
        print ("Info: Load 'n Run: " + ';'.join(self.dxe_file) + " ...")
        result = subprocess.check_output(self.run_cmd, stderr=subprocess.STDOUT, universal_newlines=True)
        print( result )
        print ("Info: Load 'n Run Done")

        with open(self.run_log_path, 'w') as f:
            f.write(result)

        print ("Info: Processing logger dumps...")
        for dxe_obj in self.dxe_objs:
            if dxe_obj.isAudioDxe:
                dxe_obj.process_logger_dumps(work_dir=self.work_dir,
                                             work_prefix = os.path.basename(dxe_obj.dxe_file.replace('.dxe','')))

    def baf_logger_paths(self):
        paths = []
        for dxe_obj in self.dxe_objs:
            path.append(dxe_obj.baf_logger_path)
        return paths

    def heap_log_buffer_paths(self):
        paths = []
        for dxe_obj in self.dxe_objs:
            path.append(dxe_obj.heap_log_buffer_path)
        return paths

#*********************************************************************************
#
#*********************************************************************************
def _cces_logger(params):
    work_dir = params['work_dir']
    platform_board = params['board']
    dxe_files = params['dxe_files']
    preload_files = params['preload_files']

    cr_obj = CCESRunner(platform_board, dxe_files, preload_files, work_dir)
    if cr_obj.isValid():
        cr_obj.run()
    else:
        print ( 'Error: failed to create/run CCESRunner' )
        return

    baf_logger = []
    heap_log_buffer = []
    exec_graph_json = []
    for e,d in zip(params['exec_graph_json'], cr_obj.dxe_objs):
        if os.path.isdir(e) or (os.path.basename(e) == 'null'):
            continue
        else:
            baf_logger.append(d.baf_logger_path)
            heap_log_buffer.append(d.heap_log_buffer_path)
            exec_graph_json.append(e)
    params['baf_logger'] = baf_logger
    params['heap_log_buffer'] = heap_log_buffer
    params['exec_graph_json'] = exec_graph_json

    params['mips_log'] = os.path.join(work_dir, 'mips.csv')
    params['times_log'] = os.path.join(work_dir, 'times.log')
    params['execgraph_html'] = os.path.join(work_dir, 'exec_graph.html')

    OJTSI.PlotExecGraphOJT.plot_exec_graph_multi( params )

    print ("Info: complete results in %s" % (work_dir))

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
    board = None
    exec_graph_json = None
    dxe_files = None
    preload_files = None
    work_dir = None

    try:
        opts, args = getopt.getopt(argv, "hb:j:d:p:w:", ["help", "board=", "json=", "dxe=", "pre=", "work="])
    except getopt.GetoptError:
        usage()
        return
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            return
        elif opt in ("-b", "--board"):
            board = arg
        elif opt in ("-j", "--json"):
            exec_graph_json = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-d", "--dxe"):
            dxe_files = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-p", "--pre"):
            preload_files = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-w", "--work"):
            work_dir = os.path.abspath(arg)

    if (board == None) or (exec_graph_json == None) or  (dxe_files == None) or (work_dir == None):
        usage()
        return

    if (len(dxe_files) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), dxe_files (%d)' % (len(exec_graph_json), len(dxe_files)))
        usage()
        return

    if not os.path.isdir(work_dir):
        print ( 'Error: work_dir ' + work_dir + ' could not be found' )
        return

    params = {'exec_graph_json':exec_graph_json,
            'dxe_files':dxe_files,
            'preload_files':preload_files,
            'board':board,
            'work_dir':work_dir}

    _cces_logger(params)

#*********************************************************************************
if __name__ == "__main__":
    main()

