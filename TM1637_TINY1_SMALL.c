﻿/**
 * Copyright (c) 2017-2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * Copyright (c) 2022, Changed to CodevisonAVR version BY Pouria Amiri 
 * This is CODEVISIONAVR library for 4-Digit LED Display based on TM1637 chip.
 *
 * Features:
 * - display raw segments
 * - display digits
 * - display colon
 * - display on/off
 * - brightness control
 *
 * References:
 * - https://github.com/pouria-workshop/tm1637-library
 * - documentation: https://github.com/pouria-workshop/tm1637-library/README.md
 * - TM1637 datasheet: https://github.com/pouria-workshop/tm1637-library/blob/master/docs/TM1637_V2.4_EN.pdf
 */

#ifndef F_CPU
#define F_CPU 20000000UL
#endif

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "TM1637_TINY1_SMALL.h"

static uint8_t display_size;

/* PIN manipulation inline functions */

static inline uint8_t TM1637_DIO_READ(void) {
	return (TM1637_DIO_PORT.IN & TM1637_DIO_MASK) ? 1 : 0;
}

static inline void TM1637_DIO_HIGH() {
	TM1637_DIO_PORT.OUTSET = TM1637_DIO_MASK;
}

static inline void TM1637_DIO_LOW() {
	TM1637_DIO_PORT.OUTCLR = TM1637_DIO_MASK;
}

static inline void TM1637_DIO_OUTPUT() {
	TM1637_DIO_PORT.DIRSET = TM1637_DIO_MASK;
}

static inline void TM1637_DIO_INPUT() {
	TM1637_DIO_PORT.DIRCLR = TM1637_DIO_MASK;
}

// Note that we have to use conventional register access
// for the clock manipulation to work. Possibly related to timing issues

static inline void TM1637_CLK_HIGH() {
	TM1637_CLK_PORT.OUT |= TM1637_CLK_MASK;
}

static inline void TM1637_CLK_LOW() {
	TM1637_CLK_PORT.OUT &= ~TM1637_CLK_MASK;
}

static void TM1637_send_config(const uint8_t enable, const uint8_t brightness);
static void TM1637_send_command(const uint8_t value);
static void TM1637_start(void);
static void TM1637_stop(void);
static uint8_t TM1637_write_byte(uint8_t value);

static uint8_t _config = TM1637_SET_DISPLAY_ON | TM1637_BRIGHTNESS_MAX;
static uint8_t _segments = 0xff;
const uint8_t _digit2segments[] PROGMEM = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static const uint8_t minus_segments = 0b01000000;

uint8_t _digit_to_segments(uint8_t digit) {
	return (digit < 10 ? pgm_read_byte_near((uint8_t *)&_digit2segments + digit) : 0x00);
}

void TM1637_init(const uint8_t enable, const uint8_t brightness, uint8_t size) {
	TM1637_CLK_PORT.DIRSET = TM1637_CLK_MASK | TM1637_DIO_MASK;    // set CLK and DIO directions
	TM1637_DIO_PORT.OUTCLR = ~TM1637_DIO_MASK | TM1637_CLK_MASK;   // CLK and DIO line low
	TM1637_send_config(enable, brightness);
	display_size = size;
}

void TM1637_enable(const uint8_t value) {
	TM1637_send_config(value, _config & TM1637_BRIGHTNESS_MAX);
}

void TM1637_set_brightness(const uint8_t value) {
	TM1637_send_config(_config & TM1637_SET_DISPLAY_ON,
		value & TM1637_BRIGHTNESS_MAX);
}

void TM1637_display_segments(const uint8_t position, const uint8_t segments) {
	TM1637_send_command(TM1637_CMD_SET_DATA | TM1637_SET_DATA_F_ADDR);
	TM1637_start();
	TM1637_write_byte(TM1637_CMD_SET_ADDR | (position & (TM1637_POSITION_MAX - 1)));
	TM1637_write_byte(segments);
	TM1637_stop();
}

