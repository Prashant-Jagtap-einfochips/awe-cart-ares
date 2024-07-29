import os
import subprocess
import shutil
import sys

from signal import SIGTERM

from OJTSI.BAFLoggerOJT import GenExecClass
from OJTSI.BAFLoggerOJT import HiFiElfClass
from OJTSI.BAFLoggerOJT import DxeClass

try:
    import spur
except ImportError:
    print ("Info: spur package not installed.")

try:
    import fabric
except ImportError:
    print ("Info: Fabric package not installed.")

def __functional_simulation_warning():
    print ("Warning: Benchmarking results are invalid!!! Functional Simulation in progress.")

def __separate_core_runlog(target, source, corestr):
    with open(source) as fp:
        runlog = fp.readlines()
    with open(target, 'w') as fp:
        for ln in runlog:
            ln = ln.strip()
            if ln.startswith(corestr):
                ln = ln.replace(corestr, '', 1).strip()
                fp.write('%s\n' % ln)

def run_target(target, source, env):
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    if 'RUNNER' in env :
        run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
        if optionArch == 'gul' :
            if optionPlatform in ['HIL','RTA']:
                run_cmd += ['-b','"0,main"']
                run_cmd += ['--core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe,'+str(source[0])+'"']
            elif optionPlatform == 'PIL' :
                if optionRunner == 'sim' :
                    run_cmd += ['--core "0,'+str(source[0])+'"']
                elif optionRunner == 'ezkit':
                    run_cmd += ['--core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe,'+str(source[0])+'"']

        elif optionArch == 'sharc' :
            if optionPlatform in ['HIL','RTA'] :
                run_cmd += ['-b','"0,main"']
                run_cmd += ['--core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['TOOLSPATH']+'SHARC/ldr/ezkitSC584_preload_core0_v01"']
                run_cmd += ['--core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+str(source[0])+'"']
            elif optionPlatform == 'PIL' :
                if optionRunner == 'sim' :
                    run_cmd += ['--core "1,'+str(source[0])+'"']
                elif optionRunner == 'ezkit' :
                    run_cmd += ['--core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['TOOLSPATH']+'SHARC/ldr/ezkitSC584_preload_core0_v01"']
                    run_cmd += ['--core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+str(source[0])+'"']
    else :
        run_cmd += ['%s' % (source[0])]
        if len(source) > 1 :
            run_cmd += ['-c %s' % (source[1])]
            run_cmd += ['-i %s' % (source[2])]
            run_cmd += ['-g %s' % (source[3])]
            run_cmd += ['-o %s' % (source[4])]
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

def run_c66_pil_evm_target(target, source, env):
    run_cmd = ['Misc\\evmc6678\\loadti.bat']
    run_cmd += [env['DSS_ROOT']]
    run_cmd += ['-r']
    run_cmd += ['-c Misc\\evmc6678\\evm6678le.ccxml']
    run_cmd += ['-s ' + str(target[0])]
    run_cmd += ['-msl ' + str(source[4])]
    run_cmd += [str(source[0])]
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd),
                            stderr=subprocess.STDOUT,
                            universal_newlines=True )
    print( result )

def gen_hifi_gdb_cmd_file(env,gdb_cmd_file_name, isSim):
    with open(gdb_cmd_file_name, 'w') as gdb_cmd_f:
        if isSim:
            gdb_cmd_f.write('target iss --mem_model --nosummary\nset max-value-size unlimited\nb main\nb platform_close\nrun\ncontinue\n')
            gdb_cmd_f.write('dump binary value ./{}/audioOut.bin data_out_binary\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('dump binary value ./{}/baf_logger.bin BAF_logger\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('dump binary value ./{}/heap_log_buffer.bin heap_log_buffer\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('finish\ncontinue\nquit')
        else:
            gdb_cmd_f.write('target remote :20000\nset max-value-size unlimited\nload\nb main\nb platform_close\ncontinue\ncontinue\n')
            gdb_cmd_f.write('dump binary value ./{}/audioOut.bin data_out_binary\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('dump binary value ./{}/baf_logger.bin BAF_logger\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('dump binary value ./{}/heap_log_buffer.bin heap_log_buffer\n'.format(env['APP_ROOT']))
            gdb_cmd_f.write('finish\ncontinue\ndetach\nquit')

# Runs xt-gdb based on gdb command file
def run_hifi_gdb(target, source, env, gdb_cmd_file_name):
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    run_cmd += [str(source[0])]
    run_cmd += env['RUNNERFLAGS']
    run_cmd += ['-command', gdb_cmd_file_name]
    print (' '.join(run_cmd))

    print ("Load 'n Run: " + str(source[0]) + " ...")
    print ("This process can take a long time, please keep the command window open.")

    result = subprocess.check_output(' '.join(run_cmd),
                            stderr=subprocess.STDOUT,
                            universal_newlines=True )
    print( result )

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)


def run_hifi4_rt685pil_evb_target(target, source, env):
    # Handle the binaries with the exec class object
    exec_obj = HiFiElfClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.elf',
                            map_ext = '.map',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')

    jlink_read_filename = env['APP_ROOT']+'/command_read.jlink'
    exec_obj.create_jlink_read_script(jlink_read_filename, env['APP_ROOT'], env['APP_ROOT'])

    # Generate the JLink script for flashing the ARM Core
    jlink_flash_filename = env['APP_ROOT']+'/command_flash_m33.jlink'
    with open(jlink_flash_filename, 'w') as jlink_flash_f:
        jlink_flash_f.write('Device MIMXRT685S_M33\nLE\nSelectInterface 1\nSpeed auto\nConnect\nReset\n')
        jlink_flash_f.write('LoadFile {}\Platform\hifi4\RT685PIL\evkmimxrt685_dsp_naturedsp_cm33.bin 0x08000000\n'.format(env['APP_ROOT']))
        jlink_flash_f.write('Reset\nGo\nWaitHalt 3000\nHalt\nExit')

    # Flash the M33 image
    run_cmd = env['JLINKEXE']+' -CommandFile '+jlink_flash_filename
    print ("Calling J-Link Commander to flash M33 image...")

    print(run_cmd)

    result = subprocess.check_output(run_cmd,
                            stderr=subprocess.STDOUT,
                            universal_newlines=True )
    print( result )

    # Pops up the xt-ocd in a new command window since it is blocking. Trying sending an ENTER command to check if connection failed
    xt_ocd_proc = subprocess.Popen(env['XTOCDEXE'],
                                text=True,
                                stdin=subprocess.PIPE,
                                creationflags=subprocess.CREATE_NEW_CONSOLE)

    connected = False
    try:
        xt_ocd_proc.communicate(input='\n', timeout=2)
        print('Error: XT-OCD could not connect to EVB.')
        xt_ocd_proc.send_signal(SIGTERM) # The process should already be terminated, but this guarantees it
    except subprocess.TimeoutExpired:
        connected = True

    if connected:
        # Generate the GDB script
        gdb_cmd_file_name = env['APP_ROOT']+'/rt685_evb_gdb_commands.gdb'
        gen_hifi_gdb_cmd_file(env, gdb_cmd_file_name, isSim=False)

        # Proceed with HiFi4 programming/running
        run_hifi_gdb(target,source,env, gdb_cmd_file_name)

        # Close the OCD daemon
        xt_ocd_proc.send_signal(SIGTERM)

        # Move audioOut.bin to sim\Baf_test\test_default
        shutil.move(env['APP_ROOT']+'/audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            exec_obj.process_logger_dumps(work_dir, work_prefix, log_dir=env['APP_ROOT']+'/')


def run_hifi4_rt685pil_sim_target(target, source, env):
    # Handle the binaries with the exec class object
    exec_obj = HiFiElfClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.elf',
                            map_ext = '.map',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')

    # Generate the GDB script
    gdb_cmd_file_name = env['APP_ROOT']+'/rt685_sim_gdb_commands.gdb'
    gen_hifi_gdb_cmd_file(env, gdb_cmd_file_name, isSim=True)

    # Proceed with HiFi4 programming/running
    run_hifi_gdb(target,source,env, gdb_cmd_file_name)

    # Move audioOut.bin to sim\Baf_test\test_default
    shutil.move(env['APP_ROOT']+'/audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        exec_obj.process_logger_dumps(work_dir, work_prefix, log_dir=env['APP_ROOT']+'/')
def run_hifi4_simulatorpil_sim_target(target, source, env):
    # Handle the binaries with the exec class object
    exec_obj = HiFiElfClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.elf',
                            map_ext = '.map',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')
    # Generate the GDB script
    gdb_cmd_file_name = env['APP_ROOT']+'/hifi4sim_sim_gdb_commands.gdb'
    gen_hifi_gdb_cmd_file(env, gdb_cmd_file_name, isSim=True)
    # Proceed with HiFi4 programming/running
    run_hifi_gdb(target,source,env, gdb_cmd_file_name)
    # Move audioOut.bin to sim\Baf_test\test_default
    shutil.move(env['APP_ROOT']+'/audioOut.bin',str(source[4]))
    work_dir = os.path.dirname(str(target[0]))
    work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
    exec_obj.process_logger_dumps(work_dir, work_prefix, log_dir=env['APP_ROOT']+'/')

def run_hifi5_ab1585pil_evb_target(target, source, env):
    # Handle the binaries with the exec class object
    exec_obj = HiFiElfClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.elf',
                            map_ext = '.map',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')

    # TODO - setup CODA? Or is a prereq that it is flashed by hand?

    # Pops up the xt-ocd in a new command window since it is blocking. Trying sending an ENTER command to check if connection failed
    xt_ocd_proc = subprocess.Popen(env['XTOCDEXE'],
                                text=True,
                                stdin=subprocess.PIPE,
                                creationflags=subprocess.CREATE_NEW_CONSOLE)

    connected = False
    try:
        xt_ocd_proc.communicate(input='\n', timeout=2)
        print('Error: XT-OCD could not connect to EVB.')
        xt_ocd_proc.send_signal(SIGTERM) # The process should already be terminated, but this guarantees it
    except subprocess.TimeoutExpired:
        connected = True

    if connected:
        # Generate the GDB script
        gdb_cmd_file_name = env['APP_ROOT']+'/ab1585_evb_gdb_commands.gdb'
        gen_hifi_gdb_cmd_file(env, gdb_cmd_file_name, isSim=False)

        # Proceed with HiFi 5 programming/running
        run_hifi_gdb(target, source, env, gdb_cmd_file_name)

        # Close the OCD daemon
        xt_ocd_proc.send_signal(SIGTERM)

        # Move audioOut.bin to sim\Baf_test\test_default
        shutil.move(env['APP_ROOT']+'/audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            exec_obj.process_logger_dumps(work_dir, work_prefix, log_dir=env['APP_ROOT']+'/')


def run_hexagon_pil_sim_target(target, source, env):
    exec_obj = GenExecClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.out',
                            map_ext = '.map',
                            map_type = 'hexagon',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')

    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [str(source[0])]

    if '--timing' not in env['RUNNERFLAGS']:
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd),
                            stderr=subprocess.STDOUT,
                            universal_newlines=True )
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        exec_obj.process_logger_dumps(work_dir, work_prefix)

def run_qnx_nto_aarch64_pil_sim_target(target, source, env):
    optionHostname = None
    optionUser = None
    optionPwd = None

    if env['optionHostname']:
        optionHostname=env['optionHostname']
    elif 'OJ_SSH_HOSTNAME' in os.environ:
        optionHostname=os.environ['OJ_SSH_HOSTNAME']

    if env['optionUser']:
        optionUser=env['optionUser']
    elif 'OJ_SSH_USERNAME' in os.environ:
        optionUser=os.environ['OJ_SSH_USERNAME']

    if env['optionPwd']:
        optionPwd=env['optionPwd']
    elif 'OJ_SSH_PASSWORD' in os.environ:
        optionPwd=os.environ['OJ_SSH_PASSWORD']

    if (optionHostname == None) or (optionUser == None) or (optionPwd == None):
        print('Error: ssh connection needs hostname, username, password to be specified either using scons options or through environment variables OJ_SSH_HOSTNAME,OJ_SSH_USERNAME,OJ_SSH_PASSWORD')
        return

    if 'fabric' not in sys.modules:
        print('Error: python modules fabric (https://www.fabfile.org/ ) is needed for qnx_nto_aarch64')
        return

    exec_obj = GenExecClass(index = None,
                            exec_file = str(source[0]),
                            file_ext = '.out',
                            map_ext = '.map',
                            map_type = 'gnu',
                            baf_logger = 'BAF_logger',
                            heap_log_buffer = 'heap_log_buffer')


    with fabric.Connection(host=optionHostname, user=optionUser, port=22, connect_kwargs={'password': optionPwd}) as cnx:
        # Board may not be clean to begin with, and put() fails in that case.
        cnx.run('rm -f /root/Baf.out')
        cnx.put(str(source[0]), '/root')

        if env['shlib'] != None:
            cnx.run('rm -f /root/lib_bose.so')
            cnx.put(str(env['shlib']), '/root')

        cnx.run('cd /root')
        cnx.run(f'chmod +x {os.path.basename(str(source[0]))}')
        result = cnx.run(f'./{os.path.basename(str(source[0]))}')
        try:
            cnx.get('/root/audioOut.bin')
            if not (('optionBafLite' in env) and (env['optionBafLite'])):
                cnx.get('/root/baf_logger.bin')
                cnx.get('/root/heap_log_buffer.bin')
        except:
            print(" Getting output files failed.")
        cnx.run('rm -f /root/Baf.out')
        cnx.run('rm -f /root/lib_bose.so')
        cnx.run('rm -f /root/audioOut.bin')
        cnx.run('rm -f /root/baf_logger.bin')
        cnx.run('rm -f /root/heap_log_buffer.bin')

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result.stdout)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        exec_obj.process_logger_dumps(work_dir, work_prefix, arch='LP64')

def run_gul_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,'+str(source[0])+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']

    if 'Functional-Sim' in ' '.join(run_cmd):
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_gxp_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    data_out_symbol = dxe_obj.get_symbol('data_out_binary.')
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "1,'+str(source[0])+'"']
    run_cmd += [' --core "2,'+env['APP_ROOT']+'/Platform/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,'+data_out_symbol.name+','+str(os.path.getsize(str(source[4])))+',1"']
    #run_cmd += [' -v2']

    if 'Functional-Sim' in ' '.join(run_cmd):
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")

    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix, debug=False)

def run_sc594_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    data_out_symbol = dxe_obj.get_symbol('data_out_binary.')
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "1,'+str(source[0])+'"']
    run_cmd += [' --core "2,'+env['APP_ROOT']+'/Platform/sc594/PIL/core2.dxe"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,'+data_out_symbol.name+','+str(os.path.getsize(str(source[4])))+',1"']

    if 'Functional-Sim' in ' '.join(run_cmd):
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")

    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix, debug=False)

def run_sc598_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    data_out_symbol = dxe_obj.get_symbol('data_out_binary.')
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "1,'+str(source[0])+'"']
    run_cmd += [' --core "2,'+env['APP_ROOT']+'/Platform/sc598/PIL/core2.dxe"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,'+data_out_symbol.name+','+str(os.path.getsize(str(source[4])))+',1"']

    if 'Functional-Sim' in ' '.join(run_cmd):
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")

    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix, debug=False)

