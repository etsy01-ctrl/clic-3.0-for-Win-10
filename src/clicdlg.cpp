// ---------------------------------------
// CLICDLG.CPP
// Implementacio de les classes de Dialegs
// ---------------------------------------

#include "clicdlg.h"
#include "miscfunc.h"
#include "rcomu\clicres.h"
#include "rcomu\clich.h"

#include <string.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <commdlg.h>

// ------- DInfo -----------

#ifndef RUNTIME
LPCSTR DInfo::getDialogName() {return MAKEINTRESOURCE(DIALEG_INFO);}
int DInfo::getHelpItem() {return HELP_CLIC_INDEX;}

BOOL DInfo::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			return TRUE;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_OK:
					 result=TRUE;
			  case IDCANCEL:
					 end();
					 return TRUE;

			  default: break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

// ------- DReport -----------

LPCSTR DReport::getDialogName() {return MAKEINTRESOURCE(DIALEG_REPORT);}
int DReport::getHelpItem() {return HELP_REPORT;}

DReport::DReport(HWND hOwner, Report *ir) : ModalDialog(hOwner)
{
  r=ir;
}


BOOL DReport::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_OK:
					 result=TRUE;
			  case IDCANCEL:
					 end();
					 return TRUE;

			  default: break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

void DReport::initDlg()
{
  char *t;
  int i, j;

  SendDlgItemMessage(hWnd, LIST_REPORT, WM_SETFONT,
							(WPARAM)GetStockObject(SYSTEM_FIXED_FONT), TRUE);

#ifdef RUNTIME
  ShowWindow(GetDlgItem(hWnd, IDHELP), SW_HIDE);
#endif

  if(r)
  { for(i=0, j=r->getCur(); i<j; i++)
	 { t=r->getFRep(i);
		SendDlgItemMessage(hWnd, LIST_REPORT, LB_ADDSTRING, 0, (LPARAM)(LPSTR)t);
	 }
	 t=r->getGPrec();
	 SetDlgItemText(hWnd, TX_TACTS, t); t+=(strlen(t)+1);
	 SetDlgItemText(hWnd, TX_TSOLVED, t); t+=(strlen(t)+1);
	 SetDlgItemText(hWnd, TX_TTIME, t); t+=(strlen(t)+1);
	 SetDlgItemText(hWnd, TX_GLOBALP, t);
  }
}
#endif

// ------- DPuzzle ------------

LPCSTR DPuzzle::getDialogName() {return MAKEINTRESOURCE(DIALEG_PUZZLE);}
int DPuzzle::getHelpItem() {return HELP_TIPUSPUZ;}

DPuzzle::DPuzzle(HWND hOwner, ActFile *actf) : ModalDialog(hOwner)
{
  af=actf;
  pmod=af->puzMode;
  ncw=af->ncw;
  nch=af->nch;
  stretch=af->stretch;
}

void DPuzzle::initDlg()
{
  SendDlgItemMessage(hWnd, ED_CASH, EM_LIMITTEXT, 3, 0);
  SendDlgItemMessage(hWnd, ED_CASV, EM_LIMITTEXT, 3, 0);
  resetDlg();
}

BOOL DPuzzle::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_CASH; sbi.currVal=&ncw;
						sbi.maxVal=MAXCW, sbi.minVal=1;
						break;

				 case SB_CASV:
						sbi.staticId=ED_CASV; sbi.currVal=&nch;
						sbi.maxVal=MAXCH, sbi.minVal=1;
						break;

				 default: return FALSE;
			  }
			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_DOBLE: pmod=DOBLE; resetDlg(); return TRUE;
			  case BOTO_FORAT: pmod=FORAT; resetDlg(); return TRUE;
			  case BOTO_INTERC: pmod=INTERC; resetDlg(); return TRUE;
			  case BOTO_MEMORY: pmod=MEMORY; resetDlg(); return TRUE;

			  case CHK_STRETCH:
					 stretch^=1;
					 resetDlg();
					 break;

			  case BOTO_OK:
					 if(checkCas(hWnd, ncw, nch, 15, 15, ED_CASH, ED_CASV)==FALSE)
					 { resetDlg();
						break;
					 }
					 else
					 { af->puzMode=pmod;
						af->ncw=ncw;
						af->nch=nch;
						af->stretch=stretch;
						result=TRUE;
					 }

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case BOTO_DEFAULT:
					 ncw=4;
					 nch=3;
					 pmod=INTERC; stretch=FALSE;
					 resetDlg();
					 return TRUE;

			  default: break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

void DPuzzle::resetDlg()
{
  int i;

  SetDlgItemInt(hWnd, ED_CASH, ncw, FALSE);
  SetDlgItemInt(hWnd, ED_CASV, nch, FALSE);
  SendDlgItemMessage(hWnd, CHK_STRETCH, BM_SETCHECK, stretch, 0);
  switch(pmod)
  { case FORAT:  i=IDS_FORAT; break;
	 case DOBLE:  i=IDS_DOBLE; break;
	 case MEMORY: i=IDS_MEMORY; break;
	 default: pmod=INTERC; i=IDS_INTERC; break;
  }
  SetDlgItemText(hWnd, INFO_MOD, getStringTbl(i));
}

// -------- DIncognita ---------------
#ifndef RUNTIME
LPCSTR DIncognita::getDialogName() {return MAKEINTRESOURCE(DIALEG_INCOGNITA);}
int DIncognita::getHelpItem() {return HELP_INCOGNITA;}

DIncognita::DIncognita(HWND hOwner, struct Incognita *iniInc):ModalDialog(hOwner)
{
  inc0=iniInc;
  inc=*inc0;
  inc.ok=NULL;
  inc.opcions=NULL;
  inc.iniText=NULL;
  inc.infoTxt=NULL;
  inc.hCBList=NULL;
}

