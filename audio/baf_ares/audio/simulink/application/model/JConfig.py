from cart.JConfig.JTest import *

if 'model_root' not in globals():
    model_root='application/model'
if 'project_name' not in globals():
    project_name='Project.prj'
if 'project_root' not in globals():
    project_root='.'

model_name = 'Model'
config_name = 'ip5'

app_param_generic = {
    'model_name': model_name,
    'dsp_target': 'Generic'
}

app_param_hexagon = {
    'model_name': model_name,
    'dsp_target': 'Hexagon'
}

app_param_baf_multi = {
    'model_name': model_name,
    'dsp_target': 'Generic',
    'baf_multi': True
}

partitionGenerateOptions_rel = f'{project_root}/configset/partitionGenOptIP5.m'
partitionGenerateOptions_x86 = f'{project_root}/configset/partitionGenOptBVP.m'
partitionGenerateOptions_hexagon = f'{project_root}/configset/partitionGenOptHexagon.m'

build_options = {
'JModule' : { 0 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } },
}

arch_rel = {
    'Baf1':   'hexagon_v66',
    'Baf2_1': 'hexagon_v73',
    'Baf2_2': 'hexagon_v73',
    'Baf2_3': 'hexagon_v73',
    'Baf2_4': 'hexagon_v73',
    'Baf2_5': 'hexagon_v73',
    'Baf2_6': 'hexagon_v73',
    'Baf3':   'hexagon_v66',
    'Baf4':   'qnx_nto_aarch64',
    'Baf5':   'hexagon_v66',
}

build_options_rel = {
    'hexagon_v66': build_options,
    'hexagon_v73': build_options,
    'qnx_nto_aarch64': build_options,
}

