// -----------------------------------
// ZONES.CPP
// Prototipus de les classes:
//  - Zona
//  - Contenidor
//  - Fil
// -----------------------------------

#include "zones.h"
#include "clic.h"
#include "rcomu\clicres.h"
#include "miscfunc.h"
#include "wcl.h"
#include <string.h>
#include <stdlib.h>
#include <dir.h>

// -----------------------------------
// Implementacio de Zona
// -----------------------------------

BaseCont * Zona::bc=NULL;
int Zona::fontReduced=-1;
Colors * Zona::extraColors=NULL;
HBITMAP * Zona::tramaBmp=NULL;

Zona::Zona()
{
  pos=NullRect;
  hWnd=0;
  flags=0;
  setFlag(HID, TRUE);
}

BOOL Zona::create(HWND hOwner, RECT *rpos, BOOL iHid, BOOL iMark, BASE_ZONES base)
{
  if((hWnd=hOwner)==0) return FALSE;
  pos=*rpos;
  setFlag(HID, iHid);
  setFlag(MARK, iMark);
  setBase(base);
  invr();
  return TRUE;
}

void Zona::update(HDC hDC, RECT *, HDC)
{
  if(hWnd)
  { FillRect(hDC, (LPRECT)&pos,
				 getFlag(MARK) ? bc[getBase()].hMarkBrush :
									  ((getFlag(HID) && !getFlag(ALT)) ? bc[getBase()].hHidBrush:
																					 bc[getBase()].hBackBrush));

	 if(getFlag(MARC)) FrameRect(hDC, &pos, bc[getBase()].hMarkBrush);

  }
}

RECT bufRect;


BOOL Zona::drawRect(HDC hDC, int offset)
{
  return Rectangle(hDC, pos.left-offset, pos.top-offset, pos.right+offset, pos.bottom+offset);
}


BOOL Zona::isIn(POINT p)
{ return (p.x>=pos.left && p.x<pos.right &&
			 p.y>=pos.top && p.y<pos.bottom);}

BOOL Zona::isIn(RECT *rect)
{
  if(!hWnd) return FALSE;
  if(equalRect(rect)) return TRUE;

  bufRect=*rect;
  return (((pos.left>=bufRect.left && pos.left<bufRect.right) ||
			  (pos.left<bufRect.left && pos.right>bufRect.left))
			 &&
			((pos.top>=bufRect.top && pos.top<bufRect.bottom) ||
			 (pos.top<bufRect.top && pos.bottom>bufRect.top)));
}

BOOL Zona::equalRect(RECT *rcp)
{ bufRect=*rcp;
  return (bufRect.left==pos.left && bufRect.right==pos.right &&
			 bufRect.top==pos.top && bufRect.bottom==pos.bottom);
}


BOOL Zona::isSubRect(RECT *rect)
{ bufRect=*rect;
  return (bufRect.left>=pos.left && bufRect.top>=pos.top &&
			 bufRect.right<=pos.right && bufRect.bottom<=pos.bottom);
}

void Zona::move(RECT *rpos)
{
  if(hWnd)
  { invr(TRUE);
	 pos=*rpos;
	 invr();
  }
}

void Zona::move(int dx, int dy)
{
  if(hWnd)
  { invr(TRUE);
	 OffsetRect((LPRECT)&pos, dx, dy);
	 invr();
  }
}

BOOL Zona::locateCursorHere()
{
	RECT wr;
	int x, y;

	if(hWnd==0) return FALSE;
	GetWindowRect(hWnd, &wr);
	x=(pos.right-pos.left)/2; y=(pos.bottom-pos.top)/2;
	SetCursorPos(wr.left+pos.left+x, wr.top+pos.top+y);
	return TRUE;
}

/*
HFONT Zona::setBaseCont(HFONT hf, Colors *c, BASE_ZONES f)
{ HFONT hfont0=bc[f].hf;
  bc[f].hf=hf;
  if(c!=NULL) bc[f].c=*c;
  return hfont0;
}
*/

BOOL Zona::setBaseBrush(BASE_ZONES f, COLORREF c, int trama)
{
  if(trama==bc[f].trama && c==bc[f].c.backColor) return TRUE;

  if(bc[f].hBackBrush) {DelObj(bc[f].hBackBrush); bc[f].hBackBrush=NULL;}

  bc[f].trama=trama;
  bc[f].c.backColor=c;

  if(trama>=0)
	 bc[f].hBackBrush=CreatePatternBrush(tramaBmp[trama]);
  else
	 bc[f].hBackBrush = CreateSolidBrush(c);

  return TRUE;
}

