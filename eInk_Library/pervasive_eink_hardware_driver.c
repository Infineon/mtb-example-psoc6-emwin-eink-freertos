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
* For the details of E-INK display hardware and driver, see the 2.7 inch display
* product specification and G2 COG driver interface documents available at the
* following location:
* http://www.pervasivedisplays.com/products/271
*******************************************************************************/

/* Header file includes */
#include <eInk_Library/pervasive_eink_hardware_driver.h>
#include <string.h>

#define BUFFERED_WRITE  1

/* Macro for the dummy byte used in the line data */
#define PV_EINK_DUMMY_BYTE     (uint8_t)(0x00u)

/* Data used to initialize the scan bytes */
#define PV_EINK_SCAN_BYTE_INIT (uint8_t)(0x00u)

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

/* Variable that stores the packets for one full drive frame. This variable
   is used for full and partial updates */
driver_data_packet_t  bulkDriverPacket[PV_EINK_VERTICAL_SIZE];

/* Variable that stores a single line data. This variable is used to prepare 
   dummy frames */
driver_data_packet_t  static driverPacket;

/* Scan table for the four stages of display update*/
uint8_t const               scanTable[PV_EINK_SCAN_TABLE_SIZE] =
                            PV_EINK_SCAN_TABLE_DATA;

/* The SPI register addresses for Channel Select */
uint8_t const               channelSelect[PV_EINK_CHANNEL_SEL_SIZE] =
                            PV_EINK_CHANNEL_SEL_DATA;

/* Variables that store the driver timing information */
uint16_t static             fullUpdateTime;
uint16_t static             partialUpdateTime;

/* Pointers for the data structures used by the driver */
uint8_t static*             dataLineEven;
uint8_t static*             dataLineOdd;
uint8_t static*             dataLineScan;

/*******************************************************************************
* Function Name: void Pv_EINK_SendData(uint8_t regAddr, uint8_t* data, 
*                                   uint16_t dataLength)
********************************************************************************
*
* Summary: Sends an array of data to the E-INK driver.
*
* Parameters:
*  uint8_t regAddr    : Display driver register address
*  uint8_t* data      : Pointer to the data array that will be sent to the driver
*  uint16_t dataLength: Length of the data array
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_SendData(uint8_t regAddr, uint8_t* data, uint16_t dataLength)
{
    /* Pull the chip select line LOW to begin communication */
    CY_EINK_CsLow;
    /* Send the header of register address index */
    Cy_EINK_WriteSPI(PV_EINK_REG_INDEX_HEADER);
    /* Send the register address */
    Cy_EINK_WriteSPI(regAddr);
    /* Push the chip select line HIGH to end communication */
    CY_EINK_CsHigh;
    
    /* Wait for 10uS before the next transmission */
    CY_EINK_Wait10uS;
    
    /* Pull the chip select line LOW to begin communication */
    CY_EINK_CsLow;
    
    /* Send the header of data write index */
    Cy_EINK_WriteSPI(PV_EINK_REG_DATA_WRITE);

#if (BUFFERED_WRITE == 1)    
    Cy_EINK_WriteSPIBuffer(data, dataLength);
#else    
    //    /* Send the array of data */
    while (dataLength--)
    {
        Cy_EINK_WriteSPI(*data++);
    }
#endif

    /* Push the chip select line HIGH to end communication */
    CY_EINK_CsHigh;
}

/*******************************************************************************
* Function Name: void Pv_EINK_SendByte(uint8_t regAddr, uint8_t data)
********************************************************************************
*
* Summary: Sends a byte of data to the E-INK driver.
*
* Parameters:
*  uint8_t regAddr: Display driver register address
*  uint8_t data   : Data byte that will be sent to the driver
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_SendByte(uint8_t regAddr, uint8_t data)
{
    /* Send a single byte of data */
    Pv_EINK_SendData(regAddr, &data, CY_EINK_SINGLE_BYTE);
}

/*******************************************************************************
* Function Name: uint8_t Pv_EINK_ReadByte(uint8_t regAddr, uint8_t data)
********************************************************************************
*
* Summary: Sends a read command to the E-INK driver and receives a byte of data
*  from the driver.
*
* Parameters:
*  uint8_t regAddr: Display driver register address
*  uint8_t data   : Data command that will be sent to the driver
*
* Return:
*  uint8_t :  Data received from the display driver
*
* Side Effects:
*  None
*******************************************************************************/
uint8_t Pv_EINK_ReadByte(uint8_t regAddr, uint8_t data)
{
    /* Variable that stores received data */
    uint8_t dataRead;
    
    /* Pull the chip select line LOW to begin communication */
    CY_EINK_CsLow;
    /* Send the header of register address index */
    Cy_EINK_WriteSPI(PV_EINK_REG_INDEX_HEADER);
    /* Send the register address */
    Cy_EINK_WriteSPI(regAddr);
    /* Push the chip select line HIGH to end communication */
    CY_EINK_CsHigh;
    
    /* Wait for 10uS before the next transmission */
    CY_EINK_Wait10uS;
    
    /* Pull the chip select line LOW to begin communication */
    CY_EINK_CsLow;
    /* Send the header of data read index */
    Cy_EINK_WriteSPI(PV_EINK_REG_DATA_READ);
    /* Send the command to receive a byte of data */
    dataRead = Cy_EINK_WriteReadSPI(data);
    /* Push the chip select line HIGH to end communication*/
    CY_EINK_CsHigh;
    
    /* Return the received data */
    return(dataRead);
}

