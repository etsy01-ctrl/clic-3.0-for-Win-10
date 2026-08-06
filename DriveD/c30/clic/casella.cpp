// -----------------------------------
// CASELLA.CPP
//
// Implementacio de les classes:
//   - Casella
//   - Graella
//   - Fil
// -----------------------------------

#include "casella.h"
#include "miscfunc.h"
#include "wcl.h"
#include <stdlib.h>
#include <string.h>

// -----------------------------------
// Implementacio de Casella
// -----------------------------------

BOOL Casella::create(int ident, HWND hOwner, RECT *rpos,
							BOOL iDl, BOOL iHid, BOOL iMark, BASE_ZONES Base)
{ BOOL r;
  r=Contenidor::create(hOwner, rpos, iHid, iMark, Base);
  setFlag(DL, iDl);
  id=ident;
  return r;
}

void Casella::update(HDC hDC, RECT *updRect, HDC hMemDC)
{
  if(!hWnd) return;
  Contenidor::update(hDC, updRect, hMemDC);
  if(getFlag(DL) && !getFlag(MARK) && !getFlag(CAPTURED))
	 DrawFocusRect(hDC, (LPRECT)&pos);
}

BOOL Casella::operator ==(const Casella& c1)
{ return id==c1.id;}

BOOL Casella::operator >=(const Casella& c1)
{
  if(&c1==NULL) return FALSE;

  switch(cont)
  { case TXT:
			if(strcmp(txt, c1.txt)) return FALSE;
			break;

	 case BMP:
			if(hTxBm==0 || c1.hTxBm==0 || hBm!=c1.hBm) return FALSE;
			break;

	 default:
			return FALSE;
  }

  if((soCont!=CAP_SO || c1.soCont!=CAP_SO) &&
	  (soCont==CAP_SO || c1.soCont==CAP_SO ||
		fnSoCont==0 || c1.fnSoCont==0 ||
		strcmp(fnSoCont, c1.fnSoCont)!=0)) return FALSE;

  return TRUE;
}

Casella& Casella::operator = (Casella& c1)
{
  switch(c1.getCont())
  { case RES:
			clear();
			break;

	 case TXT:
			getData(c1.txt);
			break;

	 case BMP:
			getData(c1.hBm, &(RECT)c1.rbm, &(POINT)c1.origin);
			break;
  }

  id=c1.id;
  flags=c1.flags;
  soCont=c1.soCont;
  if(fnSoCont) {delete fnSoCont; fnSoCont=0;}
  if(c1.fnSoCont) fnSoCont=dupStr(c1.fnSoCont);

  return *this;
}

Casella& Casella::operator >>(Casella& c1)
{
  static Casella c0;

  c0=c1;
  c1=*this;
  *this=c0;
  return *this;
}

// -----------------------------------
// Implementacio de Graella
// -----------------------------------

static char *nultxt="";

Graella::Graella()
{
  ncw=0; nch=0; ncas=0;
  casw=0; cash=0;
  cas=0; casX=0;
  casXx=casXy=0;
  setFlag(DL, TRUE);
  hBmp0=NULL; hBmpAlt=NULL;
  txt0=NULL; txtAlt=NULL;
}

BOOL Graella::create(int numCasW, int numCasH, HWND hOwner, RECT *rpos,
			  BOOL iDl, BOOL iHid, BOOL iMark, BASE_ZONES Base)
{
  int i, x, y;
  RECT tr;

  destroy();
  Zona::create(hOwner, rpos, iHid, iMark, Base);
  if(!hWnd) return FALSE;

  setFlag(DL, iDl);
  ncw=numCasW;
  nch=numCasH;
  ncas=ncw*nch;
  if(ncas==0) {destroy(); return FALSE;}

  casw=RECTW(pos)/ncw;
  cash=RECTH(pos)/nch;

  if((cas=new Casella[ncas])==0) {ncas=0; destroy(); NOMEM; return FALSE;}
  casX=cas; casXx=casXy=0;

  tr.left=pos.left; tr.top=pos.top;
  tr.right=pos.left+casw; tr.bottom=pos.top+cash;

  for(i=0, y=0; y<nch; y++)
  { tr.left=pos.left; tr.right=pos.left+casw;
	 for(x=0; x<ncw; x++, i++)
	 { cas[i].create(i, hWnd, &tr, getFlag(DL), getFlag(HID), getFlag(MARK), Base);
		tr.left+=casw; tr.right+=casw;
	 }
	 tr.top+=cash; tr.bottom+=cash;
  }
  srand((unsigned)GetTickCount());
  return TRUE;
}

void Graella::destroy()
{
  int i;

  if(cas && ncas)
  { for(i=1; i<ncas; i++)
		cas[i].Casella::~Casella();
	 delete [] cas;
  }
  cas=0; casX=0; casXx=casXy=0;
  ncw=0; nch=0; ncas=0;
  casw=0; cash=0;
  hBmp0=NULL; hBmpAlt=NULL;
  txt0=NULL; txtAlt=NULL;
  Zona::destroy();
}

