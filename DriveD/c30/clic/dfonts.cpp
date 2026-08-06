// ---------------------------------------
// DFONTS.CPP
// Diàleg fonts i colors
// ---------------------------------------

#include "dfonts.h"
#include "rcomu\clicres.h"
#include "rcomu\clich.h"
#include "miscfunc.h"

#include <commdlg.h>
#include <string.h>

#ifndef NOEDIT

int curFont=0;

// ------- DFonts ------------

LPCSTR DFonts::getDialogName() {return MAKEINTRESOURCE(textAct ? DIALEG_FONTS_TEXT : DIALEG_FONTS);}
int DFonts::getHelpItem() {return HELP_FONTS;}

char * DFonts::textMostra="ABCDEFGHIJK\nabcdefghijk";

DFonts::DFonts(HWND hOwner, BOOL iTextAct, LOGFONT *iLogFont, Colors *iColors,
					COLORREF *icUsuari, COLORREF *icError):ModalDialog(hOwner)
{
  int i;
  LOGFONT far *lffp;

  textAct=iTextAct;
  if(textAct==FALSE && curFont>2) curFont=0;

  hFont=0;
  //lf=new FONTENUM;
  //lf->next=0;
  if((fl.hg=GlobalAlloc(GHND, FONTBLOCK * sizeof(LOGFONT)))==0) NOMEM;
  fl.currSize=FONTBLOCK;
  fl.numFonts=0;

  for(i=0; i<2; i++)
  { cUsuari[i]=icUsuari[i];
	 cError[i]=icError[i];
	 hUsuariBrush[i]=0;
	 hErrorBrush[i]=0;
  }

  for(i=0;i<4;i++)
  { logF[i]=iLogFont[i];
	 colors[i]=iColors[i];
  }

  hBackBrush=0;
  hTextBrush=0;
  hShadowBrush=0;

  if(fl.hg!=0)
  { lffp=(LOGFONT far *)GlobalLock(fl.hg);
	 lffp[fl.numFonts++]=logF[curFont];
	 GlobalUnlock(fl.hg);
  }

  //lf->lf=logF[curFont];
  //lf->next=new FONTENUM;
  //lf->next->next=0;

  update=FALSE;
}

DFonts::~DFonts()
{ int i;

  if(hFont) {DelObj(hFont); hFont=0;}
  if(hBackBrush) DelObj(hBackBrush);
  if(hTextBrush) DelObj(hTextBrush);
  if(hShadowBrush) DelObj(hShadowBrush);
  for(i=0; i<2; i++)
  { if(hUsuariBrush[i]) DelObj(hUsuariBrush[i]);
	 if(hErrorBrush[i]) DelObj(hErrorBrush[i]);
  }
  /*
  if(lf)
	 for(FONTENUM *itera=lf; itera!=0; itera=itera->next)
		delete itera;
  lf=0;
  */
  if(fl.hg) {GlobalFree(fl.hg); fl.hg=0;}
}

int CALLBACK _export DFonts::FontFunc(LOGFONT FAR* lpLf,
												  NEWTEXTMETRIC FAR*,
												  int, LPARAM lParam)
{
  //FONTENUM far *lpFc=(FONTENUM far *)lParam;
  //FONTENUM far *itera=lpFc;
  FONTLIST far *lpFl=(FONTLIST far*)lParam;
  LOGFONT far *lffp;
  unsigned i;

  if(lpFl==0) return FALSE;

  if(lpFl->numFonts>=(lpFl->currSize-1))
  { HGLOBAL newHg;
	 newHg=GlobalReAlloc(lpFl->hg, (lpFl->currSize+=FONTBLOCK) * sizeof(LOGFONT), GHND);
	 if(newHg==0) {NOMEM; return FALSE;}
	 lpFl->hg=newHg;
  }

  lffp=(LOGFONT FAR *)GlobalLock(lpFl->hg);
  for(i=0; i<lpFl->numFonts; i++)
	 if(_fstricmp(lffp[i].lfFaceName, lpLf->lfFaceName)==0)
	 { GlobalUnlock(lpFl->hg); return TRUE;}

  /*
  while(itera->next!=0)
  { if(_fstricmp(itera->lf.lfFaceName, lpLf->lfFaceName)==0) return TRUE;
	 itera=itera->next;
  }
  if(LocalCompact(0)<5000 ||
	  (itera->next=new FONTENUM)==0) return FALSE;

  itera->next->next=0;
  itera->lf=*lpLf;
  if(itera->lf.lfWidth > MAXFW || itera->lf.lfHeight > MAXFH)
  { itera->lf.lfWidth=0; itera->lf.lfHeight=0;}
  */

  lffp[lpFl->numFonts]=*lpLf;
  if(lpLf->lfWidth>MAXFW || lpLf->lfHeight>MAXFH)
  { lffp[lpFl->numFonts].lfWidth=0; lffp[lpFl->numFonts].lfHeight=0;}
  lpFl->numFonts++;
  GlobalUnlock(lpFl->hg);

  return TRUE;
}

