/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/
/****************************************************
 * @file Port.h
 * 
 * @ingroup Port
 *
 * @brief Implementation of the Class Port       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port__INCLUDED_)
#define BC_Port__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "JModule_Bitfield.h"
#include "Port_Error.h"

#include "Port_Data.h"
#include "Port_Direction.h"
#include "Port_Error.h"


/**
 * @ingroup Port
 *
 * @brief Data structure of Port properties and helper APIs. A port is connected to a sample buffer, but the
 * pointer to the buffer is managed by a Port_Context associated with a JModule instance. The same
 * buffer can be shared by multiple ports, allowing for buffer reuse or for ports to be connected
 * within a single threaded processes execution.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Port
{
	/**
	 * @ingroup Port
	 *
	 * @brief Data type of the samples in the Port buffer . Valid values are one of the Port_Data enumeration
	 * values
	 */
	Port_Data data_type;

	/**
	 * @ingroup Port
	 *
	 * @brief Specify if this is an input or output port
	 */
	Port_Direction direction;

	/**
	 * @ingroup Port
	 *
	 * @brief Size, in samples, of the processing frame.
	 */
	uint32_t frame_size;

	/**
	 * @ingroup Port
	 *
	 * @brief Number of channels in the sample buffer
	 */
	uint32_t num_chan;

	/**
	 * @ingroup Port
	 *
	 * @brief Sample rate, in Hz
	 */
	uint32_t sample_rate;

	/**
	 * @ingroup Port
	 *
	 * @brief Sample size, in bytes
	 */
	uint32_t sample_size;

	/**
	 * @ingroup Port
	 *
	 * @brief Buffer frame size, in bytes = frame_size*num_chan*sample_size. It is provided as an optimization
	 * convenience, for avoiding recalculating the buffer frame size.
	 */
	uint32_t block_size;

	/**
	 * @ingroup Port
	 *
	 * @brief The id plus one of the input port in the same process that can be used for pass thru operations to
	 * the current output port. Also the current input/output port will copy the parameters the
	 * output/input port in the same process, with the id specified. A value of zero means no pass thru or
	 * parameter copy port is specified.
	 */
	uint32_t passthru_port;
} Port;

/**
 * @ingroup Port
 *
 * @brief Helper API for comparing if two ports are compatible, so they can be connected.
 * 
 * @param this Pointer to the current Port
 * 
 * @param p Pointer to the Port to compare with the current Port
 */
Port_Error Port_compare(Port* this, Port* p);

/**
 * @ingroup Port
 *
 * @brief Set the port properties
 * 
 * @param this Poiter to the current Port
 * 
 * @param frame_size Frame Size, in samples
 * 
 * @param num_chan Number of channels in the Port
 * 
 * @param sample_size Sample Size, in bytes
 * 
 * @param sample_rate Sampling Rate in Hz
 * 
 * @param data_type Data Type
 */
bool Port_setup(Port* this, uint32_t frame_size, uint32_t num_chan, uint32_t sample_size, uint32_t sample_rate, Port_Data data_type);

/**
 * @ingroup Port
 *
 * @brief API for computing the frame size in bytes = frame_size*num_chan*sample_size.
 * 
 * @param this Pointer to the current port
 */
uint32_t Port_block_size(Port* this); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port__INCLUDED_)*/
