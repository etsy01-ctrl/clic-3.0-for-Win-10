// -----------------------------------
// CTEXT.H
// Prototipus de les classes:
//  - TxContenidor
// -----------------------------------

#ifndef __CTEXTH_H
#define __CTEXTH_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

#ifndef __FLOATCNTH_H
#include "floatcnt.h"
#endif

#define TXCBASELEN 2000
#define TXCMAXLEN  32000
#define TXCBASELIN 50
#define TXCMAXLIN  2000
#define CHBLOCK 0x8E
#define CHINC 0x8F
#define CHINCS "\x8F"
#define SEP 0x90
#define TAB 0x09
#define BOTO_CORREGIR 969

struct txLine
{ int pChar;
  int pLastChar;
  int height;
};

struct Cntptr
{ Contenidor *c;
  struct Cntptr *next;
  char *txt;
  BOOL b, la;
  Cntptr() {c=0; next=0; txt=0; b=FALSE; la=TRUE;}
  ~Cntptr() {if(c) delete c; if(txt) delete txt;}
};

struct FloatInc
{ int beg;
  int end;
  int srcPos;
  struct FloatInc *next;
  FloatInc() {beg=0; end=0; srcPos=0; next=NULL;}
};

enum TIPUS_INCOGNITA {ESCRITA, LLISTA};
enum INFO_MODES {NO_INFO, ALWAYS, ON_ERROR, ON_DEMAND};
enum ESTAT_INCOGNITA {NO_AVALUADA, RESOLTA, EN_ERROR};
enum FLAGS_INFO {INFO_LA=1, INFO_ONLYPLAY=2};

class Incognita
{
  public:
  Incognita();
  ~Incognita();

  Incognita *next;
  TIPUS_INCOGNITA tipus;
  int p;
  int farciment;
  char chFarciment;
  int maxResp;
  char *ok;
  char *opcions;
  char *iniText;
  int infoMode;
  int flagsInc;
  int infoDelay;
  int infoMax;
  int infoWidth;
  int infoHeight;
  char *infoTxt;
  HWND hCBList;
  int  cbwidth;
  int  cbheight;
  ESTAT_INCOGNITA estat;
};

enum TX_ATTR {TX_INC=1, TX_USR=2, TX_ERR=4, TX_FARCIMENT=8, TX_HIDINC=16,
				  TX_FINCNOW=32, TX_ANCHORFINC=64};
enum NAV_MODE {NAV_FREE, NAV_INC, NAV_FINC, NO_NAV};

#define ID_TXSCROLL 666
class TxContenidor:public Contenidor
{
  friend DEditAct;

  public:
	TxContenidor(BASE_ZONES Base=GENERICA);
	~TxContenidor();
	BOOL create(HWND hOwner, RECT *rpos, BOOL iHid=FALSE, BOOL iMark=FALSE, BASE_ZONES base=GENERICA);

	BOOL getActData(ActFile *afgd, BOOL onlyShow=FALSE);
	virtual void clear();
	void updateX(HDC hDC, RECT *updRect=0, HDC hMemDC=0, BOOL toPrint=FALSE);
	virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0) {updateX(hDC, updRect, hMemDC);}
	virtual BOOL gainCaret();
	char *getSource();
	char *getCleanSource();
	void getFocus(BOOL status);
	BOOL getEncerts(int &nEncerts);
	void endGame();
	BOOL clearAllIncs();
	int  countIncs();
	BOOL insertCnt();
	BOOL insertIncognita(BOOL ed=TRUE);
	BOOL mKAutoIncs(BOOL expr, BOOL onlyWords, char *txInc);
	BOOL processScroll(UINT wParam, LONG lParam);
	BOOL processKeyboard(UINT msg, WPARAM wParam);
	BOOL processMouse(UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL processCBMsg(UINT msg, WPARAM wParam);
	BOOL newTabSpc(int nt);
	int  avaluaActivitat();
	BOOL setLines(int fromLine, BOOL inv=FALSE, BOOL absolute=FALSE);
	BOOL calcFont();
	BOOL scramble(int vegades, BOOL updating);
	BOOL doHelp();
	BOOL pasteClipboard();
	BOOL copyClipboard();
	BOOL cutClipboard();
	int  getTotalHeight();
	BOOL noMemAvail;
	BOOL edit;
	BOOL playing;
	BOOL waiting;
	BOOL ok;
	HWND btCorregir;
	BOOL deBoto;
	int  btHeight;
	int  txMaxLen;
	Fil fil;
	BOOL filOn;

  protected:
	HWND scrBar;
	ActFile *af;
	struct Cntptr *cnt;
	struct FloatInc *finc;
	Incognita *inc;
	Incognita *incNow;
	struct FloatInc *fincNow;
	struct FloatInc *anchorFinc;
	char *incNowText;
	int  txLen;
	int  scrWidth;
	HGLOBAL hgTxNow, hgTxAttr, hgLin;
	char far *txNow;
	char far *txAttr;
	txLine far *lin;
	int nLines;
	int nMaxLines;
	int firstLine;
	int widthTable[256];
	int lineHeight;
	int leftMargin, rightMargin;
	int memLocked;
	BOOL selActiva;
	int selStart, selEnd;
	NAV_MODE nav;
	FloatCnt popup;
	int tabLen;
	BOOL incFull;

	BOOL lockMem();
	BOOL unlockMem();
	BOOL growMem(int howManyMore=0);
	BOOL growLin();
	int  locateLineFromChar(int fromChar);
	int  locateLineFromY(int y);
	BOOL setCaretAtPos(BOOL makeVisible=TRUE);
	BOOL getPosFromPoint(POINT pt, BOOL halfChars=TRUE);
	BOOL getChPos(int c, int &l, int &x, int &y);
	struct Cntptr *getContFromChar(int c);
	Incognita *getIncFromChar(int c);
	int  getLineY(int l);
	BOOL insertChar(char ch);
	BOOL insertString(LPSTR str);
	BOOL deleteChar(BOOL prev);
	void getLineRect(int l, RECT *r);
	BOOL setScrollAt(int newFirstLine, BOOL sc=TRUE);
	BOOL cancelSel(BOOL erasing=FALSE);
	BOOL adjustSel(int cp0, BOOL drag);
	BOOL invSel();
	BOOL deleteCnt(int n);
	BOOL editCnt(struct Cntptr *cntz);
	BOOL editIncognita(Incognita *incp);
	BOOL goToInc(int from, BOOL fwd);
	BOOL goToIncEv(int from, BOOL fwd);
	BOOL activateInc(Incognita *newIncNow, BOOL changeFocus=TRUE);
	BOOL getCurIncLen(Incognita *incx, int &lTotal, int &lEfectiu);
	int  getStrWidth(char *str);
	BOOL avaluaIncognita(Incognita *in);
	int  getNextIncPos(Incognita *in);
	BOOL getIncTextNow(Incognita *in, char *&destBuff);
	BOOL marcaBloc(int p, char atr);
	BOOL activateFinc(struct FloatInc *f);
	BOOL markFinc(struct FloatInc *f, char on, char off);
	BOOL swapFincs(struct FloatInc *fi0, struct FloatInc *fi1);
	BOOL scrambleBetween(int v0, int nv);
	struct FloatInc *getFIncFromChar(int p);
	BOOL goToFinc(int from, BOOL fwd);

	BOOL acceptInput;

	static LRESULT CALLBACK _export CBSubClassProc(HWND, UINT, WPARAM, LPARAM);
	static FARPROC lpfnCBSubClassProc;
	static WNDPROC lpfnOldCBClassProc;
	static int farProcCount;
};

#endif