BOOL DFonts::dispatch(UINT msg, WPARAM wParam, LPARAM lParam)
{
  int i=0;
  COLORREF *c;

  switch(msg)
  { case WM_INITDIALOG:
			initDlg();
			resetDlg();
			return TRUE;

	 case WM_ENTERIDLE:
			if(update)
			{ doUpdateFont();
			  update=FALSE;
			}
			return FALSE;

	 case WM_DRAWITEM:
			{ DRAWITEMSTRUCT dris=*(LPDRAWITEMSTRUCT)lParam;
			  HBRUSH hbr;
			  switch(dris.CtlID)
			  { case TX_FONTDEMO:
						showMostra(&dris, &dris.rcItem, textMostra, hBackBrush,
								colors[curFont].textColor, colors[curFont].shadowColor,
								colors[curFont].shadow);

						if(textAct && curFont==0)
						{ int right0;
						  RECT r=dris.rcItem;
						  r.top=r.top+2*(RECTH(r)/3);
						  right0=r.right;
						  r.right=r.left+RECTW(r)/2;
						  showMostra(&dris, &r, getStringTbl(IDS_MOSTRAUSR), hUsuariBrush[1], cUsuari[0], NULL, FALSE);
						  r.left=r.right; r.right=right0;
						  showMostra(&dris, &r, getStringTbl(IDS_MOSTRAERR), hErrorBrush[1], cError[0], NULL, FALSE);
						}

						return TRUE;

				 case MC_ERROR:      if(i++==0) hbr=hErrorBrush[0];
				 case MC_ERROR_FONS: if(i++==0) hbr=hErrorBrush[1];
				 case MC_USUARI:     if(i++==0) hbr=hUsuariBrush[0];
				 case MC_USUARI_FONS:if(i++==0) hbr=hUsuariBrush[1];
				 case MOSTRA_COLOR3: if(i++==0) hbr=hShadowBrush;
				 case MOSTRA_COLOR2: if(i++==0) hbr=hBackBrush;
				 case MOSTRA_COLOR1: if(i++==0) hbr=hTextBrush;
						FillRect(dris.hDC, &dris.rcItem, hbr);
						return TRUE;

				 default:
						break;

			  }
			}
			break;

	 case WM_VSCROLL:
			{ ScrollBarItem sbi;
			  switch (GetDlgCtrlID((HWND)HIWORD(lParam)))
			  { case SB_LLH:
						sbi.staticId=ED_FONTSIZEW; sbi.currVal=&logF[curFont].lfWidth;
						sbi.maxVal=64, sbi.minVal=1;
						break;

				 case SB_LLV:
						sbi.staticId=ED_FONTSIZEH; sbi.currVal=&logF[curFont].lfHeight;
						sbi.maxVal=64, sbi.minVal=1;
						break;

				 default: return FALSE;
			  }
			  if(dlgScrBar(hWnd, wParam, sbi, TRUE))
			  { resetDlg(); return TRUE;}

			}
			break;

	 case WM_COMMAND:
			switch(wParam)
			{ case ED_FONTNAME:
					 if(HIWORD(lParam)==CBN_SELCHANGE) resetDlg();
					 break;

			  case CHK_BOLD:
					 logF[curFont].lfWeight=(logF[curFont].lfWeight>400 ? 400:700);
					 resetDlg();
					 break;

			  case CHK_ITALIC:
					 logF[curFont].lfItalic=(logF[curFont].lfItalic ? 0:1);
					 resetDlg();
					 break;

			  case CHK_OMBREJAT:
					 colors[curFont].shadow=(colors[curFont].shadow ? 0:1);
					 resetDlg();
					 break;

			  case CHK_COLOR_POP: i++;
			  case CHK_COLOR_MSG: i++;
			  case CHK_COLOR_B: i++;
			  case CHK_COLOR_A:
					 curFont=i;
					 SendDlgItemMessage(hWnd, ED_FONTNAME, CB_SELECTSTRING, -1,
											  (LPARAM)(LPSTR)logF[curFont].lfFaceName);
					 resetDlg();
					 break;

			  case BOTO_COPIA_COL:
					 for(i=0; i<4; i++)
						if(i!=curFont)
						{ colors[i]=colors[curFont];
						  logF[i]=logF[curFont];
						}
					 break;


			  case BC_ERROR:      if(i++==0) c=&cError[0];
			  case BC_ERROR_FONS: if(i++==0) c=&cError[1];
			  case BC_USUARI:     if(i++==0) c=&cUsuari[0];
			  case BC_USUARI_FONS:if(i++==0) c=&cUsuari[1];
			  case BOTO_COLOR3:   if(i++==0) c=&colors[curFont].shadowColor;
			  case BOTO_COLOR2:   if(i++==0) c=&colors[curFont].backColor;
			  case BOTO_COLOR1:   if(i++==0) c=&colors[curFont].textColor;
					 { CHOOSECOLOR cc;

						memset(&cc, 0, sizeof(CHOOSECOLOR));
						cc.lStructSize = sizeof(CHOOSECOLOR);
						cc.hwndOwner = hWnd;
						cc.rgbResult = *c;
						cc.Flags = CC_RGBINIT;
						cc.lpCustColors = &gv->aclrCust[0];
						if (ChooseColor(&cc))
						{ *c = cc.rgbResult;
						  resetDlg();
						}
					 }
					 break;

			  case BOTO_OK:
					 result=TRUE;

			  case BOTO_CANCEL:
					 end();
					 return TRUE;

			  default: break;
			}
			break;

	 default: break;
  }

  return ModalDialog::dispatch(msg, wParam, lParam);
}

