//****************************************************************************
// File: checkvga.c
//
// Purpose: example DLL file to interface with Wise Installation System
//
// Functions: LibMain, Check256Colors
//
//
// Programmer:  John McMillan
//
//****************************************************************************

#include <windows.h>
#include "wisedll.h"

char szErrorStr[] = "Your system does not support 256 colors, install anyway?";

//***********************************************************************
// Function: LibMain
//
// Purpose: C function called from DLL entry point.
//
// Parameters: HINSTANCE hInst;   DLL instance handle
//             WORD wSeg;         DLL data segment selector
//             WORD cbHeapSize;   DLL initial heap size in bytes
//             LPSTR lpszCmdLine; DLL command line
//
// Returns: 1 is success, 0 fail DLL load
//
// Comments:
//
// History:  Date       Author        Reason
//
//****************************************************************************

int CALLBACK LibMain(HINSTANCE hInst, WORD wDataSeg, WORD cbHeapSize, LPSTR lpszCmdLine)
{
  return(1);
}

//***********************************************************************
// Function: Check256Colors
//
// Purpose: Check for support of at least 256 color display
//
// Parameters: 
// LPDLLCALLPARAMS lpDllParams; Parameters from Wise
//
// Returns: TRUE if 256 colors is not supported on this machine
//
// Comments:
//
// History:  Date       Author        Reason
//
//****************************************************************************

BOOL CALLBACK __export Check256Colors(LPDLLCALLPARAMS lpDllParams)
{
   BOOL bNo256;
   HDC hDC;
   int nColors,nPlanes,nBitsPixel;

   bNo256 = TRUE;
   if ((hDC = GetDC(lpDllParams->hWnd)) != NULL) {
      nPlanes = GetDeviceCaps(hDC,PLANES);
      nBitsPixel = GetDeviceCaps(hDC,BITSPIXEL);
      nColors = 1 << (nPlanes * nBitsPixel);
      if (nColors >= 256) bNo256 = FALSE;
      ReleaseDC(lpDllParams->hWnd,hDC);
   }
   if (bNo256) {
      if (MessageBox(lpDllParams->hWnd,szErrorStr,"Error",MB_APPLMODAL|MB_OKCANCEL) == IDOK) {
         bNo256 = FALSE;
      }
   }
   return bNo256; // Return TRUE if no 256 color support
}
