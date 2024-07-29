//	Title:	pq_param.c
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
#include <syslog.h>
#include <ctype.h>

#include "common.h"
#include "pq_midi.h"
#include "pq_param.h"

char g_szHostName[HOSTNAME_MAX_LENGTH];
char g_szModelName[MODELNAME_MAX_LENGTH];

const char *PARAM_IO_TYPE_STRING[] = {"WRITE", "READ", "PASSTHROUGH", "CONSTANT", "SYSTEM"};
const char *PARAM_PERSISTENT_STRING[] = {"NON_PERSISTENT", "PERSISTENT"};
//const char *PARAM_DATA_TYPE_STRING[]={"INTEGER", "SINGLE", "DOUBLE"};
//const int PARAM_DATA_TYPE_SIZE[] = {MIDI_RTP_DATA_TYPE_INTEGER_SIZE,MIDI_RTP_DATA_TYPE_SINGLE_SIZE,MIDI_RTP_DATA_TYPE_DOUBLE_SIZE};
static PARAM_ELT g_param[PARAM_MAX];
static PARAM_PREAMP g_preamp;
static sig_atomic_t g_lastIndex;
static sig_atomic_t g_newIndex;

#define PARAM_FILENAME_SIZE_MAX 256
static int32_t g_paramVersion;
static char g_paramFilename[PARAM_FILENAME_SIZE_MAX];
static MIDI_QUEUE *g_paramQueueOut;

int32_t PARAM_Init(char *name, char *hostname, char *modelname, MIDI_QUEUE *paramQueueOut) {
	memset(g_param, 0, sizeof(PARAM_ELT)*PARAM_MAX);
	memset(&g_preamp, 0, sizeof(PARAM_PREAMP));
	g_preamp.system = -1;
	g_preamp.volume = -1.0;
	g_preamp.balance = -1.0;
	g_preamp.fader = -1.0;
	g_preamp.bass = -1.0;
	g_preamp.mid = -1.0;
	g_preamp.treble = -1.0;
	g_preamp.param1 = -1.0;
	g_preamp.param2 = -1.0;
	g_newIndex = 0;
	g_lastIndex = -1;
	g_paramVersion = 1;
	strlcpy(g_szHostName, hostname, HOSTNAME_MAX_LENGTH);
	strlcpy(g_szModelName, modelname, MODELNAME_MAX_LENGTH);
	strlcpy(g_paramFilename, name, PARAM_FILENAME_SIZE_MAX);
	strncat(g_paramFilename, ".param", PARAM_FILENAME_SIZE_MAX);
	g_paramQueueOut = paramQueueOut;

	int32_t index = PARAM_GetNextAvailableIndex();
	PARAM_SetName(index, name);
	PARAM_SetType(index, PARAM_IO_TYPE_SYSTEM, MIDI_RTP_DATA_TYPE_INTEGER, PARAM_NON_PERSISTENT);
	PARAM_SetRange(index, 0, 0);
	double value = 0.0;
	PARAM_SetValue(index, 1, &value);
	return PARAM_OK;
}

int32_t PARAM_Load() {
	return PARAM_OK;
}

int32_t PARAM_Save() {
	return PARAM_OK;
}


int32_t PARAM_GetNextAvailableIndex() {
	if(g_lastIndex >= (PARAM_MAX - 1)) {
		Log(LOG_ERR, "ERROR: Too Many Realtime Parameters\n");
		exit(-1);
	} else {
		g_lastIndex++;
		g_newIndex = 1;
		return g_lastIndex;
	}
}

void PARAM_PrintList() {
	int i;
	printf("Send MIDI_SCMD_RTP_REFRESH\n");
	for(i = 0; i <= g_lastIndex; i++) {
		printf("PARAM_%d= %s(%d) %s %s [%g,%g] %s\n",
			   i, g_param[i].name,
			   g_param[i].length,
			   PARAM_IO_TYPE_STRING[g_param[i].iotype],
			   MIDI_RTP_DATA_TYPE_STRING[g_param[i].datatype],
			   g_param[i].min, g_param[i].max,
			   PARAM_PERSISTENT_STRING[g_param[i].persistent]);
		//printf("    {"); for(int j=0; j<g_param[i].length; j++) printf("%,.16g ",g_param[i].data[0][j]); printf("}\n");
	}
}

int32_t PARAM_SendRefresh() {
	MIDI_MSG msg;
	if(g_newIndex) {
		MIDI_CmdEncode(&msg, MIDI_CMD_RTP, MIDI_SCMD_RTP_REFRESH, NULL, 0);
		if(MIDI_QueueWrite(g_paramQueueOut, &msg) != MIDI_OK) {
			Log(LOG_ERR, "***ERROR*** MIDI_PARAM_OUT Queue Full\n");
		}
		g_newIndex = 0;
	}
	return PARAM_OK;
}

