//st4.c
#include "st4.h"
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


//get/set direction control signals (defined in config.h)
extern uint8_t ST4_GET_DIR(void);
extern void ST4_SET_DIR(uint8_t mask);

//do step function (defined in config.h)
extern void ST4_DO_STEP(uint8_t mask);

//do step function (defined in config.h)
extern uint8_t ST4_GET_END(void);

#if (ST4_TIMER == 1)
#define OCRnA OCR1A
#define TCNTn TCNT1
#define TCCRnA TCCR1A
#define TCCRnB TCCR1B
#define TIMSKn TIMSK1
#define OCIEnA OCIE1A
#define COMnA0 COM1A0
#define COMnB0 COM1B0
#elif (ST4_TIMER == 3)
#define OCRnA OCR3A
#define TCNTn TCNT3
#define TCCRnA TCCR3A
#define TCCRnB TCCR3B
#define TIMSKn TIMSK3
#define OCIEnA OCIE3A
#define COMnA0 COM3A0
#define COMnB0 COM3B0
#endif

//global variables
uint8_t  st4_msk = 0;                  // motion and direction mask (bit 0..3 - motion, bit 4..7 - dir)
uint8_t  st4_end = 0;                  // endstop enabled mask (bit 0..3 - mask)
uint8_t  st4_esw = 0;                  // endstop signaled mask (bit 0..3 - mask)
uint8_t  st4_max = 0;                  // max steprate axis (0xff for interpolated moves)
uint16_t st4_msr = 0;                  // max steprate or diagonal steprate for interpolated moves
uint16_t st4_d2 = 0;                   // timer delay [500ns]
uint16_t st4_em = 0;                   // endstop mask
st4_axis_t st4_axis[ST4_NUMAXES+1];    // axis parameters

//macro shortcuts for more readable code
#define _res(a) (st4_axis[a].res)
#define _srx(a) (st4_axis[a].srx.ui32)
#define _srxl(a) (st4_axis[a].srx.ui16.l)
#define _srxh(a) (st4_axis[a].srx.ui16.h)
#define _sr0(a) (st4_axis[a].sr0)
#define _srm(a) (st4_axis[a].srm)
#define _acc(a) (st4_axis[a].acc)
#define _dec(a) (st4_axis[a].dec)
#define _nac(a) (st4_axis[a].nac)
#define _ndc(a) (st4_axis[a].ndc)
#define _pos(a) (st4_axis[a].pos)
#define _cnt(a) (st4_axis[a].cnt)
#define _d2s(a) (st4_axis[a].d2s)
#define _flg(a) (st4_axis[a].flg)
#define _cac(a) (st4_axis[a].cac)
#define _crm(a) (st4_axis[a].crm)
#define _cdc(a) (st4_axis[a].cdc)
#define _nse(a) (st4_axis[a].nse)


char st4_axis_chr(uint8_t axis)
{
	switch (axis)
	{
	case 0: return 'X';
	case 1: return 'Y';
	case 2: return 'Z';
	case 3: return 'E';
	}
	return '?';
}

uint8_t st4_axis_idx(char chr)
{
	switch (chr)
	{
	case 'X': return 0;
	case 'Y': return 1;
	case 'Z': return 2;
	case 'E': return 3;
	}
	return (uint8_t)-1;
}

//find axis with maximum sr
uint8_t st4_max_sr_axis(void)
{
	if (_srxh(0) >= _srxh(1))
	{
#if (ST4_NUMAXES > 2)
		if (_srxh(0) >= _srxh(2))
		{
#if (ST4_NUMAXES > 3)
			if (_srxh(0) >= _srxh(3))
				return 0;
			else
				return 3;
#else //(ST4_NUMAXES > 3)
			return 0;
#endif //(ST4_NUMAXES > 3)
		}
		else
		{
#if (ST4_NUMAXES > 3)
			if (_srxh(2) >= _srxh(3))
				return 2;
			else
				return 3;
#else //(ST4_NUMAXES > 3)
			return 2;
#endif //(ST4_NUMAXES > 3)
		}
#else //(ST4_NUMAXES > 2)
		return 0;
#endif //(ST4_NUMAXES > 2)
	}
	else
	{
#if (ST4_NUMAXES > 2)
		if (_srxh(1) >= _srxh(2))
		{
#if (ST4_NUMAXES > 3)
			if (_srxh(1) >= _srxh(3))
				return 1;
			else
				return 3;
#else //(ST4_NUMAXES > 3)
			return 1;
#endif //(ST4_NUMAXES > 3)
		}
		else
		{
#if (ST4_NUMAXES > 3)
			if (_srxh(2) >= _srxh(3))
				return 2;
			else
				return 3;
#else //(ST4_NUMAXES > 3)
			return 2;
#endif //(ST4_NUMAXES > 3)
		}
#else //(ST4_NUMAXES > 2)
		return 1;
#endif //(ST4_NUMAXES > 2)
	}
}

