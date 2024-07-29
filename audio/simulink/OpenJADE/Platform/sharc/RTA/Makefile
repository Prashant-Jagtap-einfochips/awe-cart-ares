CC = cc21k.exe
AS = easm21k.exe
AR = cc21k.exe
LD = cc21k.exe

ARMCC = arm-none-eabi-gcc
ARMLD = arm-none-eabi-gcc
ARMOBJ = .o
ARMEXE = _out

OBJ = .doj
EXE = .dxe
MAP = .map.xml

ARMCFLAGS = -g -gdwarf-2 -ffunction-sections -fdata-sections
ARMCFLAGS += -Wall -mproc=ADSP-SC584 -msi-revision=1.0
ARMCFLAGS += -Icore0/system

ARMLFLAGS = -mproc=ADSP-SC584 -msi-revision=1.0 -Wl,--gc-sections
ARMLFLAGS_END = -lm

BUILD_DIR?=build

INT_DIR_CORE0=$(BUILD_DIR)/obj/core0
INT_DIR_CORE2=$(BUILD_DIR)/obj/core2
OUT_DIR=.

LDF_FILE = system/startup_ldf/app.ldf

CORE = -swc -proc ADSP-SC584 -si-revision 1.0 -double-size-32 -char-size-8

CFLAGS = -O -Ov100 -structs-do-not-overlap -no-multiline -no-const-strings $(CORE)
CFLAGS += -g -warn-protos
CFLAGS_CORE2 = $(CFLAGS) -Icore2/system

AFLAGS = $(CORE)
AFLAGS += -isystem

LDFLAGS = $(CORE) -flags-link -e -flags-link -ip -map $(@:$(EXE)=$(MAP))

LDFLAGS_CORE2 = $(LDFLAGS) -flags-link -MDOTHERCORE,-MDDEBUG,-MDCORE2 


CORE0_OBJS = $(INT_DIR_CORE0)/adi_initialize$(ARMOBJ) \
    $(INT_DIR_CORE0)/sru_config$(ARMOBJ) \
	$(INT_DIR_CORE0)/initialize_cores$(ARMOBJ)
    
CORE2_OBJS = $(INT_DIR_CORE2)/empty_main$(OBJ)
    
CORE0_BINS = $(OUT_DIR)/core0$(ARMEXE)
CORE2_BINS = $(OUT_DIR)/core2$(EXE)

OBJS = $(CORE0_OBJS) $(CORE2_OBJS)

BINS = $(CORE0_BINS) $(CORE2_BINS)

.PHONY: all
all: mkdir $(OBJS) $(BINS)

$(INT_DIR_CORE0)/adi_initialize$(ARMOBJ): core0/system/adi_initialize.c
	$(ARMCC) $(ARMCFLAGS) -c $< -o $@
    
$(INT_DIR_CORE0)/sru_config$(ARMOBJ): core0/system/sru/sru_config.c
	$(ARMCC) $(ARMCFLAGS) -c $< -o $@
    
$(INT_DIR_CORE0)/initialize_cores$(ARMOBJ): core0/src/initialize_cores.c
	$(ARMCC) $(ARMCFLAGS) -c $< -o $@

$(OUT_DIR)/core0$(ARMEXE): $(CORE0_OBJS)
	$(ARMLD) $(ARMLFLAGS) $^ -o $@ $(ARMLFLAGS_END)

$(INT_DIR_CORE2)/empty_main$(OBJ): core2/src/empty_main.c
	$(CC) $(CFLAGS_CORE2) -c $< -o $@
    
$(OUT_DIR)/core2$(EXE): $(CORE2_OBJS)
	$(LD) $(LDFLAGS_CORE2) $^ -o $@

.PHONY: mkdir
mkdir:
	@-mkdir -p $(INT_DIR_CORE0)
	@-mkdir -p $(INT_DIR_CORE2)
	@-mkdir -p $(OUT_DIR)
	
.PHONY: clean
clean:
	rm -f $(OBJS) $(BINS)
	rm -f $(BINS:$(EXE)=$(MAP))
	rm -f $(OUT_DIR)/linker_log.xml
	rm -f *.dxe.asm *.dxe.doj

