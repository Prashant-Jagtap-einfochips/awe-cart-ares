import sys
import os


from datetime import date
from JConv import *
from JLogPrint import *



class JPySchema:
    STANDARDIMPORT = ['sys', 'os']
    def __init__(self):
        pass
    
    def Schema_ImportStandardPackage(self):
        ss = []
        for item in JPySchema.STANDARDIMPORT:
            ss.append('import ' + item + '\n')
        return ''.join(ss)


class JFrameworkPySchema(JPySchema):
    def __init__(self, jframework_name=None, alignOffset=3):
        self.jframeworkname = jframework_name
        self.align = ' ' * alignOffset

    def Schema_ImportJFrameworkPackage(self, standardpktList=[], extrapktList=[]):
        ss = []
        for item in standardpktList:
            ss.append('import ' + item + '\n')
        for item in extrapktList:
            ss.append('from ' + item + ' import *\n')
        return ''.join(ss)

    def Schema_JFrameworkDefStart(self):
        return 'class jframework_' + self.jframeworkname + '(jframework):\n'

    def Schema_JFrameworkDefaultFunc(self, jfrmobj=None):
        ss = []
        ss.append(self.align + 'def default(self, param=[32, 48000, 0, 0]):\n')
        if jfrmobj is not None:
            for key, value in jfrmobj.__dict__.items():
                if (key != 'id') and (key != 'data') and (key != 'implemented'):
                    vl = "'" + value + "'" if type(value) is str else str(value)
                    if(key=='xml' or key=='partition'):
                        ss.append('#')
                    ss.append(self.align * 2 + 'self.' + key + '=' + vl + '\n')
        else:
            ss.append(self.align * 2 + 'pass\n')
        return ''.join(ss)

    def Schema_JframeworkSelfData(self, jfrmobj=None, baseid=0, sysPortEmpty=False):
        ss = []
        if jfrmobj is not None:
            mjmodule = []
            sjmodule = []
            graph = []
            jmodule_id = baseid
            ss.append(self.align * 2 + 'self.data = {\n')
            for key, modulename in jfrmobj.data.items():
                if hasattr(modulename, 'type') and modulename.type == 'JModule':
                    if key.upper().startswith('SYS'):
                        self.jmSchema = JModulePySchema(key)
                        sjmodule.append(self.align * 3 + "'" + key + "'" + ':jmodule({\n')
                        for procname, procobj in modulename.data.items():
                            if hasattr(procobj, 'type') and procobj.type == 'JProcess':
                                # Write JProcess start
                                sjmodule.append(self.align * 2 + self.jmSchema.Schema_JProcessDefStart(procname))
                                for name, obj in procobj.data.items():
                                    if hasattr(obj, 'type') and obj.type == 'JPortIn':
                                        # Write Port In
                                        sjmodule.append(self.align * 2 + self.jmSchema.Schema_PortIn(name, obj, sysPortEmpty))
                                    elif hasattr(obj, 'type') and obj.type == 'JPortOut':
                                        # Write Port Out
                                        sjmodule.append(self.align * 2 + self.jmSchema.Schema_PortOut(name, obj, sysPortEmpty))
                                    elif name == 'callrate':
                                        sjmodule.append(self.align * 2 + self.jmSchema.Schema_Callrate(name, obj))
                                    elif name == 'process':
                                        sjmodule.append(self.align * 2 + self.jmSchema.Schema_Process(name, obj))
                                # Write JProcess end
                                sjmodule.append(self.align * 2 + self.jmSchema.Schema_JProcessDefEnd())
                        #sjmodule.append(self.align * 3 + '}, id=' + str(jfrmobj.id) + ')\n')
                        sjmodule.append(self.align * 3 + '}, id=' + str(jmodule_id) + '),\n')
                        jmodule_id = jmodule_id + 1
                    else:
                        tmpss = []
                        self.jmSchema = JModulePySchema(key)
                        tmpss.append(self.align * 3 + "'" + key + "'" + ':jmodule({\n')
                        tmpss.append(self.align * 2 + self.jmSchema.Schema_JProcessInit(key))
                        tmpss.append(self.align * 2 + self.jmSchema.Schema_JProcessSend(key))
                        tmpss.append(self.align * 2 + self.jmSchema.Schema_JProcessPoll(key))
                        for procname, procobj in modulename.data.items():
                            if hasattr(procobj, 'type') and procobj.type == 'JProcess':
                                # Write JProcess start
                                tmpss.append(self.align * 2 + self.jmSchema.Schema_JProcessDefStart(procname))
                                for name, obj in procobj.data.items():
                                    if hasattr(obj, 'type') and obj.type == 'JPortIn':
                                        # Write Port In
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_PortIn(name, obj))
                                    elif hasattr(obj, 'type') and obj.type == 'JPortOut':
                                        # Write Port Out
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_PortOut(name, obj))
                                    elif name == 'callrate':
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_Callrate(name, obj))
                                    elif name == 'process':
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_Process(name, obj))
                                    elif name == 'rate_period':
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_Rateinsec(name, obj))
                                    elif name == 'base_task':
                                        tmpss.append(self.align * 2 + self.jmSchema.Schema_Baserate(name, obj))
                                # Write JProcess end
                                tmpss.append(self.align * 2 + self.jmSchema.Schema_JProcessDefEnd())
                        #tmpss.append(self.align * 3 + '}, id=' + str(jfrmobj.id) + ')\n')
                        tmpss.append(self.align * 3 + '}, id=' + str(jmodule_id) + '),\n')
                        jmodule_id = jmodule_id + 1
                        mjmodule.append(''.join(tmpss))
                elif (key.upper().startswith('SYS')) and (modulename is None):
                    # In case 'sys' is None
                    sjmodule.append(self.align * 3 + "'" + key + "'" + ': None\n')
                elif (key == 'graph'):
                    graph.append(self.align * 3 + "'" + key + "':[")
                    for item in modulename:
                        tmpss = []
                        tmpss.append('{' + "'" + 'connect' + "'" + ' : (')
                        #tmpss.append(self.align * 6 + str(item['connect']) + '\n')
                        if item['connect'] is not None:
                            tmpss.append('\n')
                            for subitem in item['connect']:
                                tmpss.append(self.align * 7 + str(subitem) + ',\n')
                            tmpss.append(self.align * 7 + '),\n')
                        else:
                            tmpss.append(' None ),\n')
                        tmpss.append(self.align * 6 + "'" + 'exec' + "'" + ' : (\n')
                        #tmpss.append(self.align * 6 + str(item['exec']) + '\n')
                        for subitem in item['exec']:
                            tmpss.append(self.align * 7 + str(subitem) + ',\n')
                        tmpss.append(self.align * 5 + ')},')
                        graph.append(''.join(tmpss))
                    graph.append(']\n')
            # append all JModules
            for item in mjmodule:
                ss.append(item)
            # append sys JModule
            ss.append(''.join(sjmodule))
            # append graph
            ss.append(''.join(graph))
            ss.append(self.align * 2 + '}\n')
        return ''.join(ss)


