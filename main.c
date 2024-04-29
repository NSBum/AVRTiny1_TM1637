/*
 * ATtiny1616TM1637Test.c
 *
 * Created: 2024-04-26 4:35:59 PM
 * Author : ojisan
 */ 

/************************************************************************* 
* Title:    ATtiny1616TM1637Test
* Author:   Alan Duncan <duncan.alan@me.com>  https://ojisanseiuchi.com
* Github:	https://github.com/NSBum/AVRTiny1_TM1637 <@NSBum>
* File:     main.c
* Date:     2024-04-29 04-50-52* Software: AVR-GCC 5.4.0 / avr-libc 1.2.3
* Target:   ATtiny 1616
* Usage:    Test the TM1637_TINY1 library
**************************************************************************/

#define F_CPU 20000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/cpufunc.h> 
#include <stdint.h>
#include <stdbool.h>
#include "TM1637_TINY1_SMALL.h"

void _wait_clear() {
	_delay_ms(500);
	TM1637_clear();
}


int main(void)
{
    ccp_write_io((void *)&(CLKCTRL.MCLKCTRLB), 0x00);           // disable system clock prescaler
    TM1637_init(1, 5, 4);										// initialize display interface
    TM1637_clear();                                             // clear screen
	TM1637_display_digit(0,9);

	/* Run through some usage examples */
    while (1) {
        TM1637_display_digit(0,9);
		_wait_clear();

		TM1637_display_segments(1, 0b01110110);
		_wait_clear();

		TM1637_display_number_decimal(7599, false, 1); 
		_wait_clear();

		TM1637_display_number_decimal(-234, false, 0); 
		_wait_clear();
    }
}
