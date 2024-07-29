import sys
import os


from datetime import date
from JConv import *
from JLogPrint import *

class JCSchema:
    ClassAlign1st = '        '
    DONOTCHANGE_BELOW = '''/*************************************************************/
/**** Do not change the code below, updated automatically ****/
/*************************************************************/\n'''
    DONOTCHANGE_ABOVE = '''/*************************************************************/
/**** Do not change the code above, updated automatically ****/
/*************************************************************/\n\n\n'''
    DONOTCHANGE_BELOW_1st = '''    /*************************************************************/
    /**** Do not change the code below, updated automatically ****/
    /*************************************************************/\n'''
    DONOTCHANGE_ABOVE_1st = '''    /*************************************************************/
    /**** Do not change the code above, updated automatically ****/
    /*************************************************************/\n'''
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
    
    def Schema_FileInfoComment(self, file_name=None, brief=None):
        ss = '/**\n'
        ss += ' * @file ' + file_name + '\n'
        ss += ' * @date ' + date.today().strftime('%m/%d/%y') + '\n'
        ss += ' * @brief '
        ss += brief + '\n'
        ss += ' *\n'
        ss += ' * @details <OPTIONAL: Some details about file>\n'
        ss += ' *\n'
        ss += ' * @see <OPTIONAL: Links to reference/Another software component>\n'
        ss += ' *\n'
        ss += ' */\n'
        return ss

    def Schema_FileLog(self, file_name='', log=''):
        ss = '/*********************************************************************************\n'
        ss += ' *  $Log: ' + file_name + ',v $' + log
        ss += '\n'
        ss += ' *******************************************************************************/\n'
        return ss

    def Schema_FileIncludeHeader(self):
        return '/***  Include Files ***********************************************************/\n'

    def Schema_FileModuleMacros(self):
        return '/***  Module Macros  **********************************************************/\n'

    def Schema_FileModuleVariables(self):
        return '/***  Module Types / Variables  ***********************************************/\n'

    def Schema_FileExternalVariables(self):
        return '/***  External Variable References  *******************************************/\n'

    def Schema_FileModuleFunctions(self):
        return '/***  Module Functions  *******************************************************/\n'

    def Schema_FuncInfoCommentinCpp(self, brief=None, paralist=None, returnCode=None):
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

    def Schema_IfdefStatementBegin(self, iheader):
        iheadername = '__' + iheader + '_H__'
        ss = '#ifndef ' + iheadername + '\n'
        ss += '#define ' + iheadername + '\n'
        ss += '\n\n'
        return ss

    def Schema_IfdefStatementEnd(self, iheader):
        ss = '#endif' + ' // ' + iheader
        return ss

    def Schema_IfdefBCStatementBegin(self, filename):
        ss = '#if !defined(BC_' + filename + '__INCLUDED_)\n'
        ss += '#define BC_' + filename + '__INCLUDED_\n'
        return ss

    def Schema_IfdefBCStatementEnd(self, filename):
        ss = '#endif /*!defined(BC_' + filename + '__INCLUDED_)*/\n'
        return ss

    def Schema_IfdefcplusplusStatementBegin(self):
        ss = '#ifdef __cplusplus\n'
        ss += 'extern "C" {\n'
        ss += '#endif\n'
        return ss

    def Schema_IfdefcplusplusStatementEnd(self):
        ss = '#ifdef __cplusplus\n'
        ss += '}\n'
        ss += '#endif\n'
        return ss

    def Schema_IncludeHeaderFile(self, headerfile=None, qsign='"'):
        ss = ''
        if (headerfile is not None):
            if qsign == '"':
                ss += '#include ' + '"' + headerfile + '"' + '\n'
            else:
                ss += '#include ' + '<' + headerfile + '>' + '\n'
        return ss

    def Schema_EnumDefine(self, enum_name=None, enumlist=None):
        ss = ''
        if not (not enumlist):
            ss += 'typedef enum\n'
            ss += '{\n'
            for item in enumlist:
                ss += '    ' + str(item).upper() + ',\n'
            ss =ss[:-2]
            ss += '\n'
            ss += '} ' + enum_name + ';\n'
        return ss

    def Schema_Typedef(self, newtype=None, stdtype=None):
        ss = ''
        if (newtype is not None) and (stdtype is not None):
            ss += '#ifndef ' + newtype + '\n'
            ss += 'typedef ' + stdtype + '      ' + newtype + ';\n'
            ss += '#endif\n\n'
        return ss

    def Schema_Define(self, define1=None, define2=None):
        ss = ''
        if (define1 is not None) and (define2 is not None):
            ss += '#define ' + define1 + ' ' * (40 - len(define1)) + define2
            ss += '\n'
        return ss

    def Schema_DefineMulti(self, define1=None, define2List=None):
        ss = []
        if (define1 is not None) and (len(define2List) > 1):
            ss.append('#define ' + define1 + ' ' * (40 - len(define1)) + define1 + ' \\\n')
            for item in define2List[:-1]:
                ss.append(' ' * 40 + item + ' \\\n')
            ss.append(' ' * 40 + define2List[-1] + '\n')
            ss.append('\n')
        return ''.join(ss)

