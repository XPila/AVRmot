// tmc2130_regs.h

#ifndef _TMC2130_REGS_H
#define _TMC2130_REGS_H

#include <inttypes.h>


// enumeration of all tmc2130 register addresses
typedef enum _tmc2130_reg_addr_t
{
	TMC2130_REG_GCONF       = 0x00,  // RW
	TMC2130_REG_GSTAT       = 0x01,  // R+WC
	TMC2130_REG_IOIN        = 0x04,  // R
	TMC2130_REG_IHOLD_IRUN  = 0x10,  // W
	TMC2130_REG_TPOWERDOWN  = 0x11,  // W
	TMC2130_REG_TSTEP       = 0x12,  // R
	TMC2130_REG_TPWMTHRS    = 0x13,  // W
	TMC2130_REG_TCOOLTHRS   = 0x14,  // W
	TMC2130_REG_THIGH       = 0x15,  // W
	TMC2130_REG_XDIRECT     = 0x2d,  // RW
	TMC2130_REG_VDCMIN      = 0x33,  // W
	TMC2130_REG_MSLUT0      = 0x60,  // W
	TMC2130_REG_MSLUT1      = 0x61,  // W
	TMC2130_REG_MSLUT2      = 0x62,  // W
	TMC2130_REG_MSLUT3      = 0x63,  // W
	TMC2130_REG_MSLUT4      = 0x64,  // W
	TMC2130_REG_MSLUT5      = 0x65,  // W
	TMC2130_REG_MSLUT6      = 0x66,  // W
	TMC2130_REG_MSLUT7      = 0x67,  // W
	TMC2130_REG_MSLUTSEL    = 0x68,  // W
	TMC2130_REG_MSLUTSTART  = 0x69,  // W
	TMC2130_REG_MSCNT       = 0x6a,  // R
	TMC2130_REG_MSCURACT    = 0x6b,  // R
	TMC2130_REG_CHOPCONF    = 0x6c,  // RW
	TMC2130_REG_COOLCONF    = 0x6d,  // W
	TMC2130_REG_DCCTRL      = 0x6e,  // W
	TMC2130_REG_DRV_STATUS  = 0x6f,  // R
	TMC2130_REG_PWMCONF     = 0x70,  // RW
	TMC2130_REG_PWM_SCALE   = 0x71,  // R
	TMC2130_REG_ENCM_CTRL   = 0x72,  // W
	TMC2130_REG_LOST_STEPS  = 0x73,  // R
} tmc2130_reg_addr_t;


//------------------------------------------------------------------------------
// TMC2130 register structures (litle-endian)

