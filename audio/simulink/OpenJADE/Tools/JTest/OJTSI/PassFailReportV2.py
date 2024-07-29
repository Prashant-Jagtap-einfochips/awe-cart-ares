import os
import sys
import getopt
import struct
from OJTSI.LoadAudioOJT import LoadAudioOJT
try:
    import numpy as np
except ImportError:
    print ("Info: numpy package not installed.")

__usage__ = """ \

Usage:
  PassFailReportV2.py [-h] -n test_name -c test_case -p test_param -t tolerance
                        -o audio_out -r audio_ref
                        -s frame_size
                        -y channel_count
                        -f output_status_log (.status.log)

Description:

Options:
  -h,   --help   Display usage information (this message)
  -n,   --name   Test Name
  -c,   --case   Test Case
  -p,   --param  Test Param
  -t,   --tol    Test Tolerance
  -o,   --out    audioOut.bin file
  -r,   --ref    audioRef.bin file
  -s,   --size   frame_size
  -y,   --ycnt   out_channel_cnt
  -f,   --file   Output .status.log file

"""

class ChannelStats:
    def __init__(self, id, max, min, avg, tolerance):
        self.id = id
        self.max = max
        self.min = min
        self.avg = avg
        self.tolerance = tolerance
        return

    def __str__(self):
        mystr = 'channel[%d].stats max %.9f min %.9f avg %.9f' % (self.id, self.max, self.min, self.avg)
        if self.status():
            mystr = mystr + ' :Passed'
        else:
            mystr = mystr + ' :Failed'
        return mystr

    def __repr__(self):
        return self.__str__()

    def status(self):
        if self.max < self.tolerance:
            return True
        else:
            return False

class PassFailReportV2:
    def __init__(self, test_name, test_case, test_param, tolerance, audio_out, audio_ref, frame_size, channel_count, option, output_file):
        self.test_name = test_name
        self.test_case = test_case
        self.test_param = test_param
        self.tolerance = tolerance
        self.audio_out = audio_out
        self.audio_ref = audio_ref
        self.frame_size = frame_size
        self.channel_count = channel_count
        self.output_file = output_file

        self.ref_frame_shift = {}
        for channel_num in range(channel_count):
            self.ref_frame_shift[channel_num] = 0

        if option != None:
            if ('RefAudio_FrameShift' in option) and option['RefAudio_FrameShift']:
                if isinstance(option['RefAudio_FrameShift'], dict):
                    for channel_num in range(channel_count):
                        if channel_num in option['RefAudio_FrameShift']:
                            self.ref_frame_shift[channel_num] = option['RefAudio_FrameShift'][channel_num]

                if isinstance(option['RefAudio_FrameShift'], int):
                    for channel_num in range(channel_count):
                        self.ref_frame_shift[channel_num] = option['RefAudio_FrameShift']

        return

    def run(self):
        print ('Info: using %.9g as tolerance' % (self.tolerance))

        out_obj = LoadAudioOJT(self.audio_out, self.frame_size, self.channel_count)
        ref_obj = LoadAudioOJT(self.audio_ref, self.frame_size, self.channel_count)

        if (out_obj.isValid() == False) or (ref_obj.isValid() == False):
            print('Error: failed to load audio from %s or %s' % (self.audio_out, self.audio_ref))
            return

        frame_cnt = min (out_obj.getFrameCnt(), ref_obj.getFrameCnt()) - 2

        self.out_data = {}
        self.ref_data = {}
        self.diff_data = {}
        self.abs_error = {}
        self.max_error = {}
        self.min_error = {}
        self.avg_error = {}
        self.channel_stats = {}

        for channel_num in range(self.channel_count):
            start_frame = 0
            end_frame = frame_cnt
            ref_frame_shift = self.ref_frame_shift[channel_num]
            self.out_data[channel_num] = out_obj.getAudioSamplesForChannelByFrame(channel_num, start_frame=start_frame+ref_frame_shift, end_frame=end_frame)
            self.ref_data[channel_num] = ref_obj.getAudioSamplesForChannelByFrame(channel_num, start_frame=start_frame, end_frame=end_frame-ref_frame_shift)
            self.diff_data[channel_num] = np.subtract(self.out_data[channel_num], self.ref_data[channel_num])
            self.abs_error[channel_num] = np.absolute(self.diff_data[channel_num])
            self.max_error[channel_num]  = np.amax(self.abs_error[channel_num] )
            self.min_error[channel_num]  = np.amin(self.abs_error[channel_num] )
            self.avg_error[channel_num]  = np.mean(self.abs_error[channel_num] )
            self.channel_stats[channel_num] = ChannelStats(channel_num,
                                                           self.max_error[channel_num],
                                                           self.min_error[channel_num],
                                                           self.avg_error[channel_num],
                                                           self.tolerance)

        status = 'Passed'
        for channel_num in range(self.channel_count):
            print(str(self.channel_stats[channel_num]))
            if not self.channel_stats[channel_num].status():
                status = 'Failed'
        if status == 'Failed':
            print ('Error: Test '+self.test_name+':'+self.test_case+' failed')
        else:
            print ('Success: Test '+self.test_name+':'+self.test_case+' passed')
        with open(self.output_file, 'w') as fp:
            for channel_num in range(self.channel_count):
                fp.write(str(self.channel_stats[channel_num])+'\n')
            fp.write('%s/%s(%s) : %s\n' % (self.test_name, self.test_case, self.test_param, status))
        return

