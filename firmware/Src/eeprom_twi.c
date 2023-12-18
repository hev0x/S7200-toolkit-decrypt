/*
 * EEPROM_I2C.c
 *  Created on: Oct 1, 2023
 *  Author: hev0x
 *
 *  Tested memories:
 *  - ST4256BWP
 *  - ATCxx??
 *  - M24xx-BW/BR
 */

#include "eeprom_twi.h"
#include "math.h"
#include "string.h"

extern I2C_HandleTypeDef hi2c1;

#define EEPROM_I2C &hi2c1
#define EEPROM_ADDR 0xA0

// Define the memory array parameters
#define PAGE_SIZE 64     // Bytes
#define PAGE_NUM  512    // Pages

uint8_t eepromBufferFloat[4];
uint8_t dumpBuffer[100];


/* Função para calcular a quantidade de bytes a serem gravados em uma página */
uint16_t calculateBytesToWrite(uint16_t dataSize, uint16_t currentOffset) {
    if ((dataSize + currentOffset) < PAGE_SIZE) {
        return dataSize;
    } else {
        return PAGE_SIZE - currentOffset;
    }
}


/* Function to ......................... */
void fillBuffer(uint8_t *buffer, uint16_t len, uint8_t fillByte){

	for(uint16_t i=0; i < len; i++){
		buffer[i] = fillByte;
	}
}


/* Function to ......................... */
void writeDataEEPROM(uint16_t targetPage, uint16_t offset, uint8_t *data, uint16_t dataSize) {
    int pointerAddrPosition = log(PAGE_SIZE) / log(2);

    uint16_t startPage = targetPage;
    uint16_t endPage = targetPage + ((dataSize + offset) / PAGE_SIZE);
    uint16_t totalPageCount = (endPage - startPage) + 1;
    uint16_t currentPosition = 0;

    for (int i = 0; i < totalPageCount; i++) {
        uint16_t memAddress = startPage << pointerAddrPosition | offset;
        uint16_t bytesRemaining = calculateBytesToWrite(dataSize, offset);

        HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, memAddress, 2, &data[currentPosition], bytesRemaining, 1000);

        startPage += 1;
        offset = 0;
        dataSize -= bytesRemaining;
        currentPosition += bytesRemaining;

        HAL_Delay(5);
    }
}

/* Function to ......................... */
void readDataEEPROM(uint16_t targetPage, uint16_t offset, uint8_t *data, uint16_t dataSize) {
    int pointerAddrPosition = log(PAGE_SIZE) / log(2);

    uint16_t startPage = targetPage;
    uint16_t endPage = targetPage + ((dataSize + offset) / PAGE_SIZE);
    uint16_t totalPageCount = (endPage - startPage) + 1;
    uint16_t currentPosition = 0;

    for (int i = 0; i < totalPageCount; i++) {
        uint16_t memAddress = startPage << pointerAddrPosition | offset;
        uint16_t bytesRemaining = calculateBytesToWrite(dataSize, offset);
        HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, memAddress, 2, &data[currentPosition], bytesRemaining, 1000);
        startPage += 1;
        offset = 0;
        dataSize -= bytesRemaining;
        currentPosition += bytesRemaining;
    }
}

/* Function to ......................... */
void eraseEEPROMPage(uint16_t targetPage) {
    int pointerAddrPosition = log(PAGE_SIZE) / log(2);
    uint16_t memAddress = targetPage << pointerAddrPosition;

    uint8_t eraseData[PAGE_SIZE];
    memset(eraseData, 0xFF, PAGE_SIZE);

    HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, memAddress, 2, eraseData, PAGE_SIZE, 1000);

    HAL_Delay(5);
}


void EEPROM_dumpMemory()
{
    for (int page = 0; page < PAGE_NUM; page++) {
        readDataEEPROM(page, 63, dumpBuffer, PAGE_SIZE);
        vcp_send(dumpBuffer, strlen(dumpBuffer));
        HAL_Delay(5);
    }
}

/*

void float2Bytes(uint8_t * ftoa_bufferFloat,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bufferFloat[i] = thing.bytes[i];
    }

}


float Bytes2float(uint8_t * ftoa_bufferFloat)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bufferFloat[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}


/*Write the Float/Integer values to the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @data is the float/integer value that you want to write
*/

/*
void EEPROM_Write_NUM (uint16_t page, uint16_t offset, float data)
{

	float2Bytes(bufferFloat, data);

	EEPROM_Write(page, offset, bufferFloat, 4);
}

/* Reads the single Float/Integer values from the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @returns the float/integer value
 */
/*
float EEPROM_Read_NUM (uint16_t page, uint16_t offset)
{
	uint8_t buffer[4];

	EEPROM_Read(page, offset, buffer, 4);

	return (Bytes2float(buffer));
}

*/








