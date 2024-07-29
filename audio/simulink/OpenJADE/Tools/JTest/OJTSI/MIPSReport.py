import time, os
import sys
import getopt
import struct

from OJTSI.ExecLogOJT import ExecLogOJT

__usage__ = """ \

Usage:
  MIPSReport.py [-h] [-f frame_size] [-s sample_rate] -i input_file -o output_file

Description:

Options:
  -h,   --help   Display usage information (this message)
  -f,   --frame_size  Frame Size  (default 32)
  -s,   --sample_rate Sample Rate (default 48000)
  -i,   --in     Input .run.log file
  -o,   --out    Output .mips.log file
"""

def GenMIPSReport (frame_size=None, sample_rate=None, clock_rate=None, callrates_list=None, scaling_factors=None, process_list=None, skip_frames=0, probe_points=None, accel_process_list=None, input_file=None, output_file=None) :
    if not os.path.isfile(input_file):
        print ('Error: input file ' + input_file + ' could not be found')
        return

    output_dir = os.path.dirname(output_file)
    if not os.path.isdir(output_dir):
        print ('Error: output directory ' + output_dir + ' could not be found')
        return

    input_lines = []
    with open(input_file, 'r') as fp:
        line = fp.readline()
        while line:
            line = line.strip()
            if (len(line) > 0) and (line.startswith('Bg') or line.startswith('En')):
                input_lines.append(line)
            line = fp.readline()
    oj_exec_obj = ExecLogOJT(frame_size=frame_size,
                            sample_rate=sample_rate,
                            clock_rate=clock_rate,
                            callrates_list=callrates_list,
                            scaling_factors=scaling_factors,
                            process_list=process_list,
                            skip_frames=skip_frames,
                            probe_points=probe_points,
                            accel_process_list=accel_process_list,
                            input_lines=input_lines)
    oj_exec_obj.run()

    mips_report_df = oj_exec_obj.get_complete_mips_report_df()
    mips_total_df = oj_exec_obj.get_complete_mips_totals_report_df()
    cycles_report_df = oj_exec_obj.get_complete_cycles_report_df()

    with open(output_file, 'w') as mips_log_fp:
        mips_log_fp.write(cycles_report_df.to_csv(line_terminator='\n'))
        mips_log_fp.write('\n\n')
        mips_log_fp.write(mips_report_df.to_csv(line_terminator='\n'))
        mips_log_fp.write('\nTotals\n')
        mips_log_fp.write(mips_total_df.to_csv(line_terminator='\n',index=False))
        mips_log_fp.write('\n')

        if scaling_factors != None:
            mips_report_df = oj_exec_obj.get_complete_scaled_mips_report_df()
            mips_total_df = oj_exec_obj.get_complete_scaled_mips_totals_report_df()
            scaling_factors_df = oj_exec_obj.get_scaling_factors_df()
            mips_log_fp.write('\nScaled\n')
            mips_log_fp.write(mips_report_df.to_csv(line_terminator='\n'))
            mips_log_fp.write('\nTotals\n')
            mips_log_fp.write(mips_total_df.to_csv(line_terminator='\n',index=False))
            mips_log_fp.write('\n')
            mips_log_fp.write('\nScaling Factors\n')
            mips_log_fp.write(scaling_factors_df.to_csv(line_terminator='\n',index=False))
            mips_log_fp.write('\n')

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    frame_size = None
    sample_rate = None
    input_file = None
    output_file = None

    try:
        opts, args = getopt.getopt(argv, 'hf:s:i:o:', ['help','frame_size=','sample_rate=','in=','out='])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-f', '--frame_size'):
            frame_size = arg
        elif opt in ('-s', '--sample_rate'):
            sample_rate = arg
        elif opt in ('-i', '--in'):
            input_file = os.path.abspath(arg)
            if not os.path.isfile(input_file):
                print ('Error: input file ' + input_file + ' could not be found')
                sys.exit()
        elif opt in ('-o', '--out'):
            output_file = os.path.abspath(arg)
            output_dir = os.path.dirname(output_file)
            if not os.path.isdir(output_dir):
                print ('Error: output directory ' + output_dir + ' could not be found')
                sys.exit()

    if (input_file == None) or (output_file == None):
        usage()
        sys.exit()
    return frame_size, sample_rate, input_file, output_file

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    frame_size, sample_rate, input_file, output_file = parse_args(argv)
    GenMIPSReport(frame_size=frame_size, sample_rate=sample_rate, clock_rate=None,
                  callrates_list=None, scaling_factors=None, process_list=None,
                  skip_frames=0,
                  probe_points=None, accel_process_list=None,
                  input_file=input_file, output_file=output_file)

#*********************************************************************************
if __name__ == '__main__':
    main()
