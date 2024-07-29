import os

Import('env')
Import('runtime')
Import('platform')
Import('lib')
Import('target')
Import('test_bin')
Import('test_ascii')
Import('test')
Import('framework')
Import('misc_root')
Import('test_path')
Import('OptPGO')
Import('option')

local_env = env.Clone(
    CPPPATH=['.',runtime+'/BAF', runtime, platform],
    )
app_root=local_env['APP_ROOT']

def compile_target(target, source, env, for_signature):
    return '$CCCOM'

OptArch = local_env['PROCESSOR']
if option and ('BuildOptions' in option) and (option['BuildOptions']):
    if (OptArch in option['BuildOptions']) and (option['BuildOptions'][OptArch]):
        if ('cpp_flags' in option['BuildOptions'][OptArch]) and (option['BuildOptions'][OptArch]['cpp_flags']):
            custom_cppflags = option['BuildOptions'][OptArch]['cpp_flags']
            cppflags= local_env['build_options']['Base']['cppflags']
            for custom_cppflag in custom_cppflags:
                if custom_cppflag in local_env['build_options']:
                    cppflags.extend(local_env['build_options'][custom_cppflag]['cppflags'])
                else:
                    cppflags.extend([custom_cppflag])
            local_env.Replace(CPPFLAGS = cppflags)
        if ('xcpp_flags' in option['BuildOptions'][OptArch]) and (option['BuildOptions'][OptArch]['xcpp_flags']):
            for xcpp_flag in option['BuildOptions'][OptArch]['xcpp_flags']:
                if xcpp_flag == 'genpgo':
                    local_env.Append(CPPFLAGS = local_env['build_options']['GenPGO']['cppflags'])
                elif (xcpp_flag == 'usepgo') and (OptArch in ['sharc','gul','gxp']):
                    pgo_file = local_env['variant_root'] + OptArch + '/pgo/Baf.pgo'
                    local_env.Append(CPPFLAGS = pgo_file)
                else:
                    local_env.Append(CPPFLAGS = xcpp_flag)

        if ('link_flags' in option['BuildOptions'][OptArch]) and (option['BuildOptions'][OptArch]['link_flags']):
            xtra_linkflags = option['BuildOptions'][OptArch]['link_flags']
            local_env.Append(LINKFLAGS = xtra_linkflags)

ojsp_env = local_env.Clone()

if option and ('JSched' in option) and (option['JSched']):
    ojsp_env.Append(CPPFLAGS='-DBAF_PRIORITY_SCHEDULER')

if option and ('ExtScheduler' in option) and (option['ExtScheduler']):
    ojsp_env.Append(CPPFLAGS='-DFRAMEWORK_EXT_SCHEDULER=%d' % (option['ExtScheduler']))
else:
    ojsp_env.Append(CPPFLAGS='-DFRAMEWORK_EXT_SCHEDULER=0')

if option and ('MultiRatePlatform' in option) and option['MultiRatePlatform']:
    local_env.Append(CPPFLAGS='-DPLATFORM_MULTIRATE_AUDIOTHREADS')
    ojsp_env.Append(CPPFLAGS='-DPLATFORM_MULTIRATE_AUDIOTHREADS')
else:
    local_env.Append(CPPFLAGS='-DPLATFORM_BASERATE_AUDIOTHREAD')
    ojsp_env.Append(CPPFLAGS='-DPLATFORM_BASERATE_AUDIOTHREAD')

if option and ('BaseThreadLevel' in option) and option['BaseThreadLevel']:
    local_env.Append(CPPFLAGS='-DPLATFORM_BASETHREADLEVEL=%d' % (option['BaseThreadLevel']))
    ojsp_env.Append(CPPFLAGS='-DPLATFORM_BASETHREADLEVEL=%d' % (option['BaseThreadLevel']))
else:
    local_env.Append(CPPFLAGS='-DPLATFORM_BASETHREADLEVEL=0')
    ojsp_env.Append(CPPFLAGS='-DPLATFORM_BASETHREADLEVEL=0')

stats_env = local_env.Clone()
stats_env['BUILDERS']['Compile'] = stats_env.Builder(generator = compile_target, suffix='$OBJSUFFIX', src_suffix='.c')

