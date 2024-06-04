/**
 * Copyright (c) 2017-2018, Łukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * Copyright (c) 2022, Changed to CodevisonAVR version BY Pouria Amiri 
 * Copyright (c) 2024, Adapted for modern AVR devices/Atmel Studio 7 by Alan Duncan <duncan.alan@me.com>
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
#include <avr/io.h>
#include "config.h"

#ifndef TM1637_CLK_PORT
#error "TM1637_CLK_PORT must be defined in config.h, e.g. (PORTA)"
#endif

#ifndef TM1637_DIO_PORT
#error "TM1637_DIO_PORT must be defined in config.h, e.g. (PORTA)"
#endif

#ifndef TM1637_DIO_MASK
#error "TM1637_DIO_MASK must be defined in config.h - It will be something like (PIN6_bm)"
#endif

#ifndef TM1637_CLK_MASK
#error "TM1637_CLK_MASK must be fined in config.h - It will be something like (PINT7_bm)"
#endif

/**
 * @defgroup TM1637_macros TM1637 Display Macros
 * @brief Preprocessor macros for the TM1637 display module.
 * @{
 */

/**
 * @brief Delay in microseconds in the TM1637 protocol
 * 
 * This defines the number of microseconds between pulses in the TM1637 protocol.
 */
#define	TM1637_DELAY_US			(5)

/**
 * @brief Maximum brightness level of the display.
 * 
 * Defines the highest brightness to which the display can be set.
 */ 
#define	TM1637_BRIGHTNESS_MAX	(7)

/**
 * @brief Highest numbered digit 
 * 
 * Defines the highest numbered digit in the display
 */
#define	TM1637_POSITION_MAX		(4)

/**
 * @defgroup TM1637 commands
 * 
 * @brief Commands for the TM1637 protocol
 * @{
 */
// TM1637 commands
#define	TM1637_CMD_SET_DATA		 	0x40
#define	TM1637_CMD_SET_ADDR			0xC0
#define	TM1637_CMD_SET_DISPLAY		0x80

// TM1637 data settings (use bitwise OR to construct complete command)
#define	TM1637_SET_DATA_WRITE		0x00 // write data to the display register
#define	TM1637_SET_DATA_READ		0x02 // read the key scan data
#define	TM1637_SET_DATA_A_ADDR		0x00 // automatic address increment
#define	TM1637_SET_DATA_F_ADDR		0x04 // fixed address
#define	TM1637_SET_DATA_M_NORM		0x00 // normal mode
#define	TM1637_SET_DATA_M_TEST		0x10 // test mode

// TM1637 display control command set (use bitwise OR to construct complete command)
#define	TM1637_SET_DISPLAY_OFF		0x00 // off
#define	TM1637_SET_DISPLAY_ON		0x08 // on

/**
 * @}
 */

/**
 * @}
 */

/**
 * @brief Initialize the TM1637 display.
 *
 * This function initializes the TM1637 display module with the specified parameters.
 *
 * @param enable     Set to 1 to enable the display, 0 to disable
 * @param brightness The brightness level of the display (0-7).
 * @param size       The size of the display (number of digits).
 *
 * @note The brightness level can range from 0 (minimum brightness) to 7 (maximum brightness).
 * @note The size parameter determines the number of digits the display will show.
 *
 * @return None.
 */
void TM1637_init(const uint8_t enable, const uint8_t brightness, uint8_t size);

/**
 * @brief Enables and displayes the TM1637 display
 * 
 * This function permits the caller to enable or disable the display
 * 
 * @param value		Set to 1 to enable the display or 0 to disable
 * 
 * @return None.
 */ 
void TM1637_enable(const uint8_t value);

/**
 * Set display brightness.
 * Min value: 0
 * Max value: 7
 */
/**
 * @brief Sets the display brightness.
 * 
 * This function sets the brightness level of the display
 * 
 * @param value		Set to 0 for minimum brightness, 7 for maximum or any integer between
 * @return None.
 */
void TM1637_set_brightness(const uint8_t value);


/**
 * @brief Displays a pattern of segments on the TM1637 display.
 * 
 * This function displays a pattern of individual segments at the designated
 * position on the TM1637 display. Segments allow the caller to display symbols
 * that do not conform to the 0-9 digit patterns.
 * 
 * @param position The digit location (0-3) in the display.
 * @param pattern  A bit field representing the segments to be displayed.
 *                 Each bit position corresponds to the following guide:
 * 
 *                 Bit Positions:
 *                 ---------
 *                |    0    |
 *                | 5      1|
 *                |    6    |
 *                | 4      2|
 *                |    3    |
 *                 ---------
 * 
 *                 Example segment configurations:
 *                 - For character 'H', pattern = 0b01110110
 *                 - For character '-', pattern = 0b01000000
 */
void TM1637_display_segments(const uint8_t position, const uint8_t pattern);


/**
 * @brief Displays a single digit
 * 
 * This function displays a single digit at a specified position in the display.
 * 
 * @param position 	The position, with 0 being the left-most element
 * @param digit 	The decimal digit to display
 * 
 * @return None.
 */
void TM1637_display_digit(const uint8_t position, const uint8_t digit);

/**
 * @brief Turns the colon on or off
 * 
 * This function allows the caller to turn the colon on or off.
 * 
 * @param value 	Set 1 to show the colon or 0 to turn it off
 * 
 * @note If the particular display does not have a colon or decimal points, this function has no action.
 * @note If there are decimal points but no colon, the function manipulates the 2nd decimal point
 *       which is located at the 1 position of the display.
void TM1637_display_colon(const uint8_t value);


void TM1637_display_number_decimal(int16_t num, bool leading_zero, uint8_t dot_position);

/**
 * @brief Clears the display
 * 
 * This function clears the display, including decimal points and colon if present.
 * 
 * @return None.
 */
void TM1637_clear(void);

#endif	/* !_TM1637_H_ */
