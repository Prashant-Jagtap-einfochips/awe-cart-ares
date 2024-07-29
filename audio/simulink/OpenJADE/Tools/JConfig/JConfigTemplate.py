from collections import OrderedDict
import sys
import os


#
# https://docs.python.org/3/library/platform.html#cross-platform
#
def is_64bits():
    return (sys.maxsize > 2**32)

def DLL_options():
    if is_64bits() :
        return '-i32 -t32'
    return '-i64 -t64'

def ctrlgen_Translation():
    if is_64bits() :
        return 'TranslationEngine64_win64.dll'
    return 'TranslationEngine32_win64.dll'

def ctrlgen_Interpretation():
    if is_64bits() :
        return 'InterpretationEngine64_win64.dll'
    return 'InterpretationEngine32_win64.dll'



JConfig_dual_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Context':True,
        'ObjectName':'BAF_obj',
        'Path':'Runtime',
        'Option':{
            'Print':False,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Vega'
    },
    'Partitioning':{
        'num_socs':2,
        'num_cores':1,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_VegaDSP1',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioIO13.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 20),
                ('out_channel_count', 35) ]),
            'TalariaModel':{
                'SysModel': True,
                'Path': 'repo',
                'Name': 'Vega',
                'Test': {
                    'Name':'VegaTestCase',
                    'Case':'BG',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin',
                    'audioIO13.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'codegen': None,
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharcPlus.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                },
                'Parameters': OrderedDict([
                    ('sample_rate', 48000),
                    ('frame_size', 32),
                    ('inp_bg_channel_count', 2),
                    ('out_bg_channel_count', 14),
                    ('inp_eor_channel_count', 5),
                    ('out_eor_channel_count', 14),
                    ('sys_in_channel_count', 20),
                    ('sys_out_channel_count', 14)
                ])
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'2',
        'CORE_ID':'1',
        'Name':'BAF_VegaDSP2',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIO32.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 34),
                ('out_channel_count', 14) ]),
            'TalariaModel':{
                'Path': 'repo',
                'Name': 'Vega',
                'Test': {
                    'Name':'VegaTestCase',
                    'Case':'BG',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin',
                    'audioIO32.bin']}
                }
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_triple_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Context':True,
        'ObjectName':'BAF_obj',
        'Path':'Runtime',
        'Option':{
            'Print':False,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Vega'
    },
    'Partitioning':{
        'num_socs':2,
        'num_cores':1,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_VegaDSP1',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioIO13.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 20),
                ('out_channel_count', 35) ]),
            'TalariaModel':{
                'SysModel': True,
                'Path': 'repo',
                'Name': 'Vega',
                'Test': {
                    'Name':'VegaTestCase',
                    'Case':'BG',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin',
                    'audioIO13.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'codegen': None,
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharcPlus.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                },
                'Parameters': OrderedDict([
                    ('sample_rate', 48000),
                    ('frame_size', 32),
                    ('inp_bg_channel_count', 2),
                    ('out_bg_channel_count', 14),
                    ('inp_eor_channel_count', 5),
                    ('out_eor_channel_count', 14),
                    ('sys_in_channel_count', 20),
                    ('sys_out_channel_count', 14)
                ])
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'2',
        'CORE_ID':'1',
        'Name':'BAF_VegaDSP2',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIO32.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 34),
                ('out_channel_count', 14) ]),
            'TalariaModel':{
                'Path': 'repo',
                'Name': 'Vega',
                'Test': {
                    'Name':'VegaTestCase',
                    'Case':'BG',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin',
                    'audioIO32.bin']}
                }
        },],
    },{
        'Path':'Baf',
        'SOC_ID':'3',
        'CORE_ID':'1',
        'Name':'BAF_VegaDSP2',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIO32.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 34),
                ('out_channel_count', 14) ]),
            'TalariaModel':{
                'Path': 'repo',
                'Name': 'Vega',
                'Test': {
                    'Name':'VegaTestCase',
                    'Case':'BG',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin',
                    'audioIO32.bin']}
                }
        },],
    },],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_quad_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'Option':{
            'Print':False,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Model'
    },
    'Partitioning':{
        'num_socs':2,
        'num_cores':2,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_Core1',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'2',
        'Name':'BAF_Core2',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'2',
        'CORE_ID':'1',
        'Name':'BAF_Core3',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'2',
        'CORE_ID':'2',
        'Name':'BAF_Core4',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_quint_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'Option':{
            'Print':False,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Model'
    },
    'Partitioning':{
        'num_socs':1,
        'num_cores':5,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_Core1',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Talaria',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'2',
        'Name':'BAF_Core2',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'3',
        'Name':'BAF_Core3',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'4',
        'Name':'BAF_Core4',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'5',
        'Name':'BAF_Core5',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_hex_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'Option':{
            'Print':False,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Model'
    },
    'Partitioning':{
        'num_socs':1,
        'num_cores':5,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_Core1',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Talaria',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'2',
        'Name':'BAF_Core2',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'3',
        'Name':'BAF_Core3',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'4',
        'Name':'BAF_Core4',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'5',
        'Name':'BAF_Core5',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'6',
        'Name':'BAF_Core6',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_hept_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'BCF':{
        'Name':'Model'
    },
    'Partitioning':{
        'num_socs':1,
        'num_cores':5,
        'modules':{
            # modelname : 'soc_num, core_num'
            '/BasicAudio/Balance':'2,1',
            '/BasicAudio/Bass':'2,1',
            '/BasicAudio/DSP12':'2,1',
            '/BasicAudio/Mute':'2,1',
            '/BasicAudio/Treble':'2,1',
            '/BasicAudio/Volume':'1,1',
            '/Input':'1,1',
            '/Output':'2,1'
        }
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_Core1',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Talaria',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'2',
        'Name':'BAF_Core2',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'3',
        'Name':'BAF_Core3',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'4',
        'Name':'BAF_Core4',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'5',
        'Name':'BAF_Core5',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'6',
        'Name':'BAF_Core6',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'7',
        'Name':'BAF_Core7',
        'Test': {
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'Model',
            'TalariaModel':{
                'exclude': None,
                'SysModel': True,
                'Test': {
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
            },
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}


JConfig_test = {
    'Option':{
        'arch': 'x86',
        'platform': 'SIL',
        'runner':'sim',
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_Vega2',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'VegaTestCase',
            'Case':'BG',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'Vega',
            'Parameters': OrderedDict([
                ('sample_rate', 48000),
                ('frame_size', 32),
                ('in_channel_count', 20),
                ('out_channel_count', 14) ]),
            'TalariaModel':{
                'Path': 'repo/test/Vega2',
                'Name': 'Vega2',
                'ProjectPath': 'repo',
                'Project': 'Vega.prj',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'codegen': [
                    '_Vega0_TOP.c',
                    '.c',
                    '_data.c',
                    '_RTC_NTF.c',
                    '_RTC_REQ_RSP.c',
                    '_TalariaAPI.c',
                    '_TOP_Access.c'],
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                   'x86':'Misc/ConfigSetx86.m',
                   'x64':'Misc/ConfigSetx86.m',
                   'amd64':'Misc/ConfigSetx86.m',
                   'gul':'Misc/ConfigSetSharcPlus.m',
                   'sharc':'Misc/ConfigSetSharc.m',
                },
                'Parameters': OrderedDict([
                    ('sample_rate', 48000),
                    ('frame_size', 32),
                    ('fg_instance_count', 8),
                    ('fg_channel_count', 1),
                    ('bg_instance_count', 1),
                    ('bg_channel_count', 2),
                    ('sys_in_channel_count', 20),
                    ('sys_out_channel_count', 14)
                ])
            },
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'BCF':{
        'Name':'Model'
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_UnitTest',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'UnitTest',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharcPlus.m',
                    'gxp':'Misc/ConfigSetSharcPlus.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    'hifi4':'Misc/ConfigSetHifi4.m',
                    },
                }
            }]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'hifi4': {'RT685PIL': 'Platform/hifi4/RT685PIL',
                  'SIMULATORPIL': 'Platform/hifi4/SIMULATORPIL',
                  'QCC7226PIL': 'Platform/hifi4/QCC7226PIL'},
        'hifi5': {'AB1585PIL': 'Platform/hifi5/AB1585PIL'},
        'qnx_nto_aarch64': {'PIL': 'Platform/qnx_nto_aarch64/PIL',
                            'PIL_shared_library': 'Platform/qnx_nto_aarch64/PIL_shared_library'},
        'hexagon': {'PIL': 'Platform/hexagon/PIL'},
        'hexagon_v66': {'PIL': 'Platform/hexagon/PIL'},
        'hexagon_v73': {'PIL': 'Platform/hexagon/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_app_oj4 = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Context':True,
        'ObjectName':'BAF_obj',
        'Option':{
            'Print':True,
            'OJ3Support':False,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_UnitTest',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'UnitTest',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharcPlus.m',
                    'gxp':'Misc/ConfigSetSharcPlus.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    'hifi4':'Misc/ConfigSetHifi4.m',
                    },
                }
            }]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'hifi4': {'RT685PIL': 'Platform/hifi4/RT685PIL',
                  'SIMULATORPIL': 'Platform/hifi4/SIMULATORPIL',
                  'QCC7226PIL': 'Platform/hifi4/QCC7226PIL'},
        'hifi5': {'AB1585PIL': 'Platform/hifi5/AB1585PIL'},
        'qnx_nto_aarch64': {'PIL': 'Platform/qnx_nto_aarch64/PIL'},
        'hexagon': {'PIL': 'Platform/hexagon/PIL'},
        'hexagon_v66': {'PIL': 'Platform/hexagon/PIL'},
        'hexagon_v73': {'PIL': 'Platform/hexagon/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}


JConfig_app_app = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'Name':'BAF_UnitTest1',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'UnitTest',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'codegen': [
                    '_UnitTest0_TOP.c',
                    '.c',
                    '_data.c',
                    '_RTC_NTF.c',
                    '_RTC_REQ_RSP.c',
                    '_TalariaAPI.c',
                    '_TOP_Access.c'],
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            }]
        },{
        'Path':'Baf',
        'SOC_ID':'2',
        'Name':'BAF_UnitTest2',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'UnitTest',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'codegen': [
                    '_UnitTest0_TOP.c',
                    '.c',
                    '_data.c',
                    '_RTC_NTF.c',
                    '_RTC_REQ_RSP.c',
                    '_TalariaAPI.c',
                    '_TOP_Access.c'],
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            }]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_app_dualjm = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_UnitTest',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },
            {
            'Path':'JModule_Lib/Jm1',
            'Name':'UnitTest1',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            }            ]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}
