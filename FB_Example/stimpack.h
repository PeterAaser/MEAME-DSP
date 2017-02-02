#include "MEA21_lib.h"

void stimPack(Uint32 update);

struct stimulus_group
  {
    Uint32 DAC; //id
    Uint32 electrodes[2];
    Uint32 period;
    Uint32 tick;
    Uint32 sample;
  };

typedef struct stimulus_group stimulus_group;


struct stimulus_request
  {
    Uint32 frequency;
    Uint32 electrode;
    Uint32 DAC_group;
  };

typedef struct stimulus_request stimulus_request;

int read_stim_req();

#define ADD_ELECTRODE 1
#define REMOVE_ELECTRODE 2
#define CHANGE_PERIOD 3
#define CHANGE_SAMPLE 4
