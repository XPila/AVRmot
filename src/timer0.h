// timer0.h

#ifndef _TIMER0_H
#define _TIMER0_H

#include <inttypes.h>
#include "config.h"


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern volatile uint32_t timer0_ms;

extern void timer0_init(void);
extern uint32_t timer0_us(void);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)
#endif //_TIMER0_H