#ifdef ST4_DBG
#define _DBG(...) printf_P(__VA_ARGS__)
#else
#define _DBG(...)
#endif


void st4_calc_acdc(uint8_t axis)
{
	uint16_t srm_sub_sr0 = _srm(axis) - _sr0(axis);
	uint16_t srm_add_sr0 = _srm(axis) + _sr0(axis);
	if (_acc(axis))
	{
		// acceleration time [us]
		uint32_t t_ac = (4096 * (uint32_t)srm_sub_sr0) / _acc(axis);
		// acceleration steps [1]
		_nac(axis) = (uint16_t)(((t_ac >> 4) * (uint32_t)srm_add_sr0 + 62500) / 125000);
	}
	else _nac(axis) = 0;
	if (_dec(axis))
	{
		// deceleration time [us]
		uint32_t t_dc = (4096 * (uint32_t)srm_sub_sr0) / _dec(axis);
		// deceleration steps [1]
		_ndc(axis) = (uint16_t)(((t_dc >> 4) * (uint32_t)srm_add_sr0 + 62500) / 125000);
	}
	else _ndc(axis) = 0;
	_DBG(PSTR("st4_calc_acdc(%d) nac=%d ndc=%d\n"), axis, _nac(axis), _ndc(axis));
}

void st4_calc_move(uint8_t axis, uint32_t n)
{
	uint16_t nacdc = _nac(axis) + _ndc(axis);
	uint8_t flg = 0;
	if (n >= nacdc)
	{
		_cac(axis) = _nac(axis);
		_crm(axis) = n - (uint32_t)nacdc;
		_cdc(axis) = _ndc(axis);
		flg = ST4_FLG_RM;
		if (_cac(axis)) flg |= ST4_FLG_AC;
		if (_cdc(axis)) flg |= ST4_FLG_DC;
//		flg = ST4_FLG_AC | ST4_FLG_RM | ST4_FLG_DC;
	}
	else
	{
		_cac(axis) = (uint16_t)((uint32_t)_nac(axis) * n / nacdc);
		_crm(axis) = 0;
		_cdc(axis) = (uint16_t)(n - _cac(axis));
		flg = ST4_FLG_AC | ST4_FLG_DC;
	}
	_flg(axis) = flg;
	_DBG(PSTR("st4_calc_move(%d) cac=%d crm=%ld cdc=%d\n"), axis, _cac(axis), _crm(axis), _cdc(axis));
}

int8_t st4_mor(uint8_t axis, int32_t val)
{
	uint8_t msk = 1 << axis;
	if (st4_msk & msk) return -1; //busy
	if (val == 0) return 0; //no motion
	if (val > 0)
	{
		ST4_SET_DIR(ST4_GET_DIR() & ~msk);
		st4_msk &= ~(msk << 4);
		st4_calc_move(axis, (uint32_t)val);
	}
	else
	{
		ST4_SET_DIR(ST4_GET_DIR() | msk);
		st4_msk |= msk << 4;
		st4_calc_move(axis, (uint32_t)(-val));
	}
	_srxl(axis) = 0;
	_srxh(axis) = _sr0(axis);
	_nse(axis) = 64; //skip endstop check for first 64 steps
	st4_esw &= ~msk; //reset endstop status
	st4_msk |= msk;
	return 0;
}

int8_t st4_moa(uint8_t axis, int32_t val)
{
	return st4_mor(axis, val - st4_axis[axis].pos);
}

