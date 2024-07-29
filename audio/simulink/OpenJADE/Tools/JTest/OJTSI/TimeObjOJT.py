import pandas as pd
import numpy as np
import copy
import json

class TimeObj:
    PROBE_POINT_TID   = 0xFE
    ACCEL_PROCESS_TID = 0xFD

    MAX_ACCEL_TIDS              = 0x10
    FIR_ACCEL_PROCESS_START_TID = 0xD0
    FIR_ACCEL_PROCESS_STOP_TID  = FIR_ACCEL_PROCESS_START_TID + MAX_ACCEL_TIDS
    IIR_ACCEL_PROCESS_START_TID = FIR_ACCEL_PROCESS_STOP_TID
    IIR_ACCEL_PROCESS_STOP_TID  = IIR_ACCEL_PROCESS_START_TID + MAX_ACCEL_TIDS

    FIR_ACCEL_PROCESS_TIDS = list(range(FIR_ACCEL_PROCESS_START_TID, FIR_ACCEL_PROCESS_STOP_TID, 1))
    IIR_ACCEL_PROCESS_TIDS = list(range(IIR_ACCEL_PROCESS_START_TID, IIR_ACCEL_PROCESS_STOP_TID, 1))
    ACCEL_PROCESS_TIDS = [ACCEL_PROCESS_TID] + FIR_ACCEL_PROCESS_TIDS + IIR_ACCEL_PROCESS_TIDS

    AUDIO_PROCESS = 1
    PROBE_POINT = 2
    ACCEL_PROCESS = 3

    AUDIO_PROCESS_START_PID = 0x00
    AUDIO_PROCESS_STOP_PID  = 0x100
    PROBE_POINT_START_PID = 0x200
    PROBE_POINT_STOP_PID  = 0x300
    ACCEL_PROCESS_START_PID = 0x300
    ACCEL_PROCESS_STOP_PID = 0x400
    FIR_ACCEL_PROCESS_START_PID = 0x4000
    FIR_ACCEL_PROCESS_STOP_PID = 0x5000
    IIR_ACCEL_PROCESS_START_PID = 0x5000
    IIR_ACCEL_PROCESS_STOP_PID = 0x6000


    def __init__(self, start_stop, thread_level, process_id, time_stamp, time_data=None):
        self.start_stop = start_stop

        if isinstance(thread_level, str):
            self.thread_level = int(thread_level, 16)
        elif isinstance(thread_level, int):
            self.thread_level = thread_level
        else:
            self.thread_level = None

        if isinstance(process_id, str):
            self.process_id = int(process_id, 16)
        elif isinstance(process_id, int):
            self.process_id = process_id
        else:
            self.process_id = None

        if isinstance(time_stamp, str):
            self.time_stamp = int(time_stamp, 16)
        elif isinstance(time_stamp, int):
            self.time_stamp = time_stamp
        else:
            self.time_stamp = None

        self.time_data =  time_data

        self.time_pair = None

        self.is_probe_point = False
        self.is_accel_proc  = False
        self.is_audio_proc  = False

        if self.thread_level == TimeObj.PROBE_POINT_TID:
            self.type = TimeObj.PROBE_POINT
            self.process_id = self.process_id | TimeObj.PROBE_POINT_START_PID
            self.is_probe_point = True
        elif self.thread_level == TimeObj.ACCEL_PROCESS_TID:
            self.type = TimeObj.ACCEL_PROCESS
            self.process_id = self.process_id | TimeObj.ACCEL_PROCESS_START_PID
            self.is_accel_proc  = True
        elif self.thread_level in TimeObj.FIR_ACCEL_PROCESS_TIDS:
            self.type = TimeObj.ACCEL_PROCESS
            dev_id = self.thread_level - TimeObj.FIR_ACCEL_PROCESS_START_TID
            self.process_id = self.process_id + (dev_id << 8) + TimeObj.FIR_ACCEL_PROCESS_START_PID
            self.is_accel_proc  = True
        elif self.thread_level in TimeObj.IIR_ACCEL_PROCESS_TIDS:
            dev_id = self.thread_level - TimeObj.IIR_ACCEL_PROCESS_START_TID
            self.type = TimeObj.ACCEL_PROCESS
            self.process_id = self.process_id + (dev_id << 8) + TimeObj.IIR_ACCEL_PROCESS_START_PID
            self.is_accel_proc  = True
        else:
            self.type = TimeObj.AUDIO_PROCESS
            self.is_audio_proc  = True
        return

    def isStart(self):
        return (self.start_stop)

    def isStop(self):
        return (not self.start_stop)

    def isAudioProc(self):
        return (self.type == TimeObj.AUDIO_PROCESS)

    def __str__(self):
        if self.start_stop:
            start_stop = 'start'
        else:
            start_stop = 'stop'
        mystr = '(%d %d) time_stamp %d %s' % (self.thread_level, self.process_id, self.time_stamp, start_stop)
        return mystr

    def __repr__(self):
        return self.__str__()

    def __lt__(self, obj):
        return ((self.time_stamp) < (obj.time_stamp))

    def __gt__(self, obj):
        return ((self.time_stamp) > (obj.time_stamp))

    def __le__(self, obj):
        return ((self.time_stamp) <= (obj.time_stamp))

    def __ge__(self, obj):
        return ((self.time_stamp) >= (obj.time_stamp))

    def __eq__(self, obj):
        return (self.time_stamp == obj.time_stamp)

