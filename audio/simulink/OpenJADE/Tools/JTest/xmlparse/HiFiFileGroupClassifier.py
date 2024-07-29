#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import os

class HiFiFileGroupClassifier:
    g_Platform = [
        'crtend.o',
        'crtbegin.o',
        'crt1-boards.o',
        'crti.o',
        'crtn.o',
        'RT685PIL.a']
    

    g_Integration = [
         'OJSP.o',
        'Integration_Example.o',
        'baf_stats.o',
        'ram_vecs.o',
        'app_IVT.o',
        'app_heaptab.o',
        'app_startup.o']
    g_Model = ['BAF.a']

    g_CRL = ['FirEven.o']

    g_OpenJADE = ['Audio_Graph.o',
        'Connection_List.o',
        'Execution_List.o',
        'Module_List.o',
        'Port_List.o',
        'JModule_Adapt_Sys.o',
        'JModule_Desc_Sys.o',
        'JModule_List.o',
        'BAF.o',
        'Error.o',
        'Framework_Builder.o',
        'Heap.o',
        'JFramework.o',
        'JModule.o',
        'JModule_Context.o',
        'Logger.o',
        'Module.o',
        'Port.o',
        'Ports.o',
        'Process.o',
        'Process_Context.o' ]

    g_xtensalibraries = [
        'libgdbio.a',
        'libc.a',
        'libhandlers-board.a',
        'libhandler-reset.a',
        'libgcc.a',
        'libhal.a',
        'libm.a']



    g_file_groups = [
        'Integration',
        'Platform',
        'CRL',
        'OpenJADE',
        'XtensaLib',
        'Model']

    def __init__(self, path_to_filelist=None):
        # Get obj file list for OpenJADE Runtime, JFramework and Model if exists
        if path_to_filelist != None:
            rt_file = os.path.join(path_to_filelist, 'Runtime.lst')
            rt_list = []           
            if os.path.exists(rt_file):
                with open(rt_file, 'r') as f:
                    for x in f.read().split('\n'):
                        x = x[:-2]
                        if x not in self.g_OpenJADE:
                            rt_list.append(x)
                    self.g_OpenJADE += rt_list
            
            jf_file = os.path.join(path_to_filelist, 'Jframework.lst')
            if os.path.exists(jf_file):
                with open(jf_file, 'r') as f:
                    jf_list = []
                    for x in f.read().split('\n'):
                        x = x[:-2]
                        if x not in self.g_OpenJADE:
                            jf_list.append(x)
                    self.g_OpenJADE += jf_list
            
            ml_file = os.path.join(path_to_filelist, 'Model.lst')
            if os.path.exists(ml_file):
                with open(ml_file, 'r') as f:
                    ml_list = []
                    for x in f.read().split('\n'):
                        x = x[:-2]
                        if x not in self.g_Model:
                            ml_list.append(x)
                    self.g_Model += ml_list

            plt_file = os.path.join(path_to_filelist, 'Platform.lst')
            if os.path.exists(plt_file):
                with open(plt_file, 'r') as f:
                    plt_list = []
                    for x in f.read().split('\n'):
                        if x not in self.g_Platform:
                            plt_list.append(x)
                    self.g_Platform += plt_list


            crl_file = os.path.join(path_to_filelist, 'CRL.lst')
            if os.path.exists(crl_file):
                with open(crl_file, 'r') as f:
                    crl_list = []
                    for x in f.read().split('\n'):
                        if x not in self.g_CRL:
                            crl_list.append(x)
                    self.g_CRL += crl_list

           

    def groups(self):
        return self.g_file_groups

    def classify(self, input_section):
        category = None
        if ".o" in input_section.basepath:
            filename = input_section.basepath
        if len(input_section.input_groups):
                filename = input_section.input_groups[-1].file_name
        if filename in self.g_Platform:
            category = "Platform"            
        elif filename in self.g_Integration:
            category = "Integration"
        elif filename in self.g_OpenJADE:
            category = "OpenJADE"
        elif filename in self.g_CRL:
            category = "CRL"
        elif ".a" in input_section.basepath:
            filename = input_section.basepath
            if filename in self.g_Model:
                category = "Model"
            if filename in  self.g_xtensalibraries:
                category = "XtensaLib"

        if not category :
            print('Warning: file is not allocated: ' + filename)


        return category

