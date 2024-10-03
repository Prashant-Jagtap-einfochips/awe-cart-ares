#ifndef __RAM_FILE__H__
#define __RAM_FILE__H__

#include <stddef.h>

typedef struct
{
   const char *name;       //name of the file
   unsigned char *buff;    //pointer to start of file
   int   pos;              //current location
   int   len;              //length of file (in bytes)
   int   max;              //maximum memory allocated
}RAMIO_FILE;

void ramio_files_init(void);
void ramio_files_close(void);
void ramio_files_shred(const char * fname);
unsigned char * ramio_file_start_addr(const char * fname);
int ramio_file_curr_pos(const char * fname);
int ramio_file_length(const char * fname);

RAMIO_FILE* RAMIO_fopen ( const char * filename, const char * mode );
int RAMIO_fclose ( RAMIO_FILE* stream );
size_t RAMIO_fwrite( const void * ptr, size_t size, size_t count, RAMIO_FILE* stream );
size_t RAMIO_fread( void *ptr, size_t size, size_t count, RAMIO_FILE* stream );
int RAMIO_fseek( RAMIO_FILE *stream, int offset, int origin );
char * RAMIO_fgets ( char * str, int num, RAMIO_FILE * stream );
int RAMIO_feof ( RAMIO_FILE * stream );
int RAMIO_fprintf ( RAMIO_FILE * stream, const char * format, ... );

#endif //__RAM_FILE__H__

