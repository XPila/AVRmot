//adc.c
#include "adc.h"

#if (defined(ADC_CHAN_MSK) && defined(ADC_CHAN_CNT))

#include <inttypes.h>
#include <avr/io.h>


uint8_t adc_sta;                       //status (bit0..4 - seq, bit7 - setmux/sample)
uint8_t adc_idx;                       //current channel index (0 .. ADC_CHAN_CNT-1)
uint8_t adc_cnt[ADC_CHAN_CNT];         //number of samples per channel
uint16_t adc_val[ADC_CHAN_CNT];        //sum of samples per channel
uint16_t adc_sim_msk;                  //simulator mask


#ifdef ADC_READY
	extern void ADC_READY(uint8_t index);
#endif //ADC_READY

#ifdef ADC_SEQ2IDX
	extern uint8_t ADC_SEQ2IDX(uint8_t index);
#endif //ADC_SEQ


void adc_init(void)
{
	uint8_t i;
	adc_sim_msk = 0x0000;
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN);
//	ADCSRA |= (1 << ADIF) | (1 << ADSC);
	DIDR0 = (ADC_CHAN_MSK & 0xff);
#ifdef ADC_16_CHANS
	DIDR2 = (ADC_CHAN_MSK >> 8);
#endif //ADC_16_CHANS
	for (i = 0; i < ADC_CHAN_CNT; i++)
	{
		adc_val[i] = 0;
		adc_cnt[i] = 0;
	}
	adc_sta = 0;
}

void adc_set_mux(uint8_t ch)
{
	ch &= 0x0f;
#ifdef ADC_16_CHANS
	if (ch & 0x08) ADCSRB |= (1 << MUX5);
	else ADCSRB &= ~(1 << MUX5);
#endif //ADC_16_CHANS
	ADMUX = (ADMUX & ~(0x07)) | (ch & 0x07);
}

uint8_t adc_chan(uint8_t idx)
{
	uint8_t chan = 0;
	uint16_t mask = 1;
	while (mask)
	{
		if ((mask & ADC_CHAN_MSK) && (idx-- == 0)) break;
		mask <<= 1;
		chan++;
	}
	return chan;
}


// 0    - nozzle 64ms
// 1    - bed    128ms
// 2..7 - other  
//sam - 000000000011111111112222
//      012345678901234567890123
//seq - 010201030104010501060107
//
// 24 samples, 48 ms
// chan0     - 12 samples
// chan1     - 6 samples
// chan2..7  - 1 sample
//
// 2 cycles - 96ms
//sam - 000000000011111111112222222222333333333344444444
//      012345678901234567890123456789012345678901234567
//------------------------------------------------------
//seq - 010201030104010501060107010201030104010501060107
//------------------------------------------------------
//bit0- 010101010101010101010101010101010101010101010101
//bit1- 001100110011001100110011001100110011001100110011
//bit2- 000011110000111100001111000011110000111100001111
//bit3- 000000001111111100000000111111110000000011111111
//bit4- 000000000000000011111111111111110000000000000000

//03 2
//07 3
//11 4
//15 5
//19 6




void adc_cycle(void)
{
	static int cnt = 0;
	uint8_t seq;
	if (adc_sta & 0x80)
	{
//		uint8_t seq = adc_sta & 0x1f;
//		adc_idx = adc_seq2idx(seq);
		if ((adc_sim_msk & (1 << adc_idx)) == 0)
			adc_val[adc_idx] += ADC;
		if (++adc_cnt[adc_idx] >= ADC_OVRSAMPL)
		{
#ifdef ADC_READY
			ADC_READY(adc_idx);
			adc_val[adc_idx] = 0;
			adc_cnt[adc_idx] = 0;
#endif //ADC_READY
//			adc_res();
		}
//		if (++index >= ADC_CHAN_CNT)
//		{
//			index = 0;
//		}
		seq = (adc_sta & 0x1f) + 1;
		if (seq >= ADC_SEQ_LEN) seq = 0;
		adc_idx = adc_seq2idx(seq);
		adc_set_mux(adc_chan(adc_idx));
		adc_sta = seq;
	}
	else
	{
		ADCSRA |= (1 << ADSC); //start conversion
		adc_sta |= 0x80;
	}
	cnt++;
}

#endif //(defined(ADC_CHAN_MSK) && defined(ADC_CHAN_CNT))
