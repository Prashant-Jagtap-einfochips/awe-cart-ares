import os
import sys

import xml.etree.ElementTree as ElementTree
from datetime import date





class memDetail:
    def __init__(self, memSection, length):
        self.memSection = memSection
        self.memLength = length
        self.memtype = []
        self.memlen = []
        self.memDict = {}
        self.numContributor = 0

    def updateMemSize(self, length):
        self.memLength += length

    def addMemType(self, memType, length):
        self.memtype.append(memType)
        self.memlen.append(length)
        self.memDict[memType] = []

    def addMemUsage(self, memType, usageType, length):
        if not memType in self.memDict.keys():
            self.memDict[memType] = []
        self.memDict[memType].append(usageType)
        self.memDict[memType].append(length)
        self.numContributor += 1



def GenerateHtmlMemoryUsage(xmlfile=None):
    mem_cacheInfo = ['L1', 'L2']
    # Format: {Memory Block1 :{Memory section1: length, Memory section2: length}, Memory Block2 : {...}, ...}
    mem_layout = {}
    # Format: Dict of memDetail Object
    mem_detail = {}
    # Format: {'platform': 1888, 'OpenJADE': 3428, ...}
    mem_text = {}
    # Format: {'platform': 1888, 'OpenJADE': 3428, ...}
    mem_data = {}
    total_textsize = 0
    total_datasize = 0
    total_memblock = {}
    total_memblockB = {}
    percent_text = {}
    percent_data = {}
    percent_cache = {}
    # Bar Chart Scheme
    bc_scheme = ['linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                 'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                 'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                 'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)',
                 'linear-gradient(to right, #a8ff78, #78ffd6)', 'linear-gradient(to right, #a8ff78, #78ffd6)']
    try:
        # Extract all Memory info from XML file
        tree_mem = ElementTree.parse(xmlfile)
        root_mem = tree_mem.getroot()
        for memBlock in root_mem.findall('MEMORY_BLOCK'):
            bId = memBlock.get('id')
            if not bId in mem_layout.keys():
                mem_layout[bId] = {}
                total_memblockB[bId] = 0
            for memSection in memBlock.findall('MEMORY_SECTION'):
                sId = memSection.get('id')
                length = memSection.get('length')
                if not sId in memSection.keys():
                    mem_layout[bId][sId] = length
                for mem in memSection.findall('MEMORY'):
                    dId = mem.get('id')
                    dSize = mem.get('total')
                    total_memblockB[bId] += int(dSize)
                    # Search MEMORY_SECTION
                    memDtl = None
                    notfound = True
                    if bId in mem_detail.keys():
                        for item in mem_detail[bId]:
                            if sId == item.memSection:
                                memDtl = item
                                memDtl.updateMemSize(int(dSize))
                                notfound = False
                                break
                    if memDtl == None:
                        memDtl = memDetail(sId, int(dSize))  
                    memDtl.addMemType(dId, dSize)
                    for usage in mem.findall('CONTRIBUTOR'):
                        uId = usage.get('id')
                        uSize = usage.get('size')
                        memDtl.addMemUsage(dId, uId, uSize)
                        # Summerize data/text section for each software module
                        if dId == 'data':
                            mem_data[uId] = mem_data[uId] + int(uSize) if uId in mem_data.keys() else int(uSize)
                            total_datasize += int(uSize)
                        if dId == 'text':
                            mem_text[uId] = mem_text[uId] + int(uSize) if uId in mem_text.keys() else int(uSize)
                            total_textsize += int(uSize)
                    # Special case for Stack, because it does not include 'CONTRIBUTOR'
                    if dId == 'Stack':
                        memDtl.addMemUsage(dId, 'N/A', dSize)
                    if not bId in mem_detail:
                        mem_detail[bId] = []
                    if notfound:
                        mem_detail[bId].append(memDtl)
        # Calculate the percent of Cache
        used = [0 for i in range(len(mem_cacheInfo))]
        total = [0 for i in range(len(mem_cacheInfo))]
        for k, v in total_memblockB.items():
            for loc, cache_sec in enumerate(mem_cacheInfo):
                if cache_sec in k:
                    used[loc] += v
        for k, v in mem_layout.items():
            for loc, cache_sec in enumerate(mem_cacheInfo):
                if cache_sec in k:
                    total[loc] += sum([int(l) for l in mem_layout[k].values()])
        for loc, item in enumerate(mem_cacheInfo):
            #tmp = (used[loc] / total[loc]) * 100
            #percent_cache[item] = '{:.2f}'.format(tmp) + '%'
            if (total[loc] == 0):
                percent_cache[item] = 0
            else:
                percent_cache[item] = (used[loc] / total[loc]) * 100
        # Calculate the percent of each module for DATA/TEXT
        if total_textsize:
            for k, v in mem_text.items():
                percent_text[k] = (v / total_textsize) * 100
        if total_datasize:
            for k, v in mem_data.items():
                percent_data[k] = (v / total_datasize) * 100
        # Generate HTML Memory Usage
        align = '    '
        cnt = 1
        htmlpath = xmlfile[:-3] + 'html'
        with open(htmlpath, 'w') as filehandle:
            ss = []
            # HTML Start
            ss.append('<html>\n')
            ss.append(align * cnt + '<head>\n')
            cnt += 1
            ss.append(align * cnt + '<title>' + 'Memory Usage' + '</title>\n')
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
            ss.append(align * cnt + '<h1>' + 'Memory Usage' + '</h1>\n')
            ss.append(align * cnt + '<b>' + '' + '</b>\n')
            ss.append(align * cnt + '<ul>\n')
            filehandle.write(''.join(ss))
            cnt += 1
            # Memory Layout
            #ss = []
            #ss.append(align * cnt + '<li>' + 'Memory Layout' + '\n')
            #cnt += 1
            #ss.append(align * cnt + '<ul>\n')
            #cnt += 1
            #ss.append(align * cnt + '<li style="color:Blue; list-style-type:square;">\n')
            #cnt += 1
            #ss.append(align * cnt + '<table>\n')
            #cnt += 1
            #ss.append(align * cnt + '<tr style="background-color: yellow">\n')
            #ss.append(align * cnt + '  <td><b>' + 'MEMORY_BLOCK' + '</b></td>\n')
            #ss.append(align * cnt + '  <td><b>' + 'MEMORY_SECTION' + '</b></td>\n')
            #ss.append(align * cnt + '  <td><b>' + 'MEMORY_SECTION_LENGTH (Bytes)' + '</b></td>\n')
            #ss.append(align * cnt + '</tr>\n')
            #for k, v in mem_layout.items():
            #    ss.append(align * cnt + '<tr>\n')
            #    ss.append(align * cnt + '<th rowspan =' + '"' + str(len(v)) + '"' '>' + '<font size="2" face="Arial">' + k + '</font></td>\n')
            #    rowsign = 1
            #    for sk, sv in v.items():    
            #        if rowsign == 1:
            #            ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
            #            ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sv + '</font></td>\n')
            #            ss.append(align * cnt + '</tr>\n')
            #            rowsign = 0                
            #        else:
            #            ss.append(align * cnt + '<tr>\n')
            #            ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
            #            ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sv + '</font></td>\n')
            #            ss.append(align * cnt + '</tr>\n')
            #cnt -= 1
            #ss.append(align * cnt + '</table>\n')
            #cnt -= 1
            #ss.append(align * cnt + '</li>\n')
            #cnt -= 1
            #ss.append(align * cnt + '</ul>\n')
            #cnt -= 1
            #ss.append(align * cnt + '</li>\n')
            #filehandle.write(''.join(ss))
            # Memory Details
            ss = []
            ss.append('&nbsp;')
            ss.append(align * cnt + '<li>' + 'Memory Details' + '\n')
            cnt += 1
            ss.append(align * cnt + '<ul>\n')
            cnt += 1
            ss.append(align * cnt + '<li style="color:Blue; list-style-type:square;">\n')
            cnt += 1
            ss.append(align * cnt + '<table>\n')
            cnt += 1
            ss.append(align * cnt + '<tr style="background-color: yellow">\n')
            ss.append(align * cnt + '  <td><b>' + 'MEMORY_SECTION' + '</b></td>\n')
            ss.append(align * cnt + '  <td><b>' + 'Memory Type' + '</b></td>\n')
            ss.append(align * cnt + '  <td><b>' + 'Contributor' + '</b></td>\n')
            ss.append(align * cnt + '  <td><b>' + 'Size (Bytes)' + '</b></td>\n')
            ss.append(align * cnt + '</tr>\n')
            for memk, memv in mem_detail.items():
                ss.append(align * cnt + '<tr style="background-image: linear-gradient(to right, #a8ff78, #78ffd6);">\n')
                ss.append(align * cnt + '<td colspan="4"><b>' + memk + ' (<span style="font-size: 14;">' + str(total_memblockB[memk]) + 'B/' + str(sum([int(l) for l in mem_layout[memk].values()])) + 'B)' + '</span></b></td>\n')
                ss.append(align * cnt + '</tr>\n')
                total_memblock[memk] = 0
                for item in memv:
                    ss.append(align * cnt + '<tr">\n')
                    ss.append(align * cnt + '<th rowspan =' + '"' + str(item.numContributor) + '"' '>' + '<font size="2" face="Arial">' + item.memSection + ' (' + str(item.memLength) + 'B)' + '</font></td>\n')
                    total_memblock[memk] += item.memLength
                    rowsign = 1
                    #for i in range(item.numContributor):
                    for k, v in item.memDict.items():
                        for i in range(0, len(v), 2):
                            if rowsign == 1:
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + k + '</font></td>\n')
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + v[i] + '</font></td>\n')
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + v[i + 1] + '</font></td>\n')
                                ss.append(align * cnt + '</tr>\n')
                            else:
                                ss.append(align * cnt + '<tr>\n')
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + k + '</font></td>\n')
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + v[i] + '</font></td>\n')
                                ss.append(align * cnt + '  <td><font size="2" face="Arial">' + v[i + 1] + '</font></td>\n')
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
            # Memory Summary
            ss = []
            ss.append('&nbsp;\n')
            ss.append(align * cnt + '<li>' + 'Memory Summary For TEXT/DATA Section' + '\n')
            cnt += 1
            ss.append(align * cnt + '<ul>\n')
            cnt += 1
            ss.append(align * cnt + '<li style="color:Blue; list-style-type:square;">\n')
            cnt += 1
            ss.append(align * cnt + '<table>\n')
            cnt += 1
            ss.append(align * cnt + '<tr style="background-color: yellow;">\n')
            ss.append(align * cnt + '  <td><b>' + 'Memory Type' + '</b></td>\n')
            ss.append(align * cnt + '  <td><b>' + 'Contributor' + '</b></td>\n')
            ss.append(align * cnt + '  <td><b>' + 'Total Size (Bytes)' + '</b></td>\n')
            ss.append(align * cnt + '</tr>\n')
            # TEXT Section
            ss.append(align * cnt + '<tr>\n')
            ss.append(align * cnt + '<th rowspan =' + '"' + str(len(mem_text)) + '"' '>' + '<font size="2" face="Arial">' + 'TEXT' + '</font></td>\n')
            rowsign = 1
            for sk, sv in mem_text.items():    
                if rowsign == 1:
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + str(sv) + '</font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
                    rowsign = 0              
                else:
                    ss.append(align * cnt + '<tr>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + str(sv) + '</font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
            # DATA Section
            ss.append(align * cnt + '<tr>\n')
            ss.append(align * cnt + '<th rowspan =' + '"' + str(len(mem_data)) + '"' '>' + '<font size="2" face="Arial">' + 'DATA' + '</font></td>\n')
            rowsign = 1
            for sk, sv in mem_data.items():    
                if rowsign == 1:
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + str(sv) + '</font></td>\n')
                    ss.append(align * cnt + '</tr>\n')
                    rowsign = 0              
                else:
                    ss.append(align * cnt + '<tr>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + sk + '</font></td>\n')
                    ss.append(align * cnt + '  <td><font size="2" face="Arial">' + str(sv) + '</font></td>\n')
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
            ss.append(align * cnt + '<li>' + 'Memory Bar Chart' + '\n')
            cnt += 1
            ss.append(align * cnt + '<ul>\n')
            cnt += 1
            # Memory Block Bar
            ss.append(align * cnt + '<li style="color:Black; list-style-type:square;"><i>' + 'L1/L2 Section: ' + '</i>\n')
            cnt += 1

            for k, v in percent_cache.items():
                ss.append(align * cnt + '<table border="0" cellspacing="0" cellpadding="0" style="width:50%;">\n')
                ss.append(align * cnt + '<tr style="height:4px;">\n')
                ss.append(align * cnt + '<td colspan="2"><font size="2" face="Arial"><b>' + k + ': ' + '{:.2f}'.format(v) + '%' + '</b></font></td>\n')
                ss.append(align * cnt + '</tr>\n')
                ss.append(align * cnt + '<tr style="height:6px;">\n')
                ss.append(align * cnt + '<td style="width:' + str(int(v)) + '%;background-color:#003b67;"></td>\n')
                ss.append(align * cnt + '<td style="width:' + str(100 - int(v)) + '%;background-color: #4cff00;"></td>\n')
                ss.append('</tr>\n')
                ss.append('</table>\n')



            ss.append(align * cnt + '</li>\n')
            cnt -= 1
            # Memory Section Bar
            ss.append('&nbsp;\n')
            ss.append(align * cnt + '<li style="color:Black; list-style-type:square;"><i>' + 'Memory Section: ' + '</i>\n')
            cnt += 1
            # Calculate percent of Memory Section
            for k, v in total_memblock.items():
                ss.append(align * cnt + '<table border="0" cellspacing="0" cellpadding="0" style="width:50%;">\n')
                ss.append(align * cnt + '<tr style="height:4px;">\n')
                total = 0
                for sv in mem_layout[k].values():
                    total += int(sv)
                tmp = (v / total) * 100
                total_memblock[k] = '{:.2f}'.format(tmp) + '%'
                ss.append(align * cnt + '<td colspan="2"><font size="2" face="Arial"><b>' + k + ': ' + total_memblock[k] + '</b></font></td>\n')
                ss.append(align * cnt + '</tr>\n')
                ss.append(align * cnt + '<tr style="height:6px;">\n')
                ss.append(align * cnt + '<td style="width:' + str(int(tmp)) + '%;background-color:#003b67;"></td>\n')
                ss.append(align * cnt + '<td style="width:' + str(100 - int(tmp)) + '%;background-color: #4cff00;"></td>\n')
                ss.append('</tr>\n')
                ss.append('</table>\n')
            ss.append(align * cnt + '</li>\n')
            cnt -= 1
            # TEXT Bar
            ss.append('&nbsp;\n')
            ss.append(align * cnt + '<li style="color:Black; list-style-type:square;"><u><i>' + 'Text: ' + str(total_textsize) + 'Bytes in Total</i></u>\n')
            cnt += 1
            ss.append(align * cnt + '<div class="container">\n')
            for k, v in percent_text.items():
                off = 0
                percent = '{:.2f}'.format(v)
                perstr = []
                perstr.append('<p><font size="2" face="Arial">' + k + ': ' + percent + '%</font></p>')
                perstr.append('<div style="text-align:left; padding-top:5px; padding-bottom:5px; color:white; border:solid; ')
                perstr.append('width: ' + str(int(v)) + '%; background-image: ' + bc_scheme[off] + ';' + '"' + '>')
                perstr.append('</div>\n')
                ss.append(''.join(perstr))
            ss.append(align * cnt + '</div>\n')
            ss.append(align * cnt + '</li>\n')
            cnt -= 1
            # DATA Bar
            ss.append('&nbsp;\n')
            ss.append(align * cnt + '<li style="color:Black; list-style-type:square;"><u><i>' + 'Data: ' + str(total_datasize) + 'Bytes in Total</i></u>\n')
            cnt += 1
            ss.append(align * cnt + '<div class="container">\n')
            for k, v in percent_data.items():
                off = 0
                percent = '{:.2f}'.format(v)
                perstr = []
                perstr.append('<p><font size="2" face="Arial">' + k + ': ' + percent + '%</font></p>')
                perstr.append('<div style="text-align:left; padding-top:5px; padding-bottom:5px; color:white; border:solid; ')
                perstr.append('width: ' + str(int(v)) + '%; background-image: ' + bc_scheme[off] + ';' + '"' + '>')
                perstr.append('</div>\n')
                ss.append(''.join(perstr))
            ss.append(align * cnt + '</div>\n')
            ss.append(align * cnt + '</li>\n')
            cnt -= 1
            ss.append(align * cnt + '</ul>\n')
            cnt -= 1
            ss.append(align * cnt + '</li>\n')
            filehandle.write(''.join(ss))
            # HTML End
            ss = []
            cnt -= 1
            ss.append(align * cnt + '</ul>\n')
            cnt -= 1
            ss.append('&nbsp;&nbsp\n')
            ss.append('<b>Generated from OpenJADE JTest, ' + date.today().strftime('%m/%d/%y') + '.</b>\n')
            ss.append(align * cnt + '</body>\n')
            ss.append(align * cnt + '</head>\n')
            ss.append('</html>\n')
            filehandle.write(''.join(ss))
    except IOError:
        print('Warning: MemoryUsageToHtml: The XML file does not exist')
