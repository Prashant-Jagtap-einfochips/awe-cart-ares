#ifndef __OJ_PLATFORM_H__
#define __OJ_PLATFORM_H__
#include <stdint.h>
#include <stdarg.h>

#if !defined(PLATFORM_MULTIRATE_AUDIOTHREADS) && !defined(PLATFORM_BASERATE_AUDIOTHREAD)
#define PLATFORM_BASERATE_AUDIOTHREAD
#endif

#if !defined(PLATFORM_BASETHREADLEVEL)
#define PLATFORM_BASETHREADLEVEL 0
#endif

/* Platform Constants */
#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
#define PLATFORM_NUM_AUDIOTHREADS (12)
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
#define PLATFORM_NUM_AUDIOTHREADS (1)
#endif

#define PLATFORM_NUM_BACKGROUND_TASKS  (10)

/* Platform Typedefs */
typedef struct Platform_control_msg
{
	uint32_t id;
	uint32_t size;
	void *data;
}  Platform_control_msg;

typedef void*  Platform_Handle_t;

typedef void *(*Platform_Func_framework_init)(void);

typedef int32_t (*Platform_Func_control_service_message)(Platform_control_msg *p_request_message,
                                                      Platform_control_msg *p_response_message,
                                                      void *arg);

typedef int32_t (*Platform_Func_notification_poll)(void *arg);

typedef void (*Platform_Func_background_task)(void *arg);

/* Platform Function Prototypes */
#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
typedef void (*Platform_Func_audio_thread_handler)(void *arg, uint32_t thread_level);
void platform_register_audio_thread_handler(Platform_Handle_t hPlatform, Platform_Func_audio_thread_handler audio_thread_handler);
void platform_audio_thread_post(Platform_Handle_t hPlatform, uint32_t priority);
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
typedef void (*Platform_Func_audio_service_tick)(void *arg);
void platform_register_audio_service_tick(Platform_Handle_t hPlatform, Platform_Func_audio_service_tick audio_service_tick);
#endif

void platform_init(void);

Platform_Handle_t platform_get_handle(void);

void platform_register_framework_init(Platform_Handle_t hPlatform, Platform_Func_framework_init framework_init);

void platform_register_control_service_message(Platform_Handle_t hPlatform, Platform_Func_control_service_message control_service_message);

void platform_register_notification_poll(Platform_Handle_t hPlatform, Platform_Func_notification_poll notification_poll);

void platform_send_notification(Platform_control_msg *p_notification_message);

int platform_add_background_task(Platform_Handle_t hPlatform, Platform_Func_background_task background_task);

int platform_remove_background_task(Platform_Handle_t hPlatform, Platform_Func_background_task background_task);

void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[]);

uint32_t platform_rcv_input_buffer(float *buff, uint32_t num_chan, uint32_t frame_size);
uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size);

void platform_close(Platform_Handle_t hPlatform);

int platform_vprintf (const char * format, va_list arg );
int platform_printf(char* format, ...);

#endif /* __OJ_PLATFORM_H__ */

