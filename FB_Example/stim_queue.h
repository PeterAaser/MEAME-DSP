#ifndef SQ_H
#define SQ_H

#include "stim_API.h"


/**
   Stim queue is responsible for maintaining a schedule for firing electrode stimulus
   triggers.
   Setup can only be done wrt. electrode timings whereas electrode enable and friends
   are set using the Stim_API.h header.
 */

/**
   A stimulus request is a data structure holding its period,
   the requested electrodes for stimuli, and the timestep
   it should fire next.

   At priming_offes steps before firing the request is "booked" to one of
   the DAC pairs. This is to ensure that the DAC pair is configured
   before firing.
*/
typedef struct Stimulus_request
{
  int active;
  int period;
  int next_firing_timestep;
} Stimulus_request;


/**
   Runs one tick, firing stimuli triggers when appropriate
 */
io_void run_stim_queue();


/**
   Turn a single stim group on or off.
 */
void toggle_stim_group(int idx, int status);


/**
   Read a period change request.
   Does not toggle a stim group on or off
 */
void read_stim_group_request(int group, int next_period);


#define HALTED  0
#define RUNNING 1
#define SETUP   2
void set_stim_queue_state(int state);

// just resets stuff.
void setup_stim_queue();

#endif // SQ_H
