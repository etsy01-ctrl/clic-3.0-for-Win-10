// ---------------------------------------------------
// WGIFFILE.CPP
// Implementacio de loadGifFile
// ---------------------------------------------------

#include "wcl.h"
#include "rcomu\clicres.h"
#include <mem.h>
#include "miscfunc.h"

#define BYTE unsigned char
#define ulong unsigned long

#define BUFFMAXSIZE 272
#define BUFFMINSIZE 4
#define MAXBITS 12

struct RGBV
{ BYTE r;
  BYTE g;
  BYTE b;
};

struct XBITMAPINFO
{ BITMAPINFOHEADER bmiHeader;
  RGBQUAD          bmiColors[256];
} bmi;

enum BITMODE {BW, C16, C256};

struct BUFFPOINTER
{ unsigned row;
  unsigned col;
  unsigned size;
};


class GifData
{
  public:
	 GifData(HFILEFC hffc0, int bitSize0, BYTE huge *bitData0, int widthGraf0, long widthBuff0, int height0, BOOL interlaced0);
	 ~GifData();

	 int decod();

  protected:
	 HFILEFC hffc;
	 BYTE huge *bitData;
	 unsigned CC;
	 unsigned EOI;
	 unsigned rw0;
	 int buffSize;
	 BOOL lastBlock;
	 int  nextBlockSize;
	 BOOL foundEOI;
	 ulong mask;
	 ulong iniMask;
	 ulong *dataBuff;
	 int maskPos;
	 int bitSize;
	 int iniBitSize;
	 unsigned tableSize;
	 unsigned maxTableSize;
	 int widthGraf;
	 long widthBuff;
	 int height;
	 BOOL interlaced;
	 BITMODE bitMode;
	 BUFFPOINTER pstream;
	 unsigned grp1, grp2, grp3;

	 void incrementBitSize();
	 void resetTable();
	 unsigned getUnpackedData();
	 BOOL fillBuffer();
	 unsigned addToStream(BYTE b);
	 unsigned addToStream(BUFFPOINTER p);
	 BYTE getFromStream(BUFFPOINTER p);
	 long getRowOffset(unsigned row);
};


//BYTE buff[300];
BYTE *buff=NULL;
BOOL gifTrailer;

int readGifHeader(HFILEFC hffc);
int readLSD(HFILEFC hffc);
int readBlock(HFILEFC hffc, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh);
int readDataBlocks(HFILEFC hffc);
int readImage(HFILEFC hffc, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh);

int loadDIBFileGIF(char *fileName, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh)
{
  HFILEFC hffc;
  int result=0;

  if(bminf) {delete bminf; bminf=0;}
  if(bits) {GlobalFree(bits); bits=NULL;}

  if((buff=new BYTE[300])==NULL) return IDS_NOMEM;

  gifTrailer=FALSE;

  if(fc_lopen(fileName, hffc)==FALSE)
		result=IDS_ERROPEN;
  else
  { result=readGifHeader(hffc);
	 if(!result)
	 { result= readLSD(hffc);
		while(result==0 && bminf==NULL && gifTrailer==FALSE)
		  result=readBlock(hffc, bminf, bits, bmw, bmh);
		if(bminf==NULL) result=IDS_ERRBMP;
	 }
	 fc_lclose(hffc);
  }

  delete buff; buff=NULL;

  if(result)
  { if(bminf) {delete bminf; bminf=0;}
	 if(bits) {GlobalFree(bits); bits=NULL;}
  }

  return result;
}

int readGifHeader(HFILEFC hffc)
{
	int result=0;
	char v[7];

	if(fc_lread(hffc, (LPSTR)&v, 6)!=6)
	  result=IDS_ERRREAD;
	else
	  if(v[0]!='G' || v[1]!='I' || v[2]!='F' || v[3]!='8' ||
		  (v[4]!='7' && v[4]!='9') ||
		  (v[5]!='a' && v[5]!='b')) result=IDS_ERRBMP;

	return result;
}