BOOL DIncognita::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  int l1, l2;

  switch(msg)
  { case WM_INITDIALOG:
			SetDlgItemInt(hWnd, ED_CASH, inc.infoWidth, FALSE);
			SetDlgItemInt(hWnd, ED_CASV, inc.infoHeight, FALSE);
			SetDlgItemInt(hWnd, ED_FARCIT, inc.farciment, FALSE);
			SetDlgItemInt(hWnd, ED_MAXRESP, inc.maxResp, FALSE);
			SetDlgItemInt(hWnd, ED_DELAY, inc.infoDelay, FALSE);
			SetDlgItemInt(hWnd, ED_TMAX, inc.infoMax, FALSE);
			*txtbuff=inc0->chFarciment; txtbuff[1]=0;
			SetDlgItemText(hWnd, ED_CHFARCIM, txtbuff);
			SendDlgItemMessage(hWnd, ED_CHFARCIM, EM_LIMITTEXT, 1, 0);
			if(inc0->ok) SetDlgItemText(hWnd, ED_TXOK, inc0->ok);
			if(inc0->opcions) SetDlgItemText(hWnd, ED_TXOP, inc0->opcions);
			if(inc0->iniText) SetDlgItemText(hWnd, ED_INITEXT, inc0->iniText);
			if(inc0->infoTxt) SetDlgItemText(hWnd, ED_INFOTXT, inc0->infoTxt);
			SendDlgItemMessage(hWnd, CHK_LLISTA, BM_SETCHECK, inc.tipus==LLISTA, 0);
			SendDlgItemMessage(hWnd, CHK_INFO, BM_SETCHECK, inc.infoMode!=NO_INFO, 0);
			SendDlgItemMessage(hWnd, CHK_ALWAYS, BM_SETCHECK, inc.infoMode==ALWAYS, 0);
			SendDlgItemMessage(hWnd, CHK_ONERROR, BM_SETCHECK, inc.infoMode==ON_ERROR, 0);
			SendDlgItemMessage(hWnd, CHK_ONDEMAND, BM_SETCHECK, inc.infoMode==ON_DEMAND, 0);
			SendDlgItemMessage(hWnd, CHK_LA, BM_SETCHECK, (inc.flagsInc & INFO_LA)==0, 0);
			SendDlgItemMessage(hWnd, CHK_ONLYPLAY, BM_SETCHECK, (inc.flagsInc & INFO_ONLYPLAY)!=0, 0);
			resetDlg();
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_CASH; sbi.currVal=&inc.infoWidth;
						sbi.maxVal=600, sbi.minVal=2;
						break;

				 case SB_CASV:
						sbi.staticId=ED_CASV; sbi.currVal=&inc.infoHeight;
						sbi.maxVal=200, sbi.minVal=2;
						break;

				 case SB_FARCIT:
						sbi.staticId=ED_FARCIT; sbi.currVal=&inc.farciment;
						sbi.maxVal=400, sbi.minVal=1;
						break;

				 case SB_MAXRESP:
						sbi.staticId=ED_MAXRESP; sbi.currVal=&inc.maxResp;
						sbi.maxVal=400, sbi.minVal=1;
						break;

				 case SB_DELAY:
						sbi.staticId=ED_DELAY; sbi.currVal=&inc.infoDelay;
						sbi.maxVal=120, sbi.minVal=0;
						break;

				 case SB_TMAX:
						sbi.staticId=ED_TMAX; sbi.currVal=&inc.infoMax;
						sbi.maxVal=120, sbi.minVal=0;
						break;

				 default: return FALSE;
			  }

			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_LLISTA:
					 inc.tipus=(inc.tipus==ESCRITA ? LLISTA : ESCRITA);
					 resetDlg();
					 break;

			  case CHK_INFO:
					 if(inc.infoMode!=NO_INFO)
						inc.infoMode=NO_INFO;
					 else
					 { inc.infoMode=ALWAYS;
						SendDlgItemMessage(hWnd, CHK_ALWAYS, BM_SETCHECK, TRUE, 0);
					 }
					 resetDlg();
					 break;

			  case CHK_LA:
					 inc.flagsInc^=INFO_LA;
					 break;

			  case CHK_ONLYPLAY:
					 inc.flagsInc^=INFO_ONLYPLAY;
					 break;

			  case CHK_ALWAYS:
					 inc.infoMode=ALWAYS;
					 break;

			  case CHK_ONERROR:
					 inc.infoMode=ON_ERROR;
					 break;

			  case CHK_ONDEMAND:
					 inc.infoMode=ON_DEMAND;
					 break;

			  case BOTO_OK:
					 if((l1=GetWindowTextLength(GetDlgItem(hWnd, ED_TXOK)))==0)
					 { BMsgBox(hWnd, IDS_NOOK, 0);
						break;
					 }

					 if(inc.tipus==LLISTA)
					 { char *tx, *tx2, *txok;

						if((l2=GetWindowTextLength(GetDlgItem(hWnd, ED_TXOP)))==0)
						{ BMsgBox(hWnd, IDS_NOLLISTA, 0);
						  break;
						}

						if((txok=new char[l1+1])==NULL) {NOMEM; break;}
						GetDlgItemText(hWnd, ED_TXOK, txok, l1+1);

						if((tx2=new char[l2+1])==NULL) {NOMEM; break;}
						GetDlgItemText(hWnd, ED_TXOP, tx2, l2+1);

						tx=strtok(tx2, "\r\n");
						while(tx)
						{ if(strcmp(tx, txok)==0) break;
						  tx=strtok(NULL, "\r\n");
						}
						delete tx2;
						delete txok;
						if(tx==NULL)
						{ BMsgBox(hWnd, IDS_NOOKINLIST, 0);
						  break;
						}
					 }

					 GetDlgItemText(hWnd, ED_CHFARCIM, txtbuff, LMAXRFTXT-1);
					 inc.chFarciment=(*txtbuff==0 ? ' ':*txtbuff);

					 inc0->~Incognita();
					 *inc0=inc;

					 if((inc0->ok=new char[l1+1])==NULL) {NOMEM; break;}
					 GetDlgItemText(hWnd, ED_TXOK, inc0->ok, l1+1);

					 GetDlgItemText(hWnd, ED_INITEXT, txtbuff, LMAXRFTXT-1);
					 if(*txtbuff) inc0->iniText=strdup(txtbuff);

					 if(inc.infoMode!=NO_INFO)
					 { GetDlgItemText(hWnd, ED_INFOTXT, txtbuff, LMAXRFTXT-1);
						inc0->infoTxt=strdup(txtbuff);
					 }

					 if(inc.tipus==LLISTA)
					 { if((inc0->opcions=new char[l2+1])==NULL) {NOMEM; break;}
						GetDlgItemText(hWnd, ED_TXOP, inc0->opcions, l2+1);
					 }
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 break;
			}
			break;

	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DIncognita::resetDlg()
{
  int i;
  static int ctrls[]={CHK_ALWAYS, CHK_ONERROR, CHK_ONDEMAND, ED_INFOTXT, CHK_LA,
							 SB_CASV, ED_CASV, SB_CASH, ED_CASH, SB_DELAY, ED_DELAY,
							 STATIC1, STATIC2, STATIC3, ED_TMAX, SB_TMAX, TX_TMAX, CHK_ONLYPLAY};

  for(i=0; i<18; i++)
	 ShowWindow(GetDlgItem(hWnd, ctrls[i]), (inc.infoMode==NO_INFO) ? SW_HIDE : SW_SHOW);

  ShowWindow(GetDlgItem(hWnd, ED_TXOP), inc.tipus==LLISTA);

  return TRUE;
}
#endif

// -------- DFileName ---------------

#ifndef RUNTIME
LPCSTR DFileName::getDialogName() {return MAKEINTRESOURCE(DIALEG_FILENAME);}
int DFileName::getHelpItem() {return NULL;}

DFileName::DFileName(HWND hOwner, BOOL isText0):ModalDialog(hOwner)
{ isText=isText0;
  *txBuffer=0;
}