int8_t st4_mov(uint8_t axis, int32_t val)
{
	uint8_t msk = 1 << axis;
	if (val == 0)
	{
		_srxl(axis) = 0;
		_srxh(axis) = 0;
		st4_msk &= ~msk;
	}
	else
	{
		if (val > 0)
		{
			ST4_SET_DIR(ST4_GET_DIR() & ~msk);
			st4_msk &= ~(msk << 4);
		}
		else
		{
			ST4_SET_DIR(ST4_GET_DIR() | msk);
			st4_msk |= msk << 4;
			val = -val;
		}
		_srxl(axis) = 0;
		_srxh(axis) = (uint16_t)val;
		_nse(axis) = 64; //skip endstop check for first 64 steps
		st4_esw &= ~msk; //reset endstop status
		st4_msk |= msk;
	}
	return 0;
}

void st4_setup_axis(uint8_t axis, uint16_t res, uint16_t sr0, uint16_t srm, uint16_t acc, uint16_t dec)
{
	memset(&(st4_axis[axis]), 0, sizeof(st4_axis_t));
	st4_axis[axis].chr = st4_axis_chr(axis);
	st4_axis[axis].res = res;
	_sr0(axis) = sr0;
	_srm(axis) = srm;
	_acc(axis) = acc;
	_dec(axis) = dec;
	st4_calc_acdc(axis);
}

void st4_setup_axis_mm(uint8_t axis, uint16_t res, float sr0_mms, float srm_mms, float acc_mms2, float dec_mms2)
{
	memset(&(st4_axis[axis]), 0, sizeof(st4_axis_t));
	st4_axis[axis].chr = st4_axis_chr(axis);
	st4_axis[axis].res = res;
	st4_set_sr0_mms(axis, sr0_mms);
	st4_set_srm_mms(axis, srm_mms);
	st4_set_acc_mms2(axis, acc_mms2);
	st4_set_dec_mms2(axis, dec_mms2);
	st4_calc_acdc(axis);
}

float st4_get_srx_mms(uint8_t axis)
{
	return (float)_srxh(axis) / _res(axis);
}

void st4_set_srx_mms(uint8_t axis, float sr0_mms)
{
	_srx(axis) = (uint32_t)(sr0_mms * _res(axis)) << 16;
}

float st4_get_sr0_mms(uint8_t axis)
{
	return (float)_sr0(axis) / _res(axis);
}

void st4_set_sr0_mms(uint8_t axis, float sr0_mms)
{
	_sr0(axis) = (uint16_t)(sr0_mms * _res(axis));
}

float st4_get_srm_mms(uint8_t axis)
{
	return (float)_srm(axis) / _res(axis);
}

void st4_set_srm_mms(uint8_t axis, float srm_mms)
{
	_srm(axis) = (uint16_t)(srm_mms * _res(axis));
}

float st4_get_acc_mms2(uint8_t axis)
{
	return (float)_acc(axis) * 256 / _res(axis);
}

void st4_set_acc_mms2(uint8_t axis, float acc_mms2)
{
	_acc(axis) = (uint16_t)((uint32_t)(acc_mms2 * _res(axis)) / 256);
}

float st4_get_dec_mms2(uint8_t axis)
{
	return (float)_dec(axis) * 256 / _res(axis);
}

void st4_set_dec_mms2(uint8_t axis, float dec_mms2)
{
	_dec(axis) = (uint16_t)((uint32_t)(dec_mms2 * _res(axis)) / 256);
}

float st4_get_pos_mm(uint8_t axis)
{
	return (float)_pos(axis) / _res(axis);
}

void st4_set_pos_mm(uint8_t axis, float pos_mm)
{
	_pos(axis) = (int16_t)(pos_mm * _res(axis));
}

inline uint32_t calc_dsrx(uint16_t acc, uint16_t d2)
{
	if (d2 < 256) return ((uint32_t)(acc * st4_d2)) << 3;
	return ((uint32_t)acc * st4_d2) << 3;
}

