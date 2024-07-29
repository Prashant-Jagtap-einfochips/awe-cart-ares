/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/
/****************************************************
 * @file Logger.h
 *
 * @brief Implementation of the Class Logger
 *
 * @author: cb1037694
 ****************************************************/

#include "Logger.h"
#include "BAF_Callback.h"

#define _D(_x) //_x

#ifdef BAF_PRINTF_SUPPORTED
extern int BAF_printf(const char*,...);
#else
#define BAF_printf(_x,...) //x
#endif

#if defined(BAF_LOGGER)
void Logger_print(Logger_Type id, uint32_t sender_id, const char* msg, uint32_t arg1, uint32_t arg2)
{
   Logger* this = &BAF_logger[id];
   if (!this->enabled) { return; }
   if ( this->start ) {
      Log* log = this->current;
      this->current++;
      if( (this->current+1) >= this->end ){
         if(this->circular){
            this->wrapped = true;
            this->current = this->start;
         } else {
            this->current--;
            return;
         }
      }

      log->record.msg[0] = msg[0];
      log->record.msg[1] = msg[1];
      log->record.sender_id = sender_id;
      log->record.arg_byte = arg1;
      log->record.arg_word = arg2;
   }
   _D ( BAF_printf("%08X ", sender_id));
   _D ( BAF_printf(msg, arg1, arg2));
}

void Logger_print1(Logger_Type id, uint32_t sender_id, const char* msg, uint32_t arg1)
{
   Logger* this = &BAF_logger[id];
   if (!this->enabled) { return; }
   if(this->start) {
      Log* log = this->current;
      this->current++;
      if( (this->current+1) >= this->end ){
         if(this->circular){
            this->wrapped = true;
            this->current = this->start;
         } else {
            this->current--;
            return;
         }
      }
      log->record.msg[0] = msg[0];
      log->record.msg[1] = msg[1];
      log->record.sender_id = sender_id;
      log->record.arg_byte = 0;
      log->record.arg_word = arg1;
   }
   _D ( BAF_printf("%d: %08X ", this->index, sender_id));
   _D ( BAF_printf(msg, arg1));
}

void Logger_msg(Logger_Type id, uint32_t sender_id, const char* msg)
{
   Logger* this = &BAF_logger[id];
   if (!this->enabled) { return; }
   if(this->start) {
      Log* log = this->current;
      this->current++;
      if( (this->current+1) >= this->end ){
         if(this->circular){
            this->wrapped = true;
            this->current = this->start;
         } else {
            this->current--;
            return;
         }
      }
      log->record.msg[0] = msg[0];
      log->record.msg[1] = msg[1];
      log->record.sender_id = sender_id;
   }
}

void Logger_init(Logger_Type id, bool circular)
{
   Logger* this = &BAF_logger[id];
   if( this->heap.base ) {
      this->enabled = true;
      this->start = this->heap.base;
      this->end   = (void*)((intptr_t)this->heap.base + this->heap.size);
      this->current = this->start;
      this->wrapped = false;
      this->circular = circular;
   }
   else {
      this->enabled = false;
      this->start = NULL;
      this->end   = NULL;
      this->current = NULL;
   }
}

void Logger_reset(Logger_Type id)
{
   Logger* this = &BAF_logger[id];
   this->current = this->start;
   this->wrapped = false;
}

