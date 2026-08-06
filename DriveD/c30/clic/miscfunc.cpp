// ------------------------------------------
// MISCFUNC.CPP
// Funcions miscel.lanies
// ------------------------------------------

#include "miscfunc.h"
#include "clic.h"
#include "rcomu\clicres.h"
#include "wcl.h"

#include <string.h>
#include <dir.h>
#include <stdio.h>
#include <dos.h>

#include <alloc.h>


char *nullStr="";
char *spcStr=" ";

#ifdef DEBUG
BOOL DeleteObjectDebug(HGDIOBJ hgdiobj)
{
  BOOL result;

  if((result=DeleteObject(hgdiobj))==FALSE)
	 result=FALSE;

  return result;
}
#endif

GlobalVars::GlobalVars()
{
  clicPath=NULL;
  ediText=NULL;
  ediGraf=NULL;
  originalPath=NULL;
  playerId=NULL;
  iniFile=NULL;
  dbFile=NULL;
  originalDbFile=NULL;
  memset(aclrCust, 0xC0, sizeof(aclrCust));

  workPath=new char[MAX_FULL_FILENAME]; *workPath=0;
  curActPath=new char[MAX_FULL_FILENAME]; *curActPath=0;
  curPacPath=new char[MAX_FULL_FILENAME]; *curPacPath=0;
  curPath=new char[MAX_FULL_FILENAME]; *curPath=0;
  tempPath=new char[MAX_FULL_FILENAME]; *tempPath=0;
  tempPath2=new char[MAX_FULL_FILENAME]; *tempPath2=0;
  tempPath3=new char[MAX_FULL_FILENAME]; *tempPath3=0;
  txtbuff=new char[LMAXRFTXT+1]; *txtbuff=0;
  fileName0=new char[MAX_FULL_FILENAME]; *fileName0=0;
}

GlobalVars::~GlobalVars()
{
  if(clicPath) delete clicPath;
  if(ediText) delete ediText;
  if(ediGraf) delete ediGraf;
  if(originalPath) delete originalPath;
  if(originalDbFile) delete originalDbFile;
  if(playerId) delete playerId;
  if(iniFile) delete iniFile;
  if(dbFile) delete dbFile;

  if(workPath) delete workPath;
  if(curActPath) delete curActPath;
  if(curPacPath) delete curPacPath;
  if(curPath) delete curPath;
  if(tempPath) delete tempPath;
  if(tempPath2) delete tempPath2;
  if(tempPath3) delete tempPath3;
  if(txtbuff) delete txtbuff;
  if(fileName0) delete fileName0;
}

GlobalVars *gv;

RECT NullRect={0, 0, 0, 0};
POINT NullPoint={0, 0};

char cdUnit=0;

char *workPath=NULL;
char *curActPath=NULL;
char *curPacPath=NULL;
char *curPath=NULL;
char *tempPath=NULL;
char *tempPath2=NULL;
char *tempPath3=NULL;
char *txtbuff=NULL;

char *extName[]={"*.BMP", "*.GIF", "*.TXT", "*.PUZ", "*.SOP", "*.ASS", "*.CRW", "*.TXA",
		 "*.PAC", "*.PCC", "*.P?C", "*.HLP", "*.MVB", "*.EXE", "*.MID", "*.WAV", ""};

char *lastExt="   ";

BOOL detectCD()
{
  union REGS inRegs, outRegs;

  inRegs.x.ax=0x1500;
  inRegs.x.bx=0;
  int86(0x2f, &inRegs, &outRegs);

  if(outRegs.x.bx==0) cdUnit=0;
  else cdUnit=outRegs.x.cx+'A';

  return cdUnit!=0;
}

BOOL setActivePath(char *newPath)
{
  // tempPath és còpia de newPath
  // tempPath2 és directori actual

  int actDisk, lenPath, newDisk;

  if(newPath==0 || *newPath==0) return FALSE;

  lenPath=strlen(strcpy(tempPath, newPath));
  actDisk=getdisk();
  getcwd(tempPath2, MAX_FULL_FILENAME-1);

  if(lenPath>1 && tempPath[1]==':')
  { newDisk=tempPath[0];
	 if(newDisk>='a') newDisk-=('a'-'A');
	 if(newDisk<'A' || newDisk>'Z') return FALSE;
	 newDisk-='A';
	 setdisk(newDisk);
	 if(getdisk()!=newDisk)
	 { setdisk(actDisk); return FALSE;}
	 if(lenPath==2) {tempPath[2]='\\'; tempPath[3]=0; lenPath=3;}
  }

  if(lenPath>3 && tempPath[lenPath-1]=='\\') tempPath[lenPath-1]=0;
  if(chdir(tempPath)!=0)
  { setdisk(actDisk);
	 chdir(tempPath2);
	 return FALSE;
  }

  getcwd(curPath, MAX_FULL_FILENAME-1);
  if(curPath[(lenPath=strlen(curPath))-1]!='\\')
  { curPath[lenPath]='\\';
	 curPath[lenPath+1]=0;
  }

  return TRUE;
}