inline void st4_step_axis_indep(uint8_t axis, uint8_t mask)
{
	uint8_t register flg = _flg(axis); //cache flags in register
	if (st4_msk & (mask << 4)) _pos(axis)--;
	else _pos(axis)++;
	if (flg & ST4_FLG_AC)
	{
		_srx(axis) += calc_dsrx(_acc(axis), _d2s(axis));
		if ((--_cac(axis)) == 0)
		{
			flg &= ~ST4_FLG_AC;
			if (flg & ST4_FLG_RM)
			{
				_srxh(axis) = _srm(axis);
				_srxl(axis) = 0;
			}
		}
	}
	else if (flg & ST4_FLG_RM)
	{
		if ((--_crm(axis)) == 0)
		{
			flg &= ~ST4_FLG_RM;
			if ((flg & ST4_FLG_DC) == 0)
			{
				st4_msk &= ~mask;
				_srx(axis) = 0;
			}
		}
	}
	else if (flg & ST4_FLG_DC)
	{
		_srx(axis) -= calc_dsrx(_dec(axis), _d2s(axis));
		if ((--_cdc(axis)) == 0)
		{
			flg &= ~ST4_FLG_DC;
			st4_msk &= ~mask;
			_srx(axis) = 0;
		}
	}
	if (_nse(axis)) //skip endstop check?
		_nse(axis)--; //decrement counter
	else
		if (st4_em & mask) //endstop is active
		{
			st4_msk &= ~mask; //stop axis
			st4_esw |= mask; //set endswitch flag
		}
	//update flags
	_flg(axis) = flg;
}

inline uint8_t st4_cycle_axis_indep(uint8_t axis, uint8_t mask)
{
	_d2s(axis) += st4_d2;
	if (_cnt(axis) <= _srxh(axis))
	{
		_cnt(axis) += st4_msr;
		_cnt(axis) -= _srxh(axis);
		st4_step_axis_indep(axis, mask);
		_d2s(axis) = 0;
		return mask;
	}
	_cnt(axis) -= _srxh(axis);
	return 0;
}

#define M (ST4_NUMAXES)

inline uint8_t st4_cycle_axis_intpol(uint8_t axis, uint8_t mask)
{
	if (_cnt(axis) <= _srxh(axis))
	{
		_cnt(axis) += _cnt(M);
		_cnt(axis) -= _srxh(axis);
		//update position counter
//		if (st4_msk & (mask << 4)) _pos(axis)--;
//		else _pos(axis)++;
		return mask;
	}
	_cnt(axis) -= _srxh(axis);
	return 0;
}

inline void st4_step_intpol(void)
{
	//cache flags in register
	uint8_t register flg = _flg(M);
	if (flg & ST4_FLG_AC)
	{
		_srx(M) += calc_dsrx(_acc(M), st4_d2);
		if ((--_cac(M)) == 0)
		{
			flg &= ~ST4_FLG_AC;
			if (flg & ST4_FLG_RM)
			{
				_srxh(M) = _srm(M);
				_srxl(M) = 0;
			}
		}
	}
	else if (flg & ST4_FLG_RM)
	{
		if ((--_crm(M)) == 0)
		{
			flg &= ~ST4_FLG_RM;
		}
	}
	else if (flg & ST4_FLG_DC)
	{
		_srx(M) -= calc_dsrx(_dec(M), st4_d2);
		if ((--_cdc(M)) == 0)
		{
			flg &= ~ST4_FLG_DC;
			st4_msk &= ~0x0f;
			_srx(M) = 0;
		}
	}
	//update flags
	_flg(M) = flg;
}

#undef M

inline void st4_cycle_indep(void)
{
//	uint8_t axis;
	uint8_t sm = 0;
//	uint8_t em = 0;
	uint16_t tim0;
	uint16_t tim1;
	uint16_t tim2;
	TCNTn = 0;
	st4_max = st4_max_sr_axis();
	st4_msr = _srxh(st4_max);
	tim0 = TCNTn;
	if (st4_msr)
	{
		st4_em = ST4_GET_END() & st4_end;
//		em &= st4_msk;
//		st4_esw |= em;
//		st4_msk &= ~st4_esw;
//		if (em & 0x01) st4_msk &= ~0x01;
//		if (em & 0x02) st4_msk &= ~0x02;
		st4_d2 = st4_sr2d2(st4_msr);
		tim1 = TCNTn;
//		printf_P(PSTR("maxsr=%u d2=%u srx=%u sry=%u x=%li y=%li\n"), st4_msr, st4_d2, _srxh(0), _srxh(1), _pos(0), _pos(1));
//		printf_P(PSTR("maxsr=%u d2=%u srx=%u sry=%u x=%li y=%li flg=%u nac=%u nrm=%lu ndc=%u\n"), st4_msr, st4_d2, _srxh(0), _srxh(1), _pos(0), _pos(1), _flg(0), _cac(0), _crm(0), _cdc(0));
		if (st4_msk & 0x01)
			sm |= st4_cycle_axis_indep(0, 0x01);
#if (ST4_NUMAXES > 1)
		if (st4_msk & 0x02)
			sm |= st4_cycle_axis_indep(1, 0x02);
#endif //(ST4_NUMAXES > 1)
#if (ST4_NUMAXES > 2)
		if (st4_msk & 0x04)
			sm |= st4_cycle_axis_indep(2, 0x04);
#endif //(ST4_NUMAXES > 2)
#if (ST4_NUMAXES > 3)
		if (st4_msk & 0x08)
			sm |= st4_cycle_axis_indep(3, 0x08);
#endif //(ST4_NUMAXES > 3)
		ST4_DO_STEP(sm);
	}
	else
		st4_d2 = 2000;
	OCRnA = st4_d2;
	tim2 = TCNTn;
	if (tim2 >= st4_d2) TCNTn = st4_d2 - 10;
//	if (st4_msk & 0x0f)
//		printf_P(PSTR("tim0=%u tim1=%u tim2=%u\n"), tim0, tim1, tim2);
	tim0 = tim0; // prevent warning
	tim1 = tim1; // prevent warning
}

