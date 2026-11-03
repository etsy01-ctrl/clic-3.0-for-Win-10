// -----------------------------------
// ZONES.H
// Prototipus de les classes:
//  - Zona
//  - Contenidor
//  - DigitBox
//  - Fil
// -----------------------------------

#ifndef __ZONESH_H
#define __ZONESH_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif

#define NTRAMES 4
enum BASE_ZONES {GRA, GRB, MSB, POP, GENERICA, PLAYWIN, NMAXZONES};
enum EXTRA_COLORS {COL_USR, COL_ERR, NEXTRACOLORS};
enum FLAGS_ZONA {BASE=0x0007, HID=0x0008, MARK=0x0010, ALT=0x0020};

struct Colors
{
  COLORREF backColor;
  COLORREF textColor;
  COLORREF shadowColor;
  UINT shadow;
};

struct BaseCont
{ HFONT hf;
  Colors c;
  int shadowDeep;
  HBRUSH hBackBrush;
  HBRUSH hHidBrush;
  HBRUSH hMarkBrush;
  LOGFONT lfb;
  int trama;
};

class Zona
{
  friend class ModalControl;

  public:
	Zona();
	~Zona() {destroy();}

	BOOL create(HWND hOwner, RECT *rpos, BOOL iHid=FALSE, BOOL iMark=FALSE, BASE_ZONES base=GENERICA);
	void destroy() {invr(TRUE); hWnd=0;}
	BOOL isCreated() {return (hWnd!=0);}

	virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0);

	BOOL isIn(RECT *rect);
	BOOL isIn(POINT p);
	BOOL equalRect(RECT *rcp);
	BOOL isSubRect(RECT *rect);
	void hide(BOOL mode) {if(getFlag(HID)!=mode) {setFlag(HID, mode); invr(TRUE);}}
	void marked(BOOL mode) {if(getFlag(MARK)!=mode) {setFlag(MARK, mode); invr(TRUE);}}
	void altCont(BOOL mode) {if(getFlag(ALT)!=mode) {setFlag(ALT, mode); invr(TRUE);}}
	BOOL isHide() {return getFlag(HID);}
	BOOL isMarked() {return getFlag(MARK);}
	BOOL isAltCont() {return getFlag(ALT);}
	void move(RECT *rpos);
	void move(int dx, int dy);
	void move(POINT newpos) {move(newpos.x-pos.left, newpos.y-pos.top);}
	BOOL locateCursorHere();
	BASE_ZONES getBase() {return (BASE_ZONES)flags&BASE;}
	void setBase(unsigned f) {(flags&=~BASE)|=f;}
	BOOL drawRect(HDC hDC, int offset=0);

	static BOOL setBaseCont(LOGFONT *lf, Colors *c, BASE_ZONES f);
	static BOOL setBaseBrush(BASE_ZONES f, COLORREF c, int trama=-1);
	static BOOL setExtraColors(EXTRA_COLORS c, COLORREF txcolor, COLORREF bkcolor);
	static HBRUSH getBaseBrush(BASE_ZONES f) {return bc[f].hBackBrush;}
	static void clearAllBaseCont();
	static void initAllBaseCont();
	static BOOL reduceFont(BASE_ZONES f, long numerador, long denominador);
	static int fontReduced;
	static BaseCont *getBC(BASE_ZONES f) {return bc+f;}

	RECT *getRect() {return &pos;}
	int getWidth() {return pos.right-pos.left;}
	int getHeight() {return pos.bottom-pos.top;}
	void invr(BOOL fons=FALSE);

	BOOL getFlag(WORD fl) {return ((flags&fl)!=NULL);};
	void setFlag(WORD fl, BOOL status) {if(status) flags|=fl; else flags&=~fl;};

	virtual BOOL isEmpty() {return FALSE;}

	static HBITMAP *tramaBmp;

  protected:
	static  BaseCont *bc;
	static  Colors *extraColors;
	RECT    pos;
	HWND    hWnd;
	WORD    flags;
};

enum CONTINGUTS {RES, TXT, BMP};
enum FLAGS_CONTENIDOR {CAPTURED=0x0040, OWNSCARET=0x0080, STRCHMCI=0x0100,
							  FREEMCI=0x0200, UNICHAR=0x0400, MARC=0x0800, LALIGN=0x1000};

class Contenidor:public Zona
{
  public:
	Contenidor(BASE_ZONES Base=GENERICA);
	~Contenidor() {clear();}

	BOOL getData(HBITMAP hBitmap, RECT *rbmtros=0, POINT *orgBm=0);
	virtual BOOL getData(char *str);
	virtual void clear();
	virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0);
	CONTINGUTS getCont() {return cont;}
	BOOL setCont(CONTINGUTS newcont);
	char getChar() {return ((txt==uch && !getFlag(UNICHAR)) ? 0:*txt);}
	char *getTxt() {return (txt==uch && !getFlag(UNICHAR)) ? NULL:txt;}
	BOOL capture(BOOL op);
	BOOL isCaptured() {return getFlag(CAPTURED);}
	BOOL play();
	BOOL lostCaret();
	virtual BOOL gainCaret();
	BOOL insertChar(UINT kcode);
	BOOL isEmpty() {return (cont==RES || (cont==TXT && (txt==uch && *txt==0)));}

	int  extra;
	static Contenidor* caretOwner;

  protected:
	RECT        rbm;
	int         lRbmX, lRbmY;
	CONTINGUTS  cont;
	int         soCont;
	HBITMAP     hBm, hTxBm;
	POINT       origin;
	int         caretPos;
	char        *txt;
	char        *fnSoCont;
	char        uch[2];
};


class DigitBox : public Zona
{
  public:
	 BOOL create(POINT iniPos, HWND hOwner, BOOL iMark=FALSE, int ival=0, BASE_ZONES base=GENERICA);
	 int incVal(int inc) {return setVal(val+inc);}
	 int setVal(int newval) {val=newval; if(!isHide())invr(); return val;}
	 int getVal() {return val;}
	 virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0);

  protected:
	 static HBITMAP hDigit;
	 int val;
};

class Fil
{
  public:
	Fil();
	~Fil();

	void move(POINT nou, HDC hDC=0, HDC hMemDC=0);
	void init(POINT ini, HWND hParent, HCURSOR hDispCursor, Contenidor *c=0);
	void end(BOOL walk=FALSE);
	void show(HDC hDC=0, HDC hMemDC=0);
	void hide(HDC hDC=0, HDC hMemDC=0);
	BOOL isAct() {return active;}
	BOOL isVis() {return visible;}
	void getPos(POINT &o, POINT &d) {o=origen; d=desti;}
	static BOOL setWidth(int &newWidth);

  protected:
	void display(HDC hOrgDC=0, HDC hMemDC=0);

	HWND  hWnd;
	HCURSOR hCursor, hOldCursor;
	POINT origen, desti, curPosCont;
	BOOL  active, visible;
	Contenidor *cont;
	RECT oldContRect, *currRect;
	HBITMAP hBmCont, hFonsCont;
	int dxCont, dyCont;
	int lxCont, lyCont;
	static HPEN hp;
};

#endif

