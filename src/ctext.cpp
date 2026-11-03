// -----------------------------------
// CTEXT.CPP
// Implementacio de les classes:
//  - Incognita
//  - TxContenidor
// -----------------------------------

#include "ctext.h"
#include "zones.h"
#include "playwin.h"
#include "rcomu\clicres.h"
#include "miscfunc.h"
#include "clicdlg.h"
#include "cmpstr.h"
#include "wcl.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char chFBC[]={ENDDESC, CHBLOCK, CHINC, SEP, 0};
BOOL isSpace(char ch) {return (strchr(" ,.-;:_\t", ch)!=NULL);}
//BOOL isSep(char ch) {return (strchr(" .,;:'-\r\n\t", ch)!=NULL);}
BOOL isSep(char ch) {return (strchr(" .,;:'\r\n\t", ch)!=NULL);}
BOOL isForbiddenChar(char ch) {return (ch<' ' || strchr(chFBC, ch)!=NULL);}

struct FloatInc *finci;
#define ITERAFI for(finci=finc; finci!=NULL; finci=finci->next)

struct Cntptr *cnti;
#define ITERACNT for(cnti=cnt; cnti!=NULL; cnti=cnti->next)

Incognita *inci;
#define ITERAINC for(inci=inc; inci!=NULL; inci=inci->next)

Incognita::Incognita()
{ memset(this, 0, sizeof(Incognita));
  chFarciment='_';
  flagsInc=INFO_LA;
}

Incognita::~Incognita()
{ if(hCBList) DestroyWindow(hCBList);
  if(ok) {delete ok; ok=NULL;}
  if(opcions) {delete opcions; opcions=NULL;}
  if(iniText) {delete iniText; iniText=NULL;}
  if(infoTxt) {delete infoTxt; infoTxt=NULL;}
}


TxContenidor::TxContenidor(BASE_ZONES Base) : Contenidor(Base)
{
  af=NULL;
  cnt=NULL;
  txNow=NULL;
  txAttr=NULL;
  noMemAvail=FALSE;
  scrBar=NULL;
  memLocked=0;
  selActiva=FALSE;
  inc=NULL;
  incNow=NULL;
  finc=NULL;
  fincNow=NULL;
  anchorFinc=NULL;
  edit=FALSE;
  playing=FALSE;
  nav=NO_NAV;
  incNowText=NULL;
  btCorregir=NULL;
  ok=FALSE;
  incFull=FALSE;
  btHeight=GetSystemMetrics(SM_CYCAPTION)+8;
  txMaxLen=TXCBASELEN;
  nMaxLines=TXCBASELIN;
  waiting=FALSE;
  filOn=FALSE;

  if((hgTxNow=GlobalAlloc(GHND, txMaxLen+10))==NULL ||
	  (hgTxAttr=GlobalAlloc(GHND, txMaxLen+10))==NULL ||
	  (hgLin=GlobalAlloc(GHND, (nMaxLines+10) * sizeof(struct txLine)))==NULL)
  {NOMEM; noMemAvail=TRUE; txMaxLen=0; nMaxLines=0;}
  else
	 clear();

  if(farProcCount==0)
  { lpfnCBSubClassProc=MakeProcInstance((FARPROC)CBSubClassProc, WinBase::hInst);
	 lpfnOldCBClassProc=NULL;
  }
  farProcCount++;
}

TxContenidor::~TxContenidor()
{
  clear();
  if(!noMemAvail) while(memLocked) unlockMem();
  if(hgTxNow) GlobalFree(hgTxNow);
  if(hgTxAttr) GlobalFree(hgTxAttr);
  if(hgLin) GlobalFree(hgLin);
  farProcCount--;
  if(farProcCount<=0)
  { FreeProcInstance(lpfnCBSubClassProc);
	 lpfnCBSubClassProc=NULL;
	 lpfnOldCBClassProc=NULL;
  }
  //if(btCorregir) {DestroyWindow(btCorregir); btCorregir=NULL; pos.bottom+=btHeight;}
}

BOOL TxContenidor::create(HWND hOwner, RECT *rpos, BOOL iHid, BOOL iMark, BASE_ZONES base)
{
  popup.create(hOwner);
  return Contenidor::create(hOwner, rpos, iHid, iMark, base);
}


BOOL TxContenidor::lockMem()
{
  if(noMemAvail) return FALSE;
  if(memLocked==0)
  { if(hgTxNow) txNow=(char far*)GlobalLock(hgTxNow);
	 if(hgTxAttr) txAttr=(char far*)GlobalLock(hgTxAttr);
	 if(hgLin) lin=(struct txLine far*)GlobalLock(hgLin);
  }
  memLocked++;
  return TRUE;
}

BOOL TxContenidor::unlockMem()
{
  if(noMemAvail) return FALSE;
  if(memLocked==1)
  { if(hgTxNow) GlobalUnlock(hgTxNow); txNow=NULL;
	 if(hgTxAttr) GlobalUnlock(hgTxAttr); txAttr=NULL;
	 if(hgLin) GlobalUnlock(hgLin); lin=NULL;
  }
  memLocked--;
  return TRUE;
}

BOOL TxContenidor::growMem(int howManyMore)
{
  HGLOBAL hgNewTxNow, hgNewTxAttr;
  int incr;
  BOOL result=FALSE;

  incr=((howManyMore/TXCBASELEN)+1)*TXCBASELEN;

  if(noMemAvail || (txMaxLen+incr)>TXCMAXLEN) {NOMEM; return FALSE;}
  if(memLocked)
  { GlobalUnlock(hgTxNow);
	 GlobalUnlock(hgTxAttr);
  }

  if((hgNewTxNow=GlobalReAlloc(hgTxNow, txMaxLen+10+incr, GHND))!=NULL &&
	  (hgNewTxAttr=GlobalReAlloc(hgTxAttr, txMaxLen+10+incr, GHND))!=NULL)
  { hgTxNow=hgNewTxNow;
	 hgTxAttr=hgNewTxAttr;
	 txMaxLen+=incr;
	 result=TRUE;
  }

  if(memLocked)
  { if(hgTxNow) txNow=(char far*)GlobalLock(hgTxNow);
	 if(hgTxAttr) txAttr=(char far*)GlobalLock(hgTxAttr);
  }

  if(!result) NOMEM;

  return result;
}


BOOL TxContenidor::growLin()
{
  HGLOBAL hgNewLin;
  BOOL result=FALSE;

  if(noMemAvail || (nLines+TXCBASELIN)>TXCMAXLIN) {NOMEM; return FALSE;}
  if(memLocked) GlobalUnlock(hgLin);

  if((hgNewLin=GlobalReAlloc(hgLin, (nMaxLines+10+TXCBASELIN)*sizeof(struct txLine), GHND))!=NULL)
  { hgLin=hgNewLin;
	 nMaxLines+=TXCBASELIN;
	 result=TRUE;
  }

  if(memLocked) lin=(struct txLine far*)GlobalLock(hgLin);

  if(!result) NOMEM;

  return result;
}


void TxContenidor::clear()
{
  Contenidor::clear();

  while(cnt) {cnti=cnt->next; delete cnt; cnt=cnti;}
  while(inc) {inci=inc->next; delete inc; inc=inci;}
  while(finc) {finci=finc->next; delete finc; finc=finci;}
  incNow=NULL;
  fincNow=NULL;
  anchorFinc=NULL;
  playing=FALSE;
  waiting=FALSE;
  setBase(GRA);
  ok=FALSE;
  fil.end();
  filOn=FALSE;

  if(scrBar) {DestroyWindow(scrBar); scrBar=NULL;}
  if(!lockMem()) return;
  txLen=0;
  nLines=0;
  firstLine=0;
  _fmemset(txNow, 0, txMaxLen);
  _fmemset(txAttr, 0, txMaxLen);
  _fmemset(lin, 0, nMaxLines*sizeof(struct txLine));
  memset(widthTable, 0, sizeof(widthTable));
  lineHeight=0;
  leftMargin=0;
  rightMargin=0;
  af=NULL;
  selActiva=FALSE;
  nav=NO_NAV;
  scrWidth=GetSystemMetrics(SM_CXVSCROLL);
  tabLen=16*scrWidth;
  if(incNowText) {delete incNowText; incNowText=NULL;}
  if(btCorregir) {DestroyWindow(btCorregir); btCorregir=NULL; pos.bottom+=btHeight;}
  deBoto=FALSE;
  popup.show(FALSE);
  unlockMem();
}