inline void st4_cycle_intpol(void)
{
//	uint8_t axis;
	uint8_t sm = 0;
//	uint8_t em = 0;
	uint16_t tim0;
	uint16_t tim1;
	uint16_t tim2;
	TCNTn = 0;
	st4_msr = _srxh(4);
	tim0 = TCNTn;
	if (st4_msr)
	{
//		em = ST4_GET_END() & st4_end;
//		if (em & 0x01) st4_msk &= ~0x01;
//		if (em & 0x02) st4_msk &= ~0x02;
//		printf_P(PSTR("sr=%u d2=%u x=%li y=%li flg=%u nac=%u nrm=%lu ndc=%u\n"), st4_msr, st4_d2, _pos(0), _pos(1), _flg(4), _cac(4), _crm(4), _cdc(4));
		st4_d2 = st4_sr2d2(st4_msr);
		tim1 = TCNTn;
		if (st4_msk & 0x01)
			sm |= st4_cycle_axis_intpol(0, 0x01);
#if (ST4_NUMAXES > 1)
		if (st4_msk & 0x02)
			sm |= st4_cycle_axis_intpol(1, 0x02);
#endif //(ST4_NUMAXES > 1)
#if (ST4_NUMAXES > 2)
		if (st4_msk & 0x04)
			sm |= st4_cycle_axis_intpol(2, 0x04);
#endif //(ST4_NUMAXES > 2)
#if (ST4_NUMAXES > 3)
		if (st4_msk & 0x08)
			sm |= st4_cycle_axis_intpol(3, 0x08);
#endif //(ST4_NUMAXES > 3)
		st4_step_intpol();
		ST4_DO_STEP(sm);
	}
	else
		st4_d2 = 2000;
	OCRnA = st4_d2;
	tim2 = TCNTn;
	if (tim2 >= st4_d2) TCNTn = st4_d2 - 10;
//	if (st4_msk & 0x0f)
//		printf_P(PSTR("sr=%u d2=%u tim0=%u tim1=%u tim2=%u cpu=%u\n"), st4_msr, st4_d2, tim0, tim1, tim2, 100*tim2/st4_d2);
	tim0 = tim0; // prevent warning
	tim1 = tim1; // prevent warning
}

void st4_cycle(void)
{
//	st4_cycle_indep();
//	st4_cycle_intpol();
}




