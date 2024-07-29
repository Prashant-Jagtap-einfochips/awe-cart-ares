/*********************************************************************************

Copyright(c) 2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

/** @addtogroup Init_Preload_2156x Processor Initialization Code
 *  @{
 *
 */

/*!
* @file      adi_smpu_2156x_config.c
*
* @brief     SMPU configuration file.
*
* @details
*            This file is used to configure the SMPU Service as required by the init code.
*/

#include <services/smpu/adi_smpu.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "adi_smpu_2156x_config.h"

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_14_7:"Allow functions to have multiple exits for better readability and optimized code")
#endif


/**
 * @brief       Programs SMPU to prevent access to un-populated memory regions.
 * @details     Uses SMPU instances to disable accesses to system memory that may not be populated
 *              or needs to be initialized before being accessed. This will avoid the possibility
 *              of an infinite stall in the system due to a speculative access to a disabled or
 *              uninitialized memory.
 *
 *              No callbacks are enabled.
 *
 * @param[in]   bDMC0 bool to disable un-populated DMC0 access.
 * @param[in]   bSPIflash bool to disable un-populated SPI Flash access.
 *
 * @return   Status
 *           - 0: Successful in all the initializations.
 *           - 1: Error.
 *
 * @note
 **/
uint32_t adi_smpu_cfg0_init(bool bDMC0, bool bSPIflash)
{
   ADI_SMPU_RESULT result;

   if(!bDMC0)
   {
       ADI_SMPU_HANDLE hSmpuDmc0;

       /* DMC0 - SMPU instance 9 */
       result = adi_smpu_Init (ADI_SMPU_EXTDDR0_DEVICE_ID, NULL, NULL, NULL, &hSmpuDmc0);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_Reset(hSmpuDmc0);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetReadSpeculationDisable(hSmpuDmc0, true);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetSecureGlobalPermissions(hSmpuDmc0, ADI_SMPU_SECURITY_NO_ACCESS , ADI_SMPU_SECURITY_NO_ACCESS );
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetSecureBusErrorDisable(hSmpuDmc0, true);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

   }

   if(!bSPIflash)
   {
       ADI_SMPU_HANDLE hSmpuFlash;

       /* SPI Flash Memory (XIP Range) - SMPU instance 11 */
       result = adi_smpu_Init (ADI_SMPU_SPI2_3_DEVICE_ID, NULL, NULL, NULL, &hSmpuFlash);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_Reset(hSmpuFlash);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetReadSpeculationDisable(hSmpuFlash, true);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetSecureGlobalPermissions(hSmpuFlash, ADI_SMPU_SECURITY_NO_ACCESS , ADI_SMPU_SECURITY_NO_ACCESS );
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

       result = adi_smpu_SetSecureBusErrorDisable(hSmpuFlash, true);
       if(result!= ADI_SMPU_SUCCESS)
       {
            return 1u;
       }

   }

   return 0u;
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif

/*@}*/
