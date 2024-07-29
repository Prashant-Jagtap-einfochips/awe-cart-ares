/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
********************************************************************************
*  Title:       retiostream_file.c
*  Description: Simulink PIL communications channel using filesystem.
*
*  References: see rtiostream_serial.c Copyright 1994-2017 The MathWorks, Inc.
*
********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "rtiostream.h"
#include "errno.h"
//#include "tmwtypes.h"
//#include <assert.h>

/***************** DEFINES ****************************************************/

#define MAX_STREAMS  3     // number of simultaneous file-based channels supported
#define MAX_PATHLEN  200

#define TOHOST_FILENAME "toHostPIL"
#define TOTARGET_FILENAME  "toTargetPIL"


/* debug log levels:
 *
 * 0: no log output
 * 1: verbose log output
 * 2: extra verbose log output */
typedef enum { VERBOSITY_LEVEL_0 = 0, VERBOSITY_LEVEL_1, VERBOSITY_LEVEL_2, VERBOSITY_LEVEL_3 } VerbosityLevel;
/* default verbosity */
#define DEFAULT_VERBOSITY VERBOSITY_LEVEL_1


/***************** TYPEDEFS **************************************************/
// TWO files are needed for each stream: one to read and one to write
// The readFile might not exist yet, so we keep the path so later attempts
// to open can try again.
typedef struct FileStreamData_tag
{
    int isInUse;
    VerbosityLevel verbosity;
    int isTarget;
    clock_t initClock;
    const char* writeName;
    const char* readName;
    const char* logName;
    FILE* writeFile;
    FILE* readFile;
    FILE* logFile;
    char folder[MAX_PATHLEN];
    long readPos;
    long readPackageNumber;
    long readEmptyCount;
    long totalReadEmptyCount;
    long writePackageNumber;
} FileStreamData;

/**************** LOCAL DATA *************************************************/

/* All local data resides in the per instance FileStreamData structures to
 * make sure each connection is completely independent.
 *
 * The static array will be deallocated when the shared library is unloaded.
 *
 * Using an array rather than a linked list allows us to have fast direct lookup
 * of FileStreamData from streamID during calls to rtIOStreamSend/Recv */
static FileStreamData FileChannel[MAX_STREAMS];
static int isInitialized = 0;

/************** LOCAL FUNCTION PROTOTYPES ************************************/
static int processArgs(const int argc, void * argv[], FileStreamData *sd);

static void initFileStreamDataStructure(void);

static FileStreamData * getFileChannel(int streamID);

static FILE* openFile(FileStreamData* sd, const char* filename, const char* fopenMode);

static void logOut(FileStreamData* sd, int verboseLevel, const char* format, ...);

/*************** LOCAL FUNCTIONS **********************************************/

FILE* openFile(FileStreamData* sd, const char* filename, const char* fopenMode)
{
    char fullPath[MAX_PATHLEN];
    strncpy(fullPath,sd->folder,MAX_PATHLEN);
    strncat(fullPath,filename,MAX_PATHLEN);
    FILE* theFile = fopen(fullPath, fopenMode);
    if (theFile)
    {
        logOut(sd,VERBOSITY_LEVEL_3,"Opened [%s] mode [%s]\n",fullPath, fopenMode);
    }
    else
    {
        logOut(sd,0,"Could not open %s mode %s, errno:%d\n",
           fullPath, fopenMode, errno);
    }
    return theFile;
}

void logOut(FileStreamData* sd, int verboseLevel, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    if (sd && sd->logFile && sd->verbosity >= verboseLevel)
    {
        vfprintf(sd->logFile, format, args);
        fflush(sd->logFile);
    }
    va_end(args);
}

/* Function: initFileStreamDataStructure ===========================================================
 * Abstract:
 *  Initializes the data structures to zero.
 */
static void initFileStreamDataStructure(void)
{
    if (!isInitialized)
    {
        memset(&FileChannel, 0, sizeof(FileChannel));
        isInitialized = 1;
    }
}


