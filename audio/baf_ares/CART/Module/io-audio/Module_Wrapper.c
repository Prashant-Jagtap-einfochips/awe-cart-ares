/****************************************************
 *  Module_Wrapper.c                                         
 *  Created on: 03-May-2023 8:55:11 AM                      
 *  Example BAF API usage within a APX module.       
 *  Original author: st1055616                     
 ****************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdarg.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

#include "BAF_Message_Id.h"
#include "CART_Message.h"
#include "BAF.h"
#include "Error_Id.h"
#include "Module_Wrapper.h"

// #define VERBOSE_LOGGING
// #define ENABLE_MODULE_IPC

#define LOG_ID_STRING "lib_bose: "

#ifdef VERBOSE_LOGGING
const char *build_str = LOG_ID_STRING "Built: "  " " __DATE__ " " __TIME__;
#else
const char *build_str = LOG_ID_STRING;
#endif

#if !defined(PLATFORM_BASETHREADLEVEL)
#define PLATFORM_BASETHREADLEVEL 0
#endif

// Store response locally to return with GetData() as PipeData API is removed.
#define MAX_RESPONSE_SIZE 512
// Define the maximum number of threads. Autogenerating this will be read from 
// model. 
#define PLATFORM_NUM_AUDIOTHREADS      (12)
#define PLATFORM_NUM_BACKGROUND_TASKS  (10)

/* ---- Typedefs -----------------------------------------------------------------------*/

// Module configuration structure, e.g. read from filesystem
typedef struct
{
    chconfig_t const * ChanConfig;               /**< Input/Output Frame length in samples */
    unsigned int uSamplesPerFrame;               /**< Input/Output Frame length in samples */
    unsigned int uInternalBufLen;                /**< Internal per channel buffer length in samples */
    uint32_t     uSampleRate;                    /**< Sample rate (Hz) */
} module_config_t;


/* ---- Channel Map Definitions --------------------------------------------------------*/

// In this made up example, we support these channel configurations:
//  2in, 2out (2 variations)
//  2in, 4out
static snd_pcm_chmap_t const chmap_stereo    = { .channels = 2, .pos = { SND_CHMAP_FL, SND_CHMAP_FR } };
static snd_pcm_chmap_t const chmap_mono_lfe  = { .channels = 2, .pos = { SND_CHMAP_MONO, SND_CHMAP_LFE } };
static snd_pcm_chmap_t const chmap_3ch       = { .channels = 3, .pos = { SND_CHMAP_FL, SND_CHMAP_FR, SND_CHMAP_RL } };
static snd_pcm_chmap_t const chmap_4ch       = { .channels = 4, .pos = { SND_CHMAP_FL, SND_CHMAP_FR, SND_CHMAP_RL, SND_CHMAP_RR } };

static chconfig_t const chconfig_2in2out_a = { .src_chmap = &chmap_stereo, .dst_chmap = &chmap_stereo };
static chconfig_t const chconfig_2in2out_b = { .src_chmap = &chmap_mono_lfe, .dst_chmap = &chmap_mono_lfe };
static chconfig_t const chconfig_3in4out   = { .src_chmap = &chmap_3ch, .dst_chmap = &chmap_4ch };
static chconfig_t const chconfig_2in4out   = { .src_chmap = &chmap_stereo, .dst_chmap = &chmap_4ch };

static chconfig_t const * const chconfig_list[] = { &chconfig_3in4out, NULL };


static const module_config_t default_config = 
{
    .ChanConfig = &chconfig_3in4out, // Change the GetSupportedChannelConfigs accordingly.
    .uSamplesPerFrame = 32,
    .uInternalBufLen = 0, 
    .uSampleRate = 48000,
};

// Structure of allocation functions to use
// - defaults to the C standard library functions
static module_allocator_t g_allocator =
{
    .malloc        = malloc,
    .realloc       = realloc,
    .calloc        = calloc,
    .aligned_alloc = NULL, // aligned_alloc is supposed to be in malloc.h but does not compile.
    .free          = free,
};

