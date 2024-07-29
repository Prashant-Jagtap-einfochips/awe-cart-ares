import time, os, threading
import sys
import getopt
import struct
from collections import OrderedDict

is_py2 = sys.version[0] == '2'

if not is_py2:
    import codecs

__usage__ = """ \

Usage:
  PassFailReport.py [-h] -n test_name -c test_case -p test_param -t tolerance -i input_file -o output_file 

Description:
     
Options:
  -h,   --help   Display usage information (this message)
  -n,   --name   Test Name
  -c,   --case   Test Case
  -p,   --param  Test Param
  -t,   --tol    Test Tolerance
  -i,   --in     Input .run.log file
  -o,   --out    Output .status.log file
"""

def decode_hex(h):
    if is_py2:
        return h.decode('hex')
    else:
        return codecs.decode(h, 'hex')

def hex_to_int32(h):
    d = struct.unpack('>i', decode_hex(h))[0]
    return d

def hex_to_uint32(h):
    u = struct.unpack('>I', decode_hex(h))[0]
    return u

class ChannelStats:
    def __init__(self, id, max, min, avg, tolerance):  
        self.id = id
        self.max = max
        self.min = min
        self.avg = avg
        self.tolerance = tolerance
        return
        
    def __str__(self):
        mystr = 'channel[%d].stats max %.9g min %.9g avg %.9g' % (self.id, self.max, self.min, self.avg)
        if self.status():
            mystr = mystr + ' :Passed'
        else:
            mystr = mystr + ' :Failed'
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
    def status(self):
        if self.max < self.tolerance:
            return True
        else:
            return False

class PassFailReport:
    def __init__(self, test_name, test_case, test_param, tolerance, input_file, output_file):  
        self.test_name = test_name
        self.test_case = test_case
        self.test_param = test_param
        self.tolerance = tolerance
        self.input_file = input_file
        self.output_file = output_file
        self.channel_stats = OrderedDict()
        return
        
    def cycles_to_mips(self, cycles):
        return (float(cycles)*self.sample_rate/self.frame_size)/(1000000)
        
    def run(self):
        print ('Info: using %.9g as tolerance' % (self.tolerance))
        with open(self.input_file, 'r') as fp:
            line = fp.readline()
            while line:
                line = line.strip()
                if (len(line) == 0):
                    continue
                if line.startswith('channel['):
                    lns = line.split()
                    if len(lns) != 7:
                        continue
                    if not lns[0].endswith('].stats'):
                        continue
                    if lns[1] != 'max':
                        continue
                    if lns[3] != 'min':
                        continue
                    if lns[5] != 'avg':
                        continue
                    
                    id = lns[0][8:len(lns[0])-7]
                    max = float(lns[2])
                    min = float(lns[4])
                    avg = float(lns[6])
                    
                    if id not in self.channel_stats:
                        self.channel_stats[id] = ChannelStats(id, max, min, avg, self.tolerance)
                line = fp.readline()
                        
        status = 'Passed'
        for id in self.channel_stats.keys():
            if not self.channel_stats[id].status():
                status = 'Failed'
        if status == 'Failed':
            print ('Error: Test '+self.test_name+':'+self.test_case+' failed')
        else:
            print ('Success: Test '+self.test_name+':'+self.test_case+' passed')
        with open(self.output_file, 'w') as fp:
            fp.write('%s/%s(%s) : %s\n' % (self.test_name, self.test_case, self.test_param, status))
        return
        
#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    test_name = None
    test_case = None
    test_param = None
    tolerance = None
    input_file = None
    output_file = None
    
    try:
        opts, args = getopt.getopt(argv, 'hn:c:p:t:i:o:', ['help','name=','case=','param=','tol=','in=','out='])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-n', '--name'):
            test_name = arg
        elif opt in ('-c', '--case'):
            test_case = arg
        elif opt in ('-p', '--param'):
            test_param = arg
        elif opt in ('-t', '--tol'):
            tolerance = float(arg)
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
                
    if (test_name == None) or (test_case == None) or (tolerance == None) or (input_file == None) or (output_file == None):
        usage()
        sys.exit()
    
    if (test_param == None):
        test_param = ''
        
    pf = PassFailReport(test_name, test_case, test_param, tolerance, input_file, output_file)
    return pf
    
#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    
def GenPassFailReport (test_name=None, test_case=None, test_param=None, tolerance=None, input_file=None, output_file=None) :
    if (test_name == None) or (test_case == None) or (tolerance == None) or (input_file == None) or (output_file == None):
        print ('Error: one or more input paramters set to None')
        return
        
    if (test_param == None):
        test_param = ''
    
    if not os.path.isfile(input_file):
        print ('Error: input file ' + input_file + ' could not be found')
        return
        
    output_dir = os.path.dirname(output_file)
    if not os.path.isdir(output_dir):
        print ('Error: output directory ' + output_dir + ' could not be found')
        return
        
    pf = PassFailReport(test_name=test_name, test_case=test_case, test_param=test_param, tolerance=tolerance, input_file=input_file, output_file=output_file)
    pf.run()
    
#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    pf = parse_args(argv)
    pf.run()

#*********************************************************************************
if __name__ == '__main__':
    main()
