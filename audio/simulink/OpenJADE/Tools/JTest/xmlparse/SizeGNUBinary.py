# vim: set fileencoding=utf8 :

import  re, os
from itertools import chain, groupby
import sys,getopt
__usage__ = """ \

Usage:
  SizeGNUBinary.py [-h] -m mapfile [-f ParsedMemory.txt] [-i] [-o] 

Description:
   - Runs on GNU and HiFi generated map file 
   - Parses the output map file for code, data and bss
   - Logs the output into specified "output file name"

Assumptions:


Options:
  -h,   --help  Display usage information (this message)
  -m,   --map   MAP File to be loaded
  -f,   --output_file file name for output file(in txt), defaulted to be ParsedMemory.txt
  -o,   --output_section   include this tag for the output txt to display output sections
  -i,   --input_section   include this tag for the output txt to display input sections


  
"""
data_section_list = ['data','rodata','bss','text','literal']
from xmlparse.HiFiFileGroupClassifier import HiFiFileGroupClassifier

#*********************************************************************************
# Function: verify_paths(mapPath, logPath)
# Description: Verifies the following:
#      1. Correctness of the map file path (mapPath)
#      2. Existence of the directory for the logfile
#
#*********************************************************************************
def verify_paths(mapPath, logPath):
    if (mapPath == None) or (logPath == None):
        print ('Error: .map file and output log file needs to be specified')
        return False

    if (not os.path.isfile(mapPath)):
        print ('Error: .map.xml file ' + mapPath + ' could not be found')
        return False

    logDir = os.path.dirname(logPath)
    if (not os.path.isdir(logDir)):
        print ('Error: output log dir ' + logDir + ' could not be found')
        return False


    return True
#*********************************************************************************
# Function: dsp_memory_usage(mapPath, log_path,input_section = False, input_group = False)
# Description:
#      1. Generate output log file in Jtest
#
#*********************************************************************************
def dsp_memory_usage(map_path, log_path,fl = None, input_section = False, input_group = False):

    if not verify_paths(map_path, log_path):
        return False
    memory_segments,output_sections = parseSections (open(map_path),fl)

    print_file(memory_segments,log_path,input_section,input_group)
#*********************************************************************************
# class: MemorySegment
# Description: Memory Segment is the class for big segments of memory. This is parsed
#   from the memory configuration section of the map file. It usually includes the 
#   information of its name, offset, and length.
#   In this section, the code compares the memory offset and length of the memory sections 
#   to see if that memory section belongs in this memory segment.
#   Some memory sections will not belong to any memory segments.

#   This is an example of a memory segment:
#       Name Origin Length Attributes
#       QSPI_FLASH 0x08000000 0x00800000 xr
#*********************************************************************************

