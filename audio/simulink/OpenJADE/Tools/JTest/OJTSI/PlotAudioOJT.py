import os
import getopt
import sys

try:
    from bokeh.io import output_file, save
    from bokeh.plotting import figure, show
    from bokeh.models.widgets import Tabs, Panel
    from bokeh.models.widgets import NumberFormatter
    from bokeh.layouts import column, row
    from bokeh.models import ColumnDataSource, DataTable, TableColumn
    from bokeh.models import ColorBar, LinearColorMapper
    from bokeh.models import HoverTool
    from bokeh.palettes import Viridis256
    from bokeh.models import Div
except ImportError:
    print ("Info: bokeh package not installed.")
import pandas as pd
try:
    import numpy as np
except ImportError:
    print ("Info: numpy package not installed.")

g_scipy = True
try:
    from scipy import signal
except ImportError:
    g_scipy = False
    print ("Info: scipy package not installed.")

from OJTSI.LoadAudioOJT import AudioGroupOJT

__usage__ = """ \

Usage:
  PlotAudioOJT.py [-h] -i audio_in -o audio_out -r audio_ref
                      -s frame_size
                      -x in_channel_count
                      -y out_channel_count
                      -f output_html_file (.audioplots.html)

Description:

Assumptions:

Options:
  -h,   --help  Display usage information (this message)
  -i,   --in    audioIn.bin file
  -o,   --out   audioOut.bin file
  -r,   --ref   audioRef.bin file
  -s,   --size  frame_size
  -x,   --xcnt  in_channel_cnt
  -y,   --ycnt  out_channel_cnt
  -f,   --file  output_html_file (.audioplots.html)
"""

def get_frequency_panels(panel_type, audio_data, channel_count, fs):
    if g_scipy == False:
        return None

    h = 300
    w = 1000

    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for channel_num in range(channel_count):
        y_values = audio_data[channel_num]

        if True:
            freq, time, Zxx = signal.stft(y_values, fs)
            Sxx = np.abs(Zxx)
        else:
            freq, time, Sxx = signal.spectrogram(y_values, fs)

        df = pd.DataFrame(index = list(freq), data = Sxx, columns = list(time))
        df = pd.DataFrame(df.stack(), columns=['amp']).reset_index()

        spectral_mapper = LinearColorMapper(palette=Viridis256, low=df.amp.min(), high=df.amp.max())

        s = figure(# x_range=time, y_range=freq,
            plot_width=w, plot_height=h,
            tools=TOOLS, toolbar_location='right',
            tooltips=[('amp', '@amp')])

        s.circle(x="level_1", y="level_0", size=5, source=df,
            fill_color={'field': 'amp', 'transform': spectral_mapper},
            line_color=None)

        bar = ColorBar(color_mapper=spectral_mapper, location=(0,0))
        s.add_layout(bar, "left")

        t = '%s Channel %d' % (panel_type, channel_num)
        panels.append(Panel(child=s, title=t))
    return panels

