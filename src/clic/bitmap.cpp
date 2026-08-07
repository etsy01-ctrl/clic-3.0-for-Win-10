// ---------------------------------------------------
// BITMAP.CPP
// ---------------------------------------------------

#include "wcl.h"
#include "rcomu\clicres.h"
#include <mem.h>
#include "miscfunc.h"

// ---------------------------------------------------
// Implementacio de getBitmapFileInfo i loadBitmapFile
// ---------------------------------------------------


#define BITMAPID 0x4D42
#define SZBMFH sizeof(BITMAPFILEHEADER)
#define SZBMINFH sizeof(BITMAPINFOHEADER)
#define SZBMINF SZBMINFH+256*sizeof(RGBQUAD)

struct LOGPALX logPal={0x300, 0};
HPALETTE hPal0=NULL;
BOOL printerBug=FALSE;

BOOL loadImgFile(HWND hWnd, HBITMAP &hBitmap, char *fileName, int &bmw, int &bmh)
{
  BITMAPINFO *bminf=NULL;
  HGLOBAL  bits=NULL;
  int result=0;

  if(hBitmap) {DelObj(hBitmap); hBitmap=0;}

  setCursor(hWnd, CURSOR_ESPERA);
  if((result=loadDIBFile(fileName, bminf, bits, bmw, bmh))==0)
  { result=dibToHbm(hWnd, hBitmap, bminf, bits);
	 if(bminf) delete bminf;
	 if(bits) GlobalFree(bits);
  }
  resetCursor();

  if(result)
  { msgBox(hWnd, IDS_LECT, result, fileName);
	 return FALSE;
  }

  return TRUE;
}


int loadDIBFile(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits,
					 int &bmw, int &bmh)
{
  char *tx=fileName;

  // Comprovar que no sigui un GIF
  while(*tx!=0) tx++;
  switch(*(tx-3))
  { case 'G':
	 case 'g':
			return loadDIBFileGIF(fileName, bminf, bits, bmw, bmh);

	 default:
			break;
  }

  return loadDIBFileBMP(fileName, bminf, bits, bmw, bmh);
}


int loadDIBFileBMP(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits,
						  int &bmw, int &bmh)
{
  HFILEFC hffc;
  static BITMAPFILEHEADER bmfh;
  static BITMAPINFOHEADER bminfh;
  long lw, sizeimg;
  char huge *bitdata;
  int bitc, size, result=0;

  if(bminf) {delete bminf; bminf=0;}
  if(bits) {GlobalFree(bits); bits=NULL;}

  if(fc_lopen(fileName, hffc)==FALSE)
	 return IDS_ERROPEN;
  else
  { if(fc_lread(hffc, (LPSTR)&bmfh, SZBMFH)!= SZBMFH ||
		 bmfh.bfType!=BITMAPID ||
		 fc_lread(hffc, (LPSTR)&bminfh, SZBMINFH)!=SZBMINFH ||
		 bminfh.biSize!=SZBMINFH ||
		 (bitc=bminfh.biBitCount)<1 || bitc>24) result=IDS_ERRFMT;
	 else
	 { if((size=(int)bmfh.bfOffBits-SZBMFH)<SZBMINFH ||
			(bminf=(BITMAPINFO *)newChar(size))==0 ||
			memcpy(bminf, &bminfh, SZBMINFH)==0 ||
			(size-=SZBMINFH)<0 ||
			(size>0 && fc_lread(hffc,(LPSTR)bminf->bmiColors, size)!=size))
		result=IDS_NOMEM;
	 }
  }

  if(result==0)
  { bmw=(int)bminf->bmiHeader.biWidth;
	 bmh=(int)bminf->bmiHeader.biHeight;
	 lw=(((long)(bmw*bminf->bmiHeader.biBitCount)+31)/32)*4;
	 sizeimg=bminf->bmiHeader.biSizeImage=lw*bmh;

	 if((bits=GlobalAlloc(GHND, sizeimg))!=0)
	 { bitdata=(char huge *)GlobalLock(bits);
		if(fc_hread(hffc, bitdata, sizeimg)!=sizeimg) result=IDS_ERRREAD;
		GlobalUnlock(bits);
	 } else result=IDS_NOMEM;
  }

  fc_lclose(hffc);

  if(result)
  { if(bminf) {delete bminf; bminf=0;}
	 if(bits) {GlobalFree(bits); bits=NULL;}
  }

  return result;
}


