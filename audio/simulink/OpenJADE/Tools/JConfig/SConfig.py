import os
import platform

def env_common(env):
    if 'MATLAB' not in env :
        env['MATLAB'] = '\"C:/Program Files/MATLAB/R2022b/bin/matlab.exe\"'
    if 'SystemRoot' in env['ENV']:    
        env['ENV']['PATH'] += ';'+env['ENV']['SystemRoot']+'/System32/WindowsPowerShell/v1.0'


#=====================
#=====================
def c66_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'c66'
    env['DSS_ROOT'] = 'C:\\ti\\ccs1010\\ccs\\ccs_base'
    C6X_C_DIR = ' C:\\ti\\pdk_c667x_2_0_16\\packages;'
    C6X_C_DIR += ' C:\\ti\\pdk_c667x_2_0_16\\packages\\ti\\csl\\lib\\c6678\\c66\\release'
    env['ENV'].update({'C6X_C_DIR': C6X_C_DIR})
    env.SetDefault( TOOLSPATH = 'C:/ti/ccs1010/ccs/tools/compiler/ti-cgt-c6000_8.3.8/bin/' )
    env.SetDefault(CONFIG = [ '--gcc', '-mv6600', '-I${TOOLSPATH}/../include'])
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['AS'] = ''
    env['ASFLAGS'] = ''
    env['CCCOM'] = '$CC -fe $TARGET -c $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCES'
    env['CPPPATH'] = '${TOOLSPATH}/../include'
    env['CC'] = '${TOOLSPATH}/cl6x.exe'
    env.Replace(CPPFLAGS = env['CONFIG'])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['AR'] = '${TOOLSPATH}/ar6x.exe'
    env['ARFLAGS'] = 'r'
    env['ARXFLAGS'] = 'x'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '${TOOLSPATH}lnk6x.exe'
    env.Replace(LINKFLAGS = ['-x', '-l','${TOOLSPATH}/../lib/rts6600_elf.lib', '-l', 'ti.csl.ae66', '-e', '_c_int00', '--rom_model'])
    env['LIBS']= ''

    env['PROGSUFFIX'] = '.out'
    env['OBJSUFFIX'] = '.o'
    env['LIBSUFFIX'] = '.lib'
    env['MAPSUFFIX'] = '.map'
    env['MAPFLAGS'] =  '-m'
    env['LIBPREFIX'] =''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = ''
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env.Replace(RUNNERFLAGS = env['CONFIG'])
    env_common(env)

def imx8_tools(env):
    env['PROCESSOR'] = 'imx8'
    env.SetDefault( TOOLSPATH = '/opt/gcc-arm-8.3-2019.03-x86_64-aarch64-linux-gnu/bin/')
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-O2']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O2']},
    }
    env['build_options'] = build_options
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['AS'] = ''
    env['ASFLAGS'] = ''
    env['CC'] = '${TOOLSPATH}aarch64-linux-gnu-gcc'
    env['CPPFLAGS'] = ['-g', '-std=gnu99']
    #env.Replace(CPPFLAGS = env['CONFIG'])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['AR'] = '${TOOLSPATH}aarch64-linux-gnu-ar'
    env['ARFLAGS'] = 'r'
    env['ARXFLAGS'] = 'xo'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '${TOOLSPATH}aarch64-linux-gnu-gcc'
    #env.Replace(LINKFLAGS = env['CONFIG'])
    env['LIBS']= '-lm'

    env['PROGSUFFIX'] = '.elf'
    env['OBJSUFFIX'] = '.o'
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX'] =''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = ''
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env['RUNNER'] = '${TOOLSPATH}aarch64-linux-gnu-gdb'
    #env.Replace(RUNNERFLAGS = env['CONFIG'])
    env_common(env)

def hifi_common(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-O2']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O2']},
    }
    env['build_options'] = build_options
    env['AS'] = ''
    env['ASFLAGS'] = ''
    env['CC'] = '${TOOLSPATH}xt-clang.exe'
    env.Replace(CPPFLAGS = env['CONFIG'])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['AR'] = '${TOOLSPATH}xt-ar.exe'
    env['ARFLAGS'] = 'r'
    env['ARXFLAGS'] = 'xo'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '${TOOLSPATH}xt-clang.exe'
    env.Replace(LINKFLAGS = env['CONFIG'])
    env['LIBS']= '-lm'

    env['PROGSUFFIX'] = '.elf'
    env['OBJSUFFIX'] = '.o'
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['LSPFLAGS'] =  '-mlsp='
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX'] =''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = ''
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'xt-run.exe'
    env.Replace(RUNNERFLAGS = env['CONFIG'])
    env_common(env)

