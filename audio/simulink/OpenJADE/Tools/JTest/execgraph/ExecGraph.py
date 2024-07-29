import time, os, threading
import sys
import getopt
import struct
import serial
import pandas as pd

from execgraph.ExecGraphPlatform import ExecGraphPlatform
from execgraph.OJExecLog import OJExecLog
from execgraph.CCESLogger import CCESRunner

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue

__usage__ = """ \

Usage:
  ExecGraph.py [-h] [-c com_port] -x platform_xml -d dxe_file [-p preload_dxe_file] -w work_dir -l log_file

Description:

Options:
  -h,   --help   Display usage information (this message)
  -c,   --com    Valid COM port
  -x,   --xml    Platform XML file
  -d,   --dxe    dxe_file
  -p,   --pre    preload_dxe_file
  -w,   --work   work_dir
  -l,   --log    Output log file
"""

class COMPort:
    def __init__(self, com_port):
        self.com_port = com_port
        # From serialwin32.py by Chris Liechti <cliechti@gmx.net>
        # CSS 20040528 - open wasn't working for COM10 and greater, but by
        # chance the '\\.\COM10' format seems to work, yay!  But, only use
        # if for COM10 and greater in case it introduces some other
        # incompatibility.
        if self.com_port < 10:
            portstr = 'COM%d' % (self.com_port) #numbers are transformed to a string
        else:
            portstr = '\\\\.\\COM%d' % (self.com_port) #WIN NT format??
        self.serial = serial.Serial(
            port = portstr,
            baudrate = 115200,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=0.2)
        return

    def teardown(self):
        self.serial.close()

class KBInputThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self,name='KBInputThread')
        self.join_list = []
        return

    def addToJoinList(self, join_thread):
        if join_thread != None:
            self.join_list.append(join_thread)
        return

    def run(self):
        print ('Info: %s starts' % (self.getName()))
        print ('\tPress x and Enter to exit')
        while not self.stopevent.isSet():
            kb = input()
            if kb.strip() == 'x':
                for jt in self.join_list:
                    if jt.is_alive():
                        jt.join()
                self.stopevent.set()

        print ('Info: %s stops' % (self.getName()))
        return

    def join(self,timeout=None):
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

class COMReaderThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self,name='COMReaderThread')
        self.exec_platform = None
        return

    def configure(self, com_obj, read_queue, logger_queue, debug=False):
        self.serial = com_obj.serial
        self.read_queue = read_queue
        self.logger_queue = logger_queue
        self.debug = debug
        return

    def configureJTestRTA(self, exec_platform, prompt_event):
        self.prompt_event = prompt_event
        self.exec_platform = exec_platform
        return

    def configureJTestHIL(self, exec_platform, read_event):
        self.read_event = read_event
        self.exec_platform = exec_platform
        return

    def run(self):
        print ('Info: %s starts' % (self.getName()))

        if self.exec_platform.get_platform_type() in ['JTestRTA']:
            while not self.stopevent.isSet():
                buf = self.serial.readline()
                if len(buf) > 0:
                    if self.exec_platform.is_prompt(buf):
                        self.prompt_event.set()
                    else:
                        if self.debug:
                            self.logger_queue.put(buf)
                            #sys.stdout.write(buf.decode('utf-8'))
                        self.read_queue.put(buf)
        elif self.exec_platform.get_platform_type() in ['JTestHIL']:
            while not self.stopevent.isSet():
                buf = self.serial.read(size=16*1024)
                if len(buf) > 0:
                    self.read_queue.put(buf)
                    self.read_event.set()

        print ('Info: %s stops' % (self.getName()))
        return

    def join(self,timeout=None):
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

class COMWriterThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self,name='COMWriterThread')
        return

    def configure(self, com_obj, write_queue, logger_queue, debug=False):
        self.serial = com_obj.serial
        self.logger_queue = logger_queue
        self.write_queue = write_queue
        self.debug = debug
        return

    def configureJTestRTA(self, exec_platform, prompt_event):
        self.prompt_event = prompt_event
        self.exec_platform = exec_platform
        return

    def run(self):
        print ('Info: %s starts' % (self.getName()))

        if is_py2:
            log_prefix = 'XMT: '
        else:
            log_prefix = b'XMT: '

        while not self.stopevent.isSet():
            while not self.write_queue.empty():
                buf = self.write_queue.get()

                if len(buf) > 0:
                    self.serial.write(buf)

                    if self.debug:
                        self.logger_queue.put(log_prefix + buf)
                        #wstr = log_prefix + buf
                        #sys.stdout.write(wstr.decode('utf-8'))

                    if self.exec_platform.get_platform_type() in ['JTestRTA']:
                        self.prompt_event.wait()
                        self.prompt_event.clear()

                self.write_queue.task_done()
        print ('Info: %s stops' % (self.getName()))
        return

    def join(self,timeout=None):
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

class ExecGraphThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self,name='ExecGraphThread')
        return

    def configure(self, com_port, exec_platform, log_file, debug=False):
        self.com_port = com_port
        self.exec_platform = exec_platform
        self.log_file = log_file
        self.debug = debug

        self.log_fp = None
        self.com_obj = None

        self.com_reader = None
        self.com_writer = None

        self.logger_queue = None
        self.write_queue = None
        self.read_queue = None

        self.time_data_queue = None

        try:
            self.com_obj = COMPort(com_port)
        except:
            print ('Error: could not attach to COM port ' + str(self.com_port))
            raise

        if self.debug:
            try:
                self.log_fp = open(self.log_file, 'w')
            except IOError:
                print ('Error: could not open ' + self.log_file + ' for writing')
                raise

        self.read_queue = queue.Queue()
        self.write_queue = queue.Queue()
        self.logger_queue = queue.Queue()

        if self.exec_platform.get_platform_type() in ['JTestRTA']:
            self.prompt_event = threading.Event()
        elif self.exec_platform.get_platform_type() in ['JTestHIL']:
            self.read_event = threading.Event()

        self.com_reader = COMReaderThread()
        self.com_reader.configure(self.com_obj, self.read_queue, self.logger_queue, debug=self.debug)
        if self.exec_platform.get_platform_type() in ['JTestRTA']:
            self.com_reader.configureJTestRTA(self.exec_platform, self.prompt_event)
        elif self.exec_platform.get_platform_type() in ['JTestHIL']:
            self.com_reader.configureJTestHIL(self.exec_platform, self.read_event)

        self.com_writer = COMWriterThread()
        self.com_writer.configure(self.com_obj, self.write_queue, self.logger_queue, debug=self.debug)
        if self.exec_platform.get_platform_type() in ['JTestRTA']:
            self.com_writer.configureJTestRTA(self.exec_platform, self.prompt_event)

        return

    def configureTimeDataQueue(self, time_data_queue):
        self.time_data_queue = time_data_queue
        return

    def get_log_strs(self, the_queue):
        log_strs = []
        while not the_queue.empty():
            log_str = the_queue.get()
            log_strs.append(log_str)

        if self.debug:
            if is_py2:
                log_str = ''.join(log_strs)
            else:
                log_str = b''.join(log_strs)
                log_str = log_str.decode('utf-8')
            self.log_fp.write(log_str)

        return log_strs

    def run(self):
        print ('Info: %s starts' % (self.getName()))
        self.com_writer.start()
        self.com_reader.start()

        if self.exec_platform.get_platform_type() in ['JTestRTA']:
            for init_cmd in self.exec_platform.get_ExecLog_InitCmds():
                self.write_queue.put(init_cmd)
            if len(self.exec_platform.get_ExecLog_InitCmds()) > 0:
                self.write_queue.join()

            self.get_log_strs(self.logger_queue)

            while not self.read_queue.empty():
                log_str = self.read_queue.get()

        while not self.stopevent.isSet():
            if self.exec_platform.get_platform_type() in ['JTestRTA']:
                for pull_cmd in self.exec_platform.get_ExecLog_PullCmds():
                    self.write_queue.put(pull_cmd)
                if len(self.exec_platform.get_ExecLog_PullCmds()) > 0:
                    self.write_queue.join()

            elif self.exec_platform.get_platform_type() in ['JTestHIL']:
                self.read_event.wait()
                self.read_event.clear()

            self.get_log_strs(self.logger_queue)

            log_strs = self.get_log_strs(self.read_queue)

            oj_exec_log = self.exec_platform.parse_ExecLog(log_strs)

            if len(oj_exec_log) > 0:
                if self.debug:
                    self.log_fp.write('\n'.join(oj_exec_log))

                oj_exec_obj = OJExecLog(self.exec_platform.platform.get_frame_size(),
                                    self.exec_platform.platform.get_sample_rate(),
                                    self.exec_platform.platform.get_clock_rate(),
                                    oj_exec_log)
                oj_exec_obj.run()

                mips_report_df = oj_exec_obj.get_complete_mips_report_df()
                if self.debug:
                    self.log_fp.write(mips_report_df.to_csv(line_terminator='\n'))

                time_objects_df = oj_exec_obj.get_complete_time_objects_df()
                if self.debug:
                    self.log_fp.write(time_objects_df.to_csv(line_terminator='\n'))

                if self.time_data_queue != None:
                    self.time_data_queue.put({'oj_exec_obj':oj_exec_obj,
                                              'time_objects':time_objects_df,
                                              'mips_report':mips_report_df})

        self.get_log_strs(self.logger_queue)

        self.com_writer.join()
        self.com_reader.join()
        self.com_obj.teardown()

        if self.debug:
            self.log_fp.close()

        if self.time_data_queue != None:
            self.time_data_queue.put({})

        print ('Info: %s stops' % (self.getName()))
        return

    def join(self,timeout=None):
        if (self.exec_platform != None) and (self.exec_platform.get_platform_type() in ['JTestHIL']):
        # Set JTestHIL read_event so that the loop continues
            self.read_event.set()
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    log_file = None
    plat_xml = None
    com_port = None
    dxe_file = None
    work_dir = None
    preload_file = None

    try:
        opts, args = getopt.getopt(argv, 'hc:x:d:p:w:l:', ['help','com=','xml=',"dxe=", "pre=", "work=",'log='])
    except getopt.GetoptError:
        usage()
        return None
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            return None
        elif opt in ('-c', '--com'):
            try:
                com_port = int(arg)
            except ValueError:
                print ('Error: com_port is expected to be an integer value')
                return None
        elif opt in ('-x', '--xml'):
            plat_xml = os.path.abspath(arg)
            if not os.path.isfile(plat_xml):
                print ('Error: platform config xml ' + plat_xml + ' could not be found')
                return None
        elif opt in ("-d", "--dxe"):
            dxe_file = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-p", "--pre"):
            preload_file = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-w", "--work"):
            work_dir = os.path.abspath(arg)
        elif opt in ('-l', '--log'):
            log_file = os.path.abspath(arg)
            output_dir = os.path.dirname(log_file)
            if not os.path.isdir(output_dir):
                print ('Error: output directory ' + output_dir + ' could not be found')
                return None

    if (plat_xml == None) or (dxe_file == None) or (work_dir == None):
        usage()
        return None

    return {'log_file':log_file,
            'plat_xml':plat_xml,
            'com_port':com_port,
            'dxe_file':dxe_file,
            'preload_file':preload_file,
            'work_dir':work_dir}

