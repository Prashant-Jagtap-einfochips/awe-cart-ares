import sys
import os
import shutil

from JFrameworkStru import *
# import Code generation for FeatureLib in C++
from JCppBuilder import *
# import Code generation for JFramework in C
from JCBuilder import *
# import Code generation for BTC in Python
from JPyBuilder import *
# import print log
from JLogPrint import *


class JAutogenCode:
    def __init__(self, appname=None, fullgenerate=True, genConfigFile='', genConfig = None, printLevel=2):
        self.genflag = fullgenerate
        self.appname = appname
        self.printLevel = printLevel
        self.genConfigFile = genConfig
        Jlog_init(self.printLevel)

    def GenerateFeatureLibinC(self, appobj=None, modelname=None, gen_path='./FLautogen/', rtc=None, top=None):
        #Folder path
        ROOT_PATH = gen_path if gen_path[-1] == '/' else gen_path + '/'
        SETUP_PATH = ROOT_PATH + 'SetupAPI/'
        BAMPROXY_PATH = ROOT_PATH + 'Bamproxys/'
        FEATURE_PATH = ROOT_PATH + 'Features/'
        if (appobj):
            # Create Folders
            pathlist = [ROOT_PATH, SETUP_PATH, BAMPROXY_PATH, FEATURE_PATH]
            for subf in pathlist:
                if not os.path.exists(subf):
                    os.makedirs(subf, exist_ok=True)
                    os.makedirs(subf + 'inc/', exist_ok=True)
                    os.makedirs(subf + 'src/', exist_ok=True)
            # Copy template .h and .c code to specified folder
            srcpath = os.getcwd() + '/template/'
            shutil.copyfile(srcpath + 'BamProxyShadow.h', BAMPROXY_PATH + 'inc/' + 'BamProxyShadow.h')
            shutil.copyfile(srcpath + 'BamProxyShadow.c', BAMPROXY_PATH + 'src/' + 'BamProxyShadow.c')
            # Count the file generated for each BamProxy. If it exists, no need to generate it again
            bpList = []
            # Count the number of each feature based on feature type {feature type: count}
            featureDict = {}
            # Match feature name with feature type {feature type: [feature name 1, feature name 2, ...]}
            featurenameDict = {}
            # Record the implemented flag for each feature {feature type: [True, True, False, ...]}
            featureimplDict = {}
            for k, v in appobj.items():
                if isinstance(v, jobj):
                    # Check and count Feature type
                    if v.__dict__['type'] is not None:
                        featuretype = v.__dict__['type']
                        featuretype = v.__dict__['type'][0] if type(featuretype) is tuple else featuretype
                        if featuretype in featureDict:
                            featureDict[featuretype] += 1
                            featurenameDict[featuretype].append(k)
                        else:
                            featureDict[featuretype] = 1
                            featurenameDict[featuretype] = [k]
                        featureimplDict[k] = v.__dict__['implemented']
            for fea, fcnt in featureDict.items():
                bamDict = {}
                bamnameDict = {}
                bamimplDict = {}
                feaobj = appobj[featurenameDict[fea][0]]
                # Check and count BamProxy type
                for k, v in feaobj.data.items():
                    if isinstance(v, jobj):
                        if v.__dict__['type'] is not None:
                            bamtype = v.__dict__['type']
                            if bamtype in bamDict:
                                bamDict[bamtype] += 1
                                bamnameDict[bamtype].append(k)
                            else:
                                bamDict[bamtype] = 1
                                bamnameDict[bamtype] = [k]
                            bamimplDict[k] = v.__dict__['implemented']
                    elif k == 'RTC' or k == 'TOP':
                        # Feature may include 'RTC' or 'TOP'
                        bamDict[fea] = 1
                        bamnameDict[fea] = [fea]
                for bam, bcnt in bamDict.items():
                    if (bam not in bpList):
                        bpList.append(bam)
                        # Generate BamProxy .h and .c for each Bamproxy
                        bamobj = feaobj.data[bamnameDict[bam][0]] if bam != fea else feaobj
                        BAMc = JBamproxyBuilder(BAMPROXY_PATH, self.genflag, bam, fea, self.appname, self.genConfigFile)
                        BAMc.GenerateCode(bamobj, feaobj)
                # Generate SetupAPI .h and .c for each Feature
                SETUPc = JSetupAPIBuilder(SETUP_PATH, fea, fcnt, featureimplDict, bamimplDict, self.genConfigFile)
                SETUPc.GenerateCode(feaobj, featurenameDict, bamDict, bamnameDict)
                # Generate Feature .h and .c file for each Feature
                FEAc = JFeatureBuilder(FEATURE_PATH, self.genflag, fea, fcnt, self.genConfigFile)
                FEAc.GenerateCode(feaobj, featurenameDict, bamDict, bamnameDict)
            # Generate FeatureLib .h and .c file
            FLc = JFLBuilder(ROOT_PATH, self.genflag, self.genConfigFile)
            FLc.GenerateCode(featureDict)

    def GenerateFeatureLibinCpp(self, appobj=None, modelname=None, gen_path='./autogen/',rtc=None, top=None):
        rtc_count = 0
        top_count = 0
        total_rtctop = [0, 0]
        # Enumerate Class Variables
        ROOT_FEATURELIB = gen_path+'/'
        ROOT_BAMPROXY = ROOT_FEATURELIB + 'Bam_Proxys'
        ROOT_FEATURE = ROOT_FEATURELIB + 'Features'
        ROOT_PAL = ROOT_FEATURELIB + 'PAL'
        if (appobj):
            # Counting all RTC and TOP for each instance
            self.CountRTCTOP(appobj, total_rtctop)
            # Create folder if it doesn't exist
            if not os.path.exists(gen_path):
                os.makedirs(gen_path, exist_ok=True)
            # Check the existance of PAL folder
            #if not os.path.exists(ROOT_PAL):
                #os.mkdir(ROOT_PAL)
                #srcpath = os.getcwd() + '/PAL/'
                #shutil.copyfile(srcpath + 'OJPAL.h', ROOT_FEATURELIB + 'PAL/' + 'OJPAL.h')
                #shutil.copyfile(srcpath + 'OJPAL.cpp', ROOT_FEATURELIB + 'PAL/' + 'OJPAL.c')
            # Generate OJBamProxy Base Class
            ojbp = JCppBuilder(None, None, ROOT_BAMPROXY, BAMPROXYBASE, self.genConfigFile)
            ojbp.Cpp_GenerateHeaderBamProxyBase()
            ojbp.Cpp_GenerateCppBamProxyBase()
            # Generate OJFeature Base Class
            #ojfea = JCppBuilder(None, None, ROOT_FEATURE, FEATUREBASE, self.genConfigFile)
            #ojfea.Cpp_GenerateHeaderFeatureBase()
            #ojfea.Cpp_GenerateCppFeatureBase()
            # Count the file generated for each BamProxy. If it exists, no need to generate it again
            bpList = []
            # Count the number of each feature based on feature type {feature type: count}
            featureDict = {}
            # Match feature name with feature type {feature type: [feature name 1, feature name 2, ...]}
            featurenameDict = {}
            # Record the implemented flag for each feature {feature type: [True, True, False, ...]}
            featureimplDict = {}
            # All feature names in list
            featurenameList = []
            # Store The App name
            appname = modelname
            for k, v in appobj.items():
                if isinstance(v, jobj):
                    # Check and count Feature type
                    if v.__dict__['type'] is not None:
                        featuretype = v.__dict__['type']
                        featuretype = v.__dict__['type'][0] if type(featuretype) is tuple else featuretype
                        if featuretype in featureDict:
                            featureDict[featuretype] += 1
                            featurenameDict[featuretype].append(k)
                        else:
                            featureDict[featuretype] = 1
                            featurenameDict[featuretype] = [k]
                        featureimplDict[k] = v.__dict__['implemented']
                        featurenameList.append(k)
            # Generate FeatureLib
            FLcpp = JCppBuilder(None, None, ROOT_FEATURELIB, FEATURELIB, self.genConfigFile)
            FLcpp.Cpp_SetClassname('FeatureLib', appname, rtc=rtc, top=top)
            FLcpp.Cpp_SetTopname(appname)
            for fea, fcnt in featureDict.items():
                bamDict = {}
                bamnameDict = {}
                bamimplDict = {}
                featoprtcDict = {}
                feaobj = appobj[featurenameDict[fea][0]]
                # Check and count BamProxy type
                for k, v in feaobj.data.items():
                    if isinstance(v, jobj):
                        if v.__dict__['type'] is not None:
                            bamtype = v.__dict__['type']
                            if bamtype in bamDict:
                                bamDict[bamtype] += 1
                                bamnameDict[bamtype].append(k)
                            else:
                                bamDict[bamtype] = 1
                                bamnameDict[bamtype] = [k]
                            bamimplDict[k] = v.__dict__['implemented']
                    elif k == 'RTC' or k == 'TOP':
                        # Feature may include 'RTC' or 'TOP'
                        #tmp = featurenameDict[fea][0]
                        bamDict[fea] = 1
                        bamnameDict[fea] = [fea]
                        bamimplDict[fea] = True
                        featoprtcDict[fea] = featurenameDict[fea]
                # Generate Feature
                FEAcpp = JCppBuilder(fea, fea, ROOT_FEATURE, FEATUREMODULE, self.genConfigFile)
                FEAcpp.Cpp_SetClassname(fea, appname, rtc=rtc, top=top)
                FEAcpp.Cpp_SetTopname(appname)
                FEAcpp.Cpp_AddSchemaBamFeature(bamnameDict, featurenameDict[fea], featoprtcDict)
                FEAcpp.Cpp_AddSchemaBamFeatureImpl(bamimplDict, featureimplDict)
                for bam, bcnt in bamDict.items():
                    # Create Enumeration for BamObj, ActionID, Channel_In and Channel_Out
                    FEAcpp.Cpp_AddBamActionChannel(bam, fea, bamnameDict, featurenameDict, feaobj, appobj)
                    if (bam not in bpList):
                        bpList.append(bam)
                        # Generate BamProxy
                        BAMcpp = JCppBuilder(bam, bam, ROOT_BAMPROXY, BAMMODULE, self.genConfigFile)
                        BAMcpp.Cpp_SetClassname(bam, appname, rtc=rtc, top=top)
                        BAMcpp.Cpp_SetTopname(bam)
                        BAMcpp.fnList = featurenameList
                        bamobj = feaobj.data[bamnameDict[bam][0]] if bam != fea else feaobj
                        BAMcpp.Cpp_AddEnumActionVarFunc(bamobj, bamnameDict[bam][0], featoprtcDict)
                        BAMcpp.Cpp_GenerateHeadFile()
                        BAMcpp.Cpp_GenerateCppFile()
                        # Count number of RTC and TOP IDs in Total
                        rtc_count += BAMcpp.rtcCount
                        top_count += BAMcpp.topCount
                    # Add variables and functions to Feature
                    FEAcpp.Cpp_AddHeaderByName(bam)
                    FEAcpp.Cpp_AddPrivateVarByName(bam, bcnt)
                    FEAcpp.Cpp_AddPublicFuncByName(bam, bcnt)
                FEAcpp.Cpp_GenerateHeadFile()
                FEAcpp.Cpp_GenerateCppFile()
                # Add variables and functions to FeatureLib
                FLcpp.Cpp_AddHeaderByName(fea)
                FLcpp.Cpp_AddPrivateVarByName(fea, fcnt)
                FLcpp.Cpp_AddPublicFuncByName(fea, fcnt)
            FLcpp.rtcCount = total_rtctop[0]
            FLcpp.topCount = total_rtctop[1]
            FLcpp.Cpp_GenerateHeadFile()
            FLcpp.Cpp_GenerateCppFile()

    def CountRTCTOP(self, appobj=None, rtc_top=None):
        if appobj != None and rtc_top != None:
            for k, v in appobj.items():
                if isinstance(v, jobj):
                    self.CountRTCTOP(v.data, rtc_top)
                elif k == 'RTC':
                    rtc_top[0] += len(v)
                elif k == 'TOP':
                    rtc_top[1] += len(v)


    def GenerateControlAppPy(self, flobj=None, model_name=None, genflag=True, genpath='./FLCautogen', topenable=False):
        modelk = 'Model_xml'
        rtck = 'RTC_xml'
        topk = 'TOP_xml'
        if (flobj):
            gen_path = genpath if genpath != None else './'
            if not os.path.exists(gen_path) and gen_path != './':
                os.makedirs(gen_path)
            if hasattr(flobj, modelk) and hasattr(flobj, rtck) and hasattr(flobj, topk):
                flPy = JControlPyBuilder(gen_path, genflag, model_name, topenable, self.genConfigFile)
                if (flPy.GetXMLFile(flobj.Model_xml, flobj.RTC_xml, flobj.TOP_xml) == True):
                    genFlag = flPy.GenerateCode(flobj)
                else:
                    #print('The XML file cannot be found')
                    Jlog_print(JAG_ERROR, 'The XML file cannot be found')
            else:
                #print('Cannot generate Python for FeatureLib, due to the lack of XML files')
                Jlog_print(JAG_ERROR, 'Cannot generate Python for FeatureLib, due to the lack of XML files')


    def GenerateJFrameworkEx(self, bafobj = None, genpath = None):
        if (bafobj):
            for name, jframe in bafobj.items():
                if isinstance(jframe, jobj) and (jframe.__dict__['type'] == 'JFramework'):
                    self.genConfigFile = jframe.genConfig if hasattr(jframe, 'genConfig') else self.genConfigFile
                    # Generate codes for each JFramework
                    gen_path = genpath if genpath != None else './'
                    if not os.path.exists(gen_path) and gen_path != './':
                        os.makedirs(gen_path, exist_ok=True)
                    JFc = JFrameworkBuilder(gen_path, name, jframe, self.genConfigFile)
                    JFc.GenerateCode()

    def GetJFramework(self, bafobj = None, name = 'Baf'):
        if (bafobj):
            for _name, jframe in bafobj.items():
                if isinstance(jframe, jobj) and (jframe.__dict__['type'] == 'JFramework'):
                    if name == _name:
                        return jframe
        return None

    def GenerateJFrameworkExSingle(self, bafobj, name = 'Baf', genpath = None):
        jframe = self.GetJFramework(bafobj, name)
        if jframe != None:
            gen_path = genpath if genpath != None else './'
            if not os.path.exists(gen_path) and gen_path != './':
                os.makedirs(gen_path, exist_ok=True)
            self.genConfigFile = jframe.genConfig if hasattr(jframe, 'genConfig') else self.genConfigFile
            JFc = JFrameworkBuilder(gen_path, name, jframe, self.genConfigFile)
            JFc.GenerateCode()
            return
        #print ('Error: failed to generate JFramework for name %s' % name)
        Jlog_print(JAG_ERROR, 'Error: failed to generate JFramework for name %s' % name)

    def GenerateSconstructPy(self, app_root=None, filename=None, filepath=None):
        sconPy = JSconsPyBuilder()
        sconPy.GenerateSconstructPy(app_root, filename, filepath)

    def GenerateJConfigPy(self, genpath=None, dsp_arch='hexagon', platform='8155', sim_type='PIL', sim_mode='onTarget', cfgDict=None):
        sconsPy = JSconsPyBuilder()
        sconsPy.GenerateJConfigPy(genpath, dsp_arch, platform, sim_type, sim_mode, cfgDict)

    def UpdateJConfigPy(self, genpath=None, num_dsp=1):
        sconsPy = JSconsPyBuilder()
        sconsPy.UpdateJConfigPy(genpath, num_dsp)

    def GenerateJConfigAppPy(self, genpath=None, model_name=None, num_dsp=1, iportDict=None, oportDict=None):
        sconsPy = JSconsPyBuilder()
        sconsPy.GenerateJConfigAppPy(genpath, model_name, num_dsp, iportDict, oportDict)

    def UpdateJConfigAppPy(self, genpath=None, num_dsp=1, iportDict=None, oportDict=None):
        sconsPy = JSconsPyBuilder()
        sconsPy.UpdateJConfigAppPy(genpath, num_dsp, iportDict, oportDict)

    def GenerateCaPIv2(self, demeterpath=None, genpath=None, gensimpath=None, model_name=None, capiv2Dict=None):
        # Here we check the existence of parent folder, and shutil.copytree creates the destination folder.
        if os.path.exists(os.path.dirname(genpath)) and os.path.exists(os.path.dirname(gensimpath)) and os.path.exists(demeterpath):
            # Generate CaPIv2 for Hardware profiling
            shutil.copytree(os.path.join(demeterpath, 'template/capi_v2_baf_template'), genpath)
            capi = JCaPIv2Builder(genpath, model_name)
            capi.GenerateCode(capiv2Dict)
            # Generate CaPIv2 for Soft Simulation
            shutil.copytree(os.path.join(demeterpath, 'template/capi_v2_baf_sim_template'), gensimpath)
            capi = JCaPIv2Builder(gensimpath, model_name)
            capi.GenerateCode(capiv2Dict)
        else:
            Jlog_print(JAG_ERROR, 'The Path ' + genpath + ' or ' + gensimpath + ' does not exist.')

    def UpdateCaPIv2Headers(self, headerpath=None, genpath=None, gensimpath=None):
        if os.path.exists(headerpath):
            if os.path.exists(genpath):
                dstpath = os.path.join(genpath, 'inc')
                shutil.copy(headerpath, dstpath)
            else:
                Jlog_print(JAG_ERROR, 'The CaPIv2 Path ' + genpath + ' does not exist.')
            if os.path.exists(gensimpath):
                dstpath = os.path.join(gensimpath, 'inc')
                shutil.copy(headerpath, dstpath)
            else:
                Jlog_print(JAG_ERROR, 'The CaPIv2 Sim Path ' + gensimpath + ' does not exist.')
        else:
            Jlog_print(JAG_ERROR, 'The Header file Path ' + headerpath + ' does not exist.')

    def UpdateCaPIv2BAF(self, bafpath=None, genpath=None, gensimpath=None):
        if os.path.exists(bafpath):
            if os.path.exists(genpath):
                dstpath = os.path.join(genpath, 'BAF/baf.a')
                shutil.copy(bafpath, dstpath)
            else:
                Jlog_print(JAG_ERROR, 'The CaPIv2 Path ' + genpath + ' does not exist.')
            if os.path.exists(gensimpath):
                dstpath = os.path.join(gensimpath, 'BAF/baf.a')
                shutil.copy(bafpath, dstpath)
            else:
                Jlog_print(JAG_ERROR, 'The CaPIv2 Sim Path ' + gensimpath + ' does not exist.')
        else:
            Jlog_print(JAG_ERROR, 'The BAF Library Path ' + bafpath + ' does not exist.')

    def AddProbetoJModule(self, jmodule_path, modelname, mbuf_fs, mbuf_ch, funcprototype, func_str):
        shutil.copyfile(jmodule_path, jmodule_path + '.tmp')
        os.remove(jmodule_path)
        with open(jmodule_path + '.tmp', 'r') as filehandle:
            num_of_lines = filehandle.readlines()
        if len(num_of_lines) > 1:
            probjml = JProbeJModuleBuilder(jmodule_path, modelname)
            probjml.modelFs = mbuf_fs
            probjml.modelCh = mbuf_ch
            probjml.AddSingleProbetoJModule(num_of_lines, funcprototype, func_str)
            os.remove(jmodule_path + '.tmp')
        else:
            os.rename(jmodule_path + '.tmp', jmodule_path)
            Jlog_print(JAG_ERROR, 'The JModule_Adapt_Sys.c is empty.....')

    def GenerageBankMappingHeader(self, filename, path_bankheader, banksectionDict):
        jc = JCBuilder(filename, None, path_bankheader)
        jc.H_GenerateFile1(None, banksectionDict, None, None)


