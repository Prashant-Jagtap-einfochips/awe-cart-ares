#!/usr/bin/env python
###############################################################################
#
# BOSE CORPORATION
# COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
# This program may not be reproduced, in whole or in part in any
# form or any means whatsoever without the written permission of:
# BOSE CORPORATION
# The Mountain
# Framingham, MA 01701-9168
#
################################################################################

import xml.etree.ElementTree as ElementTree

class rtc_signal:
    def __init__(self, signal):
        self.name = signal.find('Name').text
        self.type = signal.find('Type').text
        self.width = signal.find('Width').text
        self.totalBitSize = signal.find('TotalBitSize').text
        
    def __str__(self):
        return "<Signal Name:%s Type:%s Width:%s TotalBitSize:%s>" % (self.name, self.type, self.width, self.totalBitSize)
        
    def __repr__(self):
        return self.__str__()

class rtc_REQ:
    def __init__(self, req):
        self.signals = []
        for signal in req.findall('Signal'):
            self.signals.append(rtc_signal(signal))
    
    def __str__(self):
        mystr = '\tREQ\n'
        for signal in self.signals:
            mystr = mystr + '\t\t' + signal.__str__() + '\n'
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
    def get_signals(self):
        return self.signals
        
class rtc_RSP:
    def __init__(self, rsp):
        self.signals = []
        for signal in rsp.findall('Signal'):
            self.signals.append(rtc_signal(signal))
    
    def __str__(self):
        mystr = '\tRSP\n'
        for signal in self.signals:
            mystr = mystr + '\t\t' + signal.__str__() + '\n'
        return mystr
        
    def __repr__(self):
        return self.__str__()

    def get_signals(self):
        return self.signals
        
class rtc_NTF:
    def __init__(self, ntf):
        self.signals = []
        for signal in ntf.findall('Signal'):
            self.signals.append(rtc_signal(signal))
    
    def __str__(self):
        mystr = '\tNTF\n'
        for signal in self.signals:
            mystr = mystr + '\t\t' + signal.__str__() + '\n'
        return mystr
        
    def __repr__(self):
        return self.__str__()

    def get_signals(self):
        return self.signals
        
class rtc_RTC:
    def __init__(self, rtc):
        self.fullname = rtc.get('fullname')
        self.id = rtc.get('id')
        
        rtc_req = rtc.find('REQ')
        if rtc_req == None:
            self.req = None
        else:
            self.req = rtc_REQ(rtc_req)
            
        rtc_rsp = rtc.find('RSP')
        if rtc_rsp == None:
            self.rsp = None
        else:
            self.rsp = rtc_RSP(rtc_rsp)
            
        rtc_ntf = rtc.find('NTF')
        if rtc_ntf == None:
            self.ntf = None
        else:
            self.ntf = rtc_NTF(rtc_ntf)
        
    def get_req_signals(self):
        if self.req == None:
            return None
        else:
            return self.req.get_signals()
    
    def get_rsp_signals(self):
        if self.rsp == None:
            return None
        else:
            return self.rsp.get_signals()
    
    def get_ntf_signals(self):
        if self.ntf == None:
            return None
        else:
            return self.ntf.get_signals()
    
    def __str__(self):
        mystr = 'RTC: ' + self.id + '\n'
        if self.req != None:
            mystr = mystr + self.req.__str__()
        if self.rsp != None:
            mystr = mystr + self.rsp.__str__()
        if self.ntf != None:
            mystr = mystr + self.ntf.__str__()
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
class rtc_info(object):
    '''
    Python class storing RTC data from a model
    '''
    def __init__(self, rtc_info_xml_filename):
        ''' Initialize data
        '''
        self.rtcs = {}
        self._filename = rtc_info_xml_filename
        self._parse_xml_file()
        
    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for rtc in root.findall('RTC'):
            name = rtc.get('id')
            self.rtcs[name] = rtc_RTC(rtc)
            
    def print_rtc_info(self):
        for name,token in self.rtcs.items():
            print token
                
    def get_rtc_ids(self):
        return self.rtcs.keys()
        
    def get_req_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_req_signals()
    
    def get_rsp_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_rsp_signals()
    
    def get_ntf_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_ntf_signals()
        
