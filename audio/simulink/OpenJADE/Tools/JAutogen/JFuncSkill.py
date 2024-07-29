import os
import sys
import glob
import re

from JAConv import *
from JALogPrint import *


##############################################################################
# class Name:  FuncNode
# Description: Describe the C function prototype, including function name,
#              return type, argument list, C file of the function and the
#              location of funtion begin and end in C file.
##############################################################################
class FuncNode:
    ##########################################################################
    # function name: __init__
    # Description:   Constructor of FuncNode.
    # Argument:      funcName:   function name
    #                funcReturn: return type
    #                argList:    List of arguments (option)
    #                cfile:      C file name
    # Return:        None
    ##########################################################################
    def __init__(self, funcName = None, funcReturn=None, argList=[], cfile=None):
        self.fname = funcName
        self.freturn = funcReturn
        self.fargList = argList
        self.fcfile = cfile
        self.oline = 0
        self.bline = 0
        self.eline = 0
        self.incall = []

    ##########################################################################
    # function name: updateFuncRangeInFile
    # Description:   Record the begin/end offset of function in C file
    # Argument:      originLIne: line of function definition
    #                beginLine:  offset of begin boundary '{'
    #                endLine:    offset of end boundary '}'
    # Return:        None
    ##########################################################################
    def updateFuncRangeInFile(self, originLine, beginLine, endLine):
        self.oline = originLine
        self.bline = beginLine
        self.eline = endLine


