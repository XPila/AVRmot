// tmc2130_mmctl.h
#ifndef _TMC2130_MMCTL_H
#define _TMC2130_MMCTL_H

#include <inttypes.h>


#if defined(__cplusplus)
extern "C" {
#endif // defined(__cplusplus)


extern void tmc2130_init(void);
extern void tmc2130_spi_txrx(uint8_t* ptx, uint8_t* prx);
extern void tmc2130_set_cs(uint8_t id, uint8_t cs);
extern uint8_t tmc2130_get_ena(void);
extern void tmc2130_set_ena(uint8_t mask);
extern uint8_t tmc2130_get_dir(void);
extern void tmc2130_set_dir(uint8_t mask);
extern void tmc2130_step(uint8_t mask);
extern void tmc2130_step_one(uint8_t id);
extern uint8_t tmc2130_get_diag(void);


#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

#endif // _TMC2130_MMCTL_H