BOOL Zona::setBaseCont(LOGFONT *lf, Colors *c, BASE_ZONES f)
{
  if(bc[f].hf) {DelObj(bc[f].hf); bc[f].hf=NULL;}
  if(lf!=NULL)
  { if(&bc[f].lfb!=lf) bc[f].lfb=*lf;
	 bc[f].hf=CreateFontIndirect(lf);
	 if(bc[f].hf)
	 { if(f==1) caretH=lf->lfHeight;
		bc[f].shadowDeep=2+lf->lfHeight/26;
	 }
  }

  if(c!=NULL)
  {
	 if(bc[f].c.backColor!=c->backColor)
	 { if(bc[f].hBackBrush) DelObj(bc[f].hBackBrush);
		bc[f].hBackBrush = CreateSolidBrush(c->backColor);
	 }

	 if(bc[f].c.shadowColor!=c->shadowColor)
	 { if(bc[f].hHidBrush) DelObj(bc[f].hHidBrush);
		bc[f].hHidBrush = CreateSolidBrush(c->shadowColor);
	 }

	 if(bc[f].c.textColor!=c->textColor)
	 { if(bc[f].hMarkBrush) DelObj(bc[f].hMarkBrush);
		bc[f].hMarkBrush = CreateSolidBrush(c->textColor);
	 }

	 bc[f].c=*c;

	 /*
	 bc[f].c=*c;

	 if(bc[f].hBackBrush) DelObj(bc[f].hBackBrush);
	 if(bc[f].hHidBrush)  DelObj(bc[f].hHidBrush);
	 if(bc[f].hMarkBrush) DelObj(bc[f].hMarkBrush);

	 bc[f].hBackBrush = CreateSolidBrush(bc[f].c.backColor);
	 bc[f].hHidBrush  = CreateSolidBrush(bc[f].c.shadowColor);
	 bc[f].hMarkBrush = CreateSolidBrush(bc[f].c.textColor);
	 */
  }

  return (bc[f].hf!=NULL);
}

BOOL Zona::setExtraColors(EXTRA_COLORS c, COLORREF txcolor, COLORREF bkcolor)
{
  extraColors[c].textColor=txcolor;
  extraColors[c].backColor=bkcolor;
  if(extraColors[c].shadow!=0) DelObj((HBRUSH)extraColors[c].shadow);
  extraColors[c].shadow=(UINT)CreateSolidBrush(bkcolor);
  return TRUE;
}

#pragma argsused
BOOL Zona::reduceFont(BASE_ZONES f, long numerador, long denominador)
{
  LONG *w, *h;

  if(bc[f].hf==NULL) return FALSE;

  w=&bc[f].lfb.lfWidth;
  h=&bc[f].lfb.lfHeight;

  if(*w<=4 || *h<=4) return FALSE;

  if((*w=(int)((numerador**w)/denominador))<4) *w=4;
  if((*h=(int)((numerador**h)/denominador))<4) *h=4;
  Zona::fontReduced=f;

  return Zona::setBaseCont(&bc[f].lfb, NULL, f);

}

void Zona::initAllBaseCont()
{
  int f, i;
  Colors c1, c2;

  bc=new BaseCont[NMAXZONES];
  tramaBmp=new HBITMAP[NTRAMES];

  for(i=0; i<NTRAMES; i++)
	 tramaBmp[i]=LoadBitmap(_hInstance, MAKEINTRESOURCE(TRAMA0+i));

  c1.backColor=RGB(0xFF, 0xFF, 0xFF);
  c2.backColor=RGB(0xC0, 0xC0, 0xC0);
  c2.textColor=c1.textColor=RGB(0, 0, 0);
  c2.shadowColor=c1.shadowColor=RGB(0x80, 0x80, 0x80);
  c2.shadow=c1.shadow=FALSE;

  memset(bc, 0, sizeof(BaseCont)*NMAXZONES);
  bc[GRA].c=bc[GRB].c=bc[POP].c=c1;
  bc[MSB].c=bc[GENERICA].c=bc[PLAYWIN].c=c2;

  for(f=0; f<NMAXZONES; f++)
  { bc[f].shadowDeep=1;
	 bc[f].hBackBrush=CreateSolidBrush(bc[f].c.backColor);
	 bc[f].hHidBrush=CreateSolidBrush(bc[f].c.shadowColor);
	 bc[f].hMarkBrush=CreateSolidBrush(bc[f].c.textColor);
	 bc[f].trama=-1;
  }

  extraColors=new Colors[NEXTRACOLORS];
  for(f=0; f<NEXTRACOLORS; f++)
	 extraColors[f]=c1;
}