char *setPathVar(char *pathAndFileName, char *destPath)
{
  char drive[MAXDRIVE];
  char dir[MAXDIR];
  char file[MAXFILE];
  char ext[MAXEXT];

  // ATENCIO: SIMPLIFYFILENAME
  fnsplit(simplifyFileName(getFullFileName(pathAndFileName)), drive, dir, file, ext);
  strcat(strcpy(destPath, drive), dir);
  if(destPath[strlen(destPath)-1]!='\\') strcat(destPath, "\\");

  /*
  if(*drive>='A') setdisk((int)(*drive)-'A');
  if(strlen(destPath)>1)
  { destPath[strlen(destPath)-1]=0;
	 chdir(destPath);
	 destPath[strlen(destPath)]='\\';
  }
  */

  return strcat(strcpy(tempPath2, file), ext);
}


char *getFullFileName(char *fileName, char *basePath)
{
  if(*fileName=='\\' ||strchr(fileName, ':'))
  { if(cdUnit==0 ||
		 strnicmp("CDR:", fileName, 4)!=0)
	 { if(strncmp("..", fileName, 2)!=0) return fileName;

		strcpy(tempPath, basePath);

		return strcat(tempPath, fileName);
	 }

	 *tempPath=cdUnit;
	 strcpy(tempPath+1, fileName+3);
	 return tempPath;
  }
  return strcat(strcpy(tempPath, basePath), fileName);
}


char *getRelativeFileName(char *fileName, char *cmpPath)
{
  int i;
  char *c, *f;

  strupr(fileName);
  strupr(cmpPath);
  if(strchr(fileName, '\\')==0 ||
	  strncmp(fileName, cmpPath, 2)!=0) return fileName;

  for(i=0, c=cmpPath, f=fileName; *f!=0 && *c==*f; c++, f++)
	 if(*c=='\\') i++;

  if(i<2) strcpy(tempPath, fileName+2);
  else
  { *tempPath=0;
	 while((c=strchr(c, '\\'))!=NULL)
	 { strcat(tempPath, "..\\"); c++;}
	 strcat(tempPath, f);
  }
  return tempPath;
}


char *simplifyFileName(char *fn)
{
  char *it, *it2;
  int i;

  if(fn!=NULL)
	for(it=fn; *it!=0; it++)
	 if(*it=='\\')
	  for(it2=it+1;*it2!=0; it2++)
		if(*it2=='\\')
		{ if(it2[1]=='.' && it2[2]=='.')
		  { for(i=0;;i++)
			 { it[i]=it2[i+3];
				if(it[i]==0) break;
			 }
			 it=fn-1;
		  }
		  break;
		}

  return fn;
}


int getExt(char *fileName)
{
  char *begExt;
  int i;

  *lastExt=0;
  if(fileName==NULL || *fileName==0) return UNKNOW_EXT;

  if((begExt=strchr(strupr(fileName), ' '))==0) begExt=fileName+strlen(fileName)-1;
  for(;begExt>fileName; begExt--) if(*begExt=='.') break;
  if(*begExt!='.') return UNKNOW_EXT;
  for(begExt++, i=0; i<UNKNOW_EXT; i++)
	 if(strncmp(begExt, extName[i]+2, 3)==0) break;

  strncpy(lastExt, begExt, 3);

  return i;
}

struct txp
{ char *txt;
  struct txp *next;
  txp() {txt=0; next=0;}
};

