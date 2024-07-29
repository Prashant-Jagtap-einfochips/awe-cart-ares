import os
import sys
from SCons.Script.Main import GetOption
from SCons.Environment import Environment
from shutil import copyfile
import OJTSI

app_root =  os.environ['APP_ROOT']

sys.path.append( app_root+'/Tools/JAutogen')
sys.path.append( app_root+'/Tools/JConfig')

from  shutil import copyfile

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

from  JSimulation import *
from  JAutogen import *
from  JAutogenBafLite import *
from  JAutogenProbe import *

def lib_merge(target, source, env, for_signature):
    for file in env.Glob('*'+env['OBJSUFFIX'], strings=True ): # remove any .o file
        os.unlink(file)
    merge_cmd=[]
    for src in source :
        merge_cmd += ['$AR $ARXFLAGS ' + str(src)]
    merge_cmd += ['$AR $ARFLAGS '  + str(target[0]) + ' *$OBJSUFFIX']
    return merge_cmd

def lib_merge_alt(target, source, env, for_signature):
    merge_cmd = ['$AR $ARFLAGS '  + str(target[0]) + ' @' + str(source[0])]
    return merge_cmd

def lib_lst_merge(target, source, env):
    with open('%s' % (target[0]), 'w') as fpout:
        for src in source :
            lst = ('%s' % src).replace(env['LIBSUFFIX'], env['LIBSUFFIX'].replace('.','_')+'.lst')
            with open(lst, 'r') as fpin:
                if env['arch'] in ['qnx_nto_aarch64']:
                    fpout.writelines([x.replace('\\','/') for x in fpin.readlines()])
                else:
                    fpout.writelines(fpin.readlines())

def generate_simulink_config(target, source, env):
    classname = os.path.splitext(os.path.basename('%s' % (target[0])))[0]
    with open('%s' % (target[0]), 'w') as m_file:
        m_file.write('classdef '+classname+'\n')
        m_file.write('    properties ( Constant = false )\n')
        param = env['JModule']['Parameters']
        if ('Version' in param ) and (param['Version'] == 2):
            if ('MatlabStruct' in param):
                for line in param['MatlabStruct']:
                    line = line.replace('[[', '[') #bidimensional array
                    line = line.replace(']]', ']')
                    line = line.replace('], [', '; ')
                    line = line.replace(']), matlab.double([', '; ')
                    line = line.replace(']), matlab.single([', '; ')
                    line = line.replace('matlab.', '')
                    m_file.write(line)
                m_file.write('   end\n')
            if ('MatlabClassInit' in param):
                m_file.write('   methods \n')
                m_file.write('      function obj='+classname+'\n')
                for k,v in param['MatlabClassInit'].items():
        #         print(k+':'+str(jobj.configFlat[k]))
                    if isinstance(v, str):
                        line = '         '+k+"='"+str(v)+"';\n"
                    else:
                        line = '         '+k+'='+str(v)+';\n'
                    line = line.replace('[[', '[') #bidimensional array
                    line = line.replace(']]', ']')
                    line = line.replace('], [', '; ')
                    line = line.replace(']), matlab.double([', '; ')
                    line = line.replace(']), matlab.single([', '; ')
                    line = line.replace('matlab.', '')
                    m_file.write(line)
                m_file.write('      end\n')
                m_file.write('   end\n')
        else: # legacy
            for parameter in param.keys():
                param_str = str(param[parameter])
                param_str = param_str.replace('[[', '[') #bidimensional array
                param_str = param_str.replace(']]', ']')
                param_str = param_str.replace('], [', '; ')
                m_file.write('        '+ parameter + ' = ' + param_str + ';\n')
            m_file.write('    end\n')
        m_file.write('end\n')
    with open( str(target[0])[:-1]+'gv', 'w') as graph_file: #replace .m with .gv
        if ('Version' in param ) and (param['Version'] == 2):
            if ('Graph' in param):
                for line in param['Graph']:
                    graph_file.write(line+'\n')

def generate_simulink_probepoints(target, source, env):
    if ('ProbePoints' in env) and (env['ProbePoints']):
        for tgt in target:
            with open('%s' % (tgt), 'w') as m_file:
                if env['ProbePoints']['enable'] == True:
                    m_file.write('probepoint_coder = true;\n')
                else:
                    m_file.write('probepoint_coder = false;\n')
                for k,v in env['ProbePoints']['processes'].items():
                    m_file.write('probepoint_%s_begin     = 0x00%02x%02x00;\n' % (k, v['thread_level'], v['process_id']))
                    m_file.write('probepoint_%s_end       = 0x00%02x%02x01;\n' % (k, v['thread_level'], v['process_id']))

def generate_atomiclist_config(target, source, env):
    # Parse JConfig to re-configure multicore-partitioning tag and re-write xxx_atomicList.m file
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write('%% This function defines the subsystem assignments\n')
        text_file.write('function subAssignment = BasicAudio_atomicList()\n')
        num_of_soc = env['Partitioning']['num_socs']
        num_of_core = env['Partitioning']['num_cores']
        cnt = 1
        for nums in range(num_of_soc):
            for numc in range(num_of_core):
                text_file.write('tag{' + str(cnt) + '} = {' + "'" + str(nums+1) + '_' + str(numc+1) + "'" + '};\n')
                cnt += 1
        text_file.write('subAssignment = {};\n')
        for modelname, corenum in env['Partitioning']['modules'].items():
            text_file.write('subAssignment{end+1} = [{[' + "'" + modelname + "'")
            text_file.write(' ' * (70 - len(modelname)))
            text_file.write(']}, tag{')
            tagnum = (int(corenum[0]) - 1) * num_of_core + int(corenum[2])
            text_file.write(str(tagnum))
            text_file.write('} ];')
            text_file.write('\n')

def generate_jmodule_config(target, source, env):
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write('#if !defined(BC_JModule_Config_' + env['JModule']['Name'] + '__INCLUDED_)\n')
        text_file.write('#define BC_JModule_Config_' + env['JModule']['Name'] + '__INCLUDED_\n')
        text_file.write('\n')
        for parameter in env['JModule']['Parameters'].keys():
            text_file.write('#define ' + env['JModule']['Name'].upper() + '_' + parameter.upper() +
                '        ('+str(env['JModule']['Parameters'][parameter])+')\n')
        text_file.write('\n')
        text_file.write('#endif /*!defined(BC_JModule_Config_' + env['JModule']['Name'] + '__INCLUDED_)*/\n')

def generate_bcf_config(target, source, env):
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write('#if !defined(BC_BCF_Config_' + env['Bcf']['Name'] + '__INCLUDED_)\n')
        text_file.write('#define BC_BCF_Config_' + env['Bcf']['Name'] + '__INCLUDED_\n')
        text_file.write('\n')
        for parameter in env['Bcf']['Parameters'].keys():
            param_str = str(env['Bcf']['Parameters'][parameter])
            param_str = param_str.replace('[', '{')
            param_str = param_str.replace(']', '}')
            param_str = param_str.replace('matlab.double(', '')
            param_str = param_str.replace('matlab.single(', '')
            param_str = param_str.replace(')', '')
            text_file.write('#define ' + env['Bcf']['Name'].upper() + '_' + parameter.upper() +
                '        '+param_str+'\n')
        text_file.write('\n')
        text_file.write('#endif /*!defined(BC_BCF_Config_' + env['Bcf']['Name'] + '__INCLUDED_)*/\n')

