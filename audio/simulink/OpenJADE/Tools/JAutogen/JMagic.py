import os
import sys
import shutil
import inspect


from JAConv import *
from JASchema import *
from JALogPrint import *




PROFILING_RUNTIME = 1
PROFILING_JPROBE = 2
PROFILE_C = 'OJ_profile.c'
PROFILE_H = 'OJ_profile.h'



class bufDetail:
    def __init__(self, name, length, comment):
        self.name = name
        self.length = length
        self.comment = comment



class JMagic:
    pass



class JProbeMagic(JMagic):
    HIERARCHY_SIGN = 'Here is the system hierarchy for this model'
    BLOCKSIGNAL_SIGN = 'Block signals (default storage)'
    BLOCKSTATE_SIGN = 'Block states (default storage)'
    STRUCTURE_SIGN = 'typedef struct'
    EXTERN_SIGN = 'extern '
    THRESHOLD = 128
    def __init__(self, matlab_model_name=None, matlab_model_path=None):
        self.hfile = None
        self.cfile = None
        self.model_num = None
        self.modelpath = matlab_model_path
        self.functobecalled = []
        # Format {model_path : model_number}
        self.mm = {}
        # Format {extern struct_name : struct_variable}
        self.sv = {}
        self.schema = JASchema()
        self.conv = JMatlabConv()
        self.signal_stru = self.conv.structname_blocksignal(matlab_model_name)
        self.state_stru = self.conv.structname_blockstate(matlab_model_name)
        self.signalname = self.conv.variablename_blocksignal(matlab_model_name)
        self.statename = self.conv.variablename_blockstate(matlab_model_name)

    def SetModelPath(self, matlab_model_path=None):
        self.modelpath = matlab_model_path

    def BasicProbeforBuffer(self):
        bsignalList = []
        bstateList = []
        sig_start = sig_end = False
        sta_start = sta_end = False
        sig_tmp = False
        sta_tmp = False
        # Read Header files and collect all Buffer information from B_Model_1_1_T and DW_Model_1_1_T
        with open(self.hfile, 'r') as filehandle:
            num_of_lines = filehandle.readlines()
            # Collect all Block Signal Buffers
            scnt = ecnt = 0
            for i in range(len(num_of_lines)):
                if JProbeMagic.BLOCKSIGNAL_SIGN in num_of_lines[i] and JProbeMagic.STRUCTURE_SIGN in num_of_lines[i + 1]:
                    scnt = i
                    ecnt = 2
                    while not (self.signal_stru in num_of_lines[i + ecnt]):
                        bufD = self.findinBufferArray(num_of_lines[i + ecnt])
                        if bufD != None and int(bufD.length) > JProbeMagic.THRESHOLD:
                            bsignalList.append(bufD)
                        ecnt += 1
                    ecnt += 1
            # Collect All Block State Buffers
            state_line = num_of_lines[scnt + ecnt :]
            for i in range(len(state_line)):
                if JProbeMagic.BLOCKSTATE_SIGN in state_line[i] and JProbeMagic.STRUCTURE_SIGN in state_line[i + 1]:
                    scnt = i
                    ecnt = 2
                    while not (self.state_stru in state_line[i + ecnt]):
                        bufD = self.findinBufferArray(state_line[i + ecnt])
                        if bufD != None and int(bufD.length) > JProbeMagic.THRESHOLD:
                            bstateList.append(bufD)
                        ecnt += 1
        # Add functions to Model_1_1.c file
        if len(bsignalList) > 0 or len(bstateList) > 0:
            with open(self.cfile, 'a') as filehandle:
                for item in bsignalList:
                    filehandle.write(self.schema.AddExportSignalBufferFunc(self.signalname, item.name, item.length, item.comment))
                for item in bstateList:
                    filehandle.write(self.schema.AddExportStateBufferFunc(self.statename, item.name, item.length, item.comment))
        else:
            JAlog_print(JAG_ERROR, 'Cannot find any buffers....')

    def CollectAllMatlabClues(self):
        # Travese all model path and extern global variable
        with open(self.hfile, 'r') as filehandle:
            all_lines = filehandle.readlines()
            for i in range(len(all_lines)):
                if JProbeMagic.EXTERN_SIGN in all_lines[i]:
                    # Check it is not a pointer, not a constant, not a function
                    if (not '*' in all_lines[i]) and (not 'const' in all_lines[i]) and (not '(' in all_lines[i]) and (not '/*' in all_lines[i]):
                        l = all_lines[i].find(' ')
                        r = all_lines[i].find(' ', l + 1)
                        e = all_lines[i].find(';', r + 1)
                        if e > 0:
                            k = all_lines[i][l + 1 : r]
                            v = all_lines[i][r + 1 : e]
                        else:
                            # in the next line
                            k = all_lines[i][l + 1 : r] if r > l else all_lines[i][l + 1 : -1]
                            e = all_lines[i + 1].find(';')
                            r = all_lines[i + 1].rfind(' ')
                            v = all_lines[i + 1][r + 1 : e]
                        self.sv[k] = v
                if JProbeMagic.HIERARCHY_SIGN in all_lines[i]:
                    break
            for ll in all_lines[i:]:
                l = ll.find('<')
                r = ll.find('>')
                if l > 0 and r > 0:
                    vl = ll[l : r + 1]
                    mid = ll.find(':')
                    if mid > 0:
                        l = ll[mid:].find("'")
                        r = ll[mid:].find("'", l + 1)
                        if l > 0 and r > 0:
                            key = ll[mid + l + 1 : mid + r]
                            self.mm[key] = vl

    def CheckExistOfModelNum(self):
        fflag = False
        if self.modelpath in self.mm.keys():
            self.model_num = self.mm[self.modelpath]
            fflag = True
        else:
            JAlog_print(JAG_ERROR, 'Model Path cannot be found....')
        return fflag

    def AdvancedProbeforBuffer(self, func_name_list=None, modelbufinfo=None):
        flag = False
        ss = []
        # Format: {struct_name : [variable_name1, array_size1, comment1, variable_name2, array_size2, comment2, ...]}
        strDict = {}
        with open(self.hfile, 'r') as filehandle:
            all_lines = filehandle.readlines()
            start = False
            for off, ll in enumerate(all_lines):
                if JProbeMagic.STRUCTURE_SIGN in ll:
                    start = True
                    # Format: [variable_name1, array_size1, comment1, variable_name2, array_size2, comment2, ...]
                    varList = []
                # Search model number inside of structure
                if start == True:
                    if (self.model_num in ll) and ('[' in ll) and (';' in ll):
                        l = ll.find('[')
                        r = ll.find(']')
                        s = ll[:l].rfind(' ')
                        c = ll.find(self.model_num)
                        e = ll.find("'", c + 1)
                        varList.append(ll[s + 1 : l])
                        varList.append(ll[l + 1 : r])
                        varList.append(ll[c : e])
                        flag = True
                if '}' in ll and start == True:
                    start = False
                    if varList:
                        structname = all_lines[off + 1][:-2]
                        strDict[structname] = varList
        #print('>>>>>' + str(strDict))
        # Write functions to .c file
        if flag == True:
            #with open(platformpath, 'a') as filehandle:
            for k, v in strDict.items():
                if k in self.sv:
                    for i in range(0, len(v), 3):
                        modelbufinfo.append(self.schema.ReturnSpecifiedBufferVar(self.sv[k], v[i]))
                        modelbufinfo.append(v[i + 1])
                        func_name_list.append(self.schema.ReturnExportSpecifiedBufferFunc(v[i], v[i + 1]))
                        ss.append(self.schema.AddExportSpecifiedBufferFunc(self.sv[k], v[i], v[i + 1], v[i + 2]))
        return ss

    def findinBufferArray(self, ss=None):
        loc_l = ss.find('[')
        loc_r = ss.find(']')
        if loc_r > loc_l:
            bufSize = ss[loc_l + 1 : loc_r]
            loc_space = ss[:loc_l].rfind(' ') if loc_l > 0 else -1
            bufName = ss[loc_space + 1 : loc_l] if loc_space > 0 else None
            loc_l = ss.find('<')
            loc_r = ss.find('>')
            bufComment = ss[loc_l : loc_r + 1] if loc_r > loc_l else None
            return bufDetail(bufName, bufSize, bufComment)
        else:
            return None


