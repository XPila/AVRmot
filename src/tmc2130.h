// tmc2130.h
#ifndef _TMC2130_H
#define _TMC2130_H

#include <inttypes.h>
#include <stdio.h>
#include "tmc2130_regs.h"

//#define TMC2130_TRACE


typedef union _tmc2130_SPI_STATUS_t
{
	struct
	{
		uint8_t reset_flag:1;      // bit 0
		uint8_t driver_error:1;    // bit 1
		uint8_t sg2:1;             // bit 2
		uint8_t standstill:1;      // bit 3
		uint8_t :4;                // bit 4-7
	};
	uint8_t ui8;                   // bit 0-7
} tmc2130_SPI_STATUS_t;

typedef struct _tmc2130_slave_config_t
{
	uint8_t mres:4;             // byte 0, bit 0-3            CHOPCONF
	uint8_t intpol:1;           // byte 0, bit 4              CHOPCONF
	uint8_t vsense:1;           // byte 0, bit 5              CHOPCONF
	uint8_t en_pwm_mode:1;      // byte 0, bit 6              GCONF
	uint8_t :1;                 // byte 0, bit 7
	uint8_t IHOLD_IRUN:5;       // byte 1, bit 0-4            IHOLD_IRUN
	uint8_t :3;                 // byte 1, bit 5-7
	uint32_t tcoolthrs:20;      // byte 2-3; byte 4, bit 0-3  TCOOLTHRS
	uint8_t :4;                 // byte 4, bit 4-7
	uint8_t sgthrs:8;           // byte 5, bit 0-7            SGTHRS
	uint8_t toff:4;             // byte 6, bit 0-3            CHOPCONF
	uint8_t hstr:3;             // byte 6, bit 4-6            CHOPCONF
	uint8_t :1;                 // byte 6, bit 7
	uint8_t hend:4;             // byte 7, bit 0-3            CHOPCONF
	uint8_t tbl:2;              // byte 7, bit 4-5            CHOPCONF
	uint8_t :2;                 // byte 7, bit 6-7
} tmc2130_slave_config_t;


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


#ifdef TMC2130_TRACE
// filestream to trace register read/write
extern FILE* tmc2130_trace;
#endif

// regiter read function
extern tmc2130_SPI_STATUS_t tmc2130_rdreg(uint8_t id, tmc2130_reg_addr_t reg, uint32_t* pdata);

// regiter write function
extern tmc2130_SPI_STATUS_t tmc2130_wrreg(uint8_t id, tmc2130_reg_addr_t reg, uint32_t data);

// slave init function
extern tmc2130_SPI_STATUS_t tmc2130_init_slave(uint8_t id, const tmc2130_slave_config_t* p);


#ifdef __cplusplus
}
#endif //__cplusplus


