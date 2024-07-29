/*
 * File    : linux_jack_main.c
 */

#define _GNU_SOURCE
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <termios.h>
#include <getopt.h>
#include <sys/mman.h>

#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sched.h>
#include <syslog.h>
#include <math.h>

#include <jack/jack.h>
#include <jack/midiport.h>

#include "oj_platform.h"

#if defined(IA32) || defined(AMD64)
	#include <ipp.h>
#endif

#include "common.h"
#include "pq_midi.h"
#include "pq_param.h"
#include "linux_jack.h"

#define SYSIN_CHANNEL_COUNT 2
#define SYSOUT_CHANNEL_COUNT 2

#define QUOTE1(name) #name
#define QUOTE(name) QUOTE1(name)

#ifndef MODEL
	# error Must define MODEL.
#endif
#ifndef SYSIN_CHANNEL_COUNT
	# error Must define SYSIN_CHANNEL_COUNT.
#endif
#ifndef SYSOUT_CHANNEL_COUNT
	# error Must define SYSOUT_CHANNEL_COUNT.
#endif

//#include "BAF.h"
//#include "OJSP.h"


#define SYSIN_FRAME_SIZE_IN_SAMPLES 32
#define SYSOUT_FRAME_SIZE_IN_SAMPLES 32
#define MODEL_SYNC_TASK_COUNT 1
#define SYSIN_TASK_RATE_FACTOR 1
#define SYSOUT_TASK_RATE_FACTOR 1
#define SYSIN_SAMPLE_RATE_IN_HERTZ 48000
#define SYSOUT_SAMPLE_RATE_IN_HERTZ 48000







int g_remKey[256];
jack_client_t *g_jackHandle;
jack_port_t *g_jackInput[SYSIN_CHANNEL_COUNT];
jack_port_t *g_jackOutput[SYSOUT_CHANNEL_COUNT];
jack_port_t *g_jackMidiIn;
jack_port_t *g_jackMidiOut;
jack_port_t *g_jackMidiConOut;
jack_port_t *g_jackMidiLcdOut;
MIDI_QUEUE g_midiQueueIn[MIDI_CHANNEL_NB_MAX];
MIDI_QUEUE g_midiQueueOut;
MIDI_QUEUE g_midiQueueConOut;
MIDI_QUEUE g_midiQueueLcdOut;

#define AUDIO_BUF_SIZE  8192
int g_modelFrameSize = 0;
int g_modelSampleRate = 0;
int g_jackAddBufNb = 0;
sig_atomic_t g_jackSampleRate = 0;
sig_atomic_t g_jackFrameSize = 0;

float g_sysin[SYSIN_CHANNEL_COUNT][SYSIN_FRAME_SIZE_IN_SAMPLES];
float g_sysout[SYSOUT_CHANNEL_COUNT][SYSOUT_FRAME_SIZE_IN_SAMPLES];
float* SYSIN = (float*)g_sysin;
float* SYSOUT = (float*)g_sysout;

sig_atomic_t g_audioBufInBegin = 0;
sig_atomic_t g_audioBufInEnd = 0;
float g_audioBufIn[SYSIN_CHANNEL_COUNT][AUDIO_BUF_SIZE];
sig_atomic_t g_audioBufOutBegin = 0;
sig_atomic_t g_audioBufOutEnd = 0;
float g_audioBufOut[SYSOUT_CHANNEL_COUNT][AUDIO_BUF_SIZE];
sig_atomic_t g_audioPreloadCpt = 0;
sig_atomic_t g_jackPreloadBufNb = 0;
sig_atomic_t g_processModelAudioRepeat = 1;
int g_modelTaskRate[] = {};

sem_t g_semDeferredStart;
sem_t g_semDeferredDone;
pthread_t g_threadDeferred;
sem_t semExit;
uint32_t g_affinityJack, g_affinityDeferred;

//#define MONITOR_BUF_LENGTH (MIDI_RTP_DATA_SIZE_BIN/sizeof(uint32_t))
//uint32_t g_monitorCpt = 0;
//uint32_t g_monitorBufA[MONITOR_BUF_LENGTH];
//uint32_t g_monitorBufB[MONITOR_BUF_LENGTH];
MIDI_MON_QUEUE g_monQueue[MODEL_SYNC_TASK_COUNT+1];
uint32_t g_xrun = 0;
uint32_t g_xrunCpt = 0;
uint32_t g_jackProcessCpt = 0;

sig_atomic_t g_lcd_refresh_flag = FALSE;
int32_t g_midi_running = FALSE;
int g_modelIsolated = FALSE;
int g_modelStarted = FALSE;

MIDI_MSG g_clDispMsg[DISP_NB_PAGE][DISP_NB_LINE];
int g_clDispStatus[DISP_NB_PAGE][DISP_NB_LINE];
float g_ecuSpeed;
float g_headTrackerCH;
float g_headTrackerDOF[HEAD_TRACKER_DOF_DIM_NUM];

float g_rigidBodyIDs[HEAD_TRACKER_IDS_MAX] = {0};
float g_rigidBodyDOFs[HEAD_TRACKER_IDS_MAX][HEAD_TRACKER_DOF_DIM_NUM] = {{0.0}};

#define PLATFORM_NUM_BACKGROUND_TASKS    (10)
typedef struct Platform_Config
{
   /* Callback functions */
   Platform_Func_framework_init             framework_init;
   Platform_Func_audio_service_tick         audio_service_tick;
   Platform_Func_control_service_message    control_service_message;
   Platform_Func_notification_poll          notification_poll;
   Platform_Func_bafexec                    bafexec;

   Platform_Func_background_task            background_tasks[PLATFORM_NUM_BACKGROUND_TASKS];
   /* Callback param */
   void   *callback_param;

   /* input and output buffers for current frame */
   int32_t *input_buff;
   int32_t *output_buff;
}  Platform_Config_t;

Platform_Config_t g_platfromConfig;

Platform_Handle_t platform_get_handle(void)
{
   return (Platform_Handle_t)&g_platfromConfig;
}



void platform_clear_background_tasks(Platform_Handle_t hPlatform)
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
   {
      pPlatform->background_tasks[i] = (Platform_Func_background_task)NULL;
   }
}
void platform_init(void)
{
   platform_clear_background_tasks((Platform_Handle_t)&g_platfromConfig);

}

void platform_close(Platform_Handle_t hPlatform)
{
}


void platform_register_framework_init(Platform_Handle_t hPlatform, Platform_Func_framework_init framework_init)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->framework_init = framework_init;
}

