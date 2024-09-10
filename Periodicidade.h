/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PERIO_NSAM                       1
#define  PERIO_NSAM_PERIODICIDADE         2       /* control type: ring, callback function: (none) */
#define  PERIO_NSAM_LABEL                 3       /* control type: textMsg, callback function: (none) */
#define  PERIO_NSAM_COMMANDBUTTON         4       /* control type: command, callback function: DefinirPeriodoNegSample */
#define  PERIO_NSAM_LABEL_2               5       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DefinirPeriodoNegSample(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