void Graella::update(HDC hDC, RECT *updRect, HDC hMemDC)
{
  int i;

  if(!hWnd || (updRect!=0 && !isIn(updRect))) return;
  if(ncas==0) Zona::update(hDC, updRect);
  else
	 for(i=0; i<ncas; i++)
		if((updRect==0 || cas[i].isIn(updRect)) && !cas[i].isCaptured())
	cas[i].update(hDC, updRect, hMemDC);

  if (Zona::fontReduced==getBase())
  { invr();
	 Zona::fontReduced=-1;
  }
}

BOOL Graella::locateCursorHere(int &rp, int skipHides)
{
  int rp0;
  if(!cas) return FALSE;
  if(rp>=ncas) rp=0;
  rp0=rp;
  switch(skipHides)
  { case 1:
			while(cas[rp].isHide())
			{ rp++; if(rp>=ncas) rp=0;
			  if(rp==rp0) break;
			}
			break;

	 case -1:
			while(!cas[rp].isHide())
			{ rp++; if(rp>=ncas) rp=0;
			  if(rp==rp0) break;
			}
			break;

	 default:
			 break;
  }
  cas[rp].locateCursorHere();
  return TRUE;
}

void Graella::move(int dx, int dy)
{
  int i;

  if(!hWnd) return;
  if(ncas) for(i=0; i<ncas; i++) cas[i].move(dx, dy);
  Zona::move(dx, dy);
}

BOOL Graella::getData(HBITMAP hBitmap, RECT *iRect,
				int firstCas, int firstId, int gdCw, int gdCh)
{
  RECT tr;
  int i, j, x, y, x0, y0, dx, dy;

  if(!hWnd || ncas<1) return FALSE;

  hBmp0=hBitmap;
  iRect0=iRect;
  firstCas0=firstCas;
  firstId0=firstId;
  gdCw0=gdCw;
  gdCh0=gdCh;

  if(!gdCw) gdCw=ncw;
  if(!gdCh) gdCh=nch;
  if(!iRect)
  { x0=0; y0=0;
	 dx=casw; dy=cash;
  }
  else
  { x0=iRect->left; y0=iRect->top;
	 dx=(iRect->right-x0)/gdCw; dy=(iRect->bottom-y0)/gdCh;
  }

  tr.left=x0;
  tr.right=x0+dx;
  tr.top=y0;
  tr.bottom=y0+dy;

  for(i=firstId, j=firstCas, y=0; y<gdCh && j<ncas; y++)
  { tr.left=x0; tr.right=x0+dx;
	 for(x=0; x<gdCw && j<ncas; x++, i++, j++)
	 { cas[j].getData(hBitmap, &tr);
		cas[j].setId(i);
		cas[j].hide(getFlag(HID));
		cas[j].marked(getFlag(MARK));
		tr.left+=dx; tr.right+=dx;
	 }
	 tr.top+=dy; tr.bottom+=dy;
  }

  return TRUE;
}

BOOL Graella::getData(char **txt, int firstCas, int firstId, int gdNc)
{
  int i, j, k;
  BOOL passat;

  if(!hWnd || ncas<1 || firstCas>=ncas) return FALSE;

  txt0=txt;
  firstCas0=firstCas;
  firstId0=firstId;
  gdNc0=gdNc;

  if(!gdNc) gdNc=ncas;

  for(passat=FALSE, i=firstId, j=firstCas, k=0;
		j<(firstCas+gdNc) && j<ncas;
		i++, j++, k++)
  { if(!passat && txt[k]==0) passat=TRUE;
	 cas[j].getData(passat ? nultxt:txt[k]);
	 cas[j].setId(i);
	 cas[j].hide(getFlag(HID));
	 cas[j].marked(getFlag(MARK));
	 cas[j].setDl(getFlag(DL));
  }
  return TRUE;
}

BOOL Graella::resetData()
{
  altCont(FALSE);
  if(hBmp0) return getData(hBmp0, iRect0, firstCas0, firstId0, gdCw0, gdCh0);
  if(txt0) return getData(txt0, firstCas0, firstId0, gdNc0);
  return FALSE;
}


BOOL Graella::setAltData(Casella *c)
{
  RECT tr;
  int i, idpos, x, y;

  if(!c || (!txtAlt && !hBmpAlt)) return FALSE;

  idpos=c->getId();
  if(txtAlt)
  { for(i=0; i<idpos; i++) if(txtAlt[i]==0) break;
	 c->getData(txtAlt[i] ? txtAlt[i]:nultxt);
  }
  else
  { y=idpos/ncw; x=idpos-(y*ncw);
	 tr.left=x*casw; tr.top=y*cash;
	 tr.right=tr.left+casw; tr.bottom=tr.top+cash;
	 c->getData(hBmpAlt, &tr);
  }
  c->altCont(TRUE);

  return TRUE;
}

void Graella::hide(BOOL mode)
{
  int i;

  if(!hWnd) return;
  for(i=0; i<ncas; i++) cas[i].hide(mode);
}

void Graella::altCont(BOOL mode)
{
  int i;

  if(!hWnd) return;
  for(i=0; i<ncas; i++) cas[i].altCont(mode);
}