class MemorySegment:
    def __init__(self, name,offset, length,used):
        self.name = name
        self.size = length
        self.offset = offset
        self.end = self.offset + self.size -1
        self.used = 0
        self.OutputSections = []
        self.dataSections = DataSection(data_section_list)

    def add_output_section(self, OutputSection):
        if OutputSection.offset >= self.offset and self.end>=OutputSection.end:
            self.OutputSections.append( OutputSection)
            self.used += OutputSection.size
            self.add_data_sections(OutputSection)
            return True
        else:
             return False
         
    def __repr__ (self):
        return 'MEMORY BLOCK:{}\toffset:0x{:x}\tsize:0x{:x} \n{}\n{}\n'.format (self.name,self.offset,self.size, repr (self.dataSections), repr (self.OutputSections))
    
    def add_data_sections(self,OutputSection):
        outSec_data = OutputSection.dataSections
        for i in range(len(data_section_list)):
            self.dataSections.size[i]+=outSec_data.size[i]
            self.dataSections.sectionIncluded[i].append(outSec_data.sectionIncluded[i])
            self.dataSections.filegroups[i].size = [self.dataSections.filegroups[i].size[z] + outSec_data.filegroups[i].size[z] for z in range(len(outSec_data.filegroups[i].size))]
            self.dataSections.filegroups[i].section_included= [self.dataSections.filegroups[i].section_included[z] + outSec_data.filegroups[i].section_included[z] for z in range(len(outSec_data.filegroups[i].section_included))]
        if outSec_data.length >len(data_section_list):
            for i in range(len(data_section_list),outSec_data.length):
                if outSec_data.sections[i] not in self.dataSections.sections:
                    self.dataSections.sections.append(outSec_data.sections[i])
                    self.dataSections.size.append(outSec_data.size[i])
                    self.dataSections.sectionIncluded.append(outSec_data.sectionIncluded[i])
                    self.dataSections.filegroups.append(outSec_data.filegroups[i])
                    self.dataSections.length += 1
                else:
                    index = self.dataSections.sections.index(outSec_data.sections[i])
                    self.dataSections.size[index]+=outSec_data.size[i]
                    self.dataSections.sectionIncluded[index].append(outSec_data.sectionIncluded[i])
                    self.dataSections.filegroups[index].size = [self.dataSections.filegroups[index].size[z] + outSec_data.filegroups[i].size[z] for z in range(len(outSec_data.filegroups[index].size))]
                    self.dataSections.filegroups[index].section_included= [self.dataSections.filegroups[index].section_included[z] + outSec_data.filegroups[i].section_included[z] for z in range(len(outSec_data.filegroups[i].section_included))]
#*********************************************************************************
# class: FileGroup
# Description:File Group class is for grouping the files based on GNU file group
#  classifier. Each one of data section in outputsection have a set of file groups 
# that goes under it. And in memory section, all those file groups get add up in
# #data sections of memory segment 
#*********************************************************************************

class FileGroup:
    def __init__(self,fl_path = None):
        self.classifier = HiFiFileGroupClassifier(fl_path)
        self.group = self.classifier.g_file_groups
        self.size = []
        self.section_included = []
        for i in self.group:
            self.size.append(0)
            self.section_included.append([])
    def add_input_section(self,input_section):
        group = self.classifier.classify(input_section)
        if group:
            index = self.group.index(group)
            self.size[index] += input_section.size
            for ins in self.section_included[index]:
                if input_section.basepath == ins.basepath and len(input_section.input_groups):
                    ins.add_input_group(input_section.input_groups[0].section,input_section.input_groups[0].offset,input_section.input_groups[0].size,input_section.input_groups[0].fill,input_section.section,input_section.basepath)
                    return "IS_INCLUDE_IG"
                    
            self.section_included[index].append(input_section)
            return

#*********************************************************************************
# class: OutputSection
# Description: output section class is for output sections in the code. They have 
#   the characteristic of stating with out a space and have offset and size after 
#   it. It may comes with a filler as well. this is an example:
#   .rel.dyn 0x08034c38 0x0
#    mem_sec offset     size
#*********************************************************************************

class OutputSection:
    def __init__ (self, section, offset, size,fl = None):
        self.section = section.strip ()
        self.offset = offset
        self.size = size
        self.end = self.offset + self.size
        self.fill = 0
        self.InputSections = []
        self.dataSections = DataSection(data_section_list,fl)

    def __repr__ (self):
        return '\tOUTPUT SECTION {}\toffset:0x{:x}\tfiller:0x{:x}\tsize:0x{:x} \n{}\n{}\n'.format (self.section, self.offset,self.fill,self.size,repr (self.dataSections),repr (self.InputSections))
    
    def add_input_section(self, InputSection):
        if InputSection:
            return self.dataSections.add_section(InputSection)

    
    def add_fill(self,offset, size):
        if not offset > self.end:
            self.fill += size