BOOL TxContenidor::getActData(ActFile *afgd, BOOL onlyShow)
{
  int i, k, j, v, b, la, s;
  RECT r;
  struct Cntptr *cnti2;
  Incognita *inc2;
  struct FloatInc *finc2=NULL;
  BOOL result=TRUE;
  char *srcPtr;
  char ch, *tx;
  BOOL inInc=FALSE;
  int iniInc=0;
  int srcPos=0;
  WNDPROC fp;
  int p[5];

  if(!isCreated()) return FALSE;

  if(!lockMem()) return FALSE;

  clear();
  af=afgd;
  if(onlyShow) setBase(POP);

  calcFont();

  firstLine=0;
  srcPtr=(onlyShow==TRUE && af->avPrevHelp==2) ? af->txPrev : af->txBase;
  finci=NULL;

  for(s=0, i=0; ; s++)
  { if((ch=srcPtr[s])==0) break;

	 if(i>=txMaxLen && !growMem()) break;
	 if(ch==CHBLOCK)
	 { s++;
		for(j=0; j<LMAXRFTXT; j++) if((ch=srcPtr[s+j])==0 || ch==CHBLOCK) break;
		if(ch==0 || j==LMAXRFTXT) {result=FALSE; break;}
		memcpy(txtbuff, srcPtr+s, j);
		txtbuff[j]=0;
		memset(&r, 0, sizeof(RECT));
		sscanf(txtbuff, "%i,%i,%i,%i,", &r.right, &r.bottom, &b, &la);
		if(r.right==0 || r.bottom==0) {result=FALSE; break;}
		for(k=0, tx=txtbuff; k<4; k++) while(*tx!=0 && *tx++!=',');
		if((cnti2=new struct Cntptr)==NULL) {result=FALSE; break;}
		cnti=cnt; cnt=cnti2; cnt->next=cnti;
		if((cnti2->c=new Contenidor)==NULL) {result=FALSE; break;}
		cnti2->c->extra=i;
		cnti2->c->create(hWnd, &r, FALSE, FALSE, GRB);
		cnti2->c->getData(tx);
		cnti2->txt=dupStr(tx);
		cnti2->b=b;
		cnti2->c->setFlag(MARC, b);
		cnti2->la=la;
		cnti2->c->setFlag(LALIGN, la);
		s+=j;
	 }

	 if(ch==CHINC)
	 { if(inInc)
		{ inInc=FALSE;
		  if(!edit && !onlyShow && (af->puzMode==BPARAULES || af->puzMode==BPARAGRAFS))
		  { if((finc2=new struct FloatInc)==NULL) {result=FALSE; break;}
			 finc2->beg=iniInc;
			 finc2->end=i-1;
			 finc2->srcPos=srcPos;
			 if(finc==NULL) finc=finc2;
			 else finci->next=finc2;
			 finci=finc2;
		  }
		  continue;
		}
		if(af->puzMode!=FORATS) {iniInc=i; srcPos=s+1; inInc=TRUE; continue;}

		s++;
		if((inc2=new Incognita)==NULL) {result=FALSE; break;}

		iniInc=s;
		for(j=0; j<5; j++)
		{ while((ch=srcPtr[s])!=CHINC && ch!=0) s++;
		  if(ch==0) break;
		  srcPtr[(p[j]=s)]=0;
		  s++;
		}
		if(j!=5) {result=FALSE; delete inc2; break;}

		sscanf(srcPtr+iniInc, "%c,%i,%i,%i,%i,%i,%i,%i,%i,", &inc2->chFarciment, &inc2->tipus,
				 &inc2->farciment, &inc2->maxResp, &inc2->infoMode, &v,
				 &inc2->infoWidth, &inc2->infoHeight, &inc2->flagsInc);

		inc2->infoDelay=(v&0x00FF);
		inc2->infoMax=(v&0xFF00)>>8;

		if((inc2->ok=dupStr(srcPtr+p[0]+1))==NULL ||
			((p[2]-p[1]>1) && (inc2->opcions=dupStr(srcPtr+p[1]+1))==NULL) ||
			((p[3]-p[2]>1) && (inc2->iniText=dupStr(srcPtr+p[2]+1))==NULL) ||
			((p[4]-p[3]>1) && (inc2->infoTxt=dupStr(srcPtr+p[3]+1))==NULL))
		{result=FALSE; delete inc2; break;}

		for(j=0; j<5; j++) srcPtr[p[j]]=CHINC;

		inc2->p=i;
		inci=inc; inc=inc2; inc->next=inci;
		inInc=TRUE;
		ch=srcPtr[s];

		if(!edit && !onlyShow)
		{ if(inc2->opcions!=0)
		  { int lcur, n;
			 char *tx2;

			 if((tx2=dupStr(inc2->opcions))==NULL) {result=FALSE; break;}
			 for(n=2, tx=tx2; *tx!=0; tx++) if(*tx==0x0D) n++;
			 if(n<1) n=1;
			 inc2->cbheight=n*lineHeight+4*GetSystemMetrics(SM_CYDLGFRAME);

			 inc2->hCBList= CreateWindow("combobox",
					  NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST,
					  0, 0, 20, inc2->cbheight, hWnd, (HMENU)COL_USR, WinBase::hInst, NULL);

			 fp=(WNDPROC)SetWindowLong(inc2->hCBList, GWL_WNDPROC, (DWORD)lpfnCBSubClassProc);
			 if(lpfnOldCBClassProc==NULL) lpfnOldCBClassProc=fp;

			 SendMessage(inc2->hCBList, WM_SETFONT, (WPARAM)bc[getBase()].hf, 0L);
			 ShowWindow(inc2->hCBList, SW_SHOW);

			 inc2->cbwidth=20;
			 tx=strtok(tx2, "\r\n");
			 while(tx)
			 { lcur=getStrWidth(tx);
				if(lcur>inc2->cbwidth) inc2->cbwidth=lcur;
				SendMessage(inc2->hCBList, CB_ADDSTRING, 0, (LONG)(LPSTR)tx);
				tx=strtok(NULL, "\r\n");
			 }
			 inc2->cbwidth+=scrWidth+4*GetSystemMetrics(SM_CXDLGFRAME);
			 if(inc2->iniText)
				SendMessage(inc2->hCBList, CB_SELECTSTRING, -1, (LPARAM)(LPCSTR)inc2->iniText);
			 txNow[i]=CHINC;
			 txAttr[i++]=TX_INC;
			 delete tx2;
		  }
		  else
		  { if(inc2->iniText)
			 { tx=inc2->iniText;
				while(*tx!=0)
				{ txNow[i]=*tx++;
				  txAttr[i++]=(TX_INC | TX_USR);
				  if(i>=txMaxLen && growMem()==FALSE) {NOMEM; break;}
				}
			 }
			 else for(j=0; j<inc2->farciment; j++)
			 { txNow[i]=inc2->chFarciment;
				txAttr[i++]=(TX_INC | TX_FARCIMENT);
				if(i>=txMaxLen && growMem()==FALSE) {NOMEM; break;}
			 }
		  }
		  continue;
		}
	 }

	 if((ch<' ' && ch!=0x0D && ch!=TAB)) continue;

	 if(inInc)
	 { switch(af->puzMode)
		{ case FORATS:
				 if(!edit && !onlyShow) continue;
				 if(!onlyShow) txAttr[i]|=TX_INC;
				 break;

		  case COMPTEXT:
				 if(!edit && !onlyShow) continue;
				 if(!onlyShow) txAttr[i]|=TX_INC;
				 break;

		  case IDLLETRES:
		  case IDPARAULES:
				 if(!onlyShow) txAttr[i]|= (edit ? TX_INC : TX_HIDINC);
				 break;

		  case BPARAULES:
		  case BPARAGRAFS:
				 if(!onlyShow) txAttr[i]|=TX_INC;
				 break;

		  default:
				 break;
		}
	 }

	 txNow[i++]=ch;
  }
  txNow[i]=0;
  txLen=i;

  if(result)
  { if(!edit && !onlyShow && (af->puzMode!=FORATS || af->avCont==FALSE) && af->btCorregir==TRUE)
	 { pos.bottom-=btHeight;
		if(btCorregir) {DestroyWindow(btCorregir); btCorregir=NULL;}
		btCorregir=CreateWindow
					  ("BUTTON", af->txBtCorregir, /*WS_CLIPSIBLINGS|*/WS_CHILD|WS_VISIBLE,
						pos.left, pos.bottom, RECTW(pos), btHeight, hWnd,
						(HMENU)BOTO_CORREGIR, WinBase::hInst, NULL);
	 }

	 scrBar=CreateWindow
			  ("SCROLLBAR", NULL, WS_CHILD|WS_VISIBLE|SBS_VERT,
				pos.right-scrWidth, pos.top, scrWidth, RECTH(pos), hWnd,
				(HMENU)ID_TXSCROLL, WinBase::hInst, NULL);

	 result=setLines(0, FALSE, TRUE);
  }

  if(result)
  { caretPos=0;
	 if(!edit && !onlyShow) switch(af->puzMode)
	 {	case FORATS:
			  if(goToInc(0, TRUE)) {gainCaret(); nav=NAV_INC;}
			  else nav=NO_NAV;
			  break;

		case COMPTEXT:
			  nav=NAV_FREE;
			  SetFocus(hWnd);
			  gainCaret();
			  break;

		case IDLLETRES:
		case IDPARAULES:
			  nav=NO_NAV;
			  break;

		case BPARAGRAFS:
		case BPARAULES:
			  nav=NAV_FINC;
			  filOn=TRUE;
			  SetFocus(hWnd);
			  activateFinc(finc);
			  break;

		default:
			  break;
	 }
	 else
	 { nav=NAV_FREE;
		if(!onlyShow) gainCaret();
	 }
  }

  playing=(!edit && !onlyShow);
  waiting=onlyShow;

  unlockMem();

  return result;
}


char *TxContenidor::getCleanSource()
{
  char *txt=NULL;
  char *txi, *txo;
  char ch;
  int i;
  BOOL inInc=FALSE;

  if(af!=NULL && af->txBase!=NULL && (txt=dupStr(af->txBase))!=NULL)
  { txi=af->txBase;
	 txo=txt;
	 *txo=0;
	 while((ch=*txi++)!=0)
	 { switch(ch)
		{ case CHINC:
				 if(inInc)
					inInc=FALSE;
				 else
				 { inInc=TRUE;
					if(af->puzMode==FORATS)
					  for(i=0; i<5 && ch!=0; i++)
						 while((ch=*txi++)!=0 && ch!=CHINC);
             }
				 break;

		  case CHBLOCK:
				 while((ch=*txi++)!=0 && ch!=CHBLOCK);
				 break;

		  default:
			 *txo++=ch;
		}
		if(ch==0) break;
	 }
	 *txo=0;
  }

  return txt;
}

char *TxContenidor::getSource()
{
  char *txt=NULL;
  HGLOBAL hgBigBuff, hgNewBuff;
  char ch, *tx2, far *bigBuff;
  BOOL inInc;
  int lBigBuff;
  int s, d, v;

  if(!lockMem()) return txt;

  lBigBuff=2*txMaxLen;

  if((hgBigBuff=GlobalAlloc(GHND, lBigBuff))!=NULL &&
	  (bigBuff=(char far *)GlobalLock(hgBigBuff))!=NULL)
  { for(s=0, d=0, inInc=FALSE; (ch=txNow[s])!=0; s++)
	 { if(d>(lBigBuff-txMaxLen))
		{ GlobalUnlock(hgBigBuff);
		  hgNewBuff=GlobalReAlloc(hgBigBuff, lBigBuff+txMaxLen, GHND);
		  if(hgNewBuff==NULL) break;
		  bigBuff=(char far *)GlobalLock(hgBigBuff=hgNewBuff);
		  lBigBuff+=txMaxLen;
		}

		if((txAttr[s]&TX_INC)!=0)
		{ if(inInc==FALSE)
		  { if(af->puzMode!=FORATS)
			 { bigBuff[d++]=CHINC;
				inInc=TRUE;
			 }
			 else if((inci=getIncFromChar(s))!=NULL)
			 { bigBuff[d++]=CHINC;
				v=inci->infoDelay+(inci->infoMax<<8);
				sprintf(txtbuff, "%c,%i,%i,%i,%i,%i,%i,%i,%i,"CHINCS,
							 inci->chFarciment, inci->tipus, inci->farciment, inci->maxResp,
							 inci->infoMode, v, inci->infoWidth, inci->infoHeight, inci->flagsInc);
				_fstrcpy(bigBuff+d, txtbuff);
				d+=strlen(txtbuff);
				tx2=inci->ok;
				_fstrcpy(bigBuff+d, tx2); d+=strlen(tx2); bigBuff[d++]=CHINC;
				tx2=inci->opcions == NULL ? nullStr:inci->opcions;
				_fstrcpy(bigBuff+d, tx2); d+=strlen(tx2); bigBuff[d++]=CHINC;
				tx2=inci->iniText == NULL ? nullStr:inci->iniText;
				_fstrcpy(bigBuff+d, tx2); d+=strlen(tx2); bigBuff[d++]=CHINC;
				tx2=inci->infoTxt == NULL ? nullStr:inci->infoTxt;
				_fstrcpy(bigBuff+d, tx2); d+=strlen(tx2); bigBuff[d++]=CHINC;
				inInc=TRUE;
			 }
		  }
		}
		else if(inInc) {bigBuff[d++]=CHINC; inInc=FALSE;}

		switch(ch)
		{ case 0x0D:
				 bigBuff[d++]=ch;
				 bigBuff[d++]=0x0A;
				 break;

		  case CHBLOCK:
				 if((cnti=getContFromChar(s))!=NULL && cnti->c->getTxt()!=NULL)
				 { bigBuff[d++]=ch;
					sprintf(txtbuff,"%i,%i,%i,%i,%s",
								 cnti->c->getWidth(), cnti->c->getHeight(), cnti->b, cnti->la, cnti->txt);
					_fstrcpy(bigBuff+d, txtbuff);
					d+=strlen(txtbuff);
					bigBuff[d++]=ch;
				 }
				 break;

		  default:
				 bigBuff[d++]=ch;
				 break;
		}
	 }

	 // ------
	 if(inInc) {bigBuff[d++]=CHINC; inInc=FALSE;}
	 // ------
	 bigBuff[d]=0;

	 if(ch==0 && (txt=new char[_fstrlen(bigBuff)+1])!=NULL)
		_fstrcpy(txt, bigBuff);

	 GlobalUnlock(hgBigBuff);
  }
  if(txt==NULL) NOMEM;
  if(hgBigBuff) GlobalFree(hgBigBuff);

  unlockMem();

  return txt;
}


BOOL TxContenidor::processScroll(UINT wParam, LONG lParam)
{
  BOOL result=FALSE;
  int fl=firstLine;
  int d=1;

  if((HWND)HIWORD(lParam)==scrBar && getTotalHeight()>RECTH(pos))
	 switch(wParam)
	 { case SB_LINEUP:
			  d=-1;
		case SB_LINEDOWN:
			  fl+=d;
			  result=TRUE;
			  break;

		case SB_PAGEUP:
			  d=-1;
		case SB_PAGEDOWN:
			  fl=locateLineFromY(getLineY(fl)+(RECTH(pos)+lineHeight)*d);
			  result=TRUE;
			  break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			  fl=LOWORD(lParam);
			  result=TRUE;
			  break;

		default:
			  break;
	 }

  if(result) setScrollAt(fl);

  return result;
}

BOOL SlideWindow(HWND hWnd, HWND hOwn, int dx, int dy)
{
  POINT pt;
  RECT r;

  GetWindowRect(hWnd, &r);
  pt.x=r.left; pt.y=r.top;
  ScreenToClient(hOwn, &pt);
  MoveWindow(hWnd, pt.x+dx, pt.y+dy, RECTW(r), RECTH(r), TRUE);
  InvalidateRect(hWnd, NULL, FALSE);

  return TRUE;
}


