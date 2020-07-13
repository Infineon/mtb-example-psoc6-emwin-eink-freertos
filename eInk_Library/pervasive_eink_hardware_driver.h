/*******************************************************************************
* Copyright (2019-2020), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death (“High Risk Product”). By
* including Cypress’s product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
********************************************************************************/
/******************************************************************************
* For the details of E-INK display hardware and driver interface, see the 
* documents available at the following location:
* http://www.pervasivedisplays.com/products/271
*******************************************************************************/

/* Include Guard */
#ifndef PERVASIVE_EINK_HARDWARE_DRIVER_H
#define PERVASIVE_EINK_HARDWARE_DRIVER_H

/* Header file includes */
#include "cy_eink_psoc_interface.h"
#include "pervasive_eink_configuration.h"

/* Macros of the basic frame (white and black) addresses.
   These addresses do not occur as pointers to actual images */
/* Null pointer is used to denote the white frame */    
#define PV_EINK_WHITE_FRAME_ADDRESS  NULL
/* Final address of the main Flash region is used to denote the black frame */
#define PV_EINK_BLACK_FRAME_ADDRESS  (uint8_t*)0x100FFFFFu
/* Macro for the dummy byte used in the line data */
#define PV_EINK_DUMMY_BYTE     (uint8_t)(0x00u)
/* Data used to initialize the scan bytes */
#define PV_EINK_SCAN_BYTE_INIT (uint8_t)(0x00u)

#define BUFFERED_WRITE  1

/* Data type of E-INK image */
typedef unsigned char                pv_eink_frame_data_t;

/* Data-type of E-INK status messages */
typedef enum
{
    PV_EINK_RES_OK,
    PV_EINK_ERROR_BUSY,
    PV_EINK_ERROR_ID,
    PV_EINK_ERROR_BREAKAGE,
    PV_EINK_ERROR_DC,
    PV_EINK_ERROR_CHARGEPUMP
}   pv_eink_status_t;

/* Data-type of E-INK update stages */
typedef enum 
{ 
    PV_EINK_STAGE1,
    PV_EINK_STAGE2,
    PV_EINK_STAGE3,
    PV_EINK_STAGE4
}   pv_eink_stage_t ;

/* Line data structure of 2.7" display. Refer to driver document Section 5.1 
   for details */
struct eink_lineData
{
    /*  Dummy byte (0x00) */
    uint8_t dummyData;

    /*  2.7" even byte array */
    uint8_t even[PV_EINK_HORIZONTAL_SIZE];

    /*  2.7" scan byte array */
    uint8_t scan[PV_EINK_SCAN_LINE_SIZE];

    /*  2.7" odd byte array */
    uint8_t odd[PV_EINK_HORIZONTAL_SIZE];
};

/* Packet structure of a line data */
typedef union
{
    /* Line data structure of 2.7" E-INK display */
    struct eink_lineData    lineDataBySize;

    /* Line buffer equal to the data line size */
    uint8_t                 lineBuffer[PV_EINK_DATA_LINE_SIZE];

    /* The maximum line buffer data size as length */
}   driver_data_packet_t;

/* Variables that store the driver timing information */
extern uint16_t              fullUpdateTime;
extern uint16_t              partialUpdateTime;

/* Pointers for the data structures used by the driver */
extern uint8_t*              dataLineEven;
extern uint8_t*              dataLineOdd;
extern uint8_t*              dataLineScan;

/* Scan table for the four stages of display update*/
extern uint8_t const        scanTable[PV_EINK_SCAN_TABLE_SIZE];

/* The SPI register addresses for Channel Select */
extern uint8_t const        channelSelect[PV_EINK_CHANNEL_SEL_SIZE];

/* Declarations of functions defined in pv_eink_hardware_driver.c */
/* Power control and initialization functions */
void                Pv_EINK_Init(void);
pv_eink_status_t    Pv_EINK_HardwarePowerOn(void);
pv_eink_status_t    Pv_EINK_HardwarePowerOff(void);

/* Set refresh time factor based on the ambient temperature */
void    Pv_EINK_SetTempFactor(int8_t temperature);

/* Display update functions */
void    Pv_EINK_FullStageHandler(pv_eink_frame_data_t* imagePtr,
                                 pv_eink_stage_t stageNumber);
void    Pv_EINK_PartialStageHandler(pv_eink_frame_data_t* previousImagePtr,
                                    pv_eink_frame_data_t* newImagePtr);
int     Cy_EINK_WriteSPIBuffer(uint8_t* data, uint16 dataLength);
void    Pv_EINK_SendData(uint8_t regAddr, uint8_t* data, uint16_t dataLength);
void    Pv_EINK_SendByte(uint8_t regAddr, uint8_t data);

#endif  /* PERVASIVE_EINK_HARDWARE_DRIVER_H */
/* [] END OF FILE */
