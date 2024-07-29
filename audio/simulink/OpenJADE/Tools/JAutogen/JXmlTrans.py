import sys
import os

from JFrameworkStru import *
from JLogPrint import *
import xml.etree.ElementTree as ElementTree



class JXmlTrans:
    JMODULE_INIT = '_initialize()'
    JMODULE_SEND = '_HandleAsdIdRequest()'
    JMODULE_POLL = '_ServiceNotifications()'
    def __init__(self, filename=None, partitionname=None):
        self.filename = filename
        self.partitionname = partitionname
        self.frame_size = 32
        self.baserate = 48000

    def ParsePingPongXML(self, pingpong_xmlfilename):
        tpDict = {'float': 4, 'uint32_t': 4, 'int': 4, 'int32_t': 4, 'uint16_t': 2, 'short': 2, 'uint8_t': 1, 'char': 1}
        ppDict = {}
        try:
            tree_pingong = ElementTree.parse(pingpong_xmlfilename)
            root_pingpong = tree_pingong.getroot()
            for ppBuf in root_pingpong.findall('Struct'):
                bufSize = 0
                for eachBuf in ppBuf.findall('Field'):
                    wd = int(eachBuf.get('width'))
                    sz = tpDict[eachBuf.get('dtypeStr')]
                    bufSize = bufSize + wd * sz
                ppDict[ppBuf.get('id')] = bufSize // 4
        except IOError:
            #print('Warning: JXmlTrans: PingPong XML File does not exist')
            Jlog_print(JAG_WARNING, 'JXmlTrans: PingPong XML File does not exist')
        return ppDict

    def GenJModule(self, objdata=None, xml_pingpong=None):
        # Extract all PingPong Info from PingPong XML file, xml_pingpong equals None means No PingPong support
        pingpongDict = self.ParsePingPongXML(xml_pingpong) if xml_pingpong is not None else {}
        partition_node = None
        
        # The format of vl: [False/True, Name of Partition, Base sampling rate, [[[num_of_chan_inport1, num_of_chan_inport2, ...], [num_of_chan_outport1, num_of_chan_outport2, ...]], [[], []], ...], [callrate1, callrate2, ...], [rate_period1, rate_period2, ...]]
        vl = [False, None, None, None, None, None]
        try:
            tree = ElementTree.parse(self.filename)
            root = tree.getroot()
            if self.partitionname == None:
                partition_node = root.findall('Partition')
                partition_node = partition_node[0]
            else:
                for item in root.findall('Partition'):
                    if item.get('id') == self.partitionname:
                        partition_node = item
                        break
            if partition_node is not None:
                vl[0] = True
                vl[1] = jmodule_id = partition_node.get('id')
                vl[2] = 48000
                vl[3] = []
                vl[4] = []
                vl[5] = []
                proc_cnt = 0
                base_task_id = partition_node.get('base_task_id')
                for item in partition_node.findall('Task'):
                    # Each 'Task' matches callrate, that may includes multi inport/outport with different number of channels
                    portNumSum = [[], []]
                    jprocDict = {}
                    inport_cnt = 1
                    outport_cnt = 1
                    for port_item in item.findall('InPort') + item.findall('Inport'):
                        # Check 'cross_core' flag
                        ccFlag = port_item.get('cross_core')
                        if (xml_pingpong is None) and (ccFlag is not None) and (ccFlag == '1'):
                            continue
                        # Format [number_of_in/out, frame_size, sample_rate, in/out_buffer_pointer]
                        jportList = []
                        if (ccFlag is None) or (ccFlag == '0'):
                            # "dimensions", it could be either a list or a scalar
                            # For list, the format is [frame_size, num_of_channels]
                            # For scalar, it means for both frame_size and num_of_channels
                            tmp = port_item.get('dimensions')
                            num_of_chan = int(tmp[tmp.find(' '):-1]) if '[' in tmp and ']' in tmp else int(tmp)
                            # dimensions format could be [frame_size, num_of_channel] or [frame_size num_of_channel]
                            self.frame_size = int(tmp[1:tmp.find(',')]) if tmp.find(',') != -1 else int(tmp[1:tmp.find(' ')])
                            samplerate = int(1 / float(item.get('rate_in_sec')) * float(self.frame_size))
                        else:
                            num_of_chan = 1
                            tagname = port_item.get('data_type')
                            #bufkey = tagname[tagname.find(jmodule_id + '_') + len(jmodule_id) + 1:(tagname.find('_tag'))]
                            bufkey = tagname[tagname.find(jmodule_id + '_'):(tagname.find('_tag'))]
                            self.frame_size = pingpongDict[bufkey]
                            # Here the defaule frame size is used: 32
                            #samplerate = int(1 / float(item.get('rate_in_sec')) * float(32))
                            samplerate = int(1 / float(item.get('rate_in_sec')))
                        if samplerate < 48100 and samplerate > 47900:
                            samplerate = 48000
                        elif samplerate < 44200 and samplerate > 44000:
                            samplerate = 44100
                        portNumSum[0].append(num_of_chan)
                        jportList.append(num_of_chan)
                        jportList.append(self.frame_size)
                        jportList.append(int(samplerate))
                        jportList.append(port_item.get('id'))
                        jprocDict['in' + str(inport_cnt)] = jport_in(jportList)
                        inport_cnt = inport_cnt + 1
                    for port_item in item.findall('OutPort') + item.findall('Outport'):
                        ccFlag = port_item.get('cross_core')
                        if (xml_pingpong is None) and (ccFlag is not None) and (ccFlag == '1'):
                            continue
                        jportList = []
                        if (ccFlag is None) or (ccFlag == '0'):
                            tmp = port_item.get('dimensions')
                            # "dimensions", it could be either a list or a scalar
                            # For list, the format is [frame_size, num_of_channels]
                            # For scalar, it means both frame_size and num_of_channels
                            if '[' in tmp and ']' in tmp:
                                num_of_chan = int(tmp[tmp.find(' '):-1])
                                # dimensions format could be [frame_size, num_of_channel] or [frame_size num_of_channel]
                                self.frame_size = int(tmp[1:tmp.find(',')]) if tmp.find(',') != -1 else int(tmp[1:tmp.find(' ')])
                            else:
                                num_of_chan = int(tmp)
                                self.frame_size = int(tmp)
                            samplerate = int(1 / float(item.get('rate_in_sec')) * float(self.frame_size))
                        else:
                            num_of_chan = 1
                            tagname = port_item.get('data_type')
                            #bufkey = tagname[tagname.find(jmodule_id + '_') + len(jmodule_id) + 1:(tagname.find('_tag'))]
                            bufkey = tagname[tagname.find(jmodule_id + '_'):(tagname.find('_tag'))]
                            self.frame_size = pingpongDict[bufkey]
                            # Here the defaule frame size is used: 32
                            #samplerate = int(1 / float(item.get('rate_in_sec')) * float(32))
                            samplerate = int(1 / float(item.get('rate_in_sec')))
                        if samplerate < 48100 and samplerate > 47900:
                            samplerate = 48000
                        elif samplerate < 44200 and samplerate > 44000:
                            samplerate = 44100
                        portNumSum[1].append(num_of_chan)
                        jportList.append(num_of_chan)
                        jportList.append(self.frame_size)
                        jportList.append(int(samplerate))
                        jportList.append(port_item.get('id'))
                        jprocDict['out' + str(outport_cnt)] = jport_out(jportList)
                        outport_cnt = outport_cnt + 1
                    jproc_id = item.get('id')
                    jprocDict['callrate'] = item.get('rate_factor')
                    jprocDict['rate_period'] = item.get('rate_in_sec')
                    if base_task_id != None:
                        jprocDict['base_task'] = True if (base_task_id == item.get('id')) else False
                    # Calculate base sampling rate
                    if (jprocDict['callrate'] == '1'):
                        samplerate = int(1 / float(jprocDict['rate_period']) * float(self.frame_size))
                        if samplerate < 48100 and samplerate > 47900:
                            samplerate = 48000
                        elif samplerate < 44200 and samplerate > 44000:
                            samplerate = 44100
                        vl[2] = samplerate
                    jprocDict['process'] = jmodule_id + '_step(' + jproc_id + ')'
                    objdata['step' + jproc_id] = jprocess(jprocDict)
                    proc_cnt = proc_cnt + 1
                    vl[3].append(portNumSum)
                    vl[4].append(jprocDict['callrate'])
                    vl[5].append(jprocDict['rate_period'])
                # Add functions
                objdata['init'] = jmodule_id + JXmlTrans.JMODULE_INIT
                objdata['send'] = jmodule_id + JXmlTrans.JMODULE_SEND
                objdata['poll'] = jmodule_id + JXmlTrans.JMODULE_POLL
            else:
                #print('Error: No JModule is found in XML file')
                Jlog_print(JAG_ERROR, 'No JModule is found in XML file')
        except IOError:
            #print('Warning: JXmlTrans: XML file does not exist')
            Jlog_print(JAG_WARNING, 'JXmlTrans: XML file does not exist')
        return vl

    def GenSysAndAudioGraph(self, frmdata, jmlInfo=None, jmlid=0, sysName='sys'):
        sysinfo = {}
        graphDict = {}
        connectTup = ()
        execTup = ()
        off = 0
        # Loop inport and outport for each callrate
        for item in jmlInfo[3]:
            jprocD = {}
            sysN = None
            inum = onum = 1
            inportDict = {}
            outportDict = {}
            sr = int(self.baserate/int(jmlInfo[4][off]))
            # Inport
            for subitem in item[0]:
                #inportDict['_out' + str(inum)] = jport_out([subitem, self.frame_size, sr, None])
                # Keep Inport initialized with no arguments
                inportDict['_out' + str(inum)] = jport_out()
                connectTup = connectTup + (((sysName, 'cr' + jmlInfo[4][off] + '_in', '_out' + str(inum)),(jmlInfo[1], 'step' + str(off), 'in' + str(inum))),)
                inum = inum + 1
            for subitem in item[1]:
                #outportDict['_in' + str(onum)] = jport_in([subitem, self.frame_size, sr, None])
                # Keep Inport initialized with no arguments
                outportDict['_in' + str(onum)] = jport_in()
                connectTup = connectTup + (((jmlInfo[1], 'step' + str(off), 'out' + str(onum)), (sysName, 'cr' + jmlInfo[4][off] + '_out', '_in' + str(onum))),)
                onum = onum + 1
            # Add JProcess to 'sys' module
            if bool(inportDict):
                inportDict['callrate'] = jmlInfo[4][off]
                sysinfo['cr' + jmlInfo[4][off] + '_in'] = jprocess(inportDict)
            if bool(outportDict):
                outportDict['callrate'] = jmlInfo[4][off]
                sysinfo['cr' + jmlInfo[4][off] + '_out'] = jprocess(outportDict)
            # Add exec
            if bool(inportDict) and bool(outportDict):
                execTup = execTup + (((sysName, 'cr' + jmlInfo[4][off] + '_in'), (jmlInfo[1], 'step' + str(off)), (sysName, 'cr' + jmlInfo[4][off] + '_out')),)
            elif bool(inportDict):
                execTup = execTup + (((sysName, 'cr' + jmlInfo[4][off] + '_in'), (jmlInfo[1], 'step' + str(off))),)
            elif bool(outportDict):
                execTup = execTup + (((jmlInfo[1], 'step' + str(off)), (sysName, 'cr' + jmlInfo[4][off] + '_out')),)
            else:
                execTup = execTup + (((jmlInfo[1], 'step' + str(off)),),)
            off = off + 1
        # Add 'sys' module to JFramework
        frmdata[sysName] = jmodule(sysinfo, jmlid) if bool(connectTup) else None
        # Add 'connect' to AudioGraph
        graphDict['connect'] = connectTup if bool(connectTup) else None
        # Add 'exec' to AudioGraph
        graphDict['exec'] = execTup
        #Instantiate 'graph'
        frmdata['graph'] = [graphDict]

