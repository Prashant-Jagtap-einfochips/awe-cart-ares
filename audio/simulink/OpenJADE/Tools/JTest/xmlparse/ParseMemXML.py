#*********************************************************************************
# This script is for parsing of the xml file to build the data structure
#
#*********************************************************************************

# libraries
import sys
import math
import xml.etree.ElementTree as et
from xml.etree.ElementTree import XMLParser
from collections import OrderedDict

from xmlparse.FileGroupClassifier import FileGroupClassifier
from xmlparse.MemoryGroupClassifier import MemoryGroupClassifier

#*********************************************************************************
# Globals
#
#*********************************************************************************
is_py2 = sys.version[0] == '2'
g_memoryGroupClassifier = None
g_fileGroupClassifier = None

#*********************************************************************************
# Class Defination - data structure
#
#*********************************************************************************

def my_ceil_div(x, y):
    return int(math.ceil(float(x)/y))

class ldf_input_section:
    def __init__(self, name, start, size, width, input_file):
        self.name = name
        self.start = int(start, 0)
        self.width = width
        self.size = int(size, 0)
        self.end = self.start + self.size - 1;
        self.size_bytes = self.size * (self.width/8)
        self.input_file = input_file

    def adjust_size(self, end):
        self.end = end
        self.size = self.end - self.start + 1;
        self.size_bytes = self.size * (self.width/8)

    def myprint(self):
        print ('%s: start 0x%8.8X size 0x%X file %s' % (self.name, self.start, self.size, self.input_file))

class ldf_output_section:
    def __init__(self, start, size, width, debug=False):
        self.start = int(start, 0)
        self.width = width
        self.size = int(size, 0)
        self.end = self.start + self.size - 1;
        self.size_bytes = self.size * (self.width/8)
        self.finalized = False
        self.INPUT_SECTIONS = OrderedDict()

        self.INPUT_SECTIONS_GROUPS = OrderedDict()
        for group in g_fileGroupClassifier.groups():
            self.INPUT_SECTIONS_GROUPS[group] = OrderedDict()

        self.debug = debug

    def set_address_ranges(self, BW, SW, NW, ISA, LW):
        self.BWstart  = BW
        self.SWstart  = SW
        self.NWstart  = NW
        self.ISAstart = ISA
        self.LWstart  = LW

        self.BWend  = BW  + my_ceil_div(self.size_bytes , (8 /8)) - 1
        self.SWend  = SW  + my_ceil_div(self.size_bytes , (16/8)) - 1
        self.NWend  = NW  + my_ceil_div(self.size_bytes , (32/8)) - 1
        self.ISAend = ISA + my_ceil_div(self.size_bytes , (48/8)) - 1
        self.LWend  = LW  + my_ceil_div(self.size_bytes , (64/8)) - 1

    def classify_input_file(self, input_file):
        return g_fileGroupClassifier.classify(input_file)

    def add_input_section(self, name, start, size, input_file):
        my_input_section = ldf_input_section(name, start, size, self.width, input_file)
        self.INPUT_SECTIONS[start] = my_input_section

        my_group = self.classify_input_file(input_file)
        if my_group == None:
            sys.stdout.write('Warning: unassigned input section ')
            my_input_section.myprint()
        else:
            self.INPUT_SECTIONS_GROUPS[my_group][start] = my_input_section

        return my_input_section

    def memory_breakdown(self):
        if self.finalized:
            memory_totals = OrderedDict()
            for x in self.INPUT_SECTIONS_GROUPS.keys():
                input_section_group = self.INPUT_SECTIONS_GROUPS[x]
                memory_total = 0
                for y in input_section_group.keys():
                    memory_total = memory_total + input_section_group[y].size_bytes
                if memory_total > 0:
                    memory_totals[x] = memory_total
            return memory_totals
        else:
            return None

    def scan_input_sections_tree(self, root):
        self.root = root
        for ins in self.root.iter('INPUT_SECTION'):
            name = ins.get('name')
            size = ins.get('size')
            if int(size, 0) == 0:
                continue
            start = ins.get('start_address')
            input_file = ins.find('INPUT_FILE').text
            my_input_section = self.add_input_section(name, start, size, input_file)
            if self.debug:
                print ('\t\t\t' + name + '\t' + input_file)

    def set_finalized(self):
        if is_py2:
            self.addresses = sorted(self.INPUT_SECTIONS.keys(), cmp=lambda x, y: cmp(self.INPUT_SECTIONS[x].start, self.INPUT_SECTIONS[y].start))
        else:
            self.addresses = sorted(self.INPUT_SECTIONS.keys(), key=lambda x: self.INPUT_SECTIONS[x].start)
        self.count = len(self.INPUT_SECTIONS)
        for idx in range(0, self.count):
            start = self.addresses[idx]
            if idx == (self.count - 1):
                end = self.end
            else:
                end = int(self.addresses[idx+1], 0) - 1
            size_bytes_org = self.INPUT_SECTIONS[start].size_bytes
            self.INPUT_SECTIONS[start].adjust_size(end)
            size_bytes_new = self.INPUT_SECTIONS[start].size_bytes
            if self.debug:
                if size_bytes_new != size_bytes_org:
                    sys.stdout.write( 'org: ' + str(size_bytes_org) + ' mod: ' + str(size_bytes_new) )
                    self.INPUT_SECTIONS[start].myprint()

        self.finalized = True

    def get_input_sections_count(self):
        if self.finalized:
            return self.count
        else:
            return None

    def get_input_section_at_address(self, address):
        if self.finalized:
            if address in self.addresses:
                return self.INPUT_SECTIONS[address]
        else:
            return None

    def get_input_section_at_index(self, idx):
        if self.finalized:
            if idx < self.count:
                return self.INPUT_SECTIONS[self.addresses[idx]]
        else:
            return None

    def get_input_sections(self):
        if self.finalized:
            return self.addresses
        else:
            return None

    def get_input_sections_groups(self):
        if self.finalized:
            return self.INPUT_SECTIONS_GROUPS
        else:
            return None

    def myprint(self):
        if self.finalized == False:
            return

        print ('\t\tstart 0x%8.8X size 0x%X width 0x%X' % (self.start, self.size, self.width))
        for address in self.addresses:
            sys.stdout.write( '\t\t\t' )
            self.INPUT_SECTIONS[address].myprint()

