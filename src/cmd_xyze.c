// cmd_xyz.c
#include "cmd_xyze.h"
#include <util/delay.h>
#include "tmc2130.h"
#include "tmc2130_hw.h"
#include "st4.h"


int8_t cmd_do_mod_wout_args_xyz(uint8_t mod_id, char pref, uint8_t cmd_id)
{
	int8_t ret = CMD_OK;
//	uint8_t val;
	if ((mod_id >= MOD_ID_X) && (mod_id <= MOD_ID_Z))
	{
		uint8_t axis = (mod_id - MOD_ID_X);
		uint8_t mask = (1 << axis);
		if (pref == '!')
		{
			switch (cmd_id)
			{
			}
		}
		else if (pref == '?')
		{
			switch (cmd_id)
			{
			case CMD_ID_:
				return ret;
			case CMD_ID_ENA:
				cmd_print_ui8((tmc2130_get_ena() & mask)?1:0);
				return ret;
			case CMD_ID_POS:
				cmd_print_i32(st4_axis[axis].pos);
				return ret;
			case CMD_ID_RES:
				cmd_print_i16(st4_axis[axis].res);
				return ret;
			case CMD_ID_SR0:
				cmd_print_i16(st4_axis[axis].sr0);
				return ret;
			case CMD_ID_SRM:
				cmd_print_i16(st4_axis[axis].srm);
				return ret;
			case CMD_ID_ACC:
				cmd_print_i16(st4_axis[axis].acc);
				return ret;
			case CMD_ID_DEC:
				cmd_print_i16(st4_axis[axis].dec);
				return ret;
			}
		}
	}
	return CMD_ER_SYN;
}

int8_t cmd_do_mod_with_args_xyz(uint8_t mod_id, char pref, uint8_t cmd_id, char* pstr)
{
	int8_t ret;
	int8_t n;
	var_num_t val0;
	var_num_t val1;
//	var_num_t val2;
//	var_num_t val3;
	if ((mod_id >= MOD_ID_X) && (mod_id <= MOD_ID_Z))
	{
		uint8_t axis = (mod_id - MOD_ID_X);
		uint8_t mask = (1 << axis);
		if (pref == '!')
			switch (cmd_id)
			{
			case CMD_ID_ENA:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				tmc2130_set_ena((tmc2130_get_ena() & ~mask) | (val0.ui8?mask:0));
				return ret;
			case CMD_ID_POS:
				if ((ret = cmd_scan_i32(pstr, &val0.i32)) < 0) return ret;
				st4_axis[axis].pos = val0.i32;
				return ret;
			case CMD_ID_RES:
				if ((ret = cmd_scan_ui16(pstr, &val0.ui16)) < 0) return ret;
				st4_axis[axis].res = val0.ui16;
				return ret;
			case CMD_ID_SR0:
				if ((ret = cmd_scan_ui16(pstr, &val0.ui16)) < 0) return ret;
				st4_axis[axis].sr0 = val0.ui16;
				st4_calc_acdc(axis);
				return ret;
			case CMD_ID_SRM:
				if ((ret = cmd_scan_ui16(pstr, &val0.ui16)) < 0) return ret;
				st4_axis[axis].srm = val0.ui16;
				st4_calc_acdc(axis);
				return ret;
			case CMD_ID_ACC:
				if ((ret = cmd_scan_ui16(pstr, &val0.ui16)) < 0) return ret;
				st4_axis[axis].acc = val0.ui16;
				st4_calc_acdc(axis);
				return ret;
			case CMD_ID_DEC:
				if ((ret = cmd_scan_ui16(pstr, &val0.ui16)) < 0) return ret;
				st4_axis[axis].dec = val0.ui16;
				st4_calc_acdc(axis);
				return ret;
			case CMD_ID_MOA:
				if ((ret = cmd_scan_i32(pstr, &val0.i32)) < 0) return ret;
				st4_moa(axis, val0.i32);
				return ret;
			case CMD_ID_MOR:
				if ((ret = cmd_scan_i32(pstr, &val0.i32)) < 0) return ret;
				st4_mor(axis, val0.i32);
				return ret;
			case CMD_ID_MOV:
				if ((ret = cmd_scan_i32(pstr, &val0.i32)) < 0) return ret;
				st4_mov(axis, val0.i32);
				return ret;
			case CMD_ID_TMC:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				n = ret;
				if ((ret = cmd_scan_ui32(pstr + n, &val1.ui32)) < 0) return ret;
				/*tmc2130_SPI_STATUS_t status = */tmc2130_wrreg(axis, val0.ui8, val1.ui32);
				return n + ret;
			case CMD_ID_STEP:
				{
					if ((ret = cmd_scan_i8(pstr, &val0.i8)) < 0) return ret;
//					n = ret;
//					if ((ret = cmd_scan_ui8(pstr + n, &val1.ui8)) < 0) return ret;
					tmc2130_set_dir((tmc2130_get_dir() & ~mask) | (val0.i8 < 0)?mask:0);
					if (val0.i8 < 0) val0.i8 = -val0.i8;
					for (int i = 0; i < val0.i8; i++)
					{
						tmc2130_step(mask);
						_delay_us(1000);
					}
				}
				return n + ret;
			}
		else if (pref == '?')
			switch (cmd_id)
			{
			case CMD_ID_TMC:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				/*tmc2130_SPI_STATUS_t status = */tmc2130_rdreg(axis, val0.ui8, &val1.ui32);
				cmd_print_ui32_hex(val1.ui32);
				return ret;
			}
	}
	return CMD_ER_SYN;
}
