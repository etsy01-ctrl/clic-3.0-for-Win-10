// ---------------------------------------
// ACTFILE.CPP
// Implementacio de la classe ActFile
// ---------------------------------------

#include "actfile.h"
#include "miscfunc.h"
#include "clicdlg.h"
#include "rcomu\clicres.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// ---------------------------------------
// Utilitats

BOOL logFontToString(LOGFONT &lf, char *str)
{
  return sprintf(str,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%s",
					  lf.lfHeight, lf.lfWidth, lf.lfEscapement, lf.lfOrientation,
					  lf.lfWeight, (UINT)lf.lfItalic, (UINT)lf.lfUnderline,
					  (UINT)lf.lfStrikeOut, (UINT)lf.lfCharSet,
					  (UINT)lf.lfOutPrecision, (UINT)lf.lfClipPrecision,
					  (UINT)lf.lfQuality, (UINT)lf.lfPitchAndFamily,
					  lf.lfFaceName)!=EOF;
}


BOOL colorsToString(Colors &c, char *str)
{
  return sprintf(str, "%06lX,%06lX,%06lX,%i",
					  c.backColor, c.textColor,
					  c.shadowColor, c.shadow)!=EOF;
}


BOOL stringToLogFont(char *str, LOGFONT &lf)
{
  char *tmptx;

  sscanf(str, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",
			&lf.lfHeight, &lf.lfWidth, &lf.lfEscapement, &lf.lfOrientation,
			&lf.lfWeight, &lf.lfItalic, &lf.lfUnderline, &lf.lfStrikeOut,
			&lf.lfCharSet, &lf.lfOutPrecision, &lf.lfClipPrecision,
			&lf.lfQuality, &lf.lfPitchAndFamily);

  if((tmptx=strrchr(str, ','))!=0) strncpy(lf.lfFaceName, tmptx+1, 30);

  return TRUE;
}


BOOL stringToColors(char *str, Colors &c)
{
  sscanf(str, "%lX,%lX,%lX,%i",
			&c.backColor, &c.textColor, &c.shadowColor, &c.shadow);

  return TRUE;
}


// ---------------------------------------


#ifndef NOEDIT
static char endDesc[2]={ENDDESC, 0};
static char txbuf[100];
#endif

UINT boolParms;
#define getFlag(fl) (boolParms&fl)!=NULL
#define setFlag(val, fl) if(val) boolParms|=fl

ActFile::ActFile()
{ tags[0]=tags[1]=graTxt=0;
  txBase=txBtCorregir=txPrev=bmpFons=initMessPrev=0;
  clear();
}

ActFile::~ActFile() {clear();}

ActFile& ActFile::operator = (const ActFile& af)
{
  int i;

  copyStyle(af);

  actMode=af.actMode;
  puzMode=af.puzMode;
  ncw=af.ncw; nch=af.nch;
  nctxw=af.nctxw; nctxh=af.nctxh;
  stretch=af.stretch;
  memcpy(fileName, af.fileName, sizeof(fileName));
  memcpy(initMess, af.initMess, sizeof(initMess));
  memcpy(endMess, af.endMess, sizeof(endMess));
  memcpy(fileDesc, af.fileDesc, sizeof(fileDesc));

  for(i=0; i<3; i++)
  { memcpy(fileNameCont[i], af.fileNameCont[i], sizeof(fileNameCont[i]));
	 cont[i]=af.cont[i];
	 nLines[i]=af.nLines[i];
  }

  for(i=0; i<2; i++)
  { bar[i]=af.bar[i];
	 delim[i]=af.delim[i];
	 ntags[i]=af.ntags[i];
	 if(tags[i]) delArray(tags[i]);
	 tags[i]=dupArray(af.tags[i]);
  }

  if(graTxt) delArray(graTxt);
  graTxt=dupArray(af.graTxt);
  memcpy(ass, af.ass, sizeof(ass));
  mAss=af.mAss;
  invAss=af.invAss;
  graPos=af.graPos;

  loaded=af.loaded;
  modified=af.modified;

  sol=af.sol;
  reported=af.reported;
  solved=af.solved;
  movToSolv=af.movToSolv;
  iniSolved=af.iniSolved;
  okToNext=af.okToNext;
  brPar=af.brPar;
  noAv=af.noAv;

  if(txBase) {delete txBase; txBase=NULL;}
  if(af.txBase) txBase=dupStr(af.txBase);
  if(txBtCorregir) {delete txBtCorregir; txBtCorregir=NULL;}
  if(af.txBtCorregir) txBtCorregir=dupStr(af.txBtCorregir);
  btCorregir=af.btCorregir;
  avCont=af.avCont;
  avMaj=af.avMaj;
  avAcc=af.avAcc;
  avPunt=af.avPunt;
  avDblSpc=af.avDblSpc;
  avLletra=af.avLletra;
  avNoSalta=af.avNoSalta;
  avScope=af.avScope;
  avMaxScope=af.avMaxScope;
  avPrevHelp=af.avPrevHelp;
  avTimePH=af.avTimePH;
  if(txPrev) {delete txPrev; txPrev=NULL;}
  if(af.txPrev) txPrev=dupStr(af.txPrev);
  if(initMessPrev) {delete initMessPrev; initMessPrev=0;}
  if(af.initMessPrev) initMessPrev=dupStr(af.initMessPrev);
  tabSpc=af.tabSpc;

  return *this;
}