class JFeatureLibSchema(JCSchema):
    GETPTRSTART = '// GetPtr Start'
    GETPTREND = '// GetPtr End'
    FEATUREHEADERSTART = '// Feature Header Start'
    FEATUREHEADEREND = '// Feature Header End'
    def __init__(self):
        self.conv = JFLConv()

    def Schema_ShadowFilename(self, fealibname=None, filetype=None):
        return self.conv.filename_FeatureLibShadow(fealibname, filetype)

    def Schema_FLGetFeaturePtr(self, featurename=None):
        funcprototype = []
        if featurename is not None:
            funcprototype.append('void')
            funcprototype.append(self.conv.funcname_FeatureLib_FeatureGetPtr(featurename))
            argu = self.conv.strutypename_FeatureType(featurename)
            funcprototype.append('(' + argu + ' **featPtr, uint8_t index' + ')')
            funcprototype.append(featurename)
        return funcprototype

    def Schema_FLAddHeaderFile(self, featurename=None):
        if featurename is not None:
            return self.conv.filename_Feature(featurename, '.h')
        else:
            return ''

    def Schema_FLMacroFeatureNum(self, featurename=None, instanceNum=None):
        macroL = []
        macroL.append(self.conv.macroname_NumofFeature(featurename))
        macroL.append(instanceNum)
        return macroL

    def Schema_EnumFeatureVariablesShadow(self, sinkList=None, feaDict=None):
        ss = []
        if sinkList is not None:
            #print('>>>>>>> Currently, it is not supported for Sink in FeatureLib.c')
            Jlog_print(JAG_WARNING, '>>>>>>> Currently, it is not supported for Sink in FeatureLib.c')
        for k, v in feaDict.items():
            ss.append('//Instance of ' + k.upper() + ' feature\n')
            ftype = self.conv.strutypename_FeatureType(k)
            fvariable = self.conv.varitypename_FeatureType(k)
            farraysize = self.conv.macroname_NumofFeature(k)
            ss.append(ftype + ' ' + fvariable + '[' + farraysize + '];\n')
            ftype = self.conv.strutypename_FeatureDataType(k)
            fvariable = self.conv.varitypename_FeatureDataType(k)
            farraysize = self.conv.macroname_NumofFeature(k)
            ss.append(ftype + ' ' + fvariable + '[' + farraysize + '];\n\n')
        return ''.join(ss)

    def Schema_EnumFeatureVariables(self):
        ss = []
        ss.append('/* Variable to indicate the BIPC initialization status */\n')
        ss.append('static bool s_feat_lib_initialized = false;\n\n')
        return ''.join(ss)


    def Schema_FL_Construct_Features(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Construct_Features - External.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : calls the "'"constructor"'" on all features\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - Function returns RET_CODE_TYPE to indicate SUCCESS or ERROR.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    RET_CODE_TYPE retCode = RET_SUCCESS;\n\n')
        ss.append('    retCode = ' + self.conv.funcname_FeatureLib_Construct(funcprototype[1]) + '();\n\n')
        ss.append('\n\n    return retCode;\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Construct_FeaturesShadow(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Construct_FeaturesShadow - External.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : calls the "'"constructor"'" on all features\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - Function returns RET_CODE_TYPE to indicate SUCCESS or ERROR.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + self.conv.funcname_FeatureLib_Construct(funcprototype[1]) + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    RET_CODE_TYPE retCode = RET_SUCCESS;\n')
        ss.append('    uint8_t i;\n\n')
        for k in feadict.keys():
            ss.append('    for (i = 0; i < ' + self.conv.macroname_NumofFeature(k) + '; i++)\n')
            ss.append('    {\n')
            ss.append('        ' + self.conv.funcname_Feature_FeatureConstructor(k))
            ss.append('(&' + self.conv.varitypename_FeatureType(k) + '[i], &' + self.conv.varitypename_FeatureDataType(k) + '[i]);\n')
            ss.append('    }\n')
        ss.append('\n\n    return retCode;\n')
        ss.append('}\n\n')
        return ''.join(ss)


    def Schema_FL_Initialize_Features(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Initialize_Features - External.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : calls the Initialize function on all features\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - Function returns RET_CODE_TYPE to indicate SUCCESS or ERROR.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    RET_CODE_TYPE this_retCode, retCode = RET_SUCCESS;\n\n')
        ss.append('    retCode = ' + self.conv.funcname_FeatureLib_Initialize(funcprototype[1]) + '();\n\n')
        ss.append('\n    return retCode;\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Initialize_FeaturesShadow(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Initialize_FeaturesShadow - External.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : calls the Initialize function on all features\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - Function returns RET_CODE_TYPE to indicate SUCCESS or ERROR.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + self.conv.funcname_FeatureLib_Initialize(funcprototype[1]) + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    RET_CODE_TYPE this_retCode, retCode = RET_SUCCESS;\n')
        ss.append('    uint8_t i;\n\n')
        for k in feadict.keys():
            ss.append('    for (i = 0; i < ' + self.conv.macroname_NumofFeature(k) + '; i++)\n')
            ss.append('    {\n')
            nfea = self.conv.varitypename_FeatureType(k)
            ss.append('        this_retCode = ' + nfea + '[i].Initialize(&' + nfea + '[i].base, i);\n')
            ss.append('        if ((RET_SUCCESS != this_retCode) && (RET_NO_ACTION != this_retCode))')
            ss.append('        {\n')
            ss.append('            retCode = this_retCode;\n')
            ss.append('            printf232(' + '"' + 'Error [0x%x] while initializing ' + k + 'feature [%x]' + '"' + ', this_retCode, i);\n')
            ss.append('            break;\n')
            ss.append('        }\n')
            ss.append('    }\n')
        ss.append('\n    return retCode;\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Is_Feature_Lib_Ready(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Is_Feature_Lib_Ready - External.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Is the Feature Library ready for send/recv any commands\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - Returns true if the Feature Library is ready for send/recv any commands, otherwise false.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    bool bipc_init_status, feat_lib_readiness = false;\n\n')
        ss.append('    bipc_init_status = Is_BIPC_Ready();\n')
        ss.append('    if(bipc_init_status && s_feat_lib_initialized)\n')
        ss.append('    {\n')
        ss.append('        feat_lib_readiness = true;\n')
        ss.append('    }\n\n')
        ss.append('    return feat_lib_readiness;\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Connect_All_Sinks(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Connect_All_Sinks - Public API exposed to connect/disconnect all sinks.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Connect/Disconnect all sinks\n')
        ss.append(' * @param   - [IN] newConnectState - bool\n')
        ss.append(' *                 : TRUE value means Connect All Sinks.\n')
        ss.append(' *                 : FALSE value means Disconnect All Sinks.\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - None.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    //TODO...\n\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Save_All_Sinks_Parameters(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Save_All_Sinks_Parameters - Public API exposed to save all sinks parameters.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Save all sinks parameters\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - None.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    //TODO...\n\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FL_Restore_All_Sinks_Parameters(self, funcprototype=None, feadict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * FL_Restore_All_Sinks_Parameters - Public API exposed to restore the state of all sinks.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Restore all sinks previous state\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - None.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    //TODO...\n\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_FeatureGetPtr(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  ' + funcprototype[1] + ' - Public API exposed to retrieve feature pointer.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Get ' + funcprototype[3] + ' Feature Pointer\n')
        ss.append(' * @param   - [IN] index - uint8_t index indicating which Announcement feature\n')
        ss.append(' * @param   - [OUT] featPtr - ANNOUNCEMENT_FEAT_TYPE *ptr - NULL if not active\n')
        ss.append(' * @return  - none.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    *featPtr = &' + self.conv.varitypename_FeatureType(funcprototype[3]) + '[index];\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_UpdateFeatureInHeader(self, headerstr=None, headerList=None, funcList=None):
        leading = JFeatureLibSchema.FEATUREHEADERSTART
        trailing = JFeatureLibSchema.FEATUREHEADEREND
        headerStart = headerstr.find(leading)
        headerEnd = headerstr.find(trailing)
        b1 = []
        for hitem in headerList:
            b1.append(self.Schema_IncludeHeaderFile(hitem, '"'))
        leading = JFeatureLibSchema.GETPTRSTART
        trailing = JFeatureLibSchema.GETPTREND
        ptrStart = headerstr.find(leading)
        ptrEnd = headerstr.find(trailing)
        b2 = []
        for fitem in funcList:
            b2.append(fitem[0] + ' ' + fitem[1] + fitem[2] + ';\n')
        return headerstr[:headerStart + len(JFeatureLibSchema.FEATUREHEADERSTART) + 1] + ''.join(b1) + headerstr[headerEnd : ptrStart + len(leading) + 1] + ''.join(b2) + headerstr[ptrEnd:]



class JSetupAPISchema(JCSchema):
    BAMID = 'bam_id'
    FEAID = 'stream_id'
    def __init__(self, featurename=None, featurecount=None):
        self.feaname = featurename
        self.feacount = featurecount
        self.conv = JFLConv()

    def Schema_SetupFilename(self, feaname=None, filetype=None):
        return self.conv.filename_SetupAPI(feaname, filetype)

    def Schema_SetupAddBamproxyHeaderFile(self, bamname=None):
        if bamname is not None:
            return self.conv.filename_Bamproxy(bamname, '.h')
        else:
            return ''

    def Schema_SetupAddFeatureHeaderFile(self, featuretype=None):
        return self.conv.filename_Feature(featuretype, '.h')

    def Schema_SetupBamproxy(self, bamname=None, bamcnt=None):
        funcprototype = []
        if bamname is not None:
            funcprototype.append('void')
            funcprototype.append(self.conv.funcname_SetupAPI_setup(self.feaname, bamname))
            argutype = self.conv.strutypename_BamproxyType(bamname)
            if self.feacount == 1:
                if (bamcnt == 1):
                    argu = '(' + argutype + ' *' + bamname + ')'
                else:
                    argu = '(' + argutype + ' *' + bamname + ', uint8_t ' + JSetupAPISchema.BAMID + ')'
            else:
                if (bamcnt == 1):
                    argu = '(' + argutype + ' *' + bamname + ', uint8_t ' + JSetupAPISchema.FEAID + ')'
                else:
                    argu = '(' + argutype + ' *' + bamname + ', uint8_t ' + JSetupAPISchema.BAMID + ', uint8_t ' + JSetupAPISchema.FEAID + ')'
            funcprototype.append(argu)
            funcprototype.append(bamname)
        return funcprototype

    def Schema_SetupBamproxyIpmle(self, funcprototype=None, feaobj=None, feanameDict=None, bamDict=None, bamnameDict=None, feaimplDict=None, bamimplDict=None):
        ss = []
        ss.append('//' + funcprototype[3] + ' Setup\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    uint32_t actionIDs[' + self.conv.defname_Feature_maxActionId(self.feaname, funcprototype[3]) + '];\n\n')
        if self.feacount > 1:
            ss.append('    switch(' + JSetupAPISchema.FEAID + ') {\n')
            i = 0
            for m in feanameDict[self.feaname]:
                feaFlag = feaimplDict[m]
                ss.append('        case ' + str(i) + ':\n')
                if bamDict[funcprototype[3]] > 1:
                        ss.append('            switch(' + JSetupAPISchema.BAMID + ') {\n')
                        for n in bamnameDict[funcprototype[3]]:
                            bamFlag = bamimplDict[n]
                            ss.append('                case ' + self.conv.defname_Feature_BamOffset(self.feaname, n) + ':\n')
                            # RTC or TOP in Bamproxy object
                            tmp = bamnameDict[funcprototype[3]]
                            ss.append(self.Schema_GenerateActionID(self.feaname, m, funcprototype[3], feaobj.data[tmp[0]], '                    ', feaFlag and bamFlag))
                            ss.append('                    break;\n')
                        ss.append('            }\n')
                else:
                    if (funcprototype[3] == self.feaname):
                        # RTC or TOP in Feature object
                        ss.append(self.Schema_GenerateActionID(self.feaname, m, None, feaobj.data, '            ', feaFlag))
                    else:
                        # RTC or TOP in Bamproxy object
                        tmp = bamnameDict[funcprototype[3]]
                        bamFlag = bamimplDict[tmp[0]]
                        ss.append(self.Schema_GenerateActionID(self.feaname, m, funcprototype[3], feaobj.data[tmp[0]], '            ', feaFlag and bamFlag))
                targetbam_id = self.conv.variname_SetupAPI_TargetBamId(self.feaname, funcprototype[3])
                ss.append('            ' + funcprototype[3] + '->AssignActionIds(' + funcprototype[3] + ', rtcIds);\n')
                ss.append('            ' + funcprototype[3] + '->base.Initialize((BAM_PROXY*)' + funcprototype[3] + ', (TARGET_BAM_TYPE) ' + targetbam_id + ');\n')
                ss.append('            break;\n')
                i = i + 1
            ss.append('    }\n')
        else:
            m = feanameDict[self.feaname][0]
            feaFlag = feaimplDict[m]
            if bamDict[funcprototype[3]] > 1:
                ss.append('    switch(' + JSetupAPISchema.BAMID + ') {\n')
                for n in bamnameDict[funcprototype[3]]:
                    bamFlag = bamimplDict[n]
                    ss.append('        case ' + self.conv.defname_Feature_BamOffset(self.feaname, n) + ':\n')
                    # RTC or TOP in Bamproxy object
                    tmp = bamnameDict[funcprototype[3]]
                    ss.append(self.Schema_GenerateActionID(self.feaname, m, funcprototype[3], feaobj.data[tmp[0]], '            ', feaFlag and bamFlag))
                    ss.append('            break;\n')
                ss.append('    }\n')
            else:
                if (funcprototype[3] == self.feaname):
                    # RTC or TOP in Feature object
                    ss.append(self.Schema_GenerateActionID(self.feaname, m, None, feaobj.data, '    ', feaFlag))
                else:
                    # RTC or TOP in Bamproxy object
                    tmp = bamnameDict[funcprototype[3]]
                    bamFlag = bamimplDict[tmp[0]]
                    ss.append(self.Schema_GenerateActionID(self.feaname, m, funcprototype[3], feaobj.data[tmp[0]], '    ', feaFlag and bamFlag))
            targetbam_id = self.conv.variname_SetupAPI_TargetBamId(self.feaname, funcprototype[3])
            ss.append('    ' + funcprototype[3] + '->AssignActionIds(' + funcprototype[3] + ', rtcIds);\n')
            ss.append('    ' + funcprototype[3] + '->base.Initialize((BAM_PROXY*)' + funcprototype[3] + ', (TARGET_BAM_TYPE) ' + targetbam_id + ');\n')
        ss.append('}\n\n')
        return ''.join(ss)

    def Schema_GenerateActionID(self, featuretype=None, featurename=None, bamname=None, objdata=None, offs=None, implemented=True):
        st = []
        objtmp = objdata.data if bamname is not None else objdata
        # use tmpname to deal with Feature including RTC or TOP directly
        tmpname = bamname if bamname is not None else featuretype
        if 'RTC' in objtmp:
            for rtc in objtmp['RTC']:
                if implemented is True:
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_RtcOffset(featuretype, tmpname, rtc) + '] = ' + self.conv.defname_Feature_RtcActionId(featurename, bamname, rtc) + ';\n')
                else:
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_RtcOffset(featuretype, tmpname, rtc) + '] = 0;\n')
        if 'TOP' in objtmp:
            for top in objtmp['TOP']:
                if implemented is True:
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_TopWriteOffset(featuretype, tmpname, top) + '] = ' + self.conv.defname_Feature_TopWriteActionId(featurename, bamname, top) + ';\n')
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_TopReadOffset(featuretype, tmpname, top) + '] = ' + self.conv.defname_Feature_TopReadActionId(featurename, bamname, top) + ';\n')        
                else:
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_TopWriteOffset(featuretype, tmpname, top) + '] = 0;\n')
                    st.append(offs + 'actionIDs[' + self.conv.defname_Feature_TopReadOffset(featuretype, tmpname, top) + '] = 0;\n')            
        return ''.join(st)


