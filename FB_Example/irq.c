#include <csl.h>
#include <cslr_tmr.h>
#include <cslr_gpio.h>
#include <cslr_chip.h>
#include <cslr_edma3cc.h>
#include <soc.h>
#include <c6x.h>

#include "main.h"
#include "stimpack.h"
#include "old_stim.h"
#include "registers.h"
#include "irq.h"
#include "MEA21_lib.h"

int num_tr_cross[HS1_CHANNELS/2];
int last_tr_cross[HS1_CHANNELS/2];

volatile Uint32 reg_written_address;
volatile Uint32 reg_value;

volatile Uint32 counter = 0;
volatile Uint32 write_req_id = 0;
volatile Uint32 write_addr = 0;
volatile Uint32 write_val = 0;
volatile Uint32 read_req_id = 0;
volatile Uint32 read_addr = 0;
volatile Uint32 read_value = 0;

// Mailbox write interrupt
// use "#define USE_MAILBOX_IRQ" in global.h to enable this interrupt


interrupt void interrupt8(void)
{
#ifdef INTERRUPTS

  //////////////////////////
  //////////////////////////
  ///// DEBUG STUFF
  reg_written_address = (READ_REGISTER(0x428)) + 0x1000;
  reg_value           = READ_REGISTER(reg_written_address);


  /**
     PING TEST
   */
  if(reg_written_address == PING_SEND){
    WRITE_REGISTER(PING_READ, reg_value);
  }
  else{
    WRITE_REGISTER(PING_READ, reg_value);
  }
  //////////////////////////
  //////////////////////////


  /**
     WRITE
   */
  if(reg_written_address == WRITE_REQ_ID){
    write_req_id = READ_REGISTER(reg_written_address);
    write_addr   = READ_REGISTER(WRITE_ADDRESS);
    write_val    = READ_REGISTER(WRITE_VALUE);

    WRITE_REGISTER(WRITE_ADDRESS, WRITE_VALUE);
    WRITE_REGISTER(WRITE_ACK_ID,  write_req_id);

    WRITE_REGISTER(DEBUG7, write_addr);
    WRITE_REGISTER(DEBUG8, write_val);
    WRITE_REGISTER(DEBUG9, write_req_id);
  }


  /**
     READ
  */
  if(reg_written_address == READ_REQ_ID){
    read_req_id = READ_REGISTER(reg_written_address);

    WRITE_REGISTER(DEBUG3, read_req_id);

    read_addr   = READ_REGISTER(READ_ADDRESS);
    read_value  = READ_REGISTER(read_addr);

    WRITE_REGISTER(READ_VALUE, read_value);
    WRITE_REGISTER(READ_ACK_ID, read_req_id);
  }


  /**
     CLEAR
   */
  if(reg_written_address == CLEAR){
    WRITE_REGISTER(DEBUG1, 0x0);
    WRITE_REGISTER(DEBUG2, 0x0);
    WRITE_REGISTER(DEBUG3, 0x0);
    WRITE_REGISTER(DEBUG4, 0x0);
    WRITE_REGISTER(DEBUG5, 0x0);
    WRITE_REGISTER(DEBUG6, 0x0);
    WRITE_REGISTER(DEBUG7, 0x0);
    WRITE_REGISTER(DEBUG8, 0x0);
    WRITE_REGISTER(DEBUG9, 0x0);

    WRITE_REGISTER(READ_ACK_ID,   0x0);
    WRITE_REGISTER(READ_ADDRESS,  0x0);
    WRITE_REGISTER(READ_VALUE,    0x0);

    WRITE_REGISTER(WRITE_ACK_ID,  0x0);
    WRITE_REGISTER(WRITE_ADDRESS, 0x0);
    WRITE_REGISTER(WRITE_VALUE,   0x0);
  }

  counter++;

#endif
}

// FPGA data available (do not use)
interrupt void interrupt4(void)
{
}

// I2C Interrupt
interrupt void interrupt5(void)
{
  //handle_i2c_commands();
}

// DMA finished Interrupt
interrupt void interrupt6(void)
{




// Lots of bullshit that I have no clue what does
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
  static int timestamp = 0;

  CSL_Edma3ccRegsOvly edma3ccRegs = (CSL_Edma3ccRegsOvly)CSL_EDMA3CC_0_REGS;

  Int32* restrict adc_i_p = &adc_intern[0];
  Int32* restrict HS1_Data_p = (Int32 *)&MeaData[HS1_DATA_OFFSET];


  CSL_FINST(edma3ccRegs->ICRH, EDMA3CC_ICRH_I52, CLEAR);

  WRITE_REGISTER(0x0310, 0x1);


  if ((int)MeaData[HS1_DATA_OFFSET + 0] > (int)threshold)
    {
      MonitorData[0] = MeaData[HS1_DATA_OFFSET + 0];
      WRITE_REGISTER(FEEDBACK_REGISTER, 1);
    }
  else
    {
      MonitorData[0] = 0; //timestamp;
      WRITE_REGISTER(FEEDBACK_REGISTER, 0);
    }
  MonitorData[1] = MeaData[HS1_DATA_OFFSET + 0];
  CSL_FINST(edma3ccRegs->ESRH, EDMA3CC_ESRH_E53, SET);

  WRITE_REGISTER(0x0310, 0x0);

  timestamp++;

  // oldStimPack(0);

  //TODO reenable
  // run_stimpack();

  ////////////////////////////////////////
  ////////////////////////////////////////
  ////////////////////////////////////////
}


// timer irq
interrupt void interrupt7(void)
{
  static int led = 0;
  CSL_FINS(gpioRegs->OUT_DATA, GPIO_OUT_DATA_OUT2, led); // LED
  led = 1 - led;
}