void ActFile::copyStyle(const ActFile& af)
{
  int i;

  if(actMode!=TEXTACT || af.actMode==actMode)
  { txtCW=af.txtCW; txtCH=af.txtCH;}

  txtCW2=af.txtCW2; txtCH2=af.txtCH2;
  memcpy(custHelpFile, af.custHelpFile, sizeof(custHelpFile));
  memcpy(hlpTopic, af.hlpTopic, sizeof(hlpTopic));

  for(i=0; i<3; i++) comptadors[i]=af.comptadors[i];

  for(i=0; i<2; i++)
  { colorFons[i]=af.colorFons[i];
	 tramaFons[i]=af.tramaFons[i];
	 colorUsuari[i]=af.colorUsuari[i];
	 colorError[i]=af.colorError[i];
  }

  shHelp=af.shHelp;
  shPuz=af.shPuz;
  shDisk=af.shDisk;
  shPrint=af.shPrint;
  shPorta=af.shPorta;
  marcs=af.marcs;
  barraAmunt=af.barraAmunt;

  for(i=0; i<4; i++)
  { logF[i]=af.logF[i];
	 colors[i]=af.colors[i];
  }
  custHlp=af.custHlp;
  noBV=af.noBV;
  btTipus=af.btTipus;

  tileBmp=af.tileBmp;
  pwTransp=af.pwTransp;
  if(bmpFons) {delete bmpFons; bmpFons=0;}
  if(af.bmpFons) bmpFons=dupStr(af.bmpFons);
  pwrp=af.pwrp;
  pwrx=af.pwrx;
  pwry=af.pwry;

  useDLL=af.useDLL;
  memcpy(dllOptions, af.dllOptions, sizeof(dllOptions));
  memcpy(rgDLL, af.rgDLL, sizeof(rgDLL));
}


void ActFile::clear()
{
  int i;

  actMode=NONE;
  puzMode=INTERC;
  graPos=AB;
  ncw=nch=nctxw=nctxh=0;
  txtCW=txtCW2=DEF_TXTCASW; txtCH=txtCH2=DEF_TXTCASH;
  stretch=FALSE;
  btCorregir=TRUE; avCont=TRUE; avAcc=TRUE; avMaj=FALSE; brPar=FALSE;
  avDblSpc=FALSE; avPunt=TRUE; avLletra=TRUE; avNoSalta=TRUE;
  avScope=3;
  avMaxScope=6;

  memset(fileName, 0, sizeof(fileName));
  memset(initMess, 0, sizeof(initMess));
  memset(endMess, 0, sizeof(endMess));
  memset(fileDesc, 0, sizeof(fileDesc));
  memset(custHelpFile, 0, sizeof(custHelpFile));
  memset(hlpTopic, 0, sizeof(hlpTopic));
  for(i=0; i<MAXCW*MAXCH; i++) ass[i]=-2;
  mAss=FALSE;
  invAss=FALSE;
  shHelp=TRUE;
  shPuz=TRUE;
  shDisk=TRUE;
  shPrint=FALSE;
  shPorta=TRUE;
  custHlp=FALSE;
  noBV=FALSE;
  barraAmunt=FALSE;
  okToNext=FALSE;
  noAv=FALSE;
  tabSpc=16;

  for(i=0; i<3; i++)
  { memset(fileNameCont[i], 0, sizeof(fileNameCont[i]));
	 cont[i]=EXT_TXT;
	 nLines[i]=0;
	 comptadors[i]=TRUE;
  }

  for(i=0; i<2; i++)
  { ntags[i]=0;
	 if(tags[i]) delArray(tags[i]);
	 bar[i]=TRUE;
	 delim[i]=TRUE;
	 colorFons[i]=RGB(0xC0, 0xC0, 0xC0);
	 tramaFons[i]=-1;
  }
  colorUsuari[0]=RGB(0x00, 0x00, 0xFF);
  colorUsuari[1]=RGB(0xC0, 0xC0, 0xC0);
  colorError[0]=RGB(0xFF, 0xFF, 0xFF);
  colorError[1]=RGB(0xFF, 0x00, 0x00);

  if(graTxt) delArray(graTxt);
  for(i=0; i<4; i++)
  { logF[i]=logFont0;
	 colors[i].backColor=RGB(0xFF, 0xFF, 0xFF);
	 colors[i].textColor=RGB(0, 0, 0);
	 colors[i].shadowColor=RGB(0x80, 0x80, 0x80);
	 colors[i].shadow=FALSE;
  }

  colors[2].backColor=RGB(0xC0, 0xC0, 0xC0);

  loaded=modified=sol=FALSE;
  reported=TRUE;
  solved=FALSE;
  movToSolv=0;
  iniSolved=0;
  marcs=TRUE;

  useDLL=FALSE;
  memset(dllOptions, 0, sizeof(dllOptions));
  memset(rgDLL, 0, sizeof(rgDLL));

  if(txBase) {delete txBase; txBase=NULL;}
  if(txBtCorregir) {delete txBtCorregir; txBtCorregir=NULL;}
  if(txPrev) {delete txPrev; txPrev=NULL;}
  if(bmpFons) {delete bmpFons; bmpFons=NULL;}
  if(initMessPrev) {delete initMessPrev; initMessPrev=NULL;}
  avPrevHelp=0;
  avTimePH=30;
  btTipus=1;
  tileBmp=TRUE;
  pwTransp=FALSE;
  pwrp=FALSE;
  pwrx=pwry=0;
}


BOOL ActFile::newAct(int tipus)
{
  int i;

  clear();

  switch(tipus)
  { case PUZZLE:
			cont[0]=EXT_BMP;
			ncw=4; nch=3;
			loaded=TRUE;
			break;

	 case ASSOCIA:
			cont[0]=EXT_TXT; cont[1]=EXT_TXT; cont[2]=EXT_TXT;
			ncw=4; nch=3;
			loaded=TRUE;
			stretch=TRUE;
			sol=FALSE;
			break;

	 case SOPA:
			bar[0]=FALSE;
			cont[0]=EXT_BMP;
			delim[0]=FALSE;
			delim[1]=TRUE;
			stretch=TRUE;
			txtCW=txtCH=20;

	 case CREUATS:
			ncw=4; nch=3;
			nctxw=10; nctxh=5;
			txtCW=txtCH=20;
			graTxt=newArray(nctxh);
			if((graTxt[0]=newChar(nctxw+1))==0) break;
			memset(graTxt[0], COMODI, nctxw);
			graTxt[nctxw]=0;
			for(i=1; i<nctxh; i++)
			  if((graTxt[i]=dupStr(graTxt[0]))==0) return FALSE;
			loaded=TRUE;
			break;

	 case TEXTACT:
			txtCW=540; txtCH=330;
			txBase=dupStr(nullStr);
			txBtCorregir=dupStr(getStringTbl(IDS_BTCORREGIR));
			txPrev=dupStr(nullStr);
			initMessPrev=dupStr(nullStr);
			loaded=TRUE;
			break;

	 default: break;
  }

  if(loaded) actMode=tipus;
  return loaded==TRUE;
}