inline tmc2130_SPI_STATUS_t tmc2130_rd_gconf(uint8_t id, tmc2130_GCONF_t* pgconf)
{ return tmc2130_rdreg(id, TMC2130_REG_GCONF, &(pgconf->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_gconf(uint8_t id, tmc2130_GCONF_t gconf)
{ return tmc2130_wrreg(id, TMC2130_REG_GCONF, gconf.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_gstat(uint8_t id, tmc2130_GSTAT_t* pgstat)
{ return tmc2130_rdreg(id, TMC2130_REG_GSTAT, &(pgstat->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_gstat(uint8_t id, tmc2130_GSTAT_t gstat)
{ return tmc2130_wrreg(id, TMC2130_REG_GSTAT, gstat.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_ioin(uint8_t id, tmc2130_IOIN_t* pioin)
{ return tmc2130_rdreg(id, TMC2130_REG_IOIN, &(pioin->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_ihold_irun(uint8_t id, tmc2130_IHOLD_IRUN_t ihold_irun)
{ return tmc2130_wrreg(id, TMC2130_REG_IHOLD_IRUN, ihold_irun.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_tpowerdown(uint8_t id, tmc2130_TPOWERDOWN_t tpowerdown)
{ return tmc2130_wrreg(id, TMC2130_REG_TPOWERDOWN, tpowerdown.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_tstep(uint8_t id, tmc2130_TSTEP_t* ptstep)
{ return tmc2130_rdreg(id, TMC2130_REG_TSTEP, &(ptstep->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_tppwmthrs(uint8_t id, tmc2130_TPWMTHRS_t tpwmthrs)
{ return tmc2130_wrreg(id, TMC2130_REG_TPWMTHRS, tpwmthrs.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_tcoolthrs(uint8_t id, tmc2130_TCOOLTHRS_t tcoolthrs)
{ return tmc2130_wrreg(id, TMC2130_REG_TCOOLTHRS, tcoolthrs.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_thigh(uint8_t id, tmc2130_THIGH_t thigh)
{ return tmc2130_wrreg(id, TMC2130_REG_THIGH, thigh.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_xdirect(uint8_t id, tmc2130_XDIRECT_t* pxdirect)
{ return tmc2130_rdreg(id, TMC2130_REG_XDIRECT, &(pxdirect->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_xdirect(uint8_t id, tmc2130_XDIRECT_t xdirect)
{ return tmc2130_wrreg(id, TMC2130_REG_XDIRECT, xdirect.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_vdcmin(uint8_t id, tmc2130_VDCMIN_t vdcmin)
{ return tmc2130_wrreg(id, TMC2130_REG_VDCMIN, vdcmin.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_mslut(uint8_t id, uint8_t index, tmc2130_MSLUT_t mslut)
{ return tmc2130_wrreg(id, TMC2130_REG_MSLUT0 + index, mslut.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_mslutsel(uint8_t id, tmc2130_MSLUTSEL_t mslutsel)
{ return tmc2130_wrreg(id, TMC2130_REG_MSLUTSEL, mslutsel.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_mslutstart(uint8_t id, tmc2130_MSLUTSTART_t mslutstart)
{ return tmc2130_wrreg(id, TMC2130_REG_MSLUTSTART, mslutstart.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_mscnt(uint8_t id, tmc2130_MSCNT_t* pmscnt)
{ return tmc2130_rdreg(id, TMC2130_REG_MSCNT, &(pmscnt->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_mscuract(uint8_t id, tmc2130_MSCURACT_t* pmscuract)
{ return tmc2130_rdreg(id, TMC2130_REG_MSCURACT, &(pmscuract->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_chopconf(uint8_t id, tmc2130_CHOPCONF_t* pchopconf)
{ return tmc2130_rdreg(id, TMC2130_REG_CHOPCONF, &(pchopconf->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_chopconf(uint8_t id, tmc2130_CHOPCONF_t chopconf)
{ return tmc2130_wrreg(id, TMC2130_REG_CHOPCONF, chopconf.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_coolconf(uint8_t id, tmc2130_COOLCONF_t coolconf)
{ return tmc2130_wrreg(id, TMC2130_REG_COOLCONF, coolconf.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_dcctrl(uint8_t id, tmc2130_DCCTRL_t dcctrl)
{ return tmc2130_wrreg(id, TMC2130_REG_DCCTRL, dcctrl.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_drv_status(uint8_t id, tmc2130_DRV_STATUS_t* pdrv_status)
{ return tmc2130_rdreg(id, TMC2130_REG_DRV_STATUS, &(pdrv_status->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_pwmconf(uint8_t id, tmc2130_PWMCONF_t* ppwmconf)
{ return tmc2130_rdreg(id, TMC2130_REG_PWMCONF, &(ppwmconf->ui32)); }

inline tmc2130_SPI_STATUS_t tmc2130_wr_pwmconf(uint8_t id, tmc2130_PWMCONF_t pwmconf)
{ return tmc2130_wrreg(id, TMC2130_REG_PWMCONF, pwmconf.ui32); }

inline tmc2130_SPI_STATUS_t tmc2130_rd_pwm_scale(uint8_t id, tmc2130_PWM_SCALE_t* ppwm_scale)
{ return tmc2130_rdreg(id, TMC2130_REG_PWM_SCALE, &(ppwm_scale->ui32)); }


#endif //_TMC2130_H
