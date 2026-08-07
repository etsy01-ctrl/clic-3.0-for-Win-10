// ------------------------------------
// DFONTS.H
// Prototipus de les classes de dialegs
// ------------------------------------

#ifndef NOEDIT

#ifndef __DFONTSH_H
#define __DFONTSH_H

#ifndef __ZONESH_H
#include "zones.h"
#endif

#ifndef __WCLH_H
#include "wcl.h"
#endif

/*
typedef struct tagFONTENUM
{ LOGFONT lf;
  struct tagFONTENUM *next;
} FONTENUM;
*/


typedef struct tagFONTLIST
{ HGLOBAL hg;
  DWORD currSize;
  unsigned numFonts;
} FONTLIST;

#define FONTBLOCK 40


#define MAXFW 64
#define MAXFH 64

class DFonts : public ModalDialog
{ public:
	 DFonts(HWND hOwner, BOOL iTextAct, LOGFONT *iLogFont, Colors *iColors, COLORREF *icUsuari, COLORREF *icError);
	 ~DFonts();
	 LOGFONT *getLFont() {return logF;}
	 Colors  *getColors() {return colors;}
	 COLORREF *getCUsuari() {return cUsuari;}
	 COLORREF *getCError() {return cError;}

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 void resetDlg();
	 BOOL initDlg();
	 void doUpdateFont();
	 static int CALLBACK _export FontFunc(LOGFONT FAR*, NEWTEXTMETRIC FAR*,
													  int, LPARAM);
  private:
	 void     showMostra(DRAWITEMSTRUCT *dris, RECT *r, char *txt, HBRUSH hFonsBrush,
								COLORREF colorText, COLORREF colorOmbra, BOOL ombra);
	 LOGFONT  logF[4];
	 Colors   colors[4];
	 HBRUSH   hBackBrush;
	 HBRUSH   hTextBrush;
	 HBRUSH   hShadowBrush;
	 HBRUSH   hUsuariBrush[2];
	 HBRUSH   hErrorBrush[2];
	 HFONT    hFont;
	 //FONTENUM *lf;
	 FONTLIST fl;
	 BOOL     update;
	 COLORREF cUsuari[2];
	 COLORREF cError[2];
	 BOOL     textAct;

	 virtual LPCSTR getDialogName();
	 static char* textMostra;
};
#endif

#endif