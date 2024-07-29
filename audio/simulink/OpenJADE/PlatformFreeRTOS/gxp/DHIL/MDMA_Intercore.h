/*******************************************************************************
 *  BOSE CORPORATION
 *  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 *  This program may not be reproduced, in whole or in part in any
 *  form or any means whatsoever without the written permission of:
 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168
 ******************************************************************************/

/**
 * @file MDMA_Intercore.h
 * @date
 * @brief
 *
 * @ingroup DSP
 *
 * @details
 *
 * @see
 *
 */

#ifndef __MDMA_INTERCORE_H__
#define __MDMA_INTERCORE_H__

/* Add your custom header content here */

/***  Types  ***********************************************************/

/** @typedef mdma_result_t
 * @brief This type is defines the function result returned
 */
typedef enum
{
    MDMA_SUCCESS,
    MDMA_FAILURE
} mdma_result_t;

/***  Function Prototypes  *********************************************/

mdma_result_t MDMA_Init( void );
void MDMA_blockAndLaunchRx( void );
void MDMA_launchTx( void );

#endif /* __MDMA_INTERCORE_H__ */