void platform_register_audio_service_tick(Platform_Handle_t hPlatform, Platform_Func_audio_service_tick audio_service_tick)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->audio_service_tick = audio_service_tick;
}

void platform_register_bafexec(Platform_Handle_t hPlatform, Platform_Func_bafexec bafexec)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->bafexec = bafexec;
}

void xmit_control_service_response(char *service, uint32_t msg_id, uint32_t cnt, uint8_t *buff)
{
   uint32_t idx;

   printf( "%s: %8.8X %8.8x", service, msg_id, cnt);
   for(idx = 0; idx < cnt; idx++)
   {
      printf(" %2.2X", *buff++);
   }
   printf( "\n");

   return;
}

void xmit_control_service_error(FILE *fp, char *service, uint32_t msg_id, int32_t status)
{

   printf("%s: %8.8X %8.8x\n", service, msg_id, status);

   return;
}

void platform_send_notification(Platform_control_msg *message)
{
   xmit_control_service_response("NTF",
                                 message->id,
                                 message->size,
                                 (uint8_t *)(message->data));
}
void platform_register_control_service_message(Platform_Handle_t hPlatform, Platform_Func_control_service_message control_service_message)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->control_service_message = control_service_message;
}

void platform_register_notification_poll(Platform_Handle_t hPlatform, Platform_Func_notification_poll notification_poll)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   pPlatform->notification_poll = notification_poll;
}

int platform_add_background_task(Platform_Handle_t hPlatform, Platform_Func_background_task background_task)
{
   int i;
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   for (i=0; i<PLATFORM_NUM_BACKGROUND_TASKS; i++)
   {
      if (pPlatform->background_tasks[i] == (Platform_Func_background_task)NULL)
      {
         pPlatform->background_tasks[i] = background_task;
         return 1;
      }
   }
   return 0;
}

uint32_t platform_rcv_input_buffer(float *buff, uint32_t num_chan, uint32_t frame_size) {
	sig_atomic_t temp;
	if (num_chan > SYSIN_CHANNEL_COUNT) {
		num_chan = SYSIN_CHANNEL_COUNT;
	}
	if((frame_size != g_jackFrameSize) || (g_jackAddBufNb != 0)) {
		for(int port = 0; port < num_chan; port++) {
			memcpy((uint8_t *)buff + port * frame_size * sizeof(float),
					&g_audioBufIn[port][g_audioBufInBegin],
					frame_size * sizeof(float));
		}
		temp = (g_audioBufInBegin + g_modelFrameSize) % AUDIO_BUF_SIZE;
		g_audioBufInBegin = temp;
	}

   return num_chan;
}

uint32_t platform_xmt_output_buffer(float *buff, uint32_t num_chan, uint32_t frame_size) {
	sig_atomic_t temp;
	if (num_chan > SYSOUT_CHANNEL_COUNT) {
		num_chan = SYSOUT_CHANNEL_COUNT;
	}
	if((frame_size != g_jackFrameSize) || (g_jackAddBufNb != 0)) {
		for(int port = 0; port < num_chan; port++) {
			memcpy(&g_audioBufOut[port][g_audioBufOutEnd],
					(uint8_t *)buff + port * frame_size * sizeof(float),
					frame_size * sizeof(float));
		}
		temp = (g_audioBufOutEnd + g_modelFrameSize) % AUDIO_BUF_SIZE;
		g_audioBufOutEnd = temp;
	}
   return num_chan;
}

void PROCESS_ModelAudio(void *arg) {

   Platform_Config_t *pPlatform = (Platform_Config_t *)arg;

	if(SYSIN_TASK_RATE_FACTOR == 1) {
		for(int rpt = 0; rpt < g_processModelAudioRepeat; rpt++) {
			//OJSP_audio_service_tick(g_BAFobj);
         pPlatform->audio_service_tick(pPlatform->callback_param);

		}
	}
	g_lcd_refresh_flag = FALSE;
}

void *THREAD_Deferred(void *arg) {
	struct sched_param  schp;
	int schedPolicy;
	pthread_t pthread = pthread_self();
	memset(&schp, 0, sizeof(schp));
	schp.sched_priority = sched_get_priority_min(SCHED_FIFO) + 82;
	if(pthread_setschedparam(pthread, SCHED_FIFO, &schp)) Log(LOG_ERR,
				"***ERROR*** THREAD_Deferred pthread_setschedparam\n");
	pthread_getschedparam(pthread, &schedPolicy, &schp);
	pid_t tid = (pid_t) syscall(SYS_gettid);
	printf("THREAD_Deferred tid=%d priority=%d\n", tid, schp.sched_priority);

	int i;
	if(g_affinityDeferred) {
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		for(i = 0; i < 8; i++) {
			if(g_affinityDeferred & (1 << i)) CPU_SET(i, &cpuset);
		}
		int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
		if(ret != 0) printf("***ERROR*** THREAD_Deferred pthread_setaffinity_np\n");
		g_affinityDeferred = 0;
	}

	while(1) {
		sem_wait(&g_semDeferredStart);
		PROCESS_ModelAudio(arg);
		sem_post(&g_semDeferredDone);
	}
	pthread_exit(NULL);
}

