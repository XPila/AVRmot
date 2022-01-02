// shr16.c - 16bit shift register (2x74595)

#include "shr16.h"
#include <avr/io.h>
#include "config.h"
#include "gpio_atmega32u4.h"


#define SHR16_LED_OUT_MSK ((SHR16_LED_MSK_0 << SHR16_LED_SHI_0) | (SHR16_LED_MSK_1 << SHR16_LED_SHI_1))
#define SHR16_ENADIR_OUT_MSK (SHR16_ENADIR_MSK << SHR16_ENADIR_SHI)


uint16_t shr16_v;


void shr16_init(void)
{
	GPIO_OUT(SHR16_DAT);
	GPIO_OUT(SHR16_LAT);
	GPIO_OUT(SHR16_CLK);
	GPIO_SET_L(SHR16_DAT);
	GPIO_SET_H(SHR16_LAT);
	GPIO_SET_L(SHR16_CLK);
	shr16_v = SHR16_INIT;
	shr16_write(shr16_v);
	shr16_write(shr16_v);
}

void shr16_write(uint16_t v)
{
	GPIO_SET_L(SHR16_LAT);
	asm("nop");
	for (uint16_t m = 0x8000; m; m >>= 1)
	{
		GPIO_SET(SHR16_DAT, m & v);
		GPIO_SET_H(SHR16_CLK);
		asm("nop");
		GPIO_SET_L(SHR16_CLK);
		asm("nop");
	}
	GPIO_SET_H(SHR16_LAT);
	asm("nop");
	shr16_v = v;
}

uint16_t shr16_get_led(void)
{
	return ((shr16_v >> SHR16_LED_SHI_0) & SHR16_LED_MSK_0) | (((shr16_v >> SHR16_LED_SHI_1) & SHR16_LED_MSK_1) << 8);
}

void shr16_set_led(uint16_t led)
{
	uint16_t v = (shr16_v & ~SHR16_LED_OUT_MSK);
	v |= ((led & SHR16_LED_MSK_0) << SHR16_LED_SHI_0);
	v |= (((led >> 8) & SHR16_LED_MSK_1) << SHR16_LED_SHI_1);
	shr16_write(v);
}

uint8_t shr16_get_enadir(void)
{
	return ((shr16_v >> SHR16_ENADIR_SHI) & SHR16_ENADIR_MSK);
}

void shr16_set_enadir(uint8_t enadir)
{
	uint16_t v = (shr16_v & ~SHR16_ENADIR_OUT_MSK);
	v |= ((enadir & SHR16_ENADIR_MSK) << SHR16_ENADIR_SHI);
	shr16_write(v);
}
