import sys
import os


from JCSchema import *
from JPySchema import *
from JXmlTrans import *
from JXmlSerial import *
from JSchemaConfig import *
from JLogPrint import *
from datetime import date








class JCBuilder:
    DEFAULT_PATH = './autogen/'
    def __init__(self, h_filename=None, c_filename=None, genpath=None):
        if (genpath is not None):
            self.path = genpath if genpath[:-1] == '/' else genpath + '/'
        else:
            self.path = JCBuilder.DEFAULT_PATH
        if not os.path.exists(self.path):
            os.makedirs(self.path, exist_ok=True)
        self.h_filename = h_filename
        self.c_filename = c_filename
        self.Schema = JCSchema()

    def H_GenerateFile(self):
        raise NotImplementedError('Must override H_GenerateFile Function')

    def C_GenerateFile(self):
        raise NotImplementedError('Must override C_GenerateFile Function')

    def H_GenerateFile1(self, headerList=None, macroDict=None, externvariableList=None, externfunctionDict=None):
        ff = os.path.join(self.path, self.h_filename)
        if os.path.exists(ff):
            os.remove(ff)
        with open(ff, 'w+') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.h_filename, 'List all Pragma Sections'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.h_filename))
            filehandle.write('\n')
            # Ifdef
            filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.h_filename[:-2].upper()))
            filehandle.write('\n')
            filehandle.write('\n\n\n')
            # include headers
            if bool(headerList):
                for item in headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(item, '"'))
            # macro definition
            if bool(macroDict):
                for k, v in macroDict.items():
                    if len(v) == 1:
                        filehandle.write(self.Schema.Schema_Define(k, v[0]))
                    elif len(v) > 1:
                        filehandle.write(self.Schema.Schema_DefineMulti(k, v))
            filehandle.write('\n\n\n')
            # extern variable
            # ...

            # extern function
            # ...

            filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.h_filename[:-2]))



