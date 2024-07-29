#!/usr/bin/env python
###############################################################################
#
# BOSE CORPORATION
# COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
# This program may not be reproduced, in whole or in part in any
# form or any means whatsoever without the written permission of:
# BOSE CORPORATION
# The Mountain
# Framingham, MA 01701-9168
#
################################################################################

'''
This file contains classes used for grabbing information inside
an *.elf file.  The classes are mostly just wrappers around the pyelftools
python library, which can be found here:

https://svn.bose.com/asd/Gates/tools/trunk/pyelftools-master.zip

'''
from elftools.elf.elffile import ELFFile
from copy import copy, deepcopy
import re

# Macro defitions for this module
bytes_per_word = 4 
tag_header = 'DW_TAG_'
att_header = 'DW_AT_'
variable_type = 'variable'
member_type = 'member'
struct_type = 'structure'
array_type = 'array'
pointer_type = 'pointer'
base_type = 'base'
type_str = att_header + 'type'
name = att_header + 'name'
extern = att_header + 'external'
sibling = att_header + 'sibling'
location = att_header + 'location'
member_location = att_header + 'data_member_location'
size = att_header + 'byte_size'
upper_bound = att_header + 'upper_bound'
count = att_header + 'count'

class ELFParser():
    ''' 
    Generic class used to parse and ELF file. Currently the functions it 
    supports is mainly for generating a DSP symbol table, though much more 
    functionality can be added.

    This class uses the pyelftools library:
    https://github.com/eliben/pyelftools
    '''
    def __init__(self, fname, regex=".*"):
        ''' 
        fname - elf file name
        regex - regular expression for determining compile units to include
        '''
        self._elffile = ELFFile(open(fname, 'rb'))
        self._regex = regex
        self._dwarfinfo = self._elffile.get_dwarf_info()
        self._src_files = []
        self._variables = []

    def get_extern_variables(self, expand_structs=True, expand_arrays=False):
        ''' 
        Gets all external variables found in the compile units matching the
        regular express.  Expands structs to inlude lower level variables
        '''
        if not self._src_files:
            self._init_src_files()
        for obj in self._src_files:
            self._variables.extend(obj.get_extern_variables(expand_structs, expand_arrays))
        return self._variables

    def _init_src_files(self):
        '''
        Create the list of compile units (source files) to look through
        '''
        sed = re.compile(self._regex)
        for CU in self._dwarfinfo.iter_CUs():
            die = CU.get_top_DIE()
            if sed.match(die.attributes[name].value):
                self._src_files.append(SourceFile(CU))


