import os
import sys
import getopt
import json
import pprint

try:
    import pandas as pd
except ImportError:
    print ("Info: pandas package not installed.")

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

import OJTSI.TimeObjOJT
import OJTSI.ExecLogOJT
import OJTSI.BAFLoggerOJT

__usage__ = """ \

Usage:
  PlotCcnt64GraphOJT.py [-h] -j json_file -b baf_logger -l heap_log_buffer -o output_html_file [-t times_log] [-m mips_log]

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
        if (df.index.size != df.index.unique().size):
            print('Warning: Ccnt64MonOJT detected duplicate indices, dropping duplicates except last row')
        df = df.reset_index().drop_duplicates(subset=['thread_level', 'process_id'], keep='last').set_index(['thread_level', 'process_id'])

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

            data[lft_key] = dfb.loc[df_row].dropna().to_list()
            data[t_key] = dft.loc[df_row].dropna().to_list()
            data[rgt_key] = dfe.loc[df_row].dropna().to_list()

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
        if (df.index.size != df.index.unique().size):
            print('Warning: Ccnt64MonOJT detected duplicate indices, dropping duplicates except last row')
        df = df.reset_index().drop_duplicates(subset=['thread_level', 'process_id'], keep='last').set_index(['thread_level', 'process_id'])

        dfb = df[[x for x in df.columns if 'b' in x]]
        dft = df[[x for x in df.columns if 't' in x]]
        dfe = df[[x for x in df.columns if 'e' in x]]

        y = 0
        source_data = {}
        y_range_factors = []
        if len(df.index) == 0:
            print('#####################################################################################')
            print('Warning: The profiling data captured is not enough, so an empty HTML file is generated.')
            print('To run again may fix the problem.')
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

            data[lft_key] = dfb.loc[df_row].dropna().to_list()
            data[t_key] = dft.loc[df_row].dropna().to_list()
            data[rgt_key] = dfe.loc[df_row].dropna().to_list()

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
        if (df.index.size != df.index.unique().size):
            print('Warning: Ccnt64MonOJT detected duplicate indices, dropping duplicates except last row')
        df = df.reset_index().drop_duplicates(subset=['thread_level', 'process_id'], keep='last').set_index(['thread_level', 'process_id'])

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
            data['x'] = dfb.loc[df_row].dropna().to_list()
            data['y'] = dfm.loc[df_row].dropna().to_list()

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

class Ccnt64MonOJT:
    def __init__(self, frame_size, sample_rate, clock_rate, callrates_list, process_list, skip_frames, probe_points, accel_process_list, input_lines):
        self.frame_size = frame_size
        self.sample_rate = sample_rate
        self.clock_rate = clock_rate
        self.input_lines = input_lines
        self.time_list = None
        self.callrates_list = callrates_list
        self.process_list = process_list
        self.skip_frames = skip_frames
        self.probe_points = probe_points
        self.accel_process_list = accel_process_list
        self.mips_report = None

        mips_reporter = OJTSI.ExecLogOJT.MIPSReporter(frame_size, sample_rate, clock_rate, callrates_list)
        if mips_reporter.isValid():
            self.mips_reporter = mips_reporter
        else:
            self.mips_reporter = None

    def run_mips_report(self):
        if self.mips_reporter == None:
            self.mips_report = None
            return

        mips_report = {}
        for thread_level in self.get_thread_levels():
            mips_report[thread_level] = {}
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                time_objects = self.get_time_objects(thread_level, process_id)
                mips_report[thread_level][process_id] = self.mips_reporter.get_mips_report(time_objects, thread_level)
        self.mips_report = mips_report

    def getProcessName(self, thread_level, process_id):
        return self.time_list.getProcessName(thread_level, process_id)

    def get_thread_levels(self):
        return self.time_list.get_thread_levels()

    def get_process_ids(self, thread_level, audio_only=True):
        return self.time_list.get_process_ids(thread_level, audio_only=audio_only)

    def get_time_objects(self, thread_level, process_id):
        if self.skip_frames == None:
            start_idx = 0
        else:
            start_idx = self.skip_frames
        time_objects = self.time_list.get_time_pairs(thread_level, process_id)
        return time_objects[start_idx:]

    def get_mips_report(self, thread_level, process_id):
        if self.mips_report == None:
            return None

        if thread_level not in self.get_thread_levels():
            return None

        if process_id not in self.get_process_ids(thread_level, audio_only=False):
            return None

        return self.mips_report[thread_level][process_id]

    def get_complete_mips_totals_report(self):
        if self.mips_report == None:
            return []
        mips_total = [0, 0, 0]
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=True):
                mr = self.get_mips_report(thread_level, process_id)
                if mr == None:
                    continue
                mr_mips = [mr.max_mips, mr.min_mips, mr.avg_mips]
                mips_total = [sum(x) for x in zip(mips_total, mr_mips)]
        return mips_total

    def get_complete_mips_totals_report_df(self):
        columns = ['thread_level', 'process_id', 'max_mips', 'min_mips', 'avg_mips']
        if self.mips_report == None:
            mips_total = []
        else:
            mips_total = [np.nan, np.nan] + self.get_complete_mips_totals_report()
        mips_total_df = pd.DataFrame([mips_total], columns = columns)
        return mips_total_df

    def get_complete_cycles_report(self):
        if self.mips_report == None:
            return []

        cycles_report = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                process_name = self.getProcessName(thread_level, process_id)
                if process_name == None:
                    row = [thread_level, process_id]
                else:
                    row = [thread_level, process_name]
                mr = self.get_mips_report(thread_level, process_id)
                if mr == None:
                    continue
                mr_cycles = [mr.max_cycles, mr.min_cycles, mr.avg_cycles]
                cycles_report.append(row + mr_cycles)
        return cycles_report

    def get_complete_cycles_report_df(self, multi_index=True):
        columns = ['thread_level', 'process_id', 'max_cycles', 'min_cycles', 'avg_cycles']
        if self.mips_report == None:
            cycles_report = []
        else:
            cycles_report = self.get_complete_cycles_report()
        cycles_report_df = pd.DataFrame(cycles_report, columns = columns)
        if multi_index:
            cycles_report_df.set_index(['thread_level', 'process_id'], inplace=True)
        return cycles_report_df

    def get_complete_mips_report(self):
        if self.mips_report == None:
            return []

        mips_report = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                process_name = self.getProcessName(thread_level, process_id)
                if process_name == None:
                    row = [thread_level, process_id]
                else:
                    row = [thread_level, process_name]
                mr = self.get_mips_report(thread_level, process_id)
                if mr == None:
                    continue
                mr_mips = [mr.max_mips, mr.min_mips, mr.avg_mips]
                mips_report.append(row + mr_mips)
        return mips_report

    def get_complete_mips_report_df(self, multi_index=True):
        columns = ['thread_level', 'process_id', 'max_mips', 'min_mips', 'avg_mips']
        if self.mips_report == None:
            mips_report = []
        else:
            mips_report = self.get_complete_mips_report()
        mips_report_df = pd.DataFrame(mips_report, columns = columns)
        if multi_index:
            mips_report_df.set_index(['thread_level', 'process_id'], inplace=True)
        return mips_report_df

    def get_mips_trend_objects(self):
        if self.mips_report == None:
            return []

        mips_trend = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                process_name = self.getProcessName(thread_level, process_id)
                if process_name == None:
                    row = [thread_level, process_id]
                else:
                    row = [thread_level, process_name]
                #mips_df = pd.DataFrame(columns=['start_time','time_stamp','stop_time','mips'])
                #idx = 0
                for time_obj in self.get_time_objects(thread_level, process_id):
                    row.extend([time_obj.start_time, time_obj.mips])
                    #mips_df.loc[idx] = [time_obj.start_time, time_obj.time_stamp, time_obj.stop_time, time_obj.mips]
                    #idx = idx + 1
                mips_trend.append(row)
                #mips_df.to_csv('mips_%s_%s.csv' % (row[0], row[1]))
        return mips_trend

    def get_mips_trend_objects_df(self, multi_index=True):
        columns = ['thread_level', 'process_id']
        mips_trend = self.get_mips_trend_objects()

        if len(mips_trend) == 0:
            return pd.DataFrame(mips_trend, columns = columns)

        mips_trend_df = pd.DataFrame(mips_trend)
        col = 2
        idx = 0
        while col < len(mips_trend_df.columns):
            columns.extend(['b%d' % idx, 'm%d' % idx])
            idx = idx + 1
            col = col + 2

        mips_trend_df.columns = columns
        if multi_index:
            mips_trend_df.set_index(['thread_level', 'process_id'], inplace=True)
        return mips_trend_df

    def get_complete_time_objects(self):
        time_objects = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                process_name = self.getProcessName(thread_level, process_id)
                if process_name == None:
                    row = [thread_level, process_id]
                else:
                    row = [thread_level, process_name]
                for time_obj in self.get_time_objects(thread_level, process_id):
                    row.extend([time_obj.start_time, time_obj.time_stamp, time_obj.stop_time])
                time_objects.append(row)
        return time_objects

    def get_complete_time_objects_df(self, multi_index=True):
        columns = ['thread_level', 'process_id']
        time_objects = self.get_complete_time_objects()

        if len(time_objects) == 0:
            return pd.DataFrame(time_objects, columns = columns)

        time_objects_df = pd.DataFrame(time_objects)
        col = 2
        idx = 0
        while col < len(time_objects_df.columns):
            columns.extend(['b%d' % idx, 't%d' % idx, 'e%d' % idx])
            idx = idx + 1
            col = col + 3

        time_objects_df.columns = columns
        if multi_index:
            time_objects_df.set_index(['thread_level', 'process_id'], inplace=True)
        return time_objects_df

    def run(self):
        debug=False
        time_list = OJTSI.TimeObjOJT.TimeList(debug=debug)
        idx = 0
        cnt = len(self.input_lines)
        while idx < cnt:
            line = self.input_lines[idx].strip()
            idx = idx + 1
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
                    line = self.input_lines[idx].strip()
                    idx = idx+1
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
                if len(stats) != 2:
                    continue

                time_stamp = int(stats[1]+stats[0],16)
                time_list.add(start_stop, thread_level, process_id, time_stamp, time_data=None)

        if time_list.count() > 0:
            if debug:
                time_list.to_csv('pre_unwrap.csv')
            time_list.unwrap()
            if debug:
                time_list.to_csv('post_unwrap.csv')
            time_list.sort()
            if debug:
                time_list.to_csv('post_sort.csv')
            time_list.subtract_initial()
            if debug:
                time_list.to_csv('post_subtract.csv')
            time_list.process_time_list(debug=False)
            time_list.add_callrates_list(self.callrates_list)
            time_list.add_process_list(self.process_list)
            if self.probe_points:
                time_list.add_probe_points(self.probe_points)
            if self.accel_process_list:
                time_list.add_accel_probes(self.accel_process_list)
            time_list.reassign_process(debug=False)

        self.time_list = time_list
        self.run_mips_report()
        return

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
            exec_graph_params['accel_process_list'] = OJTSI.TimeObjOJT.AccelProcesses(accel_json)

        baf_logger_bytes = None
        with open(baf_logger, "rb") as f:
            baf_logger_bytes = bytearray(f.read())

        heap_log_buffer_bytes = None
        with open(heap_log_buffer, "rb") as f:
            heap_log_buffer_bytes = bytearray(f.read())

        bl = OJTSI.BAFLoggerOJT.BAF_logger(baf_logger_bytes, debug=False)
        logger_id = 'LOG_TYPE_CAPTURE'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        buffer_bytes = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
        oj_exec_log = OJTSI.BAFLoggerOJT.Logger_dump(bl, logger_id, buffer_bytes)

        oj_exec_obj = Ccnt64MonOJT(frame_size=exec_graph_params['frame_size'],
                            sample_rate=exec_graph_params['sample_rate'],
                            clock_rate=exec_graph_params['clock_rate'],
                            callrates_list=exec_graph_params['callrates_list'],
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
            mips_log_fp.write('\n')

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        #print(time_objects_df)
        if params['times_log'] != None:
            times_log_fp.write('DSP %d\n' % (dsp_index))
            times_log_fp.write(time_objects_df.to_csv(line_terminator='\n'))
            times_log_fp.write('\n\n')

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
            params['accel_process_list'] = OJTSI.TimeObjOJT.AccelProcesses(accel_json)

        bl = OJTSI.BAFLoggerOJT.BAF_logger(baf_logger_bytes, debug=False)
        logger_id = 'LOG_TYPE_CAPTURE'
        buffer_start = bl.get_buffer_start(logger_id)
        buffer_len   = bl.get_buffer_len(logger_id)
        buffer_bytes = heap_log_buffer_bytes[buffer_start : buffer_start + buffer_len]
        oj_exec_log = OJTSI.BAFLoggerOJT.Logger_dump(bl, logger_id, buffer_bytes)

        oj_exec_obj = Ccnt64MonOJT(frame_size=params['frame_size'],
                            sample_rate=params['sample_rate'],
                            clock_rate=params['clock_rate'],
                            callrates_list=params['callrates_list'],
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
            mips_log_fp.write('\n')

        time_objects_df = oj_exec_obj.get_complete_time_objects_df()
        #print(time_objects_df)
        if params['times_log'] != None:
            times_log_fp.write('DSP %d\n' % (dsp_index))
            times_log_fp.write(time_objects_df.to_csv(line_terminator='\n'))
            times_log_fp.write('\n\n')

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