#define HEAP_SIZE 0x4000

typedef void (*Platform_Func_audio_thread_handler)(void *arg, uint32_t thread_level);

// Module to wrap BAAS.
typedef struct
{
    // Memory for Bose algorithm internal usage. 
    uint32_t heap_buffer[HEAP_SIZE/sizeof(uint32_t)];
  
    uint32_t baf_enable;

    BAF* baf;

    Platform_Func_audio_thread_handler audio_thread_handler;

    uint32_t channel_offset;

#ifdef VERBOSE_LOGGING
    uint32_t cnt;
#endif

    uint8_t response[MAX_RESPONSE_SIZE];
    
    uint8_t isResponseBufferValid;

} module_baas_t;

// Module instance struct
typedef struct
{
    bool bCreated;

    bool bInited;

    float *pInputBuffer;

    float *pOutputBuffer;

    module_config_t Config;

    module_baas_t baas;
} module_t;


/* ---- Function Declarations ----------------------------------------------------------*/

static int16_t Deinitialize( void * hHandle );
static int16_t Reset( void * hHandle );

int BAF_printf(char* fmt, ...)
{
    va_list args;
    int val;

    va_start(args, fmt);
    val = printf(fmt, args);
    va_end(args);

    return val;
}

void notification_handler(BAF* baf, BAF_Message *message)
{
   uint32_t * data = message->data;

   BAF_printf("\n  !!: Got msg reply notification %x %x %x %x. \n", message->id, message->size, data[0], data[1]);
}


void BAF_Callback_post(uint32_t thread_level, void* arg) 
{
}


uint32_t BAF_Callback_clock(void) 
{
    return 0;
}

void  BAF_Callback_sys_process_ctx(uint32_t proc_Id, uint32_t frame_size,
      uint32_t num_chan_in, uint32_t num_chan_out, void *src, void *dst, void* arg) 
{
    module_t* me = (module_t*)arg;

    uint32_t channel_data_size = me->Config.uSamplesPerFrame * sizeof(float);

    // Memcpy not just samples for channel, but all channels at the same time.            
    uint32_t src_size =  me->Config.ChanConfig->src_chmap->channels * channel_data_size;;

    const uint32_t dst_size = (me->Config.ChanConfig->dst_chmap->channels - me->baas.channel_offset) * channel_data_size;

    switch (proc_Id)
    {
        // Handle audio input.
		case 0:
            // Expect caller to provide scaled or normalized data. Pass it to the processing model.
            memcpy(dst, (void *)me->pInputBuffer, src_size);

            break;

        // Handle audio output.
        case 3:     
            // Copy the data from the algorithm buffer to the context buffer pointer, which is the caller/io-audio buffer.
            memcpy((void *)me->pOutputBuffer, src, dst_size);

            break;
        
        // Handle audio output.
        case 1:
            uint8_t* dst = (uint8_t*)me->pOutputBuffer;
            dst += dst_size;

            // Copy the data from the algorithm buffer to the context buffer pointer, which is the caller/io-audio buffer.
            memcpy((void *)dst, src, dst_size);

            break;

        default:
            printf("BAF_Callback_sys_process_ctx unexpected ID  %u", proc_Id);
            break;
    }
}

/* ---- Function Definitions -----------------------------------------------------------*/

/** Retrieve version information.
 *
 *  This function is used to retrieve library version information.  An instance of the
 *  library does not have to exist, i.e. you can call qwaSfoGetVersion() before qwaSfoCreate().
 *  If any parameter is NULL, the corresponding data is not retrieved.
 *
 *  @ingroup SFOAPI
 *
 *  @param   pusVersionArray   Version array.
 *  @param   pszVersionString  Version string
 *                             (the same information as psVersionArray but in string format).
 *  @param   pszVersionComment Optional version comment string.
 */
