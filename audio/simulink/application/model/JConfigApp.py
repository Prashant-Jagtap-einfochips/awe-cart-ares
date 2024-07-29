from JFrameworkStru import *

class app(jobj):
    def default(self, param=[{'model_name':'Model'
                            }]):

      param=param[0]

      in_channel_cnt = 2
      frame_size = 32
      sample_rate = 48000
      model_name = param['model_name']

      self.type = model_name
      self.data = {
         'In' : bam_inport(['UnitTest_Target_AudioIn', in_channel_cnt, frame_size, sample_rate]),
         'Out': bam_outport(['UnitTest_Target_AudioOut']),
      }

      self.baf = {
         'Baf': jframework_singlecore([model_name], id=0)
      }
      self.GenConfigFlat()
      self.GenConfigMatlabStruct()

if 'param' in globals():
    app_config = app([param])
else:
    app_config = app()