class ldf_memory_section:
    def __init__(self, start, end, width, usage, debug = False):
        self.start = int(start, 0)
        self.end = int(end, 0)
        self.width = width
        self.usage = int(usage, 0)
        self.size = (self.end - self.start + 1)
        self.size_bytes = self.size * (self.width/8)
        self.usage_bytes = self.usage * (self.width/8)
        self.finalized = False
        self.filler_bytes = 0
        self.debug = debug

        self.OUTPUT_SECTIONS = OrderedDict()
        self.OUTPUT_SECTIONS_GROUPS = OrderedDict()
        for group in g_memoryGroupClassifier.groups():
            self.OUTPUT_SECTIONS_GROUPS[group] = OrderedDict()

    def set_address_ranges(self, BW, SW, NW, ISA, LW):
        self.BWstart  = BW
        self.SWstart  = SW
        self.NWstart  = NW
        self.ISAstart = ISA
        self.LWstart  = LW

        self.BWend  = BW  + my_ceil_div(self.size_bytes , (8 /8)) - 1
        self.SWend  = SW  + my_ceil_div(self.size_bytes , (16/8)) - 1
        self.NWend  = NW  + my_ceil_div(self.size_bytes , (32/8)) - 1
        self.ISAend = ISA + my_ceil_div(self.size_bytes , (48/8)) - 1
        self.LWend  = LW  + my_ceil_div(self.size_bytes , (64/8)) - 1

    def classify_output_section(self, name):
        return g_memoryGroupClassifier.classify(name)

    def memory_breakdown(self):
        if self.finalized:
            memory_totals = OrderedDict()
            for memory_group in g_memoryGroupClassifier.groups():
                memory_totals[memory_group] = OrderedDict()
                for file_group in g_fileGroupClassifier.groups():
                    memory_totals[memory_group][file_group] = 0
                for output_section in self.OUTPUT_SECTIONS_GROUPS[memory_group].keys():
                    if output_section in ['Heap', 'Stack']:
                        memory_totals[output_section] = OrderedDict()
                        memory_totals[output_section]['total'] = self.OUTPUT_SECTIONS_GROUPS[memory_group][output_section].size_bytes

                    sub_total = self.OUTPUT_SECTIONS_GROUPS[memory_group][output_section].memory_breakdown()
                    for file_group in g_fileGroupClassifier.groups():
                        if file_group in sub_total.keys():
                            memory_totals[memory_group][file_group] = memory_totals[memory_group][file_group] + sub_total[file_group]

            for memory_group in g_memoryGroupClassifier.groups():
                if memory_group in memory_totals.keys():
                    for file_group in g_fileGroupClassifier.groups():
                        if file_group in memory_totals[memory_group].keys():
                            if memory_totals[memory_group][file_group] == 0:
                                del memory_totals[memory_group][file_group]

            for memory_group in g_memoryGroupClassifier.groups():
                if not memory_totals[memory_group]:
                    del memory_totals[memory_group]

            for memory_group in g_memoryGroupClassifier.groups():
                if memory_group in ['Heap', 'Stack']:
                    continue
                if memory_group in memory_totals.keys():
                    memory_group_total = 0
                    for file_group in g_fileGroupClassifier.groups():
                        if file_group in memory_totals[memory_group].keys():
                            memory_group_total = memory_group_total + memory_totals[memory_group][file_group]
                    memory_totals[memory_group]['total'] = memory_group_total
            memory_totals['length'] = self.size_bytes
            memory_totals['filler'] = self.filler_bytes
            return memory_totals
        else:
            return None

    def add_output_section(self, name, start, size, width):
        output_section = ldf_output_section(start, size, width, debug=self.debug)
        self.OUTPUT_SECTIONS[name] = output_section
        memory_group = self.classify_output_section(name)
        if memory_group == None:
            print ('Warning: unassigned output section ' + name)
        else:
            self.OUTPUT_SECTIONS_GROUPS[memory_group][name] = output_section

        start = int(start,0)
        offset_bytes = None
        if (width == 8):
            offset_bytes = (start - self.BWstart) * (width/8)
        elif (width == 16):
            offset_bytes = (start - self.SWstart) * (width/8)
        elif (width == 32):
            offset_bytes = (start - self.NWstart) * (width/8)
        elif (width == 48):
            offset_bytes = (start - self.ISAstart) * (width/8)
        elif (width == 64):
            offset_bytes = (start - self.LWstart) * (width/8)
        if offset_bytes != None:
            BW  = self.BWstart  + my_ceil_div(offset_bytes , (8/8))
            SW  = self.SWstart  + my_ceil_div(offset_bytes , (16/8))
            NW  = self.NWstart  + my_ceil_div(offset_bytes , (32/8))
            ISA = self.ISAstart + my_ceil_div(offset_bytes , (48/8))
            LW  = self.LWstart  + my_ceil_div(offset_bytes , (64/8))

        self.OUTPUT_SECTIONS[name].set_address_ranges(BW, SW, NW, ISA, LW)

        return self.OUTPUT_SECTIONS[name]

    def isMember(self, _start, _end, width):
        start = int(_start,0)
        end = int(_end,0)

        if (width == 8):
            s0 = self.BWstart
            e0 = self.BWend
        elif (width == 16):
            s0 = self.SWstart
            e0 = self.SWend
        elif (width == 32):
            s0 = self.NWstart
            e0 = self.NWend
        elif (width == 48):
            s0 = self.ISAstart
            e0 = self.ISAend
        elif (width == 64):
            s0 = self.LWstart
            e0 = self.LWend
        if (start >= s0) and (end <= e0):
            return True
        else:
            return False

    def findMemoryWidth(self, _addr):
        addr = int(_addr,0)
        if (addr >= self.BWstart) and (addr <= self.BWend):
            return 8
        elif (addr >= self.SWstart) and (addr <= self.SWend):
            return 16
        elif (addr >= self.NWstart) and (addr <= self.NWend):
            return 32
        elif (addr >= self.ISAstart) and (addr <= self.ISAend):
            return 48
        elif (addr >= self.LWstart) and (addr <= self.LWend):
            return 64
        else:
            return None

    def scan_output_sections_tree(self, root):
        self.root = root
        for output_section in self.root.iter('OUTPUT_SECTION'):
            name = output_section.get('name')
            if self.debug:
                print ('\t\t' + name)
            size = output_section.get('word_size')
            if int(size, 0) == 0:
                continue
            start = output_section.get('start_address')
            if output_section.get('memory_width') == None:
                if ('nw_code' in name) or ('iv_code' in name):
                    width = 48
                else:
                    width = self.findMemoryWidth(start)
            else:
                width = int(output_section.get('memory_width'),0)
            my_output_section = self.add_output_section(name, start, size, width)
            my_output_section.scan_input_sections_tree(output_section)
            my_output_section.set_finalized()

    def add_heap_block(self, root):
        for ldf_symbol in root.iter('LDF_SYMBOLS'):
            start = None
            end = None
            for symbol in ldf_symbol.iter('SYMBOL'):
                if symbol.get('name') == 'ldf_heap_space':
                    start = symbol.get('address')
                if symbol.get('name') == 'ldf_heap_end':
                    end = symbol.get('address')
                if symbol.get('name') == 'ldf_heap_length':
                    length = symbol.get('address')
            if (start != None) and (end != None):
                if self.isMember(start, end, 8):
                    if self.debug:
                        print ('\t' + 'Heap')
                    my_output_section = self.add_output_section('Heap', start, length, 8)
                    my_output_section.set_finalized()

    def add_stack_block(self, root):
        for ldf_symbol in root.iter('LDF_SYMBOLS'):
            start = None
            end = None
            for symbol in ldf_symbol.iter('SYMBOL'):
                if symbol.get('name') == 'ldf_stack_space':
                    start = symbol.get('address')
                if symbol.get('name') == 'ldf_stack_end':
                    end = symbol.get('address')
                if symbol.get('name') == 'ldf_stack_length':
                    length = symbol.get('address')
            if (start != None) and (end != None):
                if self.isMember(start, end, 8):
                    if self.debug:
                        print ('\t' + 'Stack')
                    my_output_section = self.add_output_section('Stack', start, length, 8)
                    my_output_section.set_finalized()

    def set_finalized(self):
        if is_py2:
            self.names = sorted(self.OUTPUT_SECTIONS.keys(), cmp=lambda x, y: cmp(self.OUTPUT_SECTIONS[x].BWstart, self.OUTPUT_SECTIONS[y].BWstart))
        else:
            self.names = sorted(self.OUTPUT_SECTIONS.keys(), key=lambda x: self.OUTPUT_SECTIONS[x].BWstart)

        self.count = len(self.OUTPUT_SECTIONS)
        for idx in range(0, (self.count-1)):
            curr = self.OUTPUT_SECTIONS[self.names[idx]]
            next = self.OUTPUT_SECTIONS[self.names[idx+1]]
            curr_end = curr.BWend
            next_start = next.BWstart
            filler = next_start - curr_end - 1
            if filler != 0:
                self.filler_bytes = self.filler_bytes + filler

        self.finalized = True

    def get_output_sections_count(self):
        if self.finalized:
            return self.count
        else:
            return None

    def get_output_section_by_name(self, name):
        if self.finalized:
            if name in self.names:
                return self.OUTPUT_SECTIONS[name]
        else:
            return None

    def get_output_section_at_index(self, idx):
        if self.finalized:
            if idx < self.count:
                return self.OUTPUT_SECTIONS[self.names[idx]]
        else:
            return None

    def get_output_sections_groups(self):
        if self.finalized:
            return self.OUTPUT_SECTIONS_GROUPS
        else:
            return None

    def get_output_sections(self):
        if self.finalized:
            return self.names
        else:
            return None

    def myprint(self):
        if self.finalized == False:
            return

        print ('\tstart 0x%8.8X end 0x%8.8X usage 0x%X width 0x%X' % (self.start, self.end, self.usage, self.width))
        for name in self.names:
            print ('\t\t' + name)
            self.OUTPUT_SECTIONS[name].myprint()
            print ('')

