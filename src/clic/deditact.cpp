// ----------------------------------------------------------
// DEDITACT.CPP
// Implementacio de les classes DEditAct, DAssign i DEdPaquet
// ----------------------------------------------------------

#ifndef NOEDIT

#include "deditact.h"
#include "dfonts.h"
#include "dactopt.h"
#include "clicdlg.h"
#include "miscfunc.h"
#include "rcomu\clicres.h"
#include "rcomu\clich.h"

#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <dos.h>

int botTipus[4]={BOTO_INTERC, BOTO_FORAT, BOTO_DOBLE, BOTO_MEMORY};
int codTipus[4]={INTERC, FORAT, DOBLE, MEMORY};
int posTipus[7]={STATIC1, STATIC2, STATIC3, CHK_SW0, CHK_SW1, CHK_SW2, CHK_SW3};
int posTipusMem[3]={STATIC3, CHK_SW1, CHK_SW3};

WNDPROC DEditAct::lpfnEditGra=0L;
WNDPROC DEditAct::lpfnOldEditGra=0L;

int DEditAct::lEditLin=0;
TxContenidor *DEditAct::txc=NULL;
ActFile *DEditAct::afp=NULL;

enum processTypes {PROCESS_IT, PASS_TO_APP, DONT_PROCESS};

LRESULT CALLBACK DEditAct::HandleEditGra(HWND hWindow, UINT msg,
					 WPARAM wParam, LPARAM lParam)
{
  int i, process, avant=1;

  process=PASS_TO_APP;

  switch(msg)
  { case WM_LBUTTONDBLCLK:
			process=DONT_PROCESS;
			break;

	 case WM_LBUTTONUP:
			process=PROCESS_IT;
			break;

	 case WM_KEYDOWN:
			switch(wParam)
			{ case VK_TAB:
					 process=PASS_TO_APP;
					 break;

			  case VK_LEFT:  avant=-2;
			  case VK_RIGHT:
			  case VK_UP:
			  case VK_DOWN:
					 process=PROCESS_IT;
					 break;

			  default:
					 process=DONT_PROCESS;
					 break;
			}
			break;

	 case WM_CHAR:
			switch(wParam)
			{ case VK_BACK:
			  case VK_RETURN:
					 process=DONT_PROCESS;
					 break;
			  case VK_SPACE:
					 wParam=COMODI;
			  default:
					 process=PROCESS_IT;
					 break;
			}
			break;

	 default: break;
  }

  switch(process)
  { case PROCESS_IT:
			CallWindowProc(lpfnOldEditGra, hWindow, msg, wParam, lParam);
			i=LOWORD(CallWindowProc(lpfnOldEditGra, hWindow, EM_GETSEL, 0, 0L));
			if(i<1) i=1;
			if(lEditLin && (i-1)%lEditLin>lEditLin-2) i+=avant;
			CallWindowProc(lpfnOldEditGra, hWindow, EM_SETSEL, 0, MAKELPARAM(i-1, i));
			return 0;

	 case DONT_PROCESS:
			return BWCCDefWindowProc(hWindow, msg, wParam, lParam);

	 case PASS_TO_APP:
			break;
  }

  return CallWindowProc(lpfnOldEditGra, hWindow, msg, wParam, lParam);
}



BOOL DEditAct::registerTxClass()
{
  WNDCLASS wc;

  wc.style =         CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc =   DEditAct::HandleEditText;
  wc.cbClsExtra =    0;
  wc.cbWndExtra =    0;
  wc.hInstance =     hInst;
  wc.hIcon =         NULL;
  wc.hCursor =       hGenCursor;
  wc.hbrBackground = NULL;
  wc.lpszMenuName =  NULL;
  wc.lpszClassName = "TxactContainer";

  return RegisterClass(&wc);
}


LRESULT CALLBACK DEditAct::HandleEditText(HWND hWnd, UINT msg,
					 WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_SETTEXT:
			if(txc==NULL)
			{ RECT r;
			  GetClientRect(hWnd, &r);
			  txc=new TxContenidor(GRA);
			  txc->create(hWnd, &r, FALSE, FALSE, GRA);
			}
			if(txc)
			{ txc->edit=TRUE;
			  txc->getActData(afp);
			}
			lParam=NULL;
			return 0;

	 case WM_SETFOCUS:
	 case WM_KILLFOCUS:
			checkCaret(hWnd, msg);
			if(txc) txc->getFocus(wParam==WM_SETFOCUS);
			return 0;

	 case WM_KEYDOWN:
	 case WM_KEYUP:
	 case WM_CHAR:
			if(txc) txc->processKeyboard(msg, wParam);
			return 0;

	 case WM_PAINT:
			if(txc)
			{ HDC hDC, hMemDC;
			  PAINTSTRUCT ps;

			  hDC=BeginPaint(hWnd, &ps);
			  hMemDC=CreateCompatibleDC(hDC);
			  if(caretVis) HideCaret(hWnd);

			  txc->update(hDC, &ps.rcPaint, hMemDC);

			  if(caretVis) ShowCaret(hWnd);
			  DeleteDC(hMemDC);
			  EndPaint(hWnd, &ps);
			}
			return 0;

	 case WM_GETDLGCODE:
			return DLGC_WANTALLKEYS | DLGC_HASSETSEL;

	 case WM_VSCROLL:
			if(txc) txc->processScroll(wParam, lParam);
			return 0;

	 case WM_LBUTTONDOWN:
			if (GetFocus()!=hWnd) SetFocus(hWnd);
	 case WM_LBUTTONUP:
	 case WM_RBUTTONDOWN:
	 case WM_RBUTTONUP:
	 case WM_MOUSEMOVE:
			if(txc)
			{ txc->processMouse(msg, wParam, lParam);
			  return 0;
			}
			break;


	 default: break;
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}

LPCSTR DEditAct::getDialogName()
{
  int i=0;
  switch(af.actMode)
  { case PUZZLE: i=DIALEG_EDIT_PUZ; break;
	 case ASSOCIA: i=DIALEG_EDIT_ASS; break;
	 case SOPA: i=DIALEG_EDIT_SOP; break;
	 case CREUATS: i=DIALEG_EDIT_CRW; break;
	 case TEXTACT: i=DIALEG_EDIT_TEXT; break;
	 default: return 0;
  }
  return MAKEINTRESOURCE(i);
}

int DEditAct::getHelpItem()
{ int i;

  switch(af.actMode)
  { case PUZZLE:  i=HELP_EDITPUZ; break;
	 case ASSOCIA: i=HELP_EDITASS; break;
	 case SOPA:    i=HELP_EDITSOP; break;
	 case CREUATS: i=HELP_EDITCRW; break;
	 case TEXTACT: i=HELP_EDITTXA; break;
	 default: i=HELP_CLIC_INDEX;
  }
  return i;
}

DEditAct::DEditAct(HWND hOwner, ActFile *actf):ModalDialog(hOwner)
{
  destActf=actf;
  if(destActf) af=*destActf;
  hEditW[0]=hEditW[1]=hEditW[2]=0;
  detectExtern=-1;
  filesBox[0]=filesBox[1]=filesBox[2]=FALSE;
  setActivePath(curActPath);
  caretX0=caretX; caretY0=caretY;
  caretW0=caretW; caretH0=caretH;
  txc=NULL;
  afp=&af;
}

DEditAct::~DEditAct()
{
  if(txc)
  { delete txc; txc=NULL;
	 caretX=caretX0; caretY=caretY0;
	 caretW=caretW0; caretH=caretH0;
  }
}

void DEditAct::initDlg()
{
  int i;

  SendDlgItemMessage(hWnd, ED_INIFRAS, EM_LIMITTEXT, MAX_MESS_LEN, 0);
  SendDlgItemMessage(hWnd, ED_INIFRAS, WM_SETTEXT, 0,
							(LPARAM)(LPSTR)af.initMess);
  SendDlgItemMessage(hWnd, ED_ENDFRAS, EM_LIMITTEXT, MAX_MESS_LEN, 0);
  SendDlgItemMessage(hWnd, ED_ENDFRAS, WM_SETTEXT, 0,
							(LPARAM)(LPSTR)af.endMess);

  if(af.actMode==PUZZLE)
	 for(i=0; i<4; i++)
		SendDlgItemMessage(hWnd, botTipus[i], BM_SETCHECK,
								 af.puzMode==codTipus[i], 0L);

  if(af.actMode!=CREUATS && af.actMode!=TEXTACT)
	 for(i=0; i<(af.actMode==ASSOCIA ? 2:1); i++)
	 { filesBox[i]=TRUE;
		SendDlgItemMessage(hWnd, i ? CHK_DELIM2:CHK_DELIM,
								 BM_SETCHECK, af.delim[i], 0L);
		resetFileBox(i);
	 }

  if(af.actMode==ASSOCIA)
  { filesBox[2]=TRUE;
	 resetFileBox(2);
	 changeAssMode();
  }

  if(af.actMode==SOPA || af.actMode==CREUATS)
  { initGra();
	 initTagsBox(0);
	 if(af.actMode==CREUATS) initTagsBox(1);
	 setFuncHandles(TRUE);
  }

  if(af.actMode==TEXTACT)
  { Zona::setBaseCont(&af.logF[GRA], &af.colors[GRA], GRA);
	 Zona::setBaseCont(&af.logF[GRB], &af.colors[GRB], GRB);
	 Zona::setBaseCont(&af.logF[MSB], &af.colors[MSB], MSB);
	 Zona::setBaseCont(&af.logF[POP], &af.colors[POP], POP);
	 SendDlgItemMessage(hWnd, ED_EDTX, WM_SETTEXT, 0, (LPARAM)(LPSTR)af.txBase);
	 SendDlgItemMessage(hWnd, CHK_BRPARPAR, BM_SETCHECK, af.brPar, 0L);
	 SetDlgItemInt(hWnd, ED_TABSPC, af.tabSpc, TRUE);
  }

  resetDlg();
}

