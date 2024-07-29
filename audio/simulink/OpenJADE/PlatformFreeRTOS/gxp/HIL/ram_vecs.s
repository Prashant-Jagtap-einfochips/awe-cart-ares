
.section/DOUBLE32 seg_sdram;
.align 64;
#ifdef RAM_TEST
.INC/BINARY data_request_hex.[] = "request.ojx";
#else
.BYTE data_request_hex.[] = 10,10,10,10;
#endif
.global data_request_hex.;

.section/DOUBLE32 seg_sdram;
.align 64;
#ifdef RAM_TEST
.INC/BINARY data_in_binary.[] = "audioIn.bin";
#else
.BYTE data_in_binary.[64*1024];
#endif
.global data_in_binary.;

.section/DOUBLE32 seg_sdram;
.align 64;
#ifdef RAM_TEST
.INC/BINARY data_gold_binary.[] = "audioRef.bin";
#else
.BYTE data_gold_binary.[64*1024];
#endif
.global data_gold_binary.;

.section/DOUBLE32 seg_sdram;
.align 64;
#ifdef RAM_TEST
.INC/BINARY data_out_binary.[] = "audioRef.bin";
#else
.BYTE data_out_binary.[128*1024];
#endif
.global data_out_binary.;

.section/DOUBLE32 seg_sdram;
.align 64;
.BYTE data_response_hex.[128*1024];
.global data_response_hex.;

.section/DOUBLE32 seg_sdram;
.align 64;
.global file_request_hex.;
.type file_request_hex.,STT_OBJECT;
.byte file_request_hex.[] =
    'request.ojx',0;
.file_request_hex..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global file_in_binary.;
.type file_in_binary.,STT_OBJECT;
.byte file_in_binary.[] =
    'audioIn.bin',0;
.file_in_binary..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global file_out_binary.;
.type file_out_binary.,STT_OBJECT;
.byte file_out_binary.[] =
    'audioOut.bin',0;
.file_out_binary..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global file_gold_binary.;
.type file_gold_binary.,STT_OBJECT;
.byte file_gold_binary.[] =
    'audioRef.bin',0;
.file_gold_binary..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global file_response_hex.;
.type file_response_hex.,STT_OBJECT;
.byte file_response_hex.[] =
    'response.ojx',0;
.file_response_hex..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global g_ram_files.;
.type g_ram_files.,STT_OBJECT;
g_ram_files.:
    .var = file_request_hex.;
    .var = data_request_hex.;
    .var = 0;
    .var = LENGTH(data_request_hex.);
    .var = LENGTH(data_request_hex.);

    .var = file_response_hex.;
    .var = data_response_hex.;
    .var = 0;
    .var = 0;
    .var = LENGTH(data_response_hex.);
    
    .var = file_in_binary.;
    .var = data_in_binary.;
    .var = 0;
    .var = LENGTH(data_in_binary.);
    .var = LENGTH(data_in_binary.);

    .var = file_out_binary.;
    .var = data_out_binary.;
    .var = 0;
    .var = 0;
    .var = LENGTH(data_out_binary.);

    .var = file_gold_binary.;
    .var = data_gold_binary.;
    .var = 0;
    .var = LENGTH(data_gold_binary.);
    .var = LENGTH(data_gold_binary.);
.g_ram_files..end:

.section/DOUBLE32 seg_sdram;
.align 64;
.global g_RAMIO_FILE_CNT.;
.type g_RAMIO_FILE_CNT.,STT_OBJECT;
.var g_RAMIO_FILE_CNT. = 5;
.g_RAMIO_FILE_CNT..end:


