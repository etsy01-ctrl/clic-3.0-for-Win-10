//****************************************************************************
// File: prompt.c
//
// Purpose: example DLL file to interface with Wise Installation System
//
// Functions: LibMain, Prompt, CheckType
//
//
// Programmer:  John McMillan
//
//****************************************************************************

#include <windows.h>
#include <string.h>
#include "wisedll.h"
#include "resource.h"

#define MAX_PATH_LEN 128

void GetVariable(LPDLLCALLPARAMS lpDllParams,char *szVariable,char *szValue);
void SetVariable(LPDLLCALLPARAMS lpDllParams,char *szVariable,char *szValue);
BOOL FAR PASCAL PromptDlg(HWND, UINT, WPARAM, LPARAM);

char szPathName[MAX_PATH_LEN]; // Holds the pathname of the install dir
HINSTANCE hDllInst;
char szInstallType[2];  // The type of installation

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
   hDllInst = hInst;
   return(1);
}

//***********************************************************************
// Function: Prompt
//
// Purpose: Prompt for the destination directory and type of install
//
// Parameters: 
// LPDLLCALLPARAMS lpDllParams; Parameters from Wise
//
// Returns: TRUE if user canceled the install
//
// Comments:
//
// History:  Date       Author        Reason
//
//****************************************************************************

BOOL __export CALLBACK Prompt(LPDLLCALLPARAMS lpDllParams)
{
   BOOL bResult;

   lstrcpy(szPathName,lpDllParams->lpszParam);
   szInstallType[1] = '\0';
   if (DialogBox(hDllInst,"PromptDlg",lpDllParams->hWnd,PromptDlg) == IDOK) {
      bResult = FALSE;
      SetVariable(lpDllParams,"WISE",szPathName);
      SetVariable(lpDllParams,"TYPE",szInstallType);
   } else {
      bResult = TRUE;
   }
   return bResult;
}

BOOL __export CALLBACK PromptDlg(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
   switch (message) {
    case WM_INITDIALOG:
      SetDlgItemText(hDlg,IDC_PATHNAME,szPathName);
      CheckRadioButton(hDlg,IDC_COMPLETE_INSTALL,IDC_MINIMUM_INSTALL,
         IDC_COMPLETE_INSTALL);
      return (TRUE);
    case WM_COMMAND:
      if (wParam == IDOK || wParam == IDCANCEL) {
         GetDlgItemText(hDlg,IDC_PATHNAME,szPathName,MAX_PATH_LEN);
         if (IsDlgButtonChecked(hDlg,IDC_COMPLETE_INSTALL)) *szInstallType = 'C';
         if (IsDlgButtonChecked(hDlg,IDC_NETWORK_INSTALL)) *szInstallType = 'N';
         if (IsDlgButtonChecked(hDlg,IDC_MINIMUM_INSTALL)) *szInstallType = 'M';
         EndDialog(hDlg, wParam);
         return (TRUE);
      }
      break;
   }
   return (FALSE);
}

BOOL __export CALLBACK CheckType(LPDLLCALLPARAMS lpDllParams)
{
   BOOL bResult;
   char szValue[256];
   
   GetVariable(lpDllParams,"TYPE",szValue);
   if (szValue[0] == lpDllParams->lpszParam[0]) bResult = TRUE;
   else bResult = FALSE;
   return bResult;
}

// GetVariable: Returns the value of a variable.
//
// lpDllParams  Parameter structure passed from Wise Installation
// szVariable   Name of the variable (without %'s) to get value for
// szValue      String that will hold the variables value

void GetVariable(LPDLLCALLPARAMS lpDllParams,char *szVariable,char *szValue)
{
   WORD i;
   char szVar[32];

   *szVar = '%';
   lstrcpy(&szVar[1],szVariable);
   lstrcat(szVar,"%");
   for (i = 0 ; (i < lpDllParams->wCurrReps) &&
      (lstrcmp(&lpDllParams->lpszRepName[i * lpDllParams->wRepNameWidth],szVar) != 0) ; i++) ;
   if (i < lpDllParams->wCurrReps) {
      lstrcpy(szValue,&lpDllParams->lpszRepStr[i * lpDllParams->wRepStrWidth]);
   } else *szValue = '\0';
}

// SetVariable: Sets/Creates a variable.
//
// lpDllParams  Parameter structure passed from Wise Installation
// szVariable   Name of the variable (without %'s) to set value for
// szValue      String that contains the variables new value

void SetVariable(LPDLLCALLPARAMS lpDllParams,char *szVariable,char *szValue)
{
   WORD i;
   char szVar[32];

   *szVar = '%';
   lstrcpy(&szVar[1],szVariable);
   lstrcat(szVar,"%");
   for (i = 0 ; (i < lpDllParams->wCurrReps) &&
      (lstrcmp(&lpDllParams->lpszRepName[i * lpDllParams->wRepNameWidth],szVar) != 0) ; i++) ;
   if (i >= lpDllParams->wCurrReps) {
      if (i >= lpDllParams->wMaxReplaces) return; // Too many variables
      lstrcpy(&lpDllParams->lpszRepName[i * lpDllParams->wRepNameWidth],szVar);
      lpDllParams->wCurrReps++;
   }
   lstrcpy(&lpDllParams->lpszRepStr[i * lpDllParams->wRepStrWidth],szValue);
}
