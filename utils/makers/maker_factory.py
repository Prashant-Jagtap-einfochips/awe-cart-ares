"""
maker_factory.py

Description: 
    Class definition for MakerFactory, which returns
    maker-specific object for given build process
"""
from makers import matlab_awe
from makers import win32

class MakerFactory:
    def __init__(self, args):
        self.args = args

    def get_maker(self, build_type):
        # MATLAB maker 
        if build_type == 'matlab':
            return matlab_awe.MatlabAWEMaker(self.args)
        # Win32 (x86) maker
        elif build_type == 'win32':
            return win32.PCMaker(self.args)
        else:
            # Throw error if invalid build type is provided
            raise ValueError(f"Error: unknown build type - {build_type}")