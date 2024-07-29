import os
import sys
import glob
import shutil
sys.path.append('../JConfig/')

# import xml.etree
import xml.etree.ElementTree as ElementTree
# import Convention
from JAConv import *
# import magic
from JMagic import *
# import skill
from JFuncSkill import *
# import print log
from JALogPrint import *
# import JAutogenCode
from JAutogen import *








##############################################################################
# class Name:  JAutogenProbe
# Description: It provides APIs to read/modify the existed C file. Based on some
#              Simple patterns in C files, JAutogenProbe parse C code automatically,
#              extract the hierarchy of global variables, function prototypes, etc.
#              JAutogenProbe works with JAutogenCode to add functionalities to the generated
#              C code, like profiling, enable/disable step function, buffer re-
#              direction, #progma, etc.
##############################################################################
class JAutogenProbe:
    PLATFORM_ATTACHKEYWORKD = 'PLATFORM_ATTACHED||'
    PLATFORM_BUILDKEYWORKD = 'PLATFORM_BUILD_DEFINITION'
    DSPGUL = 'DSP_GUL'
    DSPHEXAGON = 'DSP_HEXAGON'
    ARMANDROID = 'ARM_ANDROID'
    PRAGMA_GROUP_GENERAL=1
    PRAGMA_GROUP_CFILE=2
    PRAGMA_GROUP_STEP=3
    PROFILE_RUNTIME = 1
    PROFILE_JPROBE = 2
    PROFILE_BAK = '.jabak'
    FUNC_KEYWORDS = ['OJ_profileStart', 'OJ_profileStop', 'qurt_profile_get_thread_pcycles', 'qurt_profile_pktcount_get']
    HEADER_KEYWORDS = ['TOP', 'RTC', 'TSP', 'Talaria', 'rt_', 'rtwtypes', 'private']
    def __init__(self, dsp_arch='DSP_GUL', filepath='./', jmodulepath = './', genConfigFile='', printLevel=2):
        self.dsparch = dsp_arch
        self.filepath = filepath if filepath[:-1] == '/' else filepath + '/'
        self.jmodulepath = jmodulepath if jmodulepath[:-1] == '/' else jmodulepath + '/'
        self.printLevel = printLevel
        self.callrate = {}
        self.modelBuf_framesize = 0
        self.modelBuf_channel = 0
        self.func_of_buf = []
        # Format [function_name1, buffer_size1, function_name2, buffer_size2, ...]
        self.modelBuf_info = []
        self.func_name_buf = []
        self.genConfigFile = genConfigFile if os.path.exists(genConfigFile) else ''
        # List all header files, filtered by .c/.h
        header = glob.glob(self.filepath + '*.h')
        self.headerList = [f[f.rfind('\\') + 1:] for f in header if os.path.exists(f[:-2] + '.c')]
        #self.headerList = [f for f in self.headerList if any(kw in f for kw in JAutogenProbe.HEADER_KEYWORDS)]
        self.profile = None
        self.pltbuild = None
        self.profilmodelname = None
        self.profileList = []
        self.conv = JMatlabConv()
        JAlog_init(self.printLevel)

    def EvokeGoldenMatlabBuffer(self, matlab_model_name=None):
        hfile = matlab_model_name + '.h'
        cfile = matlab_model_name + '.c'
        if os.path.exists(self.filepath + hfile) and os.path.exists(self.filepath + cfile):
            shutil.copyfile(self.filepath + cfile, self.filepath + cfile + '.bak')
            jmgc = JProbeMagic(matlab_model_name, self.filepath + hfile, self.filepath + cfile)
            jmgc.BasicProbeforBuffer()
        else:
            JAlog_print(JAG_ERROR, 'Cannot find the files: ' + matlab_model_name + '.h/.c....')

    def SilenceGodenMatlabBuffer(self, matlab_model_name=None):
        cfile = matlab_model_name + '.c'
        if os.path.exists(self.filepath + cfile) and os.path.exists(self.filepath + cfile + '.bak'):
            os.remove(self.filepath + cfile)
            os.rename(self.filepath + cfile + '.bak', self.filepath + cfile)

    def EvokePlatinumMatlabBuffer1(self, matlab_model_name=None, matlab_model_path=None, mbuf_fs=0, mbuf_ch=0):
        self.modelBuf_framesize = mbuf_fs
        self.modelBuf_channel = mbuf_ch
        self.func_name_buf = []
        self.modelBuf_info = []

        jmgc = JProbeMagic(matlab_model_name, matlab_model_path)
        for f in self.headerList:
            # Filtered with keywords
            if not any(kw in f for kw in JAutogenProbe.HEADER_KEYWORDS):
                jmgc.hfile = self.filepath + f
                jmgc.cfile = self.filepath + f[:-2] + '.c'
                # Traverse All Model path first
                jmgc.CollectAllMatlabClues()
        if jmgc.CheckExistOfModelNum() == True:
            # Probe
            for f in self.headerList:
                # Filtered with keywords
                if not any(kw in f for kw in JAutogenProbe.HEADER_KEYWORDS):
                    jmgc.hfile = self.filepath + f
                    jmgc.cfile = self.filepath + f[:-2] + '.c'
                    fob = []
                    fnb = []
                    mi = []
                    fob = jmgc.AdvancedProbeforBuffer(fnb, mi)
                    self.func_of_buf += fob
                    self.func_name_buf += fnb
                    self.modelBuf_info += mi
                    #self.func_of_buf = jmgc.AdvancedProbeforBuffer(self.func_name_buf, self.modelBuf_info)

    def EvokePlatinumMatlabBuffer2(self, matlab_model_name=None, offset=None):
        jmlpath = None
        # Search JModule_Adapt_Sys.c or JModule_Adapt_sys.c
        if os.path.exists(self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_sys.c'):
            jmlpath = self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_sys.c'
        elif os.path.exists(self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_Sys.c'):
            jmlpath = self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_Sys.c'
        if jmlpath != None:
            if os.path.exists(jmlpath + '.bak'):
                os.remove(jmlpath)
                shutil.copyfile(jmlpath + '.bak', jmlpath)
            else:
                shutil.copyfile(jmlpath, jmlpath + '.bak')
            if  len(self.func_of_buf) > 0:
                jdm = JAutogenCode(matlab_model_name)
                if offset == None:
                    for index in range(0, len(self.modelBuf_info), 2):
                        if self.modelBuf_framesize * self.modelBuf_channel == int(self.modelBuf_info[index + 1]):
                            jdm.AddProbetoJModule(jmlpath, matlab_model_name, self.modelBuf_framesize, self.modelBuf_channel, self.func_of_buf[index >> 1], self.func_name_buf[index >> 1])
                            break
                else:
                    jdm.AddProbetoJModule(jmlpath, matlab_model_name, self.modelBuf_framesize, self.modelBuf_channel, self.func_of_buf[offset], self.func_name_buf[offset])
            else:
                JAlog_print(JAG_ERROR, 'Cannot find the S<xxx> based on Model Path...')
        else:
            JAlog_print(JAG_ERROR, 'The JModule Folder or its sub folders cannot be found.')

    def SilencePlatinumMatlabBuffer(self):
        jmlpath = None
        if os.path.exists(self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_sys.c'):
            jmlpath = self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_sys.c'
        elif os.path.exists(self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_Sys.c'):
            jmlpath = self.jmodulepath + 'JModule_Lib/Sys/JModule_Adapt_Sys.c'
        if jmlpath != None:
            if os.path.exists(jmlpath + '.bak'):
                os.remove(jmlpath)
                os.rename(jmlpath + '.bak', jmlpath)
        else:
            JAlog_print(JAG_ERROR, 'The JModule Folder or sub folder cannot be found')


    ##########################################################################
    # function name: PrepareForProfiling
    # Description:   Configure Profiling, the first function to be called for
    #                profiling.
    # Argument:      matlab_model_name:   Model name for c file, like Model_1_1, Model_Target
    #                path_to_callrate:    path to the xml file (Model_SyncApi.xml)
    #                inlineEnable:        Enable/Disable inline for profiling start/stop
    #                cycleOrMips:         profiling calculation in cycles or mips (1 or 2)
    #                threhold:            threhold (0 - 1.0)
    #                sst:                 in seconds, start statistical counter after
    #                                     N seconds of profiling. Disable statistical
    #                                     counter with sst = 0
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def PrepareForProfiling(self, matlab_model_name=None, path_to_callrate=None, inlineEnable=True, cycleOrmips=1, threhold=0.1, sst=5):
        vl = False
        self.profilmodelname = matlab_model_name
        if os.path.exists(path_to_callrate):
            tree = ElementTree.parse(path_to_callrate)
            root = tree.getroot()
            for item in root.findall('Partition'):
                each_model = item.get('id')
                self.callrate[each_model] = {}
                for subitem in item.findall('Task'):
                    self.callrate[each_model][subitem.get('id')] = float(subitem.get('rate_in_sec'))
            self.profile = JProfileMagic(inlineEnable, cycleOrmips, threhold, sst)
            vl = True if bool(self.callrate) else vl
        else:
            JAlog_print(JAG_ERROR, 'The file does not exist: ' + path_to_callrate)
        return vl

    ##########################################################################
    # function name: AddProfilingInitNStep
    # Description:   Based on Model name, the Model C file gets parsed to find
    #                Init functions, step functions.
    #                Add profiling to each step function if needed.
    # Argument:      stepEnable:          Enable/Disable step function profiling
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def AddProfilingInitNStep(self, stepEnable=True):
        vl = False
        # Restore all profiling .c files first
        bakfileList = [x for x in glob.glob(self.filepath + '*' + JAutogenProbe.PROFILE_BAK)] if os.path.exists(self.filepath) else []
        if bool(bakfileList):
            for bfile in bakfileList:
                os.remove(bfile[:-len(JAutogenProbe.PROFILE_BAK)])
                shutil.copyfile(bfile, bfile[:-len(JAutogenProbe.PROFILE_BAK)])
                os.remove(bfile)
        # Create backup files
        jf = JFuncSkill(self.profilmodelname, self.filepath)
        mpath = os.path.join(self.filepath, self.profilmodelname + '.c')
        if os.path.exists(mpath + JAutogenProbe.PROFILE_BAK):
            shutil.copyfile(mpath + JAutogenProbe.PROFILE_BAK, mpath)
        else:
            shutil.copyfile(mpath, mpath + JAutogenProbe.PROFILE_BAK)
        toppath = os.path.join(self.filepath, self.profilmodelname + JProfileMagic.TOP_SUFFIX)
        if os.path.exists(toppath + JAutogenProbe.PROFILE_BAK):
            shutil.copyfile(toppath + JAutogenProbe.PROFILE_BAK, mpath)
        else:
            # In case that TOP c file not generated, Error printed out in GenerateTOPCmd() function
            if os.path.exists(toppath):
                shutil.copyfile(toppath, toppath + JAutogenProbe.PROFILE_BAK)
        # Enumerate all function in model_name.c file, like Model_1_1.c
        if (jf.EnumerateFuncbyFileName(self.profilmodelname + '.c') == True):
            # Search modelname_initialize function, like Model_1_1_initialize()
            loc_init = jf.GetFuncLocationByFileName(self.profilmodelname + '.c', self.conv.funcname_model_initialize(self.profilmodelname))
            if bool(loc_init):
                self.profile.initbline = loc_init[0]
                self.profile.initeline = loc_init[1]
                # Find all step functions
                if stepEnable == True:
                    for k, v in self.callrate.items():
                        if k == self.profilmodelname:
                            if len(v) > 1:
                                # Multi-callrate
                                for stepitem in v:
                                    self.profile.pstepDict['step' + stepitem] = jf.GetFuncLocationByFileName(self.profilmodelname + '.c', self.conv.funcname_model_multistep(self.profilmodelname, stepitem))
                            else:
                                # Single-callrate
                                self.profile.pstepDict['step0'] = jf.GetFuncLocationByFileName(self.profilmodelname + '.c', self.conv.funcname_model_singlestep(self.profilmodelname))
                vl = True
            else:
                JAlog_print(JAG_ERROR, 'The Model initialize function cannot be found: ' + self.profilmodelname)
        else:
            JAlog_print(JAG_ERROR, 'The function cannot be detected in file: ' + self.profilmodelname + '.c')
        return vl

    ##########################################################################
    # function name: AddProfilingFeature
    # Description:   Add Simulink Feature for profiling, the feature is wrapped
    #                as atomic subsystem in simulink. The feature processing
    #                function get parsed.
    # Argument:      filename:            file name of feature, like PASD.c
    #                feaname:             feature name used in case that function
    #                                     name equals None. It is useful that only
    #                                     filename is specified in Simulink Code
    #                                     Generation, and function name is generated
    #                                     automatically.
    #                funcname:            function name of feature, like Model_1_1_PASD,
    #                                     It can be specified in Simulink Code Generation.
    #                                     It could be a list to include more functions, and
    #                                     in this case, it is just function profiling rather
    #                                     than feature profiling.
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def AddProfilingFeature(self, filename=None, feaname=None, funcname=None):
        vl = False
        mpath = os.path.join(self.filepath, filename)
        if os.path.exists(mpath + JAutogenProbe.PROFILE_BAK):
            shutil.copyfile(mpath + JAutogenProbe.PROFILE_BAK, mpath)
        else:
            shutil.copyfile(mpath, mpath + JAutogenProbe.PROFILE_BAK)
        fpath = os.path.join(self.filepath, filename)
        jf = JFuncSkill(self.profilmodelname, self.filepath)
        # Enumerate all function in filename.c file, like PASD.c
        if (jf.EnumerateFuncbyFileName(filename) == True):
            if type(funcname) == list:
                loc_func = []
                feaid = []
                for item in funcname:
                    loc_func.append(jf.GetFuncLocationByFileName(filename, item))
                    feaid.append(filename + '#' + item)
                if len(self.callrate[self.profilmodelname]) == 1:
                    # Single rate for the model
                    if bool(loc_func):
                        for index in range(len(loc_func)):
                            self.profile.pfeaDict[feaid[index]] = {funcname[index] + '_0':loc_func[index]}
                        vl = True
                else:
                    # Multi rate for the model
                    if bool(loc_func):
                        jf.EnumerateFuncbyFileName('*')
                        for index in range(len(funcname)):
                            callerList = []
                            jf.FindTopCallerByFuncName(funcname[index], callerList)
                            if bool(callerList):
                                stepf = callerList[len(callerList) - 2]
                                if self.profilmodelname in stepf:
                                    steplen = len(self.profilmodelname + '_step')
                                    sn = stepf[steplen:]
                                    # Feture is single rate
                                    self.profile.pfeaDict[feaid[index]] = {funcname[index] + '_' + sn:loc_func[index]}
                                else:
                                    JAlog_print(JAG_ERROR, 'The Step function is not the caller of this feature: ' + feaname)

                            else:
                                JAlog_print(JAG_ERROR, 'The Caller of the function cannot be found: ' + funcname[index])
                        vl = True
            else:
                fname = funcname if funcname != None else self.conv.funcname_model_feature(self.profilmodelname, feaname)
                loc_func = jf.GetFuncLocationByFileName(filename, fname)
                feaid = filename + '#' + fname
                if len(self.callrate[self.profilmodelname]) == 1:
                    # Single rate for the model
                    if bool(loc_func):
                        self.profile.pfeaDict[feaid] = {feaname + '_0':loc_func}
                        vl = True
                    else:
                        JAlog_print(JAG_ERROR, 'The Feature processing function cannot be found: ' + feaname)
                else:
                    # Multi rate for the model
                    if bool(loc_func):
                        # Feture is single rate
                        # Check step function calling this feature
                        jf.EnumerateFuncbyFileName('*')
                        callerList = []
                        jf.FindTopCallerByFuncName(fname, callerList)
                        if bool(callerList):
                            stepf = callerList[len(callerList) - 2]
                            if self.profilmodelname in stepf:
                                steplen = len(self.profilmodelname + '_step')
                                sn = stepf[steplen:]
                                # Feture is single rate
                                self.profile.pfeaDict[feaid] = {feaname + '_' + sn:loc_func}
                            else:
                                JAlog_print(JAG_ERROR, 'The Step function is not the caller of this feature: ' + feaname)
                        else:
                            JAlog_print(JAG_ERROR, 'The Caller of this feature cannot be found: ' + feaname)
                        vl = True
                    else:
                        # Feature is multi rate
                        loc_func = []
                        for k in self.callrate[self.profilmodelname].keys():
                            f1 = self.conv.funcname_model_feature_multirate(funcname, k)
                            f2 = self.conv.funcname_model_feature_multirate(self.conv.funcname_model_feature(self.profilmodelname, feaname), k)
                            tmp1 = jf.GetFuncLocationByFileName(filename, f1)
                            tmp2 = jf.GetFuncLocationByFileName(filename, f2)
                            if bool(tmp1):
                                if feaid in self.profile.pfeaDict:
                                    self.profile.pfeaDict[feaid][f1] = tmp1
                                else:
                                    self.profile.pfeaDict[feaid] = {f1 : tmp1}
                            elif bool(tmp2):
                                if feaid in self.profile.pfeaDict:
                                    self.profile.pfeaDict[feaid][f2] = tmp2
                                else:
                                    self.profile.pfeaDict[feaid] = {f2 : tmp2}
                        if feaid in self.profile.pfeaDict:
                            vl = True
                        else:
                            JAlog_print(JAG_ERROR, 'The Feature processing function cannot be found: ' + feaname)
            if vl == True:
                self.profileList.append(filename)
        return vl

    def LightCallrateToAll(self, matlab_model_name=None):
        vcrDict = {}
        model_funcs = JFuncSkill(matlab_model_name, self.filepath)
        model_funcs.EnumerateFuncbyFileName('*')
        #print(model_funcs.filefuncDict)
        step_filename = matlab_model_name + '.c'
        step_funckw = matlab_model_name + '_step'
        #print('===>' + step_filename)
        #print(model_funcs.filefuncDict)
        if step_filename in model_funcs.filefuncDict:
            stepL = [x[:x.find('#')] for x in model_funcs.filefuncDict[step_filename] if re.match('^' + step_funckw, x)]
            # if not single rate, remove the top step function.
            if len(stepL) > 1:
                stepL.remove(step_funckw)
            stackflow = JAutogenProbe.FUNC_KEYWORDS
            for step in stepL:
                calledD = {}
                model_funcs.FindSubFuncsByFuncNameEx(step, calledD, stackflow)

    ##########################################################################
    # function name: AddProfilingPlatform
    # Description:   Initialize function profiling for platform
    # Argument:      pltDict:        Platform profiling functions
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def AddProfilingPlatform(self, pltDict=None, pltbuild=None, continueflag=0):
        vl = False
        # Restore all profiling .c files first
        jf = JFuncSkill(self.profilmodelname, None)
        for pfile in pltDict.keys():
            if os.path.exists(pfile + JAutogenProbe.PROFILE_BAK):
                if (continueflag == 0):
                    os.remove(pfile)
                    shutil.copyfile(pfile + JAutogenProbe.PROFILE_BAK, pfile)
            else:
                shutil.copyfile(pfile, pfile + JAutogenProbe.PROFILE_BAK)
            jf.EnumerateFuncbyFileNameInPlatform(pfile)
            if type(pltDict[pfile]) is list:
                self.profile.ppltDict[pfile] = []
                for f in pltDict[pfile]:
                    floc = jf.GetFuncLocationByFileName(pfile, f)
                    self.profile.ppltDict[pfile].append({f + '_0':floc})
            else:
                floc = jf.GetFuncLocationByFileName(pfile, pltDict[pfile])
                self.profile.ppltDict[pfile] = [{pltDict[pfile] + '_0':floc}]
        self.pltbuild = pltbuild
        return vl


    ##########################################################################
    # function name: EvokeGoldenProfiling
    # Description:   Add profiling to step functions or features, and generate
    #                OJ_profile.c/.h which includes the implementation of
    #                Profiling. It also support JProbe for profiling.
    # Argument:      profiling_type:      Two types of profiling, one is JProbe
    #                                     profiling running based on emulator, the
    #                                     other is Runtime profiling.
    #                                     1: Runtime Profiling
    #                                     2: JPobe Profiling
    #                local_file:          profiling data saved to local file
    # Return:        Dictionary:
    #                Empty:           error
    ##########################################################################
    def EvokeGoldenProfiling(self, profiling_type=1, callrate_filepath=None, local_file=None):
        vl = False
        vlDict = {}
        # Re-Orgnize "pstepDict" and "pfeaDict" since some features are in same .c
        # file. Or even features could be in the same .c file as step functions. That
        # means the profiling can be done for any functions in any C files.
        dictstack = []
        for k, v in self.profile.pfeaDict.items():
            filename = k[:k.find('#')]
            if filename[:-2] == self.profilmodelname:
                for subk, subv in v.items():
                    self.profile.pstepDict[subk] = subv
                dictstack.append(k)
        # Delete key-value from feature Dictionary
        for item in dictstack:
            del(self.profile.pfeaDict[item])
        # Add profiling init and step functions
        if self.profile.AddProfilingInitStepFunc(self.filepath, self.profilmodelname, profiling_type) == True:
            # Add profiling function to other features
            if self.profile.AddProfilingFeaFunc(self.filepath, profiling_type) == True:
                # Add profiling function to platform
                self.profile.AddProfilingPltFunc(None, profiling_type, self.pltbuild)
                if profiling_type == JAutogenProbe.PROFILE_RUNTIME:
                    # Generate OJ_profile.c/.h files
                    callrate_path = callrate_filepath if callrate_filepath != None else os.getcwd()
                    vl = self.profile.GenerateProfile(self.filepath, self.profilmodelname, self.dsparch, self.callrate[self.profilmodelname], callrate_path, local_file)
                    # Generate TOP command
                    self.profile.GenerateTOPCmd(self.filepath, self.profilmodelname)
                else:
                    vl = True
                    JAlog_print(JAG_GENERAL, 'Use JProbe for JTest, OJ_profile.c/.h not generated.')
        if vl == True:
            for k in self.profile.pstepDict.keys():
                vlDict[k] = [str(self.callrate[self.profilmodelname][k[-1]]), int(k[-1])]
        return vlDict

    ##########################################################################
    # function name: SilenceGoldenProfiling
    # Description:   Remove Profiling from C files. Delete, OJ_Profile.c/.h,
    #                Modify all c files being profiled to the original
    # Argument:      None:
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def SilenceGoldenProfiling(self):
        oj_c = os.path.join(self.filepath, PROFILE_C)
        oj_h = os.path.join(self.filepath, PROFILE_H)
        if os.path.exists(oj_c):
            os.remove(oj_c)
        if os.path.exists(oj_h):
            os.remove(oj_h)
        model_bak = os.path.join(self.filepath, self.profilmodelname + '.c' + JAutogenProbe.PROFILE_BAK)
        if os.path.exists(model_bak):
            shutil.copyfile(model_bak, os.path.join(self.filepath, self.profilmodelname + '.c'))
            os.remove(model_bak)
        for item in self.profileList:
            fea_bak = os.path.join(self.filepath, item + JAutogenProbe.PROFILE_BAK)
            if os.path.exists(fea_bak):
                shutil.copyfile(fea_bak, os.path.join(self.filepath, item))
                os.remove(fea_bak)
        return True


    ##########################################################################
    # function name: EvokeGeneralPragma
    # Description:   It adds #pragma for all functions, and generate header files
    #                to include all pragma sections, and group them.
    # Argument:      matlab_model_name:    Model name, like Model_1_1
    #                path_bankheader:      the folder path of memory bank header file
    #                group_type:           How to group all pragma sections
    #                                      PRAGMA_GROUP_GENERL: group all sections in one header file
    #                                      PRAGMA_GROUP_CFILE:  group sections based on .c file
    # Return:        True:      success
    #                False:     error
    ##########################################################################
    def EvokeGeneralPragma(self, matlab_model_name=None, path_bankheader=None, group_type=PRAGMA_GROUP_GENERAL):
        model_funcs = JFuncSkill(matlab_model_name, self.filepath)
        model_funcs.EnumerateFuncbyFileName('*')
        # Format: {filename1.c: [sectionname1, sectionname2, ...], filename2.c: [sectionname1, ...]}
        pragmaDict = {}
        jdm = JAutogenCode()
        #for ff in model_funcs.filefuncList:
        for k, v in model_funcs.filefuncDict.items():
            # functions exist in c file
            if bool(v):
                pragmaDict[k] = []
                fpath = os.path.join(self.filepath, k)
                with open(fpath, 'r') as filehandle:
                    num_of_lines = filehandle.readlines()
                os.remove(fpath)
                with open(fpath, 'w+') as filehandle:
                    start = 0
                    for item in v:
                        ol = int(item[item.find('#') + 1:])
                        filehandle.writelines(num_of_lines[start : ol])
                        secstr = self.conv.sectionname_codepragma(item[:item.find('#')])
                        if '#pragma section(' not in num_of_lines[ol - 1]:
                            ss = '#pragma section(' + '"' + secstr + '"' + ')\n'
                            filehandle.write(ss)
                        start = ol
                        pragmaDict[k].append(secstr)
                    filehandle.writelines(num_of_lines[start:])
        # Generate Bank Header file based on group_type
        if group_type == JAutogenProbe.PRAGMA_GROUP_GENERAL:
            # Add all sections in one header file, not grouped
            generalList = []
            for l in pragmaDict.values():
                generalList += l
            jdm.GenerageBankMappingHeader(self.conv.filename_bankmappingheader(matlab_model_name), path_bankheader, {matlab_model_name : generalList})
        elif group_type == JAutogenProbe.PRAGMA_GROUP_CFILE:
            # Add sections grouped, based on c file
            for k, v in pragmaDict.items():
                jdm.GenerageBankMappingHeader(self.conv.filename_bankmappingheader(k[:-2]), path_bankheader, {k[:-2] : v})
        elif group_type == JAutogenProbe.PRAGMA_GROUP_STEP:
            step_filename = matlab_model_name + '.c'
            step_funckw = matlab_model_name + '_step'
            if step_filename in model_funcs.filefuncDict:
                stepL = [x[:x.find('#')] for x in model_funcs.filefuncDict[step_filename] if re.match('^' + step_funckw, x)]
                # if not single rate, remove the top step function.
                if len(stepL) > 1:
                    stepL.remove(step_funckw)
                stackflow = []
                for step in stepL:
                    calledL = []
                    model_funcs.FindSubFuncsByFuncName(step, calledL, stackflow)
                    jdm.GenerageBankMappingHeader(self.conv.filename_bankmappingheader(step), path_bankheader, {step : [self.conv.sectionname_codepragma(x) for x in calledL]})
            else:
                JAlog_print(JAG_ERROR, 'The C file: ' + step_filename + ' cannot be found.')






'''
#################################################################################################
# Unit Test for JAutogenProbe Profiling
#################################################################################################

#################################################################################################
# Test Case 1 for Profiling
#################################################################################################
ja = JAutogenProbe(os.path.join(os.getcwd(), 'testcases\melville\Model_1_1_ert_shrlib_rtw'))
ja.PrepareForProfiling('Model_1_1', os.path.join(os.getcwd(), 'testcases\Melville\Model_SyncApi.xml'))
#print(str(ja.callrate))
ja.AddProfilingInitNStep(True)
#print(str(ja.profile.pstepDict))
ja.AddProfilingFeature('User_SRCOut.c', 'SRCOut', 'User_SRCOut')
#print(str(ja.profile.pfeaDict))
ja.EvokeGoldenProfiling(PROFILING_RUNTIME)

#ja.SilenceGoldenProfiling()



#################################################################################################
# Test Case 2 for Profiling
#################################################################################################
ja = JAutogenProbe(os.path.join(os.getcwd(), 'testcases\melville\Model_1_2_ert_shrlib_rtw'))
ja.PrepareForProfiling('Model_1_2', os.path.join(os.getcwd(), 'testcases\Melville\Model_SyncApi.xml'))
#print(str(ja.callrate))
ja.AddProfilingInitNStep(True)
ja.AddProfilingFeature('PASD.c', 'PASD', 'Model_1_2_PASD')
ja.AddProfilingFeature('PostProcess.c', 'PostProcess', 'Model_1_2_PostProcess')
ja.AddProfilingFeature('Announcements.c', 'Announcements', 'Model_1_2_Announcements')
ja.EvokeGoldenProfiling(PROFILING_RUNTIME)

#ja.SilenceGoldenProfiling()
'''




'''
#################################################################################################
# Unit Test for JAutogenProbe Pragma
#################################################################################################
ja = JAutogenProbe(os.path.join(os.getcwd(), 'testcases\melville\Model_1_1_ert_shrlib_rtw'))
ja.EvokeGeneralPragma('Model_1_1', os.getcwd(), JAutogenProbe.PRAGMA_GROUP_GENERAL)
'''