BOOL TxContenidor::setScrollAt(int newFirstLine, BOOL sc)
{
  int i, fl0=firstLine;
  int dy;

  if(!lockMem()) return FALSE;

  if(newFirstLine<0) newFirstLine=0;
  if(newFirstLine>=nLines) newFirstLine=nLines-1;

  if(newFirstLine!=firstLine)
  { firstLine=newFirstLine;
	 SetScrollPos(scrBar, SB_CTL, firstLine, FALSE);
	 InvalidateRect(scrBar, NULL, FALSE);
	 invr(FALSE);
	 if(btCorregir) InvalidateRect(btCorregir, NULL, TRUE);
	 if(sc) setCaretAtPos(FALSE);
	 dy=0;
	 if(fl0<firstLine) for(i=fl0; i<firstLine; i++) dy-=lin[i].height;
	 else for(i=firstLine; i<fl0; i++) dy+=lin[i].height;

	 if(cnt) ITERACNT cnti->c->move(0, dy);

	 if(inc)
		ITERAINC if(inci->hCBList) SlideWindow(inci->hCBList, hWnd, 0, dy);

	 if(popup.isVisible()) SlideWindow(popup.hWnd, hWnd, 0, dy);
  }

  unlockMem();

  return TRUE;
}

BOOL TxContenidor::processMouse(UINT msg, WPARAM wParam, LPARAM lParam)
{
  POINT pt;
  int p, aZona;
  BOOL drag=FALSE;
  int cp0=caretPos;
  struct FloatInc *finc0;

  if(!playing && !edit) return FALSE;

  switch(msg)
  { case WM_ERASEBKGND:
			if(filOn) fil.hide();
			break;

	 case WM_MOUSEMOVE:
			if(filOn && fil.isAct())
			{ pt=MAKEPOINT(lParam);
			  fil.move(pt);
			}

			if(nav==NAV_FREE && (wParam & MK_LBUTTON)!=0)
			  drag=TRUE;
			else break;

	 case WM_LBUTTONDOWN:
			pt=MAKEPOINT(lParam);
			if(!isIn(pt)) break;

			if(GetFocus()!=hWnd) SetFocus(hWnd);

			aZona=FALSE;
			ITERACNT
			  if(cnti->c->isIn(pt)) {aZona=TRUE; break;}

			if(aZona && !drag)
			{ if(edit) editCnt(cnti);
			  else cnti->c->play();
			}

			if((p=getPosFromPoint(pt, nav!=NO_NAV && nav!=NAV_FINC))==-1 ||
				(caretVis && p==cp0)) break;

			if(edit)
			{ caretPos=p;
			  setCaretAtPos(TRUE);
			  adjustSel(cp0, drag);
			  break;
			}

			if(!playing) break;

			switch(nav)
			{ case NAV_INC:
					 if(incNow==NULL) break;
					 if(getIncFromChar(p)==incNow)
					 { if(incNow->hCBList)
						  SetFocus(incNow->hCBList);
						else
						{ caretPos=p; setCaretAtPos(TRUE);}
					 }
					 else
					 { if(!goToIncEv(p, TRUE)) goToInc(p, FALSE);
					 }
					 break;

			  case NAV_FREE:
					 caretPos=p;
					 setCaretAtPos(TRUE);
					 break;

			  case NO_NAV:
					 if(!aZona) marcaBloc(p, TX_USR);
					 break;

			  case NAV_FINC:
					 if((finc0=getFIncFromChar(p))==NULL) break;
					 if(anchorFinc)
					 { if(filOn) fil.end();
						markFinc(anchorFinc, 0, TX_ANCHORFINC|TX_ERR);
						if(anchorFinc!=finc0)
						{ swapFincs(anchorFinc, finc0);
						  fincNow=NULL;
						  activateFinc(finc0);
						}
						anchorFinc=NULL;
						invr();
					 }
					 else
					 { if(filOn) fil.init(pt, hWnd, PlayWindow::hFilCursor, 0);
						activateFinc(finc0);
						anchorFinc=finc0;
						markFinc(anchorFinc, TX_ANCHORFINC, TX_ERR);
					 }
					 break;

			  default: break;

			}
			break;

	 default: break;
  }

  return TRUE;
}


#pragma argsused
BOOL TxContenidor::editCnt(struct Cntptr *cntz)
{
  BOOL result=FALSE;
#ifndef NOEDIT
  DZones *dz;
  RECT r;

  if((dz=new DZones(hWnd, cntz->c->getWidth(), cntz->c->getHeight(),
						  cntz->b, cntz->la, cntz->txt))!=NULL)
  { if(dz->run()==TRUE)
	 { r=*cntz->c->getRect();
		r.right=r.left+dz->w;
		r.bottom=r.top+dz->h;
		cntz->b=dz->b;
		cntz->la=dz->la;
		if(cntz->txt) {delete cntz->txt; cntz->txt=dupStr(dz->txt);}
		cntz->c->getData(cntz->txt);
		cntz->c->setFlag(MARC, cntz->b);
		cntz->c->setFlag(LALIGN, cntz->la);
		cntz->c->move(&r);
		setLines(0, FALSE, TRUE);
		setCaretAtPos(TRUE);
		result=TRUE;
	 }
	 delete dz;
  }
  else NOMEM;
#endif
  return result;
}

BOOL TxContenidor::processKeyboard(UINT msg, WPARAM wParam)
{
  int p, l;
  POINT pt;
  BOOL extSel=FALSE;
  int cp0=caretPos;
  int d=1;
  char ch;
  BOOL vRet=FALSE;
  BOOL controlOn, shiftOn;

  if(!playing && !edit) return FALSE;

  if(nav==NO_NAV) return FALSE;

  if(!lockMem()) return FALSE;

  controlOn=((GetKeyState(VK_CONTROL) & 0x8000)!=0);
  shiftOn=((GetKeyState(VK_SHIFT) & 0x8000)!=0);
  if(filOn) fil.end();

  switch(msg)
  {
	 case WM_CHAR:
			switch(wParam)
			{ case VK_RETURN:
					 switch(nav)
					 { case NAV_INC:
							  goToIncEv(caretPos, TRUE);
							  break;

						case NAV_FINC:
							  if(fincNow==NULL) break;
							  if(anchorFinc)
							  { markFinc(anchorFinc, 0, TX_ANCHORFINC|TX_ERR);
								 if(anchorFinc!=fincNow)
								 { swapFincs(anchorFinc, fincNow);
									finci=fincNow; fincNow=NULL;
									activateFinc(finci);
								 }
								 anchorFinc=NULL;
								 invr();
							  }
							  else
							  { anchorFinc=fincNow;
								 markFinc(anchorFinc, TX_ANCHORFINC, TX_ERR);
							  }
							  break;

						case NAV_FREE:
							  insertChar(0x0D);
							  break;

						default: break;
					 }
					 vRet=TRUE;
					 break;

			  case VK_TAB:
					 switch(nav)
					 { case NAV_INC:
							  if(controlOn) break;
							  goToIncEv(caretPos, shiftOn ? FALSE : TRUE);
							  vRet=TRUE;
							  break;

						case NAV_FINC:
							  if(fincNow!=0)
							  { if(shiftOn)
								 { ITERAFI if(finci->next==fincNow) break;
									if(finci)
									{ activateFinc(finci);
									  vRet=TRUE;
									}
								 }
								 else if(fincNow->next!=0)
								 { activateFinc(fincNow->next);
									vRet=TRUE;
								 }
							  }
							  break;

						case NAV_FREE:
							  if(controlOn || shiftOn) break;
							  if(edit) insertChar(TAB);
							  vRet=TRUE;
							  break;

						default:
							  break;
					 }
					 break;

			  default:
					 if(nav!=NAV_FREE && nav!=NAV_INC) break;
					 ch=wParam & 0xFF;
					 if(controlOn && af!=NULL && (ch=='i' || ch=='I'))
					 { insertIncognita(TRUE); vRet=TRUE;
						break;
					 }
					 if(isForbiddenChar(ch)) break;
					 if(selActiva) cancelSel(TRUE);
					 insertChar(ch);
					 if(incFull && af!=NULL && af->avNoSalta==FALSE)
						goToIncEv(caretPos+1, TRUE);
					 vRet=TRUE;
					 break;
			}
			break;

	 case WM_KEYDOWN:
			p=-1;
			switch(wParam)
			{ case VK_RETURN:
					 if(controlOn && btCorregir)
					 { PostMessage(hWnd, WM_COMMAND, BOTO_CORREGIR, 0L);
						vRet=TRUE;
					 }
					 break;

			  case VK_ESCAPE:
					 if(popup.isVisible()==TRUE) popup.show(FALSE);
					 vRet=TRUE;
					 break;

			  case VK_RIGHT:
					 if(caretPos<txLen)
					 { p=caretPos+1;
						if(controlOn)
						  while(p<txLen && txNow[p]!=' ' && txNow[p]!=0x0D && txNow[p]!=TAB) p++;
					 }
					 extSel=TRUE;
					 break;

			  case VK_LEFT:
					 if(caretPos>0)
					 { p=caretPos-1;
						if(controlOn)
						  while(p>0 && txNow[p]!=' ' && txNow[p]!=0x0D && txNow[p]!=TAB) p--;
					 }
					 d=-1;
					 extSel=TRUE;
					 break;

			  case VK_UP:
					 d=-1;
			  case VK_DOWN:
					 l=locateLineFromChar(caretPos);
					 pt.y=getLineY(l+d);
					 pt.x=caretX;
					 p=getPosFromPoint(pt);
					 extSel=TRUE;
					 break;

			  case VK_PRIOR:
					 d=-1;
			  case VK_NEXT:
					 pt.x=caretX; pt.y=caretY+RECTH(pos)*d;
					 if((p=getPosFromPoint(pt))==-1) p=(d==-1 ? 0:txLen);
					 extSel=TRUE;
					 break;

			  case VK_INSERT:
					 if(edit)
					 { if(shiftOn) pasteClipboard();
						else if(controlOn && selActiva) copyClipboard();
					 }
					 break;

			  case VK_DELETE:
					 if(selActiva)
						cancelSel(TRUE);
					 else
						if(nav==NAV_FREE || nav==NAV_INC) deleteChar(FALSE);
					 break;

			  case VK_BACK:
					 if(selActiva)
						cancelSel(TRUE);
					 else
						if(nav==NAV_FREE || nav==NAV_INC) deleteChar(TRUE);
					 d=-1;
					 break;

			  case VK_HOME:
					 if(controlOn)
						p=0;
					 else
					 { l=locateLineFromChar(caretPos);
						p=lin[l].pChar;
					 }
					 extSel=TRUE;
					 d=-1;
					 break;

			  case VK_END:
					 if(controlOn)
						p=txLen;
					 else
					 { l=locateLineFromChar(caretPos);
						if(l==nLines-1) p=txLen;
						else p=lin[l].pLastChar;
					 }
					 extSel=TRUE;
					 break;

			  default:
					 if(edit && controlOn && af!=NULL)
					 switch(wParam)
					 { case 'I':
							  insertIncognita(TRUE);
							  vRet=TRUE;
							  break;

						case 'C':
							  //insertCnt();
							  if(selActiva) copyClipboard();
							  vRet=TRUE;
							  break;

						case 'V':
							  pasteClipboard();
							  vRet=TRUE;
							  break;

						case 'X':
							  if(selActiva) cutClipboard();
							  vRet=TRUE;
							  break;

						default: break;
					 }
					 break;
			}
			if(p>=0)
			{ if(nav==NAV_FINC)
				 goToFinc(p, d==1);
			  else if(nav==NAV_INC && getIncFromChar(p)!=incNow)
				 goToIncEv(p, d==1);
			  else
			  { caretPos=p;
				 setCaretAtPos(TRUE);
			  }
			}
			break;

	 default:
			break;
  }

  if(vRet==FALSE && extSel && edit) adjustSel(cp0, shiftOn);

  unlockMem();

  return vRet;
}


