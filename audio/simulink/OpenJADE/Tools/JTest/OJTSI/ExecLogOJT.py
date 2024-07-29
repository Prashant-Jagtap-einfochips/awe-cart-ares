import struct
import pandas as pd
import numpy as np

import OJTSI.TimeObjOJT

class MIPSReport:
    def __init__(self, max_cycles, min_cycles, avg_cycles, max_mips, min_mips, avg_mips):
        self.max_cycles = max_cycles
        self.min_cycles = min_cycles
        self.avg_cycles = avg_cycles

        self.max_mips = max_mips
        self.min_mips = min_mips
        self.avg_mips = avg_mips

    def __str__(self):
        mystr = '%d, %d, %.2f, %.2f, %.2f, %.2f' % (self.max_cycles, self.min_cycles, self.avg_cycles, self.max_mips, self.min_mips, self.avg_mips)
        return mystr

    def __repr__(self):
        return self.__str__()

class MIPSReporter:
    def __init__(self, frame_size=None, sample_rate=None, clock_rate=None, callrates_list=None):
        self.cycles_to_mips_method = None

        if callrates_list != None:
            self.cycles_to_mips_method = 'rate_period'
            self.callrates_list = callrates_list
            return

        if frame_size == None or sample_rate == None:
            if clock_rate == None:
                print ('Error: undefined MIPS measurement process')
            else:
                self.cycles_to_mips_method = 'clock_rate'
                self.clock_rate = clock_rate
        else:
            self.frame_size = frame_size
            self.sample_rate = sample_rate
            self.cycles_to_mips_method = 'sample_rate_frame_size'

    def isValid(self):
        if self.cycles_to_mips_method == None:
            return False
        return True

    def cycles_to_mips(self, cycles, frame_duration=None, thread_level=None):
        if self.cycles_to_mips_method == None:
            return 0.0

        if self.cycles_to_mips_method == 'rate_period':
            rate_period = self.callrates_list[thread_level]['rate_period']
            return (float(cycles)/float(rate_period))/(1000000)
        elif self.cycles_to_mips_method == 'sample_rate_frame_size':
            #self.clock_rate = 1e9
            #r0 = (float(cycles)*self.sample_rate/self.frame_size)/(1000000)
            #r1 = (float(cycles)*self.clock_rate/frame_duration)/(1000000)
            #print ('Info: mips r0 %f r1 %f\n' % (r0, r1))
            return (float(cycles)*self.sample_rate/self.frame_size)/(1000000)
        elif self.cycles_to_mips_method == 'clock_rate':
            return (float(cycles)*self.clock_rate/frame_duration)/(1000000)
        else:
            print ('Error: undefined MIPS measurement process')
            return 0.0

    def get_mips_report(self, time_objects, thread_level):
        if self.cycles_to_mips_method == None:
            return None

        if (len(time_objects) == 0)  or (time_objects == None):
            return None

        max_cycles = None
        min_cycles = None
        total_cycles = None
        count = 0
        for time_object in time_objects:
            time_stamp = time_object.time_stamp
            if count == 0:
                max_cycles = time_stamp
                min_cycles = time_stamp
                total_cycles = time_stamp
            else:
                max_cycles = max(max_cycles, time_stamp)
                min_cycles = min(min_cycles, time_stamp)
                total_cycles = total_cycles + time_stamp
            count = count + 1

        count = 0
        begin_current = 0

        for time_object in time_objects:
            begin_prev = begin_current
            begin_current = time_object.start_time
            if count == 0:
                begin_current = time_object.start_time
            else:
                frame_cycles = begin_current - begin_prev
                if count == 1:
                    max_frame_cycles = frame_cycles
                    min_frame_cycles = frame_cycles
                    total_frame_cycles = frame_cycles
                else:
                    max_frame_cycles = max(max_frame_cycles, frame_cycles)
                    min_frame_cycles = min(min_frame_cycles, frame_cycles)
                    total_frame_cycles = total_frame_cycles + frame_cycles
            count = count + 1

        if count > 1:
            avg_frame_cycles = float(total_frame_cycles)/(count-1)
        else:
            avg_frame_cycles = None
        #print ('Info: frame_cycles max %d avg %d min %d' % (max_frame_cycles, avg_frame_cycles, min_frame_cycles))

        avg_cycles = float(total_cycles)/count
        max_mips = self.cycles_to_mips(max_cycles, avg_frame_cycles, thread_level)
        min_mips = self.cycles_to_mips(min_cycles, avg_frame_cycles, thread_level)
        avg_mips = self.cycles_to_mips(avg_cycles, avg_frame_cycles, thread_level)

        #Populate individual MIPS for each time pair
        for time_object in time_objects:
            time_object.mips = self.cycles_to_mips(time_object.time_stamp, avg_frame_cycles, thread_level)

        mips_report = MIPSReport(max_cycles, min_cycles, avg_cycles, max_mips, min_mips, avg_mips)
        return mips_report