class AccelProcesses:
    def __init__(self, accel_json):
        self.accel_json = accel_json
        with open(accel_json) as fp:
            accel_processes = json.load(fp)

        for tid, processes in accel_processes.items():
            for pid, process in processes.items():
                thread_id = int(process['thread_id'],16)
                if thread_id in TimeObj.FIR_ACCEL_PROCESS_TIDS:
                    process['thread_name'] = 'FIR%02X' % (thread_id - TimeObj.FIR_ACCEL_PROCESS_START_TID)
                elif thread_id in TimeObj.IIR_ACCEL_PROCESS_TIDS:
                    process['thread_name'] = 'IIR%02X' % (thread_id - TimeObj.IIR_ACCEL_PROCESS_START_TID)
                symbol_name = process['symbol_name']
                if '_codeWorkMemory.' in symbol_name:
                    symbol_name = symbol_name.replace('_codeWorkMemory.','')
                elif '_codeWorkMemory' in symbol_name:
                    symbol_name = symbol_name.replace('_codeWorkMemory','')
                process['process_name'] = process['thread_name'] + '_' + symbol_name
        self.accel_processes = accel_processes

    def get_processes(self, thread_id):
        tid = '%02X' % thread_id
        if tid in self.accel_processes:
            processes = {}
            for pid, process in self.accel_processes[tid].items():
                process_id = int(pid, 16)
                processes[process_id] = process
            return processes
        else:
            return None

