import os
import sys
import re
import time
import struct
import serial
import serial.tools.list_ports

from datetime import date



def Serial_GetAllComPort():
    ComList = [item.name for item in serial.tools.list_ports.comports()]
    return ComList

def Serial_Open(comPort='COM1', baud_rate=115200):
    comHandle = None
    portstr = comPort if int(comPort[3:]) < 10 else '\\\\.\\' + comPort
    try:
        comHandle = serial.Serial(port = portstr, baudrate=baud_rate, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE,
                                stopbits=serial.STOPBITS_ONE, timeout=1)
        comHandle.reset_input_buffer()
        comHandle.reset_output_buffer()
    except:
        print('The ' + comPort + ' cannot be opened')
    return comHandle

def Serial_Configure(comHandle=None, rtimeout=1, wtimeout=1):
    if comHandle != None:
        comHandle.timeout = rtimeout
        comHandle.write_timeout = wtimeout

def Serial_Write(comHandle=None, buf=None):
    comHandle.write(buf.encode())

def Serial_Read(comHandle=None):
    buf = comHandle.readline()
    return buf

def Serial_Flush(comHandle=None):
    if comHandle != None:
        comHandle.reset_input_buffer()
        comHandle.reset_output_buffer()

def Serial_Close(comHandle=None):
    if comHandle != None:
        comHandle.close()





def Proto_DwRequest(mode=0, num_of_int=1, base_addr=None, core_num=0):
    ss = None
    if base_addr != None:
        if mode == 0:
            # Dw Read Command
            tmp = str(hex(num_of_int))
            sn = '0' * (3 - len(tmp[2:])) + tmp[2:]
            ss = '?Dw' + str(core_num) + sn + base_addr + '\r'
        else:
            # Dw Write Command
            pass
    return ss

def Proto_DtRequest(mode=0, num_of_int=1, core_num=0, topoffset=0):
    ss = None
    if mode == 0:
        # Dt Read Command
        off = hex(topoffset).upper()[2:]
        offstr = (4 - len(off)) * '0' + off
        num = hex(num_of_int).upper()[2:]
        numstr = (2 - len(num)) * '0' + num
        ss = '?Dt' + hex(8 + 1 + core_num).upper()[2:] + 'E000000' + offstr + numstr
    else:
        # Dt Write Command
        pass
    return ss