/* Function: processArgs ====================================================
 * Abstract:
 *  Process the arguments specified by the user when opening the rtIOStream.
 *
 *  If any unrecognized options are encountered, ignore them.
 *
 * Returns zero if successful or RTIOSTREAM_ERROR if
 * an error occurred.
 *
 *  o IMPORTANT!!!
 *    As the arguments are processed, their strings should be set to NULL in
 *    the argv array.
 */
static int processArgs(
    const int      argc,
    void         * argv[],
    FileStreamData *sd)
{
    int        retVal = RTIOSTREAM_NO_ERROR;
    int        count = 0;

    if (argc < 2)
    {
        printf("Target mode. (argc is only %d)\n", argc);
        sd->isTarget = 1;
        sd->readName = TOTARGET_FILENAME;
        sd->writeName = TOHOST_FILENAME;
        sd->logName = "logtarget.txt";
    }
    else
    {
        printf("Host mode. (argc is %d)\n", argc);
        sd->isTarget = 0;
        sd->readName = TOHOST_FILENAME;
        sd->writeName = TOTARGET_FILENAME;
        sd->logName = "loghost.txt";
    }
    sd->folder[0] = 0;

    while (count < argc && 0 == sd->isTarget)
    {
        const char *option = (char *)argv[count];
        count++;

        if (option != NULL)
        {
            logOut(sd,0,"Option %d is %s\n",count,option);

            if ((strcmp(option, "-path") == 0) && (count != argc))
            {
                // Host process uses path to know where test happens
                const char* thePath = (char*)argv[count];
                count++;
                strncpy(sd->folder,thePath,MAX_PATHLEN);
            }
            else if ((strcmp(option, "-verbose") == 0) && (count != argc))
            {
                int itemsConverted;
                int verbosity;
                const char *verbosityStr = (char *)argv[count];
                count++;
                itemsConverted = sscanf(verbosityStr, "%d", &verbosity);

                if ((itemsConverted != 1) || (verbosity < 0))
                {
                    retVal = RTIOSTREAM_ERROR;
                    break;
                }
                else
                {
                    sd->verbosity = (VerbosityLevel)verbosity;
                    argv[count - 2] = NULL;
                    argv[count - 1] = NULL;
                }
            }
            else
            {
                /* issue a warning for the unexpected argument: exception
                 * is first argument which might be the executable name (
                 * SIL/PIL and ext-mode use-cases). */
                if ((count != 1) || (strncmp(option, "-", 1) == 0))
                {
                    printf("The argument '%s' passed to rtiostream_file is "
                        "not valid and will be ignored.\n", option);
                }
            }
        }
    }
    return retVal;
}


static int fileInputOutputInitialize(FileStreamData* sd, const int streamID, int argc, void* argv[])
{
    int errorCode = RTIOSTREAM_NO_ERROR;
    int result;

    sd->verbosity = DEFAULT_VERBOSITY;
    sd->readPackageNumber = 0;
    sd->readEmptyCount = 0;
    sd->totalReadEmptyCount = 0;
    sd->writePackageNumber = 0;

    result = processArgs(argc, argv, sd);

    if (result == RTIOSTREAM_ERROR)
    {
        printf("processArgs failed\n");
        errorCode = RTIOSTREAM_ERROR;
    }
    else
    {
        sd->readPos = 0;
        sd->logFile = openFile(sd, sd->logName, "wb");
        logOut(sd, VERBOSITY_LEVEL_1, "\n-----------------------------------\n");
        logOut(sd, VERBOSITY_LEVEL_1, "Opening [%s] connection\n",
            sd->isTarget ? "Target" : "Host");
        // Open file to append
        sd->writeFile = openFile(sd,sd->writeName, "ab");
        sd->readFile = openFile(sd,sd->readName, "rb");
        if (sd->writeFile == 0 | sd->readFile == 0)
        {
            errorCode = RTIOSTREAM_ERROR;
        }
    }
    
    return errorCode;
}