void Zona::clearAllBaseCont()
{ int i;

  for(i=0; i<NMAXZONES; i++)
  { if(bc[i].hf) {DelObj(bc[i].hf); bc[i].hf=NULL;}
	 if(bc[i].hBackBrush) {DelObj(bc[i].hBackBrush); bc[i].hBackBrush=NULL;}
	 if(bc[i].hHidBrush) {DelObj(bc[i].hHidBrush); bc[i].hHidBrush=NULL;}
	 if(bc[i].hMarkBrush) {DelObj(bc[i].hMarkBrush); bc[i].hMarkBrush=NULL;}
  }

  delete bc;
  bc=NULL;

  for(i=0; i<NEXTRACOLORS; i++)
	 if(extraColors[i].shadow!=0)
	 { DelObj((HBRUSH)extraColors[i].shadow);
		extraColors[i].shadow=NULL;
	 }
  delete extraColors;
  extraColors=NULL;

  for(i=0; i<NTRAMES; i++)
	 DelObj(tramaBmp[i]);
  delete tramaBmp;
  tramaBmp=NULL;
}

void Zona::invr(BOOL fons)
{ if(hWnd)
	 InvalidateRect(hWnd, (LPRECT)&pos, fons);
}

// -----------------------------------
// Implementacio de Contenidor
// -----------------------------------

Contenidor* Contenidor::caretOwner=NULL;


Contenidor::Contenidor(BASE_ZONES Base)
{
  cont=RES;
  soCont=CAP_SO;
  hBm=0;
  hTxBm=0;
  rbm=NullRect;
  lRbmX=lRbmY=0;
  origin=NullPoint;
  txt=uch;
  uch[1]=0;
  fnSoCont=0;
  extra=0;
  setBase(Base);
}

BOOL Contenidor::capture(BOOL op)
{
  BOOL i=getFlag(CAPTURED);
  setFlag(CAPTURED, op);
  return i;
}

void Contenidor::clear()
{
  if(getFlag(OWNSCARET)) lostCaret();
  if(txt!=uch) {delete txt; txt=uch;}
  setFlag(UNICHAR, FALSE);
  if(fnSoCont) {delete fnSoCont; fnSoCont=0; if(mciPlaying) silenci();}
  if(hTxBm) {bmpCont::freeBmp(hTxBm);}
  cont=RES;
  soCont=CAP_SO;
  setFlag(STRCHMCI, FALSE);
  setFlag(FREEMCI, FALSE);
  invr();
}

BOOL Contenidor::play()
{

  if(soCont==CAP_SO || !fnSoCont) return FALSE;

  if(soCont==EXIT_CLIC)
  { exitDirect=TRUE;
	 PostMessage(genMsgOwn, WM_CLOSE, 0, 0L);
	 return TRUE;
  }

  if(soCont==EXEC_EXE)
  { struct ffblk fileInfo;
	 char *cmdLine;
	 char *tx;

	 cmdLine=getFullFileName(fnSoCont);
	 if((tx=strchr(cmdLine, ' '))!=NULL) *tx=0;
	 if(findfirst(cmdLine, &fileInfo, 0)==0)
	 { if(tx) *tx=' ';}
	 else cmdLine=fnSoCont;

	 if(WinExec(cmdLine, SW_SHOWNORMAL)<32)
		BMsgBox(hWnd, IDS_ERREXE, 0, fnSoCont, 0, MB_OK|MB_ICONEXCLAMATION);
	 return TRUE;
  }

  if(soCont==USER_CONT)
  { //----- Atencio:
	 strcpy(tempPath3, fnSoCont);
	 PostMessage(genMsgOwn, WM_COMMAND, IDM_OPENCMD, (LPARAM)(LPSTR)tempPath3);
	 return TRUE;
  }

  return playFile(//soCont==CDAUDIO ? fnSoCont : getFullFileName(fnSoCont),
						fnSoCont,
						(TIPUS_SO)soCont, getFlag(FREEMCI) ? 0:hWnd, &pos, getFlag(STRCHMCI));

}