void BDSP_Process(jack_midi_event_t *event, uint8_t cmd, uint8_t scmd, uint8_t *bbuf, uint8_t length);
int jack_process(jack_nframes_t nframes, void *arg) {
	uint8_t *bufBin;
	int szLength;
	char szStrOut[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];
	MIDI_MSG msg;
	char szStr[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];
	int i, j, k;
	uint8_t cmd, scmd;
	char *szpstr;
	uint8_t length;
	int ht_id_index; // variable to index associated with an ID in ID table

	//struct timespec vartime = timeStart();
	//int core = sched_getcpu();

	// CPU Monitor
	MIDI_MON_DATA mon;
	int core = sched_getcpu();
	struct timespec t;
	pthread_t self_id= pthread_self();
	int policy;
	struct sched_param param;
	pthread_getschedparam(self_id, &policy, &param);

	clock_gettime(CLOCK_REALTIME, &t);
	mon.ts = t.tv_sec*1000000000+t.tv_nsec;
	mon.index = MIDI_MON_START + 0;
	mon.priority = param.sched_priority;
	mon.core = core;
	if(MIDI_MonQueueWrite(&g_monQueue[0],&mon) != MIDI_OK) {
		Log(LOG_ERR, "***ERROR*** CPU_MONITOR Queue 0 Full\n");
	}

	if(g_affinityJack) {
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		for(i = 0; i < 8; i++) {
			if(g_affinityJack & (1 << i)) CPU_SET(i, &cpuset);
		}
		int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
		if(ret != 0) printf("***ERROR*** pthread_setaffinity_np\n");
		g_affinityJack = 0;
	}

	// Check if frame size has changed
	if(nframes != g_jackFrameSize) {
		g_jackFrameSize = nframes;
		g_audioBufInBegin = 0;
		g_audioBufInEnd = 0;
		g_audioBufOutBegin = 0;
		g_audioBufOutEnd = 0;
		g_audioPreloadCpt = 0;
		g_jackPreloadBufNb = g_jackAddBufNb;
		if(g_jackFrameSize < g_modelFrameSize) {
			g_jackPreloadBufNb += g_modelFrameSize / g_jackFrameSize;
		}
		if((g_jackPreloadBufNb * g_jackFrameSize + g_modelFrameSize) > AUDIO_BUF_SIZE) {
			g_jackPreloadBufNb = (AUDIO_BUF_SIZE - g_modelFrameSize) / g_jackFrameSize;
			printf("***ERROR*** Preload buffer reduced to: %.2fms (%d Samples)\n",
				   (float)g_jackPreloadBufNb * g_jackFrameSize * 1000.0 / (float)g_modelSampleRate,
				   g_jackPreloadBufNb * g_jackFrameSize);
		} else {
			printf("Preload buffer: %.2fms (%d Samples)\n",
				   (float)g_jackPreloadBufNb * g_jackFrameSize * 1000.0 / (float)g_modelSampleRate,
				   g_jackPreloadBufNb * g_jackFrameSize);
		}
	}

	sig_atomic_t temp;
	jack_default_audio_sample_t *bufAudio;
	// Copy data into input buffer
	if((SYSIN_FRAME_SIZE_IN_SAMPLES == g_jackFrameSize) && (g_jackAddBufNb == 0)) {
		for(int port = 0; port < SYSIN_CHANNEL_COUNT; port++) {
			bufAudio = jack_port_get_buffer(g_jackInput[port], nframes);
			memcpy(SYSIN+port*nframes, bufAudio, nframes * sizeof(float));
		}
	} else {
		for(int port = 0; port < SYSIN_CHANNEL_COUNT; port++) {
			bufAudio = jack_port_get_buffer(g_jackInput[port], nframes);
			memcpy(&g_audioBufIn[port][g_audioBufInEnd], bufAudio, nframes * sizeof(float));
		}
		temp = (g_audioBufInEnd + nframes) % AUDIO_BUF_SIZE;
		g_audioBufInEnd = temp;
	}

	// Run Model
	if(nframes < g_modelFrameSize) {
		g_processModelAudioRepeat = 1;
		if((g_audioBufInEnd % g_modelFrameSize) == 0) {
			sem_post(&g_semDeferredStart);
			printf("Post g_semDeferredStart\n");
		}
	} else {
		g_processModelAudioRepeat = nframes / g_modelFrameSize;
		if(g_jackAddBufNb == 0) {
			PROCESS_ModelAudio(arg);
		} else {
			sem_post(&g_semDeferredStart);
		}
	}

	// Copy data from output buffer
	if((SYSOUT_FRAME_SIZE_IN_SAMPLES == g_jackFrameSize) && (g_jackAddBufNb == 0)) {
		for(int port = 0; port < SYSOUT_CHANNEL_COUNT; port++) {
			bufAudio = jack_port_get_buffer(g_jackOutput[port], nframes);
			memcpy(bufAudio, SYSOUT+port*nframes, nframes * sizeof(float));
		}
	} else {
		if(g_audioPreloadCpt < (g_jackPreloadBufNb * g_jackFrameSize)) {
			g_audioPreloadCpt += nframes;
			//printf("Preloading %d\n",g_audioPreloadCpt);
			for(int port = 0; port < SYSOUT_CHANNEL_COUNT; port++) {
				bufAudio = jack_port_get_buffer(g_jackOutput[port], nframes);
				memset(bufAudio, 0, nframes * sizeof(float));
			}
		} else {
			if(nframes < g_modelFrameSize) {
				if((g_audioBufOutBegin % g_modelFrameSize) == 0) sem_wait(&g_semDeferredDone);
			} else {
				if(g_jackAddBufNb != 0) sem_wait(&g_semDeferredDone);
			}
			for(int port = 0; port < SYSOUT_CHANNEL_COUNT; port++) {
				bufAudio = jack_port_get_buffer(g_jackOutput[port], nframes);
				memcpy(bufAudio, &g_audioBufOut[port][g_audioBufOutBegin], nframes * sizeof(float));
			}
			temp = (g_audioBufOutBegin + nframes) % AUDIO_BUF_SIZE;
			g_audioBufOutBegin = temp;
		}
	}

	void *bufMidiIn = jack_port_get_buffer(g_jackMidiIn, nframes);
	void *bufMidiOut = jack_port_get_buffer(g_jackMidiOut, nframes);
	jack_midi_clear_buffer(bufMidiOut);
	void *bufMidiLcdOut = jack_port_get_buffer(g_jackMidiLcdOut, nframes);
	jack_midi_clear_buffer(bufMidiLcdOut);
	void *bufMidiConOut = jack_port_get_buffer(g_jackMidiConOut, nframes);
	jack_midi_clear_buffer(bufMidiConOut);
	PARAM_ClearPreampDirtyFlags();
	PARAM_SendRefresh();

	// Process MIDI Input Data
	jack_nframes_t frameNb = jack_midi_get_event_count(bufMidiIn);
	for(jack_nframes_t frameCpt = 0; frameCpt < frameNb; ++frameCpt) {
		jack_midi_event_t event;
		int r = jack_midi_event_get(&event, bufMidiIn, frameCpt);
		if(r == 0) {
			g_midi_running = TRUE;
			//printf ("%d:", event.time); for(j = 0; j < event.size; ++j) printf(" %02x", event.buffer[j]); printf("\n");
			uint8_t type = event.buffer[0] & 0xF0;
			uint8_t channel = event.buffer[0] & 0x0F;
			if(type == 0xF0) {
				uint32_t *bbuf;
				if(MIDI_CmdDecodeLL(event.buffer, event.size, &cmd, &scmd, (uint8_t**)&bbuf, &length) == MIDI_OK) {
					if(cmd == MIDI_CMD_JADE ) {					
						Platform_Config_t *pPlatform = arg;
						Platform_control_msg req, rsp;
						uint32_t rsp_payload[10];

					    printf("Got Jade:");
						for (int i=0; i<(length/sizeof(uint32_t));i++) {
						    printf("%08x ", bbuf[i]);
						}
					    printf("\n");
						req.id = bbuf[0];
						req.size = bbuf[1];
						req.data = &(bbuf[2]);
						rsp.id = 0;
						rsp.size = 10*sizeof(uint32_t);
						rsp.data = rsp_payload;
						pPlatform->control_service_message(&req, &rsp, pPlatform->callback_param);
					    printf("RSP Jade:");
						for (int i=0; i<rsp.size/sizeof(uint32_t); i++) {
						    printf("%02x ", rsp_payload[i]);
						}
					    printf("\n");
					}
				}
			} else {
				if(channel == 0x0F) {
					if(event.size == 3) {
						switch(type) {
						case 0x90:
							g_remKey[event.buffer[1]] = REM_KEY_EVENT_MAKE;
							break;
						case 0x80:
							g_remKey[event.buffer[1]] = REM_KEY_EVENT_BREAK;
							break;
						default:
							break;
						}
					}
				} else {
					if(MIDI_QueueWriteLL(&g_midiQueueIn[channel], event.buffer, event.size)) {
						Log(LOG_ERR, "***ERROR*** MIDI_IN Queue Full\n");
					}
				}
			}
		}
	}

	// Process MIDI Output Data
	unsigned char *bufmidi;
	while(MIDI_QueueRead(&g_midiQueueOut, &msg) == MIDI_OK) {
		bufmidi = jack_midi_event_reserve(bufMidiOut, 0, msg.length);
		if(bufmidi) {
			memcpy(bufmidi, msg.data, msg.length);
		} else {
			Log(LOG_ERR, "***ERROR*** MIDI_OUT Buffer Full\n");
			break;
		}
	}
	while(MIDI_QueueRead(&g_midiQueueLcdOut, &msg) == MIDI_OK) {
		bufmidi = jack_midi_event_reserve(bufMidiLcdOut, 0, msg.length);
		if(bufmidi) {
			memcpy(bufmidi, msg.data, msg.length);
		} else {
			Log(LOG_ERR, "***ERROR*** MIDI_LCD_OUT Buffer Full\n");
			break;
		}
	}
	while(MIDI_QueueRead(&g_midiQueueConOut, &msg) == MIDI_OK) {
		bufmidi = jack_midi_event_reserve(bufMidiConOut, 0, msg.length);
		if(bufmidi) {
			memcpy(bufmidi, msg.data, msg.length);
		} else {
			Log(LOG_ERR, "***ERROR*** MIDI_CON_OUT Buffer Full\n");
			break;
		}
	}

	// CPU Monitor
	clock_gettime(CLOCK_REALTIME, &t);
	mon.ts = t.tv_sec*1000000000+t.tv_nsec;
	mon.index = MIDI_MON_STOP + 0;
	mon.priority = param.sched_priority;
	mon.core = core;
	if(MIDI_MonQueueWrite(&g_monQueue[0],&mon) != MIDI_OK) {
		Log(LOG_ERR, "***ERROR*** CPU_MONITOR Queue 0 Full\n");
	}

	g_jackProcessCpt++;
	if(g_jackProcessCpt%2 == 0) {
		uint8_t cpt = 0;
		snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%s.%s", g_szHostName, g_szModelName);
		szLength = (strlen(szStrOut) + 1 + 3); // Add room for length + reserve
		for(int i=0; i<MODEL_SYNC_TASK_COUNT+1;i++) {
			while(MIDI_MonQueueRead(&g_monQueue[i],&mon) == MIDI_OK) {
				memcpy((uint8_t *)szStrOut + szLength + cpt*sizeof(MIDI_MON_DATA),&mon,sizeof(MIDI_MON_DATA));
				cpt++;
			}
		}
	}


	return 0;
}