#*********************************************************************************
# class: DataSection
# Description: Data section class looks for keywords from data_section_list and 
#   match it with the keywords in the OuputSection.section. It groups all 
#   InputSections into different data sections. The keyword can be changed at 
#   the top of the file as data_section_list.
#   This class is grouping everything with lists and index. For example:
#   self.sections[0] = 'data'
#   self.size[0] = '_size_of_section_data'
#   The self.length variable stores the length of data_section_list but the actually
#   length of the sections is self.length + 1 with one extra section that stores 
#   everything that does not belong to any of the sections in data_Section_list
#*********************************************************************************

class DataSection:
    def __init__ (self, data_section_list,fl = None):
        self.sections = []
        self.sectionIncluded = []
        self.size = []
        self.filegroups = []
        self.fl =fl
        for i in data_section_list:
            self.sections.append(i)
            self.sectionIncluded.append([])
            self.size.append(0)
            self.filegroups.append(FileGroup(fl_path = fl))
        self.length = len(data_section_list)

    def add_section(self, InputSection):
        for i in range(self.length):
            search_str1 = r'[.]{0}[.]+'.format(self.sections[i])
            search_str2 = r'[.]{0}$'.format(self.sections[i])
            secre1 = re.compile(search_str1)
            m = secre1.search(InputSection.section)
            secre2 = re.compile(search_str2)
            n = secre2.search(InputSection.section)
            if m or n:
                if int(InputSection.offset) == 0:
                    return
                self.size[i] += InputSection.size
                self.filegroups[i].add_input_section(InputSection)
                for ins in self.sectionIncluded[i]:
                    if InputSection.basepath == ins.basepath and len(InputSection.input_groups):
                        ins.add_input_group(InputSection.input_groups[0].section,InputSection.input_groups[0].offset,InputSection.input_groups[0].size,InputSection.input_groups[0].fill,InputSection.section,InputSection.basepath)
                        return "IS_INCLUDE_IG"
                self.sectionIncluded[i].append(InputSection)
                return
        self.sections.append(InputSection.section.strip('.'))
        self.sectionIncluded.append([InputSection])
        self.size.append( InputSection.size)
        self.filegroups.append(FileGroup(fl_path = self.fl))
        self.filegroups[-1].add_input_section(InputSection)
        self.length +=1
        return 

    
    def __repr__ (self):
        output = ""
        for i in range(self.length):
            output += "\t\t"
            output += self.sections[i]
            output += '\tTOTAL SIZE:'
            output += str(hex(self.size[i]))
            output += '\n'
        return output
    
#*********************************************************************************
# class: InputGroup
# Description: The input group class is for any input section that has a () after
#   its directory. They will be grouped under a input section with the same basepath.
#   For example:
#       .bss 0x00000000 0x0 ...\libcr_nohost_nf.a(__sys_tmpnam.o)
#   this is an input group __sys_tmpnam.o that falls under InputSection with basepath libcr_nohost_nf.a.
#*********************************************************************************

class InputGroup:
    def __init__ (self, offset, size,input_group,file_name):
        self.file_name = file_name.strip ()
        self.section = input_group.strip('\n')
        self.offset = offset
        self.size = size
        self.end = self.offset + self.size
        self.fill = 0
    
    def add_fill(self,offset, size):
        if offset ==self.end:
            self.fill = size
            self.end +=size
            self.size += size

    def __repr__ (self):
        return '\t\t\t{}\tsize:0x{:x}\tfiller:0x{:x} \n'.format (self.section,self.size,self.fill)

#*********************************************************************************
# class: InputSection
# Description: The input section class is for input sections in the map file.
#    The input sections usually comes with a section(will be used to group into
#    data sections), a offset, a size, the comment will include the path info and 
#    a input groups list that contains the input groups that goes under it. 
#    This is an example of a input section:
#        .ARM.attributes
#        0x00000000 0x38 ...\libcr_nohost_nf.a(__sys_rename.o)
#*********************************************************************************