BOOL DEditAct::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  int i, cwbk, chbk;

  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_ENTERIDLE:
			if(lParam!=0)
			  PostMessage((HWND)lParam, WM_ENTERIDLE, 0, 0);
			return FALSE;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  getGra();
			  cwbk=af.nctxw;
			  chbk=af.nctxh;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_CASH; sbi.currVal=&af.ncw;
						sbi.maxVal=MAXCW, sbi.minVal=1;
						break;

				 case SB_CASV:
						sbi.staticId=ED_CASV; sbi.currVal=&af.nch;
						sbi.maxVal=MAXCH, sbi.minVal=1;
						break;

				 case SB_LLH:
						sbi.staticId=EDIT_LLH; sbi.currVal=&af.nctxw;
						sbi.maxVal=MAXGW, sbi.minVal=1;
						break;

				 case SB_LLV:
						sbi.staticId=EDIT_LLV; sbi.currVal=&af.nctxh;
						sbi.maxVal=MAXGH, sbi.minVal=1;
						break;

				 case SB_TABSPC:
						sbi.staticId=ED_TABSPC; sbi.currVal=&af.tabSpc;
						sbi.maxVal=100, sbi.minVal=1;
						break;

				 default: return FALSE;
			  }
			  if(dlgScrBar(hWnd, wParam, sbi, TRUE))
			  { if(txc) txc->newTabSpc(af.tabSpc);
				 else if(cwbk!=af.nctxw || chbk!=af.nctxh) initGra();
				 return TRUE;
			  }
			}
			break;

	 case WM_COMMAND:
			if(processCommand(wParam, lParam)) return TRUE;
			break;

	 case WM_ACTIVATEAPP:
			if (detectExtern!=-1 && wParam==FALSE)
			{ int i=detectExtern;
			  detectExtern=-1;
			  hEditW[i]=GetActiveWindow();
			}
			break;

	 case WM_ACTIVATE:
			if(LOWORD(wParam)!=WA_INACTIVE)
			  for(i=0; i<3; i++) if(filesBox[i]) resetFileBox(i);
			break;

	 default: break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}


void DEditAct::changeAssMode()
{
	int i;
	BOOL *enbl=0;

	#define NENBL 15
	static int ctrls[NENBL]=
	{ BOTO_BITMAP2, BOTO_TEXT2, COMBO_FITX2, BOTO_EDITCONT2,
	  CHK_DELIM, CHK_DELIM2, CHK_BAR, CHK_BAR2,
	  ED_ENDFRAS,
	  CHK_SW0, CHK_SW1, CHK_SW2, CHK_SW3,
	  BOTO_ASSIGNA, BOTO_RES};

	static BOOL eNormal[NENBL]={TRUE, TRUE, TRUE, TRUE,
										 TRUE, TRUE, TRUE, TRUE,
										 TRUE,
										 TRUE, TRUE, TRUE, TRUE,
										 FALSE, TRUE};

	static BOOL eSpecial[NENBL]={TRUE, TRUE, TRUE, TRUE,
										  TRUE, TRUE, TRUE, TRUE,
										  TRUE,
										  TRUE, TRUE, TRUE, TRUE,
										  TRUE, TRUE};

	static BOOL eExplora[NENBL]={TRUE, TRUE, TRUE, TRUE,
										  TRUE, TRUE, FALSE, FALSE,
										  FALSE,
										  TRUE, TRUE, TRUE, TRUE,
										  TRUE, FALSE};

	static BOOL eInfo[NENBL]={FALSE, FALSE, FALSE, FALSE,
									  TRUE, FALSE, FALSE, FALSE,
									  FALSE,
									  FALSE, FALSE, FALSE, FALSE,
									  FALSE, FALSE};

	static BOOL eIdent[NENBL]={FALSE, FALSE, FALSE, FALSE,
									  TRUE, FALSE, TRUE, FALSE,
									  TRUE,
									  FALSE, FALSE, FALSE, FALSE,
									  TRUE, TRUE};

	static BOOL eEscriu[NENBL]={FALSE, TRUE, TRUE, TRUE,
										 TRUE, FALSE, TRUE, FALSE,
										 TRUE,
										 TRUE, TRUE, TRUE, TRUE,
										 FALSE, TRUE};


	SendDlgItemMessage(hWnd, CHK_NORMAL, BM_SETCHECK, af.puzMode==NORMAL, 0L);
	SendDlgItemMessage(hWnd, CHK_ESPECIAL, BM_SETCHECK, af.puzMode==ESPECIAL, 0L);
	SendDlgItemMessage(hWnd, CHK_EXPLORA, BM_SETCHECK, af.puzMode==EXPLORA, 0L);
	SendDlgItemMessage(hWnd, CHK_INFO, BM_SETCHECK, af.puzMode==INFO, 0L);
	SendDlgItemMessage(hWnd, CHK_IDENT, BM_SETCHECK, af.puzMode==IDENTIFICA, 0L);
	SendDlgItemMessage(hWnd, CHK_ESCRIU, BM_SETCHECK, af.puzMode==ESCRIU, 0L);

	switch(af.puzMode)
	{ case NORMAL:
			 enbl=eNormal;
			 af.mAss=FALSE;
			 break;

	  case ESPECIAL:
			 enbl=eSpecial;
			 af.mAss=TRUE;
			 break;

	  case EXPLORA:
			 enbl=eExplora;
			 af.mAss=TRUE;
			 break;

	  case INFO:
			 enbl=eInfo;
			 af.mAss=FALSE;
			 break;

	  case IDENTIFICA:
			 enbl=eIdent;
			 af.mAss=TRUE;
			 break;

	  case ESCRIU:
			 enbl=eEscriu;
			 af.mAss=FALSE;
			 if(af.cont[1]!=EXT_TXT)
			 { af.cont[1]=EXT_TXT;
				SendDlgItemMessage(hWnd, BOTO_BITMAP2, BM_SETCHECK, FALSE, 0L);
				SendDlgItemMessage(hWnd, BOTO_TEXT2, BM_SETCHECK, TRUE, 0L);
				resetFileBox(1);
			 }
			 break;

	  default: break;
	}

	if(enbl)
	  for(i=0; i<NENBL; i++)
		 EnableWindow(GetDlgItem(hWnd,ctrls[i]), enbl[i]);
}