class JFLBuilder(JCBuilder):
    HEADERFILE = 'FeatureLib.h'
    CFILE = 'FeatureLib.c'
    def __init__(self, genpath=None, generateflag=True, genConfigFile=''):
        self.path = genpath
        self.genflag = generateflag
        self.genConfigFile = genConfigFile
        self.macroList = []
        self.H_headerList = ['audio_control_common.h']
        self.H_ptrheaderList = []
        self.C_headerList = ['FeatureLib.h', 'FeatureMute.h', 'Feature.h']
        self.funcList = [['RET_CODE_TYPE', 'FL_Construct_Features', '(void)', None],
                         ['RET_CODE_TYPE', 'FL_Initialize_Features', '(void)', None],
                         ['bool', 'FL_Is_Feature_Lib_Ready', '(void)', None],
                         ['void', 'FL_Connect_All_Sinks', '(bool newConnectState)', None],
                         ['void', 'FL_Save_All_Sinks_Parameters', '(void)', None],
                         ['void', 'FL_Restore_All_Sinks_Parameters', '(void)', None]]
        self.ptrfuncList = []
        self.Schema = JFeatureLibSchema()
        self.shadowfilename = self.Schema.Schema_ShadowFilename(JFLBuilder.CFILE[:-2], '.c')

    def H_GenerateFile(self, feadict=None):
        #print('Generating Header File: ' + JFLBuilder.HEADERFILE + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + JFLBuilder.HEADERFILE + ' ....')
        incpath = self.path + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        if (self.genflag == True) or (not os.path.exists(incpath + JFLBuilder.HEADERFILE)):
            with open(incpath + JFLBuilder.HEADERFILE, 'w') as filehandle:
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                filehandle.write(self.Schema.Schema_FileInfoComment(JFLBuilder.HEADERFILE, 'Include list of features'))
                filehandle.write('\n')
                # File Log
                filehandle.write(self.Schema.Schema_FileLog(JFLBuilder.HEADERFILE))
                filehandle.write('\n')
                # Ifdef
                filehandle.write(self.Schema.Schema_IfdefStatementBegin(JFLBuilder.HEADERFILE[:-2].upper()))
                filehandle.write('\n')
                # Include Header
                filehandle.write(self.Schema.Schema_FileIncludeHeader())
                for hitem in self.H_headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write(self.Schema.DONOTCHANGE_BELOW)
                filehandle.write(JFeatureLibSchema.FEATUREHEADERSTART + '\n')
                for hitem in self.H_ptrheaderList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write(JFeatureLibSchema.FEATUREHEADEREND + '\n')
                filehandle.write(self.Schema.DONOTCHANGE_ABOVE)
                filehandle.write('\n\n')
                # Extern
                filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
                # Extern functions
                for fitem in self.funcList:
                    filehandle.write(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
                # GetPtr functions
                filehandle.write(self.Schema.DONOTCHANGE_BELOW)
                filehandle.write(JFeatureLibSchema.GETPTRSTART + '\n')
                for fitem in self.ptrfuncList:
                    filehandle.write(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
                filehandle.write(JFeatureLibSchema.GETPTREND + '\n')
                filehandle.write(self.Schema.DONOTCHANGE_ABOVE)
                filehandle.write('\n\n\n')
                filehandle.write(self.Schema.Schema_IfdefStatementEnd(JFLBuilder.HEADERFILE[:-2]))
        else:
            # Update the current FeatureLib Header file
            with open(incpath + JFLBuilder.HEADERFILE, 'r') as filehandle:
                headerStr = filehandle.read()
            with open(incpath + JFLBuilder.HEADERFILE + '.tmp', 'w') as filehandle:
                filehandle.write(self.Schema.Schema_UpdateFeatureInHeader(headerStr, self.H_ptrheaderList, self.ptrfuncList))
            if (os.path.getsize(incpath + JFLBuilder.HEADERFILE + '.tmp') != 0):
                os.remove(incpath + JFLBuilder.HEADERFILE)
                os.rename(incpath + JFLBuilder.HEADERFILE + '.tmp', incpath + JFLBuilder.HEADERFILE)
            else:
                #print('ERROR: When update ' + self.hfilename + '....')
                Jlog_print(JAG_ERROR, 'Wehn update ' + self.hfilename + '....')

    def C_GenerateFile(self, feadict=None):
        #print('Generating C File: ' + JFLBuilder.CFILE + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating C File: ' + JFLBuilder.CFILE + ' ....')
        srcpath = self.path + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        # Generate FeatureLib Shadow .c file
        with open(srcpath + self.shadowfilename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.shadowfilename, 'FeatureLib initialization'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.shadowfilename))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.C_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Macros
            filehandle.write(self.Schema.Schema_FileModuleMacros())
            for mitem in self.macroList:
                filehandle.write(self.Schema.Schema_Define(mitem[0], str(mitem[1])))
            filehandle.write('\n')
            # Extern
            filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
            filehandle.write(self.Schema.Schema_EnumFeatureVariablesShadow(None, feadict) + '\n')
            filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
            # Functions
            filehandle.write(self.Schema.Schema_FL_Construct_FeaturesShadow(self.funcList[0], feadict))
            filehandle.write(self.Schema.Schema_FL_Initialize_FeaturesShadow(self.funcList[1], feadict))
            for item in self.ptrfuncList:
                #if item[3] is not None:
                filehandle.write(self.Schema.Schema_FeatureGetPtr(item))
        # Generate FeatureLib .c file
        with open(srcpath + JFLBuilder.CFILE, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(JFLBuilder.CFILE, 'Feature Control defines for enabled features'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(JFLBuilder.CFILE))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.C_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Macros
            filehandle.write(self.Schema.Schema_FileModuleMacros() + '\n')
            # Extern
            filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
            filehandle.write(self.Schema.Schema_EnumFeatureVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
            # Functions
            for item in self.funcList:
                #if item[3] is None:
                filehandle.write(getattr(self.Schema, 'Schema_' + item[1])(item, feadict))

    def GenerateCode(self, featureDict=None):
        if (featureDict):
            for item in featureDict.keys():
                self.ptrfuncList.append(self.Schema.Schema_FLGetFeaturePtr(item))
                self.H_ptrheaderList.append(self.Schema.Schema_FLAddHeaderFile(item))
                self.macroList.append(self.Schema.Schema_FLMacroFeatureNum(item, featureDict[item]))
            self.H_GenerateFile(featureDict)
            self.C_GenerateFile(featureDict)
        else:
            #print('ERROR: feature Dict is Empty, no FeatureLib.c/h generated')
            Jlog_print(JAG_ERROR, 'feature Dict is Empty, no FeatureLib.c/h generated')



class JSetupAPIBuilder(JCBuilder):
    def __init__(self, genpath=None, featuretypename=None, featurecount=None, featureImpl=None, bamImpl=None, genConfigFile=''):
        self.path = genpath
        self.feaname = featuretypename
        self.feacount = featurecount
        self.feaimplDict = featureImpl
        self.bamimplDict = bamImpl
        self.genConfigFile = genConfigFile
        self.macroList = []
        self.H_headerList = ['options.h']
        self.C_headerList = ['Talaria_BPmap.h']
        self.funcList = []
        self.Schema = JSetupAPISchema(featuretypename, featurecount)
        self.hfilename = self.Schema.Schema_SetupFilename(featuretypename, '.h')
        self.cfilename = self.Schema.Schema_SetupFilename(featuretypename, '.c')
        self.C_headerList.append(self.hfilename)

    def H_GenerateFile(self, feaobj=None):
        #print('Generating Header File: ' + self.hfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.hfilename + ' ....')
        incpath = self.path + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        with open(incpath + self.hfilename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.hfilename, 'TODO'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.hfilename))
            filehandle.write('\n')
            # Ifdef
            filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.hfilename[:-2].upper()))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.H_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Extern
            filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
            # Extern functions
            for fitem in self.funcList:
                filehandle.write(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
            filehandle.write('\n\n\n')
            filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.hfilename[:-2]))

    def C_GenerateFile(self, feaobj=None, feanameDict=None, bamDict=None, bamnameDict=None):
        #print('Generating Header File: ' + self.cfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.cfilename + ' ....')
        srcpath = self.path + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        with open(srcpath + self.cfilename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.cfilename, 'TODO'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.cfilename))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.C_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Extern
            filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
            filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
            # Functions
            for item in self.funcList:
                if item[3] is not None:
                    filehandle.write(self.Schema.Schema_SetupBamproxyIpmle(item, feaobj, feanameDict, bamDict, bamnameDict, self.feaimplDict, self.bamimplDict))

    def GenerateCode(self, feaobj=None, feanameDict=None, bamDict=None, bamnameDict=None):
        if (feaobj) and (bamDict):
            for item, cnt in bamDict.items():
                self.funcList.append(self.Schema.Schema_SetupBamproxy(item, cnt))
                self.H_headerList.append(self.Schema.Schema_SetupAddBamproxyHeaderFile(item))
            self.H_headerList.append(self.Schema.Schema_SetupAddFeatureHeaderFile(self.feaname))
            self.H_GenerateFile(feaobj)
            self.C_GenerateFile(feaobj, feanameDict, bamDict, bamnameDict)
        else:
            #print('ERROR: feature Dict is Empty, no FeatureLib.c/h generated')
            Jlog_print(JAG_ERROR, 'feature Dict is Empty, no FeatureLib.c/h generated')


