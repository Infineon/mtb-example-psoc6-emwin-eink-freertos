/*******************************************************************************
* Copyright (2019), Cypress Semiconductor Corporation. All rights reserved.
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
#include <eInk_Library/cy_eink_psoc_interface.h>
#include <eInk_Library/pervasive_eink_configuration.h>
    
/* Macros of the basic frame (white and black) addresses.
   These addresses do not occur as pointers to actual images */
/* Null pointer is used to denote the white frame */    
#define PV_EINK_WHITE_FRAME_ADDRESS  NULL
/* Final address of the main Flash region is used to denote the black frame */
#define PV_EINK_BLACK_FRAME_ADDRESS  (uint8_t*)0x100FFFFFu
    
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

/* Declarations of functions defined in pv_eink_hardware_driver.c */
/* Power control and initialization functions */
void                Pv_EINK_Init(void);
pv_eink_status_t    Pv_EINK_HardwarePowerOn(void);
pv_eink_status_t    Pv_EINK_HardwarePowerOff(void);

/* Set refresh time factor based on the ambient temperature */
void    Pv_EINK_SetTempFactor(int8_t temperature);

/* Display update functions */
void    Pv_EINK_FullStageHandler(pv_eink_frame_data_t* imagePtr, pv_eink_stage_t stageNumber);
void    Pv_EINK_PartialStageHandler(pv_eink_frame_data_t* previousImagePtr, 
                                    pv_eink_frame_data_t* newImagePtr);
int Cy_EINK_WriteSPIBuffer(uint8_t* data, uint16 dataLength);

#endif  /* PERVASIVE_EINK_HARDWARE_DRIVER_H */
/* [] END OF FILE */