int dibToHbm(HWND hWnd, HBITMAP &hBitmap, BITMAPINFO *bminf, HGLOBAL bits)
{
  HPALETTE hPal;
  HDC hDc;
  int  i, result=0;
  char huge *bitdata;

  if(hBitmap) {DelObj(hBitmap); hBitmap=0;}

  logPal.palVersion=0x300;
  if((logPal.palNumEntries=(1<<bminf->bmiHeader.biBitCount))>256)
	 logPal.palNumEntries=256;

  for(i=0; i<logPal.palNumEntries; i++)
  { logPal.palPalEntry[i].peRed=bminf->bmiColors[i].rgbRed;
	 logPal.palPalEntry[i].peGreen=bminf->bmiColors[i].rgbGreen;
	 logPal.palPalEntry[i].peBlue=bminf->bmiColors[i].rgbBlue;
	 logPal.palPalEntry[i].peFlags=0;
  }

  if((hDc=GetDC(hWnd))!=0)
  {
	 if(//logPal.palNumEntries>16 &&
		 (hPal=CreatePalette((LPLOGPALETTE) &logPal))!=0)
	 { SetSystemPaletteUse(hDc, SYSPAL_STATIC);
		if(SelectPalette(hDc, hPal, 0)!=0)
		{ RealizePalette(hDc);
		  if (hPal0!=NULL) DelObj(hPal0);
		  hPal0=hPal;
		}
		else DelObj(hPal);
	 }

	 bitdata=(char huge *)GlobalLock(bits);
	 hBitmap=CreateDIBitmap(hDc, (LPBITMAPINFOHEADER)&(bminf->bmiHeader),
									  CBM_INIT, (LPSTR)bitdata, (LPBITMAPINFO)bminf,
									  DIB_RGB_COLORS);
	 if(hBitmap==0) result=IDS_ERRBMP;
	 GlobalUnlock(bits);

	 ReleaseDC(hWnd, hDc);
  }

  if(hBitmap) SetBitmapDimension(hBitmap, (int)bminf->bmiHeader.biWidth,
														(int)bminf->bmiHeader.biHeight);
  else result=IDS_ERRBMP;

  return result;
}

// -------------------------------
// Implementacio de displayBitmap
// -------------------------------

BOOL displayBitmapRop3(HBRUSH hFonsBrush, COLORREF tinta,
			HDC hDC, HDC hMemDC, LPCSTR name, int x, int y,
			int xs, int ys, int lx, int ly, DWORD rop)
{
  BOOL result=FALSE;
  HBITMAP hBitmap;

  if((hBitmap=LoadBitmap(_hInstance, name))!=0)
  { result=displayBitmapRop3(hFonsBrush, tinta, hDC, hMemDC, hBitmap,
									  x, y, lx, ly, xs, ys, 0, 0, rop);
	 DelObj(hBitmap);
  }
  return result;
}

BOOL displayBitmap(HDC hDC, HDC hMemDC, LPCSTR name, int x, int y,
			int xs, int ys, int lx, int ly, DWORD rop)
{
  BOOL result=FALSE;
  HBITMAP hBitmap;

  if((hBitmap=LoadBitmap(_hInstance, name))!=0)
  { result=displayBitmap(hDC, hMemDC, hBitmap,
			 x, y, lx, ly,
			 xs, ys, 0, 0, rop);
	 DelObj(hBitmap);
  }
  return result;
}


BOOL displayBitmapRop3(HBRUSH hFonsBrush, COLORREF tinta,
							  HDC hDC, HDC hMemDC, HBITMAP hBitmap,
							  int xd, int yd, int lx, int ly,
							  int xs, int ys, int lxs, int lys,
							  DWORD rop)
{
  HBRUSH hBr, hOldBr;
  RECT r;

  r.left=xd; r.top=yd; r.right=xd+lx; r.bottom=yd+ly;

  FillRect(hDC, &r, hFonsBrush);
  hBr=CreateSolidBrush(tinta);
  hOldBr=(HBRUSH)SelectObject(hDC, hBr);

  displayBitmap(hDC, hMemDC, hBitmap, xd, yd, lx, ly, xs, ys, lxs, lys, rop);

  SelectObject(hDC, hOldBr);
  DelObj(hBr);
  return TRUE;
}