class JFeatureBuilder(JCBuilder):
    def __init__(self, genpath=None, generateflag=True, featurename=None, featurecount=None, genConfigFile=''):
        self.path = genpath
        self.genflag = generateflag
        self.genConfigFile = genConfigFile
        self.feaname = featurename
        self.feacount = featurecount
        self.macroList = []
        self.externList = []
        self.variableList = [['static feature_vtbl', None],
                             ['audio_control_message', 'CtrlBuf'],
                            ['audio_control_message', 'RespBuf']]
        self.enumBamnameDict = {}
        self.enumActionidDict = {}
        self.H_headerList = ['Feature.h', 'Roommode_Download.h', 'options.h']
        self.C_headerList = ['FeatureMute.h', 'Notifications.h']
        self.funcList = []
        self.Schema = JFeatureSchema(featurename)
        self.hfilename = self.Schema.Schema_FeatureFilename(featurename, '.h')
        self.cfilename = self.Schema.Schema_FeatureFilename(featurename, '.c')
        self.shadowfilename = self.Schema.Schema_ShadowFilename(featurename, '.c')
        self.C_headerList.append(self.hfilename)

    def H_GenerateFile(self, feaobj=None, bamDict=None, bamnameDict=None):
        #print('Generating Header File: ' + self.hfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.hfilename + ' ....')
        incpath = self.path + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        if (self.genflag == True) or (not os.path.exists(incpath + self.hfilename)):
            # Create new Feature Header file
            with open(incpath + self.hfilename, 'w') as filehandle:
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                filehandle.write(self.Schema.Schema_FileInfoComment(self.hfilename, 'Include list of bamproxys'))
                filehandle.write('\n')
                # File Log
                filehandle.write(self.Schema.Schema_FileLog(self.hfilename))
                filehandle.write('\n')
                # Ifdef
                filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.hfilename[:-2].upper()))
                filehandle.write('\n')
                # Include Header
                filehandle.write(self.Schema.Schema_FileIncludeHeader())
                for hitem in self.H_headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write('\n\n')
                # Macros
                filehandle.write(self.Schema.Schema_FileModuleMacros())
                for mitem in self.macroList:
                    filehandle.write(self.Schema.Schema_Define(mitem[0], str(mitem[1])))
                filehandle.write('\n')
                #Enum
                filehandle.write(self.Schema.DONOTCHANGE_BELOW)
                filehandle.write(self.Schema.ENUM_START + '\n')
                if bool(self.enumBamnameDict):
                    for enumName, enumList in self.enumBamnameDict.items():
                        filehandle.write(self.Schema.Schema_EnumDefine(enumName, enumList))
                        filehandle.write('\n')
                if bool(self.enumActionidDict):
                    for enumName, enumList in self.enumActionidDict.items():
                        filehandle.write(self.Schema.Schema_EnumDefine(enumName, enumList))
                        filehandle.write('\n')
                filehandle.write(self.Schema.ENUM_END + '\n')
                filehandle.write(self.Schema.DONOTCHANGE_ABOVE)
                # Extern
                filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
                filehandle.write(self.Schema.Schema_FeatureType(bamDict))
                filehandle.write(self.Schema.Schema_FeatureDataType(bamDict))
                filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
                # Extern functions
                for fitem in self.funcList:
                    filehandle.write(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
                filehandle.write('\n\n\n')
                filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.hfilename[:-2]))
        else:
            # Update the current Feature Header file
            bamproxyTypeStr = self.Schema.Schema_FeatureType(bamDict, True)
            bamproxyDatatypeStr = self.Schema.Schema_FeatureDataType(bamDict, True)
            bamnameEnumList = []
            bamActionidEnumList = []
            if bool(self.enumBamnameDict):
                for enumName, enumList in self.enumBamnameDict.items():
                    bamnameEnumList.append(self.Schema.Schema_EnumDefine(enumName, enumList))
            if bool(self.enumActionidDict):
                for enumName, enumList in self.enumActionidDict.items():
                    bamActionidEnumList.append(self.Schema.Schema_EnumDefine(enumName, enumList))
            with open(incpath + self.hfilename, 'r') as filehandle:
                headerStr = filehandle.read()
            with open(incpath + self.hfilename + '.tmp', 'w') as filehandle:
                filehandle.write(self.Schema.Schema_UpdateBamproxyInHeader(headerStr, bamproxyTypeStr, bamproxyDatatypeStr, bamnameEnumList, bamActionidEnumList))
            if (os.path.getsize(incpath + self.hfilename + '.tmp') != 0):
                os.remove(incpath + self.hfilename)
                os.rename(incpath + self.hfilename + '.tmp', incpath + self.hfilename)
            else:
                #print('ERROR: When update ' + self.hfilename + '....')
                Jlog_print(JAG_ERROR, 'When update ' + self.hfilename + '....')


    def C_GenerateFile(self, bamDict=None):
        #print('Generating C File: ' + self.cfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating C File: ' + self.cfilename + ' ....')
        srcpath = self.path + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        # Generate Feature Shadow .c file
        with open(srcpath + self.shadowfilename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.shadowfilename, 'Bamproxys initialization for ' + self.feaname + ' features'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.shadowfilename))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.C_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Functions
            filehandle.write(self.Schema.Schema_FeatureInitializeShadowImple(self.externList[0], self.feacount, bamDict))
        # Generate Feature .c file
        if (self.genflag == True) or (not os.path.exists(srcpath + self.cfilename)):
            with open(srcpath + self.cfilename, 'w') as filehandle:
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                filehandle.write(self.Schema.Schema_FileInfoComment(self.cfilename, 'Feature Control defines for ' + self.feaname + ' features'))
                filehandle.write('\n')
                # File Log
                filehandle.write(self.Schema.Schema_FileLog(self.cfilename))
                filehandle.write('\n')
                # Include Header
                filehandle.write(self.Schema.Schema_FileIncludeHeader())
                for hitem in self.C_headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write('\n\n')
                # Extern
                filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
                filehandle.write(self.Schema.Schema_FeatureVariables(self.variableList) + '\n')
                filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileExtern(self.externList))
                filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
                # Functions
                for item in self.funcList:
                    filehandle.write(getattr(self.Schema, 'Schema_' + item[1].replace(self.feaname, '') + 'Impl')(item))

    def GenerateCode(self, feaobj=None, feanameDict=None, bamDict=None, bamnameDict=None):
        if (feaobj) and (bamDict):
            for item, cnt in bamDict.items():
                self.H_headerList.append(self.Schema.Schema_FeaAddHeaderFile(item))
                if (cnt > 1):
                    self.Schema.Schema_FeatureEnumBamname(self.enumBamnameDict, item, bamnameDict[item])
                obj_rtctop = feaobj if item == self.feaname else feaobj.data[bamnameDict[item][0]]
                self.Schema.Schema_FeatureEnumBamActionID(self.enumActionidDict, obj_rtctop.data, item)
            self.funcList.append(self.Schema.Schema_FeatureConstruct(self.feaname))
            self.funcList.append(self.Schema.Schema_FeatureInitialize(self.feaname))
            self.externList.append(self.Schema.Schema_FeatureInitializeShadow(self.feaname))
            self.H_GenerateFile(feaobj, bamDict, bamnameDict)
            self.C_GenerateFile(bamDict)
        else:
            #print('ERROR: feature Dict is Empty, no FeatureLib.c/h generated')
            Jlog_print(JAG_ERROR, 'feature Dict is Empty, no FeatureLib.c/h generated')



