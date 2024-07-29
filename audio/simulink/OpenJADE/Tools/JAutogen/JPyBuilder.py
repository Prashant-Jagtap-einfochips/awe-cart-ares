import os
import sys

from JPySchema import *
from JXmlTrans import *
from JHtmlSchema import *
from JSchemaConfig import *
from JLogPrint import *




class JControlPyBuilder():
    counter = 0
    shadowfile = 'JConfigAppControl.py'
    appdata_name = 'AppData'
    topic = 'FeatureLib Hierarchy'
    subtopic = 'Lists all Features, Bamproxys and RTC or TOP IDs'
    def __init__(self, genpath=None, generateflag = True, modelname=None, topenable=False, genConfigFile=None):
        self.Dict = {}
        self.topidList = []
        self.rtcidList = []
        self.ltopidList = []
        self.lrtcidList = []
        self.path = genpath
        self.genflag = generateflag
        self.topenable = topenable
        self.jcfgPkgList = ['JTest', 'JFrameworkStru']
        self.modelname = modelname if modelname is not None else 'Model'
        # Parse Schema Configuration from File
        self.schemaConfig = JFLSchemaConfig(genConfigFile)

    def GetXMLFile(self, modelxml=None, rtcxml=None, topxml=None):
        self.modelxml = modelxml
        self.rtcxml = rtcxml
        self.topxml = topxml
        return True if os.path.exists(self.modelxml) and os.path.exists(self.rtcxml) and os.path.exists(self.topxml) else False

    def GenerateCode(self, flobj):
        try:
            modeltree = ElementTree.parse(self.modelxml)
            modelroot = modeltree.getroot()
            rtctree = ElementTree.parse(self.rtcxml)
            toptree = ElementTree.parse(self.topxml)
            postTb = {}
            # Bamproxy Type, Format: {bamType1: [[RTC_ID, ...], [TOP_ID, ...]], bamType2: [[RTC_ID, ...], [TOP_D, ...]]}
            bpType = {}
            # Feature Type, Format: {featureType1: [bamType1, bamType2, ...], featureType2: [bamType1, bamType2, ...]}
            feaType = {}
            # Load All RTC ID and TOP ID from XML files
            for ritem in rtctree.findall('REQ'):
                self.rtcidList.append(ritem.get('id'))
            for titem in toptree.findall('TOP'):
                self.topidList.append(titem.get('id'))
            self.lrtcidList = [x.lower() for x in self.rtcidList]
            self.ltopidList = [x.lower() for x in self.topidList]
            # Recursively parse the whole XML and create the FeatureLib tree in three levels: /Feature/Bamproxy/Rtc&Top ID
            self.ParseFLinPy(modelroot, self.Dict, postTb, feaType, bpType)
            # Update name of Feature Type from Autogen Configuration
            self.UpdateFeatureTypeWithAlias(self.Dict, bpType)
            # Generate the full Hierarchy for the Model
            self.GenerateFLHierarchyLog(self.Dict)
            # Generate All Features and Bamproxies
            self.GenerateFeatureNBamproxy(JControlPyBuilder.appdata_name, bpType, self.Dict)
            # Load All features and Bamproxies to JConfigAJpp.py
            sys.path.append(self.path)
            jappDict = __import__(JControlPyBuilder.shadowfile[:-3], '')
            flobj.control = getattr(jappDict, JControlPyBuilder.appdata_name)
            # Generate Graph in HTML
            self.GenerateGraphInHtml(flobj.control)

        except IOError:
            #print('ERROR: JPyBuilder: XML file does not exist')
            Jlog_print(JAG_ERROR, 'JPyBuilder: XML file does not exist')
        return True

    def ParseFLinPy(self, modelroot, curDict, posttb, feaTypeDict, bamTypeDict):
        rr = modelroot.findall('FeaturePath')
        rootid = modelroot.get('id')
        rootname = modelroot.get('name')
        if rr:
            # rootid is None in case of top level, and None can be used as key in Dictionary
            for subroot in rr:
                self.ParseFLinPy(subroot, curDict, posttb, feaTypeDict, bamTypeDict)
                if bool(posttb):
                    if rootid not in curDict:
                        curDict[rootid] = [None]
                    curDict[rootid].append(posttb.copy())
                    posttb.clear()
            if rootid is not None:
                # Check if parent(Feature) includes RTC&TOP directly
                self.ParseRtcTopwithSubStr(rootid, posttb, rootname, bamTypeDict, False)
                if bool(posttb):
                    if rootid not in curDict:
                        curDict[rootid] = [None, posttb.copy()]
                    else:
                        curDict[rootid].append(posttb.copy())
                    posttb.clear()
                # Update Feature Dictionary
                if rootid in curDict.keys():
                    featype = None
                    btList = []
                    rList = []
                    tList = []
                    # Extract all Bamtype from current feature
                    for item in curDict[rootid][1:]:
                        for v in item.values():
                            if v[3] == True:
                                btList.append(v[2])
                            else:
                                rList = v[0]
                                tList = v[1]
                    # Check if the feature type exists, based on all bamproxy type, and RTC&TOP
                    for k, v in feaTypeDict.items():
                        if (btList == v[0]) and (rList == v[1]) and (tList == v[2]):
                            featype = k
                            break
                    if featype is None:
                        featype = rootname
                        feaTypeDict[featype] = [btList, rList, tList]
                    curDict[rootid][0] = featype
        else:
            # It is buttom Child, and acts as Bamproxy, search its RTC ID and TOP ID
            self.ParseRtcTopwithSubStr(rootid, posttb, rootname, bamTypeDict, True)

    def ParseRtcTopwithSubStr(self, subss, tb, subname, bamTypeDict, checkBamType):
        rtc = []
        top = []
        for rloc, ritem in enumerate(self.lrtcidList, 0):
            if ritem.find(subss.lower()) == 0:
                rtc.append(self.rtcidList[rloc][len(subss):])
                # Clear the item to avoid it is found twice by upper-level FeaturePath
                self.lrtcidList[rloc] = ''
        for tloc, titem in enumerate(self.ltopidList, 0):
            if titem.find(subss.lower()) == 0:
                top.append(self.topidList[tloc][len(subss):])
                # Clear the item to avoid it is found twice by upper-level FeaturePath
                self.ltopidList[tloc] = ''
        if rtc or top:
            # Update BamProxy Dictionary, Format {bamtype: [[RTC_ID, ...], [TOP_ID, ...]]}
            bamtype = None
            if checkBamType:
                for k, v in bamTypeDict.items():
                    if (rtc == v[0]) and (top == v[1]):
                        bamtype = k
                        break
            if bamtype is None:
                if subname in bamTypeDict.keys():
                    bamtype = subname + str(JControlPyBuilder.counter)
                    JControlPyBuilder.counter += 1
                else:
                    bamtype = subname
                bamTypeDict[bamtype] = [rtc, top, checkBamType]
            # Bamproxy format {bamproxy_name: [[RTC_ID, ...], [TOP_ID, ...], Bamproxy Type, True or False]}
            tb[subname] = [rtc, top, bamtype, checkBamType]

    def UpdateFeatureTypeWithAlias(self, modelDict, bpTypeDict):
        for k, v in modelDict.items():
            alias = self.schemaConfig.GetAliasNameofFeature(v[0])
            if alias != None:
                for loc, item in enumerate(v[1:], 0):
                    for sk, sv in item.items():
                        # When Feature Type == Bamproxy Type, Update Bamproxy type too
                        if v[0] == sv[2]:
                            self.Dict[k][loc + 1][sk][2] = alias
                            # No need to Update the bamproxy type in bpTypeDict because the Bamproxy not being 
                            # included in Feature directly is generated. But keep the following line taken out 
                            # as record
                            #bpTypeDict[alias] = bpTypeDict.pop(v[0])
                self.Dict[k][0] = alias

    def GenerateFLHierarchyLog(self, modelDict):
        filename = self.path + '/' + self.modelname + '_Hierarchy.log'
        with open(filename, 'w') as filehandle:
            filehandle.write('Model Hierarchy Log File:\n\n')
            for k, v in modelDict.items():
                if k == None:
                    #print('##### None' + str(v))
                    Jlog_print(JAG_WARNING, 'None appears as feature name, ' + str(v))
                filehandle.write('===================================================================================\n')
                filehandle.write('Feature Name: ' + k + '\n')
                filehandle.write('Feature Type: ' + str(v[0]) + '\n')
                # Loop list with start index = 1, because index=0 holdes feature type
                for item in v[1:]:
                    for sk, sv in item.items():
                        filehandle.write('    BamProxy Name: ' + sk + '\n')
                        if sv[3] == False:
                            filehandle.write('    BamProxy Type: ' + sv[2] + ' (The Following RTC & TOP included in Feature Directly.)\n')
                        else:
                            filehandle.write('    BamProxy Type: ' + str(sv[2]) + '\n')
                        filehandle.write('        RTC ID: ' + str(sv[0]) + '\n')
                        filehandle.write('        TOP ID: ' + str(sv[1]) + '\n')
                filehandle.write('===================================================================================\n')

    def GenerateFeatureNBamproxy(self, appdata_name, bamproxyD, featureD):
        filename = self.path + '/' + JControlPyBuilder.shadowfile
        fbSchema = JModelFLPySchema(self.modelname)
        feaTypeUsed = []
        # Used to record which feature should not be added to App data
        feaTypeRemoved = []
        with open(filename, 'w') as filehandle:
            filehandle.write(fbSchema.Schema_ImportJConfigPackage([], self.jcfgPkgList))
            filehandle.write('\n\n')
            # Write All types of Bamproxy
            for bmType, bmID in bamproxyD.items():
                # bmID[2] equals True means it is real BamProxy, not Bamproxy from Feature with RTC or TOP ID.
                if bmID[2]:
                    if (self.topenable == True) or ((self.topenable == False) and bool(bmID[0])):
                        filehandle.write(fbSchema.Schema_BamproxyDeclare(bmType, bmID, self.topenable))
                        filehandle.write('\n\n')
            # Write All types of Feature
            for feaBamL in featureD.values():
                if feaBamL[0] not in feaTypeUsed:
                    bmD = {}
                    rtc_under_feature = []
                    top_under_feature = []
                    for item in feaBamL[1:]:
                        for sk, sv in item.items():
                            if feaBamL[0] == sv[2]:
                                # When Feature Type == Bamproxy Type, it is RTC&TOP directly under Feature
                                rtc_under_feature = sv[0]
                                top_under_feature = sv[1]
                            else:
                                if (self.topenable == True) or (bool(sv[0])):
                                    bmD[sk] = sv[2]
                    feaTypeUsed.append(feaBamL[0])
                    if bool(bmD) or bool(rtc_under_feature):
                        filehandle.write(fbSchema.Schema_FeatureDeclare(feaBamL[0], bmD, rtc_under_feature, top_under_feature, self.topenable))
                        filehandle.write('\n\n')
                    else:
                        feaTypeRemoved.append(feaBamL[0])
            # Write App Data to hold all features
            filehandle.write(fbSchema.Schema_AppDataDeclare(appdata_name, featureD, feaTypeRemoved))

    def GenerateGraphInHtml(self, objdata=None):
        htmls = JFLHtmlSchema()
        filename = self.path + '/' + self.modelname + '_Hierarchy.html'
        with open(filename, 'w') as filehandle:
            filehandle.write(htmls.Schema_startHtml())
            filehandle.write(htmls.Schema_headHtml(JControlPyBuilder.topic))
            filehandle.write(htmls.Schema_startBody(JControlPyBuilder.topic, JControlPyBuilder.subtopic))
            # Recursively write Features, Bamproxys, RTC or TOP IDs to HTML
            self.LoopInHtml(objdata, None, htmls, filehandle)
            filehandle.write(htmls.Schema_endBody())
            filehandle.write(htmls.Schema_startHtml())

    def LoopInHtml(self, objdata=None, objtype=None, h=None, f=None):
        if objtype == None:
            # Features
            for fean, feat in objdata.items():
                f.write(h.Schema_itemFeatureStart(fean))
                self.LoopInHtml(feat.data, 'Feature', h, f)
                f.write(h.Schema_itemFeatureEnd())
        elif objtype == 'Feature':
            # Bamproxys
            for bamn, bamt in objdata.items():
                if isinstance(bamt, jobj):
                    f.write(h.Schema_itemBamproxyStart(bamn))
                    self.LoopInHtml(bamt.data, 'Bamproxy', h, f)
                    f.write(h.Schema_itemBamproxyEnd())
                elif isinstance(bamt, list):
                    # RTC or TOP IDs
                    f.write(h.Schema_itemRtcTopEx(bamn, bamt))
        elif objtype == 'Bamproxy':
            # RTC or TOP IDs
            for idn, idt in objdata.items():
                if idn == 'RTC' or idn == 'TOP':
                    f.write(h.Schema_itemRtcTop(idn, idt))



