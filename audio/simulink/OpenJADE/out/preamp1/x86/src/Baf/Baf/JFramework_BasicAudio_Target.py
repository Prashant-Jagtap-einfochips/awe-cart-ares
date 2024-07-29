import sys
import os
from JFrameworkStru import *



class jframework_BasicAudio_Target(jframework):
   def default(self, param=[32, 48000, 0, 0]):
      self.configMatlabStruct=[]
      self.classInit={}
      self.isFeature=False
      self.isClass=False
      self.type='JFramework'
      self.isJModule=True
      self.crosscore=False
#      self.xml='OpenJADE/out/preamp1//slx/code/BasicAudio_SyncApi.xml'
      self.talaria_param_version=2
      self.model_name='BasicAudio'
#      self.partition='BasicAudio_Target'
      self.data = {
         'BasicAudio_Target':jmodule({
            'init':'BasicAudio_Target_initialize()',
            'send':'BasicAudio_Target_HandleAsdIdRequest()',
            'poll':'BasicAudio_Target_ServiceNotifications()',
            'step0':jprocess({
               'in1':jport_in([2, 32, 48000, 'UnitTest_Target_AudioIn']),
               'out1':jport_out([2, 32, 48000, 'UnitTest_Target_AudioOut']),
               'callrate':1,
               'rate_period':0.00066666666666666664,
               'base_task':True,
               'process':'BasicAudio_Target_step(0)',
            }),
         }, id=0),
         'sys':jmodule({
            'cr1_in':jprocess({
               '_out1':jport_out(),
               'callrate':1,
            }),
            'cr1_out':jprocess({
               '_in1':jport_in(),
               'callrate':1,
            }),
         }, id=1),
         'graph':[{'connect' : (
                     (('sys', 'cr1_in', '_out1'), ('BasicAudio_Target', 'step0', 'in1')),
                     (('BasicAudio_Target', 'step0', 'out1'), ('sys', 'cr1_out', '_in1')),
                     ),
                  'exec' : (
                     (('sys', 'cr1_in'), ('BasicAudio_Target', 'step0'), ('sys', 'cr1_out')),
               )},]
      }