class JModulePySchema(JPySchema):
    def __init__(self, jmodule_name=None, alignOffset=3):
        self.jmodulename = jmodule_name
        self.align = ' ' * alignOffset

    def Schema_ImportJModulePackage(self, standardpktList=[], extrapktList=[]):
        ss = []
        for item in standardpktList:
            ss.append('import ' + item + '\n')
        for item in extrapktList:
            ss.append('from ' + item + ' import *\n')
        return ''.join(ss)

    def Schema_JModuleDefStart(self):
        return "'" + self.jmodulename + "'" + ':' + 'jmodule({\n'

    def Schema_JModuleDefEnd(self, coreid=None):
        return self.align + '}, id=' + str(coreid) + ')\n' if coreid is not None else self.align + '})'

    def Schema_JProcessDefStart(self, procname=None):
        return self.align * 2 + "'" + procname + "'" + ':' + 'jprocess({\n'

    def Schema_JProcessInit(self, jmname=None):
        return self.align * 2 + "'init':'" + jmname +"_initialize()',\n"

    def Schema_JProcessSend(self, jmname=None):
        return self.align * 2 + "'send':'" + jmname +"_HandleAsdIdRequest()',\n"

    def Schema_JProcessPoll(self, jmname=None):
        return self.align * 2 + "'poll':'" + jmname +"_ServiceNotifications()',\n"

    def Schema_JProcessDefEnd(self):
        return self.align * 2 + '}),\n'

    def Schema_PortIn(self, portname=None, portobj=None, portEmpty=False):
        ss = []
        ss.append(self.align * 3 + "'" + portname + "'" + ':')
        if portEmpty == False:
            ss.append('jport_in([')
            ss.append(str(portobj.data['num_chan']) + ', ')
            ss.append(str(portobj.data['frame_size']) + ', ')
            ss.append(str(portobj.data['sample_rate']) + ', ')
            tmpstr = "'" + str(portobj.data['port']) + "'"
            tmpstr = tmpstr.replace('/', '_').replace(' ', '_')
            ss.append(tmpstr)
            ss.append(']),\n')
        else:
            ss.append('jport_in(),\n')
        return  ''.join(ss)

    def Schema_PortOut(self, portname=None, portobj=None, portEmpty=False):
        ss = []
        ss.append(self.align * 3 + "'" + portname + "'" + ':')
        if portEmpty == False:
            ss.append('jport_out([')
            ss.append(str(portobj.data['num_chan']) + ', ')
            ss.append(str(portobj.data['frame_size']) + ', ')
            ss.append(str(portobj.data['sample_rate']) + ', ')
            tmpstr = "'" + str(portobj.data['port']) + "'"
            tmpstr = tmpstr.replace('/', '_').replace(' ', '_')
            ss.append(tmpstr)
            ss.append(']),\n')
        else:
            ss.append('jport_out(),\n')
        return  ''.join(ss)

    def Schema_Callrate(self, callrateName, callrateValue):
        return self.align * 3 + "'" + callrateName + "'" + ':' + callrateValue + ',\n'

    def Schema_Process(self, processName, processFuncName):
        return self.align * 3 + "'" + processName + "'" + ':' + "'" + processFuncName + "'" + ',\n'

    def Schema_JModuleFunc(self, funcPrefix=None, funcName=None):
        return self.align * 2 + "'" + funcPrefix + "'" + ':' + "'" + funcName + "'" + ',\n'

    def Schema_Rateinsec(self, rateinsec, ratevalue):
        return self.align * 3 + "'" + rateinsec + "'" + ':' + ratevalue + ',\n'

    def Schema_Baserate(self, baserate, TrueOrFalse):
        br = self.align * 3 + "'" + baserate + "'" + ':'
        brtf = 'True,\n' if TrueOrFalse == True else 'False,\n'
        return br + brtf


