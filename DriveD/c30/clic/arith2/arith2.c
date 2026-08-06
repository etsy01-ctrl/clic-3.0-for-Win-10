/* ------------------------------------------
	ARITH.C
	Generador d'activitats de càcul
	per al Clic 2.0
	------------------------------------------ */

#include "..\autoclic.h"
#include "arithrc.h"
#include <bwcc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mem.h>
#include <math.h>

#define DLL_TITLE "ARITH2.DLL"
#define ARITHVER 0x02

#define NMAXLOOPS 60
enum  ORDRES {NOSORT, SORTASC, SORTDESC};
enum  OPERACIONS {SUM=1, REST=2, MULT=4, DIV=8, NOPERACIONS=4};
char  opString[]={'+', '-', '×', ':'};
enum  TIPUS_EX {ABX=1, AXC=2, XBC=4, AXBC=8, CAXB=16, NTIPUSEX=5};
enum  OP_ESP {WZERO=1, WONE=2, WMINUSONE=4};
enum  OP_CONDS {INDIF, AGB, BGA};

#define NLIMITS 26
int limits[]={0, -9999, -1000, -999, -100, -99, -50, -25, -20, -10, -9, -5, -1, 0,
				  1, 5, 9, 10, 20, 25, 50, 99, 100, 999, 1000, 9999};

#define LIM0  13
#define LIM10 17
#define LIMI25 7
#define LIMS25 19
#define NOLIM 25
char *limCh[]={"x", "-9999", "-1000", "-999", "-100", "-99",
					"-50", "-25", "-20", "-10", "-9", "-5", "-1",
					"0", "1", "5", "9", "10", "20", "25",
					"50", "99", "100", "999", "1000", "9999"};
#define NUMLST 20

typedef struct tagOperand
{ int limInf;
  int limSup;
  int numDec;
  BOOL wZero, wOne, wMinusOne;
  int fromList;
  int lst[NUMLST];
} OPERAND;
typedef OPERAND FAR * LPOPERAND;

typedef struct tagCalcConf
{ OPERAND a;
  OPERAND b;
  BOOL BSum, BRest, BMult, BDiv;
  BOOL Babx, Baxc, Bxbc, Baxbc, Bcaxb;
  int  RlimInf, RlimSup;
  BOOL Rport;
  BOOL RnoDup;
  int  Rord;
  int  opCond;
  int  arithVer;
} CALCCONF;
typedef CALCCONF FAR * LPCALCCONF;

typedef struct tagNum
{ float vf;
  int   c;
} NUM;
typedef NUM FAR * LPNUM;

typedef struct tagOperacio
{ NUM a;
  NUM b;
  NUM r;
  int o;
} OPERACIO;
typedef OPERACIO FAR * LPOPERACIO;

BOOL calc(LPACTDATA lpAct, LPCALCCONF pcc);
BOOL CALLBACK ConfigureDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL LoadCalcConf(LPCALCCONF lpCC, LPSTR lpOps);
void adjustLimVer(int *l);
BOOL SaveCalcConf(LPSTR lpOps, LPCALCCONF lpCC);
BOOL CheckOkConf(HWND hDlg);
BOOL InitDlg(HWND hDlg);
BOOL ProcessCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);
#define RES -12345
BOOL genNum(LPNUM n, LPOPERAND op, long limInf2, long limSup2);
#define MAX_STR_LEN 100
char *LoadStr(UINT id);
BOOL genOp(LPCALCCONF cc, LPOPERACIO o);
void changeDot(char *str);

HINSTANCE dllHInst;
CALCCONF  cc;
char strBuffer[MAX_STR_LEN];

#define NCOMBOS 6
int combo[NCOMBOS]={IDC_LMINA, IDC_LMAXA,
						  IDC_LMINB, IDC_LMAXB,
						  IDC_LMINR, IDC_LMAXR};

BOOL help=FALSE;
char *helpFile="ARITH2.HLP";

#pragma argsused
int FAR PASCAL LibMain (HINSTANCE hInstance, WORD wDataSeg,
								WORD wHeapSize, LPSTR lpCmdLine)
{
	 dllHInst=hInstance;
	 if(wHeapSize!=0)    // if DLL data seg is MOVEABLE
		UnlockData(0);

	 return 1;
}

int FAR PASCAL GetClicDllVer()
{return CLIC_DLL_VERSION;}