def svn_info(target, source, env):
    run_cmd = ['svn', 'info']
    print(' '.join(run_cmd))
    result = subprocess.check_output(run_cmd, stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

def pingpongstruct_postproc(target, source, env) :
    postprocess.PingPongStruct.post_process (
        model_name=env['model_name'],
        soc_id=int(env['SOC_ID'],10),
        core_id=int(env['CORE_ID'],10),
        xml_path=source[1].srcnode().path,
        in_path=source[0].srcnode().path,
        out_path = target[0].srcnode().path
        )

def xmlmap2html_report(target, source, env):
    run_cmd = []
    run_cmd.append(env['TOOLSPATH'].replace('"',''))
    run_cmd[0] += 'xmlmap2html.exe'
    run_cmd += [str(source[0]).replace('.dxe','.map.xml')]
    print (' '.join(run_cmd))
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )

def build_memory_report(target, source, env) :
    if env['arch'] in ['sharc','gul','gxp']:
        if 'filelist_path' in env:
            filelist_path=env.Dir(env.subst(env['filelist_path'])).get_abspath()
        else:
            filelist_path=None
        map_xml = str(source[0]).replace('.dxe','.map.xml')
        xsl_file = source[1].srcnode().path
        size_log = target[0].srcnode().path
        xmlparse.SizeSharcBinary.dsp_memory_usage(map_xml, size_log, xsl_file, fl_path=filelist_path)
    elif env['arch'] in ['hifi4', 'hifi5']:
        if 'filelist_path' in env:
            filelist_path=env.Dir(env.subst(env['filelist_path'])).get_abspath()
        else:
            filelist_path=None
        map_xml = str(source[0]).replace('.elf','.map')
        # xsl_file = source[1].srcnode().path
        size_log = target[0].srcnode().path
        xmlparse.SizeGNUBinary.dsp_memory_usage(map_xml, size_log,filelist_path,False,False)
    elif env['arch'] in ['c66']:
        tools_path = env['TOOLSPATH']
        exe_file = source[0].srcnode().path
        size_log = target[0].srcnode().path
        xmlparse.SizeC6678Binary.dsp_memory_usage(tools_path, exe_file, size_log)
    elif env['arch'] in ['x86', 'amd64', 'linux', 'hifi4', 'hifi5']:
        run_cmd = [env['SIZE'], source[0].srcnode().path]
        print(' '.join(run_cmd))
        if (env.Execute(' '.join(run_cmd) + ' > ' + '%s' % (target[0]))) :
            print('error opening file in build_memory_report()')
    elif env['arch'] in ['qnx_nto_aarch64']:
        run_cmd = [env['SIZE'], source[0].srcnode().path]
        if (env.Execute(' '.join(run_cmd) + ' > ' + '%s' % (target[0]))) :
            print('error opening file in build_memory_report()')
    else:
        print ('Warning: build memory report unknown architecture ' + env['arch'])

def build_memory_visualization(target, source, env) :
    if env['arch'] in ['sharc','gul','gxp']:
        if 'filelist_path' in env:
            filelist_path=env.Dir(env.subst(env['filelist_path'])).get_abspath()
        else:
            filelist_path=None
        map_xml = str(source[0]).replace('.dxe','.map.xml')
        mem_viz = target[0].srcnode().path
        xmlparse.SHARCMemViz.visualize_memory_usage(map_xml, mem_viz, 'JTest', fl_path=filelist_path)
    else:
        print ('Warning: build memory visualization unknown architecture ' + env['arch'])

def xml_reader(target, source, env) :
    read_xml('./Baf1/ParamSchema.json', './Baf1/' + JConfig[0]['JFramework'][0]['FrameworkName'] + '.xml', ['./Baf1/' + x + '.xml' for x in JConfig[0]['JFramework'][0]['ModuleNames']] + ['./Baf1/Sys.xml'], default_module=None, output_directory='./Baf1', dependency_path='./Tools/JTest/OpenJADE_code_utility') #jmodules is a list

def generate_ascii_vectors(target, source, env) :
    WINDOWS_LINE_ENDING = b'\r\n'
    UNIX_LINE_ENDING = b'\n'
    bin_files = [ source[0].srcnode().path,
             source[1].srcnode().path,
             source[2].srcnode().path,
             source[3].srcnode().path ]
    ascii_files = [ target[0].srcnode().path,
             target[1].srcnode().path,
             target[2].srcnode().path,
             target[3].srcnode().path ]
    for bin_file, ascii_file in zip(bin_files, ascii_files):
        print ('Info: generating %s from %s' % (ascii_file, bin_file))
        with open(bin_file, 'rb') as fin:
            buf = fin.read()
        if '.ojx' in bin_file:
            buf = buf.replace(WINDOWS_LINE_ENDING, UNIX_LINE_ENDING)
        sidx = 0
        step = 16
        total = len(buf)
        fout = open(ascii_file, 'w')
        while sidx < total:
            fout.write(','.join(hex(x) for x in buf[sidx:min(sidx + step, total)])+',\n')
            sidx = sidx + step
        fout.close()

def convert_control_request(target, source, env) :
    if len(source) < 5:
        return None
    src_baf_xml = source[0].srcnode().path
    src_input_file = source[1].srcnode().path
    src_audioIn = source[2].srcnode().path
    src_audioOut = source[3].srcnode().path
    src_audioRef = source[3].srcnode().path
    if len(source) >= 5:
        src_rtc_payloads_xml = source[4].srcnode().path
    else:
        src_rtc_payloads_xml = None
    if len(source) >= 6:
        src_top_tokens_xml = source[5].srcnode().path
        src_tsp_tokens_xml = src_top_tokens_xml.replace('_TOP_Tokens.xml', '_TSP_Tokens.xml')
    else:
        src_top_tokens_xml = None
    if len(source) >= 7:
        src_tie_dll = source[6].srcnode().path
    else:
        src_tie_dll = None

    if len(source) >= 8:
        src_tte_dll = source[7].srcnode().path
    else:
        src_tte_dll = None

    if len(target) < 5:
        return None
    tgt_output_file = target[0].srcnode().path
    tgt_audioIn = target[1].srcnode().path
    tgt_audioOut = target[2].srcnode().path
    tgt_audioRef = target[3].srcnode().path
    tgt_test_folder = str(target[4])

    # Create JFramework test folder
    if not os.path.isdir(tgt_test_folder) :
        os.makedirs(tgt_test_folder)

    # Copy audio files from JModule to JFramework (other processing possible)
    copyfile(src_audioIn, tgt_audioIn)   # audioIn.bin
    copyfile(src_audioOut, tgt_audioOut) # audioOut.bin
    copyfile(src_audioRef, tgt_audioRef) # audioRef.bin

    frame_size = None
    sample_rate = None
    base_time_step = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            base_time_step = app_obj.GetRatePeriod(name=jframe_name)
    else:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                elif parameter == 'sample_rate':
                    sample_rate = env['JModule']['Parameters']['sample_rate']
                elif parameter == 'base_time_step':
                    base_time_step = env['JModule']['Parameters']['base_time_step']

    filter_core = None
    if ('arch' in env) and (env['arch']) and ('platform' in env) and (env['platform']):
        if ('JFramework' in env) and (env['JFramework']) and ('CORE_ID' in env['JFramework']):
            filter_core = env['JFramework']['CORE_ID']

    # Coonvert .ojt to .ojx
    OJTSI.ConvertRequest.CompileRequest2 (
        frame_size=frame_size,
        sample_rate=sample_rate,
        base_time_step=base_time_step,
        baf_xml=src_baf_xml,
        rtc_payloads_xml=src_rtc_payloads_xml,
        top_tokens_xml=src_top_tokens_xml,
        tsp_tokens_xml=src_tsp_tokens_xml,
        tie_dll=src_tie_dll,
        tte_dll=src_tte_dll,
        input_file=src_input_file,
        output_file=tgt_output_file,
        filter_core=filter_core
        )

