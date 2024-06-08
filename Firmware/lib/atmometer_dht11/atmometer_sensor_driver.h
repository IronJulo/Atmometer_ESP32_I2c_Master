#ifndef ATMOMETER_SENSOR_DRIVER_H
#define ATMOMETER_SENSOR_DRIVER_H

#include <Wire.h>

// 1 if set else 0
#define GET_BIT_VALUE_AT(variable, position) \
    (((variable) & (1 << (position))) >> (position))

#define SET_BIT_VALUE_AT(variable, position, value) \
    ((variable) |= ((value) << (position)))

enum Register
{
    // Read Only
    SENSOR_TYPE_MSB,
    SENSOR_TYPE_LSB,
    SENSOR_ID_MSB1,
    SENSOR_ID_MSB2,
    SENSOR_ID_LSB1,
    SENSOR_ID_LSB2,
    SENSOR_VALUE_1_MSB1,
    SENSOR_VALUE_1_MSB2,
    SENSOR_VALUE_1_LSB1,
    SENSOR_VALUE_1_LSB2,
    SENSOR_VALUE_2_MSB1,
    SENSOR_VALUE_2_MSB2,
    SENSOR_VALUE_2_LSB1,
    SENSOR_VALUE_2_LSB2,
    SENSOR_ERROR_MSB,
    SENSOR_ERROR_LSB,

    // Read/Write
    SENSOR_CONFIG,
    SENSOR_UPDATE_PERIOD,
    SENSOR_READ_ADDRESS,

    REGISTER_ENUM_END,
};

enum ConfigBitField
{
    CBF_READ_IN_PROGRESS,
    CBF_IDLE,
    CBF_SEQUENTIAL_READ,
};

class ATM_Sensor
{
public:
    ATM_Sensor() = default;
    ~ATM_Sensor() = default;

    static uint8_t set_sensor_config(TwoWire &i2cHandle, uint8_t address, uint8_t value);
    static uint8_t set_sensor_config_read_in_progress(TwoWire &i2cHandle, uint8_t address, bool value);
    static uint8_t set_sensor_config_idle(TwoWire &i2cHandle, uint8_t address, bool value);
    static uint8_t set_sensor_config_sequential_read(TwoWire &i2cHandle, uint8_t address, bool value);

    static uint8_t set_sensor_update_period(TwoWire &i2cHandle, uint8_t address, uint8_t value);
    static uint8_t set_sensor_read_address(TwoWire &i2cHandle, uint8_t address, uint8_t value);

    static uint16_t get_sensor_type(TwoWire &i2cHandle, uint8_t address);
    static uint64_t get_sensor_id(TwoWire &i2cHandle, uint8_t address);
    static uint64_t get_sensor_value_1(TwoWire &i2cHandle, uint8_t address);
    static uint64_t get_sensor_value_2(TwoWire &i2cHandle, uint8_t address);
    static uint16_t get_sensor_error(TwoWire &i2cHandle, uint8_t address);
    static uint8_t get_sensor_config(TwoWire &i2cHandle, uint8_t address);
    static uint8_t get_sensor_update_period(TwoWire &i2cHandle, uint8_t address);
};