class InputSection:
    def __init__ (self, section, offset, size, comment):
        self.section = section.strip ()
        self.offset = offset
        self.size = size
        self.path = (None, None)
        self.basepath = None
    
        self.fill = 0
        self.input_groups = []
        input_group = None
        if comment:
            self.basepath = os.path.basename (comment)
            inputre = re.compile(r'\(([^)]+)\)')
            m = inputre.search(comment)

            if m:
                input_group = m.group(1)
        if input_group:
            self.basepath = self.basepath.replace(f"({input_group})", "")            
            in_s = InputGroup(offset,size,section,input_group)
            self.input_groups.append(in_s)

    def __repr__ (self):
        return '\t\tINPUT {}:{}\t offset:0x{:x}\tsize:0x{:x}\tfiller:0x{:x} \n{}>\n'.format (self.section,self.basepath, self.offset,self.size, self.fill, self.input_groups)

    def add_input_group(self,file_name, offset, size,fill, bathpath,section_name):
        if self.basepath == bathpath:
            if file_name:
                self.input_groups.append(InputGroup( offset, size,section_name,file_name))
                self.size += size 
                self.fill+= fill
                return True
        return False
    
    def add_fill(self,offset,size):
        if len(self.input_groups):
            self.input_groups[-1].add_fill(offset,size)
            self.fill += self.input_groups[-1].fill
            self.size += self.input_groups[-1].fill
        else:
            if offset == self.offset+self.size:
                self.fill = size
                self.size +=size
#*********************************************************************************
# Function: parseSections (fd)
# Description: The parseSection function goes through the whole memory map and
#   find memory segments, output sections, input sections and filler. It can parse 
#   both Hifo and GNU memory maps with either memory configuration section in the 
#   middle or at the end by having the first_mem and first first_linker bool variables.
#*********************************************************************************

