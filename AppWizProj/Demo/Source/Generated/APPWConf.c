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
File        : APPWConf.c
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#include "AppWizard.h"
#include "Resource.h"

#ifdef WIN32
  #include "GUIDRV_Win32R.h"
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define XSIZE_PHYS       264
#define YSIZE_PHYS       176
#define COLOR_CONVERSION GUICC_1
#define DISPLAY_DRIVER   GUIDRV_WIN32R
#define NUM_BUFFERS      2
#define _aScrollerList   NULL
#define _NumScrollers    0

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
/*********************************************************************
*
*       _apRootList
*/
static APPW_ROOT_INFO * _apRootList[] = {
  &ID_SCREEN_00_RootInfo,
  &ID_SCREEN_01_RootInfo,
  &ID_SCREEN_02_RootInfo,
  &ID_SCREEN_03_RootInfo,
  &ID_SCREEN_04_RootInfo,
  &ID_SCREEN_05_RootInfo,
  &ID_SCREEN_06_RootInfo,
  &ID_SCREEN_07_RootInfo,
};

/*********************************************************************
*
*       _NumScreens
*/
static unsigned _NumScreens = GUI_COUNTOF(_apRootList);

/*********************************************************************
*
*       _aVarList
*/
static APPW_VAR_OBJECT _aVarList[] = {
  { ID_VAR_00, 0, 0, NULL },
  { ID_VAR_01, 0, 0, NULL },
};

/*********************************************************************
*
*       _NumVars
*/
static unsigned _NumVars = GUI_COUNTOF(_aVarList);

