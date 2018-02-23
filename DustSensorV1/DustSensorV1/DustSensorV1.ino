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

#pragma region Headers

/** \brief Configuration of the device. */
#include "ApplicationConfiguration.h"

/** \brief GPIO Utilities library. */
#include "GPIOUtilities.h"

/** \brief The Things Network library for RN2483A. */
#include <TheThingsNetwork.h>

/** \brief Software serial library. */
#include <SoftwareSerial.h>

#pragma endregion

#pragma region Constants

// TODO: Always remove keys before upload the code.

/** \brief Set device address. */
const char *devAddr = "XXXXXXXX";

/** \brief Set network session key. */
const char *nwkSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

/** \brief Set application session key. */
const char *appSKey = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

#pragma endregion

#pragma region Varibales

/** \brief Software serial for the LoRa module. */
SoftwareSerial LoRaSerial_g(PIN_LORA_RX, PIN_LORA_TX);

/** \brief TTN abstraction instance. */
TheThingsNetwork TTN_g(LoRaSerial_g, DEBUG_SERIAL, FREQ_PLAN);

/** \brief TTN abstraction */
double MesuredVoltage_g = 0;

/** \brief TTN abstraction */
double CalculatedVoltage_g = 0;

/** \brief TTN abstraction */
double DustDensity_g = 0;

/** \brief Output serial buffer. */
char TTNPayload_g[256];

#pragma endregion

void setup()
{
	// Setup the LED power pin.
	pinMode(PIN_DS_LED_POWER, OUTPUT);

	// Setup the fan power pin.
	pinMode(PIN_DS_FAN_POWER, OUTPUT);

	// Setup the LED power pin.
	pinMode(PIN_STATUS_LED, OUTPUT);

	// Setup the user AP button pin.
	pinMode(PIN_AP_BUTTON, INPUT);

	// Setup debugging serial port.
	DEBUG_SERIAL.begin(BAUDRATE_DEBUG_SERIAL);

	// Wait a maximum of 10s for Serial Monitor
	while (!DEBUG_SERIAL && millis() < 10000)
	{
		;
	}

	// Setup LoRaWAN module serial port.
	LoRaSerial_g.begin(BAUDRATE_LORA);

	// Reset the module.
	TTN_g.reset();
	DEBUG_SERIAL.println("-- RESET");

	// Personalize the module.
	TTN_g.personalize(devAddr, nwkSKey, appSKey);
	DEBUG_SERIAL.println("-- PERSONALIZE");

	// Show the status of the device.
	DEBUG_SERIAL.println("-- STATUS");
	TTN_g.showStatus();
}

void loop()
{
	static unsigned long CurrentMillisL = 0;
	static unsigned long PreviousMillisL = 0;

	// Update the time.
	CurrentMillisL = millis();

	// Execute when the time is come.
	if (CurrentMillisL - PreviousMillisL >= TASK_RATE)
	{
		// save the last time you blinked the LED
		PreviousMillisL = CurrentMillisL;

		// Turn on the status LED.
		digitalWrite(PIN_STATUS_LED, HIGH);

		// Turn on the fan power.
		digitalWrite(PIN_DS_FAN_POWER, HIGH);

		// Turn on the LED power.
		digitalWrite(PIN_DS_LED_POWER, LOW);

		// Wait
		delayMicroseconds(SAMPLING_TIME);

		// Read the sensor data. (dust value)
		MesuredVoltage_g = analogRead(PIN_DS_MEASURE);

		// Wait
		delayMicroseconds(DELTA_TIME);

		// Turn off the LED power.
		digitalWrite(PIN_DS_LED_POWER, HIGH);

		// Turn off the fan power.
		digitalWrite(PIN_DS_FAN_POWER, LOW);

		// 0 - 5 mapped to 0 - 1023 integer values recover voltage.
		CalculatedVoltage_g = MesuredVoltage_g * (PS_VOLTAGE / 1023);

		// Linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
		// Chris Nafis (c) 2012
		DustDensity_g = 0.17 * CalculatedVoltage_g - 0.1;

		// Debug
		DEBUG_SERIAL.print(" - Raw Signal Value (0-1023): ");
		DEBUG_SERIAL.println(MesuredVoltage_g);
		DEBUG_SERIAL.print(" - Voltage: ");
		DEBUG_SERIAL.println(CalculatedVoltage_g);
		DEBUG_SERIAL.print(" - Dust Density: ");
		DEBUG_SERIAL.println(DustDensity_g);

		String sDustDensityL = String(DustDensity_g);
		String sMesuredVoltageL = String(MesuredVoltage_g);
		String sCalculatedVoltageL = String(CalculatedVoltage_g);

		String TTNPayloadL = "D:" + sDustDensityL + ";MV:" + sMesuredVoltageL + ";CV:" + sCalculatedVoltageL + "\r\n";
				
		// Prepare payload of 1 byte to indicate LED status
		byte payload[TTNPayloadL.length()];
		TTNPayloadL.getBytes(payload, TTNPayloadL.length());

		// Send it off
		ttn_response_t response = TTN_g.sendBytes(payload, sizeof(payload));
		
		// Print the payload.
		DEBUG_SERIAL.println("");
		DEBUG_SERIAL.print("Sent: ");
		DEBUG_SERIAL.println(TTNPayloadL);
		DEBUG_SERIAL.println("");

		if (response == ttn_response_t::TTN_ERROR_SEND_COMMAND_FAILED)
		{
			DEBUG_SERIAL.println("Send failed.");
		}
		else if (response == ttn_response_t::TTN_ERROR_UNEXPECTED_RESPONSE)
		{
			DEBUG_SERIAL.println("Unexpected response.");
		}
		else if (response == ttn_response_t::TTN_SUCCESSFUL_TRANSMISSION)
		{
			DEBUG_SERIAL.println("Successful transmission.");
		}
		else if (response == ttn_response_t::TTN_SUCCESSFUL_RECEIVE)
		{
			DEBUG_SERIAL.println("Successful receive.");
		}

		// Turn off the status LED.
		digitalWrite(PIN_STATUS_LED, LOW);

	}
}