class JBamproxyBuilder(JCBuilder):
    def __init__(self, genpath=None, generateflag=True, bamproxyname=None, featurename=None, appname=None, genConfigFile=''):
        self.path = genpath
        self.genflag = generateflag
        self.genConfigFile = genConfigFile
        self.bamname = bamproxyname
        self.feaname = featurename
        self.appname = appname
        self.externList = []
        self.variableList = []
        self.enumAction = {}
        self.enumRtc = {}
        self.enumTop = {}
        self.H_headerList = ['BamProxy.h', 'BamProxyShadow.h', 'BP_common.h', 'options.h']
        self.C_headerList = ['Feature_Utils.h']
        self.funcSuffix = ['AssignActionIds', 'Init', 'Finalize', 'Response', 'Construct']
        self.funcList = []
        self.xmlser = JXmlSerial(appname)
        self.Schema = JBamproxySchema(bamproxyname)
        self.hfilename = self.Schema.Schema_BamproxyFilename(bamproxyname, '.h')
        self.cfilename = self.Schema.Schema_BamproxyFilename(bamproxyname, '.c')
        self.shadowfilename = self.Schema.Schema_ShadowFilename(bamproxyname, '.c')
        self.C_headerList.append(self.hfilename)

    def H_GenerateFile(self, bamDict=None, bamnameDict=None):
        #print('Generating Header File: ' + self.hfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.hfilename + ' ....')
        incpath = self.path + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        if (self.genflag == True) or (not os.path.exists(incpath + self.hfilename)):
            # Create new Bamproxy Header file
            with open(incpath + self.hfilename, 'w') as filehandle:
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                filehandle.write(self.Schema.Schema_FileInfoComment(self.hfilename, 'Bam Proxy Control defines for ' + self.bamname))
                filehandle.write('\n')
                # File Log
                filehandle.write(self.Schema.Schema_FileLog(self.hfilename))
                filehandle.write('\n')
                # Ifdef
                filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.hfilename[:-2].upper()))
                filehandle.write('\n')
                # Include Header
                filehandle.write(self.Schema.Schema_FileIncludeHeader())
                for hitem in self.H_headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write('\n\n')
                #Enum
                filehandle.write(self.Schema.DONOTCHANGE_BELOW)
                filehandle.write(self.Schema.ENUM_START + '\n')
                if bool(self.enumAction):
                    for enumName, enumList in self.enumAction.items():
                        filehandle.write(self.Schema.Schema_EnumDefine(enumName, enumList))
                        filehandle.write('\n')
                filehandle.write(self.Schema.ENUM_END + '\n')
                filehandle.write(self.Schema.DONOTCHANGE_ABOVE)
                # Extern
                filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
                filehandle.write(self.Schema.Schema_BamproxyType(self.bamname))
                filehandle.write(self.Schema.Schema_BamproxyDataType(self.bamname))
                filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
                # Extern functions
                for fitem in self.funcList:
                    filehandle.write(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
                filehandle.write('\n\n\n')
                filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.hfilename[:-2]))
        else:
            # Update the current Bamproxy Header file
            bamActionEnumList = []
            if bool(self.enumAction):
                for enumName, enumList in self.enumAction.items():
                    bamActionEnumList.append(self.Schema.Schema_EnumDefine(enumName, enumList))
            with open(incpath + self.hfilename, 'r') as filehandle:
                headerStr = filehandle.read()
            with open(incpath + self.hfilename + '.tmp', 'w') as filehandle:
                filehandle.write(self.Schema.Schema_UpdateActionInHeader(headerStr, bamActionEnumList))
            if (os.path.getsize(incpath + self.hfilename + '.tmp') != 0):
                os.remove(incpath + self.hfilename)
                os.rename(incpath + self.hfilename + '.tmp', incpath + self.hfilename)
            else:
                #print('ERROR: When update ' + self.hfilename + '....')
                Jlog_print(JAG_ERROR, 'When update ' + self.hfilename + '....')


    def C_GenerateFile(self, bamDict=None, obj_rtctop=None):
        #print('Generating C File: ' + self.cfilename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating C File: ' + self.cfilename + ' ....')
        srcpath = self.path + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        # Generate Feature Shadow .c file
        with open(srcpath + self.shadowfilename, 'w') as filehandle:
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.shadowfilename, 'code for ' + self.bamname + ' Bam Proxy command Initialization'))
            filehandle.write('\n')
            # File Log
            filehandle.write(self.Schema.Schema_FileLog(self.shadowfilename))
            filehandle.write('\n')
            # Include Header
            filehandle.write(self.Schema.Schema_FileIncludeHeader())
            for hitem in self.C_headerList:
                filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
            filehandle.write('\n\n')
            # Functions
            filehandle.write(self.Schema.Schema_BamproxyInitializeShadowImple(self.externList[0], self.enumAction))
            filehandle.write('\n')
            # Write Rtc & Top function
            for rtcid, rtckeyword in self.enumRtc.items():
                bamrtcfunc = self.xmlser.Jxs_RtcParseAllEx(rtckeyword)
                if bamrtcfunc is not None:
                    filehandle.write(self.Schema.Schema_RtcFuncImpl(self.bamname, rtcid, bamrtcfunc))
            for item in self.enumTop.keys():
                filehandle.write(self.Schema.Schema_TopFuncImpl(self.bamname, item))
                filehandle.write('\n')
        # Generate Feature .c file
        if (self.genflag == True) or (not os.path.exists(srcpath + self.cfilename)):
            with open(srcpath + self.cfilename, 'w') as filehandle:
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                filehandle.write(self.Schema.Schema_FileInfoComment(self.cfilename, 'Feature Control defines for ' + self.feaname + ' features'))
                filehandle.write('\n')
                # File Log
                filehandle.write(self.Schema.Schema_FileLog(self.cfilename))
                filehandle.write('\n')
                # Include Header
                filehandle.write(self.Schema.Schema_FileIncludeHeader())
                for hitem in self.C_headerList:
                    filehandle.write(self.Schema.Schema_IncludeHeaderFile(hitem, '"'))
                filehandle.write('\n\n')
                # Extern
                filehandle.write(self.Schema.Schema_FileModuleVariables() + '\n')
                filehandle.write(self.Schema.Schema_BamproxyVtable(self.bamname) + '\n')
                filehandle.write(self.Schema.Schema_FileExternalVariables() + '\n')
                filehandle.write(self.Schema.Schema_FileExtern(self.externList))
                filehandle.write(self.Schema.Schema_FileModuleFunctions() + '\n\n\n')
                # Functions
                for item in self.funcList:
                    filehandle.write(getattr(self.Schema, 'Schema_' + item[1].replace(self.bamname, '') + 'Impl')(item))
                    filehandle.write('\n')

    def GenerateCode(self, bamobj=None, feaobj=None, bamDict=None):
        if (bamobj):
            obj_rtctop = feaobj if self.bamname == self.feaname else bamobj
            self.Schema.Schema_EnumBamAction(self.enumAction, self.enumRtc, self.enumTop, obj_rtctop.data, self.bamname)
            for suf in self.funcSuffix:
                self.funcList.append(self.Schema.Schema_BamproxyFunc(suf, self.bamname))
            self.externList.append(self.Schema.Schema_BamproxyInitializeShadow(self.bamname))
            self.H_GenerateFile()
            self.C_GenerateFile(bamDict, obj_rtctop)
        else:
            #print('ERROR: feature Dict is Empty, no FeatureLib.c/h generated')
            Jlog_print(JAG_ERROR, 'feature Dict is Empty, no FeatureLib.c/h generated')




