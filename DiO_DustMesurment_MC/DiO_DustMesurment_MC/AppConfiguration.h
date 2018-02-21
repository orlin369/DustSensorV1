// AppConfiguration.h

#ifndef _APPCONFIGURATION_h
#define _APPCONFIGURATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
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
#define PIN_DS_MEASURE A0

/** \brief Dust sensor LED power pin. */
#define PIN_DS_LED_POWER 5

/** \brief Dust sensor fan power pin. */
#define PIN_DS_FAN_POWER 6

/** \brief Dust sensor sampling time. */
#define SAMPLING_TIME 280

/** \brief Dust sensor delta time. */
#define DELTA_TIME 40

/** \brief Arduino supply voltage. */
#define PS_VOLTAGE 5.0

#endif

