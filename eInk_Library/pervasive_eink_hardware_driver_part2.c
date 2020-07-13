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
* For the details of E-INK display hardware and driver, see the 2.7 inch display
* product specification and G2 COG driver interface documents available at the
* following location:
* http://www.pervasivedisplays.com/products/271
*******************************************************************************/
/* Header file includes */
#include "pervasive_eink_hardware_driver.h"
#include <string.h>

/* Variable that stores the packets for one full drive frame. This variable
   is used for full and partial updates */
driver_data_packet_t  bulkDriverPacket[PV_EINK_VERTICAL_SIZE];


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

/* [] END OF FILE */
