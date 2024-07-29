import sys
import os

from JLogPrint import *
from datetime import date


class JCppSchema:
    ClassDefBegin = '{\n'
    ClassDefEnd = '};\n'
    ClassPublicSection = 'public:\n'
    ClassProtectedSection = 'protected:\n'
    ClassPrivateSection = 'private:\n'
    ClassAlign1st = '        '
    def Schema_FileTitleComment(self):
        return '''/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
******************************************************************************/\n'''

    def Schema_FileHeaderComment(self):
        return '/***  Include Files ***********************************************************/\n'
    
    def Schema_FileInfoComment(self, file_name=None):
        ss = '/**\n'
        ss += ' * @file ' + file_name + '\n'
#        ss += ' * @date ' + date.today().strftime('%m/%d/%y') + '\n'
        if (file_name[-1] == 'h'):
            # Header File
            ss += ' * @brief Include list of available features\n'
        else:
            # CPP File
            ss += ' * @brief Feature Control defines for enabled features'
        ss += ' *\n'
        ss += ' * @details <OPTIONAL: Some details about file>\n'
        ss += ' *\n'
        ss += ' * @see <OPTIONAL: Links to reference/Another software component>\n'
        ss += ' *\n'
        ss += ' */\n'
        return ss

    def Schema_FileLogComment(self, file_name=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  $Log: ' + file_name + ',v $\n')
        ss.append('******************************************************************************/\n')
        return ''.join(ss)

    def Schema_ModuleFunctionComment(self):
        return '/***  Module Functions  *******************************************************/\n'

    def Schema_ClassInfoComment(self, class_name=None):
        ss = '/***  Module Class  ***********************************************/\n'
        ss += '/**\n'
        ss += ' * @brief   : ' + class_name + ' class\n'
        ss += ' */\n'
        return ss

    def Schema_ClassFuncInfoCommentinHeader(self, offsetAlign, class_name=None):
        ss = offsetAlign + '/**\n'
        ss += offsetAlign + '* @brief              : TBD\n'
        ss += offsetAlign + '*/\n'
        return ss

    def Schema_ClassFuncInfoCommentinCpp(self, brief=None, paralist=None, returnCode=None):
        ss = '/**\n'
        if (brief is not None):
            ss += '* @brief   : ' + brief + '\n'
        else:
            ss += '* @brief   : No Details\n'
        if (paralist is not None):
            ss += '* @param   - ' + paralist + '\n'
        else:
            ss += '* @param   - No Details\n'
        if (returnCode is not None):
            ss += '* @return  - ' + returnCode + '\n'
        else:
            ss += '* @return  - No Details.\n'
        ss += '*/\n'
        return ss

    def Schema_PublicFuncComment(self, class_name=None):
        ss = '/*******************************************************************************\n'
        ss += ' *  ' + class_name + ' - Public\n'
        ss += '*******************************************************************************/\n'
        return ss

    def Schema_PublicMemberComment(self):
        return '/* Public Member ===================================================*/\n'

    def Schema_ProtectedMemberComment(self):
        return '/* Protected Member ================================================*/\n'

    def Schema_PraviteMemberComment(self):
        return '/* Private Member ==================================================*/\n'
    
    def Schema_Constructor(self, classname=None):
        return classname + '();\n'

    def Schema_ConstructorImpl(self, classname=None):
        ss = []
        ss.append(classname + '::' + classname + '()\n')
        ss.append('{\n')
        ss.append('    // TODO\n\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_IfdefStatementBegin(self, iheader):
        iheadername = '__' + iheader + '_H__'
        ss = '#ifndef ' + iheadername + '\n'
        ss += '#define ' + iheadername + '\n'
        ss += '\n\n'
        return ss

    def Schema_IfdefStatementEnd(self, iheader):
        ss = '#endif' + ' // ' + iheader
        return ss

    def Schema_Enum(self, raw_name=None, enumDict=None):
        ss = ''
        for k, v in enumDict.items():
            ss += 'enum ' + raw_name.upper() + '_' + k.upper() + '_' + 'BAM\n'
            ss += '{\n'
            count = 0
            sortedlist = sorted(v)
            for ele in sortedlist:
                ss += '    ' + raw_name.upper() + '_' + ele.upper() + ' = ' + str(count) + ',\n'
                count += 1
            ss = ss[:-2]
            ss += '\n'
            ss += '};\n\n\n'
        return ss

    def Schema_Typedef(self, newtype=None, stdtype=None):
        ss = ''
        if (newtype is not None) and (stdtype is not None):
            ss += '#ifndef ' + newtype + '\n'
            ss += 'typedef ' + stdtype + '      ' + newtype + ';\n'
            ss += '#endif\n\n'
        return ss

    def Schema_ClassDef(self, classname, inhert=None, parentclassname=None):
        ss = 'class '
        ss += classname
        if (inhert is not None) and (parentclassname is not None):
            ss += ' : ' + inhert + ' ' + parentclassname + '\n'
        else:
            ss += '\n'
        return ss


    def Schema_HeaderNameConvert(self, varname=None):
        raise NotImplementedError('Must override methodB')

    def Schema_FuncNameConvert(self, funcname=None):
        raise NotImplementedError('Must override methodB')


class JCppFeatureLibSchema(JCppSchema):
    def __init__(self):
        self.flobj = 'sOJFLobj'

    def Schema_FeatureVarComment(self, featurename, brief=None):
        ss = '        /**\n'
        if (brief is not None):
            ss += '        * @brief' + '         : ' + brief + '\n'
        else:
            ss += '        * @brief' + '         : ' + featurename[8:featurename.find('Feature')] + ' Feature Instance\n'
        ss += '        */\n'
        return ss

    def Schema_HeaderNameConvert(self, featurename):
        return featurename + 'Feature'
    
    def Schema_FuncVarTypeConvert(self, featurename):
        return featurename + 'Feature'
    
    def Schema_VarNameConvert(self, featurename):
        return 'm' + featurename + 'Feature'

    def Schema_FuncNameConvert(self, featurename):
        return '*Get' + featurename + 'Ptr()'

    def Schema_FeatureDeclareInstance(self, classname):
        return 'static ' + classname + '   sOJFLobj;\n'
    
    def Schema_FuncGetInstance(self, classname):
        ss = classname + '* ' + classname + '::GetInstance(void)\n'
        ss += '{\n'
        ss += '    return &' + self.flobj + ';\n'
        ss += '}\n'
        return ss

    def Schema_DefineRtcTopCount(self, rtcCount, topCount):
        ss = []
        ss.append('#define RTCID_COUNT             ' + str(rtcCount) + '\n')
        ss.append('#define TOPID_COUNT             ' + str(topCount) + '\n')
        ss.append('\n\n')
        return ''.join(ss)

    def Schema_FuncInitialize(self, classname, privatevarlist):
        ss = 'RET_CODE_TYPE ' + classname + '::FL_Initialize_Features(void)\n'
        ss += '{\n'
        ss += '    RET_CODE_TYPE retCode = RET_SUCCESS;\n\n'
        # Loop each Feature variable from Header file
        for item in privatevarlist:
            offset = item.find('Feature ')
            feavar = item[offset + len('Feature '):]
            noffset = feavar.find('[')
            if (noffset == -1):
                # Cut the last 'Feature;\n'
                feaname = feavar[1 : -9]
                ss += '    ' + feavar[:-2] + '.SetInstanceNumber(0);\n'
                ss += '    ' + feavar[:-2] + '.BaseInitialize((const char*)' + '"' + feaname + '"' + ');\n'
            else:
                # Cut the last 'Feature[n];\n'
                feaname = feavar[1 : -12]
                for l in range(int(feavar[noffset + 1])):
                    ss += '    ' + feavar[:-5] + '[' + str(l) + ']' + '.SetInstanceNumber(' + str(l) + ');\n'
                    ss += '    ' + feavar[:-5] + '[' + str(l) + ']' + '.BaseInitialize((const char*)' + '"' + feaname + str(l) + '"' + ');\n'
        ss += '\n'
        ss += '    return retCode;\n'
        ss += '}\n'
        return ss


class JCppFeatureSchema(JCppSchema):
    def __init__(self):
        # Format: {BamType : {'RTC' : [RTC List]}, {'TOP' : [TOP List]}}
        self.ActId = {}
        self.BamCount_Tbl = {}
        # Format: {BamName : {BamType : FeatureName}}
        self.BamFeature_Tbl = {}
        # Format {Bamtype : BamName}
        self.Bamtypename_Tbl = {}
        # Implementation flag for each BAM instance, Format {name of bamproxy instance : True or False}
        self.BamtypeImpl_Tbl = {}
        # Feature Name
        self.featureName = []
        # Implementation flag for each feature instance, Format {name of feature instance : True or False}
        self.featureImpl = {}
        # Specify the bamtype with Feature name in case RTP&TOP is under Feature
        self.feartctop = {}

    def Schema_SchemaFeaturehFileInfoComment(self, headerfilename=None, rawname=None):
        ss = []
        ss.append('/**\n')
        ss.append(' * @file ' + headerfilename + '\n')
        ss.append(' * @date\n')
        ss.append(' * @brief Header file for feature controls of ' + rawname[2:] + '\n')
        ss.append(' * \n')
        ss.append(' * @details <OPTIONAL: Some details about file>\n')
        ss.append(' * \n')
        ss.append(' * @see <OPTIONAL: Links to reference/Another software component>\n')
        ss.append(' * \n')
        ss.append(' */\n')
        return ''.join(ss)

    def Schema_SchemaFeaturecppFileInfoComment(self, cppfilename=None, rawname=None):
        ss = []
        ss.append('/**\n')
        ss.append(' * @file ' + cppfilename + '\n')
        ss.append(' * @date\n')
        ss.append(' * @brief Defines control command/functions for ' + rawname[2:] + ' Feature\n')
        ss.append(' * \n')
        ss.append(' * @details <OPTIONAL: Some details about file>\n')
        ss.append(' * \n')
        ss.append(' * @see <OPTIONAL: Links to reference/Another software component>\n')
        ss.append(' * \n')
        ss.append(' */\n')
        return ''.join(ss)

    def Schema_HeaderNameConvert(self, bamname=None):
        return bamname + 'BamProxy'

    def Schema_FuncVarTypeConvert(self, bamname):
        return bamname + 'BamProxy'

    def Schema_VarNameConvert(self, bamname):
        return 'm' + bamname + 'BamProxy'

    def Schema_FuncNameConvert(self, classname=None, funcname=None):
        # Check if RTC or TOP is under Feature
        return classname[:-7] + funcname + 'BAMSetup' if classname[2:-7] != funcname else classname[:-7] + 'BAMSetup'

    def Schema_AddBamCount(self, bamname, count):
        self.BamCount_Tbl[bamname] = count

    def Schema_FeatureVarComment(self, featurename, brief=None):
        ss = '        /**\n'
        if (brief is not None):
            ss += '        * @brief' + '         : ' + brief + '\n'
        else:
            ss += '        * @brief' + '         : ' + featurename[8:featurename.find('BamProxy')] + ' Bam Proxy Instance\n'
        ss += '        */\n'
        return ss

    def Schema_GetBamPtr(self, classname, bamname, count):
        ssbam = bamname + 'BamProxy'
        ss = ssbam + '* ' + 'Get' + ssbam + 'Ptr(void) {return '
        if (count == 1):
            ss += '&' + self.Schema_VarNameConvert(bamname)  + ';}\n'
        else:
            ss += self.Schema_VarNameConvert(bamname)  + ';}\n'
        return ss

    def Schema_FormatRTCTOP(self, class_name=None, bam=None):
        ss = '\n'
        #mid = class_name.upper() + '_' + bam.upper() + '_'
        if (bool(self.ActId)):
            for k, v in self.ActId.items():
                count = 0
                ss += 'enum ' + bam + '_' + k + '_ACTION\n'
                ss += '{\n'
                if ('RTC' in v):
                    for item in v['RTC']:
                        ss += '    ' + bam + '_' + 'RTC_' + k.upper() + '_' + item.upper() + ' = ' + str(count) + ',\n'
                        count += 1
                if ('TOP' in v):
                    for item in v['TOP']:
                        ss += '    ' + bam + '_' + 'TOPWRITE_' + k.upper() + '_' + item.upper() + ' = ' + str(count) + ',\n'
                        count += 1
                        ss += '    ' + bam + '_' + 'TOPREAD_' + k.upper() + '_' + item.upper() + ' = ' + str(count) + ',\n'
                        count += 1
                ss += '    ' + bam + '_' + k.upper() + '_MAX_ACTIONID\n'
                ss += '};\n\n'
        return ss

    def Schema_AddActionId(self, bamproxyname, rtclist, toplist):
        if rtclist:
            self.ActId[bamproxyname] = {'RTC' : rtclist}
            if toplist:
                self.ActId[bamproxyname]['TOP'] = toplist
        elif toplist:
            self.ActId[bamproxyname] = {'TOP' : toplist}


    def Schema_AddBamFeature(self, bamproxynameDict, featurenameList, feartctopDict):
        if (bool(bamproxynameDict) and bool(featurenameList)):
            sortedlist = sorted(featurenameList)
            self.featureName = sortedlist
            self.Bamtypename_Tbl = bamproxynameDict
            self.feartctop = feartctopDict

    def Schema_AddBamFeatureImple(self, bamnameImpleDict, featurenameImpleDict):
        if (bool(bamnameImpleDict) and bool(featurenameImpleDict)):
            self.featureImpl = featurenameImpleDict
            self.BamtypeImpl_Tbl = bamnameImpleDict

    def Schema_DefineChannel(self, rawname, feaname, bm, chlist):
        ss = '#define ' + rawname + '_' + feaname.upper() + '_' + bm.upper() + '_CHANNEL_IN' + ' ' * (64 - len(feaname) - len(bm)) + '(' + str(chlist[0]) + ')\n'
        ss += '#define ' + rawname + '_' + feaname.upper() + '_' + bm.upper() + '_CHANNEL_OUT' + ' ' * (63 - len(feaname) - len(bm)) + '(' + str(chlist[1]) + ')\n'
        return ss

    def Schema_FuncInitialize(self, classname, funclist):
        ss = 'RET_CODE_TYPE ' + classname + '::OJInitialize()\n'
        ss += '{\n'
        ss += '    RET_CODE_TYPE retCode = RET_SUCCESS;\n\n'
        #ss += '    memset(mFeatureName, 0, sizeof(mFeatureName));\n'
        #ss += '    memcpy(mFeatureName, fname, strlen(fname));\n\n'
        #ss += '    memcpy_s(mFeatureName, sizeof(mFeatureName), fname, strlen(fname));\n\n'
        for item in funclist:
            ss += '    ' + 'this->' + str(item[item.rfind(' ') + 1:]).replace('void', '')
        ss += '\n    return retCode;\n'
        ss += '}\n\n'
        return ss

    def Schema_SetInstance(self, classname):
        ss = 'void ' + classname + '::SetInstanceNumber(int n)\n'
        ss += '{\n'
        ss += '    ' + 'this->mInstanceNumber = n;\n'
        ss += '}\n\n'
        return ss

    def Schema_SetBamSetup(self, classname, topname, item, bamtype, raw_name, chInfoDict):
        count = 0
        bamcount = self.BamCount_Tbl[bamtype]
        ss = item.lstrip()
        dv = ss.find(' ')
        ss = ss[:dv + 1] + classname + '::' + ss[dv + 1:-2]
        ss += '\n'
        ss += '{\n'
        actions = False
        for eachFea in self.featureName:
            bamnameList = sorted(self.Bamtypename_Tbl[bamtype])
            for eachBamname in bamnameList:
                if bamtype in self.ActId and ('RTC' in self.ActId[bamtype] or 'TOP' in self.ActId[bamtype]):
                        actions = True
        if (actions):
            ss += '    uint32_t actionIDs[' + raw_name + '_' + bamtype.upper() + '_MAX_ACTIONID];\n\n'
        # Add "switch...case..." based on Instance Number set by FeatureLib Level
        #ss += '    switch(this->mInstanceNumber) {\n'
        ss += '    switch(GetInstanceNumber()) {\n'
        # Loop for each Feature instance      
        count = 0
        for eachFea in self.featureName:
            ss += '        case ' + str(count) + ':\n'
            bamnameList = sorted(self.Bamtypename_Tbl[bamtype])
            # Loop for each Bam Name belonging to the same Bam Type
            bamcnt = 0
            for eachBamname in bamnameList:
                tcount = 0
                arr = '' if (bamcount == 1) else '[' + raw_name.upper() + '_' + str(bamnameList[bamcnt]).upper() + ']'
                # Add RTC Command
                if bamtype in self.ActId and 'RTC' in self.ActId[bamtype]:
                    # Loop for each RTC command
                    for eachRtc in self.ActId[bamtype]['RTC']:
                        rtc_name = raw_name + '_' + 'RTC_' + bamtype.upper() + '_' + eachRtc.upper()
                        ss += '            ' + 'actionIDs[' + rtc_name + '] = '
                        if self.featureImpl[eachFea]:
                            if self.BamtypeImpl_Tbl[eachBamname]:
                                # In case Feature includes RTC or TOP, then eachFea equals eachBamname
                                comboName = eachFea if eachBamname in self.feartctop.keys() else eachFea + eachBamname
                                #comboName = eachFea + eachBamname if eachFea != eachBamname else eachBamname    
                                ss += 'RTCID_' + topname + comboName + eachRtc
                            else:
                                ss += '0' + '  /* Not Implemented */'
                        else:
                            ss += '0' + '  /* Not Implemented */'
                        ss += ';\n'
                        tcount += 1
                # Add TOP Command
                if bamtype in self.ActId and 'TOP' in self.ActId[bamtype]:
                    # Loop for each TOP command
                    for eachTop in self.ActId[bamtype]['TOP']:
                        top_name = raw_name + '_' + 'TOPWRITE_' + bamtype.upper() + '_' + eachTop.upper()
                        ss += '            ' + 'actionIDs[' + top_name + '] = '
                        if self.featureImpl[eachFea] == True:
                            if self.BamtypeImpl_Tbl[eachBamname]:
                                # In case Feature includes RTC or TOP, then eachFea equals eachBamname
                                comboName = self.feartctop[eachBamname] if eachBamname in self.feartctop.keys() else eachFea + eachBamname
                                ss += 'TOP_WRITE_' + topname + comboName + eachTop
                            else:
                                ss += '0' + '  /* Not Implemented */'
                        else:
                            ss += '0' + '  /* Not Implemented */'
                        ss += ';\n'
                        tcount += 1
                        top_name = raw_name + '_' + 'TOPREAD_' + bamtype.upper() + '_' + eachTop.upper()
                        ss += '            ' + 'actionIDs[' + top_name + '] = '
                        if self.featureImpl[eachFea] == True:
                            if self.BamtypeImpl_Tbl[eachBamname]:
                                # In case Feature includes RTC or TOP, then eachFea equals eachBamname
                                comboName = self.feartctop[eachBamname] if eachBamname in self.feartctop.keys() else eachFea + eachBamname
                                ss += 'TOP_READ_' + topname + comboName + eachTop
                            else:
                                ss += '0' + '  /* Not Implemented */'
                        else:
                            ss += '0' + '  /* Not Implemented */'
                        ss += ';\n'
                        tcount += 1
                bamcnt += 1
                if (actions):
                    ss += '            ' + 'm' + bamtype + 'BamProxy' + arr + '.AssignActionIds(actionIDs);\n'
                # Add SetTotalChannels Function
                if raw_name[2:] in chInfoDict.keys():
                    ch_in = raw_name[2:] + '_' + eachFea.upper() + '_' + eachBamname.upper() + '_CHANNEL_IN'
                    ch_out = raw_name[2:]    + '_' + eachFea.upper() + '_' + eachBamname.upper() + '_CHANNEL_OUT' 
                    ss += '            ' + 'm' + bamtype + 'BamProxy' + arr + '.SetTotalChannels(' + ch_in + ', ' + ch_out + ');\n'
                else:
                    Jlog_print(JAG_WARNING, 'No Channel In & Out found: ' + eachFea + '::' + eachBamname)
                # Add initialize function
                ss += '            ' + 'm' + bamtype + 'BamProxy' + arr + '.Initialize();\n'
            ss += '            break;\n'
            count += 1
        ss += '    }\n'
        ss += '}\n'
        return ss




class JCppBamSchema(JCppSchema):
    def __init__(self):
        # Format: {BamType : {'RTC' : [RTC List]}, {'TOP' : [TOP List]}}
        self.ActId = {}

    def Schema_SchemaBamproxyhFileInfoComment(self, headerfilename=None, rawname=None):
        ss = []
        ss.append('/**\n')
        ss.append(' * @file ' + headerfilename + '\n')
        ss.append(' * @date\n')
        ss.append(' * @brief Header file for ' + rawname[2:] + ' BAM Proxy implementation\n')
        ss.append(' * \n')
        ss.append(' * @details <OPTIONAL: Some details about file>\n')
        ss.append(' * \n')
        ss.append(' * @see <OPTIONAL: Links to reference/Another software component>\n')
        ss.append(' * \n')
        ss.append(' */\n')
        return ''.join(ss)

    def Schema_SchemaBamproxycppFileInfoComment(self, cppfilename=None, rawname=None):
        ss = []
        ss.append('/**\n')
        ss.append(' * @file ' + cppfilename + '\n')
        ss.append(' * @date\n')
        ss.append(' * @brief TODO\n')
        ss.append(' * \n')
        ss.append(' * @details OO code for ' + rawname[2:] +  ' Bam Proxy command implementation\n')
        ss.append(' * \n')
        ss.append(' * @see Base bam proxy structures in BamProxy.c and BamProxy.h\n')
        ss.append(' * \n')
        ss.append(' */\n')
        return ''.join(ss)

    def Schema_HeaderNameConvert(self, varname=None):
        Jlog_print(JAG_GENERAL, 'Bam Schema: VarConvert')

    def Schema_FuncNameConvert(self, funcname=None):
        Jlog_print(JAG_GENERAL, 'Bam Schema: FuncConvert')

    def Schema_FeatureVarComment(self, bamname, brief=None):
        ss = '        /**\n'
        if (brief is not None):
            ss += '        * @brief' + '         : ' + brief + '\n'
        else:
            ss += '        * @brief' + '         : ' + bamname + ' value\n'
        ss += '        */\n'
        return ss

    def Schema_BamEnum(self, bamtype, EnumBamAction):
        ss = ''
        cnt = 0
        if EnumBamAction:
            ss += 'enum ' + bamtype.upper() + '_ACTIONS\n'
            ss += '{\n'
            for item in EnumBamAction:
                ss += '    ' + item + ' = ' + str(cnt) + ',\n'
                cnt += 1
            ss += '    ' + 'MAX_' + bamtype.upper() + '_ACTIONS\n'
            ss += '};\n'
        return ss

    def Schema_FuncInitialize(self, classname, bamAction):
        ss = 'RET_CODE_TYPE ' + classname + '::Initialize(void)\n'
        ss += '{\n'
        ss += '    RET_CODE_TYPE retCode = RET_SUCCESS;\n'
        # Here, remove RegisterBAMF for Initialize function.
        #ss += '    uint32_t ignore;\n\n'
        #for item in bamAction:
        #    ss += '    retCode = GetBAMFPtr()->RegisterBAMF(this, mActionID[' + item + '], &ignore);\n'
        ss += '\n'
        ss += '    return retCode;\n'
        ss += '}\n'
        return ss

    def Schema_ActionAssignDeclare(self, rawname):
        ss = '        void AssignActionIds'
        ss += '(uint32_t actions[MAX_' + rawname.upper() + '_ACTIONS]);\n'
        return ss

    def Schema_ActionIDDeclare(self, rawname):
        ss = '        uint32_t mActionID[MAX_' + rawname.upper() + '_ACTIONS];\n'
        return ss
    
    def Schema_AssignActionID(self, rawname, classname):
        ss = 'void ' + classname + '::AssignActionIds(uint32_t actions[MAX_' + rawname.upper() + '_ACTIONS])\n'
        ss += '{\n'
        ss += '    pal_memcpy(mActionID, actions, sizeof(mActionID), ' + 'sizeof(uint32_t) * MAX_' + rawname.upper() + '_ACTIONS);\n'
        #ss += '    memcpy_s(mActionID, sizeof(mActionID), actions, sizeof(mActionID));\n'
        ss += '}\n'
        return ss

    def Schema_RtcFuncDeclare(self, rawname, rtcname, rtcfunc, callhandler_flag):
        #s1 = rtcname[:rtcname.find('_')].lower().capitalize()
        s1 = rawname[2:]
        s2 = rtcname[rtcname.find('_') + 1:].lower().capitalize()
        ss = '        uint32_t ' + s1 + s2 + '(uint32_t* data, '
        if rtcfunc != '' and rtcfunc != None:
            ss += rtcfunc[:-2] + ', '
        ss += 'uint32_t* msgid, uint32_t caller_handle = 0);' + '\n'
        return ss 

    def Schema_TopFuncDeclare(self, topname):
        s1 = topname[:topname.find('_')].lower().capitalize()
        s2 = topname[topname.find('_') + 1:].lower().capitalize()
        ss = '        uint32_t ' + s1 + s2 + '_request' + '(uint32_t* data, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* payload, uint32_t* msgid);\n'
        return ss

    def Schema_RtcFuncImplement(self, classname, rawname, rtcname, rtcfunc, enumkeyword, callhandler_flag):
        #s1 = rtcname[:rtcname.find('_')].lower().capitalize()
        s1 = rawname[2:]
        s2 = rtcname[rtcname.find('_') + 1:].lower().capitalize()
        ss ='uint32_t ' + classname + '::' + s1 + s2 + '(uint32_t* data, '
        #if callhandler_flag == 1:
        #    ss += 'uint32_t callhandler, '
        if rtcfunc != '' and rtcfunc != None:
            ss += rtcfunc[:-2] + ', '
        ss += 'uint32_t* msgid, uint32_t caller_handle' + ')\n'
        ss += '{\n'
        #ss += '    uint32_t msg_id = mActionID[' + rtcname + '];\n'
        ss += '    uint32_t msg_id = mActionID[' + enumkeyword + '];\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE; // Skip the Talaria Header\n\n'
        ss += '    // Fill control message header\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], msg_id);\n'
        if callhandler_flag == 1:
            ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)caller_handle);\n'
        # Serialize all function parameters
        preindex = 0
        postindex = 0
        functmp = rtcfunc[:-2]
        while postindex < len(functmp):
            typeindex = postindex
            postindex = functmp.find(' ', postindex)
            # check if it reaches the end
            if postindex == -1:
                break
            # Save data type, it could be uint32_t or float (real32_t)
            vtype = functmp[typeindex:postindex-1] if (functmp[postindex - 1] == '*') else functmp[typeindex:postindex]
            # check if it is a pointer
            if (functmp[postindex - 1] == '*'):
                preindex = postindex + 1
                postindex = functmp.find(', ', postindex)
                vname = functmp[preindex:postindex]
                postindex += 2
                preindex = postindex
                postindex = functmp.find(' ', postindex)
                preindex = postindex + 1
                postindex = functmp.find(', ', postindex)
                # end of string
                if (postindex == -1):
                    postindex = len(functmp)
                vsize = functmp[preindex:postindex]
                ss += '    for (int i = 0; i < ' + vsize + '; i ++)\n'
                ss += '    {\n'
                if (vtype == 'float') or (vtype == 'real32_t') or (vtype == 'real32_T'):
                    ss += '        put_int32_into_buffer((uint8_t *)&data[Index++], ' + '*(uint32_t *)(&' + vname + '[i]));\n'
                else:
                    ss += '        put_int32_into_buffer((uint8_t *)&data[Index++], ' + vname + '[i]);\n'
                ss += '    }\n'
                preindex = postindex
            else:
                preindex = postindex + 1
                postindex = functmp.find(', ', postindex)
                if (postindex == -1):
                    postindex = len(functmp)
                vname = functmp[preindex:postindex]
                if (vtype == 'float') or (vtype == 'real32_t') or (vtype == 'real32_T'):
                    ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], ' + '*(uint32_t *)(&' + vname + '));\n'
                else:
                    ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], ' + vname + ');\n'
                postindex += 2
                preindex = postindex
        ss += '    //Update the Size\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n'
        ss += '    if (msgid != NULL) {\n'
        ss += '        *msgid = msg_id;\n'
        ss += '    }\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

    def Schema_TopFuncImplement(self, classname, topname):
        s1 = topname[:topname.find('_')].lower().capitalize()
        s2 = topname[topname.find('_') + 1:].lower().capitalize()
        ss = 'uint32_t ' + classname + '::' + s1 + s2 + '_request' + '(uint32_t* data, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* payload, uint32_t* msgid)\n'
        ss += '{\n'
        ss += '    uint32_t msg_id = mActionID[' + topname + '_SET];\n'
        ss += '    uint32_t Index;\n\n'
        ss += '    Index = top_write_request(data, msg_id, callhandler, offset, byte_count, payload);\n'
        ss += '    if (msgid != NULL) {\n'
        ss += '        *msgid = msg_id;\n'
        ss += '    }\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

