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
# interpretation_engine Loads an interpretation engine into python.
#   Given the full path of the dll corresponding to the interpretation engine
#   this routine loads the dll. If successful, 
#   the resulting object can be used to access the interpretation engine.
# Note: The interpretation engine must be built with the same
# machine architecture as the python (i.e. 64 vs. 32 bit)
################################################################################
import sys
import warnings
import ctypes
from ctypes import *

def byte_to_str(bytesThing):
    return bytesThing.decode(encoding='UTF-8')

def str_to_byte(stringThing):
    if sys.version_info < (3, 0):
        return bytes(stringThing)
    else:
        return bytes(stringThing, 'utf8')

def int32_to_uint32(i):
    return ctypes.c_uint32(i).value

class interpretation_engine:
    '''
    Python class storing Interpretation Engine from a model
    '''
    def __init__(self, interpretation_engine_dll):
        ''' Initialize data
        '''
        self._filename = interpretation_engine_dll
        try:
            self._mydll = cdll.LoadLibrary(self._filename)
        except:
            raise Exception('Error: Unable to load dll ' + self._filename)
        self._mydll.InitializeInterpretationEngine()
        
    def List(self):
        ''' List Returns a string array of recognized StateVars
        '''
        stateVars = []
        self._mydll.FirstStateVar.restype = c_char_p
        aName = self._mydll.FirstStateVar()
        if (aName != None):
            stateVars.append(byte_to_str(aName))
        self._mydll.NextStateVar.restype = c_char_p
        aName = self._mydll.NextStateVar()
        while aName != None:
            stateVars.append(byte_to_str(aName))
            self._mydll.NextStateVar.restype = c_char_p
            aName = self._mydll.NextStateVar()
        return stateVars
    
    def Ask(self,stateVarName):
        ''' Ask Returns TSPs needed to calculate StateVar
        '''
        self._mydll.GetTspsForStateVar.restype = c_char_p
        tsps = self._mydll.GetTspsForStateVar(str_to_byte(stateVarName))
        if byte_to_str(tsps) == '':
            raise Exception( 'TIE:Ask:error ' + 'Unrecognized StateVar [%s]' % stateVarName)
        return byte_to_str(tsps)
    
    def Set(self,tspName,tspValues):
        ''' Set Sets a TSP with provided space-delimited values
            Arguments: Set(self, tspName, tspValues)
            Returns 0 for OK, 1 for unrecognized TSP, 2 for too many elements
        '''
        retVal = self._mydll.SetTsp(str_to_byte(tspName), str_to_byte(tspValues))
        strings = self._mydll.GetTSPErrorStrings()
        if retVal == 1:
            raise Exception('TIE:Set:error ' + byte_to_str(c_char_p(strings).value ))
        if retVal == 2:
            warnings.warn('TIE:Set:warning ' + byte_to_str(c_char_p(strings).value ))
        return retVal
    
    def Get(self):
        ''' Get Returns the requested StateVar based on provided TSPs
        '''
        self.ClearInterpretationErrorState()
        self._mydll.CalculateStateVar.restype = c_char_p
        stateVar = self._mydll.CalculateStateVar()
        self.EmitErrors('Get')
        return byte_to_str(stateVar)
    
    def GetInterpretationErrorState(self):
        ''' GetInterpretationErrorState Returns the current error state
            for the engine. A value of "0" implies no errors.
        '''
        errorState = self._mydll.GetInterpretationErrorState()
        return errorState

    def GetErrorStrings(self):
        ''' GetErrorStrings Returns the current error strings
            for the engine.
        '''
        self._mydll.GetInterpretationErrorStrings.restype = c_char_p
        errors = self._mydll.GetInterpretationErrorStrings()
        return byte_to_str(errors)

    def ClearInterpretationErrorState(self):
        ''' ClearTranslationErrorState Resets the engine's error state
            back to "0" (no errors).
        '''
        self._mydll.ClearInterpretationErrorState()
        
    def GetTspCompat(self,overlayGroupNameHash):
        ''' GetTspCompatByIndex Returns the compatibility hash code for the TSPs
            represented within this interpretation engine. Provide
            a zero-based overlay group index number.
        '''
        compatibilityHash = self._mydll.GetTspCompat(overlayGroupNameHash)
        return '%8.8x' % int32_to_uint32(compatibilityHash)

    def GetTspCompatByName(self,overlayGroupName):
        ''' GetTspCompatByName Returns the compatibility hash code for the TSPs
            represented within this interpretation engine. Provide
            the exact name of the overlay group.
        '''
        compatibilityHash = self._mydll.GetTspCompatByName(str_to_byte(overlayGroupName))
        return '%8.8x' % int32_to_uint32(compatibilityHash)
        
    def Reset(self):
        ''' Reset Clears the engine's error state and all other states
            such as the tracking of which StateVars have been seen, and
            the flags which indicate that a interpretation is out-of-date
            and needs to be run.
        '''
        self._mydll.ClearAllStateRequests()
        self._mydll.ClearAllInterpretationRequiredFlags()
        self.ClearInterpretationErrorState()

    def EmitErrors(self,functionName):
        if (self.GetInterpretationErrorState()):
            msgStrings = self.GetErrorStrings();
            raise Exception('TIE:%s:error %s' % (functionName, msgStrings))
