import os
import sys
import getopt
import json
import pprint

try:
    import numpy as np
except ImportError:
    print ("Info: numpy package not installed.")

try:
    from bokeh.models.callbacks import CustomJS
    from bokeh.models.sources import ColumnDataSource
    from bokeh.models.ranges import FactorRange
    from bokeh.models.annotations import BoxAnnotation, Span
    from bokeh.models.tools import BoxZoomTool, HoverTool, TapTool, PointDrawTool, PanTool, WheelZoomTool, ResetTool
    from bokeh.models.widgets import Button, Slider, RangeSlider, Select, Div
    from bokeh.models.widgets.tables import DataTable, TableColumn, NumberFormatter, StringFormatter
    from bokeh.models.formatters import NumeralTickFormatter
    from bokeh.plotting import curdoc, figure
    from bokeh.layouts import row, column
    from bokeh.layouts import gridplot
    from bokeh.models.layouts import Column, Row, Panel, Tabs
    import bokeh.palettes
    from bokeh.io import output_file, save
except ImportError:
    print ("Info: bokeh package not installed.")

from OJTSI.TimeObjOJT import AccelProcesses
from OJTSI.ExecLogOJT import ExecLogOJT
from OJTSI.BAFLoggerOJT import BAF_logger, Logger_dump

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue

__usage__ = """ \

Usage:
  PlotExecGraphOJT.py [-h] -j json_file -b baf_logger -l heap_log_buffer -o output_html_file [-t times_log] [-m mips_log]

Description:
   - Parses baf_logger.bin and heap_log_buffer.bin with the help of execgraph.json file and produces execution graph.

Options:
  -h,   --help   Display usage information (this message)
  -j,   --json   execgraph.json file with graph parameters
  -b,   --baf    baf_logger
  -l,   --log    heap_log_buffer
  -o,   --out    output.execgraph.html
  -t,   --times  times log
  -m,   --mips   mips log

  if there are more than one execgraph.json, baf_logger or heap_log_buffer files, supply a semicolon separated list
"""

