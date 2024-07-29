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
# translation_engine Loads a translation engine into python.
#   Given the full path of the dll corresponding to the translation engine
#   this routine loads the dll. If successful, 
#   the resulting object can be used to access the translation engine.
# Note: The translation engine must be built with the same
# machine architecture as the python (i.e. 64 vs. 32 bit)
################################################################################
import sys
import warnings
import os
import glob
import ctypes
import struct
from collections import namedtuple
from types import SimpleNamespace

class payload_parser:
    '''
    payload_parser contains shared methods to pack/unpack binary payloads
    '''

    isLittleEndian = True

    def C99TypeToMatlabTypName(C99TypeName):
        if (C99TypeName == 'int32_t'):
            matlabTypeName = 'int32'
        elif (C99TypeName == 'uint32_t'):
            matlabTypeName = 'uint32'
        elif (C99TypeName == 'float'):
            matlabTypeName = 'single'
        elif (C99TypeName == 'double'):
            matlabTypeName = 'double'
        elif (C99TypeName == 'int8_t'):
            matlabTypeName = 'int8'
        elif (C99TypeName == 'uint8_t'):
            matlabTypeName = 'uint8'
        else:
            raise Exception('Unsupported data type: %s' % C99TypeName)
        
        return matlabTypeName

    def MatlabTypeToC99TypeName(MatlabTypeName):
        if (MatlabTypeName == 'int32'):
            C99TypeName = 'int32_t'
        elif (MatlabTypeName == 'uint32'):
            C99TypeName = 'uint32_t'
        elif (MatlabTypeName == 'single'):
            C99TypeName = 'float'
        elif (MatlabTypeName == 'double'):
            C99TypeName = 'double'
        elif (MatlabTypeName == 'int8'):
            C99TypeName = 'int8_t'
        elif (MatlabTypeName == 'uint8'):
            C99TypeName = 'uint8_t'
        else:
            raise Exception('Unsupported data type: %s' % MatlabTypeName)
        
        return C99TypeName

    def InitSpecStruct():
        '''Sets up specStruct with expected fields'''
        class SpecStruct():
            pass

        specStruct = SpecStruct()
        specStruct.charItr=1
        specStruct.binaryPayload=[]
        specStruct.featurePath=''
        specStruct.sigName=''
        specStruct.dataType=''
        specStruct.width=0

        return specStruct

    def GetDataTypeBytes(dataTypeName):
        '''
        GetDataTypeBytes returns size of datatype in bytes and and
        example zero
        Usage:
           dtBytes,aZero = payload_parser.GetDataTypeBytes(dataTypeName)
        '''
        if (dataTypeName == 'int32_t' or dataTypeName == 'int32'):
            numOfBytes = 4
            dataType = 'signedInteger'
        elif (dataTypeName == 'uint32_t' or dataTypeName == 'uint32'):
            numOfBytes = 4
            dataType = 'unsignedInteger'
        elif (dataTypeName == 'int16_t' or dataTypeName == 'int16'):
            numOfBytes = 2
            dataType = 'signedInteger'
        elif (dataTypeName == 'uint16_t' or dataTypeName == 'uint16'):
            numOfBytes = 2
            dataType = 'unsignedInteger'
        elif (dataTypeName == 'float' or dataTypeName == 'single'):
            numOfBytes = 4
            dataType = 'float'
        elif (dataTypeName == 'double' or dataTypeName == 'double'):
            numOfBytes = 8
            dataType = 'double'
        elif (dataTypeName == 'int8_t' or dataTypeName == 'int8'):
            numOfBytes = 1
            dataType = 'signedInteger'
        elif (dataTypeName == 'uint8_t' or dataTypeName == 'uint8'):
            numOfBytes = 1
            dataType = 'unsignedInteger'
        else:
            raise Exception('Unsupported data type: %s' % dataTypeName)
        
        return numOfBytes, dataType

    def GetNotificationCode():
        '''
        GetNoficiationCode returns the status code present in all
        notification payloads (e.g. "1").
        '''
        return ctypes.c_int32(0x00000001).value

    def GetStatusCodeStr(statusCode):
        '''
        GetStatusCodeStr Returns a string-version of several common
        status codes (from ASDID.h) given a numeric value.
        '''
        if (statusCode == 0):
            statusStr = 'OK'
        elif (statusCode == -1):
            statusStr = 'RSP_BUFFER_SMALL'
        elif (statusCode == -2):
            statusStr = 'REQ_BUFFER_SMALL'
        elif (statusCode == -1000):
            statusStr = 'TOP_BAD_GROUP_ID'
        elif (statusCode == -1001):
            statusStr = 'TOP_BAD_BANK_ID'
        elif (statusCode == -1002):
            statusStr = 'TOP_OUT_OF_RANGE'
        elif (statusCode == -2000):
            statusStr = 'RTC_UNKNOWN_SERVICE'
        elif (statusCode == -3000):
            statusStr = 'TSP_BAD_STATE_ID'
        elif (statusCode == -3001):
            statusStr = 'TSP_CAPTURE_OVERFLOW'
        elif (statusCode == -3002):
            statusStr = 'ASDSTATUS_TSP_CAPTURE_ALL_OFF'
        elif (statusCode == -3003):
            statusStr = 'ASDSTATUS_TSP_CAPTURE_THIS_OFF'
        elif (statusCode == -3004):
            statusStr = 'ASDSTATUS_TSP_WRITE_IMPOSSIBLE'
        elif (statusCode == -4000):
            statusStr = 'VALUE_NOT_AVAILABLE'
        elif (statusCode == -4001):
            statusStr = 'NO_COMPAT_CODES'
        elif (statusCode == 0x00000001):
            statusStr = 'RTC_NOTIFICATION'
        else:
            code = ctypes.c_uint32(statusCode).value
            statusStr = 'unknown code %d (%s)' % (statusCode, "0x{:08x}".format(code))
        return statusStr

    def UnpackPayloadElement(unpackStruct, specStruct):
        '''
        UnpackPayloadElement Adds to provided unpackStruct based on
        definitions and data in the provided specStruct. Both the
        structs must be passed in/out as they change with each call.
        Usage:
          [unpackStruct,specStruct] = payload_parser.UnpackPayloadElement(unpackStruct, specStruct)
        
        unpackStruct(*) is just a MATLAB struct
        specStruct is struct with the following member elements:
         .charItr(*) : starting at 1, tracks binary location
         .binaryPayload : the payload to unpack (uint8 vector)
         .featurePath : feature path name used for error message
         .sigName : name of the element to unpack (char/string)
         .dataType : MATLAB data type name (char/string)
         .width : count of provided dataType to unpack (numeric)
        (*) = this element is updated and must pass in/out        
        '''
        # Understand datatype's size in count of uint8s
        dtBytes, dataType = payload_parser.GetDataTypeBytes(specStruct.dataType)

        # Prepare a native value of correct size and type
        nativeValue = []

        # Fill in the native value, parsing from binary payload
        for j in range(specStruct.width):
            # Check that the data is present
            if (len(specStruct.binaryPayload) < specStruct.charItr+dtBytes-1):
                raise Exception('Supplied payload is too short. Ran out of data in %s %s element %d of %d' % (specStruct.featurePath,specStruct.sigName,j,specStruct.width))
            # Extract, cast, and move parsing iterator
            valueUint8s = specStruct.binaryPayload[specStruct.charItr-1:specStruct.charItr+dtBytes-1]
            byteArr = bytearray(valueUint8s)
            if payload_parser.isLittleEndian:
                if (dataType == 'float'):
                    nativeValue.append(struct.unpack('f', byteArr)[0])
                elif (dataType == 'double'):
                    nativeValue.append(struct.unpack('d', byteArr)[0])
                elif (dataType == 'signedInteger'):
                    nativeValue.append(int.from_bytes(byteArr, byteorder='little', signed=True))
                elif (dataType == 'unsignedInteger'):
                    nativeValue.append(int.from_bytes(byteArr, byteorder='little', signed=False))
            else:
                if (dataType == 'float'):
                    floatData = struct.unpack('>f', byteArr)
                    nativeValue.append(int(floatData[0]))
                elif (dataType == 'double'):
                    doubleData = struct.unpack('>d', byteArr)
                    nativeValue.append(int(doubleData[0]))
                elif (dataType == 'signedInteger'):
                    nativeValue.append(int.from_bytes(byteArr, byteorder='big', signed=True))
                elif (dataType == 'unsignedInteger'):
                    nativeValue.append(int.from_bytes(byteArr, byteorder='big', signed=False))
            specStruct.charItr = specStruct.charItr + dtBytes
        
        signalName = specStruct.sigName
        if len(nativeValue) > 1:
            setattr(unpackStruct, signalName, nativeValue)
        else:
            setattr(unpackStruct, signalName, nativeValue[0])

        return unpackStruct, specStruct
        
    def PackPayloadElement(srcStrArray, specStruct):
        '''
        PackPayloadElement Adds to binary payload based on
        definitions and data in the provided specStruct. Values are
        consumed and removed from provided srcStrArray which is
        returned.
        Usage:
          [srcStrArray,specStruct] = payload_parser.PackPayloadElement(srcStrArray, specStruct)
                
        srcStrArray string array of source values to consume
        specStruct is struct with the following member elements:
         .charItr(*) : starting at 1, tracks binary location
         .binaryPayload(*) : the payload being packed (uint8 vector)
         .featurePath : feature path name used for error message
         .sigName : name of the element to unpack (char/string)
         .dataType : MATLAB data type name (char/string)
         .width : count of provided dataType to unpack (numeric)
        (*) = this element is updated and must pass in/out
        '''
        # Understand datatype's size in count of uint8s
        dtBytes, dataType = payload_parser.GetDataTypeBytes(specStruct.dataType)

        # Track position in the srcStrArray
        wordItr = 1

        # Fill in the native value, consuming from srcStrArray
        for j in range(specStruct.width):
            # Check that the data is present
            if (len(srcStrArray) < wordItr):
                raise Exception('Missing value #%d in [%s] element [%s]' % (j, specStruct.featurePath, specStruct.sigName));

            # Grab, evaluate to numeric, cast to type, then uint8
            valueStr = srcStrArray[j]
            if '0x' in valueStr:
                if 'Integer' in dataType:
                    valueNum = int(valueStr,16)
                elif (dataType == 'float'):
                    if payload_parser.isLittleEndian:
                        valueNum = struct.unpack('f', bytes.fromhex(valueStr[2:]))[0]
                    else:
                        valueNum = struct.unpack('>f', bytes.fromhex(valueStr[2:]))[0]
                elif (dataType == 'double'):
                    if payload_parser.isLittleEndian:
                        valueNum = struct.unpack('d', bytes.fromhex(valueStr[2:]))[0]
                    else:
                        valueNum = struct.unpack('>d', bytes.fromhex(valueStr[2:]))[0]
            elif '.' in valueStr:
                valueNum = float(valueStr)
            else:
                valueNum = int(valueStr)
            
            if payload_parser.isLittleEndian:
                if (dataType == 'float'):
                    valueUint8s = list(struct.pack('f', valueNum))
                elif (dataType == 'double'):
                    valueUint8s = list(struct.pack('d', valueNum))
                elif (dataType == 'unsignedInteger'):
                    valueUint8s = list(valueNum.to_bytes(dtBytes, byteorder='little', signed=False))
                elif (dataType == 'signedInteger'):
                    valueUint8s = list(valueNum.to_bytes(dtBytes, byteorder='little', signed=True))
            else:
                if (dataType == 'float'):
                    valueUint8s = list(struct.pack('>f', valueNum))
                elif (dataType == 'double'):
                    valueUint8s = list(struct.pack('>d', valueNum))
                elif (dataType == 'unsignedInteger'):
                    valueUint8s = list(valueNum.to_bytes(dtBytes, byteorder='big', signed=False))
                elif (dataType == 'signedInteger'):
                    valueUint8s = list(valueNum.to_bytes(dtBytes, byteorder='big', signed=True))

            wordItr = wordItr + 1

            # Pack it
            specStruct.binaryPayload = specStruct.binaryPayload + valueUint8s
            specStruct.charItr = specStruct.charItr + dtBytes

        # Consume the words from the srcStrArray
        srcStrArray.pop(0)

        return srcStrArray, specStruct
        