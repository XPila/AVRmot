// gpio_atmega32u4.h
#ifndef _GPIO_ATMEGA32U4
#define _GPIO_ATMEGA32U4

// gpio definition - high nibble is port index (0=A,1=B...), low nibble is bit index (0-7)
#define GPIO_PB0   0x10
#define GPIO_PB1   0x11
#define GPIO_PB2   0x12
#define GPIO_PB3   0x13
#define GPIO_PB4   0x14
#define GPIO_PB5   0x15
#define GPIO_PB6   0x16
#define GPIO_PB7   0x17
#define GPIO_PC6   0x26
#define GPIO_PC7   0x27
#define GPIO_PD0   0x30
#define GPIO_PD1   0x31
#define GPIO_PD2   0x32
#define GPIO_PD3   0x33
#define GPIO_PD4   0x34
#define GPIO_PD5   0x35
#define GPIO_PD6   0x36
#define GPIO_PD7   0x37
#define GPIO_PE2   0x42
#define GPIO_PE6   0x46
#define GPIO_PF0   0x50
#define GPIO_PF1   0x51
#define GPIO_PF4   0x54
#define GPIO_PF5   0x55
#define GPIO_PF6   0x56
#define GPIO_PF7   0x57

#define __BIT_0x10   0
#define __BIT_0x11   1
#define __BIT_0x12   2
#define __BIT_0x13   3
#define __BIT_0x14   4
#define __BIT_0x15   5
#define __BIT_0x16   6
#define __BIT_0x17   7
#define __BIT_0x26   6
#define __BIT_0x27   7
#define __BIT_0x30   0
#define __BIT_0x31   1
#define __BIT_0x32   2
#define __BIT_0x33   3
#define __BIT_0x34   4
#define __BIT_0x35   5
#define __BIT_0x36   6
#define __BIT_0x37   7
#define __BIT_0x42   2
#define __BIT_0x46   6
#define __BIT_0x50   0
#define __BIT_0x51   1
#define __BIT_0x54   4
#define __BIT_0x55   5
#define __BIT_0x56   6
#define __BIT_0x57   7

#define __PIN_0x10   PINB
#define __PIN_0x11   PINB
#define __PIN_0x12   PINB
#define __PIN_0x13   PINB
#define __PIN_0x14   PINB
#define __PIN_0x15   PINB
#define __PIN_0x16   PINB
#define __PIN_0x17   PINB
#define __PIN_0x26   PINC
#define __PIN_0x27   PINC
#define __PIN_0x30   PIND
#define __PIN_0x31   PIND
#define __PIN_0x32   PIND
#define __PIN_0x33   PIND
#define __PIN_0x34   PIND
#define __PIN_0x35   PIND
#define __PIN_0x36   PIND
#define __PIN_0x37   PIND
#define __PIN_0x42   PINE
#define __PIN_0x46   PINE
#define __PIN_0x50   PINF
#define __PIN_0x51   PINF
#define __PIN_0x54   PINF
#define __PIN_0x55   PINF
#define __PIN_0x56   PINF
#define __PIN_0x57   PINF

#define __DDR_0x10   DDRB
#define __DDR_0x11   DDRB
#define __DDR_0x12   DDRB
#define __DDR_0x13   DDRB
#define __DDR_0x14   DDRB
#define __DDR_0x15   DDRB
#define __DDR_0x16   DDRB
#define __DDR_0x17   DDRB
#define __DDR_0x26   DDRC
#define __DDR_0x27   DDRC
#define __DDR_0x30   DDRD
#define __DDR_0x31   DDRD
#define __DDR_0x32   DDRD
#define __DDR_0x33   DDRD
#define __DDR_0x34   DDRD
#define __DDR_0x35   DDRD
#define __DDR_0x36   DDRD
#define __DDR_0x37   DDRD
#define __DDR_0x42   DDRE
#define __DDR_0x46   DDRE
#define __DDR_0x50   DDRF
#define __DDR_0x51   DDRF
#define __DDR_0x54   DDRF
#define __DDR_0x55   DDRF
#define __DDR_0x56   DDRF
#define __DDR_0x57   DDRF

#define __PORT_0x10   PORTB
#define __PORT_0x11   PORTB
#define __PORT_0x12   PORTB
#define __PORT_0x13   PORTB
#define __PORT_0x14   PORTB
#define __PORT_0x15   PORTB
#define __PORT_0x16   PORTB
#define __PORT_0x17   PORTB
#define __PORT_0x26   PORTC
#define __PORT_0x27   PORTC
#define __PORT_0x30   PORTD
#define __PORT_0x31   PORTD
#define __PORT_0x32   PORTD
#define __PORT_0x33   PORTD
#define __PORT_0x34   PORTD
#define __PORT_0x35   PORTD
#define __PORT_0x36   PORTD
#define __PORT_0x37   PORTD
#define __PORT_0x42   PORTE
#define __PORT_0x46   PORTE
#define __PORT_0x50   PORTF
#define __PORT_0x51   PORTF
#define __PORT_0x54   PORTF
#define __PORT_0x55   PORTF
#define __PORT_0x56   PORTF
#define __PORT_0x57   PORTF

#define __BIT(gpio) __BIT_##gpio
#define __MSK(gpio) (1 << __BIT(gpio))

#define __PIN(gpio) __PIN_##gpio
#define __PORT(gpio) __PORT_##gpio
#define __DDR(gpio) __DDR_##gpio

#define PIN(gpio) __PIN(gpio)
#define PORT(gpio) __PORT(gpio)
#define DDR(gpio) __DDR(gpio)

#define GPIO_INP(gpio) DDR(gpio) &= ~__MSK(gpio)
#define GPIO_OUT(gpio) DDR(gpio) |= __MSK(gpio)

#define GPIO_SET_L(gpio) PORT(gpio) &= ~__MSK(gpio)
#define GPIO_SET_H(gpio) PORT(gpio) |= __MSK(gpio)

#define GPIO_SET(gpio, val) if (val) GPIO_SET_H(gpio); else GPIO_SET_L(gpio)

#define GPIO_GET(gpio) ((PIN(gpio) & __MSK(gpio))?1:0)


#ifdef __cplusplus
extern "C" {
#endif //__cplusplus


extern uint8_t gpio_get(uint8_t gpio);

extern void gpio_set(uint8_t gpio, uint8_t val);

extern void gpio_cfg(uint8_t gpio, uint8_t cfg);


#ifdef __cplusplus
}
#endif //__cplusplus


#endif // _GPIO_ATMEGA32U4
