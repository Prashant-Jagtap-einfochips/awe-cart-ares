from cart.JConfig.JTest import *
from cart.JConfig.JFrameworkStru import *

try:
   import matlab.engine
   matlab_engine=True
except ImportError:
   matlab_engine=False


# ==========================================
# Common BAM classes for general feature use
# ==========================================

class BamInportConfig(jobj) :
   def default(self, param=[None, 2, 32, 48000]):
      self.type    = 'ModelRootInportConfig'
      
      self.data = {
         'BufferName'       : param[0],
         'AccessType'       : 'ImportedPointer',
         'ChannelCount'     : param[1],
         'FrameSamples'     : param[2],
         'SampleRateInHertz': param[3]
      }

class BamOutportConfig(jobj) :
   def default(self, param=[None]):
      self.type    = 'ModelRootOutportConfig'
      
      self.data = {
         'BufferName': param[0],
         'AccessType': 'ImportedPointer'
      }

class BamMute(jobj) :
   def default(self, param=[1,1]):
      self.type = 'Mute'
      self.btc = 'MuteConfig'
      
      num_chan  = param[0]
      if len(param) > 1:
         enable = param[1]
      else:
         enable = 0
      self.data = {
         'RTC'    : ['Set'],
         'num_in' : num_chan,
         'num_out': num_chan,
         'InitialMuteState' : enable,
      }

class BamEnable(jobj) :
   def default(self, param=[1,1]):
      self.type = 'Enable'
      self.data = {
         'RTC'    : ['Enable'],
      }

class BamDisable(jobj) :
   def default(self, param=[1,1]):
      self.type = 'Disable'
      self.data = {
         'RTC'    : ['Disable'],
      }

class BamShutdown(jobj) :
   def default(self, param=[1,1]):
      self.type = 'Shutdown'
      self.data = {
         'RTC'    : ['Shutdown'],
      }

class BamParamSet(jobj) :
   def default(self, param=[1,1]):
      self.type = 'ParamSet'
      self.data = {
         'RTC'    : ['Set'],
      }


# ==========================================
#    BAM classes used by only Preamp feature
# ==========================================

class BamVolume(jobj) :
   def default(self, num_chan=1):
      self.type = 'Volume'
      self.btc = 'VolumeConfig'
      self.isClass=True
      ramptime  = 1
      self.data = {
         'RTC'        : ['Set'],
         'num_in'     : num_chan,
         'num_out'    : num_chan,
         'InitialLinearGain' : 1.0,
         'DefaultTable': [[0, -99], [.388, 0], [1, 0]],
      }
      self.control = {
         'Index': 255
      }

class BamTone(jobj) :
   def default(self, param=[2, 200, 20, 20]):
      self.type = 'Tone'
      self.btc = 'ToneConfig'
      self.isClass=True
      num_chan  = param[0]
      cutoff    = param[1]
      cut_max   = param[2]
      boost_max = param[3]
      self.data = {
         'RTC': ['Set'],
         'num_in' : num_chan,
         'num_out': num_chan,
         'DefaultCutHz'   : cutoff,
         'DefaultCutDb'      : cut_max,
         'DefaultBoostHz' : cutoff,
         'DefaultBoostDb'    : boost_max,
      }
      self.control = {
         'Index': 128
      }

class BamMidrange(jobj) :
   def default(self, param=[2, 775, 20, 20, 0.9]):
      self.type = 'Midrange'
      self.btc = 'MidrangeConfig'
      self.isClass=True
      num_chan  = param[0]
      cutoff    = param[1]
      cut_max   = param[2]
      boost_max = param[3]
      q         = param[4]
      self.data = {
         'RTC': ['Set'],
         'num_in'       : num_chan,
         'num_out'      : num_chan,
         'DefaultCutHz'   : cutoff,
         'DefaultCutDb'      : cut_max,
         'DefaultCutq'        : q,
         'DefaultBoostHz' : cutoff,
         'DefaultBoostDb'    : boost_max,
         'DefaultBoostq'      : q,
      }
      self.control = {
         'Index': 128
      }


