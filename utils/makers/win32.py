"""
win32.py

Description: 
    Class definition for Windows (x86) build with CMake 
    build support
"""

import os
import glob
import shutil

import makers.maker as maker

defaults = {
    'pc_build_preset': 'win32-rel',
    'vs_tool_ver': 'vc142',
    'awe_loc': 'C:\\DSP Concepts\\AWE Designer 8.D.2.3 Pro\\'
}

def define_common_args(parser):
    for arg, def_val in defaults.items():
        parser.add_argument('--' + arg, type=str, default=def_val)

class PCMaker(maker.Maker):
    MSBUILD_EXE = 'MSBuild.exe'

    def __init__(self, args):
        self.name = type(self).__name__
        
        self.verify_args(args)

        self.build_preset = args.pc_build_preset
        self.tool_ver = args.vs_tool_ver
        self.awe_loc = args.awe_loc
        self.dbg = False

    def pre_build(self):
        print('\n********************************************************************************' +
              '\n                              Win32 (x86) Build                                 ' +
              '\n********************************************************************************\n')

    def build_cmd(self, dbg=False):
        # Determine if build with release or debug based on debug flag
        if dbg:
            print("Building with Win32 debug configuration")
            self.build_preset = 'win32-dbg'
            self.dbg = True
        else:
            print("Building with Win32 release configuration")
            self.build_preset = 'win32-rel'
        
        # Change working directory to PROJECT_ROOT 
        # (CMake needs to be run from root to call preset)
        cwd = os.getcwd()
        os.chdir(maker.PROJECT_ROOT)
        print(maker.PROJECT_ROOT)
        print('Changed working directory to ' + os.getcwd())
        
        # Run CMake configuration command first
        self.cmd = 'cmake --preset=' + self.build_preset
        self.run_cmd()
        
        # Build with CMake
        self.cmd = 'cmake --build --preset=' + self.build_preset
        self.run_cmd()
        
        # Set cmd to empty and go back to initial working directory
        self.cmd = ''
        os.chdir(cwd)
        print('Changed working directory to ' + os.getcwd())
        
        
    def post_build(self):
        # Copy DLL to AWE directory
        build_path = os.path.join(maker.PROJECT_ROOT, 'out', 'build', 
                                  self.build_preset)
        
        if self.dbg:
            src_dir = 'Debug'
            dst_dir = 'win32-' + self.tool_ver + '-dbg'
        else:
            src_dir = 'Release'
            dst_dir = 'win32-' + self.tool_ver + '-rel'
            
        src_dll = glob.glob(os.path.join(build_path, src_dir, '*.dll'))
        
        # Determine if valid DLL exists
        if src_dll:
            src_dll = src_dll[0]
        else:
            # Throw error if no valid DLL is found
            raise ValueError("Error: No valid DLL found") 
        
        # AWE workspace path
        dst_dll = os.path.join(self.awe_loc, 'Bin', dst_dir)
        
        # Check for valid AWE workspace path
        if os.path.exists(dst_dll):
            dst_dll = os.path.join(dst_dll, os.path.basename(src_dll))
        else:
            raise ValueError("Error: Invalid AWE workspace path")
        
        if os.path.exists(dst_dll):
            os.remove(dst_dll)
            print("Removed existing DLL file")
        
        shutil.copyfile(src_dll, dst_dll)
        print("Copied DLL from " + src_dll + " to " + dst_dll)