int32_t PARAM_GetIndexByName(char *name) {
	int i;

	for(i = 0; i <= g_lastIndex; i++) {
		if((strncmp(g_param[i].name, name, PARAM_NAME_SIZE_MAX) == 0)) {
			return i;
		}
	}
	return -1;
}

int32_t PARAM_checkName(char *szParam, char **szName) {
	char *szInvalid;
	char *szTemp0, *szTemp1, *szTemp2;
	char *szParamModel, *szParamHost;

	szTemp0 = strtok(szParam, ".");
	szTemp1 = strtok(NULL, ".");
	szTemp2 = strtok(NULL, ".");
	szInvalid = strtok(NULL, ".");
	if(szInvalid != NULL) {
		fprintf(stderr, "ERROR: RTP Param Name has too many fields\n");
		return -1;
	}
	if(szTemp0 == NULL) {
		fprintf(stderr, "ERROR: RTP Param Name has at least one fields\n");
		return -1;
	} else if(szTemp1 == 0) {
		*szName = szTemp0;
		szParamModel = NULL;
		szParamHost = NULL;
	} else if(szTemp2 == 0) {
		*szName = szTemp1;
		szParamModel = szTemp0;
		szParamHost = NULL;
	} else {
		*szName = szTemp2;
		szParamModel = szTemp1;
		szParamHost = szTemp0;
	}
	if(szParamHost != NULL) {
		if(strcmp(szParamHost, "*") == 0) {
			szParamHost = NULL;
		} else {
			if(strcmp(szParamHost, g_szHostName)) {
				//printf("Host does not match %s.%s.%s.%s\n",szParamHost,szParamModel,szParamCategory,szParamName);
				*szName = NULL;
				return -1;
			}
		}
	}
	if(szParamModel != NULL) {
		if(strcmp(szParamModel, "*") == 0) {
			szParamModel = NULL;
		} else {
			if(strcmp(szParamModel, g_szModelName)) {
				//printf("Model does not match %s.%s.%s.%s\n",szParamHost,szParamModel,szParamCategory,szParamName);
				*szName = NULL;
				return -1;
			}
		}
	}
	return 0;
}