def get_audio_panels(panel_type, audio_data, channel_count):
    h = 300
    w = 1000
    hover = HoverTool(
        tooltips=[
            ("index", "$index"),
            ("value", "$y"),
        ]
    )
    TOOLS = [hover, "xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for channel_num in range(channel_count):
        y_values = audio_data[channel_num]
        x_values = np.arange(y_values.size)

        s = figure(plot_width=w, plot_height=h, toolbar_location="right", tools=TOOLS)
        s.line(x_values, y_values)

        t = '%s Channel %d' % (panel_type, channel_num)
        panels.append(Panel(child=s, title=t))
    return panels

def get_histogram_panels(panel_type, audio_data, channel_count):
    h = 300
    w = 1000
    TOOLS = ["xpan, xbox_zoom, xwheel_zoom, ybox_zoom, ywheel_zoom,  undo, redo, reset"]

    panels = []
    for channel_num in range(channel_count):
        y_values = audio_data[channel_num]
        x_values = np.arange(y_values.size)

        s = figure(title='%s Difference Histogram' % panel_type, plot_width=w, plot_height=h, toolbar_location="right", tools=TOOLS)

        hist, edges = np.histogram(y_values, bins=50)
        s.quad(top=hist, bottom=0, left=edges[:-1], right=edges[1:],
           fill_color="navy", line_color="white", alpha=0.5)
        hover = HoverTool(tooltips = [('Count', '@top'),
                                      ('Range', '(@left,@right)')])
        s.add_tools(hover)
        t = 'Hist Channel %d' % channel_num
        panels.append(Panel(child=s, title=t))
    return panels

def get_error_stats_data_table(panel_type, error_stats):
    source = ColumnDataSource(error_stats)

    columns = [
        TableColumn(field="channel", title="Channel Num"),
        TableColumn(field="min", title="Min", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="q25", title="25%", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="q50", title="50%", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="q75", title="75%", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="max", title="Max", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="mean", title="Mean", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="median", title="Median", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="std", title="Standard Deviation", formatter=NumberFormatter(format="0.000000")),
        TableColumn(field="var", title="Variance", formatter=NumberFormatter(format="0.000000"))
    ]
    data_table = DataTable(source=source, columns=columns, width=1000, height=400)
    return data_table

def plot_audio(audio_in, audio_out, audio_ref, in_frame_size, out_frame_size, in_channel_count, out_channel_count, base_rate_period, output_html_file, option=None, debug=False):
    if debug:
        print('Info: PlotAudio.plot_audio')
        print('\taudio_in %s' % audio_in)
        print('\taudio_out %s' % audio_out)
        print('\taudio_ref %s' % audio_ref)
        if (in_frame_size != None):
            print('\tin_frame_size %d' % in_frame_size)
        else:
            print('\tin_frame_size None')
        print('\tout_frame_size %d' % out_frame_size)
        if (in_channel_count != None):
            print('\tin_channel_count %d' % in_channel_count)
        else:
            print('\tin_channel_count None')
        print('\tout_channel_count %d' % out_channel_count)
        print('\toutput_html_file %s' % output_html_file)

    pa_obj = AudioGroupOJT(audio_in, audio_out, audio_ref, in_frame_size, out_frame_size, in_channel_count, out_channel_count, option=option)

    if (pa_obj.isValid() == False):
        return

    output_dir = os.path.dirname(output_html_file)
    if not os.path.isdir(output_dir):
        print ('Error: output directory ' + output_dir + ' could not be found')
        return

    if base_rate_period != None:
        out_sample_frequency = float(out_frame_size)/float(base_rate_period);
    else:
        out_sample_frequency = 48000.0

    if (in_channel_count != None) and (in_channel_count > 0) and (in_frame_size != None) and (in_frame_size > 0):
        if base_rate_period != None:
            in_sample_frequency = float(in_frame_size)/float(base_rate_period);
        else:
            in_sample_frequency = 48000.0

    output_file(output_html_file, title='PlotAudioOJT')

    mytabs = []

    div = Div(text="<b>Time Domain Plots</b>", width=200, height=100)
    mytabs.append(div)

    if (in_channel_count != None) and (in_channel_count > 0):
        panels = get_audio_panels('In', pa_obj.in_data, in_channel_count)
        mytabs.append(Tabs(tabs=panels))

    panels = get_audio_panels('Out', pa_obj.out_data, out_channel_count)
    mytabs.append(Tabs(tabs=panels))

    panels = get_audio_panels('Ref', pa_obj.ref_data, out_channel_count)
    mytabs.append(Tabs(tabs=panels))

    div = Div(text="<b>Time Domain Differnces</b>", width=200, height=100)
    mytabs.append(div)

    panels = get_audio_panels('Diff', pa_obj.diff_data, out_channel_count)
    mytabs.append(Tabs(tabs=panels))

    panels = get_histogram_panels('Diff', pa_obj.abs_error, out_channel_count)
    mytabs.append(Tabs(tabs=panels))

    data_table = get_error_stats_data_table('', pa_obj.getErrorStats('abs_error'))
    mytabs.append(data_table)

    panels = get_audio_panels('Rel Abs Diff', pa_obj.rel_error, out_channel_count)
    mytabs.append(Tabs(tabs=panels))

    data_table = get_error_stats_data_table('', pa_obj.getErrorStats('rel_error'))
    mytabs.append(data_table)

    div = Div(text="""<a href="https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.stft.html">Short Time Fourier Transform Plots</a>""",
        width=200, height=100)
    if g_scipy:
        mytabs.append(div)

    if (in_channel_count != None) and (in_channel_count > 0):
        panels = get_frequency_panels('In', pa_obj.in_data, in_channel_count, fs=in_sample_frequency)
        if panels:
            mytabs.append(Tabs(tabs=panels))

    panels = get_frequency_panels('Out', pa_obj.out_data, out_channel_count, fs=out_sample_frequency)
    if panels:
        mytabs.append(Tabs(tabs=panels))

    panels = get_frequency_panels('Ref', pa_obj.ref_data, out_channel_count, fs=out_sample_frequency)
    if panels:
        mytabs.append(Tabs(tabs=panels))

    save(column(mytabs))

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
    audio_in = None
    audio_out = None
    audio_ref = None
    frame_size = None
    in_channel_count = None
    out_channel_count = None
    output_html_file = None

    try:
        opts, args = getopt.getopt(argv, "hi:o:r:s:x:y:f:", ["help", "in=", "out=", "ref=","size=","xcnt=","ycnt=","file="])
    except getopt.GetoptError:
        usage()
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-i", "--in"):
            audio_in = os.path.abspath(arg)
        elif opt in ("-o", "--out"):
            audio_out = os.path.abspath(arg)
        elif opt in ("-r", "--ref"):
            audio_ref = os.path.abspath(arg)
        elif opt in ("-s", "--size"):
            frame_size = int(arg)
            in_frame_size = frame_size
            out_frame_size = frame_size
        elif opt in ("-x", "--xcnt"):
            in_channel_count = int(arg)
        elif opt in ("-y", "--ycnt"):
            out_channel_count = int(arg)
        elif opt in ("-f", "--file"):
            output_html_file = os.path.abspath(arg)

    if (audio_in == None) or (audio_out == None) or (audio_ref == None) \
        or (frame_size == None) \
        or (in_channel_count == None) or (out_channel_count == None) \
        or (output_html_file == None):
        usage()
        sys.exit()
    base_rate_period = float(frame_size)/48000.0
    plot_audio(audio_in, audio_out, audio_ref, in_frame_size, out_frame_size, in_channel_count, out_channel_count, base_rate_period, output_html_file)

#*********************************************************************************
if __name__ == "__main__":
    main()