class JModelFLPySchema(JPySchema):
    def __init__(self, model_name=None, alignOffset=2):
        self.modelname = model_name
        self.jpconv = JFLPyConv()
        self.align = ' ' * alignOffset

    def Schema_ImportJConfigPackage(self, standardpktList=[], extrapktList=[]):
        ss = []
        for item in standardpktList:
            ss.append('import ' + item + '\n')
        for item in extrapktList:
            ss.append('from ' + item + ' import *\n')
        return ''.join(ss)

    def Schema_BamproxyDeclare(self, bamType, bamID, topenable=False):
        ss = []
        ss.append('class ' + self.jpconv.classname_bamproxy(bamType) + '(jobj):\n')
        ss.append(self.align + 'def default(self):\n')
        #ss.append(self.align * 2 + 'self.type = ' + "'" + bamType.capitalize() + "'" + '\n')
        ss.append(self.align * 2 + 'self.type = ' + "'" + bamType + "'" + '\n')
        ss.append(self.align * 2 + 'self.data = {\n')
        ss.append(self.align * 3 + "'" + 'RTC' + "'" + ':' + str(bamID[0]) + ',\n')
        if topenable:
            ss.append(self.align * 3 + "'" + 'TOP' + "'" + ':' + str(bamID[1]) + '\n')
        ss.append(self.align * 2 + '}\n')
        return ''.join(ss)

    def Schema_FeatureDeclare(self, feaType, bamD, rtc_under_feature, top_under_feature, topenable=False):
        ss = []
        ss.append('class ' + self.jpconv.classname_feature(feaType) + '(jobj):\n')
        ss.append(self.align + 'def default(self):\n')
        #ss.append(self.align * 2 + 'self.type = ' + "'" + feaType.capitalize() + "'" + ',\n')
        ss.append(self.align * 2 + 'self.type = ' + "'" + feaType + "'" + ',\n')
        ss.append(self.align * 2 + 'self.data = {\n')
        # Add all Bamproxy name and Type to current Feature
        for bamN, bamT in bamD.items():
            ss.append(self.align * 3 + "'" + bamN + "'" + ':' + self.jpconv.classname_bamproxy(bamT) + '(),\n')
        # Add RTC and TOP to current Feature
        if rtc_under_feature:
            ss.append(self.align * 3 + "'" + 'RTC' + "'" + ':' + str(rtc_under_feature) + ',\n')
        if top_under_feature and topenable:
            ss.append(self.align * 3 + "'" + 'TOP' + "'" + ':' + str(top_under_feature) + ',\n')
        ss.append(self.align * 2 + '}\n')
        return ''.join(ss)

    def Schema_AppDataDeclare(self, appdata_name, feaDict, feaRemoved):
        ss = []
        ss.append(appdata_name + ' = {\n')
        for feaName, feaTypeL in feaDict.items():
            if feaTypeL[0] not in feaRemoved:
                ss.append(self.align + "'" + feaName + "'" + ' : ' + self.jpconv.classname_feature(feaTypeL[0]) + '(),\n')
        ss.append('}\n')
        return ''.join(ss)