/*======================================================================================*/
static void GetVersion( const char ** pszVersionString )
{
   if (NULL != pszVersionString)
   {
      *pszVersionString = "APX Module 0.0.20000.0";
   }
}

/** Specify the allocator that the module should use
 *
 *  This should be called prior to Create()
 *
 *  @ingroup SFOAPI
 *
 *  @param   allocator   Structure allocator functions to use
 */
/*======================================================================================*/
static int16_t SetAllocator( module_allocator_t* allocator )
{
    if ( (allocator == NULL) ||
         (allocator->malloc == NULL) ||
         (allocator->realloc == NULL) ||
         (allocator->calloc == NULL) ||
         (allocator->aligned_alloc == NULL) ||
         (allocator->free == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    g_allocator.malloc        = allocator->malloc;
    g_allocator.realloc       = allocator->realloc;
    g_allocator.calloc        = allocator->calloc;
    g_allocator.aligned_alloc = allocator->aligned_alloc;
    g_allocator.free          = allocator->free;

    return ERR_OK;
}


/** Library constructor.  Creates instance of the library.  First stage of the two-stage initialization of the library
 *
 *  @param   phHandle      Pointer to handle which will be created by this function.
 *                         Must not be NULL
 *
 *  @return  Result status
 *  @retval  #ERR_OK                      No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Create( void ** phHandle )
{ 
    BAF_Resource *resource;

    // We will be storing BAF/CART context in here. Make sure it is not NULL.
    if (phHandle == NULL)
    {
        return ERR_NULL_POINTER;
    }

    *phHandle = g_allocator.malloc(sizeof(module_t));
    if (*phHandle == NULL)
    {
        printf("Error: malloc failure\n");
        return ERR_ALLOC;
    }

    // Initialize members
    module_t *pModule = (module_t*)(*phHandle);
    pModule->bCreated = true;
    pModule->bInited = false;

    pModule->baas.baf = BAF_create(NULL);

    resource = BAF_resource(pModule->baas.baf);

    if (resource->memory_size < HEAP_SIZE) 
    {
        resource->memory_pointer = pModule->baas.heap_buffer;

        // Remove all code generated for logging in runtime.
        resource->memory_log_pointer = NULL;

        resource->base_thread = PLATFORM_BASETHREADLEVEL;
    } 
    else 
    {
        printf("Error: BAF not enough memory req %d supplied %d\n", 
                resource->memory_size, HEAP_SIZE);

        return ERR_ALLOC;
    }

    printf("Shared library version %s, size %u threads %u.\n",
           BAF_version(pModule->baas.baf),
           resource->memory_size,
           resource->num_threads);

    return ERR_OK;
}

/** Library destructor
 *
 *  This function destroys the instance of the library.
 *
 *  @ingroup SFOAPI
 *
 *  @param   phHandle      Pointer to the handle created in Create().
 *                         Calling this function with @a *phSfo set to NULL
 *                         is possible without causing an error.
 *
 *  @return  Result status
 *  @retval  #ERR_OK                         No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Destroy( void ** phHandle )
{
    if ((phHandle != NULL) && (*phHandle != NULL))
    {
        module_t* pModule = (module_t*)(*phHandle);

        if( pModule->bCreated )
        {
            if( pModule->bInited ) 
            {
                Deinitialize( *phHandle );
            }

            pModule->bCreated = false;

            g_allocator.free( (void*)pModule );
        }

        pModule = NULL;
    }

    printf("Shared Ojbect destroyed(). \n");

    return ERR_OK;
}


/** Library initialization.  Final stage of the two-stage initialization of the library
 *
 *  @param   hHandle       Handle created via Create().
 *  @param   pConfig       Pointer to configuration structure.
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Initialize( void * hHandle, const void * pConfig )
{
    int32_t error;

    if (hHandle == NULL)
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);

    if (!pModule->bCreated)
    {
        return ERR_NOT_CREATED;
    }
    if (pModule->bInited)
    {
        return ERR_ALREADY_INIT;
    }

    error = BAF_init(pModule->baas.baf);

    if (error) 
    {
        printf("Error: init failure %08X\n", error);
        return ERR_NOT_INIT;
    }

    BAF_register_notify((void*)(pModule->baas.baf), notification_handler);

    pModule->baas.baf_enable = 1;

    // Save config params
    if (pConfig == NULL)
    {
        pModule->Config = default_config;
    }
    else
    {
        pModule->Config = *(module_config_t*)(pConfig);
    }

    pModule->baas.channel_offset = 2;

    pModule->bInited = true;

#ifdef VERBOSE_LOGGING
    pModule->baas.cnt = 0;

    printf("Init complete.");
#endif 
    return ERR_OK;
}

/** Library reset to initial conditions of current initialization configuration
 *  i.e. resets internal buffers to zeroes
 *
 *  @param   hHandle       Handle created via Create() and initialized via Initialize().
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Reset( void * hHandle )
{ 
    return ERR_OK;
}

/** Library deinitialization.
 *
 *  @param   hHandle       Handle created via Create().
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Deinitialize( void * hHandle )
{
#ifdef VERBOSE_LOGGING
    printf("Deinitialize() begin.\n");
#endif

    int retVal = EOK;

    if (hHandle != NULL)
    {
        module_t* pModule = (module_t*)(hHandle);

        if( pModule->bInited )
        {
            pModule->bInited = false;
        }
    }

#ifdef VERBOSE_LOGGING
    printf("Deinitialize(). return value %u.\n", retVal);
#endif

    return retVal;
}


/** Get supported PCM sample rates
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   pulRates           OUT: Supported sample rates as bitmask of SND_PCM_RATE_*
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSupportedPcmRates( void * hHandle, uint32_t * pulRates )
{
    if ( (hHandle == NULL) || (pulRates == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    // Above checks not really needed if supported sample rates is independent of configuration

    *pulRates = SND_PCM_RATE_48000;
    return ERR_OK;
}

/** Get supported PCM sample formats
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   pulFormats         OUT: Supported sample formats as bitmask of SND_PCM_FMT_*
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSupportedPcmFormats( void * hHandle, uint32_t * pulFormats )
{
    if ( (hHandle == NULL) || (pulFormats == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    // Above checks not really needed if supported sample formats is independent of configuration

    *pulFormats = SND_PCM_FMT_FLOAT;
    return ERR_OK;
}

/** Get supported PCM channel info
 *  - currently only checked for SND_PCM_CHNINFO_INTERLEAVE and SND_PCM_CHNINFO_NONINTERLEAVE
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   pulChannelInfo     OUT: Supported channel info as bitmask of SND_PCM_CHNINFO_*
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSupportedPcmChannelInfo( void * hHandle, uint32_t * pulChannelInfo )
{
    if ( (hHandle == NULL) || (pulChannelInfo == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    // Above checks not really needed if supported channel info is independent of configuration

    *pulChannelInfo = SND_PCM_CHNINFO_NONINTERLEAVE;
    return ERR_OK;
}


/** Get supported channel configurations
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   ppaChConfigs       OUT: Pointer to array of supported channel configs (pairings of source-input channel map & dest-output channel map)
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSupportedChannelConfigs( void * hHandle, chconfig_t const * const * * ppaChConfigs )
{
    if ( (hHandle == NULL) || (ppaChConfigs == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    *ppaChConfigs = chconfig_list;

    return ERR_OK;
}

/** Process frame of audio data
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   in_data_ptr        IN: Frame of input samples in non-interleaved order.
 *  @param   out_data_ptr       OUT: Frame of output samples in non-interleaved order.
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t Process( void * hHandle, float * pfInData, float * pfOutData )
{
    if ((NULL == pfInData) || (NULL == pfOutData) || (NULL == hHandle))
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);

    if (!pModule->bCreated)
    {
        return ERR_NOT_CREATED;
    }

    if (!pModule->bInited)
    {
        return ERR_NOT_INIT;
    }
    
    if (0 == pModule->baas.baf_enable)
    {
        return ERR_NOT_INIT;
    }

    pModule->pInputBuffer = pfInData;

    pModule->pOutputBuffer = pfOutData;

#ifdef VERBOSE_LOGGING
    pModule->baas.cnt++;

    // printf("        Process %u.\n", pModule->baas.cnt);
#endif

    BAF_exec(pModule->baas.baf, PLATFORM_BASETHREADLEVEL, pModule);

    return ERR_OK;
}

/** Data getter method
 *
 *  This function passes data from the module to the caller.
 *  - the interpretation of dataId and channel is at the discretion of the module and caller.
 *
 *  @param   hHandle        Handle created via Create() and initialized via Initialize().
 *  @param   dataId         IN:  Identifier for the data get request
 *  @param   channel        IN:  Identifier for the data get request
 *  @param   ulSize         IN:  Caller's expected size of the data request
 *                          OUT: Module's actual size of the data request
 *  @param   pData          OUT: Pointer to memory buffer to be filled by module (if not NULL).
 *                          Calling with pData=NULL, will query the required data size of the request
 *
 *  @return  Result status
 *  @retval  #ERR_OK        No error.
 *  ...
 */
static int16_t GetData( void * hHandle, int dataId, int channel, size_t * ulSize, void * pData )
{
    int16_t ret = ERR_OK;

    //printf("GetData -----------------------------------------\n");

    printf( " <<<<<<<<< Entered:  GetData 0x%p, %lu, 0x%p", hHandle, *ulSize, pData );

    if ( hHandle == NULL )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)hHandle;
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    CART_Phy_Message* storedResponse = (CART_Phy_Message*)(pModule->baas.response);

    const uint32_t respSize = CART_Message_get_size(storedResponse);

    if (respSize + CART_MESSAGE_HEADER_BYTES > *ulSize)
    {
        printf("Resp payload sz %u header %u too big for buffer.\n", CART_Message_get_size(storedResponse), CART_MESSAGE_HEADER_BYTES);

        ret = ERR_INVALID_SIZE;
    }
    else
    {
        CART_Message* rsp = (CART_Message*)pData;

        rsp->data = (uint32_t*)((uint8_t*)pData + CART_MESSAGE_HEADER_BYTES);

        CART_Message_copy_header(rsp, storedResponse);
        
        memcpy((void*)(rsp->data),  storedResponse->data, respSize);

#ifdef VERBOSE_LOGGING
            if (0 != respSize)
            {
                printf("Resp payload sz %u header %u.\n", respSize, CART_MESSAGE_HEADER_BYTES);

                printf("%08x ", CART_Message_get_id(&rsp));
                printf("%08x ", respSize);

                for (int i = CART_MESSAGE_HEADER_BYTES; i < CART_MESSAGE_HEADER_BYTES + respSize; i++)
                {
                    printf("%02x ", pData[CART_MESSAGE_HEADER_BYTES + i]);
                }
                printf("\n");
            }
#endif  //  VERBOSE_LOGGING
    }
    return ret;
}

/** Data setter method
 *
 *  This function passes data from the caller to the module.
 *  - the interpretation of dataId and channel is at the discretion of the module and caller.
 *
 *  @param   hHandle        Handle created via Create() and initialized via Initialize().
 *  @param   dataId         IN:  Identifier for the data set request
 *  @param   channel        IN:  Identifier for the data set request
 *  @param   ulSize         IN:  Size of the data being passed
 *  @param   pData          IN:  Pointer to memory buffer of data
 *
 *  @return  Result status
 *  @retval  #ERR_OK        No error.
 *  ...
 */
static int16_t SetData( void * hHandle, int dataId, int channel, size_t ulSize, void const * pData )
{
    
    int16_t ret = ERR_OK;

    printf( " >>>>>>> Entered: SetData 0x%p, %lu, 0x%p", hHandle, ulSize, pData );

    if ( (hHandle == NULL) || (pData == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)hHandle;
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }


    if (ulSize < CART_MESSAGE_HEADER_BYTES)
    {
        printf("Error: msg buffer allocated too small -  %ld bytes.\n", ulSize);

        return ERR_INVALID_SIZE;
    }

#ifdef ENABLE_MODULE_IPC
    // Serialize access to command processing APIs.
    while (sem_wait(&(pModule->baas.pipeDataLock)) && (errno == EINTR))
    {
    }
#endif

    // Subtract the message ID size to get the rest of the payload size.
    // Use this for to prevent response buffer overflow: ulSize = ulSize - 2;
    //uint8_t* ioBuffer = (uint8_t*)pData;

    pModule->baas.isResponseBufferValid = 0;

    BAF *baf = pModule->baas.baf;

    BAF_Message baf_message_req;

    CART_Message_initialize(&baf_message_req, (CART_Phy_Message*)pData);

    // Response buffer payload size
    uint32_t responseOffset = CART_MESSAGE_HEADER_BYTES + CART_Message_get_size(&baf_message_req);

    // The expected size is sum of payload and header. So, reinitialize it.
    // Talaria seems to expect a minimum size of 32 bits (atleast msg ID must be present).
    CART_Message_set_size(&baf_message_req, responseOffset);

    BAF_Message baf_message_rsp;

    CART_Message_initialize(&baf_message_rsp, (CART_Phy_Message*)(pModule->baas.response));

    // Set the available buffer size.
    CART_Message_set_size(&baf_message_rsp, MAX_RESPONSE_SIZE - CART_MESSAGE_HEADER_BYTES);

#ifdef VERBOSE_LOGGING
    if (0 != responseOffset)
    {
        for (int i = 0; i < responseOffset; i++)
        {
            printf("%x ", ioBuffer[i]);
        } 
        printf(" msg addr %x. \n", pModule->baas.cmdMsg);
    }
#endif  //  VERBOSE_LOGGING

    BAF_command(baf, &baf_message_req, &baf_message_rsp);

    uint32_t outsize = CART_Message_get_size(&baf_message_rsp);

    int16_t retVal = ERR_OK;

    if ((CART_MESSAGE_HEADER_BYTES + outsize) > MAX_RESPONSE_SIZE)
    {
        printf("Response size + CART_MESSAGE_HEADER_BYTES greater than buffer size. Potential memory corruption. \n");

        retVal = ERR_INVALID_SIZE;
    }
    else
    {
        // A valid response is stored. Retrieve with GetData().
        pModule->baas.isResponseBufferValid = 1;

        CART_Message_copy_header((CART_Phy_Message*)(pModule->baas.response), &baf_message_rsp);
    }

#ifdef ENABLE_MODULE_IPC
    int value = 0;

    if (EOK != (value = sem_post(&(pModule->baas.pipeDataLock))))
    {
        printf(" PipeDataLock sem_post failed. Return value %x. %s.\n",
                value, strerror(errno));
    }
#endif
    return ret;
}


/** Get sample rate
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   puSampleRate       OUT: Sample rate (Hz)
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSampleRate( void * hHandle, uint32_t * puSampleRate )
{
    if ( (hHandle == NULL) || (puSampleRate == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    *puSampleRate = pModule->Config.uSampleRate;
    return ERR_OK;
}

/** Get number of samples per frame (per channel)
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   puNumSamples       OUT: Number of samples per frame
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetSamplesPerFrame( void * hHandle, unsigned int * puNumSamples )
{
    if ( (hHandle == NULL) || (puNumSamples == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    *puNumSamples = pModule->Config.uSamplesPerFrame;
    return ERR_OK;
}

/** Get number of input and output channels
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   puNumChanIn        OUT: Number of input channels
 *  @param   puNumChanOut       OUT: Number of output channels
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetChannelCounts( void * hHandle, unsigned int * puNumChanIn, unsigned int * puNumChanOut )
{
    if ( (hHandle == NULL) || (puNumChanIn == NULL) || (puNumChanOut == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    *puNumChanIn  = pModule->Config.ChanConfig->src_chmap->channels;
    *puNumChanOut = pModule->Config.ChanConfig->dst_chmap->channels;
    return ERR_OK;
}

/** Get processing delay in samples
 *
 *  @param   hHandle            Handle created via Create() and initialized via Initialize().
 *  @param   puProcessingDelay  OUT: Processing delay in samples
 *
 *  @return  Result status
 *  @retval  #ERR_OK                        No error.
 *  ...
 */
/*======================================================================================*/
static int16_t GetProcessingDelay( void * hHandle, unsigned int * puProcessingDelay )
{
    if ( (hHandle == NULL) || (puProcessingDelay == NULL) )
    {
        return ERR_NULL_POINTER;
    }

    module_t* pModule = (module_t*)(hHandle);
    if ( !pModule->bCreated )
    {
        return ERR_NOT_CREATED;
    }
    if ( !pModule->bInited )
    {
        return ERR_NOT_INIT;
    }

    // Delay, if present, could be: pModule->Config.uInternalBufLen - pModule->Config.uSamplesPerFrame
    *puProcessingDelay = 0; // pModule->Config.uProcessingDelay;

    return ERR_OK;
}


/** Get binary config data corresponding to the requested parameters
 *
 *  In practice, this method is probably not needed as binary config data will 
 *  most likely be stored and loaded from the target file system.
 *
 *  The passed back data is valid until the next call to this method.
 *  i.e. subsequent calls to this method will overwrite previous results.
 *
 *  NOTE: this should be consistent with GetSupportedChannelConfigs() above
 *
 *  @param   sample_rate   
 *  @param   num_channels_in 
 *  @param   num_channels_out 
 *  @param   ppConfig           OUT: Pointer to binary config data with requested paramss
 *
 *  @return  Result status
 *  @retval  #ERR_OK                            No error.
 *  ...
 */
/*======================================================================================*/
int16_t GetConfig( uint32_t sample_rate, snd_pcm_chmap_t const * const src_chmap, snd_pcm_chmap_t const * const dst_chmap, void const * * ppConfig)
{
    static module_config_t config; 

    config = default_config;    

    if ( dst_chmap->channels == 4 ) 
    {
        config.ChanConfig = & chconfig_3in4out;
    }

    config.uSampleRate = sample_rate;

    *ppConfig = &config;

    return ERR_OK;
}


/* ---- Externals ---------------------------------------------------------------------*/

const module_funcs_t module_funcs =
{
    .get_version = GetVersion,
    .set_allocator = SetAllocator,
    .create = Create,
    .destroy = Destroy,
    .initialize = Initialize,
    .reset = Reset,
    .deinitialize = Deinitialize,
    .get_supported_pcm_rates = GetSupportedPcmRates,
    .get_supported_pcm_formats = GetSupportedPcmFormats,
    .get_supported_pcm_channel_info = GetSupportedPcmChannelInfo,
    .get_supported_channel_configs = GetSupportedChannelConfigs,
    .get_sample_rate = GetSampleRate,
    .get_samples_per_frame = GetSamplesPerFrame,
    .get_channel_counts = GetChannelCounts,
    .get_processing_delay = GetProcessingDelay,
    .process = Process,
    .get_data = GetData,
    .set_data = SetData,
    .get_config = GetConfig,
};
