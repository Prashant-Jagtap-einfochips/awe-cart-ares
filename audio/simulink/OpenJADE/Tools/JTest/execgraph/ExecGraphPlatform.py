#*********************************************************************************
# This script is for parsing xml file and identify platform type
# Then choose JTestRTA or JTestHIL to create ExecGraphPlatform
#
#*********************************************************************************

import xml.etree.ElementTree as et
from xml.etree.ElementTree import XMLParser

from execgraph.JTestEGP import JTestRTAEGP
from execgraph.JTestEGP import JTestHILEGP
from execgraph.JTestEGP import JTestPILEGP
from execgraph.JTestEGP import VegaAlpineEGP
from execgraph.JTestEGP import SpetterEGP

class ExecGraphPlatform:
    def __init__(self, config_file):
        self.platform = None

        xml_tree = et.parse(config_file)
        xml_root = xml_tree.getroot()
        platform_node = xml_root.find('PLATFORM')
        if platform_node == None:
            print ('Error: PLATFORM node could not be found in test config xml ' + config_file)
            return

        platform_type = platform_node.get('type')
        if platform_type == 'JTestRTA':
            self.platform = JTestRTAEGP(config_file)
        elif platform_type == 'JTestHIL':
            self.platform = JTestHILEGP(config_file)
        elif platform_type == 'JTestPIL':
            self.platform = JTestPILEGP(config_file)
        elif platform_type == 'VegaAlpine':
            self.platform = VegaAlpineEGP(config_file)
        elif platform_type == 'Spetter':
            self.platform = SpetterEGP(config_file)
        else:
            print ('Error: unknown PLATFORM type found in test config xml ' + config_file)
            self.platform = None

        self.platform_type = platform_type

        self.platform_board = platform_node.get('board')

        if self.platform.isValid() == False:
            self.platform = None

    def isValid(self):
        return (self.platform != None)

    def get_platform_type(self):
        return self.platform_type

    def get_platform_board(self):
        return self.platform_board

    def get_frame_size(self):
        return self.platform.get_frame_size()

    def get_sample_rate(self):
        return self.platform.get_sample_rate()

    def get_clock_rate(self):
        return self.platform.get_clock_rate()

    def get_ExecLog_InitCmds(self):
        if self.platform != None:
            return self.platform.get_ExecLog_InitCmds()
        else:
            return None

    def get_ExecLog_PullCmds(self):
        if self.platform != None:
            return self.platform.get_ExecLog_PullCmds()
        else:
            return None

    def parse_ExecLog(self, execlog):
        if self.platform != None:
            return self.platform.parse_ExecLog(execlog)
        else:
            return None

    def is_prompt(self, buf):
        if self.platform != None:
            return self.platform.is_prompt(buf)
        else:
            return None

