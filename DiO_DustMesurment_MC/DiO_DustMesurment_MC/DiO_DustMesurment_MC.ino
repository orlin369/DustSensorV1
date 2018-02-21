#include "AppConfiguration.h"
#include <TheThingsNetwork.h>
#include <SoftwareSerial.h>

// TODO: Remove keys before upload to the repository.

#pragma region Constants

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


	// Setup debugging serial port.
	DEBUG_SERIAL.begin(BAUDRATE_DEBUG_SERIAL);

	// Wait a maximum of 10s for Serial Monitor
	while (!DEBUG_SERIAL && millis() < 10000)
	{
		;
	}

	// Null terminated string.
	//TTNPayload_g[256] = '\0';

	// Setup LoRaWAN module serial port.
	LoRaSerial_g.begin(BAUDRATE_LORA);

	//
	DEBUG_SERIAL.println("-- RESET");
	TTN_g.reset();

	//
	DEBUG_SERIAL.println("-- PERSONALIZE");
	TTN_g.personalize(devAddr, nwkSKey, appSKey);

	// 
	DEBUG_SERIAL.println("-- STATUS");
	TTN_g.showStatus();
}

void loop()
{
	static unsigned long CurrentMillisL = 0;
	static unsigned long PreviousMillisL = 0;

	// Update the time.
	CurrentMillisL = millis();

	
	if (CurrentMillisL - PreviousMillisL >= TASK_RATE)
	{
		// save the last time you blinked the LED
		PreviousMillisL = CurrentMillisL;

		// Debug
		DEBUG_SERIAL.println("-- LOOP");


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

		// linear equation taken from http://www.howmuchsnow.com/arduino/airquality/
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
		byte payload[1];
		payload[0] = 72; // H => 72
		
		// Send it off
		TTN_g.sendBytes(payload, sizeof(payload));
	}
}