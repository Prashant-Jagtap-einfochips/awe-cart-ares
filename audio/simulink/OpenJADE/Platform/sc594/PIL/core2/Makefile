CC = cc21k.exe
AS = easm21k.exe
AR = cc21k.exe
LD = cc21k.exe

OBJ = .doj
EXE = .dxe
MAP = .map.xml

INT_DIR?=obj
OUT_DIR=..

LDF_FILE = app.ldf

CORE = -swc -proc ADSP-SC594W -si-revision any -double-size-32 -char-size-8

CFLAGS = -O -Ov100 -structs-do-not-overlap -no-multiline -no-const-strings $(CORE)
CFLAGS += -g -warn-protos
CFLAGS += -DCORE2

AFLAGS = $(CORE) -g

LDFLAGS = $(CORE) -flags-link -e -flags-link -ip -map $(@:$(EXE)=$(MAP))
LDFLAGS += -flags-link -MDCORE2,-MDOTHERCORE


OBJS = $(INT_DIR)/app_startup$(OBJ) $(INT_DIR)/app_IVT$(OBJ) $(INT_DIR)/app_heaptab$(OBJ) $(INT_DIR)/core2$(OBJ)
BINS = $(OUT_DIR)/core2$(EXE)

.PHONY: all
all: mkdir $(OBJS) $(BINS)

$(INT_DIR)/app_IVT$(OBJ): app_IVT.s
	$(AS) $(AFLAGS) -o $@ $<

$(INT_DIR)/app_startup$(OBJ): app_startup.s
	$(AS) $(AFLAGS) -o $@ $<

$(INT_DIR)/app_heaptab$(OBJ): app_heaptab.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INT_DIR)/core2$(OBJ): core2.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/core2$(EXE): $(OBJS) $(LDF_FILE)
	$(LD) $(LDFLAGS) -T $(LDF_FILE) $(OBJS) -o $@

.PHONY: mkdir
mkdir:
	@-mkdir -p $(INT_DIR)
	@-mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINS)
	rm -f $(BINS:$(EXE)=$(MAP))
	rm -f $(OUT_DIR)/linker_log.xml
	rm -f *.dxe.asm *.dxe.doj

