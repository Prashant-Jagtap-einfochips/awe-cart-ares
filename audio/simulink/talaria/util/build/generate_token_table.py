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

from argparse import ArgumentParser
from top import TOPInfo

class TokenTable():
    def __init__(self, top_info_file, dsp_symbol_table):
        self.top_info = TOPInfo(top_info_file)
        self.dsp_symbol_table = dsp_symbol_table
        self.debug = 0
        
    def write(self):
        '''
        Write top list to end of the table. Note that the TOPs are Simulink objects, 
        not memory addresses so they will have some special fields
        '''
        text = ""
        for key in self.top_info.tops:
            text += key + ', N/A, N/A, Simulink object, '
            text += self.top_info.tops[key]['writeToken'] + ', N/A\n'

        if self.debug:
            print "%s" % text
        else:
            with open(self.dsp_symbol_table, 'a') as fh:
                fh.write(text)
        
if __name__ == '__main__':
    ''' 
    Appends TOPs from a model to the DSP symbol table
    '''
    # Grab the input arguments
    parser = ArgumentParser()
    parser.add_argument('-d', dest='dsp_symbol_table', help='DSP symbol table name with addresses')
    parser.add_argument('-t', dest='top_info_xml_file', help='TOP file with data in xml format')
    args = parser.parse_args()

    if not args.top_info_xml_file:
        raise NameError('Must pass in file with TOP info')
    if not args.dsp_symbol_table:
        raise NameError('Must pass in the DSP symbol table')

    token_table = TokenTable(args.top_info_xml_file, args.dsp_symbol_table)
    token_table.write()

