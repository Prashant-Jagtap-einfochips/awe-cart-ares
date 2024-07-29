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
        self.index = signal.get('charOffset')
        self.name = signal.get('id')
        self.type = signal.get('dtypeStr')
        self.width = signal.get('width')
        
    def __str__(self):
        return "<Signal Index:%s Name:%s Type:%s Width:%s>" % (self.index, self.name, self.type, self.width)
        
    def __repr__(self):
        return self.__str__()

class rtc_REQ:
    def __init__(self, req):
        self.signals = []
        for signal in req.findall('Signal'):
            self.signals.append(rtc_signal(signal))
        self.signals.sort(key=lambda x: int(x.index))
    
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
        self.signals.sort(key=lambda x: int(x.index))
    
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
        self.signals.sort(key=lambda x: int(x.index))
    
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
    def __init__(self, rtc, root):
        self.id = rtc.get('id')
        token = rtc.get('msgId') 
        token = token.upper()
        if (token.startswith('0X')) and (len(token) == 10):
            self.token = token[2:]
        else:
            return None
        
        self.req = None
        self.rsp = None
        self.ntf = None
        
        rtc_reqs = root.findall('REQ')
        if len(rtc_reqs) > 0:
            rtc_req = None
            for req in rtc_reqs:
                if req.get('id') == self.id:
                    rtc_req = req
            if rtc_req != None:
                self.req = rtc_REQ(rtc_req)
            
        rtc_rsps = root.findall('RSP')
        if len(rtc_rsps) > 0:
            rtc_rsp = None
            for rsp in rtc_rsps:
                if rsp.get('id') == self.id:
                    rtc_rsp = rsp
            if rtc_rsp != None:
                self.rsp = rtc_RSP(rtc_rsp)
            
        rtc_ntfs = root.findall('NTF') 
        if len(rtc_ntfs) > 0:
            rtc_ntf = None
            for ntf in rtc_ntfs:
                if ntf.get('id') == self.id:
                    rtc_ntf = ntf
            if rtc_ntf != None:
                self.ntf = rtc_NTF(rtc_ntf)
    
    def get_token(self):
        return self.token
        
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
        mystr = 'RTC: ' + self.id + ' Token: ' + self.token + '\n'
        if self.req != None:
            mystr = mystr + self.req.__str__()
        if self.rsp != None:
            mystr = mystr + self.rsp.__str__()
        if self.ntf != None:
            mystr = mystr + self.ntf.__str__()
        return mystr
        
    def __repr__(self):
        return self.__str__()
        
class rtc_payloads(object):
    '''
    Python class storing RTC data from a model
    '''
    def __init__(self, rtc_payloads_xml_filename):
        ''' Initialize data
        '''
        self.rtcs = {}
        self._filename = rtc_payloads_xml_filename
        self._parse_xml_file()
        
    def _parse_xml_file(self):
        ''' Parses xml file
        '''
        tree = ElementTree.parse(self._filename)
        root = tree.getroot()
        for rtc in root.findall('RTC'):
            name = rtc.get('id')
            self.rtcs[name] = rtc_RTC(rtc, root)
            
    def print_rtc_info(self):
        for name,token in self.rtcs.items():
            print( token )
                
    def get_rtc_ids(self):
        return self.rtcs.keys()
        
    def get_rtc_id(self, token):
        token = token.upper()
        for rtc_id in self.get_rtc_ids():
            if token == self.get_rtc_token(rtc_id):
                return rtc_id
        return None
        
    def get_rtc_token(self, rtc_id):
        if rtc_id in self.rtcs:
            return self.rtcs[rtc_id].get_token()
        else:
            return None

    def get_req_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_req_signals()
    
    def get_rsp_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_rsp_signals()
    
    def get_ntf_signals(self, rtc_id):
        return self.rtcs[rtc_id].get_ntf_signals()
        