BOOL DFileName::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  char *ch;

  switch(msg)
  { case WM_INITDIALOG:
			SetDlgItemText(hWnd, ED_EXTENSIO, isText ? ".TXT" : ".BMP");
			SendDlgItemMessage(hWnd, ED_EDTX, EM_LIMITTEXT, 8, 0);
			return TRUE;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_OK:
					 GetDlgItemText(hWnd, ED_EDTX, txBuffer, 9);
					 if(*txBuffer==0)
					 { BMsgBox(hWnd, IDS_ERRNONAME, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
						break;
					 }
					 for(ch=txBuffer; *ch!=0; ch++)
						if((*ch<'0' || (*ch>'9' && *ch<'A') || *ch>'Z') && *ch!='-' && *ch!='_')break;
					 if(*ch!=0)
					 { BMsgBox(hWnd, IDS_ERRBADCHARS, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
						break;
					 }
					 strcat(txBuffer, isText ? ".TXT" : ".BMP");
					 { OFSTRUCT ofs;
						HFILE hf;
						ofs.cBytes=sizeof(OFSTRUCT);

						char *fn;
						fn=getFullFileName(txBuffer);
						if(OpenFile(fn, &ofs, OF_EXIST)!=HFILE_ERROR)
						{ BMsgBox(hWnd, IDS_ERRFILEEXISTS, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
						  break;
						}
						if((hf=OpenFile(fn, &ofs, OF_CREATE))==HFILE_ERROR)
						{ BMsgBox(hWnd, IDS_ERRWRITE, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
						  break;
						}
						if(!isText)
						{ HGLOBAL hg;
						  void far *hgp;
						  int bmphs, bmprest;
						  bmphs=7*16+6;
						  bmprest=32118-bmphs;

						  if((hg=GlobalAlloc(GMEM_MOVEABLE, bmprest))!=NULL)
						  { _lwrite(hf, bmpHeader, bmphs);
							 hgp=GlobalLock(hg);
							 _fmemset(hgp, 0xFF, bmprest);
							 _lwrite(hf, hgp, bmprest);
							 GlobalUnlock(hg);
							 GlobalFree(hg);
						  }
						  else NOMEM;
						}

						_lclose(hf);
					 }
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 break;
			}
			break;

	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}

char DFileName::bmpHeader[]={
0x42, 0x4D, 0x76, 0x7D, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00,
0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xC8, 0x00,  0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x7D, 0x00, 0x00, 0x12, 0x0B,  0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x10, 0x00,
0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00,  0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80,
0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x80, 0x80,  0x80, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00,  0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00};

#endif

// -------- DZones ---------------

#ifndef RUNTIME
LPCSTR DZones::getDialogName() {return MAKEINTRESOURCE(DIALEG_ZONES);}
int DZones::getHelpItem() {return HELP_ZONES;}

DZones::DZones(HWND hOwner, int w0, int h0, int b0, BOOL la0, char *txt0):ModalDialog(hOwner)
{
  w=w0;
  h=h0;
  b=b0;
  la=la0;
  txt=txtbuff;
  if(txt0) strcpy(txt, txt0); else *txt=0;
}

BOOL DZones::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			SetDlgItemText(hWnd, ED_EDTX, txt);
			SetDlgItemInt(hWnd, ED_CASH, w, FALSE);
			SetDlgItemInt(hWnd, ED_CASV, h, FALSE);
			SendDlgItemMessage(hWnd, CHK_BORDER, BM_SETCHECK, b, 0);
			SendDlgItemMessage(hWnd, CHK_LA, BM_SETCHECK, la==FALSE, 0);
			return TRUE;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_CASH; sbi.currVal=&w;
						sbi.maxVal=600, sbi.minVal=2;
						break;

				 case SB_CASV:
						sbi.staticId=ED_CASV; sbi.currVal=&h;
						sbi.maxVal=400, sbi.minVal=2;
						break;

				 default: return FALSE;
			  }

			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_BORDER:
					 b^=1;
					 break;

			  case CHK_LA:
					 la^=1;
					 break;

			  case BOTO_OK:
					 GetDlgItemText(hWnd, ED_EDTX, txt, LMAXRFTXT-1);
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 break;
			}
			break;

	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}
#endif

// ------- DEvalOp ------------
#ifndef RUNTIME
LPCSTR DEvalOp::getDialogName() {return MAKEINTRESOURCE(DIALEG_EVALOP);}
int DEvalOp::getHelpItem() {return HELP_EVALOP;}

DEvalOp::DEvalOp(HWND hOwner, ActFile *af0):ModalDialog(hOwner)
{
  af=af0;
  okToNext=af->okToNext;
  btCorregir=af->btCorregir;
  avCont=af->avCont;
  avMaj=af->avMaj;
  avAcc=af->avAcc;
  avDblSpc=af->avDblSpc;
  avPunt=af->avPunt;
  avLletra=af->avLletra;
  avScope=af->avScope;
  avMaxScope=af->avMaxScope;
  avPrevHelp=af->avPrevHelp;
  avTimePH=af->avTimePH;
  avNoSalta=af->avNoSalta;
}