int readLSD(HFILEFC hffc)
{
  int result=0;
  int i;
  RGBV *rgb;

  struct LSD
  { int lsw;
	 int lsh;
	 BYTE pf;
	 BYTE bci;
	 BYTE par;
  } lsd;

  int  gcts;

  if(fc_lread(hffc, (LPSTR)&lsd, sizeof(LSD))!=sizeof(LSD))
	 result=IDS_ERRREAD;
  else
  { logPal.palNumEntries=0;
	 if(lsd.pf & 0x80) // hi ha Global Color Table
	 { gcts = 1 << ((lsd.pf & 0x07)+1);
		if((rgb=new RGBV[gcts])!=NULL)
		{ if(fc_lread(hffc, (LPSTR)rgb, gcts*sizeof(RGBV))==gcts*sizeof(RGBV))
			 for(i=0; i<gcts; i++)
			 { bmi.bmiColors[i].rgbRed=/*logPal.palPalEntry[i].peRed=*/rgb[i].r;
				bmi.bmiColors[i].rgbGreen=/*logPal.palPalEntry[i].peGreen=*/rgb[i].g;
				bmi.bmiColors[i].rgbBlue=/*logPal.palPalEntry[i].peBlue=*/rgb[i].b;
			 }
		  else result=IDS_ERRREAD;
		  if(!result) logPal.palNumEntries=gcts;
		  delete rgb;
		}
		else result=IDS_NOMEM;
	 }
  }
  return result;
}


int readBlock(HFILEFC hffc, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh)
{
  BYTE id;
  BYTE cl;
  BOOL result=0;

  if(fc_lread(hffc, (LPSTR)&id, sizeof(id))!=sizeof(id))
	 result=IDS_ERRREAD;
  else switch(id)
  { case 0x21:  // extension
			if(fc_lread(hffc, (LPSTR)&cl, sizeof(cl))!=sizeof(cl))
			  result=IDS_ERRREAD;
			else switch(cl)
			{ case 0xF9: // GRAPHIC CONTROL EXTENSION
			  case 0xFE: // COMMENT EXTENSION
			  case 0x01: // PLAIN TEXT EXTENSION
			  case 0xFF: // APPLICATION EXTENSION
					 break;

			  default:   // UNKNOW EXTENSION
					 result=IDS_ERRBMP; // cal ser tan radicals?
			}

			if(!result) result=readDataBlocks(hffc);
			break;

	 case 0x3B:
			// GIF TRAILER
			gifTrailer=TRUE;
			break;

	 case 0x2C:
			// IMAGE DESCRIPTOR
			result=readImage(hffc, bminf, bits, bmw, bmh);
			break;

	 default:
			// UNKNOW BLOCK
			result=IDS_ERRBMP;
			break;
  }

  return result;
}


int readDataBlocks(HFILEFC hffc)
{
  int result=0;
  BYTE bs;

  while(result==0)
  { if(fc_lread(hffc, (LPSTR)&bs, sizeof(bs))!=sizeof(bs))
		result=IDS_ERRREAD;
	 else
	 { if(bs==0) break;
		if(fc_lread(hffc, (LPSTR)buff, bs)!=bs) result=IDS_ERRREAD;
	 }
  }

  return result;
}


