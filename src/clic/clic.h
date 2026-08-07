// -----------------------------------
// CLIC.H
// Parametres comuns
// -----------------------------------

#ifndef __CLICH_H
#define __CLICH_H

#ifndef __WINDOWS_H
  #include "w.h"
#endif

#define CLICVER 120
#ifndef NOEDIT
 #define CLICSTR "CLIC120"
#else
 #define CLICSTR "CLICRT120"
#endif

#ifndef NOEDIT
#define CLICVSTR "Ver. 3.0"
//#define CLICVSTR "BETA 3.0b12: "__DATE__
#else
#define CLICVSTR "Ver. 3.0 RUNTIME"
#endif

#define MAX_MESS_LEN 200
#define MAX_FULL_FILENAME 100
#define MAX_DESC 200

#define MAXCW 20
#define MAXCH 20
#define MAXGW 21
#define MAXGH 10

enum PlayModes {NONE, PUZZLE, SOPA, ASSOCIA, CREUATS, TEXTACT};
enum PuzModes  {INTERC, FORAT, DOBLE, MEMORY};
enum AssModes  {NORMAL, ESPECIAL, EXPLORA, INFO, IDENTIFICA, ESCRIU};
enum TxActModes {FORATS, COMPTEXT, IDLLETRES, IDPARAULES, BPARAULES, BPARAGRAFS};
enum GraPositions {AB, BA, AUB, BUA};

#define DEF_TXTCASW 140
#define DEF_TXTCASH 100
#define DEF_CICLES   30
#define NEGRE 160
#define COMODI '*'
#define SEPARADOR ';'
#define ENDDESC 0X87

enum WM_USERIDS {WM_PLAYW=WM_USER, WM_HELPW, WM_CBNEXT, WM_CBPREV, WM_CBGETFOCUS, WM_NOUINTENT};

enum USERMESSAGES {PLAYW_RESIZE, PLAYW_ENDGAME, PLAYW_INTENT, PLAYW_ENCERTS, PLAYW_WAITMEIN,
			PLAYW_SETMESS, PLAYW_SETMESSPLAY, HELPW_ITEMSEL, HELPW_MOUSE};

#define MWSTYLE WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX
#define NMSTYLE WS_POPUP|WS_BORDER

#define WDIGIT 13
#define HDIGIT 23

#define MARGE 6
#define BOTOW 63
#define BOTOW2 30
#define BOTOH 39

#define DIGITSW 39
#define DIGITSH 23

#define CTRLH 39   // era 41
#define CTRLW 450

#define CLW (3*MARGE+BOTOW+CTRLW)
#define CLH (6*BOTOH+7*MARGE)

#define PLWW CTRLW
#define PLWH (CLH-CTRLH-3*MARGE)

extern int passos, passos0;
extern int gAjVis, encertsNow;
extern BOOL quick, quick0, arrossega, arrossega0, sonor0, autoCursor, cursorGran;
extern BOOL doReportFile, drf0, exitDirect;
extern BOOL helponhelp, menus;
extern BOOL allowNewUsr, allowNewGrp, showUserReports;
extern int caretVis, caretX, caretY, caretW, caretH, autoCursorDelay;
extern LOGFONT logFont0, logFont1;
extern int gruixfil;
extern HCURSOR hGenCursor;

#endif

