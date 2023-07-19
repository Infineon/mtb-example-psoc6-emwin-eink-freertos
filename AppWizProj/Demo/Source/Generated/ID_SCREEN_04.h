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
File        : ID_SCREEN_04.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef ID_SCREEN_04_H
#define ID_SCREEN_04_H

#include "AppWizard.h"

/*********************************************************************
*
*       Objects
*/
#define ID_BOX_00   (GUI_ID_USER + 2)
#define ID_TEXT_00  (GUI_ID_USER + 1)
#define ID_TEXT_01  (GUI_ID_USER + 3)
#define ID_TEXT_02  (GUI_ID_USER + 4)
#define ID_TEXT_03  (GUI_ID_USER + 5)
#define ID_TEXT_04  (GUI_ID_USER + 6)
#define ID_TEXT_05  (GUI_ID_USER + 7)
#define ID_TEXT_06  (GUI_ID_USER + 8)
#define ID_BOX_01   (GUI_ID_USER + 12)
#define ID_TEXT_07  (GUI_ID_USER + 9)
#define ID_TEXT_08  (GUI_ID_USER + 11)
#define ID_TIMER_00 (GUI_ID_USER + 10)

/*********************************************************************
*
*       Slots
*/
void ID_SCREEN_04__APPW_NOTIFICATION_INITDIALOG__ID_TIMER_00__APPW_JOB_START(APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);
void ID_SCREEN_04__ID_TIMER_00__APPW_NOTIFICATION_TIMER                     (APPW_ACTION_ITEM * pAction, WM_HWIN hScreen, WM_MESSAGE * pMsg, int * pResult);

/*********************************************************************
*
*       Callback
*/
void cbID_SCREEN_04(WM_MESSAGE * pMsg);

#endif  // ID_SCREEN_04_H

/*************************** End of file ****************************/