def convert_control_request_n_correct(target, source, env) :
    src_baf_xml = source[0].srcnode().path
    src_corrections_xml = source[1].srcnode().path
    src_input_file = source[2].srcnode().path
    src_audioIn = source[3].srcnode().path
    src_audioOut = source[4].srcnode().path
    src_audioRef = source[4].srcnode().path
    if len(source) >= 6:
        src_rtc_payloads_xml = source[5].srcnode().path
    else:
        src_rtc_payloads_xml = None
    if len(source) >= 7:
        src_top_tokens_xml = source[6].srcnode().path
        src_tsp_tokens_xml = src_top_tokens_xml.replace('_TOP_Tokens.xml', '_TSP_Tokens.xml')
    else:
        src_top_tokens_xml = None
    if len(source) >= 8:
        src_tie_dll = source[7].srcnode().path
    else:
        src_tie_dll = None

    if len(source) >= 9:
        src_tte_dll = source[8].srcnode().path
    else:
        src_tte_dll = None

    tgt_output_file = target[0].srcnode().path
    tgt_audioIn = target[1].srcnode().path
    tgt_audioOut = target[2].srcnode().path
    tgt_audioRef = target[3].srcnode().path
    tgt_test_folder = str(target[4])

    # Create JFramework test folder
    if not os.path.isdir(tgt_test_folder) :
        os.makedirs(tgt_test_folder)

    # Copy audio files from JModule to JFramework (other processing possible)
    copyfile(src_audioIn, tgt_audioIn)   # audioIn.bin
    copyfile(src_audioOut, tgt_audioOut) # audioOut.bin
    copyfile(src_audioRef, tgt_audioRef) # audioRef.bin

    frame_size = None
    sample_rate = None
    base_time_step = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            base_time_step = app_obj.GetRatePeriod(name=jframe_name)
    else:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                elif parameter == 'sample_rate':
                    sample_rate = env['JModule']['Parameters']['sample_rate']
                elif parameter == 'base_time_step':
                    base_time_step = env['JModule']['Parameters']['base_time_step']

    filter_core = None
    if ('arch' in env) and (env['arch']) and ('platform' in env) and (env['platform']):
        if ('JFramework' in env) and (env['JFramework']) and ('CORE_ID' in env['JFramework']):
            filter_core = env['JFramework']['CORE_ID']

    # Convert .ojt to .ojx
    OJTSI.ConvertRequest.CompileRequest3 (
        frame_size=frame_size,
        sample_rate=sample_rate,
        base_time_step=base_time_step,
        baf_xml=src_baf_xml,
        corrections_xml=src_corrections_xml,
        rtc_payloads_xml=src_rtc_payloads_xml,
        top_tokens_xml=src_top_tokens_xml,
        tsp_tokens_xml=src_tsp_tokens_xml,
        tie_dll=src_tie_dll,
        tte_dll=src_tte_dll,
        input_file=src_input_file,
        output_file=tgt_output_file,
        filter_core=filter_core
        )

def convert_control_response(target, source, env) :
    src_input_file = source[0].srcnode().path

    if ('optionIdx' in env) and (env['optionIdx']):
        src_input_file = src_input_file.replace('.run.log','_%d.run.log' % env['optionIdx'])

    src_baf_xml = source[1].srcnode().path
    if len(source) >= 3:
        src_rtc_payloads_xml = source[2].srcnode().path
    else:
        src_rtc_payloads_xml = None
    if len(source) >= 4:
        src_top_tokens_xml = source[3].srcnode().path
        src_tsp_tokens_xml = src_top_tokens_xml.replace('_TOP_Tokens.xml', '_TSP_Tokens.xml')
    else:
        src_top_tokens_xml = None
    if len(source) >= 5:
        src_tie_dll = source[4].srcnode().path
    else:
        src_tie_dll = None

    if len(source) >= 6:
        src_tte_dll = source[5].srcnode().path
    else:
        src_tte_dll = None

    tgt_output_file = target[0].srcnode().path

    OJTSI.ConvertResponse.DecompileResponse2 (
        baf_xml=src_baf_xml,
        rtc_payloads_xml=src_rtc_payloads_xml,
        top_tokens_xml=src_top_tokens_xml,
        tsp_tokens_xml=src_tsp_tokens_xml,
        tie_dll=src_tie_dll,
        tte_dll=src_tte_dll,
        input_file=src_input_file,
        output_file=tgt_output_file
        )

def generate_pktmon_report(target, source, env) :
    process_list = None
    probe_points = None
    callrates_list = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            callrates_list = app_obj.GetCallRates(name=jframe_name)
            process_list =  app_obj.GetProcesses(name=jframe_name)

    run_log = source[0].srcnode().path
    if ('optionIdx' in env) and (env['optionIdx']):
        run_log = run_log.replace('.run.log','_%d.run.log' % env['optionIdx'])

    capture_run_log = run_log.replace('.run.log','.capture.log')
    pktmon_csv = target[0].srcnode().path

    exec_graph_params = {
        'callrates_list': callrates_list,
        'process_list': process_list}

    OJTSI.HexagonPktMonOJT.parse_pktmon_log(exec_graph_params=exec_graph_params,
                                          capture_run_log=capture_run_log,
                                          pktmon_csv=pktmon_csv )

def generate_perfmon_report(target, source, env) :
    process_list = None
    probe_points = None
    callrates_list = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            callrates_list = app_obj.GetCallRates(name=jframe_name)
            process_list =  app_obj.GetProcesses(name=jframe_name)

    if ('Option' in env) and (env['Option']) and ('probe_points' in env['Option']):
        probe_points = env['Option']['probe_points']

    run_log = source[0].srcnode().path
    if ('optionIdx' in env) and (env['optionIdx']):
        run_log = run_log.replace('.run.log','_%d.run.log' % env['optionIdx'])

    capture_run_log = run_log.replace('.run.log','.capture.log')
    perfmon_csv = target[0].srcnode().path

    exec_graph_params = {
        'callrates_list': callrates_list,
        'process_list': process_list,
        'probe_points': probe_points}

    OJTSI.GULPerfMonOJT.parse_perfmon_log(exec_graph_params=exec_graph_params,
                                          capture_run_log=capture_run_log,
                                          perfmon_mode=env['perfmon_mode'],
                                          perfmon_csv=perfmon_csv )

def generate_ccnt64mon_report(target, source, env) :
    frame_size = None
    sample_rate = None
    clock_rate = None
    skip_frames = None
    process_list = None
    probe_points = None
    callrates_list = None
    accel_process_list = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            callrates_list = app_obj.GetCallRates(name=jframe_name)
            process_list =  app_obj.GetProcesses(name=jframe_name)

    if callrates_list == None:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                if parameter == 'sample_rate':
                    sample_rate = env['JModule']['Parameters']['sample_rate']
                if parameter == 'clock_rate':
                    clock_rate = env['JModule']['Parameters']['clock_rate']

        if ('Option' in env) and (env['Option']) and ('process_list' in env['Option']):
            process_list = env['Option']['process_list']

    if ('Option' in env) and (env['Option']) and ('probe_points' in env['Option']):
        probe_points = env['Option']['probe_points']

    if ('Option' in env) and (env['Option']) and ('skip_frames' in env['Option']):
        skip_frames = env['Option']['skip_frames']

    if ('Option' in env) and (env['Option']) and ('AccelProcesses' in env['Option']):
        accel_process_list = env['Option']['AccelProcesses'][env['arch']]

    run_log = source[0].srcnode().path
    mips_log = target[0].srcnode().path

    if ('optionIdx' in env) and (env['optionIdx']):
        run_log = run_log.replace('.run.log','_%d.run.log' % env['optionIdx'])

    if env['arch'] in ['sharc','gul','gxp']:
        baf_logger = []
        heap_log_buffer = []
        baf_logger.append(run_log.replace('.run.log','.baf_logger.bin'))
        heap_log_buffer.append(run_log.replace('.run.log','.heap_log_buffer.bin'))
        execgraph_html = run_log.replace('.run.log','.exec_graph.html')
        times_log = run_log.replace('.run.log','.times_log.csv')

        exec_graph_params = {'frame_size':frame_size,
            'sample_rate':sample_rate,
            'clock_rate':clock_rate,
            'baf_logger':baf_logger,
            'heap_log_buffer':heap_log_buffer,
            'execgraph_html':execgraph_html,
            'times_log':times_log,
            'mips_log': mips_log,
            'skip_frames': skip_frames,
            'callrates_list': callrates_list,
            'process_list': process_list,
            'probe_points': probe_points,
            'accel_process_list': accel_process_list}

        OJTSI.Ccnt64MonOJT.plot_exec_graph(exec_graph_params)