#*********************************************************************************
# bkapp()
#*********************************************************************************
def bkapp(execgraph_html, time_data_list):

    output_file(execgraph_html, title='ExecGraphPlot')

    tabs = []

    #######################
    ####    Overall    ####
    #######################
    y_range_factors = []
    source_data = {}
    for i in range(len(time_data_list)):
        time_data = time_data_list[i]
        title = "DSP " + str(i)

        df = time_data['time_objects']
        dfb = df[[x for x in df.columns if 'b' in x]]
        dft = df[[x for x in df.columns if 't' in x]]
        dfe = df[[x for x in df.columns if 'e' in x]]

        for df_row in df.index:
            data = {}
            y_factor = ('dsp %s' % str(i), 'thread %s' % df_row[0], 'process %s' % df_row[1])
            y_range_factors.append(y_factor)

            y_key   = 'y_dsp%s_t%s_p%s' % (i, df_row[0], df_row[1])
            lft_key = 'lft_dsp%s_t%s_p%s' % (i, df_row[0], df_row[1])
            t_key = 't_dsp%s_t%s_p%s' % (i, df_row[0], df_row[1])
            rgt_key = 'rgt_dsp%s_t%s_p%s' % (i, df_row[0], df_row[1])

            data[lft_key] = dfb.loc[df_row].dropna().tolist()
            data[t_key] = dft.loc[df_row].dropna().tolist()
            data[rgt_key] = dfe.loc[df_row].dropna().tolist()

            num_pairs = len(data[lft_key])
            data[y_key]   = [y_factor] * num_pairs
            source_data[y_key] = ColumnDataSource(data = data)

    p = figure(y_range=FactorRange(*y_range_factors), plot_width=1500,
            toolbar_location="right", tools="xpan, xbox_zoom, xwheel_zoom, undo, redo, reset")
    p.toolbar.active_drag = None

    p.xaxis.axis_label = "Cycles"
    p.xaxis[0].formatter = NumeralTickFormatter(format="0,0")
    p.yaxis.group_text_font_size = '15pt'

    # Marker
    marker_source = ColumnDataSource({'x': [], 'y': []})
    # use image() of dash ????
    #marker = p.dash(x='x', y='y', source=marker_source, color='red', size=p.plot_height, angle=0.5*np.pi, line_dash='dashed', line_width=3)
    marker = p.circle_x(x='x', y='y', source=marker_source, color='red', size=20, fill_alpha=0.2, level='overlay') # size will affect width
    columns = [TableColumn(field="x", title="position", formatter=NumberFormatter(format='0,0'))]
    marker_table = DataTable(source=marker_source, columns=columns, editable=True, height=80)
    marker_div = Div(text='time difference(marker): 0', align="center")

    draw_tool = PointDrawTool(renderers=[marker], description='marker')
    p.add_tools(draw_tool)

    marker_callback = CustomJS(args=dict(table_source=marker_table.source, marker_div=marker_div), code="""
        var data = cb_obj.data
        var x = data['x']
        if(x.length == 2){
            var time_difference = Math.abs(Math.round(x[1]-x[0]))
            marker_div.text = 'time difference(marker): ' + time_difference
        }
        else if(x.length > 2){
            marker_div.text = 'more than two markers'
        }
        else{
            marker_div.text = 'time difference(marker): 0'
        }
    """)
    marker_table.source.js_on_change('data', marker_callback)

    # https://github.com/bokeh/bokeh/issues/8482    Now this issue is open and unsolved
    # This seems to be an API issue, because the implementation of BoxAnnotation in bokehjs allows categorical axes.
    # https://groups.google.com/a/continuum.io/forum/#!topic/bokeh/sbz8Xmb-qak
    '''
    # add box annotation to every thread
    t_cnt = 0
    p_cnt = 0
    cur_t = ''
    for y_factor in y_range_factors:
        t_name = y_factor[1]
        p_cnt += 1

        if t_name != cur_t:
            t_cnt += 1
            cur_t = t_name

        if t_cnt % 2 == 0:
            box_annotation = BoxAnnotation(bottom=p_cnt-1, top=p_cnt, fill_alpha=0.1, fill_color='navy')
            p.add_layout(box_annotation)
    '''

    # add hbar
    for y_range, q_color in zip(y_range_factors, bokeh.palettes.viridis(len(y_range_factors))):
        dsp_idx = y_range[0].split()[1]
        t_idx = y_range[1].split()[1]
        p_idx = y_range[2].split()[1]

        y_key   = 'y_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        lft_key = 'lft_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        t_key = 't_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        rgt_key = 'rgt_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        source = source_data[y_key]
        renderers = p.hbar(y=y_key, height=0.2, left=lft_key, right=rgt_key, color=q_color, source=source)

        ### custom tooltips and format
        hover = HoverTool(tooltips = [
                ("index", "dsp%s t%s p%s" % (dsp_idx, t_idx, p_idx)),
                ("start", "@%s" % lft_key),
                ("time difference", "@%s" % t_key),
                ("end", "@%s" % rgt_key)
            ], renderers = [renderers])
        p.add_tools(hover)

    layout = Column(p, row(marker_table, marker_div, align="center"))
    title = "Overall"
    tab = Panel(child=layout, title=title)
    tabs.append(tab)


    ##########################
    ####    DSP Detail    ####
    ##########################
    for i in range(len(time_data_list)):
        time_data = time_data_list[i]

        df = time_data['time_objects']
        dfb = df[[x for x in df.columns if 'b' in x]]
        dft = df[[x for x in df.columns if 't' in x]]
        dfe = df[[x for x in df.columns if 'e' in x]]

        y = 0
        source_data = {}
        y_range_factors = []
        if len(df.index) == 0:
            print('#####################################################################################')
            print('Warning: The profiling data captured is not enough, so an empty HTML file is generated.')
            print('#####################################################################################')
            break
        for df_row in df.index:
            data = {}
            y_factor = ('dsp %s' % str(i), 'thread %s' % df_row[0], 'process %s' % df_row[1])
            y_range_factors.append(y_factor)

            y_key   = 'y_t%s_p%s' % (df_row[0], df_row[1])
            lft_key = 'lft_t%s_p%s' % (df_row[0], df_row[1])
            t_key = 't_t%s_p%s' % (df_row[0], df_row[1])
            rgt_key = 'rgt_t%s_p%s' % (df_row[0], df_row[1])

            data[lft_key] = dfb.loc[df_row].dropna().tolist()
            data[t_key] = dft.loc[df_row].dropna().tolist()
            data[rgt_key] = dfe.loc[df_row].dropna().tolist()

            num_pairs = len(data[lft_key])
            data[y_key]   = [y_factor] * num_pairs
            source_data[y_key] = ColumnDataSource(data = data)
            y += 1

        # Slider to control rollover
        slider_start = 1
        slider_end = num_pairs if (num_pairs > 1) else 2
        rollover_slider = Slider(start=slider_start, end=slider_end, value=num_pairs, step=1, title="Rollover", width=500, align="center")
        # Select to choose the amount of data displayed in freezing status
        data_amount_select = Select(title="Amount:", value="all the data", options=["all the data", "5", "10", "50", "100"])
        # Paragraph to display a block of text information
        info_div = Div(text="", width=400, align="center")

        p = figure(y_range=FactorRange(*y_range_factors), plot_width=1500,
                toolbar_location="right", tools="xpan, xbox_zoom, xwheel_zoom, undo, redo, reset")
        p.toolbar.active_drag = None

        p.xaxis.axis_label = "Cycles"
        p.xaxis[0].formatter = NumeralTickFormatter(format="0,0")
        p.yaxis.group_text_font_size = '15pt'

        # Marker
        marker_source = ColumnDataSource({'x': [], 'y': []})
        # use image() of dash ????
        #marker = p.dash(x='x', y='y', source=marker_source, color='red', size=p.plot_height, angle=0.5*np.pi, line_dash='dashed', line_width=3)
        marker = p.circle_x(x='x', y='y', source=marker_source, color='red', size=20, fill_alpha=0.2, level='overlay') # size will affect width
        columns = [TableColumn(field="x", title="position", formatter=NumberFormatter(format='0,0'))]
        marker_table = DataTable(source=marker_source, columns=columns, editable=True, height=80)
        marker_div = Div(text='time difference(marker): 0', align="center")

        draw_tool = PointDrawTool(renderers=[marker], description='marker')
        p.add_tools(draw_tool)

        marker_callback = CustomJS(args=dict(table_source=marker_table.source, marker_div=marker_div), code="""
            var data = cb_obj.data
            var x = data['x']
            if(x.length == 2){
                var time_difference = Math.abs(Math.round(x[1]-x[0]))
                marker_div.text = 'time difference(marker): ' + time_difference
            }
            else if(x.length > 2){
                marker_div.text = 'more than two markers'
            }
            else{
                marker_div.text = 'time difference(marker): 0'
            }
        """)
        marker_table.source.js_on_change('data', marker_callback)

        # https://github.com/bokeh/bokeh/issues/8482    Now this issue is open and unsolved
        # This seems to be an API issue, because the implementation of BoxAnnotation in bokehjs allows categorical axes.
        # https://groups.google.com/a/continuum.io/forum/#!topic/bokeh/sbz8Xmb-qak
        '''
        # add box annotation to every thread
        t_cnt = 0
        p_cnt = 0
        cur_t = ''
        for y_factor in y_range_factors:
            t_name = y_factor[1]
            p_cnt += 1

            if t_name != cur_t:
                t_cnt += 1
                cur_t = t_name

            if t_cnt % 2 == 0:
                box_annotation = BoxAnnotation(bottom=p_cnt-1, top=p_cnt, fill_alpha=0.1, fill_color='navy')
                p.add_layout(box_annotation)
        '''

        # add hbar
        for df_row, q_color in zip(df.index, bokeh.palettes.viridis(y)):
            y_key   = 'y_t%s_p%s' % (df_row[0], df_row[1])
            lft_key = 'lft_t%s_p%s' % (df_row[0], df_row[1])
            t_key = 't_t%s_p%s' % (df_row[0], df_row[1])
            rgt_key = 'rgt_t%s_p%s' % (df_row[0], df_row[1])
            source = source_data[y_key]
            renderers = p.hbar(y=y_key, height=0.2, left=lft_key, right=rgt_key, color=q_color, source=source)

            ### custom tooltips and format
            hover = HoverTool(tooltips = [
                ("index", "thread %s process %s" % (df_row[0], df_row[1])),
                ("start", "@%s" % lft_key),
                ("time difference", "@%s" % t_key),
                ("end", "@%s" % rgt_key)
            ], renderers = [renderers])
            p.add_tools(hover)

        source = ColumnDataSource(time_data['cycles_report'])
        columns = [
            TableColumn(field="thread_level", title="Thread Num", formatter=StringFormatter(font_style="bold")),
            TableColumn(field="process_id", title="Process Id", formatter=StringFormatter(font_style="bold")),
            TableColumn(field="max_cycles", title="Cycles (max)", formatter=NumberFormatter(format="0")),
            TableColumn(field="min_cycles", title="Cycles (min)", formatter=NumberFormatter(format="0")),
            TableColumn(field="avg_cycles", title="Cycles (avg)", formatter=NumberFormatter(format="0.00"))
        ]
        cycles_table = DataTable(source=source, columns=columns, width=800, height=200)

        source = ColumnDataSource(time_data['mips_report'])
        columns = [
            TableColumn(field="thread_level", title="Thread Num", formatter=StringFormatter(font_style="bold")),
            TableColumn(field="process_id", title="Process Id", formatter=StringFormatter(font_style="bold")),
            TableColumn(field="max_mips", title="MIPS (max)", formatter=NumberFormatter(format="0.00")),
            TableColumn(field="min_mips", title="MIPS (min)", formatter=NumberFormatter(format="0.00")),
            TableColumn(field="avg_mips", title="MIPS (avg)", formatter=NumberFormatter(format="0.00"))
        ]
        mips_table = DataTable(source=source, columns=columns, width=800, height=200)

        source = ColumnDataSource(time_data['mips_total'])
        columns = [
            TableColumn(field="max_mips", title="MIPS (max)", formatter=NumberFormatter(format="0.00")),
            TableColumn(field="min_mips", title="MIPS (min)", formatter=NumberFormatter(format="0.00")),
            TableColumn(field="avg_mips", title="MIPS (avg)", formatter=NumberFormatter(format="0.00"))
        ]
        total_table = DataTable(source=source, columns=columns, width=800, height=100)

        df = time_data['mips_trend']
        dfb = df[[x for x in df.columns if 'b' in x]]
        dfm = df[[x for x in df.columns if 'm' in x]]

        mips_trend_div = Div(text='<b>MIPS Trend Plots</b>')

        mips_trend_plots = []

        hover = HoverTool(
            tooltips=[
                ("cycles", "$x"),
                ("value", "$y"),
            ]
        )
        TOOLS = [hover, BoxZoomTool(dimensions="width"), PanTool(dimensions="width"), WheelZoomTool(dimensions="width"), ResetTool()]

        for df_row in df.index:
            data = {}
            data['x'] = dfb.loc[df_row].dropna().tolist()
            data['y'] = dfm.loc[df_row].dropna().tolist()

            cds = ColumnDataSource(data = data)

            fig = figure(title=df_row[1],
                         plot_width=1200,
                         plot_height=150,
                         background_fill_color="#fafafa", tools=TOOLS)

            fig.line(x = 'x', y = 'y', source = cds, line_color = "grey")
            fig.xaxis[0].formatter = NumeralTickFormatter(format="0,0")
            mips_trend_plots.append(fig)

        mips_trend = gridplot(mips_trend_plots, ncols=1, toolbar_location="right", merge_tools=True)

        mips_hist_div = Div(text='<b>MIPS Histogram</b>')

        mips_hist_panels = []
        TOOLS = ["xpan, xbox_zoom, ybox_zoom, undo, redo, reset"]

        for df_row in df.index:
            y_values = dfm.loc[df_row].dropna().tolist()

            hist, edges = np.histogram(y_values, bins=50)

            fig = figure(plot_width=1200,
                         plot_height=250,
                         background_fill_color="#fafafa",
                         toolbar_location="right", tools=TOOLS)

            fig.quad(top=hist, bottom=0, left=edges[:-1], right=edges[1:],
                     fill_color="navy", line_color="white", alpha=0.5)
            hover = HoverTool(tooltips = [('Count', '@top'),
                                          ('Range', '(@left,@right)')])
            fig.add_tools(hover)

            t = df_row[1]
            mips_hist_panels.append(Panel(child=fig, title=t))

        mips_hist_tabs = Tabs(tabs=mips_hist_panels)

        layout = Column(p,
                        row(marker_table, marker_div, align="center"),
                        cycles_table,
                        mips_table,
                        total_table,
                        mips_trend_div,
                        mips_trend,
                        mips_hist_div,
                        mips_hist_tabs)

        title = "DSP_" + str(i)
        tab = Panel(child=layout, title=title)
        tabs.append(tab)

    tab_layout = Tabs(tabs = tabs)
    save(column(tab_layout))

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
    baf_logger = None
    heap_log_buffer = None
    execgraph_html = None
    times_log = None
    mips_log = None

    try:
        opts, args = getopt.getopt(argv, 'hj:b:l:o:t:m:', ['help', 'json=', 'baf=', 'log=', 'out=', 'times=', 'mips='])
    except getopt.GetoptError:
        usage()
        return None
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
            return None
        elif opt in ("-j", "--json"):
            exec_graph_json = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-b", "--baf"):
            baf_logger = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-l", "--log"):
            heap_log_buffer = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-o", "--out"):
            execgraph_html = os.path.abspath(arg)
        elif opt in ("-t", "--times"):
            times_log = os.path.abspath(arg)
        elif opt in ("-m", "--mips"):
            mips_log = os.path.abspath(arg)

    if (exec_graph_json == None) or (baf_logger == None) or (heap_log_buffer == None) or (execgraph_html == None):
        usage()
        return None

    if (len(baf_logger) != len(heap_log_buffer)) or (len(baf_logger) != len(exec_graph_json)):
        print ('Error: Unequal number of exec_graph_json (%d), baf_logger (%d) and heap_log_buffer (%d)' % (len(exec_graph_json), len(baf_logger), len(heap_log_buffer)))
        usage()
        return None

    return {'exec_graph_json':exec_graph_json,
            'baf_logger':baf_logger,
            'heap_log_buffer':heap_log_buffer,
            'execgraph_html':execgraph_html,
            'times_log':times_log,
            'mips_log':mips_log}

