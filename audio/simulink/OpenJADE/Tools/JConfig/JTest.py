from runpy import run_path
import sys
import os
import copy
import glob

# import all JFramework/JModule objects
from JFrameworkStru import *
# import all JConfig Template
from JConfigTemplate import *
from JAutogen import *

import OJTSI.TimeObjOJT

JConfig = []
JConfig_numItems = 0

#=========================
def JConfig_AddBuild(jconfig_seed, arch, platform=None, test=None, runner=None) :
    global JConfig
    global JConfig_numItems
    JConfig.append(jconfig_seed)
    JConfig[JConfig_numItems] = copy.deepcopy( jconfig_seed )
    JConfig[JConfig_numItems]['Option']['arch'] = arch
    if platform : JConfig[JConfig_numItems]['Option']['platform'] = platform
    if test : JConfig[JConfig_numItems]['JFramework'][0]['JModule'][0]['TalariaModel']['TestCase'] = test
    if runner : JConfig[JConfig_numItems]['Option']['runner'] = runner
    JConfig_numItems = JConfig_numItems + 1
    return JConfig[JConfig_numItems-1]

def JConfig_Add(jconfig_seed, name=None, info=None) :
    global JConfig
    global JConfig_numItems
    JConfig.append(jconfig_seed)
    JConfig[JConfig_numItems] = copy.deepcopy(jconfig_seed)
    jconfig = JConfig[JConfig_numItems]
    if name: jconfig['Name'] = name
    if info: jconfig['Info'] = info
    JConfig_numItems = JConfig_numItems + 1
    return jconfig

def JConfig_GetGenerator(jconfig):
    if 'Generator' in jconfig:
        return jconfig['Generator']
    else:
        return None

def JConfig_SetBcf(jconfig, name=None):
    jconfig['BCF']['Name'] = name

def JConfig_SetGenerator(jconfig, generator=None, app_config_root=None, arg=None, autogen_config=None):
    if (app_config_root):
        app_config_script = os.path.join(app_config_root, 'JConfigApp.py')
        if os.path.exists(app_config_script):
            app_config_globals = run_path(app_config_script, init_globals=arg)
            generator = app_config_globals['app_config']
        else:
            print('Error: SetGenerator JConfigApp %s script does not exist' % app_config_script)
            return
    else:
        if (generator==None):
            print('Error: SetGenerator either generator or app_config_script parameter must be specified')
            return

    if (generator):
        generator=copy.deepcopy(generator)
        jconfig['Generator'] = generator

        variant_root = os.getenv('VARIANT_ROOT')
        if ('Name' in jconfig) and (jconfig['Name']):
            variant_root = variant_root + jconfig['Name'] + '/'

        if (generator.baf):
            for name, jframe in generator.baf.items():
                model_name = jframe.Jfmr_GetModelName()
                if isinstance(model_name, list):
                    for model in model_name:
                        partition_name = jframe.Jfmr_GetPartitionName(model)
                        talaria_gen_folder = variant_root+'/slx/code'
                        xml_syncapi_file = '%s/%s_SyncApi.xml' % (talaria_gen_folder, model)
                        xml_syncapi = jframe.Jfmr_GetSyncAPIXML(model)
                        if xml_syncapi is not None and (xml_syncapi == '') and os.path.exists(xml_syncapi_file):
                            jframe.Jfmr_SetSyncAPIXML(xml_syncapi_file, model)
                        partition_name = '%s_Target' % (model)
                        jframe.Jfmr_SetPartitionName(partition_name, model)
                else:
                    if model_name == None:
                        print('Error: JConfig_SetGenerator - model_name could not be found')
                        return

                    partition_name = jframe.Jfmr_GetPartitionName()
                    if partition_name == None:
                        print('Error: JConfig_SetGenerator - partition_name could not be found')
                        return

                    talaria_gen_folder = variant_root+'/slx/code'
                    xml_syncapi_file = '%s/%s_SyncApi.xml' % (talaria_gen_folder, model_name)
                    xml_pingpong_file = '%s/%s_ert_shrlib_rtw/%s_PingPongStruct.xml' % (talaria_gen_folder, partition_name, partition_name)

                    xml_syncapi = jframe.Jfmr_GetSyncAPIXML()
                    if xml_syncapi != None:
                        if (xml_syncapi == '') and os.path.exists(xml_syncapi_file):
                            jframe.Jfmr_SetSyncAPIXML(xml_syncapi_file)

                    xml_pingpong = jframe.Jfmr_GetPingPongXML()
                    if xml_pingpong != None:
                        if (xml_pingpong == '') and os.path.exists(xml_pingpong_file):
                            jframe.Jfmr_SetPingPongXML(xml_pingpong_file)

        if autogen_config:
            JConfig_AutogenConfig(jconfig, path=autogen_config)
            jdm = JAutogenCode(generator.type, False, genConfig=jconfig, printLevel=2)
        else:
            jdm = JAutogenCode(generator.type, False, printLevel=2)
        if (generator.baf):
            variant_root = os.getenv('VARIANT_ROOT')
            if ('Name' in jconfig) and (jconfig['Name']):
                tmp_path = variant_root + 'tmp/' + jconfig['Name']
            else:
                tmp_path = variant_root + 'tmp/'
            if not os.path.exists(tmp_path):
                os.makedirs(tmp_path)
            try:
                jdm.GenerateJFrameworkEx(generator.baf, tmp_path) # framework autogen workaround
            except OSError as error:
                print(f'Error: exception during GenerateJFrameworkEx {error}')
            except:
                pass
        if ('BCF' in jconfig) and (jconfig['BCF']):
            JConfig_BcfParam(jconfig, param=generator.configFlat, generator=generator)
        talaria_param_v2 = {
            'Version':2,
            'MatlabStruct':generator.configMatlabStruct,
            'MatlabClassInit':generator.classInit,
            'MatlabFlat':generator.configFlat,
            'Graph':generator.configGraph
        }

        if (generator.baf):
            for name, jframe in generator.baf.items():
                if isinstance(jframe, jobj) and (jframe.__dict__['type'] == 'JFramework'):
                    core = jframe.Jfmr_GetID()

                    socId = jframe.Jfmr_GetSOCId()
                    coreId = jframe.Jfmr_GetCoreId()
                    call_rates = jframe.Jfmr_GetCallRates(idx=0)
                    if call_rates != None:
                        num_rates = len(call_rates)
                        base_thread_level = 0
                        if num_rates > 1:
                            for thread_level, proc_info in call_rates.items():
                                if proc_info['base_task'] == True:
                                    base_thread_level = thread_level
                            JConfig_MultiRatePlatform(jconfig, boolOption=True, baseThread=base_thread_level, core=core)
                            JConfig_SetExtScheduler(jconfig, boolOption=True, core=core)
                    if (socId != None) and (coreId != None):
                        JConfig_JFrameworkId (jconfig, socId=socId, coreId=coreId, core=core)
                    call_interval = jframe.Jfmr_GetCallInterval(idx=0)
                    if call_interval != None:
                        jframework=jconfig['JFramework'][core]
                        if 'Option' not in jframework: jframework['Option'] = {}
                        if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                        jframework['Option']['Application']['CallInterval'] = int(call_interval)

                    if isinstance(jconfig['Option']['arch'], dict):
                        arch = None
                        for k, v in jconfig['Option']['arch'].items():
                            if k == name:
                                arch = v
                        if arch == None:
                            print(f'Error: JConfig_SetGenerator - could not determine arch for JFramework {name}')
                            return
                    else:
                        arch = jconfig['Option']['arch']

                    JConfig_JFramework(jconfig, arch=arch, path=name, name=name, core=core, generator=generator, crosscore=jframe.Jfmr_IsCrosscore())
                    jmodule_name_list = []
                    num_of_jmodule = jframe.Jfmr_GetNumOfjmodule(jmodule_name_list)
                    if num_of_jmodule == 0: # framework autogen workaround
                        if isinstance(model_name, list):
                          for id, model in enumerate (model_name):
                                if jframe.Jfmr_TalariaParamVersion() == 2:
                                    JConfig_TalariaParam(jconfig, param=talaria_param_v2, id=id)
                                jmod_path = 'JModule_Lib/'+ model
                                JConfig_JModule(jconfig, path=jmod_path, name=model, id=id)
                        else:
                            if jframe.Jfmr_TalariaParamVersion() == 2:
                                JConfig_TalariaParam(jconfig, param=talaria_param_v2, id=0)
                            jmod_path = 'JModule_Lib/'+ name
                            JConfig_JModule(jconfig, path=jmod_path, name=name, id=0, core=core)
                    else:
                        for jmod_name in jmodule_name_list:
                            if jmod_name.upper().startswith('SYS'):
                                continue
                            else:
                                jmod = jframe.Jfmr_Getjmodule(jmod_name)
                                jmod_id = jmod.Jmdl_GetID()
                                if jframe.Jfmr_TalariaParamVersion() == 2:
                                    if jmod_name in generator.data:
                                        talaria_param_v2['MatlabStruct'] = generator.data[jmod_name].configMatlabStruct
                                        talaria_param_v2['MatlabClassInit'] = generator.data[jmod_name].classInit
                                    JConfig_TalariaParam(jconfig, param=talaria_param_v2, id=jmod_id)
                                jmod_path = 'JModule_Lib/'+ jmod_name
                                JConfig_JModule(jconfig, path=jmod_path, name=jmod_name, id=jmod_id, core=core)
                                talaria_model = jconfig['JFramework'][core]['JModule'][jmod_id]['TalariaModel']
                                if jframe.Jfmr_IsCrosscore():
                                    if 'exclude' in talaria_model:
                                        del talaria_model['exclude']
                                else:
                                    talaria_model['exclude'] = None
                        JConfig_MIPSReport(jconfig, id=0, core=core)
                        JConfig_Logger(jconfig, boolOption=True, core=core)
        else:
            print ('Error: generator does not have baf frameworks')

