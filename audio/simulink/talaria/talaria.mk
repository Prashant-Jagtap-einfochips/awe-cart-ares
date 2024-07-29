#   BOSE CORPORATION
#   COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
#   This program may not be reproduced, in whole or in part in an
#   form or any means whatsoever without the written permission of:
#
#       BOSE CORPORATION
#       The Mountain
#       Framingham, MA 01701-9168
#
# Description: Include-makefile for building/testing Talaria core components
#              as part of larger project
#
# NOTE:        Variables in ALL CAPS must be set by the Makefile included by
#              this file

################################################################################
# Matlab info
################################################################################

matlab_version      := 2022B
matlab              := "$(shell which -a matlab | grep -i $(matlab_version))"

################################################################################
# Code gen build vars
################################################################################

MODEL_CFG_DIR       ?= $(MODEL_DIR)
matlab_args         := -batch
build_error_log     := $(TALARIA_ROOT_DIR)/build_error.log
witness_file        := .talaria_build
talaria_prj_file    := Talaria.prj
test_witness_file   := .talaria_test

# These need to be set at the project level!
# MODEL_NAME          :=
# PROJECT_FILE        :=
# PROJECT_DIR         := 

# Do not include prerequisite files from the "code" subdirectory
m_files             := $(shell find . -path */code/* -prune -o -name '*.m' -type f -print)
model_files         := $(shell find . -name '*.slx' -type f)
tlc_files           := $(shell find . -name '*.tlc' -type f)
code_gen_deps       := $(m_files) $(model_files) $(tlc_files)
BUILD_MODEL_PARALLEL ?= true

################################################################################
# Files and folders that need to be cleaned
################################################################################

generated_files     += $(matlab_build_log) $(matlab_test_log) $(witness_file) \
                       $(test_witness_file)

################################################################################
# Recipes
################################################################################

.PHONY: clean test

$(witness_file): $(code_gen_deps) $(SYS_DEFS)
	@echo "Generating code for $(MODEL_NAME)"
	@$(matlab) $(matlab_args) "try build_model_cmdline('$(PROJECT_FILE)','$(MODEL_NAME)','$(BUILD_MODEL_PARALLEL)');catch ME;bdclose all;rethrow(ME);end;"
	@if [ "$$?" -eq 0 ]; then touch $@; fi;


test: $(test_witness_file)

$(test_witness_file): $(code_gen_deps) $(SYS_DEFS)
	@echo "Generating test code for $(MODEL_NAME)"
	@$(matlab) $(matlab_args) "try build_test_model_cmdline('$(PROJECT_FILE)','$(MODEL_NAME)');catch ME;bdclose all;rethrow(ME);end;"
	@if [ "$$?" -eq 0 ]; then touch $@; fi;


clean:
	@echo "Cleaning.."
	@rm -f $(generated_files)
	@rm -rf $(code_gen_dir) $(model_parts_dir)