def hifi2_tools(env):
    env['PROCESSOR'] = 'hifi2'
    env.SetDefault( TOOLSPATH = 'C:/usr/xtensa/XtDevTools/install/tools/RG-2018.9-win32/XtensaTools/bin/')
    env.SetDefault( CONFIG = ['--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RG-2018.9-win32/dirana_hifi_rg_2018_9/config', '--xtensa-core=dirana_hifi_rg_2018_9'])
    hifi_common(env)

def hifi3_tools(env):
    env['PROCESSOR'] = 'hifi3'
    env.SetDefault( TOOLSPATH = 'C:/usr/xtensa/XtDevTools/install/tools/RF-2015.2-win32/XtensaTools/bin/')
    env.SetDefault( CONFIG = ['--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RF-2015.2-win32/SAF777x_HiFi3_c2t1_T1/config/', '--xtensa-core=SAF777x_HiFi3_c2t1_T1'])
    hifi_common(env)

def hifi4_common(env):
    env['PROCESSOR'] = 'hifi4'

    hifi_common(env)

    # Allow  auto vectorization, only available through O3
    build_options_o3={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-O3']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }

    env.Replace(build_options = build_options_o3)
    env.Replace(CPPFLAGS = [    env['CONFIG'],
                                '-fdata-sections',
                                '-ffunction-sections',
                                '-LNO:simd',
                            ])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options_o3[env['BUILD']]['cppflags'])

    # Change runner to gdb so that both sim and HW profiling can be used
    env.Replace(RUNNER = 'xt-gdb.exe')

def rt685_hifi4_tools(env):
    env.SetDefault( XTOCDPATH = 'C:/Tensilica/Xtensa_OCD_Daemon_14.05/')
    env.SetDefault( JLINKPATH = 'C:/Program Files/SEGGER/JLink/')
    env['XTOCDEXE'] = env['XTOCDPATH']+'xt-ocd.exe'
    env['JLINKEXE'] = env['JLINKPATH']+'JLink.exe'
    env.SetDefault( SDKPATH = 'C:/nxp/SDK_RT685/')
    env.SetDefault( TOOLSPATH = 'C:/usr/xtensa/XtDevTools/install/tools/RI-2020.5-win32/XtensaTools/bin/')
    env.SetDefault( CONFIG = [  '--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RI-2020.5-win32/nxp_rt600_RI2020_5_newlib/config/',
                                '--xtensa-core=nxp_rt600_RI2020_5_newlib'
                                ])
    env['LINKLIB'] = 'gdbio' # sim, min-rt, or gdbio, default to gdbio
    hifi4_common(env)

    env.Append(CPPFLAGS = [ '-I'+env['SDKPATH']+'middleware/dsp/naturedsp/hifi4/include',
                            '-I'+env['SDKPATH']+'middleware/dsp/naturedsp/hifi4/include_private',
                            '-DCPU_MIMXRT685SFVKB_dsp',
                            '-D__RT685_HIFI4__'
                        ])
    
    env['SIZE'] = '${TOOLSPATH}xt-size.exe'

def simulator_hifi4_tools(env):

    env.SetDefault( TOOLSPATH = 'C:/usr/xtensa/XtDevTools/install/tools/RI-2020.5-win32/XtensaTools/bin/')
    env.SetDefault( CONFIG = [  '--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RI-2020.5-win32/hifi4_ss_spfpu_7/config/',
                                '--xtensa-core=hifi4_ss_spfpu_7'
                                ])
    env['LINKLIB'] = 'sim'   # gdbio, min-rt, or gdbio, default to gdbio

    hifi4_common(env)
    env.Append(CPPFLAGS = [ '-D__HIFI4_SIMULATOR__',
    						'-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include',
                            '-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include_private',
                            '-c',
                            '-g',
                            '-fdata-sections',
                            '-ffunction-sections',
                            '-LNO:simd',
                           ])    

    env['SIZE'] = '${TOOLSPATH}xt-size.exe'

