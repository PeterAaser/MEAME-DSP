#include "registers.h"
#include "util.h"
#include "MEA21_lib.h"
#include <stdarg.h>

#define DAC_STATE_CHANGE 1
#define CONF             2
#define CONF_RESET       3
#define CONF_START       4
#define TRIGGER          5
#define STATE_EN_ELEC    6
#define STATE_DAC_SEL    7
#define STATE_MODE       8
#define BOOKING          9
#define READ_STIM        10
#define COMMS_READ_REQ   11
#define BOOKING_FOUND    12

void MEAME_log(Uint32 count,...);
void log_msg(Uint32 msg);
void reset_logger();