class JFeatureSchema(JCSchema):
    ENUM_START = '// Enumeration Start'
    ENUM_END = '// Enumeration End'
    def __init__(self, featurename=None):
        self.feaname = featurename
        self.conv = JFLConv()

    def Schema_FeatureFilename(self, feaname=None, filetype=None):
        return self.conv.filename_Feature(feaname, filetype)

    def Schema_ShadowFilename(self, feaname=None, filetype=None):
        return self.conv.filename_FeatureShadow(feaname, filetype)

    def Schema_FeaAddHeaderFile(self, bamname=None):
        if bamname is not None:
            return self.conv.filename_Bamproxy(bamname, '.h')
        else:
            return ''

    def Schema_FileExtern(self, externList=None):
        ss = []
        for item in externList:
            ss.append('extern ' + item[0] + ' ' + item[1] + item[2] + ';\n')
        ss.append('\n')
        return ''.join(ss)

    def Schema_FeatureConstruct(self, feaname=None):
        funcprototype = ['void']
        funcprototype.append(self.conv.funcname_Feature_FeatureConstructor(feaname))
        argu = '(' + self.conv.strutypename_FeatureType(feaname) + ' *pFeature, ' + self.conv.strutypename_FeatureDataType(feaname) + ' *pFeatureData)'
        funcprototype.append(argu)
        funcprototype.append(feaname)
        return funcprototype

    def Schema_FeatureInitialize(self, feaname=None):
        funcprototype = ['RET_CODE_TYPE']
        funcprototype.append(self.conv.funcname_Feature_Initialize(feaname))
        funcprototype.append('(FEATURE *pFeature, uint8_t stream_id)')
        funcprototype.append(feaname)
        return funcprototype

    def Schema_FeatureInitializeShadow(self, feaname=None):
        funcprototype = ['RET_CODE_TYPE']
        funcprototype.append(self.conv.funcname_Feature_InitializeShadow(feaname))
        funcprototype.append('(FEATURE *pFeature, audio_control_message* ctrlBuf, audio_control_message* respBuf, uint8_t stream_id)')
        funcprototype.append(feaname)
        return funcprototype        

    def Schema_FeatureEnumBamname(self, enumDict=None, bamname=None, bamnameList=None):
        enumDict[self.conv.enumname_Feature_BamOffset(self.feaname, bamname)] = [self.conv.defname_Feature_BamOffset(self.feaname, item) for item in bamnameList]

    def Schema_FeatureEnumBamActionID(self, enumDict=None, obj_rtctop=None, bamname=None):
        st = []
        if 'RTC' in obj_rtctop:
            for rtc in obj_rtctop['RTC']:
                st.append(self.conv.defname_Feature_RtcOffset(self.feaname, bamname, rtc))
        if 'TOP' in obj_rtctop:
            for top in obj_rtctop['TOP']:
                st.append(self.conv.defname_Feature_TopWriteOffset(self.feaname, bamname, top))
                st.append(self.conv.defname_Feature_TopReadOffset(self.feaname, bamname, top))
        st.append(self.conv.defname_Feature_maxActionId(self.feaname, bamname))
        enumDict[self.conv.enumname_Feature_ActionIDOffset(self.feaname, bamname)] = st        

    def Schema_FeatureType(self, bamDict=None, bamproxyTypeOnly=False):
        ss =[]
        if (bamproxyTypeOnly is False):
            ss.append('/**\n')
            ss.append(' * @brief   : ' + self.feaname + ' Feature Structure\n')
            ss.append(' */\n')
            ss.append('typedef struct\n')
            ss.append('{\n')
            ss.append('    ' + 'FEATURE base;\n')
            ss.append('    ' + 'uint8_t stream_index;\n')
            ss.append(self.DONOTCHANGE_BELOW_1st)
        for bname, bcnt in bamDict.items():
            if bcnt > 1:
                ss.append('    ' + self.conv.strutypename_BamproxyType(bname) + ' ' + self.conv.variname_Bamproxy_BamType(self.feaname, bname) + '[' + str(bcnt) + '];\n')
            else:
                ss.append('    ' + self.conv.strutypename_BamproxyType(bname) + ' ' + self.conv.variname_Bamproxy_BamType(self.feaname, bname) + ';\n')
        if (bamproxyTypeOnly is False):
            ss.append(self.DONOTCHANGE_ABOVE_1st)
            ss.append('    ' + 'RET_CODE_TYPE (*Initialize)(FEATURE *pFeature, uint8_t stream_index);\n')
            ss.append('}' + self.conv.strutypename_FeatureType(self.feaname) + ';\n\n')
        return ''.join(ss)

    def Schema_FeatureDataType(self, bamDict=None, bamproxyDatatypeOnly=False):
        ss =[]
        if (bamproxyDatatypeOnly is False):
            ss.append('/**\n')
            ss.append(' * @brief   : ' + self.feaname + ' Feature Data Structure\n')
            ss.append(' */\n')
            ss.append('typedef struct\n')
            ss.append('{\n')  
            ss.append('    ' + 'bool     connect;\n')
            ss.append('    ' + 'bool     prev_connect;\n')
            ss.append('    ' + 'FEAT_RMDL_INFO rmdlInfo;\n')
            ss.append('    ' + 'TaskType muteNotifyTask;\n')
            ss.append(self.DONOTCHANGE_BELOW_1st)
        for bname, bcnt in bamDict.items():
            if bcnt > 1:
                ss.append('    ' + self.conv.strutypename_BamproxyDataType(bname) + ' ' + self.conv.variname_Bamproxy_BamDataType(self.feaname, bname) + '[' + str(bcnt) + '];\n')
            else:
                ss.append('    ' + self.conv.strutypename_BamproxyDataType(bname) + ' ' + self.conv.variname_Bamproxy_BamDataType(self.feaname, bname) + ';\n')
        if (bamproxyDatatypeOnly is False):
            ss.append(self.DONOTCHANGE_ABOVE_1st)
            ss.append('}' + self.conv.strutypename_FeatureDataType(self.feaname) + ';\n\n')
        return ''.join(ss)

    def Schema_FeatureVariables(self, variableList=None):
        ss = []
        for item in variableList:
            if (item[1] == None):
                ss.append(item[0] + ' '+ self.conv.variname_Feature_vtbl(self.feaname) + ';\n')
            else:
                ss.append(item[0] + ' ' + item[1] + ';\n')
        return ''.join(ss)

    def Schema_Feature_ConstructImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * ' + self.feaname + 'FeatureConstruct - Public API exposed to construct Feature.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Feature Construction Function\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - None.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'feature_Construct(&pFeature->base, &' + self.conv.variname_Feature_vtbl(self.feaname) + ', ' + '"' + self.feaname + '"' + ', (void *) pFeatureData, sizeof(pFeatureData));\n')
        ss.append('    ' + 'pFeature->Initialize = ' + self.conv.funcname_Feature_Initialize(self.feaname) + ';\n')
        ss.append('    ' + '// TODO: \n\n\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_FeatureInitializeImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * ' + self.feaname + 'FeatureInitialize - Public API exposed to Initialize Feature.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Feature Initialization Function\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - RET_SUCCESS, otherwise Error.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_ERROR;\n')
        featype = self.conv.strutypename_FeatureType(self.feaname)
        ss.append('    ' + featype + ' *pFeat = (' + featype + ' *)pFeature;\n')
        feadatatype = self.conv.strutypename_FeatureDataType(self.feaname)
        ss.append('    ' + feadatatype + ' *pFeatData = (' + feadatatype + ' *)pFeature->feature_data;\n')
        ss.append('\n\n')
        ss.append('    ' + 'pFeat->stream_index = stream_id;\n')
        ss.append('    ' + 'pFeatData->connect = false;\n')
        ss.append('    ' + 'pFeatData->prev_connect = false;\n')
        ss.append('    ' + '// Init notify task\n')
        ss.append('    ' + 'pFeatData->muteNotifyTask = INVALID_TASK;\n')
        ss.append('\n')
        ss.append('    ' + self.conv.funcname_Feature_InitializeShadow(self.feaname) + '(pFeatture, &CtrlBuf, &RespBuf, stream_id);\n')
        ss.append('    ' + '// TODO: \n\n\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_FeatureInitializeShadowImple(self, funcprototype=None, feacnt=None, bamDict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * ' + self.feaname + 'FeatureInitializeShadow - Public API exposed to Initialize All Bamproxys.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Bamproxy Initialization Function\n')
        ss.append(' * @param   - [IN] None\n')
        ss.append(' * @param   - [OUT] None\n')
        ss.append(' * @return  - RET_SUCCESS, otherwise Error.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_ERROR;\n')
        featype = self.conv.strutypename_FeatureType(self.feaname)
        ss.append('    ' + featype + ' *pFeat = (' + featype + ' *)pFeature;\n')
        feadatatype = self.conv.strutypename_FeatureDataType(self.feaname)
        ss.append('    ' + feadatatype + ' *pFeatData = (' + feadatatype + ' *)pFeature->feature_data;\n')
        ss.append('\n\n')
        for bname, bcnt in bamDict.items():
            ss.append('    ' + '//' + bname + '\n')
            fea = 'pFeat->' + self.conv.variname_Bamproxy_BamType(self.feaname, bname)
            feadata = 'pFeatData->' + self.conv.variname_Bamproxy_BamDataType(self.feaname, bname)
            if (bcnt > 1):
                ss.append('    ' + 'for (int i = 0; i < ' + str(bcnt) + '; i ++)\n')
                ss.append('    ' + '{\n')
                arr = '[i]'
                ss.append('        ' + self.conv.funcname_BamProxy_Construct(bname) + '(&' + fea + arr + ', &' + feadata + arr + ');\n')
                if (feacnt > 1):
                    ss.append('        ' + self.conv.funcname_SetupAPI_setup(self.feaname, bname) + '(&' + fea + arr + ', ' + 'i, stream_id);\n')
                else:
                    ss.append('        ' + self.conv.funcname_SetupAPI_setup(self.feaname, bname) + '(&' + fea + arr + ', ' + 'i);\n')
                ss.append('        ' + fea + arr + '.SetParentFeature((BAM_PROXY*)&' + fea + arr + ', pFeature);\n')
                ss.append('        ' + fea + arr + '.SetCtrlMessagePtr((BAM_PROXY*)&' + fea + arr + ', &CtrlBuf);\n')
                ss.append('        ' + fea + arr + '.SetRespMessagePtr((BAM_PROXY*)&' + fea + arr + ', &RespBuf);\n')
                ss.append('    ' + '}\n')
            else:
                ss.append('    ' + self.conv.funcname_BamProxy_Construct(bname) + '(&' + fea + ', &' + feadata + ');\n')
                if (feacnt > 1):
                    ss.append('    ' + self.conv.funcname_SetupAPI_setup(self.feaname, bname) + '(&' + fea + ', stream_id);\n')
                else:
                    ss.append('    ' + self.conv.funcname_SetupAPI_setup(self.feaname, bname) + '(&' + fea + ');\n')
                ss.append('    ' + fea + '.SetParentFeature((BAM_PROXY*)&' + fea + ', pFeature);\n')
                ss.append('    ' + fea + '.SetCtrlMessagePtr((BAM_PROXY*)&' + fea + ', &CtrlBuf);\n')
                ss.append('    ' + fea + '.SetRespMessagePtr((BAM_PROXY*)&' + fea + ', &RespBuf);\n')
        ss.append('\n')
        ss.append('}\n')
        return ''.join(ss)        

    def Schema_UpdateBamproxyInHeader(self, headerstr=None, bamproxyTypeStr=None, bamproxyDatatypeStr=None, bamnameEnumList=None, bamActionidEnumList=None):
        leading = JFeatureSchema.ENUM_START
        trailing = JFeatureSchema.ENUM_END
        enumStart = headerstr.find(leading)
        enumEnd = headerstr.find(trailing)
        b1 = '' if not bamnameEnumList else leading + '\n' + '\n'.join(bamnameEnumList)
        b2 = '' if not bamActionidEnumList else '\n'.join(bamActionidEnumList) + trailing + '\n\n'
        leading = 'typedef struct'
        trailing = 'FEAT_DATA_TYPE;'
        struStart = headerstr.find(leading)
        struEnd = headerstr.find(trailing)
        strList = headerstr[struStart : struEnd + len(trailing)].splitlines(True) if (struStart != -1) and (struEnd != -1) else ['']
        bamsign = bamdatasign = False
        if (struStart != -1) and (struEnd != -1):
            for i in range(1, len(strList) - 1):
                if strList[i].find('BAM_PROXY_TYPE') != -1:
                    strList[i] = bamproxyTypeStr if bamsign == False else ''
                    bamsign = True
                if strList[i].find('BAM_PROXY_DATA_TYPE') != -1:
                    strList[i] = bamproxyDatatypeStr if bamdatasign == False else ''
                    bamdatasign = True
        return headerstr[:enumStart] + b1 + b2 + headerstr[enumEnd + len(JFeatureSchema.ENUM_END) + 1 : struStart] + ''.join(strList) + headerstr[struEnd + len(trailing) + 1:] if (bamsign == True) and (bamdatasign == True) else ''


