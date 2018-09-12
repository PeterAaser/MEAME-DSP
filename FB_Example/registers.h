#ifndef REGISTERS_H_
#define REGISTERS_H_

/*
  Some registers are named twice, with and without number prefix.
  When we want to set all electrode mode registers we do so in a
  for loop which becomes clearer when using the first register
  in a block without the numeric post-fix since they will all be written to
 */

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// HS1
#define HS1_BASE           (0x8000)
#define BLANKING_EN1       (HS1_BASE + 0x150)
#define BLANKING_EN2       (HS1_BASE + 0x154)


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// MAILBOX
#define MAIL_BASE             (0x1000)
#define MAIL_BASE_END         (0x1FFC)

// When a call is completed the value at 0x1000 should be incremented by 1
#define SLAVE_INSTRUCTION_ID  (0x1000)
#define MASTER_INSTRUCTION_ID (0x1004)

#define INSTRUCTION_TYPE      (0x1008)

#define STIM_QUEUE_BASE       (0x100c)
#define STIM_QUEUE_RUNNING    (STIM_QUEUE_BASE + 0x0)

#define STIM_QUEUE_GROUP      (STIM_QUEUE_BASE + 0x4)
#define STIM_QUEUE_PERIOD     (STIM_QUEUE_BASE + 0x8)
#define STIM_QUEUE_ELEC0      (STIM_QUEUE_BASE + 0xc)
#define STIM_QUEUE_ELEC1      (STIM_QUEUE_BASE + 0x10)

#define CFG_DEBUG_ELEC0       (STIM_QUEUE_BASE + 0x14)
#define CFG_DEBUG_ELEC1       (STIM_QUEUE_BASE + 0x18)

#define CFG_DEBUG_MODE0       (STIM_QUEUE_BASE + 0x1C)
#define CFG_DEBUG_MODE1       (STIM_QUEUE_BASE + 0x20)
#define CFG_DEBUG_MODE2       (STIM_QUEUE_BASE + 0x24)
#define CFG_DEBUG_MODE3       (STIM_QUEUE_BASE + 0x28)

#define CFG_DEBUG_DAC0       (STIM_QUEUE_BASE + 0x2c)
#define CFG_DEBUG_DAC1       (STIM_QUEUE_BASE + 0x30)
#define CFG_DEBUG_DAC2       (STIM_QUEUE_BASE + 0x34)
#define CFG_DEBUG_DAC3       (STIM_QUEUE_BASE + 0x38)

#define LOG_START             (0x1100)
#define LOG_END               (0x1F00)
#define LOG_ENTRIES           (0x1FF0)


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// STG
#define STIM_BASE          (0x9000)

#define ELECTRODE_ENABLE   (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE1  (STIM_BASE + 0x158)
#define ELECTRODE_ENABLE2  (STIM_BASE + 0x15c)

#define ELECTRODE_MODE     (STIM_BASE + 0x120)
#define ELECTRODE_MODE1    (STIM_BASE + 0x120)
#define ELECTRODE_MODE2    (STIM_BASE + 0x124)
#define ELECTRODE_MODE3    (STIM_BASE + 0x128)
#define ELECTRODE_MODE4    (STIM_BASE + 0x12c)

#define ELECTRODE_DAC_SEL  (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL1 (STIM_BASE + 0x160)
#define ELECTRODE_DAC_SEL2 (STIM_BASE + 0x164)
#define ELECTRODE_DAC_SEL3 (STIM_BASE + 0x168)
#define ELECTRODE_DAC_SEL4 (STIM_BASE + 0x16c)

#define TRIGGER_REPEAT1    (STIM_BASE + 0x190)
#define TRIGGER_REPEAT2    (STIM_BASE + 0x194)
#define TRIGGER_REPEAT3    (STIM_BASE + 0x198)


#define STG_MWRITE1  (STIM_BASE + 0xf20)
#define STG_MWRITE2  (STIM_BASE + 0xf24)
#define STG_MWRITE3  (STIM_BASE + 0xf28)
#define STG_MWRITE4  (STIM_BASE + 0xf2c)
#define STG_MWRITE5  (STIM_BASE + 0xf30)
#define STG_MWRITE6  (STIM_BASE + 0xf34)
#define STG_MWRITE7  (STIM_BASE + 0xf38)
#define STG_MWRITE8  (STIM_BASE + 0xf3c)


#define STG_MWRITE1  (STIM_BASE + 0xf20)
#define STG_MWRITE2  (STIM_BASE + 0xf24)
#define STG_MWRITE3  (STIM_BASE + 0xf28)
#define STG_MWRITE4  (STIM_BASE + 0xf2c)
#define STG_MWRITE5  (STIM_BASE + 0xf30)
#define STG_MWRITE6  (STIM_BASE + 0xf34)
#define STG_MWRITE7  (STIM_BASE + 0xf38)
#define STG_MWRITE8  (STIM_BASE + 0xf3c)


#define STG_MCLEAR_AND_WRITE1  (STIM_BASE + 0xf40)
#define STG_MCLEAR_AND_WRITE2  (STIM_BASE + 0xf44)
#define STG_MCLEAR_AND_WRITE3  (STIM_BASE + 0xf48)
#define STG_MCLEAR_AND_WRITE4  (STIM_BASE + 0xf4c)
#define STG_MCLEAR_AND_WRITE5  (STIM_BASE + 0xf50)
#define STG_MCLEAR_AND_WRITE6  (STIM_BASE + 0xf54)
#define STG_MCLEAR_AND_WRITE7  (STIM_BASE + 0xf58)
#define STG_MCLEAR_AND_WRITE8  (STIM_BASE + 0xf5c)


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///// TRIGGERS

#define TRIGGER_CTRL_BASE  (0x0200)
#define TRIGGER_CTRL       (TRIGGER_CTRL_BASE)

#define MANUAL_TRIGGER     (TRIGGER_CTRL_BASE + 0x14)

/* and so on... */

#endif /* REGISTERS_H_ */
