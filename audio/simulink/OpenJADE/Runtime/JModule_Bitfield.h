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
 * @file JModule_Bitfield.h
 * 
 * @ingroup JModule_Bitfield
 *
 * @brief Implementation of the Enumeration JModule_Bitfield       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_Bitfield__INCLUDED_)
#define BC_JModule_Bitfield__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup JModule_Bitfield
 *
 * @brief Enumeration and APIs to set/get JModule IDs as bitfields in a 32bit word
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup JModule_Bitfield
	 *
	 * @brief Number of bits used for the JModule ID
	 */
	JMODULE_NUM_BIT = 8,

	/**
	 * @ingroup JModule_Bitfield
	 *
	 * @brief Number of bits used for the Process ID
	 */
	PROCESS_NUM_BIT = 4,

	/**
	 * @ingroup JModule_Bitfield
	 *
	 * @brief Number of bits used for the Port ID
	 */
	PORT_NUM_BIT = 4,

	/**
	 * @ingroup JModule_Bitfield
	 *
	 * @brief Number of bits used for the JModule Context ID
	 */
	CONTEXT_NUM_BIT = 8,

	/**
	 * @ingroup JModule_Bitfield
	 *
	 * @brief Number of bits used for specifying the port direction
	 */
	DIRECTION_NUM_BIT = 1
} JModule_Bitfield;
#define MODULE_NUM_BIT (JMODULE_NUM_BIT+CONTEXT_NUM_BIT)
typedef uint32_t Buffer_Id;

typedef uint32_t Port_Id;

typedef uint32_t Module_Id;

typedef uint32_t Process_Id;

typedef uint32_t Context_Id;

#define CONTEXT_NONE 0
#define CONTEXT_0 1
#define CONTEXT_1 2
#define CONTEXT_2 3
#define CONTEXT_3 4
#define CONTEXT_4 5
#define CONTEXT_5 6
#define CONTEXT_6 7
#define CONTEXT_7 8

#define PORT_0 0
#define PORT_1 1
#define PORT_2 2 
#define PORT_3 3
#define PORT_4 4
#define PORT_5 5
#define PORT_6 6
#define PORT_7 7
#define PORT_8 8
#define PORT_9 9
#define PORT_10 10
#define PORT_11 11
#define PORT_12 12
#define PORT_13 13
#define PORT_14 14
#define PORT_15 15

#define PROCESS_0  	0
#define PROCESS_1  	1
#define PROCESS_2  	2 
#define PROCESS_3  	3
#define PROCESS_4  	4
#define PROCESS_5  	5
#define PROCESS_6  	6
#define PROCESS_7  	7
#define PROCESS_8  	8
#define PROCESS_9  	9
#define PROCESS_10 10
#define PROCESS_11 11
#define PROCESS_12 12
#define PROCESS_13 13
#define PROCESS_14 14
#define PROCESS_15 15

#define BUFFER_0 0
#define BUFFER_1 1
#define BUFFER_2 2 
#define BUFFER_3 3
#define BUFFER_4 4
#define BUFFER_5 5
#define BUFFER_6 6
#define BUFFER_7 7
#define BUFFER_8 8
#define BUFFER_9 9
#define BUFFER_10 10
#define BUFFER_11 11
#define BUFFER_12 12
#define BUFFER_13 13
#define BUFFER_14 14
#define BUFFER_15 15
#define BUFFER_16 16
#define BUFFER_17 17
#define BUFFER_18 18
#define BUFFER_19 19
#define BUFFER_20 20
#define BUFFER_21 21
#define BUFFER_22 22
#define BUFFER_23 23


#define DIRECTION_POS_BIT (PORT_NUM_BIT+PROCESS_NUM_BIT+JMODULE_NUM_BIT+CONTEXT_NUM_BIT)
#define MODULE_POS_BIT (PORT_NUM_BIT+PROCESS_NUM_BIT)
#define CONTEXT_POS_BIT (PORT_NUM_BIT+PROCESS_NUM_BIT+JMODULE_NUM_BIT)
#define JMODULE_POS_BIT (PORT_NUM_BIT+PROCESS_NUM_BIT)
#define PROCESS_POS_BIT PROCESS_NUM_BIT
#define PORT_POS_BIT 0

#define JModule_Bitfield_port(_direction, _port, _process, _feature, _context) ( \
					((_direction) << DIRECTION_POS_BIT) | \
					((_feature)   << JMODULE_POS_BIT) | \
                    ((_process)   << PROCESS_NUM_BIT) | \
                    ((_port)      << PORT_POS_BIT) | \
				    ((_context)   << CONTEXT_POS_BIT) \
				) 

#define JModule_Bitfield_module(_feature, _context) ( \
					((_feature) << JMODULE_POS_BIT) | \
					((_context) << CONTEXT_POS_BIT)   )

#define JModule_Bitfield_process(_process, _feature, _context)  ( \
					((_feature) << JMODULE_POS_BIT) | \
                    ((_process) << PROCESS_POS_BIT) | \
				    ((_context) << CONTEXT_POS_BIT) \
				) 

#define JModule_Bitfield_get_port(_id)    (((_id) >> PORT_POS_BIT)    & ((1 << PORT_NUM_BIT) - 1))

#define JModule_Bitfield_get_direction(_id) (((_id) >> DIRECTION_POS_BIT) & ((1 << DIRECTION_NUM_BIT) - 1))

#define JModule_Bitfield_get_process(_id) (((_id) >> PROCESS_POS_BIT) & ((1 << PROCESS_NUM_BIT) - 1))

#define JModule_Bitfield_get_feature(_id) (((_id) >> JMODULE_POS_BIT)  &((1 << JMODULE_NUM_BIT) - 1))

#define JModule_Bitfield_get_context(_id) (((_id) >> CONTEXT_POS_BIT)  &((1 << CONTEXT_NUM_BIT) - 1))

#define JModule_Bitfield_get_module(_id) (((_id) >> MODULE_POS_BIT)  &((1 << MODULE_NUM_BIT) - 1))

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_Bitfield__INCLUDED_)*/