void DEditAct::resetDlg()
{

  BOOL slv;
  int i;

  if(af.actMode!=CREUATS && af.actMode!=TEXTACT)
  { SetDlgItemInt(hWnd, ED_CASH, af.ncw, FALSE);
	 SetDlgItemInt(hWnd, ED_CASV, af.nch, FALSE);
  }

  switch(af.actMode)
  { case PUZZLE:
			for(i=0; i<7; i++)
			  ShowWindow(GetDlgItem(hWnd, posTipus[i]), af.puzMode==DOBLE || af.puzMode==MEMORY);

			if(af.puzMode==MEMORY)
			{ if(af.graPos==BA) af.graPos=AB;
			  else if(af.graPos==BUA) af.graPos=AUB;
			  for(i=0; i<3; i++)
				 ShowWindow(GetDlgItem(hWnd, posTipusMem[i]), FALSE);
			}

			SendDlgItemMessage(hWnd, CHK_STRETCH, BM_SETCHECK, af.stretch, 0);
			SendDlgItemMessage(hWnd, CHK_SW0, BM_SETCHECK, af.graPos==AB, 0L);
			SendDlgItemMessage(hWnd, CHK_SW1, BM_SETCHECK, af.graPos==BA, 0L);
			SendDlgItemMessage(hWnd, CHK_SW2, BM_SETCHECK, af.graPos==AUB, 0L);
			SendDlgItemMessage(hWnd, CHK_SW3, BM_SETCHECK, af.graPos==BUA, 0L);
			break;

	 case ASSOCIA:
			SendDlgItemMessage(hWnd, CHK_BAR, BM_SETCHECK, af.bar[0], 0);
			SendDlgItemMessage(hWnd, CHK_BAR2, BM_SETCHECK, af.bar[1], 0);
			SendDlgItemMessage(hWnd, CHK_SW0, BM_SETCHECK, af.graPos==AB, 0L);
			SendDlgItemMessage(hWnd, CHK_SW1, BM_SETCHECK, af.graPos==BA, 0L);
			SendDlgItemMessage(hWnd, CHK_SW2, BM_SETCHECK, af.graPos==AUB, 0L);
			SendDlgItemMessage(hWnd, CHK_SW3, BM_SETCHECK, af.graPos==BUA, 0L);

			SendDlgItemMessage(hWnd, BOTO_RES, BM_SETCHECK, af.sol, 0L);
			slv=(af.puzMode==NORMAL || af.puzMode==ESPECIAL ||
				  af.puzMode==IDENTIFICA || af.puzMode==ESCRIU) && af.sol==TRUE;
			EnableWindow(GetDlgItem(hWnd, BOTO_BITMAP3), slv);
			EnableWindow(GetDlgItem(hWnd, BOTO_TEXT3), slv);
			EnableWindow(GetDlgItem(hWnd, COMBO_FITX3), slv);
			EnableWindow(GetDlgItem(hWnd, BOTO_EDITCONT3), slv);
			break;

	 case SOPA:
			SendDlgItemMessage(hWnd, CHK_DELIM2, BM_SETCHECK, af.delim[1], 0L);
			SendDlgItemMessage(hWnd, CHK_BAR, BM_SETCHECK, af.bar[0], 0L);
			enableSopIt();

	 case CREUATS:
			SetDlgItemInt(hWnd, EDIT_LLH, af.nctxw, FALSE);
			SetDlgItemInt(hWnd, EDIT_LLV, af.nctxh, FALSE);
			break;

	 case TEXTACT:
			SendDlgItemMessage(hWnd, CHK_FORATS, BM_SETCHECK, af.puzMode==FORATS, 0);
			SendDlgItemMessage(hWnd, CHK_COMPLTEXT, BM_SETCHECK, af.puzMode==COMPTEXT, 0);
			SendDlgItemMessage(hWnd, CHK_IDLLETRES, BM_SETCHECK, af.puzMode==IDLLETRES, 0);
			SendDlgItemMessage(hWnd, CHK_IDPARAULES, BM_SETCHECK, af.puzMode==IDPARAULES, 0);
			SendDlgItemMessage(hWnd, CHK_BPARAULES, BM_SETCHECK, af.puzMode==BPARAULES, 0);
			SendDlgItemMessage(hWnd, CHK_BPARAGRAFS, BM_SETCHECK, af.puzMode==BPARAGRAFS, 0);
			ShowWindow(GetDlgItem(hWnd, CHK_BRPARPAR), af.puzMode==BPARAULES);
			ShowWindow(GetDlgItem(hWnd, CHK_BRPARPAR2), af.puzMode==BPARAULES);
			ShowWindow(GetDlgItem(hWnd, FONS_BRPARPAR), af.puzMode==BPARAULES);
			if(txc) InvalidateRect(GetDlgItem(hWnd, ED_EDTX), NULL, TRUE);
			break;

	 default: break;
  }
}

BOOL DEditAct::enableSopIt()
{
  static int itemsToEnable[7]={BOTO_BITMAP, BOTO_TEXT, COMBO_FITX,
										 BOTO_EDITCONT, CHK_DELIM, SB_CASH, SB_CASV};
  int i;

  for(i=0; i<7; i++)
	 EnableWindow(GetDlgItem(hWnd, itemsToEnable[i]), af.bar[0]);

  SetDlgItemInt(hWnd, ED_CASH, af.bar[0] ? af.ncw:0, FALSE);
  SetDlgItemInt(hWnd, ED_CASV, af.bar[0] ? af.nch:0, FALSE);

  return TRUE;
}

void DEditAct::resetFileBox(int c)
{
  int idContTxt, idContBmp, idFitx;

  if(!filesBox[c]) return;

  idContTxt=(c==0 ? BOTO_TEXT:(c==1 ? BOTO_TEXT2:BOTO_TEXT3));
  idContBmp=(c==0 ? BOTO_BITMAP:(c==1 ? BOTO_BITMAP2:BOTO_BITMAP3));
  idFitx=(c==0 ? COMBO_FITX:(c==1 ? COMBO_FITX2:COMBO_FITX3));

  SendDlgItemMessage(hWnd, idContTxt, BM_SETCHECK, af.cont[c]==EXT_TXT, 0L);
  SendDlgItemMessage(hWnd, idContBmp, BM_SETCHECK, af.cont[c]<EXT_TXT, 0L);

  SendDlgItemMessage(hWnd, idFitx, CB_RESETCONTENT, 0, 0L);


  SendDlgItemMessage(hWnd, idFitx, CB_DIR, 0x0001,
							(LPARAM)getFullFileName(extName[af.cont[c]]));

  if(af.cont[c]==EXT_BMP || af.cont[c]==EXT_GIF)
	 SendDlgItemMessage(hWnd, idFitx, CB_DIR, 0x0001,
		 (LPARAM)getFullFileName(extName[af.cont[c]==EXT_BMP ? EXT_GIF:EXT_BMP]));

  SendDlgItemMessage(hWnd, idFitx, CB_INSERTSTRING, 0,
							(LPARAM)(LPSTR)getStringTbl(af.cont[c]==EXT_TXT ?
																 IDS_NOUTEXT : IDS_NOUBITMAP));

  if(SendDlgItemMessage(hWnd, idFitx, CB_SELECTSTRING, -1,
								(LPARAM)(LPSTR)af.fileNameCont[c])==CB_ERR)
	  SendDlgItemMessage(hWnd, idFitx, CB_SETCURSEL, 0, 0);
}

BOOL DEditAct::getFileName(int c)
{
  char tempFileName[15];

  if(!filesBox[c] ||
	  (c==2 && !af.sol) ||
	  (c==1 && (af.actMode==ASSOCIA &&
					(af.puzMode==INFO || af.puzMode==IDENTIFICA)))) return FALSE;

  *tempFileName=0;
  GetDlgItemText(hWnd, (c==0 ? COMBO_FITX:(c==1 ? COMBO_FITX2:COMBO_FITX3)), tempFileName,
					  sizeof(tempFileName));
  if(*tempFileName==0) return FALSE;
  //if(*tempFileName>'@' && *tempFileName<='Z')
  // Amb NT els noms son en majuscules. Busquem el punt.
  if(strchr(tempFileName, '.')==NULL)
  { af.fileNameCont[c][0]=0;
	 return TRUE;
  }
  strcpy(af.fileNameCont[c], tempFileName);
  if((af.cont[c]=getExt(tempFileName))>EXT_TXT)
  { af.cont[c]=EXT_BMP; return FALSE;}
  return TRUE;
}

BOOL DEditAct::editCont(int c)
{
  char *cmdLine=0;

  getFileName(0); getFileName(1); getFileName(2);
  if(getFileName(c)==FALSE) return FALSE;
  if(endEditCont(c)==TRUE) return FALSE;
  if(getExt(af.fileNameCont[c])==EXT_GIF && strstr(gv->ediGraf, "pbrush")!=NULL)
  { BMsgBox(hWnd, IDS_GIFEDIT, 0);
	 return FALSE;
  }

  if(af.fileNameCont[c][0]==0)
  { DFileName *dfn;

	 if((dfn=new DFileName(hWnd, af.cont[c]==EXT_TXT))!=NULL)
	 { if(dfn->run())
		  strcpy(af.fileNameCont[c], dfn->txBuffer);

		delete dfn;
	 }
	 else {NOMEM; return FALSE;}

	 if(af.fileNameCont[c][0]==0) return FALSE;
  }

  if((cmdLine=newChar(MAX_FULL_FILENAME+81))==0) return FALSE;
  strcat(strcat(strcpy(cmdLine, af.cont[c]==EXT_TXT ? gv->ediText:gv->ediGraf), spcStr),
			af.fileNameCont[c][0]==0 ?
			nullStr : getFullFileName(af.fileNameCont[c]));
  SetActiveWindow(hWnd);
  detectExtern=c;
  if(WinExec(cmdLine, SW_SHOWNORMAL)<32) detectExtern=-1;
  delete cmdLine;
  return TRUE;
}

BOOL DEditAct::endEditCont(int c)
{
  HWND hw=hEditW[c];

  hEditW[c]=0;

  if(hw!=0 && hw!=hWnd && IsWindow(hw))
  {
	 SetActiveWindow(hw);
	 //SendMessage(hw, WM_CLOSE, 0, 0L);
	 SendMessage(hw, WM_SYSCOMMAND, SC_CLOSE /*0xF060*/, 0L);
	 return IsWindow(hw);
  }
  return FALSE;
}


