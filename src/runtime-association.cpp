#include "runtime-association.h"
#include "runtime-i18n.h"
#include "rcomu/clicres.h"
#include <shellapi.h>
#include <stdio.h>
#include <string.h>

static LONG writeAssociationString(HKEY root, const char *path,
                                  const char *name, const char *value)
{
  HKEY key;
  DWORD disposition;
  LONG err=RegCreateKeyEx(root, path, 0, NULL, REG_OPTION_NON_VOLATILE,
                          KEY_SET_VALUE, NULL, &key, &disposition);
  if(err!=ERROR_SUCCESS) return err;
  err=RegSetValueEx(key, name, 0, REG_SZ, (const BYTE*)value, strlen(value)+1);
  RegCloseKey(key);
  return err;
}

LONG registerPccAssociation(HKEY root, const char *exePath)
{
  char command[1152], icon[1100];
  if(!exePath || !*exePath || strlen(exePath)>510) return ERROR_INVALID_PARAMETER;
  sprintf(command, "\"%s\" \"%%1\"", exePath);
  sprintf(icon, "\"%s\",0", exePath);
  struct Entry {const char *path; const char *name; const char *value;} entries[]={
    {"Software\\Classes\\ClicPlayer.Package", "", tr(PT_PACKAGE_TYPE)},
    {"Software\\Classes\\ClicPlayer.Package\\DefaultIcon", "", icon},
    {"Software\\Classes\\ClicPlayer.Package\\shell\\open\\command", "", command},
    {"Software\\Classes\\ClicPlayer.Package\\Application", "ApplicationName", "Clic Player"},
    {"Software\\Classes\\Applications\\ClicPlayer.exe", "FriendlyAppName", "Clic Player"},
    {"Software\\Classes\\Applications\\ClicPlayer.exe\\shell\\open\\command", "", command},
    {"Software\\Classes\\Applications\\ClicPlayer.exe\\SupportedTypes", ".pcc", ""},
    {"Software\\Classes\\.pcc\\OpenWithProgids", "ClicPlayer.Package", ""},
    {"Software\\ClicPlayer\\Capabilities", "ApplicationName", "Clic Player"},
    {"Software\\ClicPlayer\\Capabilities", "ApplicationDescription", tr(PT_APP_DESCRIPTION)},
    {"Software\\ClicPlayer\\Capabilities\\FileAssociations", ".pcc", "ClicPlayer.Package"},
    {"Software\\RegisteredApplications", "ClicPlayer", "Software\\ClicPlayer\\Capabilities"}
  };
  for(unsigned i=0; i<sizeof(entries)/sizeof(entries[0]); i++)
  {
    LONG err=writeAssociationString(root, entries[i].path, entries[i].name, entries[i].value);
    if(err!=ERROR_SUCCESS) return err;
  }
  return ERROR_SUCCESS;
}

BOOL associatePccFiles(HWND owner)
{
  char path[512], text[240]; GetModuleFileName(NULL,path,sizeof(path));
  LONG err=registerPccAssociation(HKEY_CURRENT_USER,path);
  if(err!=ERROR_SUCCESS)
  {
    sprintf(text,tr(PT_ASSOC_ERROR_FMT),err);
    MessageBox(owner,text,tr(PT_ASSOC_TITLE),MB_OK|MB_ICONEXCLAMATION);
    return FALSE;
  }
  HINSTANCE shell=LoadLibrary("shell32.dll");
  if(shell)
  {
    typedef void (WINAPI *ChangeNotify)(LONG,UINT,const void*,const void*);
    ChangeNotify notify=(ChangeNotify)GetProcAddress(shell,"SHChangeNotify");
    if(notify) notify(0x08000000L,0,NULL,NULL);
    FreeLibrary(shell);
  }
  if((int)ShellExecute(owner,"open","ms-settings:defaultapps",NULL,NULL,SW_SHOWNORMAL)<=32)
    MessageBox(owner,tr(PT_ASSOC_DONE_TEXT),tr(PT_ASSOC_DONE_TITLE),MB_OK|MB_ICONINFORMATION);
  return TRUE;
}