int32_t PARAM_SendValueSys(int index, long long sysmask, int offset, int length) {
	int i, cpt;
	MIDI_MSG msg;
	int dataOutTypeSize, chunkSize, chunkSizeMax, szStrOutLength;
	long long valSysmask;
	char szStrOut[MIDI_MSG_PAYLOAD_SIZE_MAX];
	int system, cmpSystem, isEqual, cursys;

	// Check command and convert system into sysmask
	valSysmask = sysmask;
	cursys = PARAM_GetPreampSystem();
	if (sysmask == 0) {
		valSysmask = 1 << cursys;
		sysmask = valSysmask;
	} else if (sysmask == -1) {
		valSysmask = MIDI_RTP_SYSMASK_MAX;
	}

	// Check if all systems in the mask have the same value; if not,
	// then send a message for each individual system
	system = -1;
	isEqual = 1;
	for(cmpSystem = 0; cmpSystem < PARAM_NB_SYSTEM_MAX; cmpSystem++) {
		if (valSysmask & (1 << cmpSystem)) {
			if (system < 0) {
				system = cmpSystem;
			} else {
				for(i = 0; i < length; i++) {
					if (g_param[index].data[system][offset + i] != g_param[index].data[cmpSystem][offset + i]) {
						isEqual = 0;
						break;
					}
				}
			}
		}
		if (!isEqual) {
			break;
		}
	}

	if (isEqual && (system >= 0)) {
		cpt = 0;
		dataOutTypeSize = MIDI_RTP_DATA_TYPE_SIZE[g_param[index].datatype];
		chunkSizeMax = MIDI_RTP_DATA_SIZE_BIN / dataOutTypeSize;
		while(cpt < length) {
			chunkSize = length - cpt;
			if(chunkSize > chunkSizeMax) chunkSize = chunkSizeMax;
			if (sysmask == -1) {
				snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%c -1 %s.%s.%s %d %d %s",
						 (char)(cursys + 97),
						 g_szHostName, g_szModelName, g_param[index].name,
						 offset + cpt, chunkSize, MIDI_RTP_DATA_TYPE_STRING[g_param[index].datatype]);
			} else {
				snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%c %X %s.%s.%s %d %d %s",
						 (char)(cursys + 97),
						 (unsigned int)sysmask,
						 g_szHostName, g_szModelName, g_param[index].name,
						 offset + cpt, chunkSize, MIDI_RTP_DATA_TYPE_STRING[g_param[index].datatype]);
			}
			szStrOutLength = strlen(szStrOut) + 1;
			if(szStrOutLength > MIDI_RTP_DATA_SIZE_SZ) {
				fprintf(stderr, "ERROR: RTP Param String Out Length Invalid\n");
				return PARAM_ERROR;
			}

			switch(g_param[index].datatype) {
			case MIDI_RTP_DATA_TYPE_INTEGER:
				for(i = 0; i < chunkSize; i++) {
#if defined(ARMV7)
					int32_t n = (int32_t)(g_param[index].data[system][offset + cpt + i]);
					for(int j = 0; j < sizeof(int32_t); j++) {
						*((uint8_t *)szStrOut + szStrOutLength + i * sizeof(int32_t) + j) = *((uint8_t *)&n + j);
					}
#else
					*(int32_t *)((uint8_t *)szStrOut + szStrOutLength + i * dataOutTypeSize) = (int32_t)(
								g_param[index].data[system][offset + cpt + i]);
#endif
				}
				break;
			case MIDI_RTP_DATA_TYPE_SINGLE:
				for(i = 0; i < chunkSize; i++) {
#if defined(ARMV7)
					float n = (float)(g_param[index].data[system][offset + cpt + i]);
					for(int j = 0; j < sizeof(float); j++) {
						*((uint8_t *)szStrOut + szStrOutLength + i * sizeof(float) + j) = *((uint8_t *)&n + j);
					}
#else
					*(float *)((uint8_t *)szStrOut + szStrOutLength + i * dataOutTypeSize) = (float)(
								g_param[index].data[system][offset + cpt + i]);
#endif
				}
				break;
			case MIDI_RTP_DATA_TYPE_DOUBLE:
				for(i = 0; i < chunkSize; i++) {
#if defined(ARMV7)
					double n = (double)(g_param[index].data[system][offset + cpt + i]);
					for(int j = 0; j < sizeof(double); j++) {
						*((uint8_t *)szStrOut + szStrOutLength + i * sizeof(double) + j) = *((uint8_t *)&n + j);
					}
#else
					*(double *)((uint8_t *)szStrOut + szStrOutLength + i * dataOutTypeSize) = (double)(
								g_param[index].data[system][offset + cpt + i]);
#endif
				}
				break;
			}
			//MIDI_CmdEncode(&msg, MIDI_CMD_RTP, cmd, (uint8_t*)szStrOut, szStrOutLength+chunkSize*dataOutTypeSize);
			MIDI_CmdEncode(&msg, MIDI_CMD_RTP, MIDI_SCMD_RTP_VALUE_SYS, (uint8_t *)szStrOut,
						   szStrOutLength + chunkSize * dataOutTypeSize);
			if(MIDI_QueueWrite(g_paramQueueOut, &msg) != MIDI_OK) {
				Log(LOG_ERR, "***ERROR*** MIDI_PARAM_OUT Queue Full\n");
				return PARAM_ERROR;
			}
			cpt += chunkSize;
		}
	} else {
		for(system = 0; system < PARAM_NB_SYSTEM_MAX; system++) {
			if (valSysmask & (1 << system)) {
				PARAM_SendValueSys(index, (1 << system), offset, length);
			}
		}
	}
	return PARAM_OK;
}


int32_t PARAM_SendUpdate(int32_t index, int32_t offset, int32_t length, int32_t repeat) {
	int i;
	for(i = 0; i < repeat; i++) {
		if(PARAM_SendValueSys(index, 0, offset, length)) {
			fprintf(stderr, "ERROR: PARAM_SendUpdate Error\n");
			return PARAM_ERROR;
		}
	}
	return PARAM_OK;
}

