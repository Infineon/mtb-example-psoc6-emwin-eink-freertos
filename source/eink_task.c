/******************************************************************************
* 
* File Name: eink_task.c
*
* Description: This file contains task and functions related to the eInk_task
* that demonstrates controlling a E-Ink display using the EmWin Graphics Library
* and Appwizard GUI design tool.
* The project displays a start up screen with Infineon logo and
* text "INFINEON EMWIN GRAPHICS DEMO EINK DISPLAY".
*
* The project then displays the following screens in a loop
*
*   1. A screen showing various text alignments, styles and modes
*   2. A screen showing normal fonts
*   3. A screen showing bold fonts
*   4. A screen showing 2D graphics with horizontal lines, vertical lines
*       arcs and filled rounded rectangle
*   5. A screen showing 2D graphics with concentric circles and ellipses
*   6. A screen showing a text box with wrapped text
*
*******************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/*******************************************************************************
 * Header file includes
 ******************************************************************************/
#include "cyhal.h"
#include "cybsp.h"
#include "AppWizard.h"
#include "../AppWizProj/Demo/Source/Generated/Resource.h"
/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Macros
*******************************************************************************/

/*******************************************************************************
* Forward declaration
*******************************************************************************/

/*******************************************************************************
 * Function Definitions
 ******************************************************************************/

/*******************************************************************************
* Function Name: void wait_for_switch_press_and_release(void)
********************************************************************************
*
* Summary: This implements a simple "Wait for button press and release"
*           function.  It first waits for the button to be pressed and then
*           waits for the button to be released.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This is a blocking function and exits only on a button press and release
*
*******************************************************************************/
void wait_for_switch_press_and_release(void)
{
    /* Wait for SW2 to be pressed */
    while( CYBSP_BTN_PRESSED != cyhal_gpio_read(CYBSP_USER_BTN));

    /* Wait for SW2 to be released */
    while( CYBSP_BTN_OFF == cyhal_gpio_read(CYBSP_USER_BTN));
}

/*******************************************************************************
* Function Name: void eInk_task(void *arg)
********************************************************************************
*
* Summary: Following functions are performed
*           1. Initialize the EmWin library
*           2. Display the startup screen for 2 seconds
*           3. Display the instruction screen and wait for key press and release
*           4. Inside a while loop scroll through the 6 demo pages on every
*               key press and release
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void eInk_task(void *arg)
{
    cy_rslt_t result;

    /* Configure Switch and LEDs*/
    result = cyhal_gpio_init( CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP,
                     CYBSP_BTN_OFF);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    result = cyhal_gpio_init( CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG,
                     CYBSP_LED_STATE_OFF);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    MainTask();
}
/* [] END OF FILE */
