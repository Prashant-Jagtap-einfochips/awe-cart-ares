import os
import sys
import getopt
import json
import pprint

from OJTSI.ExecLogOJT import ExecLogOJT
from OJTSI.PlotExecGraphOJT import bkapp

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue

__usage__ = """ \

Usage:
  OJExecLogOJT.py [-h] -j json_file -l exec_run_log -o output_html_file

Description:
   - Parses exec_run_log(s) with the help of execgraph.json file(s) and produces execution graph.

Options:
  -h,   --help   Display usage information (this message)
  -j,   --json   execgraph.json file with graph parameters
  -l,   --log    exec_run_log
  -o,   --out    output.execgraph.html

  if there are more than one execgraph.json, baf_logger or heap_log_buffer files, supply a semicolon separated list
"""

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    exec_graph_json = None
    execgraph_html = None

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
            exec_run_log = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-o", "--out"):
            execgraph_html = os.path.abspath(arg)

    if (exec_graph_json == None) or (exec_run_log == None) or (execgraph_html == None):
        usage()
        return None

    if (len(exec_run_log) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), exec_run_log (%d)' % (len(exec_graph_json), len(exec_run_log)))
        usage()
        return None

    return {'exec_graph_json':exec_graph_json,
            'exec_run_log':exec_run_log,
            'execgraph_html':execgraph_html}

#*********************************************************************************
# plot_exec_graph()
#*********************************************************************************
def plot_exec_graph( params, json_dump=True ):
    pp = pprint.PrettyPrinter(indent=4)

    if json_dump == True:
        params_json = params['execgraph_html'].replace('.exec_graph.html','.params.json')
        with open(params_json, "w") as outfile:
            json.dump(params, outfile, indent=4)

    time_data_list = []
    dsp_index = 0
    for exec_run_log in params['exec_run_log']:
        if not os.path.isfile(exec_run_log):
            print ( 'Error: exec_run_log ' + exec_run_log + ' could not be found' )
            return
        else:
            oj_exec_log = []
            with open(exec_run_log) as f:
                while True:
                    line = f.readline()
                    if not line:
                        break
                    line = line.strip()
                    if (len(line) == 0):
                        continue
                    if line.startswith('Bg') or line.startswith('En'):
                        oj_exec_log.append(line)

        exec_graph_params = params
        if ('frame_size' not in exec_graph_params):
            exec_graph_params['frame_size'] = None
        if ('sample_rate' not in exec_graph_params):
            exec_graph_params['sample_rate'] = None
        if ('clock_rate' not in exec_graph_params):
            exec_graph_params['clock_rate'] = None
        if ('callrates_list' not in exec_graph_params):
            exec_graph_params['callrates_list'] = None
        else:
            callrates_list = {int(k):{"callrate":int(v["callrate"]), "rate_period":float(v["rate_period"]), "base_task":v["base_task"] } for k,v in exec_graph_params['callrates_list'].items()}
            exec_graph_params['callrates_list'] = callrates_list
        if ('scaling_factors' not in exec_graph_params):
            exec_graph_params['scaling_factors'] = None
        if ('skip_frames' not in exec_graph_params):
            exec_graph_params['skip_frames'] = None
        if ('process_list' not in exec_graph_params):
            exec_graph_params['process_list'] = None
        if ('probe_points' not in exec_graph_params):
            exec_graph_params['probe_points'] = None
        if ('accel_process_list' not in exec_graph_params):
            exec_graph_params['accel_process_list'] = None

        #print(oj_exec_log)

        oj_exec_obj = ExecLogOJT(frame_size=exec_graph_params['frame_size'],
                            sample_rate=exec_graph_params['sample_rate'],
                            clock_rate=exec_graph_params['clock_rate'],
                            callrates_list=exec_graph_params['callrates_list'],
                            scaling_factors=exec_graph_params['scaling_factors'],
                            process_list=exec_graph_params['process_list'],
                            skip_frames=exec_graph_params['skip_frames'],
                            probe_points=exec_graph_params['probe_points'],
                            accel_process_list=exec_graph_params['accel_process_list'],
                            input_lines=oj_exec_log)
        oj_exec_obj.run()

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()

        if (exec_graph_params['callrates_list'] == None) and (exec_graph_params['scaling_factors'] != None):
            time_data = {'oj_exec_obj':oj_exec_obj,
                         'time_objects':time_objects_df,
                         'mips_trend':oj_exec_obj.get_scaled_mips_trend_objects_df(),
                         'cycles_report':oj_exec_obj.get_complete_cycles_report_df(multi_index=False),
                         'mips_report':oj_exec_obj.get_complete_scaled_mips_report_df(multi_index=False),
                         'mips_total':oj_exec_obj.get_complete_scaled_mips_totals_report_df()}
        else:
            time_data = {'oj_exec_obj':oj_exec_obj,
                         'time_objects':time_objects_df,
                         'mips_trend':oj_exec_obj.get_mips_trend_objects_df(),
                         'cycles_report':oj_exec_obj.get_complete_cycles_report_df(multi_index=False),
                         'mips_report':oj_exec_obj.get_complete_mips_report_df(multi_index=False),
                         'mips_total':oj_exec_obj.get_complete_mips_totals_report_df()}

        time_data_list.append(time_data)
        dsp_index = dsp_index+1

    bkapp(params['execgraph_html'], time_data_list)