int32_t PARAM_ProcessParamMidiMsg(uint8_t *event, int32_t eventSize, uint8_t cmd, uint8_t scmd, uint8_t *buf,
								 uint8_t size) {
	int i, preampSystem, system, index;
	int szStrInLength;
	int dataInType, dataInTypeSize;
	MIDI_MSG msg;
	uint8_t *bufBin;
	char szStrIn[MIDI_MSG_PAYLOAD_SIZE_MAX];
	char szStrOut[MIDI_MSG_PAYLOAD_SIZE_MAX];
	char *szSysmask, *szParam, *szLength, *szType, *szOffset, *szInvalid;
	char *szParamName, *szTemp;
	int length, offset;
	long long sysmask, origSysmask;

	memcpy(szStrIn, buf, size);
	szStrIn[size] = 0;
	//for(i=0; i<size; i++) if(szStr[i] < ' ') szStr[i] = ' ';
	if(cmd == MIDI_CMD_RTP) {
		switch(scmd) {
		case MIDI_SCMD_RTP_SCAN:
			system = PARAM_GetPreampSystem();
			for(i = 0; i <= g_lastIndex; i++) {
				snprintf(szStrOut, MIDI_MSG_PAYLOAD_SIZE_MAX, "%s.%s.%s %d %s %s %g %g %s",
						 g_szHostName, g_szModelName, g_param[i].name,
						 g_param[i].length,
						 PARAM_IO_TYPE_STRING[g_param[i].iotype],
						 MIDI_RTP_DATA_TYPE_STRING[g_param[i].datatype],
						 g_param[i].min, g_param[i].max,
						 PARAM_PERSISTENT_STRING[g_param[i].persistent]);
				MIDI_CmdEncode(&msg, MIDI_CMD_RTP, MIDI_SCMD_RTP_INFO, (uint8_t *)szStrOut, strlen(szStrOut));
				if(MIDI_QueueWrite(g_paramQueueOut, &msg) != MIDI_OK) {
					Log(LOG_ERR, "***ERROR*** MIDI_PARAM_OUT Queue Full\n");
				}
			}
			break;

		case MIDI_SCMD_RTP_READ:
		case MIDI_SCMD_RTP_READ_SYS:
			szStrInLength = strlen(szStrIn) + 1;
			if(szStrInLength > MIDI_RTP_DATA_SIZE_SZ) {
				fprintf(stderr, "ERROR: RTP Read Param String In Length Invalid\n");
				return PARAM_ERROR;
			}
			if (scmd == MIDI_SCMD_RTP_READ) {
				szSysmask = NULL;
				szParam = strtok(szStrIn, " ");
			} else {
				szSysmask = strtok(szStrIn, " ");
				szParam = strtok(NULL, " ");
			}
			szOffset = strtok(NULL, " ");
			szLength = strtok(NULL, " ");
			szInvalid = strtok(NULL, " ");
			if(szInvalid != NULL) {
				fprintf(stderr, "ERROR: RTP Read has too many fields\n");
				return PARAM_ERROR;
			}
			if((szParam == NULL) || (szLength == NULL) || (szOffset == NULL)) {
				fprintf(stderr, "ERROR: RTP Read has not enough fields\n");
				return PARAM_ERROR;
			}
			length = atoi(szLength);
			if(!MIDI_IsPositiveInteger(szLength) || (length <= 0)) {
				fprintf(stderr, "ERROR: RTP Read Param Length is a strict positive integer\n");
				return PARAM_ERROR;
			}
			offset = atoi(szOffset);
			if(!MIDI_IsPositiveInteger(szOffset)) {
				fprintf(stderr, "ERROR: RTP Read Param Offset is a positive integer\n");
				return PARAM_ERROR;
			}

			if(PARAM_checkName(szParam, &szParamName)) return PARAM_ERROR;
			index = PARAM_GetIndexByName(szParamName);
			if(index == -1) {
				//printf("name does not match %s.%s.%s\n",szParamHost,szParamModel,szParamName);
				return PARAM_ERROR;
			}

			if(offset >= g_param[index].length) {
				fprintf(stderr, "ERROR: RTP Read Param Offset Invalid\n");
				return PARAM_ERROR;
			}
			if((offset + length) > g_param[index].length) length = g_param[index].length - offset;

			preampSystem = PARAM_GetPreampSystem();
			if (szSysmask == NULL) {
				if(PARAM_SendValueSys(index, 0, offset, length)) {
					fprintf(stderr, "ERROR: RTP Read Param Send Value Error\n");
					return PARAM_ERROR;
				}
			} else {
				if (!strcmp(szSysmask, "-1")) {
					sysmask = -1;
				} else {
					sysmask = (long long)strtoul(szSysmask, &szTemp, 16);
					if ((*szTemp != 0) || (sysmask > MIDI_RTP_SYSMASK_MAX)) {
						fprintf(stderr, "ERROR: RTP Read Sysmask Invalid\n");
						return PARAM_ERROR;
					}
				}
				if(PARAM_SendValueSys(index, sysmask, offset, length)) {
					fprintf(stderr, "ERROR: RTP Read Param Send Value Error\n");
					return PARAM_ERROR;
				}
			}
			break;

		case MIDI_SCMD_RTP_WRITE:
		case MIDI_SCMD_RTP_WRITE_SYS:
			szStrInLength = strlen(szStrIn) + 1;
			if(szStrInLength > MIDI_RTP_DATA_SIZE_SZ) {
				fprintf(stderr, "ERROR: RTP Write Param String In Length Invalid\n");
				return PARAM_ERROR;
			}
			if (scmd == MIDI_SCMD_RTP_WRITE) {
				szSysmask = NULL;
				szParam = strtok(szStrIn, " ");
			} else {
				szSysmask = strtok(szStrIn, " ");
				szParam = strtok(NULL, " ");
			}
			szOffset = strtok(NULL, " ");
			szLength = strtok(NULL, " ");
			szType = strtok(NULL, " ");
			szInvalid = strtok(NULL, " ");
			if(szInvalid != NULL) {
				fprintf(stderr, "ERROR: RTP Write has too many fields\n");
				return PARAM_ERROR;
			}
			if((szParam == NULL) || (szLength == NULL) || (szType == NULL) || (szOffset == NULL)) {
				fprintf(stderr, "ERROR: RTP Write has not enough fields\n");
				return PARAM_ERROR;
			}
			length = atoi(szLength);
			if(!MIDI_IsPositiveInteger(szLength) || (length <= 0)) {
				fprintf(stderr, "ERROR: RTP Write Param Length is a strict positive integer\n");
				return PARAM_ERROR;
			}
			offset = atoi(szOffset);
			if(!MIDI_IsPositiveInteger(szOffset)) {
				fprintf(stderr, "ERROR: RTP Write Param Offset is a positive integer\n");
				return PARAM_ERROR;
			}
			dataInType = 0;
			dataInTypeSize = 0;
			for(i = 0; i < MIDI_RTP_DATA_TYPE_NB; i++) {
				if(!strncmp(szType, MIDI_RTP_DATA_TYPE_STRING[i], MIDI_MSG_PAYLOAD_SIZE_MAX)) {
					dataInType = i;
					dataInTypeSize = MIDI_RTP_DATA_TYPE_SIZE[i];
					break;
				}
			}
			if(dataInTypeSize == 0) {
				fprintf(stderr, "ERROR: RTP Write Param Type Invalid\n");
				return PARAM_ERROR;
			}

			if(PARAM_checkName(szParam, &szParamName)) return PARAM_ERROR;
			index = PARAM_GetIndexByName(szParamName);
			if(index == -1) {
				//printf("name does not match %s.%s.%s\n",szParamHost,szParamModel,szParamName);
				return PARAM_ERROR;
			}

			if(offset >= g_param[index].length) {
				fprintf(stderr, "ERROR: RTP Write Param Offset Invalid\n");
				return PARAM_ERROR;
			}
			if((offset + length) > g_param[index].length) length = g_param[index].length - offset;
			preampSystem = PARAM_GetPreampSystem();
			if (szSysmask == NULL) {
				sysmask = 1 << preampSystem;
				origSysmask = sysmask;
			} else if (!strcmp(szSysmask, "-1")) {
				sysmask = MIDI_RTP_SYSMASK_MAX;
				origSysmask = -1;
			} else {
				sysmask = (long long)strtoul(szSysmask, &szTemp, 16);
				if ((*szTemp != 0) || (sysmask > MIDI_RTP_SYSMASK_MAX)) {
					fprintf(stderr, "ERROR: RTP Write Sysmask Invalid\n");
					return PARAM_ERROR;
				}
				origSysmask = sysmask;
				if (sysmask == 0) {
					sysmask = 1 << preampSystem;
				}
			}
			bufBin = (uint8_t *)(&event[9] + strnlen((char *)&event[9], eventSize) + 1);
			if((g_param[index].iotype == PARAM_IO_TYPE_WRITE)
					|| (g_param[index].iotype == PARAM_IO_TYPE_PASSTHROUGH)) {
				for(system = 0; system < PARAM_NB_SYSTEM_MAX; system++) {
					if (sysmask & (1 << system)) {
						for(i = 0; i < length; i++) {
							switch(dataInType) {
							case MIDI_RTP_DATA_TYPE_INTEGER :
								g_param[index].data[system][offset + i] = *((int32_t *)((uint8_t *)(bufBin + i * dataInTypeSize)));
								break;
							case MIDI_RTP_DATA_TYPE_SINGLE :
								g_param[index].data[system][offset + i] = *((float *)((uint8_t *)(bufBin + i * dataInTypeSize)));
								break;
							case MIDI_RTP_DATA_TYPE_DOUBLE :
								g_param[index].data[system][offset + i] = *((double *)((uint8_t *)(bufBin + i * dataInTypeSize)));
								break;
							}
						}
					}
				}
			}
			if (szSysmask == NULL) {
				if(PARAM_SendValueSys(index, 0, offset, length)) {
					fprintf(stderr, "ERROR: RTP Write Param Send Value Error\n");
					return PARAM_ERROR;
				}
			} else {
				if(PARAM_SendValueSys(index, origSysmask, offset, length)) {
					fprintf(stderr, "ERROR: RTP Write Param Send Value Error\n");
					return PARAM_ERROR;
				}
			}
			g_param[index].dirty = 1;
			break;

		case MIDI_SCMD_RTP_ADD_SYS:
		case MIDI_SCMD_RTP_MUL_SYS:
		case MIDI_SCMD_RTP_OR_SYS:
		case MIDI_SCMD_RTP_AND_SYS:
		case MIDI_SCMD_RTP_XOR_SYS:
			szStrInLength = strlen(szStrIn) + 1;
			if(szStrInLength > MIDI_RTP_DATA_SIZE_SZ) {
				fprintf(stderr, "ERROR: RTP Modify Param String In Length Invalid\n");
				return PARAM_ERROR;
			}
			szSysmask = strtok(szStrIn, " ");
			szParam = strtok(NULL, " ");
			szOffset = strtok(NULL, " ");
			szLength = strtok(NULL, " ");
			szType = strtok(NULL, " ");
			szInvalid = strtok(NULL, " ");
			if(szInvalid != NULL) {
				fprintf(stderr, "ERROR: RTP Modify has too many fields\n");
				return PARAM_ERROR;
			}
			if((szParam == NULL) || (szLength == NULL) || (szType == NULL) || (szOffset == NULL)) {
				fprintf(stderr, "ERROR: RTP Modify has not enough fields\n");
				return PARAM_ERROR;
			}
			length = atoi(szLength);
			if(!MIDI_IsPositiveInteger(szLength) || (length <= 0)) {
				fprintf(stderr, "ERROR: RTP Modify Param Length is a strict positive integer\n");
				return PARAM_ERROR;
			}
			offset = atoi(szOffset);
			if(!MIDI_IsPositiveInteger(szOffset)) {
				fprintf(stderr, "ERROR: RTP Modify Param Offset is a positive integer\n");
				return PARAM_ERROR;
			}
			dataInType = 0;
			dataInTypeSize = 0;
			for(i = 0; i < MIDI_RTP_DATA_TYPE_NB; i++) {
				if(!strncmp(szType, MIDI_RTP_DATA_TYPE_STRING[i], MIDI_MSG_PAYLOAD_SIZE_MAX)) {
					dataInType = i;
					dataInTypeSize = MIDI_RTP_DATA_TYPE_SIZE[i];
					break;
				}
			}
			if(dataInTypeSize == 0) {
				fprintf(stderr, "ERROR: RTP Modify Param Type Invalid\n");
				return PARAM_ERROR;
			}

			if(PARAM_checkName(szParam, &szParamName)) return PARAM_ERROR;
			index = PARAM_GetIndexByName(szParamName);
			if(index == -1) {
				//printf("name does not match %s.%s.%s\n",szParamHost,szParamModel,szParamName);
				return PARAM_ERROR;
			}

			if(offset >= g_param[index].length) {
				fprintf(stderr, "ERROR: RTP Modify Param Offset Invalid\n");
				return PARAM_ERROR;
			}
			if((offset + length) > g_param[index].length) length = g_param[index].length - offset;
			preampSystem = PARAM_GetPreampSystem();
			if (!strcmp(szSysmask, "-1")) {
				sysmask = MIDI_RTP_SYSMASK_MAX;
				origSysmask = -1;
			} else {
				sysmask = (long long)strtoul(szSysmask, &szTemp, 16);
				if ((*szTemp != 0) || (sysmask > MIDI_RTP_SYSMASK_MAX)) {
					fprintf(stderr, "ERROR: RTP Modify Sysmask Invalid\n");
					return PARAM_ERROR;
				}
				origSysmask = sysmask;
				if (sysmask == 0) {
					sysmask = 1 << preampSystem;
				}
			}
			bufBin = (uint8_t *)(&event[9] + strnlen((char *)&event[9], eventSize) + 1);
			if((g_param[index].iotype == PARAM_IO_TYPE_WRITE)
					|| (g_param[index].iotype == PARAM_IO_TYPE_PASSTHROUGH)) {
				for(system = 0; system < PARAM_NB_SYSTEM_MAX; system++) {
					if(sysmask & (1 << system)) {
						for(i = 0; i < length; i++) {
							switch(scmd) {
							case MIDI_SCMD_RTP_ADD_SYS:
								switch(dataInType) {
								case MIDI_RTP_DATA_TYPE_INTEGER :
									g_param[index].data[system][offset + i] = (int32_t)g_param[index].data[system][offset + i] +
										*((int32_t*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_SINGLE :
									g_param[index].data[system][offset + i] = (float)g_param[index].data[system][offset + i] +
										*((float*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_DOUBLE :
									g_param[index].data[system][offset + i] = (double)g_param[index].data[system][offset + i] +
										*((double*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								}
								break;
							case MIDI_SCMD_RTP_MUL_SYS:
								switch(dataInType) {
								case MIDI_RTP_DATA_TYPE_INTEGER :
									g_param[index].data[system][offset + i] = (int32_t)g_param[index].data[system][offset + i] *
										*((int32_t*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_SINGLE :
									g_param[index].data[system][offset + i] = (float)g_param[index].data[system][offset + i] *
										*((float*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_DOUBLE :
									g_param[index].data[system][offset + i] = (double)g_param[index].data[system][offset + i] *
										*((double*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								}
								break;
							case MIDI_SCMD_RTP_OR_SYS:
								switch(dataInType) {
								case MIDI_RTP_DATA_TYPE_INTEGER :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] |
										*((uint32_t*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_SINGLE :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] |
										(uint32_t)(*((float*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								case MIDI_RTP_DATA_TYPE_DOUBLE :
									g_param[index].data[system][offset + i] = (uint64_t)g_param[index].data[system][offset + i] |
										(uint64_t)(*((double*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								}
								break;
							case MIDI_SCMD_RTP_AND_SYS:
								switch(dataInType) {
								case MIDI_RTP_DATA_TYPE_INTEGER :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] &
										*((uint32_t *)((uint8_t *)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_SINGLE :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] &
										(uint32_t)(*((float*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								case MIDI_RTP_DATA_TYPE_DOUBLE :
									g_param[index].data[system][offset + i] = (uint64_t)g_param[index].data[system][offset + i] &
										(uint64_t)(*((double*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								}
								break;
							case MIDI_SCMD_RTP_XOR_SYS:
								switch(dataInType) {
								case MIDI_RTP_DATA_TYPE_INTEGER :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] ^
										*((uint32_t*)((uint8_t*)(bufBin + i * dataInTypeSize)));
									break;
								case MIDI_RTP_DATA_TYPE_SINGLE :
									g_param[index].data[system][offset + i] = (uint32_t)g_param[index].data[system][offset + i] ^
										(uint32_t)(*((float*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								case MIDI_RTP_DATA_TYPE_DOUBLE :
									g_param[index].data[system][offset + i] = (uint64_t)g_param[index].data[system][offset + i] ^
										(uint64_t)(*((double*)((uint8_t*)(bufBin + i * dataInTypeSize))));
									break;
								}
								break;
							}
							if(g_param[index].data[system][offset + i] > g_param[index].max) g_param[index].data[system][offset
										+ i] = g_param[index].max;
							if(g_param[index].data[system][offset + i] < g_param[index].min) g_param[index].data[system][offset
										+ i] = g_param[index].min;
						}
					}
				}
			}
			if(PARAM_SendValueSys(index, origSysmask, offset, length)) {
				fprintf(stderr, "ERROR: RTP Modify Param Send Value Error\n");
				return PARAM_ERROR;
			}
			g_param[index].dirty = 1;
			break;

		default:
			break;
		}
	}
	return PARAM_OK;
}

int32_t PARAM_SetName(int32_t index, char *name) {
	if((index >= 0) && (index < PARAM_MAX)) {
		if(*g_param[index].name != 0) {
			fprintf(stderr, "ERROR: PARAM Cannot assign name to index already in use\n");
			exit(-1);
		}
		if(*name == 0) {
			fprintf(stderr, "ERROR: PARAM Cannot name cannot be empty\n");
			exit(-1);
		}
		if(PARAM_GetIndexByName(name) != -1) {
			fprintf(stderr, "ERROR: 'PARAM %s' must be unique\n", name);
			exit(-1);
		}
		strlcpy(g_param[index].name, name, PARAM_NAME_SIZE_MAX);
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_SetType(int32_t index, int32_t iotype, int32_t datatype, int32_t persistent) {
	if((index >= 0) && (index < PARAM_MAX)) {
		g_param[index].iotype = iotype;
		g_param[index].datatype = datatype;
		g_param[index].persistent = persistent;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_SetRange(int32_t index, double min, double max) {
	if((index >= 0) && (index < PARAM_MAX)) {
		g_param[index].min = min;
		g_param[index].max = max;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_SetValue(int32_t index, int32_t length, double *data) {
	int i;
	int32_t system;
	if((index >= 0) && (index < PARAM_MAX)) {
		if(g_param[index].data[0] == NULL) {
			g_param[index].length = length;
			for(i = 0; i < PARAM_NB_SYSTEM_MAX; i++) {
				g_param[index].data[i] = (double *)malloc(length * sizeof(double));
				if(g_param[index].data[i] == NULL) {
					fprintf(stderr, "ERROR: Realtime Parameters Memory Allocation\n");
					exit(-1);
				}
				memcpy(g_param[index].data[i], data, length * sizeof(double));
			}
		} else {
			system = PARAM_GetPreampSystem();
			memcpy(g_param[index].data[system], data, length * sizeof(double));
			g_param[index].dirty = 0;
		}
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_SetDirtyFlag(int32_t index) {
	if((index >= 0) && (index < PARAM_MAX)) {
		g_param[index].dirty = 1;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_ClearDirtyFlag(int32_t index) {
	if((index >= 0) && (index < PARAM_MAX)) {
		g_param[index].dirty = 0;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}

int32_t PARAM_GetName(int32_t index, char *name) {
	if((index >= 0) && (index < PARAM_MAX)) {
		name = g_param[index].name;
		return PARAM_OK;
	}
	name[0] = 0;
	return PARAM_ERROR;
}
int32_t PARAM_GetType(int32_t index, int32_t *iotype, int32_t *datatype, int32_t *persistent) {
	if((index >= 0) && (index < PARAM_MAX)) {
		*iotype = g_param[index].iotype;
		*datatype = g_param[index].datatype;
		*persistent = g_param[index].persistent;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_GetRange(int32_t index, double *min, double *max) {
	if((index >= 0) && (index < PARAM_MAX)) {
		*min = g_param[index].min;
		*max = g_param[index].max;
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_GetValue(int32_t index, int32_t *length, double **data) {
	int32_t system;
	if((index >= 0) && (index < PARAM_MAX)) {
		*length = g_param[index].length;
		system = PARAM_GetPreampSystem();
		*data = g_param[index].data[system];
		return PARAM_OK;
	}
	return PARAM_ERROR;
}
int32_t PARAM_GetDirtyFlag(int32_t index) {
	if((index >= 0) && (index < PARAM_MAX)) {
		return g_param[index].dirty;
	}
	return 0;
}


// Preamp Functions
// ****************
int32_t PARAM_ClearPreampDirtyFlags() {
	g_preamp.systemDirty = 0;
	g_preamp.volumeDirty = 0;
	g_preamp.balanceDirty = 0;
	g_preamp.faderDirty = 0;
	g_preamp.bassDirty = 0;
	g_preamp.midDirty = 0;
	g_preamp.trebleDirty = 0;
	g_preamp.param1Dirty = 0;
	g_preamp.param2Dirty = 0;
	return PARAM_OK;
}

int32_t PARAM_ProcessPreampMidiMsg(uint8_t cmd, uint8_t scmd, uint8_t *buf, uint8_t size) {
	char szStr[MIDI_MSG_PAYLOAD_SIZE_MAX + 1];
	int i;
	memcpy(szStr, buf, size);
	szStr[size] = 0;
	for(i = 0; i < size; i++) if(szStr[i] < ' ') szStr[i] = ' ';
	if(cmd == MIDI_CMD_PREAMP) {
		switch(scmd) {
		case MIDI_SCMD_PREAMP_SYSTEM:
			szStr[0] = toupper(szStr[0]);
			g_preamp.system = szStr[0] - 65;
			if(g_preamp.system < 0) g_preamp.system = 0;
			if(g_preamp.system > (PARAM_NB_SYSTEM_MAX - 1)) g_preamp.system = (PARAM_NB_SYSTEM_MAX - 1);
			g_preamp.systemDirty = 1;
			for(i = 0; i <= g_lastIndex; i++) PARAM_SetDirtyFlag(i);
			break;
		case MIDI_SCMD_PREAMP_VOL:
			g_preamp.volume = atof(szStr);
			if(g_preamp.volume < 0.0) g_preamp.volume = 0.0;
			if(g_preamp.volume > 1.0) g_preamp.volume = 1.0;
			g_preamp.volumeDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_BAL:
			g_preamp.balance = atof(szStr);
			if(g_preamp.balance < 0.0) g_preamp.balance = 0.0;
			if(g_preamp.balance > 1.0) g_preamp.balance = 1.0;
			g_preamp.balanceDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_FAD:
			g_preamp.fader = atof(szStr);
			if(g_preamp.fader < 0.0) g_preamp.fader = 0.0;
			if(g_preamp.fader > 1.0) g_preamp.fader = 1.0;
			g_preamp.faderDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_BASS:
			g_preamp.bass = atof(szStr);
			if(g_preamp.bass < 0.0) g_preamp.bass = 0.0;
			if(g_preamp.bass > 1.0) g_preamp.bass = 1.0;
			g_preamp.bassDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_MID:
			g_preamp.mid = atof(szStr);
			if(g_preamp.mid < 0.0) g_preamp.mid = 0.0;
			if(g_preamp.mid > 1.0) g_preamp.mid = 1.0;
			g_preamp.midDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_TREB:
			g_preamp.treble = atof(szStr);
			if(g_preamp.treble < 0.0) g_preamp.treble = 0.0;
			if(g_preamp.treble > 1.0) g_preamp.treble = 1.0;
			g_preamp.trebleDirty = 1;
			break;
		case MIDI_SCMD_PREAMP_PARAM1:
			g_preamp.param1 = atof(szStr);
			if(g_preamp.param1 < 0.0) g_preamp.param1 = 0.0;
			if(g_preamp.param1 > 1.0) g_preamp.param1 = 1.0;
			g_preamp.param1Dirty = 1;
			break;
		case MIDI_SCMD_PREAMP_PARAM2:
			g_preamp.param2 = atof(szStr);
			if(g_preamp.param2 < 0.0) g_preamp.param2 = 0.0;
			if(g_preamp.param2 > 1.0) g_preamp.param2 = 1.0;
			g_preamp.param2Dirty = 1;
			break;
		}
	}
	return PARAM_OK;
}

int32_t PARAM_GetPreamp(PARAM_PREAMP *preamp) {
	memcpy(preamp, &g_preamp, sizeof(PARAM_PREAMP));
	return PARAM_OK;
};

int32_t PARAM_GetPreampSystem() {
	if((g_preamp.system >= 0) && (g_preamp.system < PARAM_NB_SYSTEM_MAX))
		return g_preamp.system;
	else
		return 0;
}


