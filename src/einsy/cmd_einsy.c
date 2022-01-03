// cmd_einsy.c
#include "cmd_einsy.h"
#include <string.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "config.h"
#include "sys.h"
#include "shr16.h"
#include "einsy.h"
#include "tmc2130.h"
#include "tmc2130_hw.h"


#define _STR(x) #x
#define STR(x)  _STR(x)
const char PROGMEM fw_version_full[] = STR(FW_VERSION_FULL);

int8_t cmd_do_mod_wout_args_xyz(uint8_t mod_id, char pref, uint8_t cmd_id);
int8_t cmd_do_mod_with_args_xyz(uint8_t mod_id, char pref, uint8_t cmd_id, char* pstr);


// parse module bitmask
int8_t cmd_parse_mod_msk(char* pstr, uint16_t* pmod_msk)
{
	int8_t n = 0;
	char c;
	do
	{
		c = pstr[n];
		if ((c == '!') || (c == '?'))
		{
			switch (n)
			{
			case 0:
				*pmod_msk = MOD_MSK_0;
				return 0;
			case 1:
				switch (pstr[0])
				{
				case 'X':
					*pmod_msk = MOD_MSK_X;
					return 1;
				case 'Y':
					*pmod_msk = MOD_MSK_Y;
					return 1;
				case 'Z':
					*pmod_msk = MOD_MSK_Z;
					return 1;
				}
			case 3:
				if ((pstr[0] == 'X') && (pstr[1] == 'Y') && (pstr[2] == 'Z'))
				{
					*pmod_msk = MOD_MSK_XYZ;
					return 3;
				}
			}
			break;
		}
		n++;
	} while (n <= 3);
	return CMD_ER_SYN;
}

const uint8_t PROGMEM cmd_ext_3[] = {
'v','e','r',CMD_ID_VER,
's','e','r',CMD_ID_SER,
'r','s','t',CMD_ID_RST,
't','s','t',CMD_ID_TST,
'l','o','g',CMD_ID_LOG,
'e','r','r',CMD_ID_ERR,
'a','d','c',CMD_ID_ADC,
'e','n','a',CMD_ID_ENA,
'p','o','s',CMD_ID_POS,
'r','e','s',CMD_ID_RES,
's','r','0',CMD_ID_SR0,
's','r','m',CMD_ID_SRM,
'a','c','c',CMD_ID_ACC,
'd','e','c',CMD_ID_DEC,
'm','o','a',CMD_ID_MOA,
'm','o','r',CMD_ID_MOR,
'm','o','v',CMD_ID_MOV,
't','m','c',CMD_ID_TMC,
};

const uint8_t PROGMEM cmd_ext_4[] = {
'g','p','i','o',CMD_ID_GPIO,
'g','p','c','f',CMD_ID_GPCF,
's','t','e','p',CMD_ID_STEP,
};

uint8_t cmd_parse_tab_P(char* pstr, const uint8_t* ptab, uint8_t len, uint8_t cnt)
{
	uint8_t i;
	while (cnt--)
	{
		for (i = 0; i < len; i++)
			if (((uint8_t)pstr[i]) != pgm_read_byte(ptab + i))
				break;
		if (i == len)
			return pgm_read_byte(ptab + len);
		ptab += (len + 1);
	}
	return CMD_ID_unk;
}

int8_t cmd_parse_cmd_id(char* pstr, uint8_t* pcmd_id)
{
	//empty command (0char, no args)
	if (pstr[0] == 0)
	{
		*pcmd_id = CMD_ID_;
		return 0;
	}
	else if ((pstr[3] == 0) || (pstr[3] == ' '))
	{
		if ((*pcmd_id = cmd_parse_tab_P(pstr, cmd_ext_3, 3, sizeof(cmd_ext_3)/4)) != CMD_ID_unk)
			return 3;
	}
	else if ((pstr[4] == 0) || (pstr[4] == ' '))
	{
		if ((*pcmd_id = cmd_parse_tab_P(pstr, cmd_ext_4, 4, sizeof(cmd_ext_4)/5)) != CMD_ID_unk)
			return 4;
	}
	return CMD_ER_SYN;
}

