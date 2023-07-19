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
File        : ID_SCREEN_07.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "Resource.h"
#include "ID_SCREEN_07.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _aCreate
*/
static APPW_CREATE_ITEM _aCreate[] = {
  { WM_OBJECT_WINDOW_Create,
    ID_SCREEN_07, 0,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_BOX_Create,
    ID_BOX_00, ID_SCREEN_07,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      264, 176, 0, 0, 0, 0
    },
    { 0, ID_DRAW_003 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_00, ID_SCREEN_07,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 5, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      264, 13, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_01, ID_SCREEN_07,
    { { { DISPOSE_MODE_REL_PARENT, 0, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 85, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      132, 13, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TEXT_Create,
    ID_TEXT_02, ID_SCREEN_07,
    { { { DISPOSE_MODE_REL_PARENT, 132, 0, 0 },
        { DISPOSE_MODE_REL_PARENT, 85, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      132, 13, 0, 0, 0, 0
    },
    { 0, 0 }
  },
  { WM_OBJECT_TIMER_Create,
    ID_TIMER_00, ID_SCREEN_07,
    { { { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
        { DISPOSE_MODE_NULL, 0, 0, 0 },
      },
      0, 0, 0, 0, 0, 0
    },
    { 0, 0 }
  },
};

/*********************************************************************
*
*       _aSetup
*/
static GUI_CONST_STORAGE APPW_SETUP_ITEM _aSetup[] = {
  { ID_BOX_00,    APPW_SET_PROP_COLOR,        { ARG_V(GUI_WHITE) } },
  { ID_TEXT_00,   APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_00,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_00,   APPW_SET_PROP_FONT,         { ARG_VP(0, acArial_13_Bold_EXT) } },
  { ID_TEXT_00,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_41) } },
  { ID_TEXT_00,   APPW_SET_PROP_BKCOLOR,      { ARG_V(GUI_WHITE) } },
  { ID_TEXT_01,   APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_01,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_01,   APPW_SET_PROP_FONT,         { ARG_VP(0, acArial_13_Bold_EXT) } },
  { ID_TEXT_01,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_33) } },
  { ID_TEXT_01,   APPW_SET_PROP_BKCOLOR,      { ARG_V(GUI_WHITE) } },
  { ID_TEXT_02,   APPW_SET_PROP_COLOR,        { ARG_V(GUI_BLACK) } },
  { ID_TEXT_02,   APPW_SET_PROP_ALIGNTEXT,    { ARG_V(GUI_ALIGN_HCENTER | GUI_ALIGN_VCENTER),
                                                ARG_V(0),
                                                ARG_V(0) } },
  { ID_TEXT_02,   APPW_SET_PROP_FONT,         { ARG_VP(0, acArial_13_Bold_EXT) } },
  { ID_TEXT_02,   APPW_SET_PROP_TEXTID,       { ARG_V(ID_RTEXT_42) } },
  { ID_TEXT_02,   APPW_SET_PROP_BKCOLOR,      { ARG_V(GUI_WHITE) } },
  { ID_TIMER_00,  APPW_SET_PROP_PERIOD,       { ARG_V(500) } },
  { ID_TIMER_00,  APPW_SET_PROP_AUTORESTART,  { ARG_V(0) } },
};

/*********************************************************************
*
*       _aAction
*/
static GUI_CONST_STORAGE APPW_ACTION_ITEM _aAction[] = {
  { ID_SCREEN_07, APPW_NOTIFICATION_INITDIALOG,     ID_TIMER_00,  APPW_JOB_START,          ID_SCREEN_07__APPW_NOTIFICATION_INITDIALOG,
  },
  { ID_TIMER_00,  APPW_NOTIFICATION_TIMER,          0,            APPW_JOB_SHOWSCREEN,     ID_SCREEN_07__ID_TIMER_00__APPW_NOTIFICATION_TIMER,
    { ARG_V(ID_SCREEN_02),
    }, 0, NULL
  },
};

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
/*********************************************************************
*
*       ID_SCREEN_07_RootInfo
*/
APPW_ROOT_INFO ID_SCREEN_07_RootInfo = {
  ID_SCREEN_07,
  _aCreate, GUI_COUNTOF(_aCreate),
  _aSetup,  GUI_COUNTOF(_aSetup),
  _aAction, GUI_COUNTOF(_aAction),
  cbID_SCREEN_07,
  0
};

/*************************** End of file ****************************/
