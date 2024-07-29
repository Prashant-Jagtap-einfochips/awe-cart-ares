#
# BOSE CORPORATION
# COPYRIGHT 2024 BOSE CORPORATION ALL RIGHTS RESERVED
# This program may not be reproduced, in whole or in part in any
# form or any means whatsoever without the written permission of:
#
#     BOSE CORPORATION
#     The Mountain
#     Framingham, MA 01701-9168
#

import argparse
import json
import logging
import os
from pathlib import Path
import re
from sys import platform
from typing import List, Dict, Optional

import pycparser as pp

logging.getLogger().setLevel(logging.INFO)

SCRIPT_DIR = Path(__file__).resolve().parent
MODEL_DIR = Path("/".join(SCRIPT_DIR.parts[:-3])) / "application" / "model"

HEXAGON_SDK_ROOT = "HEXAGON_SDK_ROOT"
ALIGNMENT = "__attribute__ ((__aligned__(8)))"
ALIGNMENT_SYNTAXES = ["__attribute((aligned(8)))", ALIGNMENT]


class UndefVarClassError(Exception):
    pass


def get_hexagon_tools_versions(hexagon_tools_path: Path) -> Optional[List[Path]]:
    hexagon_tools_versions = [
        version
        for version in hexagon_tools_path.glob("*")
        if re.match(r"[0-9]\.[0-9]\.[0-9]{2}", version.name)
    ]
    if not hexagon_tools_versions:
        raise FileNotFoundError(f"There is no any version for Hexagon tools in: {hexagon_tools_path}")
    return hexagon_tools_versions


def get_cpp_path(hexagon_sdk_root: Optional[str]) -> str:
    try:
        hexagon_sdk_root = Path(os.environ[HEXAGON_SDK_ROOT])
    except(KeyError):
        if hexagon_sdk_root:
            hexagon_sdk_root = Path(hexagon_sdk_root)
        else:
            print(f"{HEXAGON_SDK_ROOT} environment variable is not defined! Please run \
setup_sdk_env.cmd from the HEXAGON_SDK directory in this session to set the environment or pass \
the path to HEXAGON_SDK directory.")
            exit(1)
    
    hexagon_tools_path = hexagon_sdk_root / "tools" / "HEXAGON_Tools"
    hexagon_tools_versions = get_hexagon_tools_versions(hexagon_tools_path)
    
    preprocessor_file_name = "hexagon-clang-cpp"
    if platform == "win32":
        preprocessor_file_name = preprocessor_file_name + ".exe"
    return str(hexagon_tools_versions[0] / "Tools" / "bin" / preprocessor_file_name)
    

def find_files_with_lines_to_align(
    model_path: Path,
    funcs_names: List[str],
    funcs_args_idxs: List[List[int]],
    blocklist_c_files: List[str],
    hexagon_sdk_root: str,
) -> Dict[str, Dict[int, str]]:
    cpp_path = get_cpp_path(hexagon_sdk_root)
    
    files_lines_to_align_map = {}
    for c_path in model_path.glob("*.c"):
        if c_path.stem not in blocklist_c_files:
            c_parsed_stmts = pp.parse_file(
                str(c_path),
                use_cpp=True,
                cpp_path=cpp_path,
                cpp_args=[
                    "-D__fp16=float",
                    "-D__inline=",
                    "-D__attribute__(x)=",
                    "-D__attribute(x)=",
                    f"-I{SCRIPT_DIR}/pycparser/utils/fake_libc_include",
                ],
            ).ext
            func_defs_stmts = [
                stmt.body.block_items
                for stmt in c_parsed_stmts
                if isinstance(stmt, pp.c_ast.FuncDef)
            ]
            for func_name, func_args_idxs in zip(funcs_names, funcs_args_idxs):
                for func_def_stmts in func_defs_stmts:
                    if func_def_stmts:
                        for stmt in func_def_stmts:
                            if isinstance(stmt, pp.c_ast.FuncCall) and stmt.name.name == func_name:
                                args_to_align = [
                                    stmt.args.exprs[idx].expr.name
                                    for idx in func_args_idxs
                                ]

                                for arg_to_align in args_to_align:
                                    if isinstance(arg_to_align, pp.c_ast.StructRef):
                                        struct_var_name = arg_to_align.name.name
                                        struct_field_name = arg_to_align.field.name

                                        struct_var_decl = [
                                            stmt
                                            for stmt in c_parsed_stmts
                                            if isinstance(stmt, pp.c_ast.Decl)
                                            and stmt.name == struct_var_name
                                        ][0]
                                        typedef_name = struct_var_decl.type.type.names[0]
                                        typedef_decl = [
                                            stmt
                                            for stmt in c_parsed_stmts
                                            if isinstance(stmt, pp.c_ast.Typedef)
                                            and stmt.name == typedef_name
                                        ][0]
                                        struct_fields = typedef_decl.type.type.decls
                                        var_decl = [
                                            decl
                                            for decl in struct_fields
                                            if decl.name == struct_field_name
                                        ][0]
                                    elif isinstance(arg_to_align, pp.c_ast.ID):
                                        var_name = arg_to_align.name
                                        var_decl = [
                                            decl
                                            for decl in c_parsed_stmts
                                            if isinstance(decl, pp.c_ast.Decl)
                                            and decl.name == var_name
                                        ][0]
                                    else:
                                        raise UndefVarClassError(f"There is not defined behaviour for class: {arg_to_align}")

                                    file_name = var_decl.coord.file
                                    line_num = var_decl.coord.line
                                    var_type = var_decl.type.type.type.names[0]

                                    if file_name not in files_lines_to_align_map:
                                        files_lines_to_align_map[file_name] = {}
                                    if line_num not in files_lines_to_align_map[file_name]:
                                        files_lines_to_align_map[file_name][line_num] = var_type

    return files_lines_to_align_map