BOOL LoadCalcConf(LPCALCCONF lpCC, LPSTR lpOps)
{
  unsigned char v;
  int i, lb, hb;
  BOOL fromBlank=FALSE;

  if((lpCC->a.limInf=*(lpOps++)&0x7F)==0)
  {fromBlank=TRUE; lpCC->a.limInf=LIM0;}
  lpCC->a.limSup=((i=*(lpOps++)&0x7F)==0 ? LIM10:i);
  lpCC->a.numDec=*(lpOps++)&0x3;
  v=*(lpOps++)&0x7F;
  lpCC->a.wZero = ((v & WZERO)!=0);
  lpCC->a.wOne  = ((v & WONE)!=0);
  lpCC->a.wMinusOne = ((v & WMINUSONE)!=0);

  lpCC->a.fromList =*(lpOps++)&0x7F;

  for(i=0; i<NUMLST; i++)
  { lb=*(lpOps++)&0x7F;
	 v=*(lpOps++)&0x7F;
	 hb=v&0x3F;
	 lpCC->a.lst[i]=lb+hb*128;
	 if((v&0x40)!=0) lpCC->a.lst[i]*=-1;
  }

  lpCC->b.limInf=((i=*(lpOps++)&0x7F)==0 ? LIM0:i);
  lpCC->b.limSup=((i=*(lpOps++)&0x7F)==0 ? LIM10:i);
  lpCC->b.numDec=*(lpOps++)&0x3;
  v=*(lpOps++)&0x7F;
  lpCC->b.wZero = (v & WZERO)!=0;
  lpCC->b.wOne  = (v & WONE)!=0;
  lpCC->b.wMinusOne = (v & WMINUSONE)!=0;

  lpCC->b.fromList = *(lpOps++)&0x7F;

  for(i=0; i<NUMLST; i++)
  { lb=*(lpOps++)&0x7F;
	 v=*(lpOps++)&0x7F;
	 hb=v&0x3F;
	 lpCC->b.lst[i]=lb+hb*128;
	 if((v&0x40)!=0) lpCC->b.lst[i]*=-1;
  }

  v=*(lpOps++)&0x7F;
  if(v==0) v=SUM;
  lpCC->BSum  = (v & SUM)!=0;
  lpCC->BRest = (v & REST)!=0;
  lpCC->BMult = (v & MULT)!=0;
  lpCC->BDiv  = (v & DIV)!=0;

  v=*(lpOps++)&0x7F;
  if(v==0) v=ABX;
  lpCC->Babx  = (v & ABX)!=0;
  lpCC->Baxc  = (v & AXC)!=0;
  lpCC->Bxbc  = (v & XBC)!=0;
  lpCC->Baxbc = (v & AXBC)!=0;
  lpCC->Bcaxb = (v & CAXB)!=0;

  lpCC->RlimInf=((i=*(lpOps++)&0x7F)==0 ? LIM0:i);
  lpCC->RlimSup=((i=*(lpOps++)&0x7F)==0 ? NOLIM:i);
  lpCC->Rport=*(lpOps++)&0x1;
  lpCC->RnoDup=*(lpOps++)&0x1;
  lpCC->Rord=*(lpOps++)&0x3;
  lpCC->opCond=*(lpOps++)&0x3;

  lpCC->arithVer=*(lpOps++)&0x7F;
  if(lpCC->arithVer==0)
  { lpCC->arithVer=ARITHVER;
	 if(!fromBlank)
	 { adjustLimVer(&lpCC->a.limInf);
		adjustLimVer(&lpCC->a.limSup);
		adjustLimVer(&lpCC->b.limInf);
		adjustLimVer(&lpCC->b.limSup);
		adjustLimVer(&lpCC->RlimInf);
		adjustLimVer(&lpCC->RlimSup);
	 }
  }

  if(lpCC->arithVer>ARITHVER)
  { BWCCMessageBox(NULL, LoadStr(IDS_BADVER), DLL_TITLE,
						 MB_OK|MB_ICONEXCLAMATION);
	 return FALSE;
  }

  return TRUE;
}

void adjustLimVer(int *l)
{ if(*l>=LIMI25) (*l)++;
  if(*l>=LIMS25) (*l)++;
}

BOOL SaveCalcConf(LPSTR lpOps, LPCALCCONF lpCC)
{
  int i, k, lb, hb;
  unsigned v;

  *(lpOps++)=lpCC->a.limInf|0x80;
  *(lpOps++)=lpCC->a.limSup|0x80;
  *(lpOps++)=lpCC->a.numDec|0x80;

  v=(lpCC->a.wZero!=0) * WZERO +
	 (lpCC->a.wOne!=0) * WONE +
	 (lpCC->a.wMinusOne!=0) * WMINUSONE;
  *(lpOps++)= v | 0x80;

  *(lpOps++)=lpCC->a.fromList|0x80;

  for(i=0; i<NUMLST; i++)
  { k=lpCC->a.lst[i];
	 if(k<0) {v=0xC0; k*=-1;}
	 else v=0x80;
	 lb=k&0x7F;
	 k/=128;
	 hb=k&0x7F;
	 *(lpOps++)=lb | 0x80;
	 *(lpOps++)=hb | v;
  }

  *(lpOps++)=lpCC->b.limInf|0x80;
  *(lpOps++)=lpCC->b.limSup|0x80;
  *(lpOps++)=lpCC->b.numDec|0x80;

  v=(lpCC->b.wZero!=0) * WZERO +
	 (lpCC->b.wOne!=0) * WONE +
	 (lpCC->b.wMinusOne!=0) * WMINUSONE;

  *(lpOps++)= v | 0x80;

  *(lpOps++)=lpCC->b.fromList|0x80;

  for(i=0; i<NUMLST; i++)
  { k=lpCC->b.lst[i];
	 if(k<0) {v=0xC0; k*=-1;}
	 else v=0x80;
	 lb=k&0x7F;
	 k/=128;
	 hb=k&0x7F;
	 *(lpOps++)=lb | 0x80;
	 *(lpOps++)=hb | v;
  }

  v=(lpCC->BSum!=0) * SUM +
	 (lpCC->BRest!=0) * REST +
	 (lpCC->BMult!=0) * MULT +
	 (lpCC->BDiv!=0) * DIV;

  *(lpOps++)= v | 0x80;

  v = (lpCC->Babx!=0) * ABX +
		(lpCC->Baxc!=0) * AXC +
		(lpCC->Bxbc!=0) * XBC +
		(lpCC->Baxbc!=0) * AXBC +
		(lpCC->Bcaxb!=0) * CAXB;

  *(lpOps++)= v | 0x80;

  *(lpOps++)=lpCC->RlimInf|0x80;
  *(lpOps++)=lpCC->RlimSup|0x80;
  *(lpOps++)=(lpCC->Rport!=0)|0x80;
  *(lpOps++)=(lpCC->RnoDup!=0)|0x80;
  *(lpOps++)=lpCC->Rord|0x80;
  *(lpOps++)=lpCC->opCond|0x80;

  *(lpOps++)=lpCC->arithVer|0x80;
  
  return TRUE;
}

