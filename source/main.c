/******************************************************************************
* File Name: main.c
*
* Description: This code example demonstrates displaying graphics on an EInk
* display using EmWin graphics library.
*
* Related Document: See Readme.md
*
*******************************************************************************
* (c) 2019-2020, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
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
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "eink_task.h"
#include "FreeRTOS.h"
#include "task.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define EINK_TASK_STACK_SIZE        (1024*10)
#define EINK_TASK_PRIORITY          (configMAX_PRIORITIES - 3)
#define CLOCK_100_MHZ               (100000000u)

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* This enables RTOS aware debugging. */
volatile int uxTopUsedPriority;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function sets up the E-Ink task and starts
*  the RTOS scheduler.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result ;
    cyhal_clock_t clock_fll;
    cyhal_clock_t clock_hf0;
    cyhal_clock_t clock_peri;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the board support package */
    result = cybsp_init() ;
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* Initialize the FLL and set frequency to 100MHz */
    cyhal_clock_get(&clock_fll, &CYHAL_CLOCK_FLL);
    cyhal_clock_init(&clock_fll);
    cyhal_clock_set_frequency(&clock_fll, CLOCK_100_MHZ, NULL);
    cyhal_clock_set_enabled(&clock_fll, true, true);

    /* Initialize the HF0 and set source as FLL */
    cyhal_clock_get(&clock_hf0, &CYHAL_CLOCK_HF[0]);
    cyhal_clock_init(&clock_hf0);
    cyhal_clock_set_source(&clock_hf0, &clock_fll);
    cyhal_clock_set_divider(&clock_hf0, 1);

    /* Initialize the Peri clock, set source as HF0 and set divider to 1 */
    cyhal_clock_get(&clock_peri, &CYHAL_CLOCK_PERI);
    cyhal_clock_init(&clock_peri);
    cyhal_clock_set_divider(&clock_peri, 1);

    /* Free the clock objects as they are no longer required */
    cyhal_clock_free(&clock_fll);
    cyhal_clock_free(&clock_hf0);
    cyhal_clock_free(&clock_peri);

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
                                 CY_RETARGET_IO_BAUDRATE);
    CY_ASSERT(result == CY_RSLT_SUCCESS);

    /* To avoid compiler warning */
    (void)result;

    /* Enable global interrupts. */
    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("**********************************************************\r\n");
    printf("PSoC 6 MCU emWin E-Ink\r\n");
    printf("**********************************************************\r\n");

    /* Create the E-Ink task */
    xTaskCreate( eInk_task, "eInkTask", EINK_TASK_STACK_SIZE,
                 NULL,  EINK_TASK_PRIORITY,  NULL);

    /* Start the FreeRTOS scheduler. */
    vTaskStartScheduler();
    
    /* Should never get here. */
    CY_ASSERT(0);
}
