#pragma once

// Default placeholder for unassigned/inactive pins
#define NO_PIN -1

// ==========================================
// ACTUATOR PINS
// ==========================================
// Fans
#define PIN_FAN_POWER     16
#define PIN_FAN_BOOST     17

// Heater
#define PIN_HEATER        NO_PIN
#define PIN_HEATER_FAN    NO_PIN

// Humidifier
#define PIN_HUMIDIFIER    NO_PIN

// Water Pump
#define PIN_WATERPUMP     18

// LED Group
#define PIN_LED_POWER     32
#define PIN_LED_PUMP      19
#define PIN_LED_FANS      5
#define PIN_LED_LAMP1     13
#define CHANNEL_LED_LAMP1 0

// ==========================================
// SENSOR PINS
// ==========================================
// 1-Wire Temperature Sensor (DS18B20)
#define PIN_ONE_WIRE_BUS  33

// I2C Pins (Default ESP32 pins are SDA=21, SCL=22)
// Devices: ADS1115 (0x48), BH1750 (0x23), SHT40 (0x44), SGP30