class BamBalance(jobj) :
   def default(self, num_chan = 2):
      self.type = 'Balance'
      self.btc  = 'BalanceConfig'
      self.isClass=True
      self.data = {
         'RTC': ['Set'],
         'num_in' : num_chan,
         'num_out': num_chan,
         'DefaultMaskChanRampMap' : [1, 2],
      }
      self.control = {
         'Index': 128
      }

class BamFade(jobj) :
   def default(self, num_chan = 2):
      self.type = 'Fade'
      self.btc  = 'FadeConfig'
      
      self.data = {
         'RTC'    : ['Set'],
         'num_in' : num_chan,
         'num_out': num_chan,
         'DefaultChanRampMap': [1, 2],
      }
      self.control = {
         'Index': 128
      }

class BamAudioPilot(jobj) :
   def default(self, num_chan=2):
      self.type = 'AudioPilot'

      self.data = {
         'RTC': [  
            'muteReference',
            'setSpeedIn',
            'setHVACSetting',
            'setDynEQBoostOverRide',
            'setVNCTrebleBoostOverRide',
            'setLoudSignalOverRide',
            'setVNCSignalOverRide',
            'setLFNoiseOverRide',
            'setWBNoiseOverRide',
            'setHFNoiseOverRide',
            'setRatioOverRide',
            'setDyneqOn',
            'setAudioPilotOn',
            'setVNCBassBoostOverRide',
            'getBassBoost',
            'setVNCMidBoostOverRide',
            'getMidBoost',
            'getTrebleBoost',
#            'SelectSet',
            'getAverageAudioPilotBoostLevel',
         ],
         'num_in' : num_chan,
         'num_out': num_chan,
      }
      self.control = {
         'controlstate'    : 1,
         'vehicleSpeed'    : 0,
         'sunRoofPosition' : 0,
         'fanSpeed'        : 0,
         'windowPositionFL': 0,
         'windowPositionFR': 0,
         'windowPositionRL': 0,
         'windowPositionRR': 0,
      }

class BamMedusa(jobj) :
   def default(self):
      self.type = 'Medusa'
      self.data = {
         'RTC':[
            'SurroundLevelControlIndex',
#            'SurroundLevelControlReset',
            'PostUpmixDisable',
            'HoligramDisable',
 #           'SelectSet',
            'HeadrestEqHeadrestHoligramDisable',
            'OverheadHoligramDisable',
            'FadeControl'
            ],
         'num_in' :  2,
         'num_out': 10,
      }
      self.control = {
         'status'        : 0,
         'seat_group'    : 1,
         'sliderposition': 0,
      }


# ==============================================
# BAM classes used by only Announcements feature
# ==============================================

class BamAnnMute(jobj) :
   def default(self, param=[1,1]):
      self.type = 'Mute'
      self.btc = 'MuteConfig'
      
      num_chan  = param[0]
      if len(param) > 1:
         enable = param[1]
      else:
         enable = 0
      self.data = {
         'RTC'    : ['Set', 'RMDLTrigger'],
         'num_in' : num_chan,
         'num_out': num_chan,
         'InitialMuteState' : enable,
      }

class BamRouterSet(jobj) :
   def default(self, param):
      self.type = 'RouterSet'
      num_in = param[0]
      num_out = param[1]
      self.data = {
         'RTC'    : ['Set'],
         'num_in' : num_in,
         'num_out': num_out,
      }
      self.control = {
         'Index': 128
      }

class BamRouterSelect(jobj) :
   def default(self, num_chan = 2):
      self.type = 'RouterSelect'
      self.data = {
         'RTC'    : ['Select'],
         'num_in' : num_chan,
         'num_out': num_chan,
      }
      self.control = {
         'Index': 128
      }