class TimePair:
    def __init__(self, start_idx, stop_idx, time_list, debug=False):
        if stop_idx <= start_idx:
            self.valid = False
            return

        self.mips = None
        self.valid = True
        self.start_idx = start_idx
        self.stop_idx = stop_idx
        self.start_obj = time_list[start_idx]
        self.stop_obj = time_list[stop_idx]
        self.start_time = time_list[start_idx].time_stamp
        self.stop_time = time_list[stop_idx].time_stamp

        self.time_list = time_list
        self.time_stamp = time_list[stop_idx].time_stamp - time_list[start_idx].time_stamp

        self.is_probe_point = time_list[start_idx].is_probe_point
        self.is_accel_proc  = time_list[start_idx].is_accel_proc
        self.is_audio_proc  = time_list[start_idx].is_audio_proc

        # Accelerator processing happens in background. It cannot have any preemptors
        if self.is_accel_proc:
            return

        self.preemptor_list = []
        preemptor_start = None
        preemptor_stop = None
        for idx in range(self.start_idx+1, self.stop_idx):
            time_obj = time_list[idx]
            if self.is_audio_proc:
                # For audio processing
                # accelerator processing happens in background. It cannot be a preemptor
                # processing captured by probe_points need to be included as well
                # It can only be preempted by other audio processing
                if not time_obj.is_audio_proc:
                    continue
            elif self.is_probe_point:
                # For probe points
                # accelerator processing happens in background. It cannot be a preemptor.
                # Inner probe points are considered included in Outer probe points pair
                # So it can only be preempted by audio processing
                if not time_obj.is_audio_proc:
                    continue

            if preemptor_start == None:
                if time_obj.isStart():
                    preemptor_start = idx
            else:
                if (time_list[preemptor_start].thread_level == time_obj.thread_level ) and \
                (time_list[preemptor_start].process_id == time_obj.process_id ) and \
                time_obj.isStop():
                    preemptor_stop = idx
                    preemptor = { 'start_idx': preemptor_start, 'stop_idx': preemptor_stop }
                    self.preemptor_list.append(preemptor)
                    preemptor_start = None
                    preemptor_stop = None

        for preemptor in self.preemptor_list:
            preemptor_start = preemptor['start_idx']
            preemptor_stop = preemptor['stop_idx']

            self.time_stamp = self.time_stamp - ( time_list[preemptor_stop].time_stamp \
                                - time_list[preemptor_start].time_stamp )

            if debug:
                print ('\t\t\tpreemptor: %d %d' % (preemptor_start, preemptor_stop))
                print ('\t\t\ttime_stamp: %d' % (self.time_stamp))
                print ('\t\t\t\tpreemptor_start: %s' % (str(time_list[preemptor_start])))
                print ('\t\t\t\tpreemptor_stop: %s' % (str(time_list[preemptor_stop])))
                print ('\t\t\t\tpreemptor_time_stamp: %d' % (time_list[preemptor_stop].time_stamp - time_list[preemptor_start].time_stamp))

            if not self.is_subset(preemptor, time_list):
                print ('Error: premptor (%s, %s) not contained in self (%s, %s)' % \
                       (str(time_list[preemptor_start]), \
                        str(time_list[preemptor_stop]),  \
                        str(time_list[self.start_idx]),      \
                        str(time_list[self.stop_idx])))
                self.valid = False
                return
        if self.time_stamp < 0:
            print ('Error: negative time_stamp encountered')
            self.valid = False
            return


    def is_subset(self, preemptor, time_list):
        preemptor_start = preemptor['start_idx']
        preemptor_stop = preemptor['stop_idx']

        return ( (time_list[preemptor_start].time_stamp > time_list[self.start_idx].time_stamp ) and \
                 (time_list[preemptor_stop].time_stamp  < time_list[self.stop_idx].time_stamp ) )