int readImage(HFILEFC hffc, BITMAPINFO *&bminf, HGLOBAL &bits, int &bmw, int &bmh)
{
  int result=0;
  int i;
  long lw, sizeimg;
  RGBV *rgb;

  struct IMGD
  { // byte imgs;  (ja llegit per ReadBlock)
	 int lp;
	 int tp;
	 int width;
	 int height;
	 BYTE pf;
  } imgd;

  BYTE mcs, bpp;
  int  lcts;
  BOOL interlaced;

  if(fc_lread(hffc, (LPSTR)&imgd, sizeof(IMGD))!=sizeof(IMGD))
  { result=IDS_ERRREAD;
	 return result;
  }

  if(imgd.pf & 0x80) // hi ha Local Color Table
  { logPal.palNumEntries=0;
	 lcts = 1 << ((imgd.pf & 0x07)+1);
	 if((rgb=new RGBV[lcts])!=NULL)
	 { if(fc_lread(hffc, (LPSTR)rgb, lcts*sizeof(RGBV))==lcts*sizeof(RGBV))
		  for(i=0; i<lcts; i++)
		  { bmi.bmiColors[i].rgbRed=/*logPal.palPalEntry[i].peRed=*/rgb[i].r;
			 bmi.bmiColors[i].rgbGreen=/*logPal.palPalEntry[i].peGreen=*/rgb[i].g;
			 bmi.bmiColors[i].rgbBlue=/*logPal.palPalEntry[i].peBlue=*/rgb[i].b;
		  }
		else result=IDS_ERRREAD;

		if(!result) logPal.palNumEntries=lcts;
		delete rgb;
	 }
	 else result=IDS_NOMEM;
  }

  if(result) return result;

  interlaced = (imgd.pf & 0x40) ? TRUE : FALSE;
  bmw = imgd.width;
  bmh = imgd.height;
  if(bmw<1 || bmh<1)
  { result=IDS_ERRBMP;
	 return result;
  }

  if(fc_lread(hffc, (LPSTR)&mcs, sizeof(mcs))!=sizeof(mcs) ||
	  (mcs!=2 && mcs!=4 && mcs!=8))
  { result=IDS_ERRBMP;
	 return result;
  }

  bpp=(mcs==2 ? 1 : mcs);
  lw=(((long)(bmw*bpp)+31)/32)*4;
  sizeimg=lw*bmh;

  if((bits=GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, sizeimg))!=0)
  {
	 GifData *gd;
	 BYTE huge *bitdata;

	 bitdata=(BYTE huge *)GlobalLock(bits);

	 gd = new GifData(hffc, (int)mcs, bitdata, bmw, lw, bmh, interlaced);

	 if(gd!=NULL)
	 { if(gd->decod()!=TRUE) result=IDS_ERRBMP;
		delete gd;

		if(!result)
		{ bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		  bmi.bmiHeader.biWidth=bmw;
		  bmi.bmiHeader.biHeight=bmh;
		  bmi.bmiHeader.biPlanes=1;
		  bmi.bmiHeader.biBitCount=bpp;
		  bmi.bmiHeader.biCompression=BI_RGB;
		  bmi.bmiHeader.biSizeImage=0;
		  bmi.bmiHeader.biXPelsPerMeter=0;
		  bmi.bmiHeader.biYPelsPerMeter=0;
		  bmi.bmiHeader.biClrImportant=0;

		  i=sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*logPal.palNumEntries;
		  if((bminf=(BITMAPINFO *)newChar(i))!=NULL)
			 memcpy(bminf, &bmi, i);
		  else result=IDS_NOMEM;
		}
	 }
	 else result=IDS_NOMEM;

	 GlobalUnlock(bits);
  }
  else result=IDS_NOMEM;

  return result;
}

// *******************************************
// Implementacio GifData


GifData::GifData(HFILEFC hffc0, int bitSize0, BYTE huge *bitData0,
					  int widthGraf0, long widthBuff0, int height0, BOOL interlaced0)
{
  int i;

  hffc=hffc0;
  bitData=bitData0;
  widthGraf=widthGraf0;
  widthBuff=widthBuff0;
  height=height0;
  rw0=height-1;
  interlaced=interlaced0;
  bitMode=(bitSize0 == 2 ? BW : (bitSize0 == 4 ? C16 : C256));
  pstream.row=0;
  pstream.col=0;
  pstream.size=0;

  CC=(1<<bitSize0);
  EOI=CC+1;
  bitSize=bitSize0+1;

  dataBuff=(ulong *)buff;
  buffSize=0;

  mask=1L;
  for(i=1; i<bitSize; i++) mask |= (mask<<1);
  maskPos=0;
  lastBlock=FALSE;
  nextBlockSize=0;

  tableSize=EOI+1;
  maxTableSize=(CC<<1);
  iniBitSize=bitSize;
  iniMask=mask;
  foundEOI=FALSE;

  if(interlaced)
  { grp1=(height-1)/8 + 1;
	 grp2=grp1+(height-5)/8 + 1;
	 grp3=grp2+(height-3)/4 + 1;
  }
}

