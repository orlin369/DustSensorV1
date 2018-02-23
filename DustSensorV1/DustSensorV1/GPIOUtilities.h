/*

Copyright (c) [2018] [Orlin Dimitrov]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef _GPIOUTILITIES_h
#define _GPIOUTILITIES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma region Definitions

#pragma endregion

#pragma region Headers

#include "ApplicationConfiguration.h"

#pragma endregion

#pragma region Prototypes

/** @brief Set peripheral devices of the pot.
 *  @param uint8_t channel, analog channel.
 *  @param uint8_t samples, number of samples.
 *  @param uint8_t delay, delay between samples. [ms].
 *  @return uint16_t, Mid avg value.
 */
uint16_t avg_filter(uint8_t channel, uint8_t samples, uint8_t time);

/** @brief Set peripheral devices of the pot.
 *  @param uint8_t channel, analog channel.
 *  @param uint8_t value, analog value set point.
 *  @param uint8_t ramp, ramp time [ms].
 *  @return Void.
 */
void ramp_analog_out(uint8_t channel, uint8_t value, uint8_t ramp);

/** @brief Converts ADC value to Volts.
 *  @param uint8_t adcValue, ADC value.
 *  @return Void.
 */
float to_volts(uint16_t adcValue);

/** @brief Converts ADC value to mili Volts.
 *  @param uint8_t adcValue, ADC value.
 *  @return Void.
 */
float to_mili_volts(uint16_t adcValue);

#pragma endregion

#endif

