/****************************************************
 *  BRF_Dll.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BRF_Dll       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(BRF_DLL_H_)
#define BRF_DLL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"
#include "BRF.h"
#include "BRF_Callback.h"

typedef BRF* (*BRF_Create_Fxn)(BRF* brf);
typedef BRF_Resource* (*BRF_Resource_Fxn)(BRF* brf);
typedef CART_STATUS (*BRF_Init_Fxn)(BRF* brf, void* arg);
typedef uint32_t (*BRF_Command_Fxn)(BRF* brf, CART_Message* req, CART_Message* rsp);
typedef uint32_t (*BRF_Exec_Fxn)(BRF* brf, uint32_t thread_level, void* arg);
typedef CART_Cmd_Ret (*BRF_Receive_Fxn)(BRF* brf, CART_Message* rsp);

typedef void (*BRF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);
typedef CART_Cmd_Ret (*BRF_Callback_Func_Notify)(CART_Message* status);
typedef CART_Cmd_Ret (*BRF_Callback_Func_Send)(CART_Message* req);
typedef CART_Cmd_Ret (*BRF_Callback_Func_Read)(BRF_STORAGE_ID storage_id, uint32_t address, uint32_t size, uint8_t* buf, void* ctx); 

typedef void (*BRF_Callback_Register_Post_Fxn)(BRF_Callback_Func_Post funcptr);
typedef void (*BRF_Callback_Register_Notify_Fxn)(BRF_Callback_Func_Notify funcptr);
typedef void (*BRF_Callback_Register_Send_Fxn)(BRF_Callback_Func_Send funcptr);
typedef void (*BRF_Callback_Register_Read_Fxn)(BRF_Callback_Func_Read funcptr);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BRF_DLL_H_)*/
 