BOOL ActFile::ajAllowed()
{
	switch(actMode)
	{ case CREUATS: return FALSE;
	  case PUZZLE:
			 if(puzMode==MEMORY) return FALSE;
			 break;
	  case ASSOCIA:
			 switch(puzMode)
			 { case IDENTIFICA:
				// Atencio:
				case ESCRIU:
				case INFO:
				case EXPLORA:
					  return FALSE;
				default:
					  break;
			 }
			 break;

	  case TEXTACT:
			 return TRUE;

	  default: break;
	}
	return TRUE;
}


BOOL ActFile::load(char *name)
{
  char **txt=0;
  char *tmptx;
  char *fn;
  int  i, l, tipus, nLin, fLine, ver, ncas, k;
  BOOL result;

  clear();

  strncpy(fileName, setPathVar(name, curActPath), 12);
  tipus=getExt(fileName);
  fn=getFullFileName(fileName, curActPath);
  fLine=0;
  ver=0;

  switch (tipus)
  { case EXT_PUZ:
	 case EXT_ASS:
	 case EXT_SOP:
	 case EXT_CRW:
	 case EXT_TXA:
	 case EXT_PAC:
			result=0;
			break;

	 default:
			result=IDS_ERRFMT;
  }

  if(result==0 && (txt=readTxtFile(fn, nLin))!=0)
  { if(nLin>2 &&
		 (ver=(ver=atoi(txt[fLine++]))>1000 ? ver-1000 : ver)>=100 &&
		 ver<=CLICVER)
	 { if(ver>107)
		{ while(fLine<nLin)
		  { if((unsigned char)*txt[fLine++]==ENDDESC) break;
			 if(*fileDesc) strcat(fileDesc, "\r\n");
			 strncat(fileDesc, txt[fLine-1], MAX_DESC-strlen(fileDesc));
		  }
		}

		if(ver>100)
		{ for(i=0; i<(ver<118 ? 1:3); i++)
		  { stringToLogFont(txt[fLine++], logF[i]);
			 if(ver>=118)
				stringToColors(txt[fLine++], colors[i]);
		  }
		  if(ver<118) for(i=1; i<4; i++) {logF[i]=logF[0];}
		}

		if(ver>118)
		{ sscanf(txt[fLine++], "%i,%i,%i,%i,%lX,%lX,%i,%i,%i,%i,%i",
					&comptadors[0], &comptadors[1], &comptadors[2], &marcs,
					&colorFons[0], &colorFons[1], &tramaFons[0], &tramaFons[1], &btTipus,
					&pwrx, &pwry);
		}
		else if(ver==118)
		{ sscanf(txt[fLine++], "%i,%i,%i,%i,%lX,%lX,%i,%i",
					&comptadors[0], &comptadors[1], &comptadors[2], &marcs,
					&colorFons[0], &colorFons[1], &tramaFons[0], &tramaFons[1]);
		}
		else tramaFons[0]=0;

		if(ver<119) btTipus=0;

		if(ver>=119)
		  if(*txt[fLine++]!=0) bmpFons=dupStr(txt[fLine-1]);

		strncpy(initMess, txt[fLine++], sizeof(initMess)-1);
		strncpy(endMess, txt[fLine++], sizeof(endMess)-1);

		if(ver>102)
		{ boolParms=atoi(txt[fLine++]);
		  delim[0]=getFlag(0x0001); delim[1]=getFlag(0x0002);
		}

		if(ver>105)
		{ if((tmptx=strchr(txt[fLine], ';'))!=0)
		  { *tmptx=0; tmptx++;}
		  else tmptx=txt[fLine];

		  txtCW=atoi(txt[fLine++]);
		  txtCW2=atoi(tmptx);

		  if((tmptx=strchr(txt[fLine], ';'))!=0)
		  { *tmptx=0; tmptx++;}
		  else tmptx=txt[fLine];

		  txtCH=atoi(txt[fLine++]);
		  txtCH2=atoi(tmptx);
		}
		else
		{ txtCW=txtCW2=DEF_TXTCASW; txtCH=txtCH2=DEF_TXTCASH;}

		if(ver<118 && (tipus==EXT_SOP || tipus==EXT_CRW))
		{ txtCW=20; txtCH=20;}

		shHelp=!getFlag(0x0004);
		graPos=(boolParms&(0x0018))>>3;
		shPuz=!getFlag(0x0020);
		noBV=getFlag(0x0040);
		if(ver>=118)
		{ shDisk=getFlag(0x0080);
		  shPorta=getFlag(0x0100);
		  barraAmunt=getFlag(0x0200);
		}

		if(ver>=119)
		{ invAss=getFlag(0x400);
		  tileBmp=getFlag(0x800);
		  pwTransp=getFlag(0x1000);
		  pwrp=getFlag(0x2000);
		  noAv=getFlag(0x4000);
		  shPrint=getFlag(0x8000);
		}

		if(ver>108)
		{ custHlp=atoi(txt[fLine++]);
		  strncpy(hlpTopic, txt[fLine++], sizeof(hlpTopic)-1);
		  strncpy(custHelpFile, txt[fLine++], sizeof(custHelpFile)-1);
		}

		if(ver>114)
		{ useDLL=atoi(txt[fLine++]);
		  strncpy(rgDLL, txt[fLine++], sizeof(rgDLL)-1);
		  strncpy(dllOptions, txt[fLine++], sizeof(dllOptions)-1);
		}

		switch(tipus)
		{ case EXT_PUZ:
				 actMode=PUZZLE;
				 if(nLin<fLine+4) break;
				 strncpy(fileNameCont[0], txt[fLine++], sizeof(fileNameCont[0])-1);
				 if((cont[0]=getExt(fileNameCont[0]))>EXT_TXT ||
					 (ncw=atoi(txt[fLine++]))<1 ||
					 (nch=atoi(txt[fLine++]))<1 ||
					 ncw>MAXCW || nch>MAXCH ||
					 (ncw==1 && nch==1) ||
					 (puzMode=atoi(txt[fLine++]))<INTERC || puzMode>MEMORY) break;
				 stretch=(nLin>fLine && atoi(txt[fLine++])==1);
				 bar[0]=bar[1]=TRUE;
				 if(ver<120 && puzMode==MEMORY) graPos=AUB;
				 result=TRUE;
				 break;

		  case EXT_ASS:
				 actMode=ASSOCIA;
				 if(nLin<fLine+7) break;

				 if(ver>109)
					puzMode=atoi(txt[fLine++]);

				 if((ncw=atoi(txt[fLine++]))<1 ||
					 (nch=atoi(txt[fLine++]))<1 ||
					  ncw>MAXCW || nch>MAXCH) break;

				 strncpy(fileNameCont[0], txt[fLine++], sizeof(fileNameCont[0])-1);
				 if((cont[0]=getExt(fileNameCont[0]))>EXT_TXT) break;

				 strncpy(fileNameCont[1], txt[fLine++], sizeof(fileNameCont[1])-1);
				 if(puzMode!=INFO && puzMode!=IDENTIFICA &&
					 (cont[1]=getExt(fileNameCont[1]))>EXT_TXT) break;

				 bar[0]=(atoi(txt[fLine++])==1);
				 bar[1]=(atoi(txt[fLine++])==1);
				 stretch=TRUE;

				 if(ver>106)
				 { if((sol=atoi(txt[fLine++]))==TRUE)
					{ strncpy(fileNameCont[2], txt[fLine++],
								 sizeof(fileNameCont[2])-1);
					  if((cont[2]=getExt(fileNameCont[2]))>EXT_TXT) break;
					}
				 }

				 if(ver>101)
				 { if(nLin<fLine+1) break;
					mAss=(atoi(txt[fLine++])==1);
					if(mAss)
					{ if(puzMode==NORMAL) puzMode=ESPECIAL;
					  ncas=ncw*nch;
					  if(nLin<fLine+ncas+2) break;
					  if((nctxw=atoi(txt[fLine++]))<1 ||
						  (nctxh=atoi(txt[fLine++]))<1 ||
						  nctxw>MAXCW || nctxh>MAXCH) break;
					  for(i=0; i<ncas; i++)
						 ass[i]=atoi(txt[fLine++]);
					}
				 }

				 result=TRUE;
				 break;

		  case EXT_SOP:
				 actMode=SOPA;
				 if(nLin<fLine+7) break;

				 strncpy(fileNameCont[0], txt[fLine++], sizeof(fileNameCont[0])-1);

				 if(fileNameCont[0][0]=='*')
				 { bar[0]=FALSE; fileNameCont[0][0]=0; cont[0]=EXT_BMP;}
				 else bar[0]=TRUE;

				 if((bar[0] && (cont[0]=getExt(fileNameCont[0]))>EXT_TXT) ||
					 (ncw=atoi(txt[fLine++]))<1 ||
					 (nch=atoi(txt[fLine++]))<1 ||
					 ncw>MAXCW || nch>MAXCH ||
					 (ncw==1 && nch==1) ||
					 (nctxw=atoi(txt[fLine++]))<1 ||
					 (nctxh=atoi(txt[fLine++]))<1 ||
					 nctxw>MAXGW || nctxh>MAXGH ||
					 (nctxw==1 && nctxh==1) ||
					 (ntags[0]=atoi(txt[fLine++]))<1 ||
					 nLin<(nctxh+ntags[0]+fLine) ||
					 (tags[0]=newArray(ntags[0]))==0 ||
					 (graTxt=newArray(nctxh))==0 ||
					 copyArray(graTxt, 0, txt, fLine, nctxh, TRUE)==FALSE ||
					 copyArray(tags[0], 0, txt, fLine+nctxh, ntags[0], TRUE)==FALSE)
					break;

				 stretch=TRUE;
				 result=TRUE;
				 break;

		  case EXT_CRW:
				 actMode=CREUATS;
				 if(nLin<fLine+6) break;
				 if((nctxw=atoi(txt[fLine++]))<1 ||
					 (nctxh=atoi(txt[fLine++]))<1 ||
					 nctxw>MAXGW || nctxh>MAXGH ||
					 (nctxw==1 && nctxh==1) ||
					 nLin<(fLine+nctxh+(ntags[0]=nctxh)+(ntags[1]=nctxw)) ||
					 (graTxt=newArray(nctxh))==0 ||
					 (tags[0]=newArray(ntags[0]))==0 ||
					 (tags[1]=newArray(ntags[1]))==0 ||
					 copyArray(graTxt, 0, txt, fLine, nctxh, TRUE)==FALSE ||
					 copyArray(tags[0], 0, txt, fLine+nctxh, ntags[0], FALSE)==FALSE ||
					 copyArray(tags[1], 0, txt, fLine+nctxh+ntags[0], ntags[1], FALSE)==FALSE)
				  break;

				bar[0]=bar[1]=FALSE;

				ncw=1; nch=4;
				stretch=FALSE;
				result=TRUE;
				break;

		  case EXT_TXA:
				 actMode=TEXTACT;
				 stringToLogFont(txt[fLine++], logF[3]);
				 stringToColors(txt[fLine++], colors[3]);
				 sscanf(txt[fLine++], "%lX,%lX,%lX,%lX",
						  &colorUsuari[0], &colorUsuari[1],
						  &colorError[0], &colorError[1]);
				 sscanf(txt[fLine++], "%i,%i,%i,%i,%i,%i,%i",
						  &puzMode, &boolParms, &avScope, &avMaxScope, &avPrevHelp, &avTimePH, &tabSpc);
				 okToNext=getFlag(0x0001);
				 btCorregir=getFlag(0x0002);
				 avCont=getFlag(0x0004);
				 avMaj=getFlag(0x0008);
				 avAcc=getFlag(0x0010);
				 brPar=getFlag(0x0020);
				 avDblSpc=getFlag(0x0040);
				 avPunt=getFlag(0x0080);
				 avLletra=getFlag(0x0100);
				 avNoSalta=getFlag(0x0200);
				 txBtCorregir=dupStr(txt[fLine++]);
				 for(k=0; k<3; k++)
				 { for(l=2, i=fLine; i<nLin; i++)
					{ if((unsigned char)*txt[i]==ENDDESC) break;
					  l+=(strlen(txt[i])+2);
					}
					if((tmptx=newChar(l))==NULL) break;
					*tmptx=0; i=0;
					while(fLine<nLin)
					{ if((unsigned char)*txt[fLine++]==ENDDESC) break;
					  if(i) strcat(tmptx, "\r\n"); else i=1;
					  strcat(tmptx, txt[fLine-1]);
					}

					*(char **)(k==0 ? &txBase : (k==1 ? &txPrev : &initMessPrev))=tmptx;
				 }
				 result=(txBase!=NULL);

		  default: break;
		}
	 }
	 delArray(txt);

	 if(result==FALSE)
	 { BMsgBox(0, IDS_LECT, ver>CLICVER ? IDS_ERRNEWV:IDS_ERRFMT, fn);
		clear();
	 }
	 else
		loaded=TRUE;
  }
  else clear();
  return result;
}

