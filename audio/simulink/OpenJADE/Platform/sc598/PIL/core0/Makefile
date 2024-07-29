CC = aarch64-none-elf-gcc.exe
AS = aarch64-none-elf-gcc.exe
AR = aarch64-none-elf-gcc.exe
LD = aarch64-none-elf-gcc.exe

OBJ = .out
EXE =
MAP = .map.xml

INT_DIR?=obj
OUT_DIR=..

LD_FILE = app.ld

CORE = -mproc=ADSP-SC598W -msi-revision=any

CFLAGS = -O -Ofast -gdwarf-2 -ffunction-sections -fdata-sections -c $(CORE)
CFLAGS += -g
CFLAGS += -DCORE0

AFLAGS = $(CORE) -g

LDFLAGS = $(CORE) -Wl,--gc-sections -mdebug-libs -lm -Wl,-Map=$(@:$(EXE)=$(MAP))

OBJS = $(INT_DIR)/apt$(OBJ) $(INT_DIR)/app_heap_stack$(OBJ) $(INT_DIR)/core0$(OBJ)
BINS = $(OUT_DIR)/core0out$(EXE)

.PHONY: all
all: mkdir $(OBJS) $(BINS)

$(INT_DIR)/apt$(OBJ): apt.c
	$(CC) $(CFLAGS) -o $@ $<

$(INT_DIR)/app_heap_stack$(OBJ): app_heap_stack.c
	$(CC) $(CFLAGS) -c $< -o $@

$(INT_DIR)/core0$(OBJ): core0.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/core0out$(EXE): $(OBJS) $(LDF_FILE)
	$(LD) $(LDFLAGS) -T $(LD_FILE) $(OBJS) -o $@

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