class BamAsdRouter(jobj) :
   def default(self, num_chan = 2):
      self.type = 'AsdRouter'
      self.btc  = 'AsdRouterConfig'
      
      self.data = {
         'RTC': [
                  'MainSelect',
                  'MainGain',
                  'AuxEnable',
                  'AuxSelect',
                  'AuxGain',
         ],
         'num_in' : num_chan,
         'num_out': num_chan,
      }
      self.control = {
         'Index': 128
      }

class BamSteering(jobj):
   def default(self, param):
      self.type = 'Steering'
      self.btc  = 'AnnSteeringConfig'
      
      num_in = param[0]
      num_out = param[1]
      self.data = {
         'RTC'    : ['Set'],
         'num_in' : num_in,
         'num_out': num_out,
      }
      self.control = {
         'RampingRate': [300]*num_out,
         'RampingTime': [100]*num_out,
         'Gain': [65535]*num_out
      }

# ==============================================
# BAM classes used by only InputRouterEQ feature
# ==============================================

class BamMultiSourceEq(jobj):
   def default(self, num_chan = 2):
      self.type = 'MultiSourceEq'
      self.btc  = 'MultiSourceEqConfig'
      
      self.data = {
         'RTC'    : ['SourceSelect'],
         'num_in' : num_chan,
         'num_out': num_chan,
      }
      self.control = {
         'Index': 128
      }

# ==============================================
# BAM classes used by only InLineRMS feature
# ==============================================
class BamRms(jobj):
   def default(self, num_chan = 2):
      self.type = 'Rms'
      self.data = {
         'RTC'    : ['Enable', 'OutputRMS'],
         'num_in' : num_chan,
         'num_out': num_chan,
      }
      self.control = {
         'Index': 128
      }

# =======================
# FEATURE classes section
# =======================

class FeaturePreamp(jobj):
   def default(self, param ):
      self.type    = 'PreAmp'
      self.btc     = 'PreAmpConfig'
      
      num_in = param[0]
      num_out = param[1]
      self.data = {
         'frame_size' : 32,
         'num_in'     : num_in,
         'num_out'    : num_out,

#         'PreAmpSelect' : BamRouterSet([4, 4]),
         'VolumeCfg'  : BamVolume(num_in),
#         'Bass'    : BamTone([num_in, 100, 9, 6]),
#         'Midrange': BamMidrange([num_in, 775, 9, 6, 0.9]),
#         'Treble'  : BamTone([num_in,8000, 9, 6]),
         'Balance_Cfg' : BamBalance(num_in),
         'FadeControl'    : BamFade(num_out),
         'Audiopilot': BamAudioPilot(num_in),
         'MedusaSettings': BamMedusa(),
      #   'RMS'     : BamRms(num_out),
      }

class FeatureArmPreamp(jobj):
   def default(self, param ):
      self.type    = 'ArmPreAmp'
      self.btc     = 'ArmPreAmpConfig'
      
      num_in = param[0]
      num_out = param[1]
      self.data = {
         'frame_size' : 32,
         'num_in'     : num_in,
         'num_out'    : num_out,

#         'PreAmpSelect' : BamRouterSet([4, 4]),
         'VolumeRefCfg'  : BamVolume(num_in),
         'Bass'    : BamTone([num_in, 100, 9, 6]),
         'MidrangeCfg': BamMidrange([num_in, 775, 9, 6, 0.9]),
         'Treble'  : BamTone([num_in,8000, 9, 6]),
      #   'RMS'     : BamRms(num_out),
      }

