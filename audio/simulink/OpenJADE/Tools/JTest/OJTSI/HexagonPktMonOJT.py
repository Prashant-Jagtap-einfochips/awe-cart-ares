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

def get_data_panels(panel_type, pktmon_dict):
    h = 300
    w = 1200

    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for thread_level, pkt_mon_threads in pktmon_dict.items():
        for process_id, obj in pkt_mon_threads.items():
            if panel_type == 'mcps':
                y_values = obj.mcps
            elif panel_type == 'mpps':
                y_values = obj.mpps

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

def get_histogram_panels(panel_type, pktmon_dict):
    h = 300
    w = 1200

    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for thread_level, pkt_mon_threads in pktmon_dict.items():
        for process_id, obj in pkt_mon_threads.items():
            if panel_type == 'mcps':
                y_values = obj.mcps
            elif panel_type == 'mpps':
                y_values = obj.mpps

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

def plot_pkt_mon(pktmon_dict, pktmon_summary, output_html_file):

    output_file(output_html_file, title='Packet Monitor Plots')

    panel_types = ['mcps', 'mpps']

    mytabs = []

    source = ColumnDataSource(pktmon_summary)
    columns = []
    columns.append( TableColumn(field="process_name", title="Process", formatter=StringFormatter(font_style="bold")) )
    for k in panel_types:
        columns.append( TableColumn(field=k+'_max', title=k+' (max)', formatter=NumberFormatter(format="0.00")) )
        columns.append( TableColumn(field=k+'_min', title=k+' (min)', formatter=NumberFormatter(format="0.00")) )
        columns.append( TableColumn(field=k+'_avg', title=k+' (avg)', formatter=NumberFormatter(format="0.00")) )

    summary_table = DataTable(source=source, columns=columns, width=1200, height=300)
    mytabs.append(summary_table)

    raw_panel_types = ['time_stamp', 'packets_cnt']
    columns = []
    columns.append( TableColumn(field="process_name", title="Process", formatter=StringFormatter(font_style="bold")) )
    for k in raw_panel_types:
        if k == 'time_stamp':
            title_base = 'cycles'
        elif k == 'packets_cnt':
            title_base = 'packets'
        else:
            title_base = k
        columns.append( TableColumn(field=k+'_max', title=title_base+' (max)', formatter=NumberFormatter(format="0")) )
        columns.append( TableColumn(field=k+'_min', title=title_base+' (min)', formatter=NumberFormatter(format="0")) )
        columns.append( TableColumn(field=k+'_avg', title=title_base+' (avg)', formatter=NumberFormatter(format="0.00")) )

    summary_table = DataTable(source=source, columns=columns, width=1200, height=300)
    mytabs.append(summary_table)

    my_div = Div(text='<b>Packet Monitor Trend Plots</b>')
    mytabs.append(my_div)

    for panel_type in panel_types:
        my_div = Div(text='<b>%s</b>' % panel_type)
        mytabs.append(my_div)

        panels = get_data_panels(panel_type, pktmon_dict)
        mytabs.append(Tabs(tabs=panels))

    my_div = Div(text='<b>Packet Monitor Histogram</b>')
    mytabs.append(my_div)

    for panel_type in panel_types:
        my_div = Div(text='<b>%s</b>' % panel_type)
        mytabs.append(my_div)

        panels = get_histogram_panels(panel_type, pktmon_dict)
        mytabs.append(Tabs(tabs=panels))

    save(column(mytabs))

def array_stats(x):
    npx = np.array(x)
    d = {'min': np.min(npx),
         'max': np.max(npx),
         'avg': np.mean(npx) }
    return d

class PktMonObj:
    def __init__(self, time_obj, debug=False):
        self.time_obj = time_obj
        return

    def __str__(self):
        time_data = self.time_obj.time_data
        if self.time_obj.isStart():
            start_stop = 'Sg'
        else:
            start_stop = 'Sn'
        mystr = '%s (%s %s) time_stamp %d packets_cnt %d ' % \
                (start_stop, self.time_obj.thread_level,
                 self.time_obj.process_id,
                 self.time_obj.time_stamp,
                 self.time_obj.time_data['packets_cnt'])

        return mystr

    def __repr__(self):
        return self.__str__()

class PktMonPair:
    def __init__(self, time_pair, debug=False):
        self.time_pair = time_pair

        time_list = time_pair.time_list
        preemptor_list = time_pair.preemptor_list
        start_idx = time_pair.start_idx
        stop_idx = time_pair.stop_idx

        packets_cnt = time_list[stop_idx].time_data['packets_cnt'] - time_list[start_idx].time_data['packets_cnt']

        for preemptor in preemptor_list:
            preemptor_start = preemptor['start_idx']
            preemptor_stop = preemptor['stop_idx']
            packets_cnt = packets_cnt - (time_list[preemptor_stop].time_data['packets_cnt']
                                         - time_list[preemptor_start].time_data['packets_cnt'])

        self.start_time = time_pair.start_time
        self.time_stamp = time_pair.time_stamp
        self.packets_cnt = packets_cnt