void jack_shutdown(void *arg) {
	fprintf(stderr, "JACK server disconnected\n");
	g_jackHandle = NULL;
	//GBLbuf.stopExecutionFlag = 1;
	sem_post(&semExit);
}

int jack_buffer_size(jack_nframes_t nframes, void *arg) {
	//printf("New JACK frame size=%d (Model Frame=%d)\n",nframes,g_modelFrameSize);
	return 0;
}

int jack_xrun(void *arg) {
	//fprintf(stderr, "JACK reported an xrun\n");
	g_xrun++;
	return 0;
}

char *DISP_formatLine(char *str) {
	int length = strlen(str);
	int i;
	for(i = length; i < DISP_NB_COL; i++) str[i] = ' ';
	str[DISP_NB_COL] = 0;
	return str;
}

void printUsage(void) {
	printf("Usage: %s [options]\n", QUOTE(MODEL));
	printf("Options:\n");
	printf("  -n              - Jack Client Name\n");
	printf("  -i              - Start with no JACK ports connected\n");
	printf("  -b <nbbuf>      - Number of buffer added on the bloc output\n");
	printf("  -c0 <mask>      - Mask for JACK Process core affinity (hex)\n");
	printf("  -c1 <mask>      - Mask for Defeered Process core affinity (hex)\n");
}

void at_exit(void) {
	PARAM_Save();
	closelog();
	if(g_jackHandle) jack_client_close(g_jackHandle);
	if(sem_destroy(&semExit)) fprintf(stderr, "Error destroying exit semaphore\n");
//	if(g_modelStarted) MODEL_FUNC(terminate)();

	// Restore Terminal
	struct termios stdtio;
	tcgetattr(0, &stdtio);
	stdtio.c_lflag |= (ICANON | ECHO | ECHONL);
	tcsetattr(0, TCSANOW, &stdtio);

	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	munlockall();
	printf("Model Cleaned up\n");
}

void sigint(int sig) {
	printf("\nSIGNAL:%d pid=%d\n", sig, getpid());
	sem_post(&semExit);
	//exit(0);
}