class FeaturePostProcess(jobj):
   def default(self, num_ch = 2):
      self.type    = 'PostProcess'
      self.btc     = 'PostProcessConfig'
      
      frame_size = 32
      num_in     = num_ch
      num_out    = num_ch
      self.data  = {
         'frame_size': frame_size,
         'num_in'    : num_in,
         'num_out'   : num_out,

         #'PostProcSelect'    : BamRouterSet([6, 6]),
         #'AsdRouter'         : BamAsdRouter(num_out),
         'PostLimiterEq'     : BamShutdown(),
         'LimiterSoftclip'   : BamDisable(),
         'LimiterSoftclipParam': BamParamSet(),
         'MuteCfg'           : BamMute([num_out, 0]),
      }

class FeatureTestRouter(jobj):
   def default(self, num_ch = 2):
      self.type    = 'TestRouter'
      self.btc     = 'TestRouterConfig'
      
      frame_size = 32
      num_in     = num_ch
      num_out    = num_ch
      self.data  = {
         'frame_size': frame_size,
         'num_in'    : num_in,
         'num_out'   : num_out,

         'ASDRouter'         : BamAsdRouter(num_out),
      }

class FeatureAnnouncements(jobj):
   def default(self, param):
      self.type  = 'Announcements'
      self.btc  =  'AnnouncementsConfig'
      
      frame_size = 32
      num_in     = param[0]
      num_out    = param[1]
      self.data  = {
         'frame_size'     : frame_size,
         'num_in'         : num_in,
         'num_out'        : num_out,

         'AnnSelect'                 : BamRouterSet([num_in, 4]),
         'AnnouncementsAnnVolume'    : BamVolume(1),
         'AnnouncementsAnnSteering'  : BamSteering([1, 6]),
         'AnnouncementsAnnRouter'    : BamRouterSet([6, num_out]),
         'AnnouncementsAnnOutputRouter' : BamAsdRouter(num_out, num_out),
         'AnnouncementsAnnMute'      : BamAnnMute([num_out, 0]),
      }

class FeatureInputSelect(jobj):
   def default(self, num_ch = 2):
      self.type  = 'InputRouterEq'
      self.btc  =  'InputRouterEqConfig'
      
      frame_size = 32
      num_in     = num_ch
      num_out    = num_ch
      self.data  = {
         'frame_size'     : frame_size,
         'num_in'         : num_in,
         'num_out'        : num_out,
         'RouterCfg'    : BamRouterSet([num_in, num_in]),
      }


# =====================================
#  FRAMEWORK Class
# =====================================



class jframework_Model_A2(jframework):
   def default(self, param=[32, 48000, 0, 0]):
      self.configMatlabStruct=[]
      self.classInit={}
      self.isFeature=False
      self.isClass=False
      self.type='JFramework'
      self.isJModule=True
      self.crosscore=True
#      self.xml='CART/out/ip2_x64_dll//slx/code/Model_SyncApi.xml'
#      self.xml_pingpong='CART/out/ip2_x64_dll//slx/code/Model_1_1_ert_shrlib_rtw/Model_1_1_PingPongStruct.xml'
      self.talaria_param_version=2
      self.model_name='Model'
      self.socId=1
      self.coreId=1