class ProcessObj:
    def __init__(self, thread_level, process_id):
        if isinstance(thread_level, str):
            self.thread_level = int(thread_level, 16)
        elif isinstance(thread_level, int):
            self.thread_level = thread_level
        else:
            self.thread_level = None

        if isinstance(process_id, str):
            self.process_id = int(process_id, 16)
        elif isinstance(process_id, int):
            self.process_id = process_id
        else:
            self.process_id = None

        self.name = None
        self.time_list = None
        self.time_pairs = None
        self.inferred_thread_level =  None

        self.is_probe_point = False
        self.is_accel_proc  = False
        self.is_audio_proc  = False

        if self.process_id in range(TimeObj.PROBE_POINT_START_PID, TimeObj.PROBE_POINT_STOP_PID):
            self.is_probe_point = True
        elif self.process_id in range(TimeObj.ACCEL_PROCESS_START_PID, TimeObj.ACCEL_PROCESS_STOP_PID):
            self.is_accel_proc  = True
        elif self.process_id in range(TimeObj.FIR_ACCEL_PROCESS_START_PID, TimeObj.FIR_ACCEL_PROCESS_STOP_PID):
            self.is_accel_proc  = True
        elif self.process_id in range(TimeObj.IIR_ACCEL_PROCESS_START_PID, TimeObj.IIR_ACCEL_PROCESS_STOP_PID):
            self.is_accel_proc  = True
        else:
            self.is_audio_proc  = True

    def get_time_pairs(self):
        return self.time_pairs

    def reassign_time_pairs(self, thread_level, debug=False):
        for time_pair in self.time_pairs:
            time_pair.start_obj.thread_level = thread_level
            time_pair.stop_obj.thread_level = thread_level

    def process_accel_procs(self, time_list, debug=False):
        self.time_list = time_list
        self.time_pairs = []
        start_idx = None
        stop_idx = None

        for idx, time_obj in enumerate(time_list):
            if (time_obj.is_accel_proc) and (self.process_id == time_obj.process_id):
                if start_idx == None:
                    if time_obj.isStart():
                        start_idx = idx
                else:
                    if time_obj.isStart():
                        start_idx = idx
                    elif time_obj.isStop():
                        stop_idx = idx
                        if debug:
                            print ('\tpair: %d %d' % (start_idx, stop_idx))
                            print ('\t\tstart: %s' % (str(time_list[start_idx])))
                            print ('\t\tstop: %s' % (str(time_list[stop_idx])))
                            print ('\t\ttime_stamp: %d' % (time_list[stop_idx].time_stamp - time_list[start_idx].time_stamp))

                        time_pair = TimePair(start_idx, stop_idx, time_list, debug=debug)
                        if time_pair.valid:
                            self.time_pairs.append(time_pair)
                            time_list[start_idx].time_pair = time_pair
                            time_list[stop_idx].time_pair = time_pair
                        start_idx = None

    def process_probe_points(self, time_list, audio_processes, debug=False):
        self.time_list = time_list
        self.time_pairs = []
        start_idx = None
        stop_idx = None

        audio_pairs = copy.deepcopy( audio_processes )
        inferred_thread_levels = {k:0 for k in audio_pairs.keys()}

        for idx, time_obj in enumerate(time_list):
            if (time_obj.is_audio_proc):
                if (time_obj.isStart()):
                    audio_pairs[time_obj.thread_level][time_obj.process_id] = time_obj.time_pair
                continue

            if (time_obj.is_probe_point) and (self.process_id == time_obj.process_id):
                if start_idx == None:
                    if time_obj.isStart():
                        start_idx = idx
                else:
                    if time_obj.isStop():
                        stop_idx = idx
                        if debug:
                            print ('\tpair: %d %d' % (start_idx, stop_idx))
                            print ('\t\tstart: %s' % (str(time_list[start_idx])))
                            print ('\t\tstop: %s' % (str(time_list[stop_idx])))
                            print ('\t\ttime_stamp: %d' % (time_list[stop_idx].time_stamp - time_list[start_idx].time_stamp))

                        time_pair = TimePair(start_idx, stop_idx, time_list, debug=debug)
                        if time_pair.valid:
                            self.time_pairs.append(time_pair)
                            time_list[start_idx].time_pair = time_pair
                            time_list[stop_idx].time_pair = time_pair
                            # Infer thread level
                            for thread_level, audio_process in audio_pairs.items():
                                for process_id, audio_pair in audio_process.items():
                                    if audio_pair == None:
                                        continue
                                    if ( (time_pair.start_time > audio_pair.start_time) and
                                         (time_pair.stop_time  < audio_pair.stop_time ) ):
                                        inferred_thread_levels[thread_level] =  inferred_thread_levels[thread_level] + 1
                        start_idx = None

        cnt = 0
        for thread_level, val in inferred_thread_levels.items():
            if val > 0:
                inferred_thread_level = thread_level
                cnt = cnt + 1
        if cnt == 0:
            print ('Error: thread level could not be inferred for Probe point process 0x%02X' %  (self.process_id))
            self.inferred_thread_level = TimeObj.PROBE_POINT_TID
        elif cnt == 1:
            print ('Info: inferred thread level 0x%02X for Probe point process 0x%02X' %  (inferred_thread_level, self.process_id & 0xFF))
            self.inferred_thread_level =  inferred_thread_level
        elif cnt > 1:
            print ('Error: multiple thread level inferred for Probe point process 0x%02X' %  (self.process_id & 0xFF))
            for thread_level, val in inferred_thread_levels.items():
                if val > 0:
                    print('\t: inferred thread_level 0x%02X count %d' % (thread_level, val))
            self.inferred_thread_level =  TimeObj.PROBE_POINT_TID

    def process_time_list(self, time_list, debug=False):
        self.time_list = time_list
        self.time_pairs = []
        start_idx = None
        stop_idx = None
        for idx, time_obj in enumerate(time_list):
            if (self.thread_level == time_obj.thread_level) and (self.process_id == time_obj.process_id):
                if start_idx == None:
                    if time_obj.isStart():
                        start_idx = idx
                else:
                    if time_obj.isStop():
                        stop_idx = idx
                        if debug:
                            print ('\tpair: %d %d' % (start_idx, stop_idx))
                            print ('\t\tstart: %s' % (str(time_list[start_idx])))
                            print ('\t\tstop: %s' % (str(time_list[stop_idx])))
                            print ('\t\ttime_stamp: %d' % (time_list[stop_idx].time_stamp - time_list[start_idx].time_stamp))

                        time_pair = TimePair(start_idx, stop_idx, time_list, debug=debug)
                        if time_pair.valid:
                            self.time_pairs.append(time_pair)
                            time_list[start_idx].time_pair = time_pair
                            time_list[stop_idx].time_pair = time_pair
                        start_idx = None

    def getProcessName(self):
        if self.name == None:
            if self.is_probe_point:
                name = 'probe%02X_p%02X' % (self.thread_level, (self.process_id-TimeObj.PROBE_POINT_START_PID))
            elif self.is_accel_proc:
                name = 'accel%02X_p%02X' % (self.thread_level, (self.process_id-TimeObj.ACCEL_PROCESS_START_PID))
            else:
                name = 't%02X_p%02X' % (self.thread_level, self.process_id)
        else:
            name = self.name
        return name

    def setProcessName(self, name):
        self.name = name

    def __str__(self):
        mystr = 'process_id %d ' % (self.process_id)
        return mystr

    def __repr__(self):
        return self.__str__()

    def __lt__(self, obj):
        return ((self.process_id) < (obj.process_id))

    def __gt__(self, obj):
        return ((self.process_id) > (obj.process_id))

    def __le__(self, obj):
        return ((self.process_id) <= (obj.process_id))

    def __ge__(self, obj):
        return ((self.process_id) >= (obj.process_id))

    def __eq__(self, obj):
        return (self.process_id == obj.process_id)