BOOL DEvalOp::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  int l1;

  switch(msg)
  { case WM_INITDIALOG:
			SendDlgItemMessage(hWnd, CHK_OKTONEXT, BM_SETCHECK, okToNext, 0);
			SendDlgItemMessage(hWnd, CHK_BTEVAL, BM_SETCHECK, btCorregir, 0);
			SendDlgItemMessage(hWnd, CHK_AVCONT, BM_SETCHECK, avCont, 0);
			SendDlgItemMessage(hWnd, CHK_MAJUSCULES, BM_SETCHECK, avMaj, 0);
			SendDlgItemMessage(hWnd, CHK_ACCENTS, BM_SETCHECK, avAcc, 0);
			SendDlgItemMessage(hWnd, CHK_DBLSPC, BM_SETCHECK, avDblSpc==FALSE, 0);
			SendDlgItemMessage(hWnd, CHK_AVPUNT, BM_SETCHECK, avPunt, 0);
			SendDlgItemMessage(hWnd, CHK_AVLLETRA, BM_SETCHECK, avLletra, 0);
			SendDlgItemMessage(hWnd, CHK_NOSALTA, BM_SETCHECK, avNoSalta==FALSE, 0);
			SetDlgItemInt(hWnd, ED_SCOPE, avScope, FALSE);
			SetDlgItemInt(hWnd, ED_MAXSCOPE, avMaxScope, FALSE);
			if(af->txBtCorregir) SetDlgItemText(hWnd, ED_TXBTEVAL, af->txBtCorregir);
			SendDlgItemMessage(hWnd, CHK_PREVHELP, BM_SETCHECK, avPrevHelp!=0, 0);
			SendDlgItemMessage(hWnd, CHK_SAMETEXT, BM_SETCHECK, avPrevHelp!=2, 0);
			SendDlgItemMessage(hWnd, CHK_NOTSAME, BM_SETCHECK, avPrevHelp==2, 0);
			SetDlgItemInt(hWnd, ED_PREVHELP, avTimePH, FALSE);
			if(af->txPrev) SetDlgItemText(hWnd, ED_PREVTXT, af->txPrev);
			if(af->initMessPrev) SetDlgItemText(hWnd, ED_INIMESSPRV, af->initMessPrev);
			resetDlg();
			return TRUE;


	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_OKTONEXT:
					 okToNext^=1;
					 resetDlg();
					 break;

			  case CHK_BTEVAL:
					 btCorregir^=1;
					 resetDlg();
					 break;

			  case CHK_AVCONT:
					 avCont^=1;
					 resetDlg();
					 break;

			  case CHK_NOSALTA:
					 avNoSalta^=1;
					 break;

			  case CHK_MAJUSCULES:
					 avMaj^=1;
					 break;

			  case CHK_ACCENTS:
					 avAcc^=1;
					 break;

			  case CHK_DBLSPC:
					 avDblSpc^=1;
					 break;

			  case CHK_AVPUNT:
					 avPunt^=1;
					 break;

			  case CHK_AVLLETRA:
					 avLletra^=1;
					 resetDlg();
					 break;

			  case CHK_PREVHELP:
					 if(avPrevHelp) avPrevHelp=0;
					 else avPrevHelp=(SendDlgItemMessage(hWnd, CHK_SAMETEXT, BM_GETCHECK, 0, 0L)!=0 ? 1 : 2);
					 resetDlg();
					 break;

			  case CHK_SAMETEXT:
					 avPrevHelp=1;
					 resetDlg();
					 break;

			  case CHK_NOTSAME:
					 avPrevHelp=2;
					 resetDlg();
					 break;

			  case BOTO_OK:
					 af->okToNext=okToNext;
					 af->btCorregir=btCorregir;
					 af->avCont=avCont;
					 af->avNoSalta=avNoSalta;
					 af->avMaj=avMaj;
					 af->avAcc=avAcc;
					 af->avDblSpc=avDblSpc;
					 af->avPunt=avPunt;
					 af->avLletra=avLletra;
					 af->avScope=avScope;
					 af->avMaxScope=avMaxScope;
					 af->avPrevHelp=avPrevHelp;
					 af->avTimePH=avTimePH;
					 GetDlgItemText(hWnd, ED_TXBTEVAL, txtbuff, LMAXRFTXT-1);
					 if(af->txBtCorregir) delete af->txBtCorregir;
					 af->txBtCorregir=dupStr(txtbuff);
					 l1=GetWindowTextLength(GetDlgItem(hWnd, ED_PREVTXT));
					 if(af->txPrev) delete af->txPrev;
					 af->txPrev=new char[l1+1];
					 GetDlgItemText(hWnd, ED_PREVTXT, af->txPrev, l1+1);
					 l1=GetWindowTextLength(GetDlgItem(hWnd, ED_INIMESSPRV));
					 if(af->initMessPrev) delete af->initMessPrev;
					 af->initMessPrev=new char[l1+1];
					 GetDlgItemText(hWnd, ED_INIMESSPRV, af->initMessPrev, l1+1);
					 result=TRUE;
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 break;
			}
			break;

	 case WM_VSCROLL:
	 { ScrollBarItem sbi;
		switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
		{ case SB_SCOPE:
				 sbi.staticId=ED_SCOPE; sbi.currVal=&avScope;
				 sbi.maxVal=10, sbi.minVal=0;
				 break;

		  case SB_MAXSCOPE:
				 sbi.staticId=ED_MAXSCOPE; sbi.currVal=&avMaxScope;
				 sbi.maxVal=20, sbi.minVal=0;
				 break;

		  case SB_PREVHELP:
				 sbi.staticId=ED_PREVHELP; sbi.currVal=&avTimePH;
				 sbi.maxVal=300, sbi.minVal=0;
				 break;

		  default: return FALSE;
		}

		if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
	 }
	 break;

	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DEvalOp::resetDlg()
{
  int sh;

  EnableWindow(GetDlgItem(hWnd, CHK_AVCONT), af->puzMode==FORATS);
  EnableWindow(GetDlgItem(hWnd, CHK_NOSALTA), af->puzMode==FORATS);
  EnableWindow(GetDlgItem(hWnd, CHK_OKTONEXT), af->puzMode==FORATS && avCont==TRUE);
  EnableWindow(GetDlgItem(hWnd, CHK_BTEVAL), af->puzMode!=FORATS || avCont==FALSE);
  EnableWindow(GetDlgItem(hWnd, ED_TXBTEVAL), btCorregir==TRUE && (af->puzMode!=FORATS || avCont==FALSE));
  sh=(af->puzMode==FORATS || af->puzMode==COMPTEXT) ? SW_SHOW : SW_HIDE;
  ShowWindow(GetDlgItem(hWnd, CHK_AVLLETRA), sh);
  sh=(sh && avLletra==TRUE) ? SW_SHOW : SW_HIDE;
  ShowWindow(GetDlgItem(hWnd, ED_SCOPE), sh);
  ShowWindow(GetDlgItem(hWnd, ED_MAXSCOPE), sh);
  ShowWindow(GetDlgItem(hWnd, SB_SCOPE), sh);
  ShowWindow(GetDlgItem(hWnd, SB_MAXSCOPE), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC1), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC2), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC3), sh);

  sh=((avPrevHelp!=0) ? SW_SHOW:SW_HIDE);
  ShowWindow(GetDlgItem(hWnd, CHK_SAMETEXT), sh);
  ShowWindow(GetDlgItem(hWnd, CHK_NOTSAME), sh);
  ShowWindow(GetDlgItem(hWnd, ED_PREVHELP), sh);
  ShowWindow(GetDlgItem(hWnd, SB_PREVHELP), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC4), sh);
  ShowWindow(GetDlgItem(hWnd, ED_INIMESSPRV), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC_INIMSPR), sh);
  ShowWindow(GetDlgItem(hWnd, STATIC_LABEL0), sh);
  ShowWindow(GetDlgItem(hWnd, ED_PREVTXT), avPrevHelp==2 ? SW_SHOW:SW_HIDE);

  return TRUE;
}
#endif

// ------- DAutoInc ------------
#ifndef RUNTIME
LPCSTR DAutoInc::getDialogName() {return MAKEINTRESOURCE(DIALEG_AUTOINC);}
int DAutoInc::getHelpItem() {return HELP_AUTOINC;}

DAutoInc::DAutoInc(HWND hOwner):ModalDialog(hOwner)
{
  expr=FALSE;
  txInc=NULL;
  onlyWords=TRUE;
}

DAutoInc::~DAutoInc()
{
  if(txInc) {delete txInc; txInc=NULL;}
}

BOOL DAutoInc::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			resetDlg();
			return TRUE;


	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_PARAULES:
			  case CHK_LLETRES:
					 expr^=1;
					 resetDlg();
					 break;

			  case CHK_ONLYWORDS:
					 onlyWords^=1;
					 break;

			  case BOTO_OK:
					 GetDlgItemText(hWnd, expr ? ED_LLETRES : ED_PARAULES, txtbuff, LMAXRFTXT-1);
					 if(txInc) delete txInc;
					 txInc=dupStr(txtbuff);
					 result=TRUE;
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 break;
			}
			break;

	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}