def JConfig_JModule (jconfig, path=None, name=None, id=0, core=None):
    if core == None:
        for jframework in jconfig['JFramework']:
            if path: jframework['JModule'][id]['Path'] = path
            if name: jframework['JModule'][id]['Name'] = name
    else:
        if path: jconfig['JFramework'][core]['JModule'][id]['Path'] = path
        if name: jconfig['JFramework'][core]['JModule'][id]['Name'] = name

def JConfig_JAutogenProbe (jconfig, config_path=None, name=None):
    with open(config_path+'/'+name, 'r') as fin:
        profile_cfg = json.load(fin)
    for jframework in jconfig['JFramework']:
        if config_path: jframework['JAutogenProbe'] = profile_cfg

def JConfig_JFramework (jconfig, arch=None, path=None, name=None, soc=None, core=0, generator=None, crosscore=False):
    jconfig['JFramework'][core]['arch'] = arch
    jconfig['JFramework'][core]['Path'] = path
    if soc is not None:
        jconfig['JFramework'][core]['SOC_ID'] = str(soc + 1)
        jconfig['JFramework'][core]['CORE_ID'] = str(core + 1)
        jconfig['JFramework'][core]['JModule'][0]['TalariaModel']['SysModel'] = True
    if name : jconfig['JFramework'][core]['Name'] = name
    if generator : jconfig['JFramework'][core]['Generator'] = generator
    jconfig['JFramework'][core]['CrossCore'] = crosscore

def JConfig_JFrameworkId (jconfig, socId=0, coreId=0, core=0):
    jconfig['JFramework'][core]['SOC_ID'] = str(socId)
    jconfig['JFramework'][core]['CORE_ID'] = str(coreId)