void platform_main(Platform_Handle_t hPlatform, int argc, char *argv[])
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)hPlatform;
   
   if (pPlatform == NULL)
   {
      printf("Error: NULL Platform handle\n");
      return;
   }

	// Lock all memory
	if(mlockall(MCL_CURRENT | MCL_FUTURE)) {
		printf("ERROR locking current pages\n");
		return;//exit(-1);
	}

	// Open Logfile
	openlog(argv[0], LOG_CONS | LOG_NDELAY, LOG_USER);
	setlogmask(LOG_UPTO(LOG_INFO));

	// Catch kill signals
	(void) signal(SIGHUP,  sigint);
	(void) signal(SIGINT,  sigint);
	(void) signal(SIGTSTP, sigint);
	(void) signal(SIGTERM, sigint);
	(void) signal(SIGQUIT, sigint);
	atexit(at_exit);

	// Parse command line options
	snprintf(g_szModelName, MODELNAME_MAX_LENGTH, "%s_%d", QUOTE(MODEL), getpid());
	static struct option long_options[] = {
		{"help", 0, 0, 'h'},
		{"name", 0, 0, 'n'},
		{"isolate", 0, 0, 'i'},
		{"buffer", 0, 0, 'b'},
		{"c0", 0, 0, 0},
		{"c1", 0, 0, 0},
		{"cpu", 0, 0, 'c'},
		{0, 0, 0, 0}
	};
	optind = 1;
	while (1) {
		int option_index = 0;
		int c = getopt_long(argc, argv, "hicn:b:0:1:", long_options, &option_index);
		if (c == -1) break;
		switch (c) {
		case 0:
			printf("option %s", long_options[option_index].name);
			if(optarg) printf(" with arg %s", optarg);
			printf("\n");
			break;
		case 'n':
			if(optarg) {
				snprintf(g_szModelName, MODELNAME_MAX_LENGTH, "%s_%d", optarg, getpid());
				printf("Model renamed: %s\n", optarg);
			}
			break;
		case 'i':
			g_modelIsolated = TRUE;
			printf("Model starts with no PORT connected\n");
			break;
		case 'b':
			if(optarg) {
				g_jackAddBufNb = atoi(optarg);
				printf("Model add %d buffer on the output\n", g_jackAddBufNb);
			}
			break;
		case '0':
			if(optarg) {
				g_affinityJack = strtol(optarg, NULL, 16);
				g_affinityJack &= 0xFF;
				printf("Jack Main Process Affinity %x\n", g_affinityJack);
			}
			break;
		case '1':
			if(optarg) {
				g_affinityDeferred = strtol(optarg, NULL, 16);
				g_affinityDeferred &= 0xFF;
				printf("Jack Deferred Process Affinity %x\n", g_affinityDeferred);
			}
			break;
		case 'c':
			break;
		default:
			printUsage();
			exit(0);
			break;
		}
	}

	printf("Talaria JACK Client\n");
	gethostname(g_szHostName, HOSTNAME_MAX_LENGTH);
	FILE *fcpuinfo = fopen("/proc/cpuinfo", "r");
	if(fcpuinfo == NULL) {
		printf("***ERROR*** Cannot Open /proc/cpuinfo\n");
		exit(-1);
	}
	char cpuinfoLine[256], *cpuInfoStr;
	do {
		cpuInfoStr = fgets(cpuinfoLine, 255, fcpuinfo);
		if(strncmp(cpuinfoLine, "model name", 10) == 0) {
			char *cpuinfoBuf = strtok(cpuinfoLine, ":");
			cpuinfoBuf = strtok(NULL, "@");
			if(cpuinfoBuf) {
				printf("CPU:%s\n", cpuinfoBuf);
				break;
			}
		}
	} while(cpuInfoStr);
	fclose(fcpuinfo);

#if 0// defined(IA32) || defined(AMD64)
	// Initialize IPP
	IppStatus retipp = ippInit();
	switch(retipp) {
	case ippStsNotSupportedCpu:
		printf("IPP: CPU Not Supported\n");
		exit(-1);
		break;
	case ippStsNonIntelCpu:
		printf("IPP: ***CPU Not Intel***\n");
		break;
	default:
	        break;
	}
	const IppLibraryVersion *lib = ippGetLibVersion();
	printf("IPP Library: %s %s\n", lib->Name, lib->Version);