#pragma argsused
BOOL FAR PASCAL ConfigureDllOptions(HWND hOwner, LPACTDATA lpAct, LPSTR lpOps)
{
  if(lpOps==NULL || lpAct==NULL) return FALSE;
  if(!IsValidAct(hOwner, lpAct, TRUE)) return FALSE;

  if(!LoadCalcConf(&cc, lpOps)) return FALSE;

  if(DialogBox(dllHInst, MAKEINTRESOURCE(DIALEG_CONFIGURA), hOwner,
					ConfigureDlgProc)==TRUE)
	 SaveCalcConf(lpOps, &cc);

  return TRUE;
}


BOOL FAR PASCAL IsValidAct(HWND hOwner, LPACTDATA lpAct, BOOL showMsg)
{
  BOOL result=FALSE;

  switch(*lpAct->actMode)
  { case PUZZLE:
			result=TRUE;
			break;

	 case ASSOCIA:
			switch(*lpAct->actType)
			{ case NORMAL:
			  case ESPECIAL:
			  case ESCRIU:
			  case IDENTIFICA:
			  case EXPLORA:
					 result=TRUE;
					 break;
			  default: break;
			}
			break;

	 default: break;
  }

  if(!result && showMsg)
	 BWCCMessageBox(hOwner, LoadStr(IDS_INVACT), DLL_TITLE,
						 MB_OK|MB_ICONEXCLAMATION);

  return result;
}

BOOL FAR PASCAL RemakeAct(LPSTR fileNameAct,
								  LPACTDATA lpAct, LPSTR lpOps)
{
  if(fileNameAct==NULL || lpAct==NULL || lpOps==NULL ||
	  *lpAct->rows==0 || *lpAct->columns==0 ||
	  lpAct->tempFName[0]==NULL ||
	  lpAct->tempFName[1]==NULL ||
	  lpAct->tempFName[2]==NULL) return FALSE;

  if(!IsValidAct(NULL, lpAct, TRUE)) return FALSE;

  if(!LoadCalcConf(&cc, lpOps)) return FALSE;
  randomize();
  return calc(lpAct, &cc);
}

typedef char BUFTX[41];
typedef BUFTX far * LPBUFTX;

