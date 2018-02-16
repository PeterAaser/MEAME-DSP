#include "stim_queue.h"
#include "MEA21_lib.h"
#include "registers.h"
#include "util.h"
#include "logger.h"

#define IDLE   0
#define PRIMED 1
#define FIRING 2


static Uint32 counter = 0;
static Uint32 canary_counter = 0;
static Uint32 mode = HALTED;

static Uint32 priming_offset = 2000;
// static Uint32 log_counter = 0;

// A stimulus request is a data structure holding its period,
// the requested electrodes for stimuli, and the timestep
// it should fire next.

// At priming_offes steps before firing the request is "booked" to one of
// the DAC pairs. This is to ensure that the DAC pair is configured
// before firing.
struct stimulus_request
{
  Uint32 idx;
  Uint32 active;
  Uint32 electrodes[2];
  Uint32 period;
  Uint32 next_firing_timestep;
};
typedef struct stimulus_request stimulus_request;

// Book-keeping for DAC pairs, keeping track of which pairs are firing,
// which pairs are idle and is used to book firings.
struct DAC_pair
{
  Uint32 idx;
  Uint32 electrodes[2]; // necessary for resetting, cannot rely on booked_for_idx
  Uint32 state;
  Uint32 next_state_timestep;
};
typedef struct DAC_pair DAC_pair;

static stimulus_request stim_reqs[4];
static DAC_pair DAC_pairs[3];

// current system state
Uint32 enabled_electrodes[2] = {0};
Uint32 DAC_select[4] = {0};
Uint32 electrode_mode[4] = {0};

void trigger_DAC_pair(DAC_pair* p);
void configure_DAC_pair(DAC_pair* p, stimulus_request* r);
void commit_electrode_config();
void log_states();

void book_stimulus(stimulus_request* booker){
  MEAME_log(BOOKING, booker->idx, 0, counter);
  Uint32 ii = 0;
  for(ii = 0; ii < 3; ii++){
    if(DAC_pairs[ii].state == IDLE){
      MEAME_log(BOOKING_FOUND, booker->idx, ii, counter);
      configure_DAC_pair(&DAC_pairs[ii], booker);
      break;
    }
  }
}

void run_stim_queue(){
  if(mode == HALTED){
    return;
  }

  int ii = 0;

  for(ii = 0; ii < 4; ii++){
    if(stim_reqs[ii].period == 0){ continue; }
    if((stim_reqs[ii].next_firing_timestep - counter) == priming_offset){
      book_stimulus(&stim_reqs[ii]);
      stim_reqs[ii].next_firing_timestep += stim_reqs[ii].period;
    }
  }

  for(ii = 0; ii < 3; ii++){
    if(DAC_pairs[ii].next_state_timestep == counter){
      if(DAC_pairs[ii].state == IDLE){
      }
      else if(DAC_pairs[ii].state == PRIMED){
        MEAME_log(DAC_STATE_CHANGE, ii, FIRING, counter);
        trigger_DAC_pair(&DAC_pairs[ii]);
      }
      else if (DAC_pairs[ii].state == FIRING){
        MEAME_log(DAC_STATE_CHANGE, ii, IDLE, counter);
        DAC_pairs[ii].state = IDLE;
      }
    }
  }

  counter++;
  if((counter % 4096) == 0){
    canary_counter++;
  }
}

void reset_DAC_pair(DAC_pair* p){

  MEAME_log(CONF, CONF_RESET, p->idx, counter);

  log_states();

  Uint32 DAC_select_reset_mask[4] = {0};
  Uint32 electrode_mode_reset_mask[4] = {0};

  Uint32 ii;
  Uint32 electrode_address;

  for (ii = 0; ii < 2; ii++){
    for (electrode_address = 0; electrode_address < 30; electrode_address++){
      if(get_bit32(p->electrodes[ii], electrode_address)){

        int a = electrode_address + (ii * 30);
        DAC_select_reset_mask[a/15]     |=   (3 << ( (2 * a) % 30) );
        electrode_mode_reset_mask[a/15] |=   (3 << ( (2 * a) % 30) );
      }
    }
  }

  enabled_electrodes[0] = ( enabled_electrodes[0] & p->electrodes[0]) ^ enabled_electrodes[0];
  enabled_electrodes[1] = ( enabled_electrodes[1] & p->electrodes[1]) ^ enabled_electrodes[1];

  DAC_select[0] = (DAC_select[0] & DAC_select_reset_mask[0]) ^ DAC_select[0];
  DAC_select[1] = (DAC_select[1] & DAC_select_reset_mask[1]) ^ DAC_select[1];
  DAC_select[2] = (DAC_select[2] & DAC_select_reset_mask[2]) ^ DAC_select[2];
  DAC_select[3] = (DAC_select[3] & DAC_select_reset_mask[3]) ^ DAC_select[3];

  electrode_mode[0] = (electrode_mode[0] & electrode_mode_reset_mask[0]) ^ electrode_mode[0];
  electrode_mode[1] = (electrode_mode[1] & electrode_mode_reset_mask[0]) ^ electrode_mode[1];
  electrode_mode[2] = (electrode_mode[2] & electrode_mode_reset_mask[0]) ^ electrode_mode[2];
  electrode_mode[3] = (electrode_mode[3] & electrode_mode_reset_mask[0]) ^ electrode_mode[3];

  log_states();
}

