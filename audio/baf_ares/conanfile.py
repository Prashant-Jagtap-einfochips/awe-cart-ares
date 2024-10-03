from cart.conan import CART_ConanFile as ConanFile
from conan.tools.files import copy
from conan.tools.scons import SConsDeps

import os
import subprocess
from io import StringIO
import fnmatch
import re

def findNPrefix(name_space, target_dir, replace_list):
    # print(f'Info: scanning {target_dir} ...')
    all_files = os.listdir(target_dir)
    src_files = fnmatch.filter(all_files, '*.c') + fnmatch.filter(all_files, '*.h')

    for src_file in src_files:
        file_path = os.path.join(target_dir, src_file)
        #print(f'\tInfo: processing {src_file} ...')

        with open(file_path) as f:
            s = f.read()

        for replace_item in replace_list:
            s = re.sub(fr'([\s#-/]){replace_item}\b', rf'\1{name_space}_{replace_item}', s)

        with open(file_path, "w") as f:
            f.write(s)

def findNReplace(file_name, replace_list):
        #print(f'\tInfo: processing {file_name} ...')

        with open(file_name) as f:
            s = f.read()

        for replace_item in replace_list:
            s = re.sub(f'{replace_item[0]}', f'{replace_item[1]}', s)
        with open(file_name, "w") as f:
            f.write(s)

def findNRename(name_space, target_dir, replace_list):
    for duplicate_file in replace_list:
        if os.path.isfile(target_dir + '/'+ duplicate_file):
            if os.path.isfile(target_dir + '/'+ name_space + duplicate_file):
                os.remove(target_dir + '/'+ name_space + duplicate_file)
            os.rename(target_dir + '/'+ duplicate_file, target_dir + '/'+ name_space+duplicate_file)

