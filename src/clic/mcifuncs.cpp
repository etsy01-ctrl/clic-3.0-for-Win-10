// ------------------------------------------
// MCIFUNCS.CPP
// Control de recursos Multimedia
// ------------------------------------------

#include "wcl.h"
#include "rcomu\clicres.h"

#include <string.h>
#include <mmsystem.h>
#include <stdio.h>

HWND MciDisplayWnd;
BOOL mciPlaying=FALSE;
BOOL mciRecording[MAXRECS]={FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
BOOL mciAllowed=FALSE, waveAllowed=FALSE, midiAllowed=FALSE;
char mciText[120], mciTx2[120];
char cdaudio[]="cdaudio";
char tmf[]="set mci time format tmsf";
char tmfWav[]="set mci time format milliseconds";
char tmfMidi[]="set mci time format song pointer";
char pmci[]="play mci";
//                    0         1         2         3
//                    01234567890123456789012345678901234567890
char opennew[]=      "open new type waveaudio alias mcirec0";   // offset 36
char settimeformat[]="set mcirec0 time format milliseconds";    // offset 10
char recordmcirec[]= "record mcirec0 overwrite";                // offset 13
char stopmcirec[]=   "stop mcirec0";                            // offset 11
char closemcirec[]=  "close mcirec0";                           // offset 12
char playmcirec[]=   "play mcirec0 from 0";                     // offset 11
BOOL sonor=TRUE;

void avisSonor(UINT tipus)
{ if(sonor) MessageBeep(tipus);}


BOOL playFile(char *fileName, TIPUS_SO tipus, HWND hWnd, RECT *r, BOOL stretch)
{
  int strErrId, i;
  char *dev=fileName, *mci2=0, *mci3=pmci, *tx;
  char *spaceToZero=NULL;
  BOOL opened=FALSE;
  DWORD err=0;
  RECT rClient;
  HWND hWndBase;
  int  clx, cly, mciw, mcih, mcix, mciy, rMain;
  long l;

  silenci(FALSE);

  switch(tipus)
  { case REC_REC:
			strErrId=IDS_ERRREC;
			if(!waveAllowed) return FALSE;
			silenci(FALSE);
			i=fileName[3]-'0'; if(i<0) i=0; else if (i>=MAXRECS) i=MAXRECS-1;
			opennew[36]=settimeformat[10]=recordmcirec[13]=closemcirec[12]='0'+i;
			if(mciRecording[i]) mciSendString(closemcirec, 0, 0, 0);
			mciRecording[i]=TRUE;
			opened=TRUE;
			if((err=mciSendString(opennew, 0, 0, 0))!=0) break;
			if((err=mciSendString(settimeformat, 0, 0, 0))!=0) break;
			strcpy(mciTx2, recordmcirec);
			if((tx=strchr(fileName, ' '))==NULL ||
				sscanf(tx, "%ld", &l)!=1 ||
				l<=0) l=5;
			l*=1000;
			sprintf(mciText, "%ld", l);
			strcat(strcat(mciTx2, " from 0 to "), mciText);
			strcat(mciTx2, " notify");
			mci3=mciTx2;
			break;

	 case REC_PLAY:
			strErrId=IDS_ERRPLAYREC;
			if(!waveAllowed) return FALSE;
			i=fileName[3]-'0'; if(i<0) i=0; else if (i>=MAXRECS) i=MAXRECS-1;
			if(!mciRecording[i])
			{ BMsgBox(genMsgOwn, strErrId);
			  return FALSE;
			}
			stopmcirec[11]=playmcirec[11]='0'+i;
			mciSendString(stopmcirec, 0, 0, 0);
			strcpy(mciTx2, playmcirec);
			mci3=mciTx2;
			opened=TRUE;
			break;

	 case CDAUDIO:
			strErrId=IDS_ERRMCI;
			if(!mciAllowed) return FALSE;
			dev=cdaudio;
			mci2=tmf;
			sprintf(mciTx2, "play mci from %s", fileName);
			mci3=mciTx2;
			break;

	 case MIDI:
			strErrId=IDS_ERRMIDI;
			if(!midiAllowed) return FALSE;
			if((spaceToZero=strchr(dev, ' '))!=NULL)
			{ *spaceToZero=0;
			  mci2=tmfMidi;
			  sprintf(mciTx2, "play mci %s", spaceToZero+1);
			  mci3=mciTx2;
			}
			break;

	 case WAVE:
			strErrId=IDS_ERRWAVE;
			if(!waveAllowed) return FALSE;
			if((spaceToZero=strchr(dev, ' '))!=NULL)
			{ *spaceToZero=0;
			  mci2=tmfWav;
			  sprintf(mciTx2, "play mci %s", spaceToZero+1);
			  mci3=mciTx2;
			}
			break;

	 case UNKNOW_MCI:
			strErrId=IDS_ERRMCI;
			if(!mciAllowed) return FALSE;

			clx=0; cly=0; rMain=0;
			// rMain significa:
			// 0 : La posicio es pren des de la casella (a no ser que hWnd sigui zero (free))
			// 1 : La posicio es pren des de la finestra de joc (MciDisplayWnd))
			// 2 : La posicio es pren des de la finestra del Clic

			if((spaceToZero=strchr(dev, ' '))!=NULL)
			{ char ch, *tx1, *tx2;

			  *spaceToZero=0;
			  tx1=spaceToZero+1;
			  while(*tx1!=0)
			  { if(strnicmp(tx1, "pos", 3)==0)
				 { sscanf(tx1+3, "%d %d %d", &clx, &cly, &rMain);
					tx1+=3;
					while(*tx1!=0)
					{ if(*tx1>'9') break;
					  tx1++;
					}
				 }
				 else
				 { tx2=tx1;
					while(*tx1!=0)
					{ if(*tx1=='p' || *tx1=='P') break;
					  tx1++;
					}
					ch=*tx1;
					*tx1=0;
					mci2=tmfWav;
					sprintf(mciTx2, "play mci %s", tx2);
					mci3=mciTx2;
					*tx1=ch;
				 }
			  }
			}

			if(hWnd==0 || rMain!=0)
			  hWndBase=(rMain==2 ? genMsgOwn : MciDisplayWnd);
			else hWndBase=hWnd;
			GetClientRect(hWndBase, &rClient);

			sprintf(mciText, "open %s alias mci parent %d style child",
									dev, hWndBase);
			if(mciSendString(mciText, 0, 0, 0)!=0) break;
			opened=TRUE;
			if(r==0) break;

			if(stretch==TRUE ||
				(mciSendString("where mci window", mciText, 120, 0)!=0 ||
				  sscanf(mciText, "%d %d %d %d", &mcix, &mciy, &mciw, &mcih)!=4 ||
				  mciw==0 || mcih==0))
			{ mciw=r->right-r->left;
			  mcih=r->bottom-r->top;
			}

			if(hWnd!=0 && rMain==0 && clx==0 && cly==0)
			{ clx=r->left; cly=r->top;}
			else
			{ if(clx==0 && cly==0)
			  { clx=rClient.left+(rClient.right-rClient.left-mciw)/2;
				 cly=rClient.top+(rClient.bottom-rClient.top-mcih)/2;
			  }
			  else
			  { clx+=rClient.left;
				 cly+=rClient.top;
			  }
			}

			sprintf(mciText, "put mci window at %d %d %d %d",
					  clx, cly, mciw, mcih);

			mciSendString(mciText, 0, 0, 0);
			break;

	 case USER_CONT:
	 default:
			return FALSE;
  }

  if(err==0 && !opened)
  { sprintf(mciText, "open %s alias mci", dev);
	 err=mciSendString(mciText, 0, 0, 0);
  }
  if(err==0 && mci2!=0)
	 err=mciSendString(mci2, 0, 0, 0);

  if(err==0)
  { err=mciSendString(mci3, 0, 0, tipus==REC_REC ? genMsgOwn : 0);
	 if(tipus==REC_REC && err==0)
		setCursor(hWnd, HANDLE_CURSOR, hCursorMicro);
  }

  if(err!=0)
  { mciSendString("close mci", 0, 0, 0);
	 mciGetErrorString(err, mciText, sizeof(mciText));
	 MessageBox(genMsgOwn, mciText, getStringTbl(strErrId), MB_OK);
  }
  else mciPlaying=TRUE;

  if(spaceToZero) *spaceToZero=' ';

  return mciPlaying;
}

void silenci(BOOL total)
{
  int i;

  if(mciPlaying)
  { mciSendString("stop mci", 0, 0, 0);
	 mciSendString("close mci", 0, 0, 0);
	 mciPlaying=FALSE;
  }

  for(i=0; i<MAXRECS; i++)
  { if(mciRecording[i])
	 { stopmcirec[11]='0'+i;
		mciSendString(stopmcirec, 0, 0, 0);
		if(total)
		{ closemcirec[12]='0'+i;
		  mciSendString(closemcirec, 0, 0, 0);
		  mciRecording[i]=FALSE;
		}
		resetCursor();
	 }
  }
}