def qcc7226_hifi4_tools(env):
    # env.SetDefault( XTOCDPATH = 'C:/Tensilica/Xtensa_OCD_Daemon_14.05/') # TODO - TBD for specific HW
    # env.SetDefault( JLINKPATH = 'C:/Program Files/SEGGER/JLink/') # TODO - TBD for specific HW
    # env.SetDefault( SDKPATH = 'C:/nxp/SDK_RT685/') # TODO - BSP TBD
    # env['XTOCDEXE'] = env['XTOCDPATH']+'xt-ocd.exe'
    # env['JLINKEXE'] = env['JLINKPATH']+'JLink.exe'
    env.SetDefault( TOOLSPATH = 'C:/usr/xtensa/XtDevTools/install/tools/RI-2021.7-win32/XtensaTools/bin/')
    env.SetDefault( CONFIG = [  '--xtensa-system=C:/usr/xtensa/XtDevTools/install/builds/RI-2021.7-win32/HiFi4_Aria1_0/config/',
                                '--xtensa-core=HiFi4_Aria1_0'
                            ])
    env['LINKLIB'] = 'gdbio' # sim, min-rt, or gdbio, default to gdbio
    hifi4_common(env)

    env.Append(CPPFLAGS = [ '-D__QCC7226_HIFI4__'])
    env['SIZE'] = '${TOOLSPATH}xt-size.exe'

def hifi5_tools(env):
    env['PROCESSOR'] = 'hifi5'

    # TODO - Eventually we want to separate platform tools vs architecture tools
    env.SetDefault( XTOCDPATH = 'C:/Tensilica/Xtensa_OCD_Daemon_14.08/')
    env.SetDefault( JLINKPATH = 'C:/Program Files/SEGGER/JLink/')

    env.SetDefault( SDKPATH = 'C:/airoha/sdk/')
    env['XTOCDEXE'] = env['XTOCDPATH']+'xt-ocd.exe'
    env['JLINKEXE'] = env['JLINKPATH']+'JLink.exe'
    env.SetDefault( TOOLSPATH = env['SDKPATH']+'dsp/tools/xtensa/RI-2021.8-win32/XtensaTools/bin/')

    # TODO - need to centralize the NatureDSP SDK
    env.SetDefault( CONFIG = [  '--xtensa-system='+env['SDKPATH']+'dsp/tools/xtensa/RI-2021.8-win32/XtensaTools/config/',
                                '--xtensa-core=AIR_PREMIUM_G3_HIFI5',
                                ])
    env['LINKLIB'] = 'gdbio-local' # sim, min-rt, or gdbio, default to gdbio. -local variants use DRAM and IRAM instead of SYSRAM
    env['SIZE'] = '${TOOLSPATH}xt-size.exe'

    hifi_common(env)

    # Allow  auto vectorization, only available through O3
    build_options_o3={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-O3']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }

    env.Replace(CC = '${TOOLSPATH}xt-clang.exe')
    env.Replace(LINK = '${TOOLSPATH}xt-clang.exe')

    env.Replace(build_options = build_options_o3)

    # TODO - Need to put the NatureDSP library in a central location
    env.Replace(CPPFLAGS = [    env['CONFIG'],
                                '-c',
                                '-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include',
                                '-IC:/nxp/SDK_RT685/middleware/dsp/naturedsp/hifi4/include_private',
                                '-g',
                                '-fdata-sections',
                                '-ffunction-sections',
                                '-LNO:simd',
                                '-D__AB1585_HIFI5__'
                            ])

    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options_o3[env['BUILD']]['cppflags'])

    # Change runner to gdb so that both sim and HW profiling can be used
    env.Replace(RUNNER = 'xt-gdb.exe')