/*********************************************************************
*
*       ID_DRAW_000
*/
APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_6, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)NULL,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x00000037, 0, ATOM_CONSTANT, 0 },
    { 0x00000107, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_5, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_6,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x00000107, 0, ATOM_CONSTANT, 0 },
    { 0x00000037, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_4, 1) {
  APPW_ID_SET_BKCOLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_5,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_3, 1) {
  APPW_ID_SET_COLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_4,
  { { 0x00ffffff, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_2, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_3,
  { { 0x00000003, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_1, 4) {
  APPW_ID_FILL_RECT,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_2,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x00000107, 0, ATOM_CONSTANT, 0 },
    { 0x00000037, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_000_Item_0, 1) {
  APPW_ID_SET_COLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_1,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

/*********************************************************************
*
*       ID_DRAW_001
*/
APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_001_Item_3, 4) {
  APPW_ID_DRAW_RECT,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)NULL,
  { { 0x0000001d, 0, ATOM_CONSTANT, 0 },
    { 0x00000013, 0, ATOM_CONSTANT, 0 },
    { 0x000000e9, 0, ATOM_CONSTANT, 0 },
    { 0x000000a6, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_001_Item_2, 4) {
  APPW_ID_DRAW_RECT,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_001_Item_3,
  { { 0x000000ee, 0, ATOM_CONSTANT, 0 },
    { 0x00000013, 0, ATOM_CONSTANT, 0 },
    { 0x00000102, 0, ATOM_CONSTANT, 0 },
    { 0x000000a6, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_001_Item_1, 4) {
  APPW_ID_DRAW_RECT,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_001_Item_2,
  { { 0x00000004, 0, ATOM_CONSTANT, 0 },
    { 0x00000013, 0, ATOM_CONSTANT, 0 },
    { 0x00000018, 0, ATOM_CONSTANT, 0 },
    { 0x000000a6, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_001_Item_0, 1) {
  APPW_ID_SET_COLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_001_Item_1,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

/*********************************************************************
*
*       ID_DRAW_002
*/
APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_26, 5) {
  APPW_ID_FILL_ROUNDED_RECT,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)NULL,
  { { 0x00000091, 0, ATOM_CONSTANT, 0 },
    { 0x0000006c, 0, ATOM_CONSTANT, 0 },
    { 0x00000106, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x00000005, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_25, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_26,
  { { 0x000000f0, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000f0, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_24, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_25,
  { { 0x00000007, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_23, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_24,
  { { 0x000000e4, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000e4, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_22, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_23,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x0000004b, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x0000004b, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_21, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_22,
  { { 0x00000006, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_20, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_21,
  { { 0x000000d8, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000d8, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_19, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_20,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x00000041, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x00000041, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_18, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_19,
  { { 0x00000005, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_17, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_18,
  { { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_16, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_17,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x00000037, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x00000037, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_15, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_16,
  { { 0x00000004, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_14, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_15,
  { { 0x000000c0, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000c0, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_13, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_14,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x0000002d, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x0000002d, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_12, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_13,
  { { 0x00000003, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_11, 5) {
  APPW_ID_DRAW_ARC,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_12,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x00000032, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x0002bf20, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_10, 5) {
  APPW_ID_DRAW_ARC,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_11,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x00000028, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x0002bf20, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_9, 5) {
  APPW_ID_DRAW_ARC,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_10,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x0000001e, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x0002bf20, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_8, 5) {
  APPW_ID_DRAW_ARC,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_9,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x0002bf20, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_7, 5) {
  APPW_ID_DRAW_ARC,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_8,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x000000a0, 0, ATOM_CONSTANT, 0 },
    { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x00000000, 0, ATOM_CONSTANT, 0 },
    { 0x0002bf20, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_6, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_7,
  { { 0x000000b4, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000b4, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_5, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_6,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x00000023, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x00000023, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_4, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_5,
  { { 0x00000002, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_3, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_4,
  { { 0x000000a8, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x000000a8, 0, ATOM_CONSTANT, 0 },
    { 0x00000050, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_2, 4) {
  APPW_ID_DRAW_LINE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_3,
  { { 0x0000000a, 0, ATOM_CONSTANT, 0 },
    { 0x00000019, 0, ATOM_CONSTANT, 0 },
    { 0x0000007d, 0, ATOM_CONSTANT, 0 },
    { 0x00000019, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_1, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_2,
  { { 0x00000001, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_002_Item_0, 1) {
  APPW_ID_SET_COLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_1,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

/*********************************************************************
*
*       ID_DRAW_003
*/
APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_10, 4) {
  APPW_ID_DRAW_ELLIPSE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)NULL,
  { { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 },
    { 0x0000000f, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_9, 4) {
  APPW_ID_DRAW_ELLIPSE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_10,
  { { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x0000001e, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_8, 4) {
  APPW_ID_DRAW_ELLIPSE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_9,
  { { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x00000028, 0, ATOM_CONSTANT, 0 },
    { 0x00000019, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_7, 4) {
  APPW_ID_DRAW_ELLIPSE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_8,
  { { 0x000000cc, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x00000032, 0, ATOM_CONSTANT, 0 },
    { 0x0000001e, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_6, 3) {
  APPW_ID_DRAW_CIRCLE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_7,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x0000000a, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_5, 3) {
  APPW_ID_DRAW_CIRCLE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_6,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x0000000f, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_4, 3) {
  APPW_ID_DRAW_CIRCLE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_5,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x00000014, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_3, 3) {
  APPW_ID_DRAW_CIRCLE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_4,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x00000019, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_2, 3) {
  APPW_ID_DRAW_CIRCLE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_3,
  { { 0x00000044, 0, ATOM_CONSTANT, 0 },
    { 0x00000033, 0, ATOM_CONSTANT, 0 },
    { 0x0000001e, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_1, 1) {
  APPW_ID_SET_PENSIZE,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_2,
  { { 0x00000002, 0, ATOM_CONSTANT, 0 }
  }
};

APPW_DRAWING_ITEM_DEF_X(_aID_DRAW_003_Item_0, 1) {
  APPW_ID_SET_COLOR,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_1,
  { { 0x00000000, 0, ATOM_CONSTANT, 0 }
  }
};

/*********************************************************************
*
*       _appDrawing
*/
static GUI_CONST_STORAGE APPW_DRAWING_ITEM * _appDrawing[] = {
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_000_Item_0,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_001_Item_0,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_002_Item_0,
  (GUI_CONST_STORAGE APPW_DRAWING_ITEM *)&_aID_DRAW_003_Item_0
};

/*********************************************************************
*
*       _NumDrawings
*/
static unsigned _NumDrawings = GUI_COUNTOF(_appDrawing);

/*********************************************************************
*
*       Multibuffering
*/
static U8 _MultibufEnable = 1;

/*********************************************************************
*
*       _ShowMissingCharacters
*/
static U8 _ShowMissingCharacters = 0;

/*********************************************************************
*
*       _apLang
*/
static GUI_CONST_STORAGE char * _apLang[] = {
  (GUI_CONST_STORAGE char *)acAPPW_Language_0,
};

/*********************************************************************
*
*       _TextInit
*/
static GUI_CONST_STORAGE APPW_TEXT_INIT _TextInit = {
  _apLang,
  GUI_COUNTOF(_apLang),
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
/*********************************************************************
*
*       _InitText
*/
static void _InitText(void) {
  APPW_TextInitMem(&_TextInit);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       APPW_X_Setup
*/
void APPW_X_Setup(void) {
  APPW_SetpfInitText(_InitText);
  APPW_X_FS_Init();
  APPW_MULTIBUF_Enable(_MultibufEnable);
  APPW_SetData(_apRootList, _NumScreens, _aVarList, _NumVars, _aScrollerList, _NumScrollers, (APPW_DRAWING_ITEM **)_appDrawing, _NumDrawings);
  APPW_SetSupportScroller(0);
  GUI_ShowMissingCharacters(_ShowMissingCharacters);
}

/*********************************************************************
*
*       APPW_X_Config
*/
#ifdef WIN32
void APPW_X_Config(void) {
  GUI_MULTIBUF_Config(NUM_BUFFERS);
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS);
  }
}
#endif

/*************************** End of file ****************************/