#pragma argsused
BOOL calc(LPACTDATA lpAct, LPCALCCONF pcc)
{
  OPERACIO o;
  LPOPERACIO op;
  int nLines, i, j, k;
  FILE *fa, *fb, *fra;
  HGLOBAL hg, hbt;
  int tipus[NTIPUSEX], numTipus, tipX;
  BOOL tipInv;
  LPBUFTX bt;
  char opa[80], opb[80], opc[80], opo[10];

  if((nLines=*lpAct->rows * *lpAct->columns)<2) return FALSE;

  numTipus=0;
  if(pcc->Babx)  tipus[numTipus++]=ABX;
  if(pcc->Baxc)  tipus[numTipus++]=AXC;
  if(pcc->Bxbc)  tipus[numTipus++]=XBC;
  if(pcc->Baxbc) tipus[numTipus++]=AXBC;
  if(numTipus==0) return FALSE;
  tipInv=pcc->Bcaxb;

  if((fa=fopen(lpAct->tempFName[0], "w+"))==NULL ||
	  (fb=fopen(lpAct->tempFName[1], "w+"))==NULL ||
	  (fra=fopen(lpAct->tempFName[2], "w+"))==NULL) return FALSE;

  if((hg=GlobalAlloc(GHND, nLines*sizeof(OPERACIO)))==NULL ||
	  (op=(LPOPERACIO)GlobalLock(hg))==NULL ||
	  (hbt=GlobalAlloc(GHND, nLines*sizeof(BUFTX)))==NULL ||
	  (bt=(LPBUFTX)GlobalLock(hbt))==NULL)
  { fclose(fa); fclose(fb); fclose(fra);
	 return FALSE;
  }

  for(i=0; i<nLines; i++)
  { for(j=0; j<NMAXLOOPS; j++)
	 { genOp(pcc, &o);
		if(pcc->RnoDup)
		{ for(k=0; k<i; k++)
			 if(o.r.vf==op[i].r.vf) break;
		  if(k==i) break;
		}
		else break;
	 }
	 op[i]=o;
  }

  if(pcc->Rord)
	 for(i=nLines-1; i>0; i--)
		for(j=0; j<i; j++)
		  if((pcc->Rord==SORTASC && op[j].r.vf > op[j+1].r.vf) ||
			  (pcc->Rord==SORTDESC && op[j].r.vf < op[j+1].r.vf))
		  { o=op[j];
			 op[j]=op[j+1];
			 op[j+1]=o;
		  }

  for(i=0; i<nLines; i++)
  { tipX=tipus[random(numTipus)];
	 sprintf(opa, "%.*f", op[i].a.c, op[i].a.vf); changeDot(opa);
	 sprintf(opb, "%.*f", op[i].b.c, op[i].b.vf); changeDot(opb);
	 sprintf(opc, "%.*f", op[i].r.c, op[i].r.vf); changeDot(opc);
	 sprintf(opo, "%c", opString[op[i].o]);

	 if(tipInv)
		fprintf(fra, "%s = %s %s %s\n", opc, opa, opo, opb);
	 else
		fprintf(fra, "%s %s %s = %s\n", opa, opo, opb, opc);


	 switch(tipX)
	 { case AXC:
			  sprintf(bt[i], "%s\n", opb);
			  if(tipInv)
				 fprintf(fa, "%s = %s %s ?\n", opc, opa, opo);
			  else
				 fprintf(fa, "%s %s ? = %s\n", opa, opo, opc);
			  break;

		case XBC:
			  sprintf(bt[i], "%s\n", opa);
			  if(tipInv)
				 fprintf(fa, "%s = ? %s %s\n", opc, opo, opb);
			  else
				 fprintf(fa, "? %s %s = %s\n", opo, opb, opc);
			  break;

		case AXBC:
			  sprintf(bt[i], "%s\n", opo);
			  if(tipInv)
				 fprintf(fa, "%s = %s ? %s\n", opc, opa, opb);
			  else
				 fprintf(fa, "%s ? %s = %s\n", opa, opb, opc);
			  break;

		default:
			  sprintf(bt[i], "%s\n", opc);
			  if(tipInv)
				 fprintf(fa, "? = %s %s %s\n", opa, opo, opb);
			  else
				 fprintf(fa, "%s %s %s =\n", opa, opo, opb);
			  break;
	 }

  }

  if(*lpAct->actMode==ASSOCIA &&
	  (*lpAct->actType==ESPECIAL || *lpAct->actType==EXPLORA))
  { k=0;
	 for(i=0; i<nLines; i++)
	 { for(j=0; j<i; j++) if(strcmp(bt[i], bt[j])==NULL) break;
		if(j==i)
		{ fputs(bt[i], fb);
		  lpAct->ass[i]=k;
		  k++;
		}
		else lpAct->ass[i]=lpAct->ass[j];
	 }
	 switch(k)
	 { case 6: *lpAct->rows2=3;
				  *lpAct->columns2=2;
				  break;

		case 8: *lpAct->rows2=4;
				  *lpAct->columns2=2;
				  break;

		case 9: *lpAct->rows2=3;
				  *lpAct->columns2=3;
				  break;

		case 10: *lpAct->rows2=5;
					*lpAct->columns2=2;
					break;

		case 12: *lpAct->rows2=4;
					*lpAct->columns2=3;
					break;

		case 14: *lpAct->rows2=7;
					*lpAct->columns2=2;
					break;

		case 15: *lpAct->rows2=5;
					*lpAct->columns2=3;
					break;

		default: *lpAct->rows2=k;
					*lpAct->columns2=1;
					break;
	 }
  }
  else for(i=0; i<nLines; i++) fputs(bt[i], fb);

  fclose(fa);
  fclose(fb);
  fclose(fra);

  GlobalUnlock(hg);
  GlobalFree(hg);

  GlobalUnlock(hbt);
  GlobalFree(hbt);

  for(i=0; i<3; i++) lpAct->useTempFName[i]=TRUE;

  return TRUE;
}


BOOL CALLBACK ConfigureDlgProc (HWND hDlg, UINT msg,
										  WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  { case WM_INITDIALOG:
			InitDlg(hDlg);
			return TRUE;

	 case WM_COMMAND:
			switch (wParam)
			{ case IDHELP:
					 help=WinHelp(hDlg, helpFile, HELP_CONTENTS, 0L);
					 break;

			  case IDOK:
					 if(!CheckOkConf(hDlg)) break;
					 if(help) WinHelp(hDlg, helpFile, HELP_QUIT, 0L);
					 EndDialog(hDlg, TRUE);  break;

			  case IDCANCEL:
					 if(help) WinHelp(hDlg, helpFile, HELP_QUIT, 0L);
					 EndDialog(hDlg, FALSE); break;

			  default:
					 ProcessCommand(hDlg, wParam, lParam);
					 break;
			}
			break;

	 default:
			break;
  }
  return FALSE;
}

