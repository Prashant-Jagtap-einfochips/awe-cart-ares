import os
import subprocess



COMPILER_PATH = "C:\Analog Devices\CrossCore Embedded Studio 2.9.1"
COMPILER_ASM = "easm21k.exe"
COMPILER_C = "cc21k.exe"
COMPILER_ASM_FLAG = "-proc ADSP-21569 -si-revision any -char-size-8 -swc -o"
COMPILER_C_FLAG = "-DPRELOAD_CODE -c -proc ADSP-21569 -si-revision any -double-size-32 -char-size-8 -swc -O -Ov100"
COMPILER_DXE1_FLAG = "-proc ADSP-21569 -si-revision any"
COMPILER_DXE2_FLAG = "-no-mem -flags-link -ip -flags-link -MDCORE0,-MDRELEASE -flags-link -e -o"

STARTUP_PATH = "../startup_ldf"
PARENT_PATH = "../"
PRELOAD_PATH = "./"
CURRENT_PATH = "./"
LDF_PATH = "../startup_ldf/app.ldf"
PRELOAD_NAME = "preload.dxe"

ASSEMBLY_FILE = ".s"
DOJ_FILE = ".doj"
XML_FILE = ".xml"
C_FILE = ".c"


# Do compiling in folder that includes both .c and .s files
def compile_file_in_directory(src_path):
    files_to_compile = os.listdir(src_path)
    for f in files_to_compile:
        run_cmd = ""
        name, ext = os.path.splitext(f)
        if ext == ASSEMBLY_FILE:
            run_cmd = '"' + COMPILER_PATH + "\\" + COMPILER_ASM + '"'
            run_cmd += " " + COMPILER_ASM_FLAG
            run_cmd += " " + CURRENT_PATH + name + DOJ_FILE
            run_cmd += " " + src_path + "/" + f
            print run_cmd
            result = subprocess.check_output(run_cmd, stderr = subprocess.STDOUT, universal_newlines = True)
        elif ext == C_FILE:
            run_cmd = '"' + COMPILER_PATH + "\\" + COMPILER_C + '"'
            run_cmd += " -o " + CURRENT_PATH + name + DOJ_FILE
            run_cmd += " " + COMPILER_C_FLAG
            run_cmd += " " + src_path + "/" + f
            print run_cmd
            result = subprocess.check_output(run_cmd, stderr = subprocess.STDOUT, universal_newlines = True)


# Do compiling out of this folder, like adi_initialize.c and heapstack_usage.c, pinmux_config.c
def compile_specified_file():
    for f in ['adi_initialize.c', 'heapstack_usage.c', 'pinmux_config.c']:
        raw_name = f[:-2]
        run_cmd = '"' + COMPILER_PATH + "\\" + COMPILER_C + '"'
        run_cmd += " -o " + CURRENT_PATH + raw_name + DOJ_FILE
        run_cmd += " " + COMPILER_C_FLAG
        run_cmd += " " + PARENT_PATH + f
        print run_cmd
        result = subprocess.check_output(run_cmd, stderr = subprocess.STDOUT, universal_newlines = True)


# Do compiling in folder that includes .c files only and generate .dxe file
def compile_preload_dxe(ldf_path, doj_path):
    run_cmd = '"' + COMPILER_PATH + "\\" + COMPILER_C + '"'
    run_cmd += " " + COMPILER_DXE1_FLAG
    run_cmd += " -T" + ldf_path
    run_cmd += " " + COMPILER_DXE2_FLAG
    run_cmd += " " + PRELOAD_NAME + " *" + DOJ_FILE
    print run_cmd
    result = subprocess.check_output(run_cmd, stderr = subprocess.STDOUT, universal_newlines = True)
    del_cmd = "rm " + "*" + DOJ_FILE + " " + "*" + XML_FILE
    result = subprocess.check_output(del_cmd, stderr = subprocess.STDOUT, universal_newlines = True)



compile_file_in_directory(STARTUP_PATH)
compile_file_in_directory(PRELOAD_PATH)
compile_specified_file()
compile_preload_dxe(LDF_PATH, PRELOAD_PATH)


print "Pre-loader compiling done."