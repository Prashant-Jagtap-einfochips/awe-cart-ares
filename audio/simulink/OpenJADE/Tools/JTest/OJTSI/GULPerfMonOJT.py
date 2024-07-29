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

try:
    from bokeh.io import output_file, save
    from bokeh.plotting import figure, show
    from bokeh.models.widgets import Tabs, Panel
    from bokeh.models.widgets import NumberFormatter, StringFormatter
    from bokeh.models.widgets import Button, Slider, RangeSlider, Select, Div
    from bokeh.layouts import column, row
    from bokeh.models import ColumnDataSource, DataTable, TableColumn
    from bokeh.models import HoverTool
    from bokeh.models.formatters import NumeralTickFormatter
except ImportError:
    print ("Info: bokeh package not installed.")

def get_data_panels(panel_type, perfmon_dict):
    h = 300
    w = 1200

    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for thread_level, perf_mon_threads in perfmon_dict.items():
        for process_id, obj in perf_mon_threads.items():
            if panel_type == 'mips':
                y_values = obj.mips
            elif panel_type == 'cntr3':
                y_values = obj.cntr3
            elif panel_type == 'cntr4':
                y_values = obj.cntr4
            elif panel_type == 'cntr5':
                y_values = obj.cntr5
            elif panel_type == 'cntr6':
                y_values = obj.cntr6

            x_values = obj.start_time

            s = figure( plot_width=w,
                    plot_height=h,
                    toolbar_location="right",
                    tools=TOOLS)
            s.line(x=x_values, y=y_values)

            hover = HoverTool(tooltips = [('cycle', '@x'),
                                          ('value', '@y')])
            s.add_tools(hover)

            s.xaxis[0].formatter = NumeralTickFormatter(format="0,0")

            t = obj.process_name
            panels.append(Panel(child=s, title=t))
    return panels

def get_histogram_panels(panel_type, perfmon_dict):
    h = 300
    w = 1200

    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for thread_level, perf_mon_threads in perfmon_dict.items():
        for process_id, obj in perf_mon_threads.items():
            if panel_type == 'mips':
                y_values = obj.mips
            elif panel_type == 'cntr3':
                y_values = obj.cntr3
            elif panel_type == 'cntr4':
                y_values = obj.cntr4
            elif panel_type == 'cntr5':
                y_values = obj.cntr5
            elif panel_type == 'cntr6':
                y_values = obj.cntr6

            s = figure( plot_width=w,
                    plot_height=h,
                    toolbar_location="right",
                    tools=TOOLS)

            hist, edges = np.histogram(y_values, bins=50)
            s.quad(top=hist, bottom=0, left=edges[:-1], right=edges[1:],
                    fill_color="navy", line_color="white", alpha=0.5)

            hover = HoverTool(tooltips = [('Count', '@top'),
                                          ('Range', '(@left,@right)')])
            s.add_tools(hover)

            t = obj.process_name
            panels.append(Panel(child=s, title=t))
    return panels

