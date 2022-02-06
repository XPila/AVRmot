// config.h - main configuration file

#ifndef _CONFIG_H
#define _CONFIG_H


//--------------------------------------
// SYS configuration (system)
#define SYS_SP_MIN    0x0900    // minimum SP addres


//--------------------------------------
// UART configuration
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
// RX and TX led
#define RX_LED_GPIO             GPIO_PB0
#define TX_LED_GPIO             GPIO_PD5


//--------------------------------------
// SHR16 - 16bit shift register control signals (data, latch, clock)
#define SHR16_DAT               GPIO_PB5
#define SHR16_LAT               GPIO_PB6
#define SHR16_CLK               GPIO_PC7
// SHR16 LED signals (shr16 bit 8-15 and 13-14)
#define SHR16_LED_MSK_0         0xff    // first byte mask
#define SHR16_LED_MSK_1         0x03    // second byte mask
#define SHR16_LED_SHI_0         8       // first byte shift
#define SHR16_LED_SHI_1         6       // second byte shift
// SHR16 ENA/DIR signals (shr16 bit 1,3,5 and 0,2,4)
#define SHR16_ENADIR_MSK        0x3f    // ena/dir mask
#define SHR16_ENADIR_SHI        0       // ena/dir shift
#define SHR16_INIT              0x002a  // initial value (ena XYZ = high)


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
#define TMC2130_X_STP           GPIO_PD6
#define TMC2130_Y_STP           GPIO_PD4
#define TMC2130_Z_STP           GPIO_PB4
#define TMC2130_X_CS            GPIO_PB7
#define TMC2130_Y_CS            GPIO_PD7
#define TMC2130_Z_CS            GPIO_PC6
// SHR16 ENA signals (shr16 bit 1,3,5)
#define TMC2130_X_ENA_SHR16     5
#define TMC2130_Y_ENA_SHR16     3
#define TMC2130_Z_ENA_SHR16     1
// SHR16 DIR signals (shr16 bit 0,2,4)
#define TMC2130_X_DIR_SHR16     4
#define TMC2130_Y_DIR_SHR16     2
#define TMC2130_Z_DIR_SHR16     0


//--------------------------------------
// ST4 - stepper motion control
#define ST4_TIMER               1
#define ST4_NUMAXES             3
// direction control
#define ST4_GET_DIR             tmc2130_get_dir
#define ST4_SET_DIR             tmc2130_set_dir
// endstop sampling function
#define ST4_GET_END             tmc2130_get_diag
// step function
#define ST4_DO_STEP             tmc2130_step
// steprate to delay calculation method
#define ST4_SR2D2               1


#endif //_CONFIG_H
