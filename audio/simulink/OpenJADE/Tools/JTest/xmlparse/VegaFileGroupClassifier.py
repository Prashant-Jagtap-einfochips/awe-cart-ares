#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import re

class VegaFileGroupClassifier:
    g_Platform = [
        'APN_FW_for_BAF_DSP1.dlb',
        'APN_FW_for_BAF_DSP2.dlb',
        'main.doj',
        'app_IVT.doj',
        'app_heaptab.doj',
        'app_startup.doj',
        'adi_initialize.doj',
        'pinmux_config.doj',
        'sru_config.doj']

    g_Integration = [
        'BAFApp.doj']

    g_OpenJADE = [
        'Audio_Graph.doj',
        'Connection_List.doj',
        'Execution_List.doj',
        'Module_List.doj',
        'Port_List.doj',
        'JModule_Adapt_Sys.doj',
        'JModule_Desc_Sys.doj',
        'JModule_List.doj',
        'BAF.doj',
        'Error.doj',
        'Framework_Builder.doj',
        'Heap.doj',
        'JFramework.doj',
        'JModule.doj',
        'JModule_Context.doj',
        'Logger.doj',
        'Module.doj',
        'Port.doj',
        'Ports.doj',
        'Process.doj',
        'Process_Context.doj' ]

    g_adilibraries = [
        'libcc.dlb',
        'libc.dlb',
        'libio.dlb',
        'libosal_noos.dlb',
        'libprofile.dlb',
        'libssl.dlb',
        'libdrv.dlb',
        'libdsp.dlb',
        'libldr.dlb',
        'adi_asrc.doj',
        'adi_spi.doj',
        'adi_sport.doj',
        'adi_uart.doj',
        'adi_pcg.doj',
        'adi_dai.doj',
        'adi_pwr.doj',
        'adi_spu.doj',
        'adi_pdma_2156x.doj',
        'adi_gpio.doj' ]

    g_file_groups = [
        'Integration',
        'Platform',
        'OpenJADE',
        'Model',
        'ADILib']

    def groups(self):
        return self.g_file_groups

    def classify(self, input_file):
        for x in self.g_Integration:
            if x in input_file:
                return 'Integration'
        for x in self.g_Platform:
            if x in input_file:
                return 'Platform'
        for x in self.g_adilibraries:
            if x in input_file:
                return 'ADILib'
        if 'BAF.dlb' in input_file:
            for x in self.g_OpenJADE:
                if x in input_file:
                    return 'OpenJADE'
                elif re.search('JModule_Adapt_.*\.doj]$', input_file) != None:
                    return 'OpenJADE'
                elif re.search('JModule_Desc_.*\.doj]$', input_file) != None:
                    return 'OpenJADE'
                else:
                    return 'Model'
        return None