class CART_Conan(ConanFile):
    def build(self):
        config = self.get_config()
        dep = config # config dependency
        if isinstance(config, dict):
            dep=list(config.values())[0]
            config=list(config.keys())[0]
        arch = self.get_arch(dep)
        def build_helper(config, dep, arch):
            #patch
            duplicates=['rt_hypotf',
                        'div_nde_s32_floor',
                        'TspDispatchTable0000Count',
                        'TspDispatchTable0000',
                        'GetPartitionRoot',
                        'RegisterPartition',
                        'GetPartitionRootByIndex',
                        'TalariaAsyncRoutingTable'
                        ]
            find_replace = [
                ('int8_T SFunction_pInputBuffer_i\[8\];',
                 'int8_T SFunction_pInputBuffer_i[8]  __attribute__((aligned(4)));')
                ]
            duplicate_files=['PoolDelayStorage.c',
                             'rtmodel.c',
                             'FFTBuffers.c',
                             'FirStorage.c',
                             'ifir.c',
                             ]
            code_folder = f'out/{dep}/slx/code/'
            findNReplace(code_folder + 'Model_1_2_ert_shrlib_rtw/Model_1_2.h', find_replace)
            partition_list = os.listdir(code_folder)
            for partition in partition_list:
                if partition.endswith('ert_shrlib_rtw'):
                    findNPrefix(partition[:10], code_folder + partition, duplicates)
                    findNRename(partition[:10], code_folder + partition, duplicate_files)

            # build release
            run_cmd = 'scons --test=%s --arch=%s' % (config, arch)
            if arch in self.conan_data['tools_path']:
                run_cmd += ' --tools="%s"' % self.replace_macro(self.conan_data['tools_path'][arch], os.environ)
            run_cmd += ' --model=no  --app=no '
            self.run( run_cmd )

            # build dependency
            run_cmd = 'scons --test=%s --arch=%s' % (dep, arch)
            if arch in self.conan_data['tools_path']:
                run_cmd += ' --tools="%s"' % self.replace_macro(self.conan_data['tools_path'][arch], os.environ)
            run_cmd += ' --model=no  --app=no '
            self.run( run_cmd )

        if config is None: return
        if isinstance(arch, list):
            for _arch in arch:
                build_helper(config, dep, _arch)
        else:
            build_helper(config, dep, arch)

    def generate(self):
        for dep in self.dependencies.values():
            if  dep.ref.name.startswith('cart'):
                copy(self, "*.*", dep.package_folder, self.build_folder+'/CART')
            elif dep.ref.name.startswith('cfg'):
                copy(self, '*.py',dep.package_folder, self.build_folder)
            else:
                copy(self, "*.*", dep.package_folder, self.build_folder)
            
    def package(self):
        config = self.get_config()
        dep = config # config dependency
        if isinstance(config, dict):
            dep=list(config.values())[0]
            config=list(config.keys())[0]
        arch = self.get_arch(config)
        arch_dep = self.get_arch(dep)
        if config is None: return
        def package_helper(config, arch):
            bf = self.build_folder+'/out/%s/%s' % (config, arch)
            baf_folder = bf+'/lib/BAF/'
            baf_list = os.listdir(baf_folder)
            if baf_list is not None:
                for baf in baf_list:
                    copy(self,'*.lib',baf_folder, self.package_folder+'/BAF/lib', keep_path=False)
                    copy(self,'*.a'  ,baf_folder, self.package_folder+'/BAF/lib', keep_path=False)
            copy(self,'*.json' ,bf+'/../slx', self.package_folder+'/BAF/cfg', keep_path=False)
            copy(self,'*.yml'  ,bf+'/../slx', self.package_folder+'/BAF/cfg', keep_path=False)
            bfc = bf+'/src/Bcf'
            bfi = bf+'/inc'
            dll_folder=bf+'/obj/Platform/'
            dll_list = os.listdir(dll_folder) if os.path.exists(dll_folder) else None
            if dll_list is not None:
                for dll in dll_list:
                    platform = os.listdir(dll_folder+dll)
                    copy(self,'*.*',dll_folder+dll+'/'+platform[0], self.package_folder+'/BAF/dll', keep_path=False)
            copy(self,'OJ*.*',bfc+'/inc' ,           self.package_folder+'/FeatureLibAuto/inc', keep_path=False)
            copy(self,'OJ*.*',bfc+'/src' ,           self.package_folder+'/FeatureLibAuto/src', keep_path=False)
            copy(self,'OJ*.*',bfc+'/Bam_Proxys/inc', self.package_folder+'/FeatureLibAuto/Bam_Proxys/inc', keep_path=False)
            copy(self,'OJ*.*',bfc+'/Bam_Proxys/src', self.package_folder+'/FeatureLibAuto/Bam_Proxys/src', keep_path=False)
            copy(self,'OJ*.*',bfc+'/Features/inc' ,  self.package_folder+'/FeatureLibAuto/Features/inc', keep_path=False)
            copy(self,'OJ*.*',bfc+'/Features/src' ,  self.package_folder+'/FeatureLibAuto/Features/src', keep_path=False)
            copy(self,'*.h'  ,bfi,                   self.package_folder+'/FeatureLib/inc', keep_path=False)
            copy(self,'*.h',self.build_folder+'/CART/Runtime/BAF', self.package_folder+'/BAF/include', keep_path=False)
            copy(self,'BAF_Dll.h',self.build_folder+'/CART/Module/dll_baf', self.package_folder+'/BAF/include', keep_path=False)

        # package both current and dependency
        if isinstance(arch, list):
            for _arch in arch:
                package_helper(config, _arch)
        else:
            package_helper(config, arch)

        if isinstance(arch_dep, list):
            for _arch in arch_dep:
                package_helper(dep, _arch)
        else:
            package_helper(dep, arch_dep)
    
    def package_info(self):
        self.cpp_info.includedirs = ['BAF/include']
        self.cpp_info.libdirs =     ['BAF/lib']
        self.cpp_info.bindirs =     ['BAF/dll']
        self.cpp_info.resdirs =     ['BAF/cfg']
        self.cpp_info.srcdirs =     []

    def layout(self):
        self.folders.source = "."
        self.folders.build =  "."
        pass