#*********************************************************************************
# plot_exec_graph_multi()
#*********************************************************************************
def plot_exec_graph_multi( params ):

    if params['mips_log'] != None:
        mips_log_fp = open(params['mips_log'], 'w')
    if params['times_log'] != None:
        times_log_fp = open(params['times_log'], 'w')

    time_data_list = []
    dsp_index = 0
    for exec_graph_json, baf_logger, heap_log_buffer in zip(params['exec_graph_json'], params['baf_logger'], params['heap_log_buffer']):
        if not os.path.isfile(baf_logger):
            print ( 'Error: baf_logger.bin ' + baf_logger + ' could not be found' )
            return

        if not os.path.isfile(heap_log_buffer):
            print ( 'Error: heap_log_buffer.bin ' + heap_log_buffer + ' could not be found' )
            return

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
                callrates_list = {int(k):{"callrate":int(v["callrate"]), "rate_period":float(v["rate_period"]), "base_task":bool(v["base_task"]) } for k,v in exec_graph_params['callrates_list'].items()}
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

        accel_json = baf_logger.replace('.baf_logger.bin','.accel.json')
        if os.path.isfile(accel_json):
            exec_graph_params['accel_process_list'] = AccelProcesses(accel_json)

        baf_logger_bytes = None
        with open(baf_logger, "rb") as f:
            baf_logger_bytes = bytearray(f.read())

        heap_log_buffer_bytes = None
        with open(heap_log_buffer, "rb") as f:
            heap_log_buffer_bytes = bytearray(f.read())

        baf_logger_arch = 'ILP32'
        if ('arch' in params) and (params['arch']):
            if (params['arch'] in ['qnx_nto_aarch64']):
                baf_logger_arch = 'LP64'
        bl = BAF_logger(baf_logger_bytes, arch=baf_logger_arch, debug=False)
        logger_id = 'LOG_TYPE_EXEC'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        buffer_bytes = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
        oj_exec_log = Logger_dump(bl, logger_id, buffer_bytes)

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

        cycles_report_df = oj_exec_obj.get_complete_cycles_report_df()
        mips_report_df = oj_exec_obj.get_complete_mips_report_df()
        mips_total_df = oj_exec_obj.get_complete_mips_totals_report_df()

        #print(mips_report_df)
        if params['mips_log'] != None:
            mips_log_fp.write('DSP %d\n\n' % (dsp_index))
            mips_log_fp.write(cycles_report_df.to_csv(line_terminator='\n'))
            mips_log_fp.write('\n\n')
            mips_log_fp.write(mips_report_df.to_csv(line_terminator='\n'))
            mips_log_fp.write('\nTotals\n')
            mips_log_fp.write(mips_total_df.to_csv(line_terminator='\n',index=False))
            mips_log_fp.write('\n')

            if (exec_graph_params['callrates_list'] == None) and (exec_graph_params['scaling_factors'] != None):
                scaled_mips_report_df = oj_exec_obj.get_complete_scaled_mips_report_df()
                scaled_mips_total_df = oj_exec_obj.get_complete_scaled_mips_totals_report_df()
                scaling_factors_df = oj_exec_obj.get_scaling_factors_df()
                mips_log_fp.write('\nScaled\n')
                mips_log_fp.write(scaled_mips_report_df.to_csv(line_terminator='\n'))
                mips_log_fp.write('\nTotals\n')
                mips_log_fp.write(scaled_mips_total_df.to_csv(line_terminator='\n',index=False))
                mips_log_fp.write('\n')
                mips_log_fp.write('\nScaling Factors\n')
                mips_log_fp.write(scaling_factors_df.to_csv(line_terminator='\n',index=False))
                mips_log_fp.write('\n')
            mips_log_fp.write('\n')

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        #print(time_objects_df)
        if params['times_log'] != None:
            times_log_fp.write('DSP %d\n' % (dsp_index))
            times_log_fp.write(time_objects_df.to_csv(line_terminator='\n'))
            times_log_fp.write('\n\n')

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

    if params['mips_log'] != None:
        mips_log_fp.close()

    if params['times_log'] != None:
        times_log_fp.close()

    bkapp(params['execgraph_html'], time_data_list)

