#*********************************************************************************
# This script is classifier for file group
#
#*********************************************************************************

import re

class BSFileGroupClassifier:
    g_file_groups = [
        'algorithm',
        'OpenJADE',
        'BIPC',
        'OS',
        'C runtime',
        'platform']

    def groups(self):
        return self.g_file_groups

    def classify(self, input_file):
        if re.search('Model_core\d\.dlb', input_file) != None:
            return 'algorithm'
        elif re.search('OpenJadeRTE\.dlb|JModule_core\d\.dlb', input_file) != None:
            return 'OpenJADE'
        elif re.search('DSP_BIPC\.dlb|DSP_BIPC\w+\.doj|mcapi\w+\.doj', input_file) != None:
            return 'BIPC'
        elif re.search('os\w+\.doj|libosal_ucos3\.dlb|adi_os\w+\.doj', input_file) != None:
            return 'OS'
        elif re.search('libcmt\.dlb|libiomt\.dlb|libcc\.dlb|libssl\.dlb|libdrv\.dlb', input_file) != None:
            return 'C runtime'
        elif re.search('.*', input_file) != None:
            return 'platform'
        return None 