/*******************************************************************************
* Function Name: void  Pv_EINK_Init()
********************************************************************************
*
* Summary: Initialize the E-INK display hardware and associated PSoC components.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_Init(void)
{
    /* Enable the level translator located between PSoC and the E-INK display */
    CY_EINK_EnableIO;
    /* Pull the Border pin to LOW */
    CY_EINK_BorderLow;
    /* Disable the load switch connected to E-INK display's Vcc */
    CY_EINK_TurnOffVcc;
    
    /* Initialize the PSoC SPI */
    Cy_EINK_InitSPI();
    /* Pull the chip select, reset and discharge pins to LOW */
    CY_EINK_CsLow;
    CY_EINK_RstLow;
    CY_EINK_DischargeLow;
    
    /* Initialize the E-INK display Timer */
    Cy_EINK_TimerInit();
}

/*******************************************************************************
* Function Name: Cy_EINK_Status PV_EINK_InitDriver(void)
********************************************************************************
*
* Summary: Initialize E-INK Driver. For detailed flow and description, please 
* refer to the driver G2 document Section 4.
*
* Parameters:
*  None
*
* Return:
*  pv_eink_status_t: Status of the initialization
*
* Side Effects:
*  None
*******************************************************************************/
pv_eink_status_t Pv_EINK_InitDriver(void)
{
    /* Variable used to count the number of loops */
    uint16_t    loopCounter = 0;
    
    /* Clear the line buffer with all zeros */
    memset(&driverPacket.lineBuffer, 0, sizeof(driverPacket.lineBuffer)) ;
    
    /* Initialize the even data, odd data and scan data pointers */
    dataLineEven = &driverPacket.lineDataBySize.even[0];
    dataLineOdd  = &driverPacket.lineDataBySize.odd[0];
    dataLineScan = &driverPacket.lineDataBySize.scan[0];
    
    /* Initialize and start the E-INK Timer   */
    Cy_EINK_TimerInit();
        
    /* Check if E-INK driver is busy */
    while (Cy_EINK_IsBusy())
    {
        /* If the E-INK driver is busy for more than maximum allowed time,
           return an error message */
        if (Cy_EINK_GetTimeTick() >= PV_EINK_MAX_PV_EINK_BUSY_TIME)
        {
            return(PV_EINK_ERROR_BUSY);
        }
    }
    
    /* Stop the E-INK Timer */
    Cy_EINK_TimerStop();
    
    /* Check the driver ID */
    if ((Pv_EINK_ReadByte(PV_EINK_DRIVER_ID_COMMAND_INDEX, 
                          PV_EINK_DRIVER_ID_COMMAND_DATA)
                          & PV_EINK_DRIVER_ID_MASK)
                          != PV_EINK_DRIVER_ID_CHECK)
    {
        /* Return an error message if incorrect driver ID is received */
        return(PV_EINK_ERROR_ID);
    }
    /* Disable OE */
    Pv_EINK_SendByte(PV_EINK_DISABLE_OE_COMMAND_INDEX, 
                     PV_EINK_DISABLE_OE_COMMAND_DATA);
    
    /* Check Breakage */
    if ((Pv_EINK_ReadByte(PV_EINK_TEST_BREAKAGE_COMMAND_INDEX,
                          PV_EINK_TEST_BREAKAGE_COMMAND_DATA) & 
                          PV_EINK_TEST_BREAKAGE_MASK)
                          != PV_EINK_TEST_BREAKAGE_CHECK)
    {
        /* Send the error message for breakage */
        return(PV_EINK_ERROR_BREAKAGE);
    }
    /* Power Saving Mode */
    Pv_EINK_SendByte(PV_EINK_PWR_SAVING_COMMAND_INDEX, 
                  PV_EINK_PWR1_SAVING_COMMAND_DATA);
    
    /* Channel Select */
    Pv_EINK_SendData(PV_EINK_CHANNEL_SEL_COMMAND_INDEX, 
                    (uint8_t*) &channelSelect,
                     PV_EINK_CHANNEL_SEL_SIZE);
    
    /* High Power Mode Oscillator Setting */
    Pv_EINK_SendByte(PV_EINK_OSC_SETTING_COMMAND_INDEX, 
                     PV_EINK_OSC_SETTING_COMMAND_DATA);
    /* Power Setting */
    Pv_EINK_SendByte(PV_EINK_PWR1_SETTING_COMMAND_INDEX,
                     PV_EINK_PWR1_SETTING_COMMAND_DATA);
    /* Set Vcom level */
    Pv_EINK_SendByte(PV_EINK_VCOM1_LEVEL_COMMAND_INDEX, 
                     PV_EINK_VCOM1_LEVEL_COMMAND_DATA);
    /* Power Setting */
    Pv_EINK_SendByte(PV_EINK_PWR2_SETTING_COMMAND_INDEX,
                     PV_EINK_PWR2_SETTING_COMMAND_DATA);
                  
    /* Driver latch on */
    Pv_EINK_SendByte(PV_EINK_LATCH_OFF_COMMAND_INDEX,
                     PV_EINK_LATCH_ON_COMMAND_DATA);
    /* Driver latch off */
    Pv_EINK_SendByte(PV_EINK_LATCH_OFF_COMMAND_INDEX, 
                     PV_EINK_LATCH_OFF_COMMAND_DATA);
    CY_EINK_Delay(PV_EINK_PIN_DELAY);
    
    /* Start the charge pump */
    do
    {
        /* Start charge pump positive voltage (VGH & VDH on) */
        Pv_EINK_SendByte(PV_EINK_VGH_VDH_ON_COMMAND_INDEX,
                      PV_EINK_VGH_VDH_ON_COMMAND_DATA);
        CY_EINK_Delay(PV_EINK_POSITIVE_V_DELAY);
        
        /* Start charge pump negative voltage (VGL & VDL on) */
        Pv_EINK_SendByte(PV_EINK_VGH_VDH_ON_COMMAND_INDEX,
                      PV_EINK_VGL_VDL_ON_COMMAND_DATA);
        CY_EINK_Delay(PV_EINK_PWR_CTRL_DELAY);
        
        /* Set charge pump (Vcom_Driver on) */
        Pv_EINK_SendByte(PV_EINK_VCOM_DRIVE_ON_COMMAND_INDEX,
                      PV_EINK_VCOM_DRIVE_ON_COMMAND_DATA);
        CY_EINK_Delay(PV_EINK_PWR_CTRL_DELAY);
        
        /* Check DC level */
        if ((Pv_EINK_ReadByte(PV_EINK_DC_LEVEL_READ_COMMAND_INDEX,
                           PV_EINK_DC_LEVEL_READ_COMMAND_DATA) & 
                           PV_EINK_DC_LEVEL_MASK)
                           != PV_EINK_DC_LEVEL_CHECK)
        {
            /* Disable OLE */
            Pv_EINK_SendByte(PV_EINK_DISABLE_OE_COMMAND_INDEX,
                          PV_EINK_DISABLE_OE_COMMAND_DATA);
            break;
        }
    }
    /* Perform this operation for required charge pump write cycles */
    while ((loopCounter++) != PV_EINK_CHARGE_PUMP_MAX_WRITE);
    
    /* If the operation not completed within the required write cycles, send an 
       error message */
    if (loopCounter >= PV_EINK_CHARGE_PUMP_MAX_WRITE)
    {
        CY_EINK_Wait10uS;
        return(PV_EINK_ERROR_CHARGEPUMP);
    }
    /* If all operations completed successfully, send the corresponding status 
       message */
    return(PV_EINK_RES_OK);
}

