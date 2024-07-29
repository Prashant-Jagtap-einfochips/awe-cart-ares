#if !defined(__ADSPSHARC__)
#error Error RAM based file I/O not supported on non GUL platforms
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <device.h>
#include <device_int.h>
#include <stdarg.h>
#include <stdint.h>

typedef struct
{
   const char *name;       //name of the file
   unsigned char *buff;    //pointer to start of file
   int   pos;              //current location
   int   len;              //length of file (in bytes)
   int   max;              //maximum memory allocated
}RAMIO_FILE;

extern uint32_t g_RAMIO_FILE_CNT;

extern RAMIO_FILE g_ram_files[];

#define RAMIO_DEVICEID   6

int   RAMIO_init  (struct DevEntry *dev);
int   RAMIO_open  (const char *name, int mode);
int   RAMIO_close (int fd);
int   RAMIO_write (int fd, unsigned char *buf, int size);
int   RAMIO_read  (int fd, unsigned char *buf, int size);
long  RAMIO_seek  (int fd, long offset, int whence);
int RAMIO_remove(const char *filename);
int RAMIO_rename(const char *oldname, const char *newname);
int RAMIO_ioctl(int fd, int request, va_list varg_list);

DevEntry_Extension RAMIO_DevEntry_Extension =
{
   DEVFLAGS_BYTEADDRESSED,
   NULL,
   NULL
};

DevEntry RAMIO_DevEntry =
{
	0,
	NULL,
	RAMIO_init,
	RAMIO_open,
	RAMIO_close,
	RAMIO_write,
	RAMIO_read,
	RAMIO_seek,
	dev_not_claimed,
	dev_not_claimed,
	dev_not_claimed, 	
   RAMIO_remove,
   RAMIO_rename,
   RAMIO_ioctl,
   &RAMIO_DevEntry_Extension,
};


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
	// Create new device ID.  The device ID is arbitrary and used globally in the application.
	int deviceID = 0;	
	
	RAMIO_DevEntry.DeviceID = RAMIO_DEVICEID;
	
	// Insert this new device into the device table.	
	deviceID = add_devtab_entry ( &RAMIO_DevEntry );
	
	if ( RAMIO_DEVICEID != deviceID )
	{
		printf("Error: Unable to register new STDIO Device Driver. Try another DeviceID\n");
		exit(-1);
	}
	else
		set_default_io_device( deviceID );

	return;
}

void ramio_files_close(void)
{
   return;
}

int RAMIO_init  ( struct DevEntry *dev )
{
   return 1;
}

int RAMIO_open( const char * fname, int mode )
{
   int i;
   for (i=0;i<g_RAMIO_FILE_CNT;i++)
   {
      if (strcmp(fname, g_ram_files[i].name) == 0)
      {
         if (mode == 0x0001)
         {
            /* if the request is for read mode for
               a file length of zero, return failure */
            if (g_ram_files[i].len == 0)
               return -1;
         }
         return i;
      }
   }
   return -1;
}

int  RAMIO_close( int fd )
{
   RAMIO_FILE * st = &g_ram_files[fd];
   st->pos = 0;
   return 0;
}

int   RAMIO_read  (int fd, unsigned char *buffer, int size)
{
   RAMIO_FILE * st = &g_ram_files[fd];
   unsigned char * dest = (unsigned char *)buffer;
   unsigned char * src = (unsigned char *)&st->buff[st->pos];
   int count = (size<(st->len-st->pos))?(size):(st->len-st->pos);

   if (count <= 0)
      return 0;

   memcpy(dest, src, count);

   st->pos += count;

   return count;
}

int   RAMIO_write (int fd, unsigned char *buffer, int size)
{
   RAMIO_FILE * st = &g_ram_files[fd];
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
   return count;
}

long  RAMIO_seek  (int fd, long offset, int origin)
{
   RAMIO_FILE * st = &g_ram_files[fd];
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

int RAMIO_remove(const char *filename)
{
   return -1;
}

int RAMIO_rename(const char *oldname, const char *newname)
{
   return -1;
}

int RAMIO_ioctl(int fd, int request, va_list varg_list)
{
   return -1;
}

