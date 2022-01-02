// cmd.h - file stream command processor
#ifndef _CMD_H
#define _CMD_H
#include <inttypes.h>
#include <stdio.h>


// result codes
enum
{
	CMD_OK     =  0, // ok - success
	CMD_ER_UNK = -1, // error 1 - unknown/unspecified failure
	CMD_ER_BSY = -2, // error 2 - busy
	CMD_ER_SYN = -3, // error 3 - syntax error
	CMD_ER_OOR = -4, // error 4 - parameter out of range
	CMD_ER_ONP = -5, // error 5 - operation not permitted
};

// data types
enum
{
	TYPE_UI8   = 0x00,
	TYPE_UI16  = 0x01,
	TYPE_UI32  = 0x02,
	TYPE_I8    = 0x04,
	TYPE_I16   = 0x05,
	TYPE_I32   = 0x06,
	TYPE_FLOAT = 0x08,
};

// variant union type
typedef union
{
	uint8_t  ui8;
	uint16_t ui16;
	uint32_t ui32;
	int8_t   i8;
	int16_t  i16;
	int32_t  i32;
	float    flt;
} var_num_t;


#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)


extern FILE* cmd_in;
extern FILE* cmd_out;
extern FILE* cmd_err;

extern uint8_t cmd_echo;


extern int8_t cmd_print_ui8(uint8_t val);
extern int8_t cmd_print_ui8_hex(uint8_t val);
extern int8_t cmd_print_ui16(uint16_t val);
extern int8_t cmd_print_ui16_hex(uint16_t val);
extern int8_t cmd_print_ui32(uint32_t val);
extern int8_t cmd_print_ui32_hex(uint32_t val);
extern int8_t cmd_print_i8(int8_t val);
extern int8_t cmd_print_i16(int16_t val);
extern int8_t cmd_print_i32(int32_t val);

extern int8_t cmd_scan_ui8(char* pstr, uint8_t* pval);
extern int8_t cmd_scan_ui8_min_max(char* pstr, uint8_t* pval, uint8_t min_val, uint8_t max_val);
extern int8_t cmd_scan_ui16(char* pstr, uint16_t* pval);
extern int8_t cmd_scan_ui16_min_max(char* pstr, uint16_t* pval, uint16_t min_val, uint16_t max_val);
extern int8_t cmd_scan_ui32(char* pstr, uint32_t* pval);
extern int8_t cmd_scan_ui32_min_max(char* pstr, uint32_t* pval, uint32_t min_val, uint32_t max_val);
extern int8_t cmd_scan_i8(char* pstr, int8_t* pval);
extern int8_t cmd_scan_i8_min_max(char* pstr, int8_t* pval, int8_t min_val, int8_t max_val);
extern int8_t cmd_scan_i16(char* pstr, int16_t* pval);
extern int8_t cmd_scan_i16_min_max(char* pstr, int16_t* pval, int16_t min_val, int16_t max_val);
extern int8_t cmd_scan_i32(char* pstr, int32_t* pval);
extern int8_t cmd_scan_i32_min_max(char* pstr, int32_t* pval, int32_t min_val, int32_t max_val);

extern void cmd_putc(char c);

extern void cmd_process(void);


#if defined(__cplusplus)
}
#endif //defined(__cplusplus)


#endif //_CMD_H