/*******************************************************************************
* Function Name: void Pv_EINK_SetTempFactor(int8_t temperature)
********************************************************************************
*
* Summary: Set the E-INK update times per the ambient temperature. For detailed
* flow and description, please refer to the driver G2 document Section 5.4.
*
* Parameters:
* int8_t temperature: Ambient temperature in degree Celsius
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_SetTempFactor(int8_t temperature)
{
    /* Variable used to store the update time */
    uint16_t    updateTime;
    /* Set the update time per the temperature table */
    if (PV_EINK_TEMP_DEG_M10 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL0;
    }
    else if (PV_EINK_TEMP_DEG_M5 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL1;
    }
    else if (PV_EINK_TEMP_DEG_5 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL2;
    }
    else if (PV_EINK_TEMP_DEG_10 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL3;
    }
    else if (PV_EINK_TEMP_DEG_15 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL4;
    }
    else if (PV_EINK_TEMP_DEG_20 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL5;
    }
    else if (PV_EINK_TEMP_DEG_40 >= temperature)
    {
        updateTime = PV_EINK_TEMP_SEL6;
    }
    else
    {
        updateTime = PV_EINK_TEMP_SEL7;
    }
    /* Scale the update times : this is a speed-contrast trade-off */
    fullUpdateTime = updateTime * PV_EINK_SCALING_FULL;
    partialUpdateTime = updateTime * PV_EINK_SCALING_PARTIAL;
}

