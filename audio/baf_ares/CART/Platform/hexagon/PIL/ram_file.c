#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include "ram_file.h"

/* From ram_vecs.c */
extern uint32_t g_RAMIO_FILE_CNT;
extern RAMIO_FILE g_ram_files[];
extern unsigned int data_out_length;
extern unsigned int data_response_length;

void ramio_files_shred(const char * fname)
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
      {
         uint32_t *ptr = (uint32_t *)g_ram_files[i].buff;
         uint32_t n;
         for (n=0;n<(g_ram_files[i].max/sizeof(uint32_t));n++)
            *ptr++ = 0xDEADBEEF;
         break;
      }
   }
   return;
}

unsigned char * ramio_file_start_addr(const char * fname)
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
      {
         return g_ram_files[i].buff;
      }
   }
   return (unsigned char *)0;
}

int ramio_file_curr_pos(const char * fname)
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
      {
         return g_ram_files[i].pos;
      }
   }
   return -1;
}

int ramio_file_length(const char * fname)
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
      {
         return g_ram_files[i].len;
      }
   }
   return -1;
}

void ramio_files_init(void)
{
	return;
}

void ramio_files_close(void)
{
   data_out_length = ramio_file_length("audioOut.bin");
   data_response_length = ramio_file_length("response.ojx");
   return;
}

RAMIO_FILE* RAMIO_fopen ( const char * fname, const char * mode )
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
         return &g_ram_files[i];
   }
   return (RAMIO_FILE*)NULL;
}

int  RAMIO_fclose( RAMIO_FILE* st )
{
   st->pos = 0;
   return 0;
}

size_t RAMIO_fread( void *buffer, size_t _size, size_t _count, RAMIO_FILE* st )
{
   int size = _size*_count;
   unsigned char * dest = (unsigned char *)buffer;
   unsigned char * src = (unsigned char *)&st->buff[st->pos];
   int count = (size<(st->len-st->pos))?(size):(st->len-st->pos);

   if (count <= 0)
      return 0;

   memcpy(dest, src, count);

   st->pos += count;

   return (size_t)(count/_size);
}

size_t RAMIO_fwrite( const void * buffer, size_t _size, size_t _count, RAMIO_FILE* st )
{
   int size = _size*_count;
   unsigned char * src = (unsigned char *)buffer;
   unsigned char * dest = (unsigned char *)&st->buff[st->pos];
   int count = size;

   if (count <= 0)
      return 0;

   st->pos += count;
   st->len = (st->len>st->pos)?st->len:st->pos;
   if (st->len > st->max)
      return -1;
   memcpy(dest, src, count);
   return (size_t)(count/_size);
}

int  RAMIO_fseek  (RAMIO_FILE* st, long offset, int origin)
{
   if (origin == SEEK_SET)
      st->pos = offset;
   else if (origin == SEEK_CUR)
      st->pos += offset;
   else if (origin == SEEK_END)
      st->pos = st->len + offset;
   else
      return -1;

   return 0;
}

/*
The C library function char *fgets(char *str, int n, FILE *stream) 
reads a line from the specified stream and stores it into the string 
pointed to by str. 
It stops when either (n-1) characters are read, 
the newline character is read, or the end-of-file is reached, 
whichever comes first.
*/
char * RAMIO_fgets ( char * buffer, int num, RAMIO_FILE * st )
{
   int size = num - 1;
   int n;
   unsigned char * dest = (unsigned char *)buffer;
   unsigned char * src = (unsigned char *)&st->buff[st->pos];
   int count = (size<(st->len-st->pos))?(size):(st->len-st->pos);

   if (count <= 0)
      return (char *)NULL;

   for (n=0;n<count;n++)
   {
      unsigned char ch = *src++;
      *dest++ = ch;
      if (ch == '\n')
         break;
   }
   *dest = 0;

   st->pos += (n+1);

   return buffer;
}

int RAMIO_feof(RAMIO_FILE* st)
{
   if (st->pos >= st->len)
      return 1;
   else
      return 0;
}

int RAMIO_fprintf ( RAMIO_FILE * st, const char * fmt, ... )
{
   char * str = (char *)&st->buff[st->pos];
   int n = (st->max-st->pos);
   int count;
   va_list args;
   va_start( args, fmt );
   count = vsnprintf( str, n, fmt, args );
   va_end( args );
   st->pos += count;
   st->len = (st->len>st->pos)?st->len:st->pos;
   return count;
}