GifData::~GifData()
{
  while(!lastBlock)
  { buffSize=0;
	 fillBuffer();
  }
}

void GifData::incrementBitSize()
{
  if(bitSize>=MAXBITS) return;
  mask|=(mask<<1);
  bitSize++;
  maxTableSize<<=1;
}

void GifData::resetTable()
{
  tableSize=EOI+1;
  bitSize=iniBitSize;
  mask=(iniMask<<maskPos);
  maxTableSize=(CC<<1);
}

unsigned GifData::getUnpackedData()
{
  unsigned d;

  if(foundEOI) return EOI;
  if(buffSize<BUFFMINSIZE && lastBlock==FALSE) fillBuffer();

  if(tableSize==maxTableSize) incrementBitSize();

  d=(unsigned)((*dataBuff & mask)>>maskPos);

  mask<<=bitSize;
  maskPos+=bitSize;

  while(maskPos>7)
  { mask>>=8;
	 maskPos-=8;
	 dataBuff=(ulong *)((BYTE *)dataBuff+1);
	 buffSize--;
  }

  if(d==CC) resetTable();
  else if(d==EOI) foundEOI=TRUE;

  return d;
}

BOOL GifData::fillBuffer()
{
  int i;

  BYTE b;
  BYTE *p;

  if(lastBlock) return FALSE;

  for(i=0, p=(BYTE *)dataBuff; i<buffSize; i++, p++)
	 buff[i]=*p;

  dataBuff=(ulong *)buff;

  if(nextBlockSize==0)
  { if(fc_lread(hffc, (LPSTR)&b, sizeof(b))!=sizeof(b)) return FALSE;
	 if(b==0) {lastBlock=TRUE; return FALSE;}
	 nextBlockSize=b;
  }

  if(fc_lread(hffc, (LPSTR)buff+buffSize, nextBlockSize)!=nextBlockSize) return FALSE;

  buffSize+=nextBlockSize;

  nextBlockSize=0;
  if(fc_lread(hffc, (LPSTR)&b, sizeof(b))!=sizeof(b)) return FALSE;
  if(b==0) lastBlock=TRUE;
  nextBlockSize=b;

  return TRUE;

}


unsigned GifData::addToStream(BYTE b)
{
  BYTE huge *p;

  if(b) switch(bitMode)
  { case BW:
			p=bitData + (getRowOffset(pstream.row) + (pstream.col>>3));
			b=0x80>>(pstream.col & 0x7);
			*p|=b;
			break;

	 case C16:
			p=bitData+ (getRowOffset(pstream.row) + (pstream.col>>1));
			if ((pstream.col & 1)==0) b<<=4;
			*p|=b;
			break;

	 default:
			p=bitData+(getRowOffset(pstream.row) + pstream.col);
			*p=b;
  }

  if((++pstream.col)==widthGraf)
  { pstream.row++;
	 pstream.col=0;
  }

  return 1;
}

long GifData::getRowOffset(unsigned row)
{
  if(interlaced)
  { if(row<grp1) row*=8;
	 else if(row<grp2) row=(row-grp1)*8+4;
	 else if(row<grp3) row=(row-grp2)*4+2;
	 else row=(row-grp3)*2+1;
  }

  return widthBuff*(rw0-row);
}

