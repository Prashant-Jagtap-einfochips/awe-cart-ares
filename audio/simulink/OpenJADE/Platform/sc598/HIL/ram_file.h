#ifndef __RAM_FILE__H__
#define __RAM_FILE__H__

#if defined(__ADSPSHARC__)
extern "C" void ramio_files_init(void);
extern "C" void ramio_files_close(void);
extern "C" void ramio_files_shred(const char * fname);
extern "C" unsigned char * ramio_file_start_addr(const char * fname);
extern "C" int ramio_file_curr_pos(const char * fname);
extern "C" int ramio_file_length(const char * fname);
#else
static inline void ramio_files_init(void)
{
   return;
}
static inline void ramio_files_close(void)
{
   return;
}
static inline void ramio_files_shred(const char * fname)
{
   return;
}
static inline unsigned char * ramio_file_start_addr(const char * fname)
{
   return (unsigned char *)0;
}
static inline int ramio_file_curr_pos(const char * fname)
{
   return 0;
}
static inline int ramio_file_length(const char * fname)
{
   return 0;
}
#endif

#endif //__RAM_FILE__H__