class JCppBamProxyBaseSchema(JCppSchema):
    def __init__(self):
        self.ss = ''

    def Schema_BamProxyBaseIfdefConvert(self, hname):
        return '__' + hname.upper() + '_H__'

    def Schema_BamProxyBaseFillImplement1(self, classname):
        ss = []
        ss.append('uint32_t ' + classname + '::' + 'top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* pPayload)\n')
        ss.append('{\n')
        ss.append('    uint32_t Index = TALARIA_HEADER_SIZE;\n')
        ss.append('    uint32_t payloadOff = 0;\n\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[0], 0);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)byte_count);\n')
        ss.append('    if (pPayload != NULL) {\n')
        ss.append('        for (int i = 0; i < byte_count; i += 4) {\n')
        ss.append('            put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)pPayload[payloadOff++]);\n')
        ss.append('        }\n')
        ss.append('    }\n')
        ss.append('    else {\n')
        ss.append('        Index +=  byte_count >> 2;\n')
        ss.append('    }\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n\n')
        ss.append('    return Index;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_BamProxyBaseFillImplement2(self, classname):
        ss = 'uint32_t ' + classname + '::' + 'top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t payload)\n'
        ss += '{\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE;\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(payload));\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

    def Schema_BamProxyBaseFillImplement3(self, classname):
        ss = 'uint32_t ' + classname + '::' + 'top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, float payload)\n'
        ss += '{\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE;\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

    def Schema_BamProxyBaseFillImplement4(self, classname):
        ss = 'uint32_t ' + classname + '::' + 'top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t payload)\n'
        ss += '{\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE;\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

    def Schema_BamProxyBaseFillImplement5(self, classname):
        ss = 'uint32_t ' + classname + '::' + 'top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, float payload)\n'
        ss += '{\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE;\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss

    def Schema_BamProxyBaseFillImplement6(self, classname):
        ss = 'uint32_t ' + classname + '::' + 'top_read_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count)\n'
        ss += '{\n'
        ss += '    uint32_t Index = TALARIA_HEADER_SIZE;\n\n'
        #ss += '    uint32_t payloadOff = 0;\n\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[0], 0);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], topid);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)byte_count);\n'
        ss += '    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n\n'
        ss += '    return Index;\n'
        ss += '}\n'
        return ss



class JCppFeatureBaseSchema(JCppSchema):
    def __init__(self):
        self.ss = ''

    def Schema_FeatureBaseIfdefConvert(self, hname):
        return '__' + hname.upper() + '_H__'

