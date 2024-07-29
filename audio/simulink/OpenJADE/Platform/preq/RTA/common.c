//	Title:	common.c
//	Author: Guillaume

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include "common.h"

uint8_t REVERSE_BYTE_LOOKUP[16] = {0x0,0x8,0x4,0xc,0x2,0xa,0x6,0xe,0x1,0x9,0x5,0xd,0x3,0xb,0x7,0xf};
uint8_t REVERSE_BYTE(uint8_t b) { return (REVERSE_BYTE_LOOKUP[b&0xF] << 4) | REVERSE_BYTE_LOOKUP[b>>4]; }



	#include <syslog.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <dirent.h>
	void DbgStringCallbackDefault(int priority, char *str) {
		//uint32_t t = time_ms();
		//printf("%d.%03d %s",t/1000,t%1000,str);
		switch(priority) {
		case LOG_DEBUG:
			printf("%s", str);
			break;
		default:
			printf("%s", str);
			syslog(priority, "%s", str);
		}
	}
	void (*DbgStringCallback)(int, char *) = DbgStringCallbackDefault;
	void DbgStringInitCallback(DBG_STRING_CALLBACK callback) {
		DbgStringCallback = callback;
	}
	void Log(int priority, const char *fmt, ...) {
		char str[256];
		va_list ap;
		va_start(ap, fmt);
		vsnprintf(str, 256, fmt, ap);
		DbgStringCallback(priority, str);
		va_end(ap);
	}

	uint32_t time_ms(void) {
		struct timeval time;
		gettimeofday(&time, NULL);
		return((time.tv_sec % 1000) * 1000 + time.tv_usec / 1000);
	}

	int32_t SEM_INIT(SEM_HANDLE *sem, uint32_t value) {
		return sem_init(sem, 0, value);
	}
	int32_t SEM_DESTROY(SEM_HANDLE *sem) {
		return sem_destroy(sem);
	}
	int32_t SEM_GETVALUE(SEM_HANDLE *sem, int32_t *val) {
		return sem_getvalue(sem, (int *)val);
	}
	int32_t SEM_WAIT(SEM_HANDLE *sem) {
		return sem_wait(sem);
	}
	int32_t SEM_POST(SEM_HANDLE *sem) {
		return sem_post(sem);
	}

	size_t strlcat(char *dst, const char *src, size_t siz) {
		char *d = dst;
		const char *s = src;
		size_t n = siz;
		size_t dlen;

		/* Find the end of dst and adjust bytes left but don't go past end */
		while (n-- != 0 && *d != '\0')
			d++;
		dlen = d - dst;
		n = siz - dlen;

		if (n == 0)
			return(dlen + strlen(s));
		while (*s != '\0') {
			if (n != 1) {
				*d++ = *s;
				n--;
			}
			s++;
		}
		*d = '\0';

		return(dlen + (s - src));   /* count does not include NUL */
	}

	//Call this function to start a nanosecond-resolution timer
	struct timespec timeStart() {
		struct timespec start_time;
		clock_gettime(CLOCK_MONOTONIC, &start_time);
		return start_time;
	}

	//Call this function to end a timer, returning nanoseconds elapsed as a long
	long timeEnd(struct timespec start_time) {
		struct timespec end_time;
		clock_gettime(CLOCK_MONOTONIC, &end_time);
		long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 +
						   (end_time.tv_nsec - start_time.tv_nsec);
		return diffInNanos;
	}

	//Call this function to end a timer, returning nanoseconds elapsed as a long
	long timeDiff(struct timespec start_time, struct timespec end_time) {
		long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 +
						   (end_time.tv_nsec - start_time.tv_nsec);
		return diffInNanos;
	}

	// Find if a process is running
	int procFind(const char *name) {
		DIR *dir;
		struct dirent *ent;
		char *endptr;
		char buf[512];

		if (!(dir = opendir("/proc"))) {
			Log(LOG_ERR, "procFind: Can't open /proc");
			return -1;
		}

		while((ent = readdir(dir)) != NULL) {
			// if endptr is not a null character, the directory is not entirely numeric, so ignore it
			long lpid = strtol(ent->d_name, &endptr, 10);
			if (*endptr != '\0') {
				continue;
			}

			snprintf(buf, sizeof(buf), "/proc/%ld/cmdline", lpid);
			FILE *fp = fopen(buf, "r");
			if (fp) {
				if (fgets(buf, sizeof(buf), fp) != NULL) {
					if (strstr(buf, name) != NULL) {
						fclose(fp);
						closedir(dir);
						return (pid_t)lpid;
					}
				}
				fclose(fp);
			}
		}
		closedir(dir);
		return -1;
	}