def generate_stackmon_report(target, source, env) :
    process_list = None
    probe_points = None
    callrates_list = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            callrates_list = app_obj.GetCallRates(name=jframe_name)
            process_list =  app_obj.GetProcesses(name=jframe_name)

    if ('Option' in env) and (env['Option']) and ('probe_points' in env['Option']):
        probe_points = env['Option']['probe_points']

    run_log = source[0].srcnode().path
    if ('optionIdx' in env) and (env['optionIdx']):
        run_log = run_log.replace('.run.log','_%d.run.log' % env['optionIdx'])

    capture_run_log = run_log.replace('.run.log','.capture.log')
    stackmon_csv = target[0].srcnode().path

    exec_graph_params = {
        'callrates_list': callrates_list,
        'process_list': process_list,
        'probe_points': probe_points}

    OJTSI.GULStackMonOJT.parse_stackmon_log(exec_graph_params=exec_graph_params,
                                          capture_run_log=capture_run_log,
                                          stackmon_csv=stackmon_csv )

def calculate_mips_report(target, source, env) :
    frame_size = None
    sample_rate = None
    clock_rate = None
    scaling_factors = None
    skip_frames = None
    process_list = None
    probe_points = None
    callrates_list = None
    accel_process_list = None
    jautogenProbeDict = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            callrates_list = app_obj.GetCallRates(name=jframe_name)
            process_list =  app_obj.GetProcesses(name=jframe_name)
            if 'JAutogenProbe' in env['JFramework']:
                jautogenProbeJson = env['JAutogenProbe'][env['JFramework']['Name']]
                with open(jautogenProbeJson) as fp:
                    jautogenProbeDict = json.load(fp)

    if callrates_list == None:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                if parameter == 'sample_rate':
                    sample_rate = env['JModule']['Parameters']['sample_rate']
                if parameter == 'clock_rate':
                    clock_rate = env['JModule']['Parameters']['clock_rate']

        if ('Option' in env) and (env['Option']) and ('scaling_factors' in env['Option']):
            scaling_factors = env['Option']['scaling_factors']

        if ('Option' in env) and (env['Option']) and ('process_list' in env['Option']):
            process_list = env['Option']['process_list']

    if ('Option' in env) and (env['Option']) and ('probe_points' in env['Option']):
        probe_points = env['Option']['probe_points']
    if (jautogenProbeDict != None):
        if probe_points == None:
            probe_points = {}
            probe_points['processes'] = {}
        if 'processes' in jautogenProbeDict:
            for k,v in jautogenProbeDict['processes'].items():
                probe_points['processes'][k] = v

    if ('Option' in env) and (env['Option']) and ('skip_frames' in env['Option']):
        skip_frames = env['Option']['skip_frames']

    if ('Option' in env) and (env['Option']) and ('AccelProcesses' in env['Option']):
        accel_process_list = env['Option']['AccelProcesses'][env['arch']]

    run_log = source[0].srcnode().path
    mips_log = target[0].srcnode().path

    if ('optionIdx' in env) and (env['optionIdx']):
        run_log = run_log.replace('.run.log','_%d.run.log' % env['optionIdx'])

    if env['arch'] in ['sharc','gul','gxp', 'hifi4', 'hifi5','hexagon','hexagon_v66','hexagon_v73','qnx_nto_aarch64']:
        baf_logger = []
        heap_log_buffer = []
        baf_logger.append(run_log.replace('.run.log','.baf_logger.bin'))
        heap_log_buffer.append(run_log.replace('.run.log','.heap_log_buffer.bin'))
        execgraph_html = run_log.replace('.run.log','.exec_graph.html')
        times_log = run_log.replace('.run.log','.times_log.csv')

        exec_graph_params = {'frame_size':frame_size,
            'sample_rate':sample_rate,
            'clock_rate':clock_rate,
            'baf_logger':baf_logger,
            'heap_log_buffer':heap_log_buffer,
            'execgraph_html':execgraph_html,
            'times_log':times_log,
            'mips_log': mips_log,
            'arch': env['arch'],
            'skip_frames': skip_frames,
            'scaling_factors': scaling_factors,
            'callrates_list': callrates_list,
            'process_list': process_list,
            'probe_points': probe_points,
            'accel_process_list': accel_process_list}

        OJTSI.PlotExecGraphOJT.plot_exec_graph(exec_graph_params)
    else:
        OJTSI.MIPSReport.GenMIPSReport (
            frame_size=frame_size,
            sample_rate=sample_rate,
            clock_rate=clock_rate,
            callrates_list=callrates_list,
            scaling_factors=scaling_factors,
            process_list=process_list,
            skip_frames=skip_frames,
            probe_points=probe_points,
            accel_process_list=accel_process_list,
            input_file=run_log,
            output_file=mips_log
        )

def audio_plots_builder(target, source, env) :
    in_frame_size = None
    out_frame_size = None
    in_channel_count = None
    out_channel_count = None
    base_rate_period = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            in_frame_size = app_obj.GetInFrameSize(name=jframe_name)
            out_frame_size = app_obj.GetOutFrameSize(name=jframe_name)
            in_channel_count = app_obj.GetNumIn(name=jframe_name)
            out_channel_count = app_obj.GetNumOut(name=jframe_name)
            base_rate_period = app_obj.GetRatePeriod(name=jframe_name)
    else:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                    in_frame_size = frame_size
                    out_frame_size = frame_size
                if parameter == 'in_channel_count':
                    in_channel_count = env['JModule']['Parameters']['in_channel_count']
                if parameter == 'out_channel_count':
                    out_channel_count = env['JModule']['Parameters']['out_channel_count']

    if ((out_frame_size == None) or
        (out_channel_count == None)):
        print('Error: audio_plots_builder one/more required parameters (out_frame_size, out_channel_count) undefined')
        return

    if ((in_frame_size == None) or
        (in_channel_count == None)):
        print('Warning: audio_plots_builder one/more parameters (in_frame_size, in_channel_count) undefined')

    OJTSI.PlotAudioOJT.plot_audio (
        audio_in=source[1].srcnode().path,
        audio_out=source[3].srcnode().path,
        audio_ref=source[2].srcnode().path,
        in_frame_size=in_frame_size,
        out_frame_size=out_frame_size,
        in_channel_count=in_channel_count,
        out_channel_count=out_channel_count,
        base_rate_period = base_rate_period,
        output_html_file=target[0].srcnode().path,
        option = env['Option'] if ('Option' in env) and (env['Option']) else None,
        debug=False
        )

