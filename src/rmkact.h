/* ----------------------------
	AUTOCLIC.H
	Declarations for DLL's
	---------------------------- */

#ifndef __AUTOCLIC_H__
#define __AUTOCLIC_H__

#include <windows.h>

typedef struct tagActData
{  char initMessage[100];
	char endMessage[100];
	LPSTR tempFName[3];
	BOOL  useTempFName[3];
} ACTDATA;

typedef ACTDATA FAR* LPACTDATA;

#define DLL_OPTIONS_SIZE 40

BOOL FAR PASCAL RemakeAct(LPSTR fileNameAct, LPACTDATA lpAct, LPSTR lpOps);
int FAR PASCAL GetClicDllVer();
BOOL FAR PASCAL ConfigureDllOptions(HWND hOwner, LPSTR lpOps);

#endif
