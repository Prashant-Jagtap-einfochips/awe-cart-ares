/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  fw_FlashFSInstance.h
********************************************************************************
*
*	  Description:	AudioWeaver Flash FileSystem main header file
*
*	  Copyright:	(c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*					3235 Kifer Road
*					Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_FLASHFSINSTANCE_H
#define _FW_FLASHFSINSTANCE_H

#include "FileInfo.h"

#ifdef	__cplusplus
extern "C" {
#endif

/** Target has no file system. */
#define FILESYSTEM_TYPE_NONE			0U

/** Target has compiled in file system. */
#define FILESYSTEM_TYPE_COMPILED_IN		1U

/** Target has flash file system (default). */
#define FILESYSTEM_TYPE_FLASH			2U

/** Target has FAT file system */
#define FILESYSTEM_TYPE_FAT				3U

/** States for ModuleFileStreaming. */
enum FileIOStatus
{
	/* ModuleFileStreaming always writes this (zero) to handshake. */
	FIOS_Null,

	/* These states are in the high byte of buffer[buffer_size]. */
	FIOS_NewStream,
	FIOS_NextBlock,
	FIOS_LastBlock,

	/* These states are reported asynchronously in asyncStatus. */
	FIOS_Stopped,
	FIOS_Paused,
	FIOS_Error,

	/* Used internally by the file streaming module to keep track of the current state */
	FIOS_Playing,
	FIOS_PlayingLast
};

/** This structure defines the file system info. */
typedef struct _FileSystemInfo
{
	UINT32 m_FileSystemType;				/* 0 */
	UINT32 m_FlashDeviceDWords;				/* 4 */
	UINT32 m_FileSystemDWords;				/* 8 */
	UINT32 m_DataStructOverheadDWords;		/* 12 */
	UINT32 m_DeletedOrCorruptedDWords;		/* 16 */
	UINT32 m_DWordsInUse;					/* 20 */
	UINT32 m_DWordsAvailable;				/* 24 */
	UINT32 m_BlkSize_MaxFilename_Len;		/* 28 */
}
FileSystemInfo;		/* size=32 (8 words) */

/**
 * All structure elements are 4-byte DWords
 *
 * Flash Memory Layout:
 *
 * TOP OF FLASH MEMORY
 *
 * File Header Info
 * Allocation table bit map
 * File Directory
 * File data
 * Application Code
 * Boot Loader
 *
 * BOTTOM OF FLASH MEMORY
 *
 * File System Header layout (padded with zeros to fill one allocation block)
 *-------------------------------------------------------------------------------------------------------
 *| Version Number | Start of Data Addr | File Directory Addr | Alloc Table Addr | Allocation Block Size |
 *-------------------------------------------------------------------------------------------------------
 *| 4 bytes        | 4 bytes            | 4 bytes             | 4 bytes          | 4 bytes               |
 *-------------------------------------------------------------------------------------------------------
 *
 * EXAMPLE Allocation Table
 *
 * Assume there are 524288 bytes available in the high end of the flash address space.
 * This is the same as 131072 4-byte DWords.
 * 131072 words divided by 16 words (allocation block size) = 8192 allocation units.
 * There are 32 bits per bitmap word so there needs to be 8192 / 32  = 256 words
 * allocated to the bit map if the allocation table is mapping the entire memory
*/

typedef struct _FSAttributes
{
    /** Byte Address in flash memory of start of file allocation table */
    UINT32 nAllocTableBitMapFlashAddr;	/* 4 */

    /** Block size for the file system 16 words/block */
    UINT32 nAllocBlockSizeInDWords;		/* 8 */

    /** Block size for the file system 64 bytes/block */
    UINT32 nAllocBlockSizeInBytes;		/* 12 */

    /** Byte Address in flash memory of the first entry of the file directory */
    UINT32 nFileDirectoryFlashAddr;		/* 16 */

    /** TOtal flash memory size in bytes */
    UINT32 nFlashMemorySizeBytes;		/* 20 */

    /** Size of flash memory eraseable block size in bytes */
    UINT32 nEraseableBlockSize;			/* 24 */

    /** Count of active files saved in the flash file system */
    INT32  nFileCnt;					/* 28 */

    /** Byte Address in flash memory of the start of file data */
    UINT32 nStartOfFileDataFlashAddr;	/* 32 */

    /** Count of the number of file allocation blocks in use */
    UINT32 nDataBlocksInUse;			/* 36 */

    /** Current directory entry when navigating through the file system */
    DIRECTORY_ENTRY CurrentDirEntry;	/* 36 + 64 = 100 */

    /** Address of first free block for file allocation */
    UINT32 nFirstFreeBlockFlashAddr;	/* 104 */

    /** File is open state */
    BOOL   bFileOpen;					/* 108 */

    /** Number of DWords read so far */
    INT32  nDWordsRead;					/* 112 */

    /** Number of DWords written so far */
    INT32  nDWordsWritten;				/* 116 */

    /** Current file position byte offset */
    UINT32 nFileCurrentPosByteOffset;	/* 120 */

    /** Offset to start of file content area in flash */
    UINT32 nFileContentReadOffset;		/* 124 */

    /** File attribute byte */
    UINT32 nNewFileAttributeByte;		/* 128 */

    /** Current directry entry when navigating */
    UINT32 CurrentDirEntryFlashAddr;	/* 132 */

    /** Start of chain of deleted files */
    UINT32 nFileDirectoryFlashAddr_toDel;	/* 136 */

    /** Residue DWords */
    UINT32 ResidueDWords[ALLOCATION_BLOCKSIZE_DWORDS];	/* 136 + 64 = 200 */

    /** Number of DWords remaining to write */
    UINT32 nRemainingDWordsToWrite;			/* 204 */

    /** Flash file system information */
    FileSystemInfo filesystem_info;			/* 204 + 32 = 236 */

} FSAttributes;

// private data members of AWEFlashFSInstance
#define FILESYSTEM_FRAMEWORK FSAttributes flashAttributes;

// public members of AWEFlashFSInstance
#include "FlashFSInstance.h"

//-----------------------------------------------------------------------------
// METHOD:	awe_fwGetFileAttribute
// PURPOSE: Get the file attribute byte for the indicated file entry in 
//          the Flash File System
//-----------------------------------------------------------------------------
UINT8 awe_fwGetFileAttribute(PDIRECTORY_ENTRY pDirectoryEntry);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwResetFlashFileSystem
// PURPOSE: Reset the flash file system
//-----------------------------------------------------------------------------
INT32 awe_fwResetFlashFileSystem(AWEFlashFSInstance* pAWEFlashFSInstance);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwGetFirstFile
// PURPOSE: Get the first file directory entry in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwGetFirstFile(AWEFlashFSInstance* pAWEFlashFSInstance, PDIRECTORY_ENTRY* pDirEntry);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwGetNextFile
// PURPOSE: Get the next file directory entry in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwGetNextFile(AWEFlashFSInstance* pAWEFlashFSInstance, PDIRECTORY_ENTRY* pDirEntry);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwExecuteFile
// PURPOSE: Execute compiled file saved in the flash file system
//-----------------------------------------------------------------------------
INT32 awe_fwExecuteFile(struct _AWEInstance* pAWE, AWEFlashFSInstance* pAWEFlashFSInstance, UINT32* pFileNameInDWords);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwOpenFile
// PURPOSE: Open a file for read or write in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwOpenFile(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32 nFileAttribute, UINT32* pFileNameInDWords, UINT32* nFileLenInDWords);


//-----------------------------------------------------------------------------
// METHOD:  awe_fwInitFlashFileSystem
// PURPOSE: Initialize the flash file system
//-----------------------------------------------------------------------------
BOOL awe_fwInitFlashFileSystem(AWEFlashFSInstance* pAWEFlashFSInstance);

//-----------------------------------------------------------------------------
// METHOD:  awe_fwInitFFS
// PURPOSE: Initialization wrapper to Flash File System init
//-----------------------------------------------------------------------------
INT32 awe_fwInitFFS(struct _AWEInstance* pAWE);

//-----------------------------------------------------------------------------
// METHOD:	awe_fwCloseFile
// PURPOSE: Close an open file in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwCloseFile(AWEFlashFSInstance* pAWEFlashFSInstance);

//-----------------------------------------------------------------------------
// METHOD:	awe_fwReadFile
// PURPOSE: Read words from file in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwReadFile(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32 nWordsToRead, UINT32* pBuffer, UINT32* pDWordsRead);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwWriteFile
// PURPOSE: Write words to file in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwWriteFile(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32 nWordsToWrite, UINT32* pBuffer, UINT32* pDWordsWritten);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwFindFile
// PURPOSE: Find the directory entry for the named file in the Flash File System
//-----------------------------------------------------------------------------
PDIRECTORY_ENTRY awe_fwFindFile(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32* pFileNameInDWords);


//-----------------------------------------------------------------------------
// METHOD:  awe_fwDeleteFile
// PURPOSE: Marks the file deleted in the Flash File System
//-----------------------------------------------------------------------------
INT32 awe_fwDeleteFile(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32* pFileNameInDWords);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwReadFileMem
// PURPOSE: Reads the Flash File System memory at offset from starting address
//-----------------------------------------------------------------------------
INT32 awe_fwReadFileMem(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32 nStartingAddress, UINT32 nOffsetInDWords, UINT32 nWordsToRead, UINT32* pBuffer);


//-----------------------------------------------------------------------------
// METHOD:	awe_fwGetFileMemStartingAddress
// PURPOSE: Looks up the given file starting memory address and length in DWords
//-----------------------------------------------------------------------------
INT32 awe_fwGetFileMemStartingAddress(AWEFlashFSInstance* pAWEFlashFSInstance, UINT32* pFileNameInDWords, UINT32* nStartingAddress, UINT32* nFileLenInDWords);

//-----------------------------------------------------------------------------
// METHOD:	awe_fwEraseFlash
// PURPOSE: Erase the flash file system to empty state
//-----------------------------------------------------------------------------
INT32 awe_fwEraseFlash(AWEFlashFSInstance* pFS);

//-----------------------------------------------------------------------------
// METHOD:	awe_fwExecuteFlashFiles
// PURPOSE: If a flash file marked for run on boot execute it
//-----------------------------------------------------------------------------
INT32 awe_fwExecuteFlashFiles(struct _AWEInstance* pAWE, AWEFlashFSInstance* pAWEFlashFSInstance);

//-----------------------------------------------------------------------------
// METHOD:	awe_fwFlashFileSystemCommandHandler
// PURPOSE: Handler to execute flash file system related commands
//-----------------------------------------------------------------------------
INT32 awe_fwFlashFileSystemCommandHandler(struct _AWEInstance* pAWE);

/**
 * @brief Copy the file system info to the caller.
 * @param [out] pFileSystemInfo		buffer to receive file system info
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetFileSystemInfo(struct _AWEFlashFSInstance * pFS, FileSystemInfo *pFileSystemInfo);

/**
 * @brief Convert a file to one in the path as needed
 * @param [out] outFile				output buffer
 * @param [in] outFileSize			size of output buffer
 * @param [in] file					file to check
 * @return							1 if exists with outFile containing path, 2 if directory with outFile containing path, else 0 if not found
 */
INT32 awe_fwFindFileInPath(char *outFile, UINT32 outFileSize, const char *file);

#ifdef __cplusplus
}
#endif

#endif	/* _FW_FLASHFSINSTANCE_H */
