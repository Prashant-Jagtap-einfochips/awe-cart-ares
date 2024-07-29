import os, threading
import time
import sys
import getopt
import numpy as np
from functools import partial
from random import random
from threading import Condition

from bokeh.models.callbacks import CustomJS
from bokeh.models.sources import ColumnDataSource
from bokeh.models.ranges import FactorRange
from bokeh.models.annotations import BoxAnnotation, Span
from bokeh.models.tools import BoxZoomTool, HoverTool, TapTool, PointDrawTool
from bokeh.models.widgets import Button, Slider, RangeSlider, Select, Div
from bokeh.models.widgets.tables import DataTable, TableColumn, NumberFormatter
from bokeh.models.formatters import NumeralTickFormatter
from bokeh.plotting import curdoc, figure
from bokeh.server.server import Server
from bokeh.layouts import row, column
from bokeh.models.layouts import Column, Row, Panel, Tabs
import bokeh.palettes

import bokeh.palettes

from tornado import gen

from execgraph.ExecGraphPlatform import ExecGraphPlatform
from execgraph.OJExecLog import OJExecLog

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue

__usage__ = """ \

Usage:
  BAFLoggerGUI.py [-h] -x platform_xml -b baf_logger -l heap_log_buffer

Description:
   - Parses baf_logger.bin and heap_log_buffer.bin

Options:
  -h,   --help   Display usage information (this message)
  -x,   --xml    Platform XML file
  -b,   --baf    baf_logger
  -l,   --log    heap_log_buffer
"""

server = None

def stop_button_callback():
    server.io_loop.stop()
    server.stop()