BOOL TxContenidor::adjustSel(int cp0, BOOL drag)
{
  int p;

  if(selActiva) invSel();
  if(drag)
  { if(selActiva)
		if(cp0>selStart) selEnd=caretPos; else selStart=caretPos;
	 else
	 { selActiva=TRUE; selStart=cp0; selEnd=caretPos;}
  }
  else
	 if(selActiva) cancelSel();

  if(selActiva)
  { if(selEnd<selStart)
	 {p=selEnd; selEnd=selStart; selStart=p;}
	 invSel();
  }

  return TRUE;
}


BOOL TxContenidor::pasteClipboard()
{
  BOOL result=FALSE;
  HANDLE hClip;
  LPSTR fclip;

  if(IsClipboardFormatAvailable(CF_TEXT))
  { if(OpenClipboard(hWnd))
	 { if((hClip=GetClipboardData(CF_TEXT))!=NULL)
		{ fclip=(LPSTR)GlobalLock(hClip);
		  // ATENCIO A CHBLOCK
		  while(*fclip) insertChar(*fclip++);
		  GlobalUnlock(hClip);
		  result=TRUE;
		}
	 }
	 CloseClipboard();
  }

  return result;

}


BOOL TxContenidor::cutClipboard()
{
  if(!copyClipboard()) return FALSE;
  return cancelSel(TRUE);
}


BOOL TxContenidor::copyClipboard()
{
  BOOL result=FALSE;
  HANDLE hClip;
  LPSTR fclip;

  if(!selActiva || selStart==selEnd) return FALSE;

  if(!lockMem()) return FALSE;
  if(OpenClipboard(hWnd))
  { EmptyClipboard();
	 if((hClip=GlobalAlloc(GHND, selEnd-selStart+1))!=NULL)
	 { if((fclip=(LPSTR)GlobalLock(hClip))!=NULL)
		{ _fmemcpy(fclip, txNow+selStart, selEnd-selStart);
		  result=TRUE;
		  GlobalUnlock(hClip);
		  if(SetClipboardData(CF_TEXT, hClip)) result=TRUE;
		}
	 }
	 CloseClipboard();
  }
  unlockMem();
  return result;
}


void TxContenidor::updateX(HDC hDC, RECT *updRect, HDC hMemDC, BOOL toPrint)
{
  int l, c, k, x, y;
  char ch, atr;
  BASE_ZONES b;
  HFONT hOldFont;
  COLORREF oldTextColor, oldBkColor;
  int oldBkMode;
  char str[2];
  str[1]=0;
  RECT r, rd;
  POINT pt;
  BOOL inSel, inUsr, inErr, inFinc, inAnch;

  if(filOn && !toPrint) fil.hide(hDC, hMemDC);

  Zona::update(hDC, updRect);

  if(!lockMem()) return;

  ITERACNT if(cnti->c->isIn(updRect)) cnti->c->update(hDC, updRect, hMemDC);

  b=getBase();
  oldBkMode=SetBkMode(hDC, TRANSPARENT);

  if(bc[b].hf)
	 hOldFont=(HFONT)SelectObject(hDC, bc[b].hf);
  else
	 hOldFont=0;

  oldTextColor=GetTextColor(hDC);
  oldBkColor=GetBkColor(hDC);

  for(k=0, l=firstLine; l<nLines; k+=lin[l++].height)
  { if(k>RECTH(pos) && !toPrint) break;
	 if(k>updRect->bottom) break;
	 getLineRect(l, &r);
	 if(!IntersectRect(&rd, &r, updRect)) continue;
	 x=leftMargin;
	 y=pos.top+k+(lin[l].height-lineHeight);
	 for(c=lin[l].pChar; c<=lin[l].pLastChar; c++)
	 { inSel=(selActiva && c>=selStart && c<selEnd);
		inUsr=((atr=txAttr[c])&(TX_INC|TX_USR))!=0;
		inErr=(atr&TX_ERR)!=0;
		inFinc=(atr&TX_FINCNOW)!=0;
		inAnch=(atr&TX_ANCHORFINC)!=0;
		ch=txNow[c];
		if(ch==0) break;
		if(edit && ch==0x0D) ch=0xB6;
		if(ch<' ' && ch!=TAB) continue;
		if(ch==CHBLOCK)
		{ if((cnti=getContFromChar(c))!=NULL) x+=cnti->c->getWidth();
		  continue;
		}
		if(ch==CHINC && (inci=getIncFromChar(c))!=NULL && inci->hCBList!=NULL)
		{ GetWindowRect(inci->hCBList, &r);
		  pt.x=r.left; pt.y=r.top;
		  ScreenToClient(hWnd, &pt);
		  if(pt.x!=x || pt.y!=y-4)
			 MoveWindow(inci->hCBList, x, y-4, inci->cbwidth, inci->cbheight, TRUE);

		  if(toPrint)
		  { HBITMAP hbm;

			 Rectangle(hDC, x+pos.left, y-2, x+pos.left+inci->cbwidth, y+lineHeight+2);
			 *txtbuff=0;
			 GetWindowText(inci->hCBList, txtbuff, LMAXRFTXT);
			 SetBkColor(hDC, af->colorUsuari[1]);
			 SetTextColor(hDC, af->colorUsuari[0]);
			 SetBkMode(hDC, OPAQUE);
			 TextOut(hDC, x+pos.left+2, y, txtbuff, strlen(txtbuff));
			 SetBkColor(hDC, oldBkColor);
			 SetBkMode(hDC, TRANSPARENT);
			 if((hbm=LoadBitmap(NULL, MAKEINTRESOURCE(OBM_DNARROW)))!=NULL)
			 { displayBitmap(hDC, hMemDC, hbm, x+pos.left+inci->cbwidth-2-GetSystemMetrics(SM_CXVSCROLL), y);
				DelObj(hbm);
			 }
		  }

		  x+=inci->cbwidth;

		  continue;
		}

		if(ch==' ' && ((inErr && bc[b].c.backColor==af->colorError[1])
							//|| (inUsr && bc[b].c.backColor==af->colorUsuari[1])
							)) *str='·';
		else
		  *str=ch;

		if(!inSel && !inUsr && !inErr && !inFinc && !inAnch && bc[b].c.shadow && ch!=TAB)
		{ SetTextColor(hDC, bc[b].c.shadowColor);
		  TextOut(hDC, x+pos.left+bc[b].shadowDeep, y+bc[b].shadowDeep, str, 1);
		}

		if(inSel)
		{ SetBkColor(hDC, bc[b].c.textColor);
		  SetTextColor(hDC, bc[b].c.backColor);
		  SetBkMode(hDC, OPAQUE);
		}
		else if(inErr)
		{ SetBkColor(hDC, af->colorError[1]);
		  SetTextColor(hDC, af->colorError[0]);
		  SetBkMode(hDC, OPAQUE);
		}
		else if(inAnch)
		{ SetBkColor(hDC, bc[b].c.textColor);
		  SetTextColor(hDC, bc[b].c.backColor);
		  SetBkMode(hDC, OPAQUE);
		}
		else if(inFinc)
		{ SetBkColor(hDC, af->colorUsuari[0]);
		  SetTextColor(hDC, af->colorUsuari[1]);
		  SetBkMode(hDC, OPAQUE);
		}
		else if(inUsr)
		{ SetBkColor(hDC, af->colorUsuari[1]);
		  SetTextColor(hDC, af->colorUsuari[0]);
		  SetBkMode(hDC, OPAQUE);
		}
		else SetTextColor(hDC, bc[b].c.textColor);

		if(ch==TAB) x=tabLen*(((x-leftMargin)/tabLen)+1)+leftMargin;
		else
		{ TextOut(hDC, x+pos.left, y, str, 1);
		  x+=widthTable[ch];
		}

		if(inSel || inUsr || inErr || inFinc || inAnch)
		{ SetBkColor(hDC, oldBkColor);
		  SetBkMode(hDC, TRANSPARENT);
		}
	 }
  }

  if(hOldFont) SelectObject(hDC, hOldFont);

  if(toPrint && btCorregir)
  { RECT r;
	 HFONT hf;
	 HBRUSH hBrOld;

	 SetBkMode(hDC, TRANSPARENT);
	 SetTextColor(hDC, RGB(0, 0, 0));
	 hf=CreateFontIndirect(&logFont1);
	 SelectObject(hDC, hf);
	 hBrOld=(HBRUSH)SelectObject(hDC, GetStockObject(LTGRAY_BRUSH));
	 r.left=pos.left;
	 r.top=pos.bottom;
	 r.right=pos.right;
	 r.bottom=pos.bottom+btHeight;
	 Rectangle(hDC, r.left, r.top, r.right, r.bottom);
	 DrawText(hDC, af->txBtCorregir, strlen(af->txBtCorregir), &r, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_NOCLIP);
	 if(hOldFont) SelectObject(hDC, hOldFont);
	 DelObj(hf);
	 SelectObject(hDC, hBrOld);
  }

  SetBkMode(hDC, oldBkMode);
  SetBkColor(hDC, oldBkColor);
  SetTextColor(hDC, oldTextColor);

  if(filOn && !toPrint) fil.show(hDC, hMemDC);

  unlockMem();
}

int TxContenidor::locateLineFromChar(int fromChar)
{
  int l;

  if(!lockMem()) return 0;

  if(nLines<1) nLines=1;
  for(l=0; l<nLines; l++)
	 if(lin[l].pChar>fromChar) break;
  if (l>0) l--;

  unlockMem();

  return l;
}

int TxContenidor::locateLineFromY(int y)
{
  int l, y0;

  if(!lockMem()) return 0;

  y0=pos.top;
  if(y<y0)
  { for(l=firstLine-1; l>=0; l--)
		if((y0-=lin[l].height)<=y) break;
  }
  else
  { for(l=firstLine; l<nLines; l++)
		if((y0+=lin[l].height)>y) break;
  }

  unlockMem();

  return (l>=nLines || l<0) ? -1 : l;
}

int  TxContenidor::getLineY(int l)
{
  int i, y;

  if(!lockMem()) return 0;

  if(l<0) l=0;
  y=pos.top;
  if(l<firstLine)
	 for(i=firstLine-1; i>=l; i--) y-=lin[i].height;
  else
	 for(i=firstLine; i<l; i++) y+=lin[i].height;

  unlockMem();

  return y;

}

