#ifndef _SYS_H
#define _SYS_H

#include <inttypes.h>


#define STATE_INI  0 //initializing
#define STATE_IDL  1 //idle
#define STATE_OPE  2 //operational
#define STATE_SER  3 //service
#define STATE_ERR -1 //error

// get state of signal (main loop or interrupt)
#define SIG_GET(id) (sys_signals & (1 << id))
// set state of signal (interrupt only)
#define SIG_SET(id) (sys_signals |= (1 << id))
// get state of signal (main loop only)
#define SIG_CLR(id) (sys_signals &= ~(1 << id))


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)

// system state
extern int8_t sys_state;

// signals from interrupt to main loop
extern uint8_t sys_signals;

extern void sys_reset(void);

extern void sys_init_wdt(void);

extern void sys_setup_osc(void);

extern void sys_disable_usb(void);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)
#endif //_SYS_H