##############################################################################
# class Name:  JFuncSkill
# Description: Function parser for C file to extract all information of functions, 
#              including function prototype, location, calling hierarchy, etc.
##############################################################################
class JFuncSkill:
    RETURN_THRESHOLD = 5
    MODEL_INITIALIZE = 'initialize'
    FUNCTION_MULTILINE = 20
    RULEOUT_KEYWORDS = ['inline', 'for', 'while', 'if', 'else', 'switch', 'case']
    TYPE_KEYWORDS = ['unsigned', 'struct', 'const']
    C_KEYWORDS = ['static']
    RULEOUT_FUNCTIONS = ['sizeof', 'memset', 'memcpy', 'aligned', 'UNUSED_PARAMETER', 'powf', 'sqrtf', 'circptr', 'logf',
                         'log10f', 'expf', 'roundf', 'fabsf', 'fminf', 'fmaxf', 'circindex', 'fmodf', 'floorf']
    ##########################################################################
    # function name: __init__
    # Description:   Constructor of JFuncSkill.
    # Argument:      modelname:  Simulink Model name in generated C code, like Model_1_1.
    #                modelpath:  The path including generated C code for each model. For
    #                            platform, it is None.
    # Return:        None
    ##########################################################################
    def __init__(self, modelname=None, modelpath=None):
        # Function Dictionary Format: {fileName#functionName: FuncNode, ...}
        self.modelname = modelname
        if modelpath != None:
            self.modelpath = modelpath if modelpath[-1] == '\\' else modelpath + '\\'
            self.cfileList = [os.path.basename(x) for x in glob.glob(self.modelpath + '*.c')] if os.path.exists(self.modelpath) else []
        else:
            self.cfileList = None
        # Format: {c_file1 : [func1, func2, ...], c_file2 : [func1, func2, ...], ...}
        self.filefuncDict = {}
        # Format: {func1: {subfunc1: num_of_call, subfunc2: num_of_call}, ...], func2:[{subfunc1: num_of_call}, ...]}
        self.funcPCDict = {}
        self.conv = JMatlabConv()

    ##########################################################################
    # function name: EnumerateFuncbyFileName
    # Description:   Enumerate all functions in C files and create function
    #                Dictionary to hold info of function prototype
    # Argument:      ssName:    C File name. If it equals '*', it means function 
    #                           prototype from all C files  
    # Return:        True:      success
    #                False:     function cannot be found in specified C file
    ########################################################################## 
    def EnumerateFuncbyFileName(self, ssName=None):
        self.funcDict = {}
        tmpfList =[]
        if ssName != '*' and ssName != '*.*':
            for f in self.cfileList:
                if self.conv.funcname_subsystem(self.modelname, ssName) == f[:-2] or ssName == f:
                    tmpfList.append(f)
        else:
            tmpfList = self.cfileList
        for f in tmpfList:
            JAlog_print(JAG_DEBUG, 'C File name enumerated: ' + str(f))
            doffset = -1
            if f not in self.filefuncDict.keys():
                self.filefuncDict[f] = []
                doffset = len(self.filefuncDict) - 1
            with open(os.path.join(self.modelpath, f), 'r') as filehandle:
                num_of_lines = filehandle.readlines()
                i = 0
                while i < len(num_of_lines):
                    # Rule out #define, #include, or preprocessor, comment, etc
                    if num_of_lines[i][0] != '#' and num_of_lines[i][0] != '/':
                        # Split and remove all whitespaces
                        tmpL = num_of_lines[i].split()
                        funcS = None
                        funcNameOff = 1
                        if len(tmpL) >= 3 and tmpL[0] in JFuncSkill.TYPE_KEYWORDS:
                            # unsigned int functionname(..)
                            funcS = re.match('\w+\(', tmpL[2])
                            funcNameOff = 2
                        elif len(tmpL) >= 3 and tmpL[0] in JFuncSkill.C_KEYWORDS:
                            # static int functionname(..)
                            funcS = re.match('\w+\(', tmpL[2])
                            funcNameOff = 2
                        elif len(tmpL) >= 4 and tmpL[0] in JFuncSkill.C_KEYWORDS:
                            # static unsigned int functionname(..)
                            funcS = re.match('\w+\(', tmpL[3]) if tmpL[1] in JFuncSkill.TYPE_KEYWORDS else None
                            funcNameOff = 3
                        elif len(tmpL) >= 2:
                            # int functionname(..)
                            funcS = re.match('\w+\(', tmpL[1])
                        if len(tmpL) >= 2 and (not tmpL[0] in JFuncSkill.RULEOUT_KEYWORDS) and tmpL[0][0].isalpha() and (funcS != None) and tmpL[len(tmpL) - 1][-1] != ';':
                            originline = i
                            # Continue to read Function prototype in single or multi-line
                            funcname = None
                            for count in range(0, JFuncSkill.FUNCTION_MULTILINE):
                                tmpM = num_of_lines[i + count].split()
                                if tmpM[len(tmpM) - 1][-1] == ';':
                                    break
                                if (tmpM[0][0] == '{') or (tmpM[len(tmpM) - 1][-1] == '{'):
                                    funcname = tmpL[funcNameOff][:tmpL[funcNameOff].find('(')]
                                    returntype = tmpL[0]
                                    startline = i + count + 1
                                    break
                            # Continue to confirm the end of function
                            if funcname != None:
                                self.funcPCDict[funcname] = {}
                                endline = None
                                callL = []
                                for off, fline in enumerate(num_of_lines[startline:]):
                                    if fline[0] == '}':
                                        endline = startline + off
                                        JAlog_print(JAG_DEBUG, '::' + funcname + '::' + str(startline) + '::' + str(endline))
                                        i = endline + 1
                                        self.funcDict[f + '#' + funcname] = FuncNode(funcname, returntype, None, f)
                                        self.funcDict[f + '#' + funcname].updateFuncRangeInFile(originline, startline, endline)
                                        self.funcDict[f + '#' + funcname].incall = callL
                                        if doffset >= 0:
                                            self.filefuncDict[f].append(funcname + '#' + str(originline))
                                        break
                                    else:
                                        # inside of the function
                                        funcL = self.CheckFunctionName(fline, num_of_lines[startline + off + 1])
                                        if bool(funcL):
                                            # count number of calls
                                            for funcele in funcL:
                                                if funcele in self.funcPCDict[funcname]:
                                                    self.funcPCDict[funcname][funcele] += 1
                                                else:
                                                    self.funcPCDict[funcname][funcele] = 1
                                            for item in funcL:
                                                if item not in callL:
                                                    callL.append(item)
                            else:
                                JAlog_print(JAG_WARNING, 'Maybe it is function declaration in multiline:' + f + '::' + str(i))
                                i = i + 1
                        else:
                            i = i + 1
                    else:
                        i = i + 1
        return True if bool(self.funcDict) else False

    ##########################################################################
    # function name: GetFuncLocationByFileName
    # Description:   Enumerate all functions in C files and create function
    #                Dictionary to hold info of function prototype
    # Argument:      fileName:     file name
    #                funcName:     function name
    # Return:        None
    ########################################################################## 
    def GetFuncLocationByFileName(self, fileName=None, funcName=None):
        info = []
        if fileName!= None and funcName != None:
            if bool(self.funcDict):
                for k, v in self.funcDict.items():
                    finame = k[:k.find('#')]
                    funame = k[k.find('#') + 1:]
                    if finame == fileName and funame == funcName:
                        info.append(v.bline)
                        info.append(v.eline)
                        break
            else:
                JAlog_print(JAG_ERROR, 'Function dictionary is empty.')
        else:
            JAlog_print(JAG_ERROR, 'File name and Function name cannot be NONE.')
        return info


    def EnumerateFuncbyFileNameInPlatform(self, ssName=None):
        self.funcDict = {}
        tmpfList =[ssName]
        for f in tmpfList:
            JAlog_print(JAG_DEBUG, 'C File name enumerated: ' + str(f))
            doffset = -1
            if f not in self.filefuncDict.keys():
                self.filefuncDict[f] = []
                doffset = len(self.filefuncDict) - 1
            with open(f, 'r') as filehandle:
                num_of_lines = filehandle.readlines()
                i = 0
                while i < len(num_of_lines):
                    # Rule out #define, #include, or preprocessor, comment, etc
                    if num_of_lines[i][0] != '#' and num_of_lines[i][0] != '/':
                        # Split and remove all whitespaces
                        tmpL = num_of_lines[i].split()
                        funcS = None
                        funcNameOff = 1
                        if len(tmpL) >= 3 and tmpL[0] in JFuncSkill.TYPE_KEYWORDS:
                            # unsigned int functionname(..)
                            funcS = re.match('\w+\(', tmpL[2])
                            funcNameOff = 2
                        elif len(tmpL) >= 3 and tmpL[0] in JFuncSkill.C_KEYWORDS:
                            # static int functionname(..)
                            funcS = re.match('\w+\(', tmpL[2])
                            funcNameOff = 2
                        elif len(tmpL) >= 4 and tmpL[0] in JFuncSkill.C_KEYWORDS:
                            # static unsigned int functionname(..)
                            funcS = re.match('\w+\(', tmpL[3]) if tmpL[1] in JFuncSkill.TYPE_KEYWORDS else None
                            funcNameOff = 3
                        elif len(tmpL) >= 2:
                            # int functionname(..)
                            funcS = re.match('\w+\(', tmpL[1])
                        if len(tmpL) >= 2 and (not tmpL[0] in JFuncSkill.RULEOUT_KEYWORDS) and tmpL[0][0].isalpha() and (funcS != None) and tmpL[len(tmpL) - 1][-1] != ';':
                            originline = i
                            # Continue to read Function prototype in single or multi-line
                            funcname = None
                            for count in range(0, JFuncSkill.FUNCTION_MULTILINE):
                                tmpM = num_of_lines[i + count].split()
                                if tmpM[len(tmpM) - 1][-1] == ';':
                                    break
                                if (tmpM[0][0] == '{') or (tmpM[len(tmpM) - 1][-1] == '{'):
                                    funcname = tmpL[funcNameOff][:tmpL[funcNameOff].find('(')]
                                    returntype = tmpL[0]
                                    startline = i + count + 1
                                    break
                            # Continue to confirm the end of function
                            if funcname != None:
                                self.funcPCDict[funcname] = {}
                                endline = None
                                callL = []
                                for off, fline in enumerate(num_of_lines[startline:]):
                                    if fline[0] == '}':
                                        cntl = 0
                                        dr = 0
                                        while (cntl < JFuncSkill.RETURN_THRESHOLD):
                                            if 'return ' in num_of_lines[startline + off - cntl] or 'return(' in num_of_lines[startline + off - cntl]:
                                                dr = 1
                                                break
                                            cntl += 1
                                        endline = startline + off if dr == 0 else startline + off - cntl
                                        JAlog_print(JAG_DEBUG, '::' + funcname + '::' + str(startline) + '::' + str(endline))
                                        i = endline + 1
                                        self.funcDict[f + '#' + funcname] = FuncNode(funcname, returntype, None, f)
                                        self.funcDict[f + '#' + funcname].updateFuncRangeInFile(originline, startline, endline)
                                        self.funcDict[f + '#' + funcname].incall = callL
                                        if doffset >= 0:
                                            self.filefuncDict[f].append(funcname + '#' + str(originline))
                                        break
                                    else:
                                        # inside of the function
                                        funcL = self.CheckFunctionName(fline, num_of_lines[startline + off + 1])
                                        if bool(funcL):
                                            # count number of calls
                                            for funcele in funcL:
                                                if funcele in self.funcPCDict[funcname]:
                                                    self.funcPCDict[funcname][funcele] += 1
                                                else:
                                                    self.funcPCDict[funcname][funcele] = 1
                                            for item in funcL:
                                                if item not in callL:
                                                    callL.append(item)
                            else:
                                JAlog_print(JAG_WARNING, 'Maybe it is function declaration in multiline:' + f + '::' + str(i))
                                i = i + 1
                        else:
                            i = i + 1
                    else:
                        i = i + 1
        return True if bool(self.funcDict) else False



    ##########################################################################
    # function name: CheckFunctionName
    # Description:   Detect function call in lines read out from c file, and 
    #                return function name in list
    # Argument:      linestr:      current line
    #                nextlinestr:  next line
    # Return:        list of function name
    ########################################################################## 
    def CheckFunctionName(self, linestr, nextlinestr):
        fn = []
        if linestr != None:
            # Remove all Comments first
            c1off = linestr.find('/*')
            c2off = linestr.find('//')
            if c1off != -1:
                ss = linestr[:c1off]
            elif c2off != -1:
                ss = linestr[:c2off]
            else:
                ss = linestr
            func_rule = '^[a-zA-Z]+[a-zA-Z0-9_]*$'
            ssList = re.split('\.|>|=|&|,| |\)', ss)
            #print(str(ssList))
            for item in ssList:
                if len(item) >= 2:
                    for start in range(len(item)):
                        if item[start] != '(':
                            break
                    tmp = item[start:]
                    off = tmp.find('(')
                    if off >= 1:
                        if re.match(func_rule, tmp[0 : off]):
                            fn.append(tmp[0 : off])
            # Check the last item and next line
            litem = ssList[len(ssList) - 1]
            if ';' not in litem:
                if re.match(func_rule, litem[:-1]) and litem[-1] == '\n':
                    nl = nextlinestr.split()
                    if bool(nl) and nl[0][0] == '(':
                        fn.append(litem[:-1])
        return fn

    ##########################################################################
    # function name: FindCallerNameByLineNumber
    # Description:   Given file name and line index, check its caller based on 
    #                the scope of each function.
    # Argument:      filename:     function name
    #                lineindex:    line index in c file
    # Return:        caller
    ########################################################################## 
    def FindCallerNameByLineNumber(self, filename, lineindex):
        caller = None
        for k, v in self.funcDict.items():
            kfilename = k[:k.find('#')]
            if kfilename == filename and lineindex > v.bline and lineindex < v.eline:
                caller = k[k.find('#') + 1:]
                break
        return caller

    ##########################################################################
    # function name: FindTopCallerByFuncName
    # Description:   Given function name, find its caller recursively to the 
    #                top caller in list.
    # Argument:      funcName:     function name
    #                callerList:   caller list
    # Return:        None
    ########################################################################## 
    def FindTopCallerByFuncName(self, funcName=None, callerList=[]):
        topCaller = None
        keyword = funcName + '('
        for f in self.cfileList:
            with open(os.path.join(self.modelpath, f), 'r') as filehandle:
                num_of_lines = filehandle.readlines()
            for index, line in enumerate(num_of_lines):
                if keyword in line and 'extern ' not in line:
                    # search index in function dictionary
                    topCaller = self.FindCallerNameByLineNumber(f, index)
                    if topCaller != None and topCaller != funcName:
                        callerList.append(topCaller)
                        self.FindTopCallerByFuncName(topCaller, callerList)
                        break
            if topCaller != None:
                #print('####' + f + '::' + str(topCaller))
                break

    ##########################################################################
    # function name: FindSubFuncsByFuncName
    # Description:   Given function name, find all child-functions recursively
    #                and generate a list
    # Argument:      funcName:     function name
    #                calledList:   list of functions called
    #                stackflow:    The stack holds functions which are not supposed
    #                              to be added to the calledList.
    # Return:        None
    ########################################################################## 
    def FindSubFuncsByFuncName(self, funcName=None, calledList=[], stackflow=None):
        for k, v in self.funcDict.items():
            kfuncname = k[k.find('#') + 1:]
            if funcName == kfuncname:
                for calledf in v.incall:
                    if calledf not in calledList and calledf not in JFuncSkill.RULEOUT_FUNCTIONS:
                        if stackflow == None:
                            calledList.append(calledf)
                            self.FindSubFuncsByFuncName(calledf, calledList, stackflow)
                        elif calledf not in stackflow:
                            calledList.append(calledf)
                            stackflow.append(calledf)
                            self.FindSubFuncsByFuncName(calledf, calledList, stackflow)
                    
    ##########################################################################
    # function name: FindSubFuncsByFuncNameEx
    # Description:   Given function name, find all child-functions recursively
    #                and generate a Dict holding function name and number of calls
    # Argument:      funcName:     function name
    #                calledDict:   Dict of functions and number of calls
    #                stackflow:    The stack holds functions which are not supposed
    #                              to be added to the calledList.
    # Return:        None
    ########################################################################## 
    def FindSubFuncsByFuncNameEx(self, funcName=None, calledDict={}, stackflow=None):
        for k, v in self.funcDict.items():
            kfuncname = k[k.find('#') + 1:]
            if funcName == kfuncname:
                for calledf in v.incall:
                    if (calledf not in JFuncSkill.RULEOUT_FUNCTIONS) and (calledf not in stackflow):
                        if calledf in calledDict.keys():
                            calledDict[calledf] += self.funcPCDict[funcName][calledf]
                        else:
                            calledDict[calledf] = self.funcPCDict[funcName][calledf]
                        self.FindSubFuncsByFuncNameEx(calledf, calledDict, stackflow)