/*******************************************************************************
* Function Name: void Pv_EINK_FullStageHandler(pv_eink_frame_data_t* imagePtr,
*                                              pv_eink_stage_t stageNumber)
********************************************************************************
*
* Summary: The full update driving stages for getting Odd and Even bytes, and
* then writing the data from memory array to E-INK driver.
*
* One dot/pixel is comprised of 2 bits that can be White(10), Black(11) or 
* Nothing * (01 or 00). The image data bytes must be divided into Odd and Even 
* bytes.
* Line data flow is first half of data bytes -> scan bytes -> second half of data
* bytes. For more details on the driving stages, please refer to the driver G2
* document Section 5.
*
* Parameters:
* pv_eink_frame_data_t* imagePtr    : The pointer to the memory that contains a
*                                     frame
* pv_eink_stage_t stageNumber       : The assigned stage number
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function. CPU will be busy during the entire operation,
* which can be 1 to 2 seconds.
*******************************************************************************/
void Pv_EINK_FullStageHandler(pv_eink_frame_data_t* imagePtr, 
                              pv_eink_stage_t stageNumber)
{
    /* Counter variable for the horizontal pixel loop */
    uint16_t    x;
    /* Counter variable for the vertical pixel loop */
    uint16_t    y;
    /* Counter variable for the horizontal byte loop */
    uint16_t    k;
      
    /* Variable for storing the line number under scan */
    int16       scanlineNumber = 0;
    
    /* Temporary storage for image data byte */
    uint8_t     tempByte = *imagePtr;
    
    /* Flag to check if the current pointer is a macro of white/black frame */
    bool        blackOrWhiteFrame;
    
    /* If the current pointer is a macro of the white frame */
    if (imagePtr == PV_EINK_WHITE_FRAME_ADDRESS)
    {
        /* Set the white/black frame flag */
        blackOrWhiteFrame = true;
        /* Set the tempByte to white pixel byte */
        tempByte = PV_EINK_WHITE_PIXEL_BYTE;
        
    }
    /* If the current pointer is a macro of the black frame */
    else if (imagePtr == PV_EINK_BLACK_FRAME_ADDRESS)
    {
         /* Set the white/black frame flag */
        blackOrWhiteFrame = true;
        /* set the tempByte to black pixel byte */
        tempByte = PV_EINK_BLACK_PIXEL_BYTE;
    }
    /* If the current pointer is of an image stored in flash */
    else
    {
        /* Clear the white/black frame flag */
        blackOrWhiteFrame = false;
    }

    /* Re-initialize E-INK timer to ensure the same duration of each stage */
    Cy_EINK_TimerInit();

    /* Vertical pixel loop */
    for (y = 0; y < PV_EINK_VERTICAL_SIZE; y++)
    {
        /* Clear the line buffer with all zeros */
        memset(&bulkDriverPacket[y].lineBuffer, 0, 
               sizeof(bulkDriverPacket[y].lineBuffer)) ;

        /* Initialize the even data, odd data and scan data pointers */
        dataLineEven = &bulkDriverPacket[y].lineDataBySize.even[0];
        dataLineOdd  = &bulkDriverPacket[y].lineDataBySize.odd[0];
        dataLineScan = &bulkDriverPacket[y].lineDataBySize.scan[0];
        
        /* Horizontal bytes initialization */
        k = PV_EINK_HORIZONTAL_SIZE;
        k--;
        
        /* Horizontal pixel loop */
        for (x = 0; x < PV_EINK_HORIZONTAL_SIZE; x++)
        {
            /* If the current pointer is of an image stored in flash */
            if(!blackOrWhiteFrame)
            {
                /* Fetch successive data bytes */
                tempByte = *imagePtr++;
            }
            /* Perform one of the four stage operations */
            switch (stageNumber)
            {
            /* Stage 1: Calculate the inverted Even and Odd bytes of 
               the previous image data */
            case PV_EINK_STAGE1:
                dataLineOdd[x]       = ((tempByte & PV_EINK_ODD_MASK_A)  ? 
                                        PV_EINK_BLACK3 : PV_EINK_WHITE3);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_B)  ? 
                                        PV_EINK_BLACK2 : PV_EINK_WHITE2);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_C)  ? 
                                        PV_EINK_BLACK1 : PV_EINK_WHITE1);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_D)  ? 
                                        PV_EINK_BLACK0 : PV_EINK_WHITE0);
                dataLineEven[k]      = ((tempByte & PV_EINK_EVEN_MASK_A) ? 
                                        PV_EINK_BLACK0 : PV_EINK_WHITE0);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_B) ? 
                                        PV_EINK_BLACK1 : PV_EINK_WHITE1);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_C) ? 
                                        PV_EINK_BLACK2 : PV_EINK_WHITE2);
                dataLineEven[k--]   |= ((tempByte & PV_EINK_EVEN_MASK_D) ? 
                                        PV_EINK_BLACK3 : PV_EINK_WHITE3);
                break;
            /* Stage 2: Calculate the Even and Odd bytes of an all-white
               frame */
            case PV_EINK_STAGE2:
                dataLineOdd[x]       = ((tempByte & PV_EINK_ODD_MASK_A)  ? 
                                        PV_EINK_WHITE3 : PV_EINK_NOTHING3);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_B)  ? 
                                        PV_EINK_WHITE2 : PV_EINK_NOTHING2);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_C)  ? 
                                        PV_EINK_WHITE1 : PV_EINK_NOTHING1);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_D)  ? 
                                        PV_EINK_WHITE0 : PV_EINK_NOTHING0);
                dataLineEven[k]      = ((tempByte & PV_EINK_EVEN_MASK_A) ?
                                        PV_EINK_WHITE0 : PV_EINK_NOTHING0);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_B) ? 
                                        PV_EINK_WHITE1 : PV_EINK_NOTHING1);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_C) ? 
                                        PV_EINK_WHITE2 : PV_EINK_NOTHING2);
                dataLineEven[k--]   |= ((tempByte & PV_EINK_EVEN_MASK_D) ? 
                                        PV_EINK_WHITE3 : PV_EINK_NOTHING3);
                break;
            /* Stage 3: Calculate the inverted Even and Odd bytes of the
               new image data */
            case PV_EINK_STAGE3:
                dataLineOdd[x]       = ((tempByte & PV_EINK_ODD_MASK_A)  ? 
                                        PV_EINK_BLACK3 : PV_EINK_NOTHING3);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_B)  ? 
                                        PV_EINK_BLACK2 : PV_EINK_NOTHING2);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_C)  ?
                                        PV_EINK_BLACK1 : PV_EINK_NOTHING1);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_D)  ?
                                        PV_EINK_BLACK0 : PV_EINK_NOTHING0);
                dataLineEven[k]      = ((tempByte & PV_EINK_EVEN_MASK_A) ?
                                        PV_EINK_BLACK0 : PV_EINK_NOTHING0);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_B) ? 
                                        PV_EINK_BLACK1 : PV_EINK_NOTHING1);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_C) ? 
                                        PV_EINK_BLACK2 : PV_EINK_NOTHING2);
                dataLineEven[k--]   |= ((tempByte & PV_EINK_EVEN_MASK_D) ? 
                                        PV_EINK_BLACK3 : PV_EINK_NOTHING3);
                break;
            /* Stage 4: Calculate the Even and Odd bytes of new image 
               data */
            case PV_EINK_STAGE4:
                dataLineOdd[x]       = ((tempByte & PV_EINK_ODD_MASK_A)  ? 
                                        PV_EINK_WHITE3 : PV_EINK_BLACK3);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_B)  ?
                                        PV_EINK_WHITE2 : PV_EINK_BLACK2);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_C)  ? 
                                        PV_EINK_WHITE1 : PV_EINK_BLACK1);
                dataLineOdd[x]      |= ((tempByte & PV_EINK_ODD_MASK_D)  ? 
                                        PV_EINK_WHITE0 : PV_EINK_BLACK0);
                dataLineEven[k]     = ((tempByte & PV_EINK_EVEN_MASK_A)  ? 
                                        PV_EINK_WHITE0 : PV_EINK_BLACK0);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_B) ? 
                                        PV_EINK_WHITE1 : PV_EINK_BLACK1);
                dataLineEven[k]     |= ((tempByte & PV_EINK_EVEN_MASK_C) ?
                                        PV_EINK_WHITE2 : PV_EINK_BLACK2);
                dataLineEven[k--]   |= ((tempByte & PV_EINK_EVEN_MASK_D) ? 
                                        PV_EINK_WHITE3 : PV_EINK_BLACK3);
                break;
            }
        }
        /* Move onto the next line */
        scanlineNumber = PV_EINK_VERTICAL_SIZE - y;
        scanlineNumber--;
        
        /* Shift Scan byte according to the data line */
        dataLineScan[(scanlineNumber >> PV_EINK_PIXEL_SIZE)] = scanTable
                            [(scanlineNumber % PV_EINK_SCAN_TABLE_SIZE)];
            
    }
            
    /* Perform the update operation until the total time of frames exceed 
       stage time */
    do
    {
        /* Perform a line by line update */
        for (y = 0; y < PV_EINK_VERTICAL_SIZE; y++)
        {
                /* Send the prepared data to the E-INK display */
            Pv_EINK_SendData(PV_EINK_PIXEL_DATA_COMMAND_INDEX,
                          (uint8_t*) &bulkDriverPacket[y].lineBuffer,
                          PV_EINK_DATA_LINE_SIZE);
                          
            /* Turn on Output Enable to latch the frame */
            Pv_EINK_SendByte(PV_EINK_ENABLE_OE_COMMAND_INDEX,
                          PV_EINK_ENABLE_OE_COMMAND_DATA);
        }
    
    }
    /* Loop until the total time of frames exceed stage time */
    while (fullUpdateTime > Cy_EINK_GetTimeTick());
    
    /* Stop the E-INK Timer */
    Cy_EINK_TimerStop();
}

