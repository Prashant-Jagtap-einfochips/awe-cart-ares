import os
import sys

from  JAConv import *



class JASchema:
    PROFILING_START = 1
    PROFILING_STOP = 2
    PROFILING_RUNTIME = 1
    PROFILING_JPROBE = 2
    PROFILING_INCYCLE = 1
    PROFILING_INMIPS = 2
    SINGLE_RATE = 1
    MULTI_RATE = 2
    PROBE_START = 0
    PROBE_END = 1
    GUL_READCLOCK = '__builtin_emuclk()'
    def __init__(self):
        self.aconv = JMatlabConv()
        self.profile_context_gul_tpl = '''
typedef struct {
    float peak_value;
    float avg_value;
    unsigned int prof_counter;
    unsigned int over_counter;
    unsigned int switch_counter;
    //void* taskptr;
    signed int taskprio;
    unsigned int cur_cycles;
    unsigned int start_counter;
    unsigned int othertask_start_counter;
} Profile_Context;'''
        self.profile_context_hexagon_tpl = '''
typedef struct {
    unsigned int pktcnt_min;
    unsigned int pktcnt_avg;
    unsigned int pktcnt_max;
    unsigned int cyccnt_min;
    unsigned int cyccnt_avg;
    unsigned int cyccnt_max;
    unsigned int start_pktcounter;
    unsigned int start_cyccounter;
    unsigned int prof_counter;
} Profile_Context;'''
        self.profile_datastructure_tpl = '''
extern const char* profiling_name[];
extern const float XXXXmodelname_mipspara[MAX_NUM_PROFILING];
extern const unsigned int XXXXmodelname_counterpara[MAX_NUM_PROFILING];


// Profiling Mask specifies the enable/disable of each profiling
extern bool XXXXmodelname_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
extern Profile_Context XXXXmodelname_profiledata[MAX_NUM_PROFILING];'''
        self.profile_structureassign_tpl = '''// Profiling Mask specifies the enable/disable of each profiling
bool XXXXmodelname_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
Profile_Context XXXXmodelname_profiledata[MAX_NUM_PROFILING];


// Profiling Mask Pointer
bool* pProfileMask = XXXXmodelname_profilemask;
// Profiling Context Pointer
Profile_Context* pProfileContext = XXXXmodelname_profiledata;
// Profiling MIPS parameter table Pointer
const float* pProfileMipsPara = XXXXmodelname_mipspara;
// Profiling Counter table Pointer
const unsigned int* pProfileCounterPara = XXXXmodelname_counterpara;
// Profiling maxinum of instances
const unsigned int max_num_profiling = MAX_NUM_PROFILING;\n\n'''


    def AddExportSignalBufferFunc(self, section, name, length, comment):
        ss = []
        ss.append('/******** Function for ' + str(comment) + '********/\n')
        ss.append('float* getBufPointer_Signal_' + name + '_' + length + '(void)\n')
        ss.append('{\n')
        ss.append('    return (float*)' + section + '.' + name + ';\n')
        ss.append('}\n')
        return ''.join(ss)

    def AddExportStateBufferFunc(self, section, name, length, comment):
        ss = []
        ss.append('/******** Function for ' + str(comment) + '********/\n')
        ss.append('float* getBufPointer_State_' + name + '_' + length + '(void)\n')
        ss.append('{\n')
        ss.append('    return (float*)' + section + '.' + name + ';\n')
        ss.append('}\n')
        return ''.join(ss)

    def AddExportSpecifiedBufferFunc(self, section, name, length, comment):
        ss = []
        ss.append('/******** Function for ' + str(comment) + '********/\n')
        ss.append(self.aconv.funcname_exportbuffer(name, length) + '\n')
        ss.append('{\n')
        ss.append('    return (float*)' + self.aconv.variablename_buffermember(section, name) + ';\n')
        ss.append('}\n')
        return ''.join(ss)

    def ReturnSpecifiedBufferVar(self, section, name):
        return self.aconv.variablename_buffermember(section, name)

    def ReturnExportSpecifiedBufferFunc(self, name, length):
        return 'getBufPointer_' + name + '_' + length

    def CheckCommentLine(self, lineitme):
        return True if lineitme.startswith('/*') or lineitme.startswith(' /*') or lineitme.startswith(' *') or lineitme.startswith('//') or lineitme.startswith('*/') else False

    def CheckIncludeHeaderLine(self, lineitem):
        return True if '#include ' in lineitem else False

    def IncludeProfileHeader(self, profile_type):
        return '#include ' + '"' + 'OJ_profile.h' + '"' + '\n' if profile_type == 1 else '#include ' + '"' + 'Logger.h' + '"' + '\n'

    def AddProbeGlobalFuncDeclare(self):
        return '\n\nextern void probe_point(uint32_t id);\n\n'

    def AddProfileInitFunc(self):
        return '    OJ_profileInit();\n'

    def AddPlatformConditionalBuild(self, conditionalbuild=None, bid=0):
        return '#if defined(' + conditionalbuild + ')\n' if bid == 0 else '#endif\n\n'

    def Addprofilestartstop(self, start_stop=None, arg=None, profile_type=None, probe_id=0):
        ss = ''
        if profile_type == JASchema.PROFILING_RUNTIME:
            if start_stop == JASchema.PROFILING_START:
                ss = '    OJ_profileStart(' + arg.upper() + '_PROFILING);\n'
            else:
                ss = '    OJ_profileStop(' + arg.upper() + '_PROFILING);\n'
        return ss

    def AddLocalFileMacro(self, local_file=None):
        ss = []
        ss.append('#define SAVE_PROFILING_TO_LOCAL\n')
        ss.append('#define PROFILING_LOCAL_FILE              ' + '"' + local_file + '"' + '\n')
        return ''.join(ss)

    def AddDefineMacroInline(self, enable=True):
        ss = '#define PROFILE_STARTSTOP_INLINE                (1)\n' if enable == True else '' 
        return ss

    def AddDefineMacroProfiling(self, name, value):
        pname = name.upper() + '_PROFILING'
        return '#define ' + pname + ' ' * (60 - len(pname)) + '(' + value + ')\n'

    def AddVariableForProfilingName(self, name_list):
        nameL = []
        nameL.append('const char* profiling_name[] = {')
        for name in name_list:
            nameL.append('"' + name.upper() + '"' + ', ')
        nameL[len(nameL) - 1] = nameL[len(nameL) - 1][:-2]
        nameL.append('};\n')
        return ''.join(nameL)

    def AddDefineProfilingConfig(self, threhold, sst, plt):
        ss = []
        if plt == 'DSP_GUL':
            tmp = JASchema.GUL_READCLOCK
            ss.append('// Profile threshold\n')
            ss.append('#define PROFILE_THRESHOLD            (' + str(threhold) + ')\n')
            ss.append('// Profile statiscal start time\n')
            ss.append('#define PROFILE_STTIME               (' + str(sst) + ')\n')
            ss.append('// Read DSP Clock\n')
            ss.append('#define READ_CLOCKCYCLE              (' + tmp + ')\n')
            ss.append('\n\n')
            ss.append('// Profile in Cycles or MIPS\n')
            ss.append('#define PROFILE_IN_CYCLE             (1)\n')
            ss.append('#define PROFILE_IN_MIPS              (2)\n')
        return ''.join(ss)

    def AddProfilingContextStruct(self, plt):
        if plt == 'DSP_GUL':
            return self.profile_context_gul_tpl
        else:
            return self.profile_context_hexagon_tpl

    def AddProfilingDataStruct(self, modelname):
        return self.profile_datastructure_tpl.replace('XXXXmodelname', modelname)

    def AddMipsNCounterPara(self, modelname, callrateList, plt):
        float_format = '{0:.8f}'
        ss = []
        ss.append('// MIPS parameter table, this should be calculated automatically in code generation.\n')
        ss.append('const float ' + modelname + '_mipspara[MAX_NUM_PROFILING] = {\n')
        ss.append('    // step functions and feature processing functions\n')
        for item in callrateList:
            para = (1.0 / item) / 1000000
            #ss.append('    ' + str(para) + ',\n')
            ss.append('    ' + float_format.format(para) + ',\n')
        ss.append('};\n')
        ss.append('\n\n')
        ss.append('// Counter table, this should be calculated automatically in code generation.\n')
        ss.append('const unsigned int ' + modelname + '_counterpara[MAX_NUM_PROFILING] = {\n')
        ss.append('// step functions and feature processing functions\n')
        for item in callrateList:
            para = int(1.0 / item)
            if plt == 'DSP_GUL':
                ss.append('    ' + str(para) + ' * PROFILE_STTIME,\n')
            else:
				# Not needed for other DSPs
                ss.append('    ' + str(para) + ' * 1,\n')
        ss.append('};\n')
        return ''.join(ss)

    def AddProfilingStructureAssign(self, modelname):
        return self.profile_structureassign_tpl.replace('XXXXmodelname', modelname)

    def CheckandUpdateCycleMips(self, cycleOrMips, filebuf):
        newbuf = None
        if cycleOrMips == JASchema.PROFILING_INMIPS:
            newbuf = filebuf.replace('PROFILE_IN_CYCLE', 'PROFILE_IN_MIPS')
        return newbuf