def hexagon_tools(env):
    build_options = {
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-O3','-g0']},
        'Debug':{'cppflags':['-O0','-g0']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'hexagon'
    env.SetDefault(CONFIG = ['-G0', '-mv66'])
    env.SetDefault( TOOLSPATH = 'C:/Qualcomm/Hexagon_SDK/4.5.0.3/tools/HEXAGON_Tools/8.5.08/Tools/bin/' )
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['AS'] = '${TOOLSPATH}hexagon-clang.exe'
    env['ASFLAGS'] = '-c'
    env['CC'] = '${TOOLSPATH}hexagon-clang.exe'
    env.Replace(CPPFLAGS = env['CONFIG'])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env.Append(CPPFLAGS = ['-flto', '-fPIC','-Wall', '-Wno-cast-align', '-Wpointer-arith', '-Wno-missing-braces', '-Wno-strict-aliasing', '-Wstrict-prototypes', '-Wnested-externs'])
    env.Append(CPPFLAGS = ['-fno-exceptions', '-fno-strict-aliasing', '-fno-zero-initialized-in-bss', '-fdata-sections'])
    env['AR'] = '${TOOLSPATH}hexagon-ar.exe'
    env['ARFLAGS'] = '-rsc'
    env['ARXFLAGS'] = 'xo'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '${TOOLSPATH}hexagon-clang.exe'
    env['LINKFLAGS'] = ['-g','-G0', '-mv66', '-mG0lib']
    env['LIBS']= '-lhexagon'

    env['PROGSUFFIX'] = '.out'
    env['OBJSUFFIX'] = '.o'
    env['LIBSUFFIX'] = '.lib'
    env['MAPSUFFIX'] = '.map'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX'] =''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = ''
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env['RUNNER'] = 'hexagon-sim.exe'
    env.Replace(RUNNERFLAGS = ['-mv66g_1024', '--timing', '--simulated_returnval'])
    #env.Replace(RUNNERFLAGS = ['-mv66g_1024', '--simulated_returnval'])
    env_common(env)

def hexagon_v66_tools(env):
    env.SetDefault( TOOLSPATH = 'C:/Qualcomm/Hexagon_SDK/5.3.0.0/tools/HEXAGON_Tools/8.6.05/Tools/bin/' )
    hexagon_tools(env)
    env['PROCESSOR'] = 'hexagon_v66'

def hexagon_v73_tools(env):
    env.SetDefault(CONFIG = ['-G0', '-mv73'])
    env.SetDefault( TOOLSPATH = 'C:/Qualcomm/Hexagon_SDK/5.3.0.0/tools/HEXAGON_Tools/8.6.05/Tools/bin/' )
    hexagon_tools(env)
    env['LINKFLAGS'] = ['-g','-G0', '-mv73', '-mG0lib']
    env.Replace(RUNNERFLAGS = ['-mv73', '--timing', '--simulated_returnval'])
    env['PROCESSOR'] = 'hexagon_v73'

def sharc_common(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-O', '-Ov100', '-g']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O', '-Ov100']},
        'Speed':{'cppflags':['-O', '-Ov100']},
        'Size':{'cppflags':['-O', '-Ov0']},
        'GenPGO':{'cppflags':['-pguide', '-prof-hw']},
        'CircBuf':{'cppflags':['-force-circbuf']},
        'SaveTemps':{'cppflags':['-save-temps']},
    }
    env['build_options'] = build_options
    env['AS'] = '\"${TOOLSPATH}easm21k.exe\"'
    env.Replace(ASFLAGS = env['CONFIG'])
    env.Append(ASFLAGS = ['-double-size-32', '-char-size-8', '-swc'])
    env.Append(ASFLAGS = ['-D_INCLUDE_LEGACY_SYSREG_NAMES'])
    env['CC'] = '\"${TOOLSPATH}cc21k.exe\"'
    env.Replace(CPPFLAGS = env['CONFIG'])
    env.Append(CPPFLAGS = ['-double-size-32', '-char-size-8', '-swc'])
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['AR'] = '\"${TOOLSPATH}elfar.exe\"'
    env['ARFLAGS'] = '-c'
    env['ARXFLAGS'] = '-e'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '\"${TOOLSPATH}cc21k.exe\"'
    env.Replace(LINKFLAGS = env['CONFIG'])
    env.Append(LINKFLAGS = ['-flags-link', '-ip', '-flags-link', '-e'])
    env['MAPFLAGS'] = '-map='
    env['PROGSUFFIX'] = '.dxe'
    env['OBJSUFFIX'] = '.doj'
    env['LIBSUFFIX'] = '.dlb'
    env['MAPSUFFIX'] = '.map.xml'
    env['LIBPREFIX'] =''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = '.dlb'
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env_common(env)

def sharc_tools(env):
    env['PROCESSOR'] = 'sharc'
    env.SetDefault(CONFIG = ['-proc', 'ADSP-SC584','-si-revision', '1.0'])
    env.SetDefault(TOOLSPATH = 'C:/Analog Devices/CrossCore Embedded Studio 2.6.0/')
    sharc_common(env)