BOOL writeTxtFile(char *fileName, char **txt, HWND hWnd)
{
  int f, i, k;
  static char ret[]={0x0D, 0x0A};
  OFSTRUCT ofs;
  int result=0;
  ofs.cBytes=sizeof(OFSTRUCT);

  if((f=OpenFile(fileName, &ofs, OF_CREATE|OF_WRITE))==-1)
	 result=IDS_ERROPEN;
  else
  { for(i=0; txt[i]!=0; i++)
		if(_lwrite(f, txt[i], (k=strlen(txt[i])))!=k ||
			_lwrite(f, ret, 2)!=2)
//		if(_lwrite(f, txt[i], strlen(txt[i]))==-1u ||
//			_lwrite(f, ret, 2)==-1u)
		{result=IDS_ERRWRITE; break;}
	 _lclose(f);
  }

  if(result)
  { BMsgBox(hWnd, IDS_WRITE, result, fileName);
	 return FALSE;
  }
  else return TRUE;
}

char **readTxtFile(char *fileName, int &nLines, HWND hWnd)
{
  char **txt=0;
  HFILEFC hffc;
  int i, j, l, result=0;
  struct txp *txpb, *txpp;
  char *buf0;

  nLines=0;
  //txtbuff[LMAXRFTXT]=0;
  if(fc_lopen(fileName, hffc)==TRUE)
  { if(hffc.lof>8000) l=8000; else l=(int)hffc.lof+1;
	 if((txpb=new struct txp)!=NULL &&
		 (buf0=new char[l+1])!=NULL)
	 { memset(buf0, 0, l+1);
		txpp=txpb;
		for(i=0; !fc_eof(hffc); i++)
		{ //if(fc_gets(txtbuff, LMAXRFTXT, hffc)==NULL)
		  if(fc_gets(buf0, l, hffc)==NULL)
		  { if(!fc_eof(hffc)) result=IDS_ERRREAD; break;}
		  //for(j=strlen(txtbuff)-1; j>=0; j--)
		  for(j=strlen(buf0)-1; j>=0; j--)
			 //if(txtbuff[j]<' ') txtbuff[j]=0; else break;
			 if(buf0[j]<' ') buf0[j]=0; else break;
		  //if((txpp->txt=dupStr(txtbuff))==0 ||
		  if((txpp->txt=dupStr(buf0))==0 ||
			  (txpp->next=new struct txp)==0)
		  {result=IDS_NOMEM; break;}
		  txpp=txpp->next;
		}
		delete buf0;
	 }
	 else result=IDS_NOMEM;
	 fc_lclose(hffc);
  }
  else result=IDS_ERROPEN;

  if(!result)
  { if(i>0)
	 { if((txt=newArray(i))!=0)
		{ nLines=i;
		  for(i=0, txpp=txpb; txpp!=0 && i<nLines; txpp=txpp->next, i++)
			 txt[i]=txpp->txt;
		}
		else result=IDS_NOMEM;
	 }
	 else result=IDS_ERRREAD;
  }

  while(txpb!=0) {txpp=txpb->next; delete txpb; txpb=txpp;}

  if(result) BMsgBox(hWnd, IDS_LECT, result, fileName);

  return txt;
}

void delArray(char **&txt)
{
  char **txt0;

  if(txt)
  { for(txt0=txt; *txt0!=0; txt0++) delete *txt0;
	 delete txt;
	 txt=0;
  }
}

char **dupArray(char **txt)
{
  char **txt0=0;
  int i;

  if(txt==0) return 0;
  for(i=0; txt[i]!=0; i++);
  if((txt0=newArray(i))==0) return 0;
  if(copyArray(txt0, 0, txt, 0, i, FALSE)==FALSE) delArray(txt0);
  return txt0;
}

char **newArray(int n)
{
  char **txt;
  int i;

  if((txt=new char *[n+1])==0) {NOMEM; return 0;}
  for(i=0; i<=n; i++) txt[i]=0;
  return txt;
}

BOOL copyArray(char **dest, int indest, char **source, int insource,
			 int e, BOOL upper)
{
  int i, d, s;

  for(i=0, d=indest, s=insource; i<e; i++, d++, s++)
  { if(dest[d]) delete dest[d];
	 if(source[s]==0 || (dest[d]=dupStr(source[s]))==0) break;
	 if(upper) AnsiUpper(dest[d]);
  }
  return (i==e);
}

char *dupStr(char *source)
{ char *txt;
  if((txt=strdup(source))==0) NOMEM;
  return txt;
}