BOOL Contenidor::getData(HBITMAP hBitmap, RECT *rbmtros, POINT *orgBm)
{
  if(!hBitmap) return FALSE;
  hBm=hBitmap;

  if(getFlag(OWNSCARET)) lostCaret();

  if(!orgBm)
  { soCont=CAP_SO;
	 if(fnSoCont) {delete fnSoCont; fnSoCont=0;}
	 setFlag(STRCHMCI, FALSE);
	 setFlag(FREEMCI, FALSE);
  }

  if(rbmtros)
  { rbm=*rbmtros;
	 lRbmX=RECTW(rbm); lRbmY=RECTH(rbm);
  }
  else rbm=NullRect;

  origin=(orgBm ? *orgBm:NullPoint);
  cont=BMP;
  invr();
  return TRUE;
}

BOOL Contenidor::getData(char *str)
{
  char *txtp, *txtb, *c;
  int i, j;
  POINT p;
  BOOL flagBmp=FALSE;
  TIPUS_SO ts;

  if(getFlag(OWNSCARET)) lostCaret();
  if(txt!=uch) {delete txt; txt=uch;}
  setFlag(UNICHAR, FALSE);
  if(!str) str=nullStr;
  cont=RES;
  soCont=CAP_SO;
  setFlag(STRCHMCI, FALSE);
  setFlag(FREEMCI, FALSE);
  if(fnSoCont) {delete fnSoCont; fnSoCont=0;}
  if(str[1]==0 || str[0]==0)
  { *uch=*str;
	 setFlag(UNICHAR, TRUE);
  }
  else
	 if((txt=dupStr(str))==0) {txt=uch; return FALSE;}

  txtp=txt;
  txtb=0;

  while((txtb=getBracketTag(txtp))!=0)
  { ts=UNKNOW_MCI;
	 switch(getExt(txtb))
	 { case EXT_BMP:
		case EXT_GIF:
			  if(bmpCont::loadBmp(hWnd, hTxBm, getFullFileName(txtb), i, j)==TRUE)
			  { p=NullPoint;
				 if(i<RECTW(pos)) p.x=(RECTW(pos)-i)/2;
				 if(j<RECTH(pos)) p.y=(RECTH(pos)-j)/2;
			  }
			  flagBmp=TRUE;
			  break;

		case EXT_PUZ:
		case EXT_SOP:
		case EXT_ASS:
		case EXT_TXA:
		case EXT_CRW:
		case EXT_PCC:
		case EXT_PAC: ts=USER_CONT;
		case EXT_MID: if(ts==UNKNOW_MCI) ts=MIDI;
		case EXT_WAV: if(ts==UNKNOW_MCI) ts=WAVE;
		case EXT_EXE: if(ts==UNKNOW_MCI) ts=EXEC_EXE;
		default:
			  if(stricmp(txtb, "stretch")==0) {setFlag(STRCHMCI, TRUE); break;}
			  if(stricmp(txtb, "free")==0) {setFlag(FREEMCI, TRUE); break;}
			  if(fnSoCont) {delete fnSoCont; fnSoCont=0; soCont=CAP_SO;}
			  if(ts==UNKNOW_MCI && strchr(txtb, '.')==NULL)
			  { if(strncmpi(txtb, "rec", 3)==0)
				 { if((c=strchr(txtb, ' '))!=NULL && stricmp(c+1, "play")==0)
					  ts=REC_PLAY;
					else
					  ts=REC_REC;
				 }
				 else if(stricmp(txtb, "exit")==0) ts=EXIT_CLIC;
				 else ts=CDAUDIO;
				 if((fnSoCont=dupStr(txtb))==0) return FALSE;
			  }
			  else if((fnSoCont=dupStr(ts==EXEC_EXE ? txtb : getFullFileName(txtb)))==0)
				 return FALSE;
			  soCont=ts;
			  break;
	 }
  }

  if(flagBmp) return getData(hTxBm, 0, &p);

  cont=TXT;
  invr();
  return TRUE;
}

BOOL Contenidor::setCont(CONTINGUTS newcont)
{
  if(getFlag(OWNSCARET)) lostCaret();
  switch(newcont)
  { case RES:
			break;

	 case BMP:
			if(hBm==0) return FALSE;
			break;

	 case TXT:
			if(txt==uch && !getFlag(UNICHAR)) return FALSE;
			break;
	 default: return FALSE;
  }

  if(cont!=newcont) {cont=newcont; invr();}
  return TRUE;
}