BOOL TxContenidor::setLines(int fromLine, BOOL inv, BOOL absolute)
{
  int i, x, y, h, lh0, l, c, w, wmax, lastSpace, nLines0;
  BOOL lastLine, rarilla;
  char ch;
  RECT r;

  if(!lockMem()) return FALSE;
  if(absolute)
  { _fmemset(lin, 0, nMaxLines*sizeof(struct txLine));
	 nLines=0;
	 fromLine=0;
	 inv=FALSE;
	 invr(TRUE);
	 if(btCorregir) InvalidateRect(btCorregir, NULL, TRUE);
  }
  nLines0=nLines;
  l=fromLine;
  lastLine=FALSE;
  while(TRUE)
  { if(l>=nMaxLines && growLin()==FALSE) break;
	 rarilla=FALSE;
	 if(l==nLines) nLines++;
	 c=lin[l].pChar;
	 lh0=lin[l].height;
	 lin[l].height=lineHeight;
	 w=0;
	 wmax=rightMargin-leftMargin;
	 lastSpace=-1;
	 while(c<txMaxLen)
	 { ch=txNow[c];
		if(ch==0) {lastLine=TRUE; break;}
		if(ch==0x0D) {if(edit) w+=widthTable[0xB6]; else {c++; break;}}
		else if(ch==CHBLOCK)
		{ if((cnti=getContFromChar(c))!=NULL)
		  { x=leftMargin+w;
			 w+=cnti->c->getWidth();
			 if(w<=wmax || c==lin[l].pChar)
			 { h=cnti->c->getHeight();
				if(lin[l].height<h) lin[l].height=h;
				y=getLineY(l)+lin[l].height-h;
				if(cnti->c->getRect()->top!=y || cnti->c->getRect()->left!=x)
				{ POINT pt;
				  pt.x=x; pt.y=y;
				  cnti->c->move(pt);
				}
			 }
		  }
		  rarilla=TRUE;
		}
		else if(ch==CHINC)
		{ if((inci=getIncFromChar(c))!=NULL && inci->hCBList!=NULL)
		  { x=w+leftMargin;
			 w+=inci->cbwidth;
			 if(w<=wmax || c==lin[l].pChar)
			 { RECT r;
				POINT pt;

				if(lin[l].height<lineHeight+8) lin[l].height=lineHeight+8;
				y=getLineY(l)+lin[l].height-lineHeight-4;

				GetWindowRect(inci->hCBList, &r);
				pt.x=r.left; pt.y=r.top;
				ScreenToClient(hWnd, &pt);
				if(pt.x!=x || pt.y!=y)
				  MoveWindow(inci->hCBList, x, y, inci->cbwidth, inci->cbheight, TRUE);
			 }
		  }
		  rarilla=TRUE;
		}
		else
		{ if(ch==TAB) w=tabLen*((w/tabLen)+1);
		  else w+=widthTable[ch];
		}

		if(w>wmax && c>lin[l].pChar)
		{ if(lastSpace!=-1) c=lastSpace+1;
		  break;
		}

		if(isSpace(ch)) lastSpace=c;

		c++;
		if(edit && ch==0x0D) break;
	 }

	 lin[l].pLastChar=c-1;

	 if(inv)
	 { if(rarilla && lh0!=lin[l].height)
		  invr();
		else
		{ getLineRect(l, &r);
		  InvalidateRect(hWnd, &r, FALSE);
		}
	 }

	 l++;

	 if(lastLine)
	 { nLines=l;

		if(nLines!=nLines0)
		{ for(; lin[l].pChar!=0 && l<nMaxLines; l++)
		  { lin[l].pChar=lin[l].pLastChar=0;
			 getLineRect(l, &r);
			 InvalidateRect(hWnd, &r, FALSE);
		  }

		  if(getTotalHeight()>RECTH(pos))
		  { for(y=0, i=nLines-1; i>=0; i--)
			 { y+=lin[i].height;
				if(y>=RECTH(pos)) break;
			 }
			 SetScrollRange(scrBar, SB_CTL, 0, i+1, FALSE);
			 SetScrollPos(scrBar, SB_CTL, firstLine, FALSE);
			 InvalidateRect(scrBar, NULL, FALSE);
			 ShowWindow(scrBar, SW_SHOWNORMAL);
		  }
		  else
			 ShowWindow(scrBar, SW_HIDE);
		}
		break;
	 }

	 if(!rarilla && lin[l].pChar==c) break; // a partir d'aqui ja esta be
	 lin[l].pChar=c;
  }

  unlockMem();
  return TRUE;
}


int TxContenidor::getTotalHeight()
{
  int y, l;

  if(!lockMem()) return FALSE;

  for(y=0, l=0; l<nLines; l++) y+=lin[l].height;

  unlockMem();

  return y;
}


BOOL TxContenidor::gainCaret()
{

  if(getFlag(OWNSCARET) || caretVis)
	 return FALSE;

  setFlag(OWNSCARET, TRUE);
  caretVis=TRUE;

  CreateCaret(hWnd, NULL, caretW, caretH);
  setCaretAtPos(TRUE);
  ShowCaret(hWnd);
  caretOwner=this;

  return TRUE;
}


BOOL TxContenidor::setCaretAtPos(BOOL makeVisible)
{
  int i, l, fl;

  if(!lockMem()) return FALSE;

  if(caretPos>=0 && caretPos<=txLen)
  {
	 l=locateLineFromChar(caretPos);
	 if(scrBar!=NULL && makeVisible)
	 { fl=firstLine;
		if((getLineY(l)+lin[l].height)>=RECTH(pos))
		{ for(i=RECTH(pos), fl=l; fl>=0; fl--)
			 if((i-=lin[fl].height)<=0) {fl++; break;}
		  if(fl<0) fl=0;
		}
		else if(l<firstLine) fl=l;

		if(fl!=firstLine) setScrollAt(fl, FALSE);
	 }

	 getChPos(caretPos, l, caretX, caretY);
	 SetCaretPos(caretX, caretY);
  }

  unlockMem();
  return TRUE;
}


BOOL TxContenidor::getChPos(int c, int &l, int &x, int &y)
{
  int i;
  char ch;

  if(!lockMem()) return FALSE;

  l=locateLineFromChar(caretPos);
  y=getLineY(l)+lin[l].height-lineHeight;
  x=pos.left+leftMargin;
  for(i=lin[l].pChar; i<c; i++)
  { switch(ch=txNow[i])
	 { case CHBLOCK:
		  if((cnti=getContFromChar(i))!=NULL) x+=cnti->c->getWidth();
		  break;

		case CHINC:
		  if((inci=getIncFromChar(i))!=NULL && inci->hCBList!=NULL) x+=inci->cbwidth;
		  break;

		default:
		  if(ch==TAB) x=tabLen*(((x-leftMargin)/tabLen)+1)+leftMargin;
		  else x+=widthTable[ch];
		  break;
	 }

  }
  unlockMem();
  return TRUE;
}


int TxContenidor::getPosFromPoint(POINT pt, BOOL halfChars)
{
  int l, p, x0, w, c;

  if((l=locateLineFromY(pt.y))==-1) return -1;

  if(!lockMem()) return -1;

  p=lin[l].pChar;
  for(x0=pos.left+leftMargin; p<=lin[l].pLastChar;)
  { // aixo combinat amb el <= es per a la ultima linia
	 if(p==lin[l].pLastChar && l!=nLines-1) break;

	 switch(c=txNow[p])
	 { case CHBLOCK:
			  if((cnti=getContFromChar(p))!=NULL) w=cnti->c->getWidth();
			  break;

		case CHINC:
			  if((inci=getIncFromChar(p))!=NULL && inci->hCBList!=NULL) w=inci->cbwidth;
			  break;

		default:
			  if(c==TAB) w=tabLen*(((x0-leftMargin)/tabLen)+1)+leftMargin-x0;
			  else w=widthTable[c];
			  break;
	 }

	 x0+=w;
	 if(x0>=pt.x)
	 { if(halfChars && pt.x>(x0-w/2)) p++;
		break;
	 }
	 p++;
  }

  unlockMem();

  return p;
}


BOOL TxContenidor::insertChar(char ch)
{
  static char tx[]=" ";

  *tx=ch;
  return insertString(tx);
}

BOOL TxContenidor::insertString(LPSTR str)
{
  int l, lt, le, cp0, i, j, atr, lenstr;
  char far *srcPtr;
  char ch;

  if(str==NULL || *str==0) return FALSE;
  lenstr=0;
  incFull=FALSE;
  srcPtr=str;
  while((ch=*srcPtr++)!=0) if(ch==0x0D || ch>=' ' || ch==TAB) lenstr++;
  if(lenstr==0) return FALSE;
  if(txLen+lenstr>=txMaxLen && !growMem(lenstr+1)) return FALSE;

  if(nav==NAV_INC && getCurIncLen(incNow, lt, le)!=NULL &&
	  (le+lenstr)>incNow->maxResp) return FALSE;

  if(!lockMem()) return FALSE;

  l=locateLineFromChar(caretPos);
  atr=txAttr[caretPos];
  _fmemmove(txNow+caretPos+lenstr, txNow+caretPos, txLen-caretPos+1);
  _fmemmove(txAttr+caretPos+lenstr, txAttr+caretPos, txLen-caretPos+1);
  for(srcPtr=str, i=0; i<lenstr; srcPtr++)
  { if((ch=*srcPtr)==0) break;
	 if(ch==0x0D || ch==TAB || ch>=' ')
	 { txNow[caretPos+i]=ch;
		txAttr[caretPos+i]=atr;
		if(!edit) txAttr[caretPos+i]|=TX_USR;
		if(nav==NAV_INC) txAttr[caretPos+i]=(TX_INC|TX_USR);
		i++;
	 }
  }

  for(i=l+1; i<nMaxLines && lin[i].pChar>0; i++)
  { lin[i].pChar+=lenstr; lin[i].pLastChar+=lenstr;}

  ITERACNT if(cnti->c->extra>=caretPos) cnti->c->extra+=lenstr;
  ITERAINC if(inci->p>caretPos) inci->p+=lenstr;
  ITERAFI  { if(finci->beg>caretPos) finci->beg+=lenstr;
				 if(finci->end>=caretPos) finci->end+=lenstr;
			  }

  caretPos+=lenstr;
  txLen+=lenstr;

  if(nav==NAV_INC && getCurIncLen(incNow, lt, le)!=NULL)
  { if(lt!=le)
	 { for(i=incNow->p, j=0; j<lt; j++, i++)
		  if(txAttr[i]&TX_FARCIMENT) break;
		if(j<lt)
		{ cp0=i<caretPos ? caretPos-1:caretPos;
		  caretPos=i;
		  for(j=0; j<lenstr; j++)
		  { txAttr[caretPos]|=TX_USR;
			 deleteChar(FALSE);
		  }
		  caretPos=cp0;
		}
	 }
	 if(le==incNow->maxResp) incFull=TRUE;
  }

  if((isSpace(*str) || lenstr>1) && l>0) setLines(l-1, TRUE, /*FALSE*/ TRUE);
  setLines(l, TRUE, FALSE);
  setCaretAtPos(TRUE);

  unlockMem();

  return TRUE;
}

BOOL TxContenidor::deleteChar(BOOL prev)
{
  int p, l, i, ch;
  Incognita *inc2;
  BOOL doDelete;

  if(txLen<1) return FALSE;
  p=caretPos;
  if(prev) p--;
  if(p<0 || p>=txLen) return FALSE;

  if(!lockMem()) return FALSE;

  doDelete=TRUE;
  if(!edit && (txAttr[p]&TX_USR)==0) {doDelete=FALSE;}
  else
  { ch=txNow[p];
	 if(ch==CHBLOCK) {deleteCnt(p); invr();}
	 if((txAttr[p]&TX_INC)==TRUE && (inc2=getIncFromChar(p))!=NULL && inc2->p==p)
	 { if((txAttr[p+1]&TX_INC)!=0 && txNow[p+1]!=0)
		  inc2->p++;
		else
		{ if(edit)
		  { if(inc2==inc)
				inc=inc2->next;
			 else
				ITERAINC if(inci->next==inc2) {inci->next=inc2->next; break;}

			 delete inc2;
		  }
		  else
		  { ch=inc2->chFarciment;
			 txNow[p]=ch;
			 txAttr[p]|=TX_FARCIMENT;
			 doDelete=FALSE;
		  }
		}
	 }

	 l=locateLineFromChar(p);

	 if(doDelete)
	 { _fmemmove(txNow+p, txNow+p+1, txLen-p+1);
		_fmemmove(txAttr+p, txAttr+p+1, txLen-p+1);
		txAttr[txLen]=0;
		txNow[txLen]=0;
		txLen--;

		for(i=l+1; i<nMaxLines && lin[i].pChar>0; i++)
		{ lin[i].pChar--; lin[i].pLastChar--;}

		ITERACNT if(cnti->c->extra>=p) cnti->c->extra--;
		ITERAINC if(inci->p>=p) inci->p--;
		ITERAFI {if(finci->beg>p) finci->beg--;
					if(finci->end>=p) finci->end--;
				  }
	 }

	 if(prev) caretPos--;

	 if((isSpace(ch) || ch==CHBLOCK) && l>0) setLines(l-1, TRUE, /*FALSE*/ (ch==CHBLOCK));
	 setLines(l, TRUE, /*FALSE*/ (ch==CHBLOCK));
	 setCaretAtPos(TRUE);

  }
  unlockMem();

  return TRUE;
}


void TxContenidor::getLineRect(int l, RECT *r)
{
  if(!lockMem()) return;

  r->left=pos.left+leftMargin; r->top=getLineY(l);
  r->right=pos.left+rightMargin; r->bottom=r->top+lin[l].height;

  unlockMem();
}

BOOL TxContenidor::cancelSel(BOOL erasing)
{
  int i, n;

  if(!selActiva) return FALSE;

  if(erasing)
  { caretPos=selStart;
	 n=selEnd-selStart;
	 for(i=0; i<n; i++) deleteChar(FALSE);
	 selActiva=FALSE;
	 return TRUE;
  }

  invSel();
  selActiva=FALSE;
  return TRUE;
}