char *txLabelV="A\r\nB\r\nC\r\nD\r\nE\r\nF\r\nG\r\nH\r\nI\r\nJ\r\nK\r\nL\r\n"
					"M\r\nN\r\nO\r\nP\r\nQ\r\nR\r\nS\r\nT\r\nU\r\n";
char *txLabelH="1\r\n2\r\n3\r\n4\r\n5\r\n6\r\n7\r\n8\r\n9\r\n10\r\n";

void DEditAct::initGra()
{
  static char nl[2]={0x0D, 0x0A};
  char c, *txt, *txtp;
  int i, j, k, ltxt, llin, ltxlin;
  BOOL passat=FALSE;
  HGDIOBJ hf;

  lEditLin=af.nctxw+sizeof(nl);

  ltxlin=af.nctxw;
  llin=ltxlin+sizeof(nl);
  ltxt=af.nctxh*llin-sizeof(nl);
  if(af.graTxt!=0 && (txt=newChar(ltxt+1))!=0)
  { memset(txt, COMODI, ltxt);
	 for(i=0; i<af.nctxh; i++)
	 { if(af.graTxt[i]==0) passat=TRUE;
		if(!passat)
		  for(j=0, k=i*llin; (c=af.graTxt[i][j])!=0 && j<ltxlin; j++, k++)
			 txt[k]=c;
		txtp=txt+i*llin+ltxlin;
		if(i==af.nctxh-1) *txtp=0;
		else memcpy(txtp, nl, sizeof(nl));
	 }

	 hf=GetStockObject(SYSTEM_FIXED_FONT);
	 SendDlgItemMessage(hWnd, EDIT_TAULA, WM_SETFONT, (WPARAM)hf, TRUE);
	 SendDlgItemMessage(hWnd, EDIT_TAULA, EM_LIMITTEXT, ltxt, 0L);
	 SetDlgItemText(hWnd, EDIT_TAULA, txt);

	 if(af.actMode==CREUATS)
	 { SetDlgItemText(hWnd, TX_GRH, txLabelH);
		SetDlgItemText(hWnd, TX_TAGH, txLabelH);
		SetDlgItemText(hWnd, TX_TAGV, txLabelV);
		SendDlgItemMessage(hWnd, TX_GRH, WM_SETFONT, (WPARAM)hf, TRUE);
		SendDlgItemMessage(hWnd, TX_GRV, WM_SETFONT, (WPARAM)hf, TRUE);
	 }

	 delete txt;
  }
}

BOOL DEditAct::getGra()
{
  int i, j;
  BOOL result=TRUE, noAst=FALSE;

  if(af.graTxt) delArray(af.graTxt);
  if((af.graTxt=newArray(af.nctxh))==0) return FALSE;

  for(i=0; i<af.nctxh; i++)
  { if((af.graTxt[i]=newChar(af.nctxw+2))==0) return FALSE;
	 memset(af.graTxt[i], 0, af.nctxw+2);
	 *(int *)(af.graTxt[i])=af.nctxw;
	 if(SendDlgItemMessage(hWnd, EDIT_TAULA, EM_GETLINE, i,
								(LPARAM)(LPSTR)af.graTxt[i])!=af.nctxw) result=FALSE;
	 if(!noAst)
		for(j=0; j<af.nctxw; j++)
		  if(af.graTxt[i][j]!='*') {noAst=TRUE; break;}
  }
  return result && noAst;
}

BOOL DEditAct::getTagsBox(int c)
{
  int l, i, j, k, itemId=(c==0 ? LIST_TAGS:LIST_TAGS2);

  delArray(af.tags[c]);
  af.ntags[c]=(int) SendDlgItemMessage(hWnd, itemId, EM_GETLINECOUNT, 0, 0L);
  if((j=af.ntags[c])>0)
  { if((af.tags[c]=newArray(j))==0) return FALSE;
	 for(i=0, k=0; i<j; i++)
	 { l=(int)SendDlgItemMessage
		  (hWnd, itemId, EM_LINELENGTH,
			(int)SendDlgItemMessage(hWnd, itemId, EM_LINEINDEX, i, 0L),
			0L);
		if(l==0) {af.ntags[c]--; continue;}
		if(l<0 || (af.tags[c][k]=newChar(l+2))==0) return FALSE;
		memset(af.tags[c][k], 0, l+2);
		*(int *)(af.tags[c][k])=l;
		SendDlgItemMessage(hWnd, itemId, EM_GETLINE, i,
				(LPARAM)(LPSTR)af.tags[c][k]);
		k++;
	 }
  }
  else return FALSE;

  return (af.ntags[c]>0);
}

BOOL DEditAct::initTagsBox(int c)
{
  static char nl[3]={0x0D, 0x0A, 0};
  char *txt, *txt0;
  int i, l;

  if(af.ntags[c]==0 || af.tags[c]==0) return FALSE;

  for(l=0, i=0; i<af.ntags[c]; i++) l+=(strlen(af.tags[c][i])+2);
  if((txt=newChar(l+1))!=0)
  { for(*txt=0, i=0, txt0=txt; i<af.ntags[c]; i++)
	 { txt0=stpcpy(txt0, af.tags[c][i]);
		txt0=stpcpy(txt0, nl);
	 }
	 SetDlgItemText(hWnd, (c==0 ? LIST_TAGS : LIST_TAGS2), txt);
	 delete txt;
  }
  else return FALSE;

  return TRUE;
}

BOOL DEditAct::checkOk()
{
  int result=FALSE;
  int i, nco, ncd, msg=0;

  if(af.actMode==CREUATS || af.actMode==TEXTACT || (af.actMode==SOPA && !af.bar[0]) ||
	  (af.actMode==ASSOCIA && af.puzMode==INFO && af.ncw>0 && af.nch>0) ||

	  (af.actMode==ASSOCIA && (af.puzMode==ESCRIU || af.puzMode==EXPLORA || af.puzMode==ESPECIAL) && (af.ncw * af.nch)==1) ||

	  checkCas(hWnd, af.ncw, af.nch, MAXCW, MAXCH, ED_CASH, ED_CASV)==TRUE)
  switch(af.actMode)
  { case ASSOCIA:
			if((!getFileName(0) || !*af.fileNameCont[0]) ||
				(af.puzMode!=INFO && af.puzMode!=IDENTIFICA &&
				 (!getFileName(1) || !*af.fileNameCont[1])) ||
				(af.sol && (!getFileName(2) || !*af.fileNameCont[2])))
			{ msg=IDS_ERRNOFILE; break;}

			if(af.mAss)
			{ if(af.puzMode==IDENTIFICA) {af.nctxw=1; af.nctxh=2;}
			  else
			  { if(!af.nctxw) af.nctxw=af.ncw;
				 if(!af.nctxh) af.nctxh=af.nch;
			  }
			  nco=af.ncw*af.nch;
			  ncd=af.nctxw*af.nctxh;
			  for(i=0; i<nco; i++)
				 if(af.ass[i]==-2) af.ass[i]=(i<ncd ? i:0);
			}
			result=TRUE;
			break;

	 case PUZZLE:
			if(!getFileName(0) || !*af.fileNameCont[0]) msg=IDS_ERRNOFILE;
			else result=TRUE;
			break;

	 case SOPA:
			if(!checkCas(hWnd, af.nctxw, af.nctxh,
							 MAXGW, MAXGH, EDIT_LLH, EDIT_LLV)) break;

			if(af.bar[0] && (!getFileName(0) || !*af.fileNameCont[0]))
			{ msg=IDS_ERRNOFILE; break;}

			if(!getGra()) {msg=IDS_ERRGRA; break;}
			if(!getTagsBox(0)) {msg=IDS_ERRTAGS0; break;}
			result=TRUE;
			break;

	 case CREUATS:
			if(!checkCas(hWnd, af.nctxw, af.nctxh,
							 MAXGW, MAXGH, EDIT_LLH, EDIT_LLV)) break;

			if(!getGra()) {msg=IDS_ERRGRA; break;}
			if(!getTagsBox(0) || af.ntags[0]!=af.nctxh)
			{ msg=IDS_ERRTAGS1; break;}
			if(!getTagsBox(1) || af.ntags[1]!=af.nctxw)
			{ msg=IDS_ERRTAGS2; break;}

			result=TRUE;
			break;

	 case TEXTACT:
			{ char *txt=NULL;
			  if(txc) txt=txc->getSource();
			  if(txt)
			  { if(af.txBase) delete af.txBase;
				 af.txBase=txt;
				 result=TRUE;
				 af.modified=TRUE;
			  }
			}
			break;

	 default: break;
  }

  if(result)
  { GetDlgItemText(hWnd, ED_INIFRAS, af.initMess, MAX_MESS_LEN);
	 GetDlgItemText(hWnd, ED_ENDFRAS, af.endMess, MAX_MESS_LEN);
  }
  else
	 if(msg) BMsgBox(hWnd, msg, 0);
	 else avisSonor(0);

  return result;
}