//look up table for calculating delay (3 segments, 128 values)
uint8_t st4_tab_sr2d2[128+128+128] = {
//seg0
255, 254, 254, 253, 253, 252, 252, 251, 251, 250, 250, 249, 249, 248, 248, 247, 247, 246, 246, 245, 245, 244, 244, 243, 243, 242, 242, 242, 241, 241, 240, 240,
239, 239, 238, 238, 237, 237, 237, 236, 236, 235, 235, 234, 234, 233, 233, 233, 232, 232, 231, 231, 230, 230, 230, 229, 229, 228, 228, 227, 227, 227, 226, 226,
225, 225, 225, 224, 224, 223, 223, 223, 222, 222, 221, 221, 221, 220, 220, 220, 219, 219, 218, 218, 218, 217, 217, 216, 216, 216, 215, 215, 215, 214, 214, 214,
213, 213, 212, 212, 212, 211, 211, 211, 210, 210, 210, 209, 209, 209, 208, 208, 207, 207, 207, 206, 206, 206, 205, 205, 205, 204, 204, 204, 203, 203, 203, 202,
//seg1
202, 201, 201, 200, 199, 199, 198, 197, 197, 196, 196, 195, 194, 194, 193, 193, 192, 191, 191, 190, 190, 189, 189, 188, 187, 187, 186, 186, 185, 185, 184, 184,
183, 182, 182, 181, 181, 180, 180, 179, 179, 178, 178, 177, 177, 176, 176, 175, 175, 174, 174, 173, 173, 172, 172, 171, 171, 171, 170, 170, 169, 169, 168, 168,
167, 167, 166, 166, 166, 165, 165, 164, 164, 163, 163, 163, 162, 162, 161, 161, 160, 160, 160, 159, 159, 158, 158, 158, 157, 157, 156, 156, 156, 155, 155, 155,
154, 154, 153, 153, 153, 152, 152, 152, 151, 151, 150, 150, 150, 149, 149, 149, 148, 148, 148, 147, 147, 147, 146, 146, 146, 145, 145, 145, 144, 144, 144, 143,
//seg2
143, 142, 141, 141, 140, 140, 139, 138, 138, 137, 136, 136, 135, 135, 134, 134, 133, 132, 132, 131, 131, 130, 130, 129, 129, 128, 128, 127, 127, 126, 126, 125,
125, 124, 124, 123, 123, 122, 122, 121, 121, 120, 120, 119, 119, 118, 118, 117, 117, 117, 116, 116, 115, 115, 114, 114, 114, 113, 113, 112, 112, 112, 111, 111,
110, 110, 110, 109, 109, 109, 108, 108, 107, 107, 107, 106, 106, 106, 105, 105, 105, 104, 104, 103, 103, 103, 102, 102, 102, 101, 101, 101, 100, 100, 100, 100,
 99,  99,  99,  98,  98,  98,  97,  97,  97,  96,  96,  96,  96,  95,  95,  95,  94,  94,  94,  94,  93,  93,  93,  92,  92,  92,  92,  91,  91,  91,  91,  90
};

uint16_t st4_sr2d2(uint16_t sr)
{
	if (sr == 0) return 0; // zero steprate - returns 0
	if (sr >= ST4_THR_SR3)
		return st4_tab_sr2d2[256 + (sr >> 6) - 218];
	if (sr >= ST4_THR_SR2)
		return st4_tab_sr2d2[128 + (sr >> 5) - 308];
	if (sr >= ST4_THR_SR1)
		return st4_tab_sr2d2[(sr >> 4) - 488];
	if (sr >= ST4_THR_SR0)
		return (uint16_t)(((uint32_t)2000000 + (sr >> 1)) / sr);
	return 0;
//	if ()
//		return ((uint32_t)2000000) / sr;
//		st4_tab_B[(sr >> 8) - 31]
}

// generate LUT segment (128 bytes)
//  sr0 - starting steprate [steps/s]
//  sh  - steprate shift [bits]
void st4_gen_seg(uint16_t sr0, uint8_t sh, uint8_t* pseg)
{
	uint8_t c = 128;           //sub-segment count
	uint8_t s = 1 << sh;       //sub-segment len
//	uint16_t srx;              //shifted steprate [s*steps/s]
//	uint16_t srx0 = sr0 >> sh; //shifted starting steprate [l*steps/s]
	uint16_t sr;               //steprate [steps/s]
	float    df;               //delay as float [us]
	uint16_t d2;               //delay*2 as 16bit uint [0.5us]
	float    de;               //delay error for d2 [%]
	float    df_sum;           //sum of df values in any sub-segment
	uint8_t i;
	uint8_t j;
	int n = 0;
	for (i = 0; i < c; i++)
	{
		df_sum = 0;
		for (j = 0; j < s; j++)
		{
			sr = sr0 + n;
			df = (float)1000000 / sr;
			df_sum += df;
			d2 = (int)(2 * df + 0.5);
			de = 100 * (((float)d2 / 2 - df) / df);
//			printf("%5d %5.2f %5.2f %5.2f%%\n", sr, df, (float)d2/2, de);
			n++;
		}
		d2 = (int)(2 * df_sum / s + 0.5);
		if (pseg) pseg[i] = (uint8_t)d2;
//		printf_P(PSTR("%5d %5.2f\n"), sr - s + 1, (float)d2/2);
		printf_P(PSTR("%3d, "), d2);
	}
	de = de; // prevent warning
}

