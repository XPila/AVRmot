// st4_sr2d.h - st4 step rate to delay calculation

#ifndef _ST4_SR2D_H
#define _ST4_SR2D_H

#include <inttypes.h>
#include <stdio.h>


//steprate thresholds [steps/s] for optimized delay calculation
#define ST4_THR_SR0      32
#define ST4_THR_SR1    7840    // 7808
#define ST4_THR_SR2    9888    // 9856
#define ST4_THR_SR3   13984    // 13952
#define ST4_THR_SR4   22176    // 22144

#define ST4_MIN_SR       32    // minimum steprate [steps/s]
#define ST4_MAX_SR    22000    // maximum steprate [steps/s]


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern uint16_t st4_sr2d2(uint16_t sr);

extern void st4_gen_seg(uint16_t sr0, uint8_t sh, uint8_t* pseg);

extern void st4_gen_tab(void);

extern void st4_fprint_sr_d2(FILE* out, uint16_t sr0, uint16_t sr1);

extern void st4_fprint_sr2d2_tab(FILE* out);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

#endif //_ST4_SR2D_H
