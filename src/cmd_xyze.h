// cmd_xyze.h

#ifndef _CMD_XYZE_H
#define _CMD_XYZE_H

#include "cmd.h"


enum
{
	MOD_ID_X = 0x00,
	MOD_ID_Y = 0x01,
	MOD_ID_Z = 0x02,
	MOD_ID_E = 0x03,
};

enum
{
	MOD_MSK_X    = 0x0001,
	MOD_MSK_Y    = 0x0002,
	MOD_MSK_Z    = 0x0004,
	MOD_MSK_E    = 0x0008,
	MOD_MSK_XY   = 0x0003,
	MOD_MSK_XYZ  = 0x0007,
	MOD_MSK_XYZE = 0x000f,
};

enum
{
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
	CMD_ID_HOME     = 0xa3, // home
};


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_CMD_XYZE_H