#*********************************************************************************
# plot_exec_graph_multi()
#*********************************************************************************
def plot_exec_graph_multi( params ):

    time_data_list = []
    dsp_index = 0
    for exec_graph_json, exec_run_log in zip(params['exec_graph_json'], params['exec_run_log']):
        if not os.path.isfile(exec_run_log):
            print ( 'Error: exec_run_log ' + exec_run_log + ' could not be found' )
            return
        else:
            oj_exec_log = []
            with open(exec_run_log) as f:
                while True:
                    line = f.readline()
                    if not line:
                        break
                    line = line.strip()
                    if (len(line) == 0):
                        continue
                    if line.startswith('Bg') or line.startswith('En'):
                        oj_exec_log.append(line)

        if not os.path.isfile(exec_graph_json):
            print ( 'Error: exec_graph.json ' + exec_graph_json + ' could not be found' )
            return
        else:
            with open(exec_graph_json) as f:
                data = f.read()
            exec_graph_params = json.loads(data)

            if ('frame_size' not in exec_graph_params):
                exec_graph_params['frame_size'] = None
            if ('sample_rate' not in exec_graph_params):
                exec_graph_params['sample_rate'] = None
            if ('clock_rate' not in exec_graph_params):
                exec_graph_params['clock_rate'] = None
            if ('callrates_list' not in exec_graph_params):
                exec_graph_params['callrates_list'] = None
            else:
                callrates_list = {int(k):{"callrate":int(v["callrate"]), "rate_period":float(v["rate_period"]) } for k,v in exec_graph_params['callrates_list'].items()}
                exec_graph_params['callrates_list'] = callrates_list
            if ('scaling_factors' not in exec_graph_params):
                exec_graph_params['scaling_factors'] = None
            if ('skip_frames' not in exec_graph_params):
                exec_graph_params['skip_frames'] = None
            if ('process_list' not in exec_graph_params):
                exec_graph_params['process_list'] = None
            if ('probe_points' not in exec_graph_params):
                exec_graph_params['probe_points'] = None
            if ('accel_process_list' not in exec_graph_params):
                exec_graph_params['accel_process_list'] = None

        #print(oj_exec_log)

        oj_exec_obj = ExecLogOJT(frame_size=exec_graph_params['frame_size'],
                            sample_rate=exec_graph_params['sample_rate'],
                            clock_rate=exec_graph_params['clock_rate'],
                            callrates_list=exec_graph_params['callrates_list'],
                            scaling_factors=exec_graph_params['scaling_factors'],
                            process_list=exec_graph_params['process_list'],
                            skip_frames=exec_graph_params['skip_frames'],
                            probe_points=exec_graph_params['probe_points'],
                            accel_process_list=exec_graph_params['accel_process_list'],
                            input_lines=oj_exec_log)
        oj_exec_obj.run()

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()

        if (exec_graph_params['callrates_list'] == None) and (exec_graph_params['scaling_factors'] != None):
            time_data = {'oj_exec_obj':oj_exec_obj,
                         'time_objects':time_objects_df,
                         'mips_trend':oj_exec_obj.get_scaled_mips_trend_objects_df(),
                         'cycles_report':oj_exec_obj.get_complete_cycles_report_df(multi_index=False),
                         'mips_report':oj_exec_obj.get_complete_scaled_mips_report_df(multi_index=False),
                         'mips_total':oj_exec_obj.get_complete_scaled_mips_totals_report_df()}
        else:
            time_data = {'oj_exec_obj':oj_exec_obj,
                         'time_objects':time_objects_df,
                         'mips_trend':oj_exec_obj.get_mips_trend_objects_df(),
                         'cycles_report':oj_exec_obj.get_complete_cycles_report_df(multi_index=False),
                         'mips_report':oj_exec_obj.get_complete_mips_report_df(multi_index=False),
                         'mips_total':oj_exec_obj.get_complete_mips_totals_report_df()}

        time_data_list.append(time_data)
        dsp_index = dsp_index+1

    bkapp(params['execgraph_html'], time_data_list)

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    params = parse_args(argv)
    if params == None:
        return
    plot_exec_graph_multi(params)

#*********************************************************************************
if __name__ == '__main__':
    main()

