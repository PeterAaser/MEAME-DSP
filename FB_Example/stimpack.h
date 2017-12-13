/* #include "MEA21_lib.h" */


/* /\** */
/*    No MCS code to worry about here. */
/*    Still much worry to be had! */

/*    Stimpack maintains 3 stim groups. Each group is essentially a counter keeping */
/*    track off when to next fire by manually triggering one of the triggers. */

/*  *\/ */
/* struct stimulus_group */
/*   { */
/*     Uint32 DAC; //group id */
/*     Uint32 electrodes[2]; */
/*     Uint32 period; */
/*     Uint32 tick; */
/*     Uint32 sample; */
/*     Uint32 fires; */
/*   }; */

/* typedef struct stimulus_group stimulus_group; */


/* struct stimulus_request */
/*   { */
/*     Uint32 frequency; */
/*     Uint32 electrode; */
/*     Uint32 DAC_group; */
/*   }; */

/* typedef struct stimulus_request stimulus_request; */

/* void read_stim_request(); */
/* void run_stimpack(); */
/* void dump_stim_group(Uint32 group); */
/* void setup_stimpack(); */

/* Uint32 get_timestep(); */
