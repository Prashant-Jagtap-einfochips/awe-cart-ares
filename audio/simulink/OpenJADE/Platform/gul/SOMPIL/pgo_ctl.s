
.section/DOUBLE32 seg_sdram;

.align 64;
.global _executable_name.;
.type _executable_name.,STT_OBJECT;
.byte _executable_name.[] =
    'Baf_pgo.dxe',0;
._executable_name..end:

.section/DOUBLE32/NO_INIT seg_sdram_noinit_data;
.align 128;
   .byte __pgo_hw_LDF_start.[256*1024];
   __pgo_hw_LDF_end.:
   .global __pgo_hw_LDF_start.;
   .global __pgo_hw_LDF_end.;