void DFonts::showMostra(DRAWITEMSTRUCT *dris, RECT *r, char *txt, HBRUSH hFonsBrush,
								COLORREF colorText, COLORREF colorOmbra, BOOL ombra)
{
  HFONT hFontBack;
  int oldBkMode;
  COLORREF oldTextColor;

  oldBkMode=SetBkMode(dris->hDC, TRANSPARENT);
  FillRect(dris->hDC, r, hFonsBrush);
  if(hFont) DelObj(hFont);
  hFont=CreateFontIndirect(&logF[curFont]);
  hFontBack=(HFONT)SelectObject(dris->hDC, hFont);
  oldTextColor=SetTextColor(dris->hDC, colorText);
  if(ombra)
  { int s=2+logF[curFont].lfHeight/26;
	 OffsetRect(r, s, s);
	 SetTextColor(dris->hDC, colorOmbra);
	 DrawText(dris->hDC, txt, -1, r, DT_CENTER);
	 SetTextColor(dris->hDC, colorText);
	 OffsetRect(r, -s, -s);
  }

  DrawText(dris->hDC, txt, -1, r, DT_CENTER);
  SelectObject(dris->hDC, hFontBack);
  SetBkMode(dris->hDC, oldBkMode);
  SetTextColor(dris->hDC, oldTextColor);
}

BOOL DFonts::initDlg()
{
  FONTENUMPROC lpEnumFunc;
  LOGFONT far *lffp;
  HDC hDC;
  int i;

  if(fl.hg==0) return FALSE;

  lpEnumFunc=(FONTENUMPROC)MakeProcInstance((FARPROC)DFonts::FontFunc, hInst);
  hDC=GetDC(hWnd);
  /*
  // AIXO NO ENTRA MAI (CREC)
  if(lf->next==0)
  { EnumFontFamilies(hDC, "System", lpEnumFunc, (LPARAM)(LPSTR)lf);
	 logF[curFont]=lf->lf;
  }
  */

  //EnumFontFamilies(hDC, NULL, lpEnumFunc, (LPARAM)(LPSTR)lf);
  EnumFontFamilies(hDC, NULL, lpEnumFunc, (LPARAM)(LPSTR)&fl);

  ReleaseDC(hWnd, hDC);
  FreeProcInstance((FARPROC)lpEnumFunc);

  /*
  for(FONTENUM *itera=lf; itera->next!=0; itera=itera->next)
	SendDlgItemMessage(hWnd, ED_FONTNAME, CB_ADDSTRING, 0,
							(LPARAM)(LPSTR)itera->lf.lfFaceName);
  */

  lffp=(LOGFONT far *)GlobalLock(fl.hg);
  for(i=0; i<fl.numFonts; i++)
	 SendDlgItemMessage(hWnd, ED_FONTNAME, CB_ADDSTRING, 0,
							  (LPARAM)(LPSTR)lffp[i].lfFaceName);
  GlobalUnlock(fl.hg);


  SendDlgItemMessage(hWnd, ED_FONTNAME, CB_SELECTSTRING, -1,
							(LPARAM)(LPSTR)logF[curFont].lfFaceName);
  return TRUE;
}

