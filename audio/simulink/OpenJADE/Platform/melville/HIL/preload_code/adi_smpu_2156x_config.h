/*********************************************************************************

Copyright(c) 2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

/*!
* @file      adi_smpu_2156x_config.h
*
* @brief     Header file for SMPU configuration .
*
* @details
*
*/

#ifndef ADI_SMPU_2156X_CONFIG_H
#define ADI_SMPU_2156X_CONFIG_H

#include <services/smpu/adi_smpu.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t adi_smpu_cfg0_init(bool bDMC0, bool bSPIflash);

#ifdef __cplusplus
}
#endif

#endif /* ADI_SMPU_2156X_CONFIG_H */
