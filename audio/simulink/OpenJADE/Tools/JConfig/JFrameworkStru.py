import sys
import os
import copy
import pprint

from JConfigVersion import *



class jobj:
   SL=1
   BCF=2
   JM=3
   max_len=80
   pp = pprint.PrettyPrinter(indent=1)
   configGraph=[]
   configMatlabStruct=[]
   classInit={}
   configFlat={}
   class_list=[]
   max_depth=10
   @staticmethod
   def featurebam(d, parent=None, bam=None):
      for k in sorted(d.keys()): #d.items():
         v=d[k]
         if isinstance(v, jobj):
                if not parent:
                    if v.type:
                        bam[k]={}
                        bam[k]['bam']={}
                        bam[k]['type']=v.type
                        jobj.featurebam(v.data, k, bam[k]['bam'])
                else:
                    if v.type:
                        bam[k]={}
                        bam[k]['type'] = v.type
                        bam[k]['bam'] ={}
                        if 'RTC' in v.data :
                            bam[k]['RTC']=[]
                            for item in v.data['RTC']:
                                bam[k]['RTC'].append('RTC_'+parent+k+item)
                        if 'TOP' in v.data :
                            bam[k]['TOP_READ']=[]
                            bam[k]['TOP_WRITE']=[]
                            for item in v.data['TOP']:
                                bam[k]['TOP_WRITE'].append('TOP_WRITE_'+parent+k+item)
                            for item in v.data['TOP']:
                                bam[k]['TOP_READ'].append('TOP_READ_'+parent+k+item)
                        jobj.featurebam(v.data, parent+k, bam[k]['bam'])
                    else:
                        jobj.featurebam(v.data, parent, bam)
   @staticmethod
   def iterdict(d, parent=None, control=False):
      for k,v in d.items():
        if isinstance(v, jobj):
            jobj.iterdict(v.data, parent+'_'+k, control) if(parent) else jobj.iterdict(v.data, k, control)
            if control and hasattr(v, 'control'):
               jobj.iterdict(v.control, parent+'_'+k, control) if(parent) else jobj.iterdict(v.data, k, control)
        elif isinstance(v, dict):
            jobj.iterdict(v, parent+'_'+k, control) if(parent) else jobj.iterdict(v, k, control)
        else:
            token = parent+'_'+k if(parent) else k
            if isinstance(v, list) and len(v) > 1 and isinstance(v[1], tuple):
                # there is a configFlat filter v(2 is the filter)
                jobj.configFlat[token]=v[0]
            else:
                jobj.configFlat[token]=v
   @staticmethod
   def classinit(d, parent='obj', parent_obj=None):
      for k,v in d.items():
        if isinstance(v, jobj):
            jobj.classinit(v.data, parent+'.'+k,v)
        elif isinstance(v, dict):
            jobj.iterdict(v, parent+'.'+k, control = False)
        elif isinstance(parent_obj, jobj) and parent_obj.isClass:
           if not(k=='num_in' or k=='num_out' or k=='RTC' or k=='TOP'):
               token = parent+'.'+k
               jobj.classInit[token]=v
   @staticmethod
   def get_edges(d, depth, values=False, parent=None, top=None):
      for k in sorted(d.keys(), reverse=True):
         v=d[k]
         if isinstance(v, jobj):
            if [v.__class__.__name__,v] not in jobj.class_list:
                jobj.class_list.append([v.__class__.__name__,v])
            if(parent):
               if( depth < jobj.max_depth ):
                  if v.isFeature:
                      jobj.get_edges(v.data, depth+1, values, parent+k,top)
                  else:
                      jobj.get_edges(v.data, depth, values, parent, top) # if not feature, do not add to the path
                  if( depth > 0 ) and v.isFeature:
                     jobj.configGraph.append(parent+k+' [shape=box,height=.1, width=2 style=filled fillcolor=orange color=red label='+k+'];')
                  if( depth > 1 ):
                     jobj.configGraph.append(parent+'->'+parent+k+'[tailport=e, headport=w];')
            else:
               if( depth < jobj.max_depth ):
                  jobj.configGraph.append('subgraph "cluster_'+k+'" { label='+k)
                  jobj.get_edges(v.data, depth+1, values, k, top)
                  if( depth > 0 ):
                     jobj.configGraph.append(k+' [shape=box,height=.1, width=2 style=filled fillcolor=lightgray];')
                  jobj.configGraph.append(' }')
         elif isinstance(v, dict):
            jobj.configGraph.append(k+' [shape=box color=red];')
            jobj.get_edges(v, depth+1, values, k, top)
         else:
            if(values):
               if isinstance(v, list) and len(v) > 1 and isinstance(v[1], tuple):
                    # there is a cfg filter (v[1] is the filter touple)
                    v=v[0]
               if(parent):
                  if(depth>1):
                     jobj.configGraph.append(parent+'->'+parent+k+':n[color=lightgray, tailport=e, headport=w];')
                  if k=='RTC' or k=='TOP':
                     if k == 'TOP' : label = k+'_WRITE_'+top+parent+v[0]  # + ' | ' +k+'_READ_'+top+parent+v[0]
                     else :          label = k+'_'+top+parent+v[0]
                     for item in v[1:]:
                        if k == 'TOP' : label = label + ' | ' + k+'_WRITE_'+top+parent+item  #+ ' | ' +k+'_READ_'+top+parent+item
                        else :          label = label + ' | ' +k+'_'+top+parent+item
                     jobj.configGraph.append(parent+k+' [shape = record,height=.1, width=3, color=blue style=filled fillcolor=lightgray label="{<n>'+k+' | {'+label+'} }"];')
                  else:
                     label = str(v).replace('], [','],| [')
                     label = label[:jobj.max_len]+(label[jobj.max_len:] and '...')  # truncate
                     jobj.configGraph.append(parent+k+' [shape = record,height=.1, width=3, color=blue label="{<n>'+k+' | {'+label+'} }"];')
               else:
                  jobj.configGraph.append('subgraph "cluster_'+top+'" { label='+top)
                  #print('app->app_'+k+':n;')
                  jobj.configGraph.append(top+'_'+k+' [shape = record,height=.1, width=3 color=blue label="{<n>'+k+' | '+str(v)+'}"];')
                  jobj.configGraph.append('}')
   @staticmethod
   def sl_cfg(d, parent=None, depth='      '):
      for k in sorted(d.keys()): #d.items():
         v=d[k]
         if isinstance(v, jobj):
               if not parent:
                  if v.isClass:
                     if hasattr(v, 'btc'):
                        btc = str(v.btc)
                     else:
                        btc = str(v.type)
                     jobj.configMatlabStruct.append(depth+k+" = "+ btc +";\n")
                  else:
                     jobj.configMatlabStruct.append(depth + k + ' = struct( ...\n')
                     jobj.sl_cfg(v.data, k, depth+'   ')
                     jobj.configMatlabStruct[-1] = ' ) ;\n' # remove comma at the end
               else:
                  if v.isClass:
                     if hasattr(v, 'btc'):
                        btc = str(v.btc)
                     else:
                        btc = str(v.type)
                     jobj.configMatlabStruct.append(depth+"'"+ k +"' , "+ btc )
                     jobj.configMatlabStruct.append(", ...\n")
                  elif v.isFeature:
                     jobj.configMatlabStruct.append(depth + "'" + k + "', struct( ...\n")
                     jobj.sl_cfg(v.data, k, depth+'   ')
                     jobj.configMatlabStruct[-1] = ' )'
                     jobj.configMatlabStruct.append(' , ...\n')
                  else:
                     jobj.sl_cfg(v.data, k, depth)
         elif isinstance(v, dict):
                if not parent:
                    jobj.configMatlabStruct.append(depth + k + ' = struct( ...\n')
                else:
                    jobj.configMatlabStruct.append(depth + "'" + k + "', struct( ...\n")
                jobj.sl_cfg(v, k, depth+'   ')
                if not parent:
                    jobj.configMatlabStruct[-1] = ' ) ;\n'
                else:
                    jobj.configMatlabStruct[-1] = ' )'
                    jobj.configMatlabStruct.append(' , ...\n')
         else:
            if isinstance(v, list) and len(v) > 1 and isinstance(v[1], tuple):
                # there is a cfg filter v(2 is the filter)
                v=v[0]
            if parent:
                if isinstance(v, str):
                    jobj.configMatlabStruct.append(depth+"'"+k+"', '"+ str(v)+"'")
                else:
                    jobj.configMatlabStruct.append(depth+"'"+k+"', "+ str(v))
                jobj.configMatlabStruct.append(",...\n")
            else:
                if isinstance(v, str):
                    jobj.configMatlabStruct.append(depth+k+" = '"+ str(v)+"';\n")
                else:
                    jobj.configMatlabStruct.append(depth+k+" = "+ str(v)+";\n")

   def default(self, data=None):
      self.data = {}
   def __init__(self, data=None, implemented=True, id=None):
      self.configMatlabStruct=[]
      self.classInit={}
      self.isFeature=True
      self.isClass=False
      self.type=None
      self.implemented = implemented
      self.id = id
      if(data):
        if (isinstance(data,dict)):
            self.default()
            self.data.update(data)
        else:
            self.default(data)
      else:
          self.default()
   def update(self, data):
      if (data):  self.data.update(data)
   def set(self, data):
      if (data):  self.data = data
   def json(self):
      if (self.data):  jobj.pp.pprint(self.data)

   def GetJFramework(self, name = 'Baf'):
      if (self.baf):
         for _name, jframe in self.baf.items():
             if isinstance(jframe, jobj) and (jframe.__dict__['type'] == 'JFramework'):
                 if name == _name:
                    return jframe
      return None

   def GetCallRates(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetCallRates(idx)
      return None

   def GetProcesses(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetProcesses(idx)
      return None

   def GetNumIn(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetNumIn(idx)
      return None

   def GetNumOut(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetNumOut(idx)
      return None

   def GetRatePeriod(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetRatePeriod(idx)
      return None

   def GetFrameSize(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetFrameSize(idx)
      return None

   def GetInFrameSize(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetInFrameSize(idx)
      return None

   def GetOutFrameSize(self, idx = 0, name = 'Baf'):
      jframe = self.GetJFramework(name)
      if jframe is not None:
         return jframe.Jfmr_GetOutFrameSize(idx)
      return None

   def GenConfigFlat(self):
      jobj.configFlat={}
      if (self.data): jobj.iterdict(self.data, control=True)
#      for k in sorted(jobj.configFlat.keys()):
#         print(k+':'+str(jobj.configFlat[k]))
   def GenConfigGraph(self, depth=10, values=True, debug=False):
      jobj.configGraph=[]
      jobj.class_list=[]
      jobj.max_depth=depth
      jobj.configGraph.append('digraph '+self.type+' {')
      jobj.configGraph.append('  rankdir=LR; nodesep=.05; clusterrank=2')
      if (self.data): jobj.get_edges(self.data, 0, values, None, self.type)
      jobj.configGraph.append('}')
      if debug:
          with open('jconfig.gv', 'w') as gv_file:
              for line in jobj.configGraph:
                  gv_file.write(line+'\n')
          with open('jconfig.h', 'w') as h_file:
              for v in jobj.class_list:
                  h_file.write(v[0]+'\n')
                  if 'RTC' in v[1].data:
                      h_file.write (', RTC:' + str(v[1].data['RTC']) + '\n')
                  if 'TOP' in v[1].data:
                      h_file.write (', TOP:' + str(v[1].data['TOP']) + '\n')
        # print(line)
   def GenConfigMatlabStruct(self, debug=False):
      jobj.configMatlabStruct=[]
      jobj.classInit={}
      if (self.data):
          jobj.sl_cfg(self.data)
          jobj.classinit(self.data)
          self.configMatlabStruct = copy.deepcopy(jobj.configMatlabStruct)
          self.classInit = copy.deepcopy(jobj.classInit)

      if debug:
          with open('jconfig.m', 'w') as gv_file:
              gv_file.write('classdef VegaCfg \n')
              gv_file.write('   properties ( Constant = false )\n')
              for line in jobj.configMatlabStruct:
                  line = line.replace('[[', '[') #bidimensional array
                  line = line.replace(']]', ']')
                  line = line.replace('], [', '; ')
                  gv_file.write(line)
              gv_file.write('   end\n')
              gv_file.write('   methods \n')
              gv_file.write('      function obj=VegaCfg \n')
              for k, v in jobj.classInit.items():
                  gv_file.write(str(k)+'='+str(v)+';\n')
              gv_file.write('   asdas 123123\n')
              gv_file.write('      end\n')
              gv_file.write('   end\n')
              gv_file.write('end\n')

class jport_in(jobj):
   def default(self, param=[0, 0, 0, None]):
      self.isFeature=False
      self.isJModule=True
      self.type='JPortIn'
      num_chan = param[0]
      frame_size = param[1]
      sample_rate = param[2]
      name = param[3]
      self.data = {
         'data_type':'PORT_DATA_REAL32',
         'direction':'PORT_DIRECTION_IN',
         'frame_size':frame_size,
         'num_chan':num_chan,
         'sample_rate':sample_rate,
         'port':name
      }

   def Jprt_Getframesize(self):
      fs = 0
      if (hasattr(self, 'data')):
         fs = self.data['frame_size']
      return fs

   def Jprt_Getnumofchannel(self):
      nc = 0
      if (hasattr(self, 'data')):
         nc = self.data['num_chan']
      return nc

   def Jprt_Getsamplerate(self):
      sr = 0
      if (hasattr(self, 'data')):
         sr = self.data['sample_rate']
      return sr


class jport_out(jobj):
   def default(self, param=[0, 0, 0, None]):
      self.isFeature=False
      self.isJModule=True
      self.type='JPortOut'
      num_chan = param[0]
      frame_size = param[1]
      sample_rate=param[2]
      name = param[3]
      self.data = {
         'data_type':'PORT_DATA_REAL32',
         'direction':'PORT_DIRECTION_OUT',
         'frame_size':frame_size,
         'num_chan':num_chan,
         'sample_rate':sample_rate,
         'port':name
      }

   def Jprt_Getframesize(self):
      fs = 0
      if (hasattr(self, 'data')):
         fs = self.data['frame_size']
      return fs

   def Jprt_Getnumofchannel(self):
      nc = 0
      if (hasattr(self, 'data')):
         nc = self.data['num_chan']
      return nc

   def Jprt_Getsamplerate(self):
      sr = 0
      if (hasattr(self, 'data')):
         sr = self.data['sample_rate']
      return sr

class jprocess(jobj):
   def default(self):
      self.isFeature=False
      self.isJModule=True
      self.type='JProcess'
      self.data = {
      }

   def Jprc_GetNumofjportin(self, jportin_name_list):
      num_of_jportin = 0
      if (hasattr(self, 'data')):
         for k, v in self.data.items():
            if (type(v) is jport_in):
               num_of_jportin = num_of_jportin + 1
               jportin_name_list.append(k)
      return num_of_jportin

   def Jprc_GetNumofjportout(self, jportout_name_list):
      num_of_jportout = 0
      if (hasattr(self, 'data')):
         for k, v in self.data.items():
            if (type(v) is jport_out):
               num_of_jportout = num_of_jportout + 1
               jportout_name_list.append(k)
      return num_of_jportout

   def Jprc_Getjportin(self, jportin_name):
      jptin = None
      if (hasattr(self, 'data')):
         jptin = self.data[jportin_name]
      return jptin

   def Jprc_Getjportout(self, jportout_name):
      jptout = None
      if (hasattr(self, 'data')):
         jptout = self.data[jportout_name]
      return jptout

   def Jprc_Getcallrate(self):
      if (hasattr(self, 'data')) and ('callrate' in self.data) and (self.data['callrate']):
        return self.data['callrate']
      else:
         print('Error: jprocess object does not have a \'callrate\' assigned')
         return None

   def Jprc_GetRatePeriod(self):
      if (hasattr(self, 'data')) and ('rate_period' in self.data) and (self.data['rate_period']):
        return self.data['rate_period']
      else:
         print('Error: jprocess object does not have a \'rate_period\' assigned')
         return None

   def Jprc_IsBaseTask(self):
      if (hasattr(self, 'data')) and ('base_task' in self.data):
        return self.data['base_task']
      else:
         return False

   def Jprc_GetID(self):
      if (self.id == None):
         print('Error: jprocess object does not have an ID assigned')
         return None
      else:
        return self.id

class jmodule(jobj):
   def default(self):
      self.isFeature=False
      self.isJModule=True
      self.type='JModule'
      self.data = {
      }

   def Jmdl_GetNumOfjprocess(self, jprocess_name_list):
      num_of_jprocess = 0
      if (hasattr(self, 'data')):
         for k, v in self.data.items():
            if (type(v) is jprocess):
               num_of_jprocess = num_of_jprocess + 1
               jprocess_name_list.append(k)
      return num_of_jprocess

   def Jmdl_Getjprocess(self, jprocess_name):
      jproc = None
      if (hasattr(self, 'data') and jprocess_name in self.data):
         jproc = self.data[jprocess_name]
      return jproc

   def Jmdl_GetID(self):
      if (self.id == None):
         print('Error: jmodule object does not have an ID assigned')
         return None
      else:
        return self.id

   def Jmdl_GetPartitionName(self):
      part = None
      if (hasattr(self, 'data')) and 'partition' in self.data:
         part = self.data['partition']
      return part

   def Jmdl_SetPartitionName(self, partition):
      if (hasattr(self, 'data')) and 'partition' in self.data:
         self.data['partition'] = partition

   def Jmdl_GetSyncApiXml(self):
      xml_file = None
      if (hasattr(self, 'data')) and 'xml' in self.data:
         xml_file = self.data['xml']
      return xml_file

   def Jmdl_SetSyncApiXml(self, sync_api_xml):
      if (hasattr(self, 'data')) and 'xml' in self.data:
               self.data['xml'] = sync_api_xml


class jframework(jobj):
   def default(self):
      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.data = {
      }

   def Jfmr_GetNumOfjmodule(self, jmodule_name_list):
      num_of_jmodule = 0
      if (hasattr(self, 'data')):
         for k, v in self.data.items():
            if (type(v) is jmodule):
               num_of_jmodule = num_of_jmodule + 1
               jmodule_name_list.append(k)
      return num_of_jmodule

   def Jfmr_Getjmodule(self, jmodule_name):
      jmod = None
      if (hasattr(self, 'data')):
         jmod = self.data[jmodule_name]
      return jmod

   def Jfmr_GetProcesses(self, idx):
      process_list = None
      if (hasattr(self, 'data') and ('graph' in self.data)):
         process_list = {}
         for thread_level, exec_item in enumerate(self.data['graph'][idx]['exec']):
            for process_id, process_item in enumerate(exec_item):
                jmodule_name  =  process_item[0]
                jprocess_name =  process_item[1]
                process_list['%s_%s' % (jmodule_name, jprocess_name)] = {'thread_level':thread_level, 'process_id':process_id}
      return process_list

   def Jfmr_GetCallRates(self, idx):
      callrates_list = None
      if (hasattr(self, 'data') and ('graph' in self.data)):
         callrates_list = {}
         for thread_level, exec_item in enumerate(self.data['graph'][idx]['exec']):
            for process_id, process_item in enumerate(exec_item):
                jmodule_name  =  process_item[0]
                jprocess_name =  process_item[1]
                if (jmodule_name.upper().startswith('SYS')) or ('_in' in jprocess_name) or ('_out' in jprocess_name):
                    continue
                jmod = self.Jfmr_Getjmodule(jmodule_name)
                jproc = jmod.Jmdl_Getjprocess(jprocess_name)
                if jproc is not None and thread_level not in callrates_list:
                    callrates_list[thread_level] = {'callrate': jproc.Jprc_Getcallrate(),
                                                    'rate_period':jproc.Jprc_GetRatePeriod(),
                                                    'base_task': jproc.Jprc_IsBaseTask() }
      return callrates_list

   def Jfmr_GetBaseTask(self, idx):
      if (hasattr(self, 'data') and ('graph' in self.data)):
         for thread_level, exec_item in enumerate(self.data['graph'][idx]['exec']):
            for process_id, process_item in enumerate(exec_item):
                jmodule_name  =  process_item[0]
                jprocess_name =  process_item[1]
                if (jmodule_name.upper().startswith('SYS')) or ('_in' in jprocess_name) or ('_out' in jprocess_name):
                    continue
                jmod = self.Jfmr_Getjmodule(jmodule_name)
                jproc = jmod.Jmdl_Getjprocess(jprocess_name)
                if jproc is not None and jproc.Jprc_IsBaseTask():
                    return jproc
      return None

   def Jfmr_GetID(self):
      if (self.id == None):
         print('Warning: jframework object does not have an ID assigned. Defaulted to zero')
         return 0
      else:
         return self.id

   def Jfmr_GetNumIn(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         jportin_name_list = []
         num_of_jportin = base_task.Jprc_GetNumofjportin(jportin_name_list)
         if num_of_jportin == 1:
            jptin = base_task.Jprc_Getjportin(jportin_name_list[0])
            return jptin.Jprt_Getnumofchannel()
      elif (hasattr(self, 'data')) and ('num_in' in self.data) and (self.data['num_in']):
         return self.data['num_in']
      elif (hasattr(self, 'test')) and ('num_in' in self.test) and (self.test['num_in']):
         return self.test['num_in']
      else:
            print('Warning: jframework object does not have a \'num_in\' assigned. Defaulted to 1')
            return 1

   def Jfmr_GetFrameSize(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         jportin_name_list = []
         num_of_jportin = base_task.Jprc_GetNumofjportin(jportin_name_list)
         if num_of_jportin == 1:
            jptin = base_task.Jprc_Getjportin(jportin_name_list[0])
            return jptin.Jprt_Getframesize()
      elif (hasattr(self, 'data')) and ('frame_size' in self.data) and (self.data['frame_size']):
         return self.data['frame_size']
      elif (hasattr(self, 'test')) and ('frame_size' in self.test) and (self.test['frame_size']):
         return self.test['frame_size']
      else:
            print('Warning: jframework object does not have a \'frame_size\' assigned. Defaulted to 32')
            return 32

   def Jfmr_GetInFrameSize(self, idx):
      return self.Jfmr_GetFrameSize(idx)

   def Jfmr_GetOutFrameSize(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         jportout_name_list = []
         num_of_jportout = base_task.Jprc_GetNumofjportout(jportout_name_list)
         if num_of_jportout == 1:
            jptout = base_task.Jprc_Getjportout(jportout_name_list[0])
            return jptout.Jprt_Getframesize()
      elif (hasattr(self, 'data')) and ('frame_size' in self.data) and (self.data['frame_size']):
         return self.data['frame_size']
      elif (hasattr(self, 'test')) and ('frame_size' in self.test) and (self.test['frame_size']):
         return self.test['frame_size']
      else:
            print('Warning: jframework object does not have a \'frame_size\' assigned. Defaulted to 32')
            return 32

   def Jfmr_GetNumOut(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         jportout_name_list = []
         num_of_jportout = base_task.Jprc_GetNumofjportout(jportout_name_list)
         if num_of_jportout == 1:
            jptout = base_task.Jprc_Getjportout(jportout_name_list[0])
            return jptout.Jprt_Getnumofchannel()
      elif (hasattr(self, 'data')) and ('num_out' in self.data) and (self.data['num_out']):
         return self.data['num_out']
      elif (hasattr(self, 'test')) and ('num_out' in self.test) and (self.test['num_out']):
         return self.test['num_out']
      else:
         print('Warning: jframework object does not have a \'num_out\' assigned. Defaulted to 1')
         return None

   def Jfmr_GetRatePeriod(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         return base_task.Jprc_GetRatePeriod()
      elif (hasattr(self, 'data')) and ('rate_period' in self.data) and (self.data['rate_period']):
         return self.data['rate_period']
      else:
         print('Warning: jframework object does not have a \'rate_period\' assigned. Defaulted to 0.00066666666666666664')
         return 0.00066666666666666664

   def Jfmr_GetCallInterval(self, idx):
      base_task = self.Jfmr_GetBaseTask(idx)
      if base_task != None:
         return base_task.Jprc_Getcallrate()
      elif (hasattr(self, 'data')) and ('call_interval' in self.data) and (self.data['call_interval']):
         return self.data['call_interval']
      elif (hasattr(self, 'test')) and ('call_interval' in self.test) and (self.test['call_interval']):
         return self.test['call_interval']
      else:
         return None

   def Jfmr_IsCrosscore(self):
      if hasattr(self, 'crosscore'):
         return self.crosscore
      else:
         return False

   def Jfmr_GetCoreId(self):
      if hasattr(self, 'coreId'):
         return self.coreId
      if (hasattr(self, 'data')) and ('coreId' in self.data) and (self.data['coreId']):
         return self.data['coreId']
      elif (hasattr(self, 'test')) and ('coreId' in self.test) and (self.test['coreId']):
         return self.test['coreId']
      else:
         return None

   def Jfmr_GetSOCId(self):
      if hasattr(self, 'socId'):
         return self.socId
      elif (hasattr(self, 'data')) and ('socId' in self.data) and (self.data['socId']):
         return self.data['socId']
      elif (hasattr(self, 'test')) and ('socId' in self.test) and (self.test['socId']):
         return self.test['socId']
      else:
         return None

   def Jfmr_TalariaParamVersion(self):
      if hasattr(self, 'talaria_param_version'):
         return self.talaria_param_version
      else:
         return 1

   def Jfmr_GetModelName(self):
      if hasattr(self, 'model_name'):
         return self.model_name
      else:
         return None

   def Jfmr_SetPartitionName(self, partition, model = None):
      if model is None and hasattr(self, 'partition'):
         self.partition = partition
      elif model is not None and model in self.data:
         jm = self.data[model]
         jm.Jmdl_SetPartitionName(partition)

   def Jfmr_GetPartitionName(self, model = None):
      if model is None and hasattr(self, 'partition'):
         return self.partition
      elif model is not None and model in self.data:
         jm = self.data[model]
         return jm.Jmdl_GetPartitionName()
      else:
         return None

   def Jfmr_GetSyncAPIXML(self, model = None):
      if model is None and hasattr(self, 'xml'):
         return self.xml
      elif model is not None and model in self.data:
         jm = self.data[model]
         return jm.Jmdl_GetSyncApiXml()
      else:
         return None

   def Jfmr_SetSyncAPIXML(self, xml_syncapi, model = None):
      if model is None:
         self.xml = xml_syncapi
      else:
         jm = self.data[model]
         jm.Jmdl_SetSyncApiXml(xml_syncapi)

   def Jfmr_GetPingPongXML(self):
      if hasattr(self, 'xml_pingpong'):
         return self.xml_pingpong
      else:
         return None

   def Jfmr_SetPingPongXML(self, xml_pingpong):
      self.xml_pingpong = xml_pingpong

class jframework_singlecore_v1(jframework):
   def default(self, param=[{'model_name':'Model'}]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param = param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'
      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=False
      self.xml=''

      self.talaria_param_version = 1
      self.model_name=model_name
      self.partition='%s_Target' % (self.model_name)


class jframework_singlecore(jframework):
   def default(self, param=[{'model_name':'Model'}]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param=param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'
      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=False
      self.xml=''

      self.talaria_param_version = 2
      self.model_name=model_name
      self.partition='%s_Target' % (self.model_name)

class jframework_crosscore_v1(jframework):
   def default(self, param=[{'model_name':'Model',
                             'socId':0,
                             'coreId':0
                            }]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param=param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'

         if 'socId' in param:
            socId = param['socId']
         else:
            socId = 0

         if 'coreId' in param:
            coreId = param['coreId']
         else:
            coreId = 0

      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'
         if len(param) > 1:
            socId = param[1]
         else:
            socId = 0
         if len(param) > 2:
            coreId = param[2]
         else:
            coreId = 0

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=True
      self.xml=''
      self.xml_pingpong=''

      self.talaria_param_version = 1
      self.model_name=model_name
      self.socId=socId
      self.coreId=coreId
      self.partition='%s_%d_%d' % (self.model_name, self.socId, self.coreId)

class jframework_crosscore(jframework):
   def default(self, param=[{'model_name':'Model',
                             'socId':0,
                             'coreId':0
                            }]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param=param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'

         if 'socId' in param:
            socId = param['socId']
         else:
            socId = 0

         if 'coreId' in param:
            coreId = param['coreId']
         else:
            coreId = 0

      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'
         if len(param) > 1:
            socId = param[1]
         else:
            socId = 0
         if len(param) > 2:
            coreId = param[2]
         else:
            coreId = 0

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=True
      self.xml=''
      self.xml_pingpong=''

      self.talaria_param_version = 2
      self.model_name=model_name
      self.socId=socId
      self.coreId=coreId
      self.partition='%s_%d_%d' % (self.model_name, self.socId, self.coreId)

class jframework_multicore_v1(jframework):
   def default(self, param=[{'model_name':'Model',
                             'socId':0,
                             'coreId':0
                            }]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param=param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'

         if 'socId' in param:
            socId = param['socId']
         else:
            socId = 0

         if 'coreId' in param:
            coreId = param['coreId']
         else:
            coreId = 0

      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'
         if len(param) > 1:
            socId = param[1]
         else:
            socId = 0
         if len(param) > 2:
            coreId = param[2]
         else:
            coreId = 0

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=False
      self.xml=''

      self.talaria_param_version = 1
      self.model_name=model_name
      self.socId=socId
      self.coreId=coreId
      self.partition='%s_%d_%d' % (self.model_name, self.socId, self.coreId)

class jframework_multicore(jframework):
   def default(self, param=[{'model_name':'Model',
                             'socId':0,
                             'coreId':0
                            }]):
      if (len(param)>0) and (isinstance(param[0], dict)):
         param=param[0]
         if 'model_name' in param:
            model_name = param['model_name']
         else:
            model_name = 'Model'

         if 'socId' in param:
            socId = param['socId']
         else:
            socId = 0

         if 'coreId' in param:
            coreId = param['coreId']
         else:
            coreId = 0

      else:
         if len(param) > 0:
            model_name = param[0]
         else:
            model_name = 'Model'
         if len(param) > 1:
            socId = param[1]
         else:
            socId = 0
         if len(param) > 2:
            coreId = param[2]
         else:
            coreId = 0

      self.isFeature=False
      self.isJModule=True
      self.type='JFramework'
      self.crosscore=False
      self.xml=''

      self.talaria_param_version = 2
      self.model_name=model_name
      self.socId=socId
      self.coreId=coreId
      self.partition='%s_%d_%d' % (self.model_name, self.socId, self.coreId)


class bam_inport(jobj) :
   def default(self,  param=[None, 2, 32, 48000]):
      self.type='ModelRootInportConfig'
      self.isClass=True
      self.data = {
         'BufferName': param[0],
         'AccessType': 'ImportedPointer',
         'ChannelCount': param[1],
         'FrameSamples': param[2],
         'SampleRateInHertz': param[3]
      }

class bam_outport(jobj) :
   def default(self,  param=[None]):
      self.type='ModelRootOutportConfig'
      self.isClass=True
      self.data = {
         'BufferName': param[0],
         'AccessType': 'ImportedPointer'
      }