class ldf_memory_range:
    def __init__(self, start, end, width):
        self.width = width
        self.start = int(start,0)
        self.end = int(end,0)
        self.size = (self.end - self.start + 1)
        self.size_bytes = self.size * (self.width/8)

class ldf_memory_block:
    def __init__(self, debug=False):
        self.LW = None
        self.ISA = None
        self.NW = None
        self.SW = None
        self.BW = None
        self.cache = None
        self.MEMORY_SECTIONS = OrderedDict()
        self.finalized = False
        self.debug = debug

    def set_cache_type(self, cache):
        self.cache = cache
        if cache != None:
            if cache == 'DM':
                self.cache_symbol = '___ldf_dmcachesize'
            elif cache == 'PM':
                self.cache_symbol = '___ldf_pmcachesize'
            elif cache == 'I':
                self.cache_symbol = '___ldf_icachesize'

    def long_word_address(self,start,end):
        self.LW = ldf_memory_range(start, end, 64)

    def ISA_code_address(self,start,end):
        self.ISA = ldf_memory_range(start, end, 48)

    def normal_word_address(self,start,end):
        self.NW = ldf_memory_range(start, end, 32)

    def short_word_address(self,start,end):
        self.SW = ldf_memory_range(start, end, 16)

    def byte_access_address(self,start,end):
        self.BW = ldf_memory_range(start, end, 8)

    def isMember(self, _start, _end, width):
        start = int(_start,0)
        end = int(_end,0)

        if (width == 8):
            s0 = self.BW.start
            e0 = self.BW.end
        elif (width == 16):
            s0 = self.SW.start
            e0 = self.SW.end
        elif (width == 32):
            s0 = self.NW.start
            e0 = self.NW.end
        elif (width == 48):
            s0 = self.ISA.start
            e0 = self.ISA.end
        elif (width == 64):
            s0 = self.LW.start
            e0 = self.LW.end
        if (start >= s0) and (end <= e0):
            return True
        else:
            return False

    def memory_breakdown(self):
        if self.finalized:
            memory_totals = OrderedDict()
            for name in self.MEMORY_SECTIONS.keys():
                memory_totals[name] = self.MEMORY_SECTIONS[name].memory_breakdown()
            return memory_totals
        else:
            return None

    def add_memory_section(self, name, start, end, width, usage):
        self.MEMORY_SECTIONS[name] = ldf_memory_section(start, end, width, usage, debug=self.debug)

        start = int(start,0)
        offset_bytes = None

        if (width == 8):
            offset_bytes = (start - self.BW.start) * (width/8)
        elif (width == 16):
            offset_bytes = (start - self.SW.start) * (width/8)
        elif (width == 32):
            offset_bytes = (start - self.NW.start) * (width/8)
        elif (width == 48):
            offset_bytes = (start - self.ISA.start) * (width/8)
        elif (width == 64):
            offset_bytes = (start - self.LW.start) * (width/8)
        if offset_bytes != None:
            BW  = self.BW.start  + my_ceil_div(offset_bytes , (8/8))
            SW  = self.SW.start  + my_ceil_div(offset_bytes , (16/8))
            NW  = self.NW.start  + my_ceil_div(offset_bytes , (32/8))
            ISA = self.ISA.start + my_ceil_div(offset_bytes , (48/8))
            LW  = self.LW.start  + my_ceil_div(offset_bytes , (64/8))

        self.MEMORY_SECTIONS[name].set_address_ranges(BW, SW, NW, ISA, LW)
        return self.MEMORY_SECTIONS[name]

    def scan_memory_sections_tree(self, root):
        self.root = root
        for ms in self.root.iter('MEMORY'):
            name = ms.get('name')
            start = ms.get('start_address')
            end = ms.get('end_address')
            width = int(ms.get('width'),0)
            usage = ms.get('words_used')
            if self.isMember(start, end, width):
                if self.debug:
                    print ('\t' + name)
                my_memory_section = self.add_memory_section(name, start, end, width, usage)
                my_memory_section.scan_output_sections_tree(ms)
                my_memory_section.add_heap_block(self.root)
                my_memory_section.add_stack_block(self.root)
                my_memory_section.set_finalized()

    def add_cache_block(self, root):
        for ldf_symbol in root.iter('LDF_SYMBOLS'):
            for symbol in ldf_symbol.iter('SYMBOL'):
                if self.cache == None:
                    continue
                if symbol.get('name') == self.cache_symbol:
                    address = symbol.get('address')
                    size = None
                    if address == '0xffffffff':
                        size = 0 * 1024
                    elif address == '0x0':
                        size = 16 * 1024
                    elif address == '0x1':
                        size = 32 * 1024
                    elif address == '0x2':
                        size = 64 * 1024
                    elif address == '0x3':
                        size = 128 * 1024
                    if size != None:
                        end = '0x%8.8X' % self.BW.end
                        start = '0x%8.8X' % (self.BW.end - size + 1)
                        width = 8
                        usage = '0x0'
                        name = self.cache + 'cache'
                        my_memory_section = self.add_memory_section(name, start, end, width, usage)
                        my_memory_section.set_finalized()

    def set_finalized(self):
        if is_py2:
            self.names = sorted(self.MEMORY_SECTIONS.keys(), cmp=lambda x, y: cmp(self.MEMORY_SECTIONS[x].BWstart, self.MEMORY_SECTIONS[y].BWstart))
        else:
            self.names = sorted(self.MEMORY_SECTIONS.keys(), key=lambda x: self.MEMORY_SECTIONS[x].BWstart)
        self.count = len(self.MEMORY_SECTIONS)
        self.finalized = True

    def get_memory_sections_count(self):
        if self.finalized:
            return self.count
        else:
            return None

    def get_memory_section_by_name(self, name):
        if self.finalized:
            if name in self.names:
                return self.MEMORY_SECTIONS[name]
        else:
            return None

    def get_memory_section_at_index(self, idx):
        if self.finalized:
            if idx < self.count:
                return self.MEMORY_SECTIONS[self.names[idx]]
        else:
            return None

    def get_memory_sections(self):
        if self.finalized:
            return self.names
        else:
            return None

    def myprint(self):
        if self.finalized == False:
            return

        print ('start 0x%8.8X end 0x%8.8X size 0x%X width 0x%X' % (self.BW.start, self.BW.end, self.BW.size, self.BW.width))
        for name in self.names:
            print ('\t' + name)
            self.MEMORY_SECTIONS[name].myprint()
            print ('')