if option and ('Logger' in option) and option['Logger']:
    if option['Logger']['enable']:
        ojsp_env.Append(CPPFLAGS='-DBAF_LOGGER')
        ojsp_env.Append(CPPFLAGS='-DBAF_LOGGER_LOG_SIZE=%d'        % option['Logger']['logSize'])
        ojsp_env.Append(CPPFLAGS='-DBAF_LOGGER_PRINTLOG_SIZE=%d'   % option['Logger']['printLogSize'])
        ojsp_env.Append(CPPFLAGS='-DBAF_LOGGER_CAPTURELOG_SIZE=%d' % option['Logger']['captureLogSize'])
        ojsp_env.Append(CPPFLAGS='-DBAF_LOGGER_EXECLOG_SIZE=%d'    % option['Logger']['execLogSize'])

if option and ('BAFStatsLogger' in option) and option['BAFStatsLogger']:
    if option['BAFStatsLogger']['enable']:
        ojsp_env.Append(CPPFLAGS='-DBAF_STATSLOGGER')

def ram_vecs(target, source, env, for_signature):
    if env['PROCESSOR'] in ['sharc','gul','gxp']:
        if OptPGO and (OptPGO == 'genpgo'):
            return ['$AS $ASFLAGS -I'+test_path+' -DRAM_TEST -DPGO_SUPPORT -o $TARGET $SOURCE']
        else:
            return ['$AS $ASFLAGS -I'+test_path+' -DRAM_TEST -o $TARGET $SOURCE']
    elif env['PROCESSOR'] in ['x86','amd64','hexagon','hexagon_v66','hexagon_v73','hifi4', 'hifi5', 'qnx_nto_aarch64']:
        return ['$CC -o $TARGET -I'+test_path+' -c $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCE']
    elif env['PROCESSOR'] in ['c66']:
        return ['$CC -fe $TARGET -I'+test_path+' -c $CFLAGS $CCFLAGS $_CCCOMCOM $SOURCE']

# Create RamVecs builder
local_env['BUILDERS']['RamVecs'] = env.Builder(generator = ram_vecs)

if local_env['PROCESSOR'] == 'gxp':
    if ('CORE_ID' in framework):
        if framework['CORE_ID'] == '1':
            ojsp_env.Append(CPPFLAGS='-DSPC1')
        else:
            ojsp_env.Append(CPPFLAGS='-DSPC2')

