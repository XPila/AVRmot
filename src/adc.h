// adc.h

#ifndef _ADC_H
#define _ADC_H

#include <inttypes.h>
#include "config.h"

#if (defined(ADC_CHAN_MSK) && defined(ADC_CHAN_CNT))

#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern uint16_t adc_val[ADC_CHAN_CNT];
extern uint16_t adc_sim_msk;


extern void adc_init(void);

extern void adc_set_mux(uint8_t ch);

extern void adc_cycle(void);

//calculate adc input voltage [mV] from 10-bit raw value
__inline uint16_t adc_calc_mV(uint16_t raw)
{
	return (uint16_t)(((uint32_t)raw * ADC_VREF) >> 10);
}

#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //(defined(ADC_CHAN_MSK) && defined(ADC_CHAN_CNT))

#endif //_ADC_H
