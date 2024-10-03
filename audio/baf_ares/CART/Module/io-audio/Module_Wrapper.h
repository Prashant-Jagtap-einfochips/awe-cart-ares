

#ifndef MODULE_WRAPPER_H
#define MODULE_WRAPPER_H

/*----------------------------------------------------------------------------*/

// Required for definition of int16_t datatype.
#include "inttypes.h"
// Required for definition of snd_pcm_chmap_t needed for the API below.
#include <sys/asound.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    void* (*malloc)( size_t size );
    void* (*realloc)( void * old_blk, size_t size );
    void* (*calloc)( size_t n, size_t size );
    void* (*aligned_alloc)( size_t alignment, size_t size );
    void  (*free)( void * ptr );
} module_allocator_t;

// Source (input) and destination (output) chmap pairing
// - describes a compatible input & output configuration
// - use .channels = 0 to indicate "any number of channels"
// - use .pos[i] = SND_CHMAP_UNKNOWN to indicate "any channel position"
typedef struct
{
    snd_pcm_chmap_t const * src_chmap;
    snd_pcm_chmap_t const * dst_chmap;
} chconfig_t;

/** Function list exported by dll module. */
typedef struct
{
    // Module info
    void (*get_version)( const char ** pszVersionString );

    // Specify the allocator for the module to use
    int16_t (*set_allocator)( module_allocator_t * allocator );

    // Module lifetime
    int16_t (*create)( void ** handle );
    int16_t (*destroy)( void ** handle );
    int16_t (*initialize)( void * handle, const void * config);
    int16_t (*reset)( void * handle);
    int16_t (*deinitialize)( void * handle );

    // Capabilities queries
    // - get range of capabilities supported by module
    // - these queries may be independent of a module instance
    int16_t (*get_supported_pcm_rates)( void * handle, uint32_t * pcm_rates );
    int16_t (*get_supported_pcm_formats)( void * handle, uint32_t * pcm_formats );
    int16_t (*get_supported_pcm_channel_info)( void * handle, uint32_t * pcm_channel_info );
    int16_t (*get_supported_channel_configs)( void * handle, chconfig_t const * const * * channel_configs );

    // Audio param queries
    // - get parameter values for a specific module instance
    int16_t (*get_sample_rate)( void * handle, uint32_t * sample_rate );
    int16_t (*get_samples_per_frame)( void * handle, unsigned int * num_samples_per_frame );
    int16_t (*get_channel_counts)( void * handle, unsigned int * num_chan_in, unsigned int * num_chan_out );
    int16_t (*get_processing_delay)( void * handle, unsigned int * delay );

    // Process frame of audio
    // - in this example, processing is done with separate input and output buffers
    int16_t (*process)( void * handle, float * in_data_ptr, float * out_data_ptr );

    // This is mapped to the libasound method: snd_pcm_get_apx_data(snd_pcm_t *pcm, uint32_t apx_id, snd_pcm_apx_param_t *param, void *data);
    //      param->size     ? size
    //      param->dataId   ? data_id
    //      param->channel  ? channel
    //      param->status   ? <return_value>
    //      data            ? data
    int16_t (*get_data)( void * handle, int data_id, int channel, size_t * size, void * data );
    // This is mapped to the libasound method: snd_pcm_set_apx_data(snd_pcm_t *pcm, uint32_t apx_id, snd_pcm_apx_param_t *param, const void *data);
    //      param->size     ? size
    //      param->dataId   ? data_id
    //      param->channel  ? channel
    //      param->status   ? <return_value>
    //      data            ? data
    int16_t (*set_data)( void * handle, int data_id, int channel, size_t size, void const * data );

    // Helpers
    int16_t (*get_config)( uint32_t sample_rate, snd_pcm_chmap_t const * const src_chmap, snd_pcm_chmap_t const * const dst_chmap, void const * * ppConfig);
} module_funcs_t;

extern const module_funcs_t module_funcs;

/** Error codes */
#define ERR_OK                            (0x0000)
#define ERR_ALLOC                         (0x0100)
#define ERR_NULL_POINTER                  (0x0101)
#define ERR_NOT_INIT                      (0x0102)
#define ERR_NOT_CREATED                   (0x0103)
#define ERR_ALREADY_INIT                  (0x0104)
#define ERR_INVALID_ID                    (0x010B)
#define ERR_INVALID_SIZE                  (0x010E)
#define ERR_PARAM                         (0x010F)
#define ERR_INVALID_CHANNEL_CNT           (0x0113)

// Data IDs
typedef enum
{
    E_DATA_ID_CHAN_CNT_IN,
    E_DATA_ID_CHAN_CNT_OUT,
    E_DATA_ID_SAMPLES_PER_FRAME,
    E_DATA_ID_PROCESSING_DELAY,
} data_id_t;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef MODULE_WRAPPER_H */