BOOL InitDlg(HWND hDlg)
{
  int i, j;
  char txt[255];
  char *tx;

  CheckDlgButton(hDlg, IDC_SUM, cc.BSum);
  CheckDlgButton(hDlg, IDC_REST, cc.BRest);
  CheckDlgButton(hDlg, IDC_MULT, cc.BMult);
  CheckDlgButton(hDlg, IDC_DIV, cc.BDiv);

  CheckDlgButton(hDlg, IDC_ABX, cc.Babx);
  CheckDlgButton(hDlg, IDC_AXC, cc.Baxc);
  CheckDlgButton(hDlg, IDC_XBC, cc.Bxbc);
  CheckDlgButton(hDlg, IDC_AXBC, cc.Baxbc);
  CheckDlgButton(hDlg, IDC_CAXB, cc.Bcaxb);

  for(i=1; i<NLIMITS; i++)
	 for(j=0; j<NCOMBOS; j++)
		SendDlgItemMessage(hDlg, combo[j], CB_ADDSTRING, 0, (LPARAM)limCh[i]);

  CheckDlgButton(hDlg, IDC_A0, cc.a.wZero);
  CheckDlgButton(hDlg, IDC_A1, cc.a.wOne);
  CheckDlgButton(hDlg, IDC_A_1, cc.a.wMinusOne);
  CheckDlgButton(hDlg, IDC_CHLSA, cc.a.fromList);

  *txt=0;
  for(tx=txt, i=0; i<cc.a.fromList; tx+=strlen(tx), i++)
	 sprintf(tx, "%d;", cc.a.lst[i]);
  SetDlgItemText(hDlg, IDC_CHEDA, txt);

  SendDlgItemMessage(hDlg, IDC_LMINA, CB_SETCURSEL, cc.a.limInf-1, 0L);
  SendDlgItemMessage(hDlg, IDC_LMAXA, CB_SETCURSEL, cc.a.limSup-1, 0L);
  EnableWindow(GetDlgItem(hDlg, IDC_CHEDA), cc.a.fromList!=0);
  EnableWindow(GetDlgItem(hDlg, IDC_LMINA), cc.a.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_LMAXA), cc.a.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_A0), cc.a.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_A1), cc.a.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_A_1), cc.a.fromList==0);
  CheckRadioButton(hDlg, IDC_AD0, IDC_AD2, IDC_AD0+cc.a.numDec);

  CheckDlgButton(hDlg, IDC_B0, cc.b.wZero);
  CheckDlgButton(hDlg, IDC_B1, cc.b.wOne);
  CheckDlgButton(hDlg, IDC_B_1, cc.b.wMinusOne);
  CheckDlgButton(hDlg, IDC_CHLSB, cc.b.fromList);

  *txt=0;
  for(tx=txt, i=0; i<cc.b.fromList; tx+=strlen(tx), i++)
	 sprintf(tx, "%d;", cc.b.lst[i]);
  SetDlgItemText(hDlg, IDC_CHEDB, txt);

  SendDlgItemMessage(hDlg, IDC_LMINB, CB_SETCURSEL, cc.b.limInf-1, 0L);
  SendDlgItemMessage(hDlg, IDC_LMAXB, CB_SETCURSEL, cc.b.limSup-1, 0L);
  EnableWindow(GetDlgItem(hDlg, IDC_CHEDB), cc.b.fromList!=0);
  EnableWindow(GetDlgItem(hDlg, IDC_LMINB), cc.b.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_LMAXB), cc.b.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_B0), cc.b.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_B1), cc.b.fromList==0);
  EnableWindow(GetDlgItem(hDlg, IDC_B_1), cc.b.fromList==0);
  CheckRadioButton(hDlg, IDC_BD0, IDC_BD2, IDC_BD0+cc.b.numDec);

  SendDlgItemMessage(hDlg, IDC_LMINR, CB_SETCURSEL, cc.RlimInf-1, 0L);
  SendDlgItemMessage(hDlg, IDC_LMAXR, CB_SETCURSEL, cc.RlimSup-1, 0L);

  CheckDlgButton(hDlg, IDC_NOP, cc.Rport);
  CheckRadioButton(hDlg, IDC_NOSORT, IDC_SORTDESC, IDC_NOSORT+cc.Rord);
  CheckDlgButton(hDlg, IDC_NOREP, cc.RnoDup);

  CheckRadioButton(hDlg, IDC_AIB, IDC_BGA, IDC_AIB+cc.opCond);

  return TRUE;
}

