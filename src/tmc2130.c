// tmc2130.c

#include "tmc2130.h"
#include <stdio.h>


#ifdef __AVR__
#include <avr/pgmspace.h>
#else // __AVR__
#define PSTR(s) s
#define PROGMEM
#define fprintf_P fprintf
#endif // __AVR__


#ifdef TMC2130_TRACE
FILE* tmc2130_trace = 0;
#define TRACE_RDREG(id, reg, txrx) \
	if (tmc2130_trace) fprintf_P(tmc2130_trace, PSTR("#TMC%d RD 0x%02x %c%c%c%c 0x%02x%02x%02x%02x %S\n"), id, reg, \
			(txrx[0]&1)?'R':'-', (txrx[0]&2)?'E':'-', (txrx[0]&4)?'G':'-', (txrx[0]&8)?'S':'-', \
			txrx[1],txrx[2],txrx[3],txrx[4], tmc2130_regname(reg))
#define TRACE_WRREG(id, reg, txrx, data) \
	if (tmc2130_trace) fprintf_P(tmc2130_trace, PSTR("#TMC%d WR 0x%02x %c%c%c%c 0x%08lx %S\n"), id, reg, \
			(txrx[0]&1)?'R':'-', (txrx[0]&2)?'E':'-', (txrx[0]&4)?'G':'-', (txrx[0]&8)?'S':'-', \
			(uint32_t)data, tmc2130_regname(reg))
const char* PROGMEM tmc2130_regname(tmc2130_reg_addr_t reg);
#else // TMC2130_TRACE
#define TRACE_RDREG(id, reg, txrx)
#define TRACE_WRREG(id, reg, txrx, data)
#endif // TMC2130_TRACE


extern void tmc2130_spi_txrx(uint8_t* ptx, uint8_t* prx);
extern void tmc2130_set_cs(uint8_t id, uint8_t cs);


tmc2130_SPI_STATUS_t tmc2130_rdreg(uint8_t id, tmc2130_reg_addr_t reg, uint32_t* pdata)
{
	uint8_t txrx[5] = {reg, 0, 0, 0, 0};
	tmc2130_set_cs(id, 0);
	tmc2130_spi_txrx(txrx, txrx);
	tmc2130_set_cs(id, 1);
	tmc2130_set_cs(id, 0);
	tmc2130_spi_txrx(txrx, txrx);
	tmc2130_set_cs(id, 1);
	if (pdata)
		*pdata =
			(((uint32_t)txrx[1]) << 24) |
			(((uint32_t)txrx[2]) << 16) |
			(((uint32_t)txrx[3]) << 8) |
			((uint32_t)txrx[4]);
	tmc2130_SPI_STATUS_t spi_status = { .ui8 = txrx[0] };
	TRACE_RDREG(id, reg, txrx);
	return spi_status;
}

tmc2130_SPI_STATUS_t tmc2130_wrreg(uint8_t id, tmc2130_reg_addr_t reg, uint32_t data)
{
	uint8_t txrx[5] = {reg | 0x80, data >> 24, (data >> 16) & 0xff, (data >> 8) & 0xff, data & 0xff};
	tmc2130_set_cs(id, 0);
	tmc2130_spi_txrx(txrx, txrx);
	tmc2130_set_cs(id, 1);
	tmc2130_SPI_STATUS_t spi_status = { .ui8 = txrx[0] };
	TRACE_WRREG(id, reg, txrx, data);
	return spi_status;
}

