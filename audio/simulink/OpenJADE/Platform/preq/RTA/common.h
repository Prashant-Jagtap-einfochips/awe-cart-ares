//	Title:  common.h
//	Author: Guillaume


#if !defined(COMMON_H)
#define COMMON_H

// Types
typedef unsigned char 		BYTE;
typedef signed char   		SBYTE;
typedef unsigned char  		UINT8;
typedef signed char    		SINT8;
typedef unsigned short 		UINT16;
typedef signed short   		SINT16;
typedef unsigned int  		UINT32;
typedef signed int    		SINT32;
typedef unsigned long long 	UINT64;
typedef long long		   	SINT64;
typedef float    			FLOAT;
typedef double    			DOUBLE;


// Macros
#if !defined(TRUE)
	#define TRUE 1
#endif

#if !defined(FALSE)
	#define FALSE 0
#endif

#define SWAP8(a,b) {BYTE c=a; a=b; b=c;}
#define SWAP16(a,b) {UINT16 c=a; a=b; b=c;}
#define SWAP32(a,b) {UINT32 c=a; a=b; b=c;}
#define REVERSE_BYTE32(i) (((UINT32)i>>24)|(((UINT32)i>>8)&0xFF00)|(((UINT32)i<<8)&0xFF0000)|(((UINT32)i<<24)&0xFF000000))
#define REVERSE_BYTE16(i) ( (((UINT16)i)<<8) | ((UINT16)i>>8) )

#define EXTRACT_B1_U32(four_byte) ((UINT8) ((four_byte >> 24) & 0xFF))
#define EXTRACT_B2_U32(four_byte) ((UINT8) ((four_byte >> 16) & 0xFF))
#define EXTRACT_B3_U32(four_byte) ((UINT8) ((four_byte >>  8) & 0xFF))
#define EXTRACT_B4_U32(four_byte) ((UINT8) ((four_byte      ) & 0xFF))


#if defined(__APPLE__)
    #include <stdbool.h>
	#include <dispatch/dispatch.h>
	#include <pthread.h>
	#include <sys/types.h>
	#include <unistd.h>
    #include <string.h>
    #include <ctype.h>
    typedef bool BOOL;
	extern void Log(int priority, const char *fmt, ...);
	#define THREAD_HANDLE_TYPE pthread_t
	#define THREAD_RETURN_TYPE void*
	#define THREAD_RETURN_ZERO NULL
	#define CLOSE_SOCKET close
	#define MUTEX_HANDLE_TYPE pthread_mutex_t
	#define SEM_HANDLE dispatch_semaphore_t

	// Mutex error checking
	#define MUTEX_ERROR_CHECKING    0
	extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
	//extern int pthread_mutex_timedlock (pthread_mutex_t *mutex, const struct timespec *abstime);

	#if(MUTEX_ERROR_CHECKING == 0)
	#define PTHREAD_MUTEX_TYPE   PTHREAD_MUTEX_NORMAL
	#define PTHREAD_MUTEX_LOCK(a) { pthread_mutex_lock(a);    }
	#define PTHREAD_MUTEX_UNLOCK(a) { pthread_mutex_unlock(a); }
	#else
	#define PTHREAD_MUTEX_TYPE   PTHREAD_MUTEX_ERRORCHECK
	#define PTHREAD_MUTEX_LOCK(a) { struct timespec timeout = {time(NULL)+2,0}; int mutex_ret = pthread_mutex_timedlock(a,&timeout); \
	        if(mutex_ret) {printf("***** mutex_lock %d %s %s *****\n",(int)getpid(),__FUNCTION__,strerror(mutex_ret));} }
	#define PTHREAD_MUTEX_UNLOCK(a) { int mutex_ret = pthread_mutex_unlock(a); \
	        if(mutex_ret) {printf("***** mutex_unlock %d %s %s *****\n",(int)getpid(),__FUNCTION__,strerror(mutex_ret));} }
	#endif
	#define PTHREAD_MUTEX_INIT(a) { \
	        pthread_mutexattr_t mutexattr; \
	        pthread_mutexattr_init(&mutexattr); \
	        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_TYPE); \
	        pthread_mutex_init(a,&mutexattr); \
	        pthread_mutexattr_destroy(&mutexattr); }
	#define PTHREAD_MUTEX_DESTROY(a) { pthread_mutex_destroy(a); }

	// Trusleep
	#define trusleep(us) { \
	    struct timespec wait; \
	    wait.tv_nsec = (us%1000000)*1000; \
	    wait.tv_sec = us/1000000; \
	    while(nanosleep(&wait,&wait)); }
	//while(nanosleep(&wait,&wait)) {printf("Nanosleep: %d %ld %ld\n",errno,wait.tv_nsec,wait.tv_sec); }; }

	// Config library hack
	#ifdef LIBCONFIG_VER_MAJOR
		#define CFG_INT int
	#else
		#define CFG_INT long
	#endif