class SourceFile():
    '''
    Source file class is a wrapper for CompileUnit class in the 
    pyelftools.  The main knoledge added to a SourceFile object over a 
    CompileUnit is the index_map, which allows grabbing DIE's from the _dielist
    variable in the CompileUnit by the offset value pointers in other DIE's
    '''
    def __init__(self, cu):
        ''' cu is a CompileUnit object '''
        self._cu = cu
        self._variables = []
        self._index_map = {}

    def get_extern_variables(self, expand_structs=True, expand_arrays=False):
        ''' Gets all external variables in the compile unit '''
        if len(self._index_map) == 0:
            self.__init_index_map()
        self.__get_external_variables(self._cu.get_top_DIE())
        if expand_structs:
            self.__expand_structs()
        if expand_arrays:
            self.__expand_arrays()
        return self._variables

    def __init_index_map(self):
        ''' Creates the dictionary mapping offset values to dielist indexes '''
        for index,die in enumerate(self._cu._dielist):
            self._index_map[die.offset - die.cu.cu_offset] = index

    def __get_external_variables(self, die):
        ''' Grabs all the variables in the cu marked as external '''
        if self.__get_die_type(die) == variable_type:
            if extern in die.attributes:
                if die.attributes[extern].value:
                    self._variables.append(self.__die_to_Variable(die))
        for child in die.iter_children():
            self.__get_external_variables(child)

    def __expand_structs(self):
        ''' Adds lower level members of a struct to the external variable list '''
        for var in self._variables:
            if var.typ == struct_type:
                dieList = self.__get_type_branch(var.die)
                for die in dieList:
                    if self.__get_die_type(die) == struct_type:
                        for member in die.iter_children():
                            self._variables.append(self.__die_to_Variable(
                                member, var.name + '.', var.address))

    def __die_to_Variable(self, die, name_prefix='', address_offset=0):
        ''' Creates Variable object from a DIE object '''
        name = name_prefix + self._get_var_name(die)
        offset = self._get_var_address(die)
        address = offset + address_offset
        (typ, size, num_elements) = self._get_var_type_and_size(die)
        return Variable(die, name, typ, address, size, num_elements, offset)

    def _get_var_name(self, die):
        return die.attributes[name].value

    def _get_var_address(self, die):
        return self.__translate_address(die)

    def _get_var_type_and_size(self, die):
        ''' Gets the variable's type and size '''
        is_pointer = False
        num_elements = 1
        num_bytes = 0
        var_type = ''

        # Look through the branch list for indications of array or pointer
        die_list = self.__get_type_branch(die)
        for d in die_list:
            tag_type = self.__get_die_type(d)
            if tag_type == pointer_type:
                is_pointer = True
                if size in d.attributes:
                    num_bytes = d.attributes[size].value
                else:
                    num_bytes = 0
            elif tag_type == array_type:
                for child in d.iter_children():
                    if count in child.attributes:
                        num_elements = child.attributes[count].value
                    else:
                        num_elements = child.attributes[upper_bound].value+1

        # now check the tip of the branch to get the type
        final_type_die = die_list.pop()
        var_type = self.__get_die_type(final_type_die)
        if  var_type == base_type:
            var_type = final_type_die.attributes[name].value
            byte_size = final_type_die.attributes[size].value
            if num_elements > 1:
                num_bytes = num_elements * byte_size
            else:
                num_bytes = byte_size
        elif var_type == pointer_type:
            var_type = 'void'
            if num_elements > 1:
                num_bytes = num_elements * final_type_die.attributes[size].value
        elif var_type == struct_type:
            num_bytes = final_type_die.attributes[size].value
        if is_pointer:
            var_type += ' *'

        return (var_type, num_bytes, num_elements)

    def __get_die_type(self, die):
        ''' Scans the DIE tag and returns the type '''
        tag_type = []
        type_sed = re.compile(tag_header + "(?P<varType>[a-zA-Z0-9]+)(_type)?")
        type_search = type_sed.search(str(die.tag))
        if type_search:
            tag_type = type_search.group('varType')
        return tag_type
        
    def __get_type_branch(self, die):
        ''' Returns a list of DIEs that make up a variable's type '''
        die_list = [die]
        while ( type_str in die.attributes ):
            die_list.append(die)
            type_value = die.attributes[type_str].value
            die = self._cu._dielist[self._index_map[type_value]]
        die_list.append(die)
        return die_list

    def __translate_address(self, die):
        ''' 
        Takes a DIE as an input and decodes the address.
        e.g. DIE location = [3, 48, 56, 3, 0] 
        address = 0x00033860

        Struct member locations are a little more confusing.  They are stated as 
        offsets, and the offset nominclature is
        DW_AT_data_member_location: 2 byte block: 23 6c   = 108
        DW_AT_data_member_location: 3 byte block: 23 90 1 = 144
        DW_AT_data_member_location: 3 byte block: 23 90 2 = 272
        DW_AT_data_member_location: 3 byte block: 23 84 6 = 772

        It seems there is an implied "1" in the 2 byte block
        '''
        address = 0
        if self.__get_die_type(die) == member_type:
            raw_value = die.attributes[member_location].value[:]
            if len(raw_value) == 2:
                address = raw_value.pop()
            else:
                offset = raw_value.pop()
                address= raw_value.pop() + (offset - 1) * 128
        else:
            raw_value = die.attributes[location].value[:]
            byte_shift = 8 * (len(raw_value) - 2)

            while len(raw_value) > 1:
                address += raw_value.pop() * 2**byte_shift
                byte_shift -= 8
        return address

    def __expand_arrays(self):
        '''
        Stupid DSP symbol table enumerates each indes of an array up to 
        index 35, so this function does just that.
        '''
        max_index = 35 # don't ask me why...
        for var in self._variables:
            if var.num_elements > 1:
                num_bytes = var.size / var.num_elements
                for index in range(min(var.num_elements, max_index)):
                    name = var.name + '[' + str(index) + ']'
                    address = var.address + num_bytes * index
                    self._variables.append(Variable(var.die, name, var.typ, 
                                                    address, num_bytes))

class Variable(object):
    """ A variable class is a glorified struct holding DWARF info on a variable
    
    The variable class holds data about a variable and prints it in a format
    in DSP_Symbol_Table.txt.  Inputs are:
    die - pyelf die object
    name - variable name
    typ - variable type
    address - physical address in memory
    size - size of the variable
    num_elements - value indicating if an array, and if so how many indices
    """    

    def __init__(self, die, name, typ, address, size, num_elements=1, offset=0):
        """ Initializes an instance of Variable class """
        self.die = die           ##!< data information entry
        self.name = name         ##!< variable name
        self.typ = typ           ##!< variable type
        self.address = address   ##!< variable address
        self.size = size         ##!< variable size
        self.num_elements = num_elements ##!< number of elements
        self.offset = offset ##!< offset in bytes if struct member
        
    def __str__(self):
        ''' print override function.  prints in DSP_symbol_table format '''
        comma = ', '
        value = '0x0'

        if self.num_elements > 1:
            typ = '[' + str(self.num_elements) + '] ' + self.typ
        else:
            typ = self.typ
        string = self.name + comma
        string += str(self.size) + comma + typ + comma
        string += "0x%X" % self.address + comma + value
        return string