def align_file_types(file_path: Path, lines_types_map: Dict[int, str]) -> None:
    with open(file_path, "r") as c_file:
        code_lines = c_file.readlines()

    with open(file_path, "w") as c_file:
        for i, line in enumerate(code_lines, 1):
            if i in lines_types_map:
                var_type = lines_types_map[i]
                if all(alignment_syntax not in line for alignment_syntax in ALIGNMENT_SYNTAXES):
                    line = re.sub(var_type, f"{ALIGNMENT} {var_type}", line)
            c_file.write(line)


def float_types_alignment(
    model_name: Optional[str],
    funcs_names: Optional[List[str]],
    funcs_args_idxs: Optional[List[List[int]]],
    blocklist_c_files: Optional[List[str]],
    hexagon_sdk_root: Optional[str],
) -> None:
    config_file = SCRIPT_DIR / "alignment_config.json"
    if not config_file.is_file():
        raise FileNotFoundError("alignment_config.json file was not found. Please make sure that \
you have this config file in the same directory as this script.")
    with open(config_file) as json_file:
        config = json.load(json_file)

    model_name = model_name if model_name else config["model_name"]
    code_dir = MODEL_DIR / "code"
    model_code_subdirs = [subdir for subdir in code_dir.glob("*") if subdir.is_dir()]
    model_atomic_list_file_path = MODEL_DIR / f"{model_name}_atomicList.m"
    if model_atomic_list_file_path.is_file():
        logging.info(
            f"{model_atomic_list_file_path.name} file found. Running alignment for partitioned code."
        )
        model_code_subdirs = [
            subdir
            for subdir in model_code_subdirs
            if re.match(f"{model_name}_[0-9]+_[0-9]+_ert_shrlib_rtw", subdir.stem)
        ]
    else:
        logging.info(
            f"{model_atomic_list_file_path.name} file not found. Running alignment for non-partitioned code."
        )
        model_code_subdirs = [
            subdir
            for subdir in model_code_subdirs
            if subdir.stem == f"{model_name}_Target_ert_shrlib_rtw"
        ]

    if not model_code_subdirs:
        raise FileNotFoundError(
            f"There are no any directories created for the model with name {model_name} in {code_dir}."
        )
    
            
    funcs_names = funcs_names if funcs_names else config["funcs_names"]
    funcs_args_idxs = (
        funcs_args_idxs if funcs_args_idxs else config["funcs_args_idxs"]
    )
    blocklist_c_files = (
        blocklist_c_files if blocklist_c_files else config["blocklist_c_files"]
    )

    for model_path in model_code_subdirs:
        files_lines_to_align_map = find_files_with_lines_to_align(
            model_path,
            funcs_names,
            funcs_args_idxs,
            blocklist_c_files,
            hexagon_sdk_root,
        )

        for file_path, lines_types_map in files_lines_to_align_map.items():
            align_file_types(Path(file_path), lines_types_map)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-model_name", help="Model name.")
    parser.add_argument(
        "-funcs_names",
        nargs = "+",
        help = "Names of the functions to have float types aligned. They can be given in any order.",
    )
    parser.add_argument(
        "-funcs_args_idxs",
        nargs = "+",
        action = "append",
        type = int,
        help = "Indexes of the arguments to be aligned of the functions passed in funcs_names. \
                Indexes are numbered from 0 and should be passed as list of lists (one list \
                corresponds to one function). Thus, indexes should be passed as follows(example): \
                -funcs_args_idxs 5 2 -funcs_args_idxs 1 9 0",
    )
    parser.add_argument(
        "-blocklist_c_files",
        nargs = "+",
        help = "List of c files, that will not be checked for alignment.",
    )
    parser.add_argument(
        "-hexagon_sdk_root",
        help="If you do not have Hexagon SDK environment set up, then you need to pass the path to \
              Hexagon_SDK directory.",
    )
    args = parser.parse_args()

    float_types_alignment(
        args.model_name, args.funcs_names, args.funcs_args_idxs, args.blocklist_c_files, args.hexagon_sdk_root,
    )
