#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import sys
import struct
import pandas as pd

is_py2 = sys.version[0] == '2'

if not is_py2:
    import codecs

def decode_hex(h):
    if is_py2:
        return h.decode('hex')
    else:
        return codecs.decode(h, 'hex')

def hex_to_int32(h):
    d = struct.unpack('>i', decode_hex(h))[0]
    return d

def hex_to_uint32(h):
    u = struct.unpack('>I', decode_hex(h))[0]
    return u

class TimeObject:
    def __init__(self, thread_level, process_id):  
        self.thread_level = thread_level
        self.process_id = process_id
        self.begin = None
        self.end = None
        self.elapsed = None
        self.beginU = None
        self.endU = None
        return
        
    def __str__(self):
        if (self.begin == None) or (self.end == None):
            return None
        mystr = 'thread_level %s process_id %s begin %s end %s elapsed %d' % (self.thread_level, self.process_id, self.begin, self.end, self.elapsed)
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
    def compute_elapsed(self):
        if (self.begin == None) or (self.end == None):
            return
        else:
            begin = hex_to_uint32(self.begin)
            end = hex_to_uint32(self.end)
            if end < begin:
                print ('Warning: wrap around detected ' + 'thread_level %s process_id %s begin %s end %s' % (self.thread_level, self.process_id, self.begin, self.end) )
                end = (1 << 32) + end
            self.elapsed = (end - begin)
            self.beginU = begin
            self.endU = end
            return

class MIPSObject:
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
    def __init__(self, frame_size=None, sample_rate=None, clock_rate=None):  
        self.cycles_to_mips_method = None
        
        if frame_size == None or sample_rate == None:
            if clock_rate == None:
                print ('Error: undefined MIPS mesurement process')
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
    
    def cycles_to_mips(self, cycles, frame_duration=None):
        if self.cycles_to_mips_method == None:
            return 0.0
            
        if self.cycles_to_mips_method == 'sample_rate_frame_size':
            #self.clock_rate = 1e9
            #r0 = (float(cycles)*self.sample_rate/self.frame_size)/(1000000)
            #r1 = (float(cycles)*self.clock_rate/frame_duration)/(1000000)
            #print ('Info: mips r0 %f r1 %f\n' % (r0, r1))
            return (float(cycles)*self.sample_rate/self.frame_size)/(1000000)
        elif self.cycles_to_mips_method == 'clock_rate':
            return (float(cycles)*self.clock_rate/frame_duration)/(1000000)
        else:
            print ('Error: undefined MIPS mesurement process')
            return 0.0
        
    def run(self, time_objects):  
        if self.cycles_to_mips_method == None:
            return None
            
        max_cycles = None
        min_cycles = None
        total_cycles = None
        count = 0
        for time_object in time_objects:
            elapsed = time_object.elapsed
            if count == 0:
                max_cycles = elapsed
                min_cycles = elapsed
                total_cycles = elapsed
            else:
                max_cycles = max(max_cycles, elapsed)
                min_cycles = min(min_cycles, elapsed)
                total_cycles = total_cycles + elapsed
            count = count + 1
                
        count = 0
        begin_current = 0
        for time_object in time_objects:
            begin_prev = begin_current
            begin_current = time_object.beginU
            if count == 0:
                begin_current = time_object.beginU
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
                
        avg_frame_cycles = float(total_frame_cycles)/(count-1)
        #print ('Info: frame_cycles max %d avg %d min %d' % (max_frame_cycles, avg_frame_cycles, min_frame_cycles))
        
        avg_cycles = float(total_cycles)/count
        max_mips = self.cycles_to_mips(max_cycles, avg_frame_cycles)
        min_mips = self.cycles_to_mips(min_cycles, avg_frame_cycles)
        avg_mips = self.cycles_to_mips(avg_cycles, avg_frame_cycles)
        
        return MIPSObject(max_cycles, min_cycles, avg_cycles, max_mips, min_mips, avg_mips)
        
