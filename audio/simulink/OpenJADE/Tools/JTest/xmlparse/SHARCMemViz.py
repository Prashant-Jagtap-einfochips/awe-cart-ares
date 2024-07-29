#*********************************************************************************
# This script is for memory usage visualization
#
#*********************************************************************************

# libraries
import sys
import os
import math
import getopt

try:
    import numpy as np
except ImportError:
    print ("Info: numpy package not installed.")

try:
    import pandas as pd
except ImportError:
    print ("Info: pandas package not installed.")

try:
    from bokeh.plotting import figure
    from bokeh.models import ColumnDataSource, FactorRange
    from bokeh.models import NumeralTickFormatter
    from bokeh.models import CustomJS, CustomJSHover
    from bokeh.models import HoverTool, PanTool, ResetTool, WheelZoomTool, SaveTool, BoxZoomTool, BoxSelectTool
    from bokeh.models import Panel, Tabs, Select, PreText, Paragraph
    from bokeh.models import DataTable, DateFormatter, TableColumn
    from bokeh.palettes import turbo, viridis
    from bokeh.io import output_file, save
    from bokeh.layouts import Column, Row
except ImportError:
    print ("Info: bokeh package not installed.")

import xmlparse.ParseMemXML

__usage__ = """ \

Usage:
  SHARCMemViz.py [-h] -m map_xml_file -o mem_viz_html [-g classifier_group]

Description:
   - Parses the map xml file for memory usage, and visualize the output with plot and table
   - Output into specified "mem_viz_html" with .memviz.html extension.

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -m,   --map   .map.xml file to be parsed
  -o,   --out   .memviz.html file to be created
  -g,   --group classifier group to be used

"""

#*********************************************************************************
#  Global Definitions
#
#*********************************************************************************
# memory block
l1_memory_blocks_sharc = ['L1 Block 0 SRAM', 'L1 Block 1 SRAM', 'L1 Block 2 SRAM', 'L1 Block 3 SRAM']
all_memory_blocks = ['L1 Block 0 SRAM', 'L1 Block 1 SRAM', 'L1 Block 2 SRAM', 'L1 Block 3 SRAM', 'L2 RAM', 'DDR RAM']

# colors
unused_color = 'skyblue'
cache_color = 'orangered'