void DFonts::resetDlg()
{
  //FONTENUM *itera;
  LOGFONT far *lffp;
  char fName[LF_FACESIZE];
  int i;

  GetDlgItemText(hWnd, ED_FONTNAME, fName, sizeof(fName)-1);
  if(fl.hg!=0 && stricmp(fName, logF[curFont].lfFaceName)!=0)
  { /*
	 for(itera=lf; itera->next!=0; itera=itera->next)
	  if(stricmp(itera->lf.lfFaceName, fName)==0) break;

	 if(itera->next!=0) logF[curFont]=itera->lf;
	 */
	 lffp=(LOGFONT far *)GlobalLock(fl.hg);
	 for(i=0;i<fl.numFonts; i++)
		if(_fstricmp(lffp[i].lfFaceName, fName)==0)
	 { logF[curFont]=lffp[i];
		break;
	 }
	 GlobalUnlock(fl.hg);
  }

  SetDlgItemInt(hWnd, ED_FONTSIZEW, logF[curFont].lfWidth, FALSE);
  SetDlgItemInt(hWnd, ED_FONTSIZEH, logF[curFont].lfHeight, FALSE);
  SendDlgItemMessage(hWnd, CHK_BOLD, BM_SETCHECK, logF[curFont].lfWeight>400, 0);
  SendDlgItemMessage(hWnd, CHK_ITALIC, BM_SETCHECK, logF[curFont].lfItalic!=0, 0);
  SendDlgItemMessage(hWnd, CHK_OMBREJAT, BM_SETCHECK, colors[curFont].shadow!=0, 0);

  SendDlgItemMessage(hWnd, CHK_COLOR_A, BM_SETCHECK, curFont==0, 0);
  SendDlgItemMessage(hWnd, CHK_COLOR_B, BM_SETCHECK, curFont==1, 0);
  SendDlgItemMessage(hWnd, CHK_COLOR_MSG, BM_SETCHECK, curFont==2, 0);
  if(textAct) SendDlgItemMessage(hWnd, CHK_COLOR_POP, BM_SETCHECK, curFont==3, 0);

  if(hBackBrush) DelObj(hBackBrush);
  hBackBrush= CreateSolidBrush(colors[curFont].backColor);
  if(hTextBrush) DelObj(hTextBrush);
  hTextBrush= CreateSolidBrush(colors[curFont].textColor);
  if(hShadowBrush) DelObj(hShadowBrush);
  hShadowBrush= CreateSolidBrush(colors[curFont].shadowColor);
  if(textAct)
  { int sh=(curFont==0 ? SW_SHOW : SW_HIDE);
	 ShowWindow(GetDlgItem(hWnd, BC_USUARI), sh);
	 ShowWindow(GetDlgItem(hWnd, MC_USUARI), sh);
	 ShowWindow(GetDlgItem(hWnd, BC_USUARI_FONS), sh);
	 ShowWindow(GetDlgItem(hWnd, MC_USUARI_FONS), sh);
	 ShowWindow(GetDlgItem(hWnd, BC_ERROR), sh);
	 ShowWindow(GetDlgItem(hWnd, MC_ERROR), sh);
	 ShowWindow(GetDlgItem(hWnd, BC_ERROR_FONS), sh);
	 ShowWindow(GetDlgItem(hWnd, MC_ERROR_FONS), sh);

	 if(sh==SW_SHOW) for(i=0; i<2; i++)
	 { if(hUsuariBrush[i]) DelObj(hUsuariBrush[i]);
		hUsuariBrush[i]= CreateSolidBrush(cUsuari[i]);
		if(hErrorBrush[i]) DelObj(hErrorBrush[i]);
		hErrorBrush[i]= CreateSolidBrush(cError[i]);
	 }
  }

  update=TRUE;
}

void DFonts::doUpdateFont()
{

  InvalidateRect(GetDlgItem(hWnd, TX_FONTDEMO), NULL, TRUE);
  InvalidateRect(GetDlgItem(hWnd, MOSTRA_COLOR1), NULL, TRUE);
  InvalidateRect(GetDlgItem(hWnd, MOSTRA_COLOR2), NULL, TRUE);
  InvalidateRect(GetDlgItem(hWnd, MOSTRA_COLOR3), NULL, TRUE);
  if(textAct)
  { InvalidateRect(GetDlgItem(hWnd, MC_USUARI), NULL, TRUE);
	 InvalidateRect(GetDlgItem(hWnd, MC_USUARI_FONS), NULL, TRUE);
	 InvalidateRect(GetDlgItem(hWnd, MC_ERROR), NULL, TRUE);
	 InvalidateRect(GetDlgItem(hWnd, MC_ERROR_FONS), NULL, TRUE);
  }

  /*
  if(hFont) DelObj(hFont);
  hFont=CreateFontIndirect(&logF[curFont]);
  SendDlgItemMessage(hWnd, TX_FONTDEMO, WM_SETFONT, (WPARAM)hFont, TRUE);
  */
}
#endif