def plot_perf_mon(perfmon_mode, perfmon_dict, perfmon_summary, output_html_file):

    output_file(output_html_file, title='Performance Monitor Plots')

    if (perfmon_mode == 'DMCACHE') or (perfmon_mode == 'PMCACHE'):
        panel_types = ['mips', 'cntr3', 'cntr4', 'cntr5', 'cntr6']
    elif perfmon_mode == 'ICACHE':
        panel_types = ['mips', 'cntr3', 'cntr4']

    mytabs = []

    source = ColumnDataSource(perfmon_summary)
    columns = []
    columns.append( TableColumn(field="process_name", title="Process", formatter=StringFormatter(font_style="bold")) )
    #columns.append( TableColumn(field="thread_level", title="tid", formatter=StringFormatter(font_style="bold")) )
    #columns.append( TableColumn(field="process_id", title="pid", formatter=StringFormatter(font_style="bold")) )
    #columns.append( TableColumn(field='time_stamp_min', title='time_stamp (min)') )
    #columns.append( TableColumn(field='time_stamp_max', title='time_stamp (max)') )
    #columns.append( TableColumn(field='time_stamp_avg', title='time_stamp (avg)') )
    for k in panel_types:
        columns.append( TableColumn(field=k+'_min', title=k+' (min)', formatter=NumberFormatter(format="0.00")) )
        columns.append( TableColumn(field=k+'_max', title=k+' (max)', formatter=NumberFormatter(format="0.00")) )
        columns.append( TableColumn(field=k+'_avg', title=k+' (avg)', formatter=NumberFormatter(format="0.00")) )

    summary_table = DataTable(source=source, columns=columns, width=1200, height=400)
    mytabs.append(summary_table)

    my_div = Div(text='<b>Performance Monitor Trend Plots</b>')
    mytabs.append(my_div)

    for panel_type in panel_types:
        my_div = Div(text='<b>%s</b>' % panel_type)
        mytabs.append(my_div)

        panels = get_data_panels(panel_type, perfmon_dict)
        mytabs.append(Tabs(tabs=panels))

    my_div = Div(text='<b>Performance Monitor Histogram</b>')
    mytabs.append(my_div)

    for panel_type in panel_types:
        my_div = Div(text='<b>%s</b>' % panel_type)
        mytabs.append(my_div)

        panels = get_histogram_panels(panel_type, perfmon_dict)
        mytabs.append(Tabs(tabs=panels))

    save(column(mytabs))

def array_stats(x):
    npx = np.array(x)
    d = {'min': np.min(npx),
         'max': np.max(npx),
         'avg': np.mean(npx) }
    return d

class PerfMonObj:
    def __init__(self, time_obj, debug=False):
        self.time_obj = time_obj
        return

    def __str__(self):
        time_data = self.time_obj.time_data
        if self.time_obj.isStart():
            start_stop = 'Sg'
        else:
            start_stop = 'Sn'
        mystr = '%s (%s %s) time_stamp %d cntr3 %d cntr4 %d cntr5 %d cntr6 %d ' % \
                (start_stop, self.time_obj.thread_level,
                 self.time_obj.process_id,
                 self.time_obj.time_stamp,
                 self.time_obj.time_data['cntr3'],
                 self.time_obj.time_data['cntr4'],
                 self.time_obj.time_data['cntr5'],
                 self.time_obj.time_data['cntr6'])

        return mystr

    def __repr__(self):
        return self.__str__()

class PerfMonPair:
    def __init__(self, time_pair, debug=False):
        self.time_pair = time_pair

        time_list = time_pair.time_list
        preemptor_list = time_pair.preemptor_list
        stop_idx = time_pair.stop_idx

        time_data = time_list[stop_idx].time_data
        cntr3  = time_data['cntr3']
        cntr4  = time_data['cntr4']
        cntr5  = time_data['cntr5']
        cntr6  = time_data['cntr6']

        for preemptor in preemptor_list:
            preemptor_start = preemptor['start_idx']
            time_data = time_list[preemptor_start].time_data
            cntr3  = cntr3  + time_data['cntr3']
            cntr4  = cntr4  + time_data['cntr4']
            cntr5  = cntr5  + time_data['cntr5']
            cntr6  = cntr6  + time_data['cntr6']

        self.start_time = time_pair.start_time
        self.time_stamp = time_pair.time_stamp
        self.cntr3 = cntr3
        self.cntr4 = cntr4
        self.cntr5 = cntr5
        self.cntr6 = cntr6