#endif

	// Initialize exit semaphore
	if(sem_init(&semExit, 0, 0)) {
		fprintf(stderr, "ERROR: Initialize semExit semaphore\n");
		exit(EXIT_FAILURE);
	}

	// Initialize parameters
	PARAM_Init(QUOTE(MODEL), g_szHostName, g_szModelName, &g_midiQueueOut);

	// Initialize JACK client
	MIDI_QueueInit(&g_midiQueueOut, MIDI_QUEUE_SIZE_MAX);
	MIDI_QueueInit(&g_midiQueueConOut, MIDI_QUEUE_SIZE_MAX);
	MIDI_QueueInit(&g_midiQueueLcdOut, MIDI_QUEUE_SIZE_MAX);
	for(int k = 0; k < MIDI_CHANNEL_NB_MAX; k++) MIDI_QueueInit(&g_midiQueueIn[k], MIDI_QUEUE_SIZE_MAX);
	g_midi_running = FALSE;
	g_lcd_refresh_flag = FALSE;
	for(int k = 0; k < DISP_NB_PAGE; k++) {
		for(int i = 0; i < DISP_NB_LINE; i++) {
			memset(&g_clDispMsg[k][i], 0, sizeof(MIDI_MSG));
			g_clDispStatus[k][i] = 0;
		}
	}
	g_ecuSpeed = 0;

	// Initialize Monitor Queues
	for(int i=0; i<MODEL_SYNC_TASK_COUNT+1;i++) {
		MIDI_MonQueueInit(&g_monQueue[i], MIDI_MON_QUEUE_SIZE_MAX);
	}

	// Set JACK client
	jack_options_t joptions = JackNoStartServer;
	jack_status_t jstatus;
	if(!(g_jackHandle = jack_client_open(g_szModelName, joptions, &jstatus))) {
		fprintf (stderr, "ERROR: Unable to start JACK client\n");
		exit(EXIT_FAILURE);
	}

	// Set JACK audio processing callback
	if(jack_set_process_callback(g_jackHandle, jack_process, pPlatform)) {
		fprintf(stderr, "ERROR: Setting JACK process callback\n");
		exit(EXIT_FAILURE);
	}
	// Set JACK shutdown callback
	jack_on_shutdown(g_jackHandle, jack_shutdown, 0);

	// Set JACK xrun callback
	if(jack_set_xrun_callback(g_jackHandle, jack_xrun, 0)) {
		fprintf(stderr, "ERROR: Setting JACK xrun callback\n");
		exit(EXIT_FAILURE);
	}
	// Set JACK buffer size callback
	if(jack_set_buffer_size_callback(g_jackHandle, jack_buffer_size, 0)) {
		fprintf(stderr, "ERROR: Setting JACK buffer size callback\n");
		exit(EXIT_FAILURE);
	}

	// Register input ports
	char portName[32];
	for(int k = 0; k < SYSIN_CHANNEL_COUNT; k++) {
		sprintf(portName, "in_%02d", k + 1);
		g_jackInput[k] = jack_port_register(g_jackHandle, portName, JACK_DEFAULT_AUDIO_TYPE,
											JackPortIsInput, 0);
		if (!g_jackInput[k]) {
			fprintf(stderr, "ERROR: register input ports\n");
			exit(EXIT_FAILURE);
		}
	}
	// Register outputs ports
	for(int k = 0; k < SYSOUT_CHANNEL_COUNT; k++) {
		sprintf(portName, "out_%02d", k + 1);
		g_jackOutput[k] = jack_port_register(g_jackHandle, portName, JACK_DEFAULT_AUDIO_TYPE,
											 JackPortIsOutput, 0);
		if (!g_jackOutput[k]) {
			fprintf(stderr, "ERROR: register output ports\n");
			exit(EXIT_FAILURE);
		}
	}
	// Register MIDI output
	g_jackMidiOut = jack_port_register(g_jackHandle, "midi_out", JACK_DEFAULT_MIDI_TYPE,
									   JackPortIsOutput, 0);
	if(g_jackMidiOut == NULL) {
		fprintf(stderr, "ERROR: register MIDI output port.\n");
		exit(EXIT_FAILURE);
	}
	// Register MIDI console
	g_jackMidiConOut = jack_port_register(g_jackHandle, "con_out", JACK_DEFAULT_MIDI_TYPE,
										  JackPortIsOutput, 0);
	if(g_jackMidiConOut == NULL) {
		fprintf(stderr, "ERROR: register MIDI con_out port.\n");
		exit(EXIT_FAILURE);
	}
	// Register MIDI LCD
	g_jackMidiLcdOut = jack_port_register(g_jackHandle, "lcd_out", JACK_DEFAULT_MIDI_TYPE,
										  JackPortIsOutput, 0);
	if(g_jackMidiLcdOut == NULL) {
		fprintf(stderr, "ERROR: register MIDI lcd_out port.\n");
		exit(EXIT_FAILURE);
	}
	// Register MIDI input
	g_jackMidiIn = jack_port_register(g_jackHandle, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput,
									  0);
	if(g_jackMidiIn == NULL) {
		fprintf(stderr, "ERROR: register MIDI input port.\n");
		exit(EXIT_FAILURE);
	}

	// Start Model
	printf("Model SYSIN: Samplerate=%d Framesize=%d Channel=%d\n",
		   SYSIN_SAMPLE_RATE_IN_HERTZ, SYSIN_FRAME_SIZE_IN_SAMPLES, SYSIN_CHANNEL_COUNT);
	printf("Model SYSOUT: Samplerate=%d Framesize=%d Channel=%d\n",
		   SYSOUT_SAMPLE_RATE_IN_HERTZ, SYSOUT_FRAME_SIZE_IN_SAMPLES, SYSOUT_CHANNEL_COUNT);
	printf("Starting Model: %s\n", g_szModelName);

//	g_BAFobj = OJSP_framework_init();
   pPlatform->callback_param = pPlatform->framework_init();
	printf("Platform initialized\n");
	printf("Model Started\n");