#model_funcs = JFuncSkill('Model_1_1', os.path.join(os.getcwd(), 'testcases\Melville\Model_1_1_ert_shrlib_rtw'))
#model_funcs.EnumerateFuncbyFileName('Model_1_1.c')
#print(str(model_funcs.funcDict))
#for k, v in model_funcs.funcDict.items():
    #print('>>>>' + v.fname + '::' + str(v.incall))
    #print('>>>>' + v.fname + '::' + str(v.oline) + '::' + str(v.bline) + '::' + str(v.eline))


#model_funcs = JFuncSkill('Model_1_2', os.path.join(os.getcwd(), 'testcases\Melville\Model_1_2_ert_shrlib_rtw'))
#model_funcs.EnumerateFuncbyFileName('*')
#print(str(model_funcs.funcDict))
#for k, v in model_funcs.funcDict.items():
#    print('>>>>' + v.fname + '::' + str(v.oline) + '::' + str(v.bline) + '::' + str(v.eline))


#model_funcs = JFuncSkill('Model_1_1', os.path.join(os.getcwd(), 'testcases\Melville\Model_1_1_ert_shrlib_rtw'))
#model_funcs.EnumerateFuncbyFileName('Model_1_1.c')
#for k, v in model_funcs.funcDict.items():
#    print('>>>>' + v.fname + '::' + str(v.bline) + '::' + str(v.eline))


#model_funcs = JFuncSkill('Model_1_1', os.path.join(os.getcwd(), 'testcases\Melville\Model_1_1_ert_shrlib_rtw'))
#model_funcs.EnumerateFuncbyFileName('*')
#calledL = []
#model_funcs.FindSubFuncsByFuncName('Model_1_1_step2', calledL)
#print('>>>>' + str(calledL))