class PerfMonPairList:
    def __init__(self, process_name, thread_level, process_id, rate_period, time_pairs, debug=False):
        self._data = []
        for time_pair in time_pairs:
            self._data.append( PerfMonPair(time_pair, debug=debug) )

        self.process_name = process_name
        self.thread_level = thread_level
        self.process_id = process_id
        self.rate_period = rate_period

        self.start_time = [x.start_time for x in self._data]
        self.time_stamp = [x.time_stamp for x in self._data]
        self.mips   = [(float(x.time_stamp)/float(rate_period))/(1000000) for x in self._data]
        self.cntr3  = [x.cntr3 for x in self._data]
        self.cntr4  = [x.cntr4 for x in self._data]
        self.cntr5  = [x.cntr5 for x in self._data]
        self.cntr6  = [x.cntr6 for x in self._data]

        self.summary = {'process_name': self.process_name,
                        'thread_level': self.thread_level,
                        'process_id':   self.process_id,
                        'time_stamp':   array_stats(self.time_stamp),
                        'mips':         array_stats(self.mips),
                        'cntr3':        array_stats(self.cntr3),
                        'cntr4':        array_stats(self.cntr4),
                        'cntr5':        array_stats(self.cntr5),
                        'cntr6':        array_stats(self.cntr6) }


    def get_df(self):
        cnt = len(self.time_stamp)
        d = {
             'thread_level' : [self.thread_level] * cnt,
             'process_id' : [self.process_id] * cnt,
             'time_stamp' : self.time_stamp,
             'mips':  self.mips,
             'cntr3': self.cntr3,
             'cntr4': self.cntr4,
             'cntr5': self.cntr5,
             'cntr6': self.cntr6 }
        df = pd.DataFrame(data=d)
        return df

    def __getitem__(self, index):
        return self._data[index]

#*********************************************************************************
# parse_perfmon_log(exec_graph_params, capture_run_log, perfmon_mode, perfmon_csv)
#*********************************************************************************
def parse_perfmon_log( exec_graph_params, capture_run_log, perfmon_mode, perfmon_csv, debug=False ):
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
                cntr3 = int(stats[2][4:8],16)
                cntr4 = int(stats[2][0:4],16)
                cntr5 = int(stats[3][4:8],16)
                cntr6 = int(stats[3][0:4],16)

                time_list.add( start_stop, thread_level,
                                   process_id, time_stamp,
                                   {
                                     'cntr3': cntr3,
                                     'cntr4': cntr4,
                                     'cntr5': cntr5,
                                     'cntr6': cntr6
                                   } )
    if debug:
        with open('x0.txt','w') as f:
            for time_obj in time_list:
                perf_mon = PerfMonObj(time_obj)
                f.write(str(perf_mon)+'\n')

    time_list.sort()
    time_list.subtract_initial()

    if debug:
        with open('x1.txt','w') as f:
            for time_obj in time_list:
                perf_mon = PerfMonObj(time_obj)
                f.write(str(perf_mon)+'\n')

    time_list.process_time_list(debug=False)
    if 'callrates_list' in exec_graph_params:
        time_list.add_callrates_list(exec_graph_params['callrates_list'])
    if 'process_list' in exec_graph_params:
        time_list.add_process_list(exec_graph_params['process_list'])
    if 'probe_points' in exec_graph_params:
        time_list.add_probe_points(exec_graph_params['probe_points'])
    time_list.reassign_process(debug=False)

    perfmon_series = []
    perfmon_dict = {}
    for thread_level in time_list.get_thread_levels(audio_only=True):
        perfmon_dict[thread_level] = {}
        for process_id in time_list.get_process_ids(thread_level, audio_only=False):

            process_name = time_list.getProcessName(thread_level, process_id)
            rate_period = time_list.thread_objs[thread_level].rate_period
            time_pairs = time_list.get_time_pairs(thread_level, process_id)

            print ('Info: processing %s: (%d, %d)' % (process_name, thread_level, process_id))
            obj =  PerfMonPairList(process_name, thread_level, process_id, rate_period, time_pairs, debug=debug)
            perfmon_series.append( obj )

            perfmon_dict[thread_level][process_id] = obj

    if debug:
        df = pd.concat([x.get_df() for x in perfmon_series])
        df.to_csv(perfmon_csv.replace('.csv','.full.csv'), index=False)

    d = {'process_name' : [x.summary['process_name'] for x in perfmon_series],
         'thread_level' : [x.summary['thread_level'] for x in perfmon_series],
         'process_id'   : [x.summary['process_id'] for x in perfmon_series] }

    perfmon_keys = None
    if (perfmon_mode == 'DMCACHE') or (perfmon_mode == 'PMCACHE'):
        perfmon_keys = ['time_stamp', 'mips', 'cntr3', 'cntr4', 'cntr5', 'cntr6']
    elif perfmon_mode == 'ICACHE':
        perfmon_keys = ['time_stamp', 'mips', 'cntr3', 'cntr4']

    for k in perfmon_keys:
        d[k+'_min'] = [x.summary[k]['min'] for x in perfmon_series]
        d[k+'_max'] = [x.summary[k]['max'] for x in perfmon_series]
        d[k+'_avg'] = [x.summary[k]['avg'] for x in perfmon_series]

    perfmon_summary = pd.DataFrame(data=d)
    perfmon_summary.to_csv(perfmon_csv, index=False)

    plot_perf_mon(perfmon_mode,
                  perfmon_dict,
                  perfmon_summary,
                  perfmon_csv.replace('.csv','.html') )

