/*
 * ATtiny1616TM1637Test.c
 *
 * Created: 2024-04-26 4:35:59 PM
 * Author : ojisan
 */ 

#define F_CPU 20000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/cpufunc.h> 
#include <stdint.h>
#include <stdbool.h>
#include "TM1637_TINY1.h"



int main(void)
{
	ccp_write_io((void *)&(CLKCTRL.MCLKCTRLB), 0x00);			// disable system clock prescaler
	pin_config_t dio_config = create_pin_config(&VPORTA, 6);	// configuration for DIO line
    pin_config_t clk_config = create_pin_config(&VPORTA, 7);	// configuration for CLK line
    TM1637_init(dio_config, clk_config, 1, 5, 4);				// initialize display interface
    TM1637_clear();												// clear screen
    TM1637_display_number_decimal(8500, false, 1);				// display same number (85.00)
    while (1) {
        
    }
}
