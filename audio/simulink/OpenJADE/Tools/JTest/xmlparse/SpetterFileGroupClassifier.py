#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import re


# This is copied from BSFileGroupClassifier.py, so copied from BSFileGroupClassifier.
class SpetterFileGroupClassifier:
    g_file_groups = [
        'Model',
        'OpenJADE',
        'BIPC',
        'OS',
        'C runtime',
        'platform']

    def __init__(self):
        # Define the match and enumerate files for Model
        self.model_file = ['rtGetInf.doj', 'rtGetNaN.doj', 'rt_nonfinite.doj', 'rtmodel.doj','OutputRouter.doj']
        self.model_match = 'Model_\w+\.doj'
        self.modelRule = self.model_match + '|' + '|'.join(self.model_file) if bool(self.model_file) else self.model_match
        # Define the match and enumerate files for OpenJADE
        self.oj_file = ['Audio_Graph.doj', 'BAF.doj', 'Connection_List.doj', 'Execution_List.doj',
                        'Framework_Builder.doj' 'Heap.doj', 'JFramework.doj', 'Module.doj', 'Process.doj',
                        'Module_List.doj', 'BAF_Lite.doj', 'Port_List.doj', 'Framework_Builder.doj',
                        'Port.doj', 'Ports.doj', 'Process_Context.doj', 'Logger.doj']
        self.oj_match = 'JModule_\w+\.doj'
        self.ojRule = self.oj_match + '|' + '|'.join(self.oj_file) if bool(self.oj_file) else self.oj_match
        # Define the match and enumerate files for BIPC
        self.bipc_file = ['Endpoint.doj', 'InterDomainEndpoint.doj', 'RemoteICC.doj', 'msg_mcapi_core_comm.doj']
        self.bipc_match = 'DSP_BIPC_\w+\.doj|BIPC_\w+\.doj|mcapi\w+\.doj'
        self.bipcRule = self.bipc_match + '|' + '|'.join(self.bipc_file) if bool(self.bipc_file) else self.bipc_match
        # Define the match and enumerate files for UCOS
        self.os_file = ['adi_osal_arch_int_table.doj', 'adi_osal_arch_c.doj', 'asd_critical_section_ucos.doj']
        self.os_match = 'os_\w+\.doj|adi_osal_ucos\w+\.doj'
        self.osRule = self.os_match + '|' + '|'.join(self.os_file) if bool(self.os_file) else self.os_match
        # Define the match and enumerate files for C Runtime
        self.rtRule = 'libcc.dlb|libcmt.dlb'


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
        elif re.match(self.rtRule, libname) != None:
            return 'C runtime'
        elif re.search('.*', filename) != None:
            return 'platform'
        else:
            print('Warning: file is not allocated: ' + input_file)
        return None 