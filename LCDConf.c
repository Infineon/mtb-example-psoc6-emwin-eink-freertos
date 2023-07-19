/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2018  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.48 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software  has been licensed to  Cypress Semiconductor Corporation,
whose registered  office is situated  at 198 Champion Ct. San Jose, CA
95134 USA  solely for the  purposes of creating  libraries for Infineon
PSoC3 and  PSoC5 processor-based devices,  sublicensed and distributed
under  the  terms  and  conditions  of  the  Infineon  End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              Cypress Semiconductor Corp, 198 Champion Ct., San Jose, CA 95134, USA
Licensed SEGGER software: emWin
License number:           GUI-00319
License model:            Services and License Agreement, signed June 10th, 2009
Licensed platform:        Any Cypress platform (Initial targets are: PSoC3, PSoC5)
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2009-06-12 - 2022-07-27
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration.
Display Ctrl: Pervasive_E2271CS021
---------------------------END-OF-HEADER------------------------------
*/

#include "emwin.h"

#if defined(EMWIN_ENABLED)

#include <stddef.h>
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "LCDConf.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy8ckit_028_epd_pins.h"
#include "mtb_e2271cs021.h"

/*********************************************************************
*
*       Layer configuration
*
**********************************************************************
*/
/* Physical display size */
#define XSIZE_PHYS                          MTB_DISPLAY_SIZE_X
#define YSIZE_PHYS                          MTB_DISPLAY_SIZE_Y
#define AMBIENT_TEMPERATURE_C               (20)
#define SPI_BAUD_RATE_HZ                    (20000000)

/* Color conversion */
#define COLOR_CONVERSION GUICC_1

/*********************************************************************
*
*       Driver determination
*
**********************************************************************
*/
#define DISPLAY_DRIVER GUIDRV_LIN_1
/* We actually just use one buffer but we update the screen when emWin switches buffers */
#define NUM_BUFFERS   2      
#define NUM_VSCREENS  1

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
/* Checking display driver configuration */
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif

/* Number of bytes per line */
#define BYTES_PER_LINE ((XSIZE_PHYS + 7) / 8)

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

static void * _aBufferPtr[NUM_BUFFERS];
/* Data arrays to be used by the display driver */
static U8 _aPlain_0[XSIZE_PHYS * YSIZE_PHYS];

/* HAL SPI object to interface with display driver */
static cyhal_spi_t spi;

/* Configuration structure defining the necessary pins to communicate with
 * the E-ink display */
static const mtb_e2271cs021_pins_t pins =
{
    .spi_mosi = CY8CKIT_028_EPD_PIN_DISPLAY_SPI_MOSI,
    .spi_miso = CY8CKIT_028_EPD_PIN_DISPLAY_SPI_MISO,
    .spi_sclk = CY8CKIT_028_EPD_PIN_DISPLAY_SPI_SCLK,
    .spi_cs = CY8CKIT_028_EPD_PIN_DISPLAY_CS,
    .reset = CY8CKIT_028_EPD_PIN_DISPLAY_RST,
    .busy = CY8CKIT_028_EPD_PIN_DISPLAY_BUSY,
    .discharge = CY8CKIT_028_EPD_PIN_DISPLAY_DISCHARGE,
    .enable = CY8CKIT_028_EPD_PIN_DISPLAY_EN,
    .border = CY8CKIT_028_EPD_PIN_DISPLAY_BORDER,
    .io_enable = CY8CKIT_028_EPD_PIN_DISPLAY_IOEN
};

/* Buffer to the previous frame written on the display */
uint8_t previous_frame[PV_EINK_IMAGE_SIZE] = {0};

/* Pointer to the new frame that need to be written */
uint8_t *current_frame;

static void _CopyBuffer(int LayerIndex, int IndexSrc, int IndexDst) 
{
    /* Do nothing since we only use one buffer */
}
/********************************************************************
*
*       CY8CKIT_028_EPD_InitController
*
* Purpose:
*   Initializes the display controller
*/
static void CY8CKIT_028_EPD_InitController(void)
{
    cy_rslt_t result;

    result = cyhal_spi_init(&spi,
                            pins.spi_mosi,
                            pins.spi_miso,
                            pins.spi_sclk,
                            NC,
                            NULL,
                            8,
                            CYHAL_SPI_MODE_00_MSB,
                            false);
    if (CY_RSLT_SUCCESS == result)
    {
        result = cyhal_spi_set_frequency(&spi, SPI_BAUD_RATE_HZ);
        if(CY_RSLT_SUCCESS == result)
        {
            result = mtb_e2271cs021_init(&pins, &spi);

        }
        /* Set ambient temperature, in degree C, in order to perform temperature
         * compensation of E-INK parameters */
        mtb_e2271cs021_set_temp_factor(AMBIENT_TEMPERATURE_C);
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void)
{
    int i;
    GUI_DEVICE * pDevice;
#if (NUM_BUFFERS > 1)
    GUI_MULTIBUF_Config(NUM_BUFFERS);
#endif
    /* Set display driver and color conversion for 1st layer */
    pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
    CY_UNUSED_PARAMETER(pDevice);
    /* Display driver configuration */
    if (LCD_GetSwapXY())
    {
        LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
        LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
    }
    else
    {
        LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
        LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
    }
    /* Init buffer pointers.
     Both point to the same address because we actually only use one buffer
     so that we can react on the buffer switch.
    */
    for (i = 0; i < GUI_COUNTOF(_aBufferPtr); i++) 
    {
         _aBufferPtr[i] = _aPlain_0;
    }
    LCD_SetVRAMAddrEx(0, (void *)_aPlain_0);
    LCD_SetBufferPtrEx(0, _aBufferPtr);
    /* Set custom function for copying the buffer that does nothing since we
     only use one buffer.
    */ 
    LCD_SetDevFunc(0, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))_CopyBuffer);}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task, the routine needs to be adapted to
*   the display controller. Note that the commands marked
*   "optional" are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Zero based layer index
*   Cmd        - Command to be executed
*   pData      - Pointer to a data structure.
*
* Return Value:
*   < -1 - Error
*     -1 - The command is not handled.
*      0 - OK.
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
    int r;
    LCD_X_SHOWBUFFER_INFO *p;
    cy_rslt_t result;
    /* GUI_USE_PARA(LayerIndex); */
    /* GUI_USE_PARA(pData); */

    switch (Cmd)
    {
        case LCD_X_INITCONTROLLER:
        {
            CY8CKIT_028_EPD_InitController();
            r = 0;
            break;
        }
        case LCD_X_SHOWBUFFER:
        {
            current_frame = (uint8_t*)_aPlain_0;  
            result = mtb_e2271cs021_show_frame(previous_frame, current_frame,
                                               MTB_E2271CS021_FULL_4STAGE, true);  
            CY_UNUSED_PARAMETER(result);                                               
            p = (LCD_X_SHOWBUFFER_INFO *)pData;
            GUI_MULTIBUF_ConfirmEx(0, p->Index);
            r = 0;
            break;
        }
        default:
        {
            r = -1;
            break;
        }
    }
    return r;
}
#endif /* defined(EMWIN_ENABLED) */
/*************************** End of file ****************************/
