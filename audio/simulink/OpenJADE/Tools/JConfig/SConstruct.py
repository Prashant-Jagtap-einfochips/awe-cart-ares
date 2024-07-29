import os
import subprocess
import pprint
import sys

from shutil import copyfile


if 'app_root' not in globals():
    app_root='.'

os.environ['APP_ROOT'] = app_root
os.environ['VARIANT_ROOT'] = app_root + '/out/'

sys.path.append( app_root+'/Tools/JTest')
sys.path.append( app_root+'/Tools/JConfig')
sys.path.append( app_root+'/Tools/JAutogen')
sys.path.append( app_root+'/Tools/JConfig/cces_gen')
sys.path.append( app_root+'/Tools/JTest/OpenJADE_code_utility')
sys.path.append( app_root )

from ojcu import *

from create_cces_project import *

import OJTSI.ConvertRequest
import OJTSI.ConvertResponse
import OJTSI.MIPSReport
import OJTSI.GULPerfMonOJT
import OJTSI.GULStackMonOJT
import OJTSI.Ccnt64MonOJT
import OJTSI.HexagonPktMonOJT
import OJTSI.PlotExecGraphOJT
import OJTSI.OJExecLogOJT
import OJTSI.PassFailReportV2
import OJTSI.PlotAudioOJT
import xmlparse.SizeSharcBinary
import xmlparse.SizeC6678Binary
import xmlparse.SizeGNUBinary

import xmlparse.SHARCMemViz
import postprocess.PingPongStruct

from  JTest import *
from  JSimulation import *
from  SConfig import *
from  JAutogen import *
from  JAutogenBafLite import *
from  JAutogenProbe import *
from  JFrameworkStru import *

from JSConsUtils import *

JTest_Add('./')

AddOption('--arch',
          default = None,
          dest='arch',
          nargs=1, type='string',
          action='store',
          metavar='<processor architecture>',
          help='<processor architecture>: sharc, gul, gxp, hexagon, hifi4, hifi5, amd64(linux only), c66, x86(default), x64(dll only), qnx_nto_aarch64')

AddOption('--cart',
          default = None,
          dest='cart',
          nargs=1, type='string',
          action='store',
          metavar='<cart component>',
          help='<cart component>: baf(default), bcf, btf, brf')

AddOption('--dump',
          default = None,
          dest='dump_alias',
          nargs=1, type='string',
          action='store',
          metavar='<alias for dump argument>',
          help='<dump>: 1(env), 2(jconfig), 3(jconfig), test, None(default)')

AddOption('--filter-arch',
          default = None,
          dest='filter_arch',
          nargs=1, type='string',
          action='store',
          metavar='<filter arch for dump=test output>',
          help='<filter>: filter dump=test output by arch')

AddOption('--filter-platform',
          default = None,
          dest='filter_platform',
          nargs=1, type='string',
          action='store',
          metavar='<filter platform for dump=test output>',
          help='<filter>: filter dump=test output by platform')

AddOption('--filter-test',
          default = None,
          dest='filter_test',
          nargs=1, type='string',
          action='store',
          metavar='<filter test name for dump=test output>',
          help='<filter>: filter dump=test output by test name')

AddOption('--test',
          default = None,
          dest='test',
          const='1',
          nargs='?', type='string',
          action='store',
          metavar='<test configuration entry>',
          help='<test configuration entry>: 1 up to number of configuration entries')

AddOption('--platform',
          default = None,
          dest='platform',
          nargs=1, type='string',
          action='store',
          metavar='<test platform>',
          help='<test platform>: RTA, HIL, PIL, SIL(default), MIL, DLL, PIL_shared_library')

AddOption('--runner',
          default = None,
          dest='runner',
          nargs=1, type='string',
          action='store',
          metavar='<running mechanism>',
          help='<running mechanism>: ezkit, sim, none(default)')

AddOption('--build',
          default = 'Default',
          dest='build',
          nargs=1, type='string',
          action='store',
          metavar='<build configuration>',
          help='<build configuration>: Debug, Release, Default')

AddOption('--tools',
          default = '',
          dest='tools',
          nargs=1, type='string',
          action='store',
          metavar='<tools path>',
          help='<tools path>: Path where compiling tools are installed')

AddOption('--matlab',
          default = '',
          dest='matlab',
          nargs=1, type='string',
          action='store',
          metavar='<matlab path>',
          help='<matlab path>: Path to matlab.exe')

AddOption('--sweep',
          default = 'no',
          const = 'yes',
          dest='sweep',
          nargs='?', type='string',
          action='store',
          metavar='<sweep>',
          help='<sweep>=[yes|no]: enable/disable sweep build')

AddOption('--sim',
          default = 'no',
          const = 'yes',
          dest='sim',
          nargs='?', type='string',
          action='store',
          metavar='<sim>',
          help='<sim>=[yes|no]: enable/disable simulations/runs')

AddOption('--app',
          default = 'yes',
          dest='app',
          nargs='?', type='string',
          action='store',
          metavar='<app>',
          help='<app>=no: App is not built. Library build only')

AddOption('--model',
          default = 'yes',
          dest='model',
          nargs='?', type='string',
          action='store',
          metavar='<model>',
          help='<model>=no: Model is not built. Useful only if prebuilt sources and test vectors exist')

AddOption('--cfg',
          default = 'yes',
          dest='cfg',
          nargs='?', type='string',
          action='store',
          metavar='<cfg>',
          help='--cfg: Generate configuration files only')

AddOption('--lib',
          default = 'yes',
          dest='lib',
          nargs='?', type='string',
          action='store',
          metavar='<lib>',
          help='<lib>=no: App and libraries are not built. Code generation only')

AddOption('--options',
          default = '',
          dest='options',
          nargs=1, type='string',
          action='store',
          metavar='<configuration options>',
          help='<configuration options>: Architecture specific configuration options ')

AddOption('--hostname',
          default = '',
          dest='hostname',
          nargs=1, type='string',
          action='store',
          metavar='<Host Name>',
          help='<Host Name>: localhost')

AddOption('--user',
          default = '',
          dest='user',
          nargs=1, type='string',
          action='store',
          metavar='<User Name>',
          help='<User Name>: user')

AddOption('--pwd',
          default = '',
          dest='pwd',
          nargs=1, type='string',
          action='store',
          metavar='<password>',
          help='<password>: password')

AddOption('--exp',
          default = 'no',
          const = 'yes',
          dest='exp',
          nargs='?', type='string',
          action='store',
          metavar='<exp>',
          help='<exp>=[yes|no]: enable/disable experimental target builds')

AddOption('--module',
          default = None,
          dest='module',
          nargs=1, type='string',
          action='store',
          metavar='<module>',
          help='<module>=no: Module build only. Ex: io-audio, capiv2, dll_baf, etc.')

AddOption('--dynamic',
          default = 'no',
          dest='dynamic',
          nargs=1, type='string',
          action='store',
          metavar='<dynamic>',
          help='<dynamic>=[yes|no]: If yes, build shared/dynamic library, else build static library.')

AddOption('--namespace',
          default = None,
          dest='namespace',
          nargs=1, type='string',
          action='store',
          metavar='<namespace>',
          help='<namespace>: C++ namespace for BCF, BTF, BRF')

if GetOption('sweep')=='yes':
    sweep_list = list(range(0, len(JConfig)))
elif GetOption('test'):
    testOption=GetOption('test').strip()
    if ',' in testOption:
        try:
            sweep_list=[(int(t)-1) for t in testOption.split(',')]
        except ValueError:
            sweep_list=[]
            for test_name in testOption.split(','):
                res = xlateTestNametoIndex(test_name, JConfig)
                if res['found'] == False:
                    print('Error: invalid test %s in test list %s' % (test_name,testOption))
                    exit()
                else:
                    sweep_list.append(res['idx']-1)
        for test_option in sweep_list:
            if(test_option < 0) or (test_option >= len(JConfig)):
                print('Error: --test option %s out of range' % testOption)
                exit()
        print('Info: test list %s' % testOption)
    else:
        sweep_start = None
        sweep_end   = None
        if ':' in testOption:
            sweepRange=testOption.split(':')
            if len(sweepRange) == 2:
                test_option=int(sweepRange[0])
                if(test_option > 0) and (test_option <= len(JConfig)):
                    sweep_start = test_option - 1

                test_option=int(sweepRange[1])
                if(test_option > 0) and (test_option <= len(JConfig)):
                    sweep_end = test_option
        else:
            try:
                test_option=int(testOption)
            except ValueError:
                res = xlateTestNametoIndex(testOption, JConfig)
                if res['found'] == False:
                    print('Error: invalid test option %s' % (testOption))
                    exit()
                else:
                    test_option = res['idx']

            if(test_option > 0) and (test_option <= len(JConfig)):
                sweep_start = test_option - 1
                sweep_end = test_option

        if (sweep_start == None) or (sweep_end == None):
            print('Error: --test option out of range ', testOption,'. Should be m or m:n with m and n between 1 and ',len(JConfig),' inclusive')
            exit()
        else:
            if sweep_start > sweep_end:
                sweep_tmp = sweep_end
                sweep_end = sweep_start
                sweep_start = sweep_tmp
        print('Info: test range [',sweep_start+1,',',sweep_end,']')
        sweep_list=list(range(sweep_start, sweep_end))
else : # first test only
    sweep_list = [0]