class TimeSeries:
    def __init__(self):  
        self.time_series = {}
        return
        
    def add_time_object(self, thread_level, process_id, time_object):  
        if thread_level not in self.time_series:
            self.time_series[thread_level] = {}
            
        if process_id not in self.time_series[thread_level]:
            self.time_series[thread_level][process_id] = []
            
        self.time_series[thread_level][process_id].append(time_object)
        
    def get_time_objects(self, thread_level, process_id):  
        if thread_level not in self.time_series:
            return None
            
        if process_id not in self.time_series[thread_level]:
            return None
            
        return self.time_series[thread_level][process_id]
        
    def get_mips_report(self, mips_reporter, thread_level, process_id):
        if thread_level not in self.time_series:
            return None
            
        if process_id not in self.time_series[thread_level]:
            return None
        
        time_objects = self.get_time_objects(thread_level, process_id)
        return mips_reporter.run(time_objects)
        
class OJExecLog:
    def __init__(self, frame_size, sample_rate, clock_rate, input_lines):  
        self.frame_size = frame_size
        self.sample_rate = sample_rate
        self.clock_rate = clock_rate
        self.input_lines = input_lines
        self.time_series = TimeSeries()
        self.thread_levels = []
        self.process_ids = {}
        
        mips_reporter = MIPSReporter(frame_size, sample_rate, clock_rate)
        if mips_reporter.isValid():
            self.mips_reporter = mips_reporter
        else:
            self.mips_reporter = None
        
    def get_thread_levels(self):
        thread_levels = self.thread_levels
        thread_levels.sort()
        return thread_levels
    
    def get_process_ids(self, thread_level):
        if thread_level in self.process_ids:
            process_ids = self.process_ids[thread_level]
            process_ids.sort()
            return process_ids
        else:
            return []
    
    def get_mips_report(self, thread_level, process_id):
        if self.mips_reporter == None:
            return None
        return self.time_series.get_mips_report(self.mips_reporter, thread_level, process_id)
        
    def get_time_objects(self, thread_level, process_id):
        return self.time_series.get_time_objects(thread_level, process_id)
        
    def get_complete_mips_report(self):
        if self.mips_reporter == None:
            return []
        mips_report = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level):
                row = [thread_level, process_id]
                mr = self.get_mips_report(thread_level, process_id) 
                row.extend([mr.max_cycles, mr.min_cycles, mr.avg_cycles, mr.max_mips, mr.min_mips, mr.avg_mips])
                mips_report.append(row)
        return mips_report
        
    def get_complete_mips_report_df(self):
        columns = ['thread_level', 'process_id', 'max_cycles', 'min_cycles', 'avg_cycles', 'max_mips', 'min_mips', 'avg_mips']
        if self.mips_reporter == None:
            mips_report = []
        else:
            mips_report = self.get_complete_mips_report()
        mips_report_df = pd.DataFrame(mips_report, columns = columns)
        mips_report_df.set_index(['thread_level', 'process_id'], inplace=True)
        return mips_report_df
        
    def get_complete_time_objects(self):
        time_objects = []
        for thread_level in self.get_thread_levels():
            for process_id in self.get_process_ids(thread_level):
                row = [thread_level, process_id]
                for time_obj in self.get_time_objects(thread_level, process_id):
                    row.extend([time_obj.beginU, time_obj.elapsed, time_obj.endU])
                time_objects.append(row)
        return time_objects
        
    def get_complete_time_objects_df(self):
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
        time_objects_df.set_index(['thread_level', 'process_id'], inplace=True)
        return time_objects_df
        
    def run(self):  
        incomplete_time_objects = []
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
                
                if thread_level not in self.thread_levels:
                    self.thread_levels.append(thread_level)
                    self.process_ids[thread_level]=[]
                    
                if process_id not in self.process_ids[thread_level]:
                    self.process_ids[thread_level].append(process_id)
                    
                clock = lns[3]
                if msg == 'Bg':
                    time_object = TimeObject(thread_level, process_id)
                    time_object.begin = clock
                    incomplete_time_objects.append(time_object)
                if msg == 'En':
                    for time_object in incomplete_time_objects:
                        if (thread_level == time_object.thread_level) and (process_id == time_object.process_id):
                            time_object.end = clock
                            time_object.compute_elapsed()
                            self.time_series.add_time_object(thread_level, process_id, time_object)
                            incomplete_time_objects.remove(time_object)
                            break
                            
