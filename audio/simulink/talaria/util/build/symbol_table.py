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
This is used to create a DSP symbol table.  They utilize ELFParser to grab info
from an ELF file and write the data to a text file.  
fixed address

'''

from ELFParser import ELFParser, Variable
from argparse import ArgumentParser

class SymbolTable(object):
    def __init__(self, output_filename):
        self.output_filename = output_filename
        self.extern_symbols = []
        self.elfs = []
        
    def add_elf(self, elf_filename, pid=0):
        elf = ELFParser(elf_filename)
        self.elfs.append(elf)
        variables = elf.get_extern_variables()
        for v in variables:
            symbol = Symbol(v, pid)
            self.extern_symbols.append(symbol)
                                   
    def write(self):
        debug = 1
        if debug:
            for v in self.extern_symbols:
                print v.__str__()
        else:
            f = open(self.output_filename, 'w')
            for v in self.extern_symbols:
                f.write(v.__str__() + "\r\n")
    
class Symbol(Variable):
    def __init__(self, var, pid=0):
        self.die = var.die
        self.name = var.name
        self.typ = var.typ
        self.address = var.address
        self.token = var.address
        self.size = var.size
        self.num_elements = var.num_elements
        self.offset = var.offset
        self.processor_id = pid

    def __str__(self):
        ''' print override function.  prints in DSP_symbol_table format '''
        comma = ', '
        value = '0x0'

        if self.num_elements > 1:
            typ = '[' + str(self.num_elements) + '] ' + self.typ
        else:
            typ = self.typ
        string = self.name + comma + str(self.processor_id) + comma
        string += str(self.size) + comma + typ + comma
        string += "0x%X" % self.token + comma + value
        return string