void Contenidor::update(HDC hDC, RECT *updRect, HDC hMemDC)
{
  int dx, dy, oldBkMode, s;
  BASE_ZONES b;
  HFONT hOldFont;
  RECT r, rp;
  COLORREF oldTextColor;
  UINT txFormat;

  if(!hWnd) return;
  if((getFlag(HID) && !getFlag(MARK) && !getFlag(ALT)) || cont==RES) Zona::update(hDC, updRect, hMemDC);
  else
  switch(cont)
  { case BMP:
			if(origin.x || origin.y)
			{ BOOL mark0=getFlag(MARK);
			  setFlag(MARK, FALSE);
			  Zona::update(hDC, updRect, hMemDC);
			  setFlag(MARK, mark0);
			}
			displayBitmap(hDC, hMemDC, hBm,
							  pos.left+origin.x, pos.top+origin.y, RECTW(pos), RECTH(pos),
							  rbm.left, rbm.top, lRbmX, lRbmY);
			if(getFlag(MARK)) drawMarc(hDC, &pos, INTERIOR_NEGRE, 2);
			break;

	 case TXT:
			b=getBase();
			Zona::update(hDC, updRect);
			if(*txt || (getFlag(OWNSCARET) && caretVis))
			{ if(bc[b].hf)
				 hOldFont=(HFONT)SelectObject(hDC, bc[b].hf);
			  else
				 hOldFont=0;

			  oldBkMode=SetBkMode(hDC, TRANSPARENT);

			  if(getFlag(LALIGN)) GetCharWidth(hDC, ' ', ' ', &s);
			  else s=bc[b].c.shadow ? bc[b].shadowDeep : 3;

			  rp=pos; rp.left+=s; rp.right-=s; rp.top-=s; rp.bottom+=s;
			  r=rp; r.bottom=r.top;
			  txFormat=DT_NOCLIP|DT_NOPREFIX|(getFlag(LALIGN)?DT_LEFT:DT_CENTER);
			  if(!getFlag(OWNSCARET)) txFormat|=DT_WORDBREAK;
			  DrawText(hDC, (LPSTR)txt, -1, (LPRECT)&r, txFormat|DT_CALCRECT);
			  while(RECTW(r)>RECTW(rp) || RECTH(r)>RECTH(rp))
			  { r=rp; r.bottom=r.top;
				 SelectObject(hDC, hOldFont);
				 dx=Zona::reduceFont(b, 3, 4);
				 SelectObject(hDC, bc[b].hf);
				 DrawText(hDC, (LPSTR)txt, -1, (LPRECT)&r, txFormat|DT_CALCRECT);
				 if(!dx) break;
			  }
			  dy=(RECTH(rp)-RECTH(r))/2;
			  //dx=/*getFlag(LALIGN) ? 0 : */(RECTW(rp)-RECTW(r))/2;
			  dx=getFlag(LALIGN) ? bc[b].shadowDeep : (RECTW(rp)-RECTW(r))/2;
			  OffsetRect((LPRECT)&r, dx, dy);
			  if(getFlag(OWNSCARET) && caretVis)
			  { RECT r2;
				 r2=r;
				 if(caretPos>0)
				 { DrawText(hDC, (LPSTR)txt, caretPos, (LPRECT)&r2,
								txFormat|DT_CALCRECT);
					caretX=r2.right;
				 }
				 else caretX=r2.left;

				 caretY=r2.top;
				 SetCaretPos(caretX, caretY);
			  }

			  if(bc[b].c.shadow && !getFlag(ALT))
			  { s=bc[b].shadowDeep;
				 oldTextColor=SetTextColor(hDC, bc[b].c.shadowColor);
				 OffsetRect(&r, s, s);
				 DrawText(hDC, (LPSTR)txt, -1, (LPRECT)&r, txFormat);
				 OffsetRect(&r, -s, -s);
				 SetTextColor(hDC, oldTextColor);
			  }

			  oldTextColor=SetTextColor(hDC,
				  getFlag(ALT) ? bc[b].c.shadowColor:
									  getFlag(MARK) ? bc[b].c.backColor :
															bc[b].c.textColor);

			  DrawText(hDC, (LPSTR)txt, -1, (LPRECT)&r, txFormat);

			  SetBkMode(hDC, oldBkMode);
			  SetTextColor(hDC, oldTextColor);
			  if(hOldFont) SelectObject(hDC, hOldFont);

			}
			else if(soCont)
			{ POINT p=NullPoint;
			  int bm;

			  if(BOTOW<RECTW(pos)) { p.x=(RECTW(pos)-BOTOW)/2; dx=BOTOW;}
			  else dx=RECTW(pos);

			  if(BOTOH<RECTH(pos)) { p.y=(RECTH(pos)-BOTOH)/2; dy=BOTOH;}
			  else dy=RECTH(pos);

			  switch(soCont)
			  { case MIDI: bm=ICOMIDI; break;
				 case WAVE: bm=ICOWAVE; break;
				 case CDAUDIO: bm=ICOCD; break;
				 case REC_REC: bm=ICOREC; break;
				 case REC_PLAY: bm=ICORPLAY; break;
				 case USER_CONT: bm=ICOCLIC; break;
				 case EXIT_CLIC: bm=ICOPORTA; break;
				 case EXEC_EXE: bm=ICOEXE; break;
				 default: bm=ICOMCI; break;
			  }

			  displayBitmap(hDC, hMemDC, MAKEINTRESOURCE(bm),
								 pos.left+p.x, pos.top+p.y, 0, 0, dx, dy);
			}

			break;

	 default: break;
  }

}