#      self.partition='Model_1_1'
      self.data = {
         'Model_1_2':jmodule({
            'init':'Model_1_2_initialize()',
            'send':'Model_1_2_HandleAsdIdRequest()',
            'poll':'Model_1_2_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_2_step(0)',
            }),
            'step1':jprocess({
               'in1':jport_in([1, 448, 1500, 'Model_1_2_N03S1_8_D1_2_F12']),
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_2_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 160, 1384, 'Model_1_2_N07S1_3_D1_2_F13']),
               'in2':jport_in([1, 160, 1384, 'Model_1_2_N15S1_6_D1_2_F13']),
               'out1':jport_out([1, 808, 1384, 'Model_1_2_N04S1_2_D1_7_F13']),
               'out2':jport_out([1, 224, 1384, 'Model_1_2_N05S1_2_D1_6_F13']),
               'out3':jport_out([1, 321, 1384, 'Model_1_2_N06S1_2_D1_3_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_2_step(2)',
            }),
            'step3':jprocess({
               'callrate':26,
               'rate_period':0.0014444444444444444,
               'base_task':False,
               'process':'Model_1_2_step(3)',
            }),
            'step4':jprocess({
               'callrate':832,
               'rate_period':0.04622222222222222,
               'base_task':False,
               'process':'Model_1_2_step(4)',
            }),
            'step5':jprocess({
               'callrate':3328,
               'rate_period':0.18488888888888888,
               'base_task':False,
               'process':'Model_1_2_step(5)',
            }),
            'step6':jprocess({
               'callrate':9984,
               'rate_period':0.55466666666666664,
               'base_task':False,
               'process':'Model_1_2_step(6)',
            }),
         }, id=0),
         'Model_1_3':jmodule({
            'init':'Model_1_3_initialize()',
            'send':'Model_1_3_HandleAsdIdRequest()',
            'poll':'Model_1_3_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_3_step(0)',
            }),
            'step1':jprocess({
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_3_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 321, 1384, 'Model_1_3_N06S1_2_D1_3_F13']),
               'in2':jport_in([1, 2, 1384, 'Model_1_3_N14S1_6_D1_3_F13']),
               'out1':jport_out([1, 160, 1384, 'Model_1_3_N07S1_3_D1_2_F13']),
               'out2':jport_out([1, 248, 1384, 'Model_1_3_N08S1_3_D1_6_F13']),
               'out3':jport_out([1, 240, 1384, 'Model_1_3_N09S1_3_D1_5_F13']),
               'out4':jport_out([1, 840, 1384, 'Model_1_3_N10S1_3_D1_4_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_3_step(2)',
            }),
            'step3':jprocess({
               'callrate':52,
               'rate_period':0.0028888888888888892,
               'base_task':False,
               'process':'Model_1_3_step(3)',
            }),
            'step4':jprocess({
               'callrate':208,
               'rate_period':0.011555555555555557,
               'base_task':False,
               'process':'Model_1_3_step(4)',
            }),
         }, id=1),
         'Model_1_4':jmodule({
            'init':'Model_1_4_initialize()',
            'send':'Model_1_4_HandleAsdIdRequest()',
            'poll':'Model_1_4_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_4_step(0)',
            }),
            'step1':jprocess({
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_4_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 840, 1384, 'Model_1_4_N10S1_3_D1_4_F13']),
               'out1':jport_out([1, 776, 1384, 'Model_1_4_N12S1_4_D1_6_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_4_step(2)',
            }),
         }, id=2),
         'Model_1_5':jmodule({
            'init':'Model_1_5_initialize()',
            'send':'Model_1_5_HandleAsdIdRequest()',
            'poll':'Model_1_5_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_5_step(0)',
            }),
            'step1':jprocess({
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_5_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 240, 1384, 'Model_1_5_N09S1_3_D1_5_F13']),
               'out1':jport_out([1, 192, 1384, 'Model_1_5_N11S1_5_D1_6_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_5_step(2)',
            }),
         }, id=3),
         'Model_1_6':jmodule({
            'init':'Model_1_6_initialize()',
            'send':'Model_1_6_HandleAsdIdRequest()',
            'poll':'Model_1_6_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_6_step(0)',
            }),
            'step1':jprocess({
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_6_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 224, 1384, 'Model_1_6_N05S1_2_D1_6_F13']),
               'in2':jport_in([1, 248, 1384, 'Model_1_6_N08S1_3_D1_6_F13']),
               'in3':jport_in([1, 192, 1384, 'Model_1_6_N11S1_5_D1_6_F13']),
               'in4':jport_in([1, 776, 1384, 'Model_1_6_N12S1_4_D1_6_F13']),
               'out1':jport_out([1, 840, 1384, 'Model_1_6_N13S1_6_D1_7_F13']),
               'out2':jport_out([1, 2, 1384, 'Model_1_6_N14S1_6_D1_3_F13']),
               'out3':jport_out([1, 160, 1384, 'Model_1_6_N15S1_6_D1_2_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_6_step(2)',
            }),
         }, id=4),
         'Model_1_7':jmodule({
            'init':'Model_1_7_initialize()',
            'send':'Model_1_7_HandleAsdIdRequest()',
            'poll':'Model_1_7_ServiceNotifications()',
            'step0':jprocess({
               'callrate':1,
               'rate_period':5.5555555555555551E-5,
               'base_task':False,
               'process':'Model_1_7_step(0)',
            }),
            'step1':jprocess({
               'out1':jport_out([1, 672, 1500, 'Model_1_7_N00S1_7_D1_9_F12']),
               'callrate':12,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'Model_1_7_step(1)',
            }),
            'step2':jprocess({
               'in1':jport_in([1, 808, 1384, 'Model_1_7_N04S1_2_D1_7_F13']),
               'in2':jport_in([1, 840, 1384, 'Model_1_7_N13S1_6_D1_7_F13']),
               'callrate':13,
               'rate_period':0.00072222222222222219,
               'base_task':False,
               'process':'Model_1_7_step(2)',
            }),
         }, id=5),
         'sys':jmodule({
            'cr12_in':jprocess({
               '_out1':jport_out(),
               'callrate':12,
            }),
            'cr12_out':jprocess({
               '_in1':jport_in(),
               'callrate':12,
            }),
         }, id=6),
         'graph':[{'connect' : (
                     (('Model_1_7', 'step1', 'out1'), ('sys', 'cr12_out', '_in1')),   #N00
                     (('sys', 'cr12_in', '_out1'), ('Model_1_2', 'step1', 'in1')),    #N03
                     (('Model_1_2', 'step2', 'out1'), ('Model_1_7', 'step2', 'in1')), #N04
                     (('Model_1_2', 'step2', 'out2'), ('Model_1_6', 'step2', 'in1')), #N05
                     (('Model_1_2', 'step2', 'out3'), ('Model_1_3', 'step2', 'in1')), #N06
                     (('Model_1_3', 'step2', 'out1'), ('Model_1_2', 'step2', 'in1')), #N07
                     (('Model_1_3', 'step2', 'out2'), ('Model_1_6', 'step2', 'in2')), #N08
                     (('Model_1_3', 'step2', 'out3'), ('Model_1_5', 'step2', 'in1')), #N09
                     (('Model_1_3', 'step2', 'out4'), ('Model_1_4', 'step2', 'in1')), #N10
                     (('Model_1_5', 'step2', 'out1'), ('Model_1_6', 'step2', 'in3')), #N11
                     (('Model_1_4', 'step2', 'out1'), ('Model_1_6', 'step2', 'in4')), #N12
                     (('Model_1_6', 'step2', 'out1'), ('Model_1_7', 'step2', 'in2')), #N13
                     (('Model_1_6', 'step2', 'out2'), ('Model_1_3', 'step2', 'in2')), #N14
                     (('Model_1_6', 'step2', 'out3'), ('Model_1_2', 'step2', 'in2')), #N15
                     ),
                  'exec' : ( # sorted by callrate (priority)
                     (
                         ('Model_1_2', 'step0'),
                         ('Model_1_3', 'step0'),
                         ('Model_1_4', 'step0'),
                         ('Model_1_5', 'step0'),
                         ('Model_1_6', 'step0'),
                         ('Model_1_7', 'step0'),
                         ), #12/13
                     (('sys', 'cr12_in'), ('Model_1_2', 'step1')), #12
                     (('Model_1_3', 'step1'),), 
                     (('Model_1_4', 'step1'),),
                     (('Model_1_5', 'step1'),),
                     (('Model_1_6', 'step1'),),
                     (('Model_1_7', 'step1'), ('sys', 'cr12_out')),
                     (('Model_1_2', 'step2'),), #13
                     (('Model_1_3', 'step2'),), 
                     (('Model_1_4', 'step2'),),
                     (('Model_1_5', 'step2'),),
                     (('Model_1_6', 'step2'),),
                     (('Model_1_7', 'step2'),),
                     (('Model_1_2', 'step3'),), #26
                     (('Model_1_3', 'step3'),), #52
                     (('Model_1_3', 'step4'),), #208
                     (('Model_1_2', 'step4'),), #832
                     (('Model_1_2', 'step5'),), #3328
                     (('Model_1_2', 'step6'),), #9984
                  )
               },]
      }