BOOL TxContenidor::invSel()
{
  int n, l, i;
  RECT r;

  if(!selActiva) return FALSE;

  n=-1;
  for(i=selStart; i<=selEnd; i++)
  { l=locateLineFromChar(i);
	 if(n!=l)
	 { n=l;
		getLineRect(l, &r);
		InvalidateRect(hWnd, &r, FALSE);
	 }
  }
  return TRUE;
}

struct Cntptr *TxContenidor::getContFromChar(int c)
{
  ITERACNT if(cnti->c->extra==c) break;
  return cnti;
}

BOOL TxContenidor::deleteCnt(int n)
{
  struct Cntptr *cnti2=NULL;

  ITERACNT
  { if(cnti->c->extra==n) break;
	 cnti2=cnti;
  }
  if(cnti==NULL) return FALSE;

  if(cnti2) cnti2->next=cnti->next;
  else cnt=cnti->next;

  delete cnti;

  return TRUE;
}

BOOL TxContenidor::insertCnt()
{
  BOOL result=TRUE;
  struct Cntptr *cnti2;
  RECT r;
  char *tx="";

  if(!lockMem()) return FALSE;
  if((txAttr[caretPos]&TX_INC)!=0) result=FALSE;
  unlockMem();
  if(!result)
  { BMsgBox(hWnd, IDS_NOCASHERE, 0);
	 return FALSE;
  }
  result=FALSE;

  if((cnti2=new struct Cntptr)==NULL) {NOMEM; return FALSE;}
  if((cnti2->c=new Contenidor)==NULL)
  {delete cnti2; NOMEM; return FALSE;}

  r.left=0; r.top=0; r.right=af->txtCW2; r.bottom=af->txtCH2;
  cnti2->c->create(hWnd, &r, FALSE, FALSE, GRB);
  cnti2->c->getData(tx);
  cnti2->txt=dupStr(tx);
  cnti2->b=TRUE;
  cnti2->la=FALSE;
  cnti2->c->setFlag(MARC, FALSE);
  cnti2->c->setFlag(LALIGN, cnti2->la);
  if(editCnt(cnti2))
  { cnti2->c->extra=caretPos;
	 insertChar(CHBLOCK);
	 cnti=cnt; cnt=cnti2; cnt->next=cnti;
	 setLines(0, TRUE, TRUE);
	 result=TRUE;
  }
  if(!result) delete cnti2;

  return result;
}


Incognita *TxContenidor::getIncFromChar(int c)
{
  inci=NULL;
  if(!lockMem()) return inci;

  if((txAttr[c]&TX_INC)!=0 || (c>0 && (txAttr[--c]&TX_INC)!=0))
  { while(c>=0 && (txAttr[c]&TX_INC)!=0) c--;
	 c++;
	 ITERAINC if(inci->p==c) break;
  }

  unlockMem();

  return inci;
}


BOOL TxContenidor::insertIncognita(BOOL ed)
{
  Incognita *inc2;
  int i, j, l;
  char ch;
  BOOL result=FALSE;

  if((inc2=getIncFromChar(caretPos))!=NULL)
  { if(selActiva==FALSE || selEnd!=inc2->p)
	 { selActiva=FALSE;
		return ed ? editIncognita(inc2):FALSE;
	 }
  }

  if(selActiva==FALSE)
  { switch(af->puzMode)
	 { case FORATS:
		case COMPTEXT:
			  BMsgBox(hWnd, IDS_NOSEL, 0);
			  return FALSE;

		default:
			  selStart=caretPos; selEnd=selStart+1;
	 }
  }

  if(!lockMem()) return FALSE;

  switch(af->puzMode)
  { case FORATS:
			if((inc2=new Incognita)==NULL) {NOMEM; break;}

			l=selEnd-selStart;
			for(i=selStart; i<selEnd; i++)
			{ if((ch=txNow[i])==CHBLOCK)
			  { if(ed) BMsgBox(hWnd, IDS_ZONAIN, 0);
				 break;
			  }
			  if(ch==TAB)
			  { if(ed) BMsgBox(hWnd, IDS_TABIN, 0);
				 break;
			  }
			  if(txAttr[i]&TX_INC)
			  { if(ed) BMsgBox(hWnd, IDS_INCIN, 0);
				 break;
			  }
			  if(ch==0x0D) l++;
			}
			if(i==selEnd)
			{ inc2->ok=new char[l+1];
			  for(i=0, j=selStart; i<l && (txAttr[j]&TX_INC)==0; i++, j++)
			  { if((ch=txNow[j])==0x0D) {inc2->ok[i++]=ch; ch=0x0A;}
				 inc2->ok[i]=ch;
			  }
			  inc2->ok[i]=0;
			  inc2->farciment=inc2->maxResp=j-selStart;
			  inc2->infoWidth=af->txtCW2;
			  inc2->infoHeight=af->txtCH2;
			  if(ed==FALSE || editIncognita(inc2)==TRUE)
			  { inc2->p=selStart;
				 inci=inc; inc=inc2; inc->next=inci;
				 for(i=selStart; i<j; i++) txAttr[i]|=TX_INC;
				 result=TRUE;
			  }
			}
			if(!result) delete inc2;
			break;

	 case COMPTEXT:
	 case IDLLETRES:
			for(i=selStart; i<selEnd; i++) txAttr[i]^=TX_INC;
			result=TRUE;
			break;

	 case BPARAULES:
	 case IDPARAULES:
			for(i=selStart; i<selEnd; i++)
			{ if(isSep(txNow[i])) continue;
			  marcaBloc(i, TX_INC);
			  i++;
			  while(i<selEnd && !isSep(txNow[i])) i++;
			}
			result=TRUE;
			break;

	 case BPARAGRAFS:
			for(i=selStart; i<selEnd; i++)
			{ if(txNow[i]==0x0D) continue;
			  marcaBloc(i, TX_INC);
			  i++;
			  while(i<selEnd && txNow[i]!=0x0D) i++;
			}
			result=TRUE;
			break;

	 default: break;
  }

  selActiva=FALSE;
  invr();

  unlockMem();

  return result;
}

BOOL TxContenidor::mKAutoIncs(BOOL expr, BOOL onlyWords, char *txInc)
{
  int i, j, l, ni, caretPos0;
  char far *txp;

  if(txInc==NULL || *txInc==0) return FALSE;

  if(!lockMem()) return FALSE;

  caretPos0=caretPos;
  l=strlen(txInc);
  for(ni=0, i=0; txNow[i]!=0; i++)
  { selActiva=FALSE;
	 if((txp=_fstrstr(txNow+i, txInc))==NULL) break;
	 i=(int)(txp-txNow);
	 if((txAttr[i]&TX_INC)==0)
	 { if(!expr)
		{ if(!onlyWords ||
			  ((i==0 || isSep(txNow[i-1])) && ((i+l)==txLen || isSep(txNow[i+l]))))
		  { selStart=i; selEnd=i+l;}
		  else
		  { i+=l; continue;}
		}
		else
		{ for(j=i; j>=0; j--) if(isSep(txNow[j])) break;
		  selStart=j+1;
		  for(j=i+l-1; txNow[j]!=0; j++) if(isSep(txNow[j])) break;
		  selEnd=j;
		}
		if(selEnd>=selStart)
		{ for(j=selStart; j<selEnd; j++) if((txAttr[j]&TX_INC)!=0) break;
		  if(j==selEnd)
		  { selActiva=TRUE;
			 caretPos=selStart;
			 if(insertIncognita(FALSE)) ni++;
		  }
		  i=selEnd;
		}
	 }
  }
  selActiva=FALSE;
  caretPos=caretPos0;

  unlockMem();

  switch(ni)
  { case 0:
			strcpy(txtbuff, getStringTbl(IDS_NOAUTOINC));
			break;

	 case 1:
			strcpy(txtbuff, getStringTbl(IDS_ONEAUTOINC));
			break;

	 default:
			wsprintf(txtbuff, getStringTbl(IDS_AUTOINCOK), ni);
			break;
  }
  BMsgBox(hWnd, 0, 0, txtbuff, 0, MB_OK);

  return TRUE;

}


#pragma argsused
BOOL TxContenidor::editIncognita(Incognita *incp)
{
  BOOL result=FALSE;
#ifndef NOEDIT
  DIncognita *di;

  if((di=new DIncognita(hWnd, incp))!=NULL)
  { result=di->run();
	 delete di;
  }
  else NOMEM;
#endif
  return result;
}

BOOL TxContenidor::newTabSpc(int nt)
{
  tabLen=nt*widthTable[' '];
  setLines(0, TRUE, TRUE);
  setCaretAtPos(TRUE);
  
  return TRUE;
}

BOOL TxContenidor::calcFont()
{
  HDC hDC;
  HFONT hOldFont;
  TEXTMETRIC tm;
  int i;

  if(!lockMem()) return FALSE;

  hDC=GetDC(hWnd);
	 if(bc[getBase()].hf)
		hOldFont=(HFONT)SelectObject(hDC, bc[getBase()].hf);
	 else
		hOldFont=0;

	 GetCharWidth(hDC, 0, 255, widthTable);
	 widthTable[0x0D]=widthTable[0x0A]=widthTable[TAB]=0;
	 GetTextMetrics(hDC, &tm);
	 lineHeight=tm.tmHeight+tm.tmExternalLeading;
	 caretH=lineHeight;
	 caretW=2;
	 CreateCaret(hWnd, NULL, caretW, caretH);

	 if(hOldFont) SelectObject(hDC, hOldFont);
  ReleaseDC(hWnd, hDC);

  leftMargin=widthTable[' '];
  rightMargin=RECTW(pos)-leftMargin-scrWidth-widthTable[' '];

  for(i=0; i<nMaxLines; i++) {lin[i].height=lineHeight;}

  tabLen=af->tabSpc*widthTable[' '];

  unlockMem();

  return TRUE;
}

BOOL TxContenidor::goToInc(int from, BOOL fwd)
{
  BOOL result=FALSE;
  Incognita *incNow0;

  if(!lockMem()) return FALSE;

  incNow0=incNow;

  if(incNow)
	 while(from>=0 && getIncFromChar(from)==incNow) from+=(fwd ? 1:-1);

  incNow=NULL;
  while(from>=0 && txNow[from]!=0 && (txAttr[from]&TX_INC)==0) from+=(fwd ? 1:-1);

  if(from>=0 && txNow[from]!=0)
	 incNow=getIncFromChar(from);

  if(incNow) result=TRUE; else incNow=incNow0;

  if(incNow) activateInc(incNow);

  unlockMem();
  return result;
}

BOOL TxContenidor::activateInc(Incognita *newIncNow, BOOL changeFocus)
{
  if(newIncNow==NULL) return FALSE;

  incNow=newIncNow;
  caretPos=incNow->p;
  setCaretAtPos(TRUE);
  if(changeFocus)
  { if(incNow->hCBList && GetFocus()!=incNow->hCBList)
	 { SetFocus(incNow->hCBList);
	 //
	 // PostMessage(incNow->hCBList, CB_SHOWDROPDOWN, TRUE, 0L);
	 //
	 }
	 else if(GetFocus()!=hWnd) SetFocus(hWnd);
  }

  getIncTextNow(incNow, incNowText);

  if(popup.isVisible()) popup.show(FALSE);
  if(incNow->infoMode!=NO_INFO)
  { RECT r, r0;
	 int x, y, l, m;

	 m=leftMargin;
	 getChPos(incNow->p, l, x, y);
	 if(incNow->hCBList!=NULL || y>pos.top+(RECTH(pos)/2))
		r.top=y-m-incNow->infoHeight;
	 else
		r.top=y+lineHeight+m;

	 r.left=x;
	 r.right=r.left+incNow->infoWidth;
	 r.bottom=r.top+incNow->infoHeight;

	 if(incNow->hCBList)
	 { r0=r;
		if(r.top<pos.top)
		{ OffsetRect(&r, incNow->cbwidth+m, incNow->infoHeight+m);
		  if(r.right>rightMargin)
		  { OffsetRect(&r, -m-incNow->cbwidth-m-incNow->infoWidth, 0);
			 if(r.left<pos.left) r=r0;
		  }
		}
	 }
	 if(r.right>rightMargin) OffsetRect(&r, rightMargin-r.right, 0);

	 popup.getData(incNow->infoTxt, r, incNow->flagsInc, incNow->infoDelay, incNow->infoMax);
	 if(incNow->infoMode==ALWAYS ||
		 (incNow->infoMode==ON_ERROR && incNow->estat==EN_ERROR))
		popup.show(TRUE);
  }

  return TRUE;
}