BOOL Contenidor::lostCaret()
{
  if(!getFlag(OWNSCARET) || !caretVis) return FALSE;
  HideCaret(hWnd);
  DestroyCaret();
  caretVis=FALSE;
  setFlag(OWNSCARET, FALSE);
  caretOwner=NULL;

  return TRUE;
}

BOOL Contenidor::gainCaret()
{
  if(getFlag(OWNSCARET) || caretVis || cont!=TXT || (txt==uch && !getFlag(UNICHAR)))
	 return FALSE;

  setFlag(OWNSCARET, TRUE);
  caretVis=TRUE;
  caretPos=0;

  CreateCaret(hWnd, NULL, caretW, caretH);
  caretX=pos.left+RECTW(pos)/2;
  caretY=pos.top+RECTH(pos)/3;
  SetCaretPos(caretX, caretY);
  invr(TRUE);
  ShowCaret(hWnd);
  caretOwner=this;
  return TRUE;
}

BOOL Contenidor::insertChar(UINT kcode)
{
  char txbuf[MAX_MESS_LEN];
  int l;
  BOOL txch=FALSE;

  if(cont!=TXT || (txt==uch && !getFlag(UNICHAR)) || !getFlag(OWNSCARET) || !caretVis)
	 return FALSE;

  l=strlen(txt);

  if((kcode & 0xFF00)==0)
  { if(l>=(MAX_MESS_LEN-1) ||
		 kcode<' ' ||
		 (kcode>'~' && kcode<'¡')) return FALSE;
	 strcpy(txbuf, txt);
	 txbuf[caretPos]=(char)kcode;
	 strcpy(txbuf+caretPos+1, txt+caretPos);
	 caretPos++;
	 txch=TRUE;
  }
  else switch(kcode & 0xFF)
  { case VK_BACK:
			if(caretPos<1 || l<1) return FALSE;
			strcpy(txbuf, txt);
			strcpy(txbuf+caretPos-1, txt+caretPos);
			caretPos--;
			txch=TRUE;
			break;

	 case VK_RIGHT:
			if(caretPos>=l) return FALSE;
			caretPos++;
			break;

	 case VK_LEFT:
			if(caretPos<1) return FALSE;
			caretPos--;
			break;

	 case VK_HOME:
			caretPos=0;
			break;

	 case VK_END:
			caretPos=l;
			break;

	 default:
			return FALSE;
  }

  if(txch)
  { if(txt!=uch) {delete txt; txt=uch;}
	 setFlag(UNICHAR, FALSE);
	 txt=dupStr(txbuf);
  }
  invr();
  return TRUE;
}


// -----------------------------------
// Implementacio de DigitBox
// -----------------------------------

HBITMAP DigitBox::hDigit=LoadBitmap(_hInstance, MAKEINTRESOURCE(DIGITS));

BOOL DigitBox::create(POINT iniPos, HWND hOwner, BOOL iMark, int ival, BASE_ZONES base)
{
  RECT r;

  val=ival;
  r.left=iniPos.x; r.top=iniPos.y;
  r.right=iniPos.x+3*WDIGIT;
  r.bottom=iniPos.y+HDIGIT;

  return Zona::create(hOwner, &r, FALSE, iMark, base);
}