##############################################################################
# class Name:  JProfileMagic
# Description: It adds start/stop function for MIPS profiling, and generates
#              OJ_profile.c/.h for each Model.
##############################################################################
class JProfileMagic(JMagic):
    MIPS_ENABLE = 1
    CYCLE_ENABLE = 2
    PROFILING_START = 1
    PROFILING_STOP = 2
    PROFILING_STRUSIZE = 36
    MAX_TOP = 14
    PROBE_STARTID = 160
    ASDID_HEADER = 'ASDID.h'
    TOP_SUFFIX = '_TOP_Access.c'
    PROFILE_TOP_EMPTY = '''    {
    },\n'''
    PROFILE_TOP_TEMPLATE = '''    {
        1, Profile_BankTable
    },\n'''
    PROFILE_TOP_STRUCTURE = '''extern ModelXXX_profiledata[];
static TOP_BankTable_type Profile_BankTable[] =
{
    {
        PROFILINGSIZEXXX, (void*)ModelXXX_profiledata
    },
};\n\n'''
    TOP_DIRECTADDRESS_KEYWORD = '#define ASDID_CORE(id)'
    TOP_DIRECTADDRESS_KEYWORD_BAK = 'TOP_GroupTable_type top_group_table'
    ##########################################################################
    # function name: __init__
    # Description:   Constructor of JProfileMagic.
    # Argument:      inlineEnable:  Enable/Disable inline for OJ_profilestart
    #                               and OJ_profilestop
    #                cycleOrmips:   Profiling in Cycle or MIPS (1 or 2)
    #                threhold:      threhold based on average
    #                sst:           in seconds, start statistical counter after
    #                               N seconds of profiling. Disable statistical
    # Return:        None
    ##########################################################################
    def __init__(self, inlineEnable=True, cycleOrmips=MIPS_ENABLE, threhold=0.1, sst=5):
        self.inlineEnable = inlineEnable
        self.cycleOrmips = cycleOrmips
        self.threhold = threhold
        self.probeCounter = JProfileMagic.PROBE_STARTID
        self.sst = sst
        self.pstepDict = {}
        self.pfeaDict = {}
        self.ppltDict = {}
        self.funcProcIdDict = {}
        self.initbline = 0
        self.initeline = 0
        self.schema = JASchema()
    ##########################################################################
    # function name: AddProfilingInitStepFunc
    # Description:   Add MIPS profiling Init function to the model_initialize
    #                function, add profiling for step functions.
    # Argument:      filepath:            file path to the C code of Model
    #                matlab_model_name:   Model name, like Model_1_1
    #                profile_type:        Two types of profiling. Runtime or
    #                                     JProbe
    # Return:        None
    ##########################################################################
    def AddProfilingInitStepFunc(self, filepath=None, matlab_model_name=None, profile_type=PROFILING_RUNTIME):
        # For Run-time profiling, add profiling for each step functions.
        vl = False
        mpath = os.path.join(filepath, matlab_model_name + '.c')
        include_one = True
        if bool(self.pstepDict):
            sortedList = [[k, v] for k, v in sorted(self.pstepDict.items(), key=lambda item: item[1])]
            off = 0
        # Add profiling Init function to model_name.c
        with open(mpath, 'r') as f:
            num_of_lines = f.readlines()
        os.remove(mpath)
        # Write new model_name.c
        with open(mpath, 'w+') as filehandle:
            total_line = len(num_of_lines)
            i = 0
            while i < total_line:
                # Add Header file
                if include_one == True and self.schema.CheckIncludeHeaderLine(num_of_lines[i]) == True:
                    while self.schema.CheckIncludeHeaderLine(num_of_lines[i]) == True:
                        filehandle.write(num_of_lines[i])
                        i += 1
                    if profile_type == PROFILING_RUNTIME:
                        filehandle.write(self.schema.IncludeProfileHeader(profile_type))
                    else:
                        filehandle.write(self.schema.AddProbeGlobalFuncDeclare())
                    include_one = False
                # Add Profile Init function
                elif profile_type == PROFILING_RUNTIME and i == self.initeline:
                    filehandle.write(self.schema.AddProfileInitFunc())
                    vl = True
                # Add Profiling for Step function
                elif bool(self.pstepDict) and off < len(sortedList):
                    if i >= sortedList[off][1][0]:
                        step_flag = True if sortedList[off][0][:4] == 'step' or sortedList[off][0][:4] == 'STEP' else False
                        if profile_type == JASchema.PROFILING_RUNTIME or (profile_type == JASchema.PROFILING_JPROBE and step_flag == False):
                            thread_level = int(sortedList[off][0][-1])
                            probe_id = (thread_level << 16) | (self.probeCounter << 8)
                            filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_START, sortedList[off][0], profile_type, probe_id))
                        diff = sortedList[off][1][1] - sortedList[off][1][0]
                        for j in range(diff):
                            filehandle.write(num_of_lines[i + j])
                        if profile_type == JASchema.PROFILING_RUNTIME or (profile_type == JASchema.PROFILING_JPROBE and step_flag == False):
                            filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_STOP, sortedList[off][0], profile_type, probe_id))
                            self.funcProcIdDict[sortedList[off][0]] = self.probeCounter
                            self.probeCounter += 1
                        # To clear the begin/end in sortedList means the profiling start/stop is added
                        sortedList[off][1][1] = sortedList[off][1][0] = 0
                        off += 1
                        i = i + j + 1
                filehandle.write(num_of_lines[i])
                i += 1
            vl = True
            # Check if profing start/stop added to all step functions
            if bool(self.pstepDict) and vl == True:
                for item in sortedList:
                    if item[1][1] != 0 or item[1][0] != 0:
                        JAlog_print(JAG_ERROR, 'There are step functions that Profiling start/stop cannot be added to.')
                        vl = False
                        break
        return vl

    ##########################################################################
    # function name: AddProfilingFeaFunc
    # Description:   Add MIPS profiling for features.
    # Argument:      filepath:            file path to the C code of Model
    #                profile_type:        Two types of profiling. Runtime or
    #                                     JProbe
    # Return:        None
    ##########################################################################
    def AddProfilingFeaFunc(self, filepath=None, profile_type=PROFILING_RUNTIME):
        vl = False
        for k, v in self.pfeaDict.items():
            fname = k[:k.find('#')]
            sortedList = [[subk, subv] for subk, subv in sorted(v.items(), key=lambda item: item[1])]
            off = 0
            fpath = os.path.join(filepath, fname)
            with open(fpath, 'r') as f:
                num_of_lines = f.readlines()
            os.remove(fpath)
            include_one = True
            with open(fpath, 'w+') as filehandle:
                total_line = len(num_of_lines)
                i = 0
                while i < total_line:
                    # Add Header file
                    #if include_one == True and self.schema.CheckIncludeHeaderLine(num_of_lines[i]) == True:
                    if include_one == True:
                        while self.schema.CheckIncludeHeaderLine(num_of_lines[i]) == True:
                            i += 1
                        if profile_type == PROFILING_RUNTIME:
                            filehandle.write(self.schema.IncludeProfileHeader(profile_type))
                        else:
                            filehandle.write(self.schema.AddProbeGlobalFuncDeclare())
                        include_one = False
                        vl = True
                    # Add Profiling for Feature Processing function
                    elif off < len(sortedList):
                        if i >= sortedList[off][1][0]:
                            step_flag = True if sortedList[off][0][:4] == 'step' or sortedList[off][0][:4] == 'STEP' else False
                            if profile_type == JASchema.PROFILING_RUNTIME or (profile_type == JASchema.PROFILING_JPROBE and step_flag == False):
                                thread_level = int(sortedList[off][0][-1])
                                probe_id = (thread_level << 16) | (self.probeCounter << 8)
                                filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_START, sortedList[off][0], profile_type, probe_id))
                            diff = sortedList[off][1][1] - sortedList[off][1][0]
                            for j in range(diff):
                                filehandle.write(num_of_lines[i + j])
                            if profile_type == JASchema.PROFILING_RUNTIME or (profile_type == JASchema.PROFILING_JPROBE and step_flag == False):
                                filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_STOP, sortedList[off][0], profile_type, probe_id))
                                self.funcProcIdDict[sortedList[off][0]] = self.probeCounter
                                self.probeCounter += 1
                            # To clear the begin/end in sortedList means the profiling start/stop is added
                            sortedList[off][1][1] = sortedList[off][1][0] = 0
                            off += 1
                            i = i + j + 1
                    filehandle.write(num_of_lines[i])
                    i += 1
            if bool(sortedList):
                for item in sortedList:
                    if item[1][1] != 0 or item[1][0] != 0:
                        JAlog_print(JAG_ERROR, 'The feature that Profiling start/stop cannot be added to.')
                        vl = False
                        break
            if vl == False:
                JAlog_print(JAG_ERROR, 'The feature profiling error, ' + str(k))
                break
        if not bool(self.pfeaDict):
            # No feature needed for profiling
            vl = True
        return vl

    def AddProfilingPltFunc(self, filepath=None, profile_type=PROFILING_RUNTIME, profile_buildoption=None):
        vl = False
        for k, v in self.ppltDict.items():
            sortedList = [[sk, sv] for ele in v for sk, sv in ele.items()]
            off = 0
            with open(k, 'r') as f:
                num_of_lines = f.readlines()
            os.remove(k)
            include_one = True
            with open(k, 'w+') as filehandle:
                total_line = len(num_of_lines)
                i = 0
                while i < total_line:
                    # Add Header file
                    if include_one == True:
                        while self.schema.CheckCommentLine(num_of_lines[i]) == True:
                            filehandle.write(num_of_lines[i])
                            i += 1
                        while self.schema.CheckIncludeHeaderLine(num_of_lines[i]) == True:
                            filehandle.write(num_of_lines[i])
                            i += 1
                        filehandle.write(self.schema.IncludeProfileHeader(profile_type))
                        include_one = False
                        vl = True
                    # Add Profiling for Feature Processing function
                    elif off < len(sortedList):
                        if i >= sortedList[off][1][0]:
                            if profile_type == JASchema.PROFILING_RUNTIME:
                                if profile_buildoption != None:
                                    filehandle.write(self.schema.AddPlatformConditionalBuild(profile_buildoption, 0))
                                filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_START, sortedList[off][0], profile_type))
                                if profile_buildoption != None:
                                    filehandle.write(self.schema.AddPlatformConditionalBuild(profile_buildoption, 1))
                            diff = sortedList[off][1][1] - sortedList[off][1][0]
                            for j in range(diff):
                                filehandle.write(num_of_lines[i + j])
                            if profile_type == JASchema.PROFILING_RUNTIME:
                                if profile_buildoption != None:
                                    filehandle.write(self.schema.AddPlatformConditionalBuild(profile_buildoption, 0))
                                filehandle.write(self.schema.Addprofilestartstop(JProfileMagic.PROFILING_STOP, sortedList[off][0], profile_type))
                                if profile_buildoption != None:
                                    filehandle.write(self.schema.AddPlatformConditionalBuild(profile_buildoption, 1))
                                self.funcProcIdDict[sortedList[off][0]] = self.probeCounter
                                self.probeCounter += 1
                            # To clear the begin/end in sortedList means the profiling start/stop is added
                            sortedList[off][1][1] = sortedList[off][1][0] = 0
                            off += 1
                            i = i + j + 1
                    filehandle.write(num_of_lines[i])
                    i += 1
            if bool(sortedList):
                for item in sortedList:
                    if item[1][1] != 0 or item[1][0] != 0:
                        JAlog_print(JAG_ERROR, 'The feature that Profiling start/stop cannot be added to.')
                        vl = False
                        break
            if vl == False:
                JAlog_print(JAG_ERROR, 'The feature profiling error, ' + str(k))
                break
        if not bool(self.ppltDict):
            # No platform needed for profiling
            vl = True
        return vl

    ##########################################################################
    # function name: GenerateProfile
    # Description:   Generate OJ_profile.c/.h for Model
    # Argument:      filepath:            file path to the C code of Model
    #                matlab_model_name:   Model name, like Model_1_1
    #                plt:                 Platform, like GUL, SHARC
    #                callrate:            callrate table including all call rates
    #                cfgpath:             path to the call rate file
    #                local_file:          local file holding profiling data for analysis
    # Return:        None
    ##########################################################################
    def GenerateProfile(self, filepath=None, matlab_model_name=None, plt='gul', callrate=None, cfgpath=None, local_file=None):
        vl = False
        callrateList = []
        # Locate to ...\JAutogen\template\ folder
        tmp_path = inspect.getfile(JMagic)
        tmp_path = tmp_path.split('\\')
        tmp_path.pop()
        tmp_path.append('template')
        tmp_path = '\\'.join(tmp_path)
        # Write OJ_Profile.h
        num_of_lines = None
        hpath = os.path.join(filepath, PROFILE_H)
        if plt == 'DSP_GUL':
            profile_header_tpl1 = 'OJ_profile_gul.h.part1'
            profile_header_tpl2 = 'OJ_profile_gul.h.part2'
        elif plt == 'DSP_HEXAGON':
            profile_header_tpl1 = 'OJ_profile_hexagon.h.part1'
            profile_header_tpl2 = 'OJ_profile_hexagon.h.part2'
        else:
            JAlog_print(JAG_ERROR, 'DSP Architecture not supported.')
            return
        shutil.copyfile(os.path.join(tmp_path, profile_header_tpl1), hpath)
        with open(os.path.join(tmp_path, profile_header_tpl2), 'r') as filehandle:
            num_of_lines = filehandle.read()
        defcounter = 0
        profile_name = []

        with open(hpath, 'a') as filehandle:
            if local_file != None:
                filehandle.write(self.schema.AddLocalFileMacro(local_file) + '\n\n')
            if self.inlineEnable == True and plt == 'DSP_GUL':
                filehandle.write(self.schema.AddDefineMacroInline(self.inlineEnable) + '\n\n')
            if bool(self.pstepDict):
                for k in self.pstepDict.keys():
                    filehandle.write(self.schema.AddDefineMacroProfiling(k, str(defcounter)))
                    callrateList.append(callrate[k[-1]])
                    defcounter += 1
                    profile_name.append(k)
            if bool(self.pfeaDict):
                for v in self.pfeaDict.values():
                    for k in v.keys():
                        filehandle.write(self.schema.AddDefineMacroProfiling(k, str(defcounter)))
                        callrateList.append(callrate[k[-1]])
                        defcounter += 1
                        profile_name.append(k)
            if bool(self.ppltDict):
                for v in self.ppltDict.values():
                    for ele in v:
                        for k in ele.keys():
                            filehandle.write(self.schema.AddDefineMacroProfiling(k, str(defcounter)))
                            callrateList.append(float(1.0))
                            defcounter += 1
                            profile_name.append(k)

            filehandle.write(self.schema.AddDefineMacroProfiling('MAX_NUM', str(defcounter)) + '\n\n')
            filehandle.write('\n\n')
            filehandle.write(self.schema.AddDefineProfilingConfig(self.threhold, self.sst, plt) + '\n\n')
            filehandle.write(self.schema.AddProfilingContextStruct(plt) + '\n\n')
            filehandle.write(self.schema.AddProfilingDataStruct(matlab_model_name))
            if num_of_lines != None:
                filehandle.write(num_of_lines)
                vl = True
            else:
                JAlog_print(JAG_ERROR, 'Read ' + profile_header_tpl2 + ' ERROR')
        # Write OJ_Profile.c
        num_of_lines = None
        cpath = os.path.join(filepath, PROFILE_C)
        if plt == 'DSP_GUL':
            profile_c_tpl1 = 'OJ_profile_gul.c.part1'
            profile_c_tpl2 = 'OJ_profile_gul.c.part2'
        elif plt == 'DSP_HEXAGON':
            profile_c_tpl1 = 'OJ_profile_hexagon.c.part1'
            profile_c_tpl2 = 'OJ_profile_hexagon.c.part2'
        else:
            JAlog_print(JAG_ERROR, 'DSP Architecture not supported.')
            return
        shutil.copyfile(os.path.join(tmp_path, profile_c_tpl1), cpath)
        with open(os.path.join(tmp_path, profile_c_tpl2), 'r') as filehandle:
            num_of_lines = filehandle.read()
        with open(cpath, 'a') as filehandle:
            filehandle.write('\n\n')
            filehandle.write(self.schema.AddVariableForProfilingName(profile_name) + '\n\n')
            filehandle.write(self.schema.AddMipsNCounterPara(matlab_model_name, callrateList, plt))
            filehandle.write(self.schema.AddProfilingStructureAssign(matlab_model_name))
            if num_of_lines != None:
                new_lines = self.schema.CheckandUpdateCycleMips(self.cycleOrmips, num_of_lines)
                if new_lines == None:
                    filehandle.write(num_of_lines)
                else:
                    filehandle.write(new_lines)
                vl = vl and True
            else:
                JAlog_print(JAG_ERROR, 'Read ' + profile_c_tpl2 + ' ERROR')
        # Generate callrate config txt file
        if cfgpath != None:
            cfgfile = os.path.join(cfgpath, matlab_model_name + '_lookup.txt')
            if os.path.exists(cfgfile):
                os.remove(cfgfile)
            with open(cfgfile, 'w+') as filehandle:
                if bool(self.pstepDict):
                    for k in self.pstepDict.keys():
                        filehandle.write(k + ': ' + '{0:.8f}'.format((1.0 / callrate[k[-1]]) / 1000000) + '\n')
                if bool(self.pfeaDict):
                    for v in self.pfeaDict.values():
                        for k in v.keys():
                            filehandle.write(k + ': ' + '{0:.8f}'.format((1.0 / callrate[k[-1]]) / 1000000) + '\n')
                if bool(self.ppltDict):
                    for v in self.ppltDict.values():
                        for ele in v:
                            for k in ele.keys():
                                filehandle.write(k + ': ' + '{0:.8f}'.format(0.0) + '\n')
        return vl

    def GenerateTOPCmd(self, filepath=None, matlab_model_name=None):
        topsize = (len(self.pfeaDict) + len(self.pstepDict) + len(self.ppltDict)) * JProfileMagic.PROFILING_STRUSIZE
        toppath = os.path.join(filepath, matlab_model_name + JProfileMagic.TOP_SUFFIX)
        asdidpath = os.path.join(filepath, JProfileMagic.ASDID_HEADER)
        if os.path.exists(toppath) and os.path.exists(asdidpath):
            #topstr = JProfileMagic.PROFILE_TOP_TEMPLATE.replace('ModelXXX', matlab_model_name)
            toptmp = JProfileMagic.PROFILE_TOP_STRUCTURE.replace('PROFILINGSIZEXXX', str(topsize))
            etnstr = toptmp.replace('ModelXXX', matlab_model_name)
            with open(asdidpath, 'r') as filehandle:
                asddata = filehandle.read()
            with open(toppath, 'r') as filehandle:
                fdata = filehandle.read()
            # Check if it is direct addressing mode or indirect addressing mode
            if JProfileMagic.TOP_DIRECTADDRESS_KEYWORD in asddata:
            #if JProfileMagic.TOP_DIRECTADDRESS_KEYWORD_BAK in fdata:
                # Direct addressing mode
                num_of_lines = fdata.split('\n')
                os.remove(toppath)
                findstat = 0
                count = 0
                with open(toppath, 'w+') as filehandle:
                    for index, line in enumerate(num_of_lines):
                        if JProfileMagic.TOP_DIRECTADDRESS_KEYWORD_BAK in line:
                            filehandle.write(etnstr)
                            findstat = 1
                        if findstat == 1 and '},' in line:
                            count = count + 1
                        elif findstat == 1 and '};' in line:
                            if count < JProfileMagic.MAX_TOP:
                                for ll in range(JProfileMagic.MAX_TOP - count - 1):
                                    filehandle.write(JProfileMagic.PROFILE_TOP_EMPTY)
                                filehandle.write(JProfileMagic.PROFILE_TOP_TEMPLATE)
                            else:
                                JAlog_print(JAG_ERROR, 'There is no place to put Profile structure in TOP command.')
                            findstat = 0
                        filehandle.write(line + '\n')
            else:
                # Indirect addressing mode
                pass
        else:
            JAlog_print(JAG_ERROR, toppath + ' does not exist.')