BOOL DAutoInc::resetDlg()
{
  SendDlgItemMessage(hWnd, CHK_PARAULES, BM_SETCHECK, !expr, 0);
  EnableWindow(GetDlgItem(hWnd, ED_PARAULES), !expr);
  SendDlgItemMessage(hWnd, CHK_LLETRES, BM_SETCHECK, expr, 0);
  EnableWindow(GetDlgItem(hWnd, ED_LLETRES), expr);
  SendDlgItemMessage(hWnd, CHK_ONLYWORDS, BM_SETCHECK, onlyWords, 0);
  EnableWindow(GetDlgItem(hWnd, CHK_ONLYWORDS), !expr);

  return TRUE;
}
#endif

// ------- DConfigura ------------
#ifndef RUNTIME
LPCSTR DConfigura::getDialogName() {return MAKEINTRESOURCE(DIALEG_BARALLAR);}
int DConfigura::getHelpItem() {return HELP_CONFI;}

DConfigura::DConfigura(HWND hOwner):ModalDialog(hOwner)
{ dbChanged=FALSE;
  p=passos; q=quick; a=arrossega; s=sonor;
  ac=autoCursor; acd=autoCursorDelay;
  gfil=gruixfil;
  cg=cursorGran;
  hoh=helponhelp;
  mn=menus;
  nu=allowNewUsr;
  ng=allowNewGrp;
  ed=(exitDirect!=TRUE);
  iu=showUserReports;

  strcpy(workChangedPath, workPath);
  if(strlen(workChangedPath)>3)
	 workChangedPath[strlen(workChangedPath)-1]=0;
  rep=doReportFile;
  setActivePath(workChangedPath);
}

BOOL DConfigura::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			SetDlgItemText(hWnd, ED_WORKPATH, workChangedPath);
			SetDlgItemText(hWnd, ED_PNAME, gv->dbFile);
			SetDlgItemText(hWnd, ED_EDITEXT, gv->ediText);
			SetDlgItemText(hWnd, ED_EDIGRAF, gv->ediGraf);
			resetDlg();
			return TRUE;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_PASSOS:
						sbi.staticId=IDB_PASSADES; sbi.currVal=&p;
						sbi.maxVal=200, sbi.minVal=1;
						break;

				 case SB_CURSD:
						sbi.staticId=TX_CURSD; sbi.currVal=&acd;
						sbi.maxVal=40, sbi.minVal=1;
						break;

				 case SB_AMPFIL:
						sbi.staticId=TX_AMPFIL; sbi.currVal=&gfil;
						sbi.maxVal=20, sbi.minVal=1;
						break;

				 default: return FALSE;
			  }

			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_REPORT:
					 rep^=1;
					 resetDlg();
					 break;

			  case CHK_HELPONHELP: hoh^=1; break;
			  case CHK_MENUS: mn^=1; break;
			  case IDB_QUICK: q^=1; break;
			  case CHK_SO:    s^=1; break;
			  case CHK_FIL:   a^=1; break;
			  case CHK_CURSORGRAN: cg^=1; break;
			  case CHK_EXITDIRECT: ed^=1; break;
			  case CHK_NU: nu^=1;
			  case CHK_INFUSR: iu^=1;
					 resetDlg();
					 break;
			  case CHK_NG: ng^=1; break;

			  case CHK_CURSD:
					 ac^=1;
					 resetDlg();
					 break;

			  case BOTO_OK:
					 GetDlgItemText(hWnd, ED_WORKPATH, workChangedPath,
										 sizeof(workChangedPath)-1);
					 if(!setActivePath(workChangedPath))
					 { BMsgBox(hWnd, IDS_ERRPATH, 0, 0, 0,
								  MB_OK|MB_ICONEXCLAMATION);
						break;
					 }
					 strcpy(workPath, curPath);
					 if(rep)
					 { GetDlgItemText(hWnd, ED_PNAME, tempPath3, MAX_FULL_FILENAME-1);
						if (stricmp(tempPath3, gv->dbFile)!=0)
						{ changeStrVar(gv->dbFile, tempPath3);
						  dbChanged=TRUE;
						}
					 }
					 doReportFile=rep;
					 passos=p;
					 quick=q; arrossega=a; sonor=s;
					 autoCursor=ac; autoCursorDelay=acd;
					 gruixfil=gfil;
					 cursorGran=cg;
					 helponhelp=hoh;
					 menus=mn;
					 exitDirect=(ed!=TRUE);
					 GetDlgItemText(hWnd, ED_EDITEXT, tempPath3, MAX_FULL_FILENAME-1);
					 changeStrVar(gv->ediText, tempPath3);
					 GetDlgItemText(hWnd, ED_EDIGRAF, tempPath3, MAX_FULL_FILENAME-1);
					 changeStrVar(gv->ediGraf, tempPath3);
					 allowNewUsr=nu;
					 allowNewGrp=ng;
					 showUserReports=iu;
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case BOTO_DEFAULT:
					 rep=drf0;
					 p=passos0;
					 hoh=TRUE;
					 mn=TRUE;
					 ed=TRUE; // va al reves
					 q=quick0; a=arrossega0; s=sonor0; cg=0;
					 gfil=1;
					 strcpy(workChangedPath, gv->originalPath);
					 if(strlen(workChangedPath)>3)
						workChangedPath[strlen(workChangedPath)-1]=0;
					 SetDlgItemText(hWnd, ED_WORKPATH, workChangedPath);
					 SetDlgItemText(hWnd, ED_PNAME, gv->originalDbFile);
					 GetWindowsDirectory(tempPath3, MAX_FULL_FILENAME);
					 strcat(tempPath3, "\\notepad.exe");
					 SetDlgItemText(hWnd, ED_EDITEXT, tempPath3);
					 GetWindowsDirectory(tempPath3, MAX_FULL_FILENAME);
					 strcat(tempPath3, "\\pbrush.exe");
					 SetDlgItemText(hWnd, ED_EDIGRAF, tempPath3);
					 nu=TRUE;
					 ng=FALSE;
					 iu=TRUE;
					 resetDlg();
					 return TRUE;

			  case BOTO_EDITEXT:
					 browseFn(ED_EDITEXT, IDS_EXEFILES, IDS_SELEDITEXT);
					 break;

			  case BOTO_EDIGRAF:
					 browseFn(ED_EDIGRAF, IDS_EXEFILES, IDS_SELEDIGRAF);
					 break;

			  case BOTO_DBFILE:
					 browseFn(ED_PNAME, IDS_DBFILES, IDS_SELDBFILE);
					 break;

			  case BOTO_WORKPATH:
					 { DWorkDir *wd;
						GetDlgItemText(hWnd, ED_WORKPATH, tempPath3, MAX_FULL_FILENAME);
						if((wd=new DWorkDir(hWnd, tempPath3, TRUE, TRUE))!=NULL)
						{ if(wd->run())
							 SetDlgItemText(hWnd, ED_WORKPATH, wd->path);
						  delete wd;
						}
						else NOMEM;
					 }
					 break;

			  default: break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