class ldf_memory_blocks:
    def __init__(self, proc, xml_root, debug = False):
        self.debug = debug
        self.proc = proc
        self.MEMORY_BLOCKS = OrderedDict()

        mb = ldf_memory_block(debug=self.debug)
        mb.long_word_address  ('0x00048000','0x0004DFFF')
        mb.ISA_code_address   ('0x00090000','0x00097FFF')
        mb.normal_word_address('0x00090000','0x0009BFFF')
        mb.short_word_address ('0x00120000','0x00137FFF')
        mb.byte_access_address('0x00240000','0x0026FFFF')
        mb.set_cache_type(None)
        self.MEMORY_BLOCKS['L1 Block 0 SRAM'] = mb

        mb = ldf_memory_block(debug=self.debug)
        mb.long_word_address  ('0x00058000','0x0005DFFF')
        mb.ISA_code_address   ('0x000B0000','0x000B7FFF')
        mb.normal_word_address('0x000B0000','0x000BBFFF')
        mb.short_word_address ('0x00160000','0x00177FFF')
        mb.byte_access_address('0x002C0000','0x002EFFFF')
        mb.set_cache_type('DM')
        self.MEMORY_BLOCKS['L1 Block 1 SRAM'] = mb

        mb = ldf_memory_block(debug=self.debug)
        mb.long_word_address  ('0x00060000','0x00063FFF')
        mb.ISA_code_address   ('0x000C0000','0x000C5554')
        mb.normal_word_address('0x000C0000','0x000C7FFF')
        mb.short_word_address ('0x00180000','0x0018FFFF')
        mb.byte_access_address('0x00300000','0x0031FFFF')
        mb.set_cache_type('PM')
        self.MEMORY_BLOCKS['L1 Block 2 SRAM'] = mb

        mb = ldf_memory_block(debug=self.debug)
        mb.long_word_address  ('0x00070000','0x00073FFF')
        mb.ISA_code_address   ('0x000E0000','0x000E5554')
        mb.normal_word_address('0x000E0000','0x000E7FFF')
        mb.short_word_address ('0x001C0000','0x001CFFFF')
        mb.byte_access_address('0x00380000','0x0039FFFF')
        mb.set_cache_type('I')
        self.MEMORY_BLOCKS['L1 Block 3 SRAM'] = mb

        if proc in ['ADSP-21584','ADSP-SC584']:
            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x005A0000','0x005AAAAF')
            mb.normal_word_address('0x08020000','0x0802FFFF')
            mb.short_word_address ('0x00BA0000','0x00BBFFFF')
            mb.byte_access_address('0x20080000','0x200BFFFF')
            self.MEMORY_BLOCKS['L2 RAM'] = mb

            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x00000000','0x00000000')
            mb.normal_word_address('0x00000000','0x00000000')
            mb.short_word_address ('0x00000000','0x00000000')
            mb.byte_access_address('0x80000000','0x8fffffff')
            self.MEMORY_BLOCKS['DDR RAM'] = mb

        elif proc in ['ADSP-21569','ADSP-21565']:
            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x005C0000','0x005EAAAA')
            mb.normal_word_address('0x08000000','0x0803FFFF')
            mb.short_word_address ('0x00B80000','0x00BFFFFF')
            mb.byte_access_address('0x20000000','0x200FFFFF')
            self.MEMORY_BLOCKS['L2 RAM'] = mb

            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x00000000','0x00000000')
            mb.normal_word_address('0x00000000','0x00000000')
            mb.short_word_address ('0x00000000','0x00000000')
            mb.byte_access_address('0x80000000','0xbfffffff')
            self.MEMORY_BLOCKS['DDR RAM'] = mb

        elif proc in ['ADSP-21593W', 'ADSP-SC594', 'ADSP-SC594W', 'ADSP-SC598', 'ADSP-SC598W']:
            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x00580000','0x005D5554')
            mb.normal_word_address('0x08000000','0x0807FFFF')
            mb.short_word_address ('0x00B00000','0x00BFFFFF')
            mb.byte_access_address('0x20000000','0x201FFFFF')
            self.MEMORY_BLOCKS['L2 RAM'] = mb

            mb = ldf_memory_block(debug=self.debug)
            mb.long_word_address  ('0x00000000','0x00000000')
            mb.ISA_code_address   ('0x00000000','0x00000000')
            mb.normal_word_address('0x00000000','0x00000000')
            mb.short_word_address ('0x00000000','0x00000000')
            mb.byte_access_address('0x80000000','0xbfffffff')
            self.MEMORY_BLOCKS['DDR RAM'] = mb

        if is_py2:
            self.names = sorted(self.MEMORY_BLOCKS.keys(), cmp=lambda x, y: cmp(self.MEMORY_BLOCKS[x].BW.start, self.MEMORY_BLOCKS[y].BW.start))
        else:
            self.names = sorted(self.MEMORY_BLOCKS.keys(), key=lambda x: self.MEMORY_BLOCKS[x].BW.start)

        self.count = len(self.MEMORY_BLOCKS)

        self.scan_map_xml(xml_root)

    def get_memory_blocks_count(self):
        return self.count

    def get_memory_block_by_name(self, name):
        if name in self.names:
            return self.MEMORY_BLOCKS[name]
        else:
            return None

    def get_memory_block_at_index(self, idx):
        if idx < self.count:
            return self.MEMORY_BLOCKS[self.names[idx]]
        else:
            return None

    def get_memory_blocks(self):
        return self.names

    def myprint(self):
        for name in self.names:
            print (name)
            self.MEMORY_BLOCKS[name].myprint()

    def scan_map_xml(self, xml_root):
        self.root = xml_root
        for mb in self.get_memory_blocks():
            if self.debug:
                print (mb)
            self.MEMORY_BLOCKS[mb].scan_memory_sections_tree(self.root)
            self.MEMORY_BLOCKS[mb].add_cache_block(self.root)
            self.MEMORY_BLOCKS[mb].set_finalized()

    def memory_breakdown(self):
        memory_totals = OrderedDict()
        for name in self.MEMORY_BLOCKS.keys():
            memory_totals[name] = self.MEMORY_BLOCKS[name].memory_breakdown()
        return memory_totals

