// mmctl.h

#ifndef _MMCTL_H
#define _MMCTL_H

#include <inttypes.h>

// MCU pin assignment
//
// TMC2130_X is U7, connector "1"
// TMC2130_X_CS  - D11 - PB7
// TMC2130_X_STP - D12 - PD6
//
// TMC2130_Y is U6, connector "2"
// TMC2130_Y_CS  - D6  - PD7
// TMC2130_Y_STP - D4  - PD4
//
// TMC2130_Z is U5, connector "3"
// TMC2130_Z_CS  - D5  - PC6
// TMC2130_Z_STP - D8  - PB4
//
// SHR16 is 2x 74595 (controlled by DATA, LATCH and CLOCK signals)
// SHR16_D   - D9  - PB5
// SHR16_L   - D10 - PB6
// SHR16_C   - D13 - PC7


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern void mmctl_init(void);

extern uint16_t mmctl_get_led(void);
extern void mmctl_set_led(uint16_t led);

extern uint16_t mmctl_get_adc(uint8_t index);


extern uint8_t gpio_get(uint8_t gpio);
extern void gpio_set(uint8_t gpio, uint8_t val);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_MMCTL_H