void DigitBox::update(HDC hDC, RECT */*updRect*/, HDC hMemDC)
{
  int i, j, w, d, valr;
  BOOL k;

  if(!hWnd) return;

  if(getFlag(HID))
  { //FillRect(hDC, (LPRECT)&pos, bc[getBase()].hBackBrush);
	 return;
  }

  valr=(val>999 ? 999:val);


  for(k=FALSE, i=0, j=100; i<3; i++, j/=10)
  { if(getFlag(MARK)) d=1;
	 else
	 { if((w=(valr/j)%10)!=0)
		{ k=TRUE; d=11-w;}
		else d=(k==TRUE || i==2 ? 11:1);
	 }
	 /*
	 displayBitmap(hDC, hMemDC, hDigit,
		  pos.left+WDIGIT*i, pos.top, WDIGIT, HDIGIT,
		  0, d*HDIGIT);
	 */

	 displayBitmapRop3(bc[getBase()].hBackBrush, bc[getBase()].c.textColor,
							 hDC, hMemDC, hDigit,
							 pos.left+WDIGIT*i, pos.top, WDIGIT, HDIGIT,
							 0, d*HDIGIT, 0, 0, BLANCTRANSP);
  }

}

// -----------------------------------
// Implementacio de Fil
// -----------------------------------

Fil::Fil()
{ active=FALSE;
  visible=FALSE;
  hWnd=0;
  hOldCursor=0;
  cont=0;
  dxCont=dyCont=0;
  oldContRect=NullRect;
  currRect=0;
  hBmCont=hFonsCont=0;
  hp=NULL;
  int x=1; setWidth(x);
}

Fil::~Fil()
{
  if(hBmCont) DelObj(hBmCont);
  if(hFonsCont) DelObj(hFonsCont);
  hBmCont=hFonsCont=0;
  if(hp!=NULL) DelObj(hp);
}

HPEN Fil::hp=NULL;

BOOL Fil::setWidth(int &newWidth)
{
  if(newWidth<1) newWidth=1;
  if(newWidth>20) newWidth=20;
  if(hp!=NULL) {DelObj(hp); hp=NULL;}
  hp=CreatePen(PS_SOLID, newWidth, RGB(128, 128, 128));
  return TRUE;
}

void Fil::display(HDC hOrgDC, HDC hMemDC)
{
  HDC hDC;
  HPEN hpenOld;

  if(!active) return;

  hDC=(hOrgDC ? hOrgDC : GetDC(hWnd));
  if(cont)
  { if(visible)
	 { displayBitmap(hDC, hMemDC, hFonsCont,
			 curPosCont.x, curPosCont.y, lxCont, lyCont);
	 }
	 else
	 { hFonsCont=getBmRect(hDC, hMemDC, currRect, hFonsCont);
		displayBitmap(hDC, hMemDC, hBmCont,
			 currRect->left, currRect->top, lxCont, lyCont);
	 }
  }
  else
  { if(hp) hpenOld = (HPEN)SelectObject(hDC, hp);

	 SetROP2(hDC, R2_NOT);
	 MoveTo(hDC, origen.x, origen.y);
	 LineTo(hDC, desti.x, desti.y);

	 if(hp) SelectObject(hDC, hpenOld);
  }

  if(!hOrgDC) ReleaseDC(hWnd, hDC);
  visible^=1;
}

void Fil::init(POINT ini, HWND hParent, HCURSOR hDispCursor, Contenidor *c)
{
  HDC hDC;

  hCursor=(hDispCursor ? hDispCursor:LoadCursor(NULL, IDC_CROSS));

  if(active) end();
  hWnd=hParent;
  origen=desti=ini;
  if(arrossega && c!=0)
  { cont=c;
	 currRect=cont->getRect();
	 oldContRect=*currRect;
	 lxCont=RECTW(oldContRect);
	 lyCont=RECTH(oldContRect);
	 curPosCont.x=oldContRect.left;
	 curPosCont.y=oldContRect.top;
	 dxCont=ini.x-oldContRect.left;
	 dyCont=ini.y-oldContRect.top;

	 if(hBmCont) {DelObj(hBmCont); hBmCont=0;}
	 if(hFonsCont) {DelObj(hFonsCont); hFonsCont=0;}
	 hDC=GetDC(hWnd);
	  hBmCont=getBmRect(hDC, 0, currRect);
	 ReleaseDC(hWnd, hDC);
	 cont->capture(TRUE);
	 InvalidateRect(hWnd, currRect, TRUE);
	 UpdateWindow(hWnd);
	 hDC=GetDC(hWnd);
	  hFonsCont=getBmRect(hDC, 0, currRect);
	 ReleaseDC(hWnd, hDC);
  }
  active=TRUE;
  visible=FALSE;
  show();
  setCursor(hWnd, HANDLE_CURSOR, hCursor);
}