BOOL DEditAct::processCommand(WPARAM wParam, LPARAM lParam)
{
  int l, i=0;
  BOOL r=FALSE;
  char *tx;

  switch(wParam)
  { case BOTO_FONTS:
			{ DFonts *dfont;
			  if((dfont=new DFonts(hWnd, af.actMode==TEXTACT, af.logF, af.colors, af.colorUsuari, af.colorError))!=0)
			  { if(dfont->run())
				 { for(i=0; i<4; i++)
					{ af.logF[i]=dfont->getLFont()[i];
					  af.colors[i]=dfont->getColors()[i];
					}
					for(i=0; i<2; i++)
					{ af.colorUsuari[i]=dfont->getCUsuari()[i];
					  af.colorError[i]=dfont->getCError()[i];
					}
				 }
				 delete dfont;

				 if(txc)
				 { Zona::setBaseCont(&af.logF[GRA], &af.colors[GRA], GRA);
					Zona::setBaseCont(&af.logF[GRB], &af.colors[GRB], GRB);
					Zona::setBaseCont(&af.logF[MSB], &af.colors[MSB], MSB);
					Zona::setBaseCont(&af.logF[POP], &af.colors[POP], POP);

					txc->calcFont();
					txc->setLines(0, TRUE, TRUE);
				 }
			  }
			  else {NOMEM; return FALSE;}
			}
			break;

	 case BOTO_ASSIGNA:
			if(!checkOk()) break;
			{ DEditAssign *dedas;
			  if((dedas=new DEditAssign(hWnd, &af))!=0)
			  { dedas->run();
				 delete dedas;
			  }
			  else {NOMEM; return FALSE;}
			}
			break;

	 case BOTO_OPCIONS:
			if(!checkOk()) break;
			{ DActOpt *dactopt;
			  if((dactopt=new DActOpt(hWnd, &af))!=0)
			  { dactopt->run();
				 delete dactopt;
			  }
			  else {NOMEM; return FALSE;}
			}
			break;

	 case BOTO_EDITCONT3: i++;
	 case BOTO_EDITCONT2: i++;
	 case BOTO_EDITCONT:
			editCont(i);
			break;

	 case BOTO_INSZONA:
			if(txc)
			{ txc->insertCnt();
			  SetFocus(txc->hWnd);
			}
			break;

	 case BOTO_INSINC:
			if(txc)
			{ txc->insertIncognita();
			  SetFocus(txc->hWnd);
			}
			break;

	 case BOTO_CUT:
			if(txc)
			{ txc->cutClipboard();
			  SetFocus(txc->hWnd);
			}
			break;

	 case BOTO_COPY:
			if(txc)
			{ txc->copyClipboard();
			  SetFocus(txc->hWnd);
			}
			break;

	 case BOTO_PASTE:
			if(txc)
			{ txc->pasteClipboard();
			  SetFocus(txc->hWnd);
			}
			break;

	 case CHK_STRETCH:
			af.stretch^=1;
			r=TRUE;
			break;

	 case CHK_BAR2: i=1;
	 case CHK_BAR:
			af.bar[i]^=1;
			if(af.actMode==SOPA) resetDlg();
			break;

	 case CHK_DELIM2: i=1;
	 case CHK_DELIM:
			af.delim[i]^=1;
			break;

	 case CHK_SW3: i++;
	 case CHK_SW2: i++;
	 case CHK_SW1: i++;
	 case CHK_SW0:
			af.graPos=i;
			break;

	 case BOTO_AUTOINC:
			if(txc)
			{ DAutoInc *autoinc;
			  if((autoinc=new DAutoInc(hWnd))!=NULL)
			  { if(autoinc->run())
					txc->mKAutoIncs(autoinc->expr, autoinc->onlyWords, autoinc->txInc);
				 delete autoinc;
			  }
			}
			break;

	 case BOTO_TEXT3: i++;
	 case BOTO_TEXT2: i++;
	 case BOTO_TEXT:
			af.cont[i]=EXT_TXT;
			resetFileBox(i);
			break;

	 case BOTO_BITMAP3: i++;
	 case BOTO_BITMAP2: i++;
	 case BOTO_BITMAP:
			af.cont[i]=EXT_BMP;
			resetFileBox(i);
			break;

	 case BOTO_RES:
			af.sol^=1;
			r=TRUE;
			break;

	 case BOTO_OK:
			if(!checkOk()) {r=TRUE; break;}
			af.modified=TRUE;
			result=TRUE;

	 case BOTO_CANCEL:
			for(i=0; i<3; i++) endEditCont(i);
			if(txc!=NULL && wParam==BOTO_CANCEL)
			{ Zona::setBaseCont(&destActf->logF[GRA], &destActf->colors[GRA], GRA);
			  Zona::setBaseCont(&destActf->logF[GRB], &destActf->colors[GRB], GRB);
			  Zona::setBaseCont(&destActf->logF[MSB], &destActf->colors[MSB], MSB);
			  Zona::setBaseCont(&destActf->logF[POP], &destActf->colors[POP], POP);
			}
			end();
			break;


	 case BOTO_EVALOP:
			{ DEvalOp *devalop;
			  if((devalop=new DEvalOp(hWnd, &af))!=0)
			  { devalop->run();
				 delete devalop;
			  }
			  else {NOMEM; return FALSE;}
			}
			break;

	 case CHK_BRPARPAR:
			af.brPar^=1;
			break;

	 case CHK_BPARAGRAFS:  i++;
	 case CHK_BPARAULES:   i++;
	 case CHK_IDPARAULES:  i++;
	 case CHK_IDLLETRES:   i++;
	 case CHK_COMPLTEXT:   i++;
	 case CHK_FORATS:
			if(txc &&
				(txc->countIncs()==0 ||
				 (BMsgBox(hWnd, IDS_CHMODE,0,0,0, MB_YESNO|MB_ICONEXCLAMATION)==IDYES &&
				  txc->clearAllIncs()==TRUE
				 )
				)
			  ) af.puzMode=i;
			r=TRUE;
			break;

	 case CHK_ESCRIU:   i++;
	 case CHK_IDENT:    i++;
	 case CHK_INFO:     i++;
	 case CHK_EXPLORA:  i++;
	 case CHK_ESPECIAL: i++;
	 case CHK_NORMAL:
			af.puzMode=i;
			af.invAss=FALSE;
			changeAssMode();
			r=TRUE;
			break;

	 case LIST_TAGS2: i++;
	 case LIST_TAGS:
			if(HIWORD(lParam)==EN_VSCROLL)
			{ l=(int)SendDlgItemMessage(hWnd, i ? LIST_TAGS2:LIST_TAGS,
												 EM_GETFIRSTVISIBLELINE, 0, 0)*3;
			  tx= (i ? txLabelV : txLabelH);
			  if(l>=strlen(tx)) l=strlen(tx);
			  SetDlgItemText(hWnd, i ? TX_TAGV:TX_TAGH, tx+l);
			}
			break;

	 default:
			for(i=0; i<4; i++)
			  if(wParam==botTipus[i])
			  { af.puzMode=codTipus[i];
			    resetDlg();
				 return TRUE;
			  }

			return FALSE;
  }

  if(r) resetDlg();
  return TRUE;
}


BOOL DEditAct::setFuncHandles(BOOL connect)
{
  HWND hEditGra;

  if(af.actMode==CREUATS || af.actMode==SOPA)
  { if((hEditGra=GetDlgItem(hWnd, EDIT_TAULA))==0) return FALSE;}
  else return FALSE;

  if(connect==TRUE)
  { if(lpfnEditGra || lpfnOldEditGra) return FALSE;
	 lpfnEditGra=(WNDPROC)MakeProcInstance((FARPROC)DEditAct::HandleEditGra, hInst);
	 lpfnOldEditGra=(WNDPROC)SetWindowLong(hEditGra, GWL_WNDPROC, (LPARAM)lpfnEditGra);
  }
  else
  { if(lpfnEditGra==0L || lpfnOldEditGra==0L) return FALSE;

	 SetWindowLong(hEditGra, GWL_WNDPROC, (DWORD)lpfnOldEditGra);

	 FreeProcInstance((FARPROC)lpfnEditGra);
	 lpfnEditGra=0;
	 lpfnOldEditGra=0;
  }
  return TRUE;
}

void DEditAct::end()
{
  int i;

  for(i=0; i<2; i++) endEditCont(i);
  setFuncHandles(FALSE);

  if(result==TRUE) *destActf=af;

  ModalDialog::end();
}


// ----------------------- DEditAssign -----------------------------------

LPCSTR DEditAssign::getDialogName()
{ return MAKEINTRESOURCE(DIALEG_ASSIGNA);}

int DEditAssign::getHelpItem() {return HELP_ASSIGNA;}