#*********************************************************************************
#  Function: img_l1_memory_blocks_usage(img, l1_memory_blocks)
#
#  Description: load L1 memory block usage data to make plot and data table
#
#  Parameters: img - ldf_memory_blocks object
#              l1_memory_blocks - predefined array
#
#  Return: Column layout containing plot and data table
#
#*********************************************************************************
def img_l1_memory_blocks_usage(img, l1_memory_blocks):
    ### load memory block and memory sections
    mb = [None, None, None, None]
    ms_cnt = [0, 0, 0, 0]
    ms_names = ['', '', '', '']
    mscnt_max = 0
    for n in range(4):
        mb[n] = img.get_memory_block_by_name(l1_memory_blocks[n])
        ms_names[n] = mb[n].get_memory_sections()
        ms_cnt[n] = mb[n].get_memory_sections_count()
        mscnt_max = max(mscnt_max, ms_cnt[n])

    ### load data from memory block and memory sections
    cell_text = []
    rows = []
    rows_desc = []
    row_data_size = []
    row_data_ms_names = []

    ## not cache
    used_cnt = 0
    unused_cnt = 0

    for idx in range(mscnt_max):
        bar_used  = [0, 0, 0, 0]
        bar_unused  = [0, 0, 0, 0]
        row_used = ['', '', '', '']
        row_unused = ['', '', '', '']
        ms_name_used = ['', '', '', '']
        ms_name_unused = ['', '', '', '']

        for n in range(4):
            if (idx < ms_cnt[n]) and ('cache' not in ms_names[n][idx]):
                ms = mb[n].get_memory_section_at_index(idx)
                bar_used[n] = ms.usage_bytes
                bar_unused[n] = ms.size_bytes - ms.usage_bytes
                if bar_used[n] > 0:
                    row_used[n] = "%s %.2fKB" % (ms_names[n][idx], bar_used[n]/1024.0)
                    ms_name_used[n] = ms_names[n][idx]
                if bar_unused[n] > 0:
                    row_unused[n] = "%s %.2fKB" % (ms_names[n][idx], bar_unused[n]/1024.0)
                    ms_name_unused[n] = ms_names[n][idx]

        if sum(bar_used) > 0:
            cell_text.append(row_used)
            used_cnt += 1
            rows.append('used' + str(used_cnt))
            rows_desc.append('used' + str(used_cnt) + '_desc')
            row_data_size.append(bar_used)
            row_data_ms_names.append(ms_name_used)

        if sum(bar_unused) > 0:
            cell_text.append(row_unused)
            unused_cnt += 1
            rows.append('unused' + str(unused_cnt))
            rows_desc.append('unused' + str(unused_cnt) + '_desc')
            row_data_size.append(bar_unused)
            row_data_ms_names.append(ms_name_unused)

    ## cahce
    bar_unused  = [0, 0, 0, 0]
    row_unused = ['', '', '', '']
    cache_names = ['','DMcache', 'PMcache', 'Icache']
    ms_name_unused = ['', '', '', '']

    for n in range(1,4):
        ms  = mb[n].get_memory_section_by_name(cache_names[n])
        if ms != None:
            bar_unused[n] = ms.size_bytes
        if bar_unused[n] > 0:
            row_unused[n] = "%s %.2fKB" % (cache_names[n], bar_unused[n]/1024.0)
            ms_name_unused[n] = cache_names[n]

    if sum(bar_unused) > 0:
        cell_text.append(row_unused)
        rows.append('cache')
        rows_desc.append('cache_desc')
        row_data_size.append(bar_unused)
        row_data_ms_names.append(ms_name_unused)

    rows = rows[::-1]
    rows_desc = rows_desc[::-1]
    cell_text.reverse()
    row_data_size.reverse()
    row_data_ms_names.reverse()


    ############ graph #############
    ### set colors
    used_colors = viridis(used_cnt)
    colors = ['' for idx in range(len(rows))]
    used_colors_idx = 0

    for n in range(len(rows)):
        if rows[n][:-1] == 'unused':
            colors[n] = unused_color
        elif rows[n][:-1] == 'used':
            colors[n] = used_colors[used_colors_idx]
            used_colors_idx += 1
        else:
            colors[n] = cache_color
    colors = colors[::-1]

    ### set data frame
    df_size = pd.DataFrame(row_data_size, index = rows, columns = l1_memory_blocks)
    df_desc = pd.DataFrame(row_data_ms_names, index = rows_desc, columns = l1_memory_blocks)
    df_size = df_size.swapaxes("index", "columns")
    df_desc = df_desc.swapaxes("index", "columns")

    df = pd.concat([df_size, df_desc], axis=1)
    """
    with pd.option_context('display.max_rows', None, 'display.max_columns', None, 'display.max_colwidth', 256):
        display.display(df)
    """

    ### make plot
    """
    note: the columns with same name will be merged, so we should use different name to mark multiple 'used' and
    'unused' columns
    """
    source = ColumnDataSource(df)

    p = figure(x_range = l1_memory_blocks,
               plot_height = 700,
               plot_width = 850,
               title = "L1 Memory Blocks")

    renderers = p.vbar_stack(rows[::-1], x = 'index', width = 0.9, color = colors, source = source, line_width = 0)

    ### custom tooltips and format
    for r in renderers:
        name = r.name
        status = ""
        status_desc = name + "_desc"
        if name == "cache":
            status = name
        else:
            status = name[:-1]

        hover = HoverTool(tooltips=[
            ("Status", "%s" % status),
            ('Memory Section', "@%s" % status_desc),
            ("Size", "@$name{(0.00 b)}")
        ], renderers=[r])
        p.add_tools(hover)

    p.y_range.start = 0
    p.x_range.range_padding = 0.1
    p.xgrid.grid_line_color = None
    p.axis.minor_tick_line_color = None
    p.outline_line_color = None
    p.title.text_font_size = '20pt'
    p.title.align = 'center'

    ### set y axis tick and format
    p.yaxis.ticker = np.arange(0, 10 * 32 * 1024,32 * 1024)
    p.yaxis.formatter = NumeralTickFormatter(format = "0,0")
    p.ygrid.ticker = p.yaxis.ticker


    ########## data table ###########
    df_table = pd.DataFrame(cell_text, index = rows, columns = l1_memory_blocks)

    index_column = [TableColumn(field = 'index', title = '', width = 180)]
    info_columns = [TableColumn(field = Ci, title = Ci, width = 400) for Ci in df_table.columns] # bokeh columns
    Columns = index_column + info_columns
    data_table = DataTable(columns = Columns, source = ColumnDataSource(df_table), width = 1000, sortable = False) # bokeh table


    ######### set layout ##############
    layout = Column(p, data_table)

    return layout