/*******************************************************************************
* Function Name:  void Pv_EINK_PartialStageHandler(pv_eink_frame_data_t* 
                            previousImagePtr, pv_eink_frame_data_t* newImagePtr)
********************************************************************************
*
* Summary: The partial update driving stage for getting Odd and Even bytes, and
* then writing the data from memory array to E-INK driver.
*
* Partial update is similar to the last stage of full update. If the new data byte
* is same as previous data byte, send a "nothing" pixel, so that E-INK pixel won't
* be altered. If the new data byte is different from the previous data byte, send
* the new data byte.
* For more details on the driving stages, please refer to the driver G2 document
* Section 5.
*
* Parameters:
* pv_eink_frame_data_t* previousImagePtr : The pointer of memory that contains 
*                                    previous frame written to the E-INK display
* pv_eink_frame_data_t* newImagePtr      : The pointer of memory that contains a 
*                       new frame which needs to be written to the E-INK display
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function. CPU will be busy during the entire operation,
*  which can be 1 to 2 seconds. 
*******************************************************************************/
void Pv_EINK_PartialStageHandler(pv_eink_frame_data_t* previousImagePtr, 
                                 pv_eink_frame_data_t* newImagePtr)
{
    /* Counter variable for the horizontal pixel loop */
    uint16_t    x;
    /* Counter variable for the vertical pixel loop */
    uint16_t    y;
    /* Counter variable for the horizontal byte loop */
    uint16_t    k;
    
    /* Temporary storage for image data bytes */
    uint8_t     oldByte;
    /* Temporary storage for image data bytes */
    uint8_t     newByte;
    
    /* Variable for storing the line number under scan */
    int16       scanlineNumber = 0;
    
    /* Re-initialize E-INK timer to ensure the same duration of each stage  */
    Cy_EINK_TimerInit();
    
    /* Vcom level adjust */
    Pv_EINK_SendByte(PV_EINK_VCOM2_LEVEL_COMMAND_INDEX,
                     PV_EINK_VCOM2_LEVEL_COMMAND_DATA);
     
    /* Vertical pixel loop */
    for (y = 0; y < PV_EINK_VERTICAL_SIZE; y++)
    {
        /* Clear the line buffer with all zeros */
        memset(&bulkDriverPacket[y].lineBuffer, 0, 
               sizeof(bulkDriverPacket[y].lineBuffer)) ;

        /* Initialize the even data, odd data and scan data pointers */
        dataLineEven = &bulkDriverPacket[y].lineDataBySize.even[0];
        dataLineOdd  = &bulkDriverPacket[y].lineDataBySize.odd[0];
        dataLineScan = &bulkDriverPacket[y].lineDataBySize.scan[0];

        /* Horizontal bytes initialization */
        k = PV_EINK_HORIZONTAL_SIZE;
        k--;
        
        /* Horizontal pixel loop */
        for (x = 0; x < PV_EINK_HORIZONTAL_SIZE; x++)
        {
            /* Fetch successive data bytes */
            oldByte = *previousImagePtr++;
            newByte = *newImagePtr++;
            
            /* Calculate the Even and Odd bytes for partial update stage. Also, 
               if the new data byte is same as the previous data byte, store a 
               "nothing" pixel, so that the E-INK pixel won't be altered.
               If the new data byte is different from the previous data byte, 
               store the new data byte */
            dataLineOdd[x]       = ((oldByte ^ newByte) & PV_EINK_ODD_MASK_A)
                                    ?((newByte & PV_EINK_ODD_MASK_A)
                                    ? PV_EINK_WHITE3
                                    : PV_EINK_BLACK3) : PV_EINK_NOTHING3;
            dataLineOdd[x]      |= ((oldByte ^ newByte) & PV_EINK_ODD_MASK_B)
                                    ?((newByte & PV_EINK_ODD_MASK_B) 
                                    ? PV_EINK_WHITE2 
                                    : PV_EINK_BLACK2) : PV_EINK_NOTHING2;
            dataLineOdd[x]      |= ((oldByte ^ newByte) & PV_EINK_ODD_MASK_C)
                                    ?((newByte & PV_EINK_ODD_MASK_C) 
                                    ? PV_EINK_WHITE1
                                    : PV_EINK_BLACK1) : PV_EINK_NOTHING1;
            dataLineOdd[x]      |= ((oldByte ^ newByte) & PV_EINK_ODD_MASK_D)
                                    ?((newByte & PV_EINK_ODD_MASK_D) 
                                    ? PV_EINK_WHITE0 
                                    : PV_EINK_BLACK0) : PV_EINK_NOTHING0;
            dataLineEven[k]      = ((oldByte ^ newByte) & PV_EINK_EVEN_MASK_A)
                                    ?((newByte & PV_EINK_EVEN_MASK_A) 
                                    ? PV_EINK_WHITE0
                                    : PV_EINK_BLACK0) : PV_EINK_NOTHING0;
            dataLineEven[k]     |= ((oldByte ^ newByte) & PV_EINK_EVEN_MASK_B) 
                                    ?((newByte & PV_EINK_EVEN_MASK_B) 
                                    ? PV_EINK_WHITE1
                                    : PV_EINK_BLACK1) : PV_EINK_NOTHING1;
            dataLineEven[k]     |= ((oldByte ^ newByte) & PV_EINK_EVEN_MASK_C)
                                    ?((newByte & PV_EINK_EVEN_MASK_C) 
                                    ? PV_EINK_WHITE2 
                                    : PV_EINK_BLACK2) : PV_EINK_NOTHING2;
            dataLineEven[k--]   |= ((oldByte ^ newByte) & PV_EINK_EVEN_MASK_D)
                                    ?((newByte & PV_EINK_EVEN_MASK_D) 
                                    ? PV_EINK_WHITE3 
                                    : PV_EINK_BLACK3) : PV_EINK_NOTHING3;
        }
        /* Move onto the next line */
        scanlineNumber = PV_EINK_VERTICAL_SIZE - y;
        scanlineNumber--;
        
        /* Shift Scan byte according to the data line */
        dataLineScan[(scanlineNumber >> PV_EINK_PIXEL_SIZE)] = scanTable
                            [(scanlineNumber % PV_EINK_SCAN_TABLE_SIZE)];
    }
    
    /* Perform the update operation until the total time of frames exceed stage
       time */
    do
    {
        /* Perform a line by line update */
        for (y = 0; y < PV_EINK_VERTICAL_SIZE; y++)
        {
            /* Send the prepared data to the E-INK display */
            Pv_EINK_SendData(PV_EINK_PIXEL_DATA_COMMAND_INDEX,
                            (uint8_t*) &bulkDriverPacket[y].lineBuffer,
                             PV_EINK_DATA_LINE_SIZE);
            /* Turn on Output Enable to latch the frame */
            Pv_EINK_SendByte(PV_EINK_ENABLE_OE_COMMAND_INDEX,
                             PV_EINK_ENABLE_OE_COMMAND_DATA);
            
            /* Calculate the current line under scan */
            scanlineNumber = PV_EINK_VERTICAL_SIZE - y;
            scanlineNumber--;
            
            /* Re-initialize the scan line (extra step for partial update) */
            bulkDriverPacket[y].lineDataBySize.scan[(scanlineNumber 
                                                     >> PV_EINK_PIXEL_SIZE)]
                                                     = PV_EINK_SCAN_BYTE_INIT;

            Pv_EINK_SendData(PV_EINK_PIXEL_DATA_COMMAND_INDEX,
                            (uint8_t*) &bulkDriverPacket[y].lineBuffer,
                             PV_EINK_DATA_LINE_SIZE);
            Pv_EINK_SendByte(PV_EINK_ENABLE_OE_COMMAND_INDEX,
                             PV_EINK_ENABLE_OE_COMMAND_DATA);
        }
    }
    /* Loop until the total time of frames exceed stage time */
    while (partialUpdateTime > Cy_EINK_GetTimeTick());
    
    /* Stop system timer */
    Cy_EINK_TimerStop();
    
    /* Re-adjust Vcom level */
    Pv_EINK_SendByte(PV_EINK_VCOM1_LEVEL_COMMAND_INDEX, 
                     PV_EINK_VCOM1_LEVEL_COMMAND_DATA);
}