#*********************************************************************************
# bkapp()
#*********************************************************************************
def bkapp(doc, params):
    time_data_list = params['time_data_list']
    exec_platform = params['exec_platform']

    tabs = []
    
    #######################
    ####    Overall    ####
    #######################
    data_buffer = []
    y_range_factors = []
    for i in range(len(time_data_list)):
        time_data = time_data_list[i]
        title = "DSP " + str(i)

        df = time_data['time_objects']
        dfb = df[[x for x in df.columns if 'b' in x]]
        dft = df[[x for x in df.columns if 't' in x]]
        dfe = df[[x for x in df.columns if 'e' in x]]

        data = {}
        num_pairs = len(dfb.columns)
        for df_row in df.index:
            y_factor = (title, 'thread %d' % int(df_row[0]), 'p%d' % int(df_row[1]))
            y_range_factors.append(y_factor)

            y_key   = 'y_dsp%s_t%d_p%d' % (i, int(df_row[0]), int(df_row[1]))
            lft_key = 'lft_dsp%s_t%d_p%d' % (i, int(df_row[0]), int(df_row[1]))
            t_key = 't_dsp%s_t%d_p%d' % (i, int(df_row[0]), int(df_row[1]))
            rgt_key = 'rgt_dsp%s_t%d_p%d' % (i, int(df_row[0]), int(df_row[1]))

            data[y_key]   = [(title, 'thread %d' % int(df_row[0]), 'p%d' % int(df_row[1]))] * num_pairs
            data[lft_key] = dfb.loc[df_row].dropna().tolist()
            data[t_key] = dft.loc[df_row].dropna().tolist()
            data[rgt_key] = dfe.loc[df_row].dropna().tolist()

        data_buffer.append(data)

    source_data = {}

    for data in data_buffer:
        source_data.update(data)
  
    source = ColumnDataSource(data = source_data)

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

    draw_tool = PointDrawTool(renderers=[marker], custom_tooltip='marker')
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
        t_name = y_factor[0]
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
        dsp_str = y_range[0].split()
        t_str = y_range[1].split()
        p_str = y_range[2]

        dsp_idx = dsp_str[1]
        t_idx = t_str[1]
        p_idx = p_str[1]

        y_key   = 'y_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        lft_key = 'lft_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        t_key = 't_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        rgt_key = 'rgt_dsp%s_t%s_p%s' % (dsp_idx, t_idx, p_idx)
        renderers = p.hbar(y=y_key, height=0.2, left=lft_key, right=rgt_key, color=q_color, source=source)

        ### custom tooltips and format
        hover = HoverTool(tooltips = [
                ("index", "dsp%d t%d p%d" % (int(dsp_idx), int(t_idx), int(p_idx))),
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

        data = {}
        y = 0
        y_range_factors = []
        num_pairs = len(dfb.columns)
        for df_row in df.index:
            y_factor = ('thread %d' % int(df_row[0]), 'p%d' % int(df_row[1]))
            y_range_factors.append(y_factor)

            y_key   = 'y_t%s_p%s' % (df_row[0], df_row[1])
            lft_key = 'lft_t%s_p%s' % (df_row[0], df_row[1])
            t_key = 't_t%s_p%s' % (df_row[0], df_row[1])
            rgt_key = 'rgt_t%s_p%s' % (df_row[0], df_row[1])

            data[y_key]   = [('thread %d' % int(df_row[0]), 'p%d' % int(df_row[1]))] * num_pairs
            data[lft_key] = dfb.loc[df_row].dropna().tolist()
            data[t_key] = dft.loc[df_row].dropna().tolist()
            data[rgt_key] = dfe.loc[df_row].dropna().tolist()
            y += 1

        source = ColumnDataSource(data = data)

        # Slider to control rollover
        rollover_slider = Slider(start=1, end=num_pairs, value=num_pairs, step=1, title="Rollover", width=500, align="center")
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

        draw_tool = PointDrawTool(renderers=[marker], custom_tooltip='marker')
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
            t_name = y_factor[0]
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
            renderers = p.hbar(y=y_key, height=0.2, left=lft_key, right=rgt_key, color=q_color, source=source)

            ### custom tooltips and format
            hover = HoverTool(tooltips = [
                ("index", "thread %d process %d" % (int(df_row[0]), int(df_row[1]))),
                ("start", "@%s" % lft_key),
                ("time difference", "@%s" % t_key),
                ("end", "@%s" % rgt_key)
            ], renderers = [renderers])
            p.add_tools(hover)

        layout = Column(p, row(marker_table, marker_div, align="center"))
        title = "DSP_" + str(i)
        tab = Panel(child=layout, title=title)
        tabs.append(tab)

        print(source.data)
        print()

    tab_layout = Tabs(tabs = tabs)
    # Button to stop the server
    stop_button = Button(label="Quit", width=80, button_type="danger")
    stop_button.on_click(partial(stop_button_callback))

    doc.add_root(column(tab_layout, row(stop_button, align="center")))

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    plat_xml = None
    baf_logger = None
    heap_log_buffer = None

    try:
        opts, args = getopt.getopt(argv, "hx:b:l:", ["help", 'xml=', "baf=", "log="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ('-x', '--xml'):
            plat_xml = os.path.abspath(arg)
            if not os.path.isfile(plat_xml):
                print ('Error: platform config xml ' + plat_xml + ' could not be found')
                return None
        elif opt in ("-b", "--baf"):
            baf_logger = [os.path.abspath(x) for x in arg.split(';')]
        elif opt in ("-l", "--log"):
            heap_log_buffer = [os.path.abspath(x) for x in arg.split(';')]

    if (plat_xml == None) or (baf_logger == None) or (heap_log_buffer == None):
        usage()
        return None

    if len(baf_logger) != len(heap_log_buffer):
        print ('Error: Unequal number of baf_logger and heap_log_buffer')
        usage()
        return None

    return {'plat_xml':plat_xml,
            'baf_logger':baf_logger,
            'heap_log_buffer':heap_log_buffer}

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    global server
    params = parse_args(argv)
    if params == None:
        return

    exec_platform = ExecGraphPlatform(params['plat_xml'])
    if not exec_platform.isValid():
        print ('Error: failure parsing platform config xml ' + params['plat_xml'])
        return

    platform_board = exec_platform.get_platform_board()
    platform_type = exec_platform.get_platform_type()

    time_data_list = []
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

        buffer_bytes = {'BAF_logger': baf_logger_bytes,
                        'heap_log_buffer': heap_log_buffer_bytes }

        oj_exec_log = exec_platform.parse_ExecLog(buffer_bytes)

        oj_exec_obj = OJExecLog(frame_size=exec_platform.get_frame_size(),
                            sample_rate=exec_platform.get_sample_rate(),
                            clock_rate=exec_platform.get_clock_rate(),
                            input_lines=oj_exec_log)
        oj_exec_obj.run()

        mips_report_df = oj_exec_obj.get_complete_mips_report_df()
        print(mips_report_df)

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        print(time_objects_df)

        time_data = {'oj_exec_obj':oj_exec_obj,
                     'time_objects':time_objects_df,
                     'mips_report':mips_report_df}

        time_data_list.append(time_data)

    '''
    bkapp_params = {'exec_platform':exec_platform,
                    'time_data':time_data_list[2]}
    '''
    bkapp_params = {'exec_platform':exec_platform,
                    'time_data_list':time_data_list}


    server = Server({'/': partial(bkapp, params=bkapp_params)}, num_procs=1)
    server.start()

    print('Opening ExecGraphViz application on http://localhost:5006/')
    server.io_loop.start()

#*********************************************************************************
if __name__ == '__main__':
    main()