class JFrameworkBuilder(JCBuilder):
    DEFAULT_JMODULEDIR = 'JModule_Lib'
    DEFAULT_AUDIOGRDIR = 'Audio_Graph'
    def __init__(self, genpath=None, jframeworkname=None, jframeworkobj=None, genConfigFile=''):
        self.desc_header = ['JModule.h', '_JModule_Id.h', 'JModule_Bitfield.h']
        self.genConfigFile = genConfigFile
        self.jfrmname = jframeworkname
        self.jfrmobj = jframeworkobj
        self.jname = ''
        self.sysflag = True
        self.jmodlDictDict = {}
        self.jprocDictList = []
        self.jportDict = {}
        self.returnList = []
        self.returnCaseList = []
        self.returnportList = []
        self.extraheaderList = []
        self.modulename = []
        self.graph = []
        # Parse Schema Configuration from File
        self.schemaConfig = JFmrSchemaConfig(genConfigFile)
        self.jfmrSchema = JFrameworkSchema(jframeworkname, self.schemaConfig)
        # Create root folder and subfoloders for each JFramework Object
        if (genpath is not None):
            self.baf_rootpath = genpath if genpath[:-1] == '/' else genpath + '/'
        else:
            self.baf_rootpath = './'
        self.baf_rootpath += self.jfrmname + '/'
        if not os.path.exists(self.baf_rootpath):
            os.makedirs(self.baf_rootpath, exist_ok=True)
        if not os.path.exists(self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR):
            os.makedirs(self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR, exist_ok=True)
        if not os.path.exists(self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR):
            os.makedirs(self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR, exist_ok=True)

    def GenerateJFrameworkPy(self, jfmrmlname, jfmrmlobj=None, id=None):
        if (jfmrmlname is not None) and (jfmrmlobj is not None):
            self.pySchema = JFrameworkPySchema(jfmrmlname)
            file_path = self.baf_rootpath + 'JFramework_' + jfmrmlname + '.py'
            if os.path.exists(file_path):
                os.remove(file_path)
            with open(file_path, 'w') as filehandle:
                # Write import
                filehandle.write(self.pySchema.Schema_ImportStandardPackage())
                filehandle.write(self.pySchema.Schema_ImportJFrameworkPackage(extrapktList=['JFrameworkStru']))
                filehandle.write('\n\n\n')
                # Write JFramework start
                filehandle.write(self.pySchema.Schema_JFrameworkDefStart())
                # Write JFramework Default function
                filehandle.write(self.pySchema.Schema_JFrameworkDefaultFunc(jfmrmlobj))
                # Write JFramework data section
                filehandle.write(self.pySchema.Schema_JframeworkSelfData(jfmrmlobj, id, True))


    def GenerateJModulePy(self, jfmrmlname=None, jfmrmlobj=None, id=None):
        if (jfmrmlname is not None) and (jfmrmlobj is not None):
            self.pySchema = JModulePySchema(jfmrmlname)
            file_path = self.baf_rootpath + 'JModule_' + jfmrmlname + '.py'
            if os.path.exists(file_path):
                os.remove(file_path)
            with open(file_path, 'w') as filehandle:
                # Write import
                filehandle.write(self.pySchema.Schema_ImportStandardPackage())
                filehandle.write(self.pySchema.Schema_ImportJModulePackage())
                filehandle.write('\n\n\n')
                # Write JModule start
                filehandle.write(self.pySchema.Schema_JModuleDefStart())
                for procname, procobj in jfmrmlobj.items():
                    if hasattr(procobj, 'type') and procobj.type == 'JProcess':
                        # Write JProcess start
                        filehandle.write(self.pySchema.Schema_JProcessDefStart(procname))
                        for name, obj in procobj.data.items():
                            if hasattr(obj, 'type') and obj.type == 'JPortIn':
                                # Write Port In
                                filehandle.write(self.pySchema.Schema_PortIn(name, obj))
                            elif hasattr(obj, 'type') and obj.type == 'JPortOut':
                                # Write Port Out
                                filehandle.write(self.pySchema.Schema_PortOut(name, obj))
                            elif name == 'callrate':
                                filehandle.write(self.pySchema.Schema_Callrate(name, obj))
                            elif name == 'process':
                                filehandle.write(self.pySchema.Schema_Process(name, obj))
                            elif name == 'rate_period':
                                filehandle.write(self.pySchema.Schema_Rateinsec(name, obj))
                            elif name == 'base_task':
                                filehandle.write(self.pySchema.Schema_Baserate(name, obj))
                        # Write JProcess end
                        filehandle.write(self.pySchema.Schema_JProcessDefEnd())
                    else:
                        if (procname != 'xml') and (procname != 'partition'):
                            # Write JModule functions
                            filehandle.write(self.pySchema.Schema_JModuleFunc(procname, procobj))
                #Write JModule end
                filehandle.write(self.pySchema.Schema_JModuleDefEnd(id))

    def ParseJModule(self, jname, jmoduleobj):
        self.jmodlDictDict = {jname : {}}
        self.jprocDictList = []
        self.jportDict = {}
        # Generate JModules from XML file
        if 'xml' in jmoduleobj.data.keys():
            part = jmoduleobj.data['partition'] if 'partition' in jmoduleobj.data.keys() else None
            xmlobj = JXmlTrans(jmoduleobj.data['xml'], part)
            retflag = xmlobj.GenJModule(jmoduleobj.data)
            if not retflag[0]:
                return
            # Generate Python file for JModule
            self.GenerateJModulePy(jname, jmoduleobj.data, jmoduleobj.id)
        # Loop JProcess
        for procname, procobj in jmoduleobj.data.items():
            #if str(type(procobj)).find('jprocess') != -1:
            if hasattr(procobj, 'type') and procobj.type == 'JProcess':
                # Store info for JProcess {JProcess Name: [name of inport, name of outport, callrate, process]}
                procinfo = {procname : [None, None, 0, None, None]}
                # Loop JPort
                for name, obj in procobj.data.items():
                    #if str(type(obj)).find('jport_in') != -1:
                    if hasattr(obj, 'type') and obj.type == 'JPortIn':
                        # Port In
                        procinfo[procname][1] = procname + '#' + name
                        self.jportDict[procname + '#' + name] = obj.data
                    #elif str(type(obj)).find('jport_out') != -1:
                    elif hasattr(obj, 'type') and obj.type == 'JPortOut':
                        # Port Out
                        procinfo[procname][0] = procname + '#' + name
                        self.jportDict[procname + '#' + name] = obj.data
                    elif name == 'callrate':
                        procinfo[procname][2] = obj
                    elif name == 'process':
                        procinfo[procname][3] = obj
                self.jprocDictList.append(procinfo)
            else:
                self.jmodlDictDict[jname][procname] = procobj

    def GenAudioGraph(self, jfrmobj, folderpath, offset):
        self.agSchema = JAudioGraphSchema()
        # Generate Module_List.c
        #print('Under ' + folderpath + ', Generate Module_List.c....')
        Jlog_print(JAG_GENERAL, 'Under ' + folderpath + ', Generate Module_List.c....')
        dir_name = folderpath + '/' + JAudioGraphSchema.AUDIO_GRAPH