#*********************************************************************************
# usage()
#*********************************************************************************
g_known_modes = ['DMCACHE', 'PMCACHE', 'ICACHE']

__usage__ = """ \

Usage:
  GULPerfMonOJT.py [-h] -j json_file -m perfmon_mode -l capture_run_log -o output_csv_file

Description:
   - Parses capture_run_log for the given perfmon_mode with the help of execgraph.json file(s)

Options:
  -h,   --help   Display usage information (this message)
  -j,   --json   execgraph.json file with graph parameters
  -m,   --mode   perfmon_mode
  -l,   --log    capture_run_log
  -o,   --out    output.perfmon.csv

"""

def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    exec_graph_json = None
    perfmon_csv = None
    perfmon_mode = None

    try:
        opts, args = getopt.getopt(argv, 'hj:m:l:o:', ['help', 'json=', 'mode=', 'log=', 'out='])
    except getopt.GetoptError:
        usage()
        return None
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            return None
        elif opt in ("-j", "--json"):
            exec_graph_json = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-m", "--mode"):
            if arg in g_known_modes:
                perfmon_mode = arg
            else:
                print ('Error: unknown perfmon mode %s expected [%s]' % (arg, ','.join(g_known_modes)))
                usage()
                return None
        elif opt in ("-l", "--log"):
            capture_run_log = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-o", "--out"):
            perfmon_csv = [os.path.abspath(x) for x in arg.split(';')]

    if (perfmon_mode == None) or (exec_graph_json == None) or (capture_run_log == None) or (perfmon_csv == None):
        usage()
        return None

    if (len(capture_run_log) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), capture_run_log (%d)' % (len(exec_graph_json), len(capture_run_log)))
        usage()
        return None

    return {'exec_graph_json':exec_graph_json,
            'capture_run_log':capture_run_log,
            'perfmon_mode':perfmon_mode,
            'perfmon_csv':perfmon_csv}

#*********************************************************************************
# parse_perfmon_log_multi()
#*********************************************************************************
def parse_perfmon_log_multi( params ):

    for exec_graph_json, capture_run_log, perfmon_csv in zip(params['exec_graph_json'], params['capture_run_log'], params['perfmon_csv']):
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
        if ('accel_process_list' not in exec_graph_params):
            exec_graph_params['accel_process_list'] = None

        parse_perfmon_log( exec_graph_params, capture_run_log, params['perfmon_mode'], perfmon_csv )

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return
    parse_perfmon_log_multi(params)

#*********************************************************************************
if __name__ == '__main__':
    main()

