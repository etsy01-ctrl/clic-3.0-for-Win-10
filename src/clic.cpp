// --------------------------------------
// CLIC.CPP
// Cos principal
// --------------------------------------


// Les variables de copilacio son:
// DEBUG
// STRICT
// STRICT;NOEDIT


#include "clic.h"
#include "playwin.h"
#include "clicdlg.h"
#ifndef NOEDIT
#include "deditact.h"
#include "dactopt.h"
#endif
#include "miscfunc.h"
#include "report.h"
#ifdef RUNTIME
#include "runtime-association.h"
#include "runtime-i18n.h"
#include "runtime-branding.h"
#endif
#include "rcomu\clicres.h"
#include "rcomu\clich.h"

#include <dir.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <shellapi.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <ddeml.h>

HINSTANCE _hInstance = GetModuleHandle(NULL);
HINSTANCE _hPrev = NULL;
LPSTR _pszCmdline = GetCommandLine();
int _cmdShow = SW_SHOWDEFAULT;

int stp=0;

#define HELPFILENAME "CLIC.HLP"
#define INIFILE "CLIC.INI"
#define DBFILE "CLICREP.EXE"


#define NZONES 4


#define EMI(m,v) if(menuVisible) EnableMenuItem(hMenu, m, ((v) ? MF_ENABLED:MF_DISABLED|MF_GRAYED))

enum Botons {BSURT, BDISC, BPRINT, BINICI, BAJUDA, BPUZZLE, BINFO, BAVANT, BENRERE, NBOTONS};
enum estat_temps {INIT_TMP, STOP_TMP, GO_TMP, END_TMP};
enum game_options {END_GAME, LOAD_GAME, REINIT_GAME, RESET_GAME};
enum report_options {BEGIN_REPORT, LINE_REPORT, END_REPORT, ACTIVATE_CLICDB};
enum id_comptadors {ENCERTS, INTENTS, TEMPS};

struct musica acabat[]={{60, 16, 0}, {62, 16, 0}, {64, 16, 0}, {65, 16, 0},
			{67, 16, 0}, {0, 0, 0}};

BOOL exitDirect=FALSE;
BOOL allowNewUsr=TRUE;
BOOL showUserReports=TRUE;
BOOL allowNewGrp=FALSE;
BOOL ok800=FALSE;
BOOL ok256=FALSE;
BOOL help=FALSE;
#ifndef RUNTIME
int forceMenu=0;
#else
int forceMenu=2;
#endif
#ifdef RUNTIME
BOOL runtimeClassicSkin=TRUE;
#endif
BOOL maxim0;
int passos, passos0, gAjVis;
int encertsNow=0;
int caretVis=FALSE, caretX=0, caretY=0, caretW=0, caretH=0, autoCursorDelay=1;
BOOL quick, quick0, editable, arrossega, arrossega0, sonor0, autoCursor, cursorGran;
BOOL doReportFile, drf0;
BOOL helponhelp, menus;
LOGFONT logFont0={21,8,0,0,FW_NORMAL,0,0,0,0,3,2,1,34,"Arial"};
LOGFONT logFont1={18,7,0,0,FW_BOLD,  0,0,0,0,3,2,1,34,"Arial"};

int gruixfil;
HCURSOR hGenCursor, hCursorGran, hCursorNormal, hCursorMicro;

// Variables impressio
BOOL noMes=FALSE;
BOOL noMesGlobal=FALSE;
BOOL bUserAbort=FALSE;
HWND hDlgPrint;

// Variables DDE
HCONV    hConv=NULL;
DWORD    idInst=NULL;
FARPROC  lpDdeProc=NULL;
HSZ      hszService=NULL;
HSZ      hszTopic=NULL;
HDDEDATA hData=NULL;
HDDEDATA EXPENTRY _export DDECallback(WORD,WORD,HCONV,HSZ,HSZ,HDDEDATA,DWORD,DWORD);

static char Clic[]="Clic";

BOOL initGlobalVars(LPSTR lpCmdLine);
void clearGlobalVars();
BOOL saveGlobalVars();
BOOL getBoolVar(int ids, char *itn, char *fn, BOOL defVal);

#ifdef RUNTIME
static int uiMin(int a, int b) {return a<b ? a:b;}
static int uiMax(int a, int b) {return a>b ? a:b;}
#endif

class MainWindow : public WindowBWCC
{
  public:
	 MainWindow();
	 ~MainWindow();

  protected:
	 LRESULT dispatch(UINT, WPARAM, LPARAM, BOOL bwcc=FALSE);
	 BOOL registerClass();
	 BOOL createWindow();

  private:
	 RECT        clRect, wRect, messRect;
	 POINT       pDigit[3], fb;
	 DigitBox    digit[3];
	 Contenidor  messBox;
	 Zona        *zones[NZONES];
	 Zona        zonaBase;
	 ModalControl bwcb[NBOTONS];
	 BOOL        bVis[NBOTONS];
	 PlayWindow  pw;
	 Paquet      pac;
	 Report      r;
	 int         waitPw;
	 BOOL        menuVisible;

	 BOOL cmd;
	 BOOL maxim, reporting;
	 int  minSizeX, minSizeY;
	 int  estatTemps;
	 char fileName[MAX_FULL_FILENAME];
	 int  autoPassDelay;
	 BOOL delaying, autoPlaying;
	 int  menuStatus;
	 char mainCapTxt[260];
	 int  bv;

	 BOOL processUserMess(WPARAM, LPARAM);
	 BOOL processCommand(WPARAM, LPARAM);
	 BOOL processPaint();
	 BOOL adjustSize();
	 BOOL openFitx();
#ifdef RUNTIME
 BOOL openRuntimePackage();
 BOOL adjustRuntimeSize();
 BOOL paintRuntime();
 BOOL drawRuntimeButton(LPARAM param);
 BOOL adjustModernSize();
 BOOL drawModernButton(LPARAM param);
 BOOL paintModern();
 void applyRuntimeSkin();
 void createRuntimeTooltips();
 void applyRuntimeLanguage(BOOL rebuildMenu=TRUE);
 BOOL classicSkin;
 HWND uiTooltip;
 HFONT uiClassicFont, uiClassicValueFont;
 HFONT uiFont, uiTitleFont, uiSmallFont, uiValueFont;
 char runtimePackage[MAX_FULL_FILENAME];
#endif
	 BOOL game(int op);
	 BOOL temps(int estat, int tnum);
	 BOOL info();
	 BOOL checkForSave(BOOL p=FALSE);
	 BOOL reporta(int rp=LINE_REPORT);
	 BOOL doInfo();
	 void setCaption(BOOL fn);
	 BOOL doPrint();
	 BOOL InitMsgDB();
	 BOOL SendMsgDB(char *msg);
};

MainWindow::MainWindow()
{
  int i;
  char *c=gv->fileName0;
#ifdef RUNTIME
  *runtimePackage=0;
  classicSkin=GetPrivateProfileInt("Player", "ClassicSkin", 1, gv->iniFile)!=0;
  runtimeClassicSkin=classicSkin; uiTooltip=NULL;
  uiClassicFont=CreateFont(-10,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Tahoma");
  uiClassicValueFont=CreateFont(-14,0,0,0,600,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Tahoma");
  uiFont=CreateFont(-16,0,0,0,500,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Segoe UI");
  uiTitleFont=CreateFont(-23,0,0,0,600,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Segoe UI");
  uiSmallFont=CreateFont(-13,0,0,0,400,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Segoe UI");
  uiValueFont=CreateFont(-21,0,0,0,600,0,0,0,DEFAULT_CHARSET,0,0,4,0,"Segoe UI");
#endif

  *fileName=0;
  cmd=FALSE;
  *mainCapTxt=0;
  bv=2;
  Zona::initAllBaseCont();
  Zona::setBaseBrush(GENERICA, RGB(0xC0, 0xC0, 0xC0), -1);

  if(c!=NULL && *c!=0 && strlen(c)<sizeof(fileName))
  { if(strnicmp("CLIC:", c, 5)==0)
	 { strcpy(fileName, gv->clicPath);
		c+=5;
		if(*c=='\\') c++;
	 }
	 strcat(fileName, c);
	 cmd=TRUE;
  }

  estatTemps=delaying=autoPlaying=FALSE;
  for(i=0; i<NBOTONS; i++) bVis[i]=FALSE;
  bVis[BSURT]=TRUE;
  bVis[BDISC]=TRUE;
  reporting=FALSE;
  waitPw=0;
  maxim=maxim0;
  for(i=0; i<3; i++) zones[i]=&digit[i];
  zones[3]=&messBox;
  autoPassDelay=0;
  pac.setBaseRep(&r);
  ActFile::createActdFiles();
  menuStatus=-1;
// AIXO VA FORA PERQUE ENCARA NO HI HA HWND
//  if(autoCursor) temps(GO_TMP, 3);
//
  hCursorGran=LoadCursor(hInst, MAKEINTRESOURCE(CURSOR_GRAN));
  hCursorNormal=LoadCursor(NULL, IDC_ARROW);
  hCursorMicro=LoadCursor(hInst, MAKEINTRESOURCE(CURSOR_MICRO));
  hGenCursor = cursorGran ? hCursorGran : hCursorNormal;
#ifdef RUNTIME
  menuVisible=TRUE;
#else
  menuVisible=!(*fileName!=0 && ((menus==FALSE && forceMenu==0) || forceMenu==2));
#endif
}

MainWindow::~MainWindow()
{
#ifdef RUNTIME
  DeleteObject(uiClassicFont); DeleteObject(uiClassicValueFont);
  DeleteObject(uiFont); DeleteObject(uiTitleFont);
  DeleteObject(uiSmallFont); DeleteObject(uiValueFont);
#endif
  temps(END_TMP, 1);
  temps(END_TMP, 2);
  temps(END_TMP, 3);
  bmpCont::flush();
  ActFile::clearActdFiles();

  Zona::clearAllBaseCont();
  DestroyCursor(hCursorGran);
  DestroyCursor(hCursorMicro);
  if(idInst)
  { if(hConv!=NULL)
	 { DdeDisconnect(hConv);
		hConv=NULL;
	 }
	 DdeUninitialize(idInst);
	 if(hszService) DdeFreeStringHandle(idInst, hszService);
	 if(hszTopic) DdeFreeStringHandle(idInst, hszTopic);
	 idInst=NULL;
  }
  if(lpDdeProc) FreeProcInstance(lpDdeProc);
}

BOOL MainWindow::registerClass()
{
  WNDCLASS wc;

  wc.style =         CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc =   WindowBWCC::wndProc;
  wc.cbClsExtra =    0;
  wc.cbWndExtra =    0;
  wc.hInstance =     hInst;
  wc.hIcon =         LoadIcon( hInst, MAKEINTRESOURCE(ICONA_PRINCIPAL));
  wc.hCursor =       hGenCursor;
//  wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
  wc.hbrBackground = NULL;
  wc.lpszMenuName =  menuVisible ? MAKEINTRESOURCE(MENU_PRINCIPAL) : NULL;
  wc.lpszClassName = CLICSTR;

#ifndef NOEDIT
  return RegisterClass(&wc) && DEditAct::registerTxClass();
#else
  return RegisterClass(&wc);
#endif
}

BOOL MainWindow::createWindow()
{
  int i, x, y, dy;

#ifdef RUNTIME
  hMenu=menuVisible ? createPlayerMenu() : NULL;
#else
  hMenu=menuVisible ? LoadMenu(hInst, (LPSTR)MENU_PRINCIPAL) : NULL;
#endif
  hAcc=menuVisible ? LoadAccelerators(hInst, (LPSTR)GLOBAL_ACC) : NULL;
  adjustSize();
  minSizeX=RECTW(wRect);
  minSizeY=RECTH(wRect);
  if((hWnd = CreateWindow(CLICSTR,
								  getStringTbl(IDS_TITOL),
								  (menuVisible ? MWSTYLE:NMSTYLE)|WS_CLIPCHILDREN,
								  CW_USEDEFAULT,
								  CW_USEDEFAULT,
								  RECTW(wRect),
								  RECTH(wRect),
								  NULL,
								  hMenu,
								  hInst,
								  NULL))==0) return FALSE;
  genMsgOwn=hWnd;

#ifndef RUNTIME
  if(!editable && menuVisible)
  { RemoveMenu(hMenu, 2, MF_BYPOSITION);
	 RemoveMenu(hMenu, 1, MF_BYPOSITION);
	 RemoveMenu(hMenu, IDM_SAVE, MF_BYCOMMAND);
	 RemoveMenu(hMenu, IDM_SAVEAS, MF_BYCOMMAND);
  }

#endif
#ifndef NOEDIT
  EMI(IDM_NEWUSER, doReportFile);
#endif
  EMI(IDM_CLICDB, FALSE);

  pw.create(hWnd);
  pw.marcs=FALSE;
  zonaBase.create(hWnd, &clRect, FALSE, FALSE, GENERICA);

  ShowWindow(hWnd, maxim ? SW_SHOWMAXIMIZED:show);

  x=fb.x+1; y=fb.y+1; dy=MARGE+BOTOH+2;

#define BOTOCLASS "button"
#ifdef RUNTIME
#define BOTOSTYLE BS_OWNERDRAW|WS_TABSTOP
#else
#define BOTOSTYLE BS_PUSHBUTTON|BS_OWNERDRAW
#endif

  bwcb[BSURT].create(BOTOCLASS, hWnd, BOTO_SURT, BOTOSTYLE, nullStr,
			  x, y, BOTOW, BOTOH, bVis[BSURT], FALSE, 1);
  bwcb[BENRERE].create(BOTOCLASS, hWnd, BOTO_ENRERE, BOTOSTYLE, nullStr,
			  x+MARGE+BOTOW, y, BOTOW2, BOTOH, FALSE, FALSE, 1);
  bwcb[BAVANT].create(BOTOCLASS, hWnd, BOTO_AVANT, BOTOSTYLE, nullStr,
			  x+2*MARGE+BOTOW+BOTOW2, y, BOTOW2, BOTOH, FALSE, FALSE, 1);
  bwcb[BDISC].create(BOTOCLASS, hWnd, BOTO_DISC, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, bVis[BDISC], FALSE, 1);
  bwcb[BPRINT].create(BOTOCLASS, hWnd, BOTO_PRINTER, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, bVis[BPRINT], FALSE, 1);
  bwcb[BINICI].create(BOTOCLASS, hWnd, BOTO_INICI, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, FALSE, FALSE, 1);
  bwcb[BAJUDA].create(BOTOCLASS, hWnd, BOTO_AJUDA, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, FALSE, FALSE, 1);
  bwcb[BPUZZLE].create(BOTOCLASS, hWnd, BOTO_PUZZLE, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, FALSE, FALSE, 1);
  bwcb[BINFO].create(BOTOCLASS, hWnd, BOTO_INFO, BOTOSTYLE, nullStr,
			  x, y-=dy, BOTOW, BOTOH, FALSE, FALSE, 1);

#ifdef RUNTIME
  SetWindowText(bwcb[BSURT].hWnd, "Salir");
  SetWindowText(bwcb[BDISC].hWnd, "Abrir juego");
  SetWindowText(bwcb[BINICI].hWnd, "Reiniciar");
  SetWindowText(bwcb[BAJUDA].hWnd, "Ayuda");
  SetWindowText(bwcb[BENRERE].hWnd, "Anterior");
  SetWindowText(bwcb[BAVANT].hWnd, "Siguiente");
  SetWindowText(bwcb[BPRINT].hWnd, "Imprimir");
  SetWindowText(bwcb[BPUZZLE].hWnd, "Puzzle");
  SetWindowText(bwcb[BINFO].hWnd, "Info");
#endif
#ifdef RUNTIME
  for(i=0; i<NBOTONS; i++) SendMessage(bwcb[i].hWnd, WM_SETFONT, (WPARAM)uiFont, FALSE);
#endif
  SetClassLong(bwcb[0].hWnd, GCL_HCURSOR, (LONG)hGenCursor);

  for(i=0; i<3; i++)
	 digit[i].create(pDigit[i], hWnd, TRUE, 0, MSB);
  encertsNow=0;

  messBox.create(hWnd, &messRect);
  messBox.setBase(MSB);
#ifdef RUNTIME
  if(!cmd)
  {
    EnableWindow(bwcb[BINICI].hWnd,FALSE); EnableWindow(bwcb[BAJUDA].hWnd,FALSE);
    EnableWindow(bwcb[BAVANT].hWnd,FALSE); EnableWindow(bwcb[BENRERE].hWnd,FALSE);
  }
  applyRuntimeLanguage(FALSE);
#endif

  UpdateWindow(hWnd);

  if(doReportFile) reporta(BEGIN_REPORT);
  if(cmd) PostMessage(hWnd, WM_COMMAND, IDM_OPENCMD, (LPARAM)(LPSTR)fileName);

  // Desplaçat des del constructor
  if(autoCursor) temps(GO_TMP, 3);
  Fil::setWidth(gruixfil);

  return TRUE;
}

LRESULT MainWindow::dispatch(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bwcc)
{
  int i;

  switch(msg)
  { case WM_QUERYENDSESSION:
			return TRUE;

	 case WM_CLOSE:
			if(!checkForSave() ||
				(exitDirect==FALSE &&
				 BMsgBox(hWnd, IDS_PROMPEXIT, 0, 0, 0,
							MB_YESNO|MB_ICONEXCLAMATION)!=IDYES)) return 0;

			game(END_GAME);
			if(help) {WinHelp(hWnd, (LPSTR)0, HELP_QUIT, 0); help=FALSE;}
			silenci();
			if(doReportFile) reporta(END_REPORT);
			closeFC();
			DestroyWindow(hWnd);
			return 0;

	 case WM_COMMAND:
			if(processCommand(wParam, lParam)) return 0;
			break;

	 case WM_LBUTTONDOWN:
			if(pw.playing && pw.txc && pw.txc->waiting)
			  pw.itsTime();
			return TRUE;

	 case WM_MOUSEMOVE:
			waitToPass=TRUE;
			break;

	 case WM_PAINT:
			processPaint();
			bwcc=TRUE;
			break;

	 case WM_TIMER:
			switch(wParam)
			{ case 1:
					 digit[TEMPS].incVal(1);
#ifdef RUNTIME
                     { RECT status; SetRect(&status, 0, clRect.bottom-(classicSkin ? 61:148), clRect.right, clRect.bottom); InvalidateRect(hWnd, &status, FALSE); }
#endif
					 if(waitPw!=0 && digit[TEMPS].getVal()>=waitPw)
					 { waitPw=0;
						pw.itsTime();
						digit[TEMPS].setVal(0);
					 }
					 break;

			  case 2:
					 if(ModalDialog::inDialog()) break;
					 if(delaying &&
						 --autoPassDelay<=0)
					 { autoPassDelay=0;
						temps(STOP_TMP, 2);
						PostMessage(hWnd, WM_COMMAND, BOTO_AVANT, 0L);
					 }
					 break;

			  case 3:
					 if(ModalDialog::inDialog()) break;
					 if(autoCursor && autoPlaying && GetFocus()==hWnd)
						pw.nextCursorPos();
					 break;

			  default: break;
			}
			break;

	 case WM_PLAYW:
#ifdef RUNTIME
            { RECT status; SetRect(&status, 0, clRect.bottom-(classicSkin ? 61:148), clRect.right, clRect.bottom); InvalidateRect(hWnd, &status, FALSE); }
#endif
			if(processUserMess(wParam, lParam)) return 0;
			break;

	 case WM_CHAR:
	 case WM_KEYDOWN:
	 case WM_KEYUP:
			if(pw.processKeyboard(msg, wParam)) return TRUE;
			break;

	 case WM_MOVE:
			GetClientRect(hWnd, &clRect);
			zonaBase.move(&clRect);
			break;

#ifdef RUNTIME
     case WM_GETMINMAXINFO:
            { MINMAXINFO *limits=(MINMAXINFO*)lParam;
              limits->ptMinTrackSize.x=uiMin(screenX, uiMax(classicSkin ? 640:920, minSizeX));
              limits->ptMinTrackSize.y=uiMin(screenY, uiMax(classicSkin ? 400:640, minSizeY));
            }
            return 0;
     case WM_SIZE:
            if(wParam!=1) {maxim=(wParam==2); adjustRuntimeSize();}
            return 0;
#else
	 case WM_SIZE:
			switch(wParam)
			{ case SIZEFULLSCREEN:
					 if(!maxim) {maxim=TRUE; adjustSize();}
					 break;

			  case SIZENORMAL:
					 if(maxim) {maxim=FALSE; adjustSize();}
					 break;

			  default: break;
			}
			break;

#endif

	 case WM_CTLCOLORMSGBOX:
	 case WM_CTLCOLOREDIT:
	 case WM_CTLCOLORLISTBOX:
	 case WM_CTLCOLORBTN:
	 case WM_CTLCOLORDLG:
	 case WM_CTLCOLORSCROLLBAR:
	 case WM_CTLCOLORSTATIC:
			return (LRESULT)Zona::getBaseBrush(GENERICA);

	 case WM_NCCREATE:
	 case WM_CREATE:
	 case WM_NCDESTROY:
	 case WM_SETFONT:
			bwcc=TRUE;
			break;

	 case WM_ERASEBKGND:
#ifndef RUNTIME
			zonaBase.update((HDC)wParam);
#endif
			return TRUE;

	 case WM_KILLFOCUS:
	 case WM_SETFOCUS:
			checkCaret(pw.hWnd, msg);
			break;

	 case WM_DESTROY:
			PostQuitMessage(0);
			break;

	 case WM_DRAWITEM:
#ifdef RUNTIME
            return drawRuntimeButton(lParam);
#else
			for(i=0; i<NBOTONS; i++)
			  if(bwcb[i].id()==wParam)
			  { bwcb[i].drawItem(wParam, lParam, pw.hBmpFons, pw.bmpfW, pw.bmpfH,
										pw.tileBmp, pw.bmpfX, pw.bmpfY);
				 break;
			  }
			return TRUE;
#endif

	 //case WM_PALETTEISCHANGING:
	 case WM_PALETTECHANGED:
			if((HWND)wParam!=hWnd && (HWND)wParam!=pw.hWnd)
			{ HDC hDc=GetDC(hWnd);
			  UpdateColors(hDc);
			  ReleaseDC(hWnd, hDc);
			}
			return TRUE;

	 case WM_QUERYNEWPALETTE:
			if(hPal0!=NULL)
			{ HDC hDc=GetDC(hWnd);
			  SetSystemPaletteUse(hDc, SYSPAL_STATIC);
			  if(SelectPalette(hDc, hPal0, 0)!=0)
			  { RealizePalette(hDc);
				 UpdateColors(hDc);
			  }
			  ReleaseDC(hWnd, hDc);
			  InvalidateRect(hWnd, NULL, TRUE);
			  InvalidateRect(pw.hWnd, NULL, TRUE);
			  return TRUE;
			}
			break;

	 case MM_MCINOTIFY:
			if(wParam==MCI_NOTIFY_SUCCESSFUL || wParam==MCI_NOTIFY_FAILURE)
			  resetCursor();
			break;

	 default: break;
  }
  return WindowBWCC::dispatch(msg, wParam, lParam, bwcc);
}

BOOL MainWindow::processUserMess(WPARAM wParam, LPARAM lParam)
{
  BOOL p=FALSE;

  switch(wParam)
  { case PLAYW_INTENT:
			if(pw.actMode==ASSOCIA && pw.puzMode==EXPLORA)
			{ //if(pac.autoPass && !pac.isEmpty() && !pac.lastAct())
			  if(pac.autoPass && !pac.isEmpty() && !pac.lastAct() && pac.noPassButtons)
			  { autoPassDelay=pac.lapToPass;
				 temps(GO_TMP, 2);
			  }
			}
			//else digit[INTENTS].incVal(1);
			else digit[INTENTS].incVal((int)lParam);
			break;

	 case PLAYW_ENCERTS:
#ifdef RUNTIME
            if(pw.noEvaluable()) lParam=0;
#endif
			if(digit[ENCERTS].getVal()!=(int)lParam)
			  digit[ENCERTS].setVal((int)lParam);
			encertsNow=digit[ENCERTS].getVal();
			break;

	 case PLAYW_SETMESSPLAY: p=TRUE;
	 case PLAYW_SETMESS:
			{ char *txt;
			  if((txt=newChar(_fstrlen((LPSTR)lParam)+1))!=0)
			  { _fstrcpy((LPSTR)txt, (LPSTR)lParam);
				 messBox.getData(txt);
				 if(pw.marcs)
				 { RECT rct=*messBox.getRect();
					InflateRect(&rct, 2, 2);
					InvalidateRect(hWnd, &rct, TRUE);
				 }
				 if(p) messBox.play();
				 delete txt;
			  }
			}
			break;

	 case PLAYW_RESIZE:
			adjustSize();
			return TRUE;

	 case PLAYW_ENDGAME:
			temps(STOP_TMP, 1);
			UpdateWindow(hWnd);
			playMusic(hWnd, acabat,
						 LoadCursor(_hInstance, MAKEINTRESOURCE(CURSOR_V)));
			reporta();
			if(pac.autoPass && !pac.isEmpty() && !pac.lastAct())
			{ autoPassDelay=pac.lapToPass;
			  temps(GO_TMP, 2);
			}
			return TRUE;

	 case PLAYW_WAITMEIN:
			waitPw=(int)lParam;
			digit[TEMPS].setVal(0);
			break;

	 default: break;
  }
  return FALSE;
}

BOOL MainWindow::processCommand(WPARAM wParam, LPARAM lParam)
{
  int i, j;
  LPSTR c;
  char *sp;

  i=0;

  switch(wParam)
  { case IDM_PRINT:
	 case BOTO_PRINTER:
			doPrint();
			break;

	 case IDFA_AJUDA: if(!bVis[BAJUDA] || !IsWindowEnabled(bwcb[BAJUDA].hWnd)) break;
	 case BOTO_AJUDA:
			pw.showHelp();
			return TRUE;

	 case IDFA_INFO: if(!bVis[BINFO]) break;
	 case BOTO_INFO:
#ifdef RUNTIME
            if(!pw.custHlp) {info(); return TRUE;}
#endif
			if(!doInfo())
			  BMsgBox(hWnd, IDS_ERRTOPIC, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
			return TRUE;

	 case IDM_INFO:
			info();
			return TRUE;

	 case IDM_INDEX:
			if(pw.txc!=NULL && pw.txc->doHelp()==TRUE) return TRUE;
	 case IDM_INDEX2:
			if(WinHelp(hWnd, helpFile, HELP_INDEX, 0)==TRUE)
			  help=TRUE;
			return TRUE;

#ifndef RUNTIME
	 case IDM_HELPTIPUS: i=HELP_CFEC;
	 case IDM_HELPEDIT:  if(!i) i=HELP_EDIT;
	 case IDM_HELPCONFI: if(!i) i=HELP_CONFIGURE;
			if(WinHelp(hWnd, helpFile, HELP_CONTEXT, i)==TRUE)
			  help=TRUE;
			return TRUE;

	 case IDM_USTECLAT:
			if(WinHelp(hWnd, (LPSTR)0, HELP_HELPONHELP, 0)==TRUE)
			  help=TRUE;
			return TRUE;
#endif

	 case IDM_OPENCMD:
			c=(LPSTR)lParam;
			if(c==0 || *c==0 || lstrlen(c)>=sizeof(fileName)) break;
			if(c!=(LPSTR)fileName)
			{ *fileName=0;
			  if (_fstrnicmp("CLIC:", c, 5)==0)
			  { strcpy(fileName, gv->clicPath);
				 c+=5;
				 if(*c=='\\') c++;
			  }
			  lstrcat((LPSTR)fileName, c);
			}

			cmd=FALSE;

			i=1;

	 case BOTO_DISC:
	 case IDM_OPENFITX:

			if(!i)
			{ if(!checkForSave()) break;
#ifdef RUNTIME
			  if(!openRuntimePackage()) break;
			  closeFC();
#else
			  if(!openFitx()) break;
			  closeFC();
#endif
			}
			i=0;

			sp=strrchr(fileName, '.');
			if(sp!=NULL &&
			   (strnicmp(sp, ".PCC", 4)==0 || strnicmp(sp, ".PAC", 4)==0 || strnicmp(sp, ".EXE", 4)==0))
			{ char *activity=sp+4;
			  if(*activity==' ')
			  { while(*activity==' ') activity++;
				 if(*activity>='0' && *activity<='9')
				 { char *endActivity=activity;
					while(*endActivity>='0' && *endActivity<='9') endActivity++;
					while(*endActivity==' ') endActivity++;
					if(*endActivity==0)
					{ i=atoi(activity);
					  sp[4]=0;
					}
				 }
			  }
			}

#ifdef RUNTIME
            { char *base=strrchr(fileName, '\\');
              strncpy(runtimePackage, base ? base+1 : fileName, sizeof(runtimePackage)-1);
              runtimePackage[sizeof(runtimePackage)-1]=0;
            }
#endif
			j=getExt(fileName);
			if(j==EXT_PCC || j==EXT_PAC) offsetBaseExe=0L;
			else if(j==EXT_EXE) offsetBaseExe=MY_SIZE;
			if((j==EXT_PCC || j==EXT_EXE) && openFC(fileName, hWnd)==TRUE)
			{ if(doReportFile)
			  { char *s;
				 if((s=strrchr(fileName, '\\'))==NULL) s=fileName; else s++;
				 SendMsgDB(s);
			  }
			  getFirstFCInto(fileName);
			}
			if(getExt(fileName)==EXT_PAC)
			{ pac.load(fileName);
			  if(pac.isEmpty()) *fileName=0;
			  else
			  { if(pac.av800 && !ok800)
				 {	ok800=TRUE;
					if(msgBox(hWnd, IDS_800x600, 0, NULL, NULL, MB_YESNO|MB_ICONSTOP)!=IDYES)
					{ exitDirect=TRUE;
					  PostMessage(hWnd, WM_CLOSE, 0, 0L);
					  return TRUE;
					}
				 }
				 if(pac.av256 && !ok256)
				 { ok256=TRUE;
					if(msgBox(hWnd, IDS_256COLORS, 0, NULL, NULL, MB_YESNO|MB_ICONSTOP)!=IDYES)
					{ exitDirect=TRUE;
					  PostMessage(hWnd, WM_CLOSE, 0, 0L);
					  return TRUE;
					}
				 }
				 if(doReportFile && !isFCActive())
				 { char *s;
					if((s=strrchr(fileName, '\\'))==NULL) s=fileName; else s++;
					SendMsgDB(s);
				 }

				 if(i>1 && pac.nActs>1)
					for(j=1; j<i && j<pac.nActs; j++) pac.nextAct();

				 strcpy(fileName, pac.getAct());
			  }
			}
			else pac.clear();
			game(LOAD_GAME);
			return TRUE;

	 case BOTO_AVANT:
			if(!checkForSave()) break;
			if(!pac.isEmpty() && !pac.lastAct())
			{ reporta();
			  pac.nextAct();
			  strcpy(fileName, pac.getAct());
			  game(LOAD_GAME);
			  return TRUE;
			}
			else return FALSE;

	 case BOTO_ENRERE:
			if(!checkForSave()) break;
			if(!pac.isEmpty() && !pac.firstAct())
			{ reporta();
			  pac.prevAct();
			  strcpy(fileName, pac.getAct());
			  game(LOAD_GAME);
			  return TRUE;
			}
			else return FALSE;

#ifdef RUNTIME
     case IDM_LANGUAGE_ES:
     case IDM_LANGUAGE_EN:
     case IDM_LANGUAGE_FR:
     case IDM_LANGUAGE_CA:
            setPlayerLanguage(wParam-IDM_LANGUAGE_ES);
            applyRuntimeLanguage();
            return TRUE;
     case IDM_LANGUAGE_SAVE:
            if(!savePlayerLanguage(gv->iniFile))
              MessageBox(hWnd,tr(PT_SAVE_LANG_ERROR),"Clic Player",MB_OK|MB_ICONEXCLAMATION);
            applyRuntimeLanguage();
            return TRUE;
     case IDM_SKIN_CLASSIC:
     case IDM_SKIN_MODERN:
            classicSkin=(wParam==IDM_SKIN_CLASSIC);
            runtimeClassicSkin=classicSkin;
            WritePrivateProfileString("Player", "ClassicSkin", classicSkin ? "1":"0", gv->iniFile);
            applyRuntimeSkin();
            { RECT outer; GetWindowRect(hWnd,&outer);
              int ww=uiMin(screenX,uiMax((int)RECTW(outer),minSizeX+16));
              int hh=uiMin(screenY,uiMax((int)RECTH(outer),minSizeY+captionY+menuY+16));
              SetWindowPos(hWnd,NULL,uiMin((int)outer.left,screenX-ww),uiMin((int)outer.top,screenY-hh),ww,hh,SWP_NOZORDER);
            }
            return TRUE;
     case IDM_ASSOCIATE_PCC:
            associatePccFiles(hWnd);
            return TRUE;
	 case IDM_MINIMIZE:
			ShowWindow(hWnd, SW_MINIMIZE);
			return TRUE;
#endif

	 case BOTO_SURT:
	 case IDM_EXIT:
			SendMessage(hWnd, WM_CLOSE, 0, 0L);
			return TRUE;

	 case BOTO_PUZZLE:
	 case IDM_PUZZLE:
			{ DPuzzle *dpuzz;
			  i=FALSE;
			  if((dpuzz=new DPuzzle(hWnd, &pw))!=0)
			  { i=dpuzz->run();
				 delete dpuzz;
			  }
			  else {NOMEM; return FALSE;}
			  if(i) game(REINIT_GAME);
			}
			return TRUE;

	 case IDM_CLEARREP:
			r.clear();
			return TRUE;

#ifndef RUNTIME
	 case IDM_REPORT:
			{ DReport *drep;
			  if((drep=new DReport(hWnd, &r))!=0)
			  { drep->run();
				 delete drep;
			  }
			  else {NOMEM; return FALSE;}
			}
			return TRUE;

	 case IDM_CLICDB:
			if(showUserReports && hConv) reporta(ACTIVATE_CLICDB);
			return TRUE;

	 case IDM_BARALLAT:
			{ BOOL dbChanged;
			  DConfigura *dconf;
			  i=FALSE;

			  temps(STOP_TMP, 3);
			  if((dconf=new DConfigura(hWnd))!=0)
			  { i=dconf->run();
				 dbChanged=dconf->dbChanged;
				 delete dconf;
			  }
			  else {NOMEM; return FALSE;}

			  if(i==TRUE && noMesGlobal==FALSE)
			  { DConfirmOps *dconfops;

				 if((dconfops=new DConfirmOps(hWnd))!=0)
				 { BOOL r;
					r=dconfops->run();
					noMesGlobal=dconfops->noMes;
					delete dconfops;

					if(r) saveGlobalVars();
				 }
				 else {NOMEM; return FALSE;}
			  }

			  if(dbChanged && doReportFile)
			  { char msg[MAX_FULL_FILENAME+20];
				 strcat(strcpy(msg, "SETDB:"), gv->dbFile);
				 if(reporting) reporta(END_REPORT);
				 SendMsgDB(msg);
				 reporta(BEGIN_REPORT);
			  }

			  EMI(IDM_CLICDB, showUserReports && hConv);

			  if(i) game(REINIT_GAME);
			}
			EMI(IDM_NEWUSER, doReportFile);

			hGenCursor = cursorGran ? hCursorGran : hCursorNormal;
			SetClassLong(hWnd, GCL_HCURSOR, (LONG)hGenCursor);
			SetClassLong(bwcb[0].hWnd, GCL_HCURSOR, (LONG)hGenCursor);
			pw.changeCursorMode();

			Fil::setWidth(gruixfil);
			if(autoCursor) temps(GO_TMP, 3);
			return TRUE;

	 case IDM_NEWUSER:
			if(doReportFile)
			{ if(reporting) reporta(END_REPORT);
			  reporta(BEGIN_REPORT);
			}
			break;
#endif

	 case IDFA_INICI: if(!bVis[BINICI]) break;
	 case BOTO_INICI:
			if(pw.useDLL)
			  game(RESET_GAME);
			else
			{ reporta();
			  pw.resetGame();
			  temps(INIT_TMP, 1);
			  for(i=0; i<3; i++) digit[i].setVal(0);
			  encertsNow=0;
			}
			return TRUE;

#ifndef NOEDIT
	 case IDM_ACTOPT:
			{ DActOpt *dac;
			  i=FALSE;

			  if((dac=new DActOpt(hWnd, &pw))!=0)
			  { i=dac->run();
				 delete dac;
			  }
			  else {NOMEM; return FALSE;}
			  if(i) game(RESET_GAME);
			}
			return TRUE;

	 case IDM_SAVEAS: i=TRUE;
	 case IDM_SAVE:   pw.save(i);
			setCaption(TRUE);
			return TRUE;

	 case IDM_PUZZ: i=PUZZLE;
	 case IDM_ASS:  if(!i) i=ASSOCIA;
	 case IDM_SOP:  if(!i) i=SOPA;
	 case IDM_CRW:  if(!i) i=CREUATS;
	 case IDM_TEXTACT:  if(!i) i=TEXTACT;
			if(!checkForSave()) break;
			if(!noMes)
			{ DWorkDir *wd;
			  if((wd=new DWorkDir(hWnd, workPath, noMes))!=0)
			  { if((j=wd->run())==TRUE)
				 { noMes=wd->noMes;
					setActivePath(wd->path);
					strcpy(workPath, curPath);
				 }
				 delete wd;
			  }
			  else {NOMEM; break;}
			  if(!j) break;
			}
			game(END_GAME);
			pac.clear();
			strcpy(curActPath, workPath);
			pw.newAct(i);

	 case IDM_EDITACT:
			if(pw.actMode!=NONE)
			{ DEditAct *dep;
			  Contenidor *caretOwn0=Contenidor::caretOwner;

			  if(caretOwn0) caretOwn0->lostCaret();
			  i=FALSE;
			  if((dep=new DEditAct(hWnd, &pw))!=0)
			  { i=dep->run();
				 delete dep;
			  }
			  else NOMEM;
			  if(caretOwn0) caretOwn0->gainCaret();
			  if(i) game(RESET_GAME);
			}
			return TRUE;

	 case IDM_PAQ:
			if(!checkForSave()) break;
			if(!noMes)
			{ DWorkDir *wd;
			  if((wd=new DWorkDir(hWnd, workPath, noMes))!=0)
			  { if(wd->run()==TRUE)
				 { noMes=wd->noMes;
					setActivePath(wd->path);
					strcpy(workPath, curPath);
				 }
				 delete wd;
			  }
			  else {NOMEM; break;}
			}
			pac.clear();
			strcpy(curPacPath, workPath);

	 case IDM_EDITPAC:
			{ DEdPaquet *dedpaq;
			  i=FALSE;
			  if((dedpaq=new DEdPaquet(hWnd, &pac))!=0)
			  { if(dedpaq->run()==TRUE)
				 { if(!pac.isEmpty()) strcpy(fileName, pac.getAct());
					i=TRUE;
				 }
				 delete dedpaq;
			  }
			  else NOMEM;
			  if(i) checkForSave(TRUE);
			  if(!pac.isEmpty()) strcpy(fileName, pac.getAct());
			  if(i) game(LOAD_GAME);
			}
			return TRUE;
#endif
	 default: break;
  }
  return FALSE;
}

BOOL MainWindow::info()
{
#ifdef RUNTIME
  MessageBox(hWnd,tr(PT_ABOUT_TEXT),tr(PT_ABOUT_TITLE),MB_OK|MB_ICONINFORMATION);
#else
  DInfo *inf;
  if((inf=new DInfo(hWnd))==0) {NOMEM; return FALSE;}
  inf->run();
  delete inf;
#endif
  return TRUE;
}

BOOL MainWindow::openFitx()
{
  int result=FALSE;
#ifndef RUNTIME
  static char captxt[160];
  DFitx *fitx;

  *captxt=0;
  *fileName=0;

  strcpy(captxt, getStringTbl(IDS_OPENFITX));
  if((fitx=new DFitx(hWnd, captxt, workPath, 0, 0, EXT_PCG, TRUE))!=0)
  { if(fitx->run()==TRUE)
	 { strcpy(fileName, fitx->getFileName());
		result=TRUE;
	 }
	 delete fitx;
  }
  else NOMEM;
#endif
  return result;
}

#ifdef RUNTIME
BOOL MainWindow::openRuntimePackage()
{
  OPENFILENAME ofn;
  char packageName[MAX_FULL_FILENAME], filter[512];
  sprintf(filter,"%s%c*.pcc;*.pac;*.exe%c%s%c*.*%c%c",tr(PT_FILTER_PACKAGES),0,0,tr(PT_FILTER_ALL),0,0,0);

  *packageName=0;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize=sizeof(ofn);
  ofn.hwndOwner=hWnd;
  ofn.lpstrFilter=filter;
  ofn.lpstrFile=packageName;
  ofn.nMaxFile=sizeof(packageName);
  ofn.Flags=OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrTitle=tr(PT_PICK_TITLE);
  ofn.lpstrDefExt="pcc";

  if(!GetOpenFileName(&ofn)) return FALSE;
  strcpy(fileName, packageName);
  return TRUE;
}
#endif

BOOL MainWindow::processPaint()
{
#ifdef RUNTIME
  return paintRuntime();
#else
  int i;
  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  RECT r1, r2;
  BOOL aZona=FALSE;

  if(IsIconic(hWnd)) return FALSE;
  hDC=BeginPaint(hWnd, &ps);
  hMemDC=CreateCompatibleDC(hDC);

  for(i=0; i<NZONES; i++)
	 if(!zones[i]->isEmpty() && zones[i]->equalRect(&ps.rcPaint)) break;

  aZona=(i<NZONES);

  if(aZona)
	 zones[i]->update(hDC, 0, hMemDC);

  if(!aZona && pw.hBmpFons)
  { if(pw.tileBmp)
		TileBmp(hDC, hMemDC, pw.hBmpFons, pw.bmpfW, pw.bmpfH, NullPoint, ps.rcPaint);
	 else
		displayBitmap(hDC, hMemDC, pw.hBmpFons, pw.bmpfX, pw.bmpfY, pw.bmpfW, pw.bmpfH);
  }

  if(!aZona)
  { for(i=0; i<NZONES; i++)
		if(!zones[i]->isEmpty() && zones[i]->isIn(&ps.rcPaint))
		  zones[i]->update(hDC, 0, hMemDC);

	 if(pw.marcs)
	 { drawMarc(hDC, &clRect, INTERIOR, 1);
		drawMarc(hDC, &pw.wRect, EXTERIOR, 1);
		if(!zones[3]->isEmpty()) drawMarc(hDC, &messRect, EXTERIOR, 1);
		if(pw.barraAmunt)
		{ r1.left=0; r1.top=2*MARGE+BOTOH; r1.right=clRect.right; r1.bottom=r1.top;
		  drawMarc(hDC, &r1, INTERIOR, 1);
		}
	 }

	 for(i=0; i<3; i++)
		if(pw.comptadors[i])
		{ r1=*digit[i].getRect();
		  r2=r1; r2.top-=16; r2.bottom=r2.top+14;
		  displayBitmapRop3(Zona::getBC(MSB)->hBackBrush, Zona::getBC(MSB)->c.textColor,
								  hDC, hMemDC, MAKEINTRESOURCE(RETOLS),
								  r2.left, r2.top, 0, 14*i, 3*WDIGIT, 14, BLANCTRANSP);

		  if(pw.marcs)
		  { drawMarc(hDC, &r1, EXTERIOR, 1);
			 drawMarc(hDC, &r2, EXTERIOR, 1);
		  }
		}

  }

  DeleteDC(hMemDC);
  EndPaint(hWnd, &ps);
  return TRUE;
#endif
}

BOOL MainWindow::adjustSize()
{
#ifdef RUNTIME
  return adjustRuntimeSize();
#else
  int i, pwW, pwH, px, py, nc, ba;
  POINT p;

  ba=pw.barraAmunt;
  clRect.left=0; clRect.top=0;
  pwW=RECTW(pw.wRect); pwH=RECTH(pw.wRect);


  minSizeX=pwW+2*MARGE+(ba || bv<2 ? 0 : MARGE + BOTOW);
  minSizeY=pwH+3*MARGE+CTRLH+(ba ? BOTOH+MARGE : 0);

  if(maxim)
  { if(hWnd) GetClientRect(hWnd, &clRect);
	 else
	 { clRect.right=GetSystemMetrics(SM_CXFULLSCREEN);
		clRect.bottom=menuVisible ? GetSystemMetrics(SM_CYFULLSCREEN)-GetSystemMetrics(SM_CYMENU):
											 GetSystemMetrics(SM_CYSCREEN);
	 }
  }
  else
  { clRect.right= pwW>PLWW ? minSizeX : CLW;
	 clRect.bottom= pwH>PLWH ? minSizeY : CLH +(ba ? BOTOH+MARGE : 0);
  }

  for(nc=0, i=0; i<3; i++) if(pw.comptadors[i]) nc++;
  p.x=clRect.right-MARGE-3*WDIGIT;
  p.y= clRect.bottom-MARGE-HDIGIT;
  for(i=2; i>=0; i--)
	 if(pw.comptadors[i])
	 { pDigit[i].x=p.x;
		pDigit[i].y=p.y;
		p.x-=(3*WDIGIT+MARGE);
	 }

  p.x+=3*WDIGIT;

  messRect.left=clRect.left+MARGE+(bv==0 || ba ? 0: BOTOW+MARGE)+
					 (MARGE+BOTOW2)*((bVis[BAVANT] || bVis[BENRERE]) ? 2:0);

  messRect.bottom= clRect.bottom-MARGE;
  messRect.right=p.x;
  messRect.top=messRect.bottom-CTRLH;

  fb.x=MARGE;
  fb.y= ba ? clRect.top+MARGE-1 : clRect.bottom-MARGE-BOTOH-1;

  p.x=0; p.y=0;
  if(hWnd) ClientToScreen(hWnd, (LPPOINT)&p);
  wRect.left=p.x; wRect.top=p.y-1;
  wRect.right=wRect.left+RECTW(clRect);
  wRect.bottom=wRect.top+RECTH(clRect);
  AdjustWindowRect((LPRECT)&wRect, MWSTYLE, TRUE);
  OffsetRect((LPRECT)&wRect,
				 wRect.right > screenX ? screenX-wRect.right : 0,
				 wRect.bottom > screenY ? screenY-wRect.bottom : 0);

  pw.wRect.left=((bv<2 || ba ? 0 : MARGE+BOTOW)+clRect.right-pwW)/2;
  pw.wRect.top= ba ? (messRect.top-pwH-MARGE-BOTOH)/2+MARGE+BOTOH : (messRect.top-pwH)/2;

  if(pw.bmpFons!=0 && pw.tileBmp==FALSE)
  { pw.bmpfX=pw.wRect.left-(pw.bmpfW-pwW)/2;
	 if((pw.bmpfX+pw.bmpfW)>clRect.right) pw.bmpfX-=(pw.bmpfX+pw.bmpfW-clRect.right);
	 if(pw.bmpfX<0) pw.bmpfX=0;
	 pw.bmpfY=pw.wRect.top-(pw.bmpfH-pwH)/2;
	 if(pw.bmpfY<0) pw.bmpfY=0;
  }
  else
  { pw.bmpfX=MARGE+(bv<2 || ba ? 0 : MARGE+BOTOW);
	 pw.bmpfY=MARGE+(ba ? 2*MARGE+BOTOH : 0);
  }

  if(pw.pwrp)
  { pw.wRect.left=pw.bmpfX+pw.pwrx;
	 pw.wRect.top=pw.bmpfY+pw.pwry;
  }

  pw.wRect.right=pw.wRect.left+pwW;
  pw.wRect.bottom=pw.wRect.top+pwH;

  if((px=pw.wRect.right-(clRect.right-MARGE))>0) OffsetRect(&pw.wRect, -px, 0);
  if((py=pw.wRect.bottom-(clRect.bottom-2*MARGE-BOTOH))>0) OffsetRect(&pw.wRect, 0, -py);

  if(hWnd)
  { if(!maxim)
		MoveWindow(hWnd, wRect.left, wRect.top,
					  RECTW(wRect), RECTH(wRect), TRUE);

	 if(pw.hWnd)
		MoveWindow(pw.hWnd, pw.wRect.left, pw.wRect.top, pwW, pwH, TRUE);

	 for(i=0; i<3; i++)
		if(pw.comptadors[i]) digit[i].move(pDigit[i]);

	 for(i=0; i<NBOTONS; i++) bwcb[i].setSet(pw.btTipus);

	 for(i=0, px=fb.x, py=fb.y; i<NBOTONS-2; i++)
	 { if(bVis[i])
		{ bwcb[i].move(px, py);
		  if(ba)
			 px+=(MARGE+BOTOW);
		  else
			 py-=(MARGE+BOTOH);

		}
		bwcb[i].show(bVis[i]);
	 }

	 px = fb.x+(bv==0 || ba ? 0: BOTOW+MARGE);
	 py = clRect.bottom-MARGE-BOTOH-1;
	 bwcb[BENRERE].move(px, py);
	 bwcb[BAVANT].move(px+(MARGE+BOTOW2), py);

	 if(bVis[BAVANT] && !pac.isEmpty() && pac.botoSep)
	 {
		OffsetRect(&messRect, -(MARGE+BOTOW2)+1, 0);
		bwcb[BAVANT].move(messRect.right+MARGE-1, py);
	 }
	 else
	 {
		bwcb[BAVANT].move(px+(MARGE+BOTOW2)-1, py);
	 }

	 messBox.move(&messRect);

	 InvalidateRect(hWnd, NULL, TRUE);
	 for(i=0; i<NBOTONS; i++)
	 {	InvalidateRect(bwcb[i].hWnd, NULL, TRUE);
		UpdateWindow(bwcb[i].hWnd);
	 }
	 UpdateWindow(hWnd);
  }
  return TRUE;
#endif
}

BOOL MainWindow::temps(int estat, int tnum)
{
  static BOOL estatTemps=FALSE;
  BOOL result=FALSE;

  switch(tnum)
  { case 1:
			switch(estat)
			{ case INIT_TMP:
					 digit[TEMPS].setVal(0);

			  case GO_TMP:
					 if(!estatTemps &&
						 SetTimer(hWnd, 1, 1000, (FARPROC)NULL)!=0)
					 result=estatTemps=TRUE;
					 break;

			  case END_TMP:
					 digit[TEMPS].setVal(0);

			  case STOP_TMP:
					 if(estatTemps) result=KillTimer(hWnd, 1);
					 estatTemps=FALSE;
					 break;

			  default: break;
			}
			break;

	 case 2:
			switch(estat)
			{ case GO_TMP:
					 if(!delaying &&
						 SetTimer(hWnd, 2, 1000, (FARPROC)NULL)!=0)
						result=delaying=TRUE;
					 break;

			  case STOP_TMP:
					 if(delaying) result=KillTimer(hWnd, 2);
					 delaying=FALSE;
					 break;

			  default: break;
			}
			break;

	 case 3:
			switch(estat)
			{ case GO_TMP:
					 if(!autoPlaying &&
						 SetTimer(hWnd, 3, autoCursorDelay*500, (FARPROC)NULL)!=0)
						result=autoPlaying=TRUE;
					  break;

			  case STOP_TMP:
					 if(autoPlaying) result=KillTimer(hWnd, 3);
					 autoPlaying=FALSE;
					 break;

			  default: break;
			}
			break;

	 default: break;
  }
  return result;
}


BOOL MainWindow::game(int op)
{
  BOOL result=FALSE;
  int i, pm=NONE;

  if(delaying) temps(STOP_TMP, 2);
  waitPw=0;

  silenci();
  reporta();
  if(pw.useDLL && op==REINIT_GAME) op=RESET_GAME;

  switch(op)
  { case LOAD_GAME:
			if(pw.load(fileName)==FALSE) break;

	 case RESET_GAME:
			bmpCont::flush();
			if(pw.useDLL) pw.callRMKDll();
			if(pw.initAct()==FALSE) break;

	 case REINIT_GAME:
			pm=pw.actMode;
			bVis[BSURT]=pw.shPorta && (pac.isEmpty() || !pac.noExitButton);
			bVis[BDISC]=pw.shDisk && (pac.isEmpty() || !pac.noDiskButton);
			bVis[BPRINT]=pw.shPrint;
			bVis[BINICI]= pm!=NONE && pw.noEvaluable()==FALSE && pw.noBV==FALSE;
			bVis[BAJUDA]= pm!=NONE && pw.ajAllowed()==TRUE && pw.shHelp==TRUE;
			bVis[BPUZZLE]=(pm==PUZZLE && pw.shPuz && pm==PUZZLE);
			bVis[BINFO]= pm!=NONE && pw.custHlp==TRUE;
			if(!pac.isEmpty())
			{ bVis[BAVANT]=!pac.lastAct() && !pac.noPassButtons;
			  bVis[BENRERE]=!pac.firstAct() && !pac.noPassButtons;
			}
			else bVis[BAVANT]=bVis[BENRERE]=FALSE;
#ifdef RUNTIME
            bVis[BSURT]=bVis[BDISC]=TRUE;
            bVis[BINICI]=bVis[BAJUDA]=TRUE;
            bVis[BAVANT]=bVis[BENRERE]=TRUE;
            EnableWindow(bwcb[BINICI].hWnd, pm!=NONE);
            EnableWindow(bwcb[BAJUDA].hWnd, pm!=NONE && pw.ajAllowed() && pw.shHelp);
            EnableWindow(bwcb[BAVANT].hWnd, !pac.isEmpty() && !pac.lastAct());
            EnableWindow(bwcb[BENRERE].hWnd, !pac.isEmpty() && !pac.firstAct());
#endif

			for(bv=0, i=0; i<NBOTONS-2; i++) if(bVis[i]) bv++;
			for(i=0; i<NBOTONS; i++) bwcb[i].show(bVis[i]);

			if(pw.initGame(bv)==FALSE ||
				(pm=pw.actMode)==NONE) break;

			temps(INIT_TMP, 1);
			for(i=0; i<3; i++)
			{ digit[i].setVal(0);
			  digit[i].marked(pw.noEvaluable());
			  #ifdef RUNTIME
              digit[i].hide(TRUE);
#else
              digit[i].hide(pw.comptadors[i]==FALSE);
#endif
			}
			encertsNow=0;
			result=TRUE;
			break;

	 default: break;
  }

  if(result==FALSE)
  { pw.endGame();
	 temps(END_TMP, 1);
	 messBox.clear();
  }
  else
  if(pm==ASSOCIA && pw.puzMode==INFO &&
	  pac.autoPass && !pac.isEmpty() && !pac.lastAct())
  { autoPassDelay=pac.lapToPass;
	 temps(GO_TMP, 2);
  }

#ifdef RUNTIME
  if(result) applyRuntimeSkin();
#endif
  setCaption(result);

#ifndef NOEDIT
  if(editable)
  { BOOL m;
	 int pis, ms;

	 m=(pm!=NONE && !isFCActive());
	 pis=(pac.isEmpty()==FALSE && !isFCActive())*2;
	 ms=m+pis;
	 if(ms!=menuStatus)
	 { EMI(IDM_EDITACT, m);
		EMI(IDM_ACTOPT, m);
		EMI(IDM_EDITPAC, (pis!=0));
		EMI(IDM_SAVE, m);
		EMI(IDM_SAVEAS, m);
		menuStatus=ms;
	 }
  }
#endif

  return result;
}


BOOL MainWindow::doInfo()
{
  int i;
  BOOL isExe;
  char cLine[MAX_FULL_FILENAME*2];
  char *caret;
  OFSTRUCT ofs;
  ofs.cBytes=sizeof(OFSTRUCT);

  i=getExt(pw.custHelpFile);
  isExe=(strcmp(strupr(lastExt), "EXE")==0 ||
			strcmp(lastExt, "COM")==0 ||
			strcmp(lastExt, "BAT")==0 ||
			strcmp(lastExt, "PIF")==0);

  if(i==EXT_HLP||i==EXT_MVB)
	 return WinHelp(hWnd, getFullFileName(pw.custHelpFile, curActPath),
						 *pw.hlpTopic ? HELP_PARTIALKEY:HELP_CONTENTS,
						 (DWORD)(LPSTR)pw.hlpTopic);

  *cLine=0;
  if(!isExe)
  { if(GetProfileString("Extensions", lastExt, nullStr,
								cLine, sizeof(cLine)-1)==0)
	 { LONG cb;
		HKEY hk, hk2;
		char *p;
		char buf1[128];
		char buf2[256];

		strcat(strcpy(buf1, "."), lastExt);
		if(RegOpenKey(HKEY_CLASSES_ROOT, buf1, &hk)==ERROR_SUCCESS)
		{ *buf2=0;
		  cb=sizeof(buf2);
		  if(RegQueryValue(hk, nullStr, buf2, &cb)==ERROR_SUCCESS && *buf2!=0)
		  { if(RegOpenKey(HKEY_CLASSES_ROOT, buf2, &hk2)==ERROR_SUCCESS)
			 { cb=sizeof(cLine);
				if(RegQueryValue(hk2, "shell\\open\\command", cLine, &cb)==ERROR_SUCCESS && *cLine!=0)
				{ if((p=strrchr(cLine, ' '))!=NULL) *p=0;
				}
				RegCloseKey(hk2);
			 }
		  }
		  RegCloseKey(hk);
		}
		if (*cLine==0) return FALSE;
	 }
	 if((caret=strchr(cLine, '^'))!=NULL) *caret=0;
	 strcat(cLine, spcStr);
  }

  if(isExe && OpenFile(pw.custHelpFile, &ofs, OF_EXIST|OF_SEARCH)!=HFILE_ERROR)
	 strcpy(cLine, pw.custHelpFile);
  else
	 strcat(cLine, getFullFileName(pw.custHelpFile, curActPath));

  if(*pw.hlpTopic) strcat(strcat(cLine, spcStr), pw.hlpTopic);

  return (WinExec(cLine, SW_SHOW)>=32);
}

void MainWindow::setCaption(BOOL fn)
{

  if(!menuVisible) return;

  strcpy(mainCapTxt, getStringTbl(IDS_TITOL));

  if(fn) strcat(strcat(mainCapTxt, " - "),
					 *pw.fileName!=0 ? pw.fileName : getStringTbl(IDS_SENSENOM));
  if(*pac.fileName) strcat(strcat(strcat(mainCapTxt, " ["), pac.fileName), "]");

#ifdef DEBUG
  { int heapfree;
	 char xx[20];
	 heapfree=LocalCompact(15000);

	 strcat(strcat(mainCapTxt, " FREE: "), itoa(heapfree, xx, 10));
  }
#endif

  SetWindowText(hWnd, (LPSTR)mainCapTxt);
}

BOOL MainWindow::reporta(int rp)
{
  static BOOL noFile=FALSE;

  switch(rp)
  { case BEGIN_REPORT:
			if(reporting) reporta(END_REPORT);
			r.clear();

			if(!doReportFile || noFile || (hConv==NULL && InitMsgDB()==FALSE))
			{noFile=TRUE; return FALSE;}

			EnableWindow(hWnd, FALSE);
			SendMsgDB("BEGIN_REPORT");
			EnableWindow(hWnd, TRUE);
			SetActiveWindow(hWnd);
			if(showUserReports && hConv) EMI(IDM_CLICDB, TRUE);

			reporting=TRUE;
			break;

	 case LINE_REPORT:
			if(pw.reported || pw.noAv || pw.noEvaluable()) return FALSE;
			r.addReport(pw.fileName, digit[TEMPS].getVal(), digit[INTENTS].getVal(),
				 digit[ENCERTS].getVal(), pw.movToSolv, pw.solved,
				 pw.noEvaluable());
			pw.reported=TRUE;

			if(doReportFile && !noFile)
			  SendMsgDB(r.getFRep());

			break;

	 case END_REPORT:
			if(!reporting) return FALSE;

			if(doReportFile && !noFile)
			  SendMsgDB("END_REPORT");

			EMI(IDM_CLICDB, FALSE);
			reporting=FALSE;
			break;

	 case ACTIVATE_CLICDB:
			if(!reporting) return FALSE;
			if(doReportFile && !noFile)
			  SendMsgDB("CLICDB");
			break;

	 default: break;
  }

  return TRUE;
}

#pragma argsused
HDDEDATA EXPENTRY _export DDECallback(WORD wType, WORD wFmt, HCONV hConvX,
			HSZ hsz1, HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2)
{
  switch (wType)
  { case XTYP_DISCONNECT:
			// El servidor ha desconnectat
			hConv = NULL;
			BMsgBox(NULL, IDS_DDEDISCON, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
			break;

	 case XTYP_ERROR:
			BMsgBox(NULL, IDS_DDEERR, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
			break;
	}

	return((HDDEDATA)NULL);
}

BOOL MainWindow::InitMsgDB()
{
  char cmd[MAX_FULL_FILENAME*3];
  struct ffblk fileInfo;

  if(hConv) return TRUE;

  if(findfirst(gv->dbFile, &fileInfo, 0)!=0)
  { BMsgBox(hWnd, IDS_RFAIL1, IDS_RFAIL2, gv->dbFile);
	 return FALSE;
  }

  strcpy(cmd, getFullFileName(DBFILE, gv->clicPath));
  strcat(strcat(cmd, " DB:"), gv->dbFile);
  if (gv->playerId) strcat(strcat(cmd, " USER:"), gv->playerId);
  if (!allowNewUsr) strcat(cmd, " NONEWUSR");
  if (!allowNewGrp) strcat(cmd, " NONEWGRP");

  if(WinExec(cmd, SW_HIDE)<32)
	 BMsgBox(NULL, IDS_DDEFAIL, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
  else
  { if(idInst==NULL && lpDdeProc==NULL)
	 { lpDdeProc = MakeProcInstance ((FARPROC)DDECallback, hInst);
		if(DdeInitialize((LPDWORD)&idInst,(PFNCALLBACK)lpDdeProc, APPCMD_CLIENTONLY, 0L)!=DMLERR_NO_ERROR)
		{ BMsgBox(NULL, IDS_DDEERR, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
		  return FALSE;
		}

		hszService = DdeCreateStringHandle(idInst, "clicrep", CP_WINANSI );
		hszTopic = DdeCreateStringHandle(idInst, "infoclic", CP_WINANSI );

		hConv = DdeConnect (idInst, hszService, hszTopic, (PCONVCONTEXT)NULL );
		if(hConv == NULL) BMsgBox(NULL, IDS_DDEERR, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
	 }
  }

  return hConv!=NULL;
}


BOOL MainWindow::SendMsgDB(char *msg)
{
  HDDEDATA hRetData;

  if(hConv==NULL || msg==NULL) return FALSE;

  hData = DdeCreateDataHandle(idInst, msg, 100, 0L, NULL, CF_TEXT, 0);
  if(hData==NULL ) {NOMEM; return FALSE;}
  hRetData = DdeClientTransaction((LPBYTE)hData, -1, hConv, NULL, CF_TEXT, XTYP_EXECUTE, /*TIMEOUT_ASYNC*/-2, NULL);
  if(hRetData!=(HDDEDATA)TRUE)
	 BMsgBox(NULL, IDS_DDEERR, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);

  return TRUE;
}

#ifndef NOEDIT
BOOL MainWindow::checkForSave(BOOL p)
{
  BOOL result=FALSE;
  static char fn[30];
  char *f;

  if(!editable || (!pw.loaded && !p) || (!pw.modified && !p)) return TRUE;
  f=(p ? pac.fileName : pw.fileName);

  strcpy(fn, (*f!=0 ? f : getStringTbl(IDS_SENSENOM)));
  switch(BMsgBox(hWnd, IDS_ALERT1, IDS_ALERT0, 0, fn,
		 MB_YESNOCANCEL|MB_ICONEXCLAMATION))
  { case IDCANCEL:
			break;

	 case IDNO:
			result=TRUE;
			break;

	 case IDYES:
			result= (p ? pac.save(TRUE) : pw.save(TRUE));
			break;

	 default: break;
  }
  return result;
}
#else
BOOL MainWindow::checkForSave(BOOL) {return TRUE;}
#endif

#define WPPS WritePrivateProfileString
BOOL saveGlobalVars()
{
#ifndef RUNTIME
  char txt[10];
  char sino[2][30];

  strcpy(sino[0], getStringTbl(IDS_FALSE));
  strcpy(sino[1], getStringTbl(IDS_TRUE));

  if(!WPPS(Clic, getStringTbl(IDS_PROFRPATH), gv->dbFile, gv->iniFile))
	 return FALSE;

  WPPS(Clic, getStringTbl(IDS_PROFDIR), workPath, gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_EDITEXT), gv->ediText, gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_EDIGRAF), gv->ediGraf, gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFSTEPS), itoa(passos, txt, 10), gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFAMPFIL), itoa(gruixfil, txt, 10), gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFQUICK), sino[(quick!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFFIL), sino[(arrossega!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFSO), sino[(sonor!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFREP), sino[(doReportFile!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFAUTOC), sino[(autoCursor!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFAUTOD), itoa(autoCursorDelay, txt, 10), gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFCGRAN), sino[(cursorGran!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFHOH), sino[(helponhelp!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFMENU), sino[(menus!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFEXIT), sino[(exitDirect!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFNU), sino[(allowNewUsr!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFNG), sino[(allowNewGrp!=0)], gv->iniFile);
  WPPS(Clic, getStringTbl(IDS_PROFINFUSR), sino[(showUserReports!=0)], gv->iniFile);
#endif
  return TRUE;
}

BOOL initGlobalVars(LPSTR lpCmdLine)
{
  int i;
  char *tx, *p, *pf, *next;
  char modulePath[MAX_FULL_FILENAME];
  BOOL forceReport=FALSE;

  detectCD();

  GetModuleFileName(_hInstance, tempPath3, MAX_FULL_FILENAME-1);
  strcpy(modulePath, tempPath3);
  setPathVar(tempPath3, curPacPath);
  if((gv->clicPath=dupStr(curPacPath))==NULL) return FALSE;

  if(lpCmdLine!=NULL)
  { tx=new char [_fstrlen(lpCmdLine)+1];
	 _fstrcpy(tx, lpCmdLine);
	 p=tx;
	 while(*p!=0)
	 { while(*p==' ') p++;
		if(*p==0) break;
		if(*p=='"')
		{ p++;
		  pf=strchr(p, '"');
		  if(pf) { *pf=0; next=pf+1; }
		  else next=NULL;
		}
		else
		{ pf=strchr(p, ' ');
		  if(pf) { *pf=0; next=pf+1; }
		  else next=NULL;
		}
		if(*p=='/')
		{ if(strnicmp(p, "/USER:", 6)==0)
		  { if(gv->playerId==NULL) gv->playerId=dupStr(p+6);
			 forceReport=TRUE;
		  }
		  else if(strnicmp(p, "/INI:", 5)==0)
		  { if(gv->iniFile==NULL) gv->iniFile=dupStr(getFullFileName(p+5, gv->clicPath));}
		  else if(strnicmp(p, "/DB:", 4)==0)
		  { if(gv->dbFile==NULL) gv->dbFile=dupStr(getFullFileName(p+4, gv->clicPath));
			 forceReport=TRUE;
		  }
		  else if(strnicmp(p, "/NOMENU", 7)==0)
		  { forceMenu=2;}
		  else if(strnicmp(p, "/MENU", 5)==0)
		  { forceMenu=1;}
		}
		else if(stricmp(p, modulePath)!=0)
		{ if(*gv->fileName0)
			 strcat(strcat(gv->fileName0, spcStr), p);
		  else
			 strcpy(gv->fileName0, p);
		}
		if(next==NULL) break;
		p=next;
	 }
	 delete tx;
  }  if(gv->iniFile==NULL)
	 gv->iniFile=dupStr(getFullFileName(INIFILE, gv->clicPath));

  if(gv->dbFile==NULL)
  { GetPrivateProfileString(Clic, getStringTbl(IDS_PROFRPATH), "CLICDB.MDB",
									 tempPath3, MAX_FULL_FILENAME-1, gv->iniFile);
	 strcpy(curActPath, getFullFileName(tempPath3, gv->clicPath));
	 if((gv->dbFile=dupStr(curActPath))==NULL) return FALSE;
	 if((gv->originalDbFile=dupStr(curActPath))==NULL) return FALSE;
  }

#ifdef NOEDIT
  if(*gv->fileName0==0)
  { GetPrivateProfileString(Clic, "Pac", nullStr,
									 tempPath3, MAX_FULL_FILENAME-1, gv->iniFile);
	 if(*tempPath3!=0)
		strcpy(gv->fileName0, getFullFileName(tempPath3, gv->clicPath));
  }

#ifndef RUNTIME
// AUTOEXEC for legacy self-contained Clic executables.
  if(*gv->fileName0==0)
  { GetModuleFileName(_hInstance, tempPath3, MAX_FULL_FILENAME-1);
	 strcpy(gv->fileName0, tempPath3);
	 offsetBaseExe=MY_SIZE;
  }
#endif
#endif

  strcpy(helpFile, getFullFileName(HELPFILENAME, gv->clicPath));

  if((i=GetPrivateProfileString(Clic, getStringTbl(IDS_PROFDIR), nullStr,
				tempPath3, MAX_FULL_FILENAME-1, gv->iniFile))>0 &&
		i<(MAX_FULL_FILENAME-1))
	 setActivePath(tempPath3);
  else
	 setActivePath(curPacPath);

  strcpy(tempPath3, curPath);
  if((gv->originalPath=dupStr(tempPath3))==NULL) return FALSE;
  strcpy(workPath, curPath);
  strcpy(curActPath, curPath);
  strcpy(curPacPath, curPath);

  GetPrivateProfileString(Clic, getStringTbl(IDS_EDITEXT), "notepad.exe",
								  tempPath3, MAX_FULL_FILENAME-1, gv->iniFile);
  for(tx=tempPath3; *tx!=0; tx++) if(*tx>='A' && *tx<='Z') *tx+=('a'-'A');
  if((gv->ediText=dupStr(tempPath3))==NULL) return FALSE;

  GetPrivateProfileString(Clic, getStringTbl(IDS_EDIGRAF), "pbrush.exe",
								  tempPath3, MAX_FULL_FILENAME-1, gv->iniFile);
  for(tx=tempPath3; *tx!=0; tx++) if(*tx>='A' && *tx<='Z') *tx+=('a'-'A');
  if((gv->ediGraf=dupStr(tempPath3))==NULL) return FALSE;

  passos0=passos=GetPrivateProfileInt(Clic, getStringTbl(IDS_PROFSTEPS),
						DEF_CICLES, gv->iniFile);

  quick0=quick=getBoolVar(IDS_PROFQUICK, Clic, gv->iniFile, FALSE);
  editable=getBoolVar(IDS_PROFEDIT, Clic, gv->iniFile, TRUE);
  arrossega0=arrossega=getBoolVar(IDS_PROFFIL, Clic, gv->iniFile, TRUE);
  sonor0=sonor=getBoolVar(IDS_PROFSO, Clic, gv->iniFile, TRUE);
  midiAllowed=getBoolVar(IDS_PROFMIDI, Clic, gv->iniFile, TRUE);
  waveAllowed=getBoolVar(IDS_PROFWAVE, Clic, gv->iniFile, TRUE);
  mciAllowed=getBoolVar(IDS_PROFMCI, Clic, gv->iniFile, TRUE);
  maxim0=getBoolVar(IDS_PROFMAX, Clic, gv->iniFile, TRUE);
  drf0=doReportFile=forceReport ? TRUE : getBoolVar(IDS_PROFREP, Clic, gv->iniFile, FALSE);
  autoCursor=getBoolVar(IDS_PROFAUTOC, Clic, gv->iniFile, FALSE);
  autoCursorDelay=GetPrivateProfileInt(Clic, getStringTbl(IDS_PROFAUTOD),
													2, gv->iniFile);
  if(autoCursorDelay>40 || autoCursorDelay<1) autoCursorDelay=2;
  gruixfil=GetPrivateProfileInt(Clic, getStringTbl(IDS_PROFAMPFIL), 1, gv->iniFile);
  Fil::setWidth(gruixfil);
  cursorGran=getBoolVar(IDS_PROFCGRAN, Clic, gv->iniFile, FALSE);
  helponhelp=getBoolVar(IDS_PROFHOH, Clic, gv->iniFile, TRUE);
  menus=getBoolVar(IDS_PROFMENU, Clic, gv->iniFile, TRUE);
  printerBug=getBoolVar(IDS_PROFPBUG, Clic, gv->iniFile, FALSE);
#ifndef RUNTIME
  exitDirect=getBoolVar(IDS_PROFEXIT, Clic, gv->iniFile, FALSE);
#else
  exitDirect=getBoolVar(IDS_PROFEXIT, Clic, gv->iniFile, TRUE);
#endif
  allowNewUsr=getBoolVar(IDS_PROFNU, Clic, gv->iniFile, TRUE);
  allowNewGrp=getBoolVar(IDS_PROFNG, Clic, gv->iniFile, FALSE);
  showUserReports=getBoolVar(IDS_PROFINFUSR, Clic, gv->iniFile, FALSE);

  if(getBoolVar(IDS_PROFTEST, Clic, gv->iniFile, TRUE))
  { HDC hDC;
	 hDC = GetDC(NULL);
	 if(GetDeviceCaps(hDC, HORZRES)>=800) ok800=TRUE;
	 if(GetDeviceCaps(hDC, BITSPIXEL)>=8) ok256=TRUE;
	 ReleaseDC(NULL, hDC);
  }
  else
  { ok800=TRUE; ok256=TRUE;}

  return TRUE;
}


BOOL getBoolVar(int ids, char *itn, char *fn, BOOL defVal)
{
  char defBuff[2][30];
  char buff[30];

  strcpy(defBuff[0], getStringTbl(IDS_FALSE));
  strcpy(defBuff[1], getStringTbl(IDS_TRUE));

  return GetPrivateProfileString(itn, getStringTbl(ids), defBuff[defVal],
				 buff, sizeof(buff)-1, fn) > 0
	 && stricmp(defBuff[TRUE], buff)==0;
}


BOOL CALLBACK _export PrintDlgProc(HWND hDlg, UINT message, UINT, LONG)
{
  switch(message)
  { case WM_INITDIALOG:
			EnableMenuItem(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_GRAYED);
			return TRUE;

	 case WM_COMMAND:
			bUserAbort=TRUE;
			EnableWindow(GetParent(hDlg), TRUE);
			DestroyWindow(hDlg);
			hDlgPrint=0;
			return TRUE;
  }
  return FALSE;
}


typedef BOOL (CALLBACK * ABORTPROCFN) (HDC, short);
BOOL CALLBACK _export AbortProc(HDC, short)
{
  MSG msg;

  while(!bUserAbort && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  { if(!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg))
	 { TranslateMessage(&msg);
		DispatchMessage(&msg);
	 }
  }
  return !bUserAbort;
}


BOOL printTitle=TRUE;
BOOL printBg=FALSE;
BOOL printComptadors=FALSE;
BOOL printBotons=FALSE;
BOOL printMarc=TRUE;
BOOL centrarH=FALSE;
BOOL centrarV=FALSE;
int escala=100;

typedef UINT (CALLBACK * LPPRINTHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
UINT CALLBACK _export PrintSetupDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch(message)
  { case WM_INITDIALOG:
			SendDlgItemMessage(hDlg, CHK_PRINTTITLE, BM_SETCHECK, printTitle, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTBG, BM_SETCHECK, printBg, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTCNT, BM_SETCHECK, printComptadors, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTBTN, BM_SETCHECK, printBotons, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTHCENT, BM_SETCHECK, centrarH, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTVCENT, BM_SETCHECK, centrarV, 0);
			SendDlgItemMessage(hDlg, CHK_PRINTMARC, BM_SETCHECK, printMarc, 0);
			SetDlgItemInt(hDlg, ED_ESCALA, escala, TRUE);
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case 1038: // Ajuda
					 if(WinHelp(genMsgOwn, helpFile, HELP_CONTEXT, HELP_PRINT)==TRUE)
						help=TRUE;
					 return TRUE;

			  case CHK_PRINTTITLE:
					 printTitle^=1;
					 break;

			  case CHK_PRINTBG:
					 printBg^=1;
					 break;

			  case CHK_PRINTCNT:
					 printComptadors^=1;
					 break;

			  case CHK_PRINTBTN:
					 printBotons^=1;
					 break;

			  case CHK_PRINTVCENT:
					 centrarV^=1;
					 break;

			  case CHK_PRINTHCENT:
					 centrarH^=1;
					 break;

			  case CHK_PRINTMARC:
					 printMarc^=1;
					 break;
			}
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  if(GetDlgCtrlID((HWND)HIWORD(lParam))==SB_ESCALA)
			  { sbi.staticId=ED_ESCALA; sbi.currVal=&escala;
				 sbi.maxVal=100, sbi.minVal=10;
				 if(dlgScrBar(hDlg, wParam, sbi, TRUE)) return TRUE;
			  }
			}
			break;
  }

  return FALSE;
}

int ajustaEscala(int n) {return (int)(((long)n*100)/escala);}

BOOL MainWindow::doPrint()
{
  PRINTDLG pd;
  DOCINFO di;
  HPEN hOldPen;
  short xPage, yPage, xsW, ysW;
  POINT p;
  int extraH, capH, margeX, margeY;
  FARPROC lpfnPrintDlgProc;
  ABORTPROCFN lpfnAbortProc;
  RECT xRect, r1, r2;
  BOOL result, bError=FALSE;
  int oldMapMode, i;

  memset(&pd, 0, sizeof(PRINTDLG));
  pd.lStructSize=sizeof(PRINTDLG);
  pd.hwndOwner=hWnd;
  pd.Flags=PD_ALLPAGES | PD_NOPAGENUMS | PD_NOSELECTION | PD_COLLATE |
			  PD_RETURNDC | PD_ENABLEPRINTTEMPLATE | PD_ENABLEPRINTHOOK | PD_SHOWHELP;
  pd.lpPrintTemplateName=MAKEINTRESOURCE(DIALEG_PRINTSETUP);
  pd.hInstance=hInst;
  pd.nCopies=1;

  di.cbSize=sizeof(DOCINFO);
  di.lpszDocName= (*pw.fileName!=0 ? pw.fileName : getStringTbl(IDS_SENSENOM));
  di.lpszOutput=NULL;

  pd.lpfnPrintHook=(LPPRINTHOOKPROC)MakeProcInstance((FARPROC)PrintSetupDlgProc, hInst);
  result=PrintDlg(&pd);
  FreeProcInstance((FARPROC)pd.lpfnPrintHook);
  if(!result) return TRUE;

  EnableWindow(hWnd, FALSE);

  bUserAbort=FALSE;
  lpfnPrintDlgProc=MakeProcInstance((FARPROC)PrintDlgProc, hInst);
  hDlgPrint=CreateDialog(hInst, MAKEINTRESOURCE(DIALEG_PRINT), hWnd, (DLGPROC)lpfnPrintDlgProc);

  lpfnAbortProc=(ABORTPROCFN)MakeProcInstance((FARPROC)AbortProc, hInst);

  SetAbortProc(pd.hDC, (ABORTPROC)lpfnAbortProc);

  hOldPen=(HPEN)SelectObject(pd.hDC, CreatePen(PS_SOLID|PS_INSIDEFRAME, 1, RGB(0, 0,0)));

  if(StartDoc(pd.hDC, &di)!=SP_ERROR &&
	  StartPage(pd.hDC)>0)
	 for(i=0; i<1; i++)
	 { lpfnAbortProc(pd.hDC, 0);
		if(bUserAbort) break;

		capH=printTitle ? BOTOH/2:0;
		margeX=margeY=BOTOH;
		extraH=pw.getExtraHeight();
		GetClientRect(hWnd, &xRect);
		xRect.bottom+=extraH;
		xsW=ajustaEscala(xRect.right+2*margeX);
		ysW=ajustaEscala(xRect.bottom+capH+2*margeY);

		xPage=GetDeviceCaps(pd.hDC, HORZRES);
		yPage=GetDeviceCaps(pd.hDC, VERTRES);

		oldMapMode=SetMapMode(pd.hDC, MM_ISOTROPIC);
		SetWindowExt(pd.hDC, xsW, ysW);
		SetViewportExt(pd.hDC, xPage, yPage);

		p.x=xPage; p.y=yPage;
		DPtoLP(pd.hDC, &p, 1);
		margeX= centrarH ? (p.x-xRect.right)/2 : ajustaEscala(margeX);
		margeY= centrarV ? (p.y-xRect.bottom-capH)/2 : ajustaEscala(margeY);

		lpfnAbortProc(pd.hDC, 0);
		if(bUserAbort) break;

		if(printTitle)
		{ HFONT hf, hOldFont;

		  hf=CreateFontIndirect(&logFont1);
		  hOldFont=(HFONT)SelectObject(pd.hDC, hf);
		  TextOut(pd.hDC, margeX, margeY, (LPSTR)mainCapTxt, strlen(mainCapTxt));
		  SelectObject(pd.hDC, hOldFont);
		  DelObj(hf);
		}
		SetWindowOrg(pd.hDC, -margeX, -margeY-capH);

		IntersectClipRect(pd.hDC, xRect.left, xRect.top, xRect.right, xRect.bottom);

		// Elements de pantalla

		if(printBg)
		{ if(pw.hBmpFons)
		  { if(pw.tileBmp)
				TileBmp(pd.hDC, 0, pw.hBmpFons, pw.bmpfW, pw.bmpfH, NullPoint, xRect);
			 else
				displayBitmap(pd.hDC, 0, pw.hBmpFons, pw.bmpfX, pw.bmpfY, pw.bmpfW, pw.bmpfH);
		  }
		  else
			 FillRect(pd.hDC, &xRect, Zona::getBaseBrush(GENERICA));
		}

		OffsetRect(&xRect, 0, -extraH);
		SetWindowOrg(pd.hDC, -margeX, -margeY-capH-extraH);

		if(!zones[3]->isEmpty() && zones[3]->isIn(&xRect))
		{ zones[3]->update(pd.hDC);
		  if(pw.marcs) drawLineRect(pd.hDC, zones[3]->getRect());
		}

		if(pw.barraAmunt && pw.marcs && printMarc)
		{ SetRect(&r1, 0, 2*MARGE+BOTOH-extraH, clRect.right, 2*MARGE+BOTOH-extraH);
		  drawLineRect(pd.hDC, &r1);
		}

		if(printBotons)
		  for(i=0; i<NBOTONS; i++)
			 if(bVis[i])
				bwcb[i].drawTo(pd.hDC, 0, ((i<NBOTONS-2) && pw.barraAmunt) ? -extraH : 0);

		if(printComptadors)
		  for(i=0; i<3; i++)
			 if(pw.comptadors[i] && !zones[i]->isEmpty() && zones[i]->isIn(&xRect))
			 { zones[i]->update(pd.hDC);

				r1=*digit[i].getRect();
				r2=r1; r2.top-=16; r2.bottom=r2.top+14;

				displayBitmapRop3(Zona::getBC(MSB)->hBackBrush, Zona::getBC(MSB)->c.textColor,
								  pd.hDC, 0, MAKEINTRESOURCE(RETOLS),
								  r2.left, r2.top, 0, 14*i, 3*WDIGIT, 14, BLANCTRANSP);

				if(pw.marcs)
				{ drawLineRect(pd.hDC, &r1);
				  drawLineRect(pd.hDC, &r2);
				}
			 }

		OffsetRect(&xRect, 0, extraH);
		SetWindowOrg(pd.hDC, -margeX, -margeY-capH);

		lpfnAbortProc(pd.hDC, 0);
		if(bUserAbort) break;

		OffsetRect(&xRect, -pw.wRect.left, -pw.wRect.top);
		SetWindowOrg(pd.hDC, -margeX-pw.wRect.left, -margeY-capH-pw.wRect.top);

		if(pw.txc!=NULL && extraH) pw.txc->getRect()->bottom+=extraH;
		pw.processPaint(pd.hDC, NULL, &xRect, TRUE);
		if(pw.txc!=NULL && extraH) pw.txc->getRect()->bottom-=extraH;

		OffsetRect(&xRect, pw.wRect.left, pw.wRect.top);
		SetWindowOrg(pd.hDC, -margeX, -margeY-capH);

		if(pw.marcs)
		{ r1=pw.wRect;
		  r1.bottom+=extraH;
		  drawLineRect(pd.hDC, &r1);
		}

		if(printMarc) drawLineRect(pd.hDC, &xRect);

		SetMapMode(pd.hDC, oldMapMode);

		lpfnAbortProc(pd.hDC, 0);

		if(EndPage(pd.hDC)<0) {bError=TRUE;}
	 }
  else
	 bError=TRUE;


  if(!bError)
  { if(bUserAbort)
		AbortDoc(pd.hDC);
	 else
		EndDoc(pd.hDC);
  }

  if(!bUserAbort)
  { EnableWindow(hWnd, TRUE);
	 DestroyWindow(hDlgPrint);
  }

  FreeProcInstance(lpfnPrintDlgProc);
  FreeProcInstance((FARPROC)lpfnAbortProc);

  SelectObject(pd.hDC, hOldPen);
  DeleteDC(pd.hDC);
  if (pd.hDevMode != NULL) GlobalFree(pd.hDevMode);
  if (pd.hDevNames != NULL) GlobalFree(pd.hDevNames);

  bmpCont::flushDIBs();

  return bError || bUserAbort;
}


int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow)
{
  _hInstance=hInst; _hPrev=hPrev; _pszCmdline=lpCmdLine; _cmdShow=nShow;
  MainWindow *mw;
  int retVal=-1;
  HWND hwOtherClic, hwndAnother;
  GlobalVars gbv;

  randomize();
  gv=&gbv;

#ifdef NOEDIT
//  if(lpCmdLine==NULL || *lpCmdLine==0) return FALSE;
  exitDirect=TRUE;
#endif

  if(!initGlobalVars(lpCmdLine))
  { NOMEM;
	 return FALSE;
  }

#ifdef NOEDIT
  GetModuleFileName(_hInstance, tempPath3, MAX_FULL_FILENAME-1);
  if(*gv->fileName0 && stricmp(gv->fileName0, tempPath3)==0)
  { *gv->fileName0=0;
    offsetBaseExe=0L;
  }

#ifndef RUNTIME
  if(*gv->fileName0==0)
  {
	 char packageName[MAX_FULL_FILENAME]="";
	 OPENFILENAME ofn;
	 memset(&ofn, 0, sizeof(ofn));
	 ofn.lStructSize=sizeof(ofn);
	 ofn.lpstrFilter=filter;
	 ofn.lpstrFile=packageName;
	 ofn.nMaxFile=sizeof(packageName);
	 ofn.Flags=OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	 ofn.lpstrTitle="Abrir paquete de actividades Clic";
	 ofn.lpstrDefExt="pcc";
	 if(!GetOpenFileName(&ofn)) return FALSE;
	 strcpy(gv->fileName0, packageName);
  }
#endif
#endif

#ifndef RUNTIME
  if(*gv->fileName0!=0 &&
#ifdef NOEDIT
	  getExt(gv->fileName0)!=EXT_EXE &&
#endif
	  (hwOtherClic=FindWindow(CLICSTR, NULL))!=NULL)
  { if((hwndAnother=GetLastActivePopup(hwOtherClic))!=hwOtherClic)
	 { SetFocus(hwndAnother);
		BMsgBox(NULL, IDS_ANOTHERWND);
	 }
	 else
	 { SetFocus(hwOtherClic);
		ShowWindow(hwOtherClic, SW_SHOWMAXIMIZED);
		SendMessage(hwOtherClic, WM_COMMAND, IDM_OPENCMD, (LPARAM)gv->fileName0);
	 }
	 return 0;
  }

#endif

#ifdef RUNTIME
  initPlayerLanguage(gv->iniFile);
#endif

  if((mw=new MainWindow)!=0)
  { mw->create();
	 retVal=mw->run();
	 delete mw;
  }
  else NOMEM;

  return retVal;
}


#include "runtime-ui.inc"

#include "runtime-classic.inc"

#include "runtime-language.inc"
