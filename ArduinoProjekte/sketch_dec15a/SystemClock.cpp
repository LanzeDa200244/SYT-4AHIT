/*
    SystemClock.cpp - Functions to initialize and use a system clock
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



#include "SystemClock.h"


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>



// The prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define kMicrosecondsPerOverflowTIMER0  ( (unsigned long) ( clockCyclesToMicroseconds( 64 * 256 ) ) )

// The whole number of milliseconds per timer0 overflow
#define kMillisInc ( (unsigned long) ( kMicrosecondsPerOverflowTIMER0 / 1000 ) )

// The fractional number of milliseconds per timer0 overflow. Shift right
// by three to fit these numbers into a byte (for 8 MHz, 12 MHz, and 16 MHz this doesn't lose precision).
#define kFractInc  ( (uint8_t) ( ( kMicrosecondsPerOverflowTIMER0 % 1000 ) >> 3 ) )
#define kFractMax  ( (uint8_t)  ( 1000 >> 3 ) )

// Variables to keep track of time
volatile uint32_t      timer0_overflow_count;
volatile uint32_t      timer0_millis;
uint8_t                timer0_fract;  //doesn't need volatile




ISR( TIMER0_OVF_vect )
{
    // Copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = timer0_millis;
    uint8_t f = timer0_fract;

    m += kMillisInc;
    f += kFractInc;
    if ( f >= kFractMax )
    {
        f -= kFractMax;
        ++m;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}



uint32_t millis()
{
    // Disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    uint32_t m;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        m = timer0_millis;
    }

    return m;
}




uint32_t micros()
{
    // Disable interrupts to avoid reading inconsistent values
    uint32_t m;
    uint8_t t;

    ATOMIC_BLOCK( ATOMIC_RESTORESTATE ) //Here, interrupts are disabled
    {
        m = timer0_overflow_count;
        t = TCNT0;

        //TCNT0 is still counting. Therefore check, if TCNT0 just had an overflow. If so, increase the
        //copy of timer0_overflow_count, because the interrupt-service-routine had not been executed yet, and has
        //not yet increased timer0_overflow_count.
        if ( ( TIFR0 & _BV(TOV0) ) && ( t < 255 ) )
        {
            m++;
        }
    }

    return ( (m << 8) + t ) * ( 64 / clockCyclesPerMicrosecond() );
}




void delayMilliseconds( uint32_t ms )
{
    uint16_t start = (uint16_t) micros();

    while ( ms > 0 )
    {
        if ( ( ((uint16_t) micros()) - start ) >= 1000 )
        {
            ms--;
            start += 1000;
        }
    }
}




// Delay for the given number of microseconds.  Assumes an 8 MHz, 12 MHz, or 16 MHz clock.
void delayMicroseconds( uint16_t us )
{
    // Code adapted from wiring.c, Copyright (c) 2005-2006 David A. Mellis

    // Calling avrlib's delay_us() function with low values (e.g. 1 or
    // 2 microseconds) gives delays longer than desired.
    // delay_us( us);

#if F_CPU == 16000000L

    // For the 16 MHz clock on most Arduino boards.

    // 16 cycles = 1 us

    // The overhead of the function call is 14 cycles for
    // constant delay, 16 for variable delay, which is 1us

    // For a one-microsecond delay, simply return

    // Takes 3 cycles if false, 4 cycles if true
    if ( us <= 1 )
    {
        return;
    }

    // The timing loop takes 1/4 of a microsecond (4 cycles) per iteration
    // So execute it four times for each microsecond of delay requested.

    // Multiply by 4; takes 4 cycles
    us <<= 2;

    // Account for the time taken in the preceeding commands.  We just
    // burned 21 (23) cycles above, plus 2 cycles for the adjustment below.
    // Total 23 (25) cycles, so remove 6 iterations, (6*4=24)
    // us is at least 8 so we can substract 6

    // Takes 2 cycles
    us -= 6;

#elif F_CPU == 12000000L

    // For the 12 MHz clock of the Adafruit Trinket and others.

    // 12 cycles - 1 us

    // The overhead of the function call is 14 (16) cycles, which is 1.5us

    // For a 1 microsecond delay, simply return

    // Takes 3 cycles if false, 4 cycles if true
    if ( us <= 1 )
    {
        return;
    }

    // The timing loop takes 1/3 of a microsecond (4 cycles) per iteration
    // So execute it three times for each microsecond of delay requested.

    // Multiply by 3 (actually by 2 and add again); takes 5 cycles
    us = (us << 1) + us;

    // Account for the time taken in the preceeding commands.  We just
    // burned 22 (24) cycles above, plus 2 cycles for the adjustment below.
    // Total 24 (26) cycles, so remove 6 iterations, (6*4=24)
    // us is at least 6 so at most we can substract 5 (us ==0 breaks the loop)
    // Close enough....

    // Takes 2 cycles
    us -= 5;

#elif F_CPU == 8000000L

    // For the 8 MHz internal clock on the ATmega168

    // 8 cycles = 1 us

    // The overhead of the function call is 14 (16) cycles, which is 2us

    // For a 1 or 2 microsecond delay, simply return

    // Takes 3 cycles if false, 4 cycles if true
    if ( us <= 2 )
    {
        return;
    }

    // The timing loop takes 1/2 of a microsecond (4 cycles) per iteration
    // So execute it twice for each microsecond of delay requested.

    // Multiply by 2; takes 2 cycles
    us <<= 1;

    // Account for the time taken in the preceeding commands.  We just
    // burned 19 (21) cycles above, plus 2 cycles for the adjustment below.
    // Total 21 (23) cycles, so remove 5 iterations, (5*4=20)
    // us is at least 6 so at most we can substract 5
    // Close enough....

    // Takes 2 cycles
    us -= 5;

#else

#error "delayMicroseconds() is only implemented for CPU speeds of 8 MHz, 12 MHz, or 16 MHz."

#endif

    // Busy wait
    __asm__ __volatile__
    (
        "1: sbiw %0,1" "\n\t" // 2 cycles
        "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
    );
}




void initTimer0AsSystemClock(uint8_t enableInterrupts)
{
    ATOMIC_BLOCK( ATOMIC_RESTORESTATE )
    {
        // Use Timer0 for the system clock, but configure it so it also supports
        // fast hardware pwm (using phase-correct PWM would mean that Timer0
        // overflowed half as often)

        TCCR0A = 0;     // Clear all settings
        TCCR0B = 0;     // Clear all settings
        TIMSK0 = 0;     // Disable all interrupts
        TCNT0  = 0;     // initialize counter value to 0

        TCCR0A |= (1 << WGM01) | (1 << WGM00);

        // set timer 0 prescale factor to 64
        TCCR0B |= (1 << CS01) | (1 << CS00);

        // enable timer 0 overflow interrupt
        TIMSK0 |= (1 << TOIE0);

        // Reset counters
        timer0_overflow_count = 0;
        timer0_millis = 0;
        timer0_fract = 0;
    }

    if (enableInterrupts)
        sei();
}


