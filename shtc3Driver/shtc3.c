
/**************************************************************************//**
* @file      shtc3.c
* @brief     Template for ESE516 with Doxygen-style comments
* @author    Robert Wang
* @date      2022-04-28

******************************************************************************/


/******************************************************************************
* Includes
******************************************************************************/
#include "shtc3.h"
#include "i2c_master.h"
#include "i2c_master_interrupt.h"
#include "SerialConsole.h"

/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/
struct i2c_master_module i2cSensorBusInstance;
static const uint16_t SHT3_wakeup_cmd = SHT3_WAKEUP_COMMAND;
static const uint16_t SHT3_sleep_cmd = SHT3_SLEEP_COMMAND;

static const uint16_t SHT3_measure_cmd = SHT3_NORMAL_MODE_MEASURE_NM;

I2C_Data i2cSHT3;

uint16_t shtBuf[2];

/******************************************************************************
* Forward Declarations
******************************************************************************/
static int shtc3Read(uint8_t *command, uint16_t length);
static int shtc3Write(uint8_t *command, uint16_t length);
static void CalculateTH();

/******************************************************************************
* Callback Functions
******************************************************************************/


/**************************************************************************//**
* @fn		int SHTC3_Measure(uint16_t *temperature, uint16_t *humidity)
* @brief	Takes the measurement of the shtc3 sensor
* @details 	Wakes up the sensor using the wakeup command, reads, and puts the sensor 
			to sleep. Then performs the calculations to get the temp and relative humidity
                				
* @param[in]	
* @param[out]	
* @return		returns if the sensor measured without error
* @note         
*****************************************************************************/

int SHTC3_Measure(uint16_t *temperature, uint16_t *humidity)
{
	
	//variable declaration
	int error;
	
	//send the wakeup command
	error = shtc3Write(wakeWriteBuffer, WRITE_BUFFER_LEN);
	if(error != ERROR_NONE)
	{
		SerialConsoleWriteString("Error sending wakeup command to SHTC3 sensor!\r\n");
	}
	
	//send the measure command
	error = shtc3Read(measureWriteBuffer, WRITE_BUFFER_LEN);
	if(error != ERROR_NONE)
	{
		SerialConsoleWriteString("Error sending measurement command to sensor!\r\n");
	}
	
	//send the sleep command
	error = shtc3Write(sleepWriteBuffer, WRITE_BUFFER_LEN);
	if(error != ERROR_NONE)
	{
		SerialConsoleWriteString("Error sending sleep command to sensor!\r\n");
	}
	
	CalculateTH();
	temperature = shtBuf[0];
	humidity = shtBuf[1];
	return error;
	
}

/**************************************************************************//**
* @fn		shtc3Read(uint8_t *command, uint16_t length)
* @brief	Sends a command to I2C to read data
* @details 	Sends the shtc3 Read data command to the sensor via I2C
                				
* @param[in]	uint8_t *command - register value of the comand
				uint16_t length - length of the command
* @param[out]	
* @return		returns if the sensor was read without error
* @note         
*****************************************************************************/

static int shtc3Read(uint8_t *command, uint16_t length)
{
	i2cSHT3.address = SHT3_LOW_ADDRESS;
	i2cSHT3.msgOut = command;
	i2cSHT3.lenOut = length;
	i2cSHT3.msgIn = buffer;
	i2cSHT3.lenIn = sizeof(uint16_t) * 6;
	return I2cReadDataWait(&i2cSHT3, 0, 100);
}


/**************************************************************************//**
* @fn		shtc3Write(uint8_t *command, uint16_t length)
* @brief	Sends a command to I2C to write data
* @details 	Sends the shtc3 Write data command to the sensor via I2C
                				
* @param[in]	uint8_t *command - register value of the comand
				uint16_t length - length of the command
* @param[out]	
* @return		returns if the sensor was written without error
* @note         
*****************************************************************************/
static int shtc3Write(uint8_t *command, uint16_t length)
{
	i2cSHT3.address = SHT3_LOW_ADDRESS;
	i2cSHT3.msgOut = command;
	i2cSHT3.lenOut = length;
	return I2cWriteDataWait(&i2cSHT3, 100);
}

/**************************************************************************//**
* @fn		CalculateTH()
* @brief	Helper function used to calculate temperature and humidity
* @details 	Through the formulas found on the sensor data sheet, we calculate the 
			temperature and relative humidity based on the data received from the sensor.
			bytes 0 and 1 are temperature, MSB and LSB respectively
			bytes 3 and 4 are humidity, MSB and LSB respectively
			So we need to shift the MSB by 8 and logical or with the LSB byte
			Then we use the formulas
			
                				
* @param[in]	
* @param[out]	
* @return		void
* @note         
*****************************************************************************/
static void CalculateTH()
{
	//Temperature is first read, then humidity
	shtBuf[0] = (buffer[0] << 8) | buffer[1];
	shtBuf[1] = (buffer[3] << 8) | buffer[4];
	
	shtBuf[0] = -45 + 175 * shtBuf[0]/65536;
	shtBuf[1] = 100 * shtBuf[1]/65536;
	
}