char *changeStrVar(char *&var, char *newText)
{
  if(newText==NULL) return NULL;
  if(var)
  { if(strlen(newText)<=strlen(var))
	 { strcpy(var, newText);
		return var;
	 }
	 delete var;
  }
  var=dupStr(newText);
  return var;
}

void drawMarc(HDC hDC, RECT *rect, int pos, int gruix)
{
  int i;
  int x0, y0, x1, y1;
  HPEN hGris=0, hBlanc=0, h1, h2, hOldPen;

  if(pos==INTERIOR_NEGRE) {h1=h2=(HPEN)GetStockObject(BLACK_BRUSH); pos=INTERIOR;}
  else
  { hBlanc=CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	 hGris=CreatePen(PS_SOLID, 1, RGB(0x80, 0x80, 0x80));
	 h1=(pos==INTERIOR ? hBlanc:hGris);
	 h2=(pos==INTERIOR ? hGris:hBlanc);
  }

  hOldPen=(HPEN)SelectObject(hDC, h1);

  x0=rect->left; y0=rect->top; x1=rect->right; y1=rect->bottom;
  if(pos==INTERIOR) {x1--; y1--;}
  else {x0--; y0--;};

  for(i=0; i<gruix; i++)
  { MoveTo(hDC, x0, y1);
	 if(h1!=h2) SelectObject(hDC, h1);
	 LineTo(hDC, x0, y0);
	 LineTo(hDC, x1, y0);

	 if(h1!=h2) SelectObject(hDC, h2);
	 LineTo(hDC, x1, y1);
	 LineTo(hDC, x0, y1);

	 if(pos==INTERIOR)
	 { x0++; y0++; x1--; y1--;}
	 else
	 { x0--; y0--; x1++; y1++;}
  }
  SelectObject(hDC, hOldPen);
  if(hGris) DelObj(hGris);
  if(hBlanc) DelObj(hBlanc);
}

BOOL playMusic(HWND hWnd, struct musica *m, HCURSOR hCursor)
{
  /*
  int i;

  if(!sonor) return TRUE;

  if((i=OpenSound())==S_SERDVNA ||
	  i==S_SEROFM ||
	  i<1) return FALSE;

  for(;m->nota!=0; m++)
	 if(SetVoiceNote(1, m->nota, m->figura, m->dot)!=0)
	 { CloseSound(); return FALSE;}

  UpdateWindow(hWnd);

  if(hCursor) setCursor(hWnd, HANDLE_CURSOR, hCursor);
  StartSound();
  WaitSoundState(S_QUEUEEMPTY);
  CloseSound();
  if(hCursor) resetCursor();
  return TRUE;
  */

  int i;
  MSG msg;
  DWORD tickCount;


  if(hCursor) setCursor(hWnd, HANDLE_CURSOR, hCursor);

  UpdateWindow(hWnd);

  if(!sonor ||
	  (i=OpenSound())==S_SERDVNA ||
	  i==S_SEROFM ||
	  i<1)
  {  tickCount=GetTickCount()+1000;
	  while(GetTickCount()<tickCount)
	  { if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		 { TranslateMessage(&msg);
			DispatchMessage(&msg);
		 }
	  }
  }
  else
  { for(;m->nota!=0; m++)
		if(SetVoiceNote(1, m->nota, m->figura, m->dot)!=0) break;

	 if(m->nota==0)
	 { StartSound();
		WaitSoundState(S_QUEUEEMPTY);
	 }

	 CloseSound();
  }

  if(hCursor) resetCursor();
  return TRUE;
}

char bufTag[BRACKET_TAG_SIZE+1];
char *getBracketTag(char *&txt)
{
  int i;

  *bufTag=0;

  for(;*txt!=0; txt++) if(*txt=='{') break;
  if(*txt!='{') return 0;
  *txt++=0;
  for(i=0; i<BRACKET_TAG_SIZE && *txt!=0; txt++, i++)
  { if(*txt=='}') {bufTag[i]=0; break;}
	 bufTag[i]=*txt;
  }
  if(*txt!='}') return 0;
  txt++;
  return bufTag;
}

// --------------------------------------------------------
// bmpCont
// --------------------------------------------------------

struct stretchTable strt[STRETCH_TABLE_SIZE];

