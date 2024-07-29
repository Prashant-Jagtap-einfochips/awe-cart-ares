import json
import xmltodict
import re
import ast
import pprint
from pathlib import Path

class JAutogenBafLite:
    def __init__(self, xml, path, part, ignore_cross):
        self.part = part
        self.ignore_cross = ignore_cross
        self.BAF_Lite_Gen(xml, path)

    def GetSyncApi(self, xml):
        with open(xml) as xml_file:
            syncapi = xmltodict.parse(xml_file.read())
            xml_file.close()
        return syncapi

    def GetPingPongStruct(self, xml):
        if not Path(xml).is_file():
            return None

        with open(xml) as xml_file:
            ppstruct = xmltodict.parse(xml_file.read())
            xml_file.close()
        return ppstruct['root']['Struct']

    #with open('SyncApi.json', 'w') as jsonFile:
    #    jsonData = json.dumps( GetSyncApi()['root'])
    #    jsonFile.write(jsonData)


    def GetIO(self, xml="Model_SyncApi.xml", path='./'):
        syncapi = self.GetSyncApi(xml)
        io = {}
        core = {}
        cross = {}
        partlist=[]
        isCross=False
        if isinstance(syncapi['root']['Partition'], list):
            partlist = syncapi['root']['Partition']
        else:
            partlist.append(syncapi['root']['Partition'])

        for part in partlist:
            part_id=part['@id']
            io[part_id]=[0]*int(part['@NumTasks'])
            core[part_id] = []
            cross[part_id] = []
            tasklist=[]
            ppstruct = self.GetPingPongStruct(path+'../'+part_id+'_ert_shrlib_rtw/'+part_id+'_PingPongStruct.xml')
            if isinstance(part['Task'], list):
                tasklist = part['Task']
            else:
                tasklist.append(part['Task'])
            for task in tasklist:
                id = int(task['@id'])
                io[part_id][id]={}
                if 'InPort' in task:
                    task_list = task['InPort']
                    if not isinstance(task_list, list): task_list = [task_list]
                    for task_item in task_list:
                        io[part_id][id]['incore'] = task_item['@id']
                        dim = ast.literal_eval(task_item['@dimensions'])
                        core[part_id].append(
                            {'id':task_item['@id'],
                            'io':'in',
                            'frame_size': dim[0],
                            'num_chan': dim[1],
                            })
                if (not self.ignore_cross) and ('Inport' in task):
                    task_list = task['Inport']
                    if not isinstance(task_list, list): task_list = [task_list]
                    for task_item in task_list:
                        io[part_id][id]['incross'] = task_item['@id']
                        if not isinstance(ppstruct, list): ppstruct = [ppstruct]
                        for pps in ppstruct:
                            if task_item['@id'] == pps['@id']:
                                ppb=[]
                                if not isinstance(pps['Field'], list): pps['Field']=[pps['Field']]
                                for f in pps['Field']:
                                    field={}
                                    field['id']=f['@id']
                                    field['frame_size']=int(f['@width'])
                                    field['num_chan']=1
                                    ppb.append(field)
                        cross[part_id].append({
                            'id':task_item['@id'],
                            'type':task_item['@data_type'].replace('_tag', '_type').replace('struct ',''),
                            'task':id,
                            'io':'in',
                            'buf': ppb})
                    isCross=True
                if 'OutPort' in task:
                    task_list = task['OutPort']
                    if not isinstance(task_list, list): task_list = [task_list]
                    for task_item in task_list:
                        io[part_id][id]['outcore'] = task_item['@id']
                        dim = ast.literal_eval(task_item['@dimensions'])
                        core[part_id].append(
                            {'id':task_item['@id'],
                            'io':'out',
                            'frame_size': dim[0],
                            'num_chan': dim[1],
                            })
                if (not self.ignore_cross) and ('Outport' in task):
                    task_list = task['Outport']
                    if not isinstance(task_list, list): task_list = [task_list]
                    for task_item in task_list:
                        io[part_id][id]['outcross'] = task_item['@id']
                        ppb={}
                        if not isinstance(ppstruct, list): ppstruct = [ppstruct]
                        for pps in ppstruct:
                            if task_item['@id'] == pps['@id']:
                                ppb=[]
                                if not isinstance(pps['Field'], list): pps['Field']=[pps['Field']]
                                for f in pps['Field']:
                                    field={}
                                    field['id']=f['@id']
                                    field['frame_size']=int(f['@width'])
                                    field['num_chan']=1
                                    ppb.append(field)
                        cross[part_id].append({
                            'id':task_item['@id'],
                            'type':task_item['@data_type'].replace('_tag', '_type').replace('struct ',''),
                            'task':id,
                            'io':'out',
                            'buf':ppb
                            })
                    isCross=True

        return core, cross, isCross

    def BVP_Config_Gen(self, core, cross, path):
        bvp_config='{\n'
        for part in core:
            bvp_config += '  "'+part + '": \n'
            bvp_config += '    [ \n'
            for port in core[part]:
                bvp_config += '      { \n'
                bvp_config += '        "id": "'        + port['id']+'",\n'
                bvp_config += '        "proc_id": '        + str(port['proc_id'])+',\n'
                bvp_config += '        "frame_size": '+ str(port['frame_size'])+',\n'
                bvp_config += '        "num_chan": '  + str(port['num_chan'])+',\n'
                bvp_config += '        "io": "'        + port['io']+'"\n'
                bvp_config += '      }, \n'
            for port in cross[part]:
                bvp_config += '      { \n'
                bvp_config += '        "id": "'        + port['id']+'",\n'
                bvp_config += '        "proc_id": '        + str(port['proc_id'])+',\n'
                bvp_config += '        "io": "cross'        + port['io']+'",\n'
                frame_size=0
                for buf in port['buf'] : frame_size += buf['frame_size']
                bvp_config += '        "frame_size": '+ str(frame_size)+',\n'
                bvp_config += '        "num_chan": '  + str(1)+',\n'
                bvp_config += '        "buf":  [ \n'
                for buf in port['buf']:
                    bvp_config += '        { \n'
                    bvp_config += '          "id": "'        + buf['id']+'",\n'
                    bvp_config += '          "frame_size": '+ str(buf['frame_size'])+',\n'
                    bvp_config += '          "num_chan": '  + str(buf['num_chan'])+'\n'
                    bvp_config += '        }, \n'
                bvp_config += '        ] \n'
                bvp_config += '      }, \n'
            bvp_config += '    ] \n'
            bvp_config += '  , \n'
        bvp_config+='}\n'
        with open(path+"../BVP_Config.json", "w") as code_file:
            code_file.write(bvp_config)
            code_file.close()

    def BVP_Config_Yaml(self, core, cross, path):
        bvp_config=''
        for part in core:
            bvp_config += part + ': \n'
            for port in core[part]:
                bvp_config += '    - \n'
                bvp_config += '      "id": "'        + port['id']+'"\n'
                bvp_config += '      "proc_id": '        + str(port['proc_id'])+'\n'
                bvp_config += '      "frame_size": '+ str(port['frame_size'])+'\n'
                bvp_config += '      "num_chan": '  + str(port['num_chan'])+'\n'
                bvp_config += '      "io": "'        + port['io']+'"\n'
            for port in cross[part]:
                bvp_config += '    - \n'
                bvp_config += '      "id": "'        + port['id']+'"\n'
                bvp_config += '      "proc_id": '        + str(port['proc_id'])+'\n'
                bvp_config += '      "io": "cross'        + port['io']+'"\n'
                frame_size=0
                for buf in port['buf'] : frame_size += buf['frame_size']
                bvp_config += '      "frame_size": '+ str(frame_size)+'\n'
                bvp_config += '      "num_chan": '  + str(1)+'\n'
                bvp_config += '      "buf":   \n'
                for buf in port['buf']:
                    bvp_config += '        - \n'
                    bvp_config += '          "id": "'        + buf['id']+'"\n'
                    bvp_config += '          "frame_size": '+ str(buf['frame_size'])+'\n'
                    bvp_config += '          "num_chan": '  + str(buf['num_chan'])+'\n'
        with open(path+"../BVP_Config.yml", "w") as code_file:
            code_file.write(bvp_config)
            code_file.close()


    def BAF_Lite_Gen(self, xml, path):
        core, cross, isCross = self.GetIO(xml, path)
        #print('cross:\n',cross)
        #print('core:\n', core)
        codegen={}

        # header
        for part in core:
            #print('part:',part)
            codegen[part] = '/*******************************************************************************\n'
            codegen[part] += '*  BOSE CORPORATION\n'
            codegen[part] += '*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED\n'
            codegen[part] += '*  This program may not be reproduced, in whole or in part in any\n'
            codegen[part] += '*  form or any means whatsoever without the written permission of:\n'
            codegen[part] += '*     BOSE CORPORATION\n'
            codegen[part] += '*     The Mountain\n'
            codegen[part] += '*     Framingham, MA 01701-9168\n'
            codegen[part] += '********************************************************************************\n'
            codegen[part] += '*  Title:       BAF_Lite.h\n'
            codegen[part] += '*\n'
            codegen[part] += '*  Description: BAF Lite macro definitions\n'
            codegen[part] += '*\n'
            codegen[part] += '*   This file has been autogenerated.\n'
            codegen[part] += '*\n'
            codegen[part] += '*******************************************************************************/\n'
            codegen[part] += '#include "TalariaAPI.h"\n\n'
            if isCross:
                codegen[part] += '#define real32_T float\n'
                codegen[part] += '#include "'+part+'_ping_pong_struct.h"\n\n'

        # pointer definitions
        for part in core:
            codegen[part] += "#define BAF_IO_POINTERS   \\\n"
            first = True
            for port in core[part]:
                if not first:
                    codegen[part] += "; \\\n"
                codegen[part] += "    float* " + port['id']
                first = False
            if isCross:
                for port in cross[part]:
                    if not first:
                        codegen[part] += "; \\\n"
                    codegen[part] += "    "+ port['type'] +"* " + port['id']
                    first = False
            codegen[part] += "\n\n"

        # BAF_RESOURCE_MEMORY_SIZE
        for part in core:
            codegen[part] += "#define BAF_RESOURCE_MEMORY_SIZE   \\\n"
            for port in core[part]:
                codegen[part] += "    " + port['id'].upper() + "_CHANNEL_COUNT*"+ port['id'].upper() + "_FRAME_SIZE_IN_SAMPLES*sizeof(float) + \\\n"
            if isCross:
                for port in cross[part]:
                    codegen[part] += "     sizeof(" +port['type']+  ") + \\\n"
            codegen[part] += "    0\n\n"

        # io pointers initialization
        for part in core:
            codegen[part] += "#define BAF_IO_POINTERS_INIT(arg) do {\\\n"
            codegen[part] += "    uint8_t* resptr = (uint8_t*)(arg)->resource.memory_pointer;\\\n"
            for port in core[part]:
                codegen[part] += "    " + port['id'] + " = (float*) resptr;"
                codegen[part] += " \\\n    resptr +=" + port['id'].upper() + "_CHANNEL_COUNT*"+ port['id'].upper() + "_FRAME_SIZE_IN_SAMPLES*sizeof(float);\\\n"
            if isCross:
                for port in cross[part]:
                    codegen[part] += "    " + port['id'] + " = ("+port['type']+"*)resptr;"
                    codegen[part] += " \\\n    resptr += sizeof(" +port['type']+  ");\\\n"
            codegen[part] += "} while(0)\n\n"

        # output callback
        for part in core:
            codegen[part] += "#define BAF_OUTPUT_CALLBACK_SYS(thread_level, arg)  do {\\\n"
            procid = 1
            for port in core[part]:
                if port['io'] == 'out':
                    codegen[part] += "  if(thread_level == " + port['id'].upper() + "_TASK_ID){ \\\n"
                    codegen[part] += "      BAF_Callback_sys_process_ctx(" + str(procid) + ", /* proc id */ \\\n"
                    codegen[part] += "          " + port['id'].upper() + "_FRAME_SIZE_IN_SAMPLES, /* frame size */ \\\n"
                    codegen[part] += "          " + port['id'].upper() + "_CHANNEL_COUNT, /* num chan in */ \\\n"
                    codegen[part] += "          0, /* num chan out */ \\\n"
                    codegen[part] += "          " + port['id'] + ", /* src */ \\\n"
                    codegen[part] += "          NULL, /* dst */ \\\n"
                    codegen[part] += "          (arg)); \\\n"
                    codegen[part] += "  }\\\n"
                    port['proc_id'] = procid
                    procid += 2
            if isCross:
                # output ping pong callback
                for port in cross[part]:
                    if port['io'] == 'out':
                        codegen[part] += "  if(thread_level == " + str(port['task'])+ "){ \\\n"
                        codegen[part] += "      BAF_Callback_sys_process_ctx(" + str(procid) + ", /* proc id */ \\\n"
                        codegen[part] += "          sizeof(" + port['type'] + ")/sizeof(float), /* frame size */ \\\n"
                        codegen[part] += "          1, /* num chan in */ \\\n"
                        codegen[part] += "          0, /* num chan out */ \\\n"
                        codegen[part] += "          " + port['id'] + ", /* src */ \\\n"
                        codegen[part] += "          NULL, /* dst */ \\\n"
                        codegen[part] += "          (arg)); \\\n"
                        codegen[part] += "  }\\\n"
                        port['proc_id'] = procid
                        procid += 2
            codegen[part] += "} while(0)\n\n"

        # input callback
        for part in core:
            codegen[part] += "#define BAF_INPUT_CALLBACK_SYS(thread_level, arg)   do {\\\n"
            procid = 0
            for port in core[part]:
                if port['io'] == 'in':
                    codegen[part] += "  if(thread_level == " + port['id'].upper() + "_TASK_ID){ \\\n"
                    codegen[part] += "      BAF_Callback_sys_process_ctx(" + str(procid) + ", /* proc id */ \\\n"
                    codegen[part] += "          " + port['id'].upper() + "_FRAME_SIZE_IN_SAMPLES, /* frame size */ \\\n"
                    codegen[part] += "          0, /* num chan in */ \\\n"
                    codegen[part] += "          " + port['id'].upper() + "_CHANNEL_COUNT, /* num chan out */ \\\n"
                    codegen[part] += "          NULL, /*  src */ \\\n"
                    codegen[part] += "          " + port['id'] + ", /* dst */ \\\n"
                    codegen[part] += "          (arg)); \\\n"
                    codegen[part] += "  }\\\n"
                    port['proc_id'] = procid
                    procid += 2
            if isCross:
                # input ping pong callback:
                for port in cross[part]:
                    if port['io'] == 'in':
                        codegen[part] += "  if(thread_level == " + str(port['task']) + "){ \\\n"
                        codegen[part] += "      BAF_Callback_sys_process_ctx(" + str(procid) + ", /* proc id */ \\\n"
                        codegen[part] += "          sizeof(" + port['type'] + ")/sizeof(float), /* frame size */ \\\n"
                        codegen[part] += "          0, /* num chan in */ \\\n"
                        codegen[part] += "          1, /* num chan out */ \\\n"
                        codegen[part] += "          NULL, /*  src */ \\\n"
                        codegen[part] += "          " + port['id'] + ", /* dst */ \\\n"
                        codegen[part] += "          (arg)); \\\n"
                        codegen[part] += "  }\\\n"
                        port['proc_id'] = procid
                        procid += 2
            codegen[part] += "} while(0)\n\n"
        self.BVP_Config_Yaml(core, cross, path)
        self.BVP_Config_Gen(core, cross, path)

        # print
        #for part in core:
        #    print(codegen[part])

#        for part in core:
        with open(path+"/BAF_Lite.h", "w") as code_file:
            code_file.write(codegen[self.part])
            code_file.close()

