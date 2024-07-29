#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************
import os
import re

class MelvilleFileGroupClassifier:
    g_OS = ['adi_osal_arch_int_table.doj',
            'adi_osal_arch_c.doj',
            'asd_critical_section_ucos.doj',
            'lib_mem.doj',
            'adi_os_init.doj',
            'libosal_ucos3.dlb']

    g_BIPC = ['spc_bipc.dlb',
              'idc_msgs',
              'Endpoint.doj',
              'InterDomainEndpoint.doj',
              'RemoteICC.doj',
              'msg_mcapi_core_comm.doj']

    g_Platform = [
        'wave_play_control.doj',
        'spi2_configuration.doj',
        'wave_file_utils.doj',
        'wave_play.doj',
        'spdif_init.doj',
        'asrc_init.doj',
        'DSP_Platform_common.doj',
        'DSP_GPIO.doj',
        'cpu_bsp.doj',
        'dsp_hw_init.doj',
        'pcg_init.doj',
        'sport_init.doj',
        'sru_init.doj',
        'CommonMsgHdlrs.doj',
        'DSPHostCommTask.doj',
        'HostProtocol.doj',
        'hal_sharc.doj',
        'Messages.doj',
        'Sharc1_Main.doj',
        'Sharc2_Main.doj',
        'adi_initialize.doj',
        'pinmux_config.doj',
        'adi_uart.doj',
        'adi_gpio.doj',
        'adi_spu.doj',
        'adi_pdma_2156x.doj',
        'adi_linkport.doj',
        'LinkPort.doj',
        'low_rate_monotonic_scheduler.doj',
        'platform_integration.doj',
        'version.doj',
        'uart_handler.doj',
        'asd_critical_section_ucos.doj',
        'spi_handler.doj',
        'adi_spi.doj']

    g_Model = ['model.dlb']

    g_CRL = ['libcrlassembly.dlb',
             'irEvenProcessSharc_plus.doj',
             'pooliirSharcPlus.doj',
             'IFirProcessSharc_plus.doj',
             'FirEvenProcessSharc_plus.doj']

    g_Drivers = ['GulEmdmaDriver.doj',
                 'GulFirAccelDriver.doj',
                 'GulIirAccelDriver.doj',
                 'gMDMA.doj']

    g_adilibraries = ['libcmt.dlb',
                      'libiomt.dlb',
                      'libssl.dlb',
                      'libcc.dlb',
                      'libdsp.dlb',
                      '21569_hdr.doj']

    g_OpenJADE = ['jmodule.dlb', 'openjade.dlb']

    g_file_groups = [
        'OS',
        'BIPC',
        'Platform',
        'Model',
        'CRL',
        'Drivers',
        'OpenJADE',
        'ADILib']

    def __init__(self, path_to_filelist=None):
        # Get obj file list for OpenJADE Runtime, JFramework and Model if exists
        if path_to_filelist != None:
            rt_file = os.path.join(path_to_filelist, 'Runtime.lst')
            if os.path.exists(rt_file):
                with open(rt_file, 'r') as f:
                    rt_list = [x[:-2] + '.doj' for x in f.read().split('\n')]
                    self.g_OpenJADE += rt_list
            jf_file = os.path.join(path_to_filelist, 'Jframework.lst')
            if os.path.exists(jf_file):
                with open(jf_file, 'r') as f:
                    jf_list = [x[:-2] + '.doj' for x in f.read().split('\n')]
                    self.g_OpenJADE += jf_list
            ml_file = os.path.join(path_to_filelist, 'Model.lst')
            if os.path.exists(ml_file):
                with open(ml_file, 'r') as f:
                    ml_list = [x[:-2] + '.doj' for x in f.read().split('\n')]
                    self.g_Model += ml_list

        # Define the match and enumerate files for Model
        self.model_match = 'Model_\w+\.doj'
        self.modelRule = self.model_match + '|' + '|'.join(self.g_Model) if bool(self.g_Model) else self.model_match
        # Define the match and enumerate files for OpenJADE
        self.oj_match = 'JModule_\w+\.doj'
        self.ojRule = self.oj_match + '|' + '|'.join(self.g_OpenJADE) if bool(self.g_OpenJADE) else self.oj_match
        # Define the match and enumerate files for BIPC
        self.bipc_match = 'DSP_BIPC_\w+\.doj|BIPC_\w+\.doj|mcapi\w*\.doj|asd_mcapi\w*\.doj'
        self.bipcRule = self.bipc_match + '|' + '|'.join(self.g_BIPC) if bool(self.g_BIPC) else self.bipc_match
        # Define the match and enumerate files for UCOS
        self.os_match = 'os_\w+\.doj|cpu_\w+\.doj|adi_osal_ucos\w+\.doj'
        self.osRule = self.os_match + '|' + '|'.join(self.g_OS) if bool(self.g_OS) else self.os_match
        # Define the match and enumerate files for ADI Libraries
        self.adiRule = '|'.join(self.g_adilibraries) if bool(self.g_adilibraries) else ''
        # Define the match and enumerate files for CRL
        self.crlRule = '|'.join(self.g_CRL) if bool(self.g_CRL) else ''
        # Define the match and enumerate files for Drivers
        self.drv_match = 'gHAM\w*\.doj'
        self.drvRule = self.drv_match + '|' + '|'.join(self.g_Drivers) if bool(self.g_Drivers) else ''
        # Define the match and enumerate files for Platform
        self.plt_match = 'SPORT\w+\.doj'
        self.pltRule = self.plt_match + '|' + '|'.join(self.g_Platform) if bool(self.g_Platform) else ''

    def groups(self):
        return self.g_file_groups

    def classify(self, input_file):
        filename = ''
        libname = ''
        # Extract the doj file name: Frist, [abc.doj]; Second, xx\xx\xx\abc.doj; Third, abc.doj
        s = input_file.find('[')
        e = input_file.find(']')
        m = input_file.rfind('\\')
        if s >= 0 and e >= 0:
            filename = input_file[s + 1 : e]
        elif m > 0:
            filename = input_file[m + 1 :]
        else:
            filename = input_file
        # Extract the dlb library name
        loce = input_file.find('.dlb')
        locs = m + 1 if m >= 0 else 0
        libname = input_file[locs : loce] + '.dlb' if loce >= 0 else ''
        # Use Regular Expression to categorize
        if re.match(self.modelRule, filename) != None:
            return 'Model'
        elif re.match(self.ojRule, filename) != None:
            return 'OpenJADE'
        elif re.match(self.bipcRule, filename) != None:
            return 'BIPC'
        elif re.match(self.osRule, filename) != None:
            return 'OS'
        elif re.match(self.crlRule, filename) != None:
            return 'CRL'
        elif re.match(self.drvRule, filename) != None:
            return 'Drivers'
        elif re.match(self.pltRule, filename) != None:
            return 'Platform'
        elif re.match(self.adiRule, libname) != None or re.match(self.adiRule, filename) != None:
            return 'ADILib'
        else:
            print('Warning: file is not allocated: ' + input_file)
        return None