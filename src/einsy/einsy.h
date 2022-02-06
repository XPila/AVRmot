// einsy.h

#ifndef _EINSY_H
#define _EINSY_H

#include <inttypes.h>

// MCU pin assignment
//


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern void einsy_init(void);

extern uint16_t einsy_get_led(void);
extern void einsy_set_led(uint16_t led);

extern uint16_t einsy_get_adc(uint8_t index);


extern uint8_t gpio_get(uint8_t gpio);
extern void gpio_set(uint8_t gpio, uint8_t val);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_EINSY_H
