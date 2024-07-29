import time, os, threading
import sys
import getopt
import struct
import serial

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue
    

__usage__ = """ \

Usage:
  TestControl.py [-h] [-w wait_period] -c com_port -i input_file -o output_file

Description:
     
Options:
  -h,   --help   Display usage information (this message)
  -w,   --wait   Wait Period between groups (def: 10s)
  -c,   --com    Valid COM port
  -i,   --in     Input request file
  -o,   --out    Output response file
"""

def swap_endian_hex32(hex32_str):
    return hex32_str[6:8]+hex32_str[4:6]+hex32_str[2:4]+hex32_str[0:2]

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
            timeout=5)
        return

    def teardown(self):
        self.serial.close()
        
class COMReaderThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self,name='COMReaderThread')
        return
    
    def configure(self, com_obj, logger_queue, prompt_event):
        self.serial = com_obj.serial
        self.logger_queue = logger_queue
        self.prompt_event = prompt_event
        return
    
    def run(self):
        print ('%s starts' % (self.getName()))
        while not self.stopevent.isSet():
            buf = self.serial.readline()
            if len(buf) > 0:
                if is_py2:
                    buf = buf
                else:
                    buf = buf.decode('utf-8')
                sys.stdout.write(buf)
                self.logger_queue.put(buf)
                if (buf == '>\n') or (buf == '?\n'):
                    self.prompt_event.set()
                
        print ('%s stops' % (self.getName()))
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
    
    def configure(self, com_obj, write_queue, logger_queue, prompt_event):
        self.serial = com_obj.serial
        self.logger_queue = logger_queue
        self.write_queue = write_queue
        self.prompt_event = prompt_event
        return
    
    def run(self):
        print ('%s starts' % (self.getName()))
        while not self.stopevent.isSet():
            while not self.write_queue.empty():
                buf = self.write_queue.get()
                    
                if len(buf) > 0:
                    write_str = 'REQ: ' + ' '.join(swap_endian_hex32(x) for x in buf.split()) + '\n'
                    
                    if is_py2:
                        self.serial.write(buf)
                    else:
                        self.serial.write(buf.encode())
                        
                    sys.stdout.write(write_str)
                    self.logger_queue.put(write_str)
                    self.prompt_event.wait()
                    self.prompt_event.clear()
                self.write_queue.task_done()
        print ('%s stops' % (self.getName()))
        return
    
    def join(self,timeout=None):
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

class OJTSIControl:
    def __init__(self, com_port, wait_period, input_file, output_file):  
        self.com_port = com_port
        self.wait_period = wait_period
        self.input_file = input_file
        self.output_file = output_file
        
        self.out_fp = None
        self.com_obj = None
        
        self.com_reader = None
        self.com_writer = None
        
        self.logger_queue = None
        self.write_queue = None
        
        try:
            self.com_obj = COMPort(com_port)
        except:
            print ('Error: could not attach to COM port ' + str(self.com_port))
            raise
            
        try:
            self.out_fp = open(self.output_file, 'w')
        except IOError:
            print ('Error: could not open ' + self.output_file + ' for writing')
            raise
        
        self.write_queue = queue.Queue()
        self.logger_queue = queue.Queue()
        self.prompt_event = threading.Event()
        
        self.com_reader = COMReaderThread()
        self.com_reader.configure(self.com_obj, self.logger_queue, self.prompt_event)
        
        self.com_writer = COMWriterThread()
        self.com_writer.configure(self.com_obj, self.write_queue, self.logger_queue, self.prompt_event)
        
        return
    
    def run(self):
        self.com_writer.start()
        self.com_reader.start()
        
        current_group = None
        
        with open(self.input_file, 'r') as in_fp:
            line = in_fp.readline()
            while line:
                line = line.strip()
                if (len(line) != 0) and (not line.startswith('#')):
                    group_num = line.split()[0]
                    if current_group == None:
                        current_group = group_num
                    elif current_group != group_num:
                        time.sleep(self.wait_period)
                        current_group = group_num
                    write_str = ' '.join(line.split()[1:]) + '\n'
                    self.write_queue.put(write_str)
                    while not self.logger_queue.empty():
                        log_str = self.logger_queue.get()
                        self.out_fp.write(log_str)
                    
                line = in_fp.readline()
                    
        self.write_queue.join()
        while not self.logger_queue.empty():
            log_str = self.logger_queue.get()
            self.out_fp.write(log_str)
        self.com_writer.join()
        self.com_reader.join()
        self.com_obj.teardown()
        
        self.out_fp.close()
        return
#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    output_file = None
    input_file = None
    com_port = None
    wait_period = 10
    
    try:
        opts, args = getopt.getopt(argv, 'hc:w:i:o:', ['help','com=','wait=','in=','out='])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-c', '--com'):
            try:
                com_port = int(arg)
            except ValueError:
                print ('Error: com_port is expected to be an integer value')
                sys.exit()
        elif opt in ('-w', '--wait'):
            wait_period = int(arg)
        elif opt in ('-i', '--in'):
            input_file = os.path.abspath(arg)
            if not os.path.isfile(input_file):
                print ('Error: input file ' + input_file + ' could not be found')
                sys.exit()
        elif opt in ('-o', '--out'):
            output_file = os.path.abspath(arg)
            output_dir = os.path.dirname(output_file)
            if not os.path.isdir(output_dir):
                print ('Error: output directory ' + output_dir + ' could not be found')
                sys.exit()
                
    if (input_file == None) or (output_file == None) or (com_port == None):
        usage()
        sys.exit()
        
    ojtsi = OJTSIControl(com_port, wait_period, input_file, output_file)
    return ojtsi
    
#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    
#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    ojtsi = parse_args(argv)
    ojtsi.run()

#*********************************************************************************
if __name__ == '__main__':
    main()
