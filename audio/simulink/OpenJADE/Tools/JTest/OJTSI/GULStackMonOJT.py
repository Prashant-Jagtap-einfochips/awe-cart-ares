#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import os
import sys
import getopt
import json
import struct
import pandas as pd
import numpy as np

import OJTSI.TimeObjOJT

def array_stats(x):
    npx = np.array(x)
    d = {'min': np.min(npx),
         'max': np.max(npx),
         'avg': np.mean(npx) }
    return d

class StackMonObj:
    def __init__(self, time_obj, debug=False):
        self.time_obj = time_obj
        return

    def __str__(self):
        time_data = self.time_obj.time_data
        if self.time_obj.isStart():
            start_stop = 'Sg'
        else:
            start_stop = 'Sn'
        mystr = '%s (%s %s) time_stamp %d length %d ' % \
                (start_stop, self.time_obj.thread_level,
                 self.time_obj.process_id,
                 self.time_obj.time_stamp,
                 self.time_obj.time_data['length'])

        return mystr

    def __repr__(self):
        return self.__str__()

class StackMonPair:
    def __init__(self, time_pair, debug=False):
        if len(time_pair.preemptor_list) > 0:
            print ('Error: premptors found. Stack measurement invalid')
            self.valid = False
            return

        self.time_pair = time_pair
        time_list = time_pair.time_list
        start_obj = time_pair.start_obj
        stop_obj = time_pair.stop_obj

        self.time_stamp = time_pair.time_stamp
        self.length  = start_obj.time_data['length'] - stop_obj.time_data['length']
        self.valid = True

class StackMonPairList:
    def __init__(self, process_name, thread_level, process_id, rate_period, time_pairs, debug=False):
        self._data = []
        for time_pair in time_pairs:
            self._data.append( StackMonPair(time_pair, debug=debug) )

        self.process_name = process_name
        self.thread_level = thread_level
        self.process_id = process_id
        self.rate_period = rate_period

        self.time_stamp = [x.time_stamp for x in self._data]
        self.mips   = [(float(x.time_stamp)/float(rate_period))/(1000000) for x in self._data]
        self.length  = [x.length for x in self._data]

        self.summary = {'process_name' : self.process_name,
                        'thread_level' : self.thread_level,
                        'process_id': self.process_id,
                        'time_stamp': array_stats(self.time_stamp),
                        'mips':       array_stats(self.mips),
                        'length':     array_stats(self.length) }

    def get_df(self):
        cnt = len(self.time_stamp)
        d = {
             'thread_level' : [self.thread_level] * cnt,
             'process_id'   : [self.process_id] * cnt,
             'time_stamp'   : self.time_stamp,
             'mips'         : self.mips,
             'length'       : self.length }
        df = pd.DataFrame(data=d)
        return df

    def __getitem__(self, index):
        return self._data[index]

