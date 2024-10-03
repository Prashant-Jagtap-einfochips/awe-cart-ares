/****************************************************
 *  BCF_Dll.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BCF_Dll       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(BCF_DLL_H_)
#define BCF_DLL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"
#include "BCF.h"
#include "BCF_Callback.h"

typedef BCF* (*BCF_Create_Fxn)(BCF* bcf);
typedef BCF_Resource* (*BCF_Resource_Fxn)(BCF* bcf);
typedef CART_STATUS (*BCF_Init_Fxn)(BCF* bcf, void* arg);
typedef uint32_t (*BCF_Command_Fxn)(BCF* bcf, CART_Message* req, CART_Message* rsp);
typedef uint32_t (*BCF_Exec_Fxn)(BCF* bcf, uint32_t thread_level, void* arg);
typedef CART_Cmd_Ret (*BCF_Receive_Fxn)(BCF* bcf, CART_Message* rsp);

typedef void (*BCF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);
typedef CART_Cmd_Ret (*BCF_Callback_Func_Notify)(CART_Message* status);
typedef CART_Cmd_Ret (*BCF_Callback_Func_Send)(CART_Message* req);

typedef void (*BCF_Callback_Register_Post_Fxn)(BCF_Callback_Func_Post funcptr);
typedef void (*BCF_Callback_Register_Notify_Fxn)(BCF_Callback_Func_Notify funcptr);
typedef void (*BCF_Callback_Register_Send_Fxn)(BCF_Callback_Func_Send funcptr);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BCF_DLL_H_)*/
 