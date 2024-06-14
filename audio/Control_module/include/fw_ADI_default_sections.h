/***********************************************************************
*
*           Audio Framework
*           ---------------
*
************************************************************************
* fw_ADI_default_sections.h
* **********************************************************************
*
* Description: set default section properteis for the ADI platforms
*
* Copyright: (c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*            3235 Kifer Road
*            Santa Clara, CA 95054
*
***********************************************************************/

#ifndef _FW_ADI_DEFAULT_SECTIONS_H
#define _FW_ADI_DEFAULT_SECTIONS_H

/* ----------------------------------------------------------------------
** Set default memory sections on the SHARC and Blackfin.  These
** sections are used for any unspecified code and data items.
** ------------------------------------------------------------------- */

#if defined(__ADSP21000__) || defined(__ADSPBLACKFIN__)
#pragma default_section(CODE, "awe_fw_slowcode")
#pragma default_section(ALLDATA, "awe_fw_slowanydata")
#pragma default_section(SWITCH, "awe_fw_slowanydata")
#endif

#endif // _FW_ADI_DEFAULT_SECTIONS_H