#*********************************************************************************
# plot_exec_graph()
#*********************************************************************************
def plot_exec_graph( params, json_dump=True ):
    pp = pprint.PrettyPrinter(indent=4)
    #pp.pprint(params)

    if json_dump == True:
        params_json = params['execgraph_html'].replace('.exec_graph.html','.params.json')
        with open(params_json, "w") as outfile:
            json.dump(params, outfile, indent=4)

    if params['mips_log'] != None:
        mips_log_fp = open(params['mips_log'], 'w')
    if params['times_log'] != None:
        times_log_fp = open(params['times_log'], 'w')

    time_data_list = []
    dsp_index = 0
    for baf_logger, heap_log_buffer in zip(params['baf_logger'], params['heap_log_buffer']):
        if not os.path.isfile(baf_logger):
            print ( 'Error: baf_logger.bin ' + baf_logger + ' could not be found' )
            return

        if not os.path.isfile(heap_log_buffer):
            print ( 'Error: heap_log_buffer.bin ' + heap_log_buffer + ' could not be found' )
            return

        baf_logger_bytes = None
        with open(baf_logger, "rb") as f:
            baf_logger_bytes = bytearray(f.read())

        heap_log_buffer_bytes = None
        with open(heap_log_buffer, "rb") as f:
            heap_log_buffer_bytes = bytearray(f.read())

        accel_json = baf_logger.replace('.baf_logger.bin','.accel.json')
        if os.path.isfile(accel_json):
            params['accel_process_list'] = AccelProcesses(accel_json)

        baf_logger_arch = 'ILP32'
        if ('arch' in params) and (params['arch']):
            if (params['arch'] in ['qnx_nto_aarch64']):
                baf_logger_arch = 'LP64'
        bl = BAF_logger(baf_logger_bytes, arch=baf_logger_arch, debug=False)
        logger_id = 'LOG_TYPE_EXEC'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        buffer_bytes = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
        oj_exec_log = Logger_dump(bl, logger_id, buffer_bytes)

        oj_exec_obj = ExecLogOJT(frame_size=params['frame_size'],
                            sample_rate=params['sample_rate'],
                            clock_rate=params['clock_rate'],
                            callrates_list=params['callrates_list'],
                            scaling_factors=params['scaling_factors'],
                            process_list=params['process_list'],
                            skip_frames=params['skip_frames'],
                            probe_points=params['probe_points'],
                            accel_process_list=params['accel_process_list'],
                            input_lines=oj_exec_log)
        oj_exec_obj.run()

        cycles_report_df = oj_exec_obj.get_complete_cycles_report_df()
        mips_report_df = oj_exec_obj.get_complete_mips_report_df()
        mips_total_df = oj_exec_obj.get_complete_mips_totals_report_df()

        #print(mips_report_df)
        if params['mips_log'] != None:
            mips_log_fp.write('DSP %d\n\n' % (dsp_index))
            mips_log_fp.write(cycles_report_df.to_csv(line_terminator='\n'))
            mips_log_fp.write('\n\n')
            mips_log_fp.write(mips_report_df.to_csv(line_terminator='\n'))
            mips_log_fp.write('\nTotals\n')
            mips_log_fp.write(mips_total_df.to_csv(line_terminator='\n',index=False))
            mips_log_fp.write('\n')

            if (params['callrates_list'] == None) and (params['scaling_factors'] != None):
                scaled_mips_report_df = oj_exec_obj.get_complete_scaled_mips_report_df()
                scaled_mips_total_df = oj_exec_obj.get_complete_scaled_mips_totals_report_df()
                scaling_factors_df = oj_exec_obj.get_scaling_factors_df()
                mips_log_fp.write('\nScaled\n')
                mips_log_fp.write(scaled_mips_report_df.to_csv(line_terminator='\n'))
                mips_log_fp.write('\nTotals\n')
                mips_log_fp.write(scaled_mips_total_df.to_csv(line_terminator='\n',index=False))
                mips_log_fp.write('\n')
                mips_log_fp.write('\nScaling Factors\n')
                mips_log_fp.write(scaling_factors_df.to_csv(line_terminator='\n',index=False))
                mips_log_fp.write('\n')
            mips_log_fp.write('\n')

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        #print(time_objects_df)
        if params['times_log'] != None:
            times_log_fp.write('DSP %d\n' % (dsp_index))
            times_log_fp.write(time_objects_df.to_csv(line_terminator='\n'))
            times_log_fp.write('\n\n')

        if (params['callrates_list'] == None) and (params['scaling_factors'] != None):
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

    if params['mips_log'] != None:
        mips_log_fp.close()

    if params['times_log'] != None:
        times_log_fp.close()

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

