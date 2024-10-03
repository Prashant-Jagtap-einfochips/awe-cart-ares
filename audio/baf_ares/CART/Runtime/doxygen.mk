#
#   BOSE CORPORATION
#   COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
#   This program may not be reproduced, in whole or in part in an
#   form or any means whatsoever without the written permission of:
#
#       BOSE CORPORATION
#       The Mountain
#       Framingham, MA 01701-9168

# This is one the global variables that should be defined in each make modules
ifndef doxygen_input
doxygen_input := 
endif

ifndef SVN
  SVN := svn
endif

# Include Makefile with files to be doxygenized
include doc/$(DOXY_PROJECT)/$(DOXY_PROJECT).make


# doxygen
doxygen_dir := doc/$(DOXY_PROJECT)/doxygen
DOXYGEN := /usr/bin/doxygen
doxygen_defines += \
	__attribute__(x)= \
	__declspec(x)=
doxygen_output_directory := doc/$(DOXY_PROJECT)
doxygen_processed := $(doxygen_output_directory)/doxygen_ts
doxygen_project_name     := $(DOXY_PROJECT)
doxygen_project_number   := Rev $(shell svn info | grep "^Revision:" | \
                           sed -e 's/Revision\:\s*//')
doxygen_project_logo     := $(doxygen_dir)/resources/Bose_Logo_Black_Small.png
doxygen_template_file    := $(doxygen_dir)/doxyfile.template
doxygen_image_path       := $(doxygen_dir)/resources
doxygen_log_file         := $(doxygen_output_directory)/doxygen_log.txt
doxygen_input      += $(doxygen_dir)/mainpage.dox
tag_file           := $(doxygen_output_directory)/$(DOXY_PROJECT).tag
DOXYGEN_GENERATED_FILES := $(doxygen_log_file) \
                           $(doxygen_processed)

PHONY_TARGETS := all

all: $(doxygen_processed)

# Rule to process doxygen target
$(doxygen_processed): $(doxygen_input) $(doxygen_template_file)
	@echo "Generating Doxygen documents..."
	@(cat $(doxygen_template_file); \
	echo "PROJECT_NAME = $(doxygen_project_name)"; \
	echo "PROJECT_NUMBER = $(doxygen_project_number)"; \
	echo "OUTPUT_DIRECTORY = $(doxygen_output_directory)"; \
	echo "PROJECT_LOGO = $(doxygen_project_logo)"; \
	echo "INPUT = $(doxygen_input)"; \
	echo "STRIP_FROM_PATH = $(doxygen_strip_from_path)"; \
	echo "PREDEFINED = $(doxygen_defines)"; \
	echo "INCLUDE_PATH = $(doxygen_include_paths)"; \
	echo "IMAGE_PATH = $(doxygen_image_path)"; \
	echo "EXAMPLE_PATH = $(doxygen_image_path)"; \
	echo "WARN_LOGFILE = $(doxygen_log_file)"; \
	echo "GENERATE_TAGFILE = $(tag_file)"; \
	) | $(DOXYGEN) - ;
	@date > $@;

	
lint:

.PHONY: $(PHONY_TARGETS)


