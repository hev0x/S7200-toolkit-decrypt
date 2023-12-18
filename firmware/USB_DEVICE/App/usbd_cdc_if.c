/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : NEFASTOR.COM Simple UART API implementation based on v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  *
  * Author : Jean Roch / Nefastor.com
  *
  * This file is a MODIFIED file based on code generated by STM32CubeIDE.
  *
  * The STM32CubeIDE file on which this file is based came with NO license and is
  * therefore considered to be provided AS-IS. That code is copyright STMicroelectronics.
  *
  * The code added by Jean ROCH for the purpose of creating a practical UART API
  * is Copyright (c) 2022 Jean Roch.
  *
  * That code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
  *
  * This VCP library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
  * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License along with this library.
  * If not, see <https://www.gnu.org/licenses/>.
  *
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RX_BUFFER_MAX_WRITE_INDEX (APP_RX_DATA_SIZE - CDC_DATA_FS_MAX_PACKET_SIZE)

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
// Circular FIFO to store outgoing data until it can be sent over USB
VCP_FIFO vcp_tx_fifo;
// Circular FIFO to store incoming data from the host over USB
VCP_FIFO vcp_rx_fifo;
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  // Circular FIFO initializations :
  vcp_tx_fifo.data = UserTxBufferFS;  // Use the buffer generated by Cube
  vcp_tx_fifo.wr = 0;
  vcp_tx_fifo.rd = 0;
  vcp_tx_fifo.lb = 0;
  vcp_rx_fifo.data = UserRxBufferFS;  // Use the buffer generated by Cube
  vcp_rx_fifo.wr = 0;
  vcp_rx_fifo.rd = 0;
  vcp_rx_fifo.lb = 0;
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:

    break;

    case CDC_GET_LINE_CODING:

    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
  // Update the write index for the next incoming packet
  vcp_rx_fifo.wr += *Len;
  // Is the new value too close to the end of the FIFO ?
  if (vcp_rx_fifo.wr >= RX_BUFFER_MAX_WRITE_INDEX)
  {
	// Solution : wrap-around (and save wr as lb)
	vcp_rx_fifo.lb = vcp_rx_fifo.wr;
	vcp_rx_fifo.wr = 0;
  }
  // Tell the driver where to write the next incoming packet
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, vcp_rx_fifo.data + vcp_rx_fifo.wr);
  // Receive the next packet
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void vcp_init ()
{
  while (vcp_tx_fifo.data == 0);		// Wait until the CDC library calls CDC_Init_FS, by checking for a non-null buffer pointer
}

int vcp_send (uint8_t* buf, uint16_t len)
{
  // Step 1 : calculate the occupied space in the Tx FIFO
  int cap = vcp_tx_fifo.wr - vcp_tx_fifo.rd;   // occupied capacity
  if (cap < 0)    // FIFO contents wrap around
    cap += APP_TX_DATA_SIZE;
  cap = APP_TX_DATA_SIZE - cap;      // available capacity
  // Step 2 : compare with argument
  if (cap < len)
    return -1;   // Not enough room to copy "buf" into the FIFO => error
  // Step 3 : does buf fit in the tail ?
  int tail = APP_TX_DATA_SIZE - vcp_tx_fifo.wr;
  if (tail >= len)
  {
    // Copy buf into the tail of the FIFO
    memcpy (&vcp_tx_fifo.data[vcp_tx_fifo.wr], buf, len);
    // Update "wr" index
    vcp_tx_fifo.wr += len;
    // In case "len" == "tail", next write goes to the head
    if (vcp_tx_fifo.wr == APP_TX_DATA_SIZE)
      vcp_tx_fifo.wr = 0;
  }
  else
  {
    // Copy the head of "buf" to the tail of the FIFO
    memcpy (&vcp_tx_fifo.data[vcp_tx_fifo.wr], buf, tail);
    // Copy the tail of "buf" to the head of the FIFO :
    memcpy (vcp_tx_fifo.data, &buf[tail], len - tail);
    // Update the "wr" index
    vcp_tx_fifo.wr = len - tail;
  }
  return 0;  // successful completion
}

int vcp_recv (uint8_t* buf, uint16_t len)
{
  // Compute how much data is in the FIFO
  int cap = vcp_rx_fifo.wr - vcp_rx_fifo.rd;
  if (cap == 0)
    return 0;      // Empty FIFO, no data to read
  if (cap < 0)  // FIFO contents wrap around
    cap += vcp_rx_fifo.lb;  // Notice the use of lb
  // Limit the FIFO read to the available data
  if (len > cap)
    len = cap;
  // Save len : it'll be the return value
  int retval = len;
  // Read the data
  while (len)
  {
    len--;
    *buf = vcp_rx_fifo.data[vcp_rx_fifo.rd];
    buf++;
    vcp_rx_fifo.rd++;    // Update read index
    if (vcp_rx_fifo.rd == vcp_rx_fifo.lb)  // Check for wrap-around
      vcp_rx_fifo.rd = 0;      // Follow wrap-around
  }
  return retval;
}

void vcp_service ()
{
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  // Test if the USB CDC is ready to transmit
  if (hcdc->TxState == 0)
  {
    // Update the FIFO to reflect the completion of the last transmission
    vcp_tx_fifo.rd = vcp_tx_fifo.lb;
    // Compute how much data is in the FIFO
    int cap = vcp_tx_fifo.wr - vcp_tx_fifo.rd;
    if (cap != 0)  // The FIFO is empty : return immediately
    {
      if (cap < 0)  // The FIFO contents wrap-around
      {
        // Send only the tail of the FIFO
        USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &vcp_tx_fifo.data[vcp_tx_fifo.rd], APP_TX_DATA_SIZE - vcp_tx_fifo.rd);
        USBD_CDC_TransmitPacket(&hUsbDeviceFS);
        vcp_tx_fifo.lb = 0;    // Lock the tail’s data
      }
      else  // No wrap-around : send the whole FIFO
      {
        USBD_CDC_SetTxBuffer(&hUsbDeviceFS, &vcp_tx_fifo.data[vcp_tx_fifo.rd], cap);
        USBD_CDC_TransmitPacket(&hUsbDeviceFS);
        vcp_tx_fifo.lb = vcp_tx_fifo.wr; // lock the data
      }
    }
  }
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
