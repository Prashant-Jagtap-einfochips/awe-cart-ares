"""
awe_runner.py

Description: 
    Class definition for AWERunner, which defines make and
    clean functions for different build types
"""
from makers.maker_factory import MakerFactory

from makers import matlab_awe
from makers import win32

def define_common_args(parser):
    matlab_awe.define_common_args(parser)
    win32.define_common_args(parser)

class AWERunner:
    # In order makers
    ALL_MAKERS = ['matlab', 'win32']

    def __init__(self, args):
        self.build_type = args.build_type
        self.args = args
        self.maker_factory = MakerFactory(self.args)
        
    def make(self):
        print(f'Running build process for {self.build_type} step(s)\n')
        if self.build_type == 'all':
            for maker in self.ALL_MAKERS:
                self.maker_factory.get_maker(maker).make(self.args.dbg)
        else:
            self.maker_factory.get_maker(self.build_type).make(self.args.dbg)

    def clean(self):
        print(f'Cleaning build artifacts from {self.build_type} step(s)\n')
        if self.build_type == 'all':
            for maker in self.ALL_MAKERS:
                out = maker in 'win32'
                self.maker_factory.get_maker(maker).clean(out)
        elif self.build_type == 'win32':
            # Remove CMake out directory by setting 'out' to TRUE
            self.maker_factory.get_maker(self.build_type).clean(True)
        else:
            self.maker_factory.get_maker(self.build_type).clean()