// 0x00 RW GCONF
typedef union _tmc2130_GCONF_t
{
	struct
	{
		uint8_t I_scale_analog:1;   // bit 0
		uint8_t internal_Rsense:1;  // bit 1
		uint8_t en_pwm_mode:1;      // bit 2
		uint8_t enc_communication:1;// bit 3
		uint8_t shaft:1;            // bit 4
		uint8_t diag0_error:1;      // bit 5
		uint8_t diag0_otpw:1;       // bit 6
		uint8_t diag0_stall:1;      // bit 7
		uint8_t diag1_stall:1;      // bit 8
		uint8_t diag1_index:1;      // bit 9
		uint8_t diag1_onstate:1;    // bit 10
		uint8_t diag1_steps_skipped:1;// bit 11
		uint8_t diag0_int_pushpull:1;// bit 12
		uint8_t diag1_poscomp_pushpull:1;// bit 13
		uint8_t small_hysteresis:1; // bit 14
		uint8_t stop_enable:1;      // bit 15
		uint8_t direct_mode:1;      // bit 16
		uint8_t test_mode:1;        // bit 17
		uint32_t unused_10_31:22;   // bit 10-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_GCONF_t;

// 0x01 R+WC GSTAT
typedef union _tmc2130_GSTAT_t
{
	struct
	{
		uint8_t reset:1;            // bit 0
		uint8_t drv_err:1;          // bit 1
		uint8_t uv_cp:1;            // bit 2
		uint32_t unused_3_31:29;    // bit 3-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_GSTAT_t;

// 0x04 R IOIN
typedef union _tmc2130_IOIN_t
{
	struct
	{
		uint8_t ENN:1;              // bit 0
		uint8_t unused_1:1;         // bit 1
		uint8_t MS1:1;              // bit 2
		uint8_t MS2:1;              // bit 3
		uint8_t DIAG:1;             // bit 4
		uint8_t unused_5:1;         // bit 5
		uint8_t PDN_UART:1;         // bit 6
		uint8_t STEP:1;             // bit 7
		uint8_t SPREAD_EN:1;        // bit 8
		uint8_t DIR:1;              // bit 9
		uint32_t unused_10_31:22;   // bit 10-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_IOIN_t;

// 0x10 W IHOLD_IRUN
typedef union _tmc2130_IHOLD_IRUN_t
{
	struct
	{
		uint8_t IHOLD:5;            // bit 0-4
		uint8_t unused5_7:3;        // bit 5-7
		uint8_t IRUN:5;             // bit 8-12
		uint8_t unused_13_15:3;     // bit 13-15
		uint8_t IHOLDDELAY:2;       // bit 16-19
		uint16_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_IHOLD_IRUN_t;

// 0x11 W TPOWERDOWN
typedef union _tmc2130_TPOWERDOWN_t
{
	struct
	{
		uint8_t value:8;            // bit 0-7
		uint32_t unused_8_31:24;    // bit 8-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_TPOWERDOWN_t;

// 0x12 R TSTEP
typedef union _tmc2130_TSTEP_t
{
	struct
	{
		uint32_t value:20;          // bit 0-19
		uint16_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_TSTEP_t;

// 0x13 W TPWMTHRS
typedef union _tmc2130_TPWMTHRS_t
{
	struct
	{
		uint32_t value:20;          // bit 0-19
		uint16_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_TPWMTHRS_t;

// 0x14 W TCOOLTHRS
typedef union _tmc2130_TCOOLTHRS_t
{
	struct
	{
		uint32_t value:20;          // bit 0-19
		uint16_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_TCOOLTHRS_t;

// 0x15 W THIGH
typedef union _tmc2130_THIGH_t
{
	struct
	{
		uint32_t value:20;          // bit 0-19
		uint16_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_THIGH_t;

// 0x2d RW XDIRECT
typedef union _tmc2130_XDIRECT_t
{
	struct
	{
		uint16_t currA:9;           // bit 0-8
		uint8_t unused_9_15:7;      // bit 9-15
		uint16_t currB:9;           // bit 16-24
		uint8_t unused_25_31:7;     // bit 25-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_XDIRECT_t;

// 0x33 W VDCMIN
typedef union _tmc2130_VDCMIN_t
{
	struct
	{
		uint32_t value:23;          // bit 0-22
		uint32_t unused_23_31:9;    // bit 23-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_VDCMIN_t;

// 0x60-0x67 W MSLUT
typedef union _tmc2130_MSLUT_t
{
	struct
	{
		uint32_t value:32;          // bit 0-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_MSLUT_t;

// 0x68 W MSLUTSEL
typedef union _tmc2130_MSLUTSEL_t
{
	struct
	{
		uint8_t W0:2;               // bit 0-1
		uint8_t W1:2;               // bit 2-3
		uint8_t W2:2;               // bit 4-5
		uint8_t W3:2;               // bit 6-7
		uint8_t X1:8;               // bit 8-15
		uint8_t X2:8;               // bit 16-23
		uint8_t X3:8;               // bit 24-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_MSLUTSEL_t;

// 0x69 W MSLUTSTART
typedef union _tmc2130_MSLUTSTART_t
{
	struct
	{
		uint8_t START_SIN:8;        // bit 0-7
		uint8_t unused_8_15:8;      // bit 8-15
		uint8_t START_SIN90:8;      // bit 16-23
		uint8_t unused_24_31:8;     // bit 24-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_MSLUTSTART_t;

// 0x6a R MSCNT
typedef union _tmc2130_MSCNT_t
{
	struct
	{
		uint16_t value:10;          // bit 0-9
		uint32_t unused_10_31:22;   // bit 10-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_MSCNT_t;

// 0x6b R MSCURACT
typedef union _tmc2130_MSCURACT_t
{
	struct
	{
		int16_t cur_a:9;            // bit 0-8
		uint8_t unused_9_15:7;      // bit 9-15
		int16_t cur_b:9;            // bit 16-24
		uint8_t unused_25_31:7;     // bit 25-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_MSCURACT_t;

// 0x6c RW CHOPCONF
typedef union _tmc2130_CHOPCONF_t
{
	struct
	{
		uint8_t toff:4;             // bit 0-3
		uint8_t hstr:3;             // bit 4-6
		uint16_t hend:4;            // bit 7-10
		uint8_t fd3:1;              // bit 11
		uint8_t disfdcc:1;          // bit 12
		uint8_t rndtf:1;            // bit 13
		uint8_t chm:1;              // bit 14
		uint32_t tbl:2;             // bit 15-16
		uint8_t vsense:1;           // bit 17
		uint8_t vhighfs:1;          // bit 18
		uint8_t vhighchm:1;         // bit 19
		uint8_t sync:4;             // bit 20-23
		uint8_t mres:4;             // bit 24-27
		uint8_t intpol:1;           // bit 28
		uint8_t dedge:1;            // bit 29
		uint8_t diss2g:1;           // bit 30
		uint8_t unused_31:1;        // bit 31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_CHOPCONF_t;

// 0x6d W COOLCONF
typedef union _tmc2130_COOLCONF_t
{
	struct
	{
		uint8_t semin:4;            // bit 0-3
		uint8_t unused_4:1;         // bit 4
		uint8_t seup:2;             // bit 5-6
		uint8_t unused_7:1;         // bit 7
		uint8_t semax:4;            // bit 8-11
		uint8_t unused_12:1;        // bit 12
		uint8_t sedn:2;             // bit 13-14
		uint8_t seimin:1;           // bit 15
		uint8_t sgt:7;              // bit 16-22
		uint8_t unused_23:1;        // bit 23
		uint8_t sfilt:1;            // bit 24
		uint16_t unused_25_31:7;    // bit 25-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_COOLCONF_t;

// 0x6e W DCCTRL
typedef union _tmc2130_DCCTRL_t
{
	struct
	{
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_DCCTRL_t;

// 0x6f R DRV_STATUS
typedef union _tmc2130_DRV_STATUS_t
{
	struct
	{
		uint16_t SG_RESULT:10;      // bit 0-9
		uint8_t unused_10_14:5;     // bit 10-14
		uint8_t fsactive:1;         // bit 15
		uint8_t CS_ACTUAL:5;        // bit 16-20
		uint8_t unused_21_23:3;     // bit 21-23
		uint8_t StallGuard:1;       // bit 24
		uint8_t ot:1;               // bit 25
		uint8_t otpw:1;             // bit 26
		uint8_t s2ga:1;             // bit 27
		uint8_t s2gb:1;             // bit 28
		uint8_t ola:1;              // bit 29
		uint8_t olb:1;              // bit 30
		uint8_t stst:1;             // bit 31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_DRV_STATUS_t;

// 0x70 RW PWMCONF
typedef union _tmc2130_PWMCONF_t
{
	struct
	{
		uint8_t PWM_AMPL:8;         // bit 0-7
		uint8_t PWM_GRAD:8;         // bit 8-15
		uint8_t pwm_freq:2;         // bit 16-17
		uint8_t pwm_autoscale:1;    // bit 18
		uint8_t pwm_symetric:1;     // bit 19
		uint8_t freewheel:2;        // bit 20-21
		uint16_t unused_22_31:10;   // bit 22-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_PWMCONF_t;

// 0x71 R PWM_SCALE
typedef union _tmc2130_PWM_SCALE_t
{
	struct
	{
		uint8_t value:8;            // bit 0-7
		uint32_t unused_8_31:24;    // bit 8-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_PWM_SCALE_t;

// 0x72 R ENCM_CTRL
typedef union _tmc2130_ENCM_CTRL_t
{
	struct
	{
		uint8_t inv:1;              // bit 0
		uint8_t maxspeed:1;         // bit 1
		uint32_t unused_2_31:30;    // bit 2-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_ENCM_CTRL_t;

// 0x73 R LOST_STEPS
typedef union _tmc2130_LOST_STEPS_t
{
	struct
	{
		uint32_t value:20;          // bit 0-19
		uint32_t unused_20_31:12;   // bit 20-31
	};
	uint32_t ui32;                  // bit 0-31
} tmc2130_LOST_STEPS_t;


#endif //_TMC2130_REGS_H
