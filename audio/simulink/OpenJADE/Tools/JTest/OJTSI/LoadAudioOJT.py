import os
try:
    import numpy as np
except ImportError:
    print ("Info: numpy package not installed.")
try:
    from scipy.io import wavfile
except ImportError:
    print ("Info: scipy package not installed.")

# WAV files have multiple formats with different min and max values. In order
# to normalize data in WAV files, it needs to be divided by the appropriate max
# value. These are the WAV formats, their respective max values, and NumPy
# dtype.
#
MAX_WAV_FLOAT32 = 1.0           # dtype: float32
MAX_WAV_INT32 = 2147483648      # dtype: int32
MAX_WAV_INT16 = 32768           # dtype: int16
MAX_WAV_INT8 = 255              # dtype: uint8
# Source
# https://docs.scipy.org/doc/scipy/reference/generated/scipy.io.wavfile.read.html

class LoadAudioOJT:
    def __init__(self, audio_file, frame_size, channel_count):
        self.audio_file = audio_file
        self.frame_size = frame_size
        self.channel_count = channel_count

        if not os.path.isfile(audio_file):
            print ( 'Error: audio file ' + audio_file + ' could not be found' )
            self.is_valid = False
            return

        if audio_file.endswith('.wav'):
            sr, data = wavfile.read(audio_file)

            # Data was read successfully
            # Normalize based on format
            dt = data.dtype
            if dt == np.float32:
                data = np.array(data) / MAX_WAV_FLOAT32
            elif dt == np.int32:
                data = np.array(data) / MAX_WAV_INT32
            elif dt == np.int16:
                data = np.array(data) / MAX_WAV_INT16
            else: # dt == np.uint8
                data = np.array(data) / MAX_WAV_INT8

            self.frame_cnt = int(data.shape[0]/self.frame_size)
            data = data.reshape((self.frame_cnt, self.frame_size, self.channel_count))
            self.data = np.swapaxes(data, 1, 2)
        else:
            data = np.fromfile(self.audio_file, dtype='f4')
            self.frame_cnt = int(data.shape[0]/(self.channel_count*self.frame_size))
            data = data[:(self.frame_cnt*self.channel_count*self.frame_size)]
            self.data = data.reshape((self.frame_cnt, self.channel_count, self.frame_size))
        self.is_valid = True

    def isValid(self):
        return self.is_valid

    def getFrameCnt(self):
        return self.frame_cnt

    def getAudioSamples(self):
        return self.data

    def getAudioSamplesForChannel(self, channel_num):
        if (channel_num >= 0) and (channel_num < self.channel_count):
            data = self.data[:, channel_num, :]
            return data.reshape(-1)
        else:
            print ('Error: Invalid channel number')
            return None

    def getAudioSamplesForChannelByIndex(self, channel_num, start_idx, end_idx):
        max_idx = self.frame_cnt * self.frame_size
        if (start_idx < 0) or (start_idx >= max_idx):
            print('Error: start index out of range')
            return None

        if (end_idx < 0) or (end_idx > max_idx):
            print('Error: end index out of range')
            return none

        if (start_idx >= end_idx):
            print('Error: start index larger than or equal to end index')
            return None

        data = self.data[:, channel_num, :]
        return data.reshape(-1)[start_idx:end_idx]

    def getAudioSamplesForChannelByFrame(self, channel_num, start_frame, end_frame):
        if (start_frame < 0) or (start_frame >= self.frame_cnt):
            print('Error: start frame out of range')
            return None
        if (end_frame < 0) or (end_frame > self.frame_cnt):
            print('Error: end frame out of range')
            return None
        if (start_frame >= end_frame):
            print('Error: start frame larger than or equal to end frame')
            return None

        data = self.data[start_frame:end_frame, channel_num, :]
        return data.reshape(-1)