BOOL TxContenidor::getCurIncLen(Incognita *incx, int &lTotal, int &lEfectiu)
{
  int p, pn, a;

  lTotal=lEfectiu=0;
  if(incx==NULL) return FALSE;

  if(!lockMem()) return FALSE;

  pn=getNextIncPos(incx);
  for(p=incx->p; p<pn && ((a=txAttr[p])&TX_INC)!=0; p++)
  { lTotal++;
	 if((a&TX_FARCIMENT)==0) lEfectiu++;
  }

  unlockMem();

  return TRUE;
}

int TxContenidor::getStrWidth(char *str)
{
  int i=0;

  // NO ES COMPTEN ELS TAB
  while(*str) i+=widthTable[*str++];

  return i;
}

BOOL TxContenidor::processCBMsg(UINT msg , WPARAM wParam)
{
  Incognita *in;
  ITERAINC if(inci->hCBList==(HWND)wParam) break;

  if(inci==NULL) return FALSE;
  in=inci;

  switch(msg)
  { case WM_CBNEXT:
			if(!goToIncEv(in->p, TRUE)) activateInc(in);
			break;

	 case WM_CBPREV:
			if(!goToIncEv(in->p, FALSE)) activateInc(in);
			break;

	 case WM_CBGETFOCUS:
			if(in!=incNow)
			{ if(af->avCont) avaluaIncognita(incNow);
			  if(af->avCont==FALSE || af->okToNext==FALSE || incNow==NULL || incNow->estat!=EN_ERROR)
				 activateInc(in, FALSE);
			  else
				 activateInc(incNow);
			}
			break;

	 default: break;
  }

  return TRUE;
}


LRESULT CALLBACK TxContenidor::CBSubClassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  UINT m=0;
  BOOL plega=FALSE;
  BOOL invr=FALSE;

  switch(msg)
  { case WM_SETFOCUS:
			m=WM_CBGETFOCUS;
	 case WM_KILLFOCUS:
			invr=TRUE;
			break;

	 case WM_COMMAND:
         // Funcio indocumentada que es dona quan es replega la llista
			if(wParam==1000) m=WM_CBNEXT;
			break;

	 case WM_KEYDOWN:
			switch (wParam)
			{ case VK_ESCAPE:
					 PostMessage(GetParent(hWnd), WM_KEYDOWN, wParam, lParam);
					 break;

			  case VK_RIGHT:
					 plega=TRUE;
			  case VK_RETURN:
					 m=WM_CBNEXT;
					 break;

			  case VK_TAB:
					 m=((GetKeyState(VK_SHIFT) & 0x8000)!=0) ? WM_CBPREV : WM_CBNEXT;
					 break;

			  case VK_LEFT:
					 plega=TRUE;
					 m=WM_CBPREV;
					 break;
			}
			invr=TRUE;
			break;

	 case WM_KEYUP:
	 case WM_CHAR:
			switch (wParam)
			{ case VK_RIGHT:
			  case VK_LEFT:
			  case VK_TAB:
					 plega=TRUE;

			  default: break;
			}
			break;

	 case WM_PAINT:

			CallWindowProc((FARPROC)lpfnOldCBClassProc, hWnd, msg, wParam, lParam);

			{ HDC hDC;
			  COLORREF oldTextColor;
			  COLORREF oldBkColor;
			  int oldBkMode;
			  HFONT hOldFont;
			  RECT r;
			  int c;

			  c=GetWindowLong(hWnd, GWL_ID)==0 ? 0 : 1;
			  hDC=GetDC(hWnd);

			  oldBkColor=SetBkColor(hDC, extraColors[c].backColor);
			  oldTextColor=SetTextColor(hDC, extraColors[c].textColor);
			  oldBkMode=SetBkMode(hDC, TRANSPARENT);
			  if(bc[GRA].hf)
				 hOldFont=(HFONT)SelectObject(hDC, bc[GRA].hf);
			  else
				 hOldFont=0;

			  GetClientRect(hWnd, &r);
			  InflateRect(&r, -3, -3);
			  r.right-=(GetSystemMetrics(SM_CXHSCROLL)-1);
			  FillRect(hDC, &r, (HBRUSH)extraColors[c].shadow);
			  if(GetFocus()==hWnd) DrawFocusRect(hDC, &r);
			  InflateRect(&r, -1, -1);
			  GetWindowText(hWnd, txtbuff, LMAXRFTXT);
			  DrawText(hDC, txtbuff, -1, &r, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
			  if(hOldFont) SelectObject(hDC, hOldFont);

			  SetBkMode(hDC, oldBkMode);
			  SetTextColor(hDC, oldTextColor);
			  SetBkColor(hDC, oldBkColor);
			  ReleaseDC(hWnd, hDC);
			  return 0;
			}


	 default: break;
  }

  if(invr) InvalidateRect(hWnd, NULL, FALSE);
  if(m) PostMessage(GetParent(hWnd), m, (WPARAM)hWnd, 0L);
  if(plega) return 0;

  return CallWindowProc((FARPROC)lpfnOldCBClassProc, hWnd, msg, wParam, lParam);
}

FARPROC TxContenidor::lpfnCBSubClassProc=NULL;
WNDPROC TxContenidor::lpfnOldCBClassProc=NULL;
int TxContenidor::farProcCount=NULL;

BOOL TxContenidor::clearAllIncs()
{
  if(!lockMem()) return FALSE;
  _fmemset(txAttr, 0, txMaxLen);
  unlockMem();

  while(inc) {inci=inc->next; delete inc; inc=inci;}
  incNow=NULL;

  while(finc) {finci=finc->next; delete finc; finc=finci;}
  fincNow=NULL;

  return TRUE;
}

int  TxContenidor::countIncs()
{
  int n, i;
  BOOL inInc;

  n=0;
  if(af->puzMode==FORATS)
	 ITERAINC n++;
  else
  { if(!lockMem()) return 0;

	 if(af->puzMode==COMPTEXT)
	 { for(i=0, n=0, inInc=FALSE; af->txBase[i]!=0; i++)
		  if(af->txBase[i]==CHINC)
		  { if(inInc) inInc=FALSE;
			 else {inInc=TRUE; n++;}
		  }
	 }
	 else
	 { for(i=0, n=0, inInc=FALSE; txNow[i]!=0 && i<txMaxLen; i++)
		  if(txAttr[i]&((af->puzMode==IDLLETRES || af->puzMode==IDPARAULES) ? TX_HIDINC : TX_INC))
		  { if(!inInc) {n++; inInc=TRUE;}}
		  else if(inInc) inInc=FALSE;
	 }

	 unlockMem();
  }

  return n;
}

void TxContenidor::getFocus(BOOL /*status*/)
{
  //controlOn=FALSE;
  //shiftOn=FALSE;

}

BOOL TxContenidor::avaluaIncognita(Incognita *in)
{
  BOOL result;
  char *tx=0;

  if(getIncTextNow(in, tx)==FALSE) return FALSE;
  if(af->avCont==TRUE && incNowText!=NULL && strcmp(tx, incNowText)==0)
  { delete tx; return FALSE;}

  if(!lockMem()) {delete tx; return FALSE;}

  result=checkString(tx, in->ok, af, in->hCBList ? 0 : TX_ERR, txAttr+in->p);
  delete tx;

  in->estat=result ? RESOLTA : EN_ERROR;

  if(in->hCBList)
  { SetWindowWord(in->hCBList, GWW_ID, result ? COL_USR : COL_ERR);
	 InvalidateRect(in->hCBList, NULL, TRUE);
  }
  else invr();

  unlockMem();

  if(af->avCont) PostMessage(hWnd, WM_NOUINTENT, 0, 0L);

  return result;
}


BOOL TxContenidor::getIncTextNow(Incognita *in, char *&destBuff)
{
  int p, pn, l;
  char a;
  BOOL result=TRUE;

  if(destBuff) {delete destBuff; destBuff=0;}
  if(in==NULL) return FALSE;

  if(!lockMem()) return FALSE;

  if(in->hCBList)
  { *txtbuff=0;
	 GetWindowText(in->hCBList, txtbuff, LMAXRFTXT);
	 destBuff=dupStr(txtbuff);
  }
  else
  { pn=getNextIncPos(in);
	 for(l=0, p=in->p; p<pn && ((a=txAttr[p])&TX_INC)!=0; p++)
		if((a&TX_FARCIMENT)==0) l++;
	 if((destBuff=new char[l+1])==NULL) {NOMEM; result=FALSE;}
	 char *tx=destBuff;
	 for(l=0, p=in->p; p<pn && ((a=txAttr[p])&TX_INC)!=0; p++)
		if((a&TX_FARCIMENT)==0) *tx++=txNow[p];
	 *tx=0;
  }

  unlockMem();

  return result;
}


int  TxContenidor::getNextIncPos(Incognita *in)
{
  int p, p0;

  if(in==NULL) return txLen;

  p0=in->p;
  p=txLen;
  ITERAINC if(inci->p>p0) if(inci->p<p) p=inci->p;

  return p;
}

BOOL TxContenidor::getEncerts(int &nEncerts)
{
  int ni=0;

  if(af->puzMode==FORATS && af->avCont==TRUE)
  { nEncerts=0;
	 ITERAINC {ni++; if(inci->estat==RESOLTA) nEncerts++;}
	 if(ni>0 && ni==nEncerts) ok=TRUE;
  }
  else nEncerts=avaluaActivitat();

  return ok;
}

void TxContenidor::endGame()
{
  playing=FALSE;

  ITERAINC if(inci->hCBList) EnableWindow(inci->hCBList, FALSE);
  if(popup.isVisible()) popup.show(FALSE);
  if(btCorregir) EnableWindow(btCorregir, FALSE);
  lostCaret();

}

BOOL TxContenidor::goToIncEv(int from, BOOL fwd)
{
  BOOL result=FALSE;
  BOOL oktonext;

  if(incNow!=0 && af->avCont) avaluaIncognita(incNow);

  oktonext=(af->okToNext ||
				(incNow!=0 && incNow->infoMode==ON_ERROR && !popup.isVisible()));

  if(af->avCont==FALSE || /*af->okToNext*/oktonext==FALSE || incNow==NULL || incNow->estat!=EN_ERROR)
	 return goToInc(from, fwd);
  else
	 if(incNow->estat==EN_ERROR)
	 { result=TRUE;
		if(incNow->hCBList && GetFocus()!=incNow->hCBList) SetFocus(incNow->hCBList);
		if(incNow->infoMode==ON_ERROR) popup.show(TRUE);
	 }

  if(incNowText) {delete incNowText; incNowText=NULL;}

  return result;
}

BOOL TxContenidor::marcaBloc(int p, char atr)
{
  BOOL result=FALSE;
  int pini, pend;

  if(!lockMem()) return FALSE;

  switch(af->puzMode)
  { case IDLLETRES:
			if(!isForbiddenChar(txNow[p]))
			{ txAttr[p]^=atr;
			  txAttr[p]&=~TX_ERR;
			  result=TRUE;
			}
			break;

	 case BPARAULES:
	 case IDPARAULES:
			if(!isSep(txNow[p]))
			{ for(pini=p; pini>=0; pini--) if(isSep(txNow[pini])) break;
			  for(pend=p+1; txNow[pend]!=0; pend++) if(isSep(txNow[pend])) break;
			  for(p=pini+1; p<pend; p++) {txAttr[p]^=atr; txAttr[p]&=~TX_ERR;}
			  result=TRUE;
			}
			break;

	 case BPARAGRAFS:
			for(pini=p; pini>=0; pini--) if(txNow[pini]==0x0D) break;
			for(pend=p; pend<txLen; pend++) if(txNow[pend]==0x0D) break;
			for(p=pini+1; p<pend; p++) {txAttr[p]^=atr; txAttr[p]&=~TX_ERR;}
			result=TRUE;

	 default: break;
  }

  unlockMem();

  if(result) invr();
  return result;
}