DEditAssign::DEditAssign(HWND hOwner, ActFile *actf) : ModalDialog(hOwner)
{
  int i;

  af=*actf;
  destActf=actf;
  txtC[0]=txtC[1]=0;
  hBm[0]=hBm[1]=hBmX[0]=hBmX[1]=0;
  bmAdapted[0]=bmAdapted[1]=FALSE;
  rBm[0]=rBm[1]=NullRect;
  io=id=0;

  for(i=0; i<2; i++)
  { if(i==1 && af.puzMode==IDENTIFICA)
	 { af.nLines[i]=2;
		txtC[i]=newArray(2);
		txtC[i][0]=dupStr(getStringTbl(IDS_FALSE));
		txtC[i][1]=dupStr(getStringTbl(IDS_TRUE));
	 }
	 else
	 switch(getExt(af.fileNameCont[i]))
	 { case EXT_TXT:
			  txtC[i]=readTxtFile(getFullFileName(af.fileNameCont[i]), af.nLines[i]);
			  break;

		case EXT_BMP:
		case EXT_GIF:
			  bmpCont::loadBmp(hWnd, hBm[i], getFullFileName(af.fileNameCont[i]), bmlx[i], bmly[i]);
			  //loadImgFile(hWnd, hBm[i], getFullFileName(af.fileNameCont[i]),
			  //					  bmlx[i], bmly[i]);
			  break;

		default: break;
	 }
  }

}

DEditAssign::~DEditAssign()
{ int i;

  delArray(txtC[0]); delArray(txtC[1]);
  for(i=0; i<2; i++)
  { if(hBmX[i]!=0 && hBmX[i]!=hBm[i])
	 {DelObj(hBmX[i]); hBmX[i]=0;}
	 if(hBm[i])
	 //{DelObj(hBm[i]); hBm[i]=0;}
	 {bmpCont::freeBmp(hBm[i]); hBm[i]=0;}
  }
}

BOOL DEditAssign::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  POINT p;
  int i=0;
  DRAWITEMSTRUCT dris;

  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			break;

	 case WM_DRAWITEM:
			dris=*(LPDRAWITEMSTRUCT)lParam;
			switch(dris.CtlID)
			{ case COMBO_FITX2: i++;
			  case COMBO_FITX:
					 if(!hBm[i]) break;
					 drawBox(i, dris);
					 return TRUE;

			  default: break;
			}
			break;

	 case WM_COMMAND:
			if(processCommand(wParam, lParam)) return TRUE;
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_CASH; sbi.currVal=&af.nctxw;
						sbi.maxVal=MAXCW, sbi.minVal=1;
						break;

				 case SB_CASV:
						sbi.staticId=ED_CASV; sbi.currVal=&af.nctxh;
						sbi.maxVal=MAXCH, sbi.minVal=1;
						break;

				 default: return FALSE;
			  }
			  if(dlgScrBar(hWnd, wParam, sbi, TRUE))
			  { ncd=af.nctxw*af.nctxh;
				 resetDestBox();
			  }
			}
			break;

	 case WM_LBUTTONDOWN:
			p.x=LOWORD(lParam);
			p.y=HIWORD(lParam);
			for(i=0; i<2; i++)
			{ if(hBm[i] && PtInRect(&rBm[i],p))
			  { int *ix, x, y, nx;
				 ix=(i ? &id:&io);
				 nx=(i ? af.nctxw:af.ncw);
				 y=(p.y-rBm[i].top)/lcy[i];
				 x=(p.x-rBm[i].left)/lcx[i];
				 *ix=x+nx*y;
				 break;
			  }
			}
			if(i<2) {resetAss(i); return FALSE;}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DEditAssign::drawBox(int i, DRAWITEMSTRUCT &dris)
{
  RECT r;
  int x, y, j, k, ncx, ncy;

  ncx= i ? af.nctxw:af.ncw;
  ncy= i ? af.nctxh:af.nch;
  j= i ? id : io;

  if(!bmAdapted[i] || !hBmX[i])
  { float frx, fry;

	 frx=1; fry=1;
	 if(bmlx[i]>RECTW(dris.rcItem)) frx=(float)RECTW(dris.rcItem)/bmlx[i];
	 if(bmly[i]>RECTH(dris.rcItem)) fry=(float)RECTH(dris.rcItem)/bmly[i];
	 frx=fry=(frx<fry ? frx:fry);
	 nlx[i]=bmlx[i]*frx;
	 nly[i]=bmly[i]*fry;
	 nlx[i]=(nlx[i]/ncx)*ncx;
	 nly[i]=(nly[i]/ncy)*ncy;
	 if(hBmX[i]!=0 && hBmX[i]!=hBm[i]) DelObj(hBmX[i]);
	 hBmX[i]=createNewBitmap(hWnd, hBm[i], nlx[i], nly[i],
				 bmlx[i], bmly[i], FALSE);
	 rBm[i]=dris.rcItem;
	 rBm[i].right=rBm[i].left+nlx[i];
	 rBm[i].bottom=rBm[i].bottom+nly[i];
	 MapWindowPoints(dris.hwndItem, hWnd, (LPPOINT)&rBm[i], 2);
	 lcx[i]=nlx[i]/ncx;
	 lcy[i]=nly[i]/ncy;
	 bmAdapted[i]=TRUE;
  }

  r=dris.rcItem;
  FillRect(dris.hDC, &r, (HBRUSH)GetStockObject(LTGRAY_BRUSH));

  displayBitmap(dris.hDC, 0, hBmX[i], r.left, r.top, nlx[i], nly[i]);

  r.right=r.left+lcx[i];
  r.bottom=r.left+lcy[i];

  for(y=0, k=0; y<ncy; y++)
  { for(x=0; x<ncx; x++, k++)
	 { if(k==j)
		  drawMarc(dris.hDC, &r, INTERIOR_NEGRE, 2);
		else
		  DrawFocusRect(dris.hDC, &r);
		OffsetRect(&r, lcx[i], 0);
	 }
	 OffsetRect(&r, 0-nlx[i], lcy[i]);
  }

  return TRUE;
}


BOOL DEditAssign::initDlg()
{
  int i;

  if(af.puzMode==IDENTIFICA) {af.nctxw=1; af.nctxh=2;}
  else
  { if(!af.nctxw) af.nctxw=af.ncw;
	 if(!af.nctxh) af.nctxh=af.nch;
  }

  nco=af.ncw*af.nch;
  ncd=af.nctxw*af.nctxh;

  if(af.puzMode==ESPECIAL)
	 SendDlgItemMessage(hWnd, CHK_INVASS, BM_SETCHECK, af.invAss, 0L);
  else
	 ShowWindow(GetDlgItem(hWnd, CHK_INVASS), SW_HIDE);

  for(i=0; i<nco; i++) if(af.ass[i]==-2) af.ass[i]=i;

  ShowWindow(GetDlgItem(hWnd, hBm[0] ? LIST_TAGS:COMBO_FITX), SW_HIDE);
  ShowWindow(GetDlgItem(hWnd, hBm[1] ? LIST_TAGS2:COMBO_FITX2), SW_HIDE);

  if(!hBm[0])
  { fillList(0);
	 SendDlgItemMessage(hWnd, LIST_TAGS, LB_SETCURSEL, 0, 0L);
  }
  SetDlgItemInt(hWnd, ED_CASH, af.nctxw, FALSE);
  SetDlgItemInt(hWnd, ED_CASV, af.nctxh, FALSE);

  if(af.puzMode==IDENTIFICA)
  { EnableWindow(GetDlgItem(hWnd, SB_CASH), FALSE);
	 EnableWindow(GetDlgItem(hWnd, SB_CASV), FALSE);
	 EnableWindow(GetDlgItem(hWnd, ED_CASH), FALSE);
	 EnableWindow(GetDlgItem(hWnd, ED_CASV), FALSE);
  }
  resetDestBox();
  return TRUE;
}

BOOL DEditAssign::resetDestBox()
{
  int i;

  if(hBm[1])
	 bmAdapted[1]=FALSE;
  else
	 fillList(1);

  for(i=0; i<nco; i++)
	 if(af.ass[i]<-1 || af.ass[i]>=ncd) af.ass[i]=0;

  return resetAss(FALSE);
}