HBITMAP mkStretchBm(int tablePos, HWND hWnd, HBITMAP hbm, int newBmW, int newBmH, int orgBmW, int orgBmH)
{
  strt[tablePos].hBmOriginal=hbm;
  strt[tablePos].orgBmW=orgBmW;
  strt[tablePos].orgBmH=orgBmH;
  strt[tablePos].newBmW=newBmW;
  strt[tablePos].newBmH=newBmH;
  return(strt[tablePos].hBmStretch=createNewBitmap(hWnd, hbm, newBmW, newBmH, orgBmW, orgBmH));
}

bmpCont::bmpCont()
{ prev=next=NULL;
  hBm=NULL;
  fileName=NULL;
  bmw=bmh=0;
  //count=0;
  bits=NULL;
  bminf=NULL;
}

bmpCont::~bmpCont()
{ unchain();
  if(fileName) {delete fileName; fileName=0;}
  if(hBm) {DelObj(hBm); hBm=NULL;}
  if(bits) {GlobalFree(bits); bits=NULL;}
  if(bminf) {delete bminf; bminf=NULL;}
}

bmpCont *bmpCont::first=0;

void bmpCont::chain()
{
  if(prev || next) unchain();
  next=first; first=this;
  if(next) next->prev=this;
}

void bmpCont::unchain()
{ if(next==0 && prev==0)
  { if(first==this) first=0;
	 return;
  }
  if(next) next->prev=prev;
  if(prev) prev->next=next;
  else first=next;
  next=prev=0;
}

BOOL bmpCont::loadBmp(HWND hWnd, HBITMAP &hBitmap, char *fn,
				int &bmwidth, int &bmheight)
{
  bmpCont *itera;

  if(hBitmap) freeBmp(hBitmap);

  for(itera=first; itera!=0; itera=itera->next)
	 if (stricmp(fn, itera->fileName)==0)
	 { hBitmap=itera->hBm;
		bmwidth=itera->bmw;
		bmheight=itera->bmh;
		//itera->count++;
		return TRUE;
	 }

  if((itera=new bmpCont)==0) {NOMEM; return FALSE;}
  if((itera->fileName=dupStr(fn))==0 ||
		loadImgFile(hWnd, hBitmap, fn, bmwidth, bmheight)==FALSE)
  { delete itera;
	 return FALSE;
  }

  itera->hBm=hBitmap;
  itera->bmw=bmwidth;
  itera->bmh=bmheight;
  //itera->count++;
  itera->chain();

  return TRUE;
}

/*
BOOL bmpCont::freeBmp(HBITMAP &hBitmap)
{
  bmpCont *itera;

  if(!hBitmap) return TRUE;
  for(itera=first; itera!=NULL; itera=itera->next)
	 if(hBitmap==itera->hBm && itera->count>0)
		itera->count--;

  hBitmap=NULL;
  return TRUE;
}
*/

void bmpCont::flush()
{ while(first) delete first;}

void bmpCont::flushDIBs()
{ bmpCont *itera;

  for(itera=first; itera!=NULL; itera=itera->next)
  { if(itera->bits) {GlobalFree(itera->bits); itera->bits=NULL;}
	 if(itera->bminf) {delete itera->bminf; itera->bminf=NULL;}
  }

}

BOOL bmpCont::getDIB(HBITMAP hBitmap, BITMAPINFO *&rbminf, HGLOBAL &rbits, int &rbmw, int &rbmh)
{
  bmpCont *itera;

  for(itera=first; itera!=NULL; itera=itera->next)
	 if(itera->hBm==hBitmap)
	 { if(itera->bits==NULL)
		  loadDIBFile(itera->fileName, itera->bminf, itera->bits, itera->bmw, itera->bmh);
		rbminf=itera->bminf;
		rbits=itera->bits;
		rbmw=itera->bmw;
		rbmh=itera->bmh;
		break;
	 }

  return itera!=NULL;
}


// --------------------------------------------------------
// fileCont
// --------------------------------------------------------

BOOL FCInitiated=FALSE;
BOOL FCFileIsOpen=FALSE;
int nFilesFC=0;
long offsetBaseFC=0L;
long offsetBaseExe=0L;
char FCBasePath[MAX_FULL_FILENAME]="";	// Directori base d'FC
int lenBasePath;
char far* FCDict;
HFILE hFC=HFILE_ERROR;

