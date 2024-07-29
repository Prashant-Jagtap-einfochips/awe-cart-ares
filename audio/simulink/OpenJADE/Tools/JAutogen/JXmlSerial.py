import sys
import os


from JLogPrint import *
import xml.etree.ElementTree as ElementTree


class JXmlSerial:
    rtcname = '_RTC_Payloads.xml'
    topname = '_TOP_Tokens.xml'
    def __init__(self, classname, rtc=None, top=None):

        self.rtcfilename = rtc if rtc else classname + JXmlSerial.rtcname
        self.topfilename = top if top else classname + JXmlSerial.topname
        self.rtcDict = {}
        self.topDict = {}
        self.rtcList = []
        self.topList = []

    def Jxs_RtcParseAll(self, bamname, featurenameList):
        # bamname format: BAMNAME_RTCCOMMAND
        bamsignal = {}
        try:
            tree = ElementTree.parse(self.rtcfilename)
            root = tree.getroot()
            # Extract keyword from bamname
            bamstr = bamname[:bamname.find('_')] + bamname[bamname.find('_') + 1:]
            for rtc in root.findall('REQ'):
                signals = ''
                id = rtc.get('id')
                #if (id.upper().endswith(bamstr)):
                feastr = id[:-len(bamstr)] if id.upper().endswith(bamstr) else None
                if feastr != None and feastr in featurenameList:
                    mflag = False
                    cflag = False
                    # BAM command is found
                    nsize = ''
                    for signal in rtc.findall('Signal'):
                        if not ((signal.get('id') == 'msgId') or (signal.get('id') == 'caller_handle')):
                            if (signal.get('width') == '1'):
                                signals += signal.get('dtypeStr') + ' ' + signal.get('id')
                            else:
                                signals += signal.get('dtypeStr') + '* ' + signal.get('id') + ', ' + 'uint32_t size' + str(nsize)
                                if nsize == '' : nsize = 0
                                nsize += 1
                            signals += ', '
                        mflag = True if signal.get('id') == 'msgId' else mflag
                        cflag = True if signal.get('id') == 'caller_handle' else cflag
                    if mflag == False:
                        #print('ERROR in RTC command: msgId is missed >>>' + id)
                        Jlog_print(JAG_ERROR, 'In RTC command: msgId is missed >>>' + id)
                    if cflag == False:
                        #print('ERROR in RTC command: caller_handle is missed >>>' + id)
                        Jlog_print(JAG_WARNING, 'In RTC command: caller_handle is missed >>>' + id)
                    # Specify the existence of caller_handle for later use
                    bamsignal[signals] = 1 if cflag == True else 0
        except IOError:
            #print('ERROR: JXmlSerial: XML file does not exist' + '  ' + self.rtcfilename)
            Jlog_print(JAG_ERROR, 'ERROR: JXmlSerial: XML file does not exist' + '  ' + self.rtcfilename)

        return bamsignal

    def Jxs_RtcParseAllEx(self, id_keyword=None):
        # bamname format: BAMNAME_RTCCOMMAND
        bamsignal = None
        try:
            tree = ElementTree.parse(self.rtcfilename)
            root = tree.getroot()
            for rtc in root.findall('REQ'):
                signals = ''
                id = rtc.get('id')
                if (id_keyword in id):
                    mflag = False
                    cflag = False
                    # BAM command is found
                    nsize = ''
                    for signal in rtc.findall('Signal'):
                        if not ((signal.get('id') == 'msgId') or (signal.get('id') == 'caller_handle')):
                            if (signal.get('width') == '1'):
                                signals += signal.get('dtypeStr') + ' ' + signal.get('id')
                            else:
                                signals += signal.get('dtypeStr') + '* ' + signal.get('id') + ', ' + 'uint32_t size' + str(nsize)
                                if nsize == '' : nsize = 0
                                nsize += 1
                            signals += ', '
                        mflag = True if signal.get('id') == 'msgId' else mflag
                        cflag = True if signal.get('id') == 'caller_handle' else cflag
                    if mflag == False:
                        #print('ERROR in RTC command: msgId is missed >>>' + id)
                        Jlog_print(JAG_ERROR, 'In RTC command: msgId is missed >>>' + id)
                    if cflag == False:
                        #print('ERROR in RTC command: caller_handle is missed >>>' + id)
                        Jlog_print(JAG_ERROR, 'In RTC command: caller_handle is missed >>>' + id)
                    #bamsignal[signals] = 1
                    bamsignal = signals
            if bamsignal is None:
                #print('ERROR: JXmlSerial: RTC ID: ' + id_keyword + ' cannot be found.')
                Jlog_print(JAG_ERROR, 'JXmlSerial: RTC ID: ' + id_keyword + ' cannot be found.')
        except IOError:
            #print('ERROR: JXmlSerial: XML file does not exist' + '  ' + self.rtcfilename)
            Jlog_print(JAG_ERROR, 'JXmlSerial: XML file does not exist' + '  ' + self.rtcfilename)

        return bamsignal

    def Jxs_TopParseAll(self, bamname):
        # This function mainly used to detect the maximum 'width' for each TOP command
        # Since this 'width' is different for feature with same BAM TOP command
        # It should be implemented in Feature Level
        # Not used currently
        bamsignal = {}
        try: 
            tree = ElementTree.parse(self.topfilename)
            root = tree.getroot()
            # Extract keyword from bamname
            bamstr = bamname[:bamname.find('_')] + bamname[bamname.find('_') + 1:]
        except IOError:
            #print('ERROR: JXmlSerial: XML file does not exist')
            Jlog_print(JAG_ERROR, 'JXmlSerial: XML file does not exist')

        return bamsignal
    
