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
 * @file OJBamProxy.cpp
 * @brief Feature Control defines for enabled features *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */




/***  Include Files ***********************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "rtwtypes.h"
#include "Feature_Utils.h"
#include "BamProxy.h"
#include "OJBamProxy.h"



/*******************************************************************************
 *  OJBamProxy - Public
*******************************************************************************/
uint32_t OJBamProxy::top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count, uint32_t* pPayload)
{
    uint32_t Index = TALARIA_HEADER_SIZE;
    uint32_t payloadOff = 0;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)byte_count);
    if (pPayload != NULL) {
        for (int i = 0; i < byte_count; i += 4) {
            put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)pPayload[payloadOff++]);
        }
    }
    else {
        Index +=  byte_count >> 2;
    }
    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));

    return Index;
}

uint32_t OJBamProxy::top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t payload)
{
    uint32_t Index = TALARIA_HEADER_SIZE;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)0);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(payload));

    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));

    return Index;
}

uint32_t OJBamProxy::top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, float payload)
{
    uint32_t Index = TALARIA_HEADER_SIZE;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)0);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));

    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));

    return Index;
}

uint32_t OJBamProxy::top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t payload)
{
    uint32_t Index = TALARIA_HEADER_SIZE;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));

    return Index;
}

uint32_t OJBamProxy::top_write_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, float payload)
{
    uint32_t Index = TALARIA_HEADER_SIZE;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)sizeof(uint32_t));
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)(*(uint32_t*)&payload));

    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));

    return Index;
}

uint32_t OJBamProxy::top_read_request(uint32_t* data, uint32_t topid, uint32_t callhandler, uint32_t offset, uint32_t byte_count)
{
    uint32_t Index = TALARIA_HEADER_SIZE;

    put_int32_into_buffer((uint8_t *)&data[0], 0);
    put_int32_into_buffer((uint8_t *)&data[Index++], topid);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)callhandler);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)offset);
    put_int32_into_buffer((uint8_t *)&data[Index++], (int32_t)byte_count);
    put_int32_into_buffer((uint8_t *)&data[1], (Index - TALARIA_HEADER_SIZE) * sizeof(data[0]));

    return Index;
}