BOOL ProcessCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
  BOOL l;

  switch (wParam)
  { case IDC_ABX: cc.Babx^=1; break;
	 case IDC_AXC: cc.Baxc^=1; break;
	 case IDC_XBC: cc.Bxbc^=1; break;
	 case IDC_AXBC: cc.Baxbc^=1; break;
	 case IDC_CAXB: cc.Bcaxb^=1; break;

	 case IDC_SUM: cc.BSum^=1; break;
	 case IDC_REST: cc.BRest^=1; break;
	 case IDC_MULT: cc.BMult^=1; break;
	 case IDC_DIV: cc.BDiv^=1; break;

	 case IDC_A0: cc.a.wZero^=1; break;
	 case IDC_A1: cc.a.wOne^=1; break;
	 case IDC_A_1: cc.a.wMinusOne^=1; break;
	 case IDC_CHLSA:
			l=IsDlgButtonChecked(hDlg, IDC_CHLSA);
			EnableWindow(GetDlgItem(hDlg, IDC_CHEDA), l);
			EnableWindow(GetDlgItem(hDlg, IDC_LMINA), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_LMAXA), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_A0), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_A1), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_A_1), !l);
			break;

	 case IDC_B0: cc.b.wZero^=1; break;
	 case IDC_B1: cc.b.wOne^=1; break;
	 case IDC_B_1: cc.b.wMinusOne^=1; break;
	 case IDC_CHLSB:
			l=IsDlgButtonChecked(hDlg, IDC_CHLSB);
			EnableWindow(GetDlgItem(hDlg, IDC_CHEDB), l);
			EnableWindow(GetDlgItem(hDlg, IDC_LMINB), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_LMAXB), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_B0), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_B1), !l);
			EnableWindow(GetDlgItem(hDlg, IDC_B_1), !l);
			break;

	 case IDC_NOSORT: cc.Rord=NOSORT; break;
	 case IDC_SORTASC: cc.Rord=SORTASC; break;
	 case IDC_SORTDESC: cc.Rord=SORTDESC; break;
	 case IDC_NOREP: cc.RnoDup^=1; break;
	 case IDC_NOP: cc.Rport^=1; break;

	 case IDC_AD0:
	 case IDC_AD1:
	 case IDC_AD2:
			cc.a.numDec=wParam-IDC_AD0;
			break;

	 case IDC_BD0:
	 case IDC_BD1:
	 case IDC_BD2:
			cc.b.numDec=wParam-IDC_BD0;
			break;

	 case IDC_AGB: cc.opCond=AGB; break;
	 case IDC_BGA: cc.opCond=BGA; break;
	 case IDC_AIB: cc.opCond=INDIF; break;

	 default: break;
  }

  if(HIWORD(lParam)==CBN_SELCHANGE)
  { int cbmax=0, cbmin=0;
	 int max, min, c;
	 int *stmax, *stmin;

	 switch(LOWORD(wParam))
	 { case IDC_LMINA:
		case IDC_LMAXA:
			  cbmax=IDC_LMAXA; cbmin=IDC_LMINA;
			  stmin=&cc.a.limInf; stmax=&cc.a.limSup;
			  break;

		case IDC_LMINB:
		case IDC_LMAXB:
			  cbmax=IDC_LMAXB; cbmin=IDC_LMINB;
			  stmin=&cc.b.limInf; stmax=&cc.b.limSup;
			  break;

		case IDC_LMINR:
		case IDC_LMAXR:
			  cbmax=IDC_LMAXR; cbmin=IDC_LMINR;
			  stmin=&cc.RlimInf; stmax=&cc.RlimSup;
			  break;

		default: break;
	 }
	 if(cbmax!=0)
	 { max=(UINT)SendDlgItemMessage(hDlg, cbmax, CB_GETCURSEL, 0, 0L);
		min=(UINT)SendDlgItemMessage(hDlg, cbmin, CB_GETCURSEL, 0, 0L);
		if(max<min)
		{ c=max; max=min; min=c;
		  SendDlgItemMessage(hDlg, cbmax, CB_SETCURSEL, max, 0L);
		  SendDlgItemMessage(hDlg, cbmin, CB_SETCURSEL, min, 0L);
		}
		*stmax=max+1;
		*stmin=min+1;
	 }
  }
  return TRUE;
}

BOOL CheckOkConf(HWND hDlg)
{
	char txt[255];
	char *tx;
	int i;

	if(IsDlgButtonChecked(hDlg, IDC_CHLSA))
	{ GetDlgItemText(hDlg, IDC_CHEDA, txt, sizeof(txt));
	  for(i=0; i<NUMLST; i++)
	  { tx=strtok(i==0 ? txt:NULL, ";");
		 if(tx==NULL) break;
		 cc.a.lst[i]=atoi(tx);
	  }
	  cc.a.fromList=i;
	  if(cc.a.fromList==0)
	  { BWCCMessageBox(hDlg, LoadStr(IDS_ERRLIM), DLL_TITLE,
							 MB_OK|MB_ICONEXCLAMATION);
		 return FALSE;
	  }
	}
	else
	  cc.a.fromList=0;

	if(IsDlgButtonChecked(hDlg, IDC_CHLSB))
	{ GetDlgItemText(hDlg, IDC_CHEDB, txt, sizeof(txt));
	  for(i=0; i<NUMLST; i++)
	  { tx=strtok(i==0 ? txt:NULL, ";");
		 if(tx==NULL) break;
		 cc.b.lst[i]=atoi(tx);
	  }
	  cc.b.fromList=i;
	  if(cc.b.fromList==0)
	  { BWCCMessageBox(hDlg, LoadStr(IDS_ERRLIM), DLL_TITLE,
							 MB_OK|MB_ICONEXCLAMATION);
		 return FALSE;
	  }
	}
	else
	  cc.b.fromList=0;


  if(!cc.BSum && !cc.BRest && !cc.BMult && !cc.BDiv)
  { BWCCMessageBox(hDlg, LoadStr(IDS_NOOP), DLL_TITLE,
							 MB_OK|MB_ICONEXCLAMATION);
	 return FALSE;
  }

  if(!cc.Babx && !cc.Baxc && !cc.Bxbc && !cc.Baxbc)
  { BWCCMessageBox(hDlg, LoadStr(IDS_NOINCOG), DLL_TITLE,
							 MB_OK|MB_ICONEXCLAMATION);
	 return FALSE;
  }

  if(cc.Rport &&
	  ((!cc.BSum && !cc.BRest) || cc.a.limInf<LIM0 || cc.b.limInf<LIM0))
	 BWCCMessageBox(hDlg, LoadStr(IDS_NOPORT), DLL_TITLE,
								 MB_OK|MB_ICONEXCLAMATION);

  return TRUE;
}