#        dir_name = folderpath + '/' + JAudioGraphSchema.MODULE_LIST
        with open(dir_name, 'w') as filehandle:
            filehandle.write(self.agSchema.Schema_FileTitleComment())
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_FileInfoComment('', 'Implementation of Module List'))
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_ModuleListImplement(self.modulename, self.extraheaderList))
        # Generate Connection_List.c
        #print('Under ' + folderpath + ', Generate Connetion_List.c....')
        Jlog_print(JAG_GENERAL, 'Under ' + folderpath + ', Generate Connetion_List.c....')
#        dir_name = folderpath + '/' + JAudioGraphSchema.CONNECTION_LIST
        with open(dir_name, 'a') as filehandle:
            filehandle.write(self.agSchema.Schema_FileInfoComment('', 'Implementation of Connection List'))
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_ConnectionListImplement(self.graph[offset]['connect'], jfrmobj, []))
        # Generate Execution_List.c
        #print('Under ' + folderpath + ', Generate Execution_List.c....')
        Jlog_print(JAG_GENERAL, 'Under ' + folderpath + ', Generate Execution_List.c....')
#        dir_name = folderpath + '/' + JAudioGraphSchema.EXECUTION_LIST
        with open(dir_name, 'a') as filehandle:
            filehandle.write(self.agSchema.Schema_FileInfoComment('', 'Implementation of Execution List'))
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_ExecutionListImplement(self.graph[offset]['exec'], []))
        # Generate Port_List.c
        #print('Under ' + folderpath + ', Generate Port_List.c....')
        Jlog_print(JAG_GENERAL, 'Under ' + folderpath + ', Generate Port_List.c....')
 #       dir_name = folderpath + '/' + JAudioGraphSchema.PORT_LIST
        with open(dir_name, 'a') as filehandle:
            filehandle.write(self.agSchema.Schema_FileInfoComment('', 'Implementation of Port List'))
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_PortListImplement(self.graph[offset]['connect'], jfrmobj, []))
        # Generate Audio_Graph.c
