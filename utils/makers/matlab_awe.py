"""
matlab_awe.py

Description: 
    Class definition for MATLAB code generation
"""
import os
import glob

from makers import maker

defaults = {
    #'matlab_loc': 'C:\\Program Files\\MATLAB\\R2022b\\',
    'matlab_loc': '/mnt/c/Program Files/MATLAB/R2022b/',
    'make_name': 'make_TestAudio_module_pack'
}

def define_common_args(parser):
    for arg, def_val in defaults.items():
        parser.add_argument('--' + arg, type=str, default=def_val)

class MatlabAWEMaker(maker.Maker):
    # Will assume a default AWE package
    MATLAB_EXE = 'bin/matlab.exe'
    
    def __init__(self, args):
        self.name = type(self).__name__
        
        self.verify_args(args)

        # Get MATLAB exe location
        self.matlab = args.matlab_loc
        
        # Get MATLAB make script name
        self.make_name = args.make_name
        
        # Determine if valid MATLAB make script exists
        if not os.path.isfile(os.path.join(maker.PROJECT_ROOT, 
                                           'matlab', self.make_name + '.m')):
            # Throw error if no valid MATLAB make script is found
            raise ValueError("Error: No valid MATLAB make script found")

    def pre_build(self):
        print('\n********************************************************************************' +
              '\n                      AWE C Code Generation with MATLAB                         ' +
              '\n********************************************************************************\n')

    # Build command for AWE code generation using MATLAB
    def build_cmd(self, dbg=False):
        cmd = ''
        cmd += '"' + os.path.join(self.matlab, self.MATLAB_EXE) + '"'
        cmd += ' -batch '
        cmd += '" '
        cmd += 'addpath(\'C:/DSP Concepts/AWE Designer 8.D.2.6 Pro/matlab\'); '
        cmd += 'awe_init; '
        cmd += 'addpath(\'C:/data/work/develop-ares/matlab\'); '
        cmd += self.make_name + '; '
        cmd += 'awe_server_command(\'exit\'); '
        cmd += 'quit force; '
        cmd += '"'
        self.cmd = cmd