#*********************************************************************************
#  Function: img_memory_blocks_usage(img, memory_block)
#
#  Description: load memory block usage data to make plot and data table
#
#  Parameters: img - ldf_memory_blocks object
#              memory_block - string
#
#  Return: Column layout containing plot and data table
#
#*********************************************************************************
def img_memory_blocks_usage(img, memory_block):
    ### load memory sections and output sections
    mb = img.get_memory_block_by_name(memory_block)
    if 'DDR' in memory_block:
        size_div = (1024.0*1024.0)
        size_str = 'MB'
    else:
        size_div = (1024.0)
        size_str = 'KB'

    ms_names = mb.get_memory_sections()
    ms_cnt = mb.get_memory_sections_count()

    if ms_cnt == 0:
        text = 'Info: No memory section present in the memory block - %s' % memory_block
        return Paragraph(text = text)

    ### load data from memory sections and output sections
    ms = [None for msidx in range(ms_cnt)]
    os_cnt = [0 for msidx in range(ms_cnt)]
    os_names = [None for msidx in range(ms_cnt)]
    oscnt_max = 0

    for n in range(ms_cnt):
        ms[n] = mb.get_memory_section_at_index(n)
        os_names[n] = ms[n].get_output_sections()
        os_cnt[n] = ms[n].get_output_sections_count()
        oscnt_max = max(oscnt_max, os_cnt[n])

    cell_text = []
    rows = []
    rows_desc = []
    rows_os_groups = []
    row_data_size = []
    row_data_section_names = []
    row_data_os_groups = []

    ## used parts
    used_cnt = 0

    for idx in range(oscnt_max):
        bar_used = [0 for msidx in range(ms_cnt)]
        row_used = ['' for msidx in range(ms_cnt)]
        os_name_used = ['' for msidx in range(ms_cnt)]
        os_group_used = ['' for msidx in range(ms_cnt)]

        for n in range(ms_cnt):
            if idx < os_cnt[n]:
                os = ms[n].get_output_section_at_index(idx)
                bar_used[n] = os.size_bytes/size_div
                if bar_used[n] > 0:
                    row_used[n] = "%s %.2f%s" % (os_names[n][idx], bar_used[n], size_str)
                    os_name = os_names[n][idx]
                    os_name_used[n] = os_name
                    os_group = xmlparse.ParseMemXML.g_memoryGroupClassifier.classify(os_name)
                    os_group_used[n] = os_group

        if sum(bar_used) > 0:
            cell_text.append(row_used)
            used_cnt += 1
            rows.append('used' + str(used_cnt))
            rows_desc.append('used' + str(used_cnt) + '_desc')
            rows_os_groups.append('used' + str(used_cnt) + '_group')

            row_data_size.append(bar_used)
            row_data_section_names.append(os_name_used)
            row_data_os_groups.append(os_group_used)

    ## unused parts
    # not cache
    bar_unused = [0 for msidx in range(ms_cnt)]
    row_unused = ['' for msidx in range(ms_cnt)]
    ms_name_unused = ['' for msidx in range(ms_cnt)]
    os_group_unused = ['' for msidx in range(ms_cnt)]  # for filling row_data_os_groups list

    for n in range(ms_cnt):
        if 'cache' in ms_names[n]:
            continue

        bar_unused[n] = (ms[n].size_bytes - ms[n].usage_bytes)/size_div
        if bar_unused[n] > 0:
            row_unused[n] = "%s %.2f%s" % (ms_names[n], bar_unused[n], size_str)
            ms_name_unused[n] = ms_names[n]

    if sum(bar_unused) > 0:
        cell_text.append(row_unused)
        rows.append('unused') # only one unused column
        rows_desc.append('unused_desc')
        rows_os_groups.append('unused_group') # for filling rows_os_groups list

        row_data_size.append(bar_unused)
        row_data_section_names.append(ms_name_unused)
        row_data_os_groups.append(os_group_unused) # for filling row_data_os_groups list

    # cache
    bar_unused = [0 for msidx in range(ms_cnt)]
    row_unused = ['' for msidx in range(ms_cnt)]
    ms_name_unused = ['' for msidx in range(ms_cnt)]
    os_group_unused = ['' for msidx in range(ms_cnt)]  # for filling row_data_os_groups list

    for n in range(ms_cnt):
        if 'cache' not in ms_names[n]:
            continue

        bar_unused[n] = (ms[n].size_bytes - ms[n].usage_bytes)/size_div
        if bar_unused[n] > 0:
            row_unused[n] = "%s %.2f%s" % (ms_names[n], bar_unused[n], size_str)
            ms_name_unused[n] = ms_names[n]

    if sum(bar_unused) > 0:
        cell_text.append(row_unused)
        rows.append('cache') # only one cache column
        rows_desc.append('cache_desc')
        rows_os_groups.append('cache_group') # for filling rows_os_groups list

        row_data_size.append(bar_unused)
        row_data_section_names.append(ms_name_unused)
        row_data_os_groups.append(os_group_unused) # for filling row_data_os_groups list

    rows = rows[::-1]
    rows_desc = rows_desc[::-1]
    rows_os_groups = rows_os_groups[::-1]
    cell_text.reverse()
    row_data_size.reverse()
    row_data_section_names.reverse()
    row_data_os_groups.reverse()


    ############# graph ################
    ### set colors
    used_colors = viridis(used_cnt)
    colors = ['' for idx in range(len(rows))]
    used_colors_idx = 0

    for n in range(len(rows)):
        if rows[n] == 'unused':
            colors[n] = unused_color
        elif rows[n][:-1] == 'used':
            colors[n] = used_colors[used_colors_idx]
            used_colors_idx += 1
        else:
            colors[n] = cache_color
    colors = colors[::-1]

    ### set data frame
    df_size = pd.DataFrame(row_data_size, index = rows, columns = ms_names)
    df_desc = pd.DataFrame(row_data_section_names, index = rows_desc, columns = ms_names)
    df_group = pd.DataFrame(row_data_os_groups, index = rows_os_groups, columns = ms_names)
    df_size = df_size.swapaxes("index", "columns")
    df_desc = df_desc.swapaxes("index", "columns")
    df_group = df_group.swapaxes("index", "columns")

    df = pd.concat([df_size, df_desc, df_group], axis=1)
    """
    with pd.option_context('display.max_rows', None, 'display.max_columns', None, 'display.max_colwidth', 256):
        display.display(df)
    """

    ### make plot
    """
    note: the columns with same name will be merged, so we should use different name to mark multiple 'used' and
    'unused' columns
    """
    source = ColumnDataSource(df)

    p = figure(x_range = ms_names,
               plot_height = 700,
               plot_width = 850,
               title = memory_block)

    renderers = p.vbar_stack(rows[::-1], x = 'index', width = 0.9, color = colors, source = source, line_width = 0)

    ### custom tooltips and format
    for r in renderers:
        name = r.name
        status = ""
        status_desc = name + "_desc"
        status_group = name + "_group"

        if name[:-1] == "used":
            status = name[:-1]
            hover = HoverTool(tooltips = [
                ("Status", "%s" % status),
                ('Output Section', "@%s" % status_desc),
                ('Memory Group', "@%s" % status_group),
                ("Size", "@$name{(0.00)}%s" % size_str)
                ], renderers = [r])
            p.add_tools(hover)

        else:
            status = name
            hover = HoverTool(tooltips = [
                ("Status", "%s" % status),
                ("Size", "@$name{(0.00)}%s" % size_str)
                ], renderers = [r])
            p.add_tools(hover)

    p.y_range.start = 0
    p.x_range.range_padding = 0.1
    p.xgrid.grid_line_color = None
    p.axis.minor_tick_line_color = None
    p.outline_line_color = None
    p.title.text_font_size = '20pt'
    p.title.align = 'center'

    ### set y axis tick and format
    if 'L1' in memory_block:
        mystep = 32
    else:
        mystep = 128
    p.yaxis.ticker = np.arange(0, 10 * mystep, mystep)
    p.yaxis.formatter = NumeralTickFormatter(format = "0,0")
    p.yaxis.axis_label = size_str
    p.ygrid.ticker = p.yaxis.ticker


    ########## data table ###########
    df_table = pd.DataFrame(cell_text, index = rows, columns = ms_names)

    index_column = [TableColumn(field = 'index', title = '', width = 180)]
    info_columns = [TableColumn(field = Ci, title = Ci, width = 400) for Ci in df_table.columns] # bokeh columns
    Columns = index_column + info_columns
    data_table = DataTable(columns = Columns, source = ColumnDataSource(df_table), width = 1400, sortable = False) # bokeh table


    ######### set layout ##############
    layout = Column(p, data_table)

    return layout