#        dir_name = folderpath + '/' + JAudioGraphSchema.AUDIO_GRAPH
        with open(dir_name, 'a') as filehandle:
            filehandle.write(self.agSchema.Schema_FileInfoComment('', 'Implementation of Audio Graph'))
            filehandle.write('\n')
            filehandle.write(self.agSchema.Schema_AudioGraphImplement(self.graph[offset]['exec']))


    def GenerateCode(self):
        # Check if JFramework only includes one JModule, and the XML file is in JFramework for all code generation
        # If XML file exist in JFramework rather than JModule, all codes are generated from XML file.
        if (hasattr(self.jfrmobj, 'xml') and hasattr(self.jfrmobj, 'partition')):
            baseid = 0
            # Check PingPong Buffer cross-core supported in XML
            ppFlag = True if (hasattr(self.jfrmobj, 'crosscore') and (self.jfrmobj.crosscore == True)) else False
            if (ppFlag == True) and (not hasattr(self.jfrmobj, 'xml_pingpong')):
                #print('ERROR: The PingPong XML file is not specified. Cannot generate code correctly')
                Jlog_print(JAG_ERROR, 'The PingPong XML file is not specified. Cannot generate code correctly')
                return
            # Fill JFramework info from XML file
            setattr(self.jfrmobj, 'data', {})
            tmpobj = {}
            xmlobj = JXmlTrans(self.jfrmobj.xml, self.jfrmobj.partition)
            jmlinfo = xmlobj.GenJModule(tmpobj) if ppFlag == False else xmlobj.GenJModule(tmpobj, self.jfrmobj.xml_pingpong)
            if not jmlinfo[0]:
                return
            else:
                self.jfrmobj.data[jmlinfo[1]] = jmodule(tmpobj, id=baseid)
            # Add 'sys' JModule, and AudioGraph
            xmlobj.GenSysAndAudioGraph(self.jfrmobj.data, jmlinfo, baseid + 1, 'sys')
            # Generate JFramework Python file
            self.GenerateJFrameworkPy(jmlinfo[1], self.jfrmobj, baseid)
        if (self.jfrmobj.data):
            # Generate code for each JModule
            for jname, jmoduleobj in self.jfrmobj.data.items():
                self.returnList = []
                self.returnCaseList = []
                self.returnportList = []
                # Check the JModule type for Python 3 & 2, since type() reurns in different format
                #if str(type(jmoduleobj)).find('jmodule') != -1 or str(type(jmoduleobj)).find('instance') != -1:
                if hasattr(jmoduleobj, 'type') and jmoduleobj.type == 'JModule':
                    # Create JModule folder
                    jmodule_path = self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR + '/' + jname
                    # Remove the previous files and folders because Windows does not generate new files with same
                    # file name, but only letter case is different.
                    if os.path.exists(jmodule_path):
                        tmpflist = os.listdir(jmodule_path)
                        for ff in tmpflist:
                            os.remove(jmodule_path + '/' + ff)
                        os.rmdir(jmodule_path)
                    os.makedirs(jmodule_path, exist_ok=True)
                    # Extract JModule info
                    self.ParseJModule(jname, jmoduleobj)
                    self.jmlSchema = JModuleSchema(jname, self.schemaConfig)
                    if jname.upper().startswith('SYS'):
                        # JModule for SYS
                        self.jmlSchema.Schema_SetJModuleType(JModuleSchema.JMODULE_SYS)
                    else:
                        # JModule for Model
                        self.jmlSchema.Schema_SetJModuleType(JModuleSchema.JMODULE_MODEL)
                        self.jname = jname
                    # Generate JModule_Adapt_xxx.c
                    #print('Generate JModule_Adapt_' + jname + '.c....')
                    Jlog_print(JAG_GENERAL, 'Generate JModule_Adapt_' + jname + '.c....')
                    file_hname = self.jmlSchema.Schema_GetJModuleAdaptName(jname, '.h')
                    file_cname = self.jmlSchema.Schema_GetJModuleAdaptName(jname, '.c')
                    # Generate JModule_Adapt_xxx.h
                    # Merge JModule_Adapt/Desc .c/.h into JModule_Adapt_xxx.h
                    Jlog_print(JAG_GENERAL, 'Generate JModule_Adapt_' + jname + '.h....')
                    #dir_name = jmodule_path + '/' + file_hname
                    dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR + '/' + file_hname
                    with open(dir_name, 'w') as filehandle:
                        filehandle.write(self.jmlSchema.Schema_JMAdaptImplement(jname, self.jprocDictList, self.jportDict, self.jmodlDictDict, self.returnList, self.returnCaseList))
                        filehandle.write(self.jmlSchema.Schema_JMDescImplement(jname, self.jprocDictList, self.jportDict, self.returnportList))
                    with open(dir_name, 'w') as filehandle:
                        filehandle.write(self.jmlSchema.Schema_FileTitleComment())
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_FileInfoComment(file_hname, ''))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMAdaptHeader(jname))
#                        filehandle.write(self.jmlSchema.Schema_FileTitleComment())
#                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMFileInfoComment(jname, '.h'))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMDescHeader(jname, len(self.jprocDictList), self.returnList, self.returnCaseList, self.returnportList))
                    #dir_name = jmodule_path + '/' + file_cname
                    with open(dir_name, 'a') as filehandle:
                        filehandle.write(self.jmlSchema.Schema_FileInfoComment(file_hname, ''))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMAdaptImplement(jname, self.jprocDictList, self.jportDict, self.jmodlDictDict, self.returnList, self.returnCaseList))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMFileInfoComment(jname, '.c'))
                        filehandle.write('\n\n')
                        #include header files
                        for item in self.desc_header:
                            filehandle.write(self.jmlSchema.Schema_IncludeHeaderFile(item, '"'))
                        #filehandle.write(self.jmlSchema.Schema_IncludeHeaderFile(file_hname, '"'))
                        filehandle.write('\n\n')
                        # Write all ports and process info
                        filehandle.write(self.jmlSchema.Schema_JMDescImplement(jname, self.jprocDictList, self.jportDict, self.returnportList))
                    self.modulename.append(jname)
                    self.extraheaderList.append(file_hname)
                elif jname.upper().startswith('SYS') and (jmoduleobj == None):
                    # This branch deals with 'sys'=None in JConfig.py, and the empty files are generated.
                    jmodule_path = self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR + '/' + jname
                    if not os.path.exists(jmodule_path):
                        os.makedirs(jmodule_path, exist_ok=True)
                    self.jmlSchema = JModuleSchema(jname, self.schemaConfig)
                    # JModule for SYS
                    self.jmlSchema.Schema_SetJModuleType(JModuleSchema.JMODULE_SYS)
                    # Generate JModule_Adapt_sys.c
                    #print('Generate JModule_Adapt_' + jname.capitalize() + '.c....')
                    Jlog_print(JAG_GENERAL, 'Generate JModule_Adapt_' + jname.capitalize() + '.c....')
                    file_hname = self.jmlSchema.Schema_GetJModuleAdaptName(jname, '.h')
                    file_cname = self.jmlSchema.Schema_GetJModuleAdaptName(jname, '.c')
                    dir_name = jmodule_path + '/' + file_cname
                    with open(dir_name, 'w') as filehandle:
                        filehandle.write(self.jmlSchema.Schema_FileTitleComment())
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_FileInfoComment(file_hname, ''))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMFileInfoComment(jname, '.c'))
                        filehandle.write('\n\n')
                        #include header files
                        for item in self.desc_header:
                            filehandle.write(self.jmlSchema.Schema_IncludeHeaderFile(item, '"'))
                        filehandle.write(self.jmlSchema.Schema_IncludeHeaderFile(file_hname, '"'))
                        filehandle.write('\n\n')
                    # Generate JModule_Adapt_sys.h
                    #print('Generate JModule_Adapt_' + jname.capitalize() + '.h....')
                    Jlog_print(JAG_GENERAL, 'Generate JModule_Adapt_' + jname.capitalize() + '.h....')
                    dir_name = jmodule_path + '/' + file_hname
                    with open(dir_name, 'w') as filehandle:
                        filehandle.write(self.jmlSchema.Schema_FileTitleComment())
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_FileInfoComment(file_hname, ''))
                        filehandle.write('\n')
                        filehandle.write(self.jmlSchema.Schema_JMFileInfoComment(jname, '.h'))
                        filehandle.write('\n')
                    self.sysflag = False

            # After all JModule files are generated, generate connection and exec for Audio_Graph
            self.graph = self.jfrmobj.data['graph']
            if (len(self.graph) == 1):
                path = self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR
                if not os.path.exists(path):
                    os.makedirs(path, exist_ok=True)
                self.GenAudioGraph(self.jfrmobj.data, path, 0)
            else:
                cnt = 0
                for item in self.graph:
                    path = self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR + str(cnt)
                    if not os.path.exists(path):
                        os.makedirs(path, exist_ok=True)
                    self.GenAudioGraph(self.jfrmobj.data, path, cnt)
                    cnt += 1

            self.jfmrSchema.Schema_SetJModuleName(self.jname)
            # Generate JModule_List.c
            #print('Generate JModule_List.c....')
            Jlog_print(JAG_GENERAL, 'Generate JModule_List.c....')
            # Merge JModule_Listc.c into Audiograph.c
            dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR + '/' + JAudioGraphSchema.AUDIO_GRAPH