def parseSections (fd,fl = None):
 
    output_sections = []
    memory_segments = []

    # skip until memory map is found
    first_mem =False
    first_linker = False
    while True:
        l = fd.readline()
        if not l:
            break
        if l.strip().lower() == 'linker script and memory map':
            first_linker = True
            break
        elif l.strip().lower() == 'memory configuration':
            first_mem = True
            break
    if not (first_mem or first_linker):
        return None

    #find all mempry segments in memory configureation
    memory_segmentre = re.compile('(?P<memory_segment>.+?.)[ ]+0x(?P<memory_start>[0-9a-f]+)[ ]+0x(?P<memory_length>[0-9a-f]+)[ ]+(0x(?P<memory_used>[0-9a-f]+)[ ]+0x(?P<memory_unused>[0-9a-f]+)[ ]+)?([a-z]+[ ]?)?[ ]?\n',re.I)
    #find memory section, start offset, size, base path, input group
    sectionre = re.compile ('(?P<section>.+?|.{14,}\n)[ ]+0x(?P<offset>[0-9a-f]+)[ ]+0x(?P<size>[0-9a-f]+)(?:[ ]+(?P<comment>.{3,}))?\n+', re.I)
    #find the fill
    fillre = re.compile('[ ][*]fill[*][ ]+[ ]+0x(?P<offset>[0-9a-f]+)[ ]+0x(?P<size>[0-9a-f]+)[ ]+(.?)+\n',re.I)
    memConfigre = re.compile('Memory Configuration[ ]?\n',re.I)
    linkerMemre = re.compile('Linker [S|s]cript and memory map[ ]?\n',re.I)
    s = fd.read ()
    pos = 0
    found_mem = False
    found_linker = False
    os =None
    ins = None

    while True:
        m = memConfigre.match(s,pos)
        if m and first_linker:
            found_mem = True
            pos = m.end ()
            continue
        m = linkerMemre.match(s,pos)
        if m and first_mem:
            found_linker = True
            pos = m.end ()
            continue
        if ((not found_mem) and first_linker) or (found_linker and first_mem):
            m = sectionre.match (s, pos)
            if not m :
                m = fillre.match (s, pos)
                if not m:
                    # skip that line
                    try:
                        nextpos = s.index ('\n', pos)+1
                        pos = nextpos
                        continue
                    except ValueError:
                        break
                else:
                    #calculate filler
                    pos = m.end ()
                    fill_offset = m.group('offset')
                    fill_offset = int (fill_offset, 16)
                    fill_size = m.group('size')
                    fill_size = int (fill_size, 16)
                    if fill_size>0:
                        if ins:
                            ins.add_fill(fill_offset,fill_size)
                        os.add_fill(fill_offset,fill_size)
                    continue
            else:        
                pos = m.end ()
                if os and ins:
                    if os.add_input_section(ins) =="IS_INCLUDE_IG":
                            ins = None
                section = m.group ('section')
                v = m.group ('offset')
                offset = int (v, 16) if v is not None else None
                v = m.group ('size')
                size = int (v, 16) if v is not None else None
                comment = m.group ('comment')

                if section != '*default*' and size > 0 and section != ' ':
                    if section.startswith (' '):
                        ins = InputSection (section, offset, size, comment)
                    else:
                        os = OutputSection (section, offset, size,fl)
                        output_sections.append(os)
                        ins = None
                    if section.startswith ('  ') :
                        ins = None
                else:
                    ins = None
      
        if (found_mem and first_linker) or (first_mem and (not found_linker)):
            m =memory_segmentre.match(s,pos)
            if m:
                pos = m.end()
                memory_segment = m.group('memory_segment')
                memory_start = m.group('memory_start')
                memory_start = int (memory_start, 16)
                memory_length = m.group('memory_length')
                memory_length = int (memory_length, 16)
                memory_used = None
                memory_used = m.group('memory_used')
                if memory_used:
                    memory_used = int (memory_used, 16)
                memory_unused = None
                memory_unused = m.group('memory_unused')
                if memory_unused:
                    memory_unused = int (memory_unused, 16)
                if memory_segment != '*default*':

                    mseg = MemorySegment(memory_segment,memory_start,memory_length,memory_used)
                    memory_segments.append(mseg)
            else:
                try:
                    nextpos = s.index ('\n', pos)+1
                    pos = nextpos
                    continue
                except ValueError:
                    break
    if memory_segments and output_sections:
        for i in output_sections:
            found = False
            for j in memory_segments:
                if j.add_output_section(i):
                    found = True
                    break
            if not found:
                print("WARNING: output section:",i.section," not found in any memory segments")
    return memory_segments,output_sections
#*********************************************************************************
# Function: print_line(fp,num,tab_num)
# Description: print a line of -
#*********************************************************************************
def print_line(fp,num,tab_num):
    for i in range(tab_num):
        fp.write('\t')
    for i in range (num):
        fp.write('-')
    fp.write('\n')
#*********************************************************************************
# Function: print_obj(fp,total_len_front,str_front,total_len_back,str_back,tab_num)
# Description: print an object in output file -
#*********************************************************************************
def print_obj(fp,total_len_front,str_front,total_len_back,str_back,tab_num):
    str_back = str(str_back)
    for i in range(tab_num):
        fp.write('\t')
    fp.write('|')
    for i in range(total_len_front -len(str_front)):
        fp.write(' ')
    fp.write(str_front)
    fp.write('|')
    fp.write(str_back)
    for i in range(total_len_back -len(str_back)):
        fp.write(' ')
    fp.write('|\n')
    