#endif


#if defined(__linux__)
	#include <stdint.h>
	#include <semaphore.h>
	#include <pthread.h>
	#include <sys/types.h>
	#include <unistd.h>
	//#include <bsd/string.h>
	typedef int BOOL;
	extern void Log(int priority, const char *fmt, ...);
	#define THREAD_HANDLE_TYPE pthread_t
	#define THREAD_RETURN_TYPE void*
	#define THREAD_RETURN_ZERO NULL
	#define CLOSE_SOCKET close
	#define MUTEX_HANDLE_TYPE pthread_mutex_t
	#define SEM_HANDLE sem_t
	#define strlcpy(d,s,len) {strncpy(d,s,len);  d[len-1]='\0';}
	extern size_t strlcat(char *dst, const char *src, size_t siz);

	// Mutex error checking
	#define MUTEX_ERROR_CHECKING	1
	extern int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
	extern int pthread_mutex_timedlock (pthread_mutex_t *mutex, const struct timespec *abstime);

	#if(MUTEX_ERROR_CHECKING == 0)
	#define PTHREAD_MUTEX_TYPE   PTHREAD_MUTEX_TIMED_NP
	#define PTHREAD_MUTEX_LOCK(a) { pthread_mutex_lock(a);	}
	#define PTHREAD_MUTEX_UNLOCK(a) { pthread_mutex_unlock(a); }
	#else
	#define PTHREAD_MUTEX_TYPE   PTHREAD_MUTEX_ERRORCHECK_NP
	#define PTHREAD_MUTEX_LOCK(a) { struct timespec timeout = {time(NULL)+2,0}; int mutex_ret = pthread_mutex_timedlock(a,&timeout); \
				if(mutex_ret) {printf("***** mutex_lock %d %s %s *****\n",(int)getpid(),__FUNCTION__,strerror(mutex_ret));} }
	#define PTHREAD_MUTEX_UNLOCK(a) { int mutex_ret = pthread_mutex_unlock(a); \
				if(mutex_ret) {printf("***** mutex_unlock %d %s %s *****\n",(int)getpid(),__FUNCTION__,strerror(mutex_ret));} }
	#endif
	#define PTHREAD_MUTEX_INIT(a) { \
			pthread_mutexattr_t mutexattr; \
			pthread_mutexattr_init(&mutexattr); \
			pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_TYPE); \
			pthread_mutex_init(a,&mutexattr); \
			pthread_mutexattr_destroy(&mutexattr); }
	#define PTHREAD_MUTEX_DESTROY(a) { pthread_mutex_destroy(a); }

	// Trusleep
	#define trusleep(us) { \
			struct timespec wait; \
			wait.tv_nsec = (us%1000000)*1000; \
			wait.tv_sec = us/1000000; \
			while(nanosleep(&wait,&wait)); }
	//while(nanosleep(&wait,&wait)) {printf("Nanosleep: %d %ld %ld\n",errno,wait.tv_nsec,wait.tv_sec); }; }

	// Config library hack
	#ifdef LIBCONFIG_VER_MAJOR
		#define CFG_INT int
	#else
		#define CFG_INT long
	#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#pragma warning(disable:4996)
	#include "Winsock2.h"
	typedef int BOOL;
	#define THREAD_HANDLE_TYPE HANDLE
	#define THREAD_RETURN_TYPE DWORD WINAPI
	#define THREAD_RETURN_ZERO 0
	#define CLOSE_SOCKET closesocket
	#define MUTEX_HANDLE_TYPE HANDLE
	#define SEM_HANDLE HANDLE
	#define strlcpy(d,s,len) {strncpy(d,s,len);  d[len-1]='\0';}
    extern size_t strlcat(char *dst, const char *src, size_t siz);

	void Log(int priority, const char *fmt, ...);

	#define snprintf sprintf_s
	#define trusleep(us) { Sleep(us/1000); }
	#define PTHREAD_MUTEX_LOCK(a) { WaitForSingleObject(a,INFINITE); }
	#define PTHREAD_MUTEX_UNLOCK(a) { ReleaseMutex(a); }
	#define PTHREAD_MUTEX_INIT(a) { *a = CreateMutex(NULL,FALSE,NULL); }
	#define PTHREAD_MUTEX_DESTROY(a) { CloseHandle(a); }
