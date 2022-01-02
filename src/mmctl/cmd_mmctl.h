// cmd_mmctl.h
#ifndef _CMD_MMCTL_H
#define _CMD_MMCTL_H

#include "cmd.h"


enum
{
	MOD_ID_0 = 0xff,
	MOD_ID_X = 0x00,
	MOD_ID_Y = 0x01,
	MOD_ID_Z = 0x02,
};

enum
{
	MOD_MSK_0 = 0x0000,
	MOD_MSK_X = 0x0001,
	MOD_MSK_Y = 0x0002,
	MOD_MSK_Z = 0x0004,
	MOD_MSK_XY = 0x0003,
	MOD_MSK_XYZ = 0x0007,
};

enum
{
	CMD_ID_unk      = 0xff, // unknown commad
	CMD_ID_         = 0x00, // empty commad

	CMD_ID_RST      = 0x01, // reset
	CMD_ID_VER      = 0x02, // version
	CMD_ID_SER      = 0x03, // serial number

	CMD_ID_ERR      = 0x21, // error
	CMD_ID_LOG      = 0x22, // log
	CMD_ID_TST      = 0x23, // test

	CMD_ID_GPIO     = 0x41, // gpio
	CMD_ID_GPCF     = 0x42, // gpcf
	CMD_ID_ADC      = 0x43, // adc
	CMD_ID_SHR      = 0x44, // shr16
	CMD_ID_LED      = 0x45, // led state

	CMD_ID_ENA      = 0x81, // enabled state get/set
	CMD_ID_POS      = 0x82, // get/set position
	CMD_ID_RES      = 0x83, // get/set resolution
	CMD_ID_SR0      = 0x84, // get/set starting steprate
	CMD_ID_SRM      = 0x85, // get/set maximum steprate
	CMD_ID_ACC      = 0x86, // get/set acceleration
	CMD_ID_DEC      = 0x87, // get/set deceleration
	CMD_ID_MOA      = 0x88, // move absolute
	CMD_ID_MOR      = 0x89, // move relative
	CMD_ID_MOV      = 0x8a, // move at velocity

	CMD_ID_TMC      = 0xa1, // read/write tmc register
	CMD_ID_STEP     = 0xa2, // step
};


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_CMD_MMCTL_H