BOOL openFC(char* fName, HWND hWnd)
{
  int result;
  char *endPath;

  if (FCInitiated) closeFC();

  if((result=readFCHeader(getFullFileName(fName), hFC, NULL, FALSE))==0)
  { strcpy(FCBasePath, fName);
	 endPath=strrchr(FCBasePath, '\\')+1;
	 if(endPath) *endPath=0;
	 lenBasePath=strlen(FCBasePath);
	 if(_lread(hFC, &nFilesFC, sizeof(int))==sizeof(int) && nFilesFC>0 &&
		 _lread(hFC, &offsetBaseFC, sizeof(long))==sizeof(long) && offsetBaseFC>0)
	 { if((FCDict=(char far*)farmalloc(offsetBaseFC))==NULL ||
			_hread(hFC, FCDict, offsetBaseFC-11)!=(offsetBaseFC-11))
		result=IDS_NOMEM;
	 }
	 else result=IDS_ERRFMT;

	 if(result) _lclose(hFC);
  }


  if(result)
  { BMsgBox(hWnd, IDS_LECT, result, fName);
	 closeFC();
  }
  else {FCInitiated=TRUE; FCFileIsOpen=FALSE;}

  return FCInitiated;
}


void closeFC()
{
  if(FCDict!=NULL) {farfree(FCDict); FCDict=NULL;}
  nFilesFC=0;
  *FCBasePath=0;
  offsetBaseFC=0L;
  offsetBaseExe=0L;
  if(hFC!=HFILE_ERROR) {_lclose(hFC); hFC=HFILE_ERROR;}
  FCInitiated=FALSE;
  FCFileIsOpen=FALSE;
}


BOOL isFCActive() {return FCInitiated;}

BOOL fc_lopen(char *fName, HFILEFC& hffc)
{
  int i;
  HFILE hf;
  char far* fd;
  char* fNameBase;
  char* fNameBase2;
  char* dsfn;

  if(FCInitiated && FCFileIsOpen==FALSE && strnicmp(fName, FCBasePath, lenBasePath)==0)
  {
	 fNameBase=fName+lenBasePath;

	 if((dsfn=dupStr(fName))==NULL) return FALSE;
	 dsfn=simplifyFileName(dsfn);
	 if(stricmp(fName, dsfn)==0 || strlen(dsfn)<lenBasePath)
	 {delete dsfn; dsfn=NULL; fNameBase2=NULL;}
	 else fNameBase2=dsfn+lenBasePath;

	 fd=FCDict;

	 for(i=0; i<nFilesFC; i++)
	 { if(_fstricmp(fd, fNameBase)==0 ||
			(fNameBase2!=0 && _fstricmp(fd, fNameBase2)==0))
		{ hffc.hf=hFC;
		  fd+=(_fstrlen(fd)+1);
		  _llseek(hFC, offsetBaseExe + *(long far*)fd, 0); fd+=sizeof(long);
		  hffc.lof=*(long far*)fd;
		  hffc.curPos=0L;
		  FCFileIsOpen=TRUE;
		  if(dsfn) delete dsfn;
		  return TRUE;
		}
		fd+=(_fstrlen(fd)+1+2*sizeof(long));
	 }

	 if(dsfn) delete dsfn;
  }

  if((hf=_lopen(fName, READ))==HFILE_ERROR) return FALSE;

  hffc.hf=hf;
  hffc.lof=_llof(hf);
  hffc.curPos=0L;

  return TRUE;
}

BOOL getFirstFCInto(char *fName)
{
  char far *lstP;
  BOOL result=FALSE;

  if(FCInitiated && FCDict!=NULL)
  { lstP=strrchr(fName, '\\');
	 if(lstP==NULL)
		lstP=fName;
	 else
		lstP++;
	 _fstrcpy(lstP, FCDict);
	 result=TRUE;
  }
  return result;
}

UINT fc_lread(HFILEFC &hffc, void _huge* hpvBuffer, UINT cbBuffer)
{
  UINT result;

  if(hffc.curPos+cbBuffer>hffc.lof)
		cbBuffer=(UINT)(hffc.lof-hffc.curPos);

  result=_lread(hffc.hf, hpvBuffer, cbBuffer);
  if((int)result!=HFILE_ERROR) hffc.curPos+=result;
  return result;
}


long fc_hread(HFILEFC &hffc, void _huge* hpvBuffer, long cbBuffer)
{
  long result;

  if(hffc.curPos+cbBuffer>hffc.lof)
		cbBuffer=hffc.lof-hffc.curPos;

  result=_hread(hffc.hf, hpvBuffer, cbBuffer);
  if(result!=-1L) hffc.curPos+=result;
  return result;
}