/* Function: getFileChannel =================================================
 * Abstract:
 *  Retrieves a FileStreamData instance given its streamID.
 *
 * NOTE: An invalid streamID will lead to a NULL pointer being returned
 */
static FileStreamData * getFileChannel(int streamID)
{
    /* return NULL for invalid or uninitialized streamIDs */
    FileStreamData * sd = NULL;
    if ((streamID >= 0) && (streamID < MAX_STREAMS))
    {
        if (FileChannel[streamID].isInUse)
        {
            sd = &FileChannel[streamID];
        }
    }
    return sd;
}

/***************** VISIBLE FUNCTIONS ******************************************/

/* Function: rtIOStreamOpen =================================================
 * Abstract:
 *  Open the connection with the target.
 */
int rtIOStreamOpen(int argc, void * argv[])
{
    int streamID;
    int errorCode;
    FileStreamData *sd;

    initFileStreamDataStructure();

    /* Find any free ports */
    for (streamID = 0; streamID < MAX_STREAMS; streamID++)
    {
        if (!FileChannel[streamID].isInUse)
        {
            break;
        }
    }
    if (streamID == MAX_STREAMS)
    {
        printf("All %d elements of the FileStreamData structure are already in use.", MAX_STREAMS);
        return RTIOSTREAM_ERROR;
    }

    sd = &FileChannel[streamID];
    /* flag instance as in use */
    sd->isInUse = 1;

    errorCode = fileInputOutputInitialize(sd, streamID, argc, argv);

    if (errorCode == RTIOSTREAM_NO_ERROR)
    {
        /* initialize the activity timer */
        sd->initClock = clock();
        return streamID;
    }
    else
    {
        /* close it down and flag error */
        rtIOStreamClose(streamID);
        return RTIOSTREAM_ERROR;
    }
}



/* Function: rtIOStreamSend =====================================================
 * Abstract:
 *  Sends the specified number of bytes on the comm line. Returns the number of
 *  bytes sent (if successful) or a negative value if an error occurred. As long
 *  as an error does not occur, this function is guaranteed to set the requested
 *  number of bytes
 */
int rtIOStreamSend(
    int streamID,
    const void * const src,
    size_t size,
    size_t *sizeSent)
{
    int retVal = RTIOSTREAM_NO_ERROR;
    FileStreamData *sd = getFileChannel(streamID);
    if (sd == NULL)
    {
        retVal = RTIOSTREAM_ERROR;
        return retVal;
    }
    long writePos = -1;
    if (NULL == sd->writeFile)
    {
        logOut(sd,0,"Failed to open writeFile [%s]\n", sd->writeName);
        retVal = RTIOSTREAM_ERROR;
        return retVal;
    }
    else
    {
        fseek(sd->writeFile, 0, SEEK_END);
        writePos = ftell(sd->writeFile);
        *sizeSent = fwrite(src, 1, size, sd->writeFile);
        fflush(sd->writeFile);
    }

    if (*sizeSent > 0) sd->writePackageNumber++;

    if ((*sizeSent > 0) || (sd->verbosity >= VERBOSITY_LEVEL_2))
    {
        size_t currElement;

        logOut(sd,VERBOSITY_LEVEL_2,"%08d rtIOStreamSend(%d): pos:%d, size:%lu, sizeSent:%lu = ",
            clock() - sd->initClock,
            sd->writePackageNumber,
            writePos,
            (unsigned long)size,
            (unsigned long)*sizeSent);

        for (currElement = 0; currElement < *sizeSent; currElement++)
        {
            logOut(sd,VERBOSITY_LEVEL_2,"%02x ", ((const unsigned char *)src)[currElement]);
        }
        logOut(sd,VERBOSITY_LEVEL_2,"\n");
    }
    return retVal;
}


/* Function: rtIOStreamRecv ================================================
 * Abstract: receive data
 *
 */
