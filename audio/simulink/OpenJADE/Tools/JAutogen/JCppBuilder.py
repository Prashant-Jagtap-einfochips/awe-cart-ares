import sys
import os

from JSchemaConfig import *
from JCppSchema import *
from JXmlSerial import *
from JBamProxy import *
from JFeature import *
from JLogPrint import *
from datetime import date


FEATURELIB = 1
FEATUREMODULE = 2
BAMMODULE = 3
BAMPROXYBASE = 4
FEATUREBASE = 5




CommonHeader = ['#include <stdlib.h>', '#include <string.h>', '#include <stdint.h>', '#include <stddef.h>', '#include "OJPAL.h"']

FeatureLib_Func = ['RET_CODE_TYPE FL_Initialize_Features(void);']
#FeatureModule_func = ['void SetInstanceNumber(int n);', 'int GetInstanceNumber(){ return mInstanceNumber; };',  'virtual RET_CODE_TYPE OJInitialize();']
FeatureModule_func = ['virtual RET_CODE_TYPE OJInitialize();']
BamproxyModule_func = ['//virtual RET_CODE_TYPE Response();']

FeatureModule_Var = [#['Instance Number', 'int mInstanceNumber;'],
                     #['Feature Name', 'char mFeatureName[64];'],
                     ['Number of Channel In', 'int num_in;'],
                     ['Number of Channel out', 'int num_out;']]

BamproxyModule_Var = [['Bam Discriptor', 'BAMF_DESCRIPTOR mBamf;']]

