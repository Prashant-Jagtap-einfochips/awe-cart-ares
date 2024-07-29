#include <stdint.h>
#include "ram_file.h"

__attribute__((section(".oj_ext_data")))  unsigned char data_request_hex[] = {
   #include "request_ojx.h"
};

__attribute__((section(".oj_ext_data"))) unsigned char data_in_binary[] = {
   #include "audioIn_bin.h"
};

__attribute__((section(".oj_ext_data"))) unsigned char data_gold_binary[] = {
   #include "audioRef_bin.h"
};

__attribute__((section(".oj_ext_data"))) unsigned char data_out_binary[] = {
   #include "audioRef_bin.h"
};

__attribute__((section(".oj_ext_data"))) unsigned char data_response_hex[128*1024];

unsigned int data_out_length;
unsigned int data_response_length;

char file_request_hex[] = "request.ojx";
char file_in_binary[] = "audioIn.bin";
char file_out_binary[] = "audioOut.bin";
char file_gold_binary[] = "audioRef.bin";
char file_response_hex[] = "response.ojx";

RAMIO_FILE g_ram_files[] = {
   {
      file_request_hex,
      data_request_hex,
      0,
      sizeof(data_request_hex)/sizeof(unsigned char),
      sizeof(data_request_hex)/sizeof(unsigned char)
   },
   {
      file_response_hex,
      data_response_hex,
      0,
      0,
      sizeof(data_response_hex)/sizeof(unsigned char)
   },
   {
      file_in_binary,
      data_in_binary,
      0,
      sizeof(data_in_binary)/sizeof(unsigned char),
      sizeof(data_in_binary)/sizeof(unsigned char)
   },
   {
      file_out_binary,
      data_out_binary,
      0,
      0,
      sizeof(data_out_binary)/sizeof(unsigned char)
   },
   {
      file_gold_binary,
      data_gold_binary,
      0,
      sizeof(data_gold_binary)/sizeof(unsigned char),
      sizeof(data_gold_binary)/sizeof(unsigned char)
   }
};

uint32_t g_RAMIO_FILE_CNT = 5;