#*********************************************************************************
#  Function: img_memory_section_usage_detailed(img, memory_blocks, memory_section)
#
#  Description: load memory section usage data to make plot and data table
#
#  Parameters: img - ldf_memory_blocks object
#              memory_blocks - predefined array
#              memory_section - string
#
#  Return: Column layout containing plot and data table
#
#*********************************************************************************
def img_memory_section_usage_detailed(img, memory_blocks, memory_section):
    ### load memory block, memory sections and output sections
    parent = None
    for memory_block in memory_blocks:
        mb = img.get_memory_block_by_name(memory_block)
        if memory_section in mb.get_memory_sections():
            parent = mb
            break
    if parent == None:
        return

    mb = parent
    ms = mb.get_memory_section_by_name(memory_section)
    os_names = ms.get_output_sections()
    os_names = [x for x in os_names if 'Heap' not in x]
    os_names = [x for x in os_names if 'Stack' not in x]
    os_cnt = len(os_names)

    if os_cnt == 0:
        text = 'Info: No output section present in the memory section - %s' % memory_section
        return Paragraph(text = text)

    ### load data from output sections
    os = [None for osidx in range(os_cnt)]
    isfile_cnt = [0 for osidx in range(os_cnt)]
    isfile_names = [None for osidx in range(os_cnt)]
    isfile_totals = [None for osidx in range(os_cnt)]
    isfilecnt_max = 0

    bar_max = 0
    for n in range(os_cnt):
        os[n] = ms.get_output_section_by_name(os_names[n])

        isfiles = [os[n].get_input_section_at_address(x).input_file.split('[')[0] for x in os[n].get_input_sections()]
        isfiles = list(dict.fromkeys(isfiles))

        isfile_cnt[n] = len(isfiles)
        isfile_names[n] = list(isfiles)
        is_total = []
        for isfile in isfiles:
            is_list = [x for x in range(os[n].get_input_sections_count()) if isfile in os[n].get_input_section_at_index(x).input_file]
            is_sizes = [os[n].get_input_section_at_index(x).size_bytes for x in is_list]
            is_total.append(sum(is_sizes))
        isfile_totals[n] = is_total
        bar_max = max(bar_max, sum(is_total))

    isfilecnt_max = max(isfile_cnt)

    size_div = (1024.0)
    size_str = 'KB'
    num_steps = 8
    bar_step = (bar_max / size_div) / num_steps
    if (bar_max / size_div) > 32:
        bar_step = (math.ceil(bar_step / 8.0)) * 8.0

    ### load data from output sections and files
    cell_text = []
    rows = []
    rows_desc = []
    rows_group = []
    row_data_size = []
    row_data_file_names = []
    row_data_file_groups = []

    rows_index = 0

    for idx in range(isfilecnt_max):
        bar_used = [0 for osidx in range(os_cnt)]
        row_used = ['' for osidx in range(os_cnt)]
        file_name_used = ['' for osidx in range(os_cnt)]
        file_group_used = ['' for osidx in range(os_cnt)]

        for n in range(os_cnt):
            if idx < isfile_cnt[n]:
                bar_used[n] = isfile_totals[n][idx] / size_div
                if bar_used[n] > 0:
                    row_used[n] = "%s %.4f%s" % (isfile_names[n][idx], bar_used[n], size_str)
                    file_name = isfile_names[n][idx]
                    file_name_used[n] = file_name
                    file_group = xmlparse.ParseMemXML.g_fileGroupClassifier.classify(file_name)
                    file_group_used[n] = file_group

        if sum(bar_used) > 0:
            cell_text.append(row_used)
            rows_index += 1
            rows.append(str(rows_index))
            rows_desc.append(str(rows_index) + '_desc')
            rows_group.append(str(rows_index) + '_group')

            row_data_size.append(bar_used)
            row_data_file_names.append(file_name_used)
            row_data_file_groups.append(file_group_used)

    rows = rows[::-1]
    rows_desc = rows_desc[::-1]
    rows_group = rows_group[::-1]
    cell_text.reverse()
    row_data_size.reverse()
    row_data_file_names.reverse()
    row_data_file_groups.reverse()


    ############# graph ################
    ### set colors
    stack_colors = turbo(len(rows))

    ### set data frame
    df_size = pd.DataFrame(row_data_size, index = rows, columns = os_names)
    df_desc = pd.DataFrame(row_data_file_names, index = rows_desc, columns = os_names)
    df_group = pd.DataFrame(row_data_file_groups, index = rows_group, columns = os_names)
    df_size = df_size.swapaxes("index", "columns")
    df_desc = df_desc.swapaxes("index", "columns")
    df_group = df_group.swapaxes("index", "columns")

    df = pd.concat([df_size, df_desc, df_group], axis = 1)

    # create [ (Group, Output Section) ... ]
    x_group = [ (xmlparse.ParseMemXML.g_memoryGroupClassifier.classify(os_name) + " group", os_name) for os_name in os_names ]
    df['x_group'] = x_group

    """
    with pd.option_context('display.max_rows', None, 'display.max_columns', None, 'display.max_colwidth', 256):
        display(df)
    """

    ### make plot
    """
    note: the columns with same name will be merged, so we should use different name to mark multiple 'used' and
    'unused' columns
    """
    source = ColumnDataSource(df)

    p = figure(x_range = FactorRange(*x_group),
               plot_height = 800,
               plot_width = 850,
               title = memory_section + " - Detailed")

    renderers = p.vbar_stack(rows[::-1], x = 'x_group', color = stack_colors, width = 0.9, source = source, line_width = 0)

    ### custom tooltips and format
    for r in renderers:
        status = r.name
        status_desc = status + "_desc"
        status_group = status + "_group"

        hover = HoverTool(tooltips = [
            ('Input File', "@%s" % status_desc),
            ('File Group', "@%s" % status_group),
            ("Size", "@$name{(0.[0000])}%s" % size_str)
            ], renderers = [r])
        p.add_tools(hover)

    p.y_range.start = 0
    p.x_range.range_padding = 0.1
    p.xgrid.grid_line_color = None
    p.xaxis.major_label_orientation = 1
    p.axis.minor_tick_line_color = None
    p.outline_line_color = None
    p.title.text_font_size = '20pt'
    p.title.align = 'center'

    ### set y axis tick and format
    p.yaxis.ticker = np.arange(0, 10 * bar_step, bar_step)
    p.yaxis.formatter = NumeralTickFormatter(format = "0.[00000]")
    p.yaxis.axis_label = size_str
    p.ygrid.ticker = p.yaxis.ticker


    ########## data table ###########
    df_table = pd.DataFrame(cell_text, columns = os_names)

    Columns = [TableColumn(field = Ci, title = Ci, width = 400) for Ci in df_table.columns] # bokeh columns
    data_table = DataTable(columns = Columns, source = ColumnDataSource(df_table), width = 1500, sortable = False) # bokeh table


    ######### set layout ##############
    layout = Column(p, data_table)

    return layout


#*********************************************************************************
#  Function: img_memory_section_memory_grouped(img, memory_blocks, memory_section)
#
#  Description: load memory section usage data to make plot and data table
#
#  Parameters: img - ldf_memory_blocks object
#              memory_blocks - predefined array
#              memory_section - string
#
#  Return: Column layout containing plot and data table
#
#*********************************************************************************
def img_memory_section_memory_grouped(img, memory_blocks, memory_section):
    ### load memory block, memory sections and output sections
    parent = None
    for memory_block in memory_blocks:
        mb = img.get_memory_block_by_name(memory_block)
        if memory_section in mb.get_memory_sections():
            parent = mb
            break
    if parent == None:
        return

    mb = parent
    ms = mb.get_memory_section_by_name(memory_section)
    os_names = ms.get_output_sections()
    os_cnt = len(os_names)
    if os_cnt == 0:
        text = 'Info: No output section present in the memory section - %s' % memory_section
        return Paragraph(text = text)

    ### load data from output_sections_groups
    output_sections_groups = ms.get_output_sections_groups()

    os_group_cnt_max = 0
    os_group_size_max = 0
    for group in output_sections_groups.keys():
        os_group_cnt_max = max(os_group_cnt_max, len(output_sections_groups[group].keys()))
        os_group_size = 0
        for os_name in output_sections_groups[group].keys():
            os_group_size += output_sections_groups[group][os_name].size_bytes
        os_group_size_max = max(os_group_size_max, os_group_size)

    size_div = (1024.0)
    size_str = 'KB'
    num_steps = 8
    bar_step = (os_group_size_max / size_div) / num_steps
    if (os_group_size_max / size_div) > 32:
        bar_step = (math.ceil(bar_step / 8.0)) * 8.0

    ### set array for dataframe
    groups_cnt = len(output_sections_groups.keys())

    group_names = []
    cell_text = []
    rows = []
    rows_desc = []
    row_data_size = []
    row_data_os_names = []

    rows_index = 0
    for idx in range(os_group_cnt_max):
        bar_used = [0 for group_idx in range(groups_cnt)]
        row_used = ['' for group_idx in range(groups_cnt)]
        os_name_used = ['' for group_idx in range(groups_cnt)]

        rows_index += 1
        rows.append(str(rows_index))
        rows_desc.append(str(rows_index) + '_desc')
        row_data_size.append(bar_used)
        cell_text.append(row_used)
        row_data_os_names.append(os_name_used)

    group_idx = 0
    for group in output_sections_groups.keys():
        group_names.append(group)
        os_name_idx = 0
        for os_name in output_sections_groups[group].keys():
            row_data_size[os_name_idx][group_idx] = output_sections_groups[group][os_name].size_bytes / size_div
            row_data_os_names[os_name_idx][group_idx] = os_name
            cell_text[os_name_idx][group_idx] = "%s %.4f%s" % (row_data_os_names[os_name_idx][group_idx], row_data_size[os_name_idx][group_idx], size_str)

            os_name_idx += 1
        group_idx += 1

    rows = rows[::-1]
    rows_desc = rows_desc[::-1]
    cell_text.reverse()
    row_data_size.reverse()
    row_data_os_names.reverse()


    ############# graph ################
    ### set colors
    stack_colors = turbo(len(rows))

    ### set data frame
    df_size = pd.DataFrame(row_data_size, index = rows, columns = group_names)
    df_desc = pd.DataFrame(row_data_os_names, index = rows_desc, columns = group_names)
    df_size = df_size.swapaxes("index", "columns")
    df_desc = df_desc.swapaxes("index", "columns")

    df = pd.concat([df_size, df_desc], axis=1)

    """
    with pd.option_context('display.max_rows', None, 'display.max_columns', None, 'display.max_colwidth', 256):
        display(df)
    """

    ### make plot
    """
    note: the columns with same name will be merged, so we should use different name to mark multiple 'used' and
    'unused' columns
    """
    source = ColumnDataSource(df)

    p = figure(x_range = group_names,
               plot_height = 600,
               plot_width = 850,
               title = memory_section + " - Memory Group")

    renderers = p.vbar_stack(rows[::-1], x = 'index', color = stack_colors, width = 0.9, source = source, line_width = 0)

    ### custom tooltips and format
    for r in renderers:
        status = r.name
        status_desc = status + "_desc"

        hover = HoverTool(tooltips = [
            ('Output Section', "@%s" % status_desc),
            ("Size", "@$name{(0.[0000])}%s" % size_str)
            ], renderers = [r])
        p.add_tools(hover)

    p.y_range.start = 0
    p.x_range.range_padding = 0.1
    p.xgrid.grid_line_color = None
    p.axis.minor_tick_line_color = None
    p.outline_line_color = None
    p.title.text_font_size = '20pt'
    p.title.align = 'center'

    ### set y axis tick and format
    p.yaxis.ticker = np.arange(0, 10 * bar_step, bar_step)
    p.yaxis.formatter = NumeralTickFormatter(format = "0.[00000]")
    p.yaxis.axis_label = size_str
    p.ygrid.ticker = p.yaxis.ticker


    ########## data table ###########
    df_table = pd.DataFrame(cell_text, columns = group_names)

    Columns = [TableColumn(field = Ci, title = Ci, width = 400) for Ci in df_table.columns] # bokeh columns
    data_table = DataTable(columns = Columns, source = ColumnDataSource(df_table), width = 1500, sortable = False) # bokeh table


    ######### set layout ##############
    layout = Column(p, data_table)

    return layout


