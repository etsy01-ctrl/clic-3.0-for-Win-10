// ---------------------------------------
// DACTOPT.CPP
// Diàleg d'opcions de les activitats
// ---------------------------------------

#ifndef NOEDIT

#include "dactopt.h"
#include "miscfunc.h"
#include "rcomu\clicres.h"
#include "rcomu\clich.h"

#include <commdlg.h>
#include <string.h>

// ------- DActOpt ------------

LPCSTR DActOpt::getDialogName() {return MAKEINTRESOURCE(DIALEG_ACTOPT);}
int DActOpt::getHelpItem() {return HELP_ACTOPT;}

DActOpt::DActOpt(HWND hOwner, ActFile *iactf):ModalDialog(hOwner)
{
  af=*iactf;
  destActf=iactf;
  setActivePath(curActPath);
  dllAvail=FALSE;
  bmpf=(af.bmpFons!=0);
}

BOOL DActOpt::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  int i=0;

  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			return TRUE;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch(GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_CASH:
						sbi.staticId=ED_TXTCASW; sbi.currVal=&af.txtCW;
						sbi.maxVal=700, sbi.minVal=10;
						break;

				 case SB_CASV:
						sbi.staticId=ED_TXTCASH; sbi.currVal=&af.txtCH;
						sbi.maxVal=500, sbi.minVal=10;
						break;

				 case SB_CASH2:
						sbi.staticId=ED_TXTCASW2; sbi.currVal=&af.txtCW2;
						sbi.maxVal=700, sbi.minVal=10;
						break;

				 case SB_CASV2:
						sbi.staticId=ED_TXTCASH2; sbi.currVal=&af.txtCH2;
						sbi.maxVal=500, sbi.minVal=10;
						break;

				 case SB_RX:
						sbi.staticId=ED_RX; sbi.currVal=&af.pwrx;
						sbi.maxVal=800, sbi.minVal=0;
						break;

				 case SB_RY:
						sbi.staticId=ED_RY; sbi.currVal=&af.pwry;
						sbi.maxVal=600, sbi.minVal=0;
						break;

				 default: return FALSE;
			  }
			  if(dlgScrBar(hWnd, wParam, sbi, TRUE)) return TRUE;
			}
			break;

	 case WM_DRAWITEM:
			{ DRAWITEMSTRUCT dris=*(LPDRAWITEMSTRUCT)lParam;
			  HBRUSH hbr=0;
			  i=0;

			  switch(dris.CtlID)
			  { case MOSTRA_TRAMA_3: i++;
				 case MOSTRA_TRAMA_2: i++;
				 case MOSTRA_TRAMA_1: i++;
				 case MOSTRA_TRAMA_0:
						hbr=CreatePatternBrush(Zona::tramaBmp[i]);

				 case MOSTRA_COLOR2: i++;
				 case MOSTRA_COLOR1:
						if(hbr==0) hbr=CreateSolidBrush(af.colorFons[i]);
						FillRect(dris.hDC, &dris.rcItem, hbr);
						DelObj(hbr);
						return TRUE;

				 default:
						break;
			  }
			}
			break;


	 case WM_COMMAND:
			switch(wParam)
			{ case CHK_AJUDA:  af.shHelp^=1; break;
			  case CHK_PUZOPT: af.shPuz^=1; break;
			  case CHK_GREEN:  af.noBV^=1; break;
			  case CHK_NODISK: af.shDisk^=1; break;
			  case CHK_BPRINT: af.shPrint^=1; break;
			  case CHK_NOEXIT: af.shPorta^=1; break;
			  case CHK_COMPT1: af.comptadors[0]^=1; break;
			  case CHK_COMPT2: af.comptadors[1]^=1; break;
			  case CHK_COMPT3: af.comptadors[2]^=1; break;
			  case CHK_MARCS:  af.marcs^=1; break;
			  case CHK_BARRAAM:  af.barraAmunt^=1; break;

			  case CHK_TRAMA3_P: i++;
			  case CHK_TRAMA2_P: i++;
			  case CHK_TRAMA1_P: i++;
			  case CHK_TRAMA0_P: i++;
			  case CHK_COLOR_P:
					 af.tramaFons[0]=i-1;
					 break;

			  case CHK_TRAMA3_F: i++;
			  case CHK_TRAMA2_F: i++;
			  case CHK_TRAMA1_F: i++;
			  case CHK_TRAMA0_F: i++;
			  case CHK_COLOR_F:
					 af.tramaFons[1]=i-1;
					 break;

			  case CHK_BT2: i++;
			  case CHK_BT1: i++;
			  case CHK_BT0:
					 af.btTipus=i;
					 break;

			  case BTN_COLOR_F: i++;
			  case BTN_COLOR_P:
					 { CHOOSECOLOR cc;

						memset(&cc, 0, sizeof(CHOOSECOLOR));
						cc.lStructSize = sizeof(CHOOSECOLOR);
						cc.hwndOwner = hWnd;
						cc.rgbResult = af.colorFons[i];
						cc.Flags = CC_RGBINIT;
						cc.lpCustColors = &gv->aclrCust[0];
						if (ChooseColor(&cc))
						{ af.colorFons[i] = cc.rgbResult;
						  af.tramaFons[i]=-1;
						  resetDlg();
						}
					 }
					 break;

			  case CHK_CUSTHELP:
					 af.custHlp^=1;
					 resetHlpf();
					 break;

			  case CHK_BMPFONS:
					 bmpf^=1;
					 resetDlg();
					 break;

			  case CHK_BMPTILE:  i=1;
			  case CHK_BMPCENTER:
					 af.tileBmp=i;
					 break;

			  case CHK_PWTRANSP:
					 af.pwTransp^=1;
					 resetDlg();
					 break;

			  case CHK_RCENT:
					 af.pwrp^=1;
					 resetDlg();
					 break;

			  case BOTO_BROW:
					 browseFn();
					 return TRUE;

			  case BOTO_OK:
					 if(!checkOk()) break;
					 af.modified=TRUE;
					 *destActf=af;
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  case BOTO_CONFDLL:
					 runDllConf(FALSE);
					 break;

			  case CHK_USEDLL:
					 af.useDLL^=1;
					 resetDlg();
					 break;

			  case BOTO_DEFAULT:
					 if(af.ajAllowed()) af.shHelp=TRUE;
					 if(af.actMode==PUZZLE) af.shPuz=TRUE;
					 af.shDisk=TRUE;
					 af.shPrint=FALSE;
					 af.shPorta=TRUE;
					 af.tramaFons[0]=0;
					 af.tramaFons[1]=-1;
					 af.colorFons[0]=af.colorFons[1]=RGB(0xC0,0xC0,0xC0);
					 af.noBV=FALSE;
					 af.txtCW=DEF_TXTCASW; af.txtCH=DEF_TXTCASH;
					 af.txtCW2=DEF_TXTCASW; af.txtCH2=DEF_TXTCASH;
					 af.useDLL=FALSE;
					 *af.rgDLL=0;
					 af.custHlp=FALSE;
					 *af.custHelpFile=0;
					 *af.hlpTopic=FALSE;
					 for(i=0; i<3; i++) af.comptadors[i]=TRUE;
					 af.marcs=TRUE;
					 resetDlg();
					 resetHlpf();
					 return TRUE;

			  case BOTO_CPESTIL:
                browseCopy();
					 break;

			  case CHK_NOAV:
			       af.noAv^=1;

			  default: break;
			}
			break;

	 default: break;
  }
  return ModalDialog::dispatch(msg, wParam, lParam);
}