class JCppBuilder:
    def __init__(self, h_filename=None, cpp_filename=None, genpath=None, cpp_type=FEATURELIB, genConfigFile=None):
        self.h_filename = h_filename
        self.cpp_filename = cpp_filename
        self.cpptype = cpp_type
        self.genConfigFile = genConfigFile
        self.rtcCount = 0
        self.topCount = 0
        self.topname = ''
        self.classname = ''
        self.rawname = ''
        self.appname = ''
        self.includeheader = []
        self.inherincludeheader = []
        self.inhercppheader = []
        self.privatevar = []
        self.protectvar = []
        self.publicvar = []
        self.privatefunc = []
        self.protectfunc = []
        self.publicfunc = []
        self.feagetptrfunc = []
        self.EnumBam = {}
        self.EnumChInfo = {}
        self.EnumBamAction = []
        self.EnumRtcAction = []
        self.EnumTopAction = []
        self.schemaConfig = JFLSchemaConfig(self.genConfigFile)
        if (genpath is not None):
            self.path = genpath if genpath[:-1] == '/' else genpath + '/'
        else:
            self.path = './autogen/'
        if not os.path.exists(self.path):
            os.makedirs(self.path, exist_ok=True)
        if (cpp_type == FEATURELIB):
            self.h_filename = 'OJFeatureLib.h'
            self.cpp_filename = 'OJFeatureLib.cpp'
            self.h_inherfilename = ''
            self.cpp_inherfilename = ''
            self.Schema = JCppFeatureLibSchema()
            hl = self.schemaConfig.GetCommonHeaderFile('FeatureLib')
            self.FeatureLib_standardheader = hl if hl != None else [x for x in CommonHeader]
        elif (cpp_type == FEATUREMODULE):
            self.h_filename = 'OJ' + h_filename + 'Feature.h'
            self.cpp_filename = 'OJ' + cpp_filename + 'Feature.cpp'
            self.h_inherfilename = h_filename + 'Feature.h'
            self.cpp_inherfilename = cpp_filename + 'Feature.cpp'
            self.Schema = JCppFeatureSchema()
            #self.includeheader.append('#include ' + '"' + JFeature.NAME + '.h' + '"')
            self.includeheader.append('#include ' + '"' + 'Feature.h' + '"')
            self.inherincludeheader.append('#include ' + '"' + 'Feature.h' + '"')
            self.inherincludeheader.append('#include ' + '"' + self.h_filename + '"')
            self.inhercppheader.append('#include ' + '"' + self.h_inherfilename + '"')
            hl = self.schemaConfig.GetCommonHeaderFile('Feature')
            self.FeatureModule_standardheader = hl if hl != None else [x for x in CommonHeader]
        elif (cpp_type == BAMMODULE):
            self.fnList = None
            self.h_filename = 'OJ' + h_filename + 'BamProxy.h'
            self.cpp_filename = 'OJ' + cpp_filename + 'BamProxy.cpp'
            self.h_inherfilename = h_filename + 'BamProxy.h'
            self.cpp_inherfilename = cpp_filename + 'BamProxy.cpp'
            self.Schema = JCppBamSchema()
            self.includeheader.append('#include ' + '"' + 'BamProxy.h' + '"')
            self.includeheader.append('#include ' + '"' + 'OJBamProxy.h' + '"')
            self.inherincludeheader.append('#include ' + '"' + 'BamProxy.h' + '"')
            self.inherincludeheader.append('#include ' + '"' + self.h_filename + '"')
            self.inhercppheader.append('#include ' + '"' + self.h_inherfilename + '"')
            self.inhercppheader.append('#include ' + '"' + 'Feature_Utils.h' + '"')
            self.inhercppheader.append('#include ' + '"' + 'Feature_Controller.h' + '"')
            #self.protectfunc.append('virtual RET_CODE_TYPE Initialize();')
            self.publicfunc.append('        virtual RET_CODE_TYPE Initialize();\n')
            hl = self.schemaConfig.GetCommonHeaderFile('Bamproxy')
            self.BamModule_standardheader = hl if hl != None else [x for x in CommonHeader]
        elif (cpp_type == BAMPROXYBASE):
            self.ojbp = JBamProxy()
        elif (cpp_type == FEATUREBASE):
            self.ojfea = JFeature()

    def CppDump(self):
        Jlog_print(JAG_GENERAL, 'Header File: ' + self.h_filename)
        Jlog_print(JAG_GENERAL, 'Cpp File: ' + self.cpp_filename)
        Jlog_print(JAG_GENERAL, 'Class Name: ' + self.classname)
        Jlog_print(JAG_GENERAL, 'Header Files:')
        for item in self.includeheader:
            Jlog_print(JAG_GENERAL, item)
        Jlog_print(JAG_GENERAL, 'Private Variables: ')
        for item in self.privatevar:
            Jlog_print(JAG_GENERAL, item)
        Jlog_print (JAG_GENERAL, 'Public Functions: ')
        for item in self.publicfunc:
            Jlog_print(JAG_GENERAL, item)

    def Cpp_SetTopname(self, name=None):
        #self.topname = name
        self.topname = ''

    def Cpp_SetClassname(self, name=None, appname=None, rtc=None, top=None):
        if (self.cpptype == FEATURELIB):
            self.classname = 'OJ' + name
        elif (self.cpptype == FEATUREMODULE):
            self.classname = 'OJ' + name + 'Feature'
            self.rawname = 'OJ' + name
            self.FeatureModule_standardheader.append('#include ' + '"' + appname + '_RTC_IDs.h' + '"')
            self.FeatureModule_standardheader.append('#include ' + '"' + appname + '_TOP_IDs.h' + '"')
        elif (self.cpptype == BAMMODULE):
            self.classname = 'OJ' + name + 'BamProxy'
            self.rawname = 'OJ' + name
            self.appname = appname
            self.xmlser = JXmlSerial(self.appname, rtc=rtc, top=top)
        else:
            raise('!!! Set Class Name error Because of the wrong cpptype in JCppBuilder')

    def Cpp_AddPrivateVar(self, vartype, varname, vardata=None):
        varFormat = vartype + ' ' + varname
        if vardata is not None:
            varFormat += ' = ' + str(vardata)
        varFormat += ';'
        self.privatevar.append(varFormat)

    def Cpp_AddHeaderByName(self, mname):
        # Rule: #include "mnameFeature.h"
        if mname is not None:
            self.includeheader.append('#include ' + '"' +  self.Schema.Schema_HeaderNameConvert(mname) + ".h" + '"')

    def Cpp_AddPrivateVarByName(self, mname, count):
        if mname is not None:
            ss = '        ' + self.Schema.Schema_FuncVarTypeConvert(mname)
            if (count == 1):
                ss += ' ' + self.Schema.Schema_VarNameConvert(mname) + ';\n'
            else:
                ss += ' ' + self.Schema.Schema_VarNameConvert(mname) + '[' + str(count) + ']' + ';\n'
            self.privatevar.append(ss)

    def Cpp_AddPublicFuncByName(self, mname, count):
        if mname is not None:
            if (self.cpptype == FEATURELIB):
                ss = '        ' + self.Schema.Schema_FuncVarTypeConvert(mname) + ' '
                ss += self.Schema.Schema_FuncNameConvert(mname) + ' '
                ss += '{ return '
                if (count == 1):
                    ss += '&' + self.Schema.Schema_VarNameConvert(mname) + ';}\n'
                else:
                    ss += self.Schema.Schema_VarNameConvert(mname) + ';}\n'
            elif (self.cpptype == FEATUREMODULE):
                self.Schema.Schema_AddBamCount(mname, count)
                ss = '        void ' + self.Schema.Schema_FuncNameConvert(self.classname, mname) + '(void);\n'
            elif (self.cpptype == BAMMODULE):
                #print('Add function for BAM Proxy')
                Jlog_print(JAG_GENERAL, 'Add function for BAM Proxy')
            else:
                #raise('!!! The cpptype is incorrect when calling AddPublicFuncByName in JCppBuilder')
                Jlog_print(JAG_ERROR, 'The cpptype is incorrect when calling AddPublicFuncByName in JCppBuilder')
            self.publicfunc.append(ss)
            # Add GetPtr(..) function for each BamProxy Instance in Feature
            if (self.cpptype == FEATUREMODULE):
                ss = self.Schema.Schema_GetBamPtr(self.classname, mname, count)
                self.feagetptrfunc.append(ss)

    def Cpp_AddSchemaBamFeature(self, bamproxynameDict, featurenameList, featoprtcDict):
        self.Schema.Schema_AddBamFeature(bamproxynameDict, featurenameList, featoprtcDict)

    def Cpp_AddSchemaBamFeatureImpl(self, bamnameImpleDict, featurenameImpleDict):
        self.Schema.Schema_AddBamFeatureImple(bamnameImpleDict, featurenameImpleDict)

    def Cpp_AddBamActionChannel(self, bam, fea, bamnameDict, feanameDict, feadata, appdata):
        if (len(bamnameDict[bam]) > 1):
            self.EnumBam[bam] = bamnameDict[bam]
        # Enumerate all RTC, TOP ID from each BAM proxy in each Feature
        # If bam == fea, the RTC or TOP is under Feature
        bamobj = feadata.data[bamnameDict[bam][0]] if bam != fea else feadata
        if (('RTC' in bamobj.data) and ('TOP' in bamobj.data)):
            self.Schema.Schema_AddActionId(bam, bamobj.data['RTC'], bamobj.data['TOP'])
        elif ('RTC' in bamobj.data):
            self.Schema.Schema_AddActionId(bam, bamobj.data['RTC'], None)
        elif ('TOP' in bamobj.data):
           self.Schema.Schema_AddActionId(bam, None, bamobj.data['TOP'])
        # Enumerate channel_in, channel_out for each Bamtype
        for i in range(len(feanameDict[fea])):
            feaobj = appdata[feanameDict[fea][i]]
            if bam == fea:
                bamobj = feaobj
                if ('num_chan' in bamobj.data):
                    ch_in = ch_out = bamobj.data['num_chan']
                else:
                    ch_in = bamobj.data['num_in'] if 'num_in' in bamobj.data else 0
                    ch_out = bamobj.data['num_out'] if 'num_out' in bamobj.data else 0
                if fea not in self.EnumChInfo.keys():
                    self.EnumChInfo[fea] = {}
                if feanameDict[fea][i] not in self.EnumChInfo[fea].keys():
                    self.EnumChInfo[fea][feanameDict[fea][i]] = {}
                if fea not in self.EnumChInfo[fea][feanameDict[fea][i]].keys():
                    self.EnumChInfo[fea][feanameDict[fea][i]][fea] = [ch_in, ch_out]
            else:
                for item in bamnameDict[bam]:
                    bamobj = feaobj.data[item]
                    if ('num_chan' in bamobj.data):
                        ch_in = ch_out = bamobj.data['num_chan']
                    else:
                        ch_in = bamobj.data['num_in'] if 'num_in' in bamobj.data else 0
                        ch_out = bamobj.data['num_out'] if 'num_out' in bamobj.data else 0
                    if fea not in self.EnumChInfo.keys():
                        self.EnumChInfo[fea] = {}
                    if feanameDict[fea][i] not in self.EnumChInfo[fea].keys():
                        self.EnumChInfo[fea][feanameDict[fea][i]] = {}
                    if fea not in self.EnumChInfo[fea][feanameDict[fea][i]].keys():
                        self.EnumChInfo[fea][feanameDict[fea][i]][item] = [ch_in, ch_out]

    def Cpp_AddEnumActionVarFunc(self, bamobj, bamname, feartctopDict):
        # If RTC&TOP is under Feature
        if bamname in feartctopDict.keys():
            prefix = feartctopDict[bamname][0]
        else:
            # If the code is generated from XML, bamobj.type is in tuple
            prefix = bamobj.type[0].upper() + '_' if type(bamobj.type) == tuple else bamobj.type.upper() + '_'
        # Here we use its bamname only for searching payload info in <Model>_RTC_Payloads.xml
        # One feature may includes the same bam twice or more times with different bamname, but
        # The payload is the same from <Model>_RTC_Payloads.xml, in which it appears more times
        # with different bamname. However, for bamproxy, we only need to remember it once.
        rtcname = bamname.upper() + '_'
        if ('RTC' in bamobj.data):
            for item in bamobj.data['RTC']:
                self.EnumBamAction.append(prefix + item.upper())
                self.EnumRtcAction.append(rtcname + item.upper())
                self.rtcCount += 1
        if ('TOP' in bamobj.data):
            for item in bamobj.data['TOP']:
                self.EnumBamAction.append(prefix + item.upper() + '_SET')
                self.EnumBamAction.append(prefix + item.upper() + '_GET')
                self.EnumTopAction.append(prefix + item.upper())
                self.topCount += 1

    def Cpp_GenerateHeaderBamProxyBase(self):
        self.ojbp.OJBP_HeaderGen(self.path)

    def Cpp_GenerateCppBamProxyBase(self):
        self.ojbp.OJBP_CppGen(self.path)

    def Cpp_GenerateHeaderFeatureBase(self):
        self.ojfea.OJFEA_HeaderGen(self.path)

    def Cpp_GenerateCppFeatureBase(self):
        self.ojfea.OJFEA_CppGen(self.path)

    def Cpp_GenerateHeadFile(self):
        incpath = self.path + 'inc/'
        if not os.path.exists(incpath):
            os.makedirs(incpath, exist_ok=True)
        # Generate OJxxx Feature/Bamproxy .h File
        with open(incpath + self.h_filename, 'w') as filehandle:
            #print('Generating Header File: ' + self.h_filename + ' ....')
            Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.h_filename + ' ....')
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.h_filename))
            filehandle.write('\n\n\n\n')
            # Header #ifndef
            filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.h_filename[:-2].upper()))
            # Include Header Files
            filehandle.write(self.Schema.Schema_FileHeaderComment())
            for item in self.includeheader:
                filehandle.write(item + '\n')
            filehandle.write('\n\n\n\n')
            # Definition
            if (self.cpptype == FEATUREMODULE):
                if self.rawname[2:] in self.EnumChInfo:
                    for feaName, v in self.EnumChInfo[self.rawname[2:]].items():
                        for bm, chlist in v.items():
                            filehandle.write(self.Schema.Schema_DefineChannel(self.rawname[2:], feaName, bm, chlist))
                filehandle.write('\n\n')
            elif (self.cpptype == FEATURELIB):
                filehandle.write(self.Schema.Schema_DefineRtcTopCount(self.rtcCount, self.topCount))
            # BAM Enumeration
            if (self.cpptype == FEATUREMODULE):
                if (bool(self.EnumBam)):
                    filehandle.write(self.Schema.Schema_Enum(self.rawname, self.EnumBam))
                    filehandle.write('\n\n')
                filehandle.write(self.Schema.Schema_FormatRTCTOP(self.topname, self.rawname))
            elif (self.cpptype == BAMMODULE):
                if (self.EnumBamAction):
                    filehandle.write(self.Schema.Schema_BamEnum(self.rawname, self.EnumBamAction))
                    filehandle.write('\n\n')
            # Class Comment
            filehandle.write(self.Schema.Schema_ClassInfoComment(self.classname))
            # Declare Class
            if (self.cpptype == FEATURELIB):
                filehandle.write(self.Schema.Schema_ClassDef(self.classname))
            elif (self.cpptype == FEATUREMODULE):
                #filehandle.write(self.Schema.Schema_ClassDef(self.classname, 'public', JFeature.NAME))
                filehandle.write(self.Schema.Schema_ClassDef(self.classname, 'public', 'Feature'))
            elif (self.cpptype == BAMMODULE):
                filehandle.write(self.Schema.Schema_ClassDef(self.classname, 'public', 'OJBamProxy'))
            filehandle.write(JCppSchema.ClassDefBegin)
            # Public section in class
            filehandle.write(JCppSchema.ClassPublicSection)
            for item in self.publicvar:
                filehandle.write(item + '\n')
            if (self.cpptype == FEATURELIB):
                for item in FeatureLib_Func:
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                    filehandle.write('        ' + item + '\n\n')
            elif (self.cpptype == FEATUREMODULE):
                for item in FeatureModule_func:
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                    filehandle.write('        ' + item + '\n\n')
                for item in self.feagetptrfunc:
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                    filehandle.write('        ' + item + '\n\n')
            elif (self.cpptype == BAMMODULE):
                for item in BamproxyModule_func:
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                    filehandle.write('        ' + item + '\n\n')
                # Write ActionAssignID function declaration
                if (self.EnumBamAction):
                    filehandle.write(self.Schema.Schema_ActionAssignDeclare(self.rawname))
                    filehandle.write('\n')
                # Write RTC function declaration
                bamrtcfunc = {}
                for item in self.EnumRtcAction:
                    bamrtcfunc = self.xmlser.Jxs_RtcParseAll(item, self.fnList)
                    for key, value in bamrtcfunc.items():
                        self.publicfunc.append(self.Schema.Schema_RtcFuncDeclare(self.rawname, item, key, value))
                # Write TOP function declaration
                #bamtopfunc = {}
                for item in self.EnumTopAction:
                    # No need to extract infr from Model_TOP_Tokens.xml
                    #bamtopfunc = self.xmlser.Jxs_TopParseAll(item)
                    self.publicfunc.append(self.Schema.Schema_TopFuncDeclare(item))
            for item in self.publicfunc:
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                filehandle.write(item + '\n')
            filehandle.write('\n')
            # Protect section in class
            filehandle.write(JCppSchema.ClassProtectedSection)
            for item in self.protectvar:
                filehandle.write(item + '\n')
            for item in self.protectfunc:
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader(JCppSchema.ClassAlign1st, self.classname))
                filehandle.write('        ' + item + '\n')
            filehandle.write('\n')
            # Private section in class
            filehandle.write(JCppSchema.ClassPrivateSection)
            if (self.cpptype == FEATUREMODULE):
                for item in FeatureModule_Var:
                    filehandle.write(self.Schema.Schema_FeatureVarComment(None, item[0]))
                    filehandle.write('        ' + item[1] + '\n\n')
            elif (self.cpptype == BAMMODULE):
                if (self.EnumBamAction):
                    filehandle.write(self.Schema.Schema_FeatureVarComment(None, 'Action ID'))
                    filehandle.write(self.Schema.Schema_ActionIDDeclare(self.rawname))
            for item in self.privatevar:
                filehandle.write(self.Schema.Schema_FeatureVarComment(item))
                filehandle.write(item + '\n')
            for item in self.privatefunc:
                filehandle.write(item + '\n')
            filehandle.write('\n')
            filehandle.write(JCppSchema.ClassDefEnd)
            # FeatureLib Specified
            if (self.cpptype == FEATURELIB):
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinHeader('', self.classname))
            filehandle.write('\n\n\n\n')
            # Header #endif
            filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.h_filename[:-2].upper()))
        # Generate xxx Feature/Bamproxy .h File, if file doesn't exit
        if (not os.path.exists(incpath + self.h_inherfilename)):
            with open(incpath + self.h_inherfilename, 'w') as filehandle:
                #print('Generating Header File: ' + self.h_inherfilename + ' ....')
                Jlog_print(JAG_GENERAL, 'Generating Header File: ' + self.h_inherfilename + ' ....')
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                if (self.cpptype == FEATUREMODULE):
                    filehandle.write(self.Schema.Schema_SchemaFeaturehFileInfoComment(self.h_inherfilename, self.rawname))
                else:
                    filehandle.write(self.Schema.Schema_SchemaBamproxyhFileInfoComment(self.h_inherfilename, self.rawname))
                filehandle.write('\n\n\n\n')
                # Header #ifndef
                filehandle.write(self.Schema.Schema_IfdefStatementBegin(self.h_inherfilename[:-2].upper()))
                # Include Header Files
                filehandle.write(self.Schema.Schema_FileHeaderComment())
                for item in self.inherincludeheader:
                    filehandle.write(item + '\n')
                filehandle.write('\n\n')
                # Class Comment
                filehandle.write(self.Schema.Schema_ClassInfoComment(self.classname[2:]))
                filehandle.write(self.Schema.Schema_ClassDef(self.classname[2:], 'public', self.classname))
                filehandle.write(JCppSchema.ClassDefBegin)
                # Public section in class
                filehandle.write('    ' + JCppSchema.ClassPublicSection)
                filehandle.write('      ' + self.Schema.Schema_PublicMemberComment())
                #filehandle.write('      ' + self.Schema.Schema_Constructor(self.classname[2:]))
                filehandle.write('\n\n')
                # Protect section in class
                filehandle.write('    ' + JCppSchema.ClassProtectedSection)
                filehandle.write('      ' + self.Schema.Schema_ProtectedMemberComment())
                #filehandle.write('      virtual RET_CODE_TYPE Initialize(void);\n')

                filehandle.write('\n\n')
                # Private section in class
                filehandle.write('    ' + JCppSchema.ClassPrivateSection)
                filehandle.write('      ' + self.Schema.Schema_PraviteMemberComment())
                filehandle.write('\n\n')
                filehandle.write(JCppSchema.ClassDefEnd)
                filehandle.write('\n\n')
                # Header #endif
                filehandle.write(self.Schema.Schema_IfdefStatementEnd(self.h_inherfilename[:-2].upper()))

    def Cpp_GenerateCppFile(self):
        #print('Generating CPP File: ' + self.cpp_filename + ' ....')
        Jlog_print(JAG_GENERAL, 'Generating CPP File: ' + self.cpp_filename + ' ....')
        srcpath = self.path + 'src/'
        if not os.path.exists(srcpath):
            os.makedirs(srcpath, exist_ok=True)
        # Generate OJxxx Feature/Bamproxy .cpp File
        with open(srcpath + self.cpp_filename, 'w') as filehandle:
            #print('Generating CPP File: ' + self.cpp_filename + ' ....')
            Jlog_print(JAG_GENERAL, 'Generating CPP File: ' + self.cpp_filename + ' ....')
            # File Title
            filehandle.write(self.Schema.Schema_FileTitleComment())
            filehandle.write('\n')
            # File Comment
            filehandle.write(self.Schema.Schema_FileInfoComment(self.cpp_filename))
            filehandle.write('\n\n\n\n')
            # Include Header Files
            filehandle.write(self.Schema.Schema_FileHeaderComment())
            if (self.cpptype == FEATURELIB):
                for item in self.FeatureLib_standardheader:
                    filehandle.write(item + '\n')
            elif (self.cpptype == FEATUREMODULE):
                for item in self.FeatureModule_standardheader:
                    filehandle.write(item + '\n')
            elif (self.cpptype == BAMMODULE):
                for item in self.BamModule_standardheader:
                    filehandle.write(item + '\n')
            filehandle.write('#include ' + '"' + self.h_filename + '"')
            filehandle.write('\n\n\n\n')
            # Public Function Comment
            filehandle.write(self.Schema.Schema_PublicFuncComment(self.classname))
            if (self.cpptype == FEATURELIB):
                #filehandle.write(self.Schema.Schema_FuncGetInstance(self.classname))
                filehandle.write('\n\n')
                filehandle.write(self.Schema.Schema_FuncInitialize(self.classname, self.privatevar))
            elif (self.cpptype == FEATUREMODULE):
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Initialization function for ' + self.classname[:-7], 'None', 'RET_CODE_TYPE to indicate the SUCCESS or ERROR'))
                filehandle.write(self.Schema.Schema_FuncInitialize(self.classname, self.publicfunc))
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Set Instance Number for ' + self.classname[:-7], 'None', 'None'))
                #filehandle.write(self.Schema.Schema_SetInstance(self.classname))
                # Write All Bam Setup Functions
                for item in self.publicfunc:
                    # Get BamProxy name
                    tmp = item[item.rfind(' ') + 1:]
                    tmp = tmp[len(self.rawname):]
                    tmp = tmp[:tmp.find('BAMSetup')]
                    # if RTC or TOP under Feature, tmp equals '', then assign Feature name to tmp as BamProxy name
                    if tmp == '':
                        tmp = self.rawname[2:]
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Set Action ID for ' + tmp + ' BAM', 'None', 'None'))
                    filehandle.write(self.Schema.Schema_SetBamSetup(self.classname, self.topname, item, tmp, self.rawname, self.EnumChInfo))
            elif (self.cpptype == BAMMODULE):
                filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Initialization function for ' + self.classname, 'None', 'RET_CODE_TYPE to indicate the SUCCESS or ERROR'))
                filehandle.write(self.Schema.Schema_FuncInitialize(self.classname, self.EnumBamAction))
                if (self.EnumBamAction):
                    filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Assign Action ID for  ' + self.classname, 'None', 'None'))
                    filehandle.write(self.Schema.Schema_AssignActionID(self.rawname, self.classname))
                    filehandle.write('\n')
                # Write RTC function implementation
                bamrtcfunc = {}
                rtccount = 0
                for item in self.EnumRtcAction:
                    bamrtcfunc = self.xmlser.Jxs_RtcParseAll(item, self.fnList)
                    for key, value in bamrtcfunc.items():
                        filehandle.write(self.Schema.Schema_RtcFuncImplement(self.classname, self.rawname, item, key, self.EnumBamAction[rtccount], value))
                        filehandle.write('\n')
                    rtccount += 1
                # Write TOP function implementation
                for item in self.EnumTopAction:
                    filehandle.write(self.Schema.Schema_TopFuncImplement(self.classname, item))
                    filehandle.write('\n')
            filehandle.write('\n')
        # Generate xxx Feature/Bamproxy .cpp File, if file doesn't exit
        if (not os.path.exists(srcpath + self.cpp_inherfilename)):
            with open(srcpath + self.cpp_inherfilename, 'w') as filehandle:
                #print('Generating CPP File: ' + self.cpp_inherfilename + ' ....')
                Jlog_print(JAG_GENERAL, 'Generating CPP File: ' + self.cpp_inherfilename + ' ....')
                # File Title
                filehandle.write(self.Schema.Schema_FileTitleComment())
                filehandle.write('\n')
                # File Comment
                if (self.cpptype == FEATUREMODULE):
                    filehandle.write(self.Schema.Schema_SchemaFeaturecppFileInfoComment(self.cpp_inherfilename, self.rawname))
                else:
                    filehandle.write(self.Schema.Schema_SchemaBamproxycppFileInfoComment(self.cpp_inherfilename, self.rawname))
                filehandle.write('\n\n')
                filehandle.write(self.Schema.Schema_FileLogComment(self.cpp_inherfilename))
                filehandle.write('\n\n')
                # Include Header Files
                filehandle.write(self.Schema.Schema_FileHeaderComment())
                for item in self.inhercppheader:
                    filehandle.write(item + '\n')
                filehandle.write('\n\n\n\n')
                # Functions
                filehandle.write(self.Schema.Schema_ModuleFunctionComment())
                filehandle.write('\n\n')
                #filehandle.write(self.Schema.Schema_ClassFuncInfoCommentinCpp('Constructor Function', '', ''))
                #filehandle.write(self.Schema.Schema_ConstructorImpl(self.classname[2:]))