#*********************************************************************************
#  Function: img_memory_section_file_grouped(img, memory_blocks, memory_section)
#
#  Description: load memory section usage data to make plot and data table
#
#  Parameters: img - ldf_memory_blocks object
#              memory_blocks - predefined array
#              memory_section - string
#
#  Return: Column layout containing plot and data table
#
#*********************************************************************************
def img_memory_section_file_grouped(img, memory_blocks, memory_section):
    ### load memory block, memory sections and output sections
    parent = None
    for memory_block in memory_blocks:
        mb = img.get_memory_block_by_name(memory_block)
        if memory_section in mb.get_memory_sections():
            parent = mb
            break
    if parent == None:
        return

    mb = parent
    ms = mb.get_memory_section_by_name(memory_section)
    os_names = ms.get_output_sections()
    os_names = [x for x in os_names if 'Heap' not in x]
    os_names = [x for x in os_names if 'Stack' not in x]
    os_cnt = len(os_names)
    if os_cnt == 0:
        text = 'Info: No output section present in the memory section - %s' % memory_section
        return Paragraph(text = text)

    ### load data from output sections and input sections
    os = [None for osidx in range(os_cnt)]
    file_group_cnt = [0 for osidx in range(os_cnt)]

    os_group_cnt_max = 0
    os_size_max = 0
    for n in range(os_cnt):
        os[n] = ms.get_output_section_by_name(os_names[n])
        input_sections_groups = os[n].get_input_sections_groups()

        group_used = [x for x in input_sections_groups.keys() if len(input_sections_groups[x].keys()) != 0]
        file_group_cnt[n] = len(group_used)

        os_size = 0
        for group in input_sections_groups.keys():
            for start_addr in input_sections_groups[group].keys():
                os_size += input_sections_groups[group][start_addr].size_bytes
        os_size_max = max(os_size_max, os_size)

    os_group_cnt_max = max(file_group_cnt)
    if (os_size_max == 0):
        text = 'Info: No information from current file classifier for the memory section - %s' % memory_section
        return Paragraph(text = text)

    size_div = (1024.0)
    size_str = 'KB'
    num_steps = 8
    bar_step = (os_size_max / size_div) / num_steps
    if (os_size_max / size_div) > 32:
        bar_step = (math.ceil(bar_step / 8.0)) * 8.0

    ### set array for dataframe
    cell_text = []
    rows = []
    rows_desc = []
    row_data_size = []
    row_data_group_names = []

    rows_index = 0
    for idx in range(os_group_cnt_max):
        bar_used = [0 for osidx in range(os_cnt)]
        row_used = ['' for osidx in range(os_cnt)]
        group_name_used = ['' for osidx in range(os_cnt)]

        rows_index += 1
        rows.append(str(rows_index))
        rows_desc.append(str(rows_index) + '_desc')
        row_data_size.append(bar_used)
        cell_text.append(row_used)
        row_data_group_names.append(group_name_used)

    for os_idx in range(os_cnt):
        input_sections_groups = os[os_idx].get_input_sections_groups()

        group_idx = 0
        for group in input_sections_groups.keys():
            group_size = 0
            for start_addr in input_sections_groups[group].keys():
                group_size += input_sections_groups[group][start_addr].size_bytes / size_div

            if group_size > 0:
                row_data_size[group_idx][os_idx] = group_size
                row_data_group_names[group_idx][os_idx] = group
                cell_text[group_idx][os_idx] = "%s %.4f%s" % (row_data_group_names[group_idx][os_idx], row_data_size[group_idx][os_idx], size_str)

                group_idx += 1

    rows = rows[::-1]
    rows_desc = rows_desc[::-1]
    cell_text.reverse()
    row_data_size.reverse()
    row_data_group_names.reverse()


    ############# graph ################
    ### set colors
    stack_colors = turbo(len(rows))

    ### set data frame
    df_size = pd.DataFrame(row_data_size, index = rows, columns = os_names)
    df_desc = pd.DataFrame(row_data_group_names, index = rows_desc, columns = os_names)
    df_size = df_size.swapaxes("index", "columns")
    df_desc = df_desc.swapaxes("index", "columns")

    df = pd.concat([df_size, df_desc], axis = 1)

    # create [ (Group, Output Section) ... ]
    x_group = [ (xmlparse.ParseMemXML.g_memoryGroupClassifier.classify(os_name) + " group", os_name) for os_name in os_names ]
    df['x_group'] = x_group

    """
    with pd.option_context('display.max_rows', None, 'display.max_columns', None, 'display.max_colwidth', 256):
        display(df)
    """

    ### make plot
    """
    note: the columns with same name will be merged, so we should use different name to mark multiple 'used' and
    'unused' columns
    """
    source = ColumnDataSource(df)

    p = figure(x_range = FactorRange(*x_group),
               plot_height = 700,
               plot_width = 850,
               title = memory_section + " - File Group")

    renderers = p.vbar_stack(rows[::-1], x = 'x_group', color = stack_colors, width = 0.9, source = source, line_width = 0)

    ### custom tooltips and format
    for r in renderers:
        status = r.name
        status_desc = status + "_desc"

        hover = HoverTool(tooltips = [
            ('File Group', "@%s" % status_desc),
            ("Size", "@$name{(0.[0000])}%s" % size_str)
            ], renderers = [r])
        p.add_tools(hover)

    p.y_range.start = 0
    p.x_range.range_padding = 0.1
    p.xgrid.grid_line_color = None
    p.axis.minor_tick_line_color = None
    p.xaxis.major_label_orientation = 1
    p.outline_line_color = None
    p.title.text_font_size = '20pt'
    p.title.align = 'center'

    ### set y axis tick and format
    p.yaxis.ticker = np.arange(0, 10 * bar_step, bar_step)
    p.yaxis.formatter = NumeralTickFormatter(format = "0.[00000]")
    p.yaxis.axis_label = size_str
    p.ygrid.ticker = p.yaxis.ticker


    ########## data table ###########
    df_table = pd.DataFrame(cell_text, columns = os_names)

    Columns = [TableColumn(field = Ci, title = Ci, width = 400) for Ci in df_table.columns] # bokeh columns
    data_table = DataTable(columns = Columns, source = ColumnDataSource(df_table), width = 1500, sortable = False) # bokeh table


    ######### set layout ##############
    layout = Column(p, data_table)

    return layout