build_cfgs = {
    '_rel_lib': {
        'info': 'release multi-arch',
        'app_param': app_param_hexagon,
        'platform': [arch_rel, None, None],
        'build_options': build_options_rel,
        'pgo_cfg': partitionGenerateOptions_rel,
    },
    '_x64_dll': {
        'info': 'x64 DLL sim',
        'app_param': app_param_generic,
        'platform': ['x64', 'DLL', 'sim', 'dll_baf'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_x86,
    },
    '_linux': {
        'info': 'amd64 DLL sim',
        'app_param': app_param_generic,
        'platform': ['amd64', 'DLL', 'sim', 'dll_baf'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_x86,
    },
    '_hexagon_lib': {
        'info': 'hexagon PIL sim',
        'app_param': app_param_hexagon,
        'platform': ['hexagon', 'PIL', 'sim'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_hexagon,
    },
    '_qnx_lib': {
        'info': 'qnx_nto_aarch64 pil',
        'app_param': app_param_generic,
        'platform': ['qnx_nto_aarch64', 'PIL', 'evb'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_x86,
    },
    '_x86_lib': {
        'info': 'x86 SIL sim',
        'app_param': app_param_generic,
        'platform': ['x86', 'SIL', 'sim'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_x86,
    },
    '_qnx_sh': {
        'info': 'qnx_nto_aarch64 PIL_shared_library',
        'app_param': app_param_generic,
        'platform': ['qnx_nto_aarch64', 'PIL_shared_library', 'evb', 'io-audio'],
        'build_options': build_options,
        'pgo_cfg': partitionGenerateOptions_x86,
    },
}

for key,build_cfg in build_cfgs.items():
    arch = build_cfg['platform'][0]
    platform = build_cfg['platform'][1]
    runner = build_cfg['platform'][2]
    if len(build_cfg['platform']) > 3:
        module = build_cfg['platform'][3]
    else:
        module = None

    jconfig = JConfig_Add(JConfig_deca_app, name='%s%s' % (config_name,key), info=build_cfg['info'])
    JConfig_Platform(jconfig, arch, platform, runner, module=module)
    JConfig_SetGenerator(jconfig, app_config_root=model_root, arg={'param':build_cfg['app_param']})
    JConfig_BafLite(jconfig, True)
    JConfig_TalariaDefConfig(jconfig, project_root, project_name, model_root, model_name)
    JConfig_TalariaPartitionGenerateOptions(jconfig, pgo_cfg=build_cfg['pgo_cfg'])
    JConfig_TalariaCodeGenExclude(jconfig, exclude=['srcThread.c'])

    JConfig_CustomBuildFlags(jconfig, arch=arch, build_options=build_cfg['build_options'])

    build_cfg['jconfig'] = jconfig

# 5 Bafs, 6 JModules in the second Baf 
#======================= BVP Release: Multi JMod =======================
build_multi_options = {
 'JModule' : { 
   0 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
   1 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
   2 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
   3 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
   4 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
   5 : {'xcpp_flags': ['-DPCPREQ4', '-Ddm=""'] } ,
}}


build_multi_cfg = {
    '_multi_rel_lib': {
        'info': 'release multi-jmodule',
        'app_param': app_param_hexagon,
        'platform': ['hexagon_v73', 'PIL', 'sim'],
        'build_options': build_multi_options,
        'pgo_cfg': partitionGenerateOptions_hexagon,
        'dep':'_rel_lib'
    },
    '_multi_x64_dll': {
        'info': 'x64 DLL multi jmodule',
        'app_param': app_param_generic,
        'platform': ['x64', 'DLL', 'sim', 'dll_baf'],
        'build_options': build_multi_options,
        'pgo_cfg': partitionGenerateOptions_x86,
        'dep':'_x64_dll'
    },
    '_multi_linux': {
        'info': 'amd64 DLL multi jmodule',
        'app_param': app_param_generic,
        'platform': ['amd64', 'DLL', 'sim', 'dll_baf'],
        'build_options': build_multi_options,
        'pgo_cfg': partitionGenerateOptions_x86,
        'dep':'_linux'
    }
}

for key,build_cfg in build_multi_cfg.items():
    arch = build_cfg['platform'][0]
    platform = build_cfg['platform'][1]
    runner = build_cfg['platform'][2]
    if len(build_cfg['platform']) > 3:
        module = build_cfg['platform'][3]
    else:
        module = None

    jconfig = JConfig_Add(JConfig_app, name='%s%s' % (config_name,key), info=build_cfg['info'])
    jconfig['JFramework'][0]['JModule'] = copy.deepcopy(JConfig_app_octo_jm['JFramework'][0]['JModule'][0:6]) #6 JMods on 2nd Baf
    JConfig_Platform(jconfig, arch, platform, runner, module=module)
    JConfig_SetCGenerator(jconfig, app_config_root=model_root, arg={'param':app_param_baf_multi})
    JConfig_SetCodeGenFolder(jconfig,'out/'+config_name+build_cfg['dep'])

    #                              Path                  exclude
    JConfig_SetC(jconfig, 'Model_1_2_ert_shrlib_rtw', ['srcThread.c'], id=0, core=0)
    JConfig_SetC(jconfig, 'Model_1_3_ert_shrlib_rtw', ['srcThread.c'], id=1, core=0)
    JConfig_SetC(jconfig, 'Model_1_4_ert_shrlib_rtw', ['srcThread.c'], id=2, core=0)
    JConfig_SetC(jconfig, 'Model_1_5_ert_shrlib_rtw', ['srcThread.c'], id=3, core=0)
    JConfig_SetC(jconfig, 'Model_1_6_ert_shrlib_rtw', ['srcThread.c'], id=4, core=0)
    JConfig_SetC(jconfig, 'Model_1_7_ert_shrlib_rtw', ['srcThread.c'], id=5, core=0)

    JConfig_BafLite(jconfig, False)
    JConfig_Runtime(jconfig, use_schedule = True, print = False, connection = True, logger = False, semaphore = True)

    JConfig_CustomBuildFlags(jconfig, arch=arch, build_options=build_cfg['build_options'])

    build_cfg['jconfig'] = jconfig