HFILE fc_lclose(HFILEFC &hffc)
{
  if(FCInitiated && hffc.hf==hFC && FCFileIsOpen==TRUE)
  { FCFileIsOpen=FALSE; return 0;}

  return _lclose(hffc.hf);
}


BOOL fc_eof(HFILEFC &hffc)
{
  return hffc.curPos>=hffc.lof;
}


char* fc_gets(char *buffer, int n, HFILEFC &hffc)
{
  int i;
  char c;

  for(i=0; i<(n-1) && !fc_eof(hffc); i++)
  { if(fc_lread(hffc, &c, 1)!=1) break;
	 buffer[i]=c;
	 if(c==0xA) {i++; break;}
  }
  buffer[i]=0;
  return buffer;
}

LONG _llof(HFILE hf) // longitud del fitxer
{
  long lof;
  long curPos=_lltell(hf);
  lof=_llseek(hf, 0, 2);
  _llseek(hf, curPos + offsetBaseExe, 0);
  return lof;
}

LONG _lltell(HFILE hf)
{
  return _llseek(hf, 0, 1) - offsetBaseExe;
}


int _lreadsz(HFILE hf, char* buffer, int maxLen)
{
  // llegeix cadenes acabades en zero
  // retorna el nombre de caracters llegits o HFILE_ERROR

  int i;
  unsigned char c;

  for(i=0; i<maxLen; i++)
  { if(_lread(hf, &c, 1)!=1) return HFILE_ERROR;
	 if((buffer[i]=c)==0) break;
  }
  return i;
}

int readFCHeader(char *fName, HFILE &hf, char *desc, BOOL close)
{

  int result=0;
  unsigned char c=0;
  char *itd;
  int i;

  if((hf=_lopen (fName, READ | OF_SHARE_COMPAT))!=HFILE_ERROR)
  { if(_llseek(hf, offsetBaseExe, 0)!=offsetBaseExe ||
		 _lread(hf, tempPath2, 5)!=5 ||
		 strncmp(tempPath2, "PCC00", 5)!=0 ||
		 _lread(hf, tempPath2, 2)!=2)
		result=IDS_ERRFMT;
	 else
	 { for(itd=desc, i=0; i<MAX_DESC; i++)
		{ if(_lread(hf, &c, 1)!=1) {result=IDS_ERRFMT; break;}
		  if(c==0x87) break;
		  if(itd!=NULL) {*itd++=c;}
		}
		if(c!=0x87 || _lread(hf, tempPath2, 2)!=2)
		  result=IDS_ERRFMT;
		else {if(itd!=NULL) *itd=0;}
	 }
	 if(close || result!=0) {_lclose(hf); hf=NULL;}
  }
  else result=IDS_ERROPEN;

  return result;
}

// SaveDir

SaveDir::SaveDir()
{ drive=getdisk();
  path[0]='\\';
  getcurdir(0, path+1);
};

SaveDir::~SaveDir()
{ setdisk(drive);
  chdir(path);
};

// ---------- Altres funcions ------------------------------------------

BOOL checkCas(HWND hDlg, int &ncw, int &nch, int maxX,  int maxY, int idH, int idV)
{
  int w, h, i;
  BOOL result=FALSE;

  if((w=GetDlgItemInt(hDlg, idH, &i, TRUE))<1 ||
		w>maxX ||
		i==0 ||
		(h=GetDlgItemInt(hDlg, idV, &i, TRUE))<1 ||
		h>maxY ||
		i==0 ||
		w==1 && h==1)
  { i=IDS_MASSA;
	 if(w==1 && h==1) i=IDS_UNA;
	 else if(w==0 || h==0) i=IDS_DUES;
	 BMsgBox(hDlg, i, 0);
  }
  else
  { ncw=w;
	 nch=h;
	 result=TRUE;
  }
  return result;
}


void checkCaret(HWND hWnd, UINT msg)
{
  switch(msg)
  { case WM_SETFOCUS:
			if(caretVis)
			{ CreateCaret(hWnd, NULL, caretW, caretH);
			  SetCaretPos(caretX, caretY);
			  ShowCaret(hWnd);
			}
			break;

	 case WM_KILLFOCUS:
			if(caretVis)
			{ HideCaret(hWnd);
			  DestroyCaret();
			}
			break;
  }
}