def generate_jcontrol(target, source, env):
    jautogenCode = JAutogenCode()
    if hasattr(env['Bcf']['Generator'], 'control' ):
        bcf_data = env['Bcf']['Generator'].control
    else:
        bcf_data = env['Bcf']['Generator'].data
    jautogenCode.GenerateFeatureLibinCpp(bcf_data, env['Bcf']['Generator'].type, gen_path=env['Bcf']['GenPath'], rtc=str(source[0]), top=str(source[1]))

def apply_jautogenProbe(target, source, env):
    c_code_path = env['CodeGenPath']
    model_cfile_name = env['ModelName'] # Model_1_1 for example
    callrate_file = env['SyncApi']
    platform = env['Platform'] # JAutogenProbe.DSPHEXAGON, JAutogenProbe.DSPGUL

    if ('JAutogenProbe' in env['JFramework']) and env['JFramework']['JAutogenProbe'] :
        profile_cfg = env['JFramework']['JAutogenProbe']
        ja = JAutogenProbe(platform, c_code_path)
        ja.PrepareForProfiling(model_cfile_name, callrate_file)
        ja.AddProfilingInitNStep(True)
        if 'Profile' in profile_cfg and model_cfile_name in profile_cfg['Profile']:
            for k, v in profile_cfg['Profile'][model_cfile_name].items():
                    ja.AddProfilingFeature(k, k[:-2], v)
        else:
            print('===> ' + model_cfile_name + ' is not in JAutogenProbeConfig.json, ignore function profiling for it.')
        tDict = ja.EvokeGoldenProfiling(JAutogenProbe.PROFILE_JPROBE, c_code_path)
        jtestDict = {'enable': False, 'processes': {}}
        if bool(tDict):
            for k, v in tDict.items():
                jtestDict['processes'][k] = {'thread_level': int(v[0]), 'process_id': v[1]}
            with open(str(target[0]), 'w') as fout:
                json.dump(jtestDict, fout, indent=4)

def jautogenBafLite(target, source, env):
    c_code_path = env['CodeGenPath']
    model_cfile_name = env['ModelName'] # Model_1_1 for example
    callrate_file = env['SyncApi']

    ignore_crosscore_buffers = False
    if ('CrossCore' in env['JFramework']):
        if env['JFramework']['CrossCore'] == True:
            ignore_crosscore_buffers = False
        else:
            ignore_crosscore_buffers = True

    if 'Option' in env['JFramework'] and 'Lite' in env['JFramework']['Option'] and env['JFramework']['Option']['Lite']:
        bl = JAutogenBafLite(callrate_file, c_code_path, model_cfile_name, ignore_crosscore_buffers)


def calculate_pass_fail(target, source, env) :
    frame_size = None
    out_channel_count = None

    if ('JFramework' in env) and (env['JFramework']):
        if ('Generator' in env['JFramework']) and (env['JFramework']['Generator']):
            app_obj = env['JFramework']['Generator']
            jframe_name = env['JFramework']['Name']
            frame_size = app_obj.GetOutFrameSize(name=jframe_name)
            out_channel_count = app_obj.GetNumOut(name=jframe_name)
    else:
        if ('JModule' in env) and ('Parameters' in env['JModule']):
            for parameter in env['JModule']['Parameters'].keys():
                if parameter == 'frame_size':
                    frame_size = env['JModule']['Parameters']['frame_size']
                if parameter == 'out_channel_count':
                    out_channel_count = env['JModule']['Parameters']['out_channel_count']

    if ((frame_size == None) or
        (out_channel_count == None)):
        print('Warning: calculate_pass_fail one/more required parameters (frame_size, out_channel_count) undefined')
        return

    OJTSI.PassFailReportV2.GenPassFailReportV2 (
        test_name=env['Test']['Name'],
        test_case=env['Test']['Case'],
        test_param=env['Test']['Param'],
        tolerance=env['Test']['Tolerance'],
        audio_out=source[3].srcnode().path,
        audio_ref=source[2].srcnode().path,
        frame_size=frame_size,
        channel_count=out_channel_count,
        output_file=target[0].srcnode().path,
        option = env['Option'] if ('Option' in env) and (env['Option']) else None
        )

def logexec_request(target, source, env) :
    stop_time = None

    with open(source[0].srcnode().path, 'r') as read_obj:
        for line in read_obj:
            line = line.strip()
            if (len(line) != 0) and (not line.startswith('#')):
                items = [x.strip() for x in line.split(',')]
                if items[0].startswith('['):
                    if items[1] == 'registerBAF':
                        baf_items = [x.strip() for x in items[0].lstrip('[').rstrip(']').split()]
                        baf_id = baf_items[0]
                        baf_payload = baf_items[1:]
                        if baf_id == 'SYS_NUM_FRAMES':
                            stop_time = baf_payload[0]


    with open(target[0].srcnode().path, 'w') as write_obj, open(source[0].srcnode().path, 'r') as read_obj:
        for line in read_obj:
            write_obj.write(line)

        if stop_time != None:
            write_obj.write('# Disable all logs at the start\n')
            write_obj.write('[SYS_LOG_DISABLE LOG_TYPE_PRINT], registerBAF, 0\n')
            write_obj.write('[SYS_LOG_DISABLE LOG_TYPE_CAPTURE], registerBAF, 0\n')
            write_obj.write('[SYS_LOG_DISABLE LOG_TYPE_EXEC], registerBAF, 0\n')
            write_obj.write('\n')
            write_obj.write('# Put exec log in circular mode\n')
            write_obj.write('[SYS_LOG_CIRCULAR LOG_TYPE_EXEC], registerBAF, 0\n')
            write_obj.write('\n')
            write_obj.write('# Enable exec log\n')
            write_obj.write('[SYS_LOG_ENABLE LOG_TYPE_EXEC], registerBAF, 0\n')
            write_obj.write('\n')
            write_obj.write('# Enable capture log\n')
            write_obj.write('[SYS_LOG_ENABLE LOG_TYPE_CAPTURE], registerBAF, 0\n')
            write_obj.write('\n')
            if env['arch'] not in ['sharc','gul','gxp','hexagon','hexagon_v66','hexagon_v73','qnx_nto_aarch64']:
                write_obj.write('# Dump exec log at the end\n')
                write_obj.write('[SYS_LOG_DUMP LOG_TYPE_EXEC], registerBAF, ' + stop_time + '\n')
                write_obj.write('\n')

def xlateTestNametoIndex(test_name, jconfig_list):
    test_idx = 1
    test_found = False
    for cfg in jconfig_list:
        if ('Name' in cfg) and (cfg['Name'] != None):
            if test_name == cfg['Name']:
                test_found = True
                break
        test_idx += 1
    return {'found':test_found, 'idx':test_idx}