class AudioGroupOJT:
    def __init__(self, audio_in, audio_out, audio_ref, in_frame_size, out_frame_size, in_channel_count, out_channel_count, option=None):
        self.audio_in = audio_in
        self.audio_out = audio_out
        self.audio_ref = audio_ref
        self.in_frame_size = in_frame_size
        self.out_frame_size = out_frame_size
        self.in_channel_count = in_channel_count
        self.out_channel_count = out_channel_count

        if (in_channel_count != None) and (in_channel_count > 0):
            in_obj = LoadAudioOJT(audio_in, in_frame_size, in_channel_count)
            if (in_obj.isValid() == False):
                self.is_valid = False
                return
        else:
            in_obj = None

        out_obj = LoadAudioOJT(audio_out, out_frame_size, out_channel_count)
        ref_obj = LoadAudioOJT(audio_ref, out_frame_size, out_channel_count)

        if (out_obj.isValid() == False) or (ref_obj.isValid() == False):
            self.is_valid = False
            return

        if in_obj == None:
            self.frame_cnt = min (out_obj.getFrameCnt(), ref_obj.getFrameCnt()) - 2
        else:
            self.frame_cnt = min (in_obj.getFrameCnt(), out_obj.getFrameCnt(), ref_obj.getFrameCnt()) - 2

        self.ref_frame_shift = {}
        for channel_num in range(out_channel_count):
            self.ref_frame_shift[channel_num] = 0

        if option != None:
            if ('RefAudio_FrameShift' in option) and option['RefAudio_FrameShift']:
                if isinstance(option['RefAudio_FrameShift'], dict):
                    for channel_num in range(out_channel_count):
                        if channel_num in option['RefAudio_FrameShift']:
                            self.ref_frame_shift[channel_num] = option['RefAudio_FrameShift'][channel_num]

                if isinstance(option['RefAudio_FrameShift'], int):
                    for channel_num in range(out_channel_count):
                        self.ref_frame_shift[channel_num] = option['RefAudio_FrameShift']

        self.in_data = {}
        self.out_data = {}
        self.ref_data = {}
        self.diff_data = {}
        self.abs_error = {}
        self.rel_error = {}

        if in_obj != None:
            for channel_num in range(in_channel_count):
                self.in_data[channel_num] = in_obj.getAudioSamplesForChannelByFrame(channel_num, start_frame=0, end_frame=self.frame_cnt)

        for channel_num in range(out_channel_count):
            start_frame = 0
            end_frame = self.frame_cnt
            ref_frame_shift = self.ref_frame_shift[channel_num]
            self.out_data[channel_num] = out_obj.getAudioSamplesForChannelByFrame(channel_num, start_frame=start_frame+ref_frame_shift, end_frame=end_frame)
            self.ref_data[channel_num] = ref_obj.getAudioSamplesForChannelByFrame(channel_num, start_frame=start_frame, end_frame=end_frame-ref_frame_shift)
            self.diff_data[channel_num] = np.subtract(self.out_data[channel_num], self.ref_data[channel_num])
            self.abs_error[channel_num] = np.absolute(self.diff_data[channel_num])
            self.rel_error[channel_num] = (self.abs_error[channel_num] * 100.0) / np.maximum( np.absolute(self.ref_data[channel_num]), np.finfo('f4').eps )

        self.is_valid = True

    def isValid(self):
        return self.is_valid

    def getFrameCnt(self):
        return self.frame_cnt

    def getErrorStats(self, error_type):
        channel_count = self.out_channel_count
        if error_type == 'rel_error':
            error_samples = self.rel_error
        elif error_type == 'abs_error':
            error_samples = self.abs_error
        else:
            return None

        error_stats = {}
        error_stats['channel'] = []
        error_stats['min'] = []
        error_stats['q25'] = []
        error_stats['q50'] = []
        error_stats['q75'] = []
        error_stats['max'] = []
        error_stats['mean'] = []
        error_stats['median'] = []
        error_stats['std'] = []
        error_stats['var'] = []

        for channel_num in range(channel_count):
            error_stats['channel'].append(channel_num)
            error_stats['min'].append(np.amin(error_samples[channel_num]))
            error_stats['q25'].append(np.percentile(error_samples[channel_num],25))
            error_stats['q50'].append(np.percentile(error_samples[channel_num],50))
            error_stats['q75'].append(np.percentile(error_samples[channel_num],75))
            error_stats['max'].append(np.amax(error_samples[channel_num]))
            error_stats['mean'].append(np.mean(error_samples[channel_num]))
            error_stats['median'].append(np.median(error_samples[channel_num]))
            error_stats['std'].append(np.std(error_samples[channel_num]))
            error_stats['var'].append(np.var(error_samples[channel_num]))
        return error_stats
