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
 * @file OJBamProxy.h
 * @brief Include list of available features
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */




#ifndef __OJBAMPROXY_H__
#define __OJBAMPROXY_H__


#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "rtwtypes.h"
#include "Feature_Utils.h"
#include "BamProxy.h"




#define TALARIA_HEADER_SIZE       2





/***  Module Class  ***********************************************/
/**
 * @brief   : OJBamProxy class
 */
class OJBamProxy : public Bam_Proxy
{
public:
        /**
        * @brief              : TBD
        */
        uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* pPayload);
        /**
        * @brief              : TBD
        */
        uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t payload);
        /**
        * @brief              : TBD
        */
        uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, float payload);
        /**
        * @brief              : TBD
        */
        uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t payload);
        /**
        * @brief              : TBD
        */
        uint32_t top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, float payload);
        /**
        * @brief              : TBD
        */
        uint32_t top_read_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count);

protected:


private:


};




#endif // OJBAMPROXY