// tmc2130_hw_mmctl.c

#include "tmc2130_hw.h"
#include <avr/pgmspace.h>
#include "tmc2130.h"
#include "gpio_atmega2560.h"
#include "spi.h"
#include "config.h"


uint8_t tmc2130_ena;
uint8_t tmc2130_dir;


#ifdef MK3TRAPEZ // trapez MK3
	#define XY_MRES       6
#else
	#define XY_MRES       4
#endif


const tmc2130_slave_config_t PROGMEM tmc2130_cfg_x = {
	.mres = XY_MRES,
	.intpol = 1,
	.vsense = 1,
	.en_pwm_mode = 0,
	.IHOLD_IRUN = 20,
	.tcoolthrs = 400,
	.sgthrs = 5,
	.toff = 3,
	.hstr = 5,
	.hend = 1,
	.tbl = 1,
};

const tmc2130_slave_config_t PROGMEM tmc2130_cfg_y = {
	.mres = XY_MRES,
	.intpol = 1,
	.vsense = 1,
	.en_pwm_mode = 0,
	.IHOLD_IRUN = 24,
	.tcoolthrs = 400,
	.sgthrs = 5,
	.toff = 3,
	.hstr = 5,
	.hend = 1,
	.tbl = 1,
};

const tmc2130_slave_config_t PROGMEM tmc2130_cfg_z = {
	.mres = 4,
	.intpol = 1,
	.vsense = 0,
	.en_pwm_mode = 0,
	.IHOLD_IRUN = 18,
	.tcoolthrs = 400,
	.sgthrs = 5,
	.toff = 3,
	.hstr = 5,
	.hend = 1,
	.tbl = 1,
};

const tmc2130_slave_config_t PROGMEM tmc2130_cfg_e = {
	.mres = 5,
	.intpol = 1,
	.vsense = 1,
	.en_pwm_mode = 0,
	.IHOLD_IRUN = 24,
	.tcoolthrs = 400,
	.sgthrs = 5,
	.toff = 3,
	.hstr = 5,
	.hend = 1,
	.tbl = 1,
};


void tmc2130_init(void)
{
	GPIO_OUT(TMC2130_X_ENA);
	GPIO_OUT(TMC2130_Y_ENA);
	GPIO_OUT(TMC2130_Z_ENA);
	GPIO_OUT(TMC2130_E_ENA);
	GPIO_OUT(TMC2130_X_DIR);
	GPIO_OUT(TMC2130_Y_DIR);
	GPIO_OUT(TMC2130_Z_DIR);
	GPIO_OUT(TMC2130_E_DIR);
	GPIO_OUT(TMC2130_X_STP);
	GPIO_OUT(TMC2130_Y_STP);
	GPIO_OUT(TMC2130_Z_STP);
	GPIO_OUT(TMC2130_E_STP);
	GPIO_OUT(TMC2130_X_CS);
	GPIO_OUT(TMC2130_Y_CS);
	GPIO_OUT(TMC2130_Z_CS);
	GPIO_OUT(TMC2130_E_CS);
	GPIO_SET_H(TMC2130_X_ENA);
	GPIO_SET_H(TMC2130_Y_ENA);
	GPIO_SET_H(TMC2130_Z_ENA);
	GPIO_SET_H(TMC2130_E_ENA);
	GPIO_SET_L(TMC2130_X_DIR);
	GPIO_SET_L(TMC2130_Y_DIR);
	GPIO_SET_L(TMC2130_Z_DIR);
	GPIO_SET_L(TMC2130_E_DIR);
	GPIO_SET_L(TMC2130_X_STP);
	GPIO_SET_L(TMC2130_Y_STP);
	GPIO_SET_L(TMC2130_Z_STP);
	GPIO_SET_L(TMC2130_E_STP);
	GPIO_SET_H(TMC2130_X_CS);
	GPIO_SET_H(TMC2130_Y_CS);
	GPIO_SET_H(TMC2130_Z_CS);
	GPIO_SET_H(TMC2130_E_CS);
	tmc2130_slave_config_t cfg;
	memcpy_P(&cfg, &tmc2130_cfg_x, sizeof(tmc2130_slave_config_t));
	tmc2130_init_slave(0, &cfg);
	memcpy_P(&cfg, &tmc2130_cfg_y, sizeof(tmc2130_slave_config_t));
	tmc2130_init_slave(1, &cfg);
	memcpy_P(&cfg, &tmc2130_cfg_z, sizeof(tmc2130_slave_config_t));
	tmc2130_init_slave(2, &cfg);
	memcpy_P(&cfg, &tmc2130_cfg_e, sizeof(tmc2130_slave_config_t));
	tmc2130_init_slave(3, &cfg);
	tmc2130_ena = 0;
	tmc2130_dir = 0;
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
	switch (id)
	{
	case 0: GPIO_SET(TMC2130_X_CS, cs); break;
	case 1: GPIO_SET(TMC2130_Y_CS, cs); break;
	case 2: GPIO_SET(TMC2130_Z_CS, cs); break;
	case 3: GPIO_SET(TMC2130_E_CS, cs); break;
	}
}

uint8_t tmc2130_get_ena(void)
{
	return tmc2130_ena;
}

void tmc2130_set_ena(uint8_t mask)
{
	GPIO_SET(TMC2130_X_ENA, ~mask & 1);
	GPIO_SET(TMC2130_Y_ENA, ~mask & 2);
	GPIO_SET(TMC2130_Z_ENA, ~mask & 4);
	GPIO_SET(TMC2130_E_ENA, ~mask & 8);
	tmc2130_ena = mask;
}

uint8_t tmc2130_get_dir(void)
{
	return tmc2130_dir;
}

void tmc2130_set_dir(uint8_t mask)
{
	GPIO_SET(TMC2130_X_DIR, mask & 1);
	GPIO_SET(TMC2130_Y_DIR, mask & 2);
	GPIO_SET(TMC2130_Z_DIR, mask & 4);
	GPIO_SET(TMC2130_E_DIR, mask & 8);
	tmc2130_dir = mask;
}

void tmc2130_step(uint8_t mask)
{
	if (mask & 1) GPIO_SET_H(TMC2130_X_STP);
	if (mask & 2) GPIO_SET_H(TMC2130_Y_STP);
	if (mask & 4) GPIO_SET_H(TMC2130_Z_STP);
	if (mask & 8) GPIO_SET_H(TMC2130_E_STP);
	if (mask & 1) GPIO_SET_L(TMC2130_X_STP);
	if (mask & 2) GPIO_SET_L(TMC2130_Y_STP);
	if (mask & 4) GPIO_SET_L(TMC2130_Z_STP);
	if (mask & 8) GPIO_SET_L(TMC2130_E_STP);
}

void tmc2130_step_one(uint8_t id)
{
	switch (id)
	{
	case 0: GPIO_SET_H(TMC2130_X_STP); asm("nop"); GPIO_SET_L(TMC2130_X_STP); break;
	case 1: GPIO_SET_H(TMC2130_Y_STP); asm("nop"); GPIO_SET_L(TMC2130_Y_STP); break;
	case 2: GPIO_SET_H(TMC2130_Z_STP); asm("nop"); GPIO_SET_L(TMC2130_Z_STP); break;
	case 3: GPIO_SET_H(TMC2130_E_STP); asm("nop"); GPIO_SET_L(TMC2130_E_STP); break;
	}
}

uint8_t tmc2130_get_diag(void)
{
	return 0;
}