char *LoadStr(UINT id)
{
  *strBuffer=0;
  LoadString(dllHInst, id, strBuffer, MAX_STR_LEN);
  return strBuffer;
}

BOOL genNum(LPNUM n, LPOPERAND op, long limInf2, long limSup2)
{
  int r, exp, rang;
  long ls, li, k, v;
  BOOL resolt=FALSE;

  n->c=op->numDec;
  exp=n->c==0 ? 1 : n->c==1 ? 10 : 100;
  ls=limits[op->limSup]; if(limSup2!=RES && limSup2<ls) ls=limSup2;
  li=limits[op->limInf]; if(limInf2!=RES && limInf2>li) li=limInf2;

  resolt=FALSE;
  if(op->fromList)
  { n->vf=(long)op->lst[random(op->fromList)];
	 resolt=TRUE;
  }
  if(!resolt)
  { r=random(100);
	 if(op->wZero && r<=10) {n->vf=0; resolt=TRUE;}
	 else if(op->wOne && r>10 && r<=20) {n->vf=1; resolt=TRUE;}
	 else if(op->wMinusOne && r>20 && r<=30)
	 {n->vf=-1; resolt=TRUE;}
  }
  if(!resolt)
  { if(li>ls) {k=li; li=ls; ls=k;}
	 rang=(int)(ls-li+1);
	 if(rang<0) rang=1;
	 v = (long)(random(rang)+li) * exp;
	 if (exp>1) v+=random(exp);
	 n->vf=((float)v)/exp;
	 resolt=TRUE;
  }

  return TRUE;
}