class PktMonPairList:
    def __init__(self, process_name, thread_level, process_id, rate_period, time_pairs, debug=False):
        self._data = []
        for time_pair in time_pairs:
            self._data.append( PktMonPair(time_pair, debug=debug) )

        self.process_name = process_name
        self.thread_level = thread_level
        self.process_id = process_id
        self.rate_period = rate_period

        self.start_time = [x.start_time for x in self._data]
        self.time_stamp = [x.time_stamp for x in self._data]
        self.packets_cnt = [x.packets_cnt for x in self._data]
        self.mcps   = [(float(x.time_stamp)/float(rate_period))/(1000000) for x in self._data]
        self.mpps   = [(float(x.packets_cnt)/float(rate_period))/(1000000) for x in self._data]

        self.summary = {'process_name': self.process_name,
                        'thread_level': self.thread_level,
                        'process_id':   self.process_id,
                        'time_stamp':   array_stats(self.time_stamp),
                        'packets_cnt':  array_stats(self.packets_cnt),
                        'mcps':         array_stats(self.mcps),
                        'mpps':         array_stats(self.mpps) }


    def get_df(self):
        cnt = len(self.time_stamp)
        d = {
             'thread_level' : [self.thread_level] * cnt,
             'process_id' : [self.process_id] * cnt,
             'time_stamp' : self.time_stamp,
             'packets_cnt' : self.packets_cnt,
             'mcps':  self.mcps,
             'mpps': self.mpps }
        df = pd.DataFrame(data=d)
        return df

    def __getitem__(self, index):
        return self._data[index]

#*********************************************************************************
# parse_pktmon_log(exec_graph_params, capture_run_log, pktmon_csv)
#*********************************************************************************
def parse_pktmon_log( exec_graph_params, capture_run_log, pktmon_csv, debug=False ):
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
                packets_cnt = int(stats[3]+stats[2],16)

                time_list.add( start_stop, thread_level,
                                   process_id, time_stamp,
                                   {
                                     'packets_cnt': packets_cnt
                                   } )
    if debug:
        with open('x0.txt','w') as f:
            for time_obj in time_list:
                pkt_mon = PktMonObj(time_obj)
                f.write(str(pkt_mon)+'\n')

    time_list.sort()
    time_list.subtract_initial()

    if debug:
        with open('x1.txt','w') as f:
            for time_obj in time_list:
                pkt_mon = PktMonObj(time_obj)
                f.write(str(pkt_mon)+'\n')

    time_list.process_time_list(debug=False)
    if 'callrates_list' in exec_graph_params:
        time_list.add_callrates_list(exec_graph_params['callrates_list'])
    if 'process_list' in exec_graph_params:
        time_list.add_process_list(exec_graph_params['process_list'])
    if 'probe_points' in exec_graph_params:
        time_list.add_probe_points(exec_graph_params['probe_points'])
    time_list.reassign_process(debug=False)

    pktmon_series = []
    pktmon_dict = {}
    for thread_level in time_list.get_thread_levels(audio_only=True):
        pktmon_dict[thread_level] = {}
        for process_id in time_list.get_process_ids(thread_level, audio_only=False):

            process_name = time_list.getProcessName(thread_level, process_id)
            rate_period = time_list.thread_objs[thread_level].rate_period
            time_pairs = time_list.get_time_pairs(thread_level, process_id)

            print ('Info: processing %s: (%d, %d)' % (process_name, thread_level, process_id))
            obj =  PktMonPairList(process_name, thread_level, process_id, rate_period, time_pairs, debug=debug)
            pktmon_series.append( obj )

            pktmon_dict[thread_level][process_id] = obj

    if debug:
        df = pd.concat([x.get_df() for x in pktmon_series])
        df.to_csv(pktmon_csv.replace('.csv','.full.csv'), index=False)

    d = {'process_name' : [x.summary['process_name'] for x in pktmon_series],
         'thread_level' : [x.summary['thread_level'] for x in pktmon_series],
         'process_id'   : [x.summary['process_id'] for x in pktmon_series] }

    pktmon_keys = ['time_stamp', 'packets_cnt', 'mcps', 'mpps']

    for k in pktmon_keys:
        d[k+'_min'] = [x.summary[k]['min'] for x in pktmon_series]
        d[k+'_max'] = [x.summary[k]['max'] for x in pktmon_series]
        d[k+'_avg'] = [x.summary[k]['avg'] for x in pktmon_series]

    pktmon_summary = pd.DataFrame(data=d)
    pktmon_summary.to_csv(pktmon_csv, index=False)

    plot_pkt_mon(pktmon_dict,
                  pktmon_summary,
                  pktmon_csv.replace('.csv','.html') )

#*********************************************************************************
# usage()
#*********************************************************************************
__usage__ = """ \

Usage:
  HexagonPktMonOJT.py [-h] -j json_file -l capture_run_log -o output_csv_file

Description:
   - Parses capture_run_log with the help of execgraph.json file(s)

Options:
  -h,   --help   Display usage information (this message)
  -j,   --json   execgraph.json file with graph parameters
  -l,   --log    capture_run_log
  -o,   --out    output.pktmon.csv

"""

def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    exec_graph_json = None
    pktmon_csv = None

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
            pktmon_csv = [os.path.abspath(x) for x in arg.split(';')]

    if (exec_graph_json == None) or (capture_run_log == None) or (pktmon_csv == None):
        usage()
        return None

    if (len(capture_run_log) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), capture_run_log (%d)' % (len(exec_graph_json), len(capture_run_log)))
        usage()
        return None

    return {'exec_graph_json':exec_graph_json,
            'capture_run_log':capture_run_log,
            'pktmon_csv':pktmon_csv}

#*********************************************************************************
# parse_pktmon_log_multi()
#*********************************************************************************
def parse_pktmon_log_multi( params ):

    for exec_graph_json, capture_run_log, pktmon_csv in zip(params['exec_graph_json'], params['capture_run_log'], params['pktmon_csv']):
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

        parse_pktmon_log( exec_graph_params, capture_run_log, pktmon_csv )

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return
    parse_pktmon_log_multi(params)

#*********************************************************************************
if __name__ == '__main__':
    main()