void st4_gen_tab(void)
{
	st4_gen_seg(ST4_THR_SR1, 4, st4_tab_sr2d2 + 0);
	st4_gen_seg(ST4_THR_SR2, 5, st4_tab_sr2d2 + 128);
	st4_gen_seg(ST4_THR_SR3, 6, st4_tab_sr2d2 + 256);
}

void st4_fprint_sr_d2(FILE* out, uint16_t sr0, uint16_t sr1)
{
	uint16_t sr; //steprate [steps/s]
	float    df; //delay as float [us]
	uint16_t d2; //delay*2 as 16bit int, calculated [0.5us resolution]
	uint16_t d2A; //delay*2 as 16bit int, from table [0.5us resolution]
	float    de; //delay error for d2 [%]
	float    deA; //delay error for d2A [%]
	for (sr = sr0; sr < sr1; sr++)
	{
		df = (float)1000000 / sr;
		d2 = (int)(2 * df + 0.5);
		d2A = st4_sr2d2(sr);
		de = 100 * (((float)d2/2 - df) / df);
		deA = 100 * (((float)d2A/2 - df) / df);
		fprintf_P(out, PSTR("%5d %5.2f %5.2f %5.2f%% %5.2f %5.2f%% \n"), sr, df, (float)d2/2, de, (float)d2A/2, deA);
	}
}

void st4_fprint_sr2d2_seg(FILE* out, uint8_t* pseg)
{
	uint8_t i;
	uint8_t j;
	fprintf_P(out, PSTR("=("));
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 32; j++)
		{
			fprintf_P(out, PSTR("%u"), pgm_read_byte(pseg + 32*i + j));
			if ((i < 3) || (j < 31))
				fprintf_P(out, PSTR(", "));
		}
		if (i < 3)
			fprintf_P(out, PSTR("\n"));
	}
	fprintf_P(out, PSTR(");\n"));
}

void st4_fprint_sr2d2_tab(FILE* out)
{
	st4_fprint_sr2d2_seg(stdout, st4_tab_sr2d2 + 0);
	st4_fprint_sr2d2_seg(stdout, st4_tab_sr2d2 + 128);
	st4_fprint_sr2d2_seg(stdout, st4_tab_sr2d2 + 256);
}

void st4_fprint_axis(FILE* out, uint8_t axis)
{
	fprintf_P(out, PSTR("axis %c\n"), st4_axis[axis].chr);
	fprintf_P(out, PSTR(" res=%8d [steps/mm]\n"), st4_axis[axis].res);
	fprintf_P(out, PSTR(" sr0=%8.0f [mm/s]\n"), st4_get_sr0_mms(axis));
	fprintf_P(out, PSTR(" srm=%8.0f [mm/s]\n"), st4_get_srm_mms(axis));
	fprintf_P(out, PSTR(" acc=%8.0f [mm/s^2]\n"), st4_get_acc_mms2(axis));
	fprintf_P(out, PSTR(" dec=%8.0f [mm/s^2]\n"), st4_get_dec_mms2(axis));
}

void st4_setup_timer(void)
{
	// waveform generation = 0100 = CTC
	TCCRnB &= ~(1 << WGM13);
	TCCRnB |=  (1 << WGM12);
	TCCRnA &= ~(1 << WGM11);
	TCCRnA &= ~(1 << WGM10);
	// output mode = 00 (disconnected)
	TCCRnA &= ~(3 << COMnA0);
	TCCRnA &= ~(3 << COMnB0);
	// Set the timer pre-scaler
	TCCRnB = (TCCRnB & ~(0x07 << CS10)) | (2 << CS10);
	// Plan the first interrupt after 8ms from now.
	OCRnA = 0x200;
	TCNTn = 0;
	TIMSKn |= (1 << OCIEnA);
}

#if (ST4_TIMER == 1)
ISR(TIMER1_COMPA_vect)
#elif (ST4_TIMER == 3)
ISR(TIMER3_COMPA_vect)
#endif
{
	st4_cycle_indep();
//	st4_cycle_intpol();
}