//	MODEL_FUNC(initialize)();
	g_modelStarted = TRUE;
	PARAM_Load();

	// Check Model Samplerate
	if(SYSIN_SAMPLE_RATE_IN_HERTZ != SYSOUT_SAMPLE_RATE_IN_HERTZ) {
		printf("ERROR: Samplerate Model IN/OUT does not match\n");
		exit(EXIT_FAILURE);
	}
	// Check Model Frame Size
	if(SYSIN_FRAME_SIZE_IN_SAMPLES != SYSOUT_FRAME_SIZE_IN_SAMPLES) {
		printf("ERROR: Frame Size Model IN/OUT does not match\n");
		exit(EXIT_FAILURE);
	}
	// Check Model Task Rate Factor
	if(SYSIN_TASK_RATE_FACTOR != SYSOUT_TASK_RATE_FACTOR) {
		printf("ERROR: Task Rate Factor IN/OUT does not match\n");
		exit(EXIT_FAILURE);
	}
	if((SYSIN_TASK_RATE_FACTOR != 1) && (SYSIN_TASK_RATE_FACTOR != SYSIN_FRAME_SIZE_IN_SAMPLES)) {
		printf("ERROR: Task Rate Factor Should 1 or Model Frame Size\n");
		exit(EXIT_FAILURE);
	}
	g_modelSampleRate = SYSIN_SAMPLE_RATE_IN_HERTZ;
	g_modelFrameSize = SYSIN_FRAME_SIZE_IN_SAMPLES;
	g_jackSampleRate = jack_get_sample_rate(g_jackHandle);
	g_jackFrameSize = jack_get_buffer_size(g_jackHandle);
	printf("JACK Samplerate=%d Frame=%d\n", g_jackSampleRate, g_jackFrameSize);

	if(g_jackSampleRate != g_modelSampleRate) {
		printf("ERROR: Samplerate Model=%d JACK=%d\n", g_modelSampleRate, g_jackSampleRate);
		exit(EXIT_FAILURE);
	}
	if(g_jackFrameSize > AUDIO_BUF_SIZE) {
		printf("ERROR: Jack Frame Size too big JACK=%d MAX=%d\n", g_jackSampleRate, AUDIO_BUF_SIZE);
		exit(EXIT_FAILURE);
	}
	if(g_modelFrameSize > AUDIO_BUF_SIZE / 2) {
		printf("ERROR: Model Frame Size too big MODEL=%d MAX=%d\n", g_jackSampleRate, AUDIO_BUF_SIZE);
		exit(EXIT_FAILURE);
	}
	if((g_modelFrameSize + g_jackAddBufNb * g_jackFrameSize) > AUDIO_BUF_SIZE) {
		printf("ERROR: Model Frame Size(+ added buffering) too big MAX=%d\n", AUDIO_BUF_SIZE);
		exit(EXIT_FAILURE);
	}
	g_jackFrameSize = 0;

	// Start Jack Deferred Thread
	sem_init(&g_semDeferredStart, 0, 0);
	sem_init(&g_semDeferredDone, 0, 0);
	int ret = pthread_create(&g_threadDeferred, NULL, &THREAD_Deferred, pPlatform);
	if(ret) {
		printf("ERROR: Create THREAD_Deferred %d (%s)\n", ret, strerror(ret));
		exit(EXIT_FAILURE);
	}
	// Start Multi-Rate Threads
	if(MODEL_SYNC_TASK_COUNT == 1) {
		printf("Single-Rate Model\n");
	}
	// Activate JACK client
	printf("Starting JACK Client\n");
	if(jack_activate(g_jackHandle)) {
		fprintf(stderr, "ERROR: activating JACK client");
		exit(EXIT_FAILURE);
	}

	// Connect Audio Ports
	char **ports;
	if(g_modelIsolated == FALSE) {
		if((ports = (char **)jack_get_ports(g_jackHandle, NULL, NULL,
											JackPortIsPhysical | JackPortIsOutput)) == NULL)
			fprintf(stderr, "ERROR: Cannot locate any physical capture ports\n");
		int k = 0;
		int i = 0;
		do {
			if(ports[k] == NULL) {
				fprintf(stderr, "WARNING: Not enough physical capture ports\n");
				break;
			} else {
				jack_port_t *portSource = jack_port_by_name(g_jackHandle, ports[k]);
				if(strcmp("32 bit float mono audio", jack_port_type(portSource)) == 0) {
					int ret = jack_connect(g_jackHandle, ports[k], jack_port_name(g_jackInput[i]));
					if((ret != 0) && (ret != EEXIST)) {
						fprintf(stderr, "Cannot connect input port %s to physical capture port %s\n",
								jack_port_name(g_jackInput[i]), ports[k]);
						free(ports);
						exit(EXIT_FAILURE);
					} else i++;
				}
			}
			k++;
		} while(i < SYSIN_CHANNEL_COUNT);
		free(ports);

		if ((ports = (char **)jack_get_ports(g_jackHandle, NULL, NULL,
											 JackPortIsPhysical | JackPortIsInput)) == NULL)
			fprintf(stderr, "ERROR: Cannot locate any physical playback ports\n");
		k = 0;
		i = 0;
		do {
			if(ports[k] == NULL) {
				fprintf(stderr, "WARNING: Not enough physical playback ports\n");
				break;
			} else {
				jack_port_t *portDest = jack_port_by_name(g_jackHandle, ports[k]);
				if(strcmp("32 bit float mono audio", jack_port_type(portDest)) == 0) {
					int ret = jack_connect(g_jackHandle, jack_port_name(g_jackOutput[i]), ports[k]);
					if((ret != 0) && (ret != EEXIST)) {
						fprintf(stderr, "Cannot connect output port %s to physical playback port %s\n",
								jack_port_name(g_jackOutput[i]), ports[k]);
						free(ports);
						exit(EXIT_FAILURE);
					} else i++;
				}
			}
			k++;
		} while(i < SYSOUT_CHANNEL_COUNT);
		free(ports);
	}

	jack_port_t *portMidi;
	portMidi = jack_port_by_name(g_jackHandle, "pq_daemon_in:con_in");
	if(portMidi) {
		int ret = jack_connect(g_jackHandle, jack_port_name(g_jackMidiConOut), "pq_daemon_in:con_in");
		if((ret != 0) && (ret != EEXIST)) {
			fprintf(stderr, "ERROR: Cannot connect MIDI Console port to pq_daemon_in:con_in\n");
			exit(EXIT_FAILURE);
		}
	} else fprintf(stderr, "WARNING: Cannot connect MIDI Console port to pq_daemon_in:con_in\n");

	portMidi = jack_port_by_name(g_jackHandle, "pq_daemon_in:lcd_in");
	if(portMidi) {
		int ret = jack_connect(g_jackHandle, jack_port_name(g_jackMidiLcdOut), "pq_daemon_in:lcd_in");
		if((ret != 0) && (ret != EEXIST)) {
			fprintf(stderr, "ERROR: Cannot connect MIDI Console port to pq_daemon_in:lcd_in\n");
			exit(EXIT_FAILURE);
		}
	} else fprintf(stderr, "WARNING: Cannot connect MIDI Console port to pq_daemon_in:lcd_in\n");

	portMidi = jack_port_by_name(g_jackHandle, "pq_daemon_in:midi_in");
	if(portMidi) {
		int ret = jack_connect(g_jackHandle, jack_port_name(g_jackMidiOut), "pq_daemon_in:midi_in");
		if((ret != 0) && (ret != EEXIST)) {
			fprintf(stderr, "ERROR: Cannot connect MIDI output port to pq_daemon_in:midi_in\n");
			exit(EXIT_FAILURE);
		}
	} else fprintf(stderr, "WARNING: Cannot connect MIDI output port to pq_daemon_in:midi_in\n");

	portMidi = jack_port_by_name(g_jackHandle, "pq_daemon:midi_out");
	if(portMidi) {
		int ret = jack_connect(g_jackHandle, "pq_daemon:midi_out", jack_port_name(g_jackMidiIn));
		if((ret != 0) && (ret != EEXIST)) {
			fprintf(stderr, "ERROR: Cannot connect pq_daemon:midi_out to MIDI input\n");
			exit(EXIT_FAILURE);
		}
	} else fprintf(stderr, "WARNING: Cannot connect pq_daemon:midi_out to MIDI input\n");

	// Set process priority
	pthread_t bgThread;
	struct sched_param schp;
	int schedPolicy;
	bgThread = pthread_self();
	schedPolicy = SCHED_FIFO;
	if (pthread_getschedparam(bgThread, &schedPolicy, &schp)) {
		fprintf(stderr, "Error getting thread scheduling parameters\n");
		exit(EXIT_FAILURE);
	}
	schp.sched_priority = sched_get_priority_min(SCHED_FIFO);
	if (pthread_setschedparam(bgThread, SCHED_FIFO, &schp)) {
		fprintf(stderr, "Error setting thread scheduling parameters\n");
		exit(EXIT_FAILURE);
	}

	sem_wait(&semExit);
	printf("Exit Semaphore Trigerred\n");
	exit(0);
}


