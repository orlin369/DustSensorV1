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

#ifndef _APPLICATIONCONFIGURATION_h
#define _APPLICATIONCONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/** \brief Debugging serial port. */
#define DEBUG_SERIAL Serial

/** \brief Debugging serial port baud rate. */
#define BAUDRATE_DEBUG_SERIAL 115200


/** \brief LoRaWAN module RX pin. */
#define PIN_LORA_RX 2

/** \brief LoRaWAN module TX pin. */
#define PIN_LORA_TX 3

/** \brief LoRaWAN module baud rate. */
#define BAUDRATE_LORA 57600

/** \brief LoRaWAN module frequency plan. It can be: TTN_FP_EU868 or TTN_FP_US915, for EU TTN_FP_EU868 */
#define FREQ_PLAN TTN_FP_EU868


/** \brief Task execution rate in [ms]. */
#define TASK_RATE 20000

/** \brief Dust sensor measuring pin. */
#define PIN_DS_MEASURE A2

/** \brief Dust sensor status LED pin. */
#define PIN_STATUS_LED 13

/** \brief Dust sensor LED power pin. */
#define PIN_DS_LED_POWER 5

/** \brief Dust sensor fan power pin. */
#define PIN_DS_FAN_POWER 9

/** \brief Dust sensor sampling time. */
#define SAMPLING_TIME 280

/** \brief Dust sensor delta time. */
#define DELTA_TIME 40

/** \brief Arduino supply voltage. */
#define PS_VOLTAGE 5.0


#endif

