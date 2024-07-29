#####################################################################################################################
# JAutogenProbe_ui.py provides command line interface to call JAutogenProbe functions.
# Profile:
#         Format:  JAutogenProbe_ui profile root_folder_of_C_code path_to_callrate_xml model_name feature_list_in_json
#         Example: JAutogenProbe_ui profile ./melville/Model_1_1_ert_shrlib_rtw ./melville/Model_SyncApi.xml Model_1_1 fealist.json
#
#####################################################################################################################
import os
import sys
import json

from JAutogenProbe import *





def runProfiling(c_code_path=None, callrate_file=None, model_name=None, feaDict=None, pltDict=None, pltbuild=None, cflag=0, dsp_arch=JAutogenProbe.DSPGUL):
    ja = JAutogenProbe(dsp_arch, c_code_path)
    ja.PrepareForProfiling(model_name, callrate_file)
    ja.AddProfilingInitNStep(True)
    if bool(feaDict):
        for k, v in feaDict.items():
            if JAutogenProbe.PLATFORM_ATTACHKEYWORKD not in k:
                ja.AddProfilingFeature(k, k[:-2], v)
    if bool(pltDict):
        ja.AddProfilingPlatform(pltDict, pltbuild, cflag)
    ja.EvokeGoldenProfiling(JAutogenProbe.PROFILE_RUNTIME)


def runFunctionMapping(c_cod_path=None, target_path=None, model_name=None, maptype=JAutogenProbe.PRAGMA_GROUP_STEP, dsp_arch=JAutogenProbe.DSPGUL):
    ja = JAutogenProbe(dsp_arch, c_cod_path)
    ja.EvokeGeneralPragma(model_name, target_path, int(maptype))

n = len(sys.argv)
if n > 2:
    if sys.argv[1] == 'profile':
        # MIPS Profiling for GUL DSP only currently...
        if os.path.exists(sys.argv[2]) and os.path.exists(sys.argv[3]) and sys.argv[4] != None:
            if n > 5:
                if os.path.exists(sys.argv[5]):
                    try:
                        with open(sys.argv[5], 'r') as filehandler:
                            cfg = json.load(filehandler)
                        # Check if DSP or Processor is specified
                        if 'Architecture' in cfg:
                            if cfg['Architecture'] == 'DSPGUL':
                                arch = JAutogenProbe.DSPGUL
                            elif cfg['Architecture'] == 'DSPHEXAGON':
                                arch = JAutogenProbe.DSPHEXAGON
                            elif cfg['Architecture'] == 'ARMANDROID':
                                arch =JAutogenProbe.ARMANDROID
                            else:
                                print('Unsupported Architecture: ' + cfg['Architecture'])
                        else:
                            arch = JAutogenProbe.DSPGUL
                        if 'Profile' in cfg and sys.argv[4] in cfg['Profile']:
                            exst = True
                            platformDict = {}
                            pltbuild = None
                            # For Model, Check if feature filename specified in JSON exists
                            for name, func in cfg['Profile'][sys.argv[4]].items():
                                if not os.path.exists(os.path.join(sys.argv[2], name)):
                                    # Check if platform profiling attached to the model
                                    if JAutogenProbe.PLATFORM_ATTACHKEYWORKD in name:
                                        for pname, fname in func.items():
                                            ppth = os.path.join(name[19:], pname)
                                            if os.path.exists(ppth):
                                                platformDict[ppth] = fname
                                            else:
                                                print('ERROR: The file ' + ppth + ' does not exist.')
                                                exst = False
                                                break
                                    else:
                                        print('ERROR: The file name for feature cannot be found: ' + name)
                                        exst = False
                                        break
                            if JAutogenProbe.PLATFORM_BUILDKEYWORKD in cfg['Profile']:
                                for k, v in cfg['Profile'][JAutogenProbe.PLATFORM_BUILDKEYWORKD].items():
                                    if sys.argv[4] in k:
                                        pltbuild = v
                                        break
                            if exst == True:
                                runProfiling(sys.argv[2], sys.argv[3], sys.argv[4], cfg['Profile'][sys.argv[4]], platformDict, pltbuild, int(sys.argv[6]), arch)
                        else:
                            print('WARNING: The Profiling Configuration cannot be found in JSON.')
                            runProfiling(sys.argv[2], sys.argv[3], sys.argv[4], None, None, None)
                    except json.decoder.JSONDecodeError:
                        print('ERROR: Decoding ERROR when parsing file ' + sys.argv[5] + '.')
                else:
                    print('ERROR: JSON file Lost for feature profiling')
            else:
                runProfiling(sys.argv[2], sys.argv[3], sys.argv[4], None, None, None)
        else:
            print('ERROR: File Path does not exist for JAutogenProbe Profiling.')
            print(sys.argv[2])
            print(sys.argv[3])
            print(sys.argv[4])
    elif sys.argv[1] == 'redirect':
        # Audio Buffer Redirection
        pass
    elif sys.argv[1] == 'mapping':
        # Code/Data section Mapping
        if sys.argv[2] == 'function':
            if os.path.exists(sys.argv[3]) and os.path.exists(sys.argv[4]) and sys.argv[5] != None and sys.argv[6] != None:
                runFunctionMapping(sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])
            else:
                print('ERROR: File Path does not exist, or model name is incorect.')
        else:
            print('ERROR: ' + sys.argv[2] + 'Not supported.')
        pass
    else:
        print('ERROR: Wrong Arguments.')
else:
    print('ERROR: No Argument: JAutogenProbe.py')




#################################################################################################
# Unit Test for Profiling command line
#################################################################################################
# python JAutogenProbe_ui.py profile "./testcases/melville/Model_1_1_ert_shrlib_rtw" "./testcases/Melville/Model_SyncApi.xml" Model_1_1 "./JAutogenProbeConfig.json"


# python JAutogenProbe_ui.py profile "./testcases/Vega/Model_1_1_ert_shrlib_rtw" "./testcases/Vega/Vega_SyncApi.xml" Vega_1_1


