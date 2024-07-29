#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import re

class MARSFileGroupClassifier:
    g_AudioWeaver = [
        'AWECore.dlb',
        'AVAS.dlb']

    g_Platform = [
        'AudioDriver.doj',
        'Core1Main.doj',
        'Core1Setup.doj',
        'PlatformCore1.doj',
        'IPCCore1.doj',
        'Core2Main.doj',
        'Core2Setup.doj',
        'PlatformCore2.doj',
        'IPCCore2.doj',
        'adc_dac_playback.doj',
        'adi_adau1962a.doj',
        'adi_adau1979.doj',
        'FileSystemAPI.doj',
        'bm_audio_flow.doj',
        'Button_LED_GPIO.doj',
        'GPIO.doj',
        'int_dispatcher_215XX.doj',
        'adi_sport.doj',
        'adi_twi.doj',
        'adi_pdma_2156x.doj',
        'adi_spu.doj',
        'app_IVT.doj',
        'adi_initialize.doj',
        'app_heaptab.doj',
        'app_startup.doj']

    g_BAF = [
        'BAF.dlb']

    g_adilibraries = [
        'libcc.dlb',
        'libc.dlb',
        'libio.dlb',
        'libosal_noos.dlb',
        'libprofile.dlb',
        'libssl.dlb',
        'libdrv.dlb',
        'libdsp.dlb',
        'libldr.dlb' ]

    g_file_groups = [
        'AudioWeaver',
        'Platform',
        'BAF',
        'ADILib']

    def groups(self):
        return self.g_file_groups

    def classify(self, input_file):
        if 'InitAWB.doj' in input_file:
            return 'BAF'
        if re.search('BoseOJ-.*\.dlb', input_file) != None:
            return 'BAF'
        if re.search('Bose.*-.*\.dlb', input_file) != None:
            return 'BAF'
        if 'ModuleList.doj' in input_file:
            return 'Platform'
        for x in self.g_AudioWeaver:
            if x in input_file:
                return 'AudioWeaver'
        for x in self.g_Platform:
            if x in input_file:
                return 'Platform'
        for x in self.g_adilibraries:
            if x in input_file:
                return 'ADILib'
        for x in self.g_BAF:
            if x in input_file:
                return 'BAF'
        return None