class JSconsPyBuilder():
    sconstruct = 'SConstruct.py'
    jconfigpy = 'JConfig.py'
    jconfigapppy = 'JConfigApp.py'
    def __init__(self, genConfigFile=None):
        self.schema = JSconsPySchema()

    def GenerateSconstructPy(self, app_root=None, filename=None, filepath=None):
        scst = os.path.join(filepath, JSconsPyBuilder.sconstruct)
        if os.path.exists(scst):
            os.remove(scst)
        with open(scst, 'w') as filehandle:
            filehandle.write(self.schema.Schema_Sconstruct(app_root, filename))

    def GenerateJConfigPy(self, genpath=None, dsp_arch='hexagon', platform='8155', sim_type='PIL', sim_mode='onTarget', cfgDict=None):
        jcp = os.path.join(genpath, JSconsPyBuilder.jconfigpy)
        if os.path.exists(jcp):
            os.remove(jcp)
        with open(jcp, 'w') as filehandle:
            filehandle.write(self.schema.Schema_JConfig(dsp_arch, platform, sim_type, sim_mode, cfgDict))  

    def UpdateJConfigPy(self, genpath, num_dsp=None):
        jcp = os.path.join(genpath, JSconsPyBuilder.jconfigpy)
        if os.path.exists(jcp):
            with open(jcp, 'r') as filehandle:
                txt_lines = filehandle.readlines()
            txt_new = self.schema.Schema_JConfigUpdate(txt_lines, num_dsp)
            new_jcp = jcp + '.new'
            with open(new_jcp, 'w') as filehandle:
                filehandle.write(txt_new)
            os.remove(jcp)
            os.rename(new_jcp, jcp)
        else:
            Jlog_print(JAG_ERROR, JSconsPyBuilder.jconfigpy + ' does not exist.')

    def GenerateJConfigAppPy(self, genpath=None, model_name=None, num_dsp=1, iportDict=None, oportDict=None):
        jcpa = os.path.join(genpath, JSconsPyBuilder.jconfigapppy)
        if os.path.exists(jcpa):
            os.remove(jcpa)
        with open(jcpa, 'w') as filehandle:
            filehandle.write(self.schema.Schema_JConfigApp(model_name, num_dsp, iportDict, oportDict))

    def UpdateJConfigAppPy(self, genpath=None, num_dsp=1, iportDict=None, oportDict=None):
        jcpa = os.path.join(genpath, JSconsPyBuilder.jconfigapppy)
        if os.path.exists(jcpa):
            with open(jcpa, 'r') as filehandle:
                txt_lines = filehandle.readlines()
            txt_new = self.schema.Schema_JConfigAppUpdate(txt_lines, num_dsp, iportDict, oportDict)
            new_jcpa = jcpa + '.new'
            with open(new_jcpa, 'w') as filehandle:
                filehandle.write(txt_new)
            os.remove(jcpa)
            os.rename(new_jcpa, jcpa)
        else:
            Jlog_print(JAG_ERROR, JSconsPyBuilder.jconfigapppy + ' does not exist.')