/*******************************************************************************
* Function Name:  void Pv_EINK_NothingFrame(void)
********************************************************************************
*
* Summary: Write a "Nothing Frame" to driver. For a "Nothing Frame",
* all pixel data D(x,y) are "01".
* For more details, please refer to the driver G2 document Section 6.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_NothingFrame(void)
{
    /* Counter variable for data line bytes */
    uint16_t    byteCounter;
    
    /* Set all Even and Odd data bytes as "Nothing" bytes */
    for (byteCounter = 0; byteCounter < PV_EINK_HORIZONTAL_SIZE; byteCounter++)
    {
        dataLineEven[byteCounter] = PV_EINK_NOTHING;
        dataLineOdd[byteCounter] = PV_EINK_NOTHING;
    }
    /* Send the same horizontal data for all vertical lines */
    for (byteCounter = 0; byteCounter < PV_EINK_VERTICAL_SIZE; byteCounter++)
    {
        /* Scan byte shift per data line */
        dataLineScan[(byteCounter >> PV_EINK_PIXEL_SIZE)] = scanTable
                            [(byteCounter % PV_EINK_SCAN_TABLE_SIZE)];
        
        /* Send the prepared data to the E-INK display */
        Pv_EINK_SendData(PV_EINK_PIXEL_DATA_COMMAND_INDEX,
                        (uint8_t*) &driverPacket.lineBuffer,
                         PV_EINK_DATA_LINE_SIZE);
        /* Turn on Output Enable to latch the frame */
        Pv_EINK_SendByte(PV_EINK_ENABLE_OE_COMMAND_INDEX, 
                         PV_EINK_ENABLE_OE_COMMAND_DATA);
        
        /* Initialize the next scan byte */
        dataLineScan[(byteCounter >> PV_EINK_PIXEL_SIZE)] = 0u;
    }
}

