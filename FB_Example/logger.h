#include "registers.h"
#include "util.h"
#include "MEA21_lib.h"
#include <stdarg.h>

#define SQ_STATE                   1
#define SQ_BOOKING                 2
#define SQ_DAC_STATE_CHANGE        3
#define SQ_CONF_START              4
#define SQ_CONF_RESET              5
#define SQ_CONF_FINISH             6
#define SQ_TRIGGER_FIRED           7
#define SQ_READ_STIM               8
#define SQ_READ_STIM_GROUP         9
#define COMMS_INSTRUCTION_RECEIVED 10
#define SQ_CANARY                  11
#define UNNAMED                    12
#define STIMULUS_WRITE             13
#define SQ_SG_STATE                14
#define SQ_DACPAIR_STATE           15


void MEAME_log(Uint32 count,...);
void log_msg(Uint32 msg);
void reset_logger();