void TM1637_display_digit(const uint8_t position, const uint8_t digit) {
	uint8_t segments = _digit_to_segments(digit);

	if (position == 0x01) {
		segments = segments | (_segments & 0x80);
		_segments = segments;
	}

	TM1637_display_segments(position, segments);
}

void TM1637_display_colon(const uint8_t value) {

	if (value) {
		_segments |= 0x80;
	} else {
		_segments &= ~0x80;
	}
	TM1637_display_segments(0x01, _segments);
}

void TM1637_clear(void) {
	uint8_t i;

	for (i = 0; i < TM1637_POSITION_MAX; ++i) {
		TM1637_display_segments(i, 0x00);
	}
}

void TM1637_send_config(const uint8_t enable, const uint8_t brightness) {
	_config = (enable ? TM1637_SET_DISPLAY_ON : TM1637_SET_DISPLAY_OFF) |
		(brightness > TM1637_BRIGHTNESS_MAX ? TM1637_BRIGHTNESS_MAX : brightness);

	TM1637_send_command(TM1637_CMD_SET_DISPLAY | _config);
}

void TM1637_send_command(const uint8_t value) {
	TM1637_start();
	TM1637_write_byte(value);
	TM1637_stop();
}

void TM1637_start(void) {
	TM1637_DIO_HIGH();
	TM1637_CLK_HIGH();
	_delay_us(TM1637_DELAY_US);
	TM1637_DIO_LOW();
}

void TM1637_stop(void) {
	TM1637_CLK_LOW();
	_delay_us(TM1637_DELAY_US);

	TM1637_DIO_LOW();
	_delay_us(TM1637_DELAY_US);

	TM1637_CLK_HIGH();
	_delay_us(TM1637_DELAY_US);

	TM1637_DIO_HIGH();
}

uint8_t TM1637_write_byte(uint8_t value) {
	uint8_t i, ack;

	for (i = 0; i < 8; ++i, value >>= 1) {
		TM1637_CLK_LOW();
		_delay_us(TM1637_DELAY_US);

		if (value & 0x01) {
			TM1637_DIO_HIGH();
		} else {
			TM1637_DIO_LOW();
		}

		TM1637_CLK_HIGH();
		_delay_us(TM1637_DELAY_US);
	}

	TM1637_CLK_LOW();
	TM1637_DIO_INPUT();
	TM1637_DIO_HIGH();
	_delay_us(TM1637_DELAY_US);

	ack = TM1637_DIO_READ();
	if (ack) {
		TM1637_DIO_OUTPUT();
		TM1637_DIO_LOW();
	}
	_delay_us(TM1637_DELAY_US);

	TM1637_CLK_HIGH();
	_delay_us(TM1637_DELAY_US);

	TM1637_CLK_LOW();
	_delay_us(TM1637_DELAY_US);

	TM1637_DIO_OUTPUT();

	return ack;
}

void TM1637_display_number_decimal(int16_t num, bool leading_zero, uint8_t dot_position) {
	uint8_t postition_segments[display_size];
   if (num == 0 && !leading_zero) {
      for (uint8_t i = 0; i < display_size - 1; ++i) {
         postition_segments[i] = 0x00;
      }
      postition_segments[display_size] = _digit_to_segments(0);
   }
   else {
      uint8_t minimum_usable_position = (num < 0) ? 1 : 0;
      if (minimum_usable_position == 1) {
         postition_segments[0] = minus_segments;
         num = -num;
      }
      for(int8_t i = display_size - 1; i >= minimum_usable_position; i--) {
         uint8_t digit = num % 10;
         /* */
         if (digit == 0 && leading_zero == false) {
            postition_segments[i] = 0x00;
         } else {
            postition_segments[i] = _digit_to_segments(digit);
         }
         num /= 10;
      }
   }
   if (dot_position != 0) {
      postition_segments[dot_position] |= 0x80;
   }
   for(uint8_t i = 0; i < display_size; i++) {
   	  TM1637_display_segments(i, postition_segments[i]);
   }
}