def run_melville_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,'+str(source[0])+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_sharc_pil_sim_target(target, source, env):
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = None)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "1,'+str(source[0])+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']

    if 'Functional-Sim' in ' '.join(run_cmd):
        __functional_simulation_warning()

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_gul_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_gul_pil_ezkit_pgo_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file,
                       genpgo=True)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    run_cmd += [' --datafile "0,out,'+dxe_obj.baf_pgo_file+',binary:le,'+dxe_obj.baf_pgo_symbol.name+','+dxe_obj.baf_pgo_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,'+dxe_obj.baf_pgt_file+',binary:le,'+dxe_obj.baf_pgt_symbol.name+','+dxe_obj.baf_pgt_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,'+dxe_obj.ram_files_file+',binary:le,'+dxe_obj.ram_files_symbol.name+','+dxe_obj.ram_files_symbol.size+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

    dxe_obj.process_pgo_dumps(work_dir, work_prefix,
                              toolspath=env['TOOLSPATH'],
                              pgo_file = env['variant_root'] + env['PROCESSOR'] + '/pgo/Baf.pgo')

def run_gxp_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21593_preload_core1.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)

    if ('optionRTOS' in env) and (env['optionRTOS']) and (env['optionRTOS'] == 'FreeRTOS'):
        core2_dxe = env['APP_ROOT']+'/PlatformFreeRTOS/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe'
    else:
        core2_dxe = env['APP_ROOT']+'/Platform/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe'

    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+core2_dxe+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)
        dxe_obj.get_accel_processes(work_dir, work_prefix)

