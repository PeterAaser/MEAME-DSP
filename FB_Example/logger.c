
#include "logger.h"
#include "MEA21_lib.h"

static Uint32 log_counter = 0;

void MEAME_log(Uint32 logger_id1, Uint32 logger_id2, Uint32 value1, Uint32 value2){

  WRITE_REGISTER((LOG_BASE + log_counter + 0),  logger_id1);
  WRITE_REGISTER((LOG_BASE + log_counter + 4),  logger_id2);
  WRITE_REGISTER((LOG_BASE + log_counter + 8),  value1);
  WRITE_REGISTER((LOG_BASE + log_counter + 12), value2);

  log_counter = log_counter + 16;
  WRITE_REGISTER(ENTRIES, log_counter/16);

  if(log_counter == (( MAILBOX_END - LOG_BASE )/4)){
    log_counter = 0;
  }
}

void reset_logger(){
  log_counter = 0;
  int ii = LOG_BASE;
  while(ii < MAILBOX_END){
    WRITE_REGISTER(ii, 0);
    ii = ii+4;
  }
}