def gul_tools(env):
    env['PROCESSOR'] = 'gul'
    env.SetDefault(CONFIG = ['-proc', 'ADSP-21569','-si-revision', '0.0'])
    env.SetDefault(TOOLSPATH = 'C:/Analog Devices/CrossCore Embedded Studio 2.9.1/')
    sharc_common(env)

def gxp_tools(env):
    env['PROCESSOR'] = 'gxp'
    env.SetDefault(CONFIG = ['-proc', 'ADSP-21593W','-si-revision', '0.0'])
    env.SetDefault(TOOLSPATH = 'C:/Analog Devices/CrossCore Embedded Studio 2.10.0/')
    sharc_common(env)

def sharc_genpgo_override(env):
    env['PROGSUFFIX'] = '_pgo.dxe'
    env['OBJSUFFIX'] = '_pgo.doj'
    env['LIBSUFFIX'] = '_pgo.dlb'
    env['MAPSUFFIX'] = '_pgo.map.xml'
    env['LIBLINKSUFFIX'] = '_pgo.dlb'

def gul_evsom_override(env, runner, tools_path=None, arch_config=None):
    tools_path_default = 'C:/Analog Devices/CrossCore Embedded Studio 2.10.0/'
    arch_config_default = ['-proc', 'ADSP-21569','-si-revision', 'any']
    env.Replace(TOOLSPATH = tools_path if tools_path else tools_path_default)
    env.Replace(CONFIG = arch_config if arch_config else arch_config_default)
    sharc_common(env)
    if runner == 'ezkit':
        gul_ezkit_runner(env)
    elif runner == 'sim':
        gul_sim_runner(env)

def sc594_evsom_override(env, runner, tools_path=None, arch_config=None):
    tools_path_default = 'C:/Analog Devices/CrossCore Embedded Studio 2.11.0/'
    arch_config_default = ['-proc', 'ADSP-SC594W','-si-revision', 'any']
    env.Replace(TOOLSPATH = tools_path if tools_path else tools_path_default)
    env.Replace(CONFIG = arch_config if arch_config else arch_config_default)
    sharc_common(env)
    if runner == 'ezkit':
        sc594_ezkit_runner(env)
    elif runner == 'sim':
        sc594_sim_runner(env)

def sc598_evsom_override(env, runner, tools_path=None, arch_config=None):
    tools_path_default = 'C:/Analog Devices/CrossCore Embedded Studio 2.11.0/'
    arch_config_default = ['-proc', 'ADSP-SC598W','-si-revision', 'any']
    env.Replace(TOOLSPATH = tools_path if tools_path else tools_path_default)
    env.Replace(CONFIG = arch_config if arch_config else arch_config_default)
    sharc_common(env)
    if runner == 'ezkit':
        sc598_ezkit_runner(env)
    elif runner == 'sim':
        sc598_sim_runner(env)

def gul_sim_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"ChipFactory Simulator"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-21569 Functional-Sim"'])

def gul_ezkit_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-21569 via ICE-2000"'])

def gxp_sim_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"ChipFactory Simulator"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-21593W Functional-Sim"'])

def gxp_ezkit_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-21593W via ICE-2000"'])

def sc594_sim_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"ChipFactory Simulator"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC594W Functional-Sim"'])

def sc594_ezkit_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC594W via ICE-2000"'])

def sc598_sim_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"ChipFactory Simulator"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC598W Functional-Sim"'])

def sc598_ezkit_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC598W via ICE-2000"'])

def sharc_sim_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"ChipFactory Simulator"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC584 Functional-Sim"'])

def sharc_ezkit_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"ADSP-SC584 via ICE-2000"'])

def melville_ezkit_single_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"Melville SHARC1 via ICE-2000"'])
    #env.Append(RUNNERFLAGS = [ '--platform', '"Dual SHARC via ICE-2000"'])

def melville_ezkit_dual_runner(env):
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['RUNNER'] = 'CCES_Runner.exe'
    env.Replace(RUNNERFLAGS = ['--cceshome', '"'+env['TOOLSPATH']+'"'])
    env.Append(RUNNERFLAGS = ['--processor', env['CONFIG'][1]])
    env.Append(RUNNERFLAGS = ['--target', '"Emulation Debug Target"'])
    env.Append(RUNNERFLAGS = [ '--platform', '"Dual SHARC via ICE-2000"'])

