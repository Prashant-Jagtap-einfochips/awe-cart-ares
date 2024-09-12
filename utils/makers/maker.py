"""
maker.py

Description: 
    Base class definition for building for a specific target or
    configuration for AWE custom module
"""
import subprocess
import os
import glob
import shutil

# Project root directory
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(__file__)))

class Maker:
    artifact_dirs = None
    artifacts = None
    out = 'out'

    def verify_args(self, args):
        pass

    def pre_build(self):
        pass

    def build_cmd(self, dbg=False):
        pass

    def run_cmd(self):
        print(self.cmd)
        out = subprocess.Popen(self.cmd,
                               shell=True,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT)
        for line in out.stdout.readlines():
            line = line.decode('utf-8').strip('\r\n')
            print(line)

    def post_build(self):
        pass

    ############################################
    # Make command
    def make(self, dbg=False):
        # Prebuild setup
        self.pre_build()
    
        # Get build commands and build accordingly
        self.build_cmd(dbg)
        self.run_cmd()

        # Run post build
        self.post_build()

    # Clean command
    def clean(self, out=False):
        # Remove out directory containing CMake build artifacts
        if out:
            out = os.path.join(PROJECT_ROOT, self.out)
            if os.path.exists(out):
                print("Cleaning " + out)
                shutil.rmtree(out)

        if self.artifact_dirs:
            for artifact in self.artifacts:
                print(f"Cleaing build artifact: {artifact}")
                for artifact_dir in self.artifact_dirs:
                    for f in glob.glob(os.path.join(PROJECT_ROOT, 
                                                    artifact_dir, 
                                                    artifact)):
                        print("Removing " + f)
                        os.remove(f)