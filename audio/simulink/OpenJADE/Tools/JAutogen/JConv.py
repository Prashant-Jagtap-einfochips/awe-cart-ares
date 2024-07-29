import sys
import os



class JConv:
    pass


class JFLConv(JConv):
    BPSETUP = 'BPSetup'
    BAMPROXY = 'BamProxy'
    FEATURE = 'Feature'
    FEATURELIBSHADOW = 'Shadow'
    FEATURESHADOW = 'FeatureShadow'
    BAMPROXYSHADOW = 'BamProxyShadow'
    def filename_SetupAPI(self, featurename=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return featurename + JFLConv.BPSETUP + filetype
        else:
            return ''

    def filename_Bamproxy(self, bamproxyname=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return bamproxyname.capitalize() + JFLConv.BAMPROXY + filetype
        else:
            return ''

    def filename_BamproxyShadow(self, bamproxyname=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return bamproxyname + JFLConv.BAMPROXYSHADOW + filetype
        else:
            return '' 

    def filename_Feature(self, featurename=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return featurename + JFLConv.FEATURE + filetype
        else:
            return ''

    def filename_FeatureShadow(self, featurename=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return featurename + JFLConv.FEATURESHADOW + filetype
        else:
            return '' 

    def filename_FeatureLibShadow(self, featurelibname=None, filetype=None):
        if filetype == '.h' or filetype == '.c':
            return featurelibname + JFLConv.FEATURELIBSHADOW + filetype
        else:
            return ''

    def funcname_SetupAPI_setup(self, featurename=None, bamproxyname=None):
        if (featurename is not None) and (bamproxyname is not None):
            return featurename + 'Setup' if featurename == bamproxyname else featurename + bamproxyname + 'Setup'
        else:
            return ''

    def funcname_FeatureLib_FeatureGetPtr(self, featurename=None):
        if featurename is not None:
            return 'FL_' + featurename[:5] + '_GetFeatPtr'
        else:
            return ''

    def funcname_FeatureLib_Initialize(self, funcname=None):
        if funcname is not None:
            return funcname + 'Shadow'
        else:
            return ''

    def funcname_FeatureLib_Construct(self, funcname=None):
        if funcname is not None:
            return funcname + 'Shadow'
        else:
            return ''

    def funcname_Feature_FeatureConstructor(self, featurename=None):
        if featurename is not None:
            return featurename + 'Feature_Construct'
        else:
            return ''

    def funcname_Feature_Initialize(self, featurename=None):
        if featurename is not None:
            return featurename + 'FeatureInitialize'
        else:
            return ''

    def funcname_Feature_InitializeShadow(self, featurename=None):
        if featurename is not None:
            return featurename + 'FeatureInitializeShadow'
        else:
            return ''

    def funcname_BamProxy_Construct(self, bamname=None):
        if bamname is not None:
            return bamname + 'BamProxy_Construct'
        else:
            return ''

    def funcname_Bamproxy_VirtualMethod(self, bamname=None, suffix=None):
        if (bamname is not None) and (suffix is not None):
            return bamname + 'BamProxy' + suffix
        else:
            return ''

    def funcname_Bamproxy_InitializeShadow(self, bamname=None):
        if bamname is not None:
            return bamname + 'BamproxyInitializeShadow'
        else:
            return ''

    def funcname_Bamproxy_rtccommand(self, bamname=None, rtcid=None):
        if (bamname is not None) and (rtcid is not None):
            return bamname + '_' + rtcid + '_rtc'
        else:
            return ''

    def funcname_Bamproxy_toprequest(self, bamname=None, topid=None):
        if (bamname is not None) and (topid is not None):
            return bamname + '_' + topid + '_request'
        else:
            return ''

    def strutypename_FeatureType(self, featurename=None):
        if featurename is not None:
            return featurename.upper() + '_FEAT_TYPE'
        else:
            return ''
    
    def strutypename_FeatureDataType(self, featurename=None):
        if featurename is not None:
            return featurename.upper() + '_FEAT_DATA_TYPE'
        else:
            return ''

    def strutypename_BamproxyType(self, bamname=None):
        if bamname is not None:
            return bamname.upper() + '_BAM_PROXY_TYPE'
        else:
            return ''

    def strutypename_BamproxyDataType(self, bamname=None):
        if bamname is not None:
            return bamname.upper() + '_BAM_PROXY_DATA_TYPE'
        else:
            return ''

    def strutypename_BamproxyVtbl(self, bamname=None):
        if bamname is not None:
            return bamname.upper() + '_FEAT_VTBL_TYPE'
        else:
            return ''
    
    def varitypename_FeatureType(self, featurename=None):
        if featurename is not None:
            return featurename.lower() + 'Feature'
        else:
            return ''

    def varitypename_FeatureDataType(self, featurename=None):
        if featurename is not None:
            return featurename.lower() + 'FeatureData'
        else:
            return ''    

    def macroname_NumofFeature(self, featurename=None):
        if featurename is not None:
            return 'NUM_' + featurename[:5].upper() + '_INDEXES'
        else:
            return ''

    def enumname_Feature_BamOffset(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            return 'OJ' + featurename[:5].upper() + '_' + bamname.upper() + '_'
        else:
            return ''

    def enumname_Feature_ActionIDOffset(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            return 'OJ' + featurename[:5].upper() + '_' + bamname.upper() + '_ACTIONID_'
        else:
            return ''

    def enumname_Bamproxy_Action(self, bamname=None):
        if (bamname is not None):
            return bamname.upper() + '_BP_ACTION'
        else:
            return ''

    def defname_Feature_maxActionId(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            return 'OJ' + featurename[:5].upper() + '_' + bamname.upper() + '_MAX_ACTIONID'
        else:
            return ''

    def defname_Feature_BamOffset(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            return 'OJ' + featurename[:5].upper() + '_' + bamname.upper()
        else:
            return ''

    def defname_Feature_RtcOffset(self, featuretypename=None, bamname=None, rtc=None):
        if (featuretypename is not None):
            if bamname is None:
                return 'OJ' + featuretypename[:5].upper() + '_RTC_' + rtc.upper()
            else:
                return 'OJ' + featuretypename[:5].upper() + '_RTC_' + bamname.upper() + '_' + rtc.upper() 
        else:
            return ''

    def defname_Feature_TopWriteOffset(self, featurename=None, bamname=None, top=None):
        if (featurename is not None):
            if bamname is None:
                return 'OJ' + featurename[:5].upper() + '_TOPWRITE_' + top.upper()
            else:
                return 'OJ' + featurename[:5].upper() + '_TOPWRITE_' + bamname.upper() + '_' + top.upper() 
        else:
            return ''

    def defname_Feature_TopReadOffset(self, featurename=None, bamname=None, top=None):
        if (featurename is not None):
            if bamname is None:
                return 'OJ' + featurename[:5].upper() + '_TOPREAD_' + top.upper()
            else:
                return 'OJ' + featurename[:5].upper() + '_TOPREAD_' + bamname.upper() + '_' + top.upper() 
        else:
            return ''

    def defname_Feature_RtcActionId(self, featurename=None, bamname=None, rtc=None):
        if (featurename is not None):
            if bamname is None:
                return 'RTCID_' + featurename + rtc.upper()
            else:
                return 'RTCID_' + featurename + bamname + rtc.upper() 
        else:
            return ''        

    def defname_Feature_TopWriteActionId(self, featurename=None, bamname=None, top=None):
        if (featurename is not None):
            if bamname is None:
                return 'TOP_WRITE_' + featurename + top.upper()
            else:
                return 'TOP_WRITE_' + featurename + bamname + top.upper() 
        else:
            return ''    

    def defname_Feature_TopReadActionId(self, featurename=None, bamname=None, top=None):
        if (featurename is not None):
            if bamname is None:
                return 'TOP_READ_' + featurename + top.upper()
            else:
                return 'TOP_READ_' + featurename + bamname + top.upper() 
        else:
            return ''    

    def defname_Bamproxy_RtcAction(self, rtc=None, bamname=None):
        if (bamname is not None):
            return rtc.upper() + '_' + bamname.upper()
        else:
            return ''

    def defname_Bamproxy_TopAction(self, top=None, bamname=None, topflag=None):
        if (bamname is not None):
            if topflag == 'w':
                ss = 'TOP_WRITE_' + top.upper() + '_' + bamname.upper()
            else:
                ss = 'TOP_READ_' + top.upper() + '_' + bamname.upper()
            return ss
        else:
            return ''

    def defname_Bamproxy_numAction(self, bamname=None):
        if (bamname is not None):
            return 'NUM_' + bamname.upper() + '_BP_ACTIONS'
        else:
            return ''

    def variname_SetupAPI_TargetBamId(self, featurename=None, bamproxyname=None):
        if (featurename is not None) and (bamproxyname is not None):
            return featurename[:5] + 'BamID' if featurename == bamproxyname else featurename[:5] + bamproxyname + 'BamID'
        else:
            return ''

    def variname_Bamproxy_BamType(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            prefix = bamname if featurename == bamname else featurename[:5] + bamname
            return prefix + 'Cmd'
        else:
            return ''

    def variname_Bamproxy_BamDataType(self, featurename=None, bamname=None):
        if (featurename is not None) and (bamname is not None):
            prefix = bamname if featurename == bamname else featurename[:5] + bamname
            return prefix + 'CmdData'
        else:
            return ''

    def variname_Bamproxy_vtbl(self, bamname=None):
        if (bamname is not None):
            return bamname + '_vtbl'
        else:
            return ''

    def variname_Feature_vtbl(self, featurename=None):
        if (featurename is not None):
            return featurename + 'feat_vtbl'
        else:
            return ''



class JFLPyConv(JConv):
    BAMPROXY_PREFIX = 'bam_'
    FEATURE_PREFIX = 'feature_'
    def classname_bamproxy(self, bamtype=None):
        if (bamtype is not None):
            return JFLPyConv.BAMPROXY_PREFIX + bamtype.lower()
        else:
            return ''

    def classname_feature(self, featuretype=None):
        if (featuretype is not None):
            return JFLPyConv.FEATURE_PREFIX + featuretype.lower()
        else:
            return ''