void DConfigura::resetDlg()
{
  SendDlgItemMessage(hWnd, CHK_EXITDIRECT, BM_SETCHECK, ed, 0);
  SendDlgItemMessage(hWnd, CHK_REPORT, BM_SETCHECK, rep, 0);
  SetDlgItemInt(hWnd, IDB_PASSADES, p, FALSE);
  SendDlgItemMessage(hWnd, IDB_QUICK, BM_SETCHECK, q, 0);
  SendDlgItemMessage(hWnd, CHK_FIL, BM_SETCHECK, a, 0);
  SendDlgItemMessage(hWnd, CHK_SO, BM_SETCHECK, s, 0);
  SendDlgItemMessage(hWnd, CHK_CURSORGRAN, BM_SETCHECK, cg, 0);
  EnableWindow(GetDlgItem(hWnd, BOTO_DBFILE), rep);
  EnableWindow(GetDlgItem(hWnd, ED_PNAME), rep);
  SendDlgItemMessage(hWnd, CHK_CURSD, BM_SETCHECK, ac, 0);
  SetDlgItemInt(hWnd, TX_CURSD, acd, FALSE);
  EnableWindow(GetDlgItem(hWnd, TX_CURSD), ac);
  EnableWindow(GetDlgItem(hWnd, SB_CURSD), ac);
  SetDlgItemInt(hWnd, TX_AMPFIL, gfil, FALSE);
  SendDlgItemMessage(hWnd, CHK_HELPONHELP, BM_SETCHECK, hoh, 0);
  SendDlgItemMessage(hWnd, CHK_MENUS, BM_SETCHECK, mn, 0);
  SendDlgItemMessage(hWnd, CHK_NU, BM_SETCHECK, nu, 0);
  SendDlgItemMessage(hWnd, CHK_NG, BM_SETCHECK, ng, 0);
  SendDlgItemMessage(hWnd, CHK_INFUSR, BM_SETCHECK, iu, 0);
  EnableWindow(GetDlgItem(hWnd, CHK_NU), rep);
  EnableWindow(GetDlgItem(hWnd, CHK_NG), nu && rep);
  EnableWindow(GetDlgItem(hWnd, CHK_INFUSR), rep);
}


void DConfigura::browseFn(int field, int ext, int mess)
{
  OPENFILENAME ofn;
  char retFile[MAX_FULL_FILENAME];
  char  r, *filter, *t, *iPath;
  int i;
  SaveDir sd;

  GetDlgItemText(hWnd, field, retFile, sizeof(retFile));
  if((iPath=strdup(retFile))==NULL) return;
  if((t=strrchr(iPath, '\\'))!=0)
	 *t=0;
  else
	 *iPath=0;

  if((filter=dupStr(getStringTbl(ext)))==NULL) return;
  r = filter[strlen(filter)-1];
  for(i = 0; filter[i]!=0; i++)
	 if(filter[i] == r) filter[i]=0;

  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= retFile;
  ofn.nMaxFile = sizeof(retFile);
  ofn.lpstrInitialDir = iPath;
  ofn.lpstrTitle=getStringTbl(mess);
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if (GetOpenFileName(&ofn))
	 SetDlgItemText(hWnd, field, retFile);

  delete filter;
  delete iPath;
}
#endif

// ------- DAjuda -----------

#define IDLIST 101
#define IDTXWIN 102

LPCSTR DAjuda::getDialogName() {return MAKEINTRESOURCE(DIALEG_AJUDA);}

int DAjuda::getHelpItem()
{
  int i;

  switch(tipus)
  { case PUZZLE:  i=HELP_PUZZLE; break;
	 case ASSOCIA:
			switch(mode)
			{ case IDENTIFICA: i=HELP_IDENT; break;
			  case INFO:       i=HELP_INFO; break;
			  case ESCRIU:		 i=HELP_ESCRIU; break;
			  case EXPLORA:    i=HELP_EXPLORE; break;
			  case ESPECIAL:   i=HELP_ASSREL; break;
			  default:         i=HELP_ASSNORM; break;
			}
			break;
	 case SOPA:    i=HELP_SOPA; break;
	 case CREUATS: i=HELP_CREUATS; break;
	 case TEXTACT: i=HELP_TXA; break;
	 default: i=HELP_CLIC_INDEX;
  }
  return i;
}

DAjuda::DAjuda(HWND hOwner, int tipusAct, int modeAct, RECT *ibmrect,
					HBITMAP ihBm, char **itxtp, BOOL ilist):ModalDialog(hOwner)
{
  text=0;
  hBm=ihBm;
  txtp=itxtp;
  list=ilist && itxtp!=0;
  bmRect=*ibmrect;
  maxlen=0;
  wRect=NullRect;
  wRect.right=RECTW(bmRect)+2*MARGE;
  wRect.bottom=RECTH(bmRect)+3*MARGE+BOTOH;
  cRect.left=MARGE;
  cRect.right=cRect.left+RECTW(bmRect);
  cRect.top=MARGE;
  cRect.bottom=cRect.top+RECTH(bmRect);
  if(txtp && !list)
  { char **t;
	 int l;

	 for(l=0, t=txtp; *t!=0; t++) l+=strlen(*t)+1;
	 if((text=newChar(l+1))!=0)
	 { *text=0;
		for(t=txtp; *t!=0; t++)
		{ strcat(text, *t); strcat(text, spcStr);}
	 }
  }
  tipus=tipusAct;
  mode=modeAct;
}

DAjuda::~DAjuda()
{
  if(text) delete text; text=0;
}

void DAjuda::initDlg()
{
  POINT p;
  int x0, y0, y;

  p.x=0; p.y=0;
  ClientToScreen(hOwn, (LPPOINT)&p);
  p.x-=2*MARGE; p.y-=(MARGE+(y=captionY));
  MoveWindow(hWnd, p.x, p.y, RECTW(wRect), RECTH(wRect)+y, TRUE);

  y0=wRect.bottom-MARGE-BOTOH;

  if(!helponhelp)
  { x0=MARGE+(RECTW(cRect)-BOTOW-MARGE)/2;
	 ShowWindow(GetDlgItem(hWnd, IDHELP), SW_HIDE);
	 MoveWindow(GetDlgItem(hWnd, BOTO_SURT), x0, y0, BOTOW+2, BOTOH+2, TRUE);
  }
  else
  { x0=MARGE+(RECTW(cRect)-2*BOTOW-MARGE)/2;
	 MoveWindow(GetDlgItem(hWnd, BOTO_SURT), x0, y0, BOTOW+2, BOTOH+2, TRUE);
	 MoveWindow(GetDlgItem(hWnd, IDHELP), x0+MARGE+BOTOW, y0,
				 BOTOW+2, BOTOH+2, TRUE);
  }

  if(list)
  { char **t;
	 int l;

	 llista.create("Listbox", hWnd, IDLIST, LBS_NOTIFY|WS_VSCROLL|WS_BORDER,
		  nullStr, cRect.left, cRect.top, RECTW(cRect), RECTH(cRect));

	 for(t=txtp; *t!=0; t++)
	 { llista.sendMess(LB_ADDSTRING, NULL, (LPARAM)(LPSTR)*t);
		if((l=strlen(*t))>maxlen) maxlen=l;
	 }
  }
  else if(tipus==TEXTACT)
  { txwin.create("EDIT", hWnd, IDTXWIN, WS_BORDER|ES_MULTILINE|ES_READONLY|WS_VSCROLL,
					  text, cRect.left, cRect.top, RECTW(cRect), RECTH(cRect));

	 SendMessage(txwin.hWnd, WM_SETFONT, (WPARAM)Zona::getBC(GRA)->hf, 0L);
  }
  else
  { cont.create(hWnd, &cRect);
	 if(hBm) cont.getData(hBm);
	 else {cont.getData(text); cont.setCont(TXT);}
  }
}