def x86_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'x86'
    name = platform.platform()    
    if 'Windows' in name:
        env.SetDefault( TOOLSPATH = 'C:/MinGW64/mingw64-6.3.0/bin/' )
    elif 'Linux' in name:
        env.SetDefault( TOOLSPATH = '/usr/bin/' )
        env['PROGSUFFIX'] = '.out'
    else:
        print('Info: OS not detected by python platform(). Some env attributes not set.')
    env['CC'] = '${TOOLSPATH}gcc'
    env['CXX'] = '${TOOLSPATH}g++'
    env['AR'] = '${TOOLSPATH}ar'
    env['SIZE'] = '${TOOLSPATH}size'
    env['CPPFLAGS'] = [ '-m32', '-fmessage-length=0', '-std=c99']
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['LINKFLAGS'] = ['-m32']
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBSUFFIX'] = '.lib'
    env['MAPSUFFIX'] = '.map'
    env['LIBPREFIX']=''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = '.lib'
    env['RANLIBCOM'] = '' #don't call ranlib
    if 'TOOLSPATH' not in env :
        env['ENV']['PATH'] += ';'+';'.join(['C:/MinGW64/mingw64-6.3.0/bin','/usr/bin'])
    else :
        env['ENV']['PATH'] = env['TOOLSPATH'] + ';' + env['ENV']['PATH']
    env_common(env)

