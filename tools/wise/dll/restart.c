//****************************************************************************
// File: restart.c
//
// Purpose: example DLL file to interface with Wise Installation System
//
// Functions: LibMain, RestartWindows
//
//
// Programmer:  John McMillan
//
//****************************************************************************

#include <windows.h>
#include "wisedll.h"

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
// Function: RestartWindows
//
// Purpose: Exit Windows and Restart
//
// Parameters: 
// LPDLLCALLPARAMS lpDllParams; Parameters from Wise
//
// If lpDllParams->lpszParam[0] == 'Y' restart Windows
// otherwise return to DOS
//
// Returns: return value from ExitWindows
//
// Comments:
//
// History:  Date       Author        Reason
//
//****************************************************************************

BOOL CALLBACK __export RestartWindows(LPDLLCALLPARAMS lpDllParams)
{
   DWORD dwFlag;
   
   if (lpDllParams->lpszParam[0] == 'Y') dwFlag = EW_RESTARTWINDOWS;
   else dwFlag = 0;
   return ExitWindows(dwFlag,0);
}
