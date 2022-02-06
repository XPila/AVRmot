// config.h - main configuration file

#ifndef _CONFIG_H
#define _CONFIG_H


//--------------------------------------
// SYS configuration (system)
#define SYS_SP_MIN    0x0900    // minimum SP addres


//--------------------------------------
// UART0 configuration
#define UART0                   // UART0 used
#define UART0_BAUD 115200       // baudrate 115200
#define UART0_IBUF 128          // input buffer size
//#define UART0_OBUF 16           // output buffer size
#define UART0_INBL              // input non blocking mode
#define UART0_FILE              // uart1io FILE stream
#define UART0_RFUL              // report rx full

//--------------------------------------
// UART1 configuration
#define UART1                   // UART1 used
#define UART1_BAUD 115200       // baudrate 115200
#define UART1_IBUF 128          // input buffer size
//#define UART1_OBUF 16           // output buffer size
#define UART1_INBL              // input non blocking mode
#define UART1_FILE              // uart1io FILE stream
#define UART1_RFUL              // report rx full


//--------------------------------------
// CMD configuration (file stream command processor)
#define CMD_MAX_LINE            64          // line buffer length


//--------------------------------------
// ADC configuration
#define ADC_CHAN_MSK            0x0060      // used AD channels bit mask (5,6)
#define ADC_CHAN_CNT            2           // number of used channels
#define ADC_OVRSAMPL            4           // oversampling multiplier
#define ADC_SEQ_LEN             2           // sampling sequence length
#define ADC_SEQ2IDX             adc_seq2idx // callback function (convert seq to channel index)
#define ADC_READY               adc_ready   // callback function
#define ADC_VREF                5010        // reference voltage [mV]


//--------------------------------------
// TIMER0
#define TIMER0_PRESCALER        (3 << CS00) // timer0 prescaler (fclk/64)
#define TIMER0_CYC_1MS          250         // timer0 cycles per 1ms


//--------------------------------------
// TMC2130
#define TMC2130_SPI_RATE        0 // fosc/4 = 4MHz
#define TMC2130_SPCR            SPI_SPCR(TMC2130_SPI_RATE, 1, 1, 1, 0)
#define TMC2130_SPSR            SPI_SPSR(TMC2130_SPI_RATE)
#define TMC2130_X_CS            GPIO_PG0
#define TMC2130_Y_CS            GPIO_PG2
#define TMC2130_Z_CS            GPIO_PK5
#define TMC2130_E_CS            GPIO_PK4
#define TMC2130_X_STP           GPIO_PC0
#define TMC2130_Y_STP           GPIO_PC1
#define TMC2130_Z_STP           GPIO_PC2
#define TMC2130_E_STP           GPIO_PC3
#define TMC2130_X_ENA           GPIO_PA7
#define TMC2130_Y_ENA           GPIO_PA6
#define TMC2130_Z_ENA           GPIO_PA5
#define TMC2130_E_ENA           GPIO_PA4
#define TMC2130_X_DIR           GPIO_PL0
#define TMC2130_Y_DIR           GPIO_PL1
#define TMC2130_Z_DIR           GPIO_PL2
#define TMC2130_E_DIR           GPIO_PL6
#define TMC2130_X_DIAG          GPIO_PK2
#define TMC2130_Y_DIAG          GPIO_PK7
#define TMC2130_Z_DIAG          GPIO_PK6
#define TMC2130_E_DIAG          GPIO_PK3


//--------------------------------------
// ST4 - stepper motion control
#define ST4_TIMER               1
#define ST4_NUMAXES             4
// direction control
#define ST4_GET_DIR             tmc2130_get_dir
#define ST4_SET_DIR             tmc2130_set_dir
// endstop sampling function
#define ST4_GET_END             tmc2130_get_diag
// step function
#define ST4_DO_STEP             tmc2130_step
#define ST4_DBG
// steprate to delay calculation method
#define ST4_SR2D2               0 // fast


//--------------------------------------
// FAN0 and FAN1
#define GPIO_FAN0               GPIO_PH5
#define GPIO_FAN1               GPIO_PH3
#define GPIO_FAN0_TACH          GPIO_PE6
#define GPIO_FAN1_TACH          GPIO_PE7


#define MK3TRAPEZ


#endif //_CONFIG_H