for cfg in [JConfig[test_option] for test_option in sweep_list]:
  env, option_dict, tools_path_override, arch_config_override  = get_env_options(cfg, globals())
  optionCart = option_dict['optionCart']
  optionArch = option_dict['optionArch']
  optionPlatform = option_dict['optionPlatform']
  optionRunner = option_dict['optionRunner']
  optionHostname = option_dict['optionHostname']
  optionUser = option_dict['optionUser']
  optionPwd = option_dict['optionPwd']
  optionSim = option_dict['optionSim']
  optionModel = option_dict['optionModel']
  optionLib = option_dict['optionLib']
  optionApp = option_dict['optionApp']
  optionMil = option_dict['optionMil']
  optionDynamic = option_dict['optionDynamic']
  optionNamespace = option_dict['optionNamespace']

  # Get OpenJADE Version
  env['OJ_VER'] = getOpenJADEVersion(app_root)
  variant_root = os.getenv('VARIANT_ROOT')

  if ('Name' in cfg) and (cfg['Name']):
      variant_root = variant_root + cfg['Name'] + '/'

  env['variant_root']=variant_root

  env['optionLib'] = optionLib
  env['optionApp'] = optionApp
  env['optionMil'] = optionMil
  env['optionModel'] = optionModel

  if (ARGUMENTS.get('dump', 0)=='test') or (GetOption('dump_alias') == 'test'):
      filter_tests = get_filter()
      print ('Index: Cart, [TestName], Arch, Module, Platform, Runner, ModelTestName, ModelTestCase, [ModelTestParam], [TestInfo]')
      filtered = 0
      for idx, jconfig in enumerate(JConfig, start=1):
          if filter_test(filter_tests, jconfig):
              filtered += 1
              continue
          test_item = str(idx)+': '
          test_item += (jconfig['Option']['cart'] if 'cart' in jconfig['Option'] else 'baf') +', '
          test_item += ('['+jconfig['Name'] + '], ' if ('Name' in jconfig) and jconfig['Name'] else ', [], ')
          test_item += (jconfig['Option']['arch'] if not isinstance(jconfig['Option']['arch'], dict) else 'multi') +', '
          test_item += (jconfig['Option']['module'] if 'yes' in jconfig['Option'] else 'no') +', '
          test_item += jconfig['Option']['platform']+', '
          test_item += jconfig['Option']['runner']
          if 'JFramework' in jconfig:
              jframework = jconfig['JFramework'][0]
              test_item += (', '+jconfig['JFramework'][0]['Test']['Name']+', ' + jconfig['JFramework'][0]['Test']['Case'] if ('Test' in jframework) and jframework['Test'] else ', , ,')
              test_item += (', ['+str(jframework['Test']['Param'])+']' if ('Test' in jframework) and jframework['Test'] and ('Param' in jframework['Test']) and jframework['Test']['Param'] else ', []')
          test_item += (', ['+jconfig['Info'] + ']' if ('Info' in jconfig) and jconfig['Info'] else ', []')
          print (test_item)

          if isinstance(jconfig['Option']['arch'], dict):
              arch_dict = jconfig['Option']['arch']
          else:
              arch_dict = {'Baf': jconfig['Option']['arch']}
          for k, v in arch_dict.items():
              local_env=env.Clone()
              (globals()[v+'_tools'])(local_env)
              if 'JFramework' in jconfig:
                  if not os.path.isfile(local_env['MATLAB'].replace('"','')):
                      print("Info: Required tool:" + local_env['MATLAB'] + " is missing")
              if 'TOOLSPATH' in local_env:
                  if not os.path.isdir(local_env['TOOLSPATH']):
                      print("Info: Required tool:" + local_env['TOOLSPATH'] + " is missing")
      if filtered == len(JConfig):
          print('Filter options did not result in any matching tests.')
      exit()

#env['JCONFIG'] = JConfig[1]
  if optionCart == 'cmb':
    for cfg in [JConfig[test_option] for test_option in sweep_list]:
        env, option_dict, tools_path_override, arch_config_override  = get_env_options(cfg, globals())
        cmb_src_root = app_root
        cmb_obj_path = variant_root+optionArch+'/lib/Cmb'
        brf_src_root = app_root+'/../BRF/'
        btf_src_root = app_root+'/../BTF/'
        Cmb = env.SConscript(
            app_root+'/SConscript.Cmb',
            src_dir= '#/'+cmb_src_root,
            exports={
                'env': env,
                'cart':'#/'+app_root,
                'target_path': '#/'+cmb_obj_path,
                'brf_path': '#/' + brf_src_root,
                'btf_path': '#/' + btf_src_root,
                'cmb_path': '#/' + app_root+'/../CMB/',
                'arch': optionArch,
            },
            variant_dir = cmb_obj_path,
            duplicate=0
        )
        env.Alias(alias='Cmb', target=Cmb)

  if optionCart == 'btf':
        for cfg in [JConfig[test_option] for test_option in sweep_list]:
            env, option_dict, tools_path_override, arch_config_override  = get_env_options(cfg, globals())

            btf_src_root = app_root+'/../BTF' #cfg['Btf']['Path']
            btf_obj_path = variant_root+'/'+optionArch+'/obj/Btf'

            Btf = env.SConscript(
                app_root+'/SConscript.Btf',
                src_dir= '#/'+btf_src_root,
                exports={
                    'env': env,
                    'cart':'#/'+app_root, #cfg['CART']['Path'],
                    'target': '#/'+btf_obj_path,
                #    'config_path':'#/' + jm_inc_path,
                #    'gen_path': jf_gen_path,
                    'arch': optionArch,
                    'namespace': optionNamespace,
                #    'jautogenCode': jautogenCode,
                #    'jconfigapp':  jconfigapp
                },
                variant_dir= btf_obj_path,
                duplicate=0
            )

            #env.Default(Btf)
            env.Alias(alias='Btf', target=Btf)
  if optionCart == 'bcf':
        for cfg in [JConfig[test_option] for test_option in sweep_list]:
            env, option_dict, tools_path_override, arch_config_override  = get_env_options(cfg, globals())

            bcf_src_root = app_root+'/../BCF' #cfg['Btf']['Path']
            bcf_obj_path = variant_root+'/'+optionArch+'/obj/Bcf'

            Bcf = env.SConscript(
                app_root+'/SConscript.Bcf',
                src_dir= '#/'+bcf_src_root,
                exports={
                    'env': env,
                    'cart':'#/'+app_root, #cfg['CART']['Path'],
                    'target': '#/'+bcf_obj_path,
                #    'config_path':'#/' + jm_inc_path,
                #    'gen_path': jf_gen_path,
                     'arch': optionArch,
                     'namespace': optionNamespace,
                #    'jautogenCode': jautogenCode,
                #    'jconfigapp':  jconfigapp
                },
                variant_dir= bcf_obj_path,
                duplicate=0
            )

            #env.Default(Btf)
            env.Alias(alias='Bcf', target=Bcf)

  if optionCart == 'brf':
        for cfg in [JConfig[test_option] for test_option in sweep_list]:
            env, option_dict, tools_path_override, arch_config_override  = get_env_options(cfg, globals())

            brf_src_root = app_root+'/../BRF' #cfg['Brf']['Path']
            brf_obj_path = variant_root+'/'+optionArch+'/obj/Brf'

            Brf = env.SConscript(
                app_root+'/SConscript.Brf',
                src_dir= '#/'+brf_src_root,
                exports={
                    'env': env,
                    'cart':'#/'+app_root, #cfg['CART']['Path'],
                    'target': '#/'+brf_obj_path,
                #    'config_path':'#/' + jm_inc_path,
                #    'gen_path': jf_gen_path,
                    'arch': optionArch,
                    'namespace': optionNamespace,
                #    'jautogenCode': jautogenCode,
                #    'jconfigapp':  jconfigapp
                },
                variant_dir= brf_obj_path,
                duplicate=0
            )

            #env.Default(Brf)
            env.Alias(alias='Brf', target=Brf)

  if optionCart == 'baf':

    # File list for Runtime, JFramework, Model
    env['RuntimeList'] = None
    env['FileListPath'] = {}
    env['JFrameworkList'] = {}
    env['ModelList'] = {}

    # JAutogenProbe json
    env['JAutogenProbe'] = {}
    env['JAutogenBafLite'] = {}

    if optionArch == 'gul':
        if optionPlatform in ['MRTA', 'MPIL', 'MHIL']:
            # Melville Single core
            melville_ezkit_single_runner(env)
        elif optionPlatform in ['MDRTA', 'MDPIL', 'MDHIL']:
            # Melville Dual core
            melville_ezkit_dual_runner(env)
        else:
            if optionRunner == 'sim':
                gul_sim_runner(env)
            elif optionRunner == 'ezkit':
                gul_ezkit_runner(env)
            else:
                gul_sim_runner(env)

    elif optionArch == 'gxp':
        if optionPlatform in ['SC594PIL','SC594HIL','SC594RTA', 'SC598PIL','SC598HIL','SC598RTA']:
            pass
        else:
            if optionRunner == 'sim':
                gxp_sim_runner(env)
            elif optionRunner == 'ezkit':
                gxp_ezkit_runner(env)
            else:
                gxp_sim_runner(env)

    elif optionArch == 'sharc':
        if optionRunner == 'sim':
            sharc_sim_runner(env)
        elif optionRunner == 'ezkit':
            sharc_ezkit_runner(env)
        else :
            sharc_sim_runner(env)

    # Default is simulate each JApplication
    optionMultiSim = False
    #On platforms that allow multiple executables, collect executables first.
    if optionApp and optionLib:
        if optionRunner == 'melville':
            optionMultiSim = True
        if (optionArch == 'gxp') and (optionPlatform in ['DPIL','DHIL','DRTA']):
            optionMultiSim = True

    # SConfig overrides
    optionPGO = cfg['Option']['pgo_option'] if 'pgo_option' in cfg['Option'] else None
    if (optionArch in ['gul','gxp']) and (optionPGO != None):
        if optionPGO == 'genpgo':
            sharc_genpgo_override(env)

    if (optionArch == 'gul') and (optionPlatform in ['SOMRTA']):
        gul_evsom_override(env, optionRunner, tools_path=tools_path_override, arch_config=arch_config_override)

    if (optionArch == 'gxp'):
        if (optionPlatform in ['SC594PIL','SC594HIL','SC594RTA']):
            sc594_evsom_override(env, optionRunner, tools_path=tools_path_override, arch_config=arch_config_override)
        elif (optionPlatform in ['SC598PIL','SC598HIL','SC598RTA']):
            sc598_evsom_override(env, optionRunner, tools_path=tools_path_override, arch_config=arch_config_override)

    # After all environment has been set add any xtra flags if they exist
    if ('xtra_flags' in cfg) and (optionArch in cfg['xtra_flags']):
        xcpp_flags = cfg['xtra_flags'][optionArch]['xcpp_flags']
        xlink_flags = cfg['xtra_flags'][optionArch]['xlink_flags']
        if xcpp_flags != None:
            env.Append(CPPFLAGS = xcpp_flags)
        if xlink_flags != None:
            env.Append(LINKFLAGS = xlink_flags)

    # Handler for dump=1 command
    if (ARGUMENTS.get('dump', 0)=='1') or (GetOption('dump_alias') == '1'):
        print (env.Dump())
    if (ARGUMENTS.get('dump', 0)=='2') or (GetOption('dump_alias') == '2'):
        pp = pprint.PrettyPrinter(indent=1)
        pp.pprint(JConfig)
    if (ARGUMENTS.get('dump', 0)=='3') or (GetOption('dump_alias') == '3'):
        pp = pprint.PrettyPrinter(indent=1)
        for idx, jconfig in enumerate(JConfig, start=1):
            print(" ===== Test "+str(idx)+" ========")
            pp.pprint(jconfig['JFramework'][0]['Test']['Name'])
            pp.pprint(jconfig['JFramework'][0]['Test']['Param'])
            pp.pprint(jconfig['JFramework'][0]['JModule'][0]['TalariaModel']['Parameters'])
    if ARGUMENTS.get('dump', 0) or (GetOption('dump_alias') in ['1','2','3','test']):
        exit()

    if 'TOOLSPATH' in env:
        if not os.path.isdir(env['TOOLSPATH']):
            if optionLib:
                print("Error: Required tool:" + env['TOOLSPATH'] + " is missing")
                exit()

    add_builders(env, optionPGO)
    env['APP_ROOT']=app_root

    JPrebuilds=[]
    JMil=[]
    JControl=None
    xml_files=[]
    # Build each jmodule configuration header files
    if ('BCF' in cfg) and (cfg['BCF']['Name'] is not None) and 'Parameters' in cfg['BCF'] and cfg['BCF']['Parameters'] and not optionModel:
        bcf_cfg = env.BcfConfig(target='#/'+variant_root+'/'+env['PROCESSOR']+'/inc/BCF_Config_'+cfg['BCF']['Name']+'.h',
                source=None,  Bcf = cfg['BCF'])

        jframework=cfg['JFramework'][0] # first framework
        jmodule=jframework['JModule'][0]  # first jmodule
        if ('TalariaModel' in jmodule) and jmodule['TalariaModel']:
            talaria_model = jmodule['TalariaModel']
            if os.path.exists(talaria_model['Path']+'/JConfig.py'):
                env.Depends(bcf_cfg, talaria_model['Path']+'/JConfig.py')
            if os.path.exists(talaria_model['Path']+'/../JConfig.py'):
                env.Depends(bcf_cfg, talaria_model['Path']+'/../JConfig.py')
            if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                env.Depends(bcf_cfg, talaria_model['Path']+'/JConfigApp.py')
            if os.path.exists(talaria_model['Path']+'/../JConfigApp.py'):
                env.Depends(bcf_cfg, talaria_model['Path']+'/../JConfigApp.py')
            if ('Generator' in cfg['BCF']) and cfg['BCF']['Generator']:
                bcf_src_root = variant_root+'/' + optionArch + '/src/Bcf'
                bcf_gen_path = bcf_src_root
                xml_path = '#/'+variant_root+'/slx/code'
                rtc = xml_path + '/' + talaria_model['Name'] + '_RTC_Payloads.xml'
                top = xml_path + '/' + talaria_model['Name'] + '_TOP_Tokens.xml'
                xml_files=[rtc, top]
                cfg['BCF']['GenPath'] = bcf_gen_path
                JControl = env.JControlGenerate(target=bcf_gen_path+'/Bam_Proxys/inc/OJBamProxy.h', source=xml_files,  Bcf = cfg['BCF'])
                env.Depends(JControl, bcf_cfg)
                if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                    env.Depends(JControl, talaria_model['Path']+'/JConfigApp.py')
                if os.path.exists(talaria_model['Path']+'/JConfig.py'):
                    env.Depends(JControl, talaria_model['Path']+'/JConfig.py')
        JPrebuilds.append(bcf_cfg)

    if 'JFramework' in cfg :
        jframework=cfg['JFramework'][0] # first framework
        jmodule=jframework['JModule'][0]  # first jmodule
        if ('TalariaModel' in jmodule) and jmodule['TalariaModel']:
            if not os.path.isfile(env['MATLAB'].replace('"','')):
                print("Info: Required tool:" + env['MATLAB'] + " is missing")

        talaria_sys = jmodule and ('TalariaModel' in jmodule) and jmodule['TalariaModel'] and ('SysModel' in jmodule['TalariaModel']) and (jmodule['TalariaModel']['SysModel'])

