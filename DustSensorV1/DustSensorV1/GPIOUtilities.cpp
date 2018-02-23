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


#include "GPIOUtilities.h"

/** @brief Set peripheral devices of the pot.
 *  @param uint8_t channel, analog channel.
 *  @param uint8_t samples, number of samples.
 *  @param uint8_t delay, delay between samples. [ms].
 *  @return uint16_t, Mid avg value.
 */
uint16_t avg_filter(uint8_t channel, uint8_t samples, uint8_t time)
{
	// 
	uint16_t OutputValueL = 0;

	// Sum number of samples.
	for (uint8_t index = 0; index < samples; index++)
	{
		OutputValueL += analogRead(channel);
		delay(time);
	}

	// Divide output value to number of the samples.
	OutputValueL = (uint16_t)(OutputValueL / samples);

	return OutputValueL;
}

/** @brief Set peripheral devices of the pot.
 *  @param uint8_t channel, analog channel.
 *  @param uint8_t value, analog value set point.
 *  @param uint8_t ramp, ramp time [ms].
 *  @return Void.
 */
void ramp_analog_out(uint8_t channel, uint8_t value, uint8_t ramp)
{
	static uint8_t OldValueL[20];

	if (value > OldValueL[channel])
	{
		// Fade in from min to max in increments of 1:
		for (uint8_t fadeValue = OldValueL[channel]; fadeValue <= value; fadeValue++)
		{
			// Sets the value (range from 0 to 255):
#ifdef ESP8266
			analogWrite(channel, fadeValue);
			//CAYENNE_PRINT.println(fadeValue);
#endif
#ifdef ESP32
			sigmaDeltaWrite(channel, fadeValue);
#endif
			// Wait for ramp milliseconds to see the dimming effect.
			delay(ramp);

			if (fadeValue == 255) break;
		}

		// Update old value.
		OldValueL[channel] = value;
	}
	else if (value < OldValueL[channel])
	{
		// Fade out from max to min in increments of 1:
		for (uint8_t fadeValue = OldValueL[channel]; fadeValue >= value; fadeValue--)
		{
			// Sets the value (range from 0 to 255):
#ifdef ESP8266
			analogWrite(channel, fadeValue);
			//CAYENNE_PRINT.println(fadeValue);
#endif
#ifdef ESP32
			sigmaDeltaWrite(channel, fadeValue);
			CAYENNE_PRINT.println(fadeValue);
#endif
			// Wait for ramp milliseconds to see the dimming effect.
			delay(ramp);

			if (fadeValue == 0) break;
		}

		// Update old value.
		OldValueL[channel] = value;
	}
}

/** @brief Converts ADC value to Volts.
 *  @param uint8_t adcValue, ADC value.
 *  @return Void.
 */
float to_volts(uint16_t adcValue)
{
	return (float)(((adcValue) * (PS_VOLTAGE / 1023)) / 1000);
}

/** @brief Converts ADC value to mili Volts.
 *  @param uint8_t adcValue, ADC value.
 *  @return Void.
 */
float to_mili_volts(uint16_t adcValue)
{
	return (float)((adcValue) * (PS_VOLTAGE / 1023));
}