#endif
#ifdef __cplusplus
}
#endif


#if defined(__ADSPSC589_FAMILY__)
	#include <os.h>
	#define THREAD_HANDLE_TYPE OS_TCB
	#define THREAD_RETURN_TYPE OS_TASK_PTR
	#define THREAD_RETURN_ZERO NULL
	#define CLOSE_SOCKET close
	#define MUTEX_HANDLE_TYPE OS_MUTEX
	#define SEM_HANDLE OS_SEM

	typedef int BOOL;
	#define strlcpy(d, s, len) { strncpy(d, s, len); d[len - 1] = '\0'; }
	void Log(int priority, const char *fmt, ...);

	#define PTHREAD_MUTEX_INIT(a)	 OSMutexCreate(a, NULL, NULL);
	#define PTHREAD_MUTEX_LOCK(a)	 OSMutexPend(a, 0, OS_OPT_PEND_BLOCKING, NULL, NULL);
	#define PTHREAD_MUTEX_UNLOCK(a)  OSMutexPost(a, OS_OPT_POST_NONE, NULL);
	#define PTHREAD_MUTEX_DESTROY(a) OSMutexDel(a, OS_OPT_DEL_ALWAYS, NULL);
	#define trusleep(us)  OSTimeDlyHMSM(0, 0, 0, us * 0.001, 0, NULL); //TODO: make this trusleep actually tru
	#define CreateThread(p_tcb, p_task, p_stk_base, stk_size, priority)	{	\
			OSSchedLock (NULL) ;                           						\
			OSTaskCreate(p_tcb,													\
						NULL,													\
						p_task,													\
						NULL,													\
						priority,												\
						p_stk_base,												\
						stk_size-1u,											\
						stk_size,												\
						0u,														\
						(OS_TICK) 0u,											\
						NULL,													\
						(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),			\
						NULL);													\
			OSSchedUnlock (NULL) ;                           					\
		}

	#ifndef LOG_DEBUG
		#define LOG_DEBUG 0
	#endif
	#ifndef LOG_INFO
		#define LOG_INFO 1
	#endif
	#ifndef LOG_WARNING
		#define LOG_WARNING 2
	#endif
	#ifndef LOG_ERR
		#define LOG_ERR 3
	#endif

	typedef void (*DBG_STRING_CALLBACK)(int priority, char *str);
	void DbgStringInitCallback(DBG_STRING_CALLBACK callback);
	UINT32 time_ms(void);
	SINT32 SEM_INIT(SEM_HANDLE *sem, UINT32 value);
	SINT32 SEM_DESTROY(SEM_HANDLE *sem);
	SINT32 SEM_GETVALUE(SEM_HANDLE *sem, SINT32 *val);
	SINT32 SEM_WAIT(SEM_HANDLE *sem);
	SINT32 SEM_POST(SEM_HANDLE *sem);
	int gethostname(char *name, size_t len);
#endif


#ifndef LOG_DEBUG
	#define LOG_DEBUG		7
#endif
#ifndef LOG_INFO
	#define LOG_INFO		6
#endif
#ifndef LOG_WARNING
	#define LOG_WARNING     4
#endif
#ifndef LOG_ERR
	#define LOG_ERR			3
#endif


typedef void (*DBG_STRING_CALLBACK)(int priority, char *str);
void DbgStringInitCallback(DBG_STRING_CALLBACK callback);
UINT32 time_ms(void);
SINT32 SEM_INIT(SEM_HANDLE *sem, UINT32 value);
SINT32 SEM_DESTROY(SEM_HANDLE *sem);
SINT32 SEM_GETVALUE(SEM_HANDLE *sem, SINT32 *val);
SINT32 SEM_WAIT(SEM_HANDLE *sem);
SINT32 SEM_POST(SEM_HANDLE *sem);

struct timespec timeStart(void);
long timeEnd(struct timespec start_time);
long timeDiff(struct timespec start_time, struct timespec end_time);
int procFind(const char *name);

#endif

