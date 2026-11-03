// ------------------------------------------
// MISCFUNC.H
// ------------------------------------------

#ifndef __MISCFUNC_H
#define __MISCFUNC_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif


#define MY_SIZE 421376L;

extern char *nullStr;
extern char *spcStr;

class GlobalVars
{ public:
	 GlobalVars();
	 ~GlobalVars();

	 char *clicPath;
	 char *ediText;
	 char *ediGraf;
	 char *originalPath;
	 char *playerId;
	 char *iniFile;
	 char *dbFile;
	 char *originalDbFile;
	 char *fileName0;
	 COLORREF aclrCust[16];
};

extern GlobalVars *gv;

enum drawMarcPos {INTERIOR, EXTERIOR, INTERIOR_NEGRE};
void drawMarc(HDC hDC, RECT *rect, int pos, int gruix=5);

#define RECTW(rect) (rect.right-rect.left)
#define RECTH(rect) (rect.bottom-rect.top)

extern RECT NullRect;
extern POINT NullPoint;

char **readTxtFile(char *fileName, int &nLines, HWND hWnd=0);
BOOL writeTxtFile(char *fileName, char **txt, HWND hWnd=0);
void delArray(char **&txt);
char **newArray(int n);
char **dupArray(char **txt);
BOOL copyArray(char **dest, int indest, char **source, int insource,
			 int e, BOOL upper=FALSE);
char *dupStr(char *source);
char *changeStrVar(char *&var, char *newText);

struct musica
{ int nota;
  int figura;
  int dot;
};

BOOL playMusic(HWND hWnd, struct musica *m, HCURSOR hCursor=0);

enum Exts {EXT_BMP, EXT_GIF, EXT_TXT, EXT_PUZ, EXT_SOP, EXT_ASS, EXT_CRW, EXT_TXA,
			  EXT_PAC, EXT_PCC, EXT_PCG, EXT_HLP, EXT_MVB, EXT_EXE, EXT_MID, EXT_WAV,
			  UNKNOW_EXT, NO_EXT=-1};

extern char cdUnit;
extern char *extName[];
extern char lastExt[4];

extern char *workPath;		// Directori de treball
extern char *curActPath;	// Directori de l'activitat actual
extern char *curPacPath;	// Directori del paquet actual
extern char *curPath;		// Directori actual (variable)
extern char *tempPath;		// Directori scratch
extern char *tempPath2;		// Directori scratch
extern char *tempPath3;	   // Directori scratch
#define LMAXRFTXT 512
extern char *txtbuff;     // Buffer per a operacions de text

char *setPathVar(char *pathAndFileName, char *destPath=curActPath);
char *getFullFileName(char *fileName, char *basePath=curActPath);
char *getRelativeFileName(char *fileName, char *cmpPath=curActPath);
char *simplifyFileName(char *fn);
int  getExt(char *fileName);
BOOL setActivePath(char *newPath);
BOOL detectCD();

#define BRACKET_TAG_SIZE 100
char *getBracketTag(char *&txt);

#define STRETCH_TABLE_SIZE 3
struct stretchTable
{ HBITMAP hBmStretch;
  HBITMAP hBmOriginal;
  int orgBmW;
  int orgBmH;
  int newBmW;
  int newBmH;
};

extern struct stretchTable strt[];
HBITMAP mkStretchBm(int tablePos, HWND hWnd, HBITMAP hbm, int newBmW, int newBmH, int orgBmW, int orgBmH);

class bmpCont
{ public:
	bmpCont();
	~bmpCont();
	static BOOL loadBmp(HWND hWnd, HBITMAP &hBitmap, char *fn,
							  int &bmwidth, int &bmheight);
	static BOOL getDIB(HBITMAP hBitmap, BITMAPINFO *&rbminf,
							 HGLOBAL &rbits, int &rbmw, int &rbmh);
	static void freeBmp(HBITMAP &hBitmap) {hBitmap=NULL;}
	static void flush();
	static void flushDIBs();

 protected:
	void chain();
	void unchain();

	HBITMAP hBm;
	char *fileName;
	int  bmw;
	int  bmh;
	HGLOBAL bits;
	BITMAPINFO *bminf;
	//int  count;

  private:
	bmpCont *prev;
	bmpCont *next;
	static bmpCont *first;
};

class SaveDir
{ private:
	 int drive;
	 char path[80];
  public:
	 SaveDir();
	 ~SaveDir();
};

// Sistema FC

struct HFILEFC
{
  HFILE hf;
  long  lof;
  long  curPos;
};

extern long offsetBaseExe;
int readFCHeader(char *fName, HFILE &hf, char *desc, BOOL close);
BOOL openFC(char* fName, HWND hWnd=0);
void closeFC();
BOOL isFCActive();
BOOL fc_lopen(char *fName, HFILEFC& hffc);
UINT fc_lread(HFILEFC &hfc, void _huge* hpvBuffer, UINT cbBuffer);
long fc_hread(HFILEFC &hffc, void _huge* hpvBuffer, long cbBuffer);
HFILE fc_lclose(HFILEFC &hfc);
BOOL fc_eof(HFILEFC &hfc);
char* fc_gets(char* buffer, int n, HFILEFC &hfc);
BOOL getFirstFCInto(char *fName);

int _lreadsz(HFILE hf, char* buffer, int maxLen);
LONG _lltell(HFILE hf);
LONG _llof(HFILE hf);

// -------- Altres funcions ---------

BOOL checkCas(HWND hDlg, int &ncw, int &nch,
			int maxX, int maxY, int idH, int idV);

void checkCaret(HWND hWnd, UINT wParam);


#endif