BOOL displayBitmap(HDC hDC, HDC hMemDC, HBITMAP hBitmap,
			int xd, int yd, int lx, int ly,
			int xs, int ys, int lxs, int lys, DWORD rop)
{
  int result=FALSE;
  HBITMAP hOldBitmap;
  static BITMAP bitmap;
  HDC hMemoryDC;

  if(hBitmap==0) return FALSE;

  if(GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap)==0) return FALSE;
  if(lx==0 || ly==0) {lx=bitmap.bmWidth; ly=bitmap.bmHeight;}

  if(GetDeviceCaps(hDC, TECHNOLOGY) & DT_RASDISPLAY /*DT_PLOTTER*/ )
  { if((hMemoryDC=hMemDC)!=0 || (hMemoryDC=CreateCompatibleDC(hDC))!=0)
	 { if((hOldBitmap=(HBITMAP)SelectObject(hMemoryDC, hBitmap))!=0)
		{ if(lxs==0 || lys==0 || (lxs==lx && lys==ly))
			 BitBlt(hDC, xd, yd, lx, ly, hMemoryDC, xs, ys, rop);
		  else
			 StretchBlt(hDC, xd, yd, lx, ly, hMemoryDC, xs, ys, lxs, lys, rop);

		  SelectObject(hMemoryDC, hOldBitmap);
		  result=TRUE;
		}
	 }
	 if(!hMemDC) DeleteDC(hMemoryDC);
  }
  else
  { HGLOBAL bits=NULL;
	 BITMAPINFO *bminf=NULL;
	 void far *lpbits;
	 int i, bmw, bmh, oldStrchMode;
	 BOOL freeData=FALSE;

	 if(!bmpCont::getDIB(hBitmap, bminf, bits, bmw, bmh))
	 { for(i=0; i<STRETCH_TABLE_SIZE; i++)
		  if(strt[i].hBmStretch==hBitmap) break;

		if(i<STRETCH_TABLE_SIZE &&
			bmpCont::getDIB(strt[i].hBmOriginal, bminf, bits, bmw, bmh))
		{ if(lxs==0) lxs=strt[i].newBmW;
		  if(lys==0) lys=strt[i].newBmH;

		  //teoricament seria strt[i].orgBmX en comptes de bmx, pero aixi funciona
		  lxs=(int)((1L*lxs*bmw)/strt[i].newBmW);
		  lys=(int)((1L*lys*bmh)/strt[i].newBmH);
		  xs=(int)((1L*xs*bmw)/strt[i].newBmW);
		  ys=(int)((1L*ys*bmh)/strt[i].newBmH);
		}
		else
		{ HDC hDCscreen;
		  int sl;
		  long sizeImage;

		  freeData=TRUE;
		  bmw=bitmap.bmWidth;
		  bmh=bitmap.bmHeight;
		  hDCscreen=GetDC(genMsgOwn);

		  if((bminf=(BITMAPINFO*)newChar(SZBMINF))!=NULL)
		  { memset(bminf, 0, SZBMINF);
			 bminf->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			 sl=GetDIBits(hDCscreen, hBitmap, 0, bmh, NULL, bminf, DIB_RGB_COLORS);
			 bmw=(int)bminf->bmiHeader.biWidth;
			 bmh=(int)bminf->bmiHeader.biHeight;
			 if((sizeImage=bminf->bmiHeader.biSizeImage)==0)
				sizeImage=1L*bitmap.bmWidthBytes*bmh;
			 if(sl!=0 &&
				 (bits=GlobalAlloc(GHND, sizeImage))!=NULL &&
				 (lpbits=GlobalLock(bits))!=NULL)
			 {
				sl=GetDIBits(hDCscreen, hBitmap, 0, bmh, lpbits, bminf, DIB_RGB_COLORS);
				GlobalUnlock(bits);
			 }
		  }
		  ReleaseDC(genMsgOwn, hDCscreen);
		}
	 }

	 if(bits!=NULL && bminf!=NULL && (lpbits=GlobalLock(bits))!=NULL)
	 { //if(lxs==0) lxs=bmw;
		//if(lys==0) lys=bmh;
		if(lxs==0) lxs=lx;
		if(lys==0) lys=ly;
		if(lxs>bmw) lxs=bmw;
		if(lys>bmh) lys=bmh;
		if(lx>bmw) lx=bmw;
		if(ly>bmh) ly=bmh;

		if(!printerBug) ys=bmh-ys-lys;

		oldStrchMode=SetStretchBltMode(hDC, COLORONCOLOR);
		StretchDIBits(hDC, xd, yd, lx, ly, xs, ys, lxs, lys,
						  lpbits, bminf, DIB_RGB_COLORS, rop);
		SetStretchBltMode(hDC, oldStrchMode);
		GlobalUnlock(bits);
		result=TRUE;
	 }
	 if(freeData)
	 { if(bits) GlobalFree(bits);
		if(bminf) delete bminf;
	 }
  }

  return result;
}