// rutina optimitzada
unsigned GifData::addToStream(BUFFPOINTER p)
{
  int i;
  BYTE huge *pli;
  BYTE huge *plo;
  BYTE b, mi, mo;

  if(interlaced)
  { switch(bitMode)
	 { case BW:
			  pli=bitData+(getRowOffset(p.row)+(p.col>>3));
			  plo=bitData+(getRowOffset(pstream.row)+(pstream.col>>3));
			  mi=(0x80>>(p.col & 7));
			  mo=(0x80>>(pstream.col & 7));
			  for(i=0; i<p.size; i++)
			  { if((*pli)&mi) *plo|=mo;

				 if(!(mi>>=1)) {pli++; mi=0x80;}
				 if(++p.col==widthGraf)
				 { p.col=0; pli=bitData+getRowOffset(++p.row); mi=0x80;}

				 if(!(mo>>=1)) {plo++; mo=0x80;}
				 if(++pstream.col==widthGraf)
				 { pstream.col=0; plo=bitData+getRowOffset(++pstream.row); mo=0x80;}
			  }
			  break;

		case C16:
			  pli=bitData+(getRowOffset(p.row)+(p.col>>1));
			  plo=bitData+(getRowOffset(pstream.row)+(pstream.col>>1));
			  mi= (p.col & 1) ? 0x0F : 0xF0;
			  mo= (pstream.col&1) ? 0x0F : 0xF0;
			  for(i=0; i<p.size; i++)
			  { if((b=(*pli) & mi)!=0)
				 { if(mo==0xF0)
					{ if(mi==0x0F) b<<=4;}
					else if(mi==0xF0) b>>=4;
					*plo|=b;
				 }

				 if(!(mi>>=4)) {pli++; mi=0xF0;}
				 if(++p.col==widthGraf)
				 { p.col=0; pli=bitData+getRowOffset(++p.row); mi=0xF0;}

				 if(!(mo>>=4)) {plo++; mo=0xF0;}
				 if(++pstream.col==widthGraf)
				 { pstream.col=0; plo=bitData+getRowOffset(++pstream.row); mo=0xF0;}
			  }
			  break;


		  default:
			  pli=bitData+(getRowOffset(p.row)+p.col);
			  plo=bitData+(getRowOffset(pstream.row)+pstream.col);
			  for(i=0; i<p.size; i++)
			  { *plo++=*pli++;

				  if(++p.col==widthGraf)
				  {p.col=0; pli=bitData+getRowOffset(++p.row);}

				  if(++pstream.col==widthGraf)
				  {pstream.col=0; plo=bitData+getRowOffset(++pstream.row);}
			  }
	 }

  }
  else
  { switch(bitMode)
	 { case BW:
			  pli=bitData+((widthBuff*(rw0-p.row))+(p.col>>3));
			  plo=bitData+((widthBuff*(rw0-pstream.row))+(pstream.col>>3));
			  mi=(0x80>>(p.col & 7));
			  mo=(0x80>>(pstream.col & 7));
			  for(i=0; i<p.size; i++)
			  { if((*pli)&mi) *plo|=mo;

				 if(!(mi>>=1)) {pli++; mi=0x80;}
				 if(++p.col==widthGraf)
				 { p.col=0; pli=bitData+(widthBuff*(rw0-(++p.row))); mi=0x80;}

				 if(!(mo>>=1)) {plo++; mo=0x80;}
				 if(++pstream.col==widthGraf)
				 { pstream.col=0; plo=bitData+(widthBuff*(rw0-(++pstream.row))); mo=0x80;}
			  }
			  break;

		case C16:
			  pli=bitData+((widthBuff*(rw0-p.row))+(p.col>>1));
			  plo=bitData+((widthBuff*(rw0-pstream.row))+(pstream.col>>1));
			  mi= (p.col & 1) ? 0x0F : 0xF0;
			  mo= (pstream.col&1) ? 0x0F : 0xF0;
			  for(i=0; i<p.size; i++)
			  { if((b=(*pli) & mi)!=0)
				 { if(mo==0xF0)
					{ if(mi==0x0F) b<<=4;}
					else if(mi==0xF0) b>>=4;
					*plo|=b;
				 }

				 if(!(mi>>=4)) {pli++; mi=0xF0;}
				 if(++p.col==widthGraf)
				 { p.col=0; pli=bitData+(widthBuff*(rw0-(++p.row))); mi=0xF0;}

				 if(!(mo>>=4)) {plo++; mo=0xF0;}
				 if(++pstream.col==widthGraf)
				 { pstream.col=0; plo=bitData+(widthBuff*(rw0-(++pstream.row))); mo=0xF0;}
			  }
			  break;


		  default:
			  pli=bitData+((widthBuff*(rw0-p.row))+p.col);
			  plo=bitData+((widthBuff*(rw0-pstream.row))+pstream.col);
			  for(i=0; i<p.size; i++)
			  { *plo++=*pli++;

				  if(++p.col==widthGraf)
				  {p.col=0; pli=bitData+(widthBuff*(rw0-(++p.row)));}

				  if(++pstream.col==widthGraf)
				  {pstream.col=0; plo=bitData+(widthBuff*(rw0-(++pstream.row)));}
			  }
	 }

  }

  return i;
}