tmc2130_SPI_STATUS_t tmc2130_init_slave(uint8_t id, const tmc2130_slave_config_t* p)
{
	tmc2130_GCONF_t gconf = { .en_pwm_mode = p->en_pwm_mode };
	tmc2130_CHOPCONF_t chopconf = { .toff = p->toff, .hstr = p->hstr, .hend = p->hend, .tbl = p->tbl, .vsense = p->vsense, .mres = p->mres, .intpol = p->intpol };
	tmc2130_PWMCONF_t pwmconf = { .PWM_AMPL = 150, .PWM_GRAD = 10, .pwm_freq = 0, .pwm_autoscale = 1 };
	tmc2130_COOLCONF_t coolconf = { .sgt = p->sgthrs };
	tmc2130_IHOLD_IRUN_t ihold_irun = { .IHOLD = p->IHOLD_IRUN, .IRUN = p->IHOLD_IRUN, .IHOLDDELAY = 1 };
	tmc2130_TPOWERDOWN_t tpowerdown = { .value = 128 };
	tmc2130_TCOOLTHRS_t tcoolthrs = { .value = p->tcoolthrs };
	tmc2130_GSTAT_t gstat = { .reset = 1, .drv_err = 1, .uv_cp = 1 };
	tmc2130_DRV_STATUS_t drv_status;
	tmc2130_wr_gconf(id, gconf);
	tmc2130_wr_gconf(id, gconf); // write twice because of undefined initial state
	tmc2130_wr_gstat(id, gstat);
	tmc2130_wr_chopconf(id, chopconf);
	tmc2130_wr_pwmconf(id, pwmconf);
	tmc2130_wr_coolconf(id, coolconf);
	tmc2130_wr_ihold_irun(id, ihold_irun);
	tmc2130_wr_tpowerdown(id, tpowerdown);
	tmc2130_wr_tcoolthrs(id, tcoolthrs);
	tmc2130_rd_gstat(id, &gstat);
	tmc2130_SPI_STATUS_t spi_status = tmc2130_rd_drv_status(id, &drv_status);
	return spi_status;
}


#ifdef TMC2130_TRACE

const char* PROGMEM tmc2130_regname(tmc2130_reg_addr_t reg)
{
	switch (reg)
	{
	case TMC2130_REG_GCONF:        return PSTR("GCONF");
	case TMC2130_REG_GSTAT:        return PSTR("GSTAT");
	case TMC2130_REG_IOIN:         return PSTR("IOIN");
	case TMC2130_REG_IHOLD_IRUN:   return PSTR("IHOLD_IRUN");
	case TMC2130_REG_TPOWERDOWN:   return PSTR("TPOWERDOWN");
	case TMC2130_REG_TSTEP:        return PSTR("TSTEP");
	case TMC2130_REG_TPWMTHRS:     return PSTR("TPWMTHRS");
	case TMC2130_REG_TCOOLTHRS:    return PSTR("TCOOLTHRS");
	case TMC2130_REG_THIGH:        return PSTR("THIGH");
	case TMC2130_REG_XDIRECT:      return PSTR("XDIRECT");
	case TMC2130_REG_VDCMIN:       return PSTR("VDCMIN");
	case TMC2130_REG_MSLUT0:       return PSTR("MSLUT0");
	case TMC2130_REG_MSLUT1:       return PSTR("MSLUT1");
	case TMC2130_REG_MSLUT2:       return PSTR("MSLUT2");
	case TMC2130_REG_MSLUT3:       return PSTR("MSLUT3");
	case TMC2130_REG_MSLUT4:       return PSTR("MSLUT4");
	case TMC2130_REG_MSLUT5:       return PSTR("MSLUT5");
	case TMC2130_REG_MSLUT6:       return PSTR("MSLUT6");
	case TMC2130_REG_MSLUT7:       return PSTR("MSLUT7");
	case TMC2130_REG_MSLUTSEL:     return PSTR("MSLUTSEL");
	case TMC2130_REG_MSLUTSTART:   return PSTR("MSLUTSTART");
	case TMC2130_REG_MSCNT:        return PSTR("MSCNT");
	case TMC2130_REG_MSCURACT:     return PSTR("MSCURACT");
	case TMC2130_REG_CHOPCONF:     return PSTR("CHOPCONF");
	case TMC2130_REG_COOLCONF:     return PSTR("COOLCONF");
	case TMC2130_REG_DCCTRL:       return PSTR("DCCTRL");
	case TMC2130_REG_DRV_STATUS:   return PSTR("DRV_STATUS");
	case TMC2130_REG_PWMCONF:      return PSTR("PWMCONF");
	case TMC2130_REG_PWM_SCALE:    return PSTR("PWM_SCALE");
	case TMC2130_REG_ENCM_CTRL:    return PSTR("ENCM_CTRL");
	case TMC2130_REG_LOST_STEPS:   return PSTR("LOST_STEPS");
	}
	return "";
}

#endif // TMC2130_TRACE