int rtIOStreamRecv(
    int      streamID,
    void   * const dst,
    size_t   size,
    size_t * sizeRecvd)
{
    int retVal = RTIOSTREAM_NO_ERROR;
    FileStreamData *sd = getFileChannel(streamID);
    if (sd == NULL)
    {
        printf("rtIOStreamRecv NULL stream object\n");
        retVal = RTIOSTREAM_ERROR;
        return retVal;
    }

    if (NULL == sd->readFile)
    {
        logOut(sd,0,"Failed to open readFile [%s]\n", sd->readName);
        return RTIOSTREAM_ERROR;
    }

    // Reset file position to pick up where we left off
    if (fseek(sd->readFile, sd->readPos, SEEK_SET))
    {
        logOut(sd,0,"Seek to offset %d failed\n", sd->readPos);
    }
    *sizeRecvd = fread(dst, 1, size, sd->readFile);

    if (*sizeRecvd)
    {
         sd->readPackageNumber++;
    }
    else
    {
        sd->readEmptyCount++;
        sd->totalReadEmptyCount++;
    }
    
    if ((*sizeRecvd > 0))
    {
        size_t currElement;
        if (sd->readEmptyCount)
        {
            logOut(sd,VERBOSITY_LEVEL_2,"[%d attempts]\n", sd->readEmptyCount);
        }
        sd->readEmptyCount = 0; // reset the empty read count
        logOut(sd,VERBOSITY_LEVEL_2,"%08d rtIOStreamRecv(%d): pos:%lu, size:%lu, sizeRecvd:%lu = ",
            clock() - sd->initClock ,
            sd->readPackageNumber,
            (unsigned long)sd->readPos,
            (unsigned long)size,
            (unsigned long)*sizeRecvd);

        for (currElement = 0; currElement < *sizeRecvd; currElement++)
        {
            logOut(sd,VERBOSITY_LEVEL_2,"%02x ", ((unsigned char *)dst)[currElement]);
        }
        logOut(sd,VERBOSITY_LEVEL_2,"\n");
    }
    else if (sd->verbosity >= VERBOSITY_LEVEL_2)
    {
        logOut(sd,VERBOSITY_LEVEL_2,".");
    }

    long newPos = ftell(sd->readFile);
    if (newPos >= 0)
    {
        long correctPos = sd->readPos + *sizeRecvd;
        if (newPos != correctPos)
        {
            logOut(sd,0,"\n\n*** Read pos jumped to %d! (~%d) Should be %d\n\n",
                newPos,
                newPos - correctPos,
                correctPos);
        }
        sd->readPos = newPos;
    }
    else
    {
        logOut(sd,0,"Error:Keeping readPos %d\n", sd->readPos);
    }
    return retVal;
}


/* Function: rtIOStreamClose ================================================
 * Abstract: close the connection.
 *
 */
int rtIOStreamClose(int streamID)
{
    int retVal = RTIOSTREAM_NO_ERROR;
    FileStreamData *sd = getFileChannel(streamID);
    if (sd == NULL)
    {
        retVal = RTIOSTREAM_ERROR;
        return retVal;
    }

    logOut(sd,VERBOSITY_LEVEL_1,"rtIOStreamClose (connection id %d). Total empty read count: %d\n", 
        streamID, sd->totalReadEmptyCount);

    /* close all the files */
    int failed;
    if (sd->readFile)
    {
        failed = fclose(sd->readFile);
        if (failed)
        {
            logOut(sd,0,"rtiostreamclose FAIL stream %d readFile close failed", streamID);
        }
        sd->readFile = 0;
    }
    if (sd->writeFile)
    {
        failed = fclose(sd->writeFile);
        if (failed)
        {
            logOut(sd,0,"rtiostreamclose FAIL stream %d writeFile close failed", streamID);
        }
        sd->writeFile = 0;
    }
    if (sd->logFile)
    {
        failed = fclose(sd->logFile);
        if (failed)
        {
            logOut(sd,0,"rtiostreamclose FAIL stream %d logFile close failed", streamID);
        }
        sd->logFile = 0;
    }

    /* clear out the folder */
    sd->folder[0] = 0;

    /* clear in use flag */
    sd->isInUse = 0;


    return retVal;
}
