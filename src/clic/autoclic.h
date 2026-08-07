/* ----------------------------
	AUTOCLIC.H
	Declarations for DLL's
	---------------------------- */

#ifndef __AUTOCLIC_H
#define __AUTOCLIC_H

#ifndef __WINDOWS_H
#include <windows.h>
#endif

#define CLIC_DLL_VERSION 0x100

#if defined( __cplusplus )
extern "C" {
#endif  /* __cplusplus */

#ifndef __CLICH_H
enum PlayModes {NONE, PUZZLE, SOPA, ASSOCIA, CREUATS};
enum PuzModes  {INTERC, FORAT, DOBLE, MEMORY};
enum AssModes  {NORMAL, ESPECIAL, EXPLORA, INFO, IDENTIFICA, ESCRIU};
enum GraPositions {AB, BA, AUB, BUA};
#endif

typedef struct tagActData
{  int   clicVersion;
	LPINT actMode;
	LPINT actType;
	LPINT arrayPos;
	LPINT rows;
	LPINT columns;
	LPINT rows2;
	LPINT columns2;
	LPINT assignAss;
	LPINT ass;
	LPSTR initMessage;
	LPSTR endMessage;
	LPSTR tempFName[3];
	BOOL  useTempFName[3];
} ACTDATA;

typedef ACTDATA FAR* LPACTDATA;

#define DLL_OPTIONS_SIZE 250

BOOL FAR PASCAL RemakeAct(LPSTR fileNameAct, LPACTDATA lpAct, LPSTR lpOps);
int FAR PASCAL GetClicDllVer();
BOOL FAR PASCAL ConfigureDllOptions(HWND hOwner, LPACTDATA lpAct, LPSTR lpOps);
BOOL FAR PASCAL IsValidAct(HWND hOwner, LPACTDATA lpAct, BOOL showMsg);

#if defined( __cplusplus )
}
#endif  /* __cplusplus */

#endif  /* __AUTOCLIC_H__ */

