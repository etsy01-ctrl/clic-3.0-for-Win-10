// -----------------------------------
// WCL.H
// Prototipus de classes
// i funcions diverses per a Windows
// -----------------------------------

#ifndef __WCLH_H
#define __WCLH_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif

#ifndef IDHELP
  #define IDHELP      998
#endif

// ----------------------------------------
//	VARIABLES INICIALITZADES PEL COMPILADOR
//	BORLAND C++
//	----------------------------------------

extern HINSTANCE _hInstance, _hPrev;
extern LPSTR _pszCmdline;
extern int _cmdShow;

// Inicialitzat a CLIC.H
extern HCURSOR hCursorMicro;

// ----------------------------------------

class WinBase
{ public:
    WinBase() {hWnd=0;};

    virtual UINT run() = 0;
	 static  HINSTANCE hInst;
    static  HINSTANCE hPrevInst;
    static  LPSTR cmd;
    static  int show;
    static  int screenX;
	 static  int screenY;
    static  int captionY;
    static  int menuY;

    HWND hWnd;
};

class ModalControl : public virtual WinBase
{
  public:
    ~ModalControl() {if(hWnd) destroy();};

    BOOL create(char *classn, HWND hOwn, UINT ident, DWORD style,
    		char *txt, int x, int y, int w, int h,
		BOOL iVis=TRUE, BOOL iDis=FALSE, int iSet=0);
	 virtual BOOL destroy();
	 UINT run() {return 0;};

	 BOOL enable(BOOL action);
	 BOOL show(BOOL action);
	 BOOL drawItem(WPARAM wParam, LPARAM lParam, HBITMAP hBmTile,
						int wTile, int hTile, BOOL tiled, int fixPosX, int fixPosY);
	 void move(int x, int y, int w=-1, int h=-1, BOOL repaint=TRUE);
	 DWORD sendMess(UINT, WPARAM, LPARAM);
	 UINT id() {return CtrlId;};
	 int  getX() {return xPos;}
	 int  getY() {return yPos;}
	 void setSet(int s) {set=s;};
    BOOL drawTo(HDC hDC, int xp, int yp);

  protected:
	 HWND hOwner;
	 UINT CtrlId;
	 int  width, height;
	 int  xPos, yPos;
	 int  set;
};

#define NMAXDLG 10

class ModalDialog : public virtual WinBase
{ public:
    ModalDialog(HWND hOwner);
    ~ModalDialog();
	 virtual UINT run();
	 static BOOL inDialog() {return nDlgs>0;}

  protected:
    virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 void end();

    UINT result;
    BOOL inHelp;
    int  tableIndex;
    virtual LPCSTR getDialogName() = 0;
    virtual int getHelpItem();
	 HWND hOwn;

  private:
    static  BOOL CALLBACK _export dlgProc(HWND, UINT, WPARAM, LPARAM);  
    static  ModalDialog *dlgP[NMAXDLG];
    static  HWND dlgW[NMAXDLG];
    static  int nDlgs;
    static  DLGPROC dlgFarProc;
};


class Window : public virtual WinBase
{ public:
    Window() {nextWin=0; prevWin=0;};
	 ~Window() {destroy();};

    virtual BOOL create(HWND hOwner=0);
    virtual BOOL destroy();
	 virtual UINT run();

  protected:
    virtual LRESULT dispatch(UINT, WPARAM, LPARAM, int extra=FALSE);
    virtual BOOL registerClass() = 0;
    virtual BOOL createWindow() = 0;
    static  LRESULT CALLBACK _export wndProc(HWND, UINT, WPARAM, LPARAM);

    void chain();
	 void unchain();
    HWND   hOwn;
    HMENU  hMenu;
    HACCEL hAcc;

  private:
    static Window *winList;
    Window *nextWin;
	 Window *prevWin;
};

class WindowBWCC : public Window
{
  protected:
    virtual LRESULT dispatch(UINT, WPARAM, LPARAM, int bwcc=FALSE);
};

#define MAX_STRINGTBL_LEN 200
#define NOMEM msgBox(0, -1)

extern char helpFile[];
extern HWND genMsgOwn;
extern BOOL sonor;
extern HWND MciDisplayWnd;