def generate_cces_project(target, source, env):
    if env['arch'] in ['sharc','gul','gxp']:
        shutil.rmtree(env['outPath'] + '\\JModule_core'+env['core'])
        shutil.rmtree(env['outPath'] + '\\Model_core'+env['core'])

        shutil.copytree(env['path'] + '\\JConfig\\cces_gen\\JModule_core1',
                        env['outPath'] + '\\JModule_core'+env['core'])
        shutil.copytree(env['path'] + '\\JConfig\\cces_gen\\Model_core1',
                        env['outPath'] + '\\Model_core'+env['core'])

        replaceCoreNum(env['path'] + '\\JConfig\\cces_gen\\JModule_core1\\.project',
                        env['core'],
                        env['outPath'] + '\\JModule_core'+env['core']+'\\.project')

        replaceCoreNum(env['path'] + '\\JConfig\\cces_gen\\Model_core1\\.project',
                        env['core'],
                        env['outPath'] + '\\Model_core'+env['core']+'\\.project')

        path_prefix = target[0].abspath.replace(target[0].srcnode().path, '')

        changeSrcPath(env['outPath'] + '\\JModule_core'+env['core']+'\\.project',
                        (path_prefix[2:]+env['srcPath']).replace('\\','/'),
                        env['outPath'] + '\\JModule_core'+env['core']+'\\.project')


        changeSrcPath(env['outPath'] + '\\Model_core'+env['core']+'\\.project',
                        (path_prefix[2:]+env['codeGenFolder']).replace('\\','/'),
                        env['outPath'] + '\\Model_core'+env['core']+'\\.project')

        replaceCoreNum(env['path'] + '\\JConfig\\cces_gen\\JModule_core1\\.cproject',
                        env['core'],
                        env['outPath'] + '\\JModule_core'+env['core']+'\\.cproject')

        replaceCoreNum(env['path'] + '\\JConfig\\cces_gen\\Model_core1\\.cproject',
                        env['core'],
                        env['outPath'] + '\\Model_core'+env['core']+'\\.cproject')

        changeCProjPaths(env['outPath'] + '\\JModule_core'+env['core']+'\\.cproject',
                        '\"'+path_prefix+env['srcPath']+'\"',
                        env['outPath'] + '\\JModule_core'+env['core']+'\\.cproject',
                        app_root)

        changeCProjPaths(env['outPath'] + '\\Model_core'+env['core']+'\\.cproject',
                        '\"'+path_prefix+env['codeGenFolder']+'\"',
                        env['outPath'] + '\\Model_core'+env['core']+'\\.cproject',
                        app_root)


def codeGenFiles(codegen_folder_prefix, code_gen_excludes, app_root, debug=False):
    code_gen_files = []
    code_gen_incs = []

    buildInfoOJTJSON = str(app_root.File(f'{codegen_folder_prefix}/buildInfoOJT.json'))
    if os.path.isfile(buildInfoOJTJSON):
        with open(buildInfoOJTJSON, 'r') as fin:
            buildInfoStr = fin.read()

        buildInfoOJT = json.loads(buildInfoStr.replace('\\','/'))

        app_root_scons = f'#/{app_root}'
        app_root_full = app_root.Dir(app_root_scons).get_abspath().replace('\\','/')

        for inc_path in buildInfoOJT['IncludePaths']:
            if buildInfoOJT['MatlabRoot'] in inc_path:
                continue
            elif app_root_full in inc_path:
                code_gen_incs.append(inc_path.replace(app_root_full, app_root_scons))

        for src_file in buildInfoOJT['FullFileList']:
            if app_root_full in src_file:
                if src_file.endswith('.h'):
                    continue
                exclude = False
                for code_gen_exclude in code_gen_excludes:
                    if code_gen_exclude in src_file:
                        exclude = True
                        break
                if exclude:
                    continue
                src_file_scons = src_file.replace(app_root_full, app_root_scons)
                code_gen_files.append(app_root.File(src_file_scons))
    else:
        code_gen_files.extend(app_root.glob(codegen_folder_prefix+'/*.c', exclude=code_gen_excludes))
        code_gen_files.extend(app_root.glob(codegen_folder_prefix+'/*.S', exclude=code_gen_excludes))
        code_gen_incs.append(codegen_folder_prefix)
    if debug:
        print('Info: code_gen_files')
        for x in code_gen_files:
            print(str(x))
        print('Info: code_gen_incs')
        for x in code_gen_incs:
            print(str(x))
    return code_gen_files, code_gen_incs

def get_filter():
    return {
        'arch': GetOption('filter_arch'),
        'platform': GetOption('filter_platform'),
        'test': GetOption('filter_test')
    }

def filter_test(filter_arg, jconfig):
    if 'JFramework' in jconfig:
        # all keys apply for this case
        arch = True if filter_arg['arch'] is None else filter_arg['arch'] == (jconfig['Option']['arch'] if not isinstance(jconfig['Option']['arch'], dict) else 'multi')
        platform = True if filter_arg['platform'] is None else filter_arg['platform'] == jconfig['Option']['platform']
        test = True if filter_arg['test'] is None else ('Name' in jconfig and jconfig['Name']) and jconfig['Name'].startswith(filter_arg['test'])
        return not (arch and platform and test)
    else:
        # only arch filter applies for this case
        return not (True if filter_arg['arch'] is None else filter_arg['arch'] == jconfig['Option']['arch'])

def get_env_options(cfg, glb):
    optionModel = GetOption('model') == 'yes'
    optionCfg = GetOption('cfg') == 'yes'
    optionArch= GetOption('arch')
    optionPlatform= GetOption('platform')
    optionRunner = GetOption('runner')
    optionHostname = GetOption('hostname')
    optionUser = GetOption('user')
    optionPwd = GetOption('pwd')
    optionSim = GetOption('sim')
    optionCart = GetOption('cart')
    optionModule = GetOption('module')
    optionDynamic = GetOption('dynamic')
    optionNamespace = GetOption('namespace')

    optionArch      = optionArch      if optionArch      else (cfg['Option']['arch']      if 'arch'      in cfg['Option'] else 'x86')
    optionPlatform  = optionPlatform  if optionPlatform  else (cfg['Option']['platform']  if 'platform'  in cfg['Option'] else 'SIL')
    optionRunner    = optionRunner    if optionRunner    else (cfg['Option']['runner']    if 'runner'    in cfg['Option'] else 'sim')
    optionHostname  = optionHostname  if optionHostname  else (cfg['Option']['hostname']  if 'hostname'  in cfg['Option'] else None)
    optionUser      = optionUser      if optionUser      else (cfg['Option']['user']      if 'user'      in cfg['Option'] else None)
    optionPwd       = optionPwd       if optionPwd       else (cfg['Option']['pwd']       if 'pwd'       in cfg['Option'] else None)
    optionCart      = optionCart      if optionCart      else (cfg['Option']['cart']      if 'cart'      in cfg['Option'] else 'baf')
    optionModule    = optionModule    if optionModule    else (cfg['Option']['module']    if 'module'    in cfg['Option'] else None)
    optionDynamic   = optionDynamic   if optionDynamic   else (cfg['Option']['dynamic']   if 'dynamic'   in cfg['Option'] else 'yes')
    optionNamespace = optionNamespace if optionNamespace else (cfg['Option']['namespace'] if 'namespace' in cfg['Option'] else None)

# Read TOOLPATH from system environment variable
    toolpath_from_ev = os.getenv('OJ_TOOLSPATH')

