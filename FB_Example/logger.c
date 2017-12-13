#define LOGGING 1

#include "logger.h"
#include "MEA21_lib.h"

static Uint32 log_counter;

void log(Uint32 logger_id1, Uint32 logger_id2, Uint32 value1, Uint32 value2){
#ifdef LOGGING
  if(((log_counter*4*4) + LOG_BASE) > MAILBOX_END){
    return;
  }

  WRITE_REGISTER((LOG_BASE + (4*4*(log_counter + 0))), logger_id1);
  WRITE_REGISTER((LOG_BASE + (4*4*(log_counter + 1))), logger_id2);
  WRITE_REGISTER((LOG_BASE + (4*4*(log_counter + 2))), value1);
  WRITE_REGISTER((LOG_BASE + (4*4*(log_counter + 3))), value2);

  log_counter++;
  WRITE_REGISTER(ENTRIES, log_counter);

#endif
}