JConfig_app_trijm = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_UnitTest',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },
            {
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },            {
            'Path':'JModule_Lib/Jm1',
            'Name':'UnitTest1',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            }            ]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_app_quadjm = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'BCF':{
        'Name':'Bcf'
    },
    'JFramework':[{
        'Path':'Baf',
        'Name':'BAF_UnitTest',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },
            {
            'Path':'JModule_Lib/Jm1',
            'Name':'UnitTest1',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },
            {
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },
            {
            'Path':'JModule_Lib/Jm0',
            'Name':'UnitTest0',
            'TalariaModel':{
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharc.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                    },
                }
            },            ]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_runtime = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime'
    },
    'JFramework':[{
        'Name': None,
        'JModule':[{}]
        }],
}

JConfig_bcf = {
    'Option':{
        'cart': 'bcf',
        'arch': 'x64',
        'platform': 'DLL',
        'runner':'sim'
    }
}
JConfig_btf = {
    'Option':{
        'cart' :'btf',
        'arch': 'x64',
        'platform': 'DLL',
        'runner':'sim'
    }
}
JConfig_brf = {
    'Option':{
        'cart' :'brf',
        'arch': 'x64',
        'platform': 'DLL',
        'runner':'sim'
    }
}

JConfig_cmb = {
    'Option':{
        'cart' :'cmb',
        'arch': 'qnx_nto_aarch64',
        'platform': 'PIL',
        'runner':'evb'
    }
}

