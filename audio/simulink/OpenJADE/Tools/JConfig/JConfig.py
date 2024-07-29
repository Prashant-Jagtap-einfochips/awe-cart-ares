
frame_size=32
#Test GID1507946_testLevelCurve 48k
jconfig = JConfig_Add(JConfig_app)
# JConfig_Platform(      jconfig,'gul', 'HIL', 'sim')
JConfig_Platform(      jconfig,'x86', 'SIL', 'sim')
JConfig_TalariaProject(jconfig,'application/model', 'Dev.prj')
JConfig_TalariaModel(  jconfig,'application/model', 'Model')  # folder, model, feature
sample_rate=48000
JConfig_TalariaTest(   jconfig,'ModelTestCase','test')
talaria_param = OrderedDict([
            ('sample_rate', sample_rate),
            ('frame_size', frame_size),
            ('in_channel_count', 2),
            ('out_channel_count', 16),
            ('sys_in_channel_count', 2),
            ('sys_out_channel_count', 16)
       ])
jmodule_param = {
            'sample_rate': sample_rate,
            'frame_size': frame_size,
            'in_channel_count': 2,
            'out_channel_count': 16
        }
JConfig_TalariaParam(jconfig, talaria_param)
JConfig_JModuleParam(jconfig, jmodule_param)
codegen = [
    '_PreAmp_p0_b0_TOP.c',
    '.c',
    '_TalariaAPI.c',
    '_TOP_Access.c',
    '_TSP_Capture.c']
JConfig_TalariaCodegen(jconfig, codegen)
#Test GID1507946_testLevelCurve 44.1k
jconfig = JConfig_Add(jconfig) # carry over previouse testcase
JConfig_Platform(      jconfig,'gul', 'PIL', 'sim')

jconfig = JConfig_Add(jconfig) # carry over previouse testcase
JConfig_Platform(      jconfig,'gul', 'PIL', 'ezkit')

jconfig = JConfig_Add(jconfig) # carry over previouse testcase
JConfig_Platform(      jconfig,'gul', 'HIL', 'ezkit')

jconfig = JConfig_Add(jconfig) # carry over previouse testcase
JConfig_Platform(      jconfig,'gul', 'RTA', 'ezkit')
