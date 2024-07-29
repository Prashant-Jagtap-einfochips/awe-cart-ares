#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import re
import os

class JTestFileGroupClassifier:
    g_Platform = [
        'DPIL.dlb',
        'DHIL.dlb',
        'DRTA.dlb',
        'SOMPIL.dlb',
        'SOMHIL.dlb',
        'SOMRTA.dlb',
        'SC594PIL.dlb',
        'SC594HIL.dlb',
        'SC594RTA.dlb',
        'SC598PIL.dlb',
        'SC598HIL.dlb',
        'SC598RTA.dlb',
        'PIL.dlb',
        'HIL.dlb',
        'RTA.dlb']

    g_Integration = [
        'OJSP.doj',
        'Integration_Example.doj',
        'baf_stats.doj',
        'ram_vecs.doj',
        'app_IVT.doj',
        'app_heaptab.doj',
        'app_startup.doj']

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
        'BAF_Lite.doj',
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
        'libcmt.dlb',
        'libiomt.dlb',
        'libprofilemt.dlb',
        'libssl.dlb',
        'libdrv.dlb',
        'libdsp.dlb',
        'libldr.dlb' ]

    g_Model = ['BAF.dlb','Baf.dlb']

    g_CRL = ['libcrlassembly.dlb',
             'irEvenProcessSharc_plus.doj',
             'pooliirSharcPlus.doj',
             'IFirProcessSharc_plus.doj',
             'FirEvenProcessSharc_plus.doj']

    g_Drivers = ['GulEmdmaDriver.doj',
                 'GulFirAccelDriver.doj',
                 'GulIirAccelDriver.doj',
                 'gMDMA.doj']

    g_file_groups = [
        'Integration',
        'Platform',
        'CRL',
        'Drivers',
        'OpenJADE',
        'Model',
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

            plt_file = os.path.join(path_to_filelist, 'Platform.lst')
            if os.path.exists(plt_file):
                with open(plt_file, 'r') as f:
                    plt_list = [x for x in f.read().split('\n')]
                    self.g_Platform += plt_list


            crl_file = os.path.join(path_to_filelist, 'CRL.lst')
            if os.path.exists(crl_file):
                with open(crl_file, 'r') as f:
                    crl_list = [x for x in f.read().split('\n')]
                    self.g_CRL += crl_list

            drv_file = os.path.join(path_to_filelist, 'Drivers.lst')
            if os.path.exists(drv_file):
                with open(drv_file, 'r') as f:
                    drv_list = [x for x in f.read().split('\n')]
                    self.g_Drivers += drv_list

        # Define the match and enumerate files for Model
        self.model_match = 'Model_\w+\.doj'
        self.modelRule = self.model_match + '|' + '|'.join(self.g_Model) if bool(self.g_Model) else self.model_match
        # Define the match and enumerate files for OpenJADE
        self.oj_match = 'JModule_\w+\.doj'
        self.ojRule = self.oj_match + '|' + '|'.join(self.g_OpenJADE) if bool(self.g_OpenJADE) else self.oj_match
        # Define the match and enumerate files for ADI Libraries
        self.adiRule = '|'.join(self.g_adilibraries) if bool(self.g_adilibraries) else ''
        # Define the match and enumerate files for Platform
        self.pltRule = '|'.join(self.g_Platform) if bool(self.g_Platform) else ''
        # Define the match and enumerate files for Drivers
        self.drvRule = '|'.join(self.g_Drivers) if bool(self.g_Drivers) else ''
        # Define the match and enumerate files for CRL
        self.crlRule = '|'.join(self.g_CRL) if bool(self.g_CRL) else ''
        # Define the match and enumerate files for Integration
        self.integRule = '|'.join(self.g_Integration) if bool(self.g_Integration) else ''

    def groups(self):
        return self.g_file_groups

    def classify(self, input_file, debug=False):
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
        filename = os.path.basename(filename)
        # Extract the dlb library name
        loce = input_file.find('.dlb')
        locs = m + 1 if m >= 0 else 0
        libname = input_file[locs : loce] + '.dlb' if loce >= 0 else ''
        # Use Regular Expression to categorize
        if re.match(self.ojRule, filename) != None:
            category = 'OpenJADE'
        elif re.match(self.integRule, filename) != None:
            category = 'Integration'
        elif re.match(self.pltRule, filename) != None:
            category = 'Platform'
        elif re.match(self.crlRule, filename) != None:
            category = 'CRL'
        elif re.match(self.drvRule, filename) != None:
            category = 'Drivers'
        elif re.match(self.adiRule, libname) != None or re.match(self.adiRule, filename) != None:
            category = 'ADILib'
        elif re.match(self.modelRule, filename) != None:
            category = 'Model'
        else:
            category = None
        if category == None:
            print('Warning: file is not allocated: ' + input_file)
        else:
            if debug==True:
                print('%s -> %s' % (input_file, category))
        return category

