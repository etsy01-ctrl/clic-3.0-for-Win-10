// ------------------------------------------
//	CLICRND.C
//	Automation DLL Sample for Clic 3.0
//	------------------------------------------
//
// This DLL builds a random list of lettres and/or numbers
// and assigns each member to "VOWEL", "CONSONANT" or "NUMBER"

#include "autoclic.h"
#include "clicrndr.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DLL_TITLE "CLICRND.DLL"
#define CLICRNDVER 0x02
#define MAX_STR_LEN 200

/* ------------------------------
	Special data structure
	used to store basic parameters
	of operation
	------------------------------ */

typedef struct tagRndConf
{ int  version;
  BOOL vowels;
  BOOL consonants;
  BOOL numbers;
} RNDCONF;
typedef RNDCONF FAR * LPRNDCONF;

BOOL buildActivity(LPACTDATA lpAct, LPRNDCONF pcc);
BOOL CALLBACK ConfigureDlgProc (HWND, UINT, WPARAM, LPARAM);
BOOL LoadRndConf(LPRNDCONF lpConf, LPSTR lpOps);
BOOL SaveRndConf(LPSTR lpOps, LPRNDCONF lpConf);
char *LoadStr(UINT id);

HINSTANCE dllHInst;
RNDCONF  conf;
char strBuffer[MAX_STR_LEN];

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

BOOL LoadRndConf(LPRNDCONF lpConf, LPSTR lpOps)
{

  // Read version
  lpConf->version=*(lpOps++)&0x7F;

  if(lpConf->version==0)
	 lpConf->version=CLICRNDVER;

  if(lpConf->version>CLICRNDVER)
  { MessageBox(NULL, LoadStr(IDS_BADVER), DLL_TITLE,
						 MB_OK|MB_ICONEXCLAMATION);
	 return FALSE;
  }

  // Read config
  lpConf->vowels=(*(lpOps++)=='1');
  lpConf->consonants=(*(lpOps++)=='1');
  lpConf->numbers=(*(lpOps++)=='1');

  // At least one option must be true
  if(lpConf->vowels==FALSE &&
	  lpConf->consonants==FALSE &&
	  lpConf->numbers==FALSE)
	  lpConf->vowels=TRUE;

  return TRUE;
}

BOOL SaveRndConf(LPSTR lpOps, LPRNDCONF lpConf)
{

  // Builds a string with the configuration options
  *(lpOps++)=lpConf->version|0x80;
  *(lpOps++)=(lpConf->vowels ? '1' : '0');
  *(lpOps++)=(lpConf->consonants ? '1' : '0');
  *(lpOps++)=(lpConf->numbers ? '1' : '0');

  return TRUE;
}

#pragma argsused
BOOL FAR PASCAL ConfigureDllOptions(HWND hOwner, LPACTDATA lpAct, LPSTR lpOps)
{
  if(lpOps==NULL || lpAct==NULL) return FALSE;
  if(!IsValidAct(hOwner, lpAct, TRUE)) return FALSE;

  if(!LoadRndConf(&conf, lpOps)) return FALSE;

  if(DialogBox(dllHInst, MAKEINTRESOURCE(DIALOG_CONFIGURE), hOwner,
					ConfigureDlgProc)==TRUE)
	 SaveRndConf(lpOps, &conf);

  return TRUE;
}


// Check if the activity can be automated
BOOL FAR PASCAL IsValidAct(HWND hOwner, LPACTDATA lpAct, BOOL showMsg)
{
  BOOL result=FALSE;

  switch(*lpAct->actMode)
  {
	 case ASSOCIA:
			switch(*lpAct->actType)
			{ case NORMAL:
			  case ESPECIAL:
			  case ESCRIU:
			  case EXPLORA:
                // ClicRnd only supports aassociations
					 result=TRUE;
					 break;
			  default: break;
			}
			break;

	 default: break;
  }

  if(!result && showMsg)
	 MessageBox(hOwner, LoadStr(IDS_INVACT), DLL_TITLE,
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

  if(!LoadRndConf(&conf, lpOps)) return FALSE;
  return buildActivity(lpAct, &conf);
}

// Main function: builds the activity contents
BOOL buildActivity(LPACTDATA lpAct, LPRNDCONF pcc)
{
  int nLines, i, k;
  FILE *fa, *fb, *fra;
  char *vowels="AEIOU";
  char *consonants="BCDFGHJKLMNPQRSTVWXYZ";
  char *numbers="01234576789";
  char *bufA=" ";
  char *bufB;
  char bufRA[50];

  if((nLines=*lpAct->rows * *lpAct->columns)<2) return FALSE;

  if((fa=fopen(lpAct->tempFName[0], "w+"))==NULL ||
	  (fb=fopen(lpAct->tempFName[1], "w+"))==NULL ||
	  (fra=fopen(lpAct->tempFName[2], "w+"))==NULL) return FALSE;

  randomize();

  for(i=0; i<nLines; i++)
  {  k=random(3);
	  *bufA=0;
	  if(k<1 && pcc->vowels==TRUE)
	  { *bufA=vowels[random(5)];
		 bufB="VOWEL";
	  }
	  else if(k<2 && pcc->consonants==TRUE)
	  { *bufA=consonants[random(21)];
		 bufB="CONSONANT";
	  }
	  else if(pcc->numbers==TRUE)
	  { *bufA=numbers[random(10)];
		 bufB="NUMBER";
	  }

	  if(*bufA!=0)
	  { sprintf(bufRA, "%s is a %s\n", bufA, bufB);
		 fputs(bufA, fa);
		 fputs("\n", fa);
		 fputs(bufB, fb);
		 fputs("\n", fb);
		 fputs(bufRA, fra);
	  }
	  else i--;
  }

  fclose(fa);
  fclose(fb);
  fclose(fra);

  for(i=0; i<3; i++) lpAct->useTempFName[i]=TRUE;

  return TRUE;
}


// Dialog callback function
#pragma argsused
BOOL CALLBACK ConfigureDlgProc (HWND hDlg, UINT msg,
										  WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  { case WM_INITDIALOG:
			CheckDlgButton(hDlg, IDC_VOWELS, conf.vowels);
			CheckDlgButton(hDlg, IDC_CONSONANTS, conf.consonants);
			CheckDlgButton(hDlg, IDC_NUMBERS, conf.numbers);
			return TRUE;

	 case WM_COMMAND:
			switch (wParam)
			{
			  case IDOK:
					 if(conf.vowels==FALSE && conf.consonants==FALSE && conf.numbers==FALSE)
					 { MessageBox(hDlg, LoadStr(IDS_NOOP), DLL_TITLE, MB_OK|MB_ICONEXCLAMATION);
						break;
					 }
					 EndDialog(hDlg, TRUE);  break;

			  case IDCANCEL:
					 EndDialog(hDlg, FALSE); break;

			  case IDC_VOWELS:
					 conf.vowels^=1;
					 break;

			  case IDC_CONSONANTS:
					 conf.consonants^=1;
					 break;

			  case IDC_NUMBERS:
					 conf.numbers^=1;
					 break;

			  default:
					 break;
			}
			break;

	 default:
			break;
  }
  return FALSE;
}


// Utility function used to show resource strings in message boxes
char *LoadStr(UINT id)
{
  *strBuffer=0;
  LoadString(dllHInst, id, strBuffer, MAX_STR_LEN);
  return strBuffer;
}


