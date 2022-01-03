// tmc2130_hw_mmctl.c

#include "tmc2130_hw.h"
#include <avr/pgmspace.h>
#include "tmc2130.h"
#include "gpio_atmega2560.h"
#include "spi.h"
#include "config.h"


#define TMC2130_ENA_MASK ((1 << TMC2130_X_ENA_SHR16) | (1 << TMC2130_Y_ENA_SHR16) | (1 << TMC2130_Z_ENA_SHR16))
#define TMC2130_DIR_MASK ((1 << TMC2130_X_DIR_SHR16) | (1 << TMC2130_Y_DIR_SHR16) | (1 << TMC2130_Z_DIR_SHR16))


const tmc2130_slave_config_t PROGMEM cfg2130 = {
	.mres = 4,
	.intpol = 1,
	.vsense = 1,
	.en_pwm_mode = 0,
	.IHOLD_IRUN = 8,
	.tcoolthrs = 400,
	.sgthrs = 5,
	.toff = 3,
	.hstr = 5,
	.hend = 1,
	.tbl = 1,
};

void tmc2130_init(void)
{
	tmc2130_slave_config_t cfg;
	memcpy_P(&cfg, &cfg2130, sizeof(tmc2130_slave_config_t));
//	shr16_set_ena(0); // TMC_X_ENA,TMC_Y_ENA,TMC_Z_ENA high (motor disabled)
//	shr16_set_dir(0); // TMC_X_DIR,TMC_Y_DIR,TMC_Z_DIR low (forward direction)
/*	GPIO_OUT(TMC2130_X_STP);
	GPIO_OUT(TMC2130_Y_STP);
	GPIO_OUT(TMC2130_Z_STP);
	GPIO_OUT(TMC2130_X_CS);
	GPIO_OUT(TMC2130_Y_CS);
	GPIO_OUT(TMC2130_Z_CS);
	GPIO_SET_L(TMC2130_X_STP);
	GPIO_SET_L(TMC2130_Y_STP);
	GPIO_SET_L(TMC2130_Z_STP);
	GPIO_SET_H(TMC2130_X_CS);
	GPIO_SET_H(TMC2130_Y_CS);
	GPIO_SET_H(TMC2130_Z_CS);
	tmc2130_init_slave(0, &cfg);
	tmc2130_init_slave(1, &cfg);
	tmc2130_init_slave(2, &cfg);*/
}

void tmc2130_spi_txrx(uint8_t* ptx, uint8_t* prx)
{
	SPCR = TMC2130_SPCR;
	SPSR = TMC2130_SPSR;
	for (int i = 0; i < 5; i++)
	{
		uint8_t rx = spi_txrx(ptx[i]);
		if (prx) prx[i] = rx;
	}
}

void tmc2130_set_cs(uint8_t id, uint8_t cs)
{
}

uint8_t tmc2130_get_ena(void)
{
	return 0;
}

void tmc2130_set_ena(uint8_t mask)
{
}

uint8_t tmc2130_get_dir(void)
{
	return 0;
}

void tmc2130_set_dir(uint8_t mask)
{
}

void tmc2130_step(uint8_t mask)
{
}

void tmc2130_step_one(uint8_t id)
{
}

uint8_t tmc2130_get_diag(void)
{
	return 0;
}