BOOL DActOpt::checkOk()
{
  OFSTRUCT ofs;
  char *fn;
  char txbuf[MAX_FULL_FILENAME+1];
  ofs.cBytes=sizeof(OFSTRUCT);

  if(af.bmpFons) {delete af.bmpFons; af.bmpFons=0;}
  if(bmpf)
  { GetDlgItemText(hWnd, COMBO_BMPFONS, txbuf, MAX_FULL_FILENAME);
	 if(*txbuf==0)
	 { BMsgBox(hWnd, IDS_ERRNOFONS, 0, 0, 0, MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	 }
	 af.bmpFons=dupStr(txbuf);
  }

  if(af.useDLL && !runDllConf(TRUE)) return FALSE;

  if(af.custHlp)
  { GetDlgItemText(hWnd, ED_TOPIC, af.hlpTopic, sizeof(af.hlpTopic)-1);
	 GetDlgItemText(hWnd, ED_PNAME, txbuf, sizeof(txbuf)-1);
	 fn=getRelativeFileName(txbuf);
	 if(fn!=txbuf) strcpy(txbuf, fn);
	 SetDlgItemText(hWnd, ED_PNAME, txbuf);
	 if(OpenFile(getFullFileName(txbuf), &ofs, OF_EXIST|OF_SEARCH)==HFILE_ERROR)
	 { BMsgBox(hWnd, IDS_ERRHLPFN, 0, 0, 0,
						  MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	 }
	 strcpy(af.custHelpFile, txbuf);
  }

  GetDlgItemText(hWnd, EDIT_INFO, af.fileDesc, sizeof(af.fileDesc)-1);

  return TRUE;
}

void DActOpt::resetHlpf()
{
  SendDlgItemMessage(hWnd, CHK_CUSTHELP, BM_SETCHECK, af.custHlp, 0);
  EnableWindow(GetDlgItem(hWnd, ED_PNAME), af.custHlp);
  EnableWindow(GetDlgItem(hWnd, ED_TOPIC), af.custHlp);
  EnableWindow(GetDlgItem(hWnd, BOTO_BROW), af.custHlp);
}

void DActOpt::initDlg()
{
  LRESULT lr;

  EnableWindow(GetDlgItem(hWnd, CHK_AJUDA), af.ajAllowed());
  EnableWindow(GetDlgItem(hWnd, CHK_PUZOPT), af.actMode==PUZZLE);
  EnableWindow(GetDlgItem(hWnd, CHK_GREEN),
					af.actMode!=ASSOCIA ||
					(af.puzMode!=INFO && af.puzMode!=EXPLORA));
  SetDlgItemText(hWnd, ED_PNAME, af.custHelpFile);
  SetDlgItemText(hWnd, ED_TOPIC, af.hlpTopic);

  DlgDirListComboBox(hWnd, getFullFileName("*.DLL", curActPath), COMBO_DLL,
							0, 0);

  lr=SendDlgItemMessage(hWnd, COMBO_DLL, CB_GETCOUNT, 0, 0L);
  dllAvail=(lr>0 && lr!=CB_ERR);
  if(dllAvail)
	  SendDlgItemMessage(hWnd, COMBO_DLL, CB_SELECTSTRING, -1,
								(LPARAM)(LPSTR)af.rgDLL);
  else
	 af.useDLL=FALSE;

  SendDlgItemMessage(hWnd, COMBO_BMPFONS, CB_DIR, 0x0001,
		 (LPARAM)getFullFileName(extName[EXT_GIF]));
  SendDlgItemMessage(hWnd, COMBO_BMPFONS, CB_DIR, 0x0001,
		 (LPARAM)getFullFileName(extName[EXT_BMP]));

  if(af.bmpFons)
        SetDlgItemText(hWnd, COMBO_BMPFONS, af.bmpFons);
  /*	  SendDlgItemMessage(hWnd, COMBO_BMPFONS, CB_SELECTSTRING, -1,
								(LPARAM)(LPSTR)af.bmpFons);
  */

  SendDlgItemMessage(hWnd, CHK_BMPFONS, BM_SETCHECK, bmpf, 0);
  SendDlgItemMessage(hWnd, CHK_BMPTILE, BM_SETCHECK, af.tileBmp==TRUE, 0);
  SendDlgItemMessage(hWnd, CHK_BMPCENTER, BM_SETCHECK, af.tileBmp==FALSE, 0);
  SendDlgItemMessage(hWnd, CHK_PWTRANSP, BM_SETCHECK, af.pwTransp, 0);


  SendDlgItemMessage(hWnd, CHK_COLOR_P, BM_SETCHECK, af.tramaFons[0]==-1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA0_P, BM_SETCHECK, af.tramaFons[0]==0, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA1_P, BM_SETCHECK, af.tramaFons[0]==1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA2_P, BM_SETCHECK, af.tramaFons[0]==2, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA3_P, BM_SETCHECK, af.tramaFons[0]==3, 0);

  SendDlgItemMessage(hWnd, CHK_COLOR_F, BM_SETCHECK, af.tramaFons[1]==-1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA0_F, BM_SETCHECK, af.tramaFons[1]==0, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA1_F, BM_SETCHECK, af.tramaFons[1]==1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA2_F, BM_SETCHECK, af.tramaFons[1]==2, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA3_F, BM_SETCHECK, af.tramaFons[1]==3, 0);

  SendDlgItemMessage(hWnd, CHK_BT0, BM_SETCHECK, af.btTipus==0, 0);
  SendDlgItemMessage(hWnd, CHK_BT1, BM_SETCHECK, af.btTipus==1, 0);
  SendDlgItemMessage(hWnd, CHK_BT2, BM_SETCHECK, af.btTipus==2, 0);

  SendDlgItemMessage(hWnd, CHK_AJUDA, BM_SETCHECK, af.shHelp, 0);
  SendDlgItemMessage(hWnd, CHK_NODISK, BM_SETCHECK, af.shDisk, 0);
  SendDlgItemMessage(hWnd, CHK_BPRINT, BM_SETCHECK, af.shPrint, 0);
  SendDlgItemMessage(hWnd, CHK_NOEXIT, BM_SETCHECK, af.shPorta, 0);
  SendDlgItemMessage(hWnd, CHK_PUZOPT, BM_SETCHECK, af.shPuz, 0);
  SendDlgItemMessage(hWnd, CHK_GREEN, BM_SETCHECK, !af.noBV, 0);
  SendDlgItemMessage(hWnd, CHK_COMPT1, BM_SETCHECK, af.comptadors[0], 0);
  SendDlgItemMessage(hWnd, CHK_COMPT2, BM_SETCHECK, af.comptadors[1], 0);
  SendDlgItemMessage(hWnd, CHK_COMPT3, BM_SETCHECK, af.comptadors[2], 0);
  SendDlgItemMessage(hWnd, CHK_MARCS, BM_SETCHECK, af.marcs, 0);
  SendDlgItemMessage(hWnd, CHK_BARRAAM, BM_SETCHECK, af.barraAmunt, 0);
  SetDlgItemInt(hWnd, ED_TXTCASW, af.txtCW, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASH, af.txtCH, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASW2, af.txtCW2, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASH2, af.txtCH2, FALSE);
  SetDlgItemText(hWnd, EDIT_INFO, af.fileDesc);
  SendDlgItemMessage(hWnd, CHK_USEDLL, BM_SETCHECK, af.useDLL, 0);

  SetDlgItemInt(hWnd, ED_RX, af.pwrx, FALSE);
  SetDlgItemInt(hWnd, ED_RY, af.pwry, FALSE);
  SendDlgItemMessage(hWnd, CHK_RCENT, BM_SETCHECK, !af.pwrp, 0);

  if(af.noEvaluable())
  { EnableWindow(GetDlgItem(hWnd, CHK_NOAV), FALSE);
	 SendDlgItemMessage(hWnd, CHK_NOAV, BM_SETCHECK, FALSE, 0);
  }
  else SendDlgItemMessage(hWnd, CHK_NOAV, BM_SETCHECK, !af.noAv, 0);


  resetDlg();
  resetHlpf();
}

void DActOpt::resetDlg()
{
  EnableWindow(GetDlgItem(hWnd, CHK_BMPTILE), bmpf);
  EnableWindow(GetDlgItem(hWnd, CHK_BMPCENTER), bmpf);
  EnableWindow(GetDlgItem(hWnd, COMBO_BMPFONS), bmpf);

  EnableWindow(GetDlgItem(hWnd, CHK_COLOR_F), !af.pwTransp);
  EnableWindow(GetDlgItem(hWnd, CHK_TRAMA0_F), !af.pwTransp);
  EnableWindow(GetDlgItem(hWnd, CHK_TRAMA1_F), !af.pwTransp);
  EnableWindow(GetDlgItem(hWnd, CHK_TRAMA2_F), !af.pwTransp);
  EnableWindow(GetDlgItem(hWnd, CHK_TRAMA3_F), !af.pwTransp);
  EnableWindow(GetDlgItem(hWnd, BTN_COLOR_F), !af.pwTransp);

  /*
  SendDlgItemMessage(hWnd, CHK_COLOR_P, BM_SETCHECK, af.tramaFons[0]==-1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA0_P, BM_SETCHECK, af.tramaFons[0]==0, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA1_P, BM_SETCHECK, af.tramaFons[0]==1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA2_P, BM_SETCHECK, af.tramaFons[0]==2, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA3_P, BM_SETCHECK, af.tramaFons[0]==3, 0);

  SendDlgItemMessage(hWnd, CHK_COLOR_F, BM_SETCHECK, af.tramaFons[1]==-1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA0_F, BM_SETCHECK, af.tramaFons[1]==0, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA1_F, BM_SETCHECK, af.tramaFons[1]==1, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA2_F, BM_SETCHECK, af.tramaFons[1]==2, 0);
  SendDlgItemMessage(hWnd, CHK_TRAMA3_F, BM_SETCHECK, af.tramaFons[1]==3, 0);

  SendDlgItemMessage(hWnd, CHK_BT0, BM_SETCHECK, af.btTipus==0, 0);
  SendDlgItemMessage(hWnd, CHK_BT1, BM_SETCHECK, af.btTipus==1, 0);
  SendDlgItemMessage(hWnd, CHK_BT2, BM_SETCHECK, af.btTipus==2, 0);

  SendDlgItemMessage(hWnd, CHK_AJUDA, BM_SETCHECK, af.shHelp, 0);
  SendDlgItemMessage(hWnd, CHK_NODISK, BM_SETCHECK, af.shDisk, 0);
  SendDlgItemMessage(hWnd, CHK_NOEXIT, BM_SETCHECK, af.shPorta, 0);
  SendDlgItemMessage(hWnd, CHK_PUZOPT, BM_SETCHECK, af.shPuz, 0);
  SendDlgItemMessage(hWnd, CHK_GREEN, BM_SETCHECK, !af.noBV, 0);
  SendDlgItemMessage(hWnd, CHK_COMPT1, BM_SETCHECK, af.comptadors[0], 0);
  SendDlgItemMessage(hWnd, CHK_COMPT2, BM_SETCHECK, af.comptadors[1], 0);
  SendDlgItemMessage(hWnd, CHK_COMPT3, BM_SETCHECK, af.comptadors[2], 0);
  SendDlgItemMessage(hWnd, CHK_MARCS, BM_SETCHECK, af.marcs, 0);
  SendDlgItemMessage(hWnd, CHK_BARRAAM, BM_SETCHECK, af.barraAmunt, 0);
  SetDlgItemInt(hWnd, ED_TXTCASW, af.txtCW, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASH, af.txtCH, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASW2, af.txtCW2, FALSE);
  SetDlgItemInt(hWnd, ED_TXTCASH2, af.txtCH2, FALSE);
  SetDlgItemText(hWnd, EDIT_INFO, af.fileDesc);
  SendDlgItemMessage(hWnd, CHK_USEDLL, BM_SETCHECK, af.useDLL, 0);
  */

  EnableWindow(GetDlgItem(hWnd, CHK_USEDLL), dllAvail);
  EnableWindow(GetDlgItem(hWnd, COMBO_DLL), dllAvail && af.useDLL);
  EnableWindow(GetDlgItem(hWnd, BOTO_CONFDLL), dllAvail && af.useDLL);

  InvalidateRect(GetDlgItem(hWnd, MOSTRA_COLOR1), NULL, TRUE);
  InvalidateRect(GetDlgItem(hWnd, MOSTRA_COLOR2), NULL, TRUE);

  ShowWindow(GetDlgItem(hWnd, STATIC1), af.pwrp);
  ShowWindow(GetDlgItem(hWnd, ED_RX), af.pwrp);
  ShowWindow(GetDlgItem(hWnd, SB_RX), af.pwrp);

  ShowWindow(GetDlgItem(hWnd, STATIC2), af.pwrp);
  ShowWindow(GetDlgItem(hWnd, ED_RY), af.pwrp);
  ShowWindow(GetDlgItem(hWnd, SB_RY), af.pwrp);
}

BOOL DActOpt::runDllConf(BOOL onlyTest)
{
  OFSTRUCT ofs;
  char txbuf[13];
  HINSTANCE dllinst;
  BOOL (FAR PASCAL *conf) (HWND, LPACTDATA, LPSTR);
  BOOL (FAR PASCAL *testAct) (HWND, LPACTDATA, BOOL);
  BOOL r=FALSE;

  ofs.cBytes=sizeof(OFSTRUCT);

  if(!af.useDLL) return FALSE;
  GetDlgItemText(hWnd, COMBO_DLL, txbuf, sizeof(txbuf));
  if(*txbuf==0 ||
	  OpenFile(getFullFileName(txbuf), &ofs, OF_EXIST)==HFILE_ERROR ||
	  (dllinst=LoadLibrary(getFullFileName(txbuf, curActPath)))<=HINSTANCE_ERROR)
  { BMsgBox(hWnd, IDS_ERRDLLNAME, 0, 0, 0,
						MB_OK|MB_ICONEXCLAMATION);
	 *af.rgDLL=0;
	 return FALSE;
  }

  af.prepareActData();

  (FARPROC)testAct=GetProcAddress(dllinst, "IsValidAct");
  if(testAct==NULL)
	 BMsgBox(0, IDS_ERRDLL);
  else
	 r=(*testAct)(hWnd, &ActFile::actd, TRUE);

  if(r==TRUE && onlyTest==FALSE)
  { (FARPROC)conf=GetProcAddress(dllinst, "ConfigureDllOptions");
	 if(conf==NULL)
		BMsgBox(0, IDS_ERRDLL);
	 else
		r=(*conf)(hWnd, &ActFile::actd, af.dllOptions);
  }

  FreeLibrary(dllinst);

  if(r) strcpy(af.rgDLL, txbuf);
  else *af.rgDLL=0;

  return r;
}

void DActOpt::browseFn()
{
  OPENFILENAME ofn;
  char retFile[sizeof(af.custHelpFile)];
  char hlpPath[sizeof(af.custHelpFile)];
  char  r, *filter, *t, *iPath;
  int i;
  SaveDir sd;

  GetDlgItemText(hWnd, ED_PNAME, hlpPath, sizeof(hlpPath));

  if((t=strrchr(hlpPath, '\\'))!=0)
  { *t=0;
	 iPath=hlpPath;
	 strcpy(retFile, t+1);
  }
  else
  { iPath=curPath;
	 strcpy(retFile, hlpPath);
  }

  if((filter=dupStr(getStringTbl(IDS_HLPFILES)))==NULL) return;
  r = filter[strlen(filter)-1];
  for(i = 0; filter[i]!=0; i++)
	 if(filter[i] == r) filter[i]=0;

  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= retFile;
  ofn.nMaxFile = sizeof(retFile);
  ofn.lpstrInitialDir = iPath;
  ofn.lpstrTitle=getStringTbl(IDS_HLPFTITLE);
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if (GetOpenFileName(&ofn))
	 SetDlgItemText(hWnd, ED_PNAME, getRelativeFileName(retFile));

  delete filter;
}

void DActOpt::browseCopy()
{
  OPENFILENAME ofn;
  char retFile[MAX_FULL_FILENAME];
  char backActPath[MAX_FULL_FILENAME];
  char  r, *filter;
  int i;

  ActFile af2;
  SaveDir sd;

  strcpy(backActPath, curActPath);

  if((filter=dupStr(getStringTbl(IDS_COPYFILES)))==NULL) return;
  r = filter[strlen(filter)-1];
  for(i = 0; filter[i]!=0; i++)
	 if(filter[i] == r) filter[i]=0;

  *retFile=0;
  memset(&ofn, 0, sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner = hWnd;
  ofn.lpstrFilter = filter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile= retFile;
  ofn.nMaxFile = sizeof(retFile);
  ofn.lpstrInitialDir = NULL;
  ofn.lpstrTitle=getStringTbl(IDS_COPYTITLE);
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

  if (GetOpenFileName(&ofn) && af2.load(retFile)==TRUE)
  { af.copyStyle(af2);
	 initDlg();
  }

  strcpy(curActPath, backActPath);

  delete filter;

}

#endif