# =====================================
#  APP Class
# =====================================

class app(jobj):
   def verify_param(self, param):
      if (isinstance(param, dict) and \
          ('model_name' in param) and \
          ('dsp_target' in param)):
         return True
      else:
         expected_param = """
             param = {
                 'model_name': ...,
                 'dsp_target': ...
             }
             """
         print('Error: JConfigApp param expected as dictionary')
         print(expected_param)
         return False

   def default(self, param=[{}]):
      param=param[0]
      if not self.verify_param(param):
         raise ValueError('Unexpected JConfigApp param')

      self.type    = param['model_name']
      sample_rate  = 48000
      frame_size   = 32
      num_in = 2
      num_out = 16
      num_ann_in = 3
      num_ann_out = 2
      num_mic_in = 1
      self.data = {
         'dsp_target'   : param['dsp_target'],
         'In'           : BamInportConfig(['UnitTest_Target_AudioIn', num_in, frame_size, sample_rate]),
         'Out'          : BamOutportConfig(['UnitTest_Target_AudioOut']),
         'ArmPreAmp'    : FeatureArmPreamp([num_in, num_out]),
         'PreAmp'       : FeaturePreamp([num_in, num_out]),
         'PostProcess'  : FeaturePostProcess(num_out),
         'TestRouter'   : FeatureTestRouter(num_out),
         'InputSelect'  : FeatureInputSelect(num_out),
       #  'Ann'          : FeatureAnnouncements([num_ann_in, num_ann_out]),
      }
      self.control = {
         'ArmPreAmp'    : FeatureArmPreamp([num_in,num_out]),
         'PreAmp'       : FeaturePreamp([num_in,num_out]),
         'TestRouter'   : FeatureTestRouter(num_out),
         'PostProcess'  : FeaturePostProcess(num_out),
         'InputSelect'  : FeatureInputSelect(num_out),
       #  'Ann'          : FeatureAnnouncements([num_ann_in, num_ann_out]),
      }
      if 'baf_multi' in param and param['baf_multi']:
         self.baf = {
            'Baf2': jframework_Model_A2(id=0),
         }
      else: 
        self.baf = {
         'Baf1':   jframework_crosscore([param['model_name'], 1,8], id=0),
         'Baf2_1': jframework_crosscore([param['model_name'], 1,2], id=1),
         'Baf2_2': jframework_crosscore([param['model_name'], 1,3], id=2),
         'Baf2_3': jframework_crosscore([param['model_name'], 1,4], id=3),
         'Baf2_4': jframework_crosscore([param['model_name'], 1,5], id=4),
         'Baf2_5': jframework_crosscore([param['model_name'], 1,6], id=5),
         'Baf2_6': jframework_crosscore([param['model_name'], 1,7], id=6),
         'Baf3':   jframework_crosscore([param['model_name'], 1,9], id=7),
         'Baf4':   jframework_crosscore([param['model_name'], 1,1], id=8),
         'Baf5':   jframework_crosscore([param['model_name'], 1,10], id=9),
        }
      
      self.GenConfigFlat()
      self.GenConfigMatlabStruct()
      self.GenConfigGraph()


if matlab_engine:
   mleng = matlab.engine.start_matlab()
   mleng.addpath("application/model")
if 'param' in globals():
   app_config = app([param])
else:
   app_config = app()
if matlab_engine:
   mleng.quit()
