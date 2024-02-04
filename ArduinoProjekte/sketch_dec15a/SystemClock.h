/*
    SystemClock.h - Functions to initialize and use a system clock
    on AVR chips that is compatible with Arduino.

    Copyright (c) 2018 Wolfgang Zukrigl. Just added cosmetic changes to
    the code from Igor Mikolic-Torreiras from the AVRTools library

    Copyright (c) 2014 Igor Mikolic-Torreira.  All right reserved.

    Functions delayMicroseconds() and delayMilliseconds() adapted from
    Arduino code that is
    Copyright (c) 2005-2006 David A. Mellis and licensed under LGPL.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



/*!
 * \file
 *
 * \brief Include this file to use the functions that instantiate and access a system clock
 * that counts elapsed milliseconds.
 *
 * To use these functions, include SystemClock.h in your source code and link against SystemClock.c.
 *
 * \note Linking against SystemClock.c installs a interrupt function on timer0.  This interrupt
 * routine is installed regardless of whether the system clock is actually initialized or not.
 * If you have other uses for timer0, do not use SystemClock functions and do not link against SystemClock.c.
 */



#ifndef SystemClock_h
#define SystemClock_h

#include <stdint.h>

#ifndef F_CPU
    #error "F_CPU must be defined, to use the SystemClock-Module"
#endif

#define clockCyclesPerMicrosecond()         ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds( a )      ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles( a )      ( (a) * clockCyclesPerMicrosecond() )

#ifdef __cplusplus
extern "C"{
#endif

/*!
 * \brief This function initializes a system clock that tracks elapsed milliseconds.
 *
 * The system clock uses timer0, so you cannot use timer0 for other functions if you
 * use the system clock functionality.
 *
 * \note Linking against SystemClock.c installs a interrupt function on timer0.  This interrupt
 * routine is installed regardless of whether the system clock is actually initialized or not.
 * If you have other uses for timer0, do not use SystemClock functions and do not link against SystemClock.c.
 *
 * @param enableInterrupts If 0, interrupts will not be globally enabled and must be enabled later
 *        by calling sei(). If not-zero, interrupts are globally enabled.
 */
void initTimer0AsSystemClock(uint8_t enableInterrupts);


/*!
 * \brief Delay a certain number of microseconds.
 * This function works independently of Timer0 (It is not necessary to call `initTimer0AsSystemClock()` before using
 * `delayMicroseconds`.
 *
 * \arg \c us the number of microseconds to delay.
 *
 * \note This function only works for CPU clocks running at either 8 MHz, 12 MHz, or 16 MHz.
 */
void delayMicroseconds(uint16_t us );


/*!
 * \brief Delay a certain number of milliseconds.
 * Note: initTimer0AsSystemClock() must have been called, and interrupts must be globally enabled
 * before using this function.
 *
 * \arg \c ms the number of milliseconds to delay.
 */
void delayMilliseconds( uint32_t ms );


/*!
 * \brief Delay a certain number of milliseconds.
 *
 * This inline function is a synonym for delayMilliseconds(); it is provided for
 * compatibility with the standard Arduino library.
 * Note: initTimer0AsSystemClock() must have been called, and interrupts must be globally enabled
 * before using this function.
 *
 * \arg \c ms the number of milliseconds to delay.
 *
 */

inline void delay( uint32_t ms )
{ delayMilliseconds( ms ); }


/*!
 * \brief Return the number of elapsed microseconds since the system clock was turned on using
 * `initTimer0AsSystemClock()` and globally enabling interrupts.
 *
 * The microsecond count will overflow back to zero after 2^32 microseconds, which is
 * approximately 70 minutes.
 *
 * \returns the number of elapsed microseconds.
 *
 */
uint32_t micros();


/*!
 * \brief Return the number of elasped milliseconds since the system clock was turned on using
 * `initTimer0AsSystemClock` and globally enabling interrupts.
 * The millisecond count will overflow back to zero after 2^32 milliseconds, which is
 * approximately 49 days.
 *
 * \returns the number of elapsed milliseconds.
 *
 */
uint32_t millis();

#ifdef __cplusplus
} // extern "C"
#endif

#endif

