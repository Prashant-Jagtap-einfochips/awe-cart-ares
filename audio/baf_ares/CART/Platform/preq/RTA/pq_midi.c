//	Title:	pq_midi.c
//	Author: Guillaume

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>

#include "common.h"
#include "pq_midi.h"

const char *MIDI_RTP_DATA_TYPE_STRING[] = {"INTEGER", "SINGLE", "DOUBLE"};
const int MIDI_RTP_DATA_TYPE_SIZE[] = {MIDI_RTP_DATA_TYPE_INTEGER_SIZE, MIDI_RTP_DATA_TYPE_SINGLE_SIZE, MIDI_RTP_DATA_TYPE_DOUBLE_SIZE};

// MIDI_QUEUE
SINT32 MIDI_QueueInit(MIDI_QUEUE *mq, SINT32 size) {
	mq->start = 0;
	mq->end = 0;
	memset(mq, 0, sizeof(MIDI_QUEUE));
	if(size <= MIDI_QUEUE_SIZE_MAX) {
		mq->size = size;
		return MIDI_OK;
	} else return MIDI_QUEUE_ERROR;
}

SINT32 MIDI_QueueClear(MIDI_QUEUE *mq) {
	UINT64 *p64mq = (UINT64 *)mq;
	*p64mq = (UINT64)0;
	//mq->start = 0;
	//mq->end = 0;
	//memset(mq,0,sizeof(MIDI_QUEUE));
	return MIDI_OK;
}

SINT32 MIDI_QueueRead(MIDI_QUEUE *mq, MIDI_MSG *mm) {
	return MIDI_QueueReadLL(mq, mm->data, &mm->length);
}
SINT32 MIDI_QueueReadLL(MIDI_QUEUE *mq, BYTE *data, SINT32 *length) {
	sig_atomic_t temp;
	if(mq->start != mq->end) {
		memcpy(data, mq->msg[mq->start].data, mq->msg[mq->start].length);
		*length = mq->msg[mq->start].length;
		temp = (mq->start + 1) % mq->size;
		mq->start = temp;
		return MIDI_OK;
	}
	return MIDI_QUEUE_EMPTY;
}

SINT32 MIDI_QueueWrite(MIDI_QUEUE *mq, MIDI_MSG *mm) {
	return MIDI_QueueWriteLL(mq, mm->data, mm->length);
}
SINT32 MIDI_QueueWriteLL(MIDI_QUEUE *mq, BYTE *data, SINT32 length) {
	if((length > 0) && (length <= MIDI_MSG_SIZE_MAX)) {
		sig_atomic_t temp = (mq->end + 1) % mq->size;
		if(temp != mq->start) {
			mq->msg[mq->end].length = length;
			memcpy(mq->msg[mq->end].data, data, length);
			mq->end = temp;
			return MIDI_OK;
		}
	} else return MIDI_MSG_ERROR;
	return MIDI_QUEUE_FULL;
}

SINT32 MIDI_QueueSize(MIDI_QUEUE *mq) {
	return mq->size;
}
SINT32 MIDI_QueueNbMsg(MIDI_QUEUE *mq) {
	sig_atomic_t start = mq->start;
	sig_atomic_t end = mq->end;
	int temp = ((end + mq->size) - start) % mq->size;
	return temp;
}


// MIDI_MONITOR_QUEUE
SINT32 MIDI_MonQueueInit(MIDI_MON_QUEUE *mq, SINT32 size) {
	mq->start = 0;
	mq->end = 0;
	memset(mq, 0, sizeof(MIDI_MON_QUEUE));
	if(size <= MIDI_MON_QUEUE_SIZE_MAX) {
		mq->size = size;
		return MIDI_OK;
	} else return MIDI_QUEUE_ERROR;
}

SINT32 MIDI_MonQueueClear(MIDI_MON_QUEUE *mq) {
	UINT64 *p64mq = (UINT64 *)mq;
	*p64mq = (UINT64)0;
	//mq->start = 0;
	//mq->end = 0;
	//memset(mq,0,sizeof(MIDI_MON_QUEUE));
	return MIDI_OK;
}

SINT32 MIDI_MonQueueRead(MIDI_MON_QUEUE *mq, MIDI_MON_DATA *data) {
	sig_atomic_t temp;
	if(mq->start != mq->end) {
		memcpy(data, &mq->data[mq->start], sizeof(MIDI_MON_DATA));
		temp = (mq->start + 1) % mq->size;
		mq->start = temp;
		return MIDI_OK;
	}
	return MIDI_QUEUE_EMPTY;
}

SINT32 MIDI_MonQueueWrite(MIDI_MON_QUEUE *mq, MIDI_MON_DATA *data) {
	sig_atomic_t temp = (mq->end + 1) % mq->size;
	if(temp != mq->start) {
		memcpy(&mq->data[mq->end], data, sizeof(MIDI_MON_DATA));
		mq->end = temp;
		return MIDI_OK;
	}
	return MIDI_QUEUE_FULL;
}

SINT32 MIDI_MonQueueSize(MIDI_MON_QUEUE *mq) {
	return mq->size;
}
SINT32 MIDI_MonQueueNbMsg(MIDI_MON_QUEUE *mq) {
	sig_atomic_t start = mq->start;
	sig_atomic_t end = mq->end;
	int temp = ((end + mq->size) - start) % mq->size;
	return temp;
}


