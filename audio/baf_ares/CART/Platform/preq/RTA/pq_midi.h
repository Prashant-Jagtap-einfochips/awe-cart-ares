//	Title:  pq_midi.h
//	Author: Guillaume

#ifndef _PQ_MIDI_H
#define _PQ_MIDI_H
#ifdef __cplusplus
extern "C" {
#endif

#define MIDI_OK       		0
#define MIDI_ERROR			-1
#define MIDI_QUEUE_ERROR    -2
#define MIDI_QUEUE_EMPTY    -3
#define MIDI_QUEUE_FULL     -4
#define MIDI_MSG_ERROR		-5

#define MIDI_QUEUE_SIZE_MAX 		512
#define MIDI_MSG_HEADER_SIZE		10
#define MIDI_MSG_PAYLOAD_SIZE_MAX	256
#define MIDI_MSG_SIZE_MAX			(MIDI_MSG_HEADER_SIZE+MIDI_MSG_PAYLOAD_SIZE_MAX)
#define MIDI_MSG_SIZE_MIN			(MIDI_MSG_HEADER_SIZE)

typedef struct {
	SINT32 length;
	BYTE data[MIDI_MSG_SIZE_MAX];
} MIDI_MSG;

typedef struct {
	SINT32 start;
	SINT32 end;
	SINT32 size;
	MIDI_MSG msg[MIDI_QUEUE_SIZE_MAX];
} MIDI_QUEUE;

typedef struct {
	BYTE header0;
	BYTE header1;
	BYTE header2;
	BYTE header3;
	BYTE header4;
	BYTE cmd;
	BYTE scmd;
	BYTE header7;
	BYTE length;
} MIDI_CMD_HEADER;

#define MIDI_CHANNEL_NB_MAX		16
#define MIDI_MSG_CHANNEL_MASK	0x0F
#define MIDI_MSG_TYPE_MASK		0xF0
#define MIDI_MSG_TYPE_NOTE_ON	0x90
#define MIDI_MSG_TYPE_NOTE_OFF	0x80
#define MIDI_MSG_TYPE_CTRL		0xB0
#define MIDI_MSG_TYPE_SYSEX		0xF0

#define MIDI_LCD_NB_PAGE	15
#define MIDI_LCD_NB_LINE	4
#define MIDI_LCD_NB_COL		20

SINT32 MIDI_QueueInit(MIDI_QUEUE *mq, SINT32 size);
SINT32 MIDI_QueueClear(MIDI_QUEUE *mq);
SINT32 MIDI_QueueRead(MIDI_QUEUE *mq, MIDI_MSG *mm);
SINT32 MIDI_QueueReadLL(MIDI_QUEUE *mq, BYTE *data, SINT32 *length);
SINT32 MIDI_QueueWrite(MIDI_QUEUE *mq, MIDI_MSG *mm);
SINT32 MIDI_QueueWriteLL(MIDI_QUEUE *mq, BYTE *data, SINT32 length);
SINT32 MIDI_QueueSize(MIDI_QUEUE *mq);
SINT32 MIDI_QueueNbMsg(MIDI_QUEUE *mq);


#define MIDI_RTP_DATA_SIZE_SZ               126
#define MIDI_RTP_DATA_SIZE_BIN              128
#define MIDI_RTP_DATA_TYPE_NB               3
#define MIDI_RTP_DATA_TYPE_INTEGER          0
#define MIDI_RTP_DATA_TYPE_INTEGER_SIZE     4
#define MIDI_RTP_DATA_TYPE_SINGLE           1
#define MIDI_RTP_DATA_TYPE_SINGLE_SIZE      4
#define MIDI_RTP_DATA_TYPE_DOUBLE           2
#define MIDI_RTP_DATA_TYPE_DOUBLE_SIZE      8
#define MIDI_RTP_SYSMASK_SIZE               4
#define MIDI_RTP_SYSMASK_MAX                (1 << (4*MIDI_RTP_SYSMASK_SIZE)) - 1
extern const char *MIDI_RTP_DATA_TYPE_STRING[];
extern const int MIDI_RTP_DATA_TYPE_SIZE[];

#define MIDI_CMD				0
#define MIDI_CMD_LCD_PAGE1		1
#define MIDI_CMD_LCD_PAGE2		2
#define MIDI_CMD_LCD_PAGE3		3
#define MIDI_CMD_LCD_PAGE4		4
#define MIDI_CMD_LCD			5
#define MIDI_CMD_MONITOR		6
#define MIDI_CMD_MODEL			20
#define MIDI_CMD_PREAMP			21
#define MIDI_CMD_PLAYER			22
#define MIDI_CMD_GEN			23
#define MIDI_CMD_ECU			24
#define MIDI_CMD_RTP			25
#define MIDI_CMD_HEAD_TRACKER	26
#define MIDI_CMD_JADE           27
#define MIDI_CMD_WP             28
#define MIDI_CMD_INVALID		0xFF
#define MIDI_SCMD_INVALID		0xFF

#define MIDI_SCMD_CONSOLE		0
#define MIDI_SCMD_LCD_REFRESH	1
#define MIDI_SCMD_LCD_FRAME_END	2


#define MIDI_SCMD_MONITOR_CPU   0
#define MIDI_SCMD_MONITOR_CPU2  1
#define MIDI_MON_QUEUE_SIZE_MAX	32
#define MIDI_MON_START	0
#define MIDI_MON_STOP	0x80
typedef struct {
	UINT32 ts;
	BYTE index;		// Rate index + Start/Stop
	BYTE priority;
	BYTE core;
	BYTE reserved;
} MIDI_MON_DATA;
typedef struct {
	SINT32 start;
	SINT32 end;
	SINT32 size;
	MIDI_MON_DATA data[MIDI_MON_QUEUE_SIZE_MAX];
} MIDI_MON_QUEUE;
SINT32 MIDI_MonQueueInit(MIDI_MON_QUEUE *mq, SINT32 size);
SINT32 MIDI_MonQueueClear(MIDI_MON_QUEUE *mq);
SINT32 MIDI_MonQueueRead(MIDI_MON_QUEUE *mq, MIDI_MON_DATA *data);
SINT32 MIDI_MonQueueWrite(MIDI_MON_QUEUE *mq, MIDI_MON_DATA *data);
SINT32 MIDI_MonQueueSize(MIDI_MON_QUEUE *mq);
SINT32 MIDI_MonQueueNbMsg(MIDI_MON_QUEUE *mq);


#define MIDI_SCMD_MODEL_START		0
#define MIDI_SCMD_MODEL_RESTART		1
#define MIDI_SCMD_MODEL_STOP		2

#define MIDI_SCMD_PREAMP_SYSTEM		0
#define MIDI_SCMD_PREAMP_VOL		1
#define MIDI_SCMD_PREAMP_VOL_P		2
#define MIDI_SCMD_PREAMP_VOL_M		3
#define MIDI_SCMD_PREAMP_BAL		4
#define MIDI_SCMD_PREAMP_BAL_P		5
#define MIDI_SCMD_PREAMP_BAL_M		6
#define MIDI_SCMD_PREAMP_FAD		7
#define MIDI_SCMD_PREAMP_FAD_P		8
#define MIDI_SCMD_PREAMP_FAD_M		9
#define MIDI_SCMD_PREAMP_BASS		10
#define MIDI_SCMD_PREAMP_BASS_P		11
#define MIDI_SCMD_PREAMP_BASS_M		12
#define MIDI_SCMD_PREAMP_MID		13
#define MIDI_SCMD_PREAMP_MID_P		14
#define MIDI_SCMD_PREAMP_MID_M		15
#define MIDI_SCMD_PREAMP_TREB		16
#define MIDI_SCMD_PREAMP_TREB_P		17
#define MIDI_SCMD_PREAMP_TREB_M		18
#define MIDI_SCMD_PREAMP_STATUS		19
#define MIDI_SCMD_PREAMP_MUTE		20
#define MIDI_SCMD_PREAMP_PARAM1		21
#define MIDI_SCMD_PREAMP_PARAM1_P	22
#define MIDI_SCMD_PREAMP_PARAM1_M	23
#define MIDI_SCMD_PREAMP_PARAM2		24
#define MIDI_SCMD_PREAMP_PARAM2_P	25
#define MIDI_SCMD_PREAMP_PARAM2_M	26
#define MIDI_SCMD_PREAMP_RESET		27

#define MIDI_SCMD_PLAYER_STATUS		0
#define MIDI_SCMD_PLAYER_PLAY		1
#define MIDI_SCMD_PLAYER_PAUSE		2
#define MIDI_SCMD_PLAYER_REW		3
#define MIDI_SCMD_PLAYER_FF			4

#define MIDI_SCMD_GEN_RESET 			0
#define MIDI_SCMD_GEN_GET_STATUS 		1
#define MIDI_SCMD_GEN_STATUS			2
#define MIDI_SCMD_GEN_SET_TYPE			3
#define MIDI_SCMD_GEN_GET_TYPE			4
#define MIDI_SCMD_GEN_TYPE				5
#define MIDI_SCMD_GEN_SET_FREQ			6
#define MIDI_SCMD_GEN_GET_FREQ			7
#define MIDI_SCMD_GEN_FREQ				8
#define MIDI_SCMD_GEN_SET_LEVEL			9
#define MIDI_SCMD_GEN_GET_LEVEL			10
#define MIDI_SCMD_GEN_LEVEL				11
#define MIDI_SCMD_GEN_SET_PHASE			12
#define MIDI_SCMD_GEN_GET_PHASE			13
#define MIDI_SCMD_GEN_PHASE				14
#define MIDI_SCMD_GEN_SET_FILT  		15
#define MIDI_SCMD_GEN_GET_FILT			16
#define MIDI_SCMD_GEN_FILT				17
#define MIDI_SCMD_GEN_SET_WIN			18
#define MIDI_SCMD_GEN_GET_WIN			19
#define MIDI_SCMD_GEN_WIN				20

#define MIDI_WP_MODE_NORMAL             0
#define MIDI_WP_MODE_MONO               1
#define MIDI_WP_MODE_INVERT             2
#define MIDI_WP_MODE_PHASE              4
#define MIDI_WP_MODE_RHD                8
#define MIDI_WP_SRC_MUTE                0
#define MIDI_WP_SRC_MUSIC               1
#define MIDI_WP_SRC_SINE                2
#define MIDI_WP_SRC_HISS                3
#define MIDI_WP_SRC_HASH                4
#define MIDI_WP_SRC_AUX                 5
#define MIDI_WP_FILT_NONE               0
#define MIDI_WP_FILT_LP                 1
#define MIDI_WP_FILT_HP                 2
#define MIDI_WP_FILT_BP                 3
#define MIDI_WP_FILT_BS                 4

#define MIDI_SCMD_WP_RESET              0
#define MIDI_SCMD_WP_GET_STATUS         1
#define MIDI_SCMD_WP_STATUS             2
#define MIDI_SCMD_WP_SET_MODE           3
#define MIDI_SCMD_WP_GET_MODE           4
#define MIDI_SCMD_WP_MODE               5
#define MIDI_SCMD_WP_SET_SRC            6
#define MIDI_SCMD_WP_GET_SRC            7
#define MIDI_SCMD_WP_SRC                8
#define MIDI_SCMD_WP_SET_SINEF          9
#define MIDI_SCMD_WP_GET_SINEF          10
#define MIDI_SCMD_WP_SINEF              11
#define MIDI_SCMD_WP_SET_MAG_LR         12
#define MIDI_SCMD_WP_GET_MAG_LR         13
#define MIDI_SCMD_WP_MAG_LR             14
#define MIDI_SCMD_WP_SET_PHASE_LR       15
#define MIDI_SCMD_WP_GET_PHASE_LR       16
#define MIDI_SCMD_WP_PHASE_LR           17
#define MIDI_SCMD_WP_SET_FILT_TYPE      18
#define MIDI_SCMD_WP_GET_FILT_TYPE      19
#define MIDI_SCMD_WP_FILT_TYPE          20
#define MIDI_SCMD_WP_SET_FILT_ORDER     21
#define MIDI_SCMD_WP_GET_FILT_ORDER     22
#define MIDI_SCMD_WP_FILT_ORDER         23
#define MIDI_SCMD_WP_SET_FILT_FREQ      24
#define MIDI_SCMD_WP_GET_FILT_FREQ      25
#define MIDI_SCMD_WP_FILT_FREQ          26
#define MIDI_SCMD_WP_SET_FILT_SPAN      27
#define MIDI_SCMD_WP_GET_FILT_SPAN      28
#define MIDI_SCMD_WP_FILT_SPAN          29

#define MIDI_SCMD_ECU_SPEED				0
#define MIDI_SCMD_HEAD_TRACKER			0
#define MIDI_SCMD_JADE_MSG              0

#define MIDI_SCMD_RTP_REFRESH 			0
#define MIDI_SCMD_RTP_SCAN              1
#define MIDI_SCMD_RTP_INFO              2
#define MIDI_SCMD_RTP_VALUE 			3
#define MIDI_SCMD_RTP_WRITE             4
#define MIDI_SCMD_RTP_READ              5
#define MIDI_SCMD_RTP_VALUE_SYS         6
#define MIDI_SCMD_RTP_WRITE_SYS         7
#define MIDI_SCMD_RTP_READ_SYS          8
#define MIDI_SCMD_RTP_BDSP_BR           9
#define MIDI_SCMD_RTP_BDSP_BW           10
#define MIDI_SCMD_RTP_ADD_SYS           11
#define MIDI_SCMD_RTP_MUL_SYS           12
#define MIDI_SCMD_RTP_OR_SYS            13
#define MIDI_SCMD_RTP_AND_SYS           14
#define MIDI_SCMD_RTP_XOR_SYS           15

SINT32 MIDI_CmdDecode(MIDI_MSG *mm, BYTE *cmd, BYTE *scmd, BYTE **buf, BYTE *size);
SINT32 MIDI_CmdDecodeLL(BYTE *data, SINT32 length, BYTE *cmd, BYTE *scmd, BYTE **buf, BYTE *size);
SINT32 MIDI_CmdEncode(MIDI_MSG *mm, BYTE cmd, BYTE scmd, BYTE *buf, BYTE size);
SINT32 MIDI_CmdEncodeLL(BYTE *data, SINT32 *length, BYTE cmd, BYTE scmd, BYTE *buf, BYTE size);


#define MIDI_DAEMON_CH			15
SINT32 MIDI_NoteOnDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_NoteOnDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_NoteOnEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value);
SINT32 MIDI_NoteOnEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value);

