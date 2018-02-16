#include "logger.h"
#include "MEA21_lib.h"


static Uint32 log_counter = 0;
static Uint32 last_entry = ((LOG_END - LOG_START)/4);

void MEAME_log(Uint32 count,...){
  if(log_counter > last_entry){
    return;
  }
  WRITE_REGISTER( (LOG_BASE + (log_counter*4)), count);
  log_counter++;

  va_list ap;
  int ii;
  va_start(ap, count);

  for(ii = 0; ii < count; ii++){
    WRITE_REGISTER( (LOG_BASE + (log_counter*4)), va_arg(ap,ii) );
    log_counter++;
  }
}

void reset_logger(){
  log_counter = 0;
}