# option cross check and defaults
    if optionPlatform == 'MIL':
        optionLib = False
        optionApp = False
        optionMil = True
    else:
        optionLib = GetOption('lib') == 'yes'
        optionApp = GetOption('app') == 'yes'
        optionMil = False
    if not optionCfg:
        optionModel = False
        optionApp = False
        optionLib = False
    if optionPlatform == 'DLL':
        optionApp = False

    if isinstance(cfg['Option']['arch'], dict):
        # Model code generation: Do not override from command line. Pretend it is x86
        if isinstance(optionArch, dict) and (optionArch is cfg['Option']['arch']):
            optionArch = 'x86'
            optionPlatform = None
        else:
        # Baf library generation: the architecture to build is an override from command line. Pretend it is SIL/PIL platform with sim runner
            if optionArch == 'x86':
                optionPlatform = 'SIL'
                optionRunner = 'sim'
            else:
                optionPlatform = 'PIL'
                optionRunner = 'sim'

    if not ('Platform' in cfg) and optionCart == 'baf':
        optionPlatform = None

    if optionPlatform in ['PIL','HIL','RTA']:
        if optionArch == 'x86':
            print( 'PIL/HIL/RTA meaningless for x86' )
            exit()
    if optionPlatform in ['HIL','MHIL','RTA','MRTA', 'SC594RTA', 'SC594HIL', 'SC598RTA', 'SC598HIL', 'SOMRTA', 'SOMHIL', 'DRTA', 'DHIL']:
        if optionSim == 'yes':
            if not(optionRunner in ['ezkit', 'preq']):
                print( 'HIL/RTA simulation requires --runner=ezkit or --runner=preq' )
                exit()
    elif optionPlatform in ['MDRTA', 'MDHIL']:
        if not (optionRunner in ['melville']):
            print( 'MDHIL/MDRTA for melville duacore requires --runner=melville' )
            exit()
    elif optionPlatform in ['SIL']:
        if not(optionArch in ['x86', 'amd64', 'linux']):
            optionPlatform = 'PIL'
            print( '--platform=SIL invalid for --arch='+optionArch+'. Switched to PIL' )
    elif optionPlatform in ['PIL', 'DPIL', 'MPIL', 'SC594PIL', 'SC598PIL', 'SOMPIL']:
        if optionRunner == 'none':
            print( 'PIL requires --runner=ezkit or --runner=sim' )
            exit()
    elif optionPlatform in ['RT685PIL']:
        if not(optionArch == 'hifi4'):
            print('RT685PIL only available for hifi4')
            exit()
    elif optionPlatform in ['SIMULATORPIL']:
        if not(optionArch == 'hifi4'):
            print('SIMULATORPIL only available for hifi4')
            exit()
    elif optionPlatform in ['QCC7226PIL']:
        if not(optionArch == 'hifi4'):
            print('QCC7226PIL only available for hifi4')
            exit()
    elif optionPlatform in ['AB1585PIL']:
        if not(optionArch == 'hifi5'):
            print('AB1585PIL only available for hifi5')
            exit()
    elif optionPlatform in ['DLL']:
        if not(optionArch in ['x86', 'x64', 'amd64' ] or (optionArch.find('android') != -1) ):
            print('DLL only available for --arch x86, x64, amd64')
            exit()
    elif optionPlatform in ['PIL_shared_library']:
        if not(optionArch == 'qnx_nto_aarch64'):
            print('PIL_shared_library only available for qnx_nto_aarch64')
            exit()
    else :
        if optionArch in ['x86', 'amd64', 'linux', 'x64']:
            optionPlatform = 'SIL'
        elif optionArch in ['gul', 'gxp', 'sharc', 'c66', 'imx8']:
            optionPlatform = 'PIL'
        else:
            print( 'Invalid --platform option. Type scons --help for details' )
            exit()

    if optionArch in ['qnx_nto_aarch64']:
        if not(optionPlatform in ['PIL', 'PIL_shared_library']):
            print( 'Invalid platform: ' + optionPlatform + ' for arch qnx_nto_aarch64. Must specify PIL or PIL_shared_library.' )
            exit()

    tools_path_override = None
    arch_config_override = None
    # Use cases such as HiFi where platforms share an architecture but different tool paths or configurations
    if(optionArch+'_tools' in glb) or ((optionPlatform[0:-3].lower()+'_'+optionArch+'_tools') in glb):
        if(optionArch+'_tools' in glb):
            tools = ['mingw','zip', glb[optionArch+'_tools']]

        elif((optionPlatform[0:-3].lower()+'_'+optionArch+'_tools') in glb):
            tools = ['mingw','zip', glb[optionPlatform[0:-3].lower()+'_'+optionArch+'_tools']]
        tools_path = GetOption('tools')
        matlab_path = GetOption('matlab')
        options = GetOption('options')
        config = options.split(' ')
        build = GetOption('build')
        if(len(tools_path) > 0) :
            tools_path_override = tools_path
            if(len(options) > 0) :
                arch_config_override = config
                env = Environment(BUILD=build, TOOLSPATH=tools_path, CONFIG=config, tools=tools )
            else:
                env = Environment(BUILD=build, TOOLSPATH=tools_path, tools=tools )
        elif toolpath_from_ev != None:
            toolpath_from_ev = toolpath_from_ev + '\\' if toolpath_from_ev[-1] != '\\' else toolpath_from_ev
            if optionArch in ['gul', 'gxp', 'sharc']:
                CCES_COMPILER = os.path.join(toolpath_from_ev, 'cc21k.exe')
                CCES_SIMULATOR = os.path.join(toolpath_from_ev, 'CCES_runner.exe')
                if (not os.path.exists(CCES_COMPILER)) or (not os.path.exists(CCES_SIMULATOR)):
                    print('######################################################################')
                    print('# The Path of CCES specified in Environment Variable is incorrect.')
                    print('# The compiler or simulartor cannot be found.')
                    print('######################################################################')
                    sys.exit()
                else:
                    tools_path_override = toolpath_from_ev
                    if(len(options) > 0):
                        arch_config_override = config
                        env = Environment(BUILD=build, TOOLSPATH=toolpath_from_ev, CONFIG=config, tools=tools )
                    else:
                        env = Environment(BUILD=build, TOOLSPATH=toolpath_from_ev, tools=tools )
            else:
                print(' No support to the tools configured in environmental variable.')
        else:
            if(len(options) > 0) :
                arch_config_override = config
                env = Environment(BUILD=build, CONFIG=config, tools=tools )
            else :
                env = Environment(BUILD=build, tools=tools )
        if(len(matlab_path) > 0) :
            env['MATLAB'] = '\"' + matlab_path + '/bin/matlab.exe\"'
        env.SetOption('warn', 'no-visual-c-missing')
        env.SetOption('warn', 'no-duplicate-environment')
    else:
        print( 'Error: No tools for processor architecture: ' + optionArch)
        exit()
    option_dict = {
        'optionArch':optionArch,
        'optionPlatform':optionPlatform,
        'optionRunner':optionRunner,
        'optionHostname':optionHostname,
        'optionUser':optionUser,
        'optionPwd':optionPwd,
        'optionSim':optionSim,
        'optionModel':optionModel,
        'optionLib':optionLib,
        'optionApp':optionApp,
        'optionMil':optionMil,
        'optionCart':optionCart,
        'optionModule':optionModule,
        'optionDynamic':optionDynamic,
        'optionNamespace':optionNamespace,
        }

    return env, option_dict, tools_path_override, arch_config_override