BOOL ActFile::createActdFiles()
{
  int i;

  for(i=0; i<3; i++)
  { actd.useTempFName[i]=FALSE;
	 if(actd.tempFName[i]==NULL)
	 { GetTempFileName(0, "clc", 0, tempPath);
		actd.tempFName[i]=dupStr(tempPath);
	 }
  }

  return TRUE;
}

void ActFile::clearActdFiles()
{
  int i;
  OFSTRUCT ofs;
  ofs.cBytes=sizeof(OFSTRUCT);

  for(i=0; i<3; i++)
	 if(actd.tempFName[i]!=NULL)
	 { OpenFile(actd.tempFName[i], &ofs, OF_DELETE);
		delete actd.tempFName[i];
		actd.tempFName[i]=NULL;
	 }
}

ACTDATA ActFile::actd={CLICVER,
							  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
							  NULL, NULL, {NULL, NULL, NULL}, {FALSE, FALSE, FALSE}};

BOOL ActFile::prepareActData()
{
  int i;

  actd.actMode=&actMode;
  actd.actType=&puzMode;
  actd.arrayPos=&graPos;
  actd.rows=&nch;
  actd.columns=&ncw;
  actd.rows2=&nctxh;
  actd.columns2=&nctxw;
  actd.assignAss=&mAss;
  actd.ass=ass;
  actd.initMessage=initMess;
  actd.endMessage=endMess;
  for(i=0; i<3; i++) actd.useTempFName[i]=FALSE;

  return TRUE;
}