void Logger_dump(Logger_Type id)
{
   Logger* this = &BAF_logger[id];
   Log* log;

   BAF_printf("LOG%d %d %d %d %08X\n", id, this->enabled, this->circular, this->wrapped, this->current - this->start);

   if(this->start)
   {
      if ((id == LOG_TYPE_EXEC) || (id == LOG_TYPE_PRINT))
      {
         if ( this->circular && this->wrapped ){
            for ( log = this->current; log < this->end-1; log++ ) {
               BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
                              log->record.sender_id, log->record.arg_byte, log->record.arg_word );
            }
         }
         for ( log = this->start; log < this->current; log++) {
            BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
                              log->record.sender_id, log->record.arg_byte, log->record.arg_word );
         }
      }
      else if (id == LOG_TYPE_CAPTURE)
      {
         /* Circular mode is not supported */
         for ( log = this->start; log < this->current; log++) {
            BAF_printf("%c%c %02X %02X %08X\n", log->record.msg[0], log->record.msg[1],
                              log->record.sender_id, log->record.arg_byte, log->record.arg_word );
            if ( log->record.msg[0] == 'C' ) {  // capture log
               uint32_t size = log->record.arg_word;
               uint32_t i;
               log++;
               for ( i = 0 ; i < size; i+=sizeof(Log)/sizeof(float), log++ )  {
                  BAF_printf(" - %.3f\n", log->flt[0]);
                  BAF_printf(" - %.3f\n", log->flt[1]);
               }
               log--;
            }
            else if ( log->record.msg[0] == 'S' ) {  // stats log
               uint32_t size = log->record.arg_word;
               uint32_t i;
               log++;
               for ( i = 0 ; i < size; i+=sizeof(Log)/sizeof(uint32_t), log++ )  {
                  BAF_printf(" - %08x\n", log->word[0]);
                  BAF_printf(" - %08x\n", log->word[1]);
               }
               log--;
            }
         }
      }
   }
}

void Logger_copy(Logger_Type id, void* buf, uint32_t size)
{
   Logger* this = &BAF_logger[id];
   if (!this->enabled) { return; }
   if (((intptr_t) buf & 3) | (size & 3) ) { return; } // word multiple
   if(this->start) {
      Log* log = this->current;
      uint32_t * buf_u = buf;
      this->current += size/sizeof(Log);
      if( this->current + 1 >= this->end ){
         this->current -= size/sizeof(Log);
         return;
      }
      uint32_t * buf_log = (uint32_t*) log;
      uint32_t i;
      for ( i = 0; i < ( size >> 2 ); i++ ) {
         buf_log[i] = buf_u[i];
      }
   }
}

#if defined(BAF_STATSLOGGER)
void Logger_stats(Logger_Type id, uint32_t thread_level, uint32_t process_id, uint32_t start_stop)
{
   Logger* this = &BAF_logger[id];
   if ((this->enabled) && (this->start) && ((this->current+1) < this->end)){
      Log* log = this->current;
      uint32_t len = (((this->end - this->current) - 1) * sizeof(Log))/(sizeof(uint32_t));
      uint32_t cnt = BAF_Callback_Stats((uint32_t *)(log + 1), len, start_stop);

      if (cnt > 0)
      {
         log->record.msg[0] = 'S';
         log->record.msg[1] = (start_stop == 0) ? 'g' : 'n';
         log->record.sender_id = thread_level;
         log->record.arg_byte = process_id;
         log->record.arg_word = cnt;
         this->current += ((cnt * sizeof(uint32_t))/sizeof(Log) + 1);
      }
   }
}
#endif

uint32_t Logger_pull(Logger_Type id, uint32_t *size, uint32_t* data ) {
   Logger* this = &BAF_logger[id];
   Log* log;
   int32_t nlogs = *size;
   if(this->start) {
      if ( this->circular && this->wrapped ){
         for ( log = this->pull; log < (this->end-1) && (nlogs > 0); log++, nlogs-- ) {
            *data++ = log->word[0];
            *data++ = log->word[1];
            this->pull = log + 1;
         }
         if(log == this->end - 1) {
            this->wrapped = false;
            this->pull = this->start;
         }
      }
      for ( log = this->pull; (log < this->current) && (nlogs > 0); log++, nlogs--) {
         *data++ = log->word[0];
         *data++ = log->word[1];
         this->pull = log + 1;
      }
   }
   return (*size - nlogs); //number of records transferred
}
#endif /*defined(BAF_LOGGER)*/