def run_gxp_pil_ezkit_dual_target(target, source, env):
    dxe_obj = []
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21593_preload_core1.dxe'
    dxe_obj.append(DxeClass(index = '1',
                      dxe_file = str(source[0]),
                      preload_file = preload_file))
    dxe_obj.append(DxeClass(index = '2',
                       dxe_file = str(source[1]),
                       preload_file = None))
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj[0].preload_file+','+dxe_obj[0].dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+dxe_obj[1].dxe_file+'"']

    for idx in range(2):
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "' + str(idx) + ',out,' + dxe_obj[idx].baf_logger_file      + ',binary:le,' + dxe_obj[idx].baf_logger_symbol.demangled      + ',' + dxe_obj[idx].baf_logger_symbol.size      + ',1"']
            run_cmd += [' --datafile "' + str(idx) + ',out,' + dxe_obj[idx].heap_log_buffer_file + ',binary:le,' + dxe_obj[idx].heap_log_buffer_symbol.demangled + ',' + dxe_obj[idx].heap_log_buffer_symbol.size + ',1"']
        run_cmd += [' --datafile "' + str(idx) + ',out,audioOut' + str(idx) + '.bin,binary,data_out_binary.,' + str(os.path.getsize(str(source[2+idx*9+3]))) + ',1"']

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + "; " + str(source[1]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    for idx in range(2):
        shutil.move('audioOut' + str(idx) + '.bin',str(source[2+idx*9+3]))

    for idx in range(2):
        __separate_core_runlog( str(target[0]).replace('.run.log','_%d.run.log' % (idx+1)),
                                str(target[0]),
                                'SPC%d>' % (idx+1) )

    work_dir = os.path.dirname(str(target[0]))
    work_prefix = os.path.basename(str(target[0])).replace('.run.log','')

    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        for idx in range(2):
            dxe_obj[idx].process_logger_dumps(work_dir, work_prefix)
            dxe_obj[idx].get_accel_processes(work_dir, work_prefix)

def run_sc594_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC594W_preload_core0'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+env['APP_ROOT']+'/Platform/sc594/PIL/core0out"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "2,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['APP_ROOT']+'/Platform/sc594/PIL/core2.dxe"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_sc598_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC598W_preload_core1.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+env['APP_ROOT']+'/Platform/sc598/PIL/core2.dxe"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_melville_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['APP_ROOT']+'/Platform/melville/RTA/preload.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_sharc_pil_ezkit_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC584_preload_core0_v01'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+str(source[0])+'"']
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
    run_cmd += [' --datafile "1,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    shutil.move('audioOut.bin',str(source[4]))
    if not (('optionBafLite' in env) and (env['optionBafLite'])):
        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
        dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_gul_hil_rta_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"0,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_gxp_hil_rta_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21593_preload_core1.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)

    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False

    if ('optionRTOS' in env) and (env['optionRTOS']) and (env['optionRTOS'] == 'FreeRTOS'):
        core2_dxe = env['APP_ROOT']+'/PlatformFreeRTOS/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe'
    else:
        core2_dxe = env['APP_ROOT']+'/Platform/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe'

    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"0,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+core2_dxe+'"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)
            dxe_obj.get_accel_processes(work_dir, work_prefix)

def run_gxp_hil_rta_dual_target(target, source, env):
    dxe_obj = []
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkit21593_preload_core1.dxe'
    dxe_obj.append(DxeClass(index = '1',
                      dxe_file = str(source[0]),
                      preload_file = preload_file))
    dxe_obj.append(DxeClass(index = '2',
                      dxe_file = str(source[1]),
                      preload_file = None))

    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False

    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"0,main"']
        run_cmd += ['-b','"1,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj[0].preload_file+','+dxe_obj[0].dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+dxe_obj[1].dxe_file+'"']
    if not free_run:
        for idx in range(2):
            if not (('optionBafLite' in env) and (env['optionBafLite'])):
                run_cmd += [' --datafile "' + str(idx) + ',out,' + dxe_obj[idx].baf_logger_file      + ',binary:le,' + dxe_obj[idx].baf_logger_symbol.demangled      + ',' + dxe_obj[idx].baf_logger_symbol.size      + ',1"']
                run_cmd += [' --datafile "' + str(idx) + ',out,' + dxe_obj[idx].heap_log_buffer_file + ',binary:le,' + dxe_obj[idx].heap_log_buffer_symbol.demangled + ',' + dxe_obj[idx].heap_log_buffer_symbol.size + ',1"']
            run_cmd += [' --datafile "' + str(idx) + ',out,audioOut' + str(idx) + '.bin,binary,data_out_binary.,' + str(os.path.getsize(str(source[2+idx*9+3]))) + ',1"']

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + "; " + str(source[1]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        for idx in range(2):
            shutil.move('audioOut' + str(idx) + '.bin',str(source[2+idx*9+3]))

        for idx in range(2):
            __separate_core_runlog( str(target[0]).replace('.run.log','_%d.run.log' % (idx+1)),
                                str(target[0]),
                                'SPC%d>' % (idx+1) )

        work_dir = os.path.dirname(str(target[0]))
        work_prefix = os.path.basename(str(target[0])).replace('.run.log','')

        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            for idx in range(2):
                dxe_obj[idx].process_logger_dumps(work_dir, work_prefix)
                dxe_obj[idx].get_accel_processes(work_dir, work_prefix)

def run_sc594_hil_rta_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC594W_preload_core0'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"1,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+env['APP_ROOT']+'/Platform/sc594/'+env['optionPlatform'].strip('SC594')+'/core0out"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "2,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['APP_ROOT']+'/Platform/sc594/'+env['optionPlatform'].strip('SC594')+'/core2.dxe"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_sc598_hil_rta_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC598W_preload_core1.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"0,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|,'+env['APP_ROOT']+'/Platform/sc598/'+env['optionPlatform'].strip('SC598')+'/core2.dxe"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.demangled+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.demangled+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += [' --datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )

    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_melville_hil_rta_single_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['APP_ROOT']+'/Platform/melville/RTA/preload.dxe'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"0,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+dxe_obj.dxe_file+'"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "0,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "0,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += ['--datafile "0,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))

    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_melville_hil_rta_dual_target(target, source, env):
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    run_cmd += ['-b','"1,main"']
    run_cmd += ['--core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['APP_ROOT']+'/Platform/melville/RTA/preload.dxe,'+str(source[0])+'"']
    run_cmd += ['-b','"0,main"']
    run_cmd += ['--core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['TOOLSPATH']+'SHARC/ldr/ezkit21569_preload.dxe,'+str(source[1])+'"']

    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

def run_sharc_hil_rta_target(target, source, env):
    if ('optionPreload' in env) and (env['optionPreload']) and (isinstance(env['optionPreload'], dict)):
        preload_file = env['optionPreload'][0]
    else:
        preload_file = env['TOOLSPATH']+'SHARC/ldr/ezkitSC584_preload_core0_v01'
    dxe_obj = DxeClass(index = None,
                       dxe_file = str(source[0]),
                       preload_file = preload_file)
    if ('optionCfg' in env) and (env['optionCfg']) and ('FreeRunning' in env['optionCfg']):
        free_run = env['optionCfg']['FreeRunning']
    else:
        free_run = False
    run_cmd = []
    if 'RUNNERPATH' in env :
        run_cmd.append(env['RUNNERPATH'].replace('"',''))
    run_cmd[0] += env['RUNNER']
    if 'RUNNERFLAGS' in env :
        run_cmd += env['RUNNERFLAGS']
    if free_run:
        run_cmd += ['-b','"1,main"']
    run_cmd += [' --core "0,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+dxe_obj.preload_file+','+env['APP_ROOT']+'/Platform/'+env['optionArch']+'/'+env['optionPlatform']+'/core0_out"']
    run_cmd += [' --core "1,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+str(source[0])+'"']
    run_cmd += [' --core "2,EMU|KIT|CLEAR_SYMBOLS|RUN_AFTER_LOAD|'+env['APP_ROOT']+'/Platform/'+env['optionArch']+'/'+env['optionPlatform']+'/core2.dxe"']
    if not free_run:
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            run_cmd += [' --datafile "1,out,'+dxe_obj.baf_logger_file+',binary:le,'+dxe_obj.baf_logger_symbol.name+','+dxe_obj.baf_logger_symbol.size+',1"']
            run_cmd += [' --datafile "1,out,'+dxe_obj.heap_log_buffer_file+',binary:le,'+dxe_obj.heap_log_buffer_symbol.name+','+dxe_obj.heap_log_buffer_symbol.size+',1"']
        run_cmd += [' --datafile "1,out,audioOut.bin,binary,data_out_binary.,'+str(os.path.getsize(str(source[4])))+',1"']
    print (' '.join(run_cmd))
    print ("Load 'n Run: " + str(source[0]) + " ...")
    result = subprocess.check_output(' '.join(run_cmd), stderr=subprocess.STDOUT, universal_newlines=True)
    print( result )
    with open('%s' % (target[0]), 'w') as text_file:
        text_file.write(result)

    if not free_run:
        shutil.move('audioOut.bin',str(source[4]))
        if not (('optionBafLite' in env) and (env['optionBafLite'])):
            work_dir = os.path.dirname(str(target[0]))
            work_prefix = os.path.basename(str(target[0])).replace('.run.log','')
            dxe_obj.process_logger_dumps(work_dir, work_prefix)

def run_amd64_pil_preq_target(target, source, env):
    print ('Info: run amd64 simulation')
    shell = spur.SshShell(hostname=env['optionHostname'], port=5679, username=env['optionUser'], password=env['optionPwd'], missing_host_key=spur.ssh.MissingHostKey.accept)
    try:
        with shell:
            remoteF = []
            # Five files: .out, audioIn.bin, audioRef.bin, request.ojx, audioOut.bin
            for i in range(5):
                with open(str(source[i]), "rb") as local_file:
                    subfolder = str(source[i]).split('\\')
                    remoteF.append(subfolder[len(subfolder) - 1])
                    with shell.open(remoteF[i], "wb") as remote_file:
                        # Copy files to Linux
                        shutil.copyfileobj(local_file, remote_file)
                        # Change permission
                        shell.run(["chmod", "u+x", remoteF[i]])
            # Run PIL
            if len(remoteF) == 5:
                result = shell.run(['./' + remoteF[0], "-c", remoteF[1], "-i", remoteF[2], "-g", remoteF[3], "-o", remoteF[4]])
                print ('Info: Preq simulation runs over SSH. The result:')
                print(result.output)
                # Write to log file
                with open('%s' % (target[0]), 'w') as text_file:
                    text_file.write(result.output)
            else:
                print ('Info: Not all files are copied to Linux machine, PIL did not run')
    except:
        print ('Error: Login failed. Please check the username, password or hostname!')

def run_simulation(target, source, env,
                   optionArch, optionRunner, optionPlatform,
                   optionCfg=None,
                   optionHostname=None, optionUser=None, optionPwd=None,
                   optionRTOS=None,
                   optionPreload=None,
                   optionBafLite=False):
    #        JSimulation = env.RunTarget(target='#/out/${PROCESSOR}/log/'+optionPlatform+'/'+jframework['Name']+'/'+jframework['TestCase'], source=source )
   JSimulation = None
   if optionArch == 'gul' :
         if optionPlatform in ['PIL', 'SOMPIL']:
            if optionRunner == 'sim' :
               JSimulation = env.RunGulPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
            elif optionRunner == 'ezkit' :
               JSimulation = env.RunGulPilEzkitTarget(target=target, source=source, optionPreload=optionPreload, optionBafLite=optionBafLite )
            else :
               JSimulation = env.RunGulPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
         elif optionPlatform in ['HIL','RTA','SOMHIL','SOMRTA']:
            JSimulation = env.RunGulHilRtaTarget(target=target, source=source, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite  )
         elif optionPlatform in ['MHIL','MRTA']:
            JSimulation = env.RunMelvilleHilRtaSingleTarget(target=target, source=source,  optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite  )
         elif optionPlatform == 'MPIL':
            JSimulation = env.RunMelvillePilEzkitTarget(target=target, source=source, optionPreload=optionPreload, optionBafLite=optionBafLite  )
         elif optionPlatform in ['MDRTA','MDHIL']:
             JSimulation = env.RunMelvilleHilRtaDualTarget(target=target, source=source, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite  )
         else:
            JSimulation = env.RunGulPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
   elif optionArch == 'gxp' :
         if optionPlatform == 'PIL':
            if optionRunner == 'sim' :
               JSimulation = env.RunGxpPilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
            elif optionRunner == 'ezkit' :
               JSimulation = env.RunGxpPilEzkitTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionRTOS=optionRTOS, optionPreload=optionPreload, optionBafLite=optionBafLite )
            else :
               JSimulation = env.RunGxpPilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
         elif optionPlatform in ['HIL','RTA']:
            JSimulation = env.RunGxpHilRtaTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionRTOS=optionRTOS, optionPreload=optionPreload, optionBafLite=optionBafLite )
         elif optionPlatform == 'DPIL':
            JSimulation = env.RunGxpDPilEzkitTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
         elif optionPlatform in ['DHIL','DRTA']:
            JSimulation = env.RunGxpDHilDRtaTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
         elif optionPlatform == 'SC594PIL':
            if optionRunner == 'sim' :
               JSimulation = env.RunSc594PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
            elif optionRunner == 'ezkit' :
               JSimulation = env.RunSc594PilEzkitTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
            else :
               JSimulation = env.RunSc594PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
         elif optionPlatform in ['SC594HIL','SC594RTA']:
            JSimulation = env.RunSc594HilRtaTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
         elif optionPlatform == 'SC598PIL':
            if optionRunner == 'sim' :
               JSimulation = env.RunSc598PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
            elif optionRunner == 'ezkit' :
               JSimulation = env.RunSc598PilEzkitTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
            else :
               JSimulation = env.RunSc598PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
         elif optionPlatform in ['SC598HIL','SC598RTA']:
            JSimulation = env.RunSc598HilRtaTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
         else:
            JSimulation = env.RunGxpPilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionBafLite=optionBafLite )
   elif optionArch == 'sharc' :
         if optionPlatform == 'PIL':
            if optionRunner == 'sim' :
               JSimulation = env.RunSharcPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
            elif optionRunner == 'ezkit' :
               JSimulation = env.RunSharcPilEzkitTarget(target=target, source=source, optionPreload=optionPreload, optionBafLite=optionBafLite )
            else :
               JSimulation = env.RunSharcPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
         elif optionPlatform in ['HIL','RTA']:
            JSimulation = env.RunSharcHilRtaTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionCfg=optionCfg, optionPreload=optionPreload, optionBafLite=optionBafLite )
         else:
            JSimulation = env.RunSharcPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
   elif optionArch == 'amd64':
         if optionRunner == 'preq':
            JSimulation = env.RunAmd64PilPreqTarget(target=target, source=source, optionHostname=optionHostname, optionUser=optionUser, optionPwd=optionPwd, optionBafLite=optionBafLite)
   elif optionArch == 'c66' :
         if optionPlatform == 'PIL':
            if optionRunner == 'evm' :
               JSimulation = env.RunC66PilEVMTarget(target=target, source=source, optionBafLite=optionBafLite )
   elif optionArch == 'hifi4' :
         if optionPlatform == 'RT685PIL':
            if optionRunner == 'evb':
                JSimulation = env.RunHifi4RT685PilEVBTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
            elif optionRunner == 'sim':
                JSimulation = env.RunHifi4RT685PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
         elif optionPlatform == 'QCC7226PIL':
            if optionRunner == 'sim':
                JSimulation = env.RunHifi4QCC7226PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
         elif optionPlatform == 'SIMULATORPIL':
            if optionRunner == 'sim':
                JSimulation = env.RunHifi4SimulatorPilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
   elif optionArch == 'hifi5' :
         if optionPlatform == 'AB1585PIL':
            if optionRunner == 'evb':
                JSimulation = env.RunHifi5AB1585PilEVBTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
            # elif optionRunner == 'sim':
            #     JSimulation = env.RunHifi5AB1585PilSimTarget(target=target, source=source, optionArch=optionArch, optionPlatform=optionPlatform, optionBafLite=optionBafLite)
   elif optionArch in ['hexagon','hexagon_v66','hexagon_v73'] :
         if optionPlatform == 'PIL':
            if optionRunner == 'sim' :
               JSimulation = env.RunHexagonPilSimTarget(target=target, source=source, optionBafLite=optionBafLite )
   elif optionArch == 'qnx_nto_aarch64' :
         if optionPlatform in ['PIL', 'PIL_shared_library']:
            if optionRunner == 'evb' :
               shlib = None
               if optionPlatform == 'PIL_shared_library':
                   shlib = env['variant_root'] + optionArch + '/obj/Platform/BAF/PIL_shared_library/'+env['SHLIBNAME'] + env['SHLIBSUFFIX']
               JSimulation = env.RunQnxNtoAarch64PilSimTarget(target=target, source=source, optionHostname=optionHostname, optionUser=optionUser, optionPwd=optionPwd, shlib = shlib, optionBafLite=optionBafLite)
   else:
         JSimulation = env.RunTarget(target=target, source=source, optionBafLite=optionBafLite )
   return JSimulation