BOOL ActFile::callRMKDll()
{
  HINSTANCE dllinst;
  BOOL (FAR PASCAL *rmk) (LPSTR, LPACTDATA, LPSTR);
  int  (FAR PASCAL *gver) (void);
  BOOL result;
  char *fn;

  result=FALSE;
  if(!useDLL) return FALSE;
  fn=getFullFileName(rgDLL,curActPath);

  dllinst=LoadLibrary(fn);
  if (dllinst<=HINSTANCE_ERROR)
  { BMsgBox(0, IDS_LECT, IDS_ERROPEN, fn);
	 BMsgBox(0, IDS_ERRDLL);
	 return FALSE;
  }

  (FARPROC)gver=GetProcAddress(dllinst, "GetClicDllVer");
  if(gver==NULL ||
	  (*gver)()> CLIC_DLL_VERSION)
  { BMsgBox(0, gver==NULL ? IDS_ERRDLL:IDS_ERRNEWV);
	 FreeLibrary(dllinst);
	 return FALSE;
  }

  prepareActData();

  (FARPROC)rmk=GetProcAddress(dllinst, "RemakeAct");
  if(rmk==NULL)
	 BMsgBox(0, IDS_ERRDLL);
  else
  { result=(*rmk)(fileName, &actd, dllOptions);
	 if(!result) BMsgBox(0, IDS_ERRDLL);
  }
  FreeLibrary(dllinst);
  return result;
}

char *ActFile::getFnCont(int i)
{ if(useDLL && i<3 && cont[i]==EXT_TXT &&
	  actd.useTempFName[i] && actd.tempFName[i]!=NULL)
  { _fstrncpy(tempPath, actd.tempFName[i], 100);
	 return tempPath;
  }
  return getFullFileName(fileNameCont[i], curActPath);
}