def android_common(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'android'
    # Android NDK may be needed at another path, developer should use --tool=<path>
    env.SetDefault( TOOLSPATH = 'C:/Android/SDK/ndk/25.2.9519653/toolchains/llvm/prebuilt/windows-x86_64/bin/' )
    env['CC'] = env['TOOLSPATH']+'clang.exe'
    env['CXX'] = env['TOOLSPATH']+'clang++.exe'
    env['AR'] = env['TOOLSPATH']+'llvm-ar.exe'
    env['SIZE'] = env['TOOLSPATH']+'llvm-size.exe'

    # TODO - -std=c11 missing from CPPFLAGS due to CPPFLAGS being added to CXXFLAGS
    env['CPPFLAGS'] = [ '-fPIC', '-m64', '-fmessage-length=0', '--target=${PLATFORM_TARGET}']
    env['CXXFLAGS'] = ['-std=c++14']
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['LINKFLAGS'] = ['-m64', '--target=${PLATFORM_TARGET}']
    env['SHLINKFLAGS'] = ['-m64', '--target=${PLATFORM_TARGET}']
    env['ARFLAGS'] = 'r'
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX']='lib'
    env['LIBSUFFIX'] = '_${PLATFORM_TARGET}.a'
    env['SHLIBPREFIX'] = 'lib'
    env['SHLIBSUFFIX'] = '.so'
    env['MAPSUFFIX'] = '.map'
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = '.a'
    env['RANLIBCOM'] = '' #don't call ranlib
    if 'TOOLSPATH' not in env :
        env['ENV']['PATH'] += ';'+';'.join(['C:/Android/SDK/ndk/25.2.9519653/toolchains/llvm/prebuilt/windows-x86_64/bin/'])
    else :
        env['ENV']['PATH'] = env['TOOLSPATH'] + ';' + env['ENV']['PATH']
    env_common(env)

def android_aarch64_tools(env):
    env.SetDefault( PLATFORM_TARGET = 'aarch64-linux-android33')
    android_common(env)

def android_armv7a_tools(env):
    env.SetDefault( PLATFORM_TARGET = 'armv7a-linux-androideabi33')
    android_common(env)

def android_x86_64_tools(env):
    env.SetDefault( PLATFORM_TARGET = 'x86_64-linux-android33')
    android_common(env)

def x64_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-fpermissive']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'x64'
    name = platform.platform() 
    if 'Windows' in name:
        env.SetDefault( TOOLSPATH = 'C:/MinGW64/mingw64-6.3.0/bin/' )
    elif 'Linux' in name:
        env.SetDefault( TOOLSPATH = '/usr/bin/' )
        env['PROGSUFFIX'] = '.out'
    else:
        print('Info: OS not detected by python platform(). Some env attributes not set.')               
    env['CC'] = '${TOOLSPATH}g++'
    env['CXX'] = '${TOOLSPATH}g++'
    env['AR'] = '${TOOLSPATH}ar'
    env['SIZE'] = '${TOOLSPATH}size'
    env['CPPFLAGS'] = ['-fPIC', '-m64', '-fmessage-length=0', '-std=c++11']
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['LINKFLAGS'] = ['-m64']
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBSUFFIX'] = '.lib'
    env['MAPSUFFIX'] = '.map'
    env['LIBPREFIX']=''
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = '.lib'
    env['SHLIBNAME'] = ''
    env['RANLIBCOM'] = '' #don't call ranlib
    if 'TOOLSPATH' not in env :
        env['ENV']['PATH'] += ';'+';'.join(['C:/MinGW64/mingw64-6.3.0/bin','/usr/bin'])
    else :
        env['ENV']['PATH'] = env['TOOLSPATH'] + ';' + env['ENV']['PATH']
    env_common(env)

def amd64_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g', '-fpermissive']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'amd64'
    if 'TOOLSPATH' not in env :
        #env['TOOLSPATH']= 'C:/MinGW64/toolchain/bin/'
        env['TOOLSPATH']= 'C:/MinGW64/x86_64-pc-linux-gnu/bin/'
    env['ENV']['PATH'] = env['TOOLSPATH']
    env['CC'] = '${TOOLSPATH}x86_64-pc-linux-gnu-g++.exe'
    env['CXX'] = '${TOOLSPATH}x86_64-pc-linux-gnu-g++.exe'
    env['SIZE'] = '${TOOLSPATH}x86_64-pc-linux-gnu-size.exe'
    env['CPPFLAGS'] = [ '-fPIC', '-m64', '--sysroot='+env['TOOLSPATH']+'/../x86_64-pc-linux-gnu/sysroot']
    env['CXXFLAGS'] = ['-std=c++11']
    #env['CFLAGS'] = ['-std=gnu99']
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['CPPPATH'] = ['usr/include']
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['LIBPREFIX']='lib'
    env['LIBLINKPREFIX'] = 'lib'
    env['LIBLINKSUFFIX'] = '.a'
    env['LINKFLAGS'] = ['-Llib64', '-lm', '-lrt', '-ljack', '-lpthread', '-lstdc++', '--sysroot='+env['TOOLSPATH']+'/../x86_64-pc-linux-gnu/sysroot']
#    env['RANLIBCOM'] = ''
    env['SHLIBPREFIX'] = 'lib'
    env['SHLIBSUFFIX'] = '.so'
    env['PROGSUFFIX'] = '.out'
    env_common(env)

def linux_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-g']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'amd64'
    if 'TOOLSPATH' not in env :
        env['TOOLSPATH']= '/usr/bin/'
    env['ENV']['PATH'] = env['TOOLSPATH']
    env['CC'] = 'g++'
    env['SIZE'] = 'size'
    env['CPPFLAGS'] = [ '-fPIC', '-m64', '-std=gnu99']
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['CPPPATH'] = ['usr/include']
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['LIBPREFIX']='lib'
    env['LIBLINKPREFIX'] = 'lib'
    env['LIBLINKSUFFIX'] = '.a'
    env['LINKFLAGS'] = ['-Llib64', '-lm', '-lrt', '-lpthread']
#    env['RANLIBCOM'] = ''
    env['PROGSUFFIX'] = '.out'
    env['MATLAB'] = '\"C:/Program Files/MATLAB/R2022b/bin/matlab.exe\"'

def qnx_nto_aarch64_tools(env):
    build_options={
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-O3', '-g', '-fpermissive']},
        'Debug':{'cppflags':['-g']},
        'Release':{'cppflags':['-O3']},
    }
    env['build_options'] = build_options
    env['PROCESSOR'] = 'qnx_nto_aarch64'
    # QNX tools user environment variable USERPROFILE to figure out the
    # location of license file
    name = platform.platform()    
    if 'Windows' in name:
        if not 'USERPROFILE' in os.environ:
            print('Info: USERPROFILE not found in os.environ. Required for qnx_nto_aarch64 architecture to pick up license file')
        env['ENV']['USERPROFILE'] = os.environ['USERPROFILE']
        env.SetDefault( TOOLSPATH = 'C:/QNX/qnx710/' )
        env['ENV']['QNX_BASE'] = env['TOOLSPATH']
        env['ENV']['QNX_TARGET'] = f"{env['TOOLSPATH']}target/qnx7"
        env['ENV']['QNX_HOST'] = f"{env['TOOLSPATH']}host/win64/x86_64"
    elif 'Linux' in name:        
        env.SetDefault( TOOLSPATH = f"{os.environ['QNX_TARGET']}../../" )
        env['ENV']['HOME'] =  os.environ['HOME']
        env['ENV']['QNX_BASE'] = f"{os.environ['QNX_TARGET']}../../" #env['TOOLSPATH']
        env['ENV']['QNX_TARGET'] = os.environ['QNX_TARGET'] #f"/home/ballmer/sanga/prebuilt_QNX710_r4.2//target/qnx7"
        env['ENV']['QNX_HOST'] = os.environ['QNX_HOST']  #f"/home/ballmer/sanga/prebuilt_QNX710_r4.2//host/linux/x86_64"
    else:
        print('Info: OS not detected by python platform(). Some env attributes not set.')
    # Changed to q++ from qcc to support building of Btf_test and Bcf_test
    env['CC'] = f"{env['ENV']['QNX_HOST']}/usr/bin/q++  -V8.3.0,gcc_ntoaarch64le"
    env['CXX'] = f"{env['ENV']['QNX_HOST']}/usr/bin/q++  -V8.3.0,gcc_ntoaarch64le"
    env['AR'] = f"{env['ENV']['QNX_HOST']}/usr/bin/ntoaarch64-ar"
    env['SIZE'] = f"{env['ENV']['QNX_HOST']}/usr/bin/ntoaarch64-size"
    env['CPPFLAGS'] = [ '-fPIC', '-Wall', '-fmessage-length=0']
    env['CLAGS'] = [ '-fPIC', '-Wall', '-fmessage-length=0', '-std=gnu99']
    env['CPPPATH'] =f"{env['TOOLSPATH']}target/qnx7/usr/include"
    env['build_options']['Base']['cppflags'] = env['CPPFLAGS']
    env.Append(CPPFLAGS = build_options[env['BUILD']]['cppflags'])
    env['LINKFLAGS'] = ['-lm']
    env['LIBPATH'] = f"{env['TOOLSPATH']}/target/qnx7/aarch64le/lib"
    env['ARXFLAGS'] = 'x'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX']='lib'    
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['SHLIBPREFIX'] = 'lib'
    env['SHLIBNAME'] = 'lib_bose'
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = '.a'
    env['PROGSUFFIX'] = '.out'
    env['SHLIBSUFFIX'] = '.so'
    env['RANLIBCOM'] = '' #don't call ranlib
    env_common(env)

def rh_tools(env):
    build_options = {
        'Base':{'cppflags':[]},
        'Default':{'cppflags':['-Ogeneral','-G']},
        'Debug':{'cppflags':['-O0','-g0']},
        'Release':{'cppflags':['-OI', '-Ospeed']},
    }
   
    env['PROCESSOR'] = 'rh850'
    env.SetDefault( TOOLSPATH = 'C:/ghs/comp_201517/' )
    env.Replace(RUNNERPATH = env['TOOLSPATH'])
    env['AS'] = '${TOOLSPATH}ease850.exe'
    env['ASFLAGS'] = ''
    env['CC'] = '${TOOLSPATH}ccrh850.exe'
    env['CXX'] = '${TOOLSPATH}cxrh850.exe'
    env.Append(CPPFLAGS = build_options['Base']['cppflags'])
    env.Append(CFLAGS = ['-gnu99']) #for C
    env.Append(CXXFLAGS = ['--ee', '--g++']) #for CPP
    env.Append(CPPFLAGS = ['-G', '-no_ansi_alias', '-bsp', 'generic', '-cpu=rh850g3kh']) #common
    env['AR'] = '${TOOLSPATH}ax.exe'
    env['ARFLAGS'] = '-rc'
    env['ARXFLAGS'] = 'xo'
    env['RANLIBCOM'] = '' #don't call ranlib
    env['LINK'] = '${TOOLSPATH}elxr.exe'
    env['LINKFLAGS'] = []
    env['LIBS']= ''

    env['PROGSUFFIX'] = '.out'
    env['OBJSUFFIX'] = '.o'
    env['LIBSUFFIX'] = '.a'
    env['MAPSUFFIX'] = '.map'
    env['MAPFLAGS'] =  '-Wl,-Map='
    env['LIBPREFIX'] = 'lib'
    env['LIBLINKPREFIX'] = ''
    env['LIBLINKSUFFIX'] = ''
    env['ENV'].update({'SystemDrive': 'C:/'}) #cygwin needs this
    env['RUNNER'] = ''
    env.Replace(RUNNERFLAGS = [])
    env_common(env)