/*******************************************************************************
* Function Name:  void Pv_EINK_DummyLine(void)
********************************************************************************
*
* Summary: Write a "Dummy Line" to driver. For a "Dummy Line" , all pixel data
* are "00". For more details, please refer to the driver G2 document Section 6.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  None
*******************************************************************************/
void Pv_EINK_DummyLine(void)
{
    /* Counter variable for data line bytes */
    uint8_t byteCounter;
    
    /* Create a data line containing dummy bytes */
    for (byteCounter = 0; byteCounter < (PV_EINK_VERTICAL_SIZE 
                                        / CY_EINK_BYTE_SIZE);
         byteCounter++)
    {
        driverPacket.lineBuffer[byteCounter] = PV_EINK_DUMMY_BYTE;
    }
    
    /* Set charge pump voltage level to reduce voltage shift */
    Pv_EINK_SendByte(PV_EINK_PWR2_SETTING_COMMAND_INDEX, 
                     PV_EINK_VOLTAGE_LEVEL);
    /* Send the prepared data to the E-INK display */
    Pv_EINK_SendData(PV_EINK_PIXEL_DATA_COMMAND_INDEX,
                    (uint8_t*) &driverPacket.lineBuffer, 
                     PV_EINK_DATA_LINE_SIZE);
    /* Turn on Output Enable to latch the frame */
    Pv_EINK_SendByte(PV_EINK_ENABLE_OE_COMMAND_INDEX, 
                     PV_EINK_ENABLE_OE_COMMAND_DATA);
}