#*********************************************************************************
# parse_args(argv)
#*********************************************************************************
def parse_args(argv):
    test_name = None
    test_case = None
    test_param = None
    tolerance = None
    audio_out = None
    audio_ref = None
    frame_size = None
    channel_count = None
    output_file = None

    try:
        opts, args = getopt.getopt(argv, 'hn:c:p:t:o:r:s:x:y:f:', ['help','name=','case=','param=','tol=','out=','ref=','size=','xcnt=','ycnt=','file='])
    except getopt.GetoptError:
        usage()
        sys.exit()
    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit()
        elif opt in ('-n', '--name'):
            test_name = arg
        elif opt in ('-c', '--case'):
            test_case = arg
        elif opt in ('-p', '--param'):
            test_param = arg
        elif opt in ('-t', '--tol'):
            tolerance = float(arg)
        elif opt in ("-o", "--out"):
            audio_out = os.path.abspath(arg)
        elif opt in ("-r", "--ref"):
            audio_ref = os.path.abspath(arg)
        elif opt in ("-s", "--size"):
            frame_size = int(arg)
        elif opt in ("-y", "--ycnt"):
            channel_count = int(arg)
        elif opt in ("-f", "--file"):
            output_file = os.path.abspath(arg)

    if (test_name == None) or (test_case == None) or (tolerance == None) \
        or (audio_out == None) or (audio_ref == None) \
        or (frame_size == None) \
        or (channel_count == None) \
        or (output_file == None):
        usage()
        sys.exit()

    if (test_param == None):
        test_param = ''

    pf = PassFailReportV2(test_name, test_case, test_param, tolerance, audio_out, audio_ref, frame_size, channel_count, output_file)
    return pf

#*********************************************************************************
# usage()
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)

def GenPassFailReportV2 (test_name=None, test_case=None, test_param=None, tolerance=None, audio_out=None, audio_ref=None, frame_size=None, channel_count=None, option=None, output_file=None) :
    if (test_name == None) or (test_case == None) or (tolerance == None) \
        or (audio_out == None) or (audio_ref == None) \
        or (frame_size == None) \
        or (channel_count == None) \
        or (output_file == None):
        print ('Error: one or more input paramters set to None')
        return

    if (test_param == None):
        test_param = ''

    output_dir = os.path.dirname(output_file)
    if not os.path.isdir(output_dir):
        print ('Error: output directory ' + output_dir + ' could not be found')
        return

    pf = PassFailReportV2(test_name=test_name, test_case=test_case, test_param=test_param, tolerance=tolerance,  audio_out=audio_out, audio_ref=audio_ref, frame_size=frame_size, channel_count=channel_count, option=option, output_file=output_file)
    pf.run()

#*********************************************************************************
# main()
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    pf = parse_args(argv)
    pf.run()

#*********************************************************************************
if __name__ == '__main__':
    main()