class JBamproxySchema(JCSchema):
    ENUM_START = '// Enumeration Start'
    ENUM_END = '// Enumeration End'
    def __init__(self, bamproxyname=None):
        self.bamname = bamproxyname
        self.conv = JFLConv()

    def Schema_BamproxyFilename(self, bamname=None, filetype=None):
        return self.conv.filename_Bamproxy(bamname, filetype)

    def Schema_ShadowFilename(self, bamname=None, filetype=None):
        return self.conv.filename_BamproxyShadow(bamname, filetype)

    def Schema_FileExtern(self, externList=None):
        ss = []
        for item in externList:
            ss.append('extern ' + item[0] + ' ' + item[1] + item[2] + ';\n')
        ss.append('\n')
        return ''.join(ss)

    def Schema_EnumBamAction(self, enumAction=None, enumRtc=None, enumTop=None, bamobj=None, bamname=None):
        st = []
        if 'RTC' in bamobj:
            for rtc in bamobj['RTC']:
                st.append(self.conv.defname_Bamproxy_RtcAction(rtc, bamname))
                enumRtc[rtc] = bamname + rtc
        if 'TOP' in bamobj:
            for top in bamobj['TOP']:
                st.append(self.conv.defname_Bamproxy_TopAction(top, bamname, 'w'))
                st.append(self.conv.defname_Bamproxy_TopAction(top, bamname, 'r'))
                enumTop[top] = bamname + top
        st.append(self.conv.defname_Bamproxy_numAction(bamname))
        enumAction[self.conv.enumname_Bamproxy_Action(bamname)] = st

    def Schema_BamproxyInitializeShadow(self, bamname=None):
        funcprototype = []
        funcprototype.append('RET_CODE_TYPE')
        funcprototype.append(self.conv.funcname_Bamproxy_InitializeShadow(bamname))
        funcprototype.append('(' + self.conv.strutypename_BamproxyType(bamname) + ' *bp, uint32_t *index)')
        funcprototype.append(bamname)
        return funcprototype

    def Schema_BamproxyFunc(self, suffix=None, bamname=None):
        funcprototype = []
        if (suffix == 'Init') or (suffix == 'Finalize') or (suffix == 'Response'):
            funcprototype.append('RET_CODE_TYPE')
            funcprototype.append(self.conv.funcname_Bamproxy_VirtualMethod(bamname, suffix))
            funcprototype.append('(' + self.conv.strutypename_BamproxyType(bamname) + '  *bp)')
            funcprototype.append(bamname)
        elif suffix == 'Construct':
            funcprototype.append('void')
            funcprototype.append(self.conv.funcname_BamProxy_Construct(bamname))
            argu = '(' + self.conv.strutypename_BamproxyType(bamname) + ' *f, ' + self.conv.strutypename_BamproxyDataType(bamname) + ' *d)'
            funcprototype.append(argu)
            funcprototype.append(bamname)
        elif suffix == 'AssignActionIds':
            funcprototype.append('static void')
            funcprototype.append(suffix)
            argu = '(' + self.conv.strutypename_BamproxyType(bamname) + ' *bp, RTC_ID_TYPE actions[' + self.conv.defname_Bamproxy_numAction(bamname) + '])'
            funcprototype.append(argu)
            funcprototype.append(bamname)
        else:
            #print('ERROR: Suffix not supported for Bamproxy: ' + bamname)   
            Jlog_print(JAG_ERROR, 'Suffix not supported for Bamproxy: ' + bamname)     
        return funcprototype

    def Schema_BamproxyType(self, bamname=None):
        ss = []
        if bamname is not None:
            numAct = self.conv.defname_Bamproxy_numAction(bamname)
            ss.append('/**\n')
            ss.append(' * @brief   : ' + bamname + ' Bamproxy Structure\n')
            ss.append(' */\n')
            ss.append('typedef struct\n')
            ss.append('{\n')
            ss.append('    ' + 'BAM_PROXY base;\n\n')
            ss.append('    ' + '/* store RTC' + "'" + 's for each action with method to set it*/\n')
            ss.append('    ' + 'RTC_ID_TYPE ActionId[' + numAct + '];\n')
            ss.append('    ' + 'void (*AssignActionIds)(' + self.conv.strutypename_BamproxyType(bamname) + ' *bp, RTC_ID_TYPE actions[' + numAct + ']);\n')
            ss.append('    ' + '\n    /* exposed volume methods */\n\n')
            ss.append('} ' + self.conv.strutypename_BamproxyType(bamname) + ';\n\n')
        return ''.join(ss)

    def Schema_BamproxyDataType(self, bamname=None):
        ss = []
        if bamname is not None:
            ss.append('/**\n')
            ss.append(' * @brief   : ' + bamname + ' Bamproxy Structure\n')
            ss.append(' */\n')
            ss.append('typedef struct\n')
            ss.append('{\n')            
            ss.append('    ' + '// TODO \n\n')
            ss.append('} ' + self.conv.strutypename_BamproxyDataType(bamname) + ';\n\n')
        return ''.join(ss)

    def Schema_UpdateActionInHeader(self, headerstr=None, actionEnumList=None):
        leading = JBamproxySchema.ENUM_START
        trailing = JBamproxySchema.ENUM_END
        enumStart = headerstr.find(leading)
        enumEnd = headerstr.find(trailing)
        b1 = '' if not actionEnumList else leading + '\n' + '\n'.join(actionEnumList) + trailing + '\n\n'
        return headerstr[:enumStart] + b1 + headerstr[enumEnd + len(JBamproxySchema.ENUM_END) + 1:]

    def Schema_BamproxyVtable(self, bamname=None):
        bamtype = self.conv.strutypename_BamproxyType(bamname)
        bamvtbl = self.conv.strutypename_BamproxyVtbl(bamname)
        ss = []
        ss.append('typedef struct\n')
        ss.append('{\n')
        ss.append('   ' + 'RET_CODE_TYPE (*Initialize)(' + bamtype + ' *bp);\n')
        ss.append('   ' + 'RET_CODE_TYPE (*Finalize)(' + bamtype + ' *bp);\n')
        ss.append('   ' + 'RET_CODE_TYPE (*Response)(' + bamtype + ' *bp);\n')
        ss.append('} ' + bamvtbl + ';\n\n')
        ss.append(bamvtbl + ' ' + self.conv.variname_Bamproxy_vtbl(bamname) + ' =\n')
        ss.append('{\n')
        ss.append('    ' + '.Initialize = ' + self.conv.funcname_Bamproxy_VirtualMethod(bamname, 'Init') + ',\n')
        ss.append('    ' + '.Finalize = ' + self.conv.funcname_Bamproxy_VirtualMethod(bamname, 'Finalize') + ',\n')
        ss.append('    ' + '.Response = ' + self.conv.funcname_Bamproxy_VirtualMethod(bamname, 'Response') + '\n')
        ss.append('};\n')
        return ''.join(ss)

    def Schema_BamProxyInitImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  ' + funcprototype[3] + 'BamProxyInit - Public\n')
        ss.append(' ********************************************************************************/\n')
        ss.append('/**\n')
        ss.append('* @brief   : Initialization function for ' + funcprototype[3] + ' Bam Proxy\n')
        ss.append('* @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append('* @return  - none.\n')
        ss.append('*/\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_SUCCESS;\n')
        bamdatatype = self.conv.strutypename_BamproxyDataType(funcprototype[3])
        ss.append('    ' + bamdatatype + ' *myData = (' + bamdatatype + '*) bp->base.bamProxy_data;\n')
        ss.append('    ' + 'uint32_t ignore;\n\n')
        ss.append('    ' + '// TODO: Initialize ' + funcprototype[3] + ' data\n\n')
        ss.append('    ' + '// Register each BAMF\n')
        ss.append('    ' + 'retCode = ' + self.conv.funcname_Bamproxy_InitializeShadow(funcprototype[3]) + '(bp, &ignore);\n')
        ss.append('    ' + '// Set EndPoint\n')
        ss.append('    ' + 'if (RET_SUCCESS == retCode)\n')
        ss.append('    ' + '{\n')
        ss.append('        ' + 'retCode = AssignEndpointBAM((BAM_PROXY *) bp);\n')
        ss.append('    ' + '}\n\n')
        ss.append('    ' + 'return retCode;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_BamProxyFinalizeImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  ' + funcprototype[3] + 'BamProxyFinalize - Public\n')
        ss.append(' ********************************************************************************/\n')
        ss.append('/**\n')
        ss.append('* @brief   : Finalize function for ' + funcprototype[3] + ' Bam Proxy\n')
        ss.append('* @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append('* @return  - none.\n')
        ss.append('*/\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_SUCCESS;\n')
        ss.append('    ' + 'return retCode;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_BamProxyResponseImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  ' + funcprototype[3] + 'BamProxyResponse - Public\n')
        ss.append(' ********************************************************************************/\n')
        ss.append('/**\n')
        ss.append('* @brief   : Response function for ' + funcprototype[3] + ' Bam Proxy\n')
        ss.append('* @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append('* @return  - none.\n')
        ss.append('*/\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_SUCCESS;\n')
        ss.append('    ' + 'audio_control_message *pRespMsg  = bp->base.pResp_message;\n\n')
        ss.append('    ' + 'if ( NULL != pRespMsg )\n')
        ss.append('    ' + '{\n')
        ss.append('        ' + '// Act on the Response Message\n')
        ss.append('    ' + '}\n')
        ss.append('    ' + 'else // error case\n')
        ss.append('    ' + '{\n')
        ss.append('        ' + 'retCode = RET_ERROR;\n')
        ss.append('    ' + '}\n')
        ss.append('    ' + 'return retCode;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_BamProxy_ConstructImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  ' + funcprototype[3] + 'BamProxy_Construct - Public\n')
        ss.append(' ********************************************************************************/\n')
        ss.append('/**\n')
        ss.append('* @brief   : Constructor function for ' + funcprototype[3] + ' Bam Proxy\n')
        ss.append('* @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append('* @param   - [IN] d - Pointer to data structure used in bam proxy\n')
        ss.append('* @return  - none.\n')
        ss.append('*/\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'bamproxy_Construct(&bp->base, (void *) d, sizeof(d));\n')
        ss.append('    ' + 'bp->base.vptr = (bamproxy_vtbl *)&' + self.conv.variname_Bamproxy_vtbl(funcprototype[3]) + ';\n\n')
        ss.append('    ' + '// RTC Set function pointer\n')
        ss.append('    ' + 'bp->AssignActionIds = AssignActionIds;\n\n')
        ss.append('    ' + '// TODO \n\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_AssignActionIdsImpl(self, funcprototype=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' *  Assign RTC Methods\n')
        ss.append(' ********************************************************************************/\n')
        ss.append('/**\n')
        ss.append('* @brief   : Assign Set Bass RTC for this bam proxy instance.\n')
        ss.append('* @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append('* @param   - [IN] actions - RTC_ID_TYPE array for ' + funcprototype[3] + ' actions\n')
        ss.append('* @return  - none.\n')
        ss.append('*/\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'memcpy(bp->ActionId, actions, sizeof(bp->ActionId));\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_BamproxyInitializeShadowImple(self, funcprototype=None, enumActionDict=None):
        ss = []
        ss.append('/*******************************************************************************\n')
        ss.append(' * ' + self.bamname + 'BamproxyInitializeShadow - Public API exposed to Register All RTC & TOP.\n')
        ss.append(' *******************************************************************************/\n')
        ss.append('/**\n')
        ss.append(' * @brief   : Bamproxy Initialization Function\n')
        ss.append(' * @param   - [IN] bp - Pointer that holds the bam proxy information.\n')
        ss.append(' * @param   - [IN] index - ignored\n')
        ss.append(' * @return  - RET_SUCCESS, otherwise Error.\n')
        ss.append(' */\n')
        ss.append(funcprototype[0] + ' ' + funcprototype[1] + funcprototype[2] + '\n')
        ss.append('{\n')
        ss.append('    ' + 'RET_CODE_TYPE retCode = RET_SUCCESS;\n')
        bamdatatype = self.conv.strutypename_BamproxyDataType(funcprototype[3])
        ss.append('    ' + bamdatatype + ' *myData = (' + bamdatatype + '*) bp->base.bamProxy_data;\n\n')
        for name, actionlist in enumActionDict.items():
            for i in range(len(actionlist) - 1):
                ss.append('    ' + 'retCode = (retCode == RET_SUCCESS) ? (RegisterBAMF((BAM_PROXY *) bp, bp->ActionId[' + actionlist[i] + '], &index) : (RetCode);\n')        
        ss.append('\n')
        ss.append('    ' + 'return retCode;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_RtcFuncImpl(self, bamname=None, rtcid=None, rtcargu=None):
        ss = []
        argu = rtcargu[:-2] + ', ' if len(rtcargu) > 0 else ''
        ss.append('uint32_t ' + self.conv.funcname_Bamproxy_rtccommand(bamname, rtcid) + '(uint32_t* data, ' + argu + 'uint32_t* msgid' + ')\n')

        ss.append('{\n')
        ss.append('    uint32_t msg_id = mActionID[' + self.conv.defname_Bamproxy_RtcAction(rtcid, bamname) + '];\n')
        ss.append('    uint32_t Index = TALARIA_HEADER_SIZE; // Skip the Talaria Header\n\n')
        ss.append('    // Fill control message header\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[0], 0);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], msg_id);\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)0x0);\n')
        # Serialize all function parameters
        preindex = 0
        postindex = 0
        functmp = rtcargu[:-2] if len(rtcargu) > 0 else ''
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
                ss.append('    for (int i = 0; i < ' + vsize + '; i ++)\n')
                ss.append('    {\n')
                if (vtype == 'float') or (vtype == 'real32_t') or (vtype == 'real32_T'):
                    ss.append('        put_int32_into_buffer((uint8_t *)&data[Index++], ' + '*(uint32_t *)(&' + vname + '[i]));\n')
                else:
                    ss.append('        put_int32_into_buffer((uint8_t *)&data[Index++], ' + vname + '[i]);\n')
                ss.append('    }\n')
                preindex = postindex
            else:
                preindex = postindex + 1
                postindex = functmp.find(', ', postindex)
                if (postindex == -1):
                    postindex = len(functmp)
                vname = functmp[preindex:postindex]
                if (vtype == 'float') or (vtype == 'real32_t') or (vtype == 'real32_T'):
                    ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], ' + '*(uint32_t *)(&' + vname + '));\n')
                else:
                    ss.append('    put_int32_into_buffer((uint8_t *)&data[Index++], ' + vname + ');\n')
                postindex += 2
                preindex = postindex
        ss.append('    //Update the Size\n')
        ss.append('    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));\n')
        ss.append('    if (msgid != NULL) {\n')
        ss.append('        *msgid = msg_id;\n')
        ss.append('    }\n\n')
        ss.append('    return Index;\n')
        ss.append('}\n')
        return ''.join(ss)

    def Schema_TopFuncImpl(self, bamname=None, topid=None):
        ss = []
        ss.append('uint32_t ' + self.conv.funcname_Bamproxy_toprequest(bamname, topid) + '(uint32_t* data, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* payload, uint32_t* msgid)\n')
        ss.append('{\n')
        ss.append('    uint32_t msg_id = mActionID[' + self.conv.defname_Bamproxy_TopAction(topid, bamname, 'w') + '];\n')
        ss.append('    uint32_t Index;\n\n')
        ss.append('    Index = top_write_request(data, msg_id, callhandler, offset, byte_count, payload);\n')
        ss.append('    if (msgid != NULL) {\n')
        ss.append('        *msgid = msg_id;\n')
        ss.append('    }\n\n')
        ss.append('    return Index;\n')
        ss.append('}\n')
        return ''.join(ss)




class JAudioGraphSchema(JCSchema):
    MODULE_LIST = 'Module_List.c'
    CONNECTION_LIST = 'Connection_List.c'
    EXECUTION_LIST = 'Execution_List.c'
    PORT_LIST = 'Port_List.c'
    AUDIO_GRAPH = 'Audio_Graph.c'
    def __init__(self):
        self.mlHeaderList = ['_BAF.h', 'Module_List.h', '_JModule_Id.h']
        self.clHeaderList = ['Connection_List.h']
        self.elHeaderList = ['Execution_List.h']
        self.plHeaderList = ['Port_List.h' ]
        self.agHeaderList = ['Audio_Graph.h']
    def Schema_ModuleListImplement(self, moduleList, extraheaderList):
        ss = ''
        for item in self.mlHeaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        for item in extraheaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        ss += '\n\n'
        ss += 'static Module_List Module_List_obj = {\n'
        ss += '    .num_items = ' + str(len(moduleList)) + ',\n'
        ss += '    {\n'
        for item in moduleList:
            ss += '        JModule_Bitfield_module(JMODULE_' + item.upper() + ', CONTEXT_0),\n'
        ss = ss[:-2]
        ss += '\n'
        ss += '    }\n'
        ss += '};\n\n'

        return ss

    def Schema_ConnectionListImplement(self, connectTuple, jframeobj, extraheaderList):
        ss = ''
        conSrc = 0
        conDst = 1
        num_of_item = len(connectTuple) if connectTuple != None else 0
        for item in self.clHeaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        for item in extraheaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        ss += '\n\n'
        ss += '#define Connection_List_get_connection(connection_list, connection_id) &(connection_list)->item[connection_id]\n'
        ss += '\n\n' 
        ss += 'static Connection_List Connection_List_obj = {\n'
        ss += '    .num_items = ' + str(num_of_item) + ',\n'
        ss += '    .item = {\n'
        if (num_of_item > 0):
            for item in connectTuple:
                ss += '                 {\n'
                constr = '                     JModule_Bitfield_port ('
                jm = item[conSrc][0]
                jp = item[conSrc][1]
                jt = item[conSrc][2]
                direct = jframeobj[jm].data[jp].data[jt].data['direction']
                if (direct == 'PORT_DIRECTION_IN'):
                    portitem = 'PORT_IN_'
                else:
                    portitem = 'PORT_OUT_'
                portitem += jm.upper() + '_' + jp.upper() + '_' + jt.upper()
                constr += direct
                constr += ', ' + portitem + ', PROCESS_' + item[conSrc][0].upper() + '_' + item[conSrc][1].upper() + ', JMODULE_' + item[conSrc][0].upper() + ', CONTEXT_0),\n'
                
                constr += '                     JModule_Bitfield_port ('
                jm = item[conDst][0]
                jp = item[conDst][1]
                jt = item[conDst][2]
                direct = jframeobj[jm].data[jp].data[jt].data['direction']
                if (direct == 'PORT_DIRECTION_IN'):
                    portitem = 'PORT_IN_'
                else:
                    portitem = 'PORT_OUT_'
                portitem += jm.upper() + '_' + jp.upper() + '_' + jt.upper()
                constr += direct
                constr += ', ' + portitem + ', PROCESS_' + item[conDst][0].upper() + '_' + item[conDst][1].upper() + ', JMODULE_' + item[conDst][0].upper() + ', CONTEXT_0)\n'
                ss += constr
                ss += '                 },\n'
        ss += '    }\n'
        ss += '};\n'
        return ss

    def Schema_ExecutionListImplement(self, exeTuple, extraheaderList):
        ss = ''
        for item in self.elHeaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        for item in extraheaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        ss += '\n\n'
        cnt = 0
        if (exeTuple != None):
            for item in exeTuple:
                exestr = 'static Execution_List Execution_List_' + str(cnt) + ' = {\n'
                exestr += '    .num_items = ' + str(len(item)) + ',\n'
                exestr += '    {\n'
                for subitem in item:
                    exestr += '        JModule_Bitfield_process(PROCESS_' + subitem[0].upper() + '_' + subitem[1].upper() + ', JMODULE_' + subitem[0].upper() + ',  CONTEXT_0),\n'
                exestr += '    }\n'
                exestr += '};\n\n'
                cnt += 1
                ss += exestr
        return ss

    def Schema_PortListImplement(self, connectTuple, jframeobj, extraheaderList):
        ss = ''
        cnt = 0
        conSrc = 0
        conDst = 1
        num_of_item = len(connectTuple) * 2 if connectTuple != None else 0
        for item in self.plHeaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        for item in extraheaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        ss += '\n\n'
        ss += 'static Port_List Port_List_obj = {\n'
        ss += '    .num_items = ' + str(num_of_item) + ',\n'
        ss += '    .item = {\n'
        if (num_of_item > 0):
            for item in connectTuple:
                plstr = '        { JModule_Bitfield_port ('
                jm = item[conSrc][0]
                jp = item[conSrc][1]
                jt = item[conSrc][2]
                direct = jframeobj[jm].data[jp].data[jt].data['direction']
                if (direct == 'PORT_DIRECTION_IN'):
                    portitem = 'PORT_IN_'
                else:
                    portitem = 'PORT_OUT_'
                portitem += jm.upper() + '_' + jp.upper() + '_' + jt.upper()
                plstr += direct
                plstr += ', ' + portitem + ', PROCESS_' + item[conSrc][0].upper() + '_' + item[conSrc][1].upper() + ', JMODULE_' + item[conSrc][0].upper() + ', CONTEXT_0),'
                plstr += ' BUFFER_' + str(cnt) + '},\n'
                plstr += '        { JModule_Bitfield_port ('
                jm = item[conDst][0]
                jp = item[conDst][1]
                jt = item[conDst][2]
                direct = jframeobj[jm].data[jp].data[jt].data['direction']
                if (direct == 'PORT_DIRECTION_IN'):
                    portitem = 'PORT_IN_'
                else:
                    portitem = 'PORT_OUT_'
                portitem += jm.upper() + '_' + jp.upper() + '_' + jt.upper()
                plstr += direct
                plstr += ', ' + portitem + ', PROCESS_' + item[conDst][0].upper() + '_' + item[conDst][1].upper() + ', JMODULE_' + item[conDst][0].upper() + ', CONTEXT_0),'
                plstr += ' BUFFER_' + str(cnt) + '},\n'
                cnt += 1
                ss += plstr
        ss += '    }\n'
        ss += '};\n'
        return ss

    def Schema_AudioGraphImplement(self, exeTuple):
        ss = ''
        num_of_item = len(exeTuple) if exeTuple != None else 0
        for item in self.agHeaderList:
            ss += self.Schema_IncludeHeaderFile(item, '"')
        ss += '\n\n'
        ss += 'static Audio_Graph Audio_Graph_obj = {\n'
        ss += '    .modules = &Module_List_obj,\n'
        ss += '    .connections = &Connection_List_obj,\n'
        ss += '    .ports = &Port_List_obj,\n'
        ss += '    .num_exec = ' + str(num_of_item) + ',\n'
        ss += '    .execution = {\n'
        for i in range(num_of_item):
            ss += '        &Execution_List_' + str(i) + ',\n'
        ss += '    }\n'
        ss += '};\n\n'
        return ss


class JModuleSchema(JCSchema):
    JMODULE_SYS = 'sys'
    JMODULE_MODEL = 'model'
    PREFIX_JMODULEDESC = 'JModule_Desc_'
    PREFIX_JMODULEADAPT = 'JModule_Adapt_'
    def __init__(self, jmodulename, schemaConfig=None):
        self.jmodule_type = JModuleSchema.JMODULE_SYS
        self.schemaConfig = schemaConfig
        if not self.schemaConfig.GetBafCallbackContext():
            self.adapt_baf_callback_sys_process = 'void BAF_Callback_sys_process(uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst);'
        else:
            self.adapt_baf_callback_sys_process = 'void _BAF_Callback_sys_process_ctx(void* framework, uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst, void* ctx);'
        self.adapt_init_sys_tpl = '''static void JModule_Adapt_Sys_init(JModule* this, Processes* processes, Processes_Context* context)
{
    this = (this == NULL) ? (NULL) : (this);
    processes = (processes == NULL) ? (NULL) : (processes);
    context = (context == NULL) ? (NULL) : (context);
}'''
        self.adapt_process_sys_tpl = 'static void JModule_Adapt__XXXXmodelname_process(JModule* this, Process* process, Process_Context* process_ctx, uint32_t thread_level, void* arg)\n'
        self.adapt_notify_sys_tpl = '''static void JModule_Adapt_XXXXmodelname_register_notify(JModule* this, JModule_Func_Notify* notify)
{
    this = (this == NULL) ? (NULL) : (this);
    notify = (notify == NULL) ? (NULL) : (notify);
}'''
        self.adapt_create_sys_tpl = 'static void JModule_Adapt_Sys_create(JModule* this)'
        self.adapt_macro_model_tpl = ''
        self.adapt_targetbuf_model_tpl = '''/* Definition of audio In/Out buffers expected by Talaria */
float *XXXXmodelname_Target_AudioOut;
float *XXXXmodelname_Target_AudioIn;'''
        self.adapt_talariafunc_model_tpl = '''/* Talaria functions */
extern void XXXXmodelname_init(void);
extern void XXXXmodelname_step(void);
extern int32_t XXXXmodelname_HandleAsdIdRequest(void *pReqBuf, uint32_t reqBufSize, void *pRspBuf, uint32_t rspBufSize);
extern int32_t XXXXmodelname_ServiceNotifications(void *pNtfBuf, uint32_t ntfBufSize);'''
        self.adapt_initfunc_model_tpl = '''static void JModule_Adapt_XXXXmodelname_init(JModule *this, Processes *processes, Processes_Context *context)
{
   this = (this == NULL) ? (NULL) : (this);
   processes = (processes == NULL) ? (NULL) : (processes);
   context = (context == NULL) ? (NULL) : (context);
XXXport_assignmentXXX

   XXXXfuncname();
}'''
        self.adapt_sendfunc_model_tpl = '''static uint32_t JModule_Adapt_XXXXmodelname_send(JModule *this, Processes_Context *context, Message *req, Message *rsp)
{
   int32_t status = XXXXfuncname(req->message_data, req->message_size, rsp->message_data, rsp->message_size);
   this = (this == NULL) ? (NULL) : (this);
   context = (context == NULL) ? (NULL) : (context);
   if (status >= 0)
   {
      rsp->message_id = req->message_id;
      rsp->message_size = status;
   }
   return (uint32_t)status;
}'''
        self.adapt_ipc_model_tpl = '''// Initialization routine called by model initialize
void XXXXmodelname_Init_IPC( void ){
};

/* Ping Pong: active buffer pointer declarations */
float *XXXXmodelname_Fwd_Ipc_buffer;'''
        self.adapt_processfunc_model_begin_tpl = '''static void JModule_Adapt_XXXXmodelname_process_XXXXprocessname(JModule *this, Process *process, Process_Context *process_ctx, uint32_t thread_level)
{
    Ports_Context *port_in_ctx = process_ctx->ports_in;
    Ports_Context *port_out_ctx = process_ctx->ports_out;
    
    this = (this == NULL) ? (NULL) : (this);
    process = (process == NULL) ? (NULL) : (process);
    thread_level = (thread_level == 0) ? (0) : (thread_level);\n'''
        self.adapt_processfunc_model_end_tpl = '''}\n'''
        self.adapt_notifyfunc_model_tpl = '''static void JModule_Adapt_XXXXmodelname_register_notify(JModule *this, JModule_Func_Notify notify, void *framework)
{
   this->notify = notify;
   this->framework = framework;
}'''
        self.adapt_pollfunc_model_tpl = '''static uint32_t JModule_Adapt_XXXXmodelname_poll(JModule *this, Processes_Context *context, Message *req, Message *rsp)
{
   uint32_t *msg = rsp->message_data;
   uint32_t ntfBytes;
   context = (context == NULL) ? (NULL) : (context);
   req = (req == NULL) ? (NULL) : (req);
   ntfBytes = XXXXfuncname(msg, rsp->message_size);
   if ((ntfBytes > 0) && (this->notify))
   {
      if (msg[2]==1)
      {
         rsp->module_id = this->id;
         rsp->message_size = ntfBytes; 
         this->notify(this->framework, rsp);
      }
   }
   return ntfBytes;
}'''
        self.adapt_createfunc_model_tpl = '''static void JModule_Adapt_XXXXmodelname_create(JModule *this)
{
XXXXexec
   this->init = JModule_Adapt_XXXXmodelname_init;
   this->command = JModule_Adapt_XXXXmodelname_send;
   this->register_notify = JModule_Adapt_XXXXmodelname_register_notify;
   this->poll = JModule_Adapt_XXXXmodelname_poll;
}'''
        self.desc_sys_str_tpl = '''typedef enum
{
	JMODULE_SYS_NUM_PROCESSES = NNNNnum,
	JMODULE_SYS_STATE_SIZE = 0,
	JMODULE_SYS_PARAM_SIZE = 0
} JModule_Desc_Sys;\n'''
        self.desc_model_str_tpl = '''typedef struct JModule_XXXXmodelname_Param
{
   uint32_t id;
} JModule_XXXXmodelname_Param;

typedef struct JModule_XXXXmodelname_State
{
   uint32_t id;
} JModule_XXXXmodelname_State;

enum JModule_Desc_XXXXmodelname
{
   JMODULE_XXXXMODELNAME_NUM_PROCESSES = 1,
   JMODULE_XXXXMODELNAME_STATE_SIZE = sizeof(JModule_XXXXmodelname_State),
   JMODULE_XXXXMODELNAME_PARAM_SIZE = sizeof(JModule_XXXXmodelname_Param)
};'''
        self.adapt_sys_str_tpl = '''#include <stdlib.h>

#include <stdint.h>
#include "Processes_Context.h"
#include "Message.h"
#include "JModule.h"


/**
 * @ingroup JModule_Adapt_XXXXmodelname
 *
 * @brief @param this
 */
\n'''
        self.adapt_model_str_tpl = '''#include "Process_Context.h"
#include "Processes_Context.h"
#include "Message.h"
#include "JModule.h"
 
/**
 * @ingroup JModule_Adapt_XXXXmodelname
 *
 * @brief @param processes
 * 
 * @param context
 */
\n'''

    def Schema_SetJModuleType(self, jmodule_type):
        self.jmodule_type = jmodule_type

    def Schema_GetJModuleDescName(self, jmodulename, filetype):
        return JModuleSchema.PREFIX_JMODULEDESC + jmodulename + filetype

    def Schema_GetJModuleAdaptName(self, jmodulename, filetype):
        return JModuleSchema.PREFIX_JMODULEADAPT + jmodulename + filetype

    def Schema_JMFileInfoComment(self, jmname, file_type):
        tlname = JModuleSchema.PREFIX_JMODULEDESC + jmname
        if file_type == '.h':
            if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
                ss = self.Schema_FileInfoComment(tlname, 'Declaration of the Enumeration ' + tlname)
            else:
                ss = self.Schema_FileInfoComment(tlname, 'Declaration of the Class' + 'JModule_' + jmname + '_Param')
        else:
            if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
                ss = self.Schema_FileInfoComment(tlname, 'Implementation of the Class ' + tlname)
            else:
                ss = self.Schema_FileInfoComment(tlname, 'Implementation of the Class' + 'JModule_' + jmname + '_Param')
        return ss

    def Schema_JMDescHeader(self, jmname, num_of_process, enumlist, enumcaselist, enumportlist):   
        if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
            tlname = JModuleSchema.PREFIX_JMODULEDESC + jmname
        else:
            tlname = 'JModule_' + jmname + '_Param'
        ss = self.Schema_IfdefBCStatementBegin(tlname)
        ss += '\n\n\n\n'
        ss += self.Schema_IfdefcplusplusStatementBegin()
        ss += '\n\n\n'
        if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
            ss += self.desc_sys_str_tpl.replace('NNNNnum', str(num_of_process))
        else:
            ss += self.Schema_IncludeHeaderFile('stdint.h', '<')
            ss += '\n\n'
            tmpstr = self.desc_model_str_tpl.replace('XXXXmodelname', jmname.capitalize())
            ss += tmpstr.replace('XXXXMODELNAME', jmname.upper())
        ss += '\n\n'
        # Add Port In & Out Enum
        if len(enumportlist) != 0:
            # Port In Enum
            for k,v in enumportlist[0].items():
                ss += self.Schema_EnumDefine('PORT_' + jmname.upper() + '_' + k.upper() +'_IN', v)
                ss += '\n\n'
            # Port Out Enum
            for k,v in enumportlist[1].items():
                ss += self.Schema_EnumDefine('PORT_' + jmname.upper() + '_' + k.upper()+ '_OUT', v)
                ss += '\n\n'
        # Add Case Enum for BAF_sys_process_callback(..)
        if len(enumcaselist) != 0:
            ss += self.Schema_EnumDefine('PROCESS_JCASENAME_' + jmname.upper(), enumcaselist)
        ss += '\n\n'
        # Add Enum for Processes
        ss += self.Schema_EnumDefine('PROCESS_JMODULENAME_' + jmname.upper(), enumlist)
        ss += '\n\n'
        ss += self.Schema_IfdefcplusplusStatementEnd()
        ss += '\n'
        ss += self.Schema_IfdefBCStatementEnd(tlname)
        return ss

    def Schema_JMAdaptHeader(self, jmname):
        tlname = JModuleSchema.PREFIX_JMODULEADAPT + jmname
        ss = self.Schema_IfdefBCStatementBegin(tlname)
        ss += '\n\n\n\n'
        ss += self.Schema_IfdefcplusplusStatementBegin()
        ss += '\n\n\n'
        if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
            ss += self.adapt_sys_str_tpl.replace('XXXXmodelname', jmname.capitalize())
        else:
            ss += self.adapt_model_str_tpl.replace('XXXXmodelname', jmname.capitalize())
        ss += '\n\n'
        ss += self.Schema_IfdefcplusplusStatementEnd()
        ss += '\n'
        ss += self.Schema_IfdefBCStatementEnd(tlname)
        return ss

    def Schema_JMDescImplement(self, jname, procList, portDict, returnportList):
        ss = ''
        # The struct format: {processname:[num_of_portin, num_of_portout, str_for_portin, str_for_portout]}
        portStruct = {}
        inlist = {}
        outlist = {}
        for name, item in portDict.items():
            subname = name[:name.find('#')]
            if not (subname in portStruct):
                portStruct[subname] = [0, 0, '', '']
                portStruct[subname][2] = 'static Ports JModule_' + jname.capitalize() + '_process_' + subname + '_in_port = {\n'
                portStruct[subname][2] += '    .num_items = xxxNumIn,\n'
                portStruct[subname][3] = 'static Ports JModule_' + jname.capitalize() + '_process_' + subname + '_out_port = {\n'
                portStruct[subname][3] += '    .num_items = xxxNumOut,\n'        
            if item['direction'] == 'PORT_DIRECTION_IN':
                num_of_port_offset = 0
                str_of_port_offset = 2
                if subname not in inlist:
                    inlist[subname]=[] 
                inlist[subname].append('PORT_IN_' + jname.upper() + '_' + subname + '_' + name[name.find('#') + 1:].upper())
            else:
                num_of_port_offset = 1
                str_of_port_offset = 3
                if subname not in outlist:
                    outlist[subname]=[] 
                outlist[subname].append('PORT_OUT_' + jname.upper() + '_' + subname + '_' + name[name.find('#') + 1:].upper())
            if (portStruct[subname][num_of_port_offset] == 0):
                portStruct[subname][str_of_port_offset] += '    {{\n'
            else:
                portStruct[subname][str_of_port_offset] += '    {\n'          
            portStruct[subname][str_of_port_offset]  += '        .data_type = '
            portStruct[subname][str_of_port_offset] +=  item['data_type'] if 'data_type' in item else ''
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .direction = ' 
            portStruct[subname][str_of_port_offset] += item['direction'] if 'direction' in item else ''
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .frame_size = ' 
            portStruct[subname][str_of_port_offset] += str(item['frame_size']) if 'frame_size' in item else '0'
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .num_chan = ' 
            portStruct[subname][str_of_port_offset] += str(item['num_chan']) if 'num_chan' in item else '0'
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .sample_rate = ' 
            portStruct[subname][str_of_port_offset] += str(item['sample_rate']) if 'sample_rate' in item else '0'
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .sample_size = ' 
            portStruct[subname][str_of_port_offset] += str(item['sample_size']) if 'sample_size' in item else '4'
            portStruct[subname][str_of_port_offset] += ',\n'
            portStruct[subname][str_of_port_offset] += '        .passthru_port = ' 
            portStruct[subname][str_of_port_offset] += str(item['passthru_port']) if 'passthru_port' in item else '0'
            portStruct[subname][str_of_port_offset] += '\n'
            portStruct[subname][str_of_port_offset] += '    },\n'
            portStruct[subname][num_of_port_offset] = portStruct[subname][num_of_port_offset] + 1
        for k, v in portStruct.items():
            if (v[0] != 0):
                v[2] = v[2][:-2]
                v[2] += '}\n'
                v[2] += '};\n\n'
                ss += v[2].replace('xxxNumIn', str(v[0]))
            if (v[1] != 0):
                v[3] = v[3][:-2]
                v[3] += '}\n'
                v[3] += '};\n\n'
                ss += v[3].replace('xxxNumOut', str(v[1]))
        ss += '\n'
        returnportList.append(inlist)
        returnportList.append(outlist)
        procstr = 'static Processes JModule_' + jname.capitalize() + '_processes = {\n'
        procstr += '    .num_items = ' + str(len(procList)) + ',\n'
        procstr += '    {{\n'
        for item in procList:         
            for k, v in item.items():
                procstr += '        .ports_in = '
                subprocname = k + '_in'
                procstr += '&JModule_' + jname.capitalize() + '_process_' + subprocname + '_port' if (v[1] is not None) else 'NULL'
                procstr += ',\n'
                procstr += '        .ports_out = '
                subprocname = k + '_out'
                procstr += '&JModule_' + jname.capitalize() + '_process_' + subprocname + '_port' if (v[0] is not None) else 'NULL'
                procstr += ',\n'
                procstr += '        .call_interval = '
                procstr += str(v[2]) if v[2] is not None else '0'
                procstr += ',\n'
                procstr += '        .exec = NULL,\n'
                procstr += '        .feature = NULL\n'
            procstr += '    },{\n'
        procstr = procstr[:-4]
        procstr += '}}\n'
        procstr += '};\n'
        ss += procstr
        ss += '\n\n'
        if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
            ss += 'static JModule JModule_Sys_obj = {\n'
            ss += '    .id = JMODULE_SYS,\n'
            ss += '    .create = JModule_Adapt_Sys_create, \n'
            ss += '    .processes = &' + 'JModule_' + jname.capitalize() + '_processes\n'
            ss += '};\n'
        else:
            tmp = jname.capitalize()
            ss += 'static JModule_' + tmp + '_State JModule_' + tmp + '_State_obj;\n'
            ss += 'static JModule_' + tmp + '_Param JModule_' + tmp + '_Param_obj;\n'
            ss += '\n'
            ss += 'static JModule JModule_' + tmp + '_obj = {\n'
            ss += '    .id = JMODULE_' + jname.upper() + ',\n'
            ss += '    .processes = &JModule_' + tmp + '_processes,\n'
            ss += '    .create = JModule_Adapt_' + jname + '_create,\n'
            ss += '    .param = &JModule_' + tmp + '_Param_obj,\n'
            ss += '    .state = &JModule_' + tmp + '_State_obj,\n'
            ss += '    .init = NULL,\n'
            ss += '    .command = NULL,\n'
            ss += '    .register_notify = NULL,\n'
            ss += '    .state_size = 0,\n'
            ss += '    .param_size = 0\n'
            ss += '};\n'
        return ss

    def Schema_JMAdaptImplement(self, jname, procList, portDict, jmoduleDict, returnProcEnumList, returnCaseEnumList):
        #ss = self.Schema_IncludeHeaderFile(JModuleSchema.PREFIX_JMODULEADAPT + jname.capitalize() + '.h', '"')
        ss = ''
        if (self.jmodule_type == JModuleSchema.JMODULE_SYS):
            ss += self.Schema_JMAdaptSysImplement(jname, procList, portDict, returnProcEnumList, returnCaseEnumList)
        else:
            ss += self.Schema_JMAdaptModelImplement(jname, procList, portDict, jmoduleDict, returnProcEnumList)
        return ss

    def Schema_JMAdaptSysImplement(self, jname, procList, portDict, returnProcEnumList, returnCaseEnumList):
        ss = self.adapt_baf_callback_sys_process + '\n\n'
        ss += self.adapt_init_sys_tpl + '\n\n'
        cnt_port = 0
        execstr = ''
        inNum = outNum = 0
        for item in procList:
            procInNum = procOutNum = 0
            procstr = ''
            # Each item only includes one key-value pair
            for k, v in item.items():
                returnProcEnumList.append('PROCESS_' + jname.upper() + '_' + k.upper())
                procstr += self.adapt_process_sys_tpl.replace('_XXXXmodelname', 'Sys_' + k + '_port')
                procstr += '{\n'
                if (v[0] is not None):
                    # Port Out
                    procstr += '    Ports *port_out = process->ports_out;\n'
                    procstr += '    Ports_Context *port_out_ctx = process_ctx->ports_out;\n'
                    procstr += '    void* dst;\n\n'
                    procstr += '    this = (this == NULL) ? (NULL) : (this);\n'
                    procstr += '    thread_level = (thread_level == 0) ? (0) : (thread_level);\n'
                    for pk, pv in portDict.items():
                        procname = pk[:pk.find('#')]
                        if procname == k:
                            enum_name = 'PROCESS_' + jname.upper() + '_' + pk.replace('#', '_').upper()
                            procstr += '    dst =  port_out_ctx->item[' + str(procOutNum) + '].payload;\n'
                            if not self.schemaConfig.GetBafCallbackContext():
                                procstr += '    BAF_Callback_sys_process(' + enum_name + '/*proc_id*/, port_out->port[' + str(procOutNum) + '].frame_size,\n'
                                procstr += '	    0 /* num_chan_in*/, port_out->port[' + str(procOutNum) + '].num_chan, NULL, dst);\n'
                            else:
                                procstr += '    _BAF_Callback_sys_process_ctx(this->framework, ' + enum_name + '/*proc_id*/, port_out->port[' + str(procOutNum) + '].frame_size,\n'
                                procstr += '         0 /* num_chan_in*/, port_out->port[' + str(procOutNum) + '].num_chan, NULL, dst, arg);\n'
                            enum_name += ' = ' + str(outNum * 2)
                            returnCaseEnumList.append(enum_name)
                            outNum = outNum + 1
                            procOutNum = procOutNum + 1
                    procstr += '}\n\n'
                else:
                    # Port In
                    procstr += '    Ports *port_in = process->ports_in;\n'
                    procstr += '    Ports_Context *port_in_ctx = process_ctx->ports_in;\n'
                    procstr += '    void* src;\n\n'
                    procstr += '    this = (this == NULL) ? (NULL) : (this);\n'
                    procstr += '    thread_level = (thread_level == 0) ? (0) : (thread_level);\n'
                    for pk, pv in portDict.items():
                        procname = pk[:pk.find('#')]
                        if procname == k:
                            enum_name = 'PROCESS_' + jname.upper() + '_' + pk.replace('#', '_').upper()
                            procstr += '    src =  port_in_ctx->item[' + str(procInNum) + '].payload;\n'
                            if not self.schemaConfig.GetBafCallbackContext():
                                procstr += '    BAF_Callback_sys_process(' + enum_name + '/*proc_id*/, port_in->port[' + str(procInNum) + '].frame_size,\n'
                                procstr += '	    port_in->port[' + str(procInNum) + '].num_chan, 0 /* num_chan_out*/, src, NULL);\n'
                            else:
                                procstr += '    _BAF_Callback_sys_process_ctx(this->framework, ' + enum_name + '/*proc_id*/, port_in->port[' + str(procInNum) + '].frame_size,\n'
                                procstr += '        port_in->port[' + str(procInNum) + '].num_chan, 0 /* num_chan_out*/, src, NULL, arg);\n'
                            enum_name += ' = ' + str(1 + inNum * 2)
                            returnCaseEnumList.append(enum_name)
                            inNum = inNum + 1   
                            procInNum = procInNum + 1
                    procstr += '}\n\n'                      
                execstr += '    this->processes->process[' + str(cnt_port) + '].exec = JModule_Adapt_Sys_' + k + '_port_process;\n'
                cnt_port += 1
            ss += procstr        
        ss += self.adapt_notify_sys_tpl.replace('XXXXmodelname', jname) + '\n'
        ss += self.adapt_create_sys_tpl + '\n'
        ss += '{\n'
        ss += execstr
        ss += '    this->init = JModule_Adapt_Sys_init;\n'
        ss += '    this->command = NULL;\n'
        ss += '    this->register_notify = NULL;\n'
        ss += '}\n'
        
        return ss
    
    def Schema_JMAdaptModelImplement(self, jname, procList, portDict, jmoduleDict, returnProcEnumList):
        ss = '\n'
        ss += self.adapt_macro_model_tpl + '\n'
        ss += '/* Definition of audio In/Out buffers expected by Talaria */\n'
        # Format: {name_of_process: [name_of_port1, direction_of_port1, name_of_port2, direction_of_port2, ...]}
        pointerDict = {}
        for k, v in portDict.items():
            ss += 'float* ' + v['port'].replace('/', '_').replace(' ', '_') + ';\n'
            procname = k[:k.find('#')]
            if not (procname in pointerDict):
                pointerDict[procname] = []
            pointerDict[procname].append(v['port'])
            pointerDict[procname].append(v['direction'])
        ss += '\n'
        # Take 'init' as exmple, and get letters before the first '_'
        mdlstr = jmoduleDict[jname]['init'][:-2]
        mdlstr = mdlstr[:mdlstr.find('_')]
        # extern Talaria functions
        externstr = '/* Talaria functions */\n'
        if 'init' in jmoduleDict[jname].keys():
            externstr += 'extern void ' + jmoduleDict[jname]['init'][:-2] + '(void);\n'
        for item in procList:
            for k, v in item.items():
                if (v[3] is not None):
                    externstr += 'extern void ' + v[3][:v[3].find('(')]
                    externstr += '(void);\n' if '()' in v[3] else '(uint32_t stepnum);\n'
        if 'send' in jmoduleDict[jname].keys():
            externstr += 'extern int32_t ' + jmoduleDict[jname]['send'][:-2] + '(void *pReqBuf, uint32_t reqBufSize, void *pRspBuf, uint32_t rspBufSize);\n'
        if 'poll' in jmoduleDict[jname].keys():
            externstr += 'extern int32_t ' + jmoduleDict[jname]['poll'][:-2] + '(void *pNtfBuf, uint32_t ntfBufSize);\n'
        ss += externstr + '\n\n'
        #ss += self.adapt_talariafunc_model_tpl.replace('XXXXmodelname', mdlstr) + '\n\n'
        # init function
        if 'init' in jmoduleDict[jname].keys():
            tmp = self.adapt_initfunc_model_tpl.replace('XXXXfuncname', jmoduleDict[jname]['init'][:-2])
        else:
            tmp = self.adapt_initfunc_model_tpl.replace('XXXXfuncname', '')
        tmp = tmp.replace('XXXXmodelname', jname)
        ppstr = ''
        proc_id = 0
        for item in procList:
            portstr = ''
            # Here item only have one key-value pair
            for k, v in item.items():
                # Aduio In/Out Pointer assignment
                pointerstr = ''
                inCnt = outCnt = 0
                if k in pointerDict:
                    for i in range(0, len(pointerDict[k]), 2):
                        if pointerDict[k][i + 1] == 'PORT_DIRECTION_IN':
                            pointerstr += '   ' + pointerDict[k][i].replace('/', '_').replace(' ', '_') + ' = (float *)(context->item[' + str(proc_id) + '].ports_in->item[' + str(inCnt) + '].payload);\n'
                            inCnt = inCnt + 1
                        else:
                            pointerstr += '   ' + pointerDict[k][i].replace('/', '_').replace(' ', '_') + ' = (float *)(context->item[' + str(proc_id) + '].ports_out->item[' + str(outCnt) + '].payload);\n'
                            outCnt = outCnt + 1              
                portstr += pointerstr
            proc_id += 1
            ppstr += portstr
        ss += tmp.replace('XXXport_assignmentXXX', ppstr) + '\n\n'

        # send function
        if 'send' in jmoduleDict[jname].keys():
            ss += self.adapt_sendfunc_model_tpl.replace('XXXXfuncname', jmoduleDict[jname]['send'][:-2]).replace('XXXXmodelname', jname)
        else:
            ss += self.adapt_sendfunc_model_tpl.replace('XXXXfuncname', '').replace('XXXXmodelname', jname)
        ss += '\n\n'
        # IPC declaration
        ss += self.adapt_ipc_model_tpl.replace('XXXXmodelname', jname.capitalize()) + '\n\n'
        # process function
        execstr = ''
        cnt_exec = 0
        for item in procList:
            procstr = self.adapt_processfunc_model_begin_tpl
            procstr = procstr.replace('XXXXmodelname', jname)
            # Here item only have one key-value pair
            for k, v in item.items():
                enum_name = 'PROCESS_' + jname.upper() + '_' + k.upper()
                returnProcEnumList.append(enum_name)
                procstr = procstr.replace('XXXXprocessname', k)
                # Remove port_in/out_ctx cause the part is moved to init function.
                procstr = procstr.replace('\n    Ports_Context *port_in_ctx = process_ctx->ports_in;', '')
                procstr = procstr.replace('\n    Ports_Context *port_out_ctx = process_ctx->ports_out;', '')                   
                if (v[3] is not None):
                    # Call Step function defined in JConfig.py for Process
                    procstr += '\n    ' + v[3] + ';\n' 
                execstr += '    this->processes->process[' + str(cnt_exec) + '].exec = JModule_Adapt_'+jname+'_process_' + k + ';\n'
                cnt_exec += 1
            procstr += self.adapt_processfunc_model_end_tpl
            ss += procstr + '\n\n'
        # notify function
        ss += self.adapt_notifyfunc_model_tpl.replace('XXXXmodelname', jname) + '\n\n'
        # poll function
        if 'poll' in jmoduleDict[jname].keys():
            ss += self.adapt_pollfunc_model_tpl.replace('XXXXfuncname', jmoduleDict[jname]['poll'][:-2]).replace('XXXXmodelname', jname)
        else:
            ss += self.adapt_pollfunc_model_tpl.replace('XXXXfuncname', '').replace('XXXXmodelname', jname)
        ss += '\n\n'
        # create function
        tmpstr = self.adapt_createfunc_model_tpl
        tmpstr = tmpstr.replace('XXXXmodelname', jname)
        tmpstr = tmpstr.replace('XXXXexec', execstr[:-1])
        ss += tmpstr + '\n'
        return ss


class JFrameworkSchema(JCSchema):
    DEFAULT_UFHEADERFILENAME = 'Unit_Framework.h'
    DEFAULT_JMIDHEADERFILENAME = '_JModule_Id.h'
    DEFAULT_JMLISTCFILENAME = 'JModule_List.c'
    DEFAULT_JMODULEIDENUMNAME = '_JModule_Id'
    def __init__(self, modulename, schemaConfig):
        self.jmodulename = ''
        self.schemaConfig = schemaConfig

    def Schema_SetJModuleName(self, jmodulename):
        self.jmodulename = jmodulename

    def Schema_GetJMIDHeaderfileName(self):
        return JFrameworkSchema.DEFAULT_JMIDHEADERFILENAME

    def Schema_GetJMListCfileName(self):
        return JFrameworkSchema.DEFAULT_JMLISTCFILENAME

    def Schema_GetUFHeaderfileName(self):
        return JFrameworkSchema.DEFAULT_UFHEADERFILENAME

    def Schema_JModuleIdImplement(self, idlist):
        jmoduleid_list = []
        for item in idlist:
            jmoduleid_list.append('JMODULE_' + item)
        jmoduleid_list.append('JMODULE_NUM_MODULE')
        ss = self.Schema_IfdefBCStatementBegin(JFrameworkSchema.DEFAULT_JMODULEIDENUMNAME)
        ss += '\n\n\n\n'
        ss += self.Schema_IfdefcplusplusStatementBegin()
        ss += '\n\n\n'
        ss += self.Schema_EnumDefine(JFrameworkSchema.DEFAULT_JMODULEIDENUMNAME, jmoduleid_list)
        ss += '\n\n'
        ss += self.Schema_IfdefcplusplusStatementEnd()
        ss += '\n'
        ss += self.Schema_IfdefBCStatementEnd(JFrameworkSchema.DEFAULT_JMODULEIDENUMNAME)
        return ss

    def Schema_JModuleListImplement(self, idlist):
        
        ss = '\n'
        ss += 'static JModule_List JModule_List_obj = {\n'
        ss += '    .num_items = '+str(len(idlist))+',\n'
        ss += '    .item = {\n'
        for item in idlist:
            ss += '        &JModule_' + item.capitalize() + '_obj,\n'
        ss += '    }\n'
        ss += '};\n\n'
        ss += 'static _BAF '+self.schemaConfig.GetBafObjectName()+' = {\n'
        ss += '	.feature_list = &JModule_List_obj,\n'
        ss += '	.audio_graph = &Audio_Graph_obj\n'
        ss += '};\n\n'
        ss += 'BAF* p'+self.schemaConfig.GetBafObjectName()+' = &'+self.schemaConfig.GetBafObjectName()+';\n'
        
        return ss

    def Schema_GetUFHeaderComment(self):
        ss = self.Schema_Define('JMODULE_UNIT', 'JMODULE_' + self.jmodulename.upper())
        ss += self.Schema_Define('JModule_Unit_obj', 'JModule_' + self.jmodulename.capitalize() + '_obj')
        return ss


class JProbeJModuleSchema(JCSchema):
    def __init__(self, modelname=None):
        self.modelname = modelname

    def Schema_AddModelHeaders(self):
        ss = []
        ss.append('#include "TalariaAPI.h"\n')
        ss.append('#include MODEL_HEADER_FILENAME\n\n\n')
        return ''.join(ss)

    def Schema_AddExportBufFunc(self, func_str):
        ss = []
        ss.append('#ifdef RTW_HEADER_' + self.modelname + '_TalariaAPI_h_\n')
        # Add Routing function
        ss.append('\nstatic int ch_num = 0;\n')
        ss.append('void setChannelNum(int ch)\n')
        ss.append('{\n')
        ss.append('    ch_num = ch;\n')
        ss.append('}\n\n')
        ss.append(func_str)
        ss.append('#endif\n\n\n')
        return ''.join(ss)

    def Schema_AddFillBuffer(self, func_str, modelFs, modelCh):
        ss = []
        ss.append('\n')
        ss.append('#ifdef RTW_HEADER_' + self.modelname + '_TalariaAPI_h_\n')
        # Add Export Buffer function
        r = func_str.find('(')
        l = func_str[:r].rfind(' ')
        ss.append('    int i, j;\n')
        ss.append('    int min_ch, min_fs, output_fs, buffer_fs, output_ch, buffer_ch;\n')
        ss.append('    float* srcptr = ' + func_str + '();\n')
        ss.append('    float* dstptr = src;\n\n')
        ss.append('    buffer_fs = ' + str(modelFs) + ';\n')
        ss.append('    output_fs = port_in->port[0].frame_size;\n')
        ss.append('    buffer_ch = ' + str(modelCh) + ';\n')
        ss.append('    output_ch = port_in->port[0].num_chan;\n')
        ss.append('    min_ch = (buffer_ch <= output_ch) ? (buffer_ch) : (output_ch);\n')
        ss.append('    min_fs = (buffer_fs <= output_fs) ? (buffer_fs) : (output_fs);\n')
        ss.append('    // Routing for 1st Channel\n')
        ss.append('    memcpy(dstptr, srcptr + ch_num * buffer_fs, min_fs * sizeof(float));\n')
        ss.append('    for (i = 1; i < min_ch; i ++) {\n')
        ss.append('        memcpy(dstptr + i * output_fs, srcptr + i * buffer_fs, min_fs * sizeof(float));\n')
        ss.append('        memset(dstptr + i * output_fs + min_fs, 0, (output_fs - min_fs) * sizeof(float));\n')
        ss.append('    }\n')
        ss.append('#endif\n\n')
        return ''.join(ss)