#*********************************************************************************
# Function: print_file(memory_segments,fileName,INS,IG)
# Description: Print all information into a txt file specified in file name. INS 
#   stands for input section and IG stands for input group. Both are defaulted 
#   to false so they will not be printed in default. They can be set to true to be 
#   printed in file.
#*********************************************************************************
def print_file(memory_segments,fileName,OS = False,INS = False):
    fp = open(fileName,"w")
    for mseg in memory_segments:
        #print memory segment info
        fp.write("MEMORY SEGMENT: ")
        fp.write(mseg.name)
        fp.write('\n')
        print_line(fp,80,0)
        print_line(fp,48,1)
        print_obj(fp,20,"TOTAL SIZE",25,mseg.size,1)
        print_obj(fp,20,"TOTAL USED",25,mseg.used,1)
        print_line(fp,48,1)
        for i in range(mseg.dataSections.length):
            if mseg.dataSections.size[i] != 0:
                print_line(fp,48,1)
                print_obj(fp,20,mseg.dataSections.sections[i],25,mseg.dataSections.size[i],1)
                print_line(fp,48,1)
                for z in range(len(mseg.dataSections.filegroups[i].group)):
                    if mseg.dataSections.filegroups[i].size[z] != 0:
                        print_obj(fp,20,mseg.dataSections.filegroups[i].group[z],25,mseg.dataSections.filegroups[i].size[z],2)


        print_line(fp,48,1)
        #print output section info
        if OS:
            for os in mseg.OutputSections:
                fp.write("\tOUTPUT SECTION: ")
                fp.write(os.section)
                fp.write('\n')
                print_line(fp,75,1)
                print_line(fp,48,2)
                print_obj(fp,20,"TOTAL SIZE USED",25,os.size,2)
                print_obj(fp,20,"FILLER",25,os.fill,2)
                for i in range(os.dataSections.length):
                    if os.dataSections.size[i] != 0:
                        print_line(fp,70,2)
                        print_obj(fp,10,os.dataSections.sections[i],57,os.dataSections.size[i],2)
                        print_line(fp,70,2)
                        for z in range(len(os.dataSections.filegroups[i].group)):
                            if os.dataSections.filegroups[i].size[z] != 0:
                                print_obj(fp,10,os.dataSections.filegroups[i].group[z],35,os.dataSections.filegroups[i].size[z],3)
                                print_line(fp,70,3)
                                if INS:
                                    for ins in os.dataSections.filegroups[i].section_included[z]:
                                        fp.write('\t\t\tINPUT SECTION: ')
                                        fp.write(ins.basepath)
                                        fp.write('\n')
                                        print_line(fp,48,3)
                                        print_obj(fp,20,"SECTION",25,ins.section,3)
                                        print_obj(fp,20,"TOTAL SIZE USED",25,ins.size,3)
                                        print_line(fp,48,3)

                                        for ig in ins.input_groups:
                                            fp.write('\t\t\t\tINPUT GROUP: ')
                                            fp.write(ig.file_name)
                                            fp.write('\n')
                                            print_line(fp,48,4)
                                            print_obj(fp,20,"SECTION",25,ig.section,4)

                                            print_obj(fp,20,"TOTAL SIZE USED",25,ig.size,4)
                                            print_line(fp,48,4)

    fp.close()

    
#*********************************************************************************
# Function: usage()
# Description:
#
#*********************************************************************************
def usage():
    sys.stdout.write(__usage__)
    sys.exit()

#*********************************************************************************
# Function: main()
# Description:
#
#*********************************************************************************
def main( argv=sys.argv[1:] ):
    try:
        opts, args = getopt.getopt(argv, "hf:oim:", ["help", "output_file=", "ouput_section","input_section", "map="])
    except getopt.GetoptError:
        usage()
    output_file_name = None
    map_file = None
    output_section = None
    input_section = None
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            usage()
        elif opt in ("-f", "--ouput_file"):
            output_file_name = arg;
        elif opt in ("-m", "--map"):
            map_file = arg;
        elif opt in ("-o", "--output_section"):
            output_section = True
        elif opt in ("-i", "--input_section"):
            input_section = True
    if output_file_name == None:
        output_file_name = "ParsedMemory.txt"
    if map_file == None:
        print("Need Input MAP File")
        return
    fd = open(map_file)
    memory_segments,output_sections = parseSections (fd)

    print_file(memory_segments,output_file_name,output_section,input_section)
    sys.exit()

if __name__ == '__main__':
    main()
