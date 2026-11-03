// -----------------------------------
// PLAYWIN.H
// Prototipus de la classe PlayWindow
// -----------------------------------

#ifndef __PLAYWINH_H
#define __PLAYWINH_H

#ifndef __WINDOWS_H
#include "w.h"
#endif

#ifndef __WCLH_H
#include "wcl.h"
#endif

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

#ifndef __CASELLAH_H
#include "casella.h"
#endif

#ifndef __CTEXTH_H
#include "ctext.h"
#endif

enum curAltModes {NOAUTOCURSOR, AUTOAB, AUTOA};

class PlayWindow : public ActFile, public Window
{
  public:
	 PlayWindow();
	 BOOL destroy();
	 BOOL endGame();
	 BOOL resetGame();
	 BOOL initAct();
	 BOOL initGame(int botonsVisibles);
	 BOOL showHelp();
	 BOOL processKeyboard(UINT msg, WPARAM wParam);
	 BOOL nextCursorPos();
	 BOOL changeCursorMode();
	 BOOL nouIntent();
	 BOOL itsTime();
	 int  getExtraHeight();

	 int nGra;
	 RECT wRect, clRect;
	 BOOL playing;
	 Zona zonaBase;
	 TxContenidor *txc;
	 HBITMAP hBmpFons;
	 int bmpfW, bmpfH, bmpfX, bmpfY;
	 static HCURSOR hFilCursor;

	 BOOL processPaint(HDC hDC0=NULL, HDC hMemDC0=NULL, LPRECT r0=NULL, BOOL toPrint=FALSE);

  protected:
	 LRESULT dispatch(UINT, WPARAM, LPARAM, int);
	 BOOL registerClass();
	 BOOL createWindow();

  private:
	 Fil     fil;
	 Graella gra[2];
	 Casella *casX, *casH, *casZ;
	 BOOL    filOn, keybOn;
	 //HBRUSH  hFonsBrush;
	 HBITMAP hBmB[3], hBmBX[3], hPortadaBm;
	 HPALETTE hPortadaPal;
	 RECT    grr[2];
	 int     nCw[2], nCh[2];
	 int     rw[2], rh[2];
	 int     bmbW[3], bmbH[3];
	 int     keybDir;
	 BOOL    *cTags;
	 char    **txtB[3];
	 int     curCurCas, curCurGra, curAltMode;

	 BOOL processUserMess(WPARAM wParam, LPARAM lParam);
	 BOOL processMouse(UINT, WPARAM, LPARAM);
	 BOOL ajustaMides(int mode, int botonsVisibles);
	 BOOL creaGraella(int ng, int cw, int ch,
			  BOOL iDel=TRUE, BOOL iHid=FALSE, BOOL iMark=FALSE);
	 BOOL ompleGraella(int ng, int iBmB=-1, int iTxtB=-1,
				int firstCas=0, int firstId=0, int gdCw=0, int gdCh=0);
	 BOOL relaciona(POINT p0, POINT p1, char c=0);
	 BOOL testEndGame(int &encerts=stp);
	 BOOL resituaCursor(int vkCode, POINT *p);
	 char *getChainInTag(int it, int pos);
	 BOOL changeKeybDir(int newDir);

	 int MaxW;
	 int MaxH;

};

extern BOOL waitToPass, forcedPass;

#endif