JConfig_Build_Test = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim',
        'user': 'username',
        'pwd': 'password',
        'hostname':'localhost'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf1',
        'Name':'BAF_Basicaudio',
        'Specific':{
            'c66':{
                'num_execs':3,
                'num_connections':4,
                'num_processes': 3,
                'num_process': 2,
                'num_jmodules':3,
                'num_ports':6 ,
                'num_port':3 }},
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'GID1507946_testLevelCurve',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin']},
        'JModule':[{
            'Path':'JModule_Lib/MyJModule',
            'Name':'MyJModule',
#            'Parameters': None,
            'CModel': {
                'Path': "",
                'CFiles': []
            },
            #'LibModel': {
            #    'ArchToLib': {},
            #    'Path': "",
            #    'Test': {
            #        'Name': 'LibraryTestCase',
            #        'TestPath': 'repo/test_lib',
            #        'InputOutput': ['audioIn.bin', 'audioOut.bin']
            #    }
            #},
            'TalariaModel': {
                'ProjectPath': 'repo',
                'Project': 'Basicaudio.prj',
                'Path': 'repo',
                'Name': 'BasicAudio',
                'Test': {
                    'Name':'BasicAudioTestCase',
                    'Case':'GID1507946_testLevelCurve',
                    'Param':None,
                    'Tolerance':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'TestCase':'Test1',
                'DLL': DLL_options(),
                'prefix': 'default',
                'codegen': [
                    '_Basicaudio0_TOP.c',
                    '.c',
                    '_data.c',
                    '_RTC_NTF.c',
                    '_RTC_REQ_RSP.c',
                    '_TalariaAPI.c',
                    '_TOP_Access.c'],
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'testgen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin'],
                #'config':{
                #    'x86':'Misc/ConfigSetx86.m',
                #    'gul':'Misc/ConfigSetSharc.m',
                #    'sharc':'Misc/ConfigSetSharc.m',
                #},
 #              'Parameters': None,
            }
        }]
    }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_capp = {
    'Option':{
        'arch': 'gul',
        'platform': 'PIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True,
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_UnitTest',
        'JModule':[{
            'Path':'JModule_Lib/Jm',
            'Name':'UnitTest',
            'TalariaModel':{
                'codegen': ['TalariaStub.c']
                    },
            }]
        }],
    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'amd64': {'SIL': 'Platform/SIL',
                  'RTA': 'Platform/preq/RTA',
                  'DLL': 'Platform/dll'},
        'x86': {'SIL': 'Platform/SIL',
                'DLL': 'Platform/dll'},
        'x64': {'DLL': 'Platform/dll'},
        'c66': {'PIL': 'Platform/c66/PIL'},
        'gul': {'PIL': 'Platform/gul/PIL',
                'HIL': 'Platform/gul/HIL',
                'RTA': 'Platform/gul/RTA',
                'SOMPIL': 'Platform/gul/SOMPIL',
                'SOMHIL': 'Platform/gul/SOMHIL',
                'SOMRTA': 'Platform/gul/SOMRTA',
                'MRTA': 'Platform/melville/RTA',
                'MPIL': 'Platform/melville/PIL',
                'MHIL': 'Platform/melville/HIL',
                'MDRTA': 'Platform/melville/RTA'},
        'gxp': {'PIL': 'Platform/gxp/PIL',
                'HIL': 'Platform/gxp/HIL',
                'RTA': 'Platform/gxp/RTA',
                'SC594PIL': 'Platform/sc594/PIL',
                'SC594HIL': 'Platform/sc594/HIL',
                'SC594RTA': 'Platform/sc594/RTA',
                'SC598PIL': 'Platform/sc598/PIL',
                'SC598HIL': 'Platform/sc598/HIL',
                'SC598RTA': 'Platform/sc598/RTA'},
        'sharc': {'PIL': 'Platform/sharc/PIL',
                  'HIL': 'Platform/sharc/HIL',
                  'RTA': 'Platform/sharc/RTA'}
    }
}

