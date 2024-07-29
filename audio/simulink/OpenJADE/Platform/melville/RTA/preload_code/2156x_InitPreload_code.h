/******************************************************************************

Copyright(c) 2018 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*******************************************************************************/

/*****************************************************************************
 * 2156x_InitPreload_code.h
 *****************************************************************************/

#ifndef ADSP2156X_INITPRELOAD_CODE_H
#define ADSP2156X_INITPRELOAD_CODE_H

/*!
* @file      2156x_InitPreload_code.h
*
* @brief     Global header file for 2156x_InitPreload_code.c
*
* @details
*
*/

#include <stdlib.h>
#include <services/pwr/adi_pwr.h>
#include <cdef2156x_rom.h>
#include <sys/platform.h>
#include "../adi_initialize.h"

#include "adi_clockrates_2156x_config.h"
#include "adi_dmc_2156x_config.h"
#include "adi_pwr_2156x_config.h"
#include "adi_smpu_2156x_config.h"

#include "config.h"

static void CheckResult(uint32_t result, const uint32_t lineErrPos);
static void ExecuteSafecode(void);
static void ExecuteIdle(void);
static uint32_t Processbootstruct(ADI_ROM_BOOT_CONFIG* pBootStruct);

#ifdef __cplusplus
extern "C" {
#endif

#pragma retain_name /* retain resolved initcode entry */
uint32_t initcode(ADI_ROM_BOOT_CONFIG* pBootStruct);

#ifdef __cplusplus
}
#endif

#endif /* ADSP2156X_INITPRELOAD_CODE_H */