void Graella::marked(BOOL mode)
{
  int i;

  if(!hWnd) return;
  for(i=0; i<ncas; i++) cas[i].marked(mode);
}

BOOL Graella::isHide(int &encerts)
{
  int i;

  for(i=0, encerts=0; i<ncas; i++) if(cas[i].isHide()) encerts++;
  return (encerts==ncas ? TRUE:FALSE);
}

BOOL Graella::isNotHide(int &encerts)
{
  int i;

  for(i=0, encerts=0; i<ncas; i++) if(!cas[i].isHide()) encerts++;
  return (encerts==ncas ? TRUE:FALSE);
}

BOOL Graella::isMarked(int &encerts)
{
  int i;

  for(i=0, encerts=0; i<ncas; i++) if(cas[i].isMarked()==TRUE) encerts++;
  return (encerts==ncas ? TRUE:FALSE);
}

BOOL Graella::checkIds(int &encerts)
{
  int i;

  for(i=0, encerts=0; i<ncas; i++) if(cas[i].getId()==i) encerts++;
  return (encerts==ncas ? TRUE:FALSE);
}

BOOL Graella::hideSelective(char c)
{
  int i;
  for(i=0; i<ncas; i++) cas[i].hide(cas[i].getChar()==c);
  return TRUE;
}

Casella *Graella::barreja(int vegades)
{
  int i;

  if(!hWnd || !cas || ncas<2) return 0;
  for(i=0; i<vegades; i++) cas[random(ncas)]>>cas[random(ncas)];
  return(checkIds() ? barreja(vegades):cas+random(ncas));
}

Casella *Graella::barrejaVeiBuit(int vegades)
{
  int i, j;
  Casella *cas0;

  if(!cas || ncas<2) return 0;
  for(j=0; j<vegades; j++)
  { do i=random(ncas); while ((cas0=veiBuit(i))==0);
	 *cas0>>cas[i];
  }
  return cas+i;
}

Casella *Graella::veiBuit(int idpos)
{
  int i, j, x, y;
  Casella *c;

  if(!cas || (c=getCas(idpos))==0 || c->isHide()) return 0;

  y=idpos/ncw;
  x=idpos-y*ncw;

  for(i=0; i<2; i++)
	for(j=-1; j<2; j+=2)
	  if((c=getCas((i ? x:x+j), (i ? y+j:y)))!=0 && c->isHide()) return c;

  return 0;
}

Casella *Graella::localitza(POINT p, int &idpos)
{
  for(idpos=0; idpos<ncas; idpos++) if(cas[idpos].isIn(p)) break;
  return (idpos==ncas ? 0:cas+idpos);
}

Casella *Graella::localitza(POINT p, int &x, int &y)
{
  Casella *cas0;
  int idpos;

  if((cas0=localitza(p, idpos))!=0)
  {y=idpos/ncw; x=idpos-y*ncw;}
  return cas0;
}

Casella *Graella::localitza(int id, int &idpos)
{
  for(idpos=0; idpos<ncas; idpos++) if(cas[idpos].getId()==id) break;
  return(idpos==ncas ? 0:cas+idpos);
}

Casella *Graella::getCas(int idpos)
{
  if(!cas) return 0;
  return (idpos<0 || idpos>=ncas) ? 0:cas+idpos;
}

Casella *Graella::getCas(int x, int y)
{
  if(x>=ncw || y>=nch || x<0 || y<0) return 0;
  return cas+x+y*ncw;
}

Casella *Graella::getRndCas()
{
  if(!cas) return 0;
  return getCas(random(ncas));
}

Casella *Graella::despl(int vkCode)
{
  if(!casX) return 0;
  switch(vkCode)
  { case VK_UP:
			if(--casXy<0)
			{ casXy=nch-1;
			  if(--casXx<0) casXx=ncw-1;
			}
			break;

	 case VK_DOWN:
			if(++casXy>=nch)
			{ casXy=0;
			  if(++casXx>=ncw) casXx=0;
			}
			break;

	 case VK_HOME:
			casXx=0;
			break;

	 case VK_END:
			casXx=ncw-1;
			break;

	 case VK_LEFT:
	 case VK_BACK:
			if(--casXx<0)
			{ casXx=ncw-1;
			  if(--casXy<0) casXy=nch-1;
			}
			break;

	 case VK_RIGHT:
			if(++casXx>=ncw)
			{ casXx=0;
			  if(++casXy>=nch) casXy=0;
			}
			break;

	 default: break;
  }

  return(casX=getCas(casXx, casXy));
}

BOOL Graella::setCasX(Casella *newCasX)
{
  int idpos;

  if(newCasX==NULL) return FALSE;

  for(idpos=0; idpos<ncas; idpos++) if(cas+idpos==newCasX) break;
  if(idpos==ncas) return FALSE;

  casXy=idpos/ncw;
  casXx=idpos-casXy*ncw;
  casX=newCasX;

  return TRUE;
}

Casella *Graella::situa(POINT p)
{
  if((casX=localitza(p, casXx, casXy))==0) casX=getCas(casXx, casXy);
  return casX;
}

