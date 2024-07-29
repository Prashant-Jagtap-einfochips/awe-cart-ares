from JTest import *

model_root='application/model'
project_name='Dev.prj'
project_root='.'

tolerance=0.005
model_name = 'BasicAudio'
config_name = 'preamp'

app_param = {
    'model_name': model_name
}

build_cfgs = {
    1: {
        'info': 'x86 sil',
        'app_param': app_param,
        'platform': ['x86', 'SIL', 'sim'],
    }
}

for idx,build_cfg in build_cfgs.items():
    arch = build_cfg['platform'][0]
    platform = build_cfg['platform'][1]
    runner = build_cfg['platform'][2]

    jconfig = JConfig_Add(JConfig_app, name='%s%d' % (config_name,idx), info=build_cfg['info'])
    JConfig_Platform(jconfig, arch, platform, runner)

    JConfig_SetGenerator(jconfig, app_config_root=model_root, arg={'param':build_cfg['app_param']})

    JConfig_TalariaDefConfig(jconfig, project_root, project_name, model_root, model_name)
    JConfig_TalariaTest(jconfig, '%sTestCase' % (model_name),'test_default', tolerance=tolerance)
    JConfig_DLL_options(jconfig, '-i32 -t32')
    build_cfg['jconfig'] = jconfig