#define LPASSOS 20
void Fil::end(BOOL walk)
{
  if(!active) return;
  if(cont && walk)
  { int i, np;
	 float x, y, dx, dy;
	 HDC hDC, hMemDC;
	 POINT oldDest, tempDest;

	 oldDest=desti;
	 dx=desti.x-origen.x;
	 dy=desti.y-origen.y;
	 np=int((abs(dx)>abs(dy) ? abs(dx):abs(dy))/LPASSOS);
	 if(np)
	 { dx/=np; dy/=np;
		tempDest=oldDest=desti;
		hDC=GetDC(hWnd);
		 hMemDC=CreateCompatibleDC(hDC);
		for(x=tempDest.x, y=tempDest.y, i=0; i<np; i++)
		{ move(tempDest, hDC, hMemDC);
		  tempDest.x=int(x-=dx);
		  tempDest.y=int(y-=dy);
		}
		hide(hDC, hMemDC);
		 DeleteDC(hMemDC);
		ReleaseDC(hWnd, hDC);
		desti=oldDest;
	 }
  }
  else hide();
  active=FALSE;
  resetCursor();
  if(cont)
  { cont->move(&oldContRect);
	 cont->capture(FALSE);
	 cont=0;
	 if(hBmCont) {DelObj(hBmCont); hBmCont=0;}
	 if(hFonsCont) {DelObj(hFonsCont); hFonsCont=0;}
	 currRect=0;
  }
}

void Fil::show(HDC hDC, HDC hMemDC)
{
  if(visible) return;
  display(hDC, hMemDC);
}

void Fil::hide(HDC hDC, HDC hMemDC)
{
  if(!visible) return;
  display(hDC, hMemDC);
}

void Fil::move(POINT nou, HDC hMainDC, HDC hSupMemDC)
{
	HDC hDC, hMemDC;

	if(!active) return;

	hDC=hMainDC ? hMainDC : GetDC(hWnd);
	hMemDC=hSupMemDC ? hSupMemDC : CreateCompatibleDC(hDC);

	if(!cont)
	{ hide(hDC, hMemDC);
	  desti=nou;
	  show(hDC, hMemDC);
	}
	else
	{ HDC hMemoryDC;
	  HBITMAP hMemBitmap, hBuffBitmap, hOldBitmap;
	  RECT newRect, memRect;
	  POINT p;
	  int dxMem, dyMem;

	  SetRect(&newRect, p.x=nou.x-dxCont, p.y=nou.y-dyCont,
				 p.x+lxCont, p.y+lyCont);

	  UnionRect(&memRect, &newRect, currRect);

	  dxMem=RECTW(memRect);
	  dyMem=RECTH(memRect);

	  hMemBitmap=CreateCompatibleBitmap(hDC, dxMem, dyMem);
		hMemoryDC=CreateCompatibleDC(hDC);
		 hOldBitmap=(HBITMAP)SelectObject(hMemoryDC, hMemBitmap);
	 hBuffBitmap=getBmRect(hDC, hMemDC, &memRect, 0);
	 displayBitmap(hMemoryDC, hMemDC, hBuffBitmap,
				 0, 0, dxMem, dyMem);

	 curPosCont.x-=memRect.left;
	 curPosCont.y-=memRect.top;

	 hide(hMemoryDC, hMemDC);
	 desti=nou;
	 curPosCont=p;
	 *currRect=newRect;

	 OffsetRect(currRect, -memRect.left, -memRect.top);
	 show(hMemoryDC, hMemDC);
	 OffsetRect(currRect, memRect.left, memRect.top);

	 SetRect(&newRect, 0, 0, dxMem, dyMem);
	 hBuffBitmap=getBmRect(hMemoryDC, hMemDC, &newRect, hBuffBitmap);
	 displayBitmap(hDC, hMemDC, hBuffBitmap,
				 memRect.left, memRect.top, dxMem, dyMem);
	 DelObj(hBuffBitmap);

		 SelectObject(hMemoryDC, hOldBitmap);
		DeleteDC(hMemoryDC);
	  DelObj(hMemBitmap);
	}

	if(!hSupMemDC) DeleteDC(hMemDC);
	if(!hMainDC) ReleaseDC(hWnd, hDC);
}