#*********************************************************************************
#  Function: img_memory_sections(img, memory_blocks):
#
#  Description: get all memory sections excluding cache from memory blocks
#
#  Parameters: img - ldf_memory_blocks object
#              memory_blocks - predefined array
#
#  Return: array containing memory sections excluding cache
#
#*********************************************************************************
def img_memory_sections(img, memory_blocks):
    ms = []
    for memory_block in memory_blocks:
        mb = img.get_memory_block_by_name(memory_block)
        ms.extend(mb.get_memory_sections())
    return [x for x in ms if 'cache' not in x]


#*********************************************************************************
# Function: verify_paths(mapPath, outPath)
# Description: Verifies the following:
#      1. Correctness of the map file path (mapPath)
#      2. Existence of the directory for the .memviz.html
#
#*********************************************************************************
def verify_paths(mapPath, outPath):
    if (mapPath == None) or (outPath == None):
        print ('Error: .map.xml file and output html file needs to be specified')
        return False

    if (not os.path.isfile(mapPath)):
        print ('Error: .map.xml file ' + mapPath + ' could not be found')
        return False

    outDir = os.path.dirname(outPath)
    if (not os.path.isdir(outDir)):
        print ('Error: output html dir ' + outDir + ' could not be found')
        return False

    return True

#*********************************************************************************
# Function: dsp_memory_usage
#
#*********************************************************************************
def visualize_memory_usage(xml_path, html_path, classifierGroup = 'JTest', debug = False, fl_path = None):

    if not verify_paths(xml_path, html_path):
        return False

    root_imgs = xmlparse.ParseMemXML.parseMemXML(xml_path, classifierGroup = classifierGroup, debug = debug, fl_path = fl_path)

    output_file(html_path, title='OpenJADE MemViz v1.0')

    tabs = []
    for root_img in root_imgs:
        img = root_img.get_img()

        lp_root = root_img.get_lp_root()

        # l1 memory blocks usage layout part
        layout_l1_memory_blocks_usage = img_l1_memory_blocks_usage(img, l1_memory_blocks_sharc)

        # memory blocks usage layout part
        tabs_memory_block = []
        for mb in all_memory_blocks:
            layout_mb_usage = img_memory_blocks_usage(img, mb)

            tab_memory_block = Panel(child = layout_mb_usage, title = mb)
            tabs_memory_block.append(tab_memory_block)

        layout_memory_blocks_usage = Tabs(tabs = tabs_memory_block)

        # memory section usage layout part
        tabs_memory_section = []
        for ms in img_memory_sections(img, all_memory_blocks):
            layout_ms_detailed = img_memory_section_usage_detailed(img, all_memory_blocks, ms)
            tab_ms_detailed = Panel(child = layout_ms_detailed, title = "Detailed")

            # layout_ms_grouped
            layout_ms_memroy_grouped = img_memory_section_memory_grouped(img, all_memory_blocks, ms)
            tab_ms_memory_grouped = Panel(child = layout_ms_memroy_grouped, title = "Memory Grouped")

            layout_ms_file_grouped = img_memory_section_file_grouped(img, all_memory_blocks, ms)
            tab_ms_file_grouped = Panel(child = layout_ms_file_grouped, title = "File Grouped")

            tabs_ms_group = []
            tabs_ms_group.append(tab_ms_memory_grouped)
            tabs_ms_group.append(tab_ms_file_grouped)
            layout_ms_group = Tabs(tabs = tabs_ms_group)
            tab_ms_groups = Panel(child = layout_ms_group, title = "Grouped")

            tabs_ms_sub = []
            tabs_ms_sub.append(tab_ms_groups)
            tabs_ms_sub.append(tab_ms_detailed)
            layout_ms_sub = Tabs(tabs = tabs_ms_sub)

            tab_memory_section = Panel(child = layout_ms_sub, title = ms)
            tabs_memory_section.append(tab_memory_section)

        layout_memory_section_usage = Tabs(tabs = tabs_memory_section)

        # tabs of overview/ mb usage / ms usage
        tab_overview = Panel(child = layout_l1_memory_blocks_usage, title = "Overview")
        tab_mb = Panel(child = layout_memory_blocks_usage, title = "Memory Blocks Usage")
        tab_ms = Panel(child = layout_memory_section_usage, title = "Memory Sections Usage")

        tabs_sub = []
        tabs_sub.append(tab_overview)
        tabs_sub.append(tab_mb)
        tabs_sub.append(tab_ms)
        layout = Tabs(tabs = tabs_sub)

        # tabs of LINKPROJECT
        tab = Panel(child = layout, title = lp_root.attrib["name"])
        tabs.append(tab)

    tab = Tabs(tabs = tabs)
    save(tab)


#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()


#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    mapPath = None
    htmlPath = None
    classifierGroup = 'JTest'

    try:
        opts, args = getopt.getopt(argv, "hm:o:g:", ["help", "map=", "out=", "group="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-m", "--map"):
            mapPath = os.path.abspath(arg)
        elif opt in ("-o", "--out"):
            htmlPath = os.path.abspath(arg)
        elif opt in ("-g", "--group"):
            classifierGroup = arg

    if visualize_memory_usage(mapPath, htmlPath, classifierGroup) == False:
        usage()

#*********************************************************************************
if __name__ == "__main__":
    main()
