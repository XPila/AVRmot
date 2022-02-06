// cmd_mmctl.h

#ifndef _CMD_MMCTL_H
#define _CMD_MMCTL_H

#include "cmd.h"
#include "cmd_xyze.h"


enum
{

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

};


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)


#endif //_CMD_MMCTL_H
