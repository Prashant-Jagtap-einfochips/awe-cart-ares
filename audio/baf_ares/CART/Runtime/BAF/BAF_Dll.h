/****************************************************
 *  BAF_Dll.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BAF_Dll      
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(BAF_DLL_H_)
#define BAF_DLL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"
#include "BAF.h"
#include "BAF_Callback.h"




typedef BAF* (*BAF_Create_Fxn)(BAF* baf);
typedef BAF_Resource* (*BAF_Resource_Fxn)(BAF* baf);
typedef CART_STATUS (*BAF_Init_Fxn)(BAF* baf);
typedef uint32_t (*BAF_Command_Fxn)(BAF* baf, CART_Message* req, CART_Message* rsp);
typedef uint32_t (*BAF_Exec_Fxn)(BAF* baf, uint32_t thread_level, void* arg);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BAF_DLL_H_)*/
 