// ------------------------------------------
// Implementacio de getBmRect
// ------------------------------------------

HBITMAP getBmRect(HDC hDC, HDC hMemDC, RECT *r, HBITMAP hOrgBm)
{
  HDC hDestiDC;
  HBITMAP hNewBitmap, hOldBitmapDesti;

  int dx, dy;

  dx=r->right-r->left;
  dy=r->bottom-r->top;
  hNewBitmap=hOrgBm;

  if((hDestiDC=hMemDC)!=0 || (hDestiDC=CreateCompatibleDC(hDC))!=0)
  { if((hNewBitmap=hOrgBm)!=0 ||
       (hNewBitmap=CreateCompatibleBitmap(hDC, dx, dy))!=0)
	 { hOldBitmapDesti=(HBITMAP)SelectObject(hDestiDC, hNewBitmap);
      BitBlt(hDestiDC, 0, 0, dx, dy, hDC, r->left, r->top, SRCCOPY);
		SelectObject(hDestiDC, hOldBitmapDesti);
    }
    else NOMEM;
    if(!hMemDC) DeleteDC(hDestiDC);
  }
  else NOMEM;

  return hNewBitmap;
}

// ---------------------------------
// Implementacio de createNewBitmap
// ---------------------------------

HBITMAP createNewBitmap(HWND hWnd, HBITMAP hBitmap,
			int nlx, int nly, int lx, int ly, BOOL captCurs)
{
  static BITMAP Bitmap;
  HBITMAP hNewBitmap, hOldBitmapOrigen, hOldBitmapDesti;
  HDC hDC, hOrigenDC, hDestiDC;
  int oldStrchMode;

  hNewBitmap=NULL;

  if(lx==0 || ly==0)
  { if(GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap))
    {lx=Bitmap.bmWidth; ly=Bitmap.bmHeight;}
	 else return hNewBitmap;
  }

  if(captCurs) setCursor(hWnd, CURSOR_ESPERA);
  if((hDC=GetDC(hWnd))!=NULL)
  { if((hOrigenDC=CreateCompatibleDC(hDC))!=NULL)
	 { hOldBitmapOrigen=(HBITMAP)SelectObject(hOrigenDC, hBitmap);
		if((hDestiDC=CreateCompatibleDC(hDC))!=NULL)
		{ if((hNewBitmap=CreateCompatibleBitmap(hOrigenDC, lx, ly))!=NULL)
		  { hOldBitmapDesti=(HBITMAP)SelectObject(hDestiDC, hNewBitmap);
			 oldStrchMode=SetStretchBltMode(hDestiDC, COLORONCOLOR);
			 StretchBlt(hDestiDC, 0, 0, nlx, nly, hOrigenDC, 0, 0, lx, ly, SRCCOPY);
			 SetStretchBltMode(hDestiDC, oldStrchMode);
			 SelectObject(hDestiDC, hOldBitmapDesti);
		  }
		  else NOMEM;
		  DeleteDC(hDestiDC);
		}
		SelectObject(hOrigenDC, hOldBitmapOrigen);
		DeleteDC(hOrigenDC);
	 }
	 ReleaseDC(hWnd, hDC);
  }
  if(captCurs) resetCursor();
  return hNewBitmap;
}


