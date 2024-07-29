#if !defined(OJSP__INCLUDED_)
#define OJSP__INCLUDED_
#include <stdint.h>
#include "oj_platform.h"

void *OJSP_framework_init(void);

#if defined(PLATFORM_MULTIRATE_AUDIOTHREADS)
void OJSP_audio_thread_handler(void *arg, uint32_t thread_level);
#elif defined(PLATFORM_BASERATE_AUDIOTHREAD)
void OJSP_audio_service_tick(void *arg);
#endif

int32_t OJSP_control_service_message(Platform_control_msg *p_request_message,
                                  Platform_control_msg *p_response_message,
                                  void *arg);
int32_t OJSP_notification_poll(void *arg);

void OJSP_background_task(void *arg);

#endif /*!defined(OJSP__INCLUDED_)*/