int8_t cmd_do_mod_wout_args(uint8_t mod_id, char pref, uint8_t cmd_id)
{
//	uint8_t val;
	if (mod_id == MOD_ID_0)
	{
		if (pref == '!')
		{
			switch (cmd_id)
			{
			case CMD_ID_RST:
//				if (cmd_err) fprintf_P(cmd_err, PSTR("RESET\n"));
				sys_reset();
				return CMD_OK;
			case CMD_ID_TST:
			{
				for (int i = 0; i < 1000; i++)
				{
					tmc2130_step(7);
					_delay_us(500);
				}
			}
				return CMD_OK;
			}
		}
		else if (pref == '?')
		{
			switch (cmd_id)
			{
			case CMD_ID_:
				cmd_print_ui16(0);
				return CMD_OK;
			case CMD_ID_VER:
				fprintf_P(cmd_out, PSTR("AVRmot_MMCtl %S "), fw_version_full);
				return CMD_OK;
			case CMD_ID_SER:
				fprintf_P(cmd_out, PSTR("0123456789"));
				cmd_putc(' ');
				return CMD_OK;
			case CMD_ID_LOG:
				cmd_print_ui8(0);
				return CMD_OK;
			case CMD_ID_ERR:
				cmd_print_ui8(0);
				return CMD_OK;
			}
		}
	}
	return CMD_ER_SYN;
}

int8_t cmd_do_wout_args(uint16_t mod_msk, char pref, uint8_t cmd_id)
{
	if (mod_msk == MOD_MSK_0)
		return cmd_do_mod_wout_args(MOD_ID_0, pref, cmd_id);
	else
		if (mod_msk & MOD_MSK_XYZ)
		{
			int8_t ret = CMD_OK;
			if (mod_msk & MOD_MSK_X)
				if ((ret = cmd_do_mod_wout_args_xyz(MOD_ID_X, pref, cmd_id)) < 0) return ret;
			if (mod_msk & MOD_MSK_Y)
				if ((ret = cmd_do_mod_wout_args_xyz(MOD_ID_Y, pref, cmd_id)) < 0) return ret;
			if (mod_msk & MOD_MSK_Z)
				if ((ret = cmd_do_mod_wout_args_xyz(MOD_ID_Z, pref, cmd_id)) < 0) return ret;
			return ret;
		}
	return CMD_ER_SYN;
}

int8_t cmd_do_mod_with_args(uint8_t mod_id, char pref, uint8_t cmd_id, char* pstr)
{
	int8_t ret;
	var_num_t val0;
	var_num_t val1;
//	var_num_t val2;
//	var_num_t val3;
	if (mod_id == MOD_ID_0)
	{
		if (pref == '!')
			switch (cmd_id)
			{
			case CMD_ID_LOG:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				//log = val0.ui8;
				return CMD_OK;
			case CMD_ID_GPIO:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				if ((ret = cmd_scan_ui8(pstr + ret, &val1.ui8)) < 0) return ret;
				gpio_set(val0.ui8, val1.ui8);
				return CMD_OK;
			case CMD_ID_GPCF:
				return CMD_OK;
			}
		else if (pref == '?')
			switch (cmd_id)
			{
			case CMD_ID_ADC:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				if (val0.ui8 > 1) return CMD_ER_OOR;
				cmd_print_ui16(einsy_get_adc(val0.ui8));
				return CMD_OK;
			case CMD_ID_GPIO:
				if ((ret = cmd_scan_ui8(pstr, &val0.ui8)) < 0) return ret;
				cmd_print_ui8(gpio_get(val0.ui8));
				return CMD_OK;
			case CMD_ID_GPCF:
				return CMD_OK;
			}
	}
	return CMD_ER_SYN;
}

int8_t cmd_do_with_args(uint16_t mod_msk, char pref, uint8_t cmd_id, char* pstr)
{
	if (mod_msk == MOD_MSK_0)
		return cmd_do_mod_with_args(MOD_ID_0, pref, cmd_id, pstr);
	else
		if (mod_msk & MOD_MSK_XYZE)
		{
			int8_t ret = 0;
			int8_t n = 0;
			if (mod_msk & MOD_MSK_X)
			{
				while (pstr[n] == ' ') n++;
				if ((ret = cmd_do_mod_with_args_xyz(MOD_ID_X, pref, cmd_id, pstr + n)) < 0) return ret;
				n += ret;
			}
			if (mod_msk & MOD_MSK_Y)
			{
				while (pstr[n] == ' ') n++;
				if ((ret = cmd_do_mod_with_args_xyz(MOD_ID_Y, pref, cmd_id, pstr + n)) < 0) return ret;
				n += ret;
			}
			if (mod_msk & MOD_MSK_Z)
			{
				while (pstr[n] == ' ') n++;
				if ((ret = cmd_do_mod_with_args_xyz(MOD_ID_Z, pref, cmd_id, pstr + n)) < 0) return ret;
				n += ret;
			}
			if (mod_msk & MOD_MSK_E)
			{
				while (pstr[n] == ' ') n++;
				if ((ret = cmd_do_mod_with_args_xyz(MOD_ID_Z, pref, cmd_id, pstr + n)) < 0) return ret;
				n += ret;
			}
			return CMD_OK;
		}
	return CMD_ER_SYN;
}
