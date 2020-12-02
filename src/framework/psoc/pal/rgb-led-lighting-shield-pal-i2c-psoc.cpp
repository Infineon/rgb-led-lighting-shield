/** 
 * @file        rgb-led-lighting-shield-pal-i2c-psoc.cpp
 * @brief       RGB LED LIGHTING SHIELD I2C Platform Abstraction Layer PSoC
 * @date        November 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "rgb-led-lighting-shield-pal-i2c-psoc.hpp"

#if (RGB_LED_LIGHTING_SHIELD_FRAMEWORK == RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6)

/**
 * @brief Constructor of the IC2-Class for PSoC
 *
 * @details This is the default constructor of the I2C-Class for PSoC.
 *
 */
I2CPALPSoC::I2CPALPSoC()
{

}

/**
 * @brief Destructor of the PSoC I2C-Class
 *
 * @details This is the destructor of the I2C-Class for PSoC.
 *
 */
I2CPALPSoC::~I2CPALPSoC()
{

}

/**
 * @brief Initialization of the I2C bus
 *
 * @details This function is initializing the I2C bus. It takes care
 * that all necessary values are set, so that the I2C bus can work properly.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::init()
{
	cy_rslt_t result;
	Error_t err = OK;

	_I2C_cfg.is_slave = false;
	_I2C_cfg.address = 0;
	_I2C_cfg.frequencyhal_hz = _i2cFreq;

	result = cyhal_i2c_init(&_I2C, P9_1, P9_0, NULL);
	if(result != CY_RSLT_SUCCESS)
	{
		err = INVALID_CONFIG_ERROR;
		return err;
	}
	result = cyhal_i2c_configure(&_I2C, &_I2C_cfg);
	if(result != CY_RSLT_SUCCESS)
	{
		err = INVALID_CONFIG_ERROR;
		return err;
	}

	return err;
}

/**
 * @brief Deinitialize the I2C bus
 *
 * @details This function is deinitializing the I2C bus. The bus
 * has to be initialized again for further usage.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::deinit()
{
	Error_t err = OK;

	cyhal_i2c_free(&_I2C); // Check this function if it is doing what I expect

	return err;
}

/**
 * @brief Set the clock frequency of the I2C bus
 *
 * @details This function is setting the clock frequency of the I2C bus, if the
 * default value(100 kHz) should be changed.
 *
 * @param[in] clockHz       Clock frequency in Hz
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::setClockFreq(uint32_t clockHz)
{
	cy_rslt_t result;
	Error_t err = OK;

	_I2C_cfg.frequencyhal_hz = clockHz;
	result = cyhal_i2c_configure(&_I2C, &_I2C_cfg);
	if(result != CY_RSLT_SUCCESS)
	{
		err = INVALID_CONFIG_ERROR;
		return err;
	}

	return err;
}

/**
 * @brief Read several bytes from the slave via I2C
 *
 * @details This functions is used to read from a slave on the I2C bus. It needs the
 * address of the slave, the required command (see rgb-led-lighting-shield-reg.hpp), a pointer to the
 * data buffer, where the received data is stored and the length of the data buffer that should be sent.
 *
 * @param[in] addr          Address of the slave device.
 * @param[in] command       Command to read out the board.
 * @param[in,out] data      Pointer to a buffer where the received data is stored.
 * @param[in] length        Number of bytes to read.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::read(uint16_t addr, uint8_t command, uint16_t *data, uint16_t length)
{
	cy_rslt_t result;
	Error_t err = OK;

	int counter = 0;
	uint8_t firstAddressbyte = addr >> 8;
	uint8_t tempBuffer[2];
	uint8_t receivedData[length];
	
	tempBuffer[0] = addr & 0xFF;
	tempBuffer[1] = command;
	result = cyhal_i2c_master_write(&_I2C, firstAddressbyte, tempBuffer, 2, 0, 0);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	result = cyhal_i2c_master_read(&_I2C, firstAddressbyte, receivedData, length, 0, 1);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	for(int y = 0; y < length/2; y++){
		data[y] = (receivedData[counter+1] << 8) | receivedData[counter];
		counter += 2;
	}

	return err;
}

/**
 * @brief Read two byte (an unsigned int16) from the I2C slave
 *
 * @details This function is used to read two byte from the I2C slave. It requires the
 * address of the slave, the command of what should be read (see rgb-led-lighting-shield-reg.hpp)
 * and a pointer to an uint16 variable where the received data is stored.
 *
 * @param[in] addr          Address of the slave device.
 * @param[in] command       Command to read out the board.
 * @param[in, out] data     Pointer to an uint16 variable to store the received data.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::read(uint16_t addr, uint8_t command, uint16_t *data)
{
	cy_rslt_t result;
	Error_t err = OK;

	uint8_t firstAddressbyte = addr >> 8;
	uint8_t tempBuffer[2];
	uint8_t receivedData[3];

	tempBuffer[0] = addr & 0xFF;
	tempBuffer[1] = command;
	result = cyhal_i2c_master_write(&_I2C, firstAddressbyte, tempBuffer, 2, 0, 0);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	result = cyhal_i2c_master_read(&_I2C, firstAddressbyte, receivedData, 2, 0, 1);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	*data = (receivedData[1] << 8) | receivedData[0];

	return err;
}

/**
 * @brief Write several bytes to the I2C slave
 *
 * @details This functions is used to write several bytes to an I2C slave.
 * It needs the address of the I2C slave, the write command (see rgb-led-lighting-shield-reg.hpp),
 * the pointer to data buffer which should be sent and the length of the data buffer.
 *
 * @param[in] addr      Address of the I2C slave device.
 * @param[in] command   Command to write to the board.
 * @param[in] data      Pointer to the data buffer that should be written to the slave.
 * @param[in] length    Number of bytes that should be written.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::write(uint16_t addr, uint8_t command, uint16_t *data, uint16_t length)
{
	cy_rslt_t result;
	Error_t err = OK;

	uint8_t tempBuffer[(length) + 2];
	uint8_t firstAddressByte = addr >> 8;

	convertData(command, data, length, tempBuffer);
	result = cyhal_i2c_master_write(&_I2C, firstAddressByte, tempBuffer, (length + 2), 0, true);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	return err;
}

/**
 * @brief Write two bytes to the I2C slave
 *
 * @details This function is used to write two bytes (an unsigned int16) to the slave device.
 * It needs the address of the slave device, the write command (see rgb-led-lighting-shield-reg.hpp) and
 * the data that should be sent.
 *
 * @param[in] addr      Address of the I2C slave device.
 * @param[in] command   Command to write to the board.
 * @param[in] data      Data which should be written to the slave.
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::write(uint16_t addr, uint8_t command, uint16_t data)
{
	cy_rslt_t result;
	Error_t err = OK;

	uint8_t dataIn[4];
	uint8_t firstAddressByte = addr >> 8;

	dataIn[0] = addr & 0xFF;
	dataIn[1] = command;
	dataIn[2] = data >> 8;
	dataIn[3] = data & 0xFF;

	result = cyhal_i2c_master_write(&_I2C, firstAddressByte, dataIn, 4, 0, true);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	return err;
}

/**
 * @brief Write to the slave via DMX
 *
 * @details This function is used to write to the slave device via the DMX-Standard.
 * It needs the address of the slave and the command (see rgb-led-lighting-shield-reg.hpp)
 * of what should be written to the slave.
 *
 * @param[in] addr      Address of the slave device
 * @param[in] command   Write command
 *
 * @return Error_t
 */