class ExecLogOJT:
    def __init__(self, frame_size, sample_rate, clock_rate, callrates_list, scaling_factors, process_list, skip_frames, probe_points, accel_process_list, input_lines):
        self.frame_size = frame_size
        self.sample_rate = sample_rate
        self.clock_rate = clock_rate
        self.input_lines = input_lines
        self.time_list = None
        self.callrates_list = callrates_list
        self.scaling_factors = scaling_factors
        self.process_list = process_list
        self.skip_frames = skip_frames
        self.probe_points = probe_points
        self.accel_process_list = accel_process_list
        self.mips_report = None

        mips_reporter = MIPSReporter(frame_size, sample_rate, clock_rate, callrates_list)
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

    def get_scaling_factors(self):
        scaling_factors = {}
        if (self.callrates_list != None) or (self.scaling_factors == None):
            for thread_level in self.get_thread_levels():
                scaling_factors[thread_level] = 1.0
        else:
            for thread_level in self.get_thread_levels():
                thread_level_x = int(thread_level)
                if thread_level_x < len(self.scaling_factors):
                    scaling_factors[thread_level] = self.scaling_factors[thread_level_x]
                else:
                    print('Warning: missing mips scaling factor for thread level %d. Using 1.0' % thread_level_x)
                    scaling_factors[thread_level] = 1.0
        return scaling_factors

    def get_scaling_factors_df(self):
        scaling_factors = self.get_scaling_factors()
        scaling_factors_df = pd.DataFrame(list(scaling_factors.items()),columns = ['thread_level','scaling_factor'])
        return scaling_factors_df

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

    def get_complete_scaled_mips_totals_report(self):
        if self.mips_report == None:
            return []
        scaling_factors = self.get_scaling_factors()
        mips_total = [0, 0, 0]
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=True):
                mr = self.get_mips_report(thread_level, process_id)
                if mr == None:
                    continue
                mr_mips = [mr.max_mips, mr.min_mips, mr.avg_mips]
                scaled_mips = [scaling_factors[thread_level] * x for x in mr_mips]
                mips_total = [sum(x) for x in zip(mips_total, scaled_mips)]
        return mips_total

    def get_complete_scaled_mips_totals_report_df(self):
        columns = ['thread_level', 'process_id', 'max_mips', 'min_mips', 'avg_mips']
        if self.mips_report == None:
            mips_total = []
        else:
            mips_total = [np.nan, np.nan] + self.get_complete_scaled_mips_totals_report()
        mips_total_df = pd.DataFrame([mips_total], columns = columns)
        return mips_total_df

    def get_complete_scaled_mips_report(self):
        if self.mips_report == None:
            return []
        scaling_factors = self.get_scaling_factors()
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
                scaled_mips = [scaling_factors[thread_level] * x for x in mr_mips]
                row.extend(scaled_mips)
                mips_report.append(row)
        return mips_report

    def get_complete_scaled_mips_report_df(self, multi_index=True):
        columns = ['thread_level', 'process_id', 'max_mips', 'min_mips', 'avg_mips']
        if self.mips_report == None:
            mips_report = []
        else:
            mips_report = self.get_complete_scaled_mips_report()
        mips_report_df = pd.DataFrame(mips_report, columns = columns)
        if multi_index:
            mips_report_df.set_index(['thread_level', 'process_id'], inplace=True)
        return mips_report_df

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

    def get_scaled_mips_trend_objects(self):
        if self.mips_report == None:
            return []

        scaling_factors = self.get_scaling_factors()
        mips_trend = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level, audio_only=False):
                process_name = self.getProcessName(thread_level, process_id)
                if process_name == None:
                    row = [thread_level, process_id]
                else:
                    row = [thread_level, process_name]
                for time_obj in self.get_time_objects(thread_level, process_id):
                    row.extend([time_obj.start_time, scaling_factors[thread_level] * time_obj.mips])
                mips_trend.append(row)
        return mips_trend

    def get_scaled_mips_trend_objects_df(self, multi_index=True):
        columns = ['thread_level', 'process_id']
        mips_trend = self.get_scaled_mips_trend_objects()

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
        for line in self.input_lines:
            line = line.strip()
            if (len(line) == 0):
                continue
            if line.startswith('Bg') or line.startswith('En'):
                lns = line.split()
                if len(lns) != 4:
                    continue
                msg = lns[0]
                thread_level = lns[1]
                process_id = lns[2]
                time_stamp = lns[3]
                start_stop = (msg == 'Bg')
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
            time_list.add_probe_points(self.probe_points)
            time_list.add_accel_probes(self.accel_process_list)
            time_list.reassign_process(debug=False)

        self.time_list = time_list
        self.run_mips_report()
        return
