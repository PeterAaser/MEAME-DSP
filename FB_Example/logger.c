#include "logger.h"
#include "MEA21_lib.h"


static Uint32 log_counter = 0;
static Uint32 last_entry = ((LOG_END - LOG_START)/4);

void MEAME_log(Uint32 count,...){
  if(log_counter > last_entry){
    return;
  }
  WRITE_REGISTER( (LOG_START + (log_counter*4)), count);
  log_counter++;

  va_list ap;
  int ii;
  va_start(ap, count);

  for(ii = 0; ii < count; ii++){
    Uint32 huh = LOG_START + (log_counter*4);
    Uint32 what = va_arg(ap, Uint32);

    WRITE_REGISTER(huh,what);
    log_counter++;
  }

  WRITE_REGISTER(LOG_ENTRIES, log_counter);
}

void reset_logger(){
  log_counter = 0;
  WRITE_REGISTER(LOG_ENTRIES, log_counter);
}
