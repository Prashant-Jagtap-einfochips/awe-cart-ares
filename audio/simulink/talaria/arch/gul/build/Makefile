#
#   BOSE CORPORATION
#   COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
#   This program may not be reproduced, in whole or in part in an
#   form or any means whatsoever without the written permission of:
#
#       BOSE CORPORATION
#       The Mountain
#       Framingham, MA 01701-9168
#
################################################################################

################################################################################
# Set default recipe name to be filled out after including sub-makefile
################################################################################

profile:

################################################################################
# Set model variables and pull in auto-generated makefile. 
# MODEL_NAME should be passed in via make
# (e.g. $ make MODLE_NAME=test_cces)
################################################################################

MODEL_NAME          := test_cces
model_makefile      := $(addsuffix .mk,$(MODEL_NAME))

include $(model_makefile)

################################################################################
# Define source file(s) to bee added to create full dxe
################################################################################

profiler_src        := test.c
profiler_config_h   := ProfileConfig.h
profiler_obj        := $(patsubst %.c, %.doj, $(profiler_src))
profiler_ldf        := $(patsubst %.c, %.ldf, $(profiler_src))
profiler_exe        := $(addsuffix .dxe,$(MODEL_NAME))

EXTRA_CFLAGS         = 
CFLAGS              += -DDO_CYCLE_COUNTS $(EXTRA_CFLAGS)

################################################################################
# Set dxe as dependent of profile recipe and add recipe to build it
################################################################################

profile : $(profiler_exe)

$(profiler_obj) : $(PRODUCT) $(profiler_src) $(profiler_config_h)

$(profiler_exe) : $(profiler_obj) $(PRODUCT) 
	@echo "### Creating standalone executable ..."
	$(LD) $(LDFLAGS) -flags-link -ip -flags-link -e -map $(MODEL_NAME).xml $^ -o $@ $(profiler_ldf) $(SYSTEM_LIBS) $(LIBS) $(TOOLCHAIN_LIBS)
	@echo "### Created dxe"