Error_t I2CPALPSoC::writeDMX(uint16_t addr, uint8_t command)
{
	cy_rslt_t result;
	Error_t err = OK;

	uint8_t firstAddressByte = addr >> 8;
	uint8_t dataIn[2];

	dataIn[0] = addr & 0xFF;
	dataIn[1] = command;

	result = cyhal_i2c_master_write(&_I2C, firstAddressByte, dataIn, 2, 0, true);
	if(result != CY_RSLT_SUCCESS)
	{
		err  = COMM_ERROR;
		return err;
	}

	return err;
}

/**
 * @brief Convert unsigned integer 16 to 8
 *
 * @details This function is used to convert the uint16_t buffer of I2C write function into
 * a uint8_t buffer to use it with the low level drivers of the PSoC (they expecting a uint8_t value).
 *
 * @param[in] 	command		Value of the command the is also given to the write function
 * @param[in] 	buffer		Pointer to a buffer with the values that should be converted
 * @param[in] 	length		Number of bytes that should be converted
 * @param[out] 	result		Pointer to the result buffer that is received after the conversion
 *
 */
void I2CPALPSoC::convertData(uint8_t command, uint16 *buffer, uint16_t length, uint8_t *result)
{
	uint8_t tempBuffer[length];
	static int y = 0;

    for(uint8_t i = 0; i< sizeof(tempBuffer); i = i + 2)
    {
        tempBuffer[i] = buffer[y] >> 8;
        tempBuffer[i+1] = buffer[y] & 0xFF;
        y++;
    }
    y = 0;
    result[0] = SECONDADDRESSBYTE;
    result[1] = command;
    memcpy(result+2, tempBuffer, length);
}

#endif /** RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6 **/
