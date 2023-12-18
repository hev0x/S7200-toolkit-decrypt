/*
 * EEPROM_I2C.h
 *
 *  Created on: Oct 1, 2023
 *      Author: hev0x
 *
 * WRITE the data to the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @data is the pointer to the data to write in bytes
 * @size is the size of the data
 *
 * READ the data from the EEPROM
 * @page is the number of the start page. Range from 0 to PAGE_NUM-1
 * @offset is the start byte offset in the page. Range from 0 to PAGE_SIZE-1
 * @data is the pointer to the data to write in bytes
 * @size is the size of the data
 *
 * Erase a page in the EEPROM Memory
 * @page is the number of page to erase
 * In order to erase multiple pages, just use this function in the for loop
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"
#include "usb_device.h"


void writeDataEEPROM(uint16_t targetPage, uint16_t offset, uint8_t *data, uint16_t dataSize);
void readDataEEPROM(uint16_t targetPage, uint16_t offset, uint8_t *data, uint16_t dataSize);
void eraseEEPROMPage(uint16_t targetPage);

//void EEPROM_Write_NUM(uint16_t page, uint16_t offset, float  fdata);
//float EEPROM_Read_NUM(uint16_t page, uint16_t offset);

void EEPROM_dumpMemory();