extern struct LOGPALX
{ WORD  palVersion;
  WORD  palNumEntries;
  PALETTEENTRY  palPalEntry[256];
} logPal;

extern HPALETTE hPal0;

char *newChar(int size);

enum CURSOR_TIPUS {CURSOR_ESPERA, CURSOR_FLETXA, CAP_CURSOR, HANDLE_CURSOR};
void setCursor(HWND hWnd, CURSOR_TIPUS op, HCURSOR hCur=NULL);
void resetCursor(void);

char *getStringTbl(UINT wID);

extern BOOL printerBug;

BOOL loadImgFile(HWND hWnd, HBITMAP &hBitmap, char *fileName,
			 int &bmw, int &bmh);

int loadDIBFile(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh);

int loadDIBFileBMP(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh);

int loadDIBFileGIF(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh);

int dibToHbm(HWND hWnd, HBITMAP &hBitmap, BITMAPINFO *bminf, HGLOBAL bits);

#define BLANCTRANSP 0xB8074AL
#define NEGRETRANSP 0xE20746L

BOOL displayBitmapRop3(HBRUSH hFonsBrush, COLORREF tinta,
							  HDC hDC, HDC hMemDC, HBITMAP hBitmap,
							  int xd, int yd, int lx, int ly,
							  int xs=0, int ys=0, int lxs=0, int lys=0,
							  DWORD rop=BLANCTRANSP);

BOOL displayBitmapRop3(HBRUSH hFonsBrush, COLORREF tinta,
			HDC hDC, HDC hMemDC, LPCSTR name, int x, int y,
			int xs, int ys, int lx, int ly, DWORD rop=BLANCTRANSP);

BOOL displayBitmap(HDC hDC, HDC hMemDC, LPCSTR name, int x=0, int y=0,
			int xs=0, int ys=0, int lx=0, int ly=0, DWORD rop=SRCCOPY);

BOOL displayBitmap(HDC hDC, HDC hMemDC, HBITMAP hBitmap,
			int xd=0, int yd=0, int lx=0, int ly=0,
			int xs=0, int ys=0, int lxs=0, int lys=0,
			DWORD rop=SRCCOPY);

BOOL TileBmp(HDC hDC, HDC hMemDC, HBITMAP hBm, int w, int h, POINT origen, RECT dest);

BOOL drawLineRect(HDC hDC, LPRECT r);

HBITMAP createNewBitmap(HWND hWnd, HBITMAP hBitmap,
			int nlx, int nly, int lx=0, int ly=0, BOOL captCurs=TRUE);

HBITMAP getBmRect(HDC hDC, HDC hMemDC, RECT *r, HBITMAP hOrgBm=0);


HBITMAP LoadResourceBitmap(HINSTANCE hInstance, const char far *lpString, HPALETTE FAR* lphPalette);
HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, LPINT lpiNumColors);

int msgBox(HWND hWnd, int mainmsg, int message=0,
	   char *endText=0, char *iniText=0, UINT type=MB_ICONSTOP|MB_OK);

int BMsgBox(HWND hWnd, int mainmsg, int message=0,
	    char *endText=0, char *iniText=0, UINT type=MB_ICONSTOP|MB_OK);

void avisSonor(UINT tipus);

typedef struct
{ int	staticId;
  int * currVal;
  int	maxVal;
  int	minVal;
} ScrollBarItem;

BOOL dlgScrBar(HWND, WPARAM, ScrollBarItem scbi, BOOL soundWarn=TRUE);

#define MAXRECS 10
extern BOOL waveAllowed, midiAllowed, mciAllowed;
extern BOOL mciPlaying, mciRecording[MAXRECS];
enum TIPUS_SO {CAP_SO, CDAUDIO, MIDI, WAVE, UNKNOW_MCI, REC_REC, REC_PLAY, USER_CONT, EXIT_CLIC, EXEC_EXE};
BOOL playFile(char *fileName, TIPUS_SO tipus,
				  HWND hWnd=NULL, RECT *r=NULL, BOOL stretch=TRUE);
void silenci(BOOL total=TRUE);

#endif