JConfig_dual_gxp = {
    'Option':{
        'arch': 'gxp',
        'platform': 'DPIL',
        'runner':'sim'
    },
    'Runtime':{
        'Path':'Runtime',
        'Option':{
            'Print':True,
            'JSched':False,
            'Connection':False,
            'Logger':True
        },
    },
    'JFramework':[{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'1',
        'Name':'BAF_SPC1',
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'test_default',
            'Param':None,
            'Src': [
                'request.ojt',
                'audioIn.bin',
                'audioOut.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm1',
            'Name':'UnitTest',
            'TalariaModel':{
                'SysModel': True,
                'ProjectPath': 'repo',
                'Project': 'UnitTest.prj',
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'test_default',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'audioIn.bin',
                    'audioOut.bin']},
                'DLL': DLL_options(),
                'crlPath':{
                    'gul': 'repo/blocklib/arch/sharc_plus',
                    'gxp': 'repo/blocklib/arch/sharc_plus',
                    'sharc': 'repo/blocklib/arch/sharc',
                    'x86': None,
                },
                'crl': {
                    'gul':[
                        'src/pooliirSharcPlus.asm',],
                    'gxp':[
                        'src/pooliirSharcPlus.asm',],
                    'sharc':[
                        'src/pooliirProcessSharc.asm',],
                        },
                'prefix': 'default',
                'codegen': None,
                'ctrlgen': [
                    'RTC_Payloads.xml',
                    'TOP_Tokens.xml',
                    ctrlgen_Interpretation(),
                    ctrlgen_Translation()],
                'config':{
                    'x86':'Misc/ConfigSetx86.m',
                    'x64':'Misc/ConfigSetx86.m',
                    'amd64':'Misc/ConfigSetx86.m',
                    'gul':'Misc/ConfigSetSharcPlus.m',
                    'gxp':'Misc/ConfigSetSharcPlus.m',
                    'sharc':'Misc/ConfigSetSharc.m',
                },
            },
        }],
    },{
        'Path':'Baf',
        'SOC_ID':'1',
        'CORE_ID':'2',
        'Name':'BAF_SPC2',
        'Test': {
            'Name':'UnitTestTestCase',
            'Case':'test_default',
            'Param':None,
            'Src': [
                'request.ojt',
                'empty.bin',
                'empty.bin'],
            'Gen': [
                'request.ojx',
                'audioIn.bin',
                'audioRef.bin',
                'audioOut.bin'],},
        'JModule':[{
            'Path':'JModule_Lib/Jm2',
            'Name':'UnitTest',
            'TalariaModel':{
                'Path': 'repo',
                'Name': 'UnitTest',
                'Test': {
                    'Name':'UnitTestTestCase',
                    'Case':'test_default',
                    'Param':None,
                    'Gen': [
                    'request.ojt',
                    'empty.bin',
                    'empty.bin']}
                }
        }],
    }],

    'Platform':{
        'qnx_nto_aarch64': {'PIL_shared_library': 'Model/io-audio'},
        'gxp': {'DPIL': 'Platform/gxp/DPIL',
                'DHIL': 'Platform/gxp/DHIL',
                'DRTA': 'Platform/gxp/DRTA'}
    }
}