// MIDI Command
SINT32 MIDI_CmdDecode(MIDI_MSG *mm, BYTE *cmd, BYTE *scmd, BYTE **buf, BYTE *size) {
	return MIDI_CmdDecodeLL(mm->data, mm->length, cmd, scmd, buf, size);
}
SINT32 MIDI_CmdDecodeLL(BYTE *data, SINT32 length, BYTE *cmd, BYTE *scmd, BYTE **buf, BYTE *size) {
	SINT32 bufmin, bufmax;
	if((length >= MIDI_MSG_SIZE_MIN) && (length <= MIDI_MSG_SIZE_MAX)) {
		if((data[0] == 0xF0) && (data[1] == 0x7E)
				&& (data[2] == 0x7F)) {     // Universal Non Real Time Sys Ex Header to All Devices
			if((data[3] == 0x05) && (data[4] == 0x03)
					&& (data[7] ==
						0x00)) { // SubID#1=Sample Dump Extensions, SubID#2=Sample Name Transmission, Sample Name Tag Length
				*cmd = data[5];
				*scmd = data[6];
				switch(*cmd) {

				case MIDI_CMD:
					switch(*scmd) {
					case MIDI_SCMD_CONSOLE      :
						bufmin = 1;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					case MIDI_SCMD_LCD_REFRESH  :
						bufmin = 0;
						bufmax = 0;
						break;
					case MIDI_SCMD_LCD_FRAME_END:
						bufmin = 0;
						bufmax = 0;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_MONITOR:
					switch(*scmd) {
					case MIDI_SCMD_MONITOR_CPU :
					case MIDI_SCMD_MONITOR_CPU2 :
						bufmin = 1;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_LCD_PAGE1:
				case MIDI_CMD_LCD_PAGE2:
				case MIDI_CMD_LCD_PAGE3:
				case MIDI_CMD_LCD_PAGE4:
					if((*scmd >= 1) && (*scmd <= MIDI_LCD_NB_LINE)) {
						bufmin = 1;
						bufmax = MIDI_LCD_NB_COL;
					} else return MIDI_MSG_ERROR;
					break;

				case MIDI_CMD_LCD:
					if((((*scmd & 0x3C) >> 2) < MIDI_LCD_NB_PAGE) && ((*scmd & 0x03) < MIDI_LCD_NB_LINE)) {
						bufmin = 1;
						bufmax = MIDI_LCD_NB_COL;
					} else return MIDI_MSG_ERROR;
					break;

				case MIDI_CMD_MODEL:
					switch(*scmd) {
					case MIDI_SCMD_MODEL_START   :
						bufmin = 1;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					case MIDI_SCMD_MODEL_RESTART :
						bufmin = 0;
						bufmax = 0;
						break;
					case MIDI_SCMD_MODEL_STOP    :
						bufmin = 0;
						bufmax = 0;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_PREAMP:
					switch(*scmd) {
					case MIDI_SCMD_PREAMP_SYSTEM	:
						bufmin = 1;
						bufmax = 1;
						break;
					case MIDI_SCMD_PREAMP_VOL		:
					case MIDI_SCMD_PREAMP_VOL_P		:
					case MIDI_SCMD_PREAMP_VOL_M		:
					case MIDI_SCMD_PREAMP_BAL		:
					case MIDI_SCMD_PREAMP_BAL_P		:
					case MIDI_SCMD_PREAMP_BAL_M		:
					case MIDI_SCMD_PREAMP_FAD		:
					case MIDI_SCMD_PREAMP_FAD_P		:
					case MIDI_SCMD_PREAMP_FAD_M		:
					case MIDI_SCMD_PREAMP_BASS		:
					case MIDI_SCMD_PREAMP_BASS_P	:
					case MIDI_SCMD_PREAMP_BASS_M	:
					case MIDI_SCMD_PREAMP_MID		:
					case MIDI_SCMD_PREAMP_MID_P		:
					case MIDI_SCMD_PREAMP_MID_M		:
					case MIDI_SCMD_PREAMP_TREB		:
					case MIDI_SCMD_PREAMP_TREB_P	:
					case MIDI_SCMD_PREAMP_TREB_M	:
					case MIDI_SCMD_PREAMP_STATUS	:
					case MIDI_SCMD_PREAMP_MUTE		:
					case MIDI_SCMD_PREAMP_PARAM1	:
					case MIDI_SCMD_PREAMP_PARAM1_P	:
					case MIDI_SCMD_PREAMP_PARAM1_M	:
					case MIDI_SCMD_PREAMP_PARAM2	:
					case MIDI_SCMD_PREAMP_PARAM2_P	:
					case MIDI_SCMD_PREAMP_PARAM2_M	:
					case MIDI_SCMD_PREAMP_RESET     :
						bufmin = 0;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_PLAYER:
					switch(*scmd) {
					case MIDI_SCMD_PLAYER_STATUS	:
					case MIDI_SCMD_PLAYER_PLAY		:
					case MIDI_SCMD_PLAYER_PAUSE		:
					case MIDI_SCMD_PLAYER_REW		:
					case MIDI_SCMD_PLAYER_FF		:
						bufmin = 0;
						bufmax = 0;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_GEN:
					switch(*scmd) {
					case MIDI_SCMD_GEN_RESET			:
					case MIDI_SCMD_GEN_GET_STATUS		:
					case MIDI_SCMD_GEN_STATUS			:
					case MIDI_SCMD_GEN_SET_TYPE			:
					case MIDI_SCMD_GEN_GET_TYPE			:
					case MIDI_SCMD_GEN_TYPE				:
					case MIDI_SCMD_GEN_SET_FREQ			:
					case MIDI_SCMD_GEN_GET_FREQ			:
					case MIDI_SCMD_GEN_FREQ				:
					case MIDI_SCMD_GEN_SET_LEVEL		:
					case MIDI_SCMD_GEN_GET_LEVEL		:
					case MIDI_SCMD_GEN_LEVEL			:
					case MIDI_SCMD_GEN_SET_PHASE		:
					case MIDI_SCMD_GEN_GET_PHASE		:
					case MIDI_SCMD_GEN_PHASE			:
					case MIDI_SCMD_GEN_SET_FILT     	:
					case MIDI_SCMD_GEN_GET_FILT			:
					case MIDI_SCMD_GEN_FILT				:
					case MIDI_SCMD_GEN_SET_WIN  		:
					case MIDI_SCMD_GEN_GET_WIN			:
					case MIDI_SCMD_GEN_WIN				:
						bufmin = 0;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_WP:
					switch(*scmd) {
					case MIDI_SCMD_WP_RESET            :
					case MIDI_SCMD_WP_GET_STATUS       :
					case MIDI_SCMD_WP_STATUS           :
					case MIDI_SCMD_WP_SET_MODE         :
					case MIDI_SCMD_WP_GET_MODE         :
					case MIDI_SCMD_WP_MODE             :
					case MIDI_SCMD_WP_SET_SRC          :
					case MIDI_SCMD_WP_GET_SRC          :
					case MIDI_SCMD_WP_SRC              :
					case MIDI_SCMD_WP_SET_SINEF        :
					case MIDI_SCMD_WP_GET_SINEF        :
					case MIDI_SCMD_WP_SINEF            :
					case MIDI_SCMD_WP_SET_MAG_LR       :
					case MIDI_SCMD_WP_GET_MAG_LR       :
					case MIDI_SCMD_WP_MAG_LR           :
					case MIDI_SCMD_WP_SET_PHASE_LR     :
					case MIDI_SCMD_WP_GET_PHASE_LR     :
					case MIDI_SCMD_WP_PHASE_LR         :
					case MIDI_SCMD_WP_SET_FILT_TYPE    :
					case MIDI_SCMD_WP_GET_FILT_TYPE    :
					case MIDI_SCMD_WP_FILT_TYPE        :
					case MIDI_SCMD_WP_SET_FILT_ORDER   :
					case MIDI_SCMD_WP_GET_FILT_ORDER   :
					case MIDI_SCMD_WP_FILT_ORDER       :
					case MIDI_SCMD_WP_SET_FILT_FREQ    :
					case MIDI_SCMD_WP_GET_FILT_FREQ    :
					case MIDI_SCMD_WP_FILT_FREQ        :
					case MIDI_SCMD_WP_SET_FILT_SPAN    :
					case MIDI_SCMD_WP_GET_FILT_SPAN    :
					case MIDI_SCMD_WP_FILT_SPAN        :
						bufmin = 0;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_ECU:
					switch(*scmd) {
					case MIDI_SCMD_ECU_SPEED     :
						bufmin = 1;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_JADE:
					switch(*scmd) {
					case MIDI_SCMD_JADE_MSG    :
						bufmin = 0;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_HEAD_TRACKER:
					switch(*scmd) {
					case MIDI_SCMD_HEAD_TRACKER   :
						bufmin = 1;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				case MIDI_CMD_RTP:
					switch(*scmd) {
					case MIDI_SCMD_RTP_REFRESH:
					case MIDI_SCMD_RTP_SCAN:
						bufmin = 0;
						bufmax = 0;
						break;
					case MIDI_SCMD_RTP_INFO:
					case MIDI_SCMD_RTP_VALUE:
					case MIDI_SCMD_RTP_WRITE:
					case MIDI_SCMD_RTP_READ:
					case MIDI_SCMD_RTP_VALUE_SYS:
					case MIDI_SCMD_RTP_WRITE_SYS:
					case MIDI_SCMD_RTP_READ_SYS:
					case MIDI_SCMD_RTP_BDSP_BR:
					case MIDI_SCMD_RTP_BDSP_BW:
					case MIDI_SCMD_RTP_ADD_SYS:
					case MIDI_SCMD_RTP_MUL_SYS:
					case MIDI_SCMD_RTP_OR_SYS:
					case MIDI_SCMD_RTP_AND_SYS:
					case MIDI_SCMD_RTP_XOR_SYS:
						bufmin = 0;
						bufmax = MIDI_MSG_PAYLOAD_SIZE_MAX;
						break;
					default:
						return MIDI_MSG_ERROR;
					}
					break;

				default:
					return MIDI_MSG_ERROR;
				}

				*size = data[8];
				*buf = &data[9];
				if((*size >= bufmin) && (*size <= bufmax)) {         // Check Length
					if(length == *size + MIDI_MSG_SIZE_MIN) {
						if(*(*buf + *size) == 0xF7) {				 // Check EOX
							return MIDI_OK;
						}
					}
				}
			}
		}
	}
	return MIDI_MSG_ERROR;
}


SINT32 MIDI_CmdEncode(MIDI_MSG *mm, BYTE cmd, BYTE scmd, BYTE *buf, BYTE size) {
	return MIDI_CmdEncodeLL(mm->data, &mm->length, cmd, scmd, buf, size);
}
SINT32 MIDI_CmdEncodeLL(BYTE *data, SINT32 *length, BYTE cmd, BYTE scmd, BYTE *buf, BYTE size) {
	*length = 0;
	//if(MIDI_MSG_PAYLOAD_SIZE_MAX < 256) {
	//	if(size > MIDI_MSG_PAYLOAD_SIZE_MAX) return MIDI_MSG_ERROR;
	//}
	data[0] = 0xF0; // Universal Non Real Time Sys Ex Header to All Devices
	data[1] = 0x7E;
	data[2] = 0x7F;
	data[3] = 0x05; // SubID#1=Sample Dump Extensions, SubID#2=Sample Name Transmission, Sample Name Tag Length
	data[4] = 0x03;
	data[5] = cmd;
	data[6] = scmd;
	data[7] = 0x00;
	data[8] = size;
	if(size) memcpy(data + 9, buf, size);
	data[9 + size] = 0xF7;
	*length = size + MIDI_MSG_HEADER_SIZE;
	return MIDI_OK;
}


// MIDI Control
SINT32 MIDI_CtrlDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value) {
	return MIDI_CtrlDecodeLL(mm->data, mm->length, channel, nb, value);
}
SINT32 MIDI_CtrlDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value) {
	BYTE type;
	if(length != 3) return MIDI_MSG_ERROR;
	type = data[0] >> 4;
	*channel = data[0] & 0x0F;
	if(type != 0xB) return MIDI_MSG_ERROR;
	//if(*channel >= 15) return MIDI_MSG_ERROR;
	*nb = data[1];
	*value = data[2];
	return MIDI_OK;
}
SINT32 MIDI_CtrlEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value) {
	return MIDI_CtrlEncodeLL(mm->data, &mm->length, channel, nb, value);
}
SINT32 MIDI_CtrlEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value) {
	*length = 0;
	if(channel > 15) return MIDI_MSG_ERROR;
	data[0] = 0xB0 | channel;
	data[1] = nb;
	data[2] = value;
	*length = 3;
	return MIDI_OK;
}

// MIDI Note ON Keyboard
SINT32 MIDI_NoteOnDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value) {
	return MIDI_NoteOnDecodeLL(mm->data, mm->length, channel, nb, value);
}
SINT32 MIDI_NoteOnDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value) {
	BYTE type;
	if(length != 3) return MIDI_MSG_ERROR;
	type = data[0] >> 4;
	*channel = data[0] & 0x0F;
	if(type != 0x9) return MIDI_MSG_ERROR;
	//if(*channel >= 15) return MIDI_MSG_ERROR;
	*nb = data[1];
	*value = data[2];
	return MIDI_OK;
}
SINT32 MIDI_NoteOnEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value) {
	return MIDI_NoteOnEncodeLL(mm->data, &mm->length, channel, nb, value);
}
SINT32 MIDI_NoteOnEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value) {
	*length = 0;
	if(channel > 15) return MIDI_MSG_ERROR;
	data[0] = 0x90 | channel;
	data[1] = nb;
	data[2] = value;
	*length = 3;
	return MIDI_OK;
}

// MIDI Note OFF Keyboard
SINT32 MIDI_NoteOffDecode(MIDI_MSG *mm, BYTE *channel, BYTE *nb, BYTE *value) {
	return MIDI_NoteOffDecodeLL(mm->data, mm->length, channel, nb, value);
}
SINT32 MIDI_NoteOffDecodeLL(BYTE *data, SINT32 length, BYTE *channel, BYTE *nb, BYTE *value) {
	BYTE type;
	if(length != 3) return MIDI_MSG_ERROR;
	type = data[0] >> 4;
	*channel = data[0] & 0x0F;
	if(type != 0x8) return MIDI_MSG_ERROR;
	//if(*channel >= 15) return MIDI_MSG_ERROR;
	*nb = data[1];
	*value = data[2];
	return MIDI_OK;
}
SINT32 MIDI_NoteOffEncode(MIDI_MSG *mm, BYTE channel, BYTE nb, BYTE value) {
	return MIDI_NoteOffEncodeLL(mm->data, &mm->length, channel, nb, value);
}
SINT32 MIDI_NoteOffEncodeLL(BYTE *data, SINT32 *length, BYTE channel, BYTE nb, BYTE value) {
	*length = 0;
	if(channel > 15) return MIDI_MSG_ERROR;
	data[0] = 0x80 | channel;
	data[1] = nb;
	data[2] = value;
	*length = 3;
	return MIDI_OK;
}


// MIDI Remote Control Key
const char *MIDI_KEY_STRING[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "A", "B", "C", "D", "E", "F",
								 "POWER", "VOL_P", "VOL_M", "CH_P", "CH_M", "MUTE", "OK", "UP", "DOWN", "LEFT", "RIGHT",
								 "PLAY", "STOP", "PAUSE", "REC", "FF", "REW", "SELECT", "CANCEL", "DVD", "TV",
								 "G", "H", "I", "J", "K", "L", "M", "N", "O", "P",
								 "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
								};
SINT32 MIDI_KeyDecode(MIDI_MSG *mm, BYTE *key, BYTE *on) {
	return MIDI_KeyDecodeLL(mm->data, mm->length, key, on);
}
SINT32 MIDI_KeyDecodeLL(BYTE *data, SINT32 length, BYTE *key, BYTE *on) {
	BYTE type, channel;
	if(length != 3) return MIDI_MSG_ERROR;
	type = data[0] >> 4;
	channel = data[0] & 0x0F;
	if((type != 0x08) && (type != 0x09)) return MIDI_MSG_ERROR;
	if(channel != 15) return MIDI_MSG_ERROR;
	*key = data[1];
	*on = (type == 0x09) ? TRUE : FALSE;
	return MIDI_OK;
}
SINT32 MIDI_KeyEncode(MIDI_MSG *mm, BYTE key, BYTE on) {
	return MIDI_KeyEncodeLL(mm->data, &mm->length, key, on);
}
SINT32 MIDI_KeyEncodeLL(BYTE *data, SINT32 *length, BYTE key, BYTE on) {
	*length = 0;
	if(key >= MIDI_KEY_NB) return MIDI_MSG_ERROR;
	if(on) data[0] = 0x9f;
	else data[0] = 0x8f;
	data[1] = key;
	data[2] = 0;
	*length = 3;
	return MIDI_OK;
}


// MIDI Verbose
SINT32 MIDI_MsgVerbose(MIDI_MSG *mm, char *str, SINT32 strSize, SINT32 mask) {
	return MIDI_MsgVerboseLL(mm->data, mm->length, str, strSize, mask);
}
SINT32 MIDI_MsgVerboseLL(BYTE *msg, SINT32 length, char *str, SINT32 strSize, SINT32 mask) {
	int valid = FALSE;
	char szStr[MIDI_MSG_SIZE_MAX];
	BYTE cmd, scmd, *dec, decsize;
	int i, j;
	int type = msg[0] & 0xf0;
	int channel = msg[0] & 0xf;
	char *rtpSzSystem, *rtpSzSysmask, *rtpSzParam, *rtpSzLength, *rtpSzType, *rtpSzOffset,
		 *rtpSzInvalid;
	int rtpLength, rtpType, rtpTypeSize;
	BYTE *rtpBufBin;
	BYTE msgl[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];

	if(length == 0) return MIDI_ERROR;
	if(strSize == 0) return MIDI_ERROR;
	str[0] = 0;
	switch (type) {
	case 0x90:
		if(length != 3) return MIDI_ERROR;
		if(channel == 15) {
			if(msg[1] < MIDI_KEY_NB) {
				snprintf(str, strSize, "[MIDI] REMOTE KEY=ON (%s) %d", MIDI_KEY_STRING[msg[1]], msg[2]);
			} else {
				snprintf(str, strSize, "[MIDI] REMOTE KEY=ON %d %d", msg[1], msg[2]);
			}
		} else {
			snprintf(str, strSize, "[MIDI] CH=%d KEY_ON %d %d", channel, msg[1], msg[2]);
		}
		valid = TRUE;
		break;
	case 0x80:
		if(length != 3) return MIDI_ERROR;
		if(channel == 15) {
			if(msg[1] < MIDI_KEY_NB) {
				snprintf(str, strSize, "[MIDI] REMOTE KEY=OFF (%s) %d", MIDI_KEY_STRING[msg[1]], msg[2]);
			} else {
				snprintf(str, strSize, "[MIDI] REMOTE KEY=OFF %d %d", msg[1], msg[2]);
			}
		} else {
			snprintf(str, strSize, "[MIDI] CH=%d KEY_OFF %d %d", channel, msg[1], msg[2]);
		}
		valid = TRUE;
		break;
	case 0xb0:
		if(length != 3) return MIDI_ERROR;
		snprintf(str, strSize, "[MIDI] CH=%d Control %d %d", channel, msg[1], msg[2]);
		valid = TRUE;
		break;
	case 0xF0:
		if(MIDI_CmdDecodeLL(msg, length, &cmd, &scmd, &dec, &decsize) == MIDI_OK) {
			valid = TRUE;
			memcpy(szStr, dec, decsize);
			szStr[decsize] = 0;
			for(i = 0; i < decsize; i++) {
				if(szStr[i] == 0) break;
				if(szStr[i] < ' ') szStr[i] = ' ';
			}
			switch(cmd) {
			case MIDI_CMD:
				switch(scmd) {
				case MIDI_SCMD_CONSOLE:
					snprintf(str, strSize, "[CONSOLE] %s", szStr);
					break;
				case MIDI_SCMD_LCD_REFRESH:
					if(mask & MIDI_VERBOSE_SHOW_LCD_REFRESH)
						snprintf(str, strSize, "[LCD_REFRESH]");
					else
						mask &= !MIDI_VERBOSE_SHOW_BINARY;
					break;
				case MIDI_SCMD_LCD_FRAME_END:
					snprintf(str, strSize, "[LCD_FRAME_END]");
					break;
				}
				break;

			case MIDI_CMD_LCD_PAGE1:
			case MIDI_CMD_LCD_PAGE2:
			case MIDI_CMD_LCD_PAGE3:
			case MIDI_CMD_LCD_PAGE4:
				snprintf(str, strSize, "[LCD_%d_%d] %s", cmd, scmd, szStr);
				break;

			case MIDI_CMD_LCD:
				snprintf(str, strSize, "[LCD_%d_%d(%d)] %s", ((scmd & 0x3C) >> 2) + 1, (scmd & 0x03) + 1,
						 (scmd & 0xC0) >> 6, szStr);
				break;

			case MIDI_CMD_MONITOR:
				switch(scmd) {
				case MIDI_SCMD_MONITOR_CPU:
					if(mask & MIDI_VERBOSE_SHOW_CPU_MONITOR) {
						snprintf(str, strSize, "[MONITOR] %s", szStr);
						rtpBufBin = (BYTE *)(&msg[9] + strnlen((char *)&msg[9], length) + 1);
						memcpy(msgl, msg, length);
						rtpSzParam = strtok((char *)&msgl[9], " ");
						rtpSzOffset = strtok(NULL, " ");
						rtpSzLength = strtok(NULL, " ");
						rtpSzInvalid = strtok(NULL, " ");
						if((rtpSzInvalid == NULL) && (rtpSzParam != NULL) && (rtpSzLength != NULL)
								&& (rtpSzOffset != NULL)) {
							rtpLength = atoi(rtpSzLength);
							if(rtpLength > 0) {
								//printf("%d  %x %x %x %x\n",(int)strnlen((char*)&msg[9],length),rtpBufBin[0],rtpBufBin[1],rtpBufBin[2],rtpBufBin[3]);
								int length = (mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA) ? rtpLength : 1;
								for(i = 0; i < length; i++) {
									snprintf(szStr, MIDI_MSG_SIZE_MAX, " %08X", *((SINT32 *)((BYTE *)(rtpBufBin + i * 4))));
									strncat(str, szStr, strSize);
								}
							}
						}
					} else mask &= !MIDI_VERBOSE_SHOW_BINARY;
					break;
				case MIDI_SCMD_MONITOR_CPU2:
					if(mask & MIDI_VERBOSE_SHOW_CPU_MONITOR) {
						rtpBufBin = (BYTE *)(&msg[9] + strnlen((char *)&msg[9], length) + 1);
						BYTE nb = *rtpBufBin;
						snprintf(str, strSize, "[MONITOR_2] %s %d", szStr, nb);
						rtpBufBin += 3;
						nb = (mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA) ? nb : 1;
						for(i = 0; i < nb; i++) {
						 	snprintf(szStr, MIDI_MSG_SIZE_MAX, " %08X-%08X",
						 		*((SINT32 *)((BYTE *)(rtpBufBin + i * 8))),
						 		*((SINT32 *)((BYTE *)(rtpBufBin + i * 8 + 4))));
							strncat(str, szStr, strSize);
						}
					} else mask &= !MIDI_VERBOSE_SHOW_BINARY;
					break;
				}
				break;

			case MIDI_CMD_MODEL:
				switch(scmd) {
				case MIDI_SCMD_MODEL_START:
					snprintf(str, strSize, "[MODEL_START] %s", szStr);
					break;
				case MIDI_SCMD_MODEL_RESTART:
					snprintf(str, strSize, "[MODEL_RESTART]");
					break;
				case MIDI_SCMD_MODEL_STOP:
					snprintf(str, strSize, "[MODEL_STOP]");
					break;
				}
				break;

			case MIDI_CMD_PREAMP:
				switch(scmd) {
				case MIDI_SCMD_PREAMP_SYSTEM:
					snprintf(str, strSize, "[PREAMP_SYSTEM] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_VOL:
					snprintf(str, strSize, "[PREAMP_VOLUME] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_VOL_P:
					snprintf(str, strSize, "[PREAMP_VOLUME+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_VOL_M:
					snprintf(str, strSize, "[PREAMP_VOLUME-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BAL:
					snprintf(str, strSize, "[PREAMP_BALANCE] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BAL_P:
					snprintf(str, strSize, "[PREAMP_BALANCE+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BAL_M:
					snprintf(str, strSize, "[PREAMP_BALANCE-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_FAD:
					snprintf(str, strSize, "[PREAMP_FADER] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_FAD_P:
					snprintf(str, strSize, "[PREAMP_FADER+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_FAD_M:
					snprintf(str, strSize, "[PREAMP_FADER-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BASS:
					snprintf(str, strSize, "[PREAMP_BASS] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BASS_P:
					snprintf(str, strSize, "[PREAMP_BASS+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_BASS_M:
					snprintf(str, strSize, "[PREAMP_BASS-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_MID:
					snprintf(str, strSize, "[PREAMP_MID] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_MID_P:
					snprintf(str, strSize, "[PREAMP_MID+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_MID_M:
					snprintf(str, strSize, "[PREAMP_MID-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_TREB:
					snprintf(str, strSize, "[PREAMP_TREBLE] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_TREB_P:
					snprintf(str, strSize, "[PREAMP_TREBLE+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_TREB_M:
					snprintf(str, strSize, "[PREAMP_TREBLE-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_STATUS:
					snprintf(str, strSize, "[PREAMP_STATUS]");
					break;
				case MIDI_SCMD_PREAMP_MUTE:
					snprintf(str, strSize, "[PREAMP_MUTE\\UNMUTE]");
					break;
				case MIDI_SCMD_PREAMP_PARAM1:
					snprintf(str, strSize, "[PREAMP_PARAM1] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_PARAM1_P:
					snprintf(str, strSize, "[PREAMP_PARAM1+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_PARAM1_M:
					snprintf(str, strSize, "[PREAMP_PARAM1-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_PARAM2:
					snprintf(str, strSize, "[PREAMP_PARAM2] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_PARAM2_P:
					snprintf(str, strSize, "[PREAMP_PARAM2+] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_PARAM2_M:
					snprintf(str, strSize, "[PREAMP_PARAM2-] %s", szStr);
					break;
				case MIDI_SCMD_PREAMP_RESET:
					snprintf(str, strSize, "[PREAMP_RESET]");
					break;
				}
				break;

			case MIDI_CMD_PLAYER:
				switch(scmd) {
				case MIDI_SCMD_PLAYER_STATUS:
					snprintf(str, strSize, "[PLAYER_STATUS]");
					break;
				case MIDI_SCMD_PLAYER_PLAY:
					snprintf(str, strSize, "[PLAYER_PLAY]");
					break;
				case MIDI_SCMD_PLAYER_PAUSE:
					snprintf(str, strSize, "[PLAYER_PAUSE]");
					break;
				case MIDI_SCMD_PLAYER_REW:
					snprintf(str, strSize, "[PLAYER_REW]");
					break;
				case MIDI_SCMD_PLAYER_FF:
					snprintf(str, strSize, "[PLAYER_FF]");
					break;
				}
				break;

			case MIDI_CMD_GEN:
				switch(scmd) {
				case MIDI_SCMD_GEN_RESET:
					snprintf(str, strSize, "[GEN_RESET] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_STATUS:
					snprintf(str, strSize, "[GEN_GET_STATUS] %s", szStr);
					break;
				case MIDI_SCMD_GEN_STATUS:
					snprintf(str, strSize, "[GEN_STATUS] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_TYPE:
					snprintf(str, strSize, "[GEN_SET_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_TYPE:
					snprintf(str, strSize, "[GEN_GET_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_TYPE:
					snprintf(str, strSize, "[GEN_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_FREQ:
					snprintf(str, strSize, "[GEN_SET_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_FREQ:
					snprintf(str, strSize, "[GEN_GET_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_GEN_FREQ:
					snprintf(str, strSize, "[GEN_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_LEVEL:
					snprintf(str, strSize, "[GEN_SET_LEVEL] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_LEVEL:
					snprintf(str, strSize, "[GEN_GET_LEVEL] %s", szStr);
					break;
				case MIDI_SCMD_GEN_LEVEL:
					snprintf(str, strSize, "[GEN_LEVEL] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_PHASE:
					snprintf(str, strSize, "[GEN_SET_PHASE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_PHASE:
					snprintf(str, strSize, "[GEN_GET_PHASE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_PHASE:
					snprintf(str, strSize, "[GEN_PHASE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_FILT:
					snprintf(str, strSize, "[GEN_SET_FILT] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_FILT:
					snprintf(str, strSize, "[GEN_GET_FILT] %s", szStr);
					break;
				case MIDI_SCMD_GEN_FILT:
					snprintf(str, strSize, "[GEN_FILT] %s", szStr);
					break;
				case MIDI_SCMD_GEN_SET_WIN:
					snprintf(str, strSize, "[GEN_SET_WIN] %s", szStr);
					break;
				case MIDI_SCMD_GEN_GET_WIN:
					snprintf(str, strSize, "[GEN_GET_WIN_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_GEN_WIN:
					snprintf(str, strSize, "[GEN_WIN_TYPE] %s", szStr);
					break;
				}
				break;

			case MIDI_CMD_WP:
				switch(scmd) {
				case MIDI_SCMD_WP_RESET:
					snprintf(str, strSize, "[WP_RESET] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_STATUS:
					snprintf(str, strSize, "[WP_GET_STATUS] %s", szStr);
					break;
				case MIDI_SCMD_WP_STATUS:
					snprintf(str, strSize, "[WP_STATUS] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_MODE:
					snprintf(str, strSize, "[WP_SET_MODE] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_MODE:
					snprintf(str, strSize, "[WP_GET_MODE] %s", szStr);
					break;
				case MIDI_SCMD_WP_MODE:
					snprintf(str, strSize, "[WP_MODE] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_SRC:
					snprintf(str, strSize, "[WP_SET_SRC] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_SRC:
					snprintf(str, strSize, "[WP_GET_SRC] %s", szStr);
					break;
				case MIDI_SCMD_WP_SRC:
					snprintf(str, strSize, "[WP_SRC] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_SINEF:
					snprintf(str, strSize, "[WP_SET_SINEF] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_SINEF:
					snprintf(str, strSize, "[WP_GET_SINEF] %s", szStr);
					break;
				case MIDI_SCMD_WP_SINEF:
					snprintf(str, strSize, "[WP_SINEF] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_MAG_LR:
					snprintf(str, strSize, "[WP_SET_MAG_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_MAG_LR:
					snprintf(str, strSize, "[WP_GET_MAG_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_MAG_LR:
					snprintf(str, strSize, "[WP_MAG_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_PHASE_LR:
					snprintf(str, strSize, "[WP_SET_PHASE_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_PHASE_LR:
					snprintf(str, strSize, "[WP_GET_PHASE_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_PHASE_LR:
					snprintf(str, strSize, "[WP_PHASE_LR] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_FILT_TYPE:
					snprintf(str, strSize, "[WP_SET_FILT_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_FILT_TYPE:
					snprintf(str, strSize, "[WP_GET_FILT_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_WP_FILT_TYPE:
					snprintf(str, strSize, "[WP_FILT_TYPE] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_FILT_ORDER:
					snprintf(str, strSize, "[WP_SET_FILT_ORDER] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_FILT_ORDER:
					snprintf(str, strSize, "[WP_GET_FILT_ORDER] %s", szStr);
					break;
				case MIDI_SCMD_WP_FILT_ORDER:
					snprintf(str, strSize, "[WP_FILT_ORDER] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_FILT_FREQ:
					snprintf(str, strSize, "[WP_SET_FILT_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_FILT_FREQ:
					snprintf(str, strSize, "[WP_GET_FILT_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_WP_FILT_FREQ:
					snprintf(str, strSize, "[WP_FILT_FREQ] %s", szStr);
					break;
				case MIDI_SCMD_WP_SET_FILT_SPAN:
					snprintf(str, strSize, "[WP_SET_FILT_SPAN] %s", szStr);
					break;
				case MIDI_SCMD_WP_GET_FILT_SPAN:
					snprintf(str, strSize, "[WP_GET_FILT_SPAN] %s", szStr);
					break;
				case MIDI_SCMD_WP_FILT_SPAN:
					snprintf(str, strSize, "[WP_FILT_SPAN] %s", szStr);
					break;
				}
				break;

			case MIDI_CMD_ECU:
				switch(scmd) {
				case MIDI_SCMD_ECU_SPEED:
					snprintf(str, strSize, "[ECU_SPEED] %s", szStr);
					break;
				}
				break;

			case MIDI_CMD_JADE:
				switch(scmd) {
				case MIDI_SCMD_JADE_MSG:
					snprintf(str, strSize, "[JADE_MSG] %s", szStr);
					break;
				}
				break;

			case MIDI_CMD_HEAD_TRACKER:
				switch(scmd) {
				case MIDI_SCMD_HEAD_TRACKER:
					if(mask & MIDI_VERBOSE_SHOW_TRACKER_DATA) {
						snprintf(str, strSize, "[HEAD_TRACKER] %s", szStr);
					} else mask &= !MIDI_VERBOSE_SHOW_BINARY;
					break;
				}
				break;

			case MIDI_CMD_RTP:
				switch(scmd) {
				case MIDI_SCMD_RTP_REFRESH:
					snprintf(str, strSize, "[RTP_REFRESH]");
					break;
				case MIDI_SCMD_RTP_SCAN:
					snprintf(str, strSize, "[RTP_SCAN]");
					break;
				case MIDI_SCMD_RTP_INFO:
					snprintf(str, strSize, "[RTP_INFO] %s", szStr);
					break;
				case MIDI_SCMD_RTP_READ:
					snprintf(str, strSize, "[RTP_READ] %s", szStr);
					break;
				case MIDI_SCMD_RTP_READ_SYS:
					snprintf(str, strSize, "[RTP_READ_SYS] %s", szStr);
					break;

				case MIDI_SCMD_RTP_VALUE:
				case MIDI_SCMD_RTP_WRITE:
					if(scmd == MIDI_SCMD_RTP_VALUE) snprintf(str, strSize, "[RTP_VALUE] %s", szStr);
					if(scmd == MIDI_SCMD_RTP_WRITE)snprintf(str, strSize, "[RTP_WRITE] %s", szStr);
					if((mask & MIDI_VERBOSE_SHOW_FIRST_RTP_DATA) || (mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA)) {
						rtpBufBin = (BYTE *)(&msg[9] + strnlen((char *)&msg[9], length) + 1);
						memcpy(msgl, msg, length);
						rtpSzParam = strtok((char *)&msgl[9], " ");
						rtpSzOffset = strtok(NULL, " ");
						rtpSzLength = strtok(NULL, " ");
						rtpSzType = strtok(NULL, " ");
						rtpSzInvalid = strtok(NULL, " ");
						if((rtpSzInvalid == NULL) && (rtpSzParam != NULL) && (rtpSzLength != NULL) && (rtpSzType != NULL)
								&& (rtpSzOffset != NULL)) {
							rtpLength = atoi(rtpSzLength);
							if(rtpLength > 0) {
								rtpType = 0;
								rtpTypeSize = 0;
								for(i = 0; i < MIDI_RTP_DATA_TYPE_NB; i++) {
									if(!strncmp(rtpSzType, MIDI_RTP_DATA_TYPE_STRING[i], MIDI_MSG_PAYLOAD_SIZE_MAX)) {
										rtpType = i;
										rtpTypeSize = MIDI_RTP_DATA_TYPE_SIZE[i];
										break;
									}
								}
								//printf("%d  %x %x %x %x\n",(int)strnlen((char*)&msg[9],length),rtpBufBin[0],rtpBufBin[1],rtpBufBin[2],rtpBufBin[3]);
								if(rtpTypeSize != 0) {
									if(mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA) j = rtpLength;
									else j = 1;
									SINT32 ni = 0;
									FLOAT nf = 0;
									DOUBLE nd = 0;
									for(i = 0; i < j; i++) {
										switch(rtpType) {
										case MIDI_RTP_DATA_TYPE_INTEGER :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(SINT32); k++) {
												*((BYTE *)&ni + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											ni = *((SINT32 *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %d", ni);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_SINGLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(FLOAT); k++) {
												*((BYTE *)&nf + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nf = *((FLOAT *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nf);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_DOUBLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(DOUBLE); k++) {
												*((BYTE *)&nd + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nd = *((DOUBLE *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nd);
											strncat(str, szStr, strSize);
											break;
										}
									}
								}
							}
						}
					}
					break;

				case MIDI_SCMD_RTP_WRITE_SYS:
				case MIDI_SCMD_RTP_ADD_SYS:
				case MIDI_SCMD_RTP_MUL_SYS:
				case MIDI_SCMD_RTP_OR_SYS:
				case MIDI_SCMD_RTP_AND_SYS:
				case MIDI_SCMD_RTP_XOR_SYS:
					switch(scmd) {
					case MIDI_SCMD_RTP_WRITE_SYS:
						snprintf(str, strSize, "[RTP_WRITE_SYS] %s", szStr);
						break;
					case MIDI_SCMD_RTP_ADD_SYS:
						snprintf(str, strSize, "[RTP_ADD_SYS] %s", szStr);
						break;
					case MIDI_SCMD_RTP_MUL_SYS:
						snprintf(str, strSize, "[RTP_MUL_SYS] %s", szStr);
						break;
					case MIDI_SCMD_RTP_OR_SYS:
						snprintf(str, strSize, "[RTP_OR_SYS] %s", szStr);
						break;
					case MIDI_SCMD_RTP_AND_SYS:
						snprintf(str, strSize, "[RTP_AND_SYS] %s", szStr);
						break;
					case MIDI_SCMD_RTP_XOR_SYS:
						snprintf(str, strSize, "[RTP_XOR_SYS] %s", szStr);
						break;
					}
					if((mask & MIDI_VERBOSE_SHOW_FIRST_RTP_DATA) || (mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA)) {
						rtpBufBin = (BYTE *)(&msg[9] + strnlen((char *)&msg[9], length) + 1);
						memcpy(msgl, msg, length);
						rtpSzSysmask = strtok((char *)&msgl[9], " ");
						rtpSzParam = strtok(NULL, " ");
						rtpSzOffset = strtok(NULL, " ");
						rtpSzLength = strtok(NULL, " ");
						rtpSzType = strtok(NULL, " ");
						rtpSzInvalid = strtok(NULL, " ");
						if((rtpSzInvalid == NULL) && (rtpSzSysmask != NULL) && (rtpSzParam != NULL)
								&& (rtpSzLength != NULL) && (rtpSzType != NULL) && (rtpSzOffset != NULL)) {
							rtpLength = atoi(rtpSzLength);
							if(rtpLength > 0) {
								rtpType = 0;
								rtpTypeSize = 0;
								for(i = 0; i < MIDI_RTP_DATA_TYPE_NB; i++) {
									if(!strncmp(rtpSzType, MIDI_RTP_DATA_TYPE_STRING[i], MIDI_MSG_PAYLOAD_SIZE_MAX)) {
										rtpType = i;
										rtpTypeSize = MIDI_RTP_DATA_TYPE_SIZE[i];
										break;
									}
								}
								//printf("%d  %x %x %x %x\n",(int)strnlen((char*)&msg[9],length),rtpBufBin[0],rtpBufBin[1],rtpBufBin[2],rtpBufBin[3]);
								if(rtpTypeSize != 0) {
									if(mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA) j = rtpLength;
									else j = 1;
									SINT32 ni = 0;
									FLOAT nf = 0;
									DOUBLE nd = 0;
									for(i = 0; i < j; i++) {
										switch(rtpType) {
										case MIDI_RTP_DATA_TYPE_INTEGER :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(SINT32); k++) {
												*((BYTE *)&ni + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											ni = *((SINT32 *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %d", ni);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_SINGLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(FLOAT); k++) {
												*((BYTE *)&nf + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nf = *((FLOAT *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nf);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_DOUBLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(DOUBLE); k++) {
												*((BYTE *)&nd + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nd = *((DOUBLE *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nd);
											strncat(str, szStr, strSize);
											break;
										}
									}
								}
							}
						}
					}
					break;

				case MIDI_SCMD_RTP_VALUE_SYS:
					snprintf(str, strSize, "[RTP_VALUE_SYS] %s", szStr);
					if((mask & MIDI_VERBOSE_SHOW_FIRST_RTP_DATA) || (mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA)) {
						rtpBufBin = (BYTE *)(&msg[9] + strnlen((char *)&msg[9], length) + 1);
						memcpy(msgl, msg, length);
						rtpSzSystem = strtok((char *)&msgl[9], " ");
						rtpSzSysmask = strtok(NULL, " ");
						rtpSzParam = strtok(NULL, " ");
						rtpSzOffset = strtok(NULL, " ");
						rtpSzLength = strtok(NULL, " ");
						rtpSzType = strtok(NULL, " ");
						rtpSzInvalid = strtok(NULL, " ");
						//printf("%s %s %s %s %s %s (%s)\n",rtpSzSystem,rtpSzSysmask,rtpSzParam,rtpSzOffset,rtpSzLength,rtpSzType,rtpSzInvalid);
						if((rtpSzInvalid == NULL) && (rtpSzSystem != NULL) && (rtpSzSysmask != NULL)
								&& (rtpSzParam != NULL) && (rtpSzLength != NULL) && (rtpSzType != NULL) && (rtpSzOffset != NULL)) {
							rtpLength = atoi(rtpSzLength);
							if(rtpLength > 0) {
								rtpType = 0;
								rtpTypeSize = 0;
								for(i = 0; i < MIDI_RTP_DATA_TYPE_NB; i++) {
									if(!strncmp(rtpSzType, MIDI_RTP_DATA_TYPE_STRING[i], MIDI_MSG_PAYLOAD_SIZE_MAX)) {
										rtpType = i;
										rtpTypeSize = MIDI_RTP_DATA_TYPE_SIZE[i];
										break;
									}
								}
								//printf("%d  %x %x %x %x\n",(int)strnlen((char*)&msg[9],length),rtpBufBin[0],rtpBufBin[1],rtpBufBin[2],rtpBufBin[3]);
								if(rtpTypeSize != 0) {
									if(mask & MIDI_VERBOSE_SHOW_ALL_RTP_DATA) j = rtpLength;
									else j = 1;
									SINT32 ni = 0;
									FLOAT nf = 0;
									DOUBLE nd = 0;
									for(i = 0; i < j; i++) {
										switch(rtpType) {
										case MIDI_RTP_DATA_TYPE_INTEGER :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(SINT32); k++) {
												*((BYTE *)&ni + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											ni = *((SINT32 *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %d", ni);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_SINGLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(FLOAT); k++) {
												*((BYTE *)&nf + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nf = *((FLOAT *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nf);
											strncat(str, szStr, strSize);
											break;
										case MIDI_RTP_DATA_TYPE_DOUBLE :
#if defined(ARMV7)
											for(int k = 0; k < sizeof(DOUBLE); k++) {
												*((BYTE *)&nd + k) = *(BYTE *)(rtpBufBin + i * rtpTypeSize + k);
											}
#else
											nd = *((DOUBLE *)((BYTE *)(rtpBufBin + i * rtpTypeSize)));
#endif
											snprintf(szStr, MIDI_MSG_SIZE_MAX, " %g", nd);
											strncat(str, szStr, strSize);
											break;
										}
									}
								}
							}
						}
					}
					break;

				case MIDI_SCMD_RTP_BDSP_BR:
					snprintf(str, strSize, "[RTP_BDSP_BR] %s", szStr);
					break;
				case MIDI_SCMD_RTP_BDSP_BW:
					snprintf(str, strSize, "[RTP_BDSP_BW] %s", szStr);
					break;
				}
			}
		}
		break;
	default:
		snprintf(str, strSize, "???");
		break;
	}
	if(!valid || (mask & MIDI_VERBOSE_SHOW_BINARY)) {
		strncat(str, " (", strSize);
		for(i = 0; i < length; i++) {
			snprintf(szStr, MIDI_MSG_SIZE_MAX, " %02x", msg[i]);
			strncat(str, szStr, strSize);
		}
		strncat(str, ")", strSize);
	}
	return valid;
}

// Utility Functions
SINT32 MIDI_IsNumber(char *str) {
	int cpt = 0;
	if(str == NULL) return 0;
	while(str[cpt]) {
		if(!isdigit(str[cpt]) && (str[cpt] != '.') && (str[cpt] != '-') && (str[cpt] != 'e')
				&& (str[cpt] != 'E')) return FALSE;
		cpt++;
	}
	return TRUE;
}

SINT32 MIDI_IsInteger(char *szStr) {
	char *str = szStr;
	if(str == NULL) return 0;
	while(*str) {
		if(!isdigit(*str) && (*str != '-')) return FALSE;
		str++;
	}
	return TRUE;
}

SINT32 MIDI_IsPositiveInteger(char *szStr) {
	char *str = szStr;
	if(str == NULL) return 0;
	while(*str) {
		if(!isdigit(*str)) return FALSE;
		str++;
	}
	return TRUE;
}
