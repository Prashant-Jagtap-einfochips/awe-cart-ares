// File    : linux_jack_main.h
#ifndef LINUX_JACK_MAIN_H
#define LINUX_JACK_MAIN_H
 
// #ifndef EXIT_FAILURE
//     #define EXIT_FAILURE  1
// #endif
// #ifndef EXIT_SUCCESS
//     #define EXIT_SUCCESS  0
// #endif

#define REM_KEY_1		0
#define REM_KEY_2		1
#define REM_KEY_3		2
#define REM_KEY_4		3
#define REM_KEY_5		4
#define REM_KEY_6		5
#define REM_KEY_7		6
#define REM_KEY_8		7
#define REM_KEY_9		8
#define REM_KEY_0		9
#define REM_KEY_A		10
#define REM_KEY_B		11
#define REM_KEY_C		12
#define REM_KEY_D		13
#define REM_KEY_E		14
#define REM_KEY_F		15
#define REM_KEY_POWER	16
#define REM_KEY_VOL_P	17
#define REM_KEY_VOL_M	18
#define REM_KEY_CH_P	19
#define REM_KEY_CH_M	20
#define REM_KEY_MUTE	21
#define REM_KEY_OK		22
#define REM_KEY_UP		23
#define REM_KEY_DOWN	24
#define REM_KEY_LEFT	25
#define REM_KEY_RIGHT	26
#define REM_KEY_PLAY	27
#define REM_KEY_STOP	28
#define REM_KEY_PAUSE	29
#define REM_KEY_REC		30
#define REM_KEY_FF		31
#define REM_KEY_REW		32
#define REM_KEY_SELECT	33
#define REM_KEY_CANCEL	34
#define REM_KEY_DVD		35
#define REM_KEY_TV		36
#define REM_KEY_G		37
#define REM_KEY_H		38
#define REM_KEY_I		39
#define REM_KEY_J		40
#define REM_KEY_K		41
#define REM_KEY_L		42
#define REM_KEY_M		43
#define REM_KEY_N		44
#define REM_KEY_O		45
#define REM_KEY_P		46
#define REM_KEY_Q		47
#define REM_KEY_R		48
#define REM_KEY_S		49
#define REM_KEY_T		50
#define REM_KEY_U		51
#define REM_KEY_V		52
#define REM_KEY_W		53
#define REM_KEY_X		54
#define REM_KEY_Y		55
#define REM_KEY_Z		56
#define REM_KEY_MAX_NUMBER REM_KEY_Z
#define REM_KEY_UNKNOWN	255

#define REM_KEY_EVENT_BREAK	0
#define REM_KEY_EVENT_MAKE	1
#define REM_KEY_EVENT_HOLD	2
extern int g_remKey[256];

#define HOSTNAME_MAX_LENGTH 13
#define MODELNAME_MAX_LENGTH 32
extern char g_szHostName[HOSTNAME_MAX_LENGTH];
extern char g_szModelName[MODELNAME_MAX_LENGTH];

#define DISP_NB_COL 20
extern char *DISP_formatLine(char *str);

extern int32_t g_midi_running;
extern MIDI_QUEUE g_midiQueueIn[MIDI_CHANNEL_NB_MAX];
extern MIDI_QUEUE g_midiQueueOut;
extern MIDI_QUEUE g_midiQueueConOut;
extern MIDI_QUEUE g_midiQueueLcdOut;

#define DISP_NB_PAGE (MIDI_LCD_NB_PAGE-1)
#define DISP_NB_LINE (MIDI_LCD_NB_LINE)
extern MIDI_MSG g_clDispMsg[DISP_NB_PAGE][DISP_NB_LINE];
extern int g_clDispStatus[DISP_NB_PAGE][DISP_NB_LINE];


#define HEAD_TRACKER_IDS_MAX        20
#define HEAD_TRACKER_DOF_DIM_NUM    10

extern float g_ecuSpeed;
extern float g_headTrackerCH;
extern float g_headTrackerDOF[HEAD_TRACKER_DOF_DIM_NUM];
extern float g_rigidBodyIDs[HEAD_TRACKER_IDS_MAX];
extern float g_rigidBodyDOFs[HEAD_TRACKER_IDS_MAX][HEAD_TRACKER_DOF_DIM_NUM];
// we have to make sure ht_getIDindex is extern so that the same static index variable 
//  is used for both linux_jack_main and pq4HeadTracker. We are safe to use this method
//  (in addition to all of the other extern variables are safe to do this) because 
//  linux_jack_main's processing is single threaded.
extern int ht_getIDindex(float ID);

#endif