BOOL TileBmp(HDC hDC, HDC hMemDC, HBITMAP hBm, int w, int h, POINT origen, RECT dest)
{
  RECT r1, r2, r3;

  if(w==0 || h==0) return FALSE;
  r1.left=origen.x+w*((dest.left-origen.x)/w);
  r1.top=origen.y+h*((dest.top-origen.y)/h);
  r1.right=r1.left+w;
  r1.bottom=r1.top+h;

  while(r1.top<=dest.bottom)
  { r2=r1;
	 while(r2.left<=dest.right)
	 { if(IntersectRect(&r3, &r2, &dest))
		  displayBitmap(hDC, hMemDC, hBm, r2.left, r2.top, w, h);
		OffsetRect(&r2, w, 0);
	 }
	 OffsetRect(&r1, 0, h);
  }

  return TRUE;
}


BOOL drawLineRect(HDC hDC, LPRECT r)
{
  POINT aPoints[5];

  aPoints[0].x=r->left;
  aPoints[0].y=r->top;

  aPoints[1].x=r->right;
  aPoints[1].y=r->top;

  aPoints[2].x=r->right;
  aPoints[2].y=r->bottom;

  aPoints[3].x=r->left;
  aPoints[3].y=r->bottom;

  aPoints[4]=aPoints[0];

  return Polyline(hDC, aPoints, 5);
}


// NOVA TECNICA PER PALETES

HBITMAP LoadResourceBitmap(HINSTANCE hInstance, const char far* lpString, HPALETTE FAR* lphPalette)
{
	HRSRC  hRsrc;
	HGLOBAL hGlobal;
	HBITMAP hBitmapFinal = NULL;
	LPBITMAPINFOHEADER  lpbi;
	HDC hdc;
	int iNumColors;
	char huge *bi;

	if((hRsrc = FindResource(hInstance, lpString, RT_BITMAP))!=NULL)
		{
		hGlobal = LoadResource(hInstance, hRsrc);
		lpbi = (LPBITMAPINFOHEADER)LockResource(hGlobal);

		hdc = GetDC(NULL);
		*lphPalette =  CreateDIBPalette ((LPBITMAPINFO)lpbi, &iNumColors);
		if (*lphPalette)
			{
			SelectPalette(hdc,*lphPalette,FALSE);
			RealizePalette(hdc);
			}

	 bi=(LPSTR)lpbi;
	 bi+=lpbi->biSize;
	 bi+=iNumColors * sizeof(RGBQUAD);

	 hBitmapFinal = CreateDIBitmap(hdc,
					 (LPBITMAPINFOHEADER)lpbi,
					 (LONG)CBM_INIT,
					 //(LPSTR)lpbi + lpbi->biSize + iNumColors * sizeof(RGBQUAD),
					 bi,

					 (LPBITMAPINFO)lpbi,
					 DIB_RGB_COLORS );

	 ReleaseDC(NULL,hdc);
	 UnlockResource(hGlobal);
	 FreeResource(hGlobal);
	 }
	 return (hBitmapFinal);
}

HPALETTE CreateDIBPalette (LPBITMAPINFO lpbmi, LPINT lpiNumColors)
{
	LPBITMAPINFOHEADER  lpbi;
	LPLOGPALETTE     lpPal;
	HANDLE           hLogPal;
	HPALETTE         hPal = NULL;
	int              i;

	lpbi = (LPBITMAPINFOHEADER)lpbmi;
	if (lpbi->biBitCount <= 8)

		 *lpiNumColors = (1 << lpbi->biBitCount);

	else

		 *lpiNumColors = 0;  // No palette needed for 24 BPP DIB

	if (lpbi->biClrUsed > 0)

		 *lpiNumColors = (int)lpbi->biClrUsed;  // Use biClrUsed

	if (*lpiNumColors)

		{
		hLogPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
									  sizeof (PALETTEENTRY) * (*lpiNumColors));
		lpPal = (LPLOGPALETTE) GlobalLock (hLogPal);
		lpPal->palVersion    = 0x300;
		lpPal->palNumEntries = *lpiNumColors;

		for (i = 0;  i < *lpiNumColors;  i++)
			{
			lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
			lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
			lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
			lpPal->palPalEntry[i].peFlags = 0;
			}
		hPal = CreatePalette (lpPal);
		GlobalUnlock (hLogPal);
		GlobalFree   (hLogPal);

	}
	return hPal;
}

