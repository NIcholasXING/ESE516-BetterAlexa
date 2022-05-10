/**************************************************************************/ /**
 * @file      sht3.h
 * @brief     Driver for the temperature sensor. Uses no clock stretching mode.
 * @author    Eduardo Garcia
 * @date      2021-03-18

 ******************************************************************************/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "I2cDriver/I2cDriver.h"  // Include any libraries you need. In this case, we include the I2C driver

/******************************************************************************
 * Defines
 ******************************************************************************/
#define SHT3_LOW_ADDRESS 0x70  ///< I2C address

#define SHT3_SLEEP_COMMAND 0xB098  ///< Sleep command for the SHT3
#define SHT3_WAKEUP_COMMAND 0x3517  ///< Sleep command for the SHT3

#define SHT3_NORMAL_MODE_MEASURE_NM 0x7866  ///< Command to measure temperature first, then RH, in normal power mode, no clock streching
#define SHT3_NORMAL_MODE_MEASURE_LPM 0x609C  ///< Command to measure temperature first, then RH, in low power mode, no clock streching

#define WRITE_BUFFER_LEN 2
#define READ_BUFFER_LEN 6

static uint8_t wakeWriteBuffer[WRITE_BUFFER_LEN] = {0x35, 0x17};
static uint8_t measureWriteBuffer[WRITE_BUFFER_LEN] = {0x78, 0x66};
static uint8_t sleepWriteBuffer[WRITE_BUFFER_LEN] = {0xB0, 0x98};
uint8_t readBuffer[READ_BUFFER_LEN];

#define SHT3_OK 0  ///< Returns that the sensor had no error
#define SHT3_COMM_ERROR 1  ///< SHT3 Communication error

uint16_t buffer[6];
/******************************************************************************
 * Structures and Enumerations
 ******************************************************************************/

/******************************************************************************
 * Global Function Declaration
 ******************************************************************************/

int SHTC3_Measure(uint16_t *temperature, uint16_t *humidity);


#ifdef __cplusplus
}
#endif