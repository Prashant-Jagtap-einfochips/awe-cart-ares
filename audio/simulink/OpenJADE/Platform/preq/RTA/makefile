#----------------------------------------------------------------------------
# make all = Make software.
# make clean = Clean out built project files.
#----------------------------------------------------------------------------

# Target file name (without extension).
#TARGET = BasicAudio
ifndef TARGET
$(error Usage: make TARGET=name)
endif


# Architecture settings
ARCH=IA32
UNAME_P=$(shell uname -p)
ARPREQ=$(shell (cat /opt/version_major))
ifeq ($(UNAME_P),x86_64)
	 ARCH=AMD64
endif
ifneq ($(filter arm%,$(UNAME_P)),)
	 ARCH=ARMV7
endif
ifneq ($(filter aarch%,$(UNAME_P)),)
	 ARCH=ARMV8
endif

ifeq "$(ARCH)" "IA32"
	CC = gcc
	CFLAGS = -I$(IPPROOT)/include -I$(MKLROOT)/include
	LDFLAGS = -L$(IPPROOT)/lib/ia32 -L$(MKLROOT)/lib/ia32  -lipps -lippvm -lippcore -lBAF
endif

ifeq "$(ARCH)" "AMD64"
	ifneq "$(wildcard /opt/intel/system_studio_*/compilers_and_libraries/linux/bin/intel64/icc)" ""
		CC = icc
	else
		CC = gcc
	endif
	CFLAGS = -I$(IPPROOT)/include -I$(MKLROOT)/include
	LDFLAGS = -L$(IPPROOT)/lib/intel64 -L$(MKLROOT)/lib/intel64 -lipps -lippvm -lippcore -Wl,--no-as-needed -lmkl_core -lmkl_intel_lp64 -lmkl_sequential
endif

ifeq "$(ARCH)" "ARMV7"
	CC = gcc
	CFLAGS = -mfloat-abi=hard -mfpu=neon -march=armv7-a -funroll-loops --param max-unroll-times=2
	LDFLAGS = -L$(HOME)/src/preq6/linux/pq_lib/armv7 -mfloat-abi=hard -mfpu=neon -lNE10
endif

ifeq "$(ARCH)" "ARMV8"
	CC = gcc
	CFLAGS = -march=armv8-a -funroll-loops --param max-unroll-times=2
	LDFLAGS = -L$(HOME)/src/preq6/linux/pq_lib/armv8 -lNE10
endif


#---------------- Compiler Options ----------------
#VCSDEF = -D'VCS_REV="$(shell svnversion -n .)"'
#VCSDEF = -D'VCS_REV="$(shell (hostname; git show-ref -s --abbrev $(shell git branch --no-color 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/\1/')))"'
#VCSDEF = -D'VCS_REV="$(shell (git rev-list --all --count))-$(shell (git describe --long --tags --dirty --always))"'
VCSDEF = -D'VCS_REV="1.00"'
PREQDEF = -D'ARPREQ=$(shell (cat /opt/version_major))'
CCDEF = -D'CC="$(shell ($(CC) --version | grep ^$(CC)))"'
CFLAGS += -g -xc
CFLAGS += -std=gnu99 -Wall -Wa,-adhlns=$(<:.c=.lst) -Wno-unused-variable -Wno-format-security
CFLAGS += -D$(ARCH) $(CCDEF) $(VCSDEF) $(PREQDEF)
CFLAGS += -DMODEL=$(TARGET) -I../pq_lib -I. -I./$(TARGET)_ert_shrlib_rtw -I./BAF

#---------------- Linker Options ----------------
LDFLAGS += -L$(HOME)/models/build/testfft_build_baf -lm -lpthread -lrt -ljack -lconfig -lsndfile -lasound -lfftw3 -lBAF
LDFLAGS += -Wl,-Map=$(TARGET).map


#---------------- Build -------------------------
src = $(wildcard *.c)
src += $(wildcard ./$(TARGET)_ert_shrlib_rtw/*.c)
obj = $(src:.c=.o)
lst = $(src:.c=.lst)
dep = $(obj:.o=.d)


all: $(TARGET).elf

.SECONDARY : $(TARGET).elf
.PRECIOUS : $(obj)
%.elf: $(obj)
	@echo "Linking:" $@
	$(CC) $^ -o $(TARGET).elf $(LDFLAGS)
	mv $@ ../$(TARGET)

%.o : %.c
	@echo "Compiling:" $<
	$(CC) $(CFLAGS) -MP -MMD -c $< -o $@

-include $(dep)

.PHONY: clean
clean :
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).sym $(TARGET).lss $(TARGET).bin
	rm -f $(obj)
	rm -f $(dep)
	rm -f $(lst)
	rm -f *.gdb *~