SINT32 MIDI_NoteOffDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_NoteOffDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_NoteOffEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value);
SINT32 MIDI_NoteOffEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value);

#define MIDI_LED_SYSTEM_A		120
#define MIDI_LED_SYSTEM_B		121
#define MIDI_LED_SYSTEM_C		122
#define MIDI_LED_SYSTEM_D		123
#define MIDI_LED_SYSTEM_E		124
#define MIDI_LED_SYSTEM_F		125
SINT32 MIDI_CtrlDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_CtrlDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value);
SINT32 MIDI_CtrlEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value);
SINT32 MIDI_CtrlEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value);

#define MIDI_KEY_1		0
#define MIDI_KEY_2		1
#define MIDI_KEY_3		2
#define MIDI_KEY_4		3
#define MIDI_KEY_5		4
#define MIDI_KEY_6		5
#define MIDI_KEY_7		6
#define MIDI_KEY_8		7
#define MIDI_KEY_9		8
#define MIDI_KEY_0		9
#define MIDI_KEY_A		10
#define MIDI_KEY_B		11
#define MIDI_KEY_C		12
#define MIDI_KEY_D		13
#define MIDI_KEY_E		14
#define MIDI_KEY_F		15
#define MIDI_KEY_POWER	16
#define MIDI_KEY_VOL_P	17
#define MIDI_KEY_VOL_M	18
#define MIDI_KEY_CH_P	19
#define MIDI_KEY_CH_M	20
#define MIDI_KEY_MUTE	21
#define MIDI_KEY_OK		22
#define MIDI_KEY_UP		23
#define MIDI_KEY_DOWN	24
#define MIDI_KEY_LEFT	25
#define MIDI_KEY_RIGHT	26
#define MIDI_KEY_PLAY	27
#define MIDI_KEY_STOP	28
#define MIDI_KEY_PAUSE	29
#define MIDI_KEY_REC	30
#define MIDI_KEY_FF		31
#define MIDI_KEY_REW	32
#define MIDI_KEY_SELECT	33
#define MIDI_KEY_CANCEL	34
#define MIDI_KEY_DVD	35
#define MIDI_KEY_TV		36
#define MIDI_KEY_G		37
#define MIDI_KEY_H		38
#define MIDI_KEY_I		39
#define MIDI_KEY_J		40
#define MIDI_KEY_K		41
#define MIDI_KEY_L		42
#define MIDI_KEY_M		43
#define MIDI_KEY_N		44
#define MIDI_KEY_O		45
#define MIDI_KEY_P		46
#define MIDI_KEY_Q		47
#define MIDI_KEY_R		48
#define MIDI_KEY_S		49
#define MIDI_KEY_T		50
#define MIDI_KEY_U		51
#define MIDI_KEY_V		52
#define MIDI_KEY_W		53
#define MIDI_KEY_X		54
#define MIDI_KEY_Y		55
#define MIDI_KEY_Z		56
#define MIDI_KEY_NB 	(MIDI_KEY_Z+1)
#define MIDI_KEY_UNKNOWN	255
extern const char *MIDI_KEY_STRING[];
SINT32 MIDI_KeyDecode(MIDI_MSG *mm, BYTE *key, BYTE *on);
SINT32 MIDI_KeyDecodeLL(BYTE *data, SINT32 length, BYTE *key, BYTE *on);
SINT32 MIDI_KeyEncode(MIDI_MSG *mm, BYTE key, BYTE on);
SINT32 MIDI_KeyEncodeLL(BYTE *data, SINT32 *length, BYTE key, BYTE on);


