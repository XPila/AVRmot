// shr16.h - 16bit shift register (2x74595)

#ifndef _SHR16_H
#define _SHR16_H

#include <inttypes.h>


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern uint16_t shr16_v;


extern void shr16_init(void);

extern void shr16_write(uint16_t v);

extern uint16_t shr16_get_led(void);

extern void shr16_set_led(uint16_t led);

extern void shr16_set_enadir(uint8_t enadir);

extern uint8_t shr16_get_enadir(void);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_SHR16_H
