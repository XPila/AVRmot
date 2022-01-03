// gpio_atmega2560.h
#ifndef _GPIO_ATMEGA2560
#define _GPIO_ATMEGA2560

// gpio definition - high nibble is port index (0=A,1=B...), low nibble is bit index (0-7)
#define GPIO_PA0   0x00
#define GPIO_PA1   0x01
#define GPIO_PA2   0x02
#define GPIO_PA3   0x03
#define GPIO_PA4   0x04
#define GPIO_PA5   0x05
#define GPIO_PA6   0x06
#define GPIO_PA7   0x07
#define GPIO_PB0   0x10
#define GPIO_PB1   0x11
#define GPIO_PB2   0x12
#define GPIO_PB3   0x13
#define GPIO_PB4   0x14
#define GPIO_PB5   0x15
#define GPIO_PB6   0x16
#define GPIO_PB7   0x17
#define GPIO_PC0   0x20
#define GPIO_PC1   0x21
#define GPIO_PC2   0x22
#define GPIO_PC3   0x23
#define GPIO_PC4   0x24
#define GPIO_PC5   0x25
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
#define GPIO_PE0   0x40
#define GPIO_PE1   0x41
#define GPIO_PE2   0x42
#define GPIO_PE3   0x43
#define GPIO_PE4   0x44
#define GPIO_PE5   0x45
#define GPIO_PE6   0x46
#define GPIO_PE7   0x47
#define GPIO_PF0   0x50
#define GPIO_PF1   0x51
#define GPIO_PF2   0x52
#define GPIO_PF3   0x53
#define GPIO_PF4   0x54
#define GPIO_PF5   0x55
#define GPIO_PF6   0x56
#define GPIO_PF7   0x57
#define GPIO_PG0   0x60
#define GPIO_PG1   0x61
#define GPIO_PG2   0x62
#define GPIO_PG3   0x63
#define GPIO_PG4   0x64
#define GPIO_PG5   0x65
#define GPIO_PH0   0x70
#define GPIO_PH1   0x71
#define GPIO_PH2   0x72
#define GPIO_PH3   0x73
#define GPIO_PH4   0x74
#define GPIO_PH5   0x75
#define GPIO_PH6   0x76
#define GPIO_PH7   0x77

#define __BIT_0x00   0
#define __BIT_0x01   1
#define __BIT_0x02   2
#define __BIT_0x03   3
#define __BIT_0x04   4
#define __BIT_0x05   5
#define __BIT_0x06   6
#define __BIT_0x07   7
#define __BIT_0x10   0
#define __BIT_0x11   1
#define __BIT_0x12   2
#define __BIT_0x13   3
#define __BIT_0x14   4
#define __BIT_0x15   5
#define __BIT_0x16   6
#define __BIT_0x17   7
#define __BIT_0x20   0
#define __BIT_0x21   1
#define __BIT_0x22   2
#define __BIT_0x23   3
#define __BIT_0x24   4
#define __BIT_0x25   5
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
#define __BIT_0x40   0
#define __BIT_0x41   1
#define __BIT_0x42   2
#define __BIT_0x43   3
#define __BIT_0x44   4
#define __BIT_0x45   5
#define __BIT_0x46   6
#define __BIT_0x47   7
#define __BIT_0x50   0
#define __BIT_0x51   1
#define __BIT_0x52   2
#define __BIT_0x53   3
#define __BIT_0x54   4
#define __BIT_0x55   5
#define __BIT_0x56   6
#define __BIT_0x57   7
#define __BIT_0x60   0
#define __BIT_0x61   1
#define __BIT_0x62   2
#define __BIT_0x63   3
#define __BIT_0x64   4
#define __BIT_0x65   5
#define __BIT_0x70   0
#define __BIT_0x71   1
#define __BIT_0x72   2
#define __BIT_0x73   3
#define __BIT_0x74   4
#define __BIT_0x75   5
#define __BIT_0x76   6
#define __BIT_0x77   7

#define __PIN_0x00   PINA
#define __PIN_0x01   PINA
#define __PIN_0x02   PINA
#define __PIN_0x03   PINA
#define __PIN_0x04   PINA
#define __PIN_0x05   PINA
#define __PIN_0x06   PINA
#define __PIN_0x07   PINA
#define __PIN_0x10   PINB
#define __PIN_0x11   PINB
#define __PIN_0x12   PINB
#define __PIN_0x13   PINB
#define __PIN_0x14   PINB
#define __PIN_0x15   PINB
#define __PIN_0x16   PINB
#define __PIN_0x17   PINB
#define __PIN_0x20   PINC
#define __PIN_0x21   PINC
#define __PIN_0x22   PINC
#define __PIN_0x23   PINC
#define __PIN_0x24   PINC
#define __PIN_0x25   PINC
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
#define __PIN_0x40   PINE
#define __PIN_0x41   PINE
#define __PIN_0x42   PINE
#define __PIN_0x43   PINE
#define __PIN_0x44   PINE
#define __PIN_0x45   PINE
#define __PIN_0x46   PINE
#define __PIN_0x47   PINE
#define __PIN_0x50   PINF
#define __PIN_0x51   PINF
#define __PIN_0x52   PINF
#define __PIN_0x53   PINF
#define __PIN_0x54   PINF
#define __PIN_0x55   PINF
#define __PIN_0x56   PINF
#define __PIN_0x57   PINF
#define __PIN_0x60   PING
#define __PIN_0x61   PING
#define __PIN_0x62   PING
#define __PIN_0x63   PING
#define __PIN_0x64   PING
#define __PIN_0x65   PING
#define __PIN_0x70   PINH
#define __PIN_0x71   PINH
#define __PIN_0x72   PINH
#define __PIN_0x73   PINH
#define __PIN_0x74   PINH
#define __PIN_0x75   PINH
#define __PIN_0x76   PINH
#define __PIN_0x77   PINH

