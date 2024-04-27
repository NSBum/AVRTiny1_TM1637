/**
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

#ifndef	_TM1637_H_
#define	_TM1637_H_

#include <stdint.h>
#include <stdbool.h>

// Main Settings
#define	TM1637_DIO_MASK			PIN6_bm
#define	TM1637_CLK_MASK			PIN7_bm
#define TM1637_DIO_PIN			PIN6_bp
#define TM1637_CLK_PIN			PIN7_bp

#define	TM1637_DELAY_US			(5)
#define	TM1637_BRIGHTNESS_MAX	(7)
#define	TM1637_POSITION_MAX		(4)

// TM1637 commands
#define	TM1637_CMD_SET_DATA		0x40
#define	TM1637_CMD_SET_ADDR		0xC0
#define	TM1637_CMD_SET_DSIPLAY		0x80

// TM1637 data settings (use bitwise OR to contruct complete command)
#define	TM1637_SET_DATA_WRITE		0x00 // write data to the display register
#define	TM1637_SET_DATA_READ		0x02 // read the key scan data
#define	TM1637_SET_DATA_A_ADDR		0x00 // automatic address increment
#define	TM1637_SET_DATA_F_ADDR		0x04 // fixed address
#define	TM1637_SET_DATA_M_NORM		0x00 // normal mode
#define	TM1637_SET_DATA_M_TEST		0x10 // test mode

// TM1637 display control command set (use bitwise OR to consruct complete command)
#define	TM1637_SET_DISPLAY_OFF		0x00 // off
#define	TM1637_SET_DISPLAY_ON		0x08 // on


/**
 * Initialize TM1637 display driver.
 * Clock pin (TM1637_CLK_PIN) and data pin (TM1637_DIO_PIN)
 * are defined at the top of this file.
 * Display size in number of digits
 */
void TM1637_init(const uint8_t enable, const uint8_t brightness, uint8_t size);

/**
 * Turn display on/off.
 * value: 1 - on, 0 - off
 */
void TM1637_enable(const uint8_t value);

/**
 * Set display brightness.
 * Min value: 0
 * Max value: 7
 */
void TM1637_set_brightness(const uint8_t value);

/**
 * Display raw segments at position (0x00..0x03)
 *
 *      bits:
 *        -- 0 --
 *       |       |
 *       5       1
 *       |       |
 *        -- 6 --
 *       |       |
 *       4       2
 *       |       |
 *        -- 3 --
 *
 * Example segment configurations:
 * - for character 'H', segments=0b01110110
 * - for character '-', segments=0b01000000
 * - etc.
 */
void TM1637_display_segments(const uint8_t position, const uint8_t segments);

/**
 * Display digit ('0'..'9') at position (0x00..0x03)
 */
void TM1637_display_digit(const uint8_t position, const uint8_t digit);

/**
 * Display colon on/off.
 * value: 1 - on, 0 - off
 */
void TM1637_display_colon(const uint8_t value);

void TM1637_display_number_decimal(int16_t num, bool leading_zero, uint8_t dot_position);

/**
 * Clear all segments (including colon).
 */
void TM1637_clear(void);

#endif	/* !_TM1637_H_ */