BOOL DAjuda::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  POINT p;
  RECT r;

  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_PAINT:
			if(!list && tipus!=TEXTACT)
			{ HDC hDC;
			  PAINTSTRUCT ps;

			  hDC=BeginPaint(hWnd, &ps);

			  if(cont.isIn(&ps.rcPaint))
			  { cont.update(hDC);
				 FrameRect(hDC, (LPRECT)cont.getRect(),
							  (HBRUSH)GetStockObject(BLACK_BRUSH));
			  }
			  EndPaint(hWnd, &ps);
			  return TRUE;
			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case BOTO_SURT:
					 cont.destroy();
					 llista.destroy();
					 txwin.destroy();
					 result=TRUE;
					 end();
					 return TRUE;

			  case IDLIST:
					 if(HIWORD(lParam)==LBN_SELCHANGE)
					 { DWORD l;
						if((l=llista.sendMess(LB_GETCURSEL, 0, 0L))!=(unsigned)LB_ERR)
						  PostMessage(hOwn, WM_HELPW, HELPW_ITEMSEL, l);
						return TRUE;
					 }
					 break;

			  default: break;
			}
			break;

	 case WM_LBUTTONDOWN:
			p.x=LOWORD(lParam);
			p.y=HIWORD(lParam);
			if(hBm && cont.isIn(p))
			{ p.x-=cont.getRect()->left; p.y-=cont.getRect()->top;
			  PostMessage(hOwn, WM_HELPW, HELPW_MOUSE, MAKELPARAM(p.x, p.y));
			  return TRUE;
			}
			break;

	 case WM_ERASEBKGND:
			GetClientRect(hWnd, &r);
			FillRect((HDC)wParam, &r, Zona::getBaseBrush(GENERICA));
			return TRUE;

	 default: break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}

// ------- DFitx -----------
#ifndef RUNTIME
int tipusId[6]={BOTO_PAQUET, BOTO_PUZZLE, BOTO_ASSOCIA, BOTO_SOPA, BOTO_CREUATS, BOTO_TEXTACT};
int tipusExt[6]={EXT_PCG, EXT_PUZ, EXT_ASS, EXT_SOP, EXT_CRW, EXT_TXA};

LPCSTR DFitx::getDialogName() {return MAKEINTRESOURCE(DIALEG_FITXERS);}
int DFitx::getHelpItem() {return (me ? HELP_CARREGA:HELP_DESA);}

DFitx::DFitx(HWND hOwner,
				 char *caption, char *iniPath, char *iniName, char *iniDesc,
				 int extensio, BOOL mustExist) : ModalDialog(hOwner)
{
  capt=caption;
  iPath=iniPath;
  iName=iniName;
  memset(desc, 0, sizeof(desc));
  if(iniDesc) strncpy(desc, iniDesc, MAX_DESC);
  ext=extensio;
  me=mustExist;
  isHere=FALSE;

  *path=0;

  if(iPath!=0 && *iPath!=0) strcpy(path, iPath);
  else getcwd(path, sizeof(path));
}

UINT DFitx::run()
{
  SaveDir sd;
  if(ModalDialog::run()==FALSE) return FALSE;
  strupr(path);
  return result;
}

BOOL DFitx::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG: return initDlg();

	 case WM_COMMAND:
			switch(wParam)
			{ case LIST_FITX:
					 if(HIWORD(lParam)==LBN_DBLCLK)
						return dispatch(msg, BOTO_OK, lParam);
			  case LIST_DIRS:
					 return processList(wParam, lParam);

			  case BOTO_OK:
					 if(!checkOk()) break;
					 result=TRUE;
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default:
					 for(int i=0; i<6; i++)
						if(wParam==tipusId[i] && ext!=tipusExt[i])
						{ ext=tipusExt[i];
						  strcpy(path, extName[ext]);
						  return resetDlg();
						}
					 break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DFitx::initDlg()
{
  HWND hEditFitx;

  if(!me)
	 for(int i=0; i<6; i++)
		if(ext!=tipusExt[i])
		  EnableWindow(GetDlgItem(hWnd, tipusId[i]), FALSE);

  SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(LPSTR) capt);

  EnableWindow(GetDlgItem(hWnd, EDIT_INFO), !me);
  SendDlgItemMessage(hWnd, EDIT_INFO, EM_LIMITTEXT, MAX_DESC, 0);
  SetDlgItemText(hWnd, EDIT_INFO, desc);

  SendDlgItemMessage(hWnd, EDIT_FITX, EM_LIMITTEXT, MAXPATH, 0);
  if(!me && (hEditFitx=GetDlgItem(hWnd, EDIT_FITX))!=0)
	 EnableWindow(hEditFitx, TRUE);

  if(path[strlen(path)-1]!='\\') strcat(path, "\\");
  strcat(path, extName[ext]);

  return resetDlg();
}

BOOL DFitx::resetDlg()
{
  int i;

  switch(ext)
  { case EXT_PUZ: i=IDS_BMP; break;
	 case EXT_ASS: i=IDS_ASS; break;
	 case EXT_SOP: i=IDS_SOPA; break;
	 case EXT_CRW: i=IDS_CRW; break;
	 case EXT_PAC: i=IDS_PCG; break;
	 case EXT_PCC: i=IDS_PCG; break;
	 case EXT_PCG: i=IDS_PCG; break;
	 case EXT_TXA: i=IDS_TEXTACT; break;
	 default: i=-1;
  }
  SetDlgItemText(hWnd, INFO_MOD, i==-1 ? nullStr:getStringTbl(i));

  setCursor(hWnd, CURSOR_ESPERA);
  if(DlgDirList(hWnd, path, LIST_DIRS, INFO_PATH, 0xC010)==0)
  { strcpy(path, extName[ext]);
	 DlgDirList(hWnd, path, LIST_DIRS, INFO_PATH, 0xC010);
  }
  DlgDirList(hWnd, path, LIST_FITX, INFO_PATH, 0x0001);

  strcpy(tempPath3, path);

  if(me) SendDlgItemMessage(hWnd, LIST_FITX, LB_GETTEXT,
					0, (LPARAM)(LPSTR)tempPath3);
  else if(iName) strcpy(tempPath3, iName);
  updateFName(tempPath3);
  resetCursor();

  return TRUE;
}

