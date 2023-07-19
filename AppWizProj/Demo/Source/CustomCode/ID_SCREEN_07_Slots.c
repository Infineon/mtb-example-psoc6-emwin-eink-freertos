/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2023  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : ID_SCREEN_07_Slots.c
Purpose     : AppWizard managed file, function content could be changed
---------------------------END-OF-HEADER------------------------------
*/

#include "Application.h"
#include "../Generated/Resource.h"
#include "../Generated/ID_SCREEN_07.h"

/*** Begin of user code area ***/
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "../../../../source/eink_task.h"
/*** End of user code area ***/

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       cbID_SCREEN_07
*/
void cbID_SCREEN_07(WM_MESSAGE * pMsg) {
  GUI_USE_PARA(pMsg);
}

/*********************************************************************
*
*       ID_SCREEN_07__APPW_NOTIFICATION_INITDIALOG
*/
void ID_SCREEN_07__APPW_NOTIFICATION_INITDIALOG(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
}

/*********************************************************************
*
*       ID_SCREEN_07__ID_TIMER_00__APPW_NOTIFICATION_TIMER
*/
void ID_SCREEN_07__ID_TIMER_00__APPW_NOTIFICATION_TIMER(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult) {
  GUI_USE_PARA(pAction);
  GUI_USE_PARA(hScreen);
  GUI_USE_PARA(pMsg);
  GUI_USE_PARA(pResult);
  cyhal_gpio_write( CYBSP_USER_LED, CYBSP_LED_STATE_OFF);
  wait_for_switch_press_and_release();
  cyhal_gpio_write( CYBSP_USER_LED, CYBSP_LED_STATE_ON);
}

/*************************** End of file ****************************/
