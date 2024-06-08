#include <Arduino.h>

#include <Wire.h>

#include "atmometer_sensor_driver.h"

#define I2C_SDA 0
#define I2C_SCL 1

#define SLAVE_ADDRESS 12

void setup()
{
	Wire.begin(I2C_SDA, I2C_SCL);
	Serial.begin(9600);
	Serial.println("\nI2C Scanner");

	ATM_Sensor::set_sensor_config_sequential_read(Wire, SLAVE_ADDRESS, true);
	delay(1000);
}
int toto = 0;
void loop()
{
	Serial.print("Start while(true):  ");
	Serial.println(toto);
	toto++;
	byte error;
	Wire.beginTransmission(SLAVE_ADDRESS);
	error = Wire.endTransmission();
	if (error == 0)
	{
		Serial.println("I2C device found");
		Serial.print("sensor type: ");
		Serial.println(ATM_Sensor::get_sensor_type(Wire, SLAVE_ADDRESS));
		Serial.print("sensor humidity: ");
		uint64_t value = ATM_Sensor::get_sensor_value_1(Wire, SLAVE_ADDRESS);
		float real_value = (float)value / 1000;
		Serial.println(real_value);

		Serial.print("sensor temperature: ");
		value = ATM_Sensor::get_sensor_value_2(Wire, SLAVE_ADDRESS);
		real_value = (float)value / 1000;
		Serial.println(real_value);

		Serial.println("\n");
	}

	Serial.println("waiting");
	delay(2000);
}