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

import filecmp
from pathlib import Path
import shutil
import tempfile
from unittest.mock import patch

import pytest

import float_types_alignment as service

THIS_SCRIPT_DIR = Path(__file__).resolve().parent
TEST_DIR = THIS_SCRIPT_DIR / "testdir"
INPUT_DIR = TEST_DIR / "input"
EXPECTED_DIR = TEST_DIR / "expected"


def test_main_func_e2e():
    # given
    model_target_h = "Model_Target.h"
    post_process_h = "PostProcess.h"
    pool_delay_storage_h = "PoolDelayStorage.h"
    model_target_top_h = "Model_Target_TOP.h"
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_dir = Path(temp_dir) / "temp"
        with patch("float_types_alignment.MODEL_DIR", temp_dir):
            shutil.copytree(INPUT_DIR, temp_dir)
            # when
            service.float_types_alignment("Model", None, None, None, None)

            # then
            test_target_dir = temp_dir / "code" / "Model_Target_ert_shrlib_rtw"
            assert filecmp.cmp(
                test_target_dir / model_target_h, EXPECTED_DIR/ model_target_h
            )
            assert filecmp.cmp(
                test_target_dir / post_process_h, EXPECTED_DIR / post_process_h
            )
            assert filecmp.cmp(
                test_target_dir / pool_delay_storage_h, EXPECTED_DIR / pool_delay_storage_h
            )
            assert filecmp.cmp(
                test_target_dir / model_target_top_h, EXPECTED_DIR / model_target_top_h
            )
            
        
@patch("float_types_alignment.HEXAGON_SDK_ROOT", "WRONG_DEFINED_HEXAGON_SDK_ROOT")
def test_get_cpp_path_raise_when_hexagon_sdk_root_not_defined(capfd):
    # given
    log_msg = "WRONG_DEFINED_HEXAGON_SDK_ROOT environment variable is not defined! Please run \
setup_sdk_env.cmd from the HEXAGON_SDK directory in this session to set the environment or pass \
the path to HEXAGON_SDK directory.\n"
    with pytest.raises(SystemExit) as err:
        # when
        service.get_cpp_path("")
    
    out, _ = capfd.readouterr()
    # then
    assert log_msg == out 
    assert err.type == SystemExit
    assert err.value.code == 1
    
    
def test_get_hexagon_tools_versions_raise_when_no_versions():
    # given
    hexagon_tools_path = THIS_SCRIPT_DIR
    log_msg = f"There is no any version for Hexagon tools in: {hexagon_tools_path}"
    with pytest.raises(FileNotFoundError) as err:
        # when
        service.get_hexagon_tools_versions(hexagon_tools_path)
        
    # then
    assert str(err.value) == log_msg
    

@patch("float_types_alignment.SCRIPT_DIR", Path("Non_existing_dir"))
def test_main_func_raise_when_no_config():
    # given
    log_msg = "alignment_config.json file was not found. Please make sure that you have this \
config file in the same directory as this script."
    with tempfile.TemporaryDirectory() as temp_dir:
        temp_dir = Path(temp_dir) / "temp"
        shutil.copytree(INPUT_DIR, temp_dir)
        with pytest.raises(FileNotFoundError) as err:
            # when
            service.float_types_alignment("Model", None, None, None, None)
            
    # then
    assert str(err.value) == log_msg
    
    
def test_main_func_raise_when_wrong_model_path_passed():
    # given
    code_dir = INPUT_DIR / "code"
    log_msg = f"There are no any directories created for the model with name Non_existing_model in {code_dir}."
    with patch("float_types_alignment.MODEL_DIR", INPUT_DIR):
        with pytest.raises(FileNotFoundError) as err:
            # when
            service.float_types_alignment("Non_existing_model", None, None, None, None)
            
    # then
    assert str(err.value) == log_msg
    