/*******************************************************************************
* Function Name: pv_eink_status_t Pv_EINK_HardwarePowerOn()
********************************************************************************
*
* Summary: Power on E-INK Driver. For detailed flow and description, please refer
* to the driver G2 document Section 3.
*
* Parameters:
*  None
*
* Return:
*  pv_eink_status_t: Status of the power on operation
*
* Side Effects:
*  None
*******************************************************************************/
pv_eink_status_t Pv_EINK_HardwarePowerOn(void)
{
    /* Enable the load switch connected to E-INK display's Vcc, and wait till
    the voltage ramps up to the level required for proper operation of E-INK */
    CY_EINK_TurnOnVcc;
    CY_EINK_Delay(PV_EINK_PWR_DELAY);
    
    /* Attach PSoC's SPI hardware to E-INK driver */
    Cy_EINK_AttachSPI();
    /* Push the chip select, and border pins to HIGH */
    CY_EINK_CsHigh;
    CY_EINK_BorderHigh;
    
    /* Create two pulses on the reset line to properly reset the E-INK driver */
    CY_EINK_RstHigh;
    CY_EINK_Delay(PV_EINK_PIN_DELAY);
    CY_EINK_RstLow;
    CY_EINK_Delay(PV_EINK_PIN_DELAY);
    CY_EINK_RstHigh;
    CY_EINK_Delay(PV_EINK_PIN_DELAY);
    
    /* Initialize E-INK Driver */
    return(Pv_EINK_InitDriver());
}

/*******************************************************************************
* Function Name:  pv_eink_status_t Pv_EINK_HardwarePowerOff(void)
********************************************************************************
*
* Summary: Safely power off the E-INK display. For detailed flow and description,
* please refer to the driver G2 document Section 6.
*
* Parameters:
*  None
*
* Return:
*  pv_eink_status_t : Status of the power off operation
*
* Side Effects:
*  None
*******************************************************************************/
pv_eink_status_t Pv_EINK_HardwarePowerOff(void)
{
    /* Send a "Nothing Frame", followed by a "Dummy Line" to the E-INK driver */
    Pv_EINK_NothingFrame();
    Pv_EINK_DummyLine();
    
    /* After E-INK updates, the border color may degrade to a gray level that is not
    as white as the active area. Toggle the Border pin to avoid this phenomenon. */
    CY_EINK_Delay(PV_EINK_DUMMY_LINE_DELAY);
    CY_EINK_BorderLow;
    CY_EINK_Delay(PV_EINK_BOARDER_DELAY);
    CY_EINK_BorderHigh;
    
    /* Check DC level */
    if ((Pv_EINK_ReadByte(PV_EINK_DC_LEVEL_READ_COMMAND_INDEX,
                       PV_EINK_DC_LEVEL_READ_COMMAND_DATA) & PV_EINK_DC_LEVEL_MASK
        ) == PV_EINK_DC_LEVEL_CHECK)
    {
        /* Return error message for improper DC level */
        return(PV_EINK_ERROR_DC);
    }
    
    /* Power Saving */
    Pv_EINK_SendByte(PV_EINK_PWR_SAVING_COMMAND_INDEX,
                     PV_EINK_PWR2_SAVING_COMMAND_DATA);
    /* Turn on Latch Reset */
    Pv_EINK_SendByte(PV_EINK_LATCH_ON_COMMAND_INDEX,
                     PV_EINK_LATCH_ON_COMMAND_DATA);
    /* Power off charge pump Vcom */
    Pv_EINK_SendByte(PV_EINK_VGL_VDL_ON_COMMAND_INDEX,
                     PV_EINK_VGL_VDL_ON_COMMAND_DATA);
    /* Power off charge pump negative voltage */
    Pv_EINK_SendByte(PV_EINK_VGH_VDH_ON_COMMAND_INDEX,
                     PV_EINK_VGH_VDH_ON_COMMAND_DATA);
    
    CY_EINK_Delay(PV_EINK_DISCH_SPI_DELAY);
    /* Discharge internal SPI */
    Pv_EINK_SendByte(PV_EINK_DISCH_SPI_COMMAND_INDEX, 
                     PV_EINK_DISCH_SPI_COMMAND_DATA);
    /* Turn off all charge pump */
    Pv_EINK_SendByte(PV_EINK_CH_PUMP_OFF_COMMAND_INDEX, 
                     PV_EINK_CH_PUMP_OFF_COMMAND_DATA);
    /* Turn off OSC */
    Pv_EINK_SendByte(PV_EINK_OSC_OFF_COMMAND_INDEX, 
                     PV_EINK_OSC_OFF_COMMAND_DATA);
    CY_EINK_Delay(PV_EINK_PWR_OFF_DELAY);
    
    /* Detach SPI and disable the load switch connected to E-INK display's Vcc */
    Cy_EINK_DetachSPI();
    CY_EINK_TurnOffVcc;
    
    /* Return the pins to their default (OFF) values*/
    CY_EINK_BorderLow;
    CY_EINK_Delay(PV_EINK_CS_OFF_DELAY);
    CY_EINK_CsLow;
    CY_EINK_RstLow;
    CY_EINK_DischargeHigh;
    CY_EINK_Delay(PV_EINK_DETACH_DELAY);
    CY_EINK_DischargeLow;
    
    /* If all operations were completed successfully, send the corresponding flag */
    return(PV_EINK_RES_OK);
}

/* [] END OF FILE */