class ThreadObj:
    def __init__(self, thread_level):
        if isinstance(thread_level, str):
            self.thread_level = int(thread_level, 16)
        elif isinstance(thread_level, int):
            self.thread_level = thread_level
        else:
            self.thread_level = None

        self.process_list = {}
        self.callrate = None
        self.base_task = None
        self.rate_period = None

    def add_callrate(self, callrate):
        self.callrate = callrate['callrate']
        self.rate_period = callrate['rate_period']
        self.base_task = callrate['base_task']

    def add_process(self, process_id):
        self.process_list[process_id] = ProcessObj(self.thread_level, process_id)

    def get_process_ids(self, audio_only=True):
        process_ids = sorted(self.process_list.keys())
        if audio_only:
            process_ids = [k for k in process_ids if k in range(TimeObj.AUDIO_PROCESS_START_PID, TimeObj.AUDIO_PROCESS_STOP_PID)]
        return process_ids

    def get_process_obj(self, process_id):
        if process_id in self.process_list:
            return self.process_list[process_id]
        else:
            return None

    def get_time_pairs(self, process_id):
        if process_id in self.process_list:
            return self.process_list[process_id].get_time_pairs()
        else:
            return None

    def reassign_process(self, process_id, process_obj):
        self.process_list[process_id] = process_obj
        process_obj.reassign_time_pairs(self.thread_level)

    def getProcessName(self, process_id):
        if process_id in self.process_list:
            return self.process_list[process_id].getProcessName()
        else:
            return None

    def setProcessName(self, process_id, name):
        if process_id in self.process_list:
            self.process_list[process_id].setProcessName(name)

    def process_time_list(self, time_list, debug=False):
        for process_id, process_obj in self.process_list.items():
            process_obj.process_time_list(time_list, debug=debug)

    def process_probe_points(self, time_list, audio_threads, debug=False):
        for process_id, process_obj in self.process_list.items():
            process_obj.process_probe_points(time_list, audio_threads, debug=debug)

    def process_accel_procs(self, time_list, debug=False):
        for process_id, process_obj in self.process_list.items():
            process_obj.process_accel_procs(time_list, debug=debug)

    def __str__(self):
        mystr = 'thread_level %d ' % (self.thread_level)
        return mystr

    def __repr__(self):
        return self.__str__()

    def __lt__(self, obj):
        return ((self.thread_level) < (obj.thread_level))

    def __gt__(self, obj):
        return ((self.thread_level) > (obj.thread_level))

    def __le__(self, obj):
        return ((self.thread_level) <= (obj.thread_level))

    def __ge__(self, obj):
        return ((self.thread_level) >= (obj.thread_level))

    def __eq__(self, obj):
        return (self.thread_level == obj.thread_level)

