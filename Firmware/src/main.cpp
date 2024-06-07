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
		Serial.println("OK done");
		Serial.println("\n");
	}

	Serial.println("waiting");
	delay(1000);
}