#include "stimulus.h"
#include "MEA21_lib.h"
#include <math.h>

void AddLoop(int Channel, int Vectors, int Repeats);
void ClearChannel(int channel);
int AddDataPoint(int Channel, int duration, int value);

void UploadSine(int Channel, int Amplitude, int Period, int Repeats, int Stepsize)
{
  int yold = 0;
  int duration = 0;
  int datapoints = 0;
  volatile int i;
  int y;
  int vectors_used;

  vectors_used = 0;

  ClearChannel(Channel);
  for (i = 0; i < Period; i++)
    {
      y = Amplitude * sin((((double)i)/Period)*2*3.1415);

      if (abs(y - yold) > Stepsize)
        {
          vectors_used += AddDataPoint(Channel, duration, yold+0x8000);
          datapoints++;
          yold = y;
          duration = 1; // 20 us
        }
      else
        {
          duration++;
        }
    }
  vectors_used += AddDataPoint(Channel, duration, yold+0x8000);
  AddLoop(Channel, vectors_used, Repeats);

  // Create Sideband Information
  ClearChannel(Channel+1);
  vectors_used = 0;
  vectors_used += AddDataPoint(Channel+1, Period, 0x0019);
  AddLoop(Channel+1, vectors_used, Repeats);
  //	AddDataPoint(Channel+1, 10, 0x0009); // keep Electrode connected to ground after stimulation
}

void AddLoop(int Channel, int Vectors, int Repeats)
{
  Uint32 ChannelReg;
  Uint32 LoopVector;

  ChannelReg = 0x9f20 + Channel*4;

  if (Repeats > 1)
    {
      LoopVector = 0x10000000 | (Repeats << 16) | Vectors;
      WRITE_REGISTER(ChannelReg, LoopVector);
    }
}

void ClearChannel(int Channel)
{
  Uint32 ClearReg = 0x920c + Channel*0x20;
  WRITE_REGISTER(ClearReg, 0);      // Any write to this register clears the Channeldata
}


void SetSegment(int Channel, int Segment)
{
  Uint32 SegmentReg = 0x9200 + Channel*0x20;
  WRITE_REGISTER(SegmentReg, Segment);  // Any write to this register clears the Channeldata
}


int AddDataPoint(int Channel, int duration, int value)
{
  int vectors_used = 0;
  int	Vector;
  Uint32 ChannelReg = 0x9f20 + Channel*4;

  if (duration > 1000)
    {
      Vector = 0x04000000 | (((duration / 1000) - 1) << 16) | (value & 0xffff);
      WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
      duration %= 1000;
      vectors_used++;
    }

  if (duration > 0)
    {
      Vector = ((duration - 1) << 16) | (value & 0xffff);
      WRITE_REGISTER(ChannelReg, Vector);  // Write Datapoint to STG Memory
      vectors_used++;
    }

  return vectors_used;
}
