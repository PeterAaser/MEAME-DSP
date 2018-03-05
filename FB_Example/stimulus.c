#include "stimulus.h"
#include "MEA21_lib.h"
#include <math.h>
#include "registers.h"
#include "logger.h"

void AddLoop(int Channel, int Vectors, int Repeats);
void ClearChannel(int channel);
int AddDataPoint(int Channel, int duration, int value);

#define OFFSET 0x8000

/* #define LOGGING */

/**
   This garbage is mostly taken directly from the MCS code, only difference is breaking
   it down into more logical units. Beware ye who enter.
 */
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//// MCS CODE
// [[file:img/mcs_overview.png]]

void write_register(Uint32 addr, Uint32 val, Uint32 line){
  WRITE_REGISTER(addr, val);

#ifdef LOGGING
  MEAME_log(4, STIMULUS_WRITE, addr, val, line);
#endif
}

void UploadSine(int Channel, int Amplitude, int Period, int Repeats, int Stepsize){


  int yold = 0;
  int duration = 0;
  int datapoints = 0;
  volatile int i;
  int y;
  int vectors_used;

  vectors_used = 0;

  ClearChannel(Channel);

  // Ultra-shitty, Period is in 20µs it seems btw
  for (i = 0; i < Period; i++){
    y = Amplitude * sin((((double)i)/Period)*2*3.1415);

    if (abs(y - yold) > Stepsize){
      vectors_used += AddDataPoint(Channel, duration, yold + OFFSET);
      datapoints++;
      yold = y;
      duration = 1; // 20 us
    }
    else{
      duration++;
    }
  }

  // huh?
  vectors_used += AddDataPoint(Channel, duration, yold + OFFSET);
  AddLoop(Channel, vectors_used, Repeats);

  // Create Sideband Information
  ClearChannel(Channel+1);
  vectors_used = 0;

  // x19 = 0001 1001
  // Adds sideband information. Amplifier, stim select & stim switch set to 1
  vectors_used += AddDataPoint(Channel+1, Period, 0x0019);
  AddLoop(Channel+1, vectors_used, Repeats);
}

// Loops a sequence. Translates to
// for(i=0; i < repeats; i++){
//   exec from current address - vectors to current address
// }
void AddLoop(int Channel, int Vectors, int Repeats)
{
  Uint32 ChannelReg;
  Uint32 LoopVector;

  ChannelReg = 0x9f20 + Channel*4;

  if (Repeats > 1)
    {
      // 28th bit set (001 : loop pointer vector)
      // Repeats set to Number of repeats
      // Vectors: How long should we jump backwards

      // The result is a simple for loop
      LoopVector = 0x10000000 | (Repeats << 16) | Vectors;
      write_register(ChannelReg, LoopVector, __LINE__);
    }
}

void ClearChannel(int Channel)
{
  Uint32 ClearReg = 0x920c + Channel*0x20;
  write_register(ClearReg, 0, __LINE__);      // Any write to this register clears the Channeldata
}


void SetSegment(int Channel, int Segment)
{
  Uint32 SegmentReg = 0x9200 + Channel*0x20;
  write_register(SegmentReg, Segment, __LINE__);  // Any write to this register clears the Channeldata
}


// misnomer, may add several values
int AddDataPoint(int Channel, int duration, int value)
{
  int vectors_used = 0;
  int	Vector;
  Uint32 ChannelReg = 0x9f20 + Channel*4;

  // Adds sine wave points. If the duration bef
  if (duration > 1000){
    // The value is decoded in steps. The first 3 bits (30 - 28) decides how
    // to interpret the remaining data
    // 31 - reserverd
    // 30 - 28:

    // 000 <- DAC/SBS data vector
    // 001 Loop pointer vector
    // 010 Long loop pointer vector
    // 011 Loop pointer counter vector
    // 111 END command

    ////////////////////////////////////////
    ////////////////////////////////////////
    // DAC/SBS datavector
    //
    // DAC:
    //
    // 27: reserved
    // 26: timebase for repeats
    // 25 - 16 : Number of repeats in timebase (plus one, thus 0 gives 1 repeat, 4 gives 5 repeats etc)
    // 15 - 0  :  DAC data value
    //
    // SBS:
    //
    // 15-8 : Electrode Config ID (for list-mode only)
    // 4    : Stimulus Select
    // 3    : Stimulation Switch
    // 2-1  : reserved(??)
    // 0    : Amplifier Protection Switch/Blanking


    // 00000100 00000000 00000000 00000000 = 0x04000000
    // Sets bit 26 to 1, setting repeat time-base to 1000 * 20µs
    Vector = 0x04000000 | (((duration / 1000) - 1) << 16) | (value & 0xffff);
    write_register(ChannelReg, Vector, __LINE__);  // Write Datapoint to STG Memory


    duration %= 1000;
    vectors_used++;
  }

  if (duration > 0){
    // (value & 0xffff) sets bits 31 to 16 <- 0
    // The vector is thus the duration, shifted to bytes 1 and 2, and bytes 3 and 4 containing the value
    Vector = ((duration - 1) << 16) | (value & 0xffff);

    write_register(ChannelReg, Vector, __LINE__);  // Write Datapoint to STG Memory
    vectors_used++;
  }

  return vectors_used;
}