class TimeList:
    def __init__(self, debug=False):
        self.debug = debug
        self._data = []

        self.process_names = []
        self.callrates_list = None
        self.process_list = None
        self.probe_points = None
        self.accel_process_list = None
        self.base_thread = None

        self.thread_objs = {}

    def to_csv(self, csv_file):
        d = {
             'start_stop' : ['start' if x.start_stop else 'stop' for x in self._data],
             'thread_level' : [x.thread_level for x in self._data],
             'process_id' : [x.process_id for x in self._data],
             'time_stamp' : [x.time_stamp for x in self._data] }
        df = pd.DataFrame(data=d)
        df.to_csv(csv_file)

    def add(self, start_stop, thread_level, process_id, time_stamp, time_data):
        time_obj = TimeObj(start_stop, thread_level, process_id, time_stamp, time_data)
        self._data.append(time_obj)

    def __getitem__(self, index):
        return self._data[index]

    def add_callrates_list(self, callrates_list=None):
        self.callrates_list = callrates_list
        base_thread = None
        for thread_level, callrate in self.callrates_list.items():
            if callrate['base_task'] == True:
                base_thread = thread_level
            if thread_level in self.thread_objs:
                self.thread_objs[thread_level].add_callrate(callrate)
        if base_thread != None:
            self.base_thread = base_thread

    def add_process_list(self, process_list=None):
        self.process_list = process_list
        for process_name, process_info in self.process_list.items():
            if process_name in self.process_names:
                print(f'Warning: duplicate process name {process_name} detected')
            else:
                self.process_names.append(process_name)
            thread_level = process_info['thread_level']
            process_id = process_info['process_id']
            if thread_level in self.thread_objs:
                self.thread_objs[thread_level].setProcessName(process_id, process_name)

    def add_probe_points(self, probe_points=None):
        self.probe_points = probe_points
        thread_level = TimeObj.PROBE_POINT_TID

        if thread_level in self.thread_objs:
            for process_name, process_info in self.probe_points['processes'].items():
                if process_name in self.process_names:
                    print(f'Warning: duplicate process name {process_name} detected')
                else:
                    self.process_names.append(process_name)
                process_id = process_info['process_id'] + TimeObj.PROBE_POINT_START_PID
                self.thread_objs[thread_level].setProcessName(process_id, process_name)

    def add_accel_probes(self, accel_process_list=None):
        self.accel_process_list = accel_process_list
        if isinstance(accel_process_list, AccelProcesses):
            for thread_level in TimeObj.FIR_ACCEL_PROCESS_TIDS:
                fir_processes = accel_process_list.get_processes(thread_level)
                if fir_processes != None:
                    for pid, fir_process in fir_processes.items():
                        dev_id = thread_level - TimeObj.FIR_ACCEL_PROCESS_START_TID
                        process_id = pid + (dev_id << 8) + TimeObj.FIR_ACCEL_PROCESS_START_PID
                        process_name =  fir_process['process_name']
                        if process_name in self.process_names:
                            print(f'Warning: duplicate process name {process_name} detected')
                        else:
                            self.process_names.append(process_name)
                        self.thread_objs[thread_level].setProcessName(process_id, process_name)

            for thread_level in TimeObj.IIR_ACCEL_PROCESS_TIDS:
                iir_processes = accel_process_list.get_processes(thread_level)
                if iir_processes != None:
                    for pid, iir_process in iir_processes.items():
                        dev_id = thread_level - TimeObj.IIR_ACCEL_PROCESS_START_TID
                        process_id = pid + (dev_id << 8) + TimeObj.IIR_ACCEL_PROCESS_START_PID
                        process_name =  iir_process['process_name']
                        if process_name in self.process_names:
                            print(f'Warning: duplicate process name {process_name} detected')
                        else:
                            self.process_names.append(process_name)
                        self.thread_objs[thread_level].setProcessName(process_id, process_name)
        else:
            thread_level = TimeObj.ACCEL_PROCESS_TID
            if thread_level in self.thread_objs:
                for k, v in self.accel_process_list.items():
                    for idx, entry in enumerate(v):
                        process_id = entry['process_id'] + TimeObj.ACCEL_PROCESS_START_PID
                        process_name = '{}_accel{}'.format(k, idx)
                        self.thread_objs[thread_level].setProcessName(process_id, process_name)

    def unwrap(self, threshold=(1<<30), increment=(1<<32)):
        if len(self._data) <= 0:
            return

        tprev = self._data[0].time_stamp
        wrap = 0
        for idx, d in enumerate(self._data):
            tcurr = d.time_stamp
            if tcurr < tprev:
                tdiff = tprev - tcurr
                if tdiff > threshold:
                    wrap = wrap + 1
                    print ('Info: timestamp wrapping number %d detected at %d' % (wrap, idx))
            if wrap > 0:
                d.time_stamp = tcurr + wrap * increment
            tprev = tcurr

    def subtract_initial(self):
        if len(self._data) > 0:
            initial_timestamp = self._data[0].time_stamp
        for d in self._data:
            d.time_stamp = d.time_stamp - initial_timestamp

    def count(self):
        return len(self._data)

    def sort(self):
        self._data.sort()

    def process_time_list(self, debug=False):
        for d in self._data:
            thread_level = d.thread_level
            process_id = d.process_id

            if thread_level not in self.thread_objs:
                self.thread_objs[thread_level] = ThreadObj(thread_level)

            if process_id not in self.thread_objs[thread_level].process_list:
                self.thread_objs[thread_level].add_process(process_id)

        for thread_level, thread_obj in self.thread_objs.items():
            if thread_level in [TimeObj.PROBE_POINT_TID] + TimeObj.ACCEL_PROCESS_TIDS:
                continue
            thread_obj.process_time_list(self._data, debug=debug)

    def reassign_process(self, debug=False):
        if TimeObj.PROBE_POINT_TID in self.thread_objs:
            probe_point_thread = self.thread_objs[TimeObj.PROBE_POINT_TID]
            probe_point_thread.process_probe_points(self._data, self.get_audio_processes(), debug=debug)
            # reassign probe point processes
            for process_id, process_obj in probe_point_thread.process_list.items():
                self.thread_objs[process_obj.inferred_thread_level].reassign_process(process_id, process_obj)

        # reassign fir and iir accel processes to base_thread
        for thread_level in TimeObj.FIR_ACCEL_PROCESS_TIDS:
            if thread_level in self.thread_objs:
                accel_proc_thread = self.thread_objs[thread_level]
                accel_proc_thread.process_accel_procs(self._data, debug=debug)
                for process_id, process_obj in accel_proc_thread.process_list.items():
                    self.thread_objs[self.base_thread].reassign_process(process_id, process_obj)

        for thread_level in TimeObj.IIR_ACCEL_PROCESS_TIDS:
            if thread_level in self.thread_objs:
                accel_proc_thread = self.thread_objs[thread_level]
                accel_proc_thread.process_accel_procs(self._data, debug=debug)
                for process_id, process_obj in accel_proc_thread.process_list.items():
                    self.thread_objs[self.base_thread].reassign_process(process_id, process_obj)

        if TimeObj.ACCEL_PROCESS_TID in self.thread_objs:
            accel_proc_thread = self.thread_objs[TimeObj.ACCEL_PROCESS_TID]
            accel_proc_thread.process_accel_procs(self._data, debug=debug)

            # reassign accelerator processes to user specified thread_level or base_thread
            for process_id, process_obj in accel_proc_thread.process_list.items():
                _thread_level = None
                for k, v in self.accel_process_list.items():
                    for idx, entry in enumerate(v):
                        if process_id == (entry['process_id'] | TimeObj.ACCEL_PROCESS_START_PID):
                            if '_thread_level' in entry:
                                _thread_level = entry['_thread_level']
                                break
                    if _thread_level != None:
                        break
                if _thread_level == None:
                    _thread_level = self.base_thread
                self.thread_objs[_thread_level].reassign_process(process_id, process_obj)

    def get_thread_levels(self, audio_only=True):
        thread_levels = sorted(self.thread_objs.keys())
        if audio_only:
            thread_levels = [k for k in thread_levels if k not in [TimeObj.PROBE_POINT_TID] + TimeObj.ACCEL_PROCESS_TIDS]
        return thread_levels

    def get_process_ids(self, thread_level, audio_only=True):
        if thread_level in self.thread_objs:
            return self.thread_objs[thread_level].get_process_ids(audio_only=audio_only)
        else:
            return None

    def get_audio_processes(self):
        audio_processes = {}
        for thread_level in self.get_thread_levels(audio_only=True):
            audio_processes[thread_level] = {}
            for process_id in self.get_process_ids(thread_level, audio_only=True):
                audio_processes[thread_level][process_id] = None
        return audio_processes

    def get_time_pairs(self, thread_level, process_id):
        if thread_level in self.thread_objs:
            return self.thread_objs[thread_level].get_time_pairs(process_id)
        else:
            return None

    def getProcessName(self, thread_level, process_id):
        if thread_level in self.thread_objs:
            return self.thread_objs[thread_level].getProcessName(process_id)
        else:
            return None