int TxContenidor::avaluaActivitat()
{
  int nEncerts=0;
  int i, i0, falten=0;
  BOOL inInc, lastIncOk, usErr;
  char *txt, *txp, ch1, ch2;
  Incognita *in0;

  if(!lockMem()) return FALSE;

  for(i=0; i<txLen; i++) txAttr[i]&=(~TX_ERR);

  switch(af->puzMode)
  { case FORATS:
			if(deBoto)
			{ ok=TRUE;
			  for(in0=inc; in0!=NULL; in0=in0->next)
			  { avaluaIncognita(in0);
				 if(in0->estat==RESOLTA) nEncerts++; else ok=FALSE;
			  }
			}
			else ok=FALSE;

			break;

	 case IDLLETRES:
			ok=TRUE;
			for(i=0; i<txLen; i++)
			  if((txAttr[i]&TX_HIDINC)!=0)
			  { if(txAttr[i]&TX_USR) nEncerts++;
				 else {falten++; ok=FALSE;}
			  }
			  else if(txAttr[i]&TX_USR) {nEncerts--; txAttr[i]|=TX_ERR; ok=FALSE;}
			if(nEncerts<=0) ok=FALSE;
			break;

	 case IDPARAULES:
			ok=TRUE;
			inInc=FALSE;
			lastIncOk=FALSE;
			usErr=FALSE;
			for(i=0; i<txLen; i++)
			  if((txAttr[i]&TX_HIDINC)!=0)
			  { if(!inInc) {inInc=TRUE; usErr=FALSE; lastIncOk=TRUE;}
				 if((txAttr[i]&TX_USR)==0) {lastIncOk=FALSE;}
			  }
			  else
			  { if(inInc)
				 { inInc=FALSE;
					if(lastIncOk) nEncerts++;
					else {ok=FALSE; falten++;}
				 }
				 if(txAttr[i]&TX_USR)
				 { txAttr[i]|=TX_ERR;
					ok=FALSE;
					if(!usErr) {usErr=TRUE; nEncerts--;}
				 }
				 else usErr=FALSE;
			  }
			if(inInc) {if(lastIncOk) nEncerts++; else {ok=FALSE; falten++;}}
			break;


	 case COMPTEXT:
			ok=TRUE;
			inInc=FALSE;
			txt=af->txBase;
			i=0;
			while(*txt!=0)
			{ if((txp=strchr(txt, CHINC))==0) txp=txt+strlen(txt);
			  if(inInc)
			  { i0=i;
				 while((txAttr[i]&TX_USR)!=0 && txNow[i]!=0) i++;
				 ch2=txNow[i];
				 txNow[i]=0;
				 ch1=*txp;
				 *txp=0;
				 if(i0==i) {falten++; ok=FALSE;}
				 else
				 { if(checkString(txNow+i0, txt, af, TX_ERR, txAttr+i0))
					  nEncerts++;
					else
					  ok=FALSE;
				 }
				 *txp=ch1;
				 txNow[i]=ch2;
			  }
			  else for(;txt<txp;txt++)
			  { if(*txt==0x0A) continue;
				 if(*txt==CHBLOCK)
				 { txt++;
					while(txt<txp && *txt++!=CHBLOCK);
					i++;
				 }
				 if(txAttr[i]&TX_USR)
				 { ok=FALSE;
					while(txAttr[i]&TX_USR) txAttr[i++]|=TX_ERR;
				 }
				 i++;
			  }
			  txt=txp;
			  if(*txt) txt++;
			  inInc^=1;
			}
			if(txNow[i]!=0)
			{ ok=FALSE;
			  for(;txNow[i]!=0; i++) txAttr[i]|=TX_ERR;
			}
			break;

	 case BPARAULES:
	 case BPARAGRAFS:
			if(!deBoto) break;
			ok=TRUE;
			if(anchorFinc)
			{ markFinc(anchorFinc, 0, TX_ANCHORFINC);
			  anchorFinc=NULL;
			}
			ITERAFI
			{ ch1=txNow[finci->end+1];
			  txNow[finci->end+1]=0;
			  for(txp=af->txBase+finci->srcPos; *txp!=0 && *txp!=CHINC; txp++);
			  ch2=*txp;
			  *txp=0;
			  if(checkString(txNow+finci->beg, af->txBase+finci->srcPos, af))
				 nEncerts++;
			  else
			  { ok=FALSE;
				 markFinc(finci, TX_ERR, 0);
			  }
			  txNow[finci->end+1]=ch1;
			  *txp=ch2;
			}
			break;

	 default: break;
  }
  if(nEncerts<=0) ok=FALSE;

  unlockMem();

  invr(TRUE);
  UpdateWindow(hWnd);
  if(deBoto && falten)
  { if(falten==1) strcpy(txtbuff, getStringTbl(IDS_FALTA));
	 else wsprintf(txtbuff, getStringTbl(IDS_FALTEN), falten);
	 BMsgBox(hWnd, 0, 0, txtbuff, 0, MB_OK);
  }
  if(deBoto && GetFocus()!=hWnd) SetFocus(hWnd);
  deBoto=FALSE;

  return nEncerts;
}

BOOL TxContenidor::activateFinc(struct FloatInc *f)
{
  if(f==NULL || (fincNow!=NULL && fincNow==f)) return FALSE;

  if(fincNow) markFinc(fincNow, 0, TX_FINCNOW|TX_ERR);
  fincNow=f;
  markFinc(fincNow, TX_FINCNOW, TX_ERR);
  caretPos=f->beg;
  setCaretAtPos(TRUE);

  return TRUE;
}

BOOL TxContenidor::markFinc(struct FloatInc *f, char on, char off)
{
  int i, l0, l1;
  RECT r;

  if(f==NULL) return FALSE;

  if(!lockMem()) return FALSE;

  l0=-1;
  for(i=f->beg; i<=f->end; i++)
  { if(on) txAttr[i]|=on;
	 if(off) txAttr[i]&=~off;
  }

  l0=locateLineFromChar(f->beg);
  l1=locateLineFromChar(f->end);
  for(i=l0; i<=l1; i++)
  { getLineRect(i, &r);
	 InvalidateRect(hWnd, &r, FALSE);
  }

  unlockMem();
  return TRUE;
}

struct FloatInc *TxContenidor::getFIncFromChar(int p)
{
  ITERAFI if(p>=finci->beg && p<=finci->end) break;

  return finci;

}

BOOL TxContenidor::swapFincs(struct FloatInc *fi0, struct FloatInc *fi1)
{
  int i, j, lfi0, lfi1, m;
  HGLOBAL hBuff;
  char far *ftxbuff;
  BOOL result=FALSE;

  if(fi0==NULL || fi1==NULL || fi0==fi1) return FALSE;

  if(fi1->beg < fi0->beg) {finci=fi1; fi1=fi0; fi0=finci;}

  lfi0 = fi0->end - fi0->beg + 1;
  lfi1 = fi1->end - fi1->beg + 1;

  if((hBuff=GlobalAlloc(GHND, lfi0+lfi1+1))!=NULL)
  { if((ftxbuff=(char far*)GlobalLock(hBuff))!=NULL)
	 { if(lockMem())
		{ for(i=0; i<lfi0; i++) ftxbuff[i]=txNow[fi0->beg+i];
		  for(i=0; i<lfi1; i++) ftxbuff[lfi0+i]=txNow[fi1->beg+i];
		  m=lfi1-lfi0;
		  if(m && (fi1->beg!=fi0->end+1))
		  { _fmemmove(txNow+fi0->end+1+m, txNow+fi0->end+1, fi1->beg-fi0->end-1);
			 _fmemmove(txAttr+fi0->end+1+m, txAttr+fi0->end+1, fi1->beg-fi0->end-1);
			 ITERACNT if(cnti->c->extra>=fi0->end && cnti->c->extra<=fi1->beg) cnti->c->extra+=m;
			 ITERAINC if(inci->p>=fi0->end && inci->p<=fi1->beg) inci->p+=m;
			 ITERAFI  if(finci->beg>fi0->end && finci->end<fi1->beg)
						 { finci->beg+=m;
							finci->end+=m;
						 }
		  }
		  for(i=0, j=fi1->beg+m; i<lfi0; i++) {txNow[j+i]=ftxbuff[i]; txAttr[j+i]=TX_INC;}
		  for(i=0, j=fi0->beg; i<lfi1; i++) {txNow[j+i]=ftxbuff[lfi0+i]; txAttr[j+i]=TX_INC;}
		  fi1->beg+=m;
		  //i=fi0->beg; fi0->beg=fi1->beg+m; fi1->beg=i;
		  fi0->end=fi0->beg+lfi1-1;
		  fi1->end=fi1->beg+lfi0-1;

		  /*
		  i=locateLineFromChar(fi0->beg);
		  j=locateLineFromChar(fi1->end);
		  if(i>0) setLines(i-1, TRUE, FALSE);
		  for(; i<=j; i++) setLines(i, TRUE, FALSE);
		  */
		  setLines(0, FALSE, TRUE);
		  invr();
		  result=TRUE;
		  unlockMem();
		}
		GlobalUnlock(hBuff);
	 }
	 GlobalFree(hBuff);
  }

  if(!result) {NOMEM; return FALSE;}

  return TRUE;

}

BOOL TxContenidor::goToFinc(int from, BOOL fwd)
{
  struct FloatInc *f;

  if(!lockMem()) return FALSE;

  // se suposa que sempre hi ha un separador entre incognites
  while(from>0 && txNow[from]!=0 && (txAttr[from]&TX_INC)!=0) from+=(fwd ? 1:-1);
  while(from>0 && txNow[from]!=0 && (txAttr[from]&TX_INC)==0) from+=(fwd ? 1:-1);
  f=getFIncFromChar(from);
  if(f) activateFinc(f);

  unlockMem();
  return TRUE;
}

BOOL TxContenidor::scramble(int vegades, BOOL updating)
{
  int nfi, i, j, k, v0, nv;
  struct FloatInc *fincNow0;
  BOOL result;

  nfi=0;
  ITERAFI nfi++;
  if(edit || nfi<2) return FALSE;

  if(!lockMem()) return FALSE;
  fincNow0=fincNow;
  for(i=0; i<vegades; i++)
  { result=FALSE;
	 if(af->puzMode==BPARAULES && af->brPar==FALSE)
	 { for(j=0; j<txLen; j++)
		{ for(k=j; k<txLen; k++) if(txNow[k]==0x0D) break;
		  v0=0; ITERAFI {if(finci->beg >=j) break; v0++;}
		  if(finci!=0 && finci->beg<k)
		  { nv=0;
			 while(finci!=0 && finci->beg<k) {finci=finci->next; nv++;}
			 if(nv>=2) result=scrambleBetween(v0, nv);
		  }
		  j=k;
		}
	 }
	 else
	 { v0=0;
		nv=nfi;
		result=scrambleBetween(0, nfi);
	 }
	 if(result && updating) UpdateWindow(hWnd);
  }
  if(fincNow0) {fincNow=NULL; activateFinc(fincNow0);}
  unlockMem();

  return TRUE;

}

BOOL TxContenidor::scrambleBetween(int v0, int nv)
{
  struct FloatInc *fi0, *fi1;
  int k, j;

  j=v0+random(nv); k=0; ITERAFI {if(k==j) break; k++;} fi0=finci;
  j=v0+random(nv); k=0; ITERAFI {if(k==j) break; k++;} fi1=finci;
  if(fi0!=fi1)
  { swapFincs(fi0, fi1);
	 return TRUE;
  }
  return FALSE;
}

BOOL TxContenidor::doHelp()
{
  if(!playing || edit || af->puzMode!=FORATS) return FALSE;

  if(incNow!=NULL && incNow->infoMode==ON_DEMAND)
	 popup.show(TRUE, TRUE);

  return TRUE;
}