/*
// versio lenta
unsigned GifData::addToStream(BUFFPOINTER p)
{
  int i;

  for(i=0; i<p.size; i++)
  { addToStream(getFromStream(p));
	 if((++p.col)==widthGraf)
	 { p.row++;
		p.col=0;
	 }
  }

  return i;
}
*/

BYTE GifData::getFromStream(BUFFPOINTER bp)
{
  BYTE huge *p;
  BYTE b;
  BYTE m;

  switch(bitMode)
  { case BW:
			p=bitData + (getRowOffset(bp.row) + (bp.col>>3));
			m=bp.col & 7;
			b=(((*p) & (0x80>>m))!=0);
			break;

	 case C16:
			p=bitData+(getRowOffset(bp.row) + (bp.col>>1));
			if(bp.col & 1)
			  b=(*p) & 0xF;
			else
			  b=((*p) & 0xF0)>>4;
			break;


	 default:
			p=bitData+(getRowOffset(bp.row) + bp.col);
			b=*p;
  }

  return b;
}

int GifData::decod()
{
  unsigned code;
  BUFFPOINTER far *table;
  BUFFPOINTER old, ct;
  unsigned n;

  HGLOBAL hg;
  if((hg=GlobalAlloc(GMEM_MOVEABLE, 0x1000 * sizeof(BUFFPOINTER)))==NULL ||
	  (table=(BUFFPOINTER far *)GlobalLock(hg))==NULL)
	 return IDS_NOMEM;


  while(TRUE)
  { code=getUnpackedData();
	 if(code==CC) continue;
	 if(code==EOI) break;
	 old=pstream;
	 addToStream((BYTE)code);
	 old.size=1;

	 while(TRUE)
	 { code=getUnpackedData();
		if(code==EOI || code==CC) break;
		if(code<tableSize)
		{
		  ct.row=pstream.row;
		  ct.col=pstream.col;

		  if(code<CC)
			 n=addToStream((BYTE)code);
		  else
			 n=addToStream(table[code]);
		  old.size++;

		  table[tableSize++]=old;

		  old.row=ct.row;
		  old.col=ct.col;
		  old.size=n;
		}
		else
		{
		  ct.row=pstream.row;
		  ct.col=pstream.col;

		  addToStream(old);
		  addToStream(getFromStream(old));
		  old.row=ct.row;
		  old.col=ct.col;
		  old.size++;

		  table[tableSize++]=old;
		}
	 }
	 if(code==EOI) break;
  }

  GlobalUnlock(hg);
  GlobalFree(hg);

  return (code==EOI);
}