#*********************************************************************************
# parse_stackmon_log(exec_graph_params, capture_run_log, stackmon_csv)
#*********************************************************************************
def parse_stackmon_log( exec_graph_params, capture_run_log, stackmon_csv, debug=False ):
    if not os.path.isfile(capture_run_log):
        print ( 'Error: capture_run_log ' + capture_run_log + ' could not be found' )
        return

    time_list = OJTSI.TimeObjOJT.TimeList(debug=debug)
    with open(capture_run_log) as f:
        while True:
            line = f.readline()
            if not line:
                break
            line = line.strip()
            if (len(line) == 0):
                continue
            if line.startswith('Sg') or line.startswith('Sn'):
                lns = line.split()
                if len(lns) != 4:
                    continue
                start_stop = (lns[0] == 'Sg')
                thread_level = lns[1]
                process_id = lns[2]
                num_stats = int(lns[3])
                stats = []
                for n in range(num_stats):
                    line = f.readline()
                    line = line.strip()
                    if (len(line) == 0):
                        continue
                    lns = line.split()
                    if len(lns) != 2:
                        continue
                    if lns[0] != '-':
                        continue
                    if len(lns[1]) != 8:
                        continue
                    stats.append(lns[1])
                if len(stats) != 4:
                    continue

                time_stamp = int(stats[1]+stats[0],16)
                length = int(stats[2],16)
                time_list.add( start_stop, thread_level,
                                   process_id, time_stamp,
                                   {
                                     'length': length
                                   } )
    if debug:
        with open('x0.txt','w') as f:
            for time_obj in time_list:
                stack_mon = StackMonObj(time_obj)
                f.write(str(stack_mon)+'\n')

    time_list.sort()
    time_list.subtract_initial()

    if debug:
        with open('x1.txt','w') as f:
            for time_obj in time_list:
                stack_mon = StackMonObj(time_obj)
                f.write(str(stack_mon)+'\n')

    time_list.process_time_list(debug=False)
    if 'callrates_list' in exec_graph_params:
        time_list.add_callrates_list(exec_graph_params['callrates_list'])
    if 'process_list' in exec_graph_params:
        time_list.add_process_list(exec_graph_params['process_list'])
    if 'probe_points' in exec_graph_params:
        time_list.add_probe_points(exec_graph_params['probe_points'])
    time_list.reassign_process(debug=False)

    stack_mon_series = []
    for thread_level in time_list.get_thread_levels(audio_only=True):
        for process_id in time_list.get_process_ids(thread_level, audio_only=False):
            process_name = time_list.getProcessName(thread_level, process_id)
            rate_period = time_list.thread_objs[thread_level].rate_period
            time_pairs = time_list.get_time_pairs(thread_level, process_id)

            print ('Info: processing %s: (%d, %d)' % (process_name, thread_level, process_id))

            obj =  StackMonPairList(process_name, thread_level, process_id, rate_period, time_pairs, debug=debug)
            stack_mon_series.append( obj )

    if debug:
        df = pd.concat([x.get_df() for x in stack_mon_series])
        df.to_csv(stackmon_csv.replace('.csv','.full.csv'), index=False)

    d = {'process_name' : [x.summary['process_name'] for x in stack_mon_series],
         'thread_level' : [x.summary['thread_level'] for x in stack_mon_series],
         'process_id'   : [x.summary['process_id'] for x in stack_mon_series] }

    stackmon_keys = ['mips','length']

    for k in stackmon_keys:
        d[k+'_min'] = [x.summary[k]['min'] for x in stack_mon_series]
        d[k+'_max'] = [x.summary[k]['max'] for x in stack_mon_series]
        d[k+'_avg'] = [x.summary[k]['avg'] for x in stack_mon_series]

    df = pd.DataFrame(data=d)
    df.to_csv(stackmon_csv, index=False)

#*********************************************************************************
# usage()
#*********************************************************************************
__usage__ = """ \

Usage:
  GULStackMonOJT.py [-h] -j json_file -l capture_run_log -o output_csv_file

Description:
   - Parses capture_run_log with the help of execgraph.json file(s)

Options:
  -h,   --help   Display usage information (this message)
  -j,   --json   execgraph.json file with graph parameters
  -l,   --log    capture_run_log
  -o,   --out    output.stackmon.csv

"""

def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    exec_graph_json = None
    stackmon_csv = None

    try:
        opts, args = getopt.getopt(argv, 'hj:l:o:', ['help', 'json=', 'log=', 'out='])
    except getopt.GetoptError:
        usage()
        return None
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            return None
        elif opt in ("-j", "--json"):
            exec_graph_json = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-l", "--log"):
            capture_run_log = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-o", "--out"):
            stackmon_csv = [os.path.abspath(x) for x in arg.split(';')]

    if (exec_graph_json == None) or (capture_run_log == None) or (stackmon_csv == None):
        usage()
        return None

    if (len(capture_run_log) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), capture_run_log (%d)' % (len(exec_graph_json), len(capture_run_log)))
        usage()
        return None

    return {'exec_graph_json':exec_graph_json,
            'capture_run_log':capture_run_log,
            'stackmon_csv':stackmon_csv}

#*********************************************************************************
# parse_stackmon_log_multi()
#*********************************************************************************
def parse_stackmon_log_multi( params ):

    for exec_graph_json, capture_run_log, stackmon_csv in zip(params['exec_graph_json'], params['capture_run_log'], params['stackmon_csv']):
        if not os.path.isfile(exec_graph_json):
            print ( 'Error: exec_graph.json ' + exec_graph_json + ' could not be found' )
            return

        with open(exec_graph_json) as f:
            data = f.read()
        exec_graph_params = json.loads(data)

        if ('callrates_list' not in exec_graph_params):
            exec_graph_params['callrates_list'] = None
        else:
            callrates_list = {int(k):{"callrate":int(v["callrate"]), "rate_period":float(v["rate_period"]) } for k,v in exec_graph_params['callrates_list'].items()}
            exec_graph_params['callrates_list'] = callrates_list
        if ('process_list' not in exec_graph_params):
            exec_graph_params['process_list'] = None
        if ('probe_points' not in exec_graph_params):
            exec_graph_params['probe_points'] = None

        parse_stackmon_log( exec_graph_params, capture_run_log, stackmon_csv )

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return
    parse_stackmon_log_multi(params)

#*********************************************************************************
if __name__ == '__main__':
    main()


