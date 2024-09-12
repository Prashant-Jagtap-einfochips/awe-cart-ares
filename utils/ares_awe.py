"""
ares_awe.py

Description: 
    Python tool for building AWE custom modules. Comprises:
        1. AWE code generation with MATLAB
        2. Build for Visual Studio (x86) with CMake
    
Usage:
python ares.py build_type [--clean] [--dbg]
"""

import os
import argparse
import sys

import commands.awe_runner as awe_runner

def handle_awe(args):
    if not args.clean:
        awe_runner.AWERunner(args).make()
    else:
        awe_runner.AWERunner(args).clean()

def main():
    print('\n********************************************************************************' +
          '\n                      ARES AWE Custom Module Build Tool                         ' +
          '\n********************************************************************************\n')
    
    parser_awe = argparse.ArgumentParser(
        description="Provides AWE custom module build tools for ARES project")

    ############################################
    # awe command
    parser_awe.add_argument('build_type', help='build type [all, matlab, win32]')
    parser_awe.add_argument('--clean', action='store_true', default=False, 
                            help='Clean artifacts')
    parser_awe.add_argument('--dbg', action='store_true', default=False, 
                            help='Build for debug')
    awe_runner.define_common_args(parser_awe)
    parser_awe.set_defaults(func=handle_awe)
    ############################################

    args, unknownargs = parser_awe.parse_known_args()
    
    if unknownargs:
        args.func(unknownargs)
    else:
        args.func(args)


if __name__ == '__main__':
    main()