void configure_DAC_pair(DAC_pair* p, stimulus_request* r){

  MEAME_log(CONF_START, p->idx, r->idx, counter);

  int mux = p->idx + 1;
  int config = 0;

  reset_DAC_pair(p);

  int electrode_address;
  int ii;
  for (ii = 0; ii < 2; ii++){
    for (electrode_address = 0; electrode_address < 30; electrode_address++){
      if(get_bit32(r->electrodes[ii], electrode_address)){

        int a = electrode_address + (ii * 30);
        DAC_select[a/15]     |=   (mux    << ( (2 * a) % 30) );
        electrode_mode[a/15] |=   (config << ( (2 * a) % 30) );
      }
    }
  }

  p->electrodes[0] = r->electrodes[0];
  p->electrodes[1] = r->electrodes[1];

  enabled_electrodes[0] |= r->electrodes[0];
  enabled_electrodes[1] |= r->electrodes[1];

  p->state = PRIMED;
  p->next_state_timestep = counter + priming_offset;

  log_states();

  commit_electrode_config();

  return;
}

void trigger_DAC_pair(DAC_pair* d){
  Uint32 trigger_value;
  if(d->idx == 0){
    MEAME_log(TRIGGER, d->idx, 1, counter);
    trigger_value = 0x1;
  }
  else if(d->idx == 1){
    MEAME_log(TRIGGER, d->idx, 2, counter);
    trigger_value = 0x2;
  }
  else{
    MEAME_log(TRIGGER, d->idx, 4, counter);
    trigger_value = 0x4;
  }
  MEAME_log(DAC_STATE_CHANGE, d->idx, FIRING, counter);
  d->state = FIRING;
  d->next_state_timestep = (counter + FIRING_PERIOD);
  WRITE_REGISTER(0x0214, trigger_value); // Manually fire trigger
}

void commit_electrode_config(){
  write_segment( ELECTRODE_ENABLE,  2, enabled_electrodes);
  write_segment( ELECTRODE_DAC_SEL, 4, DAC_select);
  write_segment( ELECTRODE_MODE,    4, electrode_mode);
}


// when the period is reduced we want to make sure that the next timestep is at most period steps away
void read_stim_request(Uint32 group, Uint32 next_period, Uint32 elec0, Uint32 elec1)
{
  Uint32 next_firing_timestep_diff = next_period - stim_reqs[group].period;

  MEAME_log(READ_STIM, group, next_period, counter);

  stim_reqs[group].period = next_period;
  stim_reqs[group].next_firing_timestep += next_firing_timestep_diff;

  if(stim_reqs[group].next_firing_timestep < counter){
    stim_reqs[group].next_firing_timestep = stim_reqs[group].period + counter;
  }
  // In case the next firing event is further away than the period of the stim req
  if((stim_reqs[group].next_firing_timestep - counter) > stim_reqs[group].period){
    stim_reqs[group].next_firing_timestep = counter + stim_reqs[group].period;
  }

  stim_reqs[group].electrodes[0] = elec0;
  stim_reqs[group].electrodes[1] = elec1;
}

void setup_stim_queue(){
  int ii;
  for(ii = 0; ii < 4; ii++){
    stim_reqs[ii].active = 0;
    stim_reqs[ii].idx = ii;
    stim_reqs[ii].period = 0xFFFFFFFF;
    stim_reqs[ii].electrodes[0] = 0;
    stim_reqs[ii].electrodes[1] = 0;
    stim_reqs[ii].next_firing_timestep = 0xFFFFFFFF;
  }
  for(ii = 0; ii < 3; ii++){
    DAC_pairs[ii].electrodes[0] = 0;
    DAC_pairs[ii].electrodes[1] = 0;

    DAC_pairs[ii].idx = ii;
    DAC_pairs[ii].state = IDLE;
    DAC_pairs[ii].next_state_timestep = 0xFFFFFFFF;

  }
  enabled_electrodes[0] = 0;
  enabled_electrodes[1] = 0;
  DAC_select[0] = 0;
  DAC_select[1] = 0;
  DAC_select[2] = 0;
  DAC_select[3] = 0;
  electrode_mode[0] = 0;
  electrode_mode[1] = 0;
  electrode_mode[2] = 0;
  electrode_mode[3] = 0;
}

void log_states(){
  MEAME_log(STATE_EN_ELEC, 0, enabled_electrodes[0], counter);
  MEAME_log(STATE_EN_ELEC, 1, enabled_electrodes[1], counter);

  MEAME_log(STATE_DAC_SEL, 0, DAC_select[0], counter);
  MEAME_log(STATE_DAC_SEL, 1, DAC_select[1], counter);
  MEAME_log(STATE_DAC_SEL, 2, DAC_select[2], counter);
  MEAME_log(STATE_DAC_SEL, 3, DAC_select[3], counter);

  MEAME_log(STATE_MODE, 0, electrode_mode[0], counter);
  MEAME_log(STATE_MODE, 1, electrode_mode[1], counter);
  MEAME_log(STATE_MODE, 2, electrode_mode[2], counter);
  MEAME_log(STATE_MODE, 3, electrode_mode[3], counter);
}


void set_stim_queue_running(Uint32 running){
  mode = running;
}