##############################################################################
# class Name:  MipsReport
# Description: To save all MIPS data for model. It prints out the MIPS report.
#              Or generate MIPS report in HTML.
##############################################################################
class MipsReport:
    NAMEOFFSET = 36
    NAMEMAXLEN = 32
    MIPSMAXLEN = 20
    HTMLNAME = 'MIPSReport.html'
    ##########################################################################
    # function name: __init__
    # Description:   Constructor of MipsReport.
    # Argument:      None
    # Return:        None
    ##########################################################################
    def __init__(self):
        self.num = 0
        self.name = []
        self.p_cycle = []
        self.a_cycle = []
        self.p_mips = []
        self.a_mips = []
        self.reportDict = {}

    ##########################################################################
    # function name: AddSingleModelToReport
    # Description:   It is for single model. By calling this, it adds MIPS
    #                measurements along with the name of each profiling, like
    #                step0, or feature name.
    # Argument:      name:           name of the profiling, like step1, step2,
    #                                or feature name
    #                peak_cycle:     peak value in cycle
    #                average_cycle:  average value in cycle
    #                peak_mips:      available in DSP memory, but obseleted
    #                average_mips:   average in MIPS
    # Return:        None
    ##########################################################################
    def AddSingleModelToReport(self, name, peak_cycle, average_cycle, peak_mips, average_mips):
        self.name.append(name)
        self.p_cycle.append(peak_cycle)
        self.a_cycle.append(average_cycle)
        self.p_mips.append(peak_mips)
        self.a_mips.append(average_mips)
        self.num = self.num + 1

    ##########################################################################
    # function name: ClearSingleModelReport
    # Description:   Clear all MIPS data stored for the Model.
    # Argument:      None
    # Return:        None
    ##########################################################################
    def ClearSingleModelReport(self):
        self.num = 0
        self.name = []
        self.p_cycle = []
        self.a_cycle = []
        self.p_mips = []
        self.a_mips = []

    ##########################################################################
    # function name: PrintSingleModelReport
    # Description:   Print MIPS report in command line window.
    # Argument:      model_name:     model name
    # Return:        None
    ##########################################################################
    def PrintSingleModelReport(self, model_name):
        if self.num != 0:
            print('*******************************************************************************************************************')
            print('MIPS Report for ' + model_name)
            print('*******************************************************************************************************************')
            print('-------------------------------------------------------------------------------------------------------------------')
            print(' Name                               Peak Cycle          Average Cycle       Peak MIPS           Average MIPS')
            print('-------------------------------------------------------------------------------------------------------------------')
            for i in range(self.num):
                ss=[]
                tmp = self.name[i][:MipsReport.NAMEMAXLEN] + '...' if len(self.name[i]) > MipsReport.NAMEMAXLEN else self.name[i]
                ss.append(tmp)
                ss.append(' ' * (MipsReport.NAMEOFFSET - len(tmp)))
                ss.append(self.p_cycle[i] + ' ' * (MipsReport.MIPSMAXLEN - len(self.p_cycle[i])))
                ss.append(self.a_cycle[i] + ' ' * (MipsReport.MIPSMAXLEN - len(self.a_cycle[i])))
                ss.append(self.p_mips[i] + ' ' * (MipsReport.MIPSMAXLEN - len(self.p_mips[i])))
                ss.append(self.a_mips[i] + ' ' * (MipsReport.MIPSMAXLEN - len(self.a_mips[i])))
                print(''.join(ss))
            print('\r\n')
        else:
            print('There is no Report, please run MIPS measurement first.')

    ##########################################################################
    # function name: GenerateMultiModelReportinHTML
    # Description:   Generate MIPS report in HTML. All MIPS data is stored in
    #                self.reportDict dictionary.
    # Argument:      file_path:      file path to save HTML report
    # Return:        None
    ##########################################################################
    def GenerateMultiModelReportinHTML(self, file_path=None):
        # Bar Chart Scheme
        bc_scheme = ['linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                    'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)']
        fpath = os.path.join(file_path, MipsReport.HTMLNAME)
        align = '    '
        cnt = 1
        with open(fpath, 'w') as filehandle:
            ss = []
            # HTML Start
            ss.append('<html>\n')
            ss.append(align * cnt + '<head>\n')
            cnt += 1
            ss.append(align * cnt + '<title>' + 'MIPS Usage' + '</title>\n')
            ss.append(align * cnt + '<style>\n')
            cnt += 1
            ss.append(align * cnt + 'body {background-color: white;}\n')
            ss.append(align * cnt + 'li {font-weight: bold;}\n')
            ss.append(align * cnt + 'table {border-collapse: collapse; width: 50%;}\n')
            ss.append(align * cnt + 'th, td {border: 1px solid gray;}')
            ss.append(align * cnt + '* {box-sizing:border-box}')
            ss.append(align * cnt + '.container {width: 50%;}')
            cnt -=1
            ss.append(align * cnt + '</style>\n')
            cnt -= 1
            ss.append(align * cnt + '<body>\n')
            cnt += 1
            ss.append(align * cnt + '<h1>' + 'MIPS Usage' + '</h1>\n')
            ss.append(align * cnt + '<b>' + '' + '</b>\n')
            ss.append(align * cnt + '<ul>\n')
            filehandle.write(''.join(ss))
            # MIPS Details
            for k, v in self.reportDict.items():
                # MIPS Details
                total_mips = 0.0
                ss = []
                ss.append('&nbsp;')
                ss.append(align * cnt + '<li>' + k + ' MIPS Details' + '\n')
                cnt += 1
                ss.append(align * cnt + '<ul>\n')
                cnt += 1
                ss.append(align * cnt + '<li style="color:Blue; list-style-type:square;">\n')
                cnt += 1
                ss.append(align * cnt + '<table>\n')
                cnt += 1
                ss.append(align * cnt + '<tr style="background-color: yellow">\n')
                ss.append(align * cnt + '  <td><b>' + 'Name' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Peak Cycles' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Average Cycles' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Peak MIPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Average MIPS' + '</b></td>\n')
                ss.append(align * cnt + '</tr>\n')
                for n, m in v.items():
                    ss.append(align * cnt + '<tr">\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + n + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + m[0] + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + m[1] + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + m[2] + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + m[3] + '</font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
                    total_mips = total_mips + float(m[3]) if re.search('^step', n) else total_mips
                ss.append(align * cnt + '<tr style="background-image: linear-gradient(to right, #a8ff78, #78ffd6);">\n')
                ss.append(align * cnt + '<td colspan="4"><b>' + 'Total MIPS:' + ' <span style="font-size: 14;">' + '{:.2f}'.format(total_mips) + '</span></b></td>\n')
                ss.append(align * cnt + '</tr>\n')

                cnt -= 1
                ss.append(align * cnt + '</table>\n')
                cnt -= 1
                ss.append(align * cnt + '</li>\n')
                cnt -= 1
                ss.append(align * cnt + '</ul>\n')
                cnt -= 1
                ss.append(align * cnt + '</li>\n')
                filehandle.write(''.join(ss))
            # Bar Chart
            ss = []
            ss.append('&nbsp;\n')
            ss.append(align * cnt + '<li>' + 'MIPS Bar Chart' + '\n')
            cnt += 1
            ss.append(align * cnt + '<ul>\n')
            cnt += 1
            # MIPS Block Bar
            for k, v in self.reportDict.items():
                ss.append(align * cnt + '<li style="color:Black; list-style-type:square;"><i>' + k + '</i>\n')
                cnt += 1
                for n, m in v.items():
                    percent = (float(m[3]) / 1000 ) * 100
                    ss.append(align * cnt + '<table border="0" cellspacing="0" cellpadding="0" style="width:50%;">\n')
                    ss.append(align * cnt + '<tr style="height:4px;">\n')
                    ss.append(align * cnt + '<td colspan="2"><font size="2" face="Arial"><b>' + n + ': ' + '{:.2f}'.format(percent) + '%' + '</b></font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
                    ss.append(align * cnt + '<tr style="height:6px;">\n')
                    ss.append(align * cnt + '<td style="width:' + str(int(percent)) + '%;background-color:#003b67;"></td>\n')
                    ss.append(align * cnt + '<td style="width:' + str(100 - int(percent)) + '%;background-color: #4cff00;"></td>\n')
                    ss.append('</tr>\n')
                    ss.append('</table>\n')
            ss.append(align * cnt + '</ul>\n')
            cnt -= 1
            ss.append(align * cnt + '</li>\n')
            cnt -= 1
            filehandle.write(''.join(ss))
            # HTML End
            ss = []
            cnt -= 1
            ss.append(align * cnt + '</ul>\n')
            cnt -= 1
            ss.append('&nbsp;&nbsp\n')
            ss.append('<b>Generated from OpenJADE JScope, ' + date.today().strftime('%m/%d/%y') + '.</b>\n')
            ss.append(align * cnt + '</body>\n')
            ss.append(align * cnt + '</head>\n')
            ss.append('</html>\n')
            filehandle.write(''.join(ss))

    def GenerateReportInHtmlQualcomm(self, profDict=None, file_path=None):
        # Bar Chart Scheme
        bc_scheme = ['linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                     'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                    'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)']
        fpath = os.path.join(file_path, MipsReport.HTMLNAME)
        if os.path.exists(fpath):
            os.remove(fpath)
        align = '    '
        cnt = 1
        with open(fpath, 'w') as filehandle:
            ss = []
            # HTML Start
            ss.append('<html>\n')
            ss.append(align * cnt + '<head>\n')
            cnt += 1
            ss.append(align * cnt + '<title>' + 'MIPS Usage' + '</title>\n')
            ss.append(align * cnt + '<style>\n')
            cnt += 1
            ss.append(align * cnt + 'body {background-color: white;}\n')
            ss.append(align * cnt + 'li {font-weight: bold;}\n')
            ss.append(align * cnt + 'table {border-collapse: collapse; width: 50%;}\n')
            ss.append(align * cnt + 'th, td {border: 1px solid gray;}')
            ss.append(align * cnt + '* {box-sizing:border-box}')
            ss.append(align * cnt + '.container {width: 50%;}')
            cnt -=1
            ss.append(align * cnt + '</style>\n')
            cnt -= 1
            ss.append(align * cnt + '<body>\n')
            cnt += 1
            ss.append(align * cnt + '<h1>' + 'MIPS Usage' + '</h1>\n')
            ss.append(align * cnt + '<b>' + '' + '</b>\n')
            ss.append(align * cnt + '<ul>\n')
            filehandle.write(''.join(ss))
            # MIPS Details
            for k, v in profDict.items():
                # MIPS Details
                total_mips = 0.0
                ss = []
                ss.append('&nbsp;')
                ss.append(align * cnt + '<li>' + k + ' MIPS Details' + '\n')
                cnt += 1
                ss.append(align * cnt + '<ul>\n')
                cnt += 1
                ss.append(align * cnt + '<li style="color:Blue; list-style-type:square;">\n')
                cnt += 1
                ss.append(align * cnt + '<table style="width: 70%">\n')
                cnt += 1
                ss.append(align * cnt + '<tr style="background-color: yellow">\n')
                ss.append(align * cnt + '  <td><b>' + 'Name' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Min MPPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Avg MPPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Max MPPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Min MCPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Avg MCPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Max MCPS' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Min CPP' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Avg CPP' + '</b></td>\n')
                ss.append(align * cnt + '  <td><b>' + 'Max CPP' + '</b></td>\n')
                ss.append(align * cnt + '</tr>\n')
                for n, m in v.items():
                    ss.append(align * cnt + '<tr">\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + n + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[0]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[1]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[2]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[3]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[4]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[5]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[6]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[7]) + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + '{:.2f}'.format(m[8]) + '</font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
                    total_mips = total_mips + float(m[4]) if re.search('^step', n) else total_mips
                ss.append(align * cnt + '<tr style="background-image: linear-gradient(to right, #a8ff78, #78ffd6);">\n')
                ss.append(align * cnt + '<td colspan="4"><b>' + 'Total MCPS:' + ' <span style="font-size: 14;">' + '{:.2f}'.format(total_mips) + '</span></b></td>\n')
                ss.append(align * cnt + '</tr>\n')
                cnt -= 1
                ss.append(align * cnt + '</table>\n')
                cnt -= 1
                ss.append(align * cnt + '</li>\n')
                cnt -= 1
                ss.append(align * cnt + '</ul>\n')
                cnt -= 1
                ss.append(align * cnt + '</li>\n')
                filehandle.write(''.join(ss))
            # Bar Chart
            ss = []
            ss.append('&nbsp;\n')
            filehandle.write(''.join(ss))
            # HTML End
            ss = []
            cnt -= 1
            ss.append(align * cnt + '</ul>\n')
            cnt -= 1
            ss.append('&nbsp;&nbsp\n')
            ss.append('<b>Generated from OpenJADE JScope, ' + date.today().strftime('%m/%d/%y') + '.</b>\n')
            ss.append(align * cnt + '</body>\n')
            ss.append(align * cnt + '</head>\n')
            ss.append('</html>\n')
            filehandle.write(''.join(ss))




##############################################################################
# class Name:  JProfileClient
# Description: Profiling Client base class.
##############################################################################
class JProfileClient:
    DW_CMD = 1
    DT_CMD = 2
    OFFSET = 36
    # Board info for profiling, temperorily defined here, will be moved to upper level.
    PROFILE_DICT = {
                        'melville_board': [['Model_1_1', 'Model_1_2'], ['profile_buffer0.bin', 'profile_buffer1.bin']]
                   }
    ##########################################################################
    # function name: __init__
    # Description:   Constructor
    # Argument:      None
    # Return:        None
    ##########################################################################
    def __init__(self):
        pass

    ##########################################################################
    # function name: FormatProfilingMips
    # Description:   Return base address of profiling data structure in DSP,
    #                based on *.map.xml file.
    # Argument:      mips:           MIPS
    # Return:        MIPS in string format
    ##########################################################################
    def FormatProfilingMips(self, mips):
        return '{:.2f}'.format(mips)

    ##########################################################################
    # function name: FindProfilingDataBaseAddr
    # Description:   Return base address of profiling data structure in DSP,
    #                based on *.map.xml file.
    # Argument:      matlab_model_name:    model name
    #                memory_xml_path:      path to map file
    # Return:        base address
    ##########################################################################
    def FindProfilingDataBaseAddr(self, matlab_model_name=None, memory_xml_path=None):
        baseaddr = None
        if os.path.exists(memory_xml_path):
            with open(memory_xml_path, 'r') as filehandle:
                num_of_lines = filehandle.readlines()
            ss = '<SYMBOL name=' + "'" + matlab_model_name + '_profiledata.' + "'"
            for item in num_of_lines:
                if ss in item:
                    tpL = item.split()
                    l = tpL[2].find("'")
                    r = tpL[2].find("'", l + 1)
                    baseaddr = tpL[2][l + 1 : r]
                    break
        else:
            print('DSP Memory XML file cannot be found.')
        return baseaddr

    ##########################################################################
    # function name: FindProfilingNameNRate
    # Description:   Read callrate config file and generate a list including
    #                all callrate and its name.
    # Argument:      profile_cfg_path:      file path to callrate config file
    # Return:        List of callrate plus its name
    ##########################################################################
    def FindProfilingNameNRate(self, profile_cfg_path=None):
        nrList = []
        if os.path.exists(profile_cfg_path):
            with open(profile_cfg_path, 'r') as filehandle:
                num_of_lines = filehandle.readlines()
            nrList = [[x[:x.find(':')], x[x.find(':') + 1 : -1]] for x in num_of_lines]
        else:
            print('Profiling Configure file cannot be found.')
        return nrList





##############################################################################
# class Name:  JProfilePluginClient
# Description: Plugin client used to integrated into the third-party platform.
#              It only generates Dw or Dt commands, and reads Dw or Dt response
#              to generate MIPS report.
##############################################################################
class JProfilePluginClient(JProfileClient):
    ##########################################################################
    # function name: __init__
    # Description:   Constructor
    # Argument:      report_path:      path to HTML report
    # Return:        None
    ##########################################################################
    def __init__(self, report_path=None):
        self.report_path = report_path
        # Format {matlab_model_name1: [core_num, memory_xml_path, profile_cfg_path, [[funcname1, callrate1], [funcname2, callrate2], ...], base_addr], matlab_model_name2:[...]}
        self.mipsdata = {}
        self.mipsReport = MipsReport()

    ##########################################################################
    # function name: AddMipsObjByModelName
    # Description:   Add model for MIPS profiling, with core num = 0, 1, etc,
    #                model name, path of DSP map xml file, callrate config file
    #                specified.
    # Argument:      matlab_model_name:    model name
    #                corenum:              core id, should be 0, 1, 2, etc
    #                memory_xml_path:      path to DSP map xml file
    #                profile_cfg_path:     path to callrate config file
    # Return:        None
    ##########################################################################
    def AddMipsObjByModelName(self, matlab_model_name=None, corenum=0, memory_xml_path=None, profile_cfg_path=None):
        self.mipsdata[matlab_model_name] = [corenum, memory_xml_path, profile_cfg_path]
        if profile_cfg_path is not None:
            self.mipsdata[matlab_model_name].append(self.FindProfilingNameNRate(profile_cfg_path))
        if memory_xml_path is not None:
            self.mipsdata[matlab_model_name].append(self.FindProfilingDataBaseAddr(matlab_model_name, memory_xml_path))
        else:
            self.mipsdata[matlab_model_name].append(0)
        self.mipsReport.reportDict[matlab_model_name] = {}

    ##########################################################################
    # function name: GetNumberofProfilingItems
    # Description:   Get number of profiling items for each model.
    # Argument:      matlab_model_name:    model name
    # Return:        None
    ##########################################################################
    def GetNumberofProfilingItems(self, matlab_model_name):
        return len(self.mipsdata[matlab_model_name][3]) if matlab_model_name in self.mipsdata else 0

    ##########################################################################
    # function name: GetDwCmdByObjIndex
    # Description:   It returns Dw command for each profiling item in model.
    # Argument:      matlab_model_name:    model name
    #                index:                index of the profilnig item
    # Return:        None
    ##########################################################################
    def GetDwCmdByObjIndex(self, matlab_model_name, index):
        ss = None
        mdata = self.mipsdata[matlab_model_name]
        if mdata[4] != None:
            bsaddr_int = int(mdata[4], 16)
            bsaddr = str(hex(bsaddr_int + index * JProfileClient.OFFSET))[2:].zfill(8)
            if index >= 0 and index < len(mdata[3]):
                ss = Proto_DwRequest(0, 3, bsaddr, mdata[0])
            else:
                print('The index of profiling item is out of range.')
        else:
            print('The Profiling data Base Address cannot be found.')
        return ss

    ##########################################################################
    # function name: GetDtCmdByObjIndex
    # Description:   It returns Dt command for each profiling item in model.
    # Argument:      matlab_model_name:    model name
    #                index:                index of the profilnig item
    # Return:        None
    ##########################################################################
    def GetDtCmdByObjIndex(self, matlab_model_name, index):
        ss = None
        mdata = self.mipsdata[matlab_model_name]
        #bsaddr_int = int(mdata[4], 16)
        #bsaddr = str(hex(bsaddr_int + index * JProfileClient.OFFSET))[2:].zfill(8)
        if index >= 0 and index < len(mdata[3]):
            ss = Proto_DtRequest(0, 2, mdata[0], 9*index)
        else:
            print('The index of profiling item is out of range.')
        return ss

    ##########################################################################
    # function name: AddDwResponseToReport
    # Description:   It accepts Dw response and extract MIPS data
    # Argument:      resp:                 response payload from DSP
    #                matlab_model_name:    model name
    #                index:                index of the profilnig item
    # Return:        None
    ##########################################################################
    def AddDwResponseToReport(self, resp=None, matlab_model_name=None, index=0):
        if matlab_model_name in self.mipsdata:
            tmpL = resp[0].decode().split()
            if len(tmpL) == 5 and tmpL[0] == 'DSP':
                p_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                tmpL = resp[1].decode().split()
                a_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                pname = self.mipsdata[matlab_model_name][3][index][0]
                pcoff = self.mipsdata[matlab_model_name][3][index][1]
                pcoff = float(pcoff)
                a_mips = a_cycle * pcoff
                p_mips = p_cycle * pcoff
                self.mipsReport.reportDict[matlab_model_name][pname] = [self.FormatProfilingMips(p_cycle), self.FormatProfilingMips(a_cycle), self.FormatProfilingMips(p_mips), self.FormatProfilingMips(a_mips)]
            else:
                print('ERROR: Dw Response is in wrong format.')
        else:
            print('The model name is Invalid.')

    ##########################################################################
    # function name: AddDtResponseToReport
    # Description:   It accepts Dt response and extract MIPS data
    # Argument:      resp:                 response payload from DSP
    #                matlab_model_name:    model name
    #                index:                index of the profilnig item
    # Return:        None
    ##########################################################################
    def AddDtResponseToReport(self, resp=None, matlab_model_name=None, index=0):
        if matlab_model_name in self.mipsdata:
            p_cycle = bytearray.fromhex(resp[7 : 7 + 8].decode())
            p_cycle = struct.unpack('>f',p_cycle)
            p_cycle = p_cycle[0]
            a_cycle = bytearray.fromhex(resp[7+8 : 7+2*8].decode())
            a_cycle = struct.unpack('>f',a_cycle)
            a_cycle = a_cycle[0]
            pname = self.mipsdata[matlab_model_name][3][index][0]
            pcoff = self.mipsdata[matlab_model_name][3][index][1]
            pcoff = float(pcoff)
            a_mips = a_cycle * pcoff
            p_mips = p_cycle * pcoff
            self.mipsReport.reportDict[matlab_model_name][pname] = [self.FormatProfilingMips(p_cycle), self.FormatProfilingMips(a_cycle), self.FormatProfilingMips(p_mips), self.FormatProfilingMips(a_mips)]
        else:
            print('The Model name is invalid.')

    ##########################################################################
    # function name: PrintMipsReportByModelName
    # Description:   Print out the MIPS report in command line for each model.
    # Argument:      matlab_model_name:    model name
    # Return:        None
    ##########################################################################
    def PrintMipsReportByModelName(self, matlab_model_name=None):
        if matlab_model_name in self.mipsReport.reportDict:
            self.mipsReport.ClearSingleModelReport()
            for k, v in self.mipsReport.reportDict[matlab_model_name].items():
                self.mipsReport.AddSingleModelToReport(k, v[0], v[1], v[2], v[3])
            self.mipsReport.PrintSingleModelReport(matlab_model_name)
        else:
            print('The Model name is invalid')

    ##########################################################################
    # function name: GenerateMipsReportInHTML
    # Description:   Generate MIPS report in HTML.
    # Argument:      None
    # Return:        None
    ##########################################################################
    def GenerateMipsReportInHTML(self):
        self.mipsReport.GenerateMultiModelReportinHTML(self.report_path)






##############################################################################
# class Name:  JProfileSerialClient
# Description: Profiling Client running on PC to communicate with Microcontroller
#              via UART. It parses DSP memory XML file to get baseaddr of profiling
#              data structure in DSP, and send Dw command to read data out.
##############################################################################
class JProfileSerialClient(JProfileClient):
    JPCLIENT_INIT1 = 0
    JPCLIENT_INIT2 = 1
    JPCLIENT_WORK1 = 2
    ##########################################################################
    # function name: __init__
    # Description:   Constructor
    # Argument:      memory_xml_path:      path to DSP map xml file
    #                profile_cfg_path:     path to callrate config file
    # Return:        None
    ##########################################################################
    def __init__(self, memory_xml_path=None, profile_cfg_path=None):
        self.state_machine = JProfileSerialClient.JPCLIENT_INIT1
        self.baseaddr = None
        self.nrList = []
        self.mipsReport = MipsReport()
        self.mem_xmlpath = memory_xml_path
        self.profile_cfgpath = profile_cfg_path

    ##########################################################################
    # function name: FindProfilingStuff
    # Description:   Parse DSP map file and callrate config file to get base
    #                address of profiling data structure in DSP, and all callrate
    #                and its names.
    # Argument:      matlab_model_name:    model name
    # Return:        Success or not:       True, or False
    ##########################################################################
    def FindProfilingStuff(self, matlab_model_name=None):
        vl = False
        self.modelname = matlab_model_name
        if self.state_machine == JProfileSerialClient.JPCLIENT_INIT1:
            self.baseaddr = self.FindProfilingDataBaseAddr(matlab_model_name, self.mem_xmlpath)
            if self.baseaddr != None:
                self.nrList = self.FindProfilingNameNRate(self.profile_cfgpath)
                if bool(self.nrList):
                    self.state_machine = JProfileSerialClient.JPCLIENT_WORK1
                    vl = True
        return vl

    ##########################################################################
    # function name: GetProfilingFromUart
    # Description:   Send Dw command to query MIPS measurements, and save it in
    #                report.
    # Argument:      comport:          COM Port
    #                core_num:         core id, should be 0, 1, etc.
    # Return:        None
    ##########################################################################
    def GetProfilingFromUart(self, comport, core_num):
        if self.state_machine == JProfileSerialClient.JPCLIENT_WORK1:
            self.mipsList = []
            self.core_num = core_num
            comhandler = Serial_Open(comport, 115200)
            Serial_Flush(comhandler)
            if comhandler != None:
                bsaddr_int = int(self.baseaddr, 16)
                counter = 0
                sim_time = 0.0
                for item in self.nrList:
                    bsaddr = '0x' + str(hex(bsaddr_int + counter * JProfileClient.OFFSET))[2:].zfill(8)
                    dw_request = Proto_DwRequest(0, 3, bsaddr[2:], self.core_num)
                    Serial_Write(comhandler, dw_request)
                    time.sleep(0.2)
                    # Read Echo
                    msg1 = Serial_Read(comhandler)
                    # Read Profiling Peak Value
                    msg2 = Serial_Read(comhandler)
                    # Read Profiling Average Value
                    msg3 = Serial_Read(comhandler)
                    # Read Profiling counter
                    msg4 = Serial_Read(comhandler)
                    tmpL = msg2.decode().split()
                    # Read one more time
                    msg5 = Serial_Read(comhandler)
                    tmpL = msg2.decode().split()
                    if len(tmpL) == 5 and tmpL[0] == 'DSP':
                        if item[0].startswith('step') or item[0].startswith('Step'):
                            # STEP function
                            p_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            p_mips = p_cycle * float(item[1])
                            tmpL = msg3.decode().split()
                            a_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            a_mips = a_cycle * float(item[1])
                            tmpL = msg4.decode().split()
                            prof_cnt = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            if sim_time == 0.0:
                                sim_time = (float(prof_cnt) / (float(item[1]) * 1000000.0))
                        else:
                            # Model or Platform function
                            p_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            #p_mips = p_cycle * float(item[1])
                            tmpL = msg3.decode().split()
                            a_cycle = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            #a_mips = a_cycle * float(item[1])
                            tmpL = msg4.decode().split()
                            prof_cnt = struct.unpack('!f', bytes.fromhex(tmpL[4]))[0]
                            p_mips = ((p_cycle * prof_cnt) / sim_time) / 1000000
                            a_mips = ((a_cycle * prof_cnt) / sim_time) / 1000000
                        self.mipsReport.AddSingleModelToReport(item[0], self.FormatProfilingMips(p_cycle), self.FormatProfilingMips(a_cycle), self.FormatProfilingMips(p_mips), self.FormatProfilingMips(a_mips))
                        counter = counter + 1
                    else:
                        print('ERROR: Cannot receive MIPS measurement from DSP via UART.')
                        print('Error Message: ' + msg2.decode())
            Serial_Close(comhandler)

    ##########################################################################
    # function name: PrintOutMipsReport
    # Description:   Print MIPS report for single model in command line.
    # Argument:      None
    # Return:        None
    ##########################################################################
    def PrintOutMipsReport(self):
        self.mipsReport.PrintSingleModelReport(self.modelname)






##############################################################################
# class Name:  JProfileEmulatorClient
# Description: Profiling Client running on PC to Load image through Emulator.
#              And run executables, read profiling data from memory.
##############################################################################
class JProfileEmulatorClient(JProfileClient):
    ##########################################################################
    # function name: __init__
    # Description:   Read binary files generated from emulator, the binary file
    #                is a copy of DSP memory that holds profiling data. And by
    #                parsing the file, MIPS measurements are saved in reportDict.
    # Argument:      board_type:      board type, like 'melville_board'
    #                callrate_path:   path to callrate config file
    #                profile_path:    path to binary file generated by Emulator
    # Return:        None
    ##########################################################################
    def __init__(self, board_type=None, callrate_path=None, profile_path=None):
        self.boardtype = board_type
        self.callrate_path = callrate_path
        self.profile_path = profile_path
        self.mipsReport = MipsReport()

    ##########################################################################
    # function name: ParseProfiling
    # Description:   Read binary files generated from emulator, the binary file
    #                is a copy of DSP memory that holds profiling data. And by
    #                parsing the file, MIPS measurements are saved in reportDict.
    # Argument:      None
    # Return:        None
    ##########################################################################
    def ParseProfiling(self):
        if self.boardtype in JProfileClient.PROFILE_DICT.keys():
            modelnameL = JProfileClient.PROFILE_DICT[self.boardtype][0]
            profilenameL = JProfileClient.PROFILE_DICT[self.boardtype][1]
            for index in range(len(modelnameL)):
                model_nrL = self.FindProfilingNameNRate(os.path.join(self.callrate_path, modelnameL[index] + '_lookup.txt'))
                if bool(model_nrL):
                    profile_dataF = os.path.join(self.profile_path, profilenameL[index])
                    if os.path.exists(profile_dataF):
                        self.mipsReport.reportDict[modelnameL[index]] = {}
                        with open(profile_dataF, 'rb') as filehandle:
                            fdata = filehandle.read().hex()
                            hexdata = [fdata[index : index + 2] for index in range(0, len(fdata), 2)]
                            hdata = [''.join(hexdata[index : index + 4][::-1]) for index in range(0, len(hexdata), 4)]
                            count = 0
                            # The simulation time is calculated by step function
                            sim_time = 0.0
                            for item in model_nrL:
                                if item[0].startswith('step') or item[0].startswith('Step'):
                                    # STEP function
                                    p_cycle = struct.unpack('!f', bytes.fromhex(hdata[count]))[0]
                                    p_mips = p_cycle * float(item[1])
                                    a_cycle = struct.unpack('!f', bytes.fromhex(hdata[count + 1]))[0]
                                    a_mips = a_cycle * float(item[1])
                                    prof_cnt = struct.unpack('!f', bytes.fromhex(hdata[count + 2]))[0]
                                    if sim_time == 0.0:
                                        sim_time = (float(prof_cnt) / (float(item[1]) * 1000000.0))
                                else:
                                    # Model or Platform function
                                    p_cycle = struct.unpack('!f', bytes.fromhex(hdata[count]))[0]
                                    #p_mips = p_cycle * float(item[1])
                                    a_cycle = struct.unpack('!f', bytes.fromhex(hdata[count + 1]))[0]
                                    #a_mips = a_cycle * float(item[1])
                                    prof_cnt = struct.unpack('!f', bytes.fromhex(hdata[count + 2]))[0]
                                    p_mips = ((p_cycle * prof_cnt) / sim_time) / 1000000
                                    a_mips = ((a_cycle * prof_cnt) / sim_time) / 1000000
                                self.mipsReport.reportDict[modelnameL[index]][item[0]] = [self.FormatProfilingMips(p_cycle), self.FormatProfilingMips(a_cycle), self.FormatProfilingMips(p_mips), self.FormatProfilingMips(a_mips)]
                                count = count + int(JProfileClient.OFFSET / 4)
                    else:
                        print('ERROR: Simulation bin file cannot be found: ' + profilenameL[index])
                else:
                    print('ERROR: Callrate file cannot be found for Model: ' + modelnameL[index])
        else:
            print('ERROR: Unsupported board type: ' + self.boardtype)

    ##########################################################################
    # function name: GenerateMipsReportInHTML
    # Description:   Generate report in HTML.
    # Argument:      file_path:     file path to HTML report
    # Return:        None
    ##########################################################################
    def GenerateMipsReportInHTML(self, file_path):
        self.mipsReport.GenerateMultiModelReportinHTML(file_path)




#################################################################################################
# Unit Test for profileClient command line
#################################################################################################
# python profileClient.py serial COM3 0 Model_1_1 "./spc1.map.xml" "./JAutogen/Model_1_1_lookup.txt"
# python profileClient.py serial COM3 0 Model_1_2 "./spc2.map.xml" "./JAutogen/Model_1_2_lookup.txt"
# python profileClient.py emulator melville_board "./" "../../project/work/"
if __name__ == '__main__':
    n = len(sys.argv)
    if n > 2:
        if sys.argv[1] == 'serial':
            if n >= 7:
                if os.path.exists(sys.argv[5]) and os.path.exists(sys.argv[6]):
                    jclient = JProfileSerialClient(sys.argv[5], sys.argv[6])
                    # Find base address of Profiling data in DSP memory, all profiling name and callrates
                    jclient.FindProfilingStuff(sys.argv[4])
                    # Get MIPS over UART
                    jclient.GetProfilingFromUart(sys.argv[2], int(sys.argv[3]))
                    # Print out MIPS Report
                    jclient.PrintOutMipsReport()
                else:
                    print('The Memory map file or callrate file cannot be found.')
            else:
                print('ERROR: More arguments needed for profiling based on serial port.')
        elif sys.argv[1] == 'emulator':
            if n >= 4:
                if os.path.exists(sys.argv[3]) and os.path.exists(sys.argv[4]):
                    jclient = JProfileEmulatorClient(sys.argv[2], sys.argv[3], sys.argv[4])
                    jclient.ParseProfiling()
                    # Generate Report in HTML
                    jclient.GenerateMipsReportInHTML(sys.argv[4])
                else:
                    print('The profiling bin file or callrate file cannot be found.')
            else:
                print('ERROR: More Argument needed for profiling based on emulator')
        else:
            print('ERROR: First argument should be "serial" or "emultor".')
    else:
        print('ERROR: Needs Argument: profileClient.py.')








# The following is a testcase for JProfilePluginClient Dw command on Melville.
'''
comhandler = Serial_Open('COM3', 115200)
Serial_Flush(comhandler)

client = JProfilePluginClient(os.getcwd())
mem_xml = 'C:\svn\go_melville_latest\project\spc\spc1\Release\spc.map.xml'
profile_txt = 'C:\svn\go_melville_latest\\audio\simulink\Model_1_1_lookup.txt'
client.AddMipsObjByModelName('Model_1_1', 0, mem_xml, profile_txt)
num = client.GetNumberofProfilingItems('Model_1_1')
for i in range(num):
    cmd = client.GetDwCmdByObjIndex('Model_1_1', i)
    Serial_Write(comhandler, cmd)
    # Read Echo
    msg1 = Serial_Read(comhandler)
    # Read Profiling Peak Value
    msg2 = Serial_Read(comhandler)
    # Read Profiling Average Value
    msg3 = Serial_Read(comhandler)
    # Read one more time
    msg4 = Serial_Read(comhandler)
    client.AddDwResponseToReport([msg2, msg3], 'Model_1_1', i)

time.sleep(2)

mem_xml = 'C:\svn\go_melville_latest\project\spc\spc2\Release\spc.map.xml'
profile_txt = 'C:\svn\go_melville_latest\\audio\simulink\Model_1_2_lookup.txt'
client.AddMipsObjByModelName('Model_1_2', 1, mem_xml, profile_txt)
num = client.GetNumberofProfilingItems('Model_1_2')
for i in range(num):
    cmd = client.GetDwCmdByObjIndex('Model_1_2', i)
    Serial_Write(comhandler, cmd)
    print('####' + str(cmd))
    # Read Echo
    msg1 = Serial_Read(comhandler)
    # Read Profiling Peak Value
    msg2 = Serial_Read(comhandler)
    # Read Profiling Average Value
    msg3 = Serial_Read(comhandler)
    # Read one more time
    msg4 = Serial_Read(comhandler)
    print('>>>>' + str(msg2) + '::' + str(msg3))
    client.AddDwResponseToReport([msg2, msg3], 'Model_1_2', i)

client.PrintMipsReportByModelName('Model_1_1')
client.PrintMipsReportByModelName('Model_1_2')
'''