BOOL DEditAssign::resetAss(BOOL canvia)
{
  DWORD dw;

  if(!hBm[0])
  { dw=SendDlgItemMessage(hWnd, LIST_TAGS, LB_GETCURSEL, 0, 0L);
	 io=(dw==(unsigned)LB_ERR ? 0:(int)dw);
  }

  if(canvia)
  { if(!hBm[1])
	 { if(id==-1)
		  SendDlgItemMessage(hWnd, LIST_TAGS2, LB_SETCURSEL, -1, 0L);
		else
		{ dw=SendDlgItemMessage(hWnd, LIST_TAGS2, LB_GETCURSEL, 0, 0L);
		  id=(dw==(unsigned)LB_ERR ? 0:(int)dw);
		  //if(id==ncd) id=-1;
		}
	 }
	 af.ass[io]=id;
  }
  else
  { id=af.ass[io];
	 if(!hBm[1]) SendDlgItemMessage(hWnd, LIST_TAGS2, LB_SETCURSEL, id, 0L); //id=-1 anula la seleccio
	 if(hBm[0]) InvalidateRect(GetDlgItem(hWnd, COMBO_FITX), NULL, TRUE);
  }

  SendDlgItemMessage(hWnd, CHK_NOASSIGN, BM_SETCHECK, id==-1, 0L);
  if(hBm[1]) InvalidateRect(GetDlgItem(hWnd, COMBO_FITX2), NULL, TRUE);

  return TRUE;
}

BOOL DEditAssign::processCommand(WPARAM wParam, LPARAM lParam)
{
  int i=FALSE;

  switch(wParam)
  { case BOTO_OK:
			for(i=0; i<nco; i++)
			  if(af.ass[i]!=-1) break;
			if(i==nco)
			{ BMsgBox(hWnd, IDS_ERRNOASGN, 0);
			  break;
			}
			if(af.invAss)
			{ char *testB;
			  int j;

			  testB=new char[ncd];
			  for(i=0; i<ncd; i++) testB[i]=0;
			  for(i=0; i<nco; i++) if((j=af.ass[i])>=0) testB[j]=1;
			  for(i=0; i<ncd; i++) if(testB[i]==0) break;
			  delete testB;
			  if(i!=ncd)
			  { BMsgBox(hWnd, IDS_NOINVASS, 0);
				 break;
			  }
			}
			result=TRUE;
			*destActf=af;

	 case BOTO_CANCEL:
			end();
			break;

	 case BOTO_RESETASS:
			for(i=0; i<nco; i++) af.ass[i]=id;
			break;

	 case CHK_NOASSIGN:
			id=-1;
			resetAss(TRUE);
			break;

	 case CHK_INVASS:
			af.invAss^=1;
			break;

	 case LIST_TAGS2:
			i=TRUE;
			id=0;
	 case LIST_TAGS:
			if(HIWORD(lParam)==LBN_SELCHANGE) return resetAss(i);
			break;

	 default: return FALSE;
  }
  return TRUE;
}

BOOL DEditAssign::fillList(int l)
{
  int i, lb, nc;
  LPSTR blankLine=getStringTbl(IDS_BLANKLINE);

  if(hBm[l]) return FALSE;

  lb=l ? LIST_TAGS2 : LIST_TAGS;
  nc=l ? ncd : nco;

  SendDlgItemMessage(hWnd, lb, LB_RESETCONTENT, 0, 0L);

  for(i=0; i<nc; i++)
	 SendDlgItemMessage(hWnd, lb, LB_ADDSTRING, 0,
		(LPARAM)(txtC[l] && i<af.nLines[l] ? (LPSTR)txtC[l][i] : blankLine));

  return TRUE;
}

// ------- DEdPaquet -----------

#define VALIDEXTSPAC 5
static Exts validFilesPac[VALIDEXTSPAC]={EXT_PUZ, EXT_SOP, EXT_ASS, EXT_CRW, EXT_TXA};
#define NITEMSPAC 1
static UINT itemsPac[NITEMSPAC]={LIST_FITX};

DEdPaquet::DEdPaquet(HWND hOwner, Paquet *iPac):ModalDialog(hOwner)
{
  result=FALSE;
  destPac=iPac;
  if(iPac) pac=*iPac;
}

LPCSTR DEdPaquet::getDialogName() {return MAKEINTRESOURCE(DIALEG_PAQUET);}
int DEdPaquet::getHelpItem() {return HELP_EDITPAC;}

BOOL DEdPaquet::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_COMMAND:
			switch(wParam)
			{ case IDB_PASSADES:
					 { DChain *dch;
						if((dch=new DChain(hWnd, &pac))!=NULL)
						{ dch->run();
						  delete dch;
						}
						else NOMEM;
					 }
					 return TRUE;

			  case CHK_AUTOPASS:
					 pac.autoPass^=1;
					 if(!pac.autoPass) pac.noPassButtons=FALSE;
					 pac.modified=TRUE;
					 resetDlg();
					 return TRUE;

			  case CHK_NOPBUTTONS:
					 pac.noPassButtons^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 return TRUE;

			  case CHK_BOTOSEP:
					 pac.botoSep^=1;
					 pac.modified=TRUE;
					 return TRUE;

			  case CHK_NODISK:
					 pac.noDiskButton^=1;
					 pac.modified=TRUE;
					 return TRUE;

			  case CHK_NOEXIT:
					 pac.noExitButton^=1;
					 pac.modified=TRUE;
					 return TRUE;

			  case CHK_TEST800:
					 pac.av800^=1;
					 pac.modified=TRUE;
					 return TRUE;

			  case CHK_TEST256:
					 pac.av256^=1;
					 pac.modified=TRUE;
					 return TRUE;

			  case BOTO_AFEGIR:
					 if(addItem()) pac.modified=TRUE;
					 return TRUE;

			  case BOTO_ELIM:
					 if(deleteItem()) pac.modified=TRUE;
					 return TRUE;

			  case BOTO_OK:
					 if(pac.modified)
					 { if(!checkOk())
						{ avisSonor(0);
						  break;
						}
						if(destPac) {*destPac=pac;}
						result=TRUE;
					 }
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case LIST_FITX:
			  case LIST_PAQ:
					 if(HIWORD(lParam)==LBN_SELCHANGE)
					 resetDlg();
					 return TRUE;

			  default: break;
			}
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_LPT:
						sbi.staticId=ED_LPT; sbi.currVal=&pac.lapToPass;
						sbi.maxVal=60, sbi.minVal=0;
						break;

				 default: return FALSE;
			  }

			  if(dlgScrBar(hWnd, wParam, sbi, TRUE))
				 pac.modified=TRUE;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DEdPaquet::initDlg()
{
  int i;

  setCursor(hWnd, CURSOR_ESPERA);

  searchIter(nullStr, hWnd, NITEMSPAC, itemsPac, VALIDEXTSPAC, validFilesPac, TRUE);

  if(!pac.isEmpty())
	 for(i=0; i<pac.nActs; i++)
		SendDlgItemMessage(hWnd, LIST_PAQ, LB_ADDSTRING, 0,
			 (DWORD)AnsiLower(pac.acts[i]));
  resetCursor();

  CheckDlgButton(hWnd, CHK_NODISK, !pac.noDiskButton);
  CheckDlgButton(hWnd, CHK_NOEXIT, !pac.noExitButton);
  SetDlgItemInt(hWnd, ED_LPT, pac.lapToPass, FALSE);
  CheckDlgButton(hWnd, CHK_TEST800, pac.av800);
  CheckDlgButton(hWnd, CHK_TEST256, pac.av256);

  return resetDlg();
}

BOOL DEdPaquet::resetDlg()
{
  EnableWindow(GetDlgItem(hWnd, BOTO_AFEGIR),
			  SendDlgItemMessage(hWnd, LIST_FITX, LB_GETCURSEL, 0, 0L)!=LB_ERR);

  EnableWindow(GetDlgItem(hWnd, BOTO_ELIM),
			  SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETCURSEL, 0, 0L)!=LB_ERR);

  EnableWindow(GetDlgItem(hWnd, CHK_NOPBUTTONS), pac.autoPass);
  EnableWindow(GetDlgItem(hWnd, CHK_BOTOSEP), !pac.noPassButtons);
  EnableWindow(GetDlgItem(hWnd, SB_LPT), pac.autoPass);

  CheckDlgButton(hWnd, CHK_AUTOPASS, pac.autoPass);
  CheckDlgButton(hWnd, CHK_NOPBUTTONS, !pac.noPassButtons);
  CheckDlgButton(hWnd, CHK_BOTOSEP, pac.botoSep);

  return TRUE;
}

BOOL DEdPaquet::addItem()
{
  int i, j;

  if((i=(int)SendDlgItemMessage(hWnd, LIST_FITX,
				LB_GETCURSEL, 0, 0L))==LB_ERR ||
	  (j=(int)SendDlgItemMessage(hWnd, LIST_FITX,
				LB_GETTEXT, i, (LPARAM)(LPSTR)tempPath3))==0 ||
		j==LB_ERR) return FALSE;

  if((i=(int)SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETCURSEL, 0, 0L))==LB_ERR)
	 i=-1;
  else i++;

  j=(int)SendDlgItemMessage(hWnd, LIST_PAQ, LB_INSERTSTRING,
				 i, (LPARAM)(LPSTR)tempPath3);

  SendDlgItemMessage(hWnd, LIST_PAQ, LB_SETCURSEL, j, 0L);

  return resetDlg();
}