#ifndef NOEDIT
BOOL ActFile::save(BOOL promptForName)
{
  char **txt=0;
  BOOL result=FALSE;
  int fLine, ncas, i;

  if(!loaded) return FALSE;

  if(promptForName || *fileName==0)
  { DFitx *fitx;
	 BOOL result=FALSE;
	 int ext;

	 switch(actMode)
	 { case PUZZLE:  ext=EXT_PUZ; break;
		case ASSOCIA: ext=EXT_ASS; break;
		case SOPA:    ext=EXT_SOP; break;
		case CREUATS: ext=EXT_CRW; break;
		case TEXTACT: ext=EXT_TXA; break;
		default: return FALSE;
	 }

	 if((fitx=new DFitx(genMsgOwn, getStringTbl(IDS_SAVEFITX),
							  curActPath, fileName, fileDesc, ext, FALSE))!=0)
	 { if(fitx->run()==TRUE)
		{ strcpy(fileName, setPathVar(fitx->getFileName(), curActPath));
		  strcpy(fileDesc, fitx->getDesc());
		  result=TRUE;
		}
		delete fitx;
	 }
	 else NOMEM;
	 if(!result) return FALSE;
  }

  fLine=22;

  switch(actMode)
  { case PUZZLE:
			if((txt=newArray(fLine+5))!=0 &&
				(txt[fLine++]=dupStr(fileNameCont[0]))!=0 &&
				(txt[fLine++]=dupStr(itoa(ncw, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(nch, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(puzMode, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(stretch, txbuf, 10)))!=0) result=TRUE;
			break;

	 case ASSOCIA:
			ncas=ncw*nch;
			if((txt=newArray(fLine+9+(sol!=0)+(mAss ? ncas+2:0)))==NULL ||
				(txt[fLine++]=dupStr(itoa(puzMode, txbuf, 10)))==NULL ||
				(txt[fLine++]=dupStr(itoa(ncw, txbuf, 10)))==NULL ||
				(txt[fLine++]=dupStr(itoa(nch, txbuf, 10)))==NULL ||
				(txt[fLine++]=dupStr(fileNameCont[0]))==NULL ||
				(txt[fLine++]=dupStr(fileNameCont[1]))==NULL ||
				(txt[fLine++]=dupStr(itoa(bar[0], txbuf, 10)))==NULL ||
				(txt[fLine++]=dupStr(itoa(bar[1], txbuf, 10)))==NULL ||
				(txt[fLine++]=dupStr(itoa(sol, txbuf, 10)))==NULL ||
				( sol && (txt[fLine++]=dupStr(fileNameCont[2]))==NULL) ||
				(txt[fLine++]=dupStr(itoa(mAss, txbuf, 10)))==NULL) break;

			if(mAss)
			{ if((txt[fLine++]=dupStr(itoa(nctxw, txbuf, 10)))==NULL ||
				  (txt[fLine++]=dupStr(itoa(nctxh, txbuf, 10)))==NULL) break;

			  for(i=0; i<ncas; i++)
				 if((txt[fLine++]=dupStr(itoa(ass[i], txbuf, 10)))==NULL) break;

			  result=(i==ncas);
			}
			else result=TRUE;

			break;

	 case SOPA:
			if((txt=newArray(fLine+6+nctxh+ntags[0]))!=0 &&
				(txt[fLine++]=dupStr(bar[0] ? fileNameCont[0]:"*"))!=0 &&
				(txt[fLine++]=dupStr(itoa(ncw, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(nch, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(nctxw, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(nctxh, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(ntags[0], txbuf, 10)))!=0 &&
				copyArray(txt, fLine, graTxt, 0, nctxh, TRUE)==TRUE &&
				copyArray(txt, fLine+nctxh, tags[0], 0, ntags[0])==TRUE)
			  result=TRUE;
			break;

	 case CREUATS:
			if((txt=newArray(fLine+2+nctxh+ntags[0]+ntags[1]))!=0 &&
				(txt[fLine++]=dupStr(itoa(nctxw, txbuf, 10)))!=0 &&
				(txt[fLine++]=dupStr(itoa(nctxh, txbuf, 10)))!=0 &&
				copyArray(txt, fLine, graTxt, 0, nctxh, TRUE)==TRUE &&
				copyArray(txt, fLine+nctxh, tags[0], 0, ntags[0], FALSE)==TRUE &&
				copyArray(txt, fLine+nctxh+ntags[0],
				tags[1], 0, ntags[1], FALSE)==TRUE)
			  result=TRUE;
			break;

	 case TEXTACT:
			boolParms=0;
			setFlag(okToNext, 0x0001);
			setFlag(btCorregir, 0x0002);
			setFlag(avCont, 0x0004);
			setFlag(avMaj, 0x0008);
			setFlag(avAcc, 0x0010);
			setFlag(brPar, 0x0020);
			setFlag(avDblSpc, 0x0040);
			setFlag(avPunt, 0x0080);
			setFlag(avLletra, 0x0100);
			setFlag(avNoSalta, 0x0200);

			if((txt=newArray(fLine+11))!=0 &&

				logFontToString(logF[3], txbuf)==TRUE &&
				(txt[fLine++]=dupStr(txbuf))!=0 &&

				colorsToString(colors[3], txbuf)==TRUE &&
				(txt[fLine++]=dupStr(txbuf))!=0 &&

				sprintf(txbuf, "%06lX,%06lX,%06lX,%06lX",
					  colorUsuari[0], colorUsuari[1],
					  colorError[0], colorError[1])!=EOF &&

				(txt[fLine++]=dupStr(txbuf))!=0 &&
				sprintf(txbuf, "%i,%i,%i,%i,%i,%i,%i", puzMode, boolParms, avScope, avMaxScope, avPrevHelp, avTimePH, tabSpc)!=EOF &&
				(txt[fLine++]=dupStr(txbuf))!=0 &&
				(txt[fLine++]=dupStr(txBtCorregir))!=0 &&
				(txt[fLine++]=dupStr(txBase))!=0 &&
				(txt[fLine++]=dupStr(endDesc))!=0 &&
				(txt[fLine++]=dupStr(txPrev))!=0 &&
				(txt[fLine++]=dupStr(endDesc))!=0 &&
				(txt[fLine++]=dupStr(initMessPrev))!=0 &&
				(txt[fLine++]=dupStr(endDesc))!=0
				)
			  result=TRUE;
			break;


	 default: break;
  }

  if(result==FALSE ||
	  (txt[0]=dupStr(itoa(CLICVER, txbuf, 10)))==0 ||
	  (txt[1]=dupStr(fileDesc))==0 ||
	  (txt[2]=dupStr(endDesc))==0) result=FALSE;

  if(result==TRUE)
	 for(i=0; i<3; i++)
		if(logFontToString(logF[i], txbuf)!=TRUE ||
			(txt[3+2*i]=dupStr(txbuf))==0 ||
			colorsToString(colors[i], txbuf)!=TRUE ||
			(txt[4+2*i]=dupStr(txbuf))==0)
		{result=FALSE; break;}

  txt[9]=dupStr(bmpFons==0 ? nullStr:bmpFons);

  boolParms=0;
  setFlag(delim[0], 0x0001);
  setFlag(delim[1], 0x0002);
  setFlag(!shHelp, 0x0004);
  boolParms|=(graPos<<3);
  setFlag(!shPuz, 0x0020);
  setFlag(noBV, 0x0040);
  setFlag(shDisk, 0x0080);
  setFlag(shPorta, 0x0100);
  setFlag(barraAmunt, 0x0200);
  setFlag(invAss, 0x0400);
  setFlag(tileBmp, 0x0800);
  setFlag(pwTransp, 0x1000);
  setFlag(pwrp, 0x2000);
  setFlag(noAv, 0x4000);
  setFlag(shPrint, 0x8000);

  if(result==FALSE ||
	  sprintf(txbuf, "%i,%i,%i,%i,%06lX,%06lX,%i,%i,%i,%i,%i",
				  comptadors[0], comptadors[1], comptadors[2], marcs,
				  colorFons[0], colorFons[1], tramaFons[0], tramaFons[1],
				  btTipus, pwrx, pwry)==EOF ||
	  (txt[9]=dupStr(txbuf))==0 ||
	  (txt[10]=dupStr(bmpFons==0 ? nullStr:bmpFons))==0 ||
	  (txt[11]=dupStr(initMess))==0 ||
	  (txt[12]=dupStr(endMess))==0 ||
	  itoa(boolParms, txbuf, 10)!=txbuf ||
	  (txt[13]=dupStr(txbuf))==0 ||
	  sprintf(txbuf, "%i;%i", txtCW, txtCW2)==EOF ||
	  (txt[14]=dupStr(txbuf))==0 ||
	  sprintf(txbuf, "%i;%i", txtCH, txtCH2)==EOF ||
	  (txt[15]=dupStr(txbuf))==0 ||
	  itoa(custHlp, txbuf, 10)!=txbuf ||
	  (txt[16]=dupStr(txbuf))==0 ||
	  (txt[17]=dupStr(hlpTopic))==0 ||
	  (txt[18]=dupStr(custHelpFile))==0 ||
	  itoa(useDLL, txbuf, 10)!=txbuf ||
	  (txt[19]=dupStr(txbuf))==0 ||
	  (txt[20]=dupStr(rgDLL))==0 ||
	  (txt[21]=dupStr(dllOptions))==0 ||
	  writeTxtFile(getFullFileName(fileName), txt)==FALSE) result=FALSE;
  else modified=FALSE;

  delArray(txt);
  return result;

}
#else
BOOL ActFile::save(BOOL) {return TRUE;}
#endif

BOOL ActFile::noEvaluable()
{
  BOOL nev=FALSE;

  if(!loaded) return TRUE;

  else switch(actMode)
  { case ASSOCIA:
			if(puzMode==INFO || puzMode==EXPLORA) nev=TRUE;
			break;

	 case TEXTACT:
			if(btCorregir==FALSE && (puzMode!=FORATS || avCont==FALSE)) nev=TRUE;
			break;

	 default: break;
  }

  return nev;
}



// ----------------------------------------------------
// Paquet
// ----------------------------------------------------

Paquet::Paquet() {acts=0; chDef=chSup=chInf=NULL; clear();}

Paquet::~Paquet() {clear();}

Report *Paquet::baseRep=NULL;
int  Paquet::lastElRep=0;

void Paquet::clear()
{
  if(acts) {delArray(acts); acts=NULL;}
  memset(fileName, 0, sizeof(fileName));
  memset(fileDesc, 0, sizeof(fileDesc));
  nActs=0; act=0;
  autoPass=FALSE;
  noPassButtons=FALSE;
  noDiskButton=FALSE;
  noExitButton=FALSE;
  av800=FALSE;
  av256=FALSE;
  lapToPass=0;
  loaded=FALSE;
  modified=FALSE;
  def=sup=inf=supTFlag=infTFlag=FALSE;
  botoSep=FALSE;
  infT=supT=0;
  infP=25; supP=75;
  if(chDef) {delete chDef; chDef=NULL;}
  if(chSup) {delete chSup; chSup=NULL;}
  if(chInf) {delete chInf; chInf=NULL;}
}

Paquet& Paquet::operator = (const Paquet& pq)
{
  clear();
  memcpy(fileName, pq.fileName, sizeof(fileName));
  memcpy(fileDesc, pq.fileDesc, sizeof(fileDesc));

  if(pq.acts) acts=dupArray(pq.acts);

  act=pq.act;
  nActs=pq.nActs;
  loaded=pq.loaded;
  modified=pq.modified;
  autoPass=pq.autoPass;
  noPassButtons=pq.noPassButtons;
  noDiskButton=pq.noDiskButton;
  noExitButton=pq.noExitButton;
  lapToPass=pq.lapToPass;
  botoSep=pq.botoSep;
  av800=pq.av800;
  av256=pq.av256;

  def=pq.def; sup=pq.sup; inf=pq.inf;
  supTFlag=pq.supTFlag; infTFlag=pq.infTFlag;
  infP=pq.infP; infT=pq.infT;
  supP=pq.supP; supT=pq.supT;

  if(pq.chDef) chDef=dupStr(pq.chDef);
  if(pq.chSup) chSup=dupStr(pq.chSup);
  if(pq.chInf) chInf=dupStr(pq.chInf);

  return *this;
}

BOOL Paquet::load(char *name)
{
  char **txt=0;
  char *fn;
  char *str;
  int  nLin, fLine, ver;
  BOOL result=FALSE;

  clear();

  fn=name;
  strncpy(fileName, setPathVar(fn, curPacPath), 12);
  fn=getFullFileName(fileName, curPacPath);

  if(getExt(fn)==EXT_PCC)
  { if (openFC(fn)!=TRUE) return FALSE;
	 getFirstFCInto(fileName);
	 fn=getFullFileName(fileName, curPacPath);
  }

  fLine=0;
  ver=0;

  if((txt=readTxtFile(fn, nLin))!=0)
  { ver=atoi(txt[fLine++]);
	 if(ver>1000) ver-=1000;
	 if(ver>107 && ver<=CLICVER)
	 { fLine++;
		while(fLine<nLin)
		{ if((unsigned char)*txt[fLine++]==ENDDESC) break;
		  if(*fileDesc) strcat(fileDesc, "\r\n");
		  strncat(fileDesc, txt[fLine-1], MAX_DESC-strlen(fileDesc));
		}
		if(ver>=113)
		{ boolParms=atoi(txt[fLine++]);
		  autoPass=getFlag(0x0001);
		  noPassButtons=getFlag(0x0002);
		  noDiskButton=getFlag(0x0004);
		  noExitButton=getFlag(0x0008);
		  lapToPass=atoi(txt[fLine++]);

		  boolParms=atoi(txt[fLine++]);
		  def=getFlag(0x0001);
		  sup=getFlag(0x0002);
		  inf=getFlag(0x0004);
		  supTFlag=getFlag(0x0008);
		  infTFlag=getFlag(0x0010);
		  if(ver>117) botoSep=getFlag(0x0020);
		  if(ver>=119)
		  { av800=getFlag(0x0040);
			 av256=getFlag(0x0080);
		  }
		  str=txt[fLine++]; if(*str!=0) chDef=dupStr(str);
		  str=txt[fLine++]; if(*str!=0) chSup=dupStr(str);
		  str=txt[fLine++]; if(*str!=0) chInf=dupStr(str);
		  supP=atoi(txt[fLine++]);
		  supT=atoi(txt[fLine++]);
		  infP=atoi(txt[fLine++]);
		  infT=atoi(txt[fLine++]);
		}
	 }
	 if(fLine<nLin && ver<=CLICVER)
	 { nActs=nLin-fLine;
		if((acts=newArray(nActs))!=0 &&
			copyArray(acts, 0, txt, fLine, nActs, TRUE)) result=TRUE;
	 }
	 delArray(txt);

	 if(result==FALSE)
	 { BMsgBox(0, IDS_LECT, ver>CLICVER ? IDS_ERRNEWV:IDS_ERRFMT, fn);
		clear();
	 }
	 else
	 { loaded=TRUE;
		if(baseRep) lastElRep=baseRep->getCur();
	 }
  }
  else clear();

  return result;
}

#ifndef NOEDIT
BOOL Paquet::save(BOOL promptForName)
{
  char **txt=0;
  BOOL result=FALSE;

  if(isEmpty()) return FALSE;

  if(promptForName || *fileName==0)
  { DFitx *fitx;
	 BOOL result=FALSE;

	 if((fitx=new DFitx(genMsgOwn, getStringTbl(IDS_SAVEFITX),
				 curPacPath, fileName, fileDesc, EXT_PAC, FALSE))!=0)
	 { if(fitx->run()==TRUE)
		{ strcpy(fileName, setPathVar(fitx->getFileName(), curPacPath));
		  strcpy(fileDesc, fitx->getDesc());
		  result=TRUE;
		}
		delete fitx;
	 }
	 else NOMEM;
	 if(!result) return FALSE;
  }

  boolParms=0;
  setFlag(autoPass, 0x0001);
  setFlag(noPassButtons, 0x0002);
  setFlag(noDiskButton, 0x0004);
  setFlag(noExitButton, 0x0008);
  // --- aqui tallem boolParms: base 0x000F
  setFlag(def, 0x0010);
  setFlag(sup, 0x0020);
  setFlag(inf, 0x0040);
  setFlag(supTFlag, 0x0080);
  setFlag(infTFlag, 0x0100);
  setFlag(botoSep, 0x0200);
  setFlag(av800, 0x0400);
  setFlag(av256, 0x0800);

  if((txt=newArray(nActs+13))!=0)
  { if((txt[0]=dupStr(itoa(CLICVER, txbuf, 10)))==0 ||
		 (txt[1]=dupStr(fileDesc))==0 ||
		 (txt[2]=dupStr(endDesc))==0 ||
		 itoa(boolParms & 0x000F, txbuf, 10)!=txbuf ||
		 (txt[3]=dupStr(txbuf))==0 ||
		 itoa(lapToPass, txbuf, 10)!=txbuf ||
		 (txt[4]=dupStr(txbuf))==0 ||
		 itoa(boolParms>>4, txbuf, 10)!=txbuf ||
		 (txt[5]=dupStr(txbuf))==0 ||
		 (txt[6]=dupStr(def ? chDef : nullStr))==0 ||
		 (txt[7]=dupStr(sup ? chSup : nullStr))==0 ||
		 (txt[8]=dupStr(inf ? chInf : nullStr))==0 ||
		 (txt[9]=dupStr(itoa(supP, txbuf, 10)))==0 ||
		 (txt[10]=dupStr(itoa(supT, txbuf, 10)))==0 ||
		 (txt[11]=dupStr(itoa(infP, txbuf, 10)))==0 ||
		 (txt[12]=dupStr(itoa(infT, txbuf, 10)))==0 ||
		 copyArray(txt, 13, acts, 0, nActs, TRUE)==FALSE ||
		 writeTxtFile(getFullFileName(fileName, curPacPath), txt)==FALSE) result=FALSE;
	 else modified=FALSE;
	 delArray(txt);
  }
  return result;
}
#else
BOOL Paquet::save(BOOL) {return TRUE;}
#endif

BOOL Paquet::firstAct()
{ return act==0;}

BOOL Paquet::lastAct()
{ return act==(nActs-1) && !def;}

void Paquet::nextAct()
{ int prec, time;
  char *ch;

  if(act<nActs-1) act++;
  else if(def)
  { *tempPath3=0;
	 ch=chDef;
	 if(baseRep)
	 { baseRep->getDataFrom(lastElRep, prec, time);
		if(sup==TRUE &&
			chSup!=NULL &&
			prec>=supP &&
			(supTFlag==FALSE || time<=supT)) ch=chSup;
		else
		if(inf==TRUE &&
			chInf!=NULL &&
			(prec<=infP ||
			 (infTFlag==TRUE && time>=infT))) ch=chInf;
	 }
	 strcat(strcpy(tempPath3, curPacPath), ch);
	 load(tempPath3);
  }
}

void Paquet::prevAct()
{ if(act>0) act--;}

char *Paquet::getAct()
{
  *tempPath3=0;

  if(!acts) return tempPath3;

  if(!strchr(acts[act], ':'))
	 strcat(strcpy(tempPath3, curPacPath), acts[act]);
  else
	 strcpy(tempPath3, acts[act]);

  return tempPath3;
}