def JConfig_MiscRoot(jconfig, misc_root=None, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['misc_root'] = '#/'+ misc_root

            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['driversInitPath'] = '#/'+ misc_root + '/drivers'
    else:
        jframework = jconfig['JFramework'][core]
        jframework['JModule'][id]['TalariaModel']['misc_root'] = '#/'+ misc_root

        if 'Option' not in jframework: jframework['Option'] = {}
        if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
        jframework['Option']['Platform']['driversInitPath'] = '#/'+ misc_root + '/drivers'

def JConfig_TalariaModel(jconfig, path, model, feature=None, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['Name'] = model
            jframework['JModule'][id]['TalariaModel']['CtrlGenName'] = model.split('_')[0]
            jframework['JModule'][id]['TalariaModel']['Path'] = path
            #if (feature) :
            #    jframework['JModule'][id]['TalariaModel']['codegen'][0] = '_'+feature+'0_TOP.c'

    else:
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Name'] = model
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['CtrlGenName'] = model.split('_')[0]
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Path'] = path
        #if (feature) :
        #    jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['codegen'][0] = '_'+feature+'0_TOP.c'

def JConfig_TalariaCodegen(jconfig, codegen, id=0, core=0, prefix='default'):
    jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['codegen'] = copy.deepcopy(codegen)
    if prefix == 'default':
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['prefix'] = prefix
    else:
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['prefix'] = None

def JConfig_TalariaPartitioning(jconfig, partitioning=None):
    if partitioning == None:
        jconfig['Partitioning'] = None
    else:
        jconfig['Partitioning'] = copy.deepcopy(partitioning)

def JConfig_TalariaCtrlgen(jconfig, ctrlgen, id=0, core=None):
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['ctrlgen'] = copy.deepcopy(ctrlgen)
    else:
        jframework = jconfig['JFramework'][core]
        jframework['JModule'][id]['TalariaModel']['ctrlgen'] = copy.deepcopy(ctrlgen)

def JConfig_TalariaCodeGenExclude(jconfig, exclude=None, id=0, core=None):
    if not isinstance(exclude, list):
        print('Error: TalariaCodeGenExclude exclude must be a list of files to exclude from the build')
        return
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['exclude'] = copy.deepcopy(exclude)
    else:
        jframework = jconfig['JFramework'][core]
        jframework['JModule'][id]['TalariaModel']['exclude'] = copy.deepcopy(exclude)

def JConfig_TalariaTest(jconfig, test, testcase, fwtest='default', param=None, tolerance=None, correction=None, skip=None, combinedBuildTest=False, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['Test']['Name'] = test
            jframework['JModule'][id]['TalariaModel']['Test']['Case'] = testcase
            jframework['JModule'][id]['TalariaModel']['Test']['combinedBuildTest'] = combinedBuildTest
            if param:
                jframework['JModule'][id]['TalariaModel']['Test']['Param'] = param
            if tolerance:
                jframework['JModule'][id]['TalariaModel']['Test']['Tolerance'] = tolerance
            if correction:
                jframework['JModule'][id]['TalariaModel']['Test']['Correction'] = correction
            if skip is not None:
                jframework['JModule'][id]['TalariaModel']['Test']['Skip'] = skip
            if fwtest == 'default':
                jframework['Test']['Name'] = test
                jframework['Test']['Case'] = testcase
            else:
                jframework['Test']= fwtest
            if param:
                jframework['Test']['Param'] = param
    else:
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Name'] = test
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Case'] = testcase
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['combinedBuildTest'] = combinedBuildTest
        if param:
            jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Param'] = param
        if tolerance:
            jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Tolerance'] = tolerance
        if correction:
            jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Correction'] = correction
        if fwtest == 'default':
            jconfig['JFramework'][core]['Test']['Name'] = test
            jconfig['JFramework'][core]['Test']['Case'] = testcase
        else:
            jconfig['JFramework'][core]['Test']= fwtest
        if param:
            jconfig['JFramework'][core]['Test']['Param'] = param

def JConfig_Platform(jconfig, arch, platform=None, runner=None, rtos=None, module=None, namespace=None ) :
    if arch is not None: jconfig['Option']['arch'] = arch
    if platform is not None: jconfig['Option']['platform'] = platform
    if runner is not None: jconfig['Option']['runner'] = runner
    if module is not None: jconfig['Option']['module'] = module
    if namespace is not None: jconfig['Option']['namespace'] = namespace

    if isinstance(jconfig['Option']['arch'], dict):
        jconfig['Option']['platform'] = None
        jconfig['Option']['runner'] = None
        jconfig['Option']['module'] = None
        jconfig['Option']['namespace'] = None

    if (platform) and (runner) and (platform in ['PIL','DPIL','SOMPIL','SC594PIL','SC598PIL']) and (runner=='ezkit'):
        JConfig_PIL_EZKitOnly(jconfig, True)
    if (platform):
        if (platform in ['HIL','DHIL','SOMHIL','SC594HIL','SC598HIL']):
            JConfig_FreeRunning(jconfig, False)
        elif (platform in ['RTA','DRTA','SOMRTA','SC594RTA','SC598RTA']):
            JConfig_FreeRunning(jconfig, True)

    jconfig['Option']['rtos'] = None
    if (rtos):
        if (rtos == 'FreeRTOS'):
            jconfig['Option']['rtos'] = 'FreeRTOS'
            for k, v in jconfig['Platform'].items():
                for ki, vi in v.items():
                    jconfig['Platform'][k][ki] = vi.replace('Platform','PlatformFreeRTOS')
            JConfig_MultiRatePlatform(jconfig, boolOption=True)

def JConfig_PGOOption(jconfig, pgo_option=None) :
    if pgo_option in ['genpgo', 'usepgo']:
        jconfig['Option']['pgo_option'] = pgo_option
    else:
        jconfig['Option']['pgo_option'] = None

def JConfig_PreloadFile(jconfig, preload_file=None, core=0) :
    if preload_file:
        if os.path.exists(preload_file):
            if 'preload_file' not in jconfig['Option']: jconfig['Option']['preload_file'] = {}
            jconfig['Option']['preload_file'][core] = preload_file
        else:
            print('Error: PreloadFile %s does not exist' % preload_file)
            return

def JConfig_UseFileListForLink(jconfig, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['link_option'] = 'file_list'
    else:
        jframework=jconfig['JFramework'][core]
        if 'Option' not in jframework: jframework['Option'] = {}
        if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
        jframework['Option']['Application']['link_option'] = 'file_list'

def JConfig_XtraBuildFlags(jconfig, arch, xcpp_flags=None, xlink_flags=None) :
    if isinstance(arch, dict):
        for k, _arch in arch.items():
            JConfig_XtraBuildFlagsHelper(jconfig, arch=_arch, xcpp_flags=xcpp_flagss[_arch], xlink_flags=xlink_flags[_arch])
    else:
        JConfig_XtraBuildFlagsHelper(jconfig, arch=arch, xcpp_flags=xcpp_flags, xlink_flags=xlink_flags)

def JConfig_XtraBuildFlagsHelper(jconfig, arch, xcpp_flags=None, xlink_flags=None) :
    if (xcpp_flags != None) or (xlink_flags != None):
        if 'xtra_flags' not in jconfig: jconfig['xtra_flags'] = {}
        jconfig['xtra_flags'][arch] = {'xcpp_flags':xcpp_flags, 'xlink_flags':xlink_flags}

def JConfig_CustomBuildFlags(jconfig, arch, build_options=None, core=None) :
    if isinstance(arch, dict):
        for k, _arch in arch.items():
            JConfig_CustomBuildFlagsHelper(jconfig, arch=_arch, build_options=build_options[_arch], core=core)
    else:
        JConfig_CustomBuildFlagsHelper(jconfig, arch=arch, build_options=build_options, core=core)

def JConfig_CustomBuildFlagsHelper(jconfig, arch, build_options=None, core=None) :
    if (build_options != None):
        build_options_usage = """
         build_options is a nested dictionary
         build_options = {
            'Platform'    : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list] },
            'CRL'         : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list] },
            'Drivers'     : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list] },
            'Runtime'     : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list] },
            'JFramework'  : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list] },
            'Application' : {'cpp_flags': [cpp_flags_list], 'xcpp_flags': [xtra_cpp_flags_list], 'link_flags': [link_flags_list] },
            'JModule' : {
                           0 : {'cpp_flags': [cpp_flags_list],  'xcpp_flags': [xtra_cpp_flags_list] },
                           ...
                           N : {'cpp_flags': [cpp_flags_list],  'xcpp_flags': [xtra_cpp_flags_list] }
                        }
         }
        cpp_flags specified will be added on top of 'Base' flags defined for the architecture
        xcpp_flags specified will be added on top of existing flags defined for the architecture
        link_flags specified will be added on top of existing flags defined for the architecture
        """
        if not isinstance(build_options, dict):
            print('Error: JConfig_CustomBuildFlags build_options usage %s' % build_options_usage)
            return

        for component, value in build_options.items():
            if component == 'Application':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        if 'Option' not in jframework: jframework['Option'] = {}
                        if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                        if 'BuildOptions' not in jframework['Option']['Application']: jframework['Option']['Application']['BuildOptions'] = {}
                        jframework['Option']['Application']['BuildOptions'][arch] = value
                else:
                    jframework=jconfig['JFramework'][core]
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                    if 'BuildOptions' not in jframework['Option']['Application']: jframework['Option']['Application']['BuildOptions'] = {}
                    jframework['Option']['Application']['BuildOptions'][arch] = value
            elif component == 'CRL':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        if 'Option' not in jframework: jframework['Option'] = {}
                        if 'CRL' not in jframework['Option']: jframework['Option']['CRL'] = {}
                        if 'BuildOptions' not in jframework['Option']['CRL']: jframework['Option']['CRL']['BuildOptions'] = {}
                        jframework['Option']['CRL']['BuildOptions'][arch] = value
                else:
                    jframework=jconfig['JFramework'][core]
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'CRL' not in jframework['Option']: jframework['Option']['CRL'] = {}
                    if 'BuildOptions' not in jframework['Option']['CRL']: jframework['Option']['CRL']['BuildOptions'] = {}
                    jframework['Option']['CRL']['BuildOptions'][arch] = value
            elif component == 'Drivers':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        if 'Option' not in jframework: jframework['Option'] = {}
                        if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
                        if 'BuildOptions' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['BuildOptions'] = {}
                        jframework['Option']['Drivers']['BuildOptions'][arch] = value
                else:
                    jframework=jconfig['JFramework'][core]
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
                    if 'BuildOptions' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['BuildOptions'] = {}
                    jframework['Option']['Drivers']['BuildOptions'][arch] = value
            elif component == 'Platform':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        if 'Option' not in jframework: jframework['Option'] = {}
                        if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                        if 'BuildOptions' not in jframework['Option']['Platform']: jframework['Option']['Platform']['BuildOptions'] = {}
                        jframework['Option']['Platform']['BuildOptions'][arch] = value
                else:
                    jframework=jconfig['JFramework'][core]
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                    if 'BuildOptions' not in jframework['Option']['Platform']: jframework['Option']['Platform']['BuildOptions'] = {}
                    jframework['Option']['Platform']['BuildOptions'][arch] = value
            elif component == 'Runtime':
                if 'Option' not in jconfig['Runtime']: jconfig['Runtime']['Option'] = {}
                if 'BuildOptions' not in jconfig['Runtime']['Option']: jconfig['Runtime']['Option']['BuildOptions'] = {}
                jconfig['Runtime']['Option']['BuildOptions'][arch] = value
            elif component == 'JFramework':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        if 'BuildOptions' not in jframework: jframework['BuildOptions'] = {}
                        jframework['BuildOptions'][arch] = value
                else:
                    jframework=jconfig['JFramework'][core]
                    if 'BuildOptions' not in jframework: jframework['BuildOptions'] = {}
                    jframework['BuildOptions'][arch] = value
            elif component == 'JModule':
                if core == None:
                    for jframework in jconfig['JFramework']:
                        for id, v in value.items():
                            if 'BuildOptions' not in jframework['JModule'][id]: jframework['JModule'][id]['BuildOptions'] = {}
                            jframework['JModule'][id]['BuildOptions'][arch] = v
                else:
                    jframework=jconfig['JFramework'][core]
                    for id, v in value.items():
                        if 'BuildOptions' not in jframework['JModule'][id]: jframework['JModule'][id]['BuildOptions'] = {}
                        jframework['JModule'][id]['BuildOptions'][arch] = v
            else:
                print('Error: JConfig_CustomBuildFlags build_options usage %s' % build_options_usage)
                return

def JConfig_TalariaDefConfig(jconfig,
                             project_root, project_name,
                             model_root, model_name,
                             misc_root=None,
                             id=0, core=None) :
    if ('Option' in jconfig) and ('arch' in jconfig['Option']) and (jconfig['Option']['arch']):
        if isinstance(jconfig['Option']['arch'], dict):
            arch_dict = jconfig['Option']['arch']
        else:
            arch_dict = {'Baf': jconfig['Option']['arch']}
    else:
        print('Error: JConfig_TalariaDefConfig - could not determine arch')
        return

    if misc_root==None:
        app_root=os.getenv('APP_ROOT')
        misc_root=app_root+'/Misc'

    if not os.path.exists(misc_root):
        print('Error: JConfig_TalariaDefConfig - misc_root %s path not found' % (misc_root))
        return

    model_ctrlgen = [ 'RTC_Payloads.xml', 'TOP_Tokens.xml', ctrlgen_Interpretation(), ctrlgen_Translation()]

    crlfiles_sharc =['src/pooliirProcessSharc.asm',
           'src/FirEvenSharc.asm',
           'src/IFirProcessSharc.asm']

    crlfiles_gul =['src/pooliirSharcPlus.asm',
           'src/FirEvenProcessSharc_plus.asm',
           'src/IFirProcessSharc_plus.asm',
           'src/fir_core.c',
           'src/fir_vec_mc_21XI.asm',
           'src/upfirProcessSharc_plus.asm']

    crlfiles_gxp =['src/pooliirSharcPlus.asm',
           'src/FirEvenProcessSharc_plus.asm',
           'src/IFirProcessSharc_plus.asm',
           'src/fir_core.c',
           'src/fir_vec_mc_21XI.asm',
           'src/upfirProcessSharc_plus.asm']

    crlfiles_hifi4 =['src/FirEven.c']
    crlfiles_hifi5 =['src/FirEven.c']


    JConfig_TalariaProject(jconfig, project_root, project_name, id=id, core=core)
    JConfig_TalariaModel(jconfig, model_root, model_name, id=id, core=core)

    JConfig_TalariaCtrlgen(jconfig, model_ctrlgen, id=id, core=core)
    JConfig_DLL_options(jconfig, DLL_options(), id=id, core=core)

    JConfig_TalariaPartitioning(jconfig, partitioning=None )

    for k, arch in arch_dict.items():
        if arch in ['x86', 'c66', 'imx8', 'hifi2', 'hifi3', 'amd64', 'x64', 'qnx_nto_aarch64']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetx86.m', crlPath=None, crlFiles=None, id=id, core=core)
        elif arch in ['hexagon','hexagon_v66','hexagon_v73']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetHexagon.m', crlPath=None, crlFiles=None, id=id, core=core)
        elif arch in ['sharc']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetSharc.m', crlPath='blocklib/arch/sharc', crlFiles=crlfiles_sharc, id=id, core=core)
        elif arch in ['gul']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetSharcPlus.m', crlPath='blocklib/arch/sharc_plus', crlFiles=crlfiles_gul, id=id, core=core)
        elif arch in ['gxp']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetSharcPlusGXP.m', crlPath='blocklib/arch/sharc_plus_gxp', crlFiles=crlfiles_gxp, id=id, core=core)
        elif arch in ['hifi4']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetHifi4.m', crlPath='blocklib/arch/hifi4', crlFiles=crlfiles_hifi4, id=id, core=core)
        elif arch in ['hifi5']:
            JConfig_TalariaCrl(jconfig, arch=arch, config=misc_root+'/ConfigSetHifi5.m', crlPath='blocklib/arch/hifi5', crlFiles=crlfiles_hifi5, id=id, core=core)
        else:
            print('Error: JConfig_TalariaDefConfig - unknown arch %s' % (arch))
            return

        if arch in ['gul']:
            JConfig_AddTalariaDrivers(jconfig, 'gul', driversPath=misc_root+'/drivers/gul')
        elif arch in ['gxp']:
            JConfig_AddTalariaDrivers(jconfig, 'gxp', driversPath=misc_root+'/drivers/gxp')

    JConfig_MiscRoot(jconfig, misc_root, id=id, core=core)
    JConfig_TalariaTest(jconfig, '%sTestCase' % (model_name), 'test_default', id=id, core=core)

def JConfig_ProbePointReadJson(jconfig, filename=None, filepath=None, id=0, core=0) :
        path = None
        #
        # filepath and filename provides two manners of specifying the probe points json file
        # filepath overrides filename
        #       filepath is usually handwritten probe points json file
        # filename is assumed to be relative to variant_root/jconfig['Name']
        #      scripts such as code_tagger are supposed to generate the probe points json file
        #      at variant_root/jconfig['Name']
        #
        if filepath != '' and filepath != None:
            path = filepath
        else:
            if filename != '' and filename != None:
                if ('Name' in jconfig) and (jconfig['Name']):
                    path = f"{os.getenv('VARIANT_ROOT')}{jconfig['Name']}/{filename}"

        if path != '' and path != None:
            probe_points = None
            try:
                with open(path, 'r', encoding='utf-8') as filehandler:
                    probe_points = json.load(filehandler)
            except IOError:
                print('Info: JConfig_ProbePointReadJson file ' + path + ' does not exist')
                pass
            except json.decoder.JSONDecodeError as e:
                print(f'Error: JConfig_ProbePointReadJson parsing file {path} exception {e}')

            if probe_points:
                JConfig_TalariaProbePoints(jconfig, probe_points, id=id, core=core)

def JConfig_TalariaProbePoints(jconfig, probe_points=None, id=0, core=0) :
    if probe_points:
        probe_points_usage = """
        probe_points is a nested dictionary. For example
        probe_points = {
            'enable': False,
            'processes': {
                'volume48K':    {'process_id':0x80},
                'balance48K':   {'process_id':0x81},
                'mute48K':      {'process_id':0x82},
                'bass48K':      {'process_id':0x83},
                'treble48K':    {'process_id':0x84},
            }
        }
        """
        if not isinstance(probe_points, dict):
            print('Error: JConfig_TalariaProbePoints probe_points usage %s' % probe_points_usage)
            return
        if ('processes' not in probe_points) or (not isinstance(probe_points['processes'], dict)):
            print('Error: JConfig_TalariaProbePoints probe_points usage %s' % probe_points_usage)
            return
        for k, process in probe_points['processes'].items():
            if (not isinstance(process, dict)) or ('process_id' not in process):
                print('Error: JConfig_TalariaProbePoints probe_points usage %s' % probe_points_usage)
                return
            else:
                process['thread_level'] = OJTSI.TimeObjOJT.TimeObj.PROBE_POINT_TID
        if core == None:
            for jframework in jconfig['JFramework']:
                jframework['JModule'][id]['TalariaModel']['ProbePoints'] = copy.deepcopy(probe_points)

                if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
                if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
                jframework['MIPSReport']['Option']['probe_points'] = jframework['JModule'][id]['TalariaModel']['ProbePoints']
        else:
            jframework = jconfig['JFramework'][core]
            jframework['JModule'][id]['TalariaModel']['ProbePoints'] = copy.deepcopy(probe_points)

            if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
            if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
            jframework['MIPSReport']['Option']['probe_points'] = jframework['JModule'][id]['TalariaModel']['ProbePoints']

def JConfig_TalariaParam(jconfig, param, id=0, core=0) :
    jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Parameters'] =  copy.deepcopy(param)

def JConfig_TalariaPartitionGenerateOptions(jconfig, pgo_cfg=None, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['PartitionGenerateOptions'] = pgo_cfg
    else:
        jframework = jconfig['JFramework'][core]
        jframework['JModule'][id]['TalariaModel']['PartitionGenerateOptions'] = pgo_cfg

def JConfig_DLL_options(jconfig, dll_options, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['DLL'] = dll_options
    else:
        jframework = jconfig['JFramework'][core]
        jframework['JModule'][id]['TalariaModel']['DLL'] = dll_options

def JConfig_Platform_InterCoreXfer(jconfig, xfer='SharedMem', core=None) :
    if xfer in ['SharedMem', 'MDMA']:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['InterCoreXfer'] = xfer
        else:
            jframework = jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['InterCoreXfer'] = xfer

def JConfig_TalariaProject(jconfig, path, name, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            jframework['JModule'][id]['TalariaModel']['ProjectPath'] =  path
            jframework['JModule'][id]['TalariaModel']['Project'] =  name
    else:
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['ProjectPath'] =  path
        jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Project'] =  name

def JConfig_NoTalaria(jconfig, id=0, core=0) :
    jconfig['JFramework'][core]['JModule'][id]['TalariaModel'] = None

def JConfig_ReadXML(jconfig, frameworkName, moduleNames, core=0) :
    jconfig['JFramework'][core]['FrameworkName'] = frameworkName
    jconfig['JFramework'][core]['ModuleNames'] = moduleNames
    jconfig['ReadXML'] = True

def JConfig_SetLib(jconfig, path, arch, libName, id=0, core=0) :
    jconfig['JFramework'][core]['JModule'][id]['LibModel']['ArchToLib'][arch] = '\\' + path + '\\' + libName
    jconfig['JFramework'][core]['JModule'][id]['LibModel']['Path'] = '\\' + path + '\\'

def JConfig_SetC(jconfig, path, fileNames, id=0, core=0) :
    jconfig['JFramework'][core]['JModule'][id]['CModel']['Path'] = "\\" + path + "\\"
    jconfig['JFramework'][core]['JModule'][id]['CModel']['CFiles'] = fileNames

def JConfig_PrimeBAFBuffer(jconfig, boolOption) :
    jconfig['Runtime']['Option']['PrimeBuffer'] = boolOption

def JConfig_MultiRatePlatform(jconfig, boolOption=False, baseThread=0, core=None) :
    if boolOption:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['MultiRatePlatform'] = boolOption
                jframework['Option']['Platform']['BaseThreadLevel'] = baseThread

                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['MultiRatePlatform'] = boolOption
                jframework['Option']['Application']['BaseThreadLevel'] = baseThread
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['MultiRatePlatform'] = boolOption
            jframework['Option']['Platform']['BaseThreadLevel'] = baseThread

            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['MultiRatePlatform'] = boolOption
            jframework['Option']['Application']['BaseThreadLevel'] = baseThread

def JConfig_SetScheduler(jconfig, boolOption=False, call_interval=None, core=None) :
    if boolOption:
        jconfig['Runtime']['Option']['JSched'] = boolOption
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['JSched'] = boolOption

                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['JSched'] = boolOption
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['JSched'] = boolOption

            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['JSched'] = boolOption

        if call_interval != None:
            if core == None:
                for jframework in jconfig['JFramework']:
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                    jframework['Option']['Application']['CallInterval'] = call_interval
            else:
                jframework=jconfig['JFramework'][core]
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['CallInterval'] = call_interval

def JConfig_SetExtScheduler(jconfig, boolOption=False, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['ExtScheduler'] = 1 if boolOption else 0
    else:
        jframework=jconfig['JFramework'][core]
        if 'Option' not in jframework: jframework['Option'] = {}
        if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
        jframework['Option']['Application']['ExtScheduler'] =	1 if boolOption else 0

def JConfig_EnableGHAM(jconfig, boolOption=False, core=None) :
    if boolOption:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['EnableGHAM'] = boolOption
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['EnableGHAM'] = boolOption

def JConfig_FreeRunning(jconfig, boolOption=False, core=None) :
    if boolOption:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['FreeRunning'] = boolOption

                if 'Simulation' not in jframework['Option']: jframework['Option']['Simulation'] = {}
                jframework['Option']['Simulation']['FreeRunning'] = boolOption
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['FreeRunning'] = boolOption

            if 'Simulation' not in jframework['Option']: jframework['Option']['Simulation'] = {}
            jframework['Option']['Simulation']['FreeRunning'] = boolOption

def JConfig_BankMappingInc(jconfig, bank_mapping_inc, core=None) :
    if bank_mapping_inc:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['bank_mapping_inc'] = bank_mapping_inc
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['bank_mapping_inc'] = bank_mapping_inc

def JConfig_LinkerCommandFile(jconfig, linker_cmd_file, core=None) :
    if linker_cmd_file:
        linker_cmd_file = os.path.normpath(linker_cmd_file)
        bank_mapping_file = os.path.join(os.path.dirname(linker_cmd_file),'jtestBankMapping.h')
        bank_mapping_inc = None
        if os.path.exists(bank_mapping_file):
            bank_mapping_inc = os.path.dirname(linker_cmd_file)
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['linker_cmd_file'] = linker_cmd_file
                jframework['Option']['Application']['bank_mapping_inc'] = bank_mapping_inc
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['linker_cmd_file'] = linker_cmd_file
            jframework['Option']['Application']['bank_mapping_inc'] = bank_mapping_inc

def JConfig_PIL_DCacheFlushInvalidate(jconfig, cacheflushinv=False, core=None) :
    if cacheflushinv:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['PIL_DCacheFlushInvalidate'] = cacheflushinv
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['PIL_DCacheFlushInvalidate'] = cacheflushinv

def JConfig_PIL_InterFrameDelay(jconfig, core_cycles, core=None) :
    if core_cycles:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['PIL_InterFrameDelay'] = core_cycles
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['PIL_InterFrameDelay'] = core_cycles

def JConfig_PIL_EZKitOnly(jconfig, ezkit_only, core=None) :
    if ezkit_only:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['PIL_EzKitOnly'] = ezkit_only
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['PIL_EzKitOnly'] = ezkit_only

def JConfig_MIPSReport_SkipFrames(jconfig, skip_frames=0, core=None) :
    if skip_frames != None:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
                if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
                jframework['MIPSReport']['Option']['skip_frames'] = skip_frames
        else:
            jframework=jconfig['JFramework'][core]
            if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
            if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
            jframework['MIPSReport']['Option']['skip_frames'] = skip_frames

def JConfig_MIPSReport_ScalingFactors(jconfig, scaling_factors, core=0) :
    jframework=jconfig['JFramework'][core]
    if scaling_factors != None:
        if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
        if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
        jframework['MIPSReport']['Option']['scaling_factors'] = scaling_factors

def JConfig_MIPSReport(jconfig, scaling_factors=None, id=0, core=0) :
    jframework=jconfig['JFramework'][core]
    if scaling_factors != None:
        if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
        if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
        jframework['MIPSReport']['Option']['scaling_factors'] = scaling_factors
    if ('Generator' in jframework) and (jframework['Generator']):
        generator = jframework['Generator']
        if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
        if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
        jframework['MIPSReport']['Option']['process_list'] = generator.GetProcesses(0, jframework['Name'])
    if ('ProbePoints' in jframework['JModule'][id]['TalariaModel']) and (jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['ProbePoints']):
        probe_points = jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['ProbePoints']
        if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
        if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
        jframework['MIPSReport']['Option']['probe_points'] = probe_points

def JConfig_RefAudio_FrameShift(jconfig, frame_cnt, core=0) :
    jframework=jconfig['JFramework'][core]
    if frame_cnt:
        if 'AudioPlot' not in jframework: jframework['AudioPlot'] = {}
        if 'Option' not in jframework['AudioPlot']: jframework['AudioPlot']['Option'] = {}
        jframework['AudioPlot']['Option']['RefAudio_FrameShift'] = frame_cnt

def JConfig_RemoveTalariaCrl(jconfig, arch, id=0, core=0) :
    talaria_model=jconfig['JFramework'][core]['JModule'][id]['TalariaModel']
    if 'crlPath' in talaria_model: del(talaria_model['crlPath'])
    if 'crl' in talaria_model: del(talaria_model['crl'])

def JConfig_TalariaConfig(jconfig, arch, config=None, id=0, core=0) :
    talaria_model=jconfig['JFramework'][core]['JModule'][id]['TalariaModel']
    if config:
        if 'config' not in talaria_model: talaria_model['config']={}
        talaria_model['config'][arch] =  config

def JConfig_TalariaCrl(jconfig, arch, config=None, crlPath=None, crlFiles=None, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            talaria_model=jframework['JModule'][id]['TalariaModel']
            if crlPath == None:
                if 'crlPath' not in talaria_model: talaria_model['crlPath']={}
                talaria_model['crlPath'][arch] = None
            elif crlPath:
                if 'crlPath' not in talaria_model: talaria_model['crlPath']={}
                talaria_model['crlPath'][arch] = '#/'+ crlPath
            if crlFiles == None:
                if 'crl' not in talaria_model: talaria_model['crl']={}
                talaria_model['crl'][arch] =  None
            elif crlFiles:
                if 'crl' not in talaria_model: talaria_model['crl']={}
                talaria_model['crl'][arch] =  copy.deepcopy(crlFiles)
            if config:
                if 'config' not in talaria_model: talaria_model['config']={}
                talaria_model['config'][arch] =  config
    else:
        talaria_model=jconfig['JFramework'][core]['JModule'][id]['TalariaModel']
        if crlPath == None:
            if 'crlPath' not in talaria_model: talaria_model['crlPath']={}
            talaria_model['crlPath'][arch] = None
        elif crlPath:
            if 'crlPath' not in talaria_model: talaria_model['crlPath']={}
            talaria_model['crlPath'][arch] = '#/'+ crlPath
        if crlFiles == None:
            if 'crl' not in talaria_model: talaria_model['crl']={}
            talaria_model['crl'][arch] =  None
        elif crlFiles:
            if 'crl' not in talaria_model: talaria_model['crl']={}
            talaria_model['crl'][arch] =  copy.deepcopy(crlFiles)
        if config:
            if 'config' not in talaria_model: talaria_model['config']={}
            talaria_model['config'][arch] =  config

def JConfig_RemoveTalariaDrivers(jconfig, arch, id=0, core=0) :
    talaria_model=jconfig['JFramework'][core]['JModule'][id]['TalariaModel']
    if 'driversPath' in talaria_model: del(talaria_model['driversPath'])
    if 'driversFiles' in talaria_model: del(talaria_model['driversFiles'])

def JConfig_AddTalariaDrivers(jconfig, arch, driversPath=None, driversFiles=None, id=0, core=None) :
    if core == None:
        for jframework in jconfig['JFramework']:
            talaria_model=jframework['JModule'][id]['TalariaModel']
            if driversPath:
                if 'driversPath' not in talaria_model: talaria_model['driversPath']={}
                talaria_model['driversPath'][arch] = '#/'+ driversPath
                if 'driversFiles' not in talaria_model: talaria_model['driversFiles']={}
                if driversFiles:
                    talaria_model['driversFiles'][arch] =  copy.deepcopy(driversFiles)
                else:
                    talaria_model['driversFiles'][arch] = [os.path.basename(t) for t in glob.glob(driversPath+'\*.c')]

                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
                jframework['Option']['Platform']['driversPath'] = '#/'+ driversPath
    else:
        talaria_model=jconfig['JFramework'][core]['JModule'][id]['TalariaModel']
        if driversPath:
            if 'driversPath' not in talaria_model: talaria_model['driversPath']={}
            talaria_model['driversPath'][arch] = '#/'+ driversPath
            if 'driversFiles' not in talaria_model: talaria_model['driversFiles']={}
            if driversFiles:
                talaria_model['driversFiles'][arch] =  copy.deepcopy(driversFiles)
            else:
                talaria_model['driversFiles'][arch] = [os.path.basename(t) for t in glob.glob(driversPath+'\*.c')]

            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Platform' not in jframework['Option']: jframework['Option']['Platform'] = {}
            jframework['Option']['Platform']['driversPath'] = '#/'+ driversPath

def JConfig_JModuleParam(jconfig, param, core=0, id=0) :
    jconfig['JFramework'][core]['JModule'][id]['Parameters'] =  copy.deepcopy(param)

def JConfig_BcfParam(jconfig, param, core=0, id=0, generator=None) :

    jconfig['BCF']['Parameters'] =  copy.deepcopy(param)
    if generator: jconfig['BCF']['Generator'] =  generator

def JConfig_BAFStatsLogger(jconfig, boolOption=False, statsLogger=None, statsConfig=None, core=None) :
    g_known_stats = ['PerfMon','StackMon', 'PktMon', 'Ccnt64Mon']
    if boolOption == True:
        if statsLogger not in g_known_stats:
            print ('Error: JConfig_BAFStatsLogger statsLogger not in %s' % ','.join(g_known_stats))
            return
        if statsLogger == 'PerfMon':
            g_known_config = ['DMCACHE', 'PMCACHE', 'ICACHE']
            if statsConfig not in g_known_config:
                print ('Error: JConfig_BAFStatsLogger statsConfig not in %s' % ','.join(g_known_config))
                return

    if 'Option' not in jconfig['Runtime']: jconfig['Runtime']['Option'] = {}
    jconfig['Runtime']['Option']['BAFStatsLogger'] = boolOption

    if boolOption == True:
        if statsLogger in g_known_stats:
            stats_config = {'enable': boolOption,
                            'statsLogger' : statsLogger,
                            'statsConfig' : statsConfig }
            if core == None:
                for jframework in jconfig['JFramework']:
                    if 'Option' not in jframework: jframework['Option'] = {}
                    if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                    jframework['Option']['Application']['BAFStatsLogger'] = copy.deepcopy(stats_config)
            else:
                jframework=jconfig['JFramework'][core]
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['BAFStatsLogger'] = copy.deepcopy(stats_config)

def JConfig_Logger(jconfig, boolOption=True, logSize=None, printLogSize=None, captureLogSize=None, execLogSize=None, core=None) :
    if 'Option' not in jconfig['Runtime']: jconfig['Runtime']['Option'] = {}
    jconfig['Runtime']['Option']['Logger'] = boolOption

    logger_config = {'enable': boolOption,
                     'logSize': 6*1024,
                     'printLogSize': 2*1024,
                     'captureLogSize': 2*1024,
                     'execLogSize': 2*1024 }

    if logSize != None:
        logger_config['logSize'] = logSize

    if (printLogSize != None) or (captureLogSize != None) or (execLogSize != None):
        logger_config['printLogSize'] = 0
        logger_config['captureLogSize'] = 0
        logger_config['execLogSize'] = 0

    if (printLogSize != None):
        logger_config['printLogSize'] = printLogSize
    if captureLogSize != None:
        logger_config['captureLogSize'] = captureLogSize
    if execLogSize != None:
        logger_config['execLogSize'] = execLogSize
    if (( logger_config['printLogSize'] + logger_config['captureLogSize'] + logger_config['execLogSize'] ) > logger_config['logSize'] ):
        print('Error: JConfig_Logger combined size %d smaller than sum of individual sizes (print %d, capture %d, exec %d)' %
                logger_config['logSize'],
                logger_config['printLogSize'],
                logger_config['captureLogSize'],
                logger_config['execLogSize'] )
    else:
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
                jframework['Option']['Application']['Logger'] = copy.deepcopy(logger_config)
        else:
            jframework=jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Application' not in jframework['Option']: jframework['Option']['Application'] = {}
            jframework['Option']['Application']['Logger'] = copy.deepcopy(logger_config)

def JTest_Add(name, arg = None):
    filename = name + '/JConfig.py'
    if os.path.exists(filename):
        run_path(filename, init_globals=arg)
        #exec(compile(source=open(filename).read(), filename=filename, mode='exec'), arg)

def JTest_JConfigApp(jconfig = None, app_config_root=None, arg=None):
    if app_config_root:
        app_config_script = os.path.join(app_config_root, 'JConfigApp.py')
        if os.path.exists(app_config_script):
            app_config_globals = run_path(app_config_script, init_globals=arg)
            app_obj = app_config_globals['app_config']
            if jconfig:
                JConfig_SetGenerator(jconfig, generator=app_obj)
            return app_obj
        else:
            print('Error: JConfigApp %s does not exist' % app_config_script)
            return None
    else:
        print('Error: JConfigApp app_config_script parameter is not optional')
        return None

def JConfig_SShLoginConfigure(jconfig, hostname, username, pwd):
    jconfig['Option']['hostname'] = hostname
    jconfig['Option']['user'] = username
    jconfig['Option']['pwd'] = pwd

def JConfig_SetDefaultTestSrc(jconfig, testFiles, core=0):
    jconfig['JFramework'][core]['Test']['Src'] = testFiles

def JConfig_SetDefaultTestGen(jconfig, testFiles):
    jconfig['JFramework'][0]['DefaultTest']['Gen'] = testFiles

def JConfig_SetTalariaTestGen(jconfig, testFiles, core=0, id=0):
    jconfig['JFramework'][core]['JModule'][id]['TalariaModel']['Test']['Gen'] = testFiles

def JConfig_ProfileAccelerators(jconfig, arch, core=None, accelerators=None):
    if arch == 'gxp':
        accelerators = {
            'FIR': [{'thread_level':OJTSI.TimeObjOJT.TimeObj.FIR_ACCEL_PROCESS_START_TID}],
            'IIR': [{'thread_level':OJTSI.TimeObjOJT.TimeObj.IIR_ACCEL_PROCESS_START_TID}]
        }

        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
                if 'AccelProfile' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['AccelProfile'] = {}
                jframework['Option']['Drivers']['AccelProfile'][arch] = accelerators
        else:
            jframework = jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
            if 'AccelProfile' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['AccelProfile'] = {}
            jframework['Option']['Drivers']['AccelProfile'][arch] = accelerators

    elif arch == 'gul':
        accelerators_usage = """
        accelerators is a nested dictionary
        accelerators = {
            'IIR': [{'process_id':0x70}, ...],
            'FIR': [{'process_id':0x71}, ...]
        }
        """
        if not isinstance(accelerators, dict):
            print('Error: JConfig_ProfileAccelerators accelerators usage %s' % accelerators_usage)
            return

        accelerators=copy.deepcopy(accelerators)

        for k, accelerator in accelerators.items():
            for accel_instance in accelerator:
                if (not isinstance(accel_instance, dict)) or ('process_id' not in accel_instance):
                    print('Error: JConfig_ProfileAccelerators accelerators usage %s' % accelerators_usage)
                    return

                #If the user has specified a thread_level, save it in a copy
                if 'thread_level' in accel_instance:
                    accel_instance['_thread_level'] = accel_instance['thread_level']
                else:
                    accel_instance['_thread_level'] = None
                accel_instance['thread_level'] = OJTSI.TimeObjOJT.TimeObj.ACCEL_PROCESS_TID
        if core == None:
            for jframework in jconfig['JFramework']:
                if 'Option' not in jframework: jframework['Option'] = {}
                if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
                if 'AccelProfile' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['AccelProfile'] = {}
                jframework['Option']['Drivers']['AccelProfile'][arch] = accelerators

                if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
                if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
                jframework['MIPSReport']['Option']['AccelProcesses'] = {arch: jframework['Option']['Drivers']['AccelProfile'][arch]}
        else:
            jframework = jconfig['JFramework'][core]
            if 'Option' not in jframework: jframework['Option'] = {}
            if 'Drivers' not in jframework['Option']: jframework['Option']['Drivers'] = {}
            if 'AccelProfile' not in jframework['Option']['Drivers']: jframework['Option']['Drivers']['AccelProfile'] = {}
            jframework['Option']['Drivers']['AccelProfile'][arch] = accelerators

            if 'MIPSReport' not in jframework: jframework['MIPSReport'] = {}
            if 'Option' not in jframework['MIPSReport']: jframework['MIPSReport']['Option'] = {}
            jframework['MIPSReport']['Option']['AccelProcesses'] = {arch: jframework['Option']['Drivers']['AccelProfile'][arch]}

def JConfig_AutogenConfig(jconfig, path=None):
    if path != '' and path != None:
        try:
            with open(path, 'r') as filehandler:
                 config = json.load(filehandler)
        except IOError:
            print('Error: JConfig_AutogenConfig file ' + path + ' does not exist')
        except json.decoder.JSONDecodeError as e:
            print(f'Error: JConfig_AutogenConfig parsing file {path} exception {e}')
    jconfig['Runtime']['Context'] = config['JFRAMEWORK']['BAF_CALLBACK_CONTEXT']=='enable' if config is not None and 'JFRAMEWORK' in config and 'BAF_CALLBACK_CONTEXT' in config['JFRAMEWORK']  else False

def JConfig_SetAutogenContext(jconfig, enable=True):
    jconfig['Runtime']['Context'] = enable

def JConfig_SetBafObjectName(jconfig, name='BAF_obj'):
    jconfig['Runtime']['ObjectName'] = name

def JConfig_BafLite(jconfig, enable = True):
    if 'Option' not in jconfig['Runtime']: jconfig['Runtime']['Option'] = {}
    jconfig['Runtime']['Option']['Lite'] = enable
    for jframework in jconfig['JFramework']:
        if 'Option' not in jframework: jframework['Option'] = {}
        jframework['Option']['Lite'] = enable


