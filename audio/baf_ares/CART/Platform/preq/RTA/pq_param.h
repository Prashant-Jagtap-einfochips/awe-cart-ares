//	Title:  pq_param.h
//	Author: Guillaume

#ifndef _PQ_PARAM_H
#define _PQ_PARAM_H
#ifdef __cplusplus
extern "C" {
#endif

#define PARAM_OK       		0
#define PARAM_ERROR			-1

#define PARAM_NB_SYSTEM_MAX     4
#define PARAM_MAX               256
#define PARAM_SIZE_MAX          256
#define PARAM_NAME_SIZE_MAX     50

#define PARAM_IO_TYPE_WRITE         0
#define PARAM_IO_TYPE_READ          1
#define PARAM_IO_TYPE_PASSTHROUGH   2
#define PARAM_IO_TYPE_CONSTANT      3
#define PARAM_IO_TYPE_SYSTEM        4
#define PARAM_NON_PERSISTENT        0
#define PARAM_PERSISTENT            1

#define HOSTNAME_MAX_LENGTH 13
#define MODELNAME_MAX_LENGTH 32

extern const char *PARAM_IO_TYPE_STRING[];
//extern const char *PARAM_DATA_TYPE_STRING[];
//extern const int PARAM_DATA_TYPE_SIZE[];
extern const char *PARAM_PERSISTENT_STRING[];

typedef struct {
	int32_t dirty;
	char name[PARAM_NAME_SIZE_MAX];
	int32_t iotype;
	int32_t datatype;
	double min;
	double max;
	int32_t persistent;
	int32_t length;
	double *data[PARAM_NB_SYSTEM_MAX];
} PARAM_ELT;

typedef struct {
	int32_t system;
	int32_t systemDirty;
	double volume;
	int32_t volumeDirty;
	double balance;
	int32_t balanceDirty;
	double fader;
	int32_t faderDirty;
	double bass;
	int32_t bassDirty;
	double mid;
	int32_t midDirty;
	double treble;
	int32_t trebleDirty;
	double param1;
	int32_t param1Dirty;
	double param2;
	int32_t param2Dirty;
} PARAM_PREAMP;

int32_t PARAM_Init(char *name, char *hostname, char *modelname, MIDI_QUEUE *paramQueueOut);
int32_t PARAM_Load();
int32_t PARAM_Save();
int32_t PARAM_GetNextAvailableIndex();
int32_t PARAM_SetName(int32_t index, char *name);
int32_t PARAM_SetType(int32_t index, int32_t iotype, int32_t datatype, int32_t persistent);
int32_t PARAM_SetRange(int32_t index, double min, double max);
int32_t PARAM_SetValue(int32_t index, int32_t length, double *data);
int32_t PARAM_SetDirtyFlag(int32_t index);
int32_t PARAM_ClearDirtyFlag(int32_t index);
int32_t PARAM_GetName(int32_t index, char *name);
int32_t PARAM_GetType(int32_t index, int32_t *iotype, int32_t *datatype, int32_t *persistent);
int32_t PARAM_GetRange(int32_t index, double *min, double *max);
int32_t PARAM_GetValue(int32_t index, int32_t *length, double **data);
int32_t PARAM_GetDirtyFlag(int32_t index);
int32_t PARAM_GetIndexByName(char *name);
int32_t PARAM_SendRefresh();
int32_t PARAM_SendUpdate(int32_t index, int32_t offset, int32_t length, int32_t repeat);
int32_t PARAM_ProcessParamMidiMsg(uint8_t *event, int32_t eventSize, uint8_t cmd, uint8_t scmd, uint8_t *buf,
								 uint8_t size);

int32_t PARAM_GetPreamp(PARAM_PREAMP *preamp);
int32_t PARAM_GetPreampSystem();
int32_t PARAM_ClearPreampDirtyFlags();
int32_t PARAM_ProcessPreampMidiMsg(uint8_t cmd, uint8_t scmd, uint8_t *buf, uint8_t size);
int32_t PARAM_checkName(char *szParam, char **szName);

#ifdef __cplusplus
}
#endif
#endif