BOOL DFitx::processList(WPARAM wParam, LPARAM lParam)
{
  switch(HIWORD(lParam))
  { case LBN_DBLCLK:
			if(wParam==LIST_FITX) return dispatch(WM_COMMAND, BOTO_OK, 0L);
			else return resetDlg();

	 case LBN_SELCHANGE:
			if(wParam==LIST_FITX)
			{ DlgDirSelect(hWnd, tempPath3, LIST_FITX);
			  updateFName(tempPath3);
			}
			else if(DlgDirSelect(hWnd, path, LIST_DIRS)!=0)
			{ strcat(path, extName[ext]);
			  updateFName(path);
			}
			break;

	 default: break;
  }
  return TRUE;
}

BOOL DFitx::updateFName(char *fName)
{
  char **txt=0;
  int fLine=0, ver, nLines;

  if(me)
  { *desc=0;
	 if(strchr(fName, '*')==NULL)
	 { if(getExt(fName)==EXT_PCC)
		{ HFILE hf;
		  if (readFCHeader(fName, hf, desc, TRUE)!=0) *desc=0;
		}
		else
		{ if((txt=readTxtFile(fName, nLines))!=0)
		  { if(nLines>2 && (ver=atoi(txt[fLine++]))>107 && ver<=CLICVER)
			 { while(fLine<nLines)
				{ if((unsigned char)*txt[fLine++]==ENDDESC) break;
				  if(*desc) strcat(desc, "\r\n");
				  strncat(desc, txt[fLine-1], MAX_DESC-strlen(desc));
				}
			 }
			 delArray(txt);
		  }
		}
	 }
	 SetDlgItemText(hWnd, EDIT_INFO, desc);
  }

  SetDlgItemText(hWnd, EDIT_FITX, fName);
  return TRUE;
}

BOOL DFitx::checkOk()
{
  ffblk fileInfo;
  char lastChar;
  char *p;
  int l;
  BOOL arreglat=FALSE;

  *tempPath3=0;
  GetDlgItemText(hWnd, EDIT_FITX, tempPath3, MAX_FULL_FILENAME);

  if(*tempPath3==0 ||
	  (lastChar=tempPath3[strlen(tempPath3)-1])=='\\' ||
	  lastChar==':')
  { resetDlg();
	 return FALSE;
  }

  if(strpbrk(tempPath3, "*?")!=0)
  { avisSonor(0);
	 return FALSE;
  }

  isHere=(findfirst(tempPath3, &fileInfo, 0)==0);
  if(me)
  { if(!isHere)
	 { avisSonor(0);
		return FALSE;
	 }
  }
  else
  { if((p=strchr(strupr(tempPath3), '.'))==0) l=strlen(tempPath3);
	 else l=(int)(p-tempPath3);

	 if(p==0 || l==0 || l>8) {tempPath3[8]=0; p=0; arreglat=TRUE;}
	 if(p && strcmp(p, extName[ext]+1)!=0) {*p=0; arreglat=TRUE;}

	 if(arreglat)
	 { strcat(tempPath3, extName[ext]+1);
		updateFName(tempPath3);
		avisSonor(0);
		return FALSE;
	 }

	 if(isHere && (iName==0 || strcmp(iName, tempPath3)!=0) &&
		 BMsgBox(hWnd, IDS_EXIST0, IDS_EXIST1, 0, tempPath3,
		 MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return FALSE;
  }

  getcwd(path, sizeof(path));
  if(path[strlen(path)-1]!='\\') strcat(path, "\\");
  strcat(path, tempPath3);

  GetDlgItemText(hWnd, EDIT_INFO, desc, MAX_DESC);

  return TRUE;
}
#endif

// ------- DWorkDir -----------
#ifndef RUNTIME
LPCSTR DWorkDir::getDialogName() {return MAKEINTRESOURCE(DIALEG_WORKDIR);}
int DWorkDir::getHelpItem() {return (HELP_WORKDIR);}

DWorkDir::DWorkDir(HWND hOwner, char *iniPath, BOOL iNoMes, BOOL iHideNoMes) : ModalDialog(hOwner)
{
  *path=0;
  if(iniPath!=0 && *iniPath!=0) strcpy(path, iniPath);
  else getcwd(path, sizeof(path));

  noMes=iNoMes;
  hideNoMes=iHideNoMes;
}

UINT DWorkDir::run()
{
  SaveDir sd;
  if(ModalDialog::run()==FALSE) return FALSE;
  strupr(path);
  return result;
}

BOOL DWorkDir::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  { case WM_INITDIALOG:
			if(hideNoMes) ShowWindow(GetDlgItem(hWnd, CHK_MAIMES), SW_HIDE);
			return resetDlg();

	 case WM_COMMAND:
			switch(wParam)
			{ case LIST_DIRS:
					 switch(HIWORD(lParam))
					 { case LBN_SELCHANGE:
							  if(DlgDirSelect(hWnd, path, LIST_DIRS)==0) break;
							  return resetDlg();

						default: break;

					 }
					 break;

			  case BOTO_OK:
					 GetDlgItemText(hWnd, INFO_PATH, path, sizeof(path)-1);
					 /*
					 if(!setActivePath(path))
					 { BMsgBox(hWnd, IDS_ERRPATH, 0, 0, 0,
								  MB_OK|MB_ICONEXCLAMATION);
						break;
					 }
					 strcpy(workPath, curPath);
					 */
					 result=TRUE;
			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case CHK_MAIMES:
					 noMes^=1;
					 break;

			  default:
					 break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}

BOOL DWorkDir::resetDlg()
{
  SendDlgItemMessage(hWnd, CHK_MAIMES, BM_SETCHECK, noMes, 0);
  DlgDirList(hWnd, path, LIST_DIRS, INFO_PATH, 0xC010);
  return TRUE;
}
#endif

// ------- DConfirmOps -----------
#ifndef RUNTIME
LPCSTR DConfirmOps::getDialogName() {return MAKEINTRESOURCE(DIALEG_CONFIRM);}
int DConfirmOps::getHelpItem() {return (HELP_CONFIGURE);}

DConfirmOps::DConfirmOps(HWND hOwner) : ModalDialog(hOwner)
{ noMes=FALSE;
}

BOOL DConfirmOps::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{ switch(msg)
  { case WM_COMMAND:
			switch(wParam)
			{ case CHK_MAIMES:
					 noMes^=1;
					 break;

			  case IDYES:
					 result=TRUE;
			  case IDNO:
					 end();
					 return TRUE;
			}
			break;
	 default:
			break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}
#endif