#define __DDR_0x00   DDRA
#define __DDR_0x01   DDRA
#define __DDR_0x02   DDRA
#define __DDR_0x03   DDRA
#define __DDR_0x04   DDRA
#define __DDR_0x05   DDRA
#define __DDR_0x06   DDRA
#define __DDR_0x07   DDRA
#define __DDR_0x10   DDRB
#define __DDR_0x11   DDRB
#define __DDR_0x12   DDRB
#define __DDR_0x13   DDRB
#define __DDR_0x14   DDRB
#define __DDR_0x15   DDRB
#define __DDR_0x16   DDRB
#define __DDR_0x17   DDRB
#define __DDR_0x20   DDRC
#define __DDR_0x21   DDRC
#define __DDR_0x22   DDRC
#define __DDR_0x23   DDRC
#define __DDR_0x24   DDRC
#define __DDR_0x25   DDRC
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
#define __DDR_0x40   DDRE
#define __DDR_0x41   DDRE
#define __DDR_0x42   DDRE
#define __DDR_0x43   DDRE
#define __DDR_0x44   DDRE
#define __DDR_0x45   DDRE
#define __DDR_0x46   DDRE
#define __DDR_0x47   DDRE
#define __DDR_0x50   DDRF
#define __DDR_0x51   DDRF
#define __DDR_0x52   DDRF
#define __DDR_0x53   DDRF
#define __DDR_0x54   DDRF
#define __DDR_0x55   DDRF
#define __DDR_0x56   DDRF
#define __DDR_0x57   DDRF
#define __DDR_0x60   DDRG
#define __DDR_0x61   DDRG
#define __DDR_0x62   DDRG
#define __DDR_0x63   DDRG
#define __DDR_0x64   DDRG
#define __DDR_0x65   DDRG
#define __DDR_0x70   DDRH
#define __DDR_0x71   DDRH
#define __DDR_0x72   DDRH
#define __DDR_0x73   DDRH
#define __DDR_0x74   DDRH
#define __DDR_0x75   DDRH
#define __DDR_0x76   DDRH
#define __DDR_0x77   DDRH

#define __PORT_0x00   PORTA
#define __PORT_0x01   PORTA
#define __PORT_0x02   PORTA
#define __PORT_0x03   PORTA
#define __PORT_0x04   PORTA
#define __PORT_0x05   PORTA
#define __PORT_0x06   PORTA
#define __PORT_0x07   PORTA
#define __PORT_0x10   PORTB
#define __PORT_0x11   PORTB
#define __PORT_0x12   PORTB
#define __PORT_0x13   PORTB
#define __PORT_0x14   PORTB
#define __PORT_0x15   PORTB
#define __PORT_0x16   PORTB
#define __PORT_0x17   PORTB
#define __PORT_0x20   PORTC
#define __PORT_0x21   PORTC
#define __PORT_0x22   PORTC
#define __PORT_0x23   PORTC
#define __PORT_0x24   PORTC
#define __PORT_0x25   PORTC
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
#define __PORT_0x40   PORTE
#define __PORT_0x41   PORTE
#define __PORT_0x42   PORTE
#define __PORT_0x43   PORTE
#define __PORT_0x44   PORTE
#define __PORT_0x45   PORTE
#define __PORT_0x46   PORTE
#define __PORT_0x47   PORTE
#define __PORT_0x50   PORTF
#define __PORT_0x51   PORTF
#define __PORT_0x52   PORTF
#define __PORT_0x53   PORTF
#define __PORT_0x54   PORTF
#define __PORT_0x55   PORTF
#define __PORT_0x56   PORTF
#define __PORT_0x57   PORTF
#define __PORT_0x60   PORTG
#define __PORT_0x61   PORTG
#define __PORT_0x62   PORTG
#define __PORT_0x63   PORTG
#define __PORT_0x64   PORTG
#define __PORT_0x65   PORTG
#define __PORT_0x70   PORTH
#define __PORT_0x71   PORTH
#define __PORT_0x72   PORTH
#define __PORT_0x73   PORTH
#define __PORT_0x74   PORTH
#define __PORT_0x75   PORTH
#define __PORT_0x76   PORTH
#define __PORT_0x77   PORTH

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


#endif // _GPIO_ATMEGA2560