#define MIDI_VERBOSE_HIDE_BINARY			0
#define MIDI_VERBOSE_SHOW_BINARY			0x1
#define MIDI_VERBOSE_HIDE_LCD_REFRESH		0
#define MIDI_VERBOSE_SHOW_LCD_REFRESH		0x2
#define MIDI_VERBOSE_HIDE_CPU_MONITOR		0
#define MIDI_VERBOSE_SHOW_CPU_MONITOR		0x4
#define MIDI_VERBOSE_HIDE_RTP_DATA			0
#define MIDI_VERBOSE_SHOW_FIRST_RTP_DATA	0x8
#define MIDI_VERBOSE_SHOW_ALL_RTP_DATA		0x10
#define MIDI_VERBOSE_HIDE_TRACKER_DATA		0
#define MIDI_VERBOSE_SHOW_TRACKER_DATA		0x20
SINT32 MIDI_MsgVerbose(MIDI_MSG *mm, char *str, SINT32 strSize, SINT32 mask);
SINT32 MIDI_MsgVerboseLL(BYTE *msg, SINT32 length, char *str, SINT32 strSize, SINT32 mask);

SINT32 MIDI_IsNumber(char *str);
SINT32 MIDI_IsInteger(char *str);
SINT32 MIDI_IsPositiveInteger(char *str);

#ifdef __cplusplus
}
#endif
#endif