class ldf_root_img:
    def __init__(self, root, img, debug = False):
        self.root = root
        self.img = img

    def get_lp_root(self):
        return self.root

    def get_img(self):
        return self.img


#*********************************************************************************
# Function: parse XML file
#
#*********************************************************************************
def parseMemXML(xmlPath, classifierGroup = 'JTest', debug = False, fl_path=None, classifiers=None):
    global g_fileGroupClassifier
    global g_memoryGroupClassifier
    root_imgs = []

    if ((classifiers is not None)
        and (isinstance(classifiers, dict))
        and ('flieGroupClassifer' in classifiers) and (classifiers['flieGroupClassifer'])
        and ('memoryGroupClassifer' in classifiers) and (classifiers['memoryGroupClassifer'])):
        g_fileGroupClassifier = classifiers['flieGroupClassifer']
        g_memoryGroupClassifier = classifiers['memoryGroupClassifer']
    else:
        if classifierGroup == 'JTest':
            g_fileGroupClassifier = FileGroupClassifier('JTest', fl_path)
            g_memoryGroupClassifier = MemoryGroupClassifier('JTest')
        elif classifierGroup == 'BS':
            g_fileGroupClassifier = FileGroupClassifier('BS')
            g_memoryGroupClassifier = MemoryGroupClassifier('JTest')
        elif classifierGroup == 'Vega':
            g_fileGroupClassifier = FileGroupClassifier('Vega')
            g_memoryGroupClassifier = MemoryGroupClassifier('JTest')
        elif classifierGroup == 'Melville':
            g_fileGroupClassifier = FileGroupClassifier('Melville', fl_path)
            g_memoryGroupClassifier = MemoryGroupClassifier('JTest')
        elif classifierGroup == 'Spetter':
            g_fileGroupClassifier = FileGroupClassifier('Spetter')
            g_memoryGroupClassifier = MemoryGroupClassifier('JTest')
        elif classifierGroup == 'MARS':
            g_fileGroupClassifier = FileGroupClassifier('MARS')
            g_memoryGroupClassifier = MemoryGroupClassifier('MARS')
        else:
            return None

    xml_tree = et.parse(xmlPath)
    xml_root = xml_tree.getroot()

    for lp_root in xml_root.iter('LINKPROJECT'):
        proc = lp_root.attrib["architecture"]
        img = ldf_memory_blocks(proc, lp_root, debug = debug)

        root_img = ldf_root_img(lp_root, img)
        root_imgs.append(root_img)

    return root_imgs