def add_builders(env, optionPGO):
        # Create Svn info builder
    env['BUILDERS']['SvnInfo'] = env.Builder(action = svn_info, suffix='.log')

    # Create LibMerge builder
    env['BUILDERS']['LibMerge'] = env.Builder(generator = lib_merge, suffix='$LIBSUFFIX', src_suffix='$LIBSUFFIX')
    env['BUILDERS']['LibMergeAlt'] = env.Builder(generator = lib_merge_alt, suffix='$LIBSUFFIX', src_suffix='$LIBSUFFIX')
    env['BUILDERS']['LibLstMerge'] = env.Builder(action = lib_lst_merge,
                                                 suffix=env['LIBSUFFIX'].replace('.','_')+'.lst',
                                                 src_suffix='$LIBSUFFIX')

    # Create Simulation builder
    env['BUILDERS']['RunTarget'] = env.Builder(action = run_target, suffix='.run.log')

    env['BUILDERS']['RunC66PilEVMTarget'] = env.Builder(action = run_c66_pil_evm_target, suffix='.run.log')

    env['BUILDERS']['RunHifi4RT685PilEVBTarget'] = env.Builder(action = run_hifi4_rt685pil_evb_target, suffix='.run.log')
    env['BUILDERS']['RunHifi4RT685PilSimTarget'] = env.Builder(action = run_hifi4_rt685pil_sim_target, suffix='.run.log')

    env['BUILDERS']['RunHifi4SimulatorPilSimTarget'] = env.Builder(action = run_hifi4_simulatorpil_sim_target, suffix='.run.log')

    env['BUILDERS']['RunHifi5AB1585PilEVBTarget'] = env.Builder(action = run_hifi5_ab1585pil_evb_target, suffix='.run.log')
    # env['BUILDERS']['RunHifi5AB1585PilSimTarget'] = env.Builder(action = run_hifi5_ab1585pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunHifi4QCC7226PilSimTarget'] = env.Builder(action = run_hifi4_rt685pil_sim_target, suffix='.run.log')

    env['BUILDERS']['RunQnxNtoAarch64PilSimTarget'] = env.Builder(action = run_qnx_nto_aarch64_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunHexagonPilSimTarget'] = env.Builder(action = run_hexagon_pil_sim_target, suffix='.run.log')

    env['BUILDERS']['RunGulPilSimTarget'] = env.Builder(action = run_gul_pil_sim_target, suffix='.run.log')
    if optionPGO == 'genpgo':
        env['BUILDERS']['RunGulPilEzkitTarget'] = env.Builder(action = run_gul_pil_ezkit_pgo_target, suffix='.run.log')
    else:
        env['BUILDERS']['RunGulPilEzkitTarget'] = env.Builder(action = run_gul_pil_ezkit_target, suffix='.run.log')

    env['BUILDERS']['RunGulHilRtaTarget'] = env.Builder(action = run_gul_hil_rta_target, suffix='.run.log')

    env['BUILDERS']['RunGxpPilSimTarget'] = env.Builder(action = run_gxp_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunGxpPilEzkitTarget'] = env.Builder(action = run_gxp_pil_ezkit_target, suffix='.run.log')
    env['BUILDERS']['RunGxpHilRtaTarget'] = env.Builder(action = run_gxp_hil_rta_target, suffix='.run.log')
    env['BUILDERS']['RunGxpDPilEzkitTarget'] = env.Builder(action = run_gxp_pil_ezkit_dual_target, suffix='.run.log')
    env['BUILDERS']['RunGxpDHilDRtaTarget'] = env.Builder(action = run_gxp_hil_rta_dual_target, suffix='.run.log')

    env['BUILDERS']['RunSc594PilSimTarget'] = env.Builder(action = run_sc594_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunSc594PilEzkitTarget'] = env.Builder(action = run_sc594_pil_ezkit_target, suffix='.run.log')
    env['BUILDERS']['RunSc594HilRtaTarget'] = env.Builder(action = run_sc594_hil_rta_target, suffix='.run.log')

    env['BUILDERS']['RunSc598PilSimTarget'] = env.Builder(action = run_sc598_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunSc598PilEzkitTarget'] = env.Builder(action = run_sc598_pil_ezkit_target, suffix='.run.log')
    env['BUILDERS']['RunSc598HilRtaTarget'] = env.Builder(action = run_sc598_hil_rta_target, suffix='.run.log')

    env['BUILDERS']['RunSharcPilSimTarget'] = env.Builder(action = run_sharc_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunSharcPilEzkitTarget'] = env.Builder(action = run_sharc_pil_ezkit_target, suffix='.run.log')
    env['BUILDERS']['RunSharcHilRtaTarget'] = env.Builder(action = run_sharc_hil_rta_target, suffix='.run.log')

    env['BUILDERS']['RunMelvilleHilRtaSingleTarget'] = env.Builder(action = run_melville_hil_rta_single_target, suffix='.run.log')
    env['BUILDERS']['RunMelvilleHilRtaDualTarget'] = env.Builder(action = run_melville_hil_rta_dual_target, suffix='.run.log')
    env['BUILDERS']['RunMelvillePilSimTarget'] = env.Builder(action = run_melville_pil_sim_target, suffix='.run.log')
    env['BUILDERS']['RunMelvillePilEzkitTarget'] = env.Builder(action = run_melville_pil_ezkit_target, suffix='.run.log')
    env['BUILDERS']['RunAmd64PilPreqTarget'] = env.Builder(action = run_amd64_pil_preq_target, suffix = '.run.log')

    # Create Memory Report builder
    env['BUILDERS']['XmlMap2HtmlBuilder'] = env.Builder(action = xmlmap2html_report, suffix='.map.html')
    env['BUILDERS']['MemoryReportBuilder'] = env.Builder(action = build_memory_report, suffix='.size.log')
    env['BUILDERS']['MemoryVizBuilder'] = env.Builder(action = build_memory_visualization, suffix='.memviz.html')

    # PostProcessing builders
    env['BUILDERS']['PingPongStructPostProc'] = env.Builder(action = pingpongstruct_postproc, suffix='.c')

    # Create XML Reader
    env['BUILDERS']['XmlReader'] = env.Builder(action = xml_reader, suffix='.c')

    # Create ConvertRequest builder
    env['BUILDERS']['ConvertControlRequest'] = env.Builder(action = convert_control_request)
    env['BUILDERS']['ConvertControlRequestNCorrect'] = env.Builder(action = convert_control_request_n_correct)
    env['BUILDERS']['LogExecRequest'] = env.Builder(action = logexec_request)
    env['BUILDERS']['ConvertControlResponse'] = env.Builder(action = convert_control_response, suffix='.response.log')
    env['BUILDERS']['GenerateAsciiVectors'] = env.Builder(action = generate_ascii_vectors)

    # Create MIPS Report builder
    env['BUILDERS']['CalculateMIPSReport'] = env.Builder(action = calculate_mips_report, suffix='.mips.csv')
    env['BUILDERS']['GenerateCcnt64MonReport'] = env.Builder(action = generate_ccnt64mon_report, suffix='.ccnt64.csv')

    # Create PerfMon Report builder
    env['BUILDERS']['GeneratePktMonReport'] = env.Builder(action = generate_pktmon_report, suffix='.pktmon.csv')
    env['BUILDERS']['GeneratePerfMonReport'] = env.Builder(action = generate_perfmon_report, suffix='.perfmon.csv')
    env['BUILDERS']['GenerateStackMonReport'] = env.Builder(action = generate_stackmon_report, suffix='.stackmon.csv')

    # Create Pass/Fail Report builder
    env['BUILDERS']['CalculatePassFail'] = env.Builder(action = calculate_pass_fail, suffix='.status.log')

    # Create Audio Plots builder
    env['BUILDERS']['AudioPlotsBuilder'] = env.Builder(action = audio_plots_builder, suffix='.audioplots.html')

    # Create Simulink Config file builder
    env['BUILDERS']['SimulinkConfig'] = env.Builder(action = generate_simulink_config, suffix='.m')
    env['BUILDERS']['SimulinkProbePoints'] = env.Builder(action = generate_simulink_probepoints, suffix='.m')

    # Create AtomicList Config file builder
    env['BUILDERS']['AtomicListConfig'] = env.Builder(action = generate_atomiclist_config, suffix='.m')

    # Create JModule Config file builder
    env['BUILDERS']['JModConfig'] = env.Builder(action = generate_jmodule_config, suffix='.h')

    # Create JModule Config file builder
    env['BUILDERS']['BcfConfig'] = env.Builder(action = generate_bcf_config, suffix='.h')

    env['BUILDERS']['JControlGenerate'] = env.Builder(action = generate_jcontrol, suffix='.cpp')

    # Create CCES project builder
    env['BUILDERS']['CCESProject'] = env.Builder(action = generate_cces_project)

    # Apply JAutogenProbe benchmark code
    env['BUILDERS']['JAutogenProbe'] = env.Builder(action = apply_jautogenProbe)

    # Generate BAF Lite header
    env['BUILDERS']['JAutogenBafLite'] = env.Builder(action = jautogenBafLite)