# Multi-core Talaria build
        if talaria_sys:
            jm_test_folder=None
            jm_test_files = []
            talaria_model = jmodule['TalariaModel']
            test_case = talaria_model['Test']['Case']

            code_gen_folder = '#/'+variant_root+'/slx/'
            if ('Parameters' in talaria_model) and talaria_model['Parameters']:
                jsimcfg = env.SimulinkConfig(target=code_gen_folder+'/'+talaria_model['Name']+'Cfg.m', source=None,  JModule = jmodule['TalariaModel'])
                if os.path.exists(talaria_model['Path']+'/JConfig.py'):
                    env.Depends(jsimcfg, talaria_model['Path']+'/JConfig.py')
                if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                    env.Depends(jsimcfg, talaria_model['Path']+'/JConfigApp.py')
            if 'Test' in talaria_model :
                jm_src_path = variant_root+'/slx/'
                jm_test_folder='#/' + jm_src_path +'/test/'+test_case
                jm_test_files = []
                for suffix in talaria_model['Test']['Gen']:
                    jm_test_files.append(jm_test_folder +'/' + suffix)

                test_case_tolerance = talaria_model['Test']['Tolerance']  if ('Tolerance' in talaria_model['Test']) and talaria_model['Test']['Tolerance'] else None

            code_gen_parts_folder = code_gen_folder+'/parts'
            if ('ProbePoints' in talaria_model) and talaria_model['ProbePoints']:
                probe_points_target = []
                probe_points_target.append(code_gen_folder+'/'+talaria_model['Name']+'ProbePoints.m')
                probe_points_target.append(code_gen_parts_folder+'/'+talaria_model['Name']+'ProbePoints.m')
                jprobepoints = env.SimulinkProbePoints(target=probe_points_target, source=None,  ProbePoints = talaria_model['ProbePoints'])
            else:
                jprobepoints = None

            code_gen_files = []
            if ('CORE_ID' in jframework) and ('SOC_ID' in jframework) :
                codegen_folder_prefix = code_gen_folder +'code/' + talaria_model['Name'] + '_'+ jframework["SOC_ID"] + '_'+ jframework["CORE_ID"] + '_ert_shrlib_rtw/'
                codegen_file_prefix = talaria_model['Name'] + '_' + jframework["SOC_ID"] + '_'+ jframework["CORE_ID"]
            else:
                codegen_folder_prefix = code_gen_folder +'code/' + talaria_model['Name'] + '_Target_ert_shrlib_rtw/'
                codegen_file_prefix = talaria_model['Name'] + '_Target'
            if 'prefix' in talaria_model and talaria_model['prefix'] == 'default':
                name_prefix = codegen_folder_prefix+codegen_file_prefix
            else:
                name_prefix = codegen_folder_prefix
            if 'codegen' in talaria_model and talaria_model['codegen']:
                for suffix in talaria_model['codegen']:
                    if isinstance(suffix, dict):
                        code_gen_files.append( name_prefix + suffix['file_name'] )
                    elif isinstance(suffix, list):
                        code_gen_files.append( name_prefix + suffix[0] )
                    else:
                        code_gen_files.append(name_prefix + suffix)
            else:
                exclude = [f'{codegen_folder_prefix}/{x}' for x in talaria_model['exclude']] if ('exclude' in talaria_model and talaria_model['exclude'] is not None) else [];
                code_gen_files, code_gen_incs = codeGenFiles(codegen_folder_prefix, exclude, Dir(app_root))

            control_files = []
            for suffix in talaria_model['ctrlgen']:
                if suffix != None:
                    control_files.append(code_gen_folder +'code/' + talaria_model['CtrlGenName'] + '_' + suffix)

            # Check if Partitioning is in JConfig
            if 'Partitioning' in cfg.keys() and cfg['Partitioning']:
                atomiclist_path = app_root+'/'+cfg['JFramework'][0]['JModule'][0]['TalariaModel']['ProjectPath']+'/'
                atomiclist_path += cfg['JFramework'][0]['JModule'][0]['TalariaModel']['Name'] + '_atomicList.m'
                # Check if atomicList.m exits in Talaria project folder
                if os.path.isfile(atomiclist_path):
                    os.remove(atomiclist_path)
                # Generate new atomicList.m
                jatomiclistcfg = env.AtomicListConfig(target=code_gen_folder+'/'+talaria_model['Name']+'_atomicList.m', source=None, Partitioning = cfg['Partitioning'])
                env.Alias('jatomiclist', jatomiclistcfg)
            else:
                # delete the atomiclist file generated before if it exists
                atomiclist_path = code_gen_folder+'/'+talaria_model['Name']+'_atomicList.m'
                if os.path.isfile(atomiclist_path):
                    os.remove(atomiclist_path)
                jatomiclistcfg = None

            if optionMil :
                optionArch = 'mil'
            if optionModel:
                model_target = env.SConscript(
                    app_root+'/SConscript.Model',
                    src_dir='#/'+talaria_model['Path'],
                    exports={'env': env,
                        'project_path': '#/'+talaria_model['ProjectPath'],
                        'model_path': '#/'+talaria_model['Path'],
                        'target': code_gen_files + control_files,
                        'code_gen_folder':code_gen_folder,
                        'test_folder':jm_test_folder if optionApp else False,
                        'talaria_model': talaria_model,
                        'jtest_path':  app_root + '/Tools/JTest',
                        'test_bin': jm_test_files,
                        'arch': optionArch,
                        },
                    duplicate=0)
                if JControl != None:
                    env.Depends(JControl, model_target)
                env.Depends(model_target, jsimcfg)
                if jprobepoints != None:
                    env.Depends(model_target, jprobepoints)
                env.Depends(model_target, jatomiclistcfg)
                JPrebuilds.append(model_target)
                JMil.append(model_target)
            else:
                model_target = None
                JPrebuilds.append(jsimcfg)
                if jprobepoints != None:
                    JPrebuilds.append(jprobepoints)

        MultiSimApps = []
        MultiSimTests = []
        AppReports = []
        SimReports = []
        Simulations = []
        for jf_idx, jframework in enumerate(cfg['JFramework']):

            if optionArch != jframework['arch']:
                continue

            # Build Runtime
            runtime_dir = jframework['Name'] if jframework['Name'] else 'Baf'

            OpenJADE = env.SConscript(
                app_root+'/SConscript.Runtime',
                src_dir= '#/'+app_root+'/'+cfg['Runtime']['Path'],
                exports={'env': env,
                    'target': '#/'+variant_root+'/${PROCESSOR}/lib/'+runtime_dir+'/OpenJADE',
                    'runtime': cfg['Runtime'] if ('Runtime' in cfg) else None,
                    'option':  cfg['Runtime']['Option'] if ('Runtime' in cfg) and ('Option' in cfg['Runtime']) else None,
                    'source_dir': '#/'+app_root+'/'+cfg['Runtime']['Path']
                },
                variant_dir = variant_root + '/${PROCESSOR}/obj/Runtime/'+runtime_dir+'/',
                duplicate=0)

            # If jframework['Name'] is not specified we cannot proceed further. So just build runtime
            if jframework['Name'] == None:
                continue

        # Build each JFramework and JModule adapters

            jframework_test_param = str(jframework['Test']['Param']) if ('Test' in jframework) and jframework['Test'] and ('Param' in jframework['Test']) and jframework['Test']['Param'] else ""
            jm_obj_path = variant_root+'/${PROCESSOR}/obj/' + jframework['Name']+jframework_test_param
            jm_inc_path = variant_root+'/${PROCESSOR}/inc/' + jframework['Name']+jframework_test_param
            jm_lib_path = variant_root+'/${PROCESSOR}/lib/' + jframework['Name']+jframework_test_param
            fw_lib_path = variant_root+'/${PROCESSOR}/lib/' + jframework['Name']+jframework_test_param
            fw_obj_path = variant_root+'/${PROCESSOR}/obj/' + jframework['Name']+jframework_test_param
            if ('CModel' in jmodule) and jmodule['CModel'] :
                c_lib_path  = variant_root+'/${PROCESSOR}/lib/' + jmodule['Name']+jframework_test_param

            if ('Generator' in jframework) and (jframework['Generator']):
                jf_gen_path = variant_root+'/' + optionArch + '/src/' + jframework['Name']+jframework_test_param
                jf_src_root = jf_gen_path+'/'+jframework['Name']
            else:
                jf_src_root = app_root+'/'+jframework['Path']
                jf_gen_path = None

            jconfigapp = None
            crl_files = []
            drivers_files = []
            JFramework = []
            crl_folder = None
            drivers_folder = None
            misc_root = None

            jmodule=jframework['JModule'][0]  # first jmodule
            if ('TalariaModel' in jmodule) and jmodule['TalariaModel']:
                talaria_model = jmodule['TalariaModel']
                if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                    jconfigapp = talaria_model['Path']+'/JConfigApp.py'

                if (('crl' in talaria_model) and (optionArch in talaria_model['crl']) and talaria_model['crl'][optionArch] and
                        ('crlPath' in talaria_model) and (optionArch in talaria_model['crlPath']) and talaria_model['crlPath'][optionArch]):
                    crl_folder=talaria_model['crlPath'][optionArch]
                    for suffix in talaria_model['crl'][optionArch]:
                        crl_files.append(suffix)
                else:
                    crl_folder=None

                if (('driversFiles' in talaria_model) and (optionArch in talaria_model['driversFiles']) and talaria_model['driversFiles'][optionArch] and
                        ('driversPath' in talaria_model) and (optionArch in talaria_model['driversPath']) and talaria_model['driversPath'][optionArch]):
                    drivers_folder=talaria_model['driversPath'][optionArch]
                    for suffix in talaria_model['driversFiles'][optionArch]:
                        drivers_files.append(suffix)
                else:
                    drivers_folder=None

                if ('misc_root' in talaria_model):
                    misc_root = talaria_model['misc_root']
                else:
                    misc_root = None

            jautogenCode = JAutogenCode(genConfig=cfg)
            jftarget = env.SConscript(
                app_root+'/SConscript.JFramework',
                src_dir= '#/'+jf_src_root,
                exports={'env': env,
                    'runtime':'#/'+app_root+'/'+cfg['Runtime']['Path'],
                    'jframework':jframework,
                    'target': '#/'+fw_lib_path,
                    'config_path':'#/' + jm_inc_path,
                    'gen_path': jf_gen_path,
                    'arch': optionArch,
                    'jautogenCode': jautogenCode,
                    'jconfigapp':  jconfigapp
                },
                variant_dir= fw_obj_path,
                duplicate=0)
            JFramework.append(jftarget)

            if 'ReadXML' in JConfig[0] and JConfig[0]['ReadXML'] == True:
                xml_builder = env.SConscript(
                    app_root+'/SConscript.ReadXml',
                    src_dir= app_root+'/'+jframework['Path'],
                    exports={'env': env,
                    'jframework':jframework}
                )
                env.Requires(JFramework, xml_builder)
                env.Depends(JFramework, xml_builder)

            if crl_folder != None:
                jcrllib = env.SConscript(
                    app_root+'/SConscript.Crl',
                    src_dir =  crl_folder,
                    exports={'env': env,
                        'crl_folder':crl_folder,
                        'drivers_folder':drivers_folder,
                        'crl_files':crl_files,
                        'crl_target_folder':'#/' +  fw_obj_path + '/crl',
                        'misc_root':misc_root,
                        'option': jframework['Option']['CRL'] if ('Option' in jframework) and (jframework['Option']) and ('CRL' in jframework['Option']) and (jframework['Option']['CRL']) else None,
                        'target':'#/' + fw_lib_path + '/crl'},
                    variant_dir='#/' +  fw_obj_path + '/crl',
                    duplicate=False)
                JFramework.append(jcrllib)

            if drivers_folder != None:
                jdriverslib = env.SConscript(
                    app_root+'/SConscript.Drivers',
                    src_dir =  drivers_folder,
                    exports={'env': env,
                        'drivers_folder':drivers_folder,
                        'drivers_files':drivers_files,
                        'drivers_target_folder':'#/' +  fw_obj_path + '/drivers',
                        'misc_root':misc_root,
                        'runtime':'#/'+app_root+'/'+cfg['Runtime']['Path'],
                        'option': jframework['Option']['Drivers'] if ('Option' in jframework) and (jframework['Option']) and ('Drivers' in jframework['Option']) and (jframework['Option']['Drivers']) else None,
                        'target':'#/' + fw_lib_path + '/drivers'},
                    variant_dir='#/' +  fw_obj_path + '/drivers',
                    duplicate=0)
                JFramework.append(jdriverslib)

        # Build each JModule in each JFramework
            JTest=[]
            JModule=[]
            fw_test_folder = None
            if not talaria_sys:
                if ('Test' in jframework) and jframework['Test'] and ('Case' in jframework['Test']):
                    test_case = jframework['Test']['Case']
                else:
                    test_case = 'test'
            for jmodule in jframework['JModule']:
                test_target = None
                jm_test_files = []

                if ('LibModel' in jmodule) and jmodule['LibModel'] :
                    JModuleLib = app_root+'/'+jframework['Path']+'/'+jmodule['Path']+jmodule['LibModel']['ArchToLib'][optionArch]
                    JModule.append(JModuleLib)

                if ('CModel' in jmodule) and jmodule['CModel'] :
                    CModel = env.SConscript(
                        app_root+'/SConscript.CFile',
                        src_dir =  app_root+'/'+jframework['Path']+'/'+jmodule['Path'],
                        exports={'env': env,
                        'target': '#/'+c_lib_path,
                        'jmodule':jmodule}
                    )
                    JModule.append(CModel)
                if talaria_sys:
                    talaria_model = jmodule['TalariaModel']
                else:
                    # Build each Talaria model
                    talaria_model = None
                    model_target = None
                    test_target = None
                    jm_test_folder = None
                    code_gen_folder = None

                    if not (('TalariaModel' in jmodule) and jmodule['TalariaModel']):
                        fw_test_folder =variant_root+'/slx/test/'+jframework['Name']+'_test'
                        jm_test_files  = ['#/tests/request.ojt','#/tests/audioIn.bin','#/tests/audioRef.bin']
                        fw_test_files = []
                        fw_ascii_files = []
                        for suffix in ['request.ojx','audioIn.bin','audioRef.bin','audioOut.bin']:
                            fw_test_files.append('#/'+fw_test_folder +'/' + suffix)
                            fw_ascii_files.append('#/'+fw_test_folder +'/' + suffix.replace('.','_')+'.h')
                        test_target = env.ConvertControlRequest(
                                    target = fw_test_files + [Dir(fw_test_folder)],
                                    source = [  app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                                                jm_test_files,
                                    arch = optionArch,
                                    platform = optionPlatform
                                    )
                        JTest.append(fw_test_files)
                        JTest.append(test_target)

                        ascii_target = env.GenerateAsciiVectors( source = fw_test_files, target = fw_ascii_files )
                        env.Depends(ascii_target, test_target)
                        JTest.append(ascii_target)
                        continue
                    else:
                        talaria_model = jmodule['TalariaModel']
                        jm_src_path = variant_root+'/slx/'
                        jm_parts_path = jm_src_path +'/parts'
                        if 'Parameters' in jmodule['TalariaModel']:
                            jsimcfg = env.SimulinkConfig(target='#/'+jm_src_path+'/'+talaria_model['Name']+'Cfg.m', source=None,  JModule = jmodule['TalariaModel'])
                        else:
                            jsimcfg = env.SimulinkConfig(target='#/'+jframework['Path']+'/'+jmodule['Path']+'/'+talaria_model['Path']+'/'+talaria_model['Name']+'Cfg.m', source=None,  JModule = jmodule)
                        if os.path.exists(talaria_model['Path']+'/JConfig.py'):
                            env.Depends(jsimcfg, talaria_model['Path']+'/JConfig.py')
                        if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                            env.Depends(jsimcfg, talaria_model['Path']+'/JConfigApp.py')
                        if os.path.exists(talaria_model['Path']+'/../JConfigApp.py'):
                            env.Depends(jsimcfg, talaria_model['Path']+'/../JConfigApp.py')

                        if ('ProbePoints' in talaria_model) and talaria_model['ProbePoints']:
                            probe_points_target = []
                            if 'Parameters' in jmodule['TalariaModel']:
                                probe_points_target.append('#/'+jm_src_path+'/'+talaria_model['Name']+'ProbePoints.m')
                                probe_points_target.append('#/'+jm_parts_path+'/'+talaria_model['Name']+'ProbePoints.m')
                            else:
                                probe_points_target.append('#/'+jframework['Path']+'/'+jmodule['Path']+'/'+talaria_model['Path']+'/'+talaria_model['Name']+'ProbePoints.m')
                                probe_points_target.append('#/'+jframework['Path']+'/'+jmodule['Path']+'/parts/'+talaria_model['Name']+'ProbePoints.m')
                            jprobepoints = env.SimulinkProbePoints(target=probe_points_target, source=None,  ProbePoints = talaria_model['ProbePoints'])
                        else:
                            jprobepoints = None

                        code_gen_folder = '#/' + jm_src_path
                        if 'Test' in talaria_model :
                            jm_test_folder='#/' + jm_src_path +'/test/'+test_case
                            for suffix in talaria_model['Test']['Gen']:
                                jm_test_files.append(jm_test_folder +'/' + suffix)

                            test_case_tolerance = talaria_model['Test']['Tolerance']  if ('Tolerance' in talaria_model['Test']) and talaria_model['Test']['Tolerance'] else None
                        codegen_folder_prefix = code_gen_folder +'code/' + talaria_model['Name'] + '_Target_ert_shrlib_rtw/'
                        codegen_file_prefix = talaria_model['Name'] + '_Target'

                        code_gen_files = []
                        if 'prefix' in talaria_model and talaria_model['prefix'] == 'default':
                            name_prefix = codegen_folder_prefix+codegen_file_prefix
                        else:
                            name_prefix = codegen_folder_prefix

                        if 'codegen' in talaria_model and talaria_model['codegen']:
                            for suffix in talaria_model['codegen']:
                                if isinstance(suffix, dict):
                                    code_gen_files.append( name_prefix + suffix['file_name'] )
                                elif isinstance(suffix, list):
                                    code_gen_files.append( name_prefix + suffix[0] )
                                else:
                                    code_gen_files.append(name_prefix + suffix)
                        else:
                            exclude = [f'{codegen_folder_prefix}/{x}' for x in talaria_model['exclude']] if ('exclude' in talaria_model and talaria_model['exclude'] is not None) else [];
                            code_gen_files, code_gen_incs = codeGenFiles(codegen_folder_prefix, exclude, Dir(app_root))
 
                        control_files = []
                        if 'ctrlgen' in talaria_model:
                            for suffix in talaria_model['ctrlgen']:
                                if suffix != None:
                                    control_files.append(code_gen_folder +'code/' + talaria_model['CtrlGenName'] + '_' + suffix)

                        if optionMil :
                            optionArch = 'mil'
                        if optionModel:
                            xml_path = '#/'+variant_root+'/slx/code'
                            rtc = xml_path + '/' + talaria_model['Name'] + '_RTC_Payloads.xml'
                            top = xml_path + '/' + talaria_model['Name'] + '_TOP_Tokens.xml'
                            xml_files=[rtc, top]
                            model_target = env.SConscript(
                                app_root+'/SConscript.Model',
                                src_dir='#/'+talaria_model['Path'],
                                exports={'env': env,
                                    'project_path': '#/'+talaria_model['ProjectPath'],
                                    'model_path': '#/'+talaria_model['Path'],
                                    'target': code_gen_files + control_files + xml_files,
                                    'code_gen_folder':code_gen_folder,
                                    'test_folder':jm_test_folder if optionApp else False,
                                    'talaria_model': talaria_model,
                                    'jtest_path': app_root+'/Tools/JTest',
                                    'test_bin': jm_test_files,
                                    'arch': optionArch,
                                    },
                                duplicate=0)
                            if JControl != None:
                                env.Depends(model_target, JControl)
                            env.Depends(model_target, jsimcfg)
                            if jprobepoints != None:
                                env.Depends(model_target, jprobepoints)
                            JPrebuilds.append(model_target)
                            JMil.append(model_target)
                        else:
                            model_target=None
                            JPrebuilds.append(jsimcfg)
                            if jprobepoints != None:
                                JPrebuilds.append(jprobepoints)

                if jm_test_folder and ('Test' in jframework) and jframework['Test']:
                    fw_test_folder =variant_root+'/slx/test/'+jframework['Name']+'_test/'+test_case
                    jm_test_folder= jm_src_path +'/test/'+test_case

                    fw_test_files = []
                    fw_ascii_files = []
                    jm_test_files = []
                    mips_target = None
                    if 'Gen' in jframework['Test']:
                      for suffix in jframework['Test']['Gen']:
                        fw_test_files.append('#/'+fw_test_folder +'/' + suffix)
                        fw_ascii_files.append('#/'+fw_test_folder +'/' + suffix.replace('.','_')+'.h')
                      for suffix in jframework['Test']['Src']:
                        jm_test_files.append(jm_test_folder +'/' + suffix)

                      mips_target = env.LogExecRequest(
                                    target = [jm_test_files[0].replace('.ojt', '.mjt')],
                                    source = [jm_test_files[0]],
                                    arch = optionArch
                                    )

                      env.Depends(mips_target, model_target)

                      jm_test_files[0] = jm_test_files[0].replace('.ojt', '.mjt')


                    if 'Skip' in talaria_model['Test'] and talaria_model['Test']['Skip']:
                        pass # skip so only one jmodule test in the fw test, for now
                    else:
                        if(('Correction' in talaria_model['Test']) and talaria_model['Test']['Correction']):
                            test_target = env.ConvertControlRequestNCorrect(
                                            target = fw_test_files + [Dir(fw_test_folder)],
                                            source = [ app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                                                    [ talaria_model['Path']+'/'+talaria_model['Test']['Correction']] +
                                                    jm_test_files +
                                                    control_files,
                                            JModule = jmodule,
                                            JFramework = jframework,
                                            arch = optionArch,
                                            platform = optionPlatform
                                                    )
                        else:
                            test_target = env.ConvertControlRequest(
                                            target = fw_test_files + [Dir(fw_test_folder)],
                                            source = [  app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                                                    jm_test_files +
                                                    control_files,
                                            JModule = jmodule,
                                            JFramework = jframework,
                                            arch = optionArch,
                                            platform = optionPlatform
                                                    )
                        env.Depends(test_target, mips_target)
                        JTest.append(test_target)

                        ascii_target = env.GenerateAsciiVectors( source = fw_test_files, target = fw_ascii_files )
                        env.Depends(ascii_target, test_target)
                        JTest.append(ascii_target)

                else : # use defaults
                    fw_test_folder =variant_root+'/slx/test/'+jframework['Name']+'_test'
                    jm_test_files  = []
                    fw_test_files = []
                    fw_ascii_files = []

                    if ('DefaultTest' in jframework) and jframework['DefaultTest']:
                        jm_test_files = ['#/tests/' + i for i in jframework['DefaultTest']['Src']]
                        for suffix in jframework['DefaultTest']['Gen']:
                            fw_test_files.append('#/'+fw_test_folder +'/' + suffix)
                            fw_ascii_files.append('#/'+fw_test_folder +'/' + suffix.replace('.','_')+'.h')
                    else:
                        jm_test_files = ['#/tests/request.ojt','#/tests/audioIn.bin','#/tests/audioRef.bin']
                        for suffix in ['request.ojx','audioIn.bin','audioRef.bin','audioOut.bin']:
                            fw_test_files.append('#/'+fw_test_folder +'/' + suffix)
                            fw_ascii_files.append('#/'+fw_test_folder +'/' + suffix.replace('.','_')+'.h')

                    #for suffix in ['request.ojx','audioIn.bin','audioRef.bin','audioOut.bin']:
                    #for suffix in jframework['DefaultTest']['Gen']:
                    #    fw_test_files.append('#/'+fw_test_folder +'/' + suffix)
                    test_target = env.ConvertControlRequest(
                                    target = fw_test_files + [Dir(fw_test_folder)],
                                    source = [  app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                                                jm_test_files,
                                    arch = optionArch,
                                    platform = optionPlatform
                                    )
                    JTest.append(fw_test_files)
                    JTest.append(test_target)

                    ascii_target = env.GenerateAsciiVectors( source = fw_test_files, target = fw_ascii_files )
                    env.Depends(ascii_target, test_target)
                    JTest.append(ascii_target)

        # Build each jmodule configuration header files
                jmodcfg = None
                if ('Generator' not in jframework) or (not jframework['Generator']):
                    if 'Parameters' in jmodule:
                        jmodcfg = env.JModConfig(target='#/'+jm_inc_path+'/JModule_Config_'+jmodule['Name']+'.h', source=None,  JModule = jmodule)
                        if talaria_model:
                            if os.path.exists(talaria_model['Path']+'/JConfig.py'):
                                 env.Depends(jmodcfg, talaria_model['Path']+'/JConfig.py')
                        if (talaria_model) and (os.path.exists(talaria_model['Path']+'/JConfigApp.py')): env.Depends(jmodcfg, talaria_model['Path']+'/JConfigApp.py')
                        JPrebuilds.append(jmodcfg)

        # Build each JModule repo in each JFramework
                code_gen_files = []
                if talaria_model:
                    if ('CORE_ID' in jframework) and ('SOC_ID' in jframework) :
                        codegen_folder_prefix = code_gen_folder +'code/' + talaria_model['Name'] + '_'+ jframework["SOC_ID"] + '_'+ jframework["CORE_ID"] + '_ert_shrlib_rtw/'
                        codegen_file_prefix = talaria_model['Name'] + '_' + jframework["SOC_ID"] + '_'+ jframework["CORE_ID"]
                    else:
                        codegen_folder_prefix = code_gen_folder +'code/' + talaria_model['Name'] + '_Target_ert_shrlib_rtw/'
                        codegen_file_prefix = talaria_model['Name'] + '_Target'

                    if 'prefix' in talaria_model and talaria_model['prefix'] == 'default':
                        name_prefix = codegen_folder_prefix+codegen_file_prefix
                    else:
                        name_prefix = codegen_folder_prefix

                    if 'codegen' in talaria_model and talaria_model['codegen']:
                        for suffix in talaria_model['codegen']:
                            if isinstance(suffix, dict):
                                suffix['file_name'] = name_prefix + suffix['file_name']
                            elif isinstance(suffix, list):
                                suffix[0] = name_prefix + suffix[0]
                            else:
                                suffix = name_prefix + suffix
                            code_gen_files.append(suffix)
                    else:
                        exclude = [f'{codegen_folder_prefix}/{x}' for x in talaria_model['exclude']] if ('exclude' in talaria_model and talaria_model['exclude'] is not None) else [];
                        code_gen_files, code_gen_incs = codeGenFiles(codegen_folder_prefix, exclude, Dir(app_root))

                    talaria_model_path = '#/'+talaria_model['Path']
                else:
                    talaria_model_path = None

                if (optionArch == 'gxp') and (optionPlatform in ['DPIL','DHIL','DRTA']):
                    InterCoreXfer = jframework['Option']['Platform']['InterCoreXfer'] if ('Option' in jframework) and ('Platform' in jframework['Option']) and ('InterCoreXfer' in jframework['Option']['Platform']) else 'SharedMem'
                    if (InterCoreXfer == 'SharedMem'):
                        pingpongstruct_src = None
                        for index, item in enumerate(code_gen_files):
                            if 'ping_pong_struct.c' in item.name:
                                pingpongstruct_name = item.name
                                pingpongstruct_src = item
                                pingpongstruct_xml = item.File(pingpongstruct_name.replace('ping_pong_struct.c', 'PingPongStruct.xml'))
                                pingpongstruct_tgt = item.File('ojt/'+pingpongstruct_name)
                                code_gen_files[index] = pingpongstruct_tgt

                        if pingpongstruct_src != None:
                            env.PingPongStructPostProc(
                                target = pingpongstruct_tgt,
                                source = [pingpongstruct_src, pingpongstruct_xml],
                                model_name = talaria_model['Name'],
                                SOC_ID = jframework['SOC_ID'] if ('SOC_ID' in jframework) else None,
                                CORE_ID = jframework['CORE_ID'] if ('CORE_ID' in jframework) else None
                            )
                jmtarget = env.SConscript(
                    app_root+'/SConscript.JModule',
                    src_dir =  '#/'+app_root+'/'+jframework['Path']+'/'+jmodule['Path'],
                    exports={'env': env,
                        'talaria_model_path': talaria_model_path,
                        'talaria_model': talaria_model,
                        'codegen_folder_prefix':codegen_folder_prefix,
                        'code_gen_folder':code_gen_folder,
                        'code_gen_files':code_gen_files,
                        'code_gen_incs':code_gen_incs,
                        'crl_folder':crl_folder,
                        'drivers_folder':drivers_folder,
                        'runtime':'#/'+app_root+'/'+cfg['Runtime']['Path'],
                        'misc_root':misc_root,
                        'jmodule': jmodule,
                        'target':'#/' + fw_lib_path + jmodule['Name'],
                        'jframework': jframework,
                        'jautogenProbePlatform' : JAutogenProbe.DSPHEXAGON if optionArch in ['hexagon','hexagon_v66','hexagon_v73'] else JAutogenProbe.DSPGUL
                    },
                    variant_dir= fw_obj_path + jmodule['Name'],
                    duplicate=0)
                if 'ReadXML' in JConfig[0] and JConfig[0]['ReadXML'] == True:
                    env.Depends(jmtarget, xml_builder)

                env.Alias(jframework['Name']+jmodule['Name'], jmtarget)
                if(jmodcfg) :
                    env.Depends(jmtarget, jmodcfg)
                if(model_target) :
                    env.Depends(jmtarget, model_target)
                JModule.append(jmtarget)
                if os.path.exists(talaria_model['Path']+'/JConfigApp.py'):
                    env.Depends(jmtarget, talaria_model['Path']+'/JConfigApp.py')

                #ccesProject = env.CCESProject(
                #    target=['#/'+variant_root+'/${PROCESSOR}/projects/JModule_core'+str(jf_idx+1)+'/.cproject',
                #            '#/'+variant_root+'/${PROCESSOR}/projects/Model_core'+str(jf_idx+1)+'/.cproject',
                #            '#/'+variant_root+'/${PROCESSOR}/projects/Model_core'+str(jf_idx+1)+'/.project',
                #            '#/'+variant_root+'/${PROCESSOR}/projects/Model_core'+str(jf_idx+1)+'/.project'],
                #    source= [ app_root+'/'+'\\Tools\\JConfig\\cces_gen\\JModule_core1\\.cproject',
                #              app_root+'/'+'\\Tools\\JConfig\\cces_gen\\Model_core1\\.cproject',
                #              app_root+'/'+'\\Tools\\JConfig\\cces_gen\\JModule_core1\\.project',
                #              app_root+'/'+'\\Tools\\JConfig\\cces_gen\\Model_core1\\.project'],
                #    JModule=jmodule,
                #    arch=optionArch,
                #    path=app_root + '/' + cfg['Runtime']['Path'],
                #    outPath=variant_root +'/'+optionArch+'/projects/',
                #    core=str(jf_idx+1),
                #    srcPath=jf_src_root,
                #    codeGenFolder=codegen_folder_prefix[2:]
                #)
                #if optionArch in ['sharc', 'gul', 'gxp']:
                #    env.Depends(jmtarget, ccesProject)

            if optionApp and optionLib and ('Platform' in cfg):
                moduleName = ''
                if 'optionModule' in option_dict and option_dict['optionModule'] != None:
                    moduleName = option_dict['optionModule']
                modulePath = '#/'+app_root+'/Module/'+ moduleName
                # Build Platform
                Platform = env.SConscript(
                   app_root+'/SConscript.Platform',
                   src_dir= '#/'+app_root+'/'+cfg['Platform'][optionArch][optionPlatform],
                   exports={'env': env ,
                        'target':  '#/'+variant_root+'/${PROCESSOR}/lib/Platform/'+jframework['Name']+'/'+optionPlatform,
                        'runtime':'#/'+app_root+'/'+cfg['Runtime']['Path'],
                        'platform':'#/'+app_root+'/'+cfg['Platform'][optionArch][optionPlatform],
                        'OptArch': optionArch,
                        'OptPlatform': optionPlatform,
                        'OptRTOS': cfg['Option']['rtos'] if ('rtos' in cfg['Option']) and (cfg['Option']['rtos']) else None,
                        'modulePath': modulePath,
                        'jframework': jframework,
                        'SOC_ID': jframework['SOC_ID'] if ('SOC_ID' in jframework) else None,
                        'CORE_ID': jframework['CORE_ID'] if ('CORE_ID' in jframework) else None,
                        'code_gen_folder':code_gen_folder,
                        'talaria_model':talaria_model,
                        'base_rate_period': jframework['Generator'].GetRatePeriod(name=jframework['Name']) if ('Generator' in jframework) and (jframework['Generator']) else None,
                        'option': jframework['Option']['Platform'] if ('Option' in jframework) and (jframework['Option']) and ('Platform' in jframework['Option']) and (jframework['Option']['Platform']) else None},
                   variant_dir = variant_root+'/${PROCESSOR}/obj/Platform/'+jframework['Name']+'/'+optionPlatform,
                   duplicate=0)

        # Merge Runtime, JFramework and JModule Libraries into BAF library (One BAF per JFramework)
            if optionArch in ['hifi4', 'hifi5', 'qnx_nto_aarch64']:
                BAFLst = env.LibLstMerge(
                    target=['#/'+variant_root+'/${PROCESSOR}/lib/BAF/'+jframework_test_param+'/'+env['LIBPREFIX']+jframework['Name']],
                    source = [JModule, OpenJADE, JFramework],
                    arch = optionArch
                    )

                BAF = env.LibMergeAlt(
                    target=['#/'+variant_root+'/${PROCESSOR}/lib/BAF/'+jframework_test_param+'/'+env['LIBPREFIX']+jframework['Name']],
                    source = [BAFLst, JModule, OpenJADE, JFramework])
                env.Depends(BAF, OpenJADE)
                env.Depends(BAF, JModule)
                env.Depends(BAF, JFramework)
            else:
                BAF = env.LibMerge(
                    target=['#/'+variant_root+'/${PROCESSOR}/lib/BAF/'+jframework_test_param+'/'+env['LIBPREFIX']+jframework['Name']],
                    source = [JModule, OpenJADE, JFramework])
                env.SideEffect(Glob('*'+env['OBJSUFFIX']), BAF)
                env.Depends(BAF, OpenJADE)
                env.Depends(BAF, JModule)
                env.Depends(BAF, JFramework)

                BAFLst = env.LibLstMerge(
                    target=['#/'+variant_root+'/${PROCESSOR}/lib/BAF/'+jframework_test_param+'/'+env['LIBPREFIX']+jframework['Name']],
                    source = [JModule, OpenJADE, JFramework],
                    arch = optionArch
                    )
                env.Depends(BAFLst, BAF)

        # Generate File list for OpenJADE Runtime, JFramework, Model and maybe Platform
            filelist_path = '#/'+variant_root+'/${PROCESSOR}/lib/BAF/'+jframework['Name']+jframework_test_param+'/'+env['LIBPREFIX']
            ObjFL = env.SConscript(
                    app_root+'/SConscript.FileList',
                    exports={'env': env,
                        'filelist_path': filelist_path,
                        'jframework_name': jframework['Name'],
                        'jmodule_name': jmodule['Name'],
                        },
                    duplicate=0)
            env['FileListPath'][jframework['Name']] = filelist_path
            env.Depends(ObjFL, BAF)

            shared_lib = []
            if optionLib:
                if optionPlatform in ['PIL_shared_library', 'DLL']:
                    optionDynamic = 'yes'
                lib_path = variant_root+'/${PROCESSOR}/lib/BAF/'+jframework_test_param
                if (option_dict['optionModule'] is not None):
                    modulePath = '#/'+app_root+'/Module/'+ option_dict['optionModule']
                    shared_lib = env.SConscript(
                        app_root+'/SConscript.Module',
                        src_dir= '#/'+app_root+'/'+cfg['Platform'][optionArch][optionPlatform],
                        exports={'env': env,
                            'runtime':app_root+'/'+cfg['Runtime']['Path'],
                            'platform': optionPlatform,
                            'modulePath': modulePath,
                            'dynamic': optionDynamic,
                            'lib':BAF,
                            'lib_path': lib_path,
                            'option': jframework['Option']['Platform'] if ('Option' in jframework) and (jframework['Option']) and ('Platform' in jframework['Option']) and (jframework['Option']['Platform']) else None,
                            'arch': optionArch,
                            'targetName': jframework['Name'],
                            'target':'#/' + variant_root+'/${PROCESSOR}/exe/'+optionPlatform+'_'+test_case+'/'+jframework['Name']},
                        variant_dir = variant_root+'/${PROCESSOR}/obj/Platform/'+jframework['Name']+'/'+optionPlatform,
                        duplicate=0)
                    JFramework.append(shared_lib)
                    env.Depends(shared_lib[0], BAF)

            if (optionApp and optionLib):
                # Build JApplication each BAF
                JApplication = env.SConscript(
                    app_root+'/SConscript.App',
                    src_dir=app_root,
                    exports = {'env': env,
                        'runtime': cfg['Runtime']['Path'],
                        'platform': cfg['Platform'][optionArch][optionPlatform],
                        'lib':[BAF, Platform],
                        'framework':jframework,
                        'target':'#/' + variant_root+'/${PROCESSOR}/exe/'+optionPlatform+'_'+test_case+'/'+jframework['Name'],
                        'misc_root':misc_root,
                        'test_path': fw_test_folder,
                        'test_bin': fw_test_files + [Dir(fw_test_folder)],
                        'test_ascii': fw_ascii_files,
                        'test': test_case,
                        'OptPGO': optionPGO,
                        'option': jframework['Option']['Application'] if ('Option' in jframework) and (jframework['Option']) and ('Application' in jframework['Option']) and (jframework['Option']['Application']) else None
                    },
                    variant_dir= '#/'+variant_root+'/${PROCESSOR}/obj/app/'+jframework['Name'],
                    duplicate=0)
                env.Clean(JApplication, [ variant_root+'/${PROCESSOR}/obj', variant_root+'/${PROCESSOR}/exe/'+optionPlatform, variant_root+'/${PROCESSOR}/lib'])
                env.Alias(jframework['Name']+'App', JApplication)

                if optionApp and optionLib: # DLLs don't have tests
                    env.Depends(JApplication, JTest)
                    env.Depends(JApplication, Platform)
                    MultiSimTests.extend(JTest)
                    if optionPlatform in ['PIL_shared_library']:
                        env.Depends(JApplication, shared_lib[0])

                env.Depends(JApplication, BAF)
                env.Depends(JApplication, BAFLst)
                MultiSimApps.extend(JApplication)

                JSize = env.MemoryReportBuilder(
                                target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'/'+jframework['Name']+'/'+test_case,
                                source = [JApplication] +
                                [ app_root + '/Tools/JTest/xmlparse/ojtsi_size.xsl' ],
                                arch = optionArch,
                                filelist_path = env['FileListPath'][jframework['Name']],
                                )

                env.Depends(JSize, ObjFL)
                env.Depends(JSize, JApplication)
                if optionPGO != 'genpgo':
                    AppReports.append(JSize)

                if optionArch in ['sharc','gul','gxp']:
                    JMemViz = env.MemoryVizBuilder(
                            target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'/'+jframework['Name']+'/'+test_case,
                            source = [JApplication],
                            arch = optionArch,
                            filelist_path = env['FileListPath'][jframework['Name']],
                            )

                    env.Depends(JMemViz, ObjFL)
                    env.Depends(JMemViz, JApplication)
                    if optionPGO != 'genpgo':
                        AppReports.append(JMemViz)

                    JMapHtml = env.XmlMap2HtmlBuilder(
                            target = '#/' + variant_root+'/${PROCESSOR}/exe/'+optionPlatform+'_'+test_case+'/'+jframework['Name'],
                            source = [JApplication]
                            )

                    env.Depends(JMapHtml, JApplication)
                    if optionPGO != 'genpgo':
                        AppReports.append(JMapHtml)
                if JControl != None:
                    env.Default(JControl)

            # Simulate each JApplication
                if (not optionMultiSim):
                    JSimulation = run_simulation(
                        '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case, # target
                        [JApplication]+[JTest], # source
                        env,
                        optionArch=optionArch, optionRunner=optionRunner, optionPlatform=optionPlatform,
                        optionCfg=jframework['Option']['Simulation'] if ('Option' in jframework) and (jframework['Option']) and ('Simulation' in jframework['Option']) and (jframework['Option']['Simulation']) else None,
                        optionHostname=optionHostname, optionUser=optionUser, optionPwd=optionPwd,
                        optionRTOS=cfg['Option']['rtos'] if ('rtos' in cfg['Option']) and (cfg['Option']['rtos']) else None,
                        optionPreload=cfg['Option']['preload_file'] if ('preload_file' in cfg['Option']) and (cfg['Option']['preload_file']) else None,
                        optionBafLite=jframework['Option']['Lite'] if ('Option' in jframework) and (jframework['Option']) and ('Lite' in jframework['Option']) else False
                        )

                    env.Depends(JSimulation,JApplication)
                    Simulations.append(JSimulation)
                    reports_env = env.Clone()
                    reports_env.Decider('make')

                    if (jm_test_folder) and not (optionPlatform in ['RTA','DRTA','SOMRTA','SC594RTA','SC598RTA']):
                        JControlResponse = reports_env.ConvertControlResponse(
                                target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                source = [JSimulation] +
                                [ app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                                control_files,
                                JModule = jmodule
                                )

                        reports_env.Depends(JControlResponse, JSimulation)
                        SimReports.append(JControlResponse)

                        if ('Option' in jframework) and (jframework['Option']) and \
                            ('Lite' in jframework['Option']) and (jframework['Option']['Lite']):
                            pass
                        else:
                            JMIPSReport = reports_env.CalculateMIPSReport(
                                target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                source = [JSimulation],
                                arch = optionArch,
                                JModule = jmodule,
                                JFramework = jframework,
                                Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None
                                )
                            env.Alias(jframework['Name'], JMIPSReport)
                            reports_env.Depends(JMIPSReport, JSimulation)
                            SimReports.append(JMIPSReport)

                        if ('Option' in jframework) and (jframework['Option']) and \
                            ('Application' in jframework['Option']) and (jframework['Option']['Application']) and \
                            ('BAFStatsLogger' in jframework['Option']['Application']) and \
                            (jframework['Option']['Application']['BAFStatsLogger']) and \
                            (jframework['Option']['Application']['BAFStatsLogger']['enable']):
                            if (optionArch in ['gul','gxp']) :
                                if (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'PerfMon'):
                                    JPerfMonReport = reports_env.GeneratePerfMonReport(
                                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                        source = [JSimulation],
                                        JFramework = jframework,
                                        perfmon_mode = jframework['Option']['Application']['BAFStatsLogger']['statsConfig'],
                                        Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None
                                    )
                                    reports_env.Depends(JPerfMonReport, JSimulation)
                                    SimReports.append(JPerfMonReport)

                                elif (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'StackMon'):
                                    JStackMonReport = reports_env.GenerateStackMonReport(
                                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                        source = [JSimulation],
                                        JFramework = jframework,
                                        Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None
                                    )
                                    reports_env.Depends(JStackMonReport, JSimulation)
                                    SimReports.append(JStackMonReport)

                                elif (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'Ccnt64Mon'):
                                    JCcnt64MonReport = reports_env.GenerateCcnt64MonReport(
                                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                        source = [JSimulation],
                                        arch = optionArch,
                                        JModule = jmodule,
                                        JFramework = jframework,
                                        Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None
                                    )
                                    reports_env.Depends(JCcnt64MonReport, JSimulation)
                                    SimReports.append(JCcnt64MonReport)

                            elif (optionArch in ['hexagon','hexagon_v66','hexagon_v73']) :
                                if (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'PktMon'):
                                    JPktMonReport = reports_env.GeneratePktMonReport(
                                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                        source = [JSimulation],
                                        JFramework = jframework
                                    )
                                    reports_env.Depends(JPktMonReport, JSimulation)
                                    SimReports.append(JPktMonReport)

                        JAudioPlots = reports_env.AudioPlotsBuilder(
                            target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                            source = [JTest],
                            JModule = jmodule,
                            JFramework = jframework,
                            Option = jframework['AudioPlot']['Option'] if ('AudioPlot' in jframework) and ('Option' in jframework['AudioPlot']) else None
                            )

                        reports_env.Depends(JAudioPlots, JSimulation)
                        if optionPGO != 'genpgo':
                            SimReports.append(JAudioPlots)

                        if test_case_tolerance:
                            JPassFail = reports_env.CalculatePassFail(
                                target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                source = [JTest],
                                Test = talaria_model['Test'],
                                JModule = jmodule,
                                JFramework = jframework,
                                Option = jframework['AudioPlot']['Option'] if ('AudioPlot' in jframework) and ('Option' in jframework['AudioPlot']) else None
                                )
                            reports_env.Depends(JPassFail, JSimulation)
                            if optionPGO != 'genpgo':
                                SimReports.append(JPassFail)

                    env.Default(MultiSimApps)
                    env.Default(AppReports)
                    if optionSim == 'yes':
                        env.Default(Simulations)
                        env.Default(SimReports)
                env.Default(ObjFL)
            elif optionLib : # --app=no
                env.Default(BAF)
                env.Default(ObjFL)
                env.Default(BAFLst)
                if JControl != None:
                    env.Default(JControl)
                if shared_lib and optionPlatform in ['PIL_shared_library', 'DLL']:
                    env.Default(shared_lib[0])

            else :  # --lib=no
                if optionMil:
                    env.Default(JMil)
                else:
                    env.Default(JPrebuilds)

        if (optionMultiSim):
            MultiSimSrcs = [x for x in MultiSimApps]
            MultiSimSrcs.extend( [x for y in MultiSimTests for x in y] )
            JSimulation = run_simulation(
                '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case, # target
                MultiSimSrcs, # source
                env,
                optionArch=optionArch, optionRunner=optionRunner, optionPlatform=optionPlatform,
                optionCfg=jframework['Option']['Simulation'] if ('Option' in jframework) and (jframework['Option']) and ('Simulation' in jframework['Option']) and (jframework['Option']['Simulation']) else None,
                optionHostname=optionHostname, optionUser=optionUser, optionPwd=optionPwd,
                optionRTOS=cfg['Option']['rtos'] if ('rtos' in cfg['Option']) and (cfg['Option']['rtos']) else None,
                optionPreload=cfg['Option']['preload_file'] if ('preload_file' in cfg['Option']) and (cfg['Option']['preload_file']) else None,
                optionBafLite=jframework['Option']['Lite'] if ('Option' in jframework) and (jframework['Option']) and ('Lite' in jframework['Option']) else False
                )

            env.Depends(JSimulation, MultiSimApps)
            Simulations.append(JSimulation)
            reports_env = env.Clone()
            reports_env.Decider('make')

            if (jm_test_folder) and not (optionPlatform in ['RTA','DRTA','SOMRTA']):
                for jf_idx, jframework in enumerate(cfg['JFramework']):
                    JControlResponse = reports_env.ConvertControlResponse(
                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                        source = [JSimulation] +
                        [ app_root +'/' + cfg['Runtime']['Path'] + '/Message_Id.xml'] +
                        control_files,
                        optionIdx = jf_idx+1
                    )

                    reports_env.Depends(JControlResponse, JSimulation)
                    SimReports.append(JControlResponse)

                    if ('Option' in jframework) and (jframework['Option']) and \
                        ('Lite' in jframework['Option']) and (jframework['Option']['Lite']):
                        pass
                    else:
                        JMIPSReport = reports_env.CalculateMIPSReport(
                            target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                            source = [JSimulation],
                            arch = optionArch,
                            JModule = None,
                            JFramework = jframework,
                            Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None,
                            optionIdx = jf_idx+1
                        )
                        env.Alias(jframework['Name'], JMIPSReport)
                        reports_env.Depends(JMIPSReport, JSimulation)
                        SimReports.append(JMIPSReport)

                    if ('Option' in jframework) and (jframework['Option']) and \
                        ('Application' in jframework['Option']) and (jframework['Option']['Application']) and \
                        ('BAFStatsLogger' in jframework['Option']['Application']) and \
                        (jframework['Option']['Application']['BAFStatsLogger']) and \
                        (jframework['Option']['Application']['BAFStatsLogger']['enable']):
                        if (optionArch in ['gul','gxp']) :
                            if (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'PerfMon'):
                                JPerfMonReport = reports_env.GeneratePerfMonReport(
                                    target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                                    source = [JSimulation],
                                    JFramework = jframework,
                                    perfmon_mode = jframework['Option']['Application']['BAFStatsLogger']['statsConfig'],
                                    Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None,
                                    optionIdx = jf_idx+1
                                )
                                reports_env.Depends(JPerfMonReport, JSimulation)
                                SimReports.append(JPerfMonReport)

                            elif (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'StackMon'):
                                JStackMonReport = reports_env.GenerateStackMonReport(
                                    target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                                    source = [JSimulation],
                                    JFramework = jframework,
                                    Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None,
                                    optionIdx = jf_idx+1
                                )
                                reports_env.Depends(JStackMonReport, JSimulation)
                                SimReports.append(JStackMonReport)

                            elif (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'Ccnt64Mon'):
                                JCcnt64MonReport = reports_env.GenerateCcnt64MonReport(
                                    target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+jframework['Name']+'/'+test_case,
                                    source = [JSimulation],
                                    arch = optionArch,
                                    JModule = jmodule,
                                    JFramework = jframework,
                                    Option = jframework['MIPSReport']['Option'] if ('MIPSReport' in jframework) and ('Option' in jframework['MIPSReport']) else None,
                                    optionIdx = jf_idx+1
                                )
                                reports_env.Depends(JCcnt64MonReport, JSimulation)
                                SimReports.append(JCcnt64MonReport)

                        elif (optionArch in ['hexagon','hexagon_v66','hexagon_v73']) :
                            if (jframework['Option']['Application']['BAFStatsLogger']['statsLogger'] == 'PktMon'):
                                JPktMonReport = reports_env.GeneratePktMonReport(
                                    target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                                    source = [JSimulation],
                                    JFramework = jframework,
                                    optionIdx = jf_idx+1
                                )
                                reports_env.Depends(JPktMonReport, JSimulation)
                                SimReports.append(JPktMonReport)

                    JAudioPlots = reports_env.AudioPlotsBuilder(
                        target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                        source = MultiSimTests[2*jf_idx],
                        JModule = None,
                        JFramework = jframework,
                        Option = jframework['AudioPlot']['Option'] if ('AudioPlot' in jframework) and ('Option' in jframework['AudioPlot']) else None,
                        )

                    reports_env.Depends(JAudioPlots, JSimulation)
                    SimReports.append(JAudioPlots)

                    if test_case_tolerance:
                        JPassFail = reports_env.CalculatePassFail(
                            target = '#/' + variant_root+'/${PROCESSOR}/log/'+optionPlatform+'_'+optionRunner+'/'+test_case+'_%d' % (jf_idx+1),
                            source = MultiSimTests[2*jf_idx],
                            Test = talaria_model['Test'],
                            JModule = None,
                            JFramework = jframework,
                            Option = jframework['AudioPlot']['Option'] if ('AudioPlot' in jframework) and ('Option' in jframework['AudioPlot']) else None,
                            optionIdx = jf_idx+1
                            )
                        reports_env.Depends(JPassFail, JSimulation)
                        SimReports.append(JPassFail)

            env.Default(MultiSimApps)
            env.Default(AppReports)
            if optionSim == 'yes':
                env.Default(Simulations)
                env.Default(SimReports)

#Svninfo = env.SvnInfo(target='#/out/release/svninfo.txt',source=None)
#env.InstallAs('#/out/release/lib', '#/out/${PROCESSOR}/lib/BAF/')
#env.Install(target = '#/out/release/inc', source = Glob(cfg['Runtime']['Path']+'/BAF/*.h'))
#Release = env.Zip('out/release_${PROCESSOR}', 'out/release', ZIPROOT='out/release')
#env.Depends(Release,Svninfo)
#env.Depends(Release,JApplication)
#env.Alias('release', Release)

#env.Package( NAME           = 'BAF',
#             VERSION        = '0.0.2',
#             PACKAGEVERSION = 0,
#             PACKAGETYPE    = 'zip',
#             LICENSE        = 'BOSE',
#
#        )