#            dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR + '/' + self.jfmrSchema.Schema_GetJMListCfileName()
            with open(dir_name, 'a') as filehandle:
                filehandle.write(self.jfmrSchema.Schema_FileInfoComment('', 'Implementation of the Class JModule_List'))
                filehandle.write('\n')
                filehandle.write(self.jfmrSchema.Schema_IncludeHeaderFile('JModule_List.h', '"'))
                filehandle.write('\n\n')
                filehandle.write(self.jfmrSchema.Schema_JModuleListImplement(self.modulename))
            # TODO dummy JModule_List.c
            dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR + '/' + self.jfmrSchema.Schema_GetJMListCfileName()
            with open(dir_name, 'w') as filehandle:
                filehandle.write(self.jfmrSchema.Schema_FileTitleComment())

            # Generate _JModule_Id.h
            #print('Generate _JModule_Id.h....')
            Jlog_print(JAG_GENERAL, 'Generate _JModule_Id.h....')
            dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_AUDIOGRDIR + '/' + self.jfmrSchema.Schema_GetJMIDHeaderfileName()
            #dir_name = self.baf_rootpath + JFrameworkBuilder.DEFAULT_JMODULEDIR + '/' + self.jfmrSchema.Schema_GetJMIDHeaderfileName()
            with open(dir_name, 'a') as filehandle:
                filehandle.write(self.jfmrSchema.Schema_FileTitleComment())
                filehandle.write('\n')
                filehandle.write(self.jfmrSchema.Schema_FileInfoComment(self.jfmrSchema.Schema_GetJMIDHeaderfileName(), 'Implementation of the Enumeration _JModule_Id'))
                filehandle.write('\n')
                filehandle.write(self.jfmrSchema.Schema_JModuleIdImplement(self.modulename))

    def H_GenerateFile(self):
        #print('In function H_GenerateFile')
        Jlog_print(JAG_GENERAL, 'In function H_GenerateFile')

    def C_GenerateFile(self):
        #print('In function C_GenerateFile')
        Jlog_print(JAG_GENERAL, 'In function C_GenerateFile')



class JProbeJModuleBuilder(JCBuilder):
    DEFAULT_JMODULEDIR = 'JModule_Lib'
    DEFAULT_AUDIOGRDIR = 'Audio_Graph'
    def __init__(self, filepath=None, modelname=None):
        self.filepath = filepath
        self.modelFs = 32
        self.modelCh = 8
        self.schema = JProbeJModuleSchema(modelname)

    def AddSingleProbetoJModule(self, jmodule_from_file, funcprototype, func_str):
        state = 0
        with open(self.filepath, 'w+') as filehandle:
            for loc, item in enumerate(jmodule_from_file):
                filehandle.write(item)
                # Add Header files and export buffer function
                if state == 0 and '#include' in item:
                    filehandle.write(self.schema.Schema_AddModelHeaders())
                    filehandle.write(self.schema.Schema_AddExportBufFunc(funcprototype))
                    state = 1
                if state == 1 and 'void * src = port_in_ctx' in item:
                    filehandle.write(self.schema.Schema_AddFillBuffer(func_str, self.modelFs, self.modelCh))
                    state = 2

    def H_GenerateFile(self):
        #print('In function H_GenerateFile')
        Jlog_print(JAG_GENERAL, 'In function H_GenerateFile')

    def C_GenerateFile(self):
        #print('In function C_GenerateFile')
        Jlog_print(JAG_GENERAL, 'In function C_GenerateFile')


class JCaPIv2Builder(JCBuilder):
    CAPIV2_CONFIG='data/baf.cfg'
    CAPIV2_CFILE = 'src/capi_v2_baf.c'
    def __init__(self, genpath=None, model_name=None):
        self.genpath = genpath
        self.modelname = model_name

    def GenerateCode(self, capiv2Dict=None):
        if os.path.exists(self.genpath):
            # Write Configuration file
            configfile = os.path.join(self.genpath, JCaPIv2Builder.CAPIV2_CONFIG)
            with open(configfile, 'r') as fh:
                cfg = fh.read()
            os.remove(configfile)
            numbits = int(capiv2Dict['samplesize']) * 8
            ncfg = cfg.replace('xxxchannelinxxx', capiv2Dict['channelinmap']).replace('xxxbitspersamplexxx', str(numbits))
            ncfg = ncfg.replace('xxxsamplerateinxxx', capiv2Dict['sampleratein']).replace('xxxsamplerateoutxxx', capiv2Dict['samplerateout'])
            ncfg = ncfg.replace('xxxchanneloutxxx', capiv2Dict['channeloutmap'])
            with open(configfile, 'w') as fh:
                fh.write(ncfg)
            # Write C file
            cfile = os.path.join(self.genpath, JCaPIv2Builder.CAPIV2_CFILE)
            with open(cfile, 'r') as fh:
                ccode = fh.read()
            os.remove(cfile)
            nccode = ccode.replace('xxxcapiv2_stacksizexxx', capiv2Dict['capiv2_stacksize']).replace('xxxbaf_heapsizexxx', capiv2Dict['baf_heapsize'])
            nccode = nccode.replace('xxxstep_stacksizexxx', capiv2Dict['step_stacksize']).replace('xxxnum_stepxxx', capiv2Dict['num_step'])
            nccode = nccode.replace('xxxchannelinxxx', capiv2Dict['channelin']).replace('xxxchanneloutxxx', capiv2Dict['channelout'])
            nccode = nccode.replace('xxxsamplesizexxx', capiv2Dict['samplesize']).replace('xxxframesizexxx', capiv2Dict['framesize'])
            nccode = nccode.replace('xxxmodel_cfilexxx', capiv2Dict['model_cfile']).replace('xxxbase_task_idxxx', capiv2Dict['base_task_id'])
            with open(cfile, 'w') as fh:
                fh.write(nccode)
        else:
            Jlog_print(JAG_ERROR, 'The Path ' + self.path + ' does not exist.')