BOOL genOp(LPCALCCONF cc, LPOPERACIO o)
{
  int i;
  int ops[NOPERACIONS], nops, op;
  long rlinf, rlsup, ri2, rs2;
  int a, b;
  float q;
  char bufa[20], bufb[20];

  rlinf=limits[cc->RlimInf];
  rlsup=limits[cc->RlimSup];

  nops=0;
  if(cc->BSum) ops[nops++]=SUM;
  if(cc->BRest) ops[nops++]=REST;
  if(cc->BMult) ops[nops++]=MULT;
  if(cc->BDiv) ops[nops++]=DIV;

  op=ops[random(nops)];
  switch(op)
  { case SUM:
			for(i=0; i<NMAXLOOPS; i++)
			{ genNum(&o->a, &cc->a, RES, rlsup);
			  ri2= o->a.vf<rlinf ? rlinf-(long)o->a.vf:RES;
			  rs2=rlsup-(long)o->a.vf;
			  switch(cc->opCond)
			  { case AGB:
						if(rs2==RES || rs2>o->a.vf) rs2=(long)o->a.vf;
						break;
				 case BGA:
						if(ri2==RES || ri2<o->a.vf) ri2=(long)o->a.vf;
						break;
				 default: break;
			  }
			  genNum(&o->b, &cc->b, ri2, rs2);
			  o->r.vf=o->a.vf+o->b.vf;
			  if(o->r.vf>=rlinf && o->r.vf<=rlsup) break;
			}
			o->r.c = o->a.c > o->b.c ? o->a.c : o->b.c;
			o->o=0;
			if(cc->Rport && o->a.vf>0 && o->b.vf>0)
			// versio nova
			{ q=pow10(o->r.c);
			  sprintf(bufa, "%010d", (long)(o->a.vf*q+0.5));
			  sprintf(bufb, "%010d", (long)(o->b.vf*q+0.5));
			  for(i=0; i<10; i++) if(bufa[i]!='0' || bufb[i]!='0') break;
			  for(i++; i<10; i++)
			  { a=bufa[i]-'0'; b=bufb[i]-'0';
				 if(a+b<10) continue;
				 //while(a+b>9) if(a>b) a--; else b--;
				 while(a+b>9) if(a>b) a=random(a); else b=random(b);
				 bufa[i]=a+'0'; bufb[i]=b+'0';
			  }
			  o->a.vf=atol(bufa);
			  o->b.vf=atol(bufb);
			  o->r.vf=(float)(long)(o->a.vf + o->b.vf + 0.5);

			  o->a.vf/=q;
			  o->b.vf/=q;
			  o->r.vf/=q;
			}
			// Versio antiga
			/*
			{ q=pow10(o->r.c);
			  sprintf(bufa, "%010f", o->a.vf*q);
			  sprintf(bufb, "%010f", o->b.vf*q);
			  for(i=0; i<10; i++) if(bufa[i]!='0' || bufb[i]!='0') break;
			  for(i++; i<10; i++)
			  { a=bufa[i]-'0'; b=bufb[i]-'0';
				 if(a+b<10) continue;
				 while(a+b>9) if(a>b) a--; else b--;
				 bufa[i]=a+'0'; bufb[i]=b+'0';
			  }
			  o->a.vf=atof(bufa)/q;
			  o->b.vf=atof(bufb)/q;
			  o->r.vf=o->a.vf+o->b.vf;
			}
			*/
			break;

	 case REST:
			for(i=0; i<NMAXLOOPS; i++)
			{ genNum(&o->a, &cc->a, rlinf, RES);
			  ri2= o->a.vf > rlsup ? o->a.vf - rlsup : RES;
			  rs2= o->a.vf - rlinf;
			  switch(cc->opCond)
			  { case AGB:
						if(rs2==RES || rs2>o->a.vf) rs2=(long)o->a.vf;
						break;
				 case BGA:
						if(ri2==RES || ri2<o->a.vf) ri2=(long)o->a.vf;
						break;
				 default: break;
			  }
			  genNum(&o->b, &cc->b, ri2, rs2);
			  o->r.vf=o->a.vf-o->b.vf;
			  if(o->r.vf>=rlinf && o->r.vf<=rlsup) break;
			}
			o->r.c = o->a.c > o->b.c ? o->a.c : o->b.c;
			o->o=1;
			if(cc->Rport && o->a.vf>0 && o->b.vf>0 && o->a.vf>=o->b.vf)
			// versio nova
			{ q=pow10(o->r.c);
			  sprintf(bufa, "%010d", (long)(o->a.vf*q+0.5));
			  sprintf(bufb, "%010d", (long)(o->b.vf*q+0.5));
			  for(i=0; i<10; i++) if(bufb[i]!='0') break;
			  for(; i<10; i++)
			  { a=bufa[i]-'0'; b=bufb[i]-'0';
				 if(a>=b) continue;
				 b=random(a);
				 bufb[i]=b+'0';
			  }

			  o->a.vf=atol(bufa);
			  o->b.vf=atol(bufb);
			  o->r.vf=(float)(long)(o->a.vf - o->b.vf + 0.5);

			  o->a.vf/=q;
			  o->b.vf/=q;
			  o->r.vf/=q;
			}
			// versio antiga
			/*
			{ q=pow10(o->r.c);
			  sprintf(bufa, "%010f", o->a.vf*q);
			  sprintf(bufb, "%010f", o->b.vf*q);
			  for(i=0; i<10; i++) if(bufb[i]!='0') break;
			  for(; i<10; i++)
			  { a=bufa[i]-'0'; b=bufb[i]-'0';
				 if(a>=b) continue;
				 b=random(a);
				 bufb[i]=b+'0';
			  }
			  o->b.vf=atof(bufb)/q;
			  o->r.vf=o->a.vf-o->b.vf;
			}
			*/
			break;

	 case MULT:
			for(i=0; i<NMAXLOOPS; i++)
			{ genNum(&o->a, &cc->a, RES, RES);
			  ri2= limits[cc->b.limInf];
			  rs2= limits[cc->b.limSup];
			  switch(cc->opCond)
			  { case AGB:
						if(rs2>o->a.vf) rs2=(long)o->a.vf;
						break;
				 case BGA:
						if(ri2<o->a.vf) ri2=(long)o->a.vf;
						break;
				 default: break;
			  }
			  genNum(&o->b, &cc->b, ri2, rs2);
			  o->r.vf=o->a.vf * o->b.vf;
			  if(o->r.vf>=rlinf && o->r.vf<=rlsup) break;
			}
			o->r.c = o->a.c + o->b.c;
			o->o=2;
			break;

	 case DIV:
			for(i=0; i<NMAXLOOPS; i++)
			{ genNum(&o->a, &cc->a, RES, RES);
			  ri2= limits[cc->b.limInf];
			  rs2= limits[cc->b.limSup];
			  switch(cc->opCond)
			  { case AGB:
						if(rs2>o->a.vf) rs2=(long)o->a.vf;
						break;
				 case BGA:
						if(ri2<o->a.vf) ri2=(long)o->a.vf;
						break;
				 default: break;
			  }
			  genNum(&o->b, &cc->b, ri2, rs2);
			  if(o->b.vf!=0 &&
				  fabs(o->a.vf)>=fabs(o->b.vf) &&
				  (o->r.vf=o->a.vf/o->b.vf)>=rlinf && o->r.vf<=rlsup) break;
			}
			if(o->b.vf==0) o->b.vf=1;
			o->r.vf=o->a.vf / o->b.vf;
			i=o->a.c - o->b.c;
			q=pow10(i);
			o->a.vf*=q;
			o->r.vf*=q;
			o->r.vf=((long)o->r.vf);
			o->a.vf=o->r.vf*o->b.vf;
			o->a.vf/=q;
			o->r.vf/=q;
			o->r.c = i>0 ? i : 0;
			o->o=3;
			break;

	 default:
			return FALSE;
  }

  return TRUE;
}

void changeDot(char *str)
{ char *p;

  if((p=strchr(str, '.'))!=NULL) *p=',';
}