// BDSP Processing
#define BDSP_MAX_READ_WORDS 32
#define BDSP_MAX_WRITE_WORDS 32
int g_BDSP_arg[8];
void BDSP_Process(jack_midi_event_t *event, uint8_t cmd, uint8_t scmd, uint8_t *bbuf, uint8_t length) {
	uint8_t *bufBin;
	int szLength;
	char szStrIn[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];
	char szStrOut[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];
	char *szpstr;
	MIDI_MSG msg;
	char *szParamName;

	switch(scmd) {
	case MIDI_SCMD_RTP_BDSP_BR:
		// a=pql('cmd','rtp_bdsp_br','bdsp','134608060','32')
		// a=pq('read')
		// a=pq('read')
		// a{6}

		memcpy(szStrIn, bbuf, length);
		szStrIn[length] = 0;
		szpstr = strtok(szStrIn, " ");
		if(!PARAM_checkName(szpstr, &szParamName)) {
			//printf("BDSP_BR: %s\n",szpstr);
			for(int i = 0; i < 2; i++) {
				szpstr = strtok(NULL, " ");
				//printf("BDSP_ARG[%d]=%s\n",i+1,szpstr);
				if(szpstr)
					g_BDSP_arg[i + 1] = atoi(szpstr);
				else
					g_BDSP_arg[i + 1] = -1;
			}

			if(g_BDSP_arg[2] > BDSP_MAX_READ_WORDS) {
				g_BDSP_arg[2] = BDSP_MAX_READ_WORDS;
				printf("ERROR: RTP_BDSP_BR called with length>%d now set to %d.\n", BDSP_MAX_READ_WORDS,
					   BDSP_MAX_READ_WORDS);
			}
			snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%s.%s.%s %d %d %s",
					 g_szHostName, g_szModelName, "bdsp", g_BDSP_arg[1], g_BDSP_arg[2], "INTEGER");
			szLength = strlen(szStrOut) + 1;
			for(int i = 0; i < g_BDSP_arg[2]; i++) {
				*(int32_t *)((uint8_t *)szStrOut + szLength + (4 * i)) = (int32_t)(*((int32_t *)((long)(g_BDSP_arg[1] +
						(4 * i)))));
			}
			MIDI_CmdEncode(&msg, MIDI_CMD_RTP, MIDI_SCMD_RTP_VALUE, (uint8_t *)szStrOut,
						   szLength + 4 * g_BDSP_arg[2]);
			if(MIDI_QueueWrite(&g_midiQueueOut, &msg) != MIDI_OK) {
				Log(LOG_ERR, "***ERROR*** MIDI_OUT Queue Full\n");
			}
		}
		break;
	case MIDI_SCMD_RTP_BDSP_BW:
		//a=pql('cmd','rtp_bdsp_bw','bdsp','0','2','integer',[1234 3456])
		memcpy(szStrIn, bbuf, length);
		szStrIn[length] = 0;
		szpstr = strtok(szStrIn, " ");
		if(!PARAM_checkName(szpstr, &szParamName)) {
			//printf("BDSP_BR: %s\n",szpstr);
			bufBin = (uint8_t *)(&event->buffer[9] + strnlen((char *)&event->buffer[9], event->size) + 1);
			for(int i = 0; i < 3; i++) {
				szpstr = strtok(NULL, " ");
				//printf("BDSP_ARG[%d]=%s\n",i+1,szpstr);
				if(szpstr)
					g_BDSP_arg[i + 1] = atoi(szpstr);
				else
					g_BDSP_arg[i + 1] = -1;
			}
			if(g_BDSP_arg[2] > BDSP_MAX_WRITE_WORDS) {
				g_BDSP_arg[2] = BDSP_MAX_WRITE_WORDS;
				printf("ERROR: RTP_BDSP_BW called with length>%d now set to %d.\n", BDSP_MAX_WRITE_WORDS,
					   BDSP_MAX_WRITE_WORDS);
			}
			for(int i = 0; i < g_BDSP_arg[2]; i++) {
				*((uint32_t *)((long)(g_BDSP_arg[1] + (4 * i)))) = *((uint32_t *)((uint8_t *)(bufBin + 4 * i)));
			}
			//printf("%d %d\n",*((int32_t*)((uint8_t*)(bufBin))),*((int32_t*)((uint8_t*)(bufBin+4))));
			// Return value is simply the length that was actually written.
			snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%s.%s.%s %d %d %s",
					 g_szHostName, g_szModelName, "bdsp", g_BDSP_arg[1], g_BDSP_arg[2], "INTEGER");
			szLength = strlen(szStrOut) + 1;
			for(int i = 0; i < g_BDSP_arg[2]; i++) {
				*(int32_t *)((uint8_t *)szStrOut + szLength + (4 * i)) = (int32_t)(*((int32_t *)((long)(g_BDSP_arg[1] +
						(4 * i)))));
			}

			MIDI_CmdEncode(&msg, MIDI_CMD_RTP, MIDI_SCMD_RTP_VALUE, (uint8_t *)szStrOut,
						   szLength + 4 * g_BDSP_arg[2]);
			if(MIDI_QueueWrite(&g_midiQueueOut, &msg) != MIDI_OK) {
				Log(LOG_ERR, "***ERROR*** MIDI_OUT Queue Full\n");
			}
		}
		break;
	default:
		break;
	}
}

/*
 * ht_getIDindex - input a rigid body ID and get the associated index in the message table for that ID,
 *  return -1 if the maximum number of IDs has been hit
 */
int ht_getIDindex(float ID) {
    static int ht_id_num = 0; // number of IDs registered in the ID table
    int search_index; // index used for searching the ID table

    // search our ID list for the target ID
    for( search_index=0; search_index < HEAD_TRACKER_IDS_MAX; search_index++ ) {
        if( g_rigidBodyIDs[search_index] == ID ) {
            return search_index;
        }
    }
    // we don't currently have this ID registered, as long as we have space in our table, enter the ID into the next
    //  available open slot.
    if( ht_id_num < HEAD_TRACKER_IDS_MAX ) {
        g_rigidBodyIDs[ht_id_num] = ID;
        return ht_id_num++;
    }
    // we don't have this ID registered and there is no space to register it in our table
    return -1;
}

int platform_vprintf (const char * format, va_list arg )
{
   return vprintf ( format, arg );
}

int platform_printf(char* format, ...) 
{
   va_list args;
   int val;
   va_start( args, format );
   val = vprintf(format, args );
   va_end(args);
   return val;
}

void platform_thread_handler(void *pAppHandle, void *pArg)
{
   Platform_Config_t *pPlatform = (Platform_Config_t *)pAppHandle;
   pPlatform->bafexec(pPlatform->callback_param, (uint32_t)pArg);
}


void platform_thread_init(void)
{
   ;
}

void platform_thread_add(int callrate_index)
{
   ;
}

int platform_thread_post(int callrate_index, void* baf, void* arg)
{
   return 0;
}