if local_env['PROCESSOR'] in ['sharc','gul','gxp']:
    startup_ldf = platform+'/startup_ldf'
    if local_env['PROCESSOR'] == 'gxp':
        if ('CORE_ID' in framework):
            if framework['CORE_ID'] == '1':
                startup_ldf = platform+'/startup_ldf/spc1'
            else:
                startup_ldf = platform+'/startup_ldf/spc2'
            local_env['LINKFLAGS'].append(['-flags-link', '-MDSM_FILE=%s' % os.path.join(os.path.dirname(Dir(target).path),'l2_shared.sm')])

    if option and ('linker_cmd_file' in option) and (option['linker_cmd_file']):
        local_env['LINKFLAGS'].append(['-T', option['linker_cmd_file']])
    else:
        local_env['LINKFLAGS'].append(['-T', app_root+'/'+startup_ldf+'/app.ldf'])
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])
    if option and ('bank_mapping_inc' in option) and (option['bank_mapping_inc']):
        if option['bank_mapping_inc'] != None:
            local_env['LINKFLAGS'].append(['-flags-link', '-MDINC_JTEST_BANK_MAPPING'])
            local_env['LINKFLAGS'].append(['-I', option['bank_mapping_inc']])
    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.s']+[test_bin],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    if OptPGO and (OptPGO == 'genpgo'):
        source += [local_env.Object(platform+'/pgo_ctl.s')]

    if option and ('BAFStatsLogger' in option) and option['BAFStatsLogger']:
        if option['BAFStatsLogger']['enable']:
            if option['BAFStatsLogger']['statsLogger'] == 'PerfMon':
                stats_env.Append(CPPFLAGS='-DPERFMON_MODE_%s'% option['BAFStatsLogger']['statsConfig'])
                stats_obj = stats_env.Compile(target = 'baf_stats.doj', source = misc_root+'/sharcxi/perfmon.c')
                source += [stats_obj]
            elif option['BAFStatsLogger']['statsLogger'] == 'StackMon':
                stats_obj = stats_env.Compile(target = 'baf_stats.doj', source = misc_root+'/sharcxi/stackmon.c')
                source += [stats_obj]
            elif option['BAFStatsLogger']['statsLogger'] == 'Ccnt64Mon':
                stats_obj = stats_env.Compile(target = 'baf_stats.doj', source = misc_root+'/sharcxi/ccnt64mon.c')
                source += [stats_obj]

    source += [local_env.Object(startup_ldf+'/app_startup.s'),
        local_env.Object(startup_ldf+'/app_heaptab.c'),
        local_env.Object(startup_ldf+'/app_IVT.s'),
        ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c')]

    if option and ('link_option' in option) and option['link_option'] == 'file_list':
        for x in lib:
            lst_file = ('@%s' % (x[0])).replace(env['LIBSUFFIX'], env['LIBSUFFIX'].replace('.','_')+'.lst')
            local_env['LINKFLAGS'].append(['-flags-link', lst_file])
    else:
        source += [lib]

    JApplication = local_env.Program(
        target = target,
        source = source)
    if option and ('linker_cmd_file' in option) and (option['linker_cmd_file']):
        local_env.Depends(JApplication, '#/'+option['linker_cmd_file'])
    else:
        local_env.Depends(JApplication, startup_ldf+'/app.ldf')
    if option and ('bank_mapping_inc' in option) and (option['bank_mapping_inc']):
        if option['bank_mapping_inc'] != None:
            local_env.Depends(JApplication, '#/'+ option['bank_mapping_inc'] + '/jtestBankMapping.h')

elif local_env['PROCESSOR'] in ['c66']:
    local_env['LINKFLAGS'].append([app_root+'/'+platform+'/c6678_lnk.cmd'])
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
    local_env.Depends(JApplication, platform+'/c6678_lnk.cmd')

elif local_env['PROCESSOR'] in ['hifi4']:
    local_env['LINKFLAGS'].append(local_env['LSPFLAGS']+app_root+'/'+platform+'/xtensa/'+local_env['LINKLIB'])
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/memmap.xmm')
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/specs')
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/ldscripts/elf32xtensa.x')

elif local_env['PROCESSOR'] in ['hifi5']:
    local_env['LINKFLAGS'].append(local_env['LSPFLAGS']+app_root+'/'+platform+'/xtensa/'+local_env['LINKLIB'])
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/memmap.xmm')
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/specs')
    local_env.Depends(JApplication, platform+'/xtensa/'+local_env['LINKLIB']+'/ldscripts/elf32xtensa.x')

elif local_env['PROCESSOR'] in ['hexagon','hexagon_v66','hexagon_v73']:
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    if option and ('BAFStatsLogger' in option) and option['BAFStatsLogger']:
        if option['BAFStatsLogger']['enable']:
            if option['BAFStatsLogger']['statsLogger'] == 'PktMon':
                stats_obj = stats_env.Compile(target = 'baf_stats'+local_env['OBJSUFFIX'], source = misc_root+'/hexagon/pktmon.c')
                source += [stats_obj]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
elif local_env['PROCESSOR'] in ['qnx_nto_aarch64']:
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
elif local_env['PROCESSOR'] in ['x86', 'amd64']:
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)
else:
    local_env['LINKFLAGS'].append(local_env['MAPFLAGS']+Dir(target).path+local_env['MAPSUFFIX'])

    source = []
    if test_path:
        RamVecs = local_env.RamVecs(
          target = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']],
          source = [platform+'/ram_vecs.c']+[test_ascii],
          PROCESSOR = local_env['PROCESSOR'])
        source = [framework['Name']+'/'+platform+'/'+test+'/ram_vecs'+local_env['OBJSUFFIX']]

    source += [ojsp_env.Object(target=framework['Name']+'/'+'OJSP'+local_env['OBJSUFFIX'], source='OJSP.c'),
        local_env.Object('Integration_Example.c'),
        lib]
    JApplication = local_env.Program(
        target = target,
        source = source)

Return("JApplication")