BOOL DEdPaquet::deleteItem()
{
  int i, j;

  if((j=(int)SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETCOUNT, 0, 0L))==LB_ERR||
	  j==0 ||
	  (i=(int)SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETCURSEL, 0, 0L))==LB_ERR)
	 return FALSE;
  SendDlgItemMessage(hWnd, LIST_PAQ, LB_DELETESTRING, i, 0L);
  SendDlgItemMessage(hWnd, LIST_PAQ, LB_SETCURSEL, (i==j-1 ? --i:i), 0L);
  return resetDlg();
}

BOOL DEdPaquet::checkOk()
{
  int i, j;

  if(pac.acts) {delArray(pac.acts); pac.acts=NULL;}
  pac.nActs=0; pac.act=0;

  if((i=(int)SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETCOUNT, 0, 0L))==LB_ERR ||
		i==0) return FALSE;

  if((pac.acts=newArray(pac.nActs=i))==NULL) return FALSE;
  for(j=0; j<i; j++)
  { if(SendDlgItemMessage(hWnd, LIST_PAQ, LB_GETTEXT,
			  j, (LPARAM)(LPSTR)tempPath3)==LB_ERR ||
		 (pac.acts[j]=dupStr(tempPath3))==NULL) break;
  }
  return (j==i);
}

// ------- DChain -----------

#define VALIDEXTSCHAIN 2
static Exts validFilesChain[VALIDEXTSCHAIN]={EXT_PAC, EXT_PCC};
#define NITEMSCHAIN 3
static UINT itemsChain[NITEMSCHAIN]={COMBO_FITX, COMBO_FITX2, COMBO_FITX3};

DChain::DChain(HWND hOwner, Paquet *iPac):ModalDialog(hOwner)
{
  result=FALSE;
  destPac=iPac;
  if(iPac) pac=*iPac;
}

LPCSTR DChain::getDialogName() {return MAKEINTRESOURCE(DIALEG_CHAIN);}
int DChain::getHelpItem() {return HELP_CHAIN;}

BOOL DChain::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_OK:
					 if(!checkOk())
					 { avisSonor(0);
						break;
					 }
					 pac.modified=TRUE;
					 if(destPac) {*destPac=pac;}
					 result=TRUE;
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case CHK_DEF:
					 pac.def^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 break;

			  case CHK_MIN:
					 pac.inf^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 break;

			  case CHK_MAX:
					 pac.sup^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 break;

			  case CHK_MINT:
					 pac.infTFlag^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 break;

			  case CHK_MAXT:
					 pac.supTFlag^=1;
					 pac.modified=TRUE;
					 resetDlg();
					 break;

			  default: break;
			}
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_MINP:
						sbi.staticId=ED_MINP; sbi.currVal=&pac.infP;
						sbi.maxVal=100, sbi.minVal=0;
						break;

				 case SB_MINT:
						sbi.staticId=ED_MINT; sbi.currVal=&pac.infT;
						sbi.maxVal=999, sbi.minVal=0;
						break;

				 case SB_MAXP:
						sbi.staticId=ED_MAXP; sbi.currVal=&pac.supP;
						sbi.maxVal=100, sbi.minVal=0;
						break;

				 case SB_MAXT:
						sbi.staticId=ED_MAXT; sbi.currVal=&pac.supT;
						sbi.maxVal=999, sbi.minVal=0;
						break;

				 default: return FALSE;
			  }

			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DChain::initDlg()
{
  CheckDlgButton(hWnd, CHK_DEF, pac.def);
  CheckDlgButton(hWnd, CHK_MAX, pac.sup);
  CheckDlgButton(hWnd, CHK_MIN, pac.inf);

  SetDlgItemInt(hWnd, ED_MAXP, pac.supP, FALSE);
  SetDlgItemInt(hWnd, ED_MAXT, pac.supT, FALSE);
  SetDlgItemInt(hWnd, ED_MINP, pac.infP, FALSE);
  SetDlgItemInt(hWnd, ED_MINT, pac.infT, FALSE);

  searchIter(nullStr, hWnd, NITEMSCHAIN, itemsChain,
				 VALIDEXTSCHAIN, validFilesChain, FALSE);

  if(pac.chDef)
	 SetDlgItemText(hWnd, COMBO_FITX, pac.chDef);
	 //SendDlgItemMessage(hWnd, COMBO_FITX, CB_SELECTSTRING,
	 //						  -1, (LPARAM)(LPSTR)pac.chDef);

  if(pac.chInf)
	 SetDlgItemText(hWnd, COMBO_FITX2, pac.chInf);
	 //SendDlgItemMessage(hWnd, COMBO_FITX2, CB_SELECTSTRING,
	 //						  -1, (LPARAM)(LPSTR)pac.chInf);

  if(pac.chSup)
	 SetDlgItemText(hWnd, COMBO_FITX3, pac.chSup);
	 //SendDlgItemMessage(hWnd, COMBO_FITX3, CB_SELECTSTRING,
	 //						  -1, (LPARAM)(LPSTR)pac.chSup);

  return resetDlg();
}

BOOL DChain::resetDlg()
{
  EnableWindow(GetDlgItem(hWnd, CHK_MAX), pac.def);
  EnableWindow(GetDlgItem(hWnd, CHK_MIN), pac.def);
  EnableWindow(GetDlgItem(hWnd, COMBO_FITX), pac.def);
  EnableWindow(GetDlgItem(hWnd, COMBO_FITX2), pac.def & pac.inf);
  EnableWindow(GetDlgItem(hWnd, COMBO_FITX3), pac.def & pac.sup);
  EnableWindow(GetDlgItem(hWnd, CHK_MAXT), pac.def & pac.sup);
  EnableWindow(GetDlgItem(hWnd, CHK_MINT), pac.def & pac.inf);
  EnableWindow(GetDlgItem(hWnd, SB_MINP), pac.def & pac.inf);
  EnableWindow(GetDlgItem(hWnd, SB_MINT), pac.def & pac.inf & pac.infTFlag);
  EnableWindow(GetDlgItem(hWnd, SB_MAXP), pac.def & pac.sup);
  EnableWindow(GetDlgItem(hWnd, SB_MAXT), pac.def & pac.sup & pac.supTFlag);

  return TRUE;
}

BOOL DChain::checkOk()
{
  if(!pac.def) return TRUE;
  *tempPath3=0;
  if(GetDlgItemText(hWnd, COMBO_FITX, tempPath3, MAX_FULL_FILENAME)==0 ||
	  *tempPath3==0) return FALSE;
  if(pac.chDef) {delete pac.chDef; pac.chDef=NULL;}
  pac.chDef=dupStr(tempPath3);

  if(pac.inf)
  { if(GetDlgItemText(hWnd, COMBO_FITX2, tempPath3, MAX_FULL_FILENAME)==0 ||
		 *tempPath3==0) return FALSE;
	 if(pac.chInf) {delete pac.chInf; pac.chInf=NULL;}
	 pac.chInf=dupStr(tempPath3);
  }

  if(pac.sup)
  { if(GetDlgItemText(hWnd, COMBO_FITX3, tempPath3, MAX_FULL_FILENAME)==0 ||
		 *tempPath3==0) return FALSE;
	 if(pac.chSup) {delete pac.chSup; pac.chSup=NULL;}
	 pac.chSup=dupStr(tempPath3);
  }

  return TRUE;
}


// ----------------- SearchIter ----------------------------------

void searchIter(char *pathToExplore, HWND hWnd, int nItems, UINT *listItems,
					 int numExts, Exts *validFiles, BOOL isList)
{
  int i, j, k;
  char *p;
  struct ffblk ffblk;
  char patbuff[MAX_FULL_FILENAME];
  BOOL done;

  strcat(strcat(strcpy(tempPath3, curPacPath), pathToExplore), "*.*");
  for(done=findfirst(tempPath3, &ffblk, FA_DIREC); !done; done=findnext(&ffblk))
  { if(ffblk.ff_attrib & FA_DIREC)
	 { if(*ffblk.ff_name=='.') continue;
		strcat(strcat(strcpy(patbuff, pathToExplore), ffblk.ff_name), "\\");
		searchIter(patbuff, hWnd, nItems, listItems, numExts, validFiles, isList);
	 }
	 else
	 { if((j=strlen(ffblk.ff_name))<5) continue;
		p=ffblk.ff_name+j-4;
		for(i=0; i<numExts; i++)
		if(stricmp(p, extName[validFiles[i]]+1)==0)
		{ strcat(strcpy(tempPath3, pathToExplore), ffblk.ff_name);
		  AnsiLower(tempPath3);
		  for(k=0; k<nItems; k++)
			 SendDlgItemMessage(hWnd, listItems[k],
									  isList ? LB_ADDSTRING:CB_ADDSTRING,
									  0, (LPARAM)(LPSTR)tempPath3);
		  break;
		}
	 }
  }
}

#endif