class JSconsPySchema(JPySchema):
    JCONFIG_DONTEDIT_START = 'Do not Edit the below'
    JCONFIG_DONTEDIT_END = 'Do not Edit the above'
    JCONFIG_DONTEDIT_START_STR1 = '###/\/\/\/\### Do not Edit the below ###/\/\/\/\###'
    JCONFIG_DONTEDIT_START_STR2 = '###################################################'
    JCONFIG_DONTEDIT_END_STR1 = '###################################################'
    JCONFIG_DONTEDIT_END_STR2 = '###/\/\/\/\### Do not Edit the above ###/\/\/\/\###'
    JCONFIG_QUALCOMM_TEMPLATE = '''from JTest import *

###/\/\/\/\### Do not Edit the below ###/\/\/\/\###
###################################################
project_root='xxxx_project_root'
project_name='xxxx_project_name'
model_root='xxxx_model_root'
model_name = 'xxxx_model_name'
model_cfg = xxxx_JConfig_app
model_x86 = ['x86', 'SIL', 'sim', 'xxx_x86_name_xxx']
model_plt = ['hexagon', 'PIL', 'sim', 'xxx_plt_name_xxx']
###################################################
###/\/\/\/\### Do not Edit the above ###/\/\/\/\###
model_testname = model_name + 'TestCase'
model_testcase = 'test_default'
tolerance=0.005

app_param = {
    'model_name': model_name
}

buildcfg_x86 = JConfig_Add(model_cfg, name=model_x86[3], info=model_x86[0] + ' ' + model_x86[1])
JConfig_Platform(buildcfg_x86, model_x86[0], model_x86[1], model_x86[2])
JConfig_SetGenerator(buildcfg_x86, app_config_root='./', arg={'param':app_param})
JConfig_TalariaDefConfig(buildcfg_x86, project_root, project_name, model_root, model_name)
JConfig_TalariaTest(buildcfg_x86, model_testname, model_testcase, tolerance=tolerance)


buildcfg_snapdragon = JConfig_Add(model_cfg, name=model_plt[3], info=model_plt[0] + ' ' + model_plt[1])
JConfig_Platform(buildcfg_snapdragon, model_plt[0], model_plt[1], model_plt[2])
JConfig_SetGenerator(buildcfg_snapdragon, app_config_root='./', arg={'param':app_param})
JConfig_TalariaDefConfig(buildcfg_snapdragon, project_root, project_name, model_root, model_name)
JConfig_TalariaTest(buildcfg_snapdragon, model_testname, model_testcase, tolerance=tolerance)
JConfig_XtraBuildFlags(buildcfg_snapdragon, model_plt[0], xcpp_flags=['-D__V_DYNAMIC__','-fPIC'], xlink_flags=None)
JConfig_AutogenConfig(buildcfg_snapdragon, './JDefaultConfig.json')
'''
    JCONFIGAPP_QUALCOMM_TEMPLATE = '''from JFrameworkStru import *


class app(jobj):
    def default(self, param=[{'model_name':'xxx_modelname'}]):
        param=param[0]
        model_name = param['model_name']
        self.type = model_name
'''
    def __init__(self):
        pass

    def Schema_Sconstruct(self, app_root=None, filename=None):
        ss = []
        ss.append('app_root=' + "'" + app_root + "'" + '\n')
        ss.append('exec(compile(source=open(app_root + ' + "'" + '/Tools/JConfig/' + filename + "'" + ').read(), filename=' + "'" + filename + "'" + ', mode=' + "'" + 'exec' + "'" + '))\n')
        return ''.join(ss)

    def Schema_JConfig(self, dsp_arch='hexagon', platform='8155', sim_type='PIL', sim_mode='onTarget', cfgDict=None):
        ss = ''
        if dsp_arch == 'hexagon':
            ss = JSconsPySchema.JCONFIG_QUALCOMM_TEMPLATE.replace('xxxx_project_root', cfgDict['prj_root']).replace('xxxx_project_name', cfgDict['prj_name'])
            ss = ss.replace('xxxx_model_root', cfgDict['mdl_root']).replace('xxxx_model_name', cfgDict['mdl_name'])
            config_tpl = 'JConfig_app' if cfgDict['num_dsp'] == 1 else 'JConfig_dual_app' if cfgDict['num_dsp'] == 2 else 'JConfig_quad_app'
            ss = ss.replace('xxxx_JConfig_app', config_tpl).replace('xxx_x86_name_xxx', cfgDict['x86_name']).replace('xxx_plt_name_xxx', cfgDict['plt_name'])
        return ss

    def Schema_JConfigUpdate(self, txt_lines=None, num_dsp=None):
        ss = []
        tmp = 'JConfig_app' if num_dsp == 1 else 'JConfig_dual_app' if num_dsp == 2 else 'JConfig_quad_app'
        new_modelcfg = 'model_cfg = ' + tmp + '\n'
        loop_state = 0
        for ll in txt_lines:
            if JSconsPySchema.JCONFIG_DONTEDIT_START in ll:
                loop_state = 1
            elif JSconsPySchema.JCONFIG_DONTEDIT_END in ll:
                loop_state = 0
            if loop_state == 0:
                ss.append(ll)
            else:
                if 'model_cfg' in ll:
                    ss.append(new_modelcfg)
                else:
                    ss.append(ll)
        return ''.join(ss)

    def Schema_JConfigApp(self, model_name=None, num_dsp=1, iportDict=None, oportDict=None):
        ss = []
        ss.append(JSconsPySchema.JCONFIGAPP_QUALCOMM_TEMPLATE.replace('xxx_modelname', model_name))
        ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
        ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
        if isinstance(iportDict, list):
            cnt = 1
            for subDict in iportDict:
                ss.append('        channel_in' + str(cnt) + ' = ' + str(subDict['channel_in']) + '\n')
                ss.append('        frame_size' + str(cnt) + ' = ' + str(subDict['framesize']) + '\n')
                ss.append('        sample_rate' + str(cnt) + ' = ' + str(subDict['samplerate']) + '\n')
                cnt = cnt + 1
            ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
            ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
            ss.append('        self.data = {\n')
            ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
            ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
            cnt = 1
            for subDict in iportDict:
                ss.append('            ' + "'" + 'In' + str(cnt) + "'" + ' : bam_inport([' + "'" + subDict['name'].replace('/', '_').replace(' ', '_') + "'" + ', channel_in' + str(cnt) + ', frame_size' + str(cnt) + ', sample_rate' + str(cnt) + ']),\n')
                cnt = cnt + 1
        else:
            ss.append('        channel_in = ' + str(iportDict['channel_in']) + '\n')
            ss.append('        frame_size = ' + str(iportDict['framesize']) + '\n')
            ss.append('        sample_rate = ' + str(iportDict['samplerate']) + '\n')
            ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
            ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
            ss.append('        self.data = {\n')
            ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
            ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
            ss.append('            ' + "'" + 'In' + "'" + ' : bam_inport([' + "'" + iportDict['name'].replace('/', '_').replace(' ', '_') + "'" + ', channel_in, frame_size, sample_rate]),\n')
        if isinstance(oportDict, list):
            cnt = 1
            for subDict in oportDict:
                ss.append('            ' + "'" + 'Out' + str(cnt) + "'" + ': bam_outport([' + "'" + subDict['name'].replace('/', '_').replace(' ', '_') + "'" + ']),\n')
                cnt = cnt + 1
        else:
            ss.append('            ' + "'" + 'Out' + "'" + ': bam_outport([' + "'" + oportDict['name'].replace('/', '_').replace(' ', '_') + "'" + ']),\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
        ss.append('        }\n')
        ss.append('        self.baf = {\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
        if num_dsp == 1:
            ss.append('            ' + "'" + 'Baf' + "'" + ': jframework_singlecore([model_name], id=0)\n')
        elif num_dsp == 2:
            ss.append('            ' + "'" + 'Baf1' + "'" + ': jframework_crosscore([model_name, 1, 1], id=0),\n')
            ss.append('            ' + "'" + 'Baf2' + "'" + ': jframework_crosscore([model_name, 1, 2], id=1),\n')
        else:
            ss.append('            ' + "'" + 'Baf1' + "'" + ': jframework_crosscore([model_name, 1, 1], id=0),\n')
            ss.append('            ' + "'" + 'Baf2' + "'" + ': jframework_crosscore([model_name, 1, 2], id=1),\n')
            ss.append('            ' + "'" + 'Baf3' + "'" + ': jframework_crosscore([model_name, 2, 1], id=2),\n')
            ss.append('            ' + "'" + 'Baf4' + "'" + ': jframework_crosscore([model_name, 2, 2], id=3),\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
        ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
        ss.append('        }\n')
        ss.append('        self.GenConfigFlat()\n')
        ss.append('        self.GenConfigMatlabStruct()\n')
        ss.append('\n\n')
        ss.append('app_config = app()\n')

        return ''.join(ss)

    def Schema_JConfigAppUpdate(self, txt_lines=None, num_dsp=1, iportDict=None, oportDict=None):
        ss = []
        port_ss = []
        data_ss = []
        baf_ss = []
        port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
        port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
        if isinstance(iportDict, list):
            cnt = 1
            for subDict in iportDict:
                port_ss.append('        channel_in' + str(cnt) + ' = ' + str(subDict['channel_in']) + '\n')
                port_ss.append('        frame_size' + str(cnt) + ' = ' + str(subDict['framesize']) + '\n')
                port_ss.append('        sample_rate' + str(cnt) + ' = ' + str(subDict['samplerate']) + '\n')
                cnt = cnt + 1
            port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
            port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
            data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
            data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
            cnt = 1
            for subDict in iportDict:
                data_ss.append('            ' + "'" + 'In' + str(cnt) + "'" + ' : bam_inport([' + "'" + subDict['name'].replace('/', '_').replace(' ', '_') + "'" + ', channel_in' + str(cnt) + ', frame_size' + str(cnt) + ', sample_rate' + str(cnt) + ']),\n')
                cnt = cnt + 1
        else:
            port_ss.append('        channel_in = ' + str(iportDict['channel_in']) + '\n')
            port_ss.append('        frame_size = ' + str(iportDict['framesize']) + '\n')
            port_ss.append('        sample_rate = ' + str(iportDict['samplerate']) + '\n')
            port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
            port_ss.append('        ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
            data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
            data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
            data_ss.append('            ' + "'" + 'In' + "'" + ' : bam_inport([' + "'" + iportDict['name'].replace('/', '_').replace(' ', '_') + "'" + ', channel_in, frame_size, sample_rate]),\n')
        if isinstance(oportDict, list):
            cnt = 1
            for subDict in oportDict:
                data_ss.append('            ' + "'" + 'Out' + str(cnt) + "'" + ': bam_outport([' + "'" + subDict['name'].replace('/', '_').replace(' ', '_') + "'" + ']),\n')
                cnt = cnt + 1
        else:
            data_ss.append('            ' + "'" + 'Out' + "'" + ': bam_outport([' + "'" + oportDict['name'].replace('/', '_').replace(' ', '_') + "'" + ']),\n')
        data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
        data_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
        # For self.baf section
        baf_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR1 + '\n')
        baf_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_START_STR2 + '\n')
        if num_dsp == 1:
            baf_ss.append('            ' + "'" + 'Baf' + "'" + ': jframework_singlecore([model_name], id=0)\n')
        elif num_dsp == 2:
            baf_ss.append('            ' + "'" + 'Baf1' + "'" + ': jframework_crosscore([model_name, 1, 1], id=0),\n')
            baf_ss.append('            ' + "'" + 'Baf2' + "'" + ': jframework_crosscore([model_name, 1, 2], id=1),\n')
        else:
            baf_ss.append('            ' + "'" + 'Baf1' + "'" + ': jframework_crosscore([model_name, 1, 1], id=0),\n')
            baf_ss.append('            ' + "'" + 'Baf2' + "'" + ': jframework_crosscore([model_name, 1, 2], id=1),\n')
            baf_ss.append('            ' + "'" + 'Baf3' + "'" + ': jframework_crosscore([model_name, 2, 1], id=2),\n')
            baf_ss.append('            ' + "'" + 'Baf4' + "'" + ': jframework_crosscore([model_name, 2, 2], id=3),\n')
        baf_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR1 + '\n')
        baf_ss.append('            ' + JSconsPySchema.JCONFIG_DONTEDIT_END_STR2 + '\n')
        ccnt = 0
        while ccnt < len(txt_lines):
            if JSconsPySchema.JCONFIG_DONTEDIT_START in txt_lines[ccnt]:
                ccnt += 1
                flag = 0
                while JSconsPySchema.JCONFIG_DONTEDIT_END not in txt_lines[ccnt]:
                    if 'bam_inport' in txt_lines[ccnt] or 'bam_outport' in txt_lines[ccnt]:
                        flag = 1
                    elif 'jframework_singlecore' in txt_lines[ccnt] or 'jframework_crosscore' in txt_lines[ccnt]:
                        flag = 2
                    ccnt += 1
                ccnt += 1
                if flag == 0:
                    ss.append(''.join(port_ss))
                elif flag == 1:
                    ss.append(''.join(data_ss))
                else:
                    ss.append(''.join(baf_ss))
            else:
                ss.append(txt_lines[ccnt])
                ccnt += 1

        return ''.join(ss)


