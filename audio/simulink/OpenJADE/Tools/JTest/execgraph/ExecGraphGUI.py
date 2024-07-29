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
import bokeh.palettes

from tornado import gen

from execgraph.ExecGraph import ExecGraphThread
from execgraph.ExecGraph import jtest_pil_exec_graph
from execgraph.ExecGraphPlatform import ExecGraphPlatform
from execgraph.CCESLogger import CCESRunner

is_py2 = sys.version[0] == '2'
if is_py2:
    import Queue as queue
else:
    import queue as queue

__usage__ = """ \

Usage:
  ExecGraphGUI.py [-h] [-c com_port] -x platform_xml -d dxe_file [-p preload_dxe_file] -w work_dir -l log_file

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

server = None
hex_max_value = int("ffffffff", 16)

def freeze_button_callback(td_thread):
    if td_thread.is_alive():
        td_thread.configureFreezeStatus(True)

def freeze_success_callback(new_data, source, new_text, div):
    div.update(text=new_text)
    source.update(data=new_data)

def freeze_warning_callback(new_text, div):
    div.update(text=new_text)

def resume_button_callback(td_thread):
    if td_thread.freeze == False:
        return
    else:
        td_thread.configureFreezeStatus(False)
        td_thread.configureRefresh(True)
        

def stop_button_callback(exec_graph, td_thread):
    if (exec_graph != None) and (exec_graph.is_alive()):
        exec_graph.join()
    if (td_thread != None) and (td_thread.is_alive()):
        td_thread.join()
    server.io_loop.stop()
    server.stop()

@gen.coroutine
def update(new_data, source, rollover, new_text, div):
    div.update(text=new_text)
    source.stream(new_data, rollover=rollover)

class TimeDataThread(threading.Thread):
    def __init__(self):
        self.stopevent = threading.Event()
        threading.Thread.__init__(self, name='TimeDataThread')
        self.doc = None
        self.server = None
        self.freeze = False
        self.check = False
        self.refresh = True
        self.info_div = None
        # data buffer
        self.data_buffer = []

        return

    def configureTimeDataQueue(self, time_data_queue):
        self.time_data_queue = time_data_queue
        return

    def configureCurDoc(self, doc):
        self.doc = doc
        return

    def configureColumnDataSource(self, source):
        self.source = source
        return

    def configureRolloverSlider(self, slider):
        self.rollover_slider = slider
        return

    def configureDataAmountSelect(self, select):
        self.data_amount_select = select
        return

    def configureInfoDiv(self, div):
        self.info_div = div
        return

    def configureFreezeStatus(self, status):
        self.freeze = status
        return

    def configureRefresh(self, refresh):
        self.refresh = refresh
        return

    def run(self):
        print ('Info: %s starts' % (self.getName()))
        
        pre = int("ffffffff", 16) #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        coefficient = -1
        while not self.stopevent.isSet():
            time_data = self.time_data_queue.get()

            # Empty dict indicates end
            if not time_data:
                self.stopevent.set()

            else:
                # when freeze, stop refreshing the browser page
                if self.refresh == False:
                    continue

                if self.doc != None:
                    ###################  freeze  ###################
                    if self.freeze == True:
                        if(self.data_amount_select.value == "all the data"):
                            data_amount = len(self.data_buffer)
                        else:
                            data_amount = int(self.data_amount_select.value)

                        if(len(self.data_buffer) < data_amount):
                            text = "There is no enough data in data buffer: the size of data buffer - " + str(len(self.data_buffer))
                            self.doc.add_next_tick_callback(partial(freeze_warning_callback, new_text=text, div=self.info_div))
                            print("There is no enough data in data buffer: the size of data buffer - " + str(len(self.data_buffer)))
                            continue

                        text = "Freezing: the amount of data buffer - " + str(data_amount)
                        n = len(self.data_buffer)
                        data = {}
                        # data = self.data_buffer[0] # this will update self.data_buffer[0]
                        for k in self.data_buffer[n-1].keys():
                            data[k] = self.data_buffer[n-1][k]

                        for i in range(data_amount-1):
                            data_dict = self.data_buffer[n-2-i]
                            for k in data_dict.keys():
                                data[k] = np.concatenate((data[k], data_dict[k]))

                        self.doc.add_next_tick_callback(partial(freeze_success_callback, new_data=data, source=self.source, new_text=text, div=self.info_div))
                        self.configureRefresh(False)

                    
                    ###################  running  ###################
                    else:
                        df = time_data['time_objects']
                        dfb = df[[x for x in df.columns if 'b' in x]]
                        dft = df[[x for x in df.columns if 't' in x]]
                        dfe = df[[x for x in df.columns if 'e' in x]]

                        # data is one frame - collection
                        data = {}
                        num_pairs = len(dfb.columns)
                        for df_row in df.index:
                            y_key   = 'y_t%s_p%s' % (df_row[0], df_row[1])
                            lft_key = 'lft_t%s_p%s' % (df_row[0], df_row[1])
                            t_key = 't_t%s_p%s' % (df_row[0], df_row[1])
                            rgt_key = 'rgt_t%s_p%s' % (df_row[0], df_row[1])

                            data[y_key]   = [('thread %d' % int(df_row[0]), 'p%d' % int(df_row[1]))] * num_pairs
                            data[lft_key] = dfb.loc[df_row].dropna().tolist()
                            data[t_key] = dft.loc[df_row].dropna().tolist()
                            data[rgt_key] = dfe.loc[df_row].dropna().tolist()

                        print(data)
                        print()
                        # Check if need update coefficient
                        if data['lft_t00_p00'][0] < pre:
                            coefficient = coefficient + 1
                        pre = data['rgt_t00_p00'][num_pairs-1]
                        
                        # update data value with 
                        for df_row in df.index:
                            lft_key = 'lft_t%s_p%s' % (df_row[0], df_row[1])
                            rgt_key = 'rgt_t%s_p%s' % (df_row[0], df_row[1])

                            for i in range(num_pairs):
                                data[lft_key][i] += (coefficient * hex_max_value)
                                data[rgt_key][i] += (coefficient * hex_max_value)
                            
                        print(data)
                        print()
                        text = "Running"
                        self.data_buffer.append(data)
                        self.doc.add_next_tick_callback(partial(update, new_data=data, source=self.source, rollover=self.rollover_slider.value, new_text=text, div=self.info_div))

        print ('Info: %s stops' % (self.getName()))
        return

    def join(self,timeout=None):
        self.stopevent.set()
        threading.Thread.join(self,timeout)
        return

#*********************************************************************************
# bkapp()
#*********************************************************************************
def bkapp(doc, params):
    td_thread = params['td_thread']
    exec_graph = params['exec_graph']
    time_data = params['time_data']
    exec_platform = params['exec_platform']

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
    rollover_slider = Slider(start=1, end=num_pairs, value=num_pairs, step=1, title="Rollover", width=450, align="center")
    # Select to choose the amount of data displayed in freezing status
    data_amount_select = Select(title="Amount:", value="1", options=["1", "5", "10", "50", "100", "all the data"], width=120)
    # Paragraph to display a block of text information
    info_div = Div(text="", width=300, align="center")

    if exec_platform.get_platform_type() in ['JTestHIL','JTestRTA']:
        td_thread.configureCurDoc(doc)
        td_thread.configureColumnDataSource(source)
        td_thread.configureRolloverSlider(rollover_slider)
        td_thread.configureDataAmountSelect(data_amount_select)
        td_thread.configureInfoDiv(info_div)

    p = figure(y_range=FactorRange(*y_range_factors), plot_width=1500,
            toolbar_location="right", tools="xpan, xbox_zoom, xwheel_zoom, undo, redo, reset")
    p.toolbar.active_drag = None

    p.xaxis.axis_label = "Cycles"
    p.xaxis[0].formatter = NumeralTickFormatter(format="0,0")
    p.yaxis.group_text_font_size = '15pt'

    '''
    # Slider for Spans
    range_slider = RangeSlider(start=0, end=4294967295, value=(0,4294967295), step=1, title="Range", width=1400, align="center")
    range_slider1 = RangeSlider(start=range_slider.value[0], end=range_slider.value[1], value=(range_slider.value[0],range_slider.value[1]), step=1, 
                                title="Range(fine tuning)", width=1400, align="center")

    # Spans as vertical marker
    start_span = Span(location=range_slider.value[0], dimension='height', line_color='green', line_dash='dashed', line_width=3)
    end_span = Span(location=range_slider.value[1], dimension='height', line_color='red', line_dash='dashed', line_width=3)
    
    # Div for Span(Marker)
    span_div = Div(text='start position: 0 &nbsp&nbsp&nbsp&nbsp&nbsp end position: 4294967295 &nbsp&nbsp&nbsp&nbsp&nbsp time difference(span): 4294967295', 
                align="center")

    # RangeSlider callback
    range_slider_callback = CustomJS(args=dict(start_span=start_span, end_span=end_span, slider=range_slider, slider1=range_slider1, div=span_div), code="""
        start_span.location = slider.value[0]
        end_span.location = slider.value[1]
        slider1.start = slider.value[0]
        slider1.end = slider.value[1]
        div.text = 'start position: ' + start_span.location + '&nbsp' + 'end position: ' + end_span.location + '&nbsp' + 'time difference(span): ' + (end_span.location-start_span.location)
    """)
    range_slider.js_on_change('value', range_slider_callback)
    
    range_slider1_callback = CustomJS(args=dict(start_span=start_span, end_span=end_span, slider=range_slider, slider1=range_slider1, div=span_div), code="""
        start_span.location = slider1.value[0]
        end_span.location = slider1.value[1]
        slider.value[0] = slider1.value[0]
        slider.value[1] = slider1.value[1]
        div.text = 'start position: ' + start_span.location + '&nbsp&nbsp&nbsp&nbsp&nbsp' + 'end position: ' + end_span.location + '&nbsp&nbsp&nbsp&nbsp&nbsp' + 'time difference(span): ' + (end_span.location-start_span.location)
    """)
    range_slider1.js_on_change('value', range_slider1_callback)

    p.renderers.extend([start_span, end_span])
    '''

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
    # TapTool to calculate time difference between bars marked
    taptool_div = Div(text="")
    taptool_list = []
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

        """
        # renderers.data_source or x range?
        taptool_callback = CustomJS(args=dict(source=source, taptool_list=taptool_list, taptool_div=taptool_div), code='''        
            var selected_y_key = source.data.y_key[source.selected.indices];
            var selected_lft_key = source.data.lft_key[source.selected.indices];
            var selected_t_key = source.data.t_key[source.selected.indices];
            var selected_rgt_key = source.data.rgt_key[source.selected.indices];

            var selected_data = source.data[source.selected.indices];
            System.out.println(selected_data);
            if (taptool_list.length == 0) { // indicates that this is the first bar selected
                taptool_div.text = 'First bar selected: [start]' + selected_data[lft_key] + ' [time difference]' + selected_data[t_key] + ' [end]' + selected_data[rgt_key];
                taptool_list.push(selected_data);
            }
            else if (taptool_list.length == 1)  { // this is the second bar selected
                taptool_div.text = taptool_div.text + '\n'
                            + 'Second bar selected: [start]' + selected_data[lft_key] + ' [time difference]' + selected_data[t_key] + ' [end]' + selected_data[rgt_key] + '\n'
                            + 'Distance: ' + '[left1 - left2]' + Math.abs(selected_data[lft_key] - taptool_list[0][lft_key])) + ' [right1 - right2]' + Math.abs(selected_data[rgt_key] - taptool_list[0][rgt_key])) + ' [left1 - right2]' + Math.abs(selected_data[rgt_key] - taptool_list[0][lft_key])) + ' [right1 - left2]' +  Math.abs(selected_data[lft_key] - taptool_list[0][rgt_key]));
                taptool_list.pop();
            }
            else { // more than two bars selected

            }
        ''')
        p.add_tools(TapTool(callback=taptool_callback, renderers = [renderers]))
        """
        
    # Button to stop the server
    stop_button = Button(label="Stop", align="end", width=80, button_type="danger")
    stop_button.on_click(partial(stop_button_callback, exec_graph=exec_graph, td_thread=td_thread))

    if exec_platform.get_platform_type() in ['JTestHIL','JTestRTA']:
        # Button to freeze the display
        freeze_button = Button(label='Freeze', align="end", width=80, button_type="primary")
        freeze_button.on_click(partial(freeze_button_callback, td_thread=td_thread))

        # Button to resume the display
        resume_button = Button(label='Resume', align="end", width=80, button_type="success")
        resume_button.on_click(partial(resume_button_callback, td_thread=td_thread))

        doc.add_root(column(row(rollover_slider, 
                                row(data_amount_select, freeze_button, align="center"), 
                                info_div, resume_button, stop_button, align="center"), 
                            p, row(marker_table, marker_div, align="center")))

    if exec_platform.get_platform_type() in ['JTestPIL','VegaAlpine', 'Spetter']:
        doc.add_root(column(p, row(stop_button, align="center")))

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
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    global server
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

    if platform_type == 'JTestPIL':
        if buffer_bytes_list != None:
            time_data = jtest_pil_exec_graph(exec_platform, buffer_bytes_list[0], debug=debug)
            exec_graph = None
            td_thread = None

    elif platform_type == 'VegaAlpine':
        if buffer_bytes_list != None:
            time_data_list = []
            for buffer_bytes in buffer_bytes_list:
                time_data_list.append ( jtest_pil_exec_graph(exec_platform, buffer_bytes, debug=debug) )
            time_data = time_data_list[1]
            exec_graph = None
            td_thread = None

    elif platform_type == 'Spetter':
        if buffer_bytes_list != None:
            time_data_list = []
            for buffer_bytes in buffer_bytes_list:
                time_data_list.append ( jtest_pil_exec_graph(exec_platform, buffer_bytes, debug=debug) )
            time_data = time_data_list[0]
            exec_graph = None
            td_thread = None

    elif platform_type in ['JTestHIL','JTestRTA']:
        exec_graph = ExecGraphThread()
        exec_graph.configure(params['com_port'], exec_platform, params['log_file'], debug=debug)

        td_thread = TimeDataThread()

        time_data_queue = queue.Queue()
        td_thread.configureTimeDataQueue(time_data_queue)
        exec_graph.configureTimeDataQueue(time_data_queue)

        exec_graph.start()
        td_thread.start()

        # peek
        while time_data_queue.qsize() == 0:
            print(1) # add this line will increase operating speed!  strange!!!!
            continue
        time_data = time_data_queue.queue[0]

    bkapp_params = {'exec_platform':exec_platform,
                    'exec_graph':exec_graph,
                    'td_thread':td_thread,
                    'time_data':time_data}

    server = Server({'/': partial(bkapp, params=bkapp_params)}, num_procs=1)
    server.start()

    print('Opening ExecGraphViz application on http://localhost:5006/')
    server.io_loop.start()

#*********************************************************************************
if __name__ == '__main__':
    main()