#*********************************************************************************
# Function: jtest_pil_exec_graph(platform_xml, dxe_file, work_dir)
# Description: Exec Graph for JTestPIL platform
#
#*********************************************************************************
def jtest_pil_exec_graph(exec_platform, buffer_bytes, debug=False):
    oj_exec_log = exec_platform.parse_ExecLog(buffer_bytes)
    if debug:
        for x in oj_exec_log:
            print ( x )

    oj_exec_obj = OJExecLog(frame_size=exec_platform.get_frame_size(),
                            sample_rate=exec_platform.get_sample_rate(),
                            clock_rate=exec_platform.get_clock_rate(),
                            input_lines=oj_exec_log)
    oj_exec_obj.run()

    mips_report_df = oj_exec_obj.get_complete_mips_report_df()
    #print(mips_report_df)

    time_objects_df = oj_exec_obj.get_complete_time_objects_df()
    #print(time_objects_df)

    return {'oj_exec_obj':oj_exec_obj,
            'time_objects':time_objects_df,
            'mips_report':mips_report_df}

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return

    debug = False
    if params['log_file'] != None:
        debug = True

    exec_platform = ExecGraphPlatform(params['plat_xml'])
    if not exec_platform.isValid():
        print ('Error: failure parsing platform config xml ' + params['plat_xml'])
        return

    platform_board = exec_platform.get_platform_board()
    platform_type = exec_platform.get_platform_type()

    if platform_type in ['JTestHIL','JTestRTA']:
        if params['com_port'] == None:
            print ('Error: com_port must be specified')
            return

    cr_obj = CCESRunner({'type':platform_type, 'board':platform_board},
                         params['dxe_file'],
                         params['preload_file'],
                         params['work_dir'])

    if cr_obj.isValid():
        buffer_bytes_list = cr_obj.run()
    else:
        return

    if platform_type in ['VegaAlpine', 'JTestPIL']:
        if buffer_bytes_list != None:
            for buffer_bytes in buffer_bytes_list:
                time_data = jtest_pil_exec_graph(exec_platform, buffer_bytes, debug=debug)
                print(time_data['mips_report'])
        return

    if platform_type in ['JTestHIL','JTestRTA']:
        time_data_queue = queue.Queue()

        exec_graph = ExecGraphThread()
        exec_graph.configure(params['com_port'], exec_platform, params['log_file'], debug=debug)
        exec_graph.configureTimeDataQueue(time_data_queue)

        kb_input = KBInputThread()
        kb_input.addToJoinList(exec_graph)
        kb_input.start()

        exec_graph.start()

        loop_flag = True
        loop_cnt = 0
        while loop_flag:
            print ('loop_cnt: ' + str(loop_cnt) + '\n')
            loop_cnt = loop_cnt + 1
            time_data = time_data_queue.get()
            # Empty dict indicates end
            if not time_data:
                loop_flag = False
            else:
                print(time_data['mips_report'])

#*********************************************************************************
if __name__ == '__main__':
    main()
