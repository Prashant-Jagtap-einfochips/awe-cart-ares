#!/usr/bin/env python
###############################################################################
#
# BOSE CORPORATION
# COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
# self program may not be reproduced, in whole or in part in any
# form or any means whatsoever without the written permission of:
# BOSE CORPORATION
# The Mountain
# Framingham, MA 01701-9168
#
# translation_engine Loads a translation engine into python.
#   Given the full path of the dll corresponding to the translation engine
#   self routine loads the dll. If successful, 
#   the resulting object can be used to access the translation engine.
# Note: The translation engine must be built with the same
# machine architecture as the python (i.e. 64 vs. 32 bit)
################################################################################
import sys
import warnings
import os
import xml.etree.ElementTree as ET
from collections import namedtuple
from payload_parser import payload_parser

class rtc_parser:
    '''
    rtc_parser Loads the generated RTC_Payloads.xml file and enables
    conversion between binary-packed payloads and string representations of
    RTC payloads
    '''
    def __init__(self, RtcPayloadsXmlFilePath, isLittleEndian=True):
        '''
        rtc_parser Constructs an RTC parser object.
        Given a RTC_Payloads.XML path, the parser describes the payloads.
        '''
        
        if (False == os.path.isfile(RtcPayloadsXmlFilePath)):
            raise Exception('Could not find RTC XML file %s' % RtcPayloadsXmlFilePath)
        else:
            self.XmlFilePath = RtcPayloadsXmlFilePath

        self.isLittleEndian = isLittleEndian
        payload_parser.isLittleEndian = isLittleEndian
        tree = ET.parse(self.XmlFilePath)

        self.MapFPathToToken = dict()
        self.MapTokenToFPath = dict()
        self.ReqMap = dict()
        self.RspMap = dict()
        self.NtfMap = dict()

        class MapStruct():
            pass

        for elem in tree.iter():
            if elem.tag == 'RTC':
                attribute = elem.attrib
                featurePath = attribute.get('id')
                token = attribute.get('msgId')
                mapStruct = MapStruct()
                mapStruct.token = token
                for element in elem:
                    if element.tag == 'PayloadSizes':
                        elemAttr = element.attrib
                        mapStruct.reqSize = int(elemAttr.get('REQ'))
                        mapStruct.rspSize = int(elemAttr.get('RSP'))
                        mapStruct.ntfSize = int(elemAttr.get('NTF'))
                self.MapFPathToToken[featurePath] = mapStruct
                self.MapTokenToFPath[token] = featurePath
            elif elem.tag == 'REQ':
                attribute = elem.attrib
                featurePath = attribute.get('id')
                signals = []
                for element in elem:
                    if element.tag == 'Signal':
                        signals.append(element.attrib)
                self.ReqMap[featurePath] = signals
            elif elem.tag == 'RSP':
                attribute = elem.attrib
                featurePath = attribute.get('id')
                signals = []
                for element in elem:
                    if element.tag == 'Signal':
                        signals.append(element.attrib)
                self.RspMap[featurePath] = signals
            elif elem.tag == 'NTF':
                attribute = elem.attrib
                featurePath = attribute.get('id')
                signals = []
                for element in elem:
                    if element.tag == 'Signal':
                        signals.append(element.attrib)
                self.NtfMap[featurePath] = signals

    def PackRequest(self, requestString):
        '''
        PackRequest Returns a packed binary RTC payload given a
        string expression of the request.
        Usage:
           binaryReqPayload = obj.PackRequest(requestString)
        Example:
           req = obj.PackRequest('RumbleGainSet 1234 120 0 2000 0')
        '''

        # Break up the request string into words, recognize the first
        # one as a feature path name, and use map to retrieve the token
        srcStrArray = requestString.split()
        featurePath = srcStrArray[0]
        mapStruct = self.MapFPathToToken[featurePath]

        # Use a payload_parser specStruct to help pack the request
        specStruct = payload_parser.InitSpecStruct()
        specStruct.featurePath = featurePath
        specStruct.binaryPayload = []

        # Replace the featurepath name with token
        srcStrArray[0] = mapStruct.token

        # Get all the request signals
        reqSigs = self.ReqMap[featurePath]
        
        # Pack all the signals into the payload
        for i in range(len(reqSigs)):
            aSig = reqSigs[i]
            specStruct.sigName = str(aSig.get('id'))
            specStruct.dataType = payload_parser.C99TypeToMatlabTypName(str(aSig.get('dtypeStr')))
            specStruct.width = int(aSig.get('width'))
            srcStrArray, specStruct = payload_parser.PackPayloadElement(srcStrArray,specStruct)

        # Return the packed binary payload
        return specStruct.binaryPayload
    
    def UnpackResponse(self, binaryPayload):
        '''
        UnpackResponse Returns a MATLAB struct representing the
        supplied binary payload interpretted as an RTC response.
        Usage:
           theStruct = UnpackResponse(self, binaryPayload)
        '''
        theStruct = self.UnpackPayload(binaryPayload,self.RspMap)
        theStruct.CalcPayloadType = 'RTC Response'
        return theStruct
        
    def UnpackRequest(self, binaryPayload):
        '''
        UnpackRequest Returns a MATLAB struct representing the
        supplied binary payload interpretted as an RTC request.
        Usage:
           theStruct = UnpackRequest(self, binaryPayload)
        '''
        theStruct = self.UnpackPayload(binaryPayload,self.ReqMap)
        theStruct.CalcPayloadType = 'RTC Request'
        return theStruct
            
    def UnpackNotification(self, binaryPayload):
        '''
        UnpackNotification Returns a MATLAB struct representing the
        supplied binary payload interpretted as an RTC notification.
        Usage:
           theStruct = UnpackNotification(self, binaryPayload)
        '''
        theStruct = self.UnpackPayload(binaryPayload,self.NtfMap)
        theStruct.CalcPayloadType = 'RTC Notification'
        return theStruct
    
    def UnpackPayload(self, binaryPayload, payloadSpec):
        '''
        UnpackPayload Returns a MATLAB struct given a  specification
        from one of the the payload maps.
        Usage:
          theStruct = obj.UnpackPayload(binaryRspPayload, payloadSpec)
        '''
        
        # Retrieve the token from the payload and lookup feature path
        byteArr = bytearray(binaryPayload[0:4])
        if self.isLittleEndian:
            token = int.from_bytes(byteArr, byteorder='little', signed=False)
        else:
            token = int.from_bytes(byteArr, byteorder='big', signed=False)
        tokenStr = "0x{:08x}".format(token)
        featurePath = self.MapTokenToFPath[tokenStr];

        class Struct():
            pass
        theStruct = Struct()
        theStruct.CalcFeaturePath = featurePath;
        theStruct.CalcToken = tokenStr;

        # Use a payload_parser specStruct to help unpack the request
        specStruct = payload_parser.InitSpecStruct();
        specStruct.featurePath = featurePath;
        specStruct.binaryPayload = binaryPayload;
        
        # Pull out all signals using XML doc as payload specification
        theSigs = payloadSpec[featurePath];
        for i in range(len(theSigs)):
            aSig = theSigs[i];
            specStruct.sigName = str(aSig.get('id'));
            specStruct.width = int(aSig.get('width'));
            specStruct.dataType = payload_parser.C99TypeToMatlabTypName(str(aSig.get('dtypeStr')));
            [theStruct,specStruct] = payload_parser.UnpackPayloadElement(theStruct,specStruct);
            if ("status_code" == specStruct.sigName):
                theStruct.CalcStatusCodeStr = payload_parser.GetStatusCodeStr(theStruct.status_code);
                if (theStruct.status_code < 0):
                    # The payload represents an error condition! In
                    # that case, the payload should end here
                    if (len(specStruct.binaryPayload) < specStruct.charItr):
                        return;
        return theStruct
