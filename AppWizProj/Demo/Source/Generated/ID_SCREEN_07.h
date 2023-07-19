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
File        : ID_SCREEN_07.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef ID_SCREEN_07_H
#define ID_SCREEN_07_H

#include "AppWizard.h"

/*********************************************************************
*
*       Objects
*/
#define ID_BOX_00   (GUI_ID_USER + 1)
#define ID_TEXT_00  (GUI_ID_USER + 2)
#define ID_TEXT_01  (GUI_ID_USER + 3)
#define ID_TEXT_02  (GUI_ID_USER + 4)
#define ID_TIMER_00 (GUI_ID_USER + 5)

/*********************************************************************
*
*       Slots
*/
void ID_SCREEN_07__APPW_NOTIFICATION_INITDIALOG        (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_07__ID_TIMER_00__APPW_NOTIFICATION_TIMER(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);

/*********************************************************************
*
*       Callback
*/
void cbID_SCREEN_07(WM_MESSAGE * pMsg);

#endif  // ID_SCREEN_07_H

/*************************** End of file ****************************/