uint8_t simple_write(TwoWire &i2cHandle, uint8_t i2cAddress, uint8_t registerAddress, uint8_t value)
{
    uint8_t error;

    Wire.beginTransmission(i2cAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    error = Wire.endTransmission();
    return error;
}

uint8_t simple_read(TwoWire &i2cHandle, uint8_t i2cAddress, uint8_t registerAddress, uint8_t *value)
{
    uint8_t error = simple_write(i2cHandle, i2cAddress, Register::SENSOR_READ_ADDRESS, registerAddress);
    // TODO handle errors
    Wire.requestFrom(i2cAddress, 1);
    uint8_t realValue = Wire.read();

    *value = realValue;

    return error;
}

uint8_t simple_read(TwoWire &i2cHandle, uint8_t i2cAddress, uint8_t registerAddress, uint64_t *value)
{
    uint8_t error = 0;
    error = simple_write(i2cHandle, i2cAddress, Register::SENSOR_READ_ADDRESS, registerAddress);
    // TODO handle errors

    *value = 0;
    uint8_t read = 0;

    Wire.requestFrom(i2cAddress, 1);
    read = Wire.read();
    *value += (read << 24);

    Wire.requestFrom(i2cAddress, 1);
    read = Wire.read();
    *value += (read << 16);

    Wire.requestFrom(i2cAddress, 1);
    read = Wire.read();
    *value += (read << 8);

    Wire.requestFrom(i2cAddress, 1);
    read = Wire.read();
    *value += (read << 0);

    return error;
}

uint8_t ATM_Sensor::set_sensor_config(TwoWire &i2cHandle, uint8_t address, uint8_t value)
{
    return simple_write(i2cHandle, address, Register::SENSOR_CONFIG, value);
}

uint8_t ATM_Sensor::set_sensor_config_read_in_progress(TwoWire &i2cHandle, uint8_t address, bool value)
{
    return 0;
}

uint8_t ATM_Sensor::set_sensor_config_idle(TwoWire &i2cHandle, uint8_t address, bool value)
{
    return 0;
}

uint8_t ATM_Sensor::set_sensor_config_sequential_read(TwoWire &i2cHandle, uint8_t address, bool value)
{
    uint8_t config_value = 0;
    uint8_t error = simple_read(i2cHandle, address, Register::SENSOR_CONFIG, &config_value);

    if (value)
    {
        SET_BIT_VALUE_AT(config_value, ConfigBitField::CBF_SEQUENTIAL_READ, 1);
        error = simple_write(i2cHandle, address, Register::SENSOR_CONFIG, config_value);
    }
    else
    {
        SET_BIT_VALUE_AT(config_value, ConfigBitField::CBF_SEQUENTIAL_READ, 0);
        error = simple_write(i2cHandle, address, Register::SENSOR_CONFIG, config_value);
    }
    return error;
}

uint8_t ATM_Sensor::set_sensor_update_period(TwoWire &i2cHandle, uint8_t address, uint8_t value)
{
    return 0;
}

uint8_t ATM_Sensor::set_sensor_read_address(TwoWire &i2cHandle, uint8_t address, uint8_t value)
{
    return 0;
}

uint16_t ATM_Sensor::get_sensor_type(TwoWire &i2cHandle, uint8_t address)
{
    uint8_t error;
    Wire.beginTransmission(address);
    Wire.write(Register::SENSOR_READ_ADDRESS);
    Wire.write(Register::SENSOR_TYPE_MSB);
    error = Wire.endTransmission();
    // TODO handle errors

    Wire.requestFrom(address, 1);
    uint16_t value = Wire.read();
    value <<= 8;

    Wire.requestFrom(address, 1);
    value |= Wire.read();

    return value;
}

uint64_t ATM_Sensor::get_sensor_id(TwoWire &i2cHandle, uint8_t address)
{
    return 0;
}

uint64_t ATM_Sensor::get_sensor_value_1(TwoWire &i2cHandle, uint8_t address)
{
    uint8_t error;
    uint64_t value = 0;
    error = simple_read(i2cHandle, address, Register::SENSOR_VALUE_1_MSB1, &value);

    return value;
}

uint64_t ATM_Sensor::get_sensor_value_2(TwoWire &i2cHandle, uint8_t address)
{
    uint8_t error;
    uint64_t value = 0;
    error = simple_read(i2cHandle, address, Register::SENSOR_VALUE_2_MSB1, &value);

    return value;
}

uint16_t ATM_Sensor::get_sensor_error(TwoWire &i2cHandle, uint8_t address)
{
    return 0;
}

uint8_t ATM_Sensor::get_sensor_config(TwoWire &i2cHandle, uint8_t address)
{
    return 0;
}

uint8_t ATM_Sensor::get_sensor_update_period(TwoWire &i2cHandle, uint8_t address)
{
    return 0;
}

#endif /* ATMOMETER_SENSOR_DRIVER_H */