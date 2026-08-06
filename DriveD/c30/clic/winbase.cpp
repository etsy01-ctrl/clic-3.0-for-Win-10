// ------------------------------------------
// WINBASE.CPP
// ------------------------------------------

#include "wcl.h"
#include "rcomu\clicres.h"
#include "zones.h"
#include <bwcc.h>
#include <string.h>

// ------------------------------------------
// Implementacio de WinBase
// ------------------------------------------

HINSTANCE WinBase::hInst = _hInstance;
HINSTANCE WinBase::hPrevInst = _hPrev;
LPSTR  WinBase::cmd = _pszCmdline;
int    WinBase::show = _cmdShow;
int    WinBase::captionY = GetSystemMetrics(SM_CYCAPTION);
#ifdef RUNTIME
int    WinBase::screenX = GetSystemMetrics(SM_CXFULLSCREEN);
int    WinBase::screenY = GetSystemMetrics(SM_CYFULLSCREEN)+captionY;
#else
//int    WinBase::screenX = GetSystemMetrics(SM_CXSCREEN);
//int    WinBase::screenY = GetSystemMetrics(SM_CYSCREEN);
int    WinBase::screenX = GetSystemMetrics(SM_CXFULLSCREEN);
int    WinBase::screenY = GetSystemMetrics(SM_CYFULLSCREEN);
#endif
int    WinBase::menuY =	GetSystemMetrics(SM_CYMENU);

// ------------------------------------------
// Implementacio de Window
// ------------------------------------------

HWND genMsgOwn=0;

Window *Window::winList = 0;

void Window::chain()
{ if(nextWin || prevWin) unchain();
  nextWin=winList; winList=this;
  if(nextWin) nextWin->prevWin=this;
}

void Window::unchain()
{ if(nextWin==0 && prevWin==0)
  { if(winList==this) winList=0;
    return;
  }
  if(nextWin) nextWin->prevWin=prevWin;
  if(prevWin) prevWin->nextWin=nextWin;
  else winList=nextWin;
  nextWin=0; prevWin=0;
}

BOOL Window::create(HWND hOwner)
{
  if(hWnd) destroy();
  hOwn=hOwner;
  hAcc=0;
  chain();
  if((registerClass()==FALSE && hPrevInst==0 && hOwn==0) ||
     createWindow()==FALSE)
  {unchain(); return FALSE;}
  return TRUE;
}

BOOL Window::destroy()
{
  unchain();
  if(hWnd && IsWindow(hWnd)) DestroyWindow(hWnd);
  unchain();
  hWnd=0;
  return TRUE;
}

UINT Window::run()
{ MSG msg;

  if(!hWnd) return 0;

  while(GetMessage(&msg, NULL, NULL, NULL) != 0)
  { if(!hAcc || !TranslateAccelerator(hWnd, hAcc, &msg))
    { TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return msg.wParam;
}

LRESULT Window::dispatch(UINT msg, WPARAM wParam, LPARAM lParam, int)
{ return DefWindowProc(hWnd, msg, wParam, lParam);}

LRESULT CALLBACK Window::wndProc(HWND hWindow, UINT msg,
				 WPARAM wParam, LPARAM lParam)
{
  Window *cur = Window::winList;
  while(cur!=0 && cur->hWnd!=hWindow) cur=cur->nextWin;
  return cur ? cur->dispatch(msg, wParam, lParam, 0)
	     : DefWindowProc(hWindow, msg, wParam, lParam);
}

// ------------------------------------------
// Implementacio de WindowBWCC
// ------------------------------------------

LRESULT WindowBWCC::dispatch(UINT msg, WPARAM wParam, LPARAM lParam, int bwcc)
{ return bwcc ? BWCCDefWindowProc(hWnd, msg, wParam, lParam)
			: DefWindowProc(hWnd, msg, wParam, lParam);}

// ------------------------------------------
// Implementacio ModalDialog
// ------------------------------------------

char helpFile[100]="";

ModalDialog *ModalDialog::dlgP[NMAXDLG]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

HWND ModalDialog::dlgW[NMAXDLG]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

DLGPROC ModalDialog::dlgFarProc=0;

int ModalDialog::nDlgs=0;

ModalDialog::ModalDialog(HWND hOwner)
{
  result=inHelp=FALSE;
  hOwn=hOwner;
  for(tableIndex=0; tableIndex<NMAXDLG; tableIndex++)
	 if(dlgP[tableIndex]==0)
	 { dlgP[tableIndex]=this; dlgW[tableIndex]=0; break;}

  if(tableIndex<NMAXDLG)
  { nDlgs++;
	 if(dlgFarProc==0)
		dlgFarProc=(DLGPROC)MakeProcInstance((FARPROC)ModalDialog::dlgProc, hInst);
  }
}

ModalDialog::~ModalDialog()
{
  if(tableIndex<NMAXDLG)
  { if(--nDlgs==0) {FreeProcInstance((FARPROC)dlgFarProc); dlgFarProc=0;}
	 dlgP[tableIndex]=0;
	 dlgW[tableIndex]=0;
	 hWnd=0;
  }
}

UINT ModalDialog::run()
{
  result=FALSE;

  if(tableIndex<NMAXDLG)
	 DialogBox(hInst, getDialogName(), hOwn, dlgFarProc);

  return result;
}

BOOL CALLBACK _export ModalDialog::dlgProc(HWND hDlg, UINT msg,
							WPARAM wParam, LPARAM lParam)
{
  int i;
  ModalDialog *curDlg=0;

  for(i=0; i<NMAXDLG; i++)
	 if(dlgW[i]==hDlg) {curDlg=dlgP[i]; break;}

  if(!curDlg)
	 for(i=0; i<NMAXDLG; i++)
		if(dlgW[i]==0 && dlgP[i]!=0)
		{ dlgW[i]=hDlg; curDlg=dlgP[i]; curDlg->hWnd=hDlg; break;}

  return curDlg ? curDlg->dispatch(msg, wParam, lParam):FALSE;
}

int ModalDialog::getHelpItem() {return 0;}

BOOL ModalDialog::dispatch(UINT msg, WPARAM wParam, LPARAM)
{
  if(msg==WM_COMMAND && wParam==IDHELP && getHelpItem()!=0 &&
	  WinHelp(hOwn, helpFile, HELP_CONTEXT, (DWORD) getHelpItem())==TRUE)
		 inHelp=TRUE;

  return FALSE;
}

void ModalDialog::end()
{ if(inHelp) WinHelp(hOwn, (LPSTR)0, HELP_QUIT, 0);
  EndDialog(hWnd, TRUE);
}

// --------------------------
// Implementacio de msgBox
// --------------------------

char msgtext[MAX_STRINGTBL_LEN+100];

void msgStr(int mainmsg, int message, char *endText, char *iniText)
{
  if(iniText) strcpy(msgtext, iniText); else *msgtext=0;
  if(mainmsg==-1) mainmsg=IDS_NOMEM;
  if(mainmsg) strcat(msgtext, getStringTbl(mainmsg));
  if(endText) strcat(msgtext, endText);
  if(message)
  { if(*msgtext) strcat(msgtext, "\n\n");
	 strcat(msgtext, getStringTbl(message));
  }
}

int msgBox(HWND hWnd, int mainmsg, int message,
		char *endText, char *iniText, UINT type)
{
  msgStr(mainmsg, message, endText, iniText);
  return MessageBox(hWnd ? hWnd : genMsgOwn,
			 msgtext, getStringTbl(IDS_TITOL), type);
}

char *newChar(int size)
{ char *txt;
  if((txt=new char[size])==0) NOMEM;
  return txt;
}

// --------------------------
// Implementacio de BMsgBox
// --------------------------

extern char msgtext[MAX_STRINGTBL_LEN+100];
void msgStr(int mainmsg, int message, char *endText, char *iniText);

int BMsgBox(HWND hWnd, int mainmsg, int message,
		 char *endText, char *iniText, UINT type)
{
  msgStr(mainmsg, message, endText, iniText);
  return BWCCMessageBox(hWnd ? hWnd : genMsgOwn,
			msgtext, getStringTbl(IDS_TITOL), type);
}

// ---------------------------------------------
// Processament de scrollBar en controls mixtes
// ---------------------------------------------


int *sbicurval=0;
int repetidor=0;

BOOL dlgScrBar(HWND hDlg, WPARAM wParam, ScrollBarItem scbi, BOOL soundWarn)
{
  int v, delta=-1;

  switch(wParam)
  { case SB_ENDSCROLL:
			sbicurval=0;
			repetidor=0;
			return TRUE;

	 case SB_LINEUP: delta=1;
	 case SB_LINEDOWN: break;
	 default:
			return FALSE;
  }

  if(sbicurval==scbi.currVal)
	 repetidor++;
  else {sbicurval=scbi.currVal; repetidor=0;}

  if(repetidor>=10) delta*=10;

  v=*sbicurval;
  if((v+=delta) > scbi.maxVal) {v=scbi.maxVal; if(soundWarn) avisSonor(0);}
  else if(v<scbi.minVal) {v=scbi.minVal; if(soundWarn) avisSonor(0);}
  *sbicurval=v;
  SetDlgItemInt(hDlg, scbi.staticId, v, FALSE);
  return TRUE;
}

// ------------------------------------------
// Implementacio de ModalControl
// ------------------------------------------

BOOL ModalControl::create(char *classn, HWND hOwn, UINT ident, DWORD style,
			  char *txt, int x, int y, int w, int h,
			  BOOL iVis, BOOL iDis, int iSet)
{ if(hWnd) destroy();
  hOwner=hOwn; CtrlId=ident;
  set=iSet;
  hWnd=CreateWindow(classn, txt,
						  style | WS_CHILD |
			 (iVis ? WS_VISIBLE:0)|(iDis ? WS_DISABLED:0),
			 x, y, w, h, hOwner, (HMENU)id(), hInst, NULL);

  //width=w+2; height=h+2;
  width=w; height=h;
  xPos=x; yPos=y;
  return (hWnd ? TRUE:FALSE);
}

BOOL ModalControl::destroy()
{ if(hWnd && IsWindow(hWnd)) DestroyWindow(hWnd);
  hWnd=0;
  return TRUE;
}

BOOL ModalControl::enable(BOOL action)
{ return (hWnd ? EnableWindow(hWnd, action):FALSE);}

BOOL ModalControl::show(BOOL action)
{ return (hWnd ? ShowWindow(hWnd, action ? SW_SHOW:SW_HIDE):FALSE);}

void ModalControl::move(int x, int y, int w, int h, BOOL repaint)
{ if(hWnd)
	MoveWindow(hWnd, x, y, w==-1 ? width:w, h==-1 ? height:h, repaint);
  xPos=x; yPos=y;
}

DWORD ModalControl::sendMess(UINT wMsg, WPARAM wParam, LPARAM lParam)
{ return (hWnd ? SendMessage(hWnd, wMsg, wParam, lParam):NULL);}

BOOL ModalControl::drawItem(WPARAM wParam, LPARAM lParam, HBITMAP hBmTile,
							int wTile, int hTile, BOOL tiled, int fixPosX, int fixPosY)
{
  DRAWITEMSTRUCT dris;
  HDC hMemDC;
  HBITMAP hBm, hOldBitmap;
  int xs;

  if(lParam==NULL) return FALSE;
  dris=*(LPDRAWITEMSTRUCT)lParam;

  //if(wParam!=CtrlId) return FALSE;
  //if(dris.itemAction==ODA_FOCUS) return FALSE;
  if(wParam!=CtrlId || dris.CtlType!=ODT_BUTTON || dris.CtlID!=CtrlId ||
	  dris.hDC==NULL || dris.itemAction==ODA_FOCUS ||
	  (dris.itemAction & (ODA_DRAWENTIRE | ODA_SELECT))==0 ||
	  dris.hwndItem!=hWnd)
	 return FALSE;

  if((hBm=LoadBitmap(_hInstance, MAKEINTRESOURCE(CtrlId+1500)))==NULL) return FALSE;

  if((hMemDC=CreateCompatibleDC(dris.hDC))==NULL)
  { DelObj(hBm);
	 return FALSE;}

  if(hBmTile==0 || tiled==FALSE)
  {
	 /*
	 HBRUSH hBrOld;
	 HPEN hPenOld;

	 SetBrushOrg(dris.hDC, (xPos+1) % 8, (yPos+1) % 8);
	 hBrOld=(HBRUSH)SelectObject(dris.hDC, Zona::getBaseBrush(GENERICA));
	 hPenOld=(HPEN)SelectObject(dris.hDC, GetStockObject(NULL_PEN));
	 //FillRect(dris.hDC, &dris.rcItem, Zona::getBaseBrush(GENERICA));
	 //Rectangle(dris.hDC, dris.rcItem.left-1, dris.rcItem.top-1, dris.rcItem.right+1, dris.rcItem.bottom+1);
	 //Rectangle(dris.hDC, -1, -1, width+2, height+2);
	 SelectObject(dris.hDC, hBrOld);
	 SelectObject(dris.hDC, hPenOld);
	 SetBrushOrg(dris.hDC, 0, 0);
	 */

	 FillRect(dris.hDC, &dris.rcItem, Zona::getBaseBrush(GENERICA));

  }

  if(hBmTile)
  { if(tiled)
	 { POINT pt;
		pt.x=-xPos; pt.y=-yPos;
		TileBmp(dris.hDC, hMemDC, hBmTile, wTile, hTile, pt, dris.rcItem);
	 }
	 else
		displayBitmap(dris.hDC, hMemDC, hBmTile, fixPosX-xPos, fixPosY-yPos, wTile, hTile);
  }

  hOldBitmap=(HBITMAP)SelectObject(hMemDC, hBm);
  xs=width*(2*set+((dris.itemState & ODS_SELECTED)!=0));


  if(set==2)
  { HBRUSH hBr, hOldBr;

	 hBr=CreateSolidBrush(Zona::bc[MSB].c.textColor);
	 hOldBr=(HBRUSH)SelectObject(dris.hDC, hBr);

	 BitBlt(dris.hDC, 0, 0, width, height, hMemDC, xs, height, BLANCTRANSP);

	 SelectObject(dris.hDC, hOldBr);
	 DelObj(hBr);
  }
  else
  { BitBlt(dris.hDC, 0, 0, width, height, hMemDC, xs, height, SRCAND);
	 BitBlt(dris.hDC, 0, 0, width, height, hMemDC, xs, 0, SRCPAINT);
  }

  SelectObject(hMemDC, hOldBitmap);
  DeleteDC(hMemDC);
  DelObj(hBm);

  return TRUE;
}


BOOL ModalControl::drawTo(HDC hDC, int deltaX, int deltaY)
{
  HBITMAP hBm;
  RECT r;

  if((hBm=LoadBitmap(_hInstance, MAKEINTRESOURCE(CtrlId+1500)))==0) return FALSE;

  r.left=xPos+deltaX; r.top=yPos+deltaY;
  r.right=r.left+width; r.bottom=r.top+height;

  if(set>0)
	 displayBitmapRop3((HBRUSH)GetStockObject(HOLLOW_BRUSH), Zona::bc[MSB].c.textColor,
							 hDC, (HDC)0, hBm, r.left, r.top, width, height,
							 4*width, height, width, height, BLANCTRANSP);
  else
  { displayBitmap(hDC, 0, hBm, r.left, r.top, width, height, 0, 0, width, height);
	 drawLineRect(hDC, &r);
  }

  DelObj(hBm);

  return TRUE;
}

// ------------------------------------------
// Implementacio de setCursor i resetCursor
// ------------------------------------------

static HCURSOR hSaveCursor=NULL;

void setCursor(HWND hWnd, CURSOR_TIPUS op, HCURSOR hCur)
{
  HCURSOR hCursor;

  SetCapture(hWnd);
  switch(op)
  { case HANDLE_CURSOR: hCursor=hCur; break;
	 case CURSOR_ESPERA: hCursor=LoadCursor(NULL, IDC_WAIT); break;
	 case CURSOR_FLETXA: hCursor=LoadCursor(NULL, IDC_ARROW); break;
	 default: hCursor=0; break;
  }

  hSaveCursor=SetCursor(hCursor);
}

void resetCursor(void)
{
  if(hSaveCursor) SetCursor(hSaveCursor);
  ReleaseCapture();
  hSaveCursor=NULL;
}

// ------------------------------------------
// Implementacio de getStringTbl
// ------------------------------------------

static char stringBuff[MAX_STRINGTBL_LEN+50];

char *getStringTbl(UINT wID)
{
  if(LoadString(_hInstance, wID, stringBuff, MAX_STRINGTBL_LEN+49)==0)
	 *stringBuff=0;
  return stringBuff;
}


