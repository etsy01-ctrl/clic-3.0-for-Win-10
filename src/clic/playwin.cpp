// -----------------------------------
// PLAYWIN.CPP
// Implementacio de la classe PlayWin
// -----------------------------------

#include "playwin.h"
#include "clicdlg.h"
#include "miscfunc.h"
#include "rcomu\clicres.h"
#include "cmpstr.h"

#include <string.h>
#include <stdlib.h>

enum AjsModes {DEFAULT, SINGLE, DOUBLE, SINGCAS0, PORT};
BOOL waitToPass=FALSE, forcedPass=TRUE;
HCURSOR PlayWindow::hFilCursor=NULL;

PlayWindow::PlayWindow()
{
  int i;

  curAltMode=NOAUTOCURSOR;
  hFilCursor=NULL;
  changeCursorMode();

  MaxW=screenX-3*MARGE-BOTOW;
  MaxH=screenY-captionY-menuY-CTRLH-3*MARGE;
  casH=casX=0;
  nCw[0]=nCw[1]=nCh[0]=nCh[1]=1;
  for(i=0; i<3; i++) {hBmB[i]=0; hBmBX[i]=0; txtB[i]=0;}
  memset(strt, 0, sizeof(struct stretchTable)*STRETCH_TABLE_SIZE);
  for(i=0; i<2; i++) grr[i]=NullRect;
  for(i=0; i<3; i++) comptadors[i]=FALSE;
  cTags=0;
  keybDir=0;
  playing=FALSE;
#ifndef RUNTIME
  //hPortadaBm=LoadBitmap(hInst, MAKEINTRESOURCE(PORTADA));
  hPortadaBm=LoadResourceBitmap(hInst, MAKEINTRESOURCE(PORTADA), &hPortadaPal);
#else
  hPortadaBm=0;
  hPortadaPal=0;
#endif
  hBmpFons=0;
  filOn=FALSE;
  keybOn=FALSE;
  wRect.left=0; wRect.top=0;
  wRect.right=PORTW; wRect.bottom=PORTH;
  txc=NULL;
}

char *pwClassName="PLAYWINDOW";

BOOL PlayWindow::registerClass()
{
  WNDCLASS wc;

  wc.style =         CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc =   Window::wndProc;
  wc.cbClsExtra =    0;
  wc.cbWndExtra =    0;
  wc.hInstance =     hInst;
  wc.hIcon =         NULL;
  wc.hCursor =       hGenCursor;
  //wc.hbrBackground = hFonsBrush;
  wc.hbrBackground = NULL;
  wc.lpszMenuName =  NULL;
  wc.lpszClassName = pwClassName;

  return RegisterClass(&wc);
}

BOOL PlayWindow::createWindow()
{
  hMenu=NULL;
  if((hWnd = CreateWindow(pwClassName,
								  NULL,
								  WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN,
								  wRect.left,
								  wRect.top,
								  RECTW(wRect),
								  RECTH(wRect),
								  hOwn,
								  (HMENU)2222,
								  hInst,
								  NULL))==0) return FALSE;

  GetClientRect(hWnd, &clRect);
  zonaBase.create(hWnd, &clRect, FALSE, FALSE, PLAYWIN);
  ShowWindow(hWnd, SW_SHOW);
  MciDisplayWnd=hWnd;
  UpdateWindow(hWnd);
  return TRUE;
}

int PlayWindow::getExtraHeight()
{ int xh=0;

  if(txc) xh=txc->getTotalHeight()+(txc->btCorregir!=NULL ? txc->btHeight : 0)-RECTH(wRect);
  if(xh<0) xh=0;
  return xh;
}


BOOL PlayWindow::changeCursorMode()
{

  SetClassWord(hWnd, GCW_HCURSOR, (WORD)hGenCursor);

  if(hFilCursor!=NULL) {DestroyCursor(hFilCursor); hFilCursor=NULL;}
  if(cursorGran)
	 hFilCursor=LoadCursor(hInst, MAKEINTRESOURCE(CURSOR_GRAN_INV));
  else
	 hFilCursor=LoadCursor(hInst, MAKEINTRESOURCE(CURSOR_MA));

  return TRUE;
}

BOOL PlayWindow::initAct()
{
  int i, j, k, result=FALSE;

  if(!hWnd || !loaded) return FALSE;
  endGame();
  iniSolved=0;
  movToSolv=0;

  switch(actMode)
  { case PUZZLE:
			switch(cont[0])
			{ case EXT_BMP:
			  case EXT_GIF:
					 //result=loadImgFile(hWnd, hBmB[0], getFnCont(0),
					 //							  bmbW[0], bmbH[0]);
					 result=bmpCont::loadBmp(hWnd, hBmB[0], getFnCont(0), bmbW[0], bmbH[0]);
					 break;

			  case EXT_TXT:
					 if((txtB[0]=readTxtFile(getFnCont(0), nLines[0]))!=NULL)
						result=TRUE;
					 break;

			  default: break;
			}
			movToSolv=ncw*nch;
			break;

	 case ASSOCIA:
			for(i=0; i<3; i++)
			{ if((i==1 && puzMode==INFO) ||
				  (i==2 && !sol)) {i=3; break;}
			  if(i==1 && puzMode==IDENTIFICA) continue;
			  switch(cont[i])
			  { case EXT_BMP:
				 case EXT_GIF:
						//if(loadImgFile(hWnd, hBmB[i], getFnCont(i),
						//						bmbW[i], bmbH[i])==TRUE) continue;
						if(bmpCont::loadBmp(hWnd, hBmB[i], getFnCont(i), bmbW[i], bmbH[i])==TRUE) continue;

						break;

				 case EXT_TXT:
						if((txtB[i]=readTxtFile(getFnCont(i), nLines[i]))!=0)
						continue;
						break;

				 default: break;
			  }
			  break;
			}
			result=(i==3);
			if(puzMode==IDENTIFICA)
			{ for(i=0; i<ncw*nch; i++) if(ass[i]==1) movToSolv++;}
			else if(mAss)
			{ if(invAss) movToSolv=nctxw*nctxh;
			  else
			  { for(i=0; i<ncw*nch; i++) if(ass[i]>=0) movToSolv++;
				 iniSolved=(ncw*nch)-movToSolv;
			  }
			}
			else movToSolv=ncw*nch;
			if(movToSolv<=0) movToSolv=1;
			break;

	 case CREUATS:
			if((txtB[0]=newArray(nctxw*nctxh))==0 ||
				(txtB[1]=newArray(1*4))==0 ||
				(txtB[1][0]=dupStr(getStringTbl(IDS_HORITZ)))==0 ||
				(txtB[1][1]=dupStr(spcStr))==0 ||
				(txtB[1][2]=dupStr(getStringTbl(IDS_VERT)))==0 ||
				(txtB[1][3]=dupStr(spcStr))==0) break;

			for(k=0, i=0; i<nctxh; i++)
			{ for(j=0; j<nctxw; j++, k++)
			  { if((txtB[0][k]=dupStr(spcStr))==0) break;
				 if(graTxt[i][j]==COMODI)
					txtB[0][k][0]=NEGRE;
				 else movToSolv++;
			  }
			}
			result=(k==nctxw*nctxh);
			break;

	 case SOPA:
			if(bar[0] &&
				((cont[0]<EXT_TXT &&
				  //loadImgFile(hWnd, hBmB[1], getFnCont(0), bmbW[1], bmbH[1])==FALSE) ||
				  bmpCont::loadBmp(hWnd, hBmB[1], getFnCont(0), bmbW[1], bmbH[1])==FALSE) ||
				(cont[0]==EXT_TXT &&
				 (txtB[1]=readTxtFile(getFnCont(0), nLines[1]))==0))) break;

			if((txtB[0]=newArray(nctxw*nctxh))==0 ||
				(cTags=new BOOL[ntags[0]])==0) {NOMEM; break;}

			for(i=0; i<ntags[0]; i++) cTags[i]=FALSE;
			for(k=0, i=0; i<nctxh; i++)
			for(j=0; j<nctxw; j++, k++)
			  if((txtB[0][k]=dupStr(spcStr))==0) break;
			result=(k==nctxw*nctxh);
			movToSolv=ntags[0];
			break;

	 case TEXTACT:
			// movToSolv es mira mes endavant
			result=TRUE;
			break;

	 default: break;
  }

  if(result==FALSE) {endGame(); return FALSE;}

  if(hPortadaBm)
  { DelObj(hPortadaBm);
	 hPortadaBm=0;
  }
  if(hPortadaPal)
  {
	 DelObj(hPortadaPal);
	 hPortadaPal=0;
  }

  if(bmpFons)
		//loadImgFile(hWnd, hBmpFons, getFullFileName(bmpFons), bmpfW, bmpfH);
		bmpCont::loadBmp(hWnd, hBmpFons, getFullFileName(bmpFons), bmpfW, bmpfH);

  Zona::setBaseBrush(GENERICA, colorFons[0], tramaFons[0]);
  Zona::setBaseBrush(PLAYWIN, colorFons[1], tramaFons[1]);
  Zona::setBaseCont(&logF[GRA], &colors[GRA], GRA);
  Zona::setBaseCont(&logF[GRB], &colors[GRB], GRB);
  Zona::setBaseCont(&logF[MSB], &colors[MSB], MSB);
  Zona::setBaseCont(&logF[POP], &colors[POP], POP);
  Zona::setExtraColors(COL_USR, colorUsuari[0], colorUsuari[1]);
  Zona::setExtraColors(COL_ERR, colorError[0], colorError[1]);

  zonaBase.invr(TRUE);

  return result;
}

BOOL PlayWindow::initGame(int botonsVisibles)
{
  int i, j, nc;

  switch(actMode)
  { case PUZZLE:
	 case ASSOCIA:
			nCw[0]=nCw[1]=ncw;
			nCh[0]=nCh[1]=nch;
			if(actMode==ASSOCIA)
			{ if(puzMode==EXPLORA || puzMode==ESCRIU) {nCw[1]=1; nCh[1]=1;}
			  else if(mAss) {nCw[1]=nctxw; nCh[1]=nctxh;}
			}
			break;

	 case CREUATS:
			nCw[0]=nctxw; nCh[0]=nctxh;
			nCw[1]=1; nCh[1]=4;
			break;

	 case SOPA:
			nCw[0]=nctxw; nCh[0]=nctxh;
			if(bar[0]) {nCw[1]=ncw; nCh[1]=nch;}
			break;

	 case TEXTACT:
			nCw[0]=nCw[1]=1;
			nCh[0]=nCh[1]=1;
			break;

	 default: return FALSE;
  }

  if(actMode!=TEXTACT &&
	  (nCw[0]==0 || nCh[0]==0 ||
		(hBmB[0] && txtB[0]) || (!hBmB[0] && !txtB[0]) ||
		(hBmB[1] && txtB[1]))) return FALSE;

  for(i=0; i<2; i++)
  { grr[i]=NullRect;
	 if(hBmB[i])
	 { rw[i]=bmbW[i]; rh[i]=bmbH[i];
		if(i==1 && actMode==ASSOCIA && (puzMode==EXPLORA||puzMode==ESCRIU))
		{ rw[i]/=nctxw; rh[i]/=nctxh;}
	 }
	 else
	 { rw[i]=(i==0 ? txtCW:txtCW2)*nCw[i]; rh[i]=(i==0 ? txtCH:txtCH2)*nCh[i];
		if(i==1 && actMode==ASSOCIA &&
			(puzMode==EXPLORA || puzMode==ESCRIU) &&
			(graPos==AUB || graPos==BUA)) rw[i]=rw[0];
	 }
	 if(hBmBX[i]) {DelObj(hBmBX[i]); hBmBX[i]=strt[i].hBmStretch=NULL;}
  }

  SendMessage(hOwn, WM_PLAYW, PLAYW_SETMESS, (DWORD)(avPrevHelp ? initMessPrev:initMess));

  switch(actMode)
  { case PUZZLE:
			switch(puzMode)
			{ case FORAT:
					 if(ajustaMides(SINGCAS0, botonsVisibles) == FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0]) == FALSE ||
						 ompleGraella(0, 0, 0) == FALSE ||
						 creaGraella(1, 1, 1, FALSE) == FALSE)
						return FALSE;
					 break;

			  case INTERC:
					 if(ajustaMides(SINGLE, botonsVisibles) == FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0])==FALSE ||
						 ompleGraella(0, 0, 0)==FALSE)
						return FALSE;
					 break;

			  case DOBLE:
					 rw[1]=rw[0]; rh[1]=rh[0];
					 if(ajustaMides(DOUBLE, botonsVisibles)==FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0])==FALSE ||
						 ompleGraella(0, 0, 0)==FALSE ||
						 creaGraella(1, nCw[1], nCh[1], delim[0], TRUE)==FALSE ||
						 ompleGraella(1, 0, 0)==FALSE)
						return FALSE;
					 break;

			  case MEMORY:
					 if(graPos==AB || graPos==BA)
					 { i=nCw[0];
						nc=nCh[0]*i;
						rw[0]*=2; nCw[0]*=2;
						if(ajustaMides(SINGLE, botonsVisibles)==FALSE ||
							creaGraella(0, nCw[0], nCh[0], delim[0], TRUE)==FALSE ||
							ompleGraella(0, 0, 0, 0, 0, i, nCh[0])==FALSE ||
							ompleGraella(0, 0, 0, nc, 0, i, nCh[0])==FALSE)
						  return FALSE;
					 }
					 else
					 { i=nCh[0];
						nc=nCw[0]*i;
						rh[0]*=2; nCh[0]*=2;
						if(ajustaMides(SINGLE, botonsVisibles)==FALSE ||
							creaGraella(0, nCw[0], nCh[0], delim[0], TRUE)==FALSE ||
							ompleGraella(0, 0, 0, 0, 0, nCw[0], i)==FALSE ||
							ompleGraella(0, 0, 0, nc, 0, nCw[0], i)==FALSE)
						  return FALSE;
					 }
					 break;

			  default: return FALSE;
			}
			break;

	 case CREUATS:
			//rw[0]=20*nCw[0]; rh[0]=20*nCh[0];
			rw[1]=200; rh[1]=200;
			if(txtB[0]==0 || txtB[1]==0 || tags[0]==0 || tags[1]==0 ||
				ajustaMides(DOUBLE, botonsVisibles)==FALSE ||
				creaGraella(GRA, nCw[0], nCh[0], TRUE, FALSE, FALSE)==FALSE ||
				ompleGraella(0, -1, 0)==FALSE ||
				creaGraella(GRB, nCw[1], nCh[1], FALSE, FALSE, FALSE)==FALSE ||
				ompleGraella(1, -1, 1)==FALSE)
			  return FALSE;

			gra[1].getCas(0)->setBase(PLAYWIN);
			gra[1].getCas(2)->setBase(PLAYWIN);
			break;

	 case SOPA:
			//rw[0]=20*nCw[0]; rh[0]=20*nCh[0];
			if(txtB[0]==0 || tags[0]==0 ||
				ajustaMides(bar[0] ? DOUBLE:SINGLE, botonsVisibles)==FALSE ||
				creaGraella(0, nCw[0], nCh[0], delim[1])==FALSE ||
				ompleGraella(0, -1, 0)==FALSE ||
				(bar[0] &&
				 (creaGraella(1, nCw[1], nCh[1], delim[0], TRUE)==FALSE ||
				  ompleGraella(1, 1, 1)==FALSE)))
			  return FALSE;
			break;

	 case ASSOCIA:
			switch(puzMode)
			{ case IDENTIFICA:
			  case INFO:
					 if(ajustaMides(SINGLE, botonsVisibles) == FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0])==FALSE ||
						 ompleGraella(0, 0, 0)==FALSE)
						return FALSE;
					 break;

			  case ESCRIU:
			  case EXPLORA:
					 if(ajustaMides(DOUBLE, botonsVisibles) == FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0]) == FALSE ||
						 ompleGraella(0, 0, 0) == FALSE ||
						 creaGraella(1, 1, 1, FALSE) == FALSE)
						return FALSE;
					 if(hBmB[1])
						hBmBX[1]=mkStretchBm(1, hWnd, hBmB[1], rw[1]*nctxw, rh[1]*nctxh,
													(bmbW[1]/nctxw)*nctxw, (bmbH[1]/nctxh)*nctxh);
						//hBmBX[1]=createNewBitmap(hWnd, hBmB[1],
						//								 rw[1]*nctxw,
						//								 rh[1]*nctxh,
						//								 (bmbW[1]/nctxw)*nctxw,
						//								 (bmbH[1]/nctxh)*nctxh);
					 break;

			  default:
					 if(cont[0]!=cont[1])
					 { j=((i=(txtB[0]==0))==0);
						if(rw[i]>rw[j]) rw[i]=rw[j];
						if(rh[i]>rh[j]) rh[i]=rh[j];
					 }
					 if(ajustaMides(DOUBLE, botonsVisibles)==FALSE ||
						 creaGraella(0, nCw[0], nCh[0], delim[0])==FALSE ||
						 ompleGraella(0, 0, 0)==FALSE ||
						 creaGraella(1, nCw[1], nCh[1], delim[1])==FALSE ||
						 ompleGraella(1, 1, 1)==FALSE)
						return FALSE;
					 break;
			}
			if(sol)
			  if(hBmB[2]) gra[0].getAltData(hBmB[2]);
			  else gra[0].getAltData(txtB[2]);
			break;

	 case TEXTACT:
	      if(txc) {delete txc; txc=NULL;}
			if(ajustaMides(SINGLE, botonsVisibles)==FALSE ||
				(txc=new TxContenidor(GRA))==NULL ||
				txc->create(hWnd, &grr[0], FALSE, FALSE, GRA)==FALSE)
			  return FALSE;
			break;

	 default: return FALSE;
  }

  casZ=gra[1].getCas(0);

  return resetGame();
}

BOOL PlayWindow::endGame()
{
  int i;

  curAltMode=NOAUTOCURSOR;
  fil.end();
  if(txc) txc->fil.end();

  nGra=0; casX=0;
  for(i=0; i<3; i++)
  { if(hBmB[i]) //{DelObj(hBmB[i]); hBmB[i]=0;}
                bmpCont::freeBmp(hBmB[i]);
	 if(hBmBX[i]) {DelObj(hBmBX[i]); hBmBX[i]=strt[i].hBmStretch=NULL;}
	 if(txtB[i]) delArray(txtB[i]);
  }
  if(hBmpFons) //{DelObj(hBmpFons); hBmpFons=0;}
               bmpCont::freeBmp(hBmpFons);
  for(i=0; i<2; i++) if(gra[i].isCreated()) gra[i].destroy();
  if(cTags) {delete cTags; cTags=0;}
  if(txc) {delete txc; txc=NULL;}
  playing=FALSE;
  bmpCont::flush();
  return TRUE;
}

BOOL PlayWindow::destroy()
{
  endGame();
  if(hFilCursor!=NULL) DestroyCursor(hFilCursor);
  return Window::destroy();
}

LRESULT PlayWindow::dispatch(UINT msg, WPARAM wParam, LPARAM lParam, int)
{
  switch(msg)
  { case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;

	 case WM_QUERYENDSESSION:
			return TRUE;

	 case WM_MOVE:
			GetClientRect(hWnd, &clRect);
			zonaBase.move(&clRect);
			break;

	 case WM_ERASEBKGND:
			zonaBase.update((HDC)wParam);
	 case WM_MOUSEMOVE:
	 case WM_LBUTTONDOWN:
			if(processMouse(msg, wParam, lParam) || msg==WM_ERASEBKGND) return TRUE;
			break;

	 case WM_CHAR:
	 case WM_KEYDOWN:
	 case WM_KEYUP:
			if(processKeyboard(msg, wParam)) return TRUE;
			break;

	 case WM_PAINT:
			if(processPaint()) return TRUE;
			break;

	 case WM_HELPW:
			if(processUserMess(wParam, lParam)) return 0;
			break;

	 case WM_VSCROLL:
			if(txc)
			  if(txc->processScroll(wParam, lParam)) return 0;
			break;

	 case WM_SETFOCUS:
	 case WM_KILLFOCUS:
			checkCaret(hWnd, msg);
			break;

	 case WM_CBNEXT:
	 case WM_CBPREV:
	 case WM_CBGETFOCUS:
			if(txc!=NULL)
			  txc->processCBMsg(msg, wParam);
			break;

	 case WM_NOUINTENT:
			if(txc!=NULL) nouIntent();
			break;

	 case WM_COMMAND:
			if(wParam==BOTO_CORREGIR && txc!=NULL) {txc->deBoto=TRUE; nouIntent();}
			break;

	 default: break;
  }
  return Window::dispatch(msg, wParam, lParam);
}

BOOL PlayWindow::processKeyboard(UINT msg, WPARAM wParam)
{
  if(!playing) return FALSE;

#ifdef ONCE
  switch(msg)
  {
	 case WM_CHAR:
			switch(wParam)
			{ case VK_SPACE:
			  case VK_RETURN:
					 SendMessage(hOwn, WM_COMMAND, BOTO_AVANT, NULL);
					 return FALSE;
			}
			break;

	 case WM_KEYDOWN:
			switch(wParam)
			{ case VK_RIGHT:
					 SendMessage(hOwn, WM_COMMAND, BOTO_AVANT, NULL);
					 return FALSE;
			  case VK_LEFT:
					 SendMessage(hOwn, WM_COMMAND, BOTO_ENRERE, NULL);
					 return FALSE;
					 default: break;
			}
			break;

	 default: break;
  }

#endif

  if(!keybOn)
  { if(curAltMode!=NOAUTOCURSOR && autoCursor==TRUE &&
		 msg==WM_CHAR)
	 { POINT p;
		RECT r;
		GetCursorPos(&p);
		GetWindowRect(hWnd, &r);
		processMouse(WM_LBUTTONDOWN, 0, (LPARAM)MAKELONG(p.x-r.left, p.y-r.top));
	 }
	 return FALSE;
  }

  switch(actMode)
  { case TEXTACT:
			if(txc) return txc->processKeyboard(msg, wParam);
			break;

	 case ASSOCIA:
			switch(msg)
			{ case WM_CHAR:
					 switch(wParam)
					 { case VK_RETURN:
							  relaciona(NullPoint, NullPoint, 0xFF);
							  return TRUE;

						default:
							  if(wParam<' ' ||
								  (wParam>'~' && wParam<'¡')) break;
							  return casZ->insertChar(wParam & 0xFF);
					 }
					 break;

			  case WM_KEYDOWN:
					 switch(wParam)
					 { case VK_RIGHT:
						case VK_LEFT:
						case VK_HOME:
						case VK_END:
						case VK_BACK:
							  return casZ->insertChar(wParam | 0xFF00);
						default: break;
					 }
					 break;

			  default:
					 break;
			}
			break;

	 case CREUATS:
			if((casX=gra[0].getCasX())==NULL) return FALSE;
			switch(msg)
			{ case WM_CHAR:
					 switch(wParam)
					 { case VK_BACK:
						case VK_ESCAPE:
						case VK_RETURN:
						case VK_TAB:
							  return FALSE;
						default: break;
					 }
					 relaciona(NullPoint, NullPoint, (char)wParam);
					 switch(keybDir)
					 { case 1: wParam=VK_RIGHT; break;
						case 2: wParam=VK_DOWN; break;
						default: return TRUE;
					 }

			  case WM_KEYDOWN:
					 switch(wParam)
					 { case VK_RIGHT:
						case VK_LEFT:
						case VK_UP:
						case VK_DOWN:
						case VK_HOME:
						case VK_END:
						case VK_BACK:
							  casX->marked(FALSE);
							  return resituaCursor(wParam, 0);

						default: break;
					 }
					 break;

			  default: break;
			}
			break;

	 default: break;
  }
  return FALSE;
}

BOOL PlayWindow::processUserMess(WPARAM wParam, LPARAM lParam)
{
  int x, y;
  if(casH) {casH->marked(FALSE); casH=0;}

  if(!playing) return FALSE;
  x=LOWORD(lParam);
  y=HIWORD(lParam);

  switch(wParam)
  { case HELPW_ITEMSEL:
			if(nGra<2) break;
			if(mAss) x=ass[x];
			if(x<0 || x>=(nCw[1]*nCh[1])) break;
			casH=gra[1].localitza(x);
			break;

	 case HELPW_MOUSE:
			if(nGra<2) break;
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			casH=gra[1].localitza(gra[0].getVirtualId(x, y));
			break;

	 default: break;
  }
  if(casH==0) return FALSE;
  if(casH->isMarked()==FALSE) casH->marked(TRUE);
  return TRUE;
}

BOOL PlayWindow::nextCursorPos()
{
  int skh;

  if(!playing ||
	  !autoCursor ||
	  curAltMode==NOAUTOCURSOR ||
	  nGra<1 ||
	  curCurGra>=nGra ||
	  actMode==CREUATS) return FALSE;

  if(waitToPass) {waitToPass=FALSE; return TRUE;}

  skh=0;
  switch(actMode)
  { case ASSOCIA:
			skh=1;
			break;

	 case PUZZLE:
			switch(puzMode)
			{ case DOBLE:
					 if(curCurGra==0) skh=1; else skh=-1;
					 break;

			  case MEMORY:
					 skh=-1;
					 break;

			  case FORAT:
					 skh=1;
					 break;

			  default:
					 break;
			}

	 default:
			break;
  }

  gra[curCurGra].locateCursorHere(curCurCas, skh);
  curCurCas++;
  forcedPass=TRUE;

  return TRUE;
}

BOOL PlayWindow::processMouse(UINT msg, WPARAM wParam, LPARAM lParam)
{
  POINT p, q;
  Casella *c0;
  BOOL beep=FALSE;

  if(!playing) return FALSE;

  if(txc)
  { if(txc->waiting && msg==WM_LBUTTONDOWN)
      itsTime();
	 else
		txc->processMouse(msg, wParam, lParam);
	 return 0;
  }

  p.x=LOWORD(lParam);
  p.y=HIWORD(lParam);
  q=p;

  switch(msg)
  { case WM_ERASEBKGND:
			if(filOn) fil.hide();
			break;

	 case WM_LBUTTONDOWN:
			if(nGra<1) break;
			silenci(FALSE);
			if(curAltMode==AUTOAB) {curCurGra^=1; curCurCas=0;}
			if(filOn && fil.isAct())
			{ fil.getPos(p, q);
			  return relaciona(p, q);
			}
			else
			switch(actMode)
			{ case PUZZLE:
					 if(gra[0].isIn(p)==FALSE)
					 { beep=TRUE;
						break;
					 }
					 c0=gra[0].localitza(p);
					 switch(puzMode)
					 { case DOBLE:
							  if(c0==0 || c0->isHide())
							  { beep=TRUE; break;}

						case INTERC:
							  fil.init(p, hWnd, hFilCursor, c0);
							  c0->play();
							  return TRUE;

						case FORAT:
							  c0->play();
							  return relaciona(p, p);

						case MEMORY:
							  if(c0==0 || !c0->isHide())
							  { beep=TRUE; break;}
							  c0->hide(FALSE);
							  fil.init(p, hWnd, hFilCursor, 0);
							  c0->play();
							  return TRUE;

						default: beep=TRUE; break;
					 }
					 break;

			  case SOPA:
					 if((c0=gra[0].localitza(p))==0)
					 { beep=TRUE; break;}
					 fil.init(p, hWnd, hFilCursor, 0);
					 return TRUE;

			  case CREUATS:
					 if(GetFocus()!=hOwn) SetFocus(hOwn);
					 if((c0=gra[1].localitza(p))!=0)
					 { int i=0;
						if(!c0->play())
						switch(c0->getId())
						{ case 3:
						  case 2: i++;
						  case 1:
						  case 0: i++; return changeKeybDir(keybDir==i ? 0:i);
						  default: beep=TRUE; break;
						}
						break;
					 }
					 if(gra[0].localitza(p)==0) {beep=TRUE; break;}
					 if(casX) casX->marked(FALSE);
					 return resituaCursor(0, &p);

			  case ASSOCIA:
					 switch(puzMode)
					 { case INFO:
							  if(gra[0].isIn(p))
							  { c0=gra[0].localitza(p);
								 c0->play();
							  }
							  break;

						case IDENTIFICA:
							  if(!gra[0].isIn(p) ||
								  (c0=gra[0].localitza(p))==0 ||
								  c0->isHide()) {beep=TRUE; break;}
							  c0->play();
							  return relaciona(p, p);

						case EXPLORA:
							  if(gra[0].isIn(p)==FALSE)
							  { beep=TRUE;
								 break;
							  }
							  c0=gra[0].localitza(p);
							  c0->play();
							  return relaciona(p, p);

						case ESCRIU:
							  if(GetFocus()!=hOwn) SetFocus(hOwn);
							  if(gra[0].isIn(p)==FALSE)
							  { beep=TRUE;
								 break;
							  }
							  if(casX) relaciona(p, p);
							  c0=gra[0].localitza(p);
							  if(!c0->isHide())
							  { if(casX) casX->marked(FALSE);
								 casX=c0;
								 casX->play();
								 casX->marked(TRUE);
							  }
							  else beep=TRUE;
							  casZ->getData(nullStr);
							  if(casX) casZ->gainCaret();
							  return TRUE;

						default:
							  if(((c0=gra[0].localitza(p))==0 &&
								  (c0=gra[1].localitza(p))==0) ||
								  c0->isHide()) {beep=TRUE; break;}
							  fil.init(p, hWnd, hFilCursor, 0);
							  c0->play();
							  return TRUE;
					 }
					 break;

			  default: break;
			}
			break;

	 case WM_MOUSEMOVE:
			if(!forcedPass) waitToPass=TRUE;
			if(filOn) fil.move(p);
			return TRUE;

	 default: break;
  }
  if(beep) avisSonor(2);
  return FALSE;
}

BOOL PlayWindow::processPaint(HDC hDC0, HDC hMemDC0, LPRECT r0, BOOL toPrint)
{
  int i;
  //static char *msg[2]={CLICVSTR, "© Francesc Busquets, 1992-99"};
  static char *msg[2]={"", ""};
  //static char *msg[3]={CLICVSTR, "© Francesc Busquets, 1992-99", "\x22Informatika aztertzen sakonduz\x22 lan taldeak euskaratua"};
  //static char *msg[3]={CLICVSTR, "© Francesc Busquets, 1992-99", "Version en Lingua Galega: Emiliano Orizales"};

  HDC hDC, hMemDC;
  PAINTSTRUCT ps;
  RECT r;

  if(hDC0) hDC=hDC0;
  else hDC=BeginPaint(hWnd, &ps);

  r=hDC0 ? *r0 : ps.rcPaint;

	if(hMemDC0 || toPrint) hMemDC=hMemDC0;
	else hMemDC=CreateCompatibleDC(hDC);

	 if(caretVis) HideCaret(hWnd);
	 if(hDC0==NULL) fil.hide(hDC, hMemDC);
	  if(pwTransp && txc==0 && hPortadaBm==0 && !toPrint)
	  { for(i=0; i<nGra; i++) if(gra[i].isSubRect(&r)) break;
		 if(i==nGra)
		 { if(tileBmp==FALSE || hBmpFons==0)
			  FillRect(hDC, &r, Zona::getBaseBrush(GENERICA));

			if(hBmpFons)
			{ if(tileBmp)
			  { POINT pt;
				 pt.x=-wRect.left; pt.y=-wRect.top;
				 TileBmp(hDC, hMemDC, hBmpFons, bmpfW, bmpfH, pt, r);
			  }
			  else
				 displayBitmap(hDC, hMemDC, hBmpFons, bmpfX-wRect.left, bmpfY-wRect.top, bmpfW, bmpfH);
			}
		 }
	  }

	  if(hPortadaBm)
	  { int oldBkMode;
		 COLORREF oldTxColor;
		 HFONT hFont, hOldFont;

		 if(hPortadaPal)
		 { SelectPalette(hDC, hPortadaPal, FALSE);
			RealizePalette(hDC);
			SelectPalette(hMemDC, hPortadaPal, FALSE);
			RealizePalette(hMemDC);
		 }

		 displayBitmap(hDC, hMemDC, hPortadaBm, 0,0);
		 hFont=CreateFontIndirect(&logFont0);
		 hOldFont=(HFONT)SelectObject(hDC, hFont);
		 oldBkMode=SetBkMode(hDC, TRANSPARENT);
		 oldTxColor=SetTextColor(hDC, RGB(0, 0, 0x80));

		 //for(i=0; i<3; i++)
		 //	TextOut(hDC, 10, PORTH-68+20*i, msg[i], strlen(msg[i]));
		 for(i=0; i<2; i++)
			TextOut(hDC, 10, PORTH-48+20*i, msg[i], strlen(msg[i]));

		 SetTextColor(hDC, oldTxColor);
		 SetBkMode(hDC, oldBkMode);
		 SelectObject(hDC, hOldFont);
		 DelObj(hFont);
	  }
	  else if(txc) txc->updateX(hDC, &r, hMemDC, toPrint);
	  else for(i=0; i<nGra; i++) gra[i].update(hDC, &r, hMemDC);

	 if(hDC0==NULL) fil.show(hDC, hMemDC);
	 if(caretVis) ShowCaret(hWnd);
	if(hMemDC0==NULL && !toPrint) DeleteDC(hMemDC);
  if(hDC0==NULL) EndPaint(hWnd, &ps);
  return TRUE;
}

BOOL PlayWindow::ajustaMides(int mode, int botonsVisibles)
{
  int a, b, i, m, j, wW, wH;
  float frx, fry;

  MaxW=screenX-2*MARGE;
  if(barraAmunt==FALSE && botonsVisibles>1) MaxW-=(MARGE+BOTOW);
  MaxH=screenY-captionY-menuY-CTRLH-3*MARGE;
  if(barraAmunt) MaxH-=(MARGE+BOTOH);


  grr[0]=grr[1]=NullRect;
  frx=fry=1;
  nGra=0;

  switch(mode)
  { case PORT:
			wW=PORTW; wH=PORTH;
			break;

	 case DEFAULT:
			wW=PLWW; wH=PLWH;
			break;

	 case SINGLE:
			if(rw[0]>MaxW) frx=(float)MaxW/rw[0];
			if(rh[0]>MaxH) fry=(float)MaxH/rh[0];
			if(stretch) frx=fry=(frx<fry ? frx:fry);
			rw[0]*=frx; rh[0]*=fry;

			rw[0]=(rw[0]/nCw[0])*nCw[0];
			rh[0]=(rh[0]/nCh[0])*nCh[0];
			wW=rw[0]; wH=rh[0];
			nGra=1;
			break;

	 case DOUBLE:
			a=graPos&1 ? 1:0;
			b=graPos&1 ? 0:1;

			switch(graPos)
			{ case AB:
			  case BA:
					 m=rh[a]>rh[b] ? a:b;

					 if((i=rw[a]+rw[b])>MaxW-4*MARGE) frx=(float)(MaxW-4*MARGE)/i;
					 if(rh[m]>MaxH-2*MARGE) fry=(float)(MaxH-2*MARGE)/rh[m];
					 if(stretch) frx=fry=(frx<fry ? frx:fry);
					 for(i=0; i<2; i++)
					 { rw[i]*=frx;
						rw[i]=(rw[i]/nCw[i])*nCw[i];
						rh[i]*=fry;
						rh[i]=(rh[i]/nCh[i])*nCh[i];
					 }

					 grr[a].left=MARGE; grr[a].top=MARGE;
					 grr[b].left=rw[a]+3*MARGE; grr[b].top=MARGE;
					 if(rh[a]!=rh[b])
					 { j=m^1;
						grr[j].top+=(rh[m]-rh[j])/2;
					 }
					 wH=rh[m]+2*MARGE;
					 wW=rw[a]+4*MARGE+rw[b];
					 nGra=2;
					 break;

			  case AUB:
			  case BUA:
					 m=rw[a]>rw[b] ? a:b;

					 if((i=rh[a]+rh[b])>MaxH-4*MARGE) fry=(float)(MaxH-4*MARGE)/i;
					 if(rw[m]>MaxW-2*MARGE) frx=(float)(MaxW-2*MARGE)/rw[m];
					 if(stretch) fry=frx=(fry<frx ? fry:frx);
					 for(i=0; i<2; i++)
					 { rw[i]*=frx;
						rw[i]=(rw[i]/nCw[i])*nCw[i];
						rh[i]*=fry;
						rh[i]=(rh[i]/nCh[i])*nCh[i];
					 }

					 grr[a].top=MARGE; grr[a].left=MARGE;
					 grr[b].top=rh[a]+3*MARGE; grr[b].left=MARGE;
					 if(rw[a]!=rw[b])
					 { j=m^1;
						grr[j].left+=(rw[m]-rw[j])/2;
					 }
					 wW=rw[m]+2*MARGE;
					 wH=rh[a]+4*MARGE+rh[b];
					 nGra=2;
					 break;

			  default: return FALSE;
			}
			break;

	 case SINGCAS0:
			if(rw[0]+4*MARGE+rw[0]/nCw[0] > MaxW)
			  frx=(float)(nCw[0]*(MaxW-4*MARGE)/(nCw[0]+1))/rw[0];

			if(rh[0]>MaxH-2*MARGE) fry=(float)(MaxH-2*MARGE)/rh[0];
			if(stretch) frx=fry=(frx<fry ? frx:fry);
			rw[0]*=frx; rh[0]*=fry;

			rw[1]=rw[0]/nCw[0]; rh[1]=rh[0]/nCh[0];
			rw[0]=rw[1]*nCw[0]; rh[0]=rh[1]*nCh[0];
			grr[0].left=MARGE; grr[0].top=MARGE;
			grr[1].left=rw[0]+3*MARGE;
			grr[1].top=MARGE+rh[0]*(nCh[0]-1)/(2*nCh[0]);
			wW=rw[0]+4*MARGE+rw[1]; wH=rh[0]+2*MARGE;
			nGra=2;
			break;

	 default: return FALSE;
  }

  for(i=0; i<nGra; i++)
  { grr[i].right=grr[i].left+rw[i];
	 grr[i].bottom=grr[i].top+rh[i];
  }

  wRect.left=0; wRect.top=0;
  wRect.right=wW; wRect.bottom=wH;

  if(hOwn) SendMessage(hOwn, WM_PLAYW, PLAYW_RESIZE, 0);

  return TRUE;
}

BOOL PlayWindow::creaGraella(int ng, int cw, int ch,
									  BOOL iDl, BOOL iHid, BOOL iMark)
{
  gra[ng].destroy();
  gra[ng].create(cw, ch, hWnd, &grr[ng], iDl, iHid, iMark, (BASE_ZONES)ng);

  UpdateWindow(hOwn);
  return gra[ng].isCreated();
}

BOOL PlayWindow::ompleGraella(int ng, int iBmB, int iTxtB,
										int firstCas, int firstId, int gdCw, int gdCh)
{
  int h;

  if(!gra[ng].isCreated()) return FALSE;

  UpdateWindow(hOwn);

  if(iBmB!=-1 && hBmB[iBmB]!=0)
  { if(stretch==TRUE && hBmBX[iBmB]==NULL)
	 { h=(gdCh!=0 && gdCh!=nCh[ng] ? rh[ng]*gdCh/nCh[ng]: rh[ng]);

		hBmBX[iBmB]=mkStretchBm(iBmB, hWnd, hBmB[iBmB], rw[ng], h,
											 (bmbW[iBmB]/nCw[ng])*nCw[ng]-1,
											 (bmbH[iBmB]/nCh[ng])*nCh[ng]-1);
		//hBmBX[iBmB]=createNewBitmap(hWnd, hBmB[iBmB], rw[ng], h,
		//									 (bmbW[iBmB]/nCw[ng])*nCw[ng]-1,
		//									 (bmbH[iBmB]/nCh[ng])*nCh[ng]-1);
	 }
	 return gra[ng].getData((stretch ? hBmBX[iBmB]:hBmB[iBmB]),
									0, firstCas, firstId, gdCw, gdCh);
  }

  if(iTxtB!=-1 && txtB[iTxtB]!=0)
	 return gra[ng].getData(txtB[iTxtB], firstCas, firstId, gdCw*gdCh);

  return TRUE;
}

BOOL PlayWindow::relaciona(POINT p0, POINT p1, char c)
{
  int i, idpos, x0, y0, x1, y1, dx, dy, ltxt;
  BOOL intent, beeped;
  Casella *cas0=0, *cas1=0;
  char *txt;
  static char *txtbf=" ";

  if(nGra==0) return FALSE;

  intent=FALSE;
  beeped=FALSE;

  switch(actMode)
  { case PUZZLE:
			switch(puzMode)
			{ case INTERC:
					 fil.end(FALSE);
					 if((cas0=gra[0].localitza(p0))==0 ||
						 (cas1=gra[0].localitza(p1))==0) break;
					 *cas0 >> *cas1;
					 intent=TRUE;
					 break;

			  case FORAT:
					 if((cas0=gra[0].localitza(p0, idpos))==0 ||
						 (cas1=gra[0].veiBuit(idpos))==0) break;
					 *cas0 >> *cas1;
					 casX=cas0;
					 intent=TRUE;
					 break;

			  case DOBLE:
					 if((cas0=gra[0].localitza(arrossega ? p1:p0))==0 ||
						 (cas1=gra[1].localitza(p1))==0)
					 { avisSonor(0); beeped=TRUE; fil.end(TRUE); break;}

					 if(*cas0==*cas1 ||
						 *cas1>=*gra[1].localitza(cas0->getId()))
					 { fil.end(FALSE); cas0->hide(TRUE); cas1->hide(FALSE);}
					 else
					 { avisSonor(1); beeped=TRUE; fil.end(TRUE);}

					 intent=TRUE;
					 break;

			  case MEMORY:
					 fil.end(FALSE);
					 if((cas0=gra[0].localitza(p0))==0) break;
					 if((cas1=gra[0].localitza(p1))==0)
					 { cas0->hide(TRUE);
						break;
					 }
					 if(cas1->isHide()==TRUE)
					 { cas1->hide(FALSE);
						cas1->play();
						if(*cas0==*cas1 || *cas0>=*cas1)
						{}
						else
						{ cas0->hide(TRUE);
						  fil.init(p1, hWnd, hFilCursor, 0);
						}
					 }
					 else fil.init(p0, hWnd, hFilCursor, 0);
					 intent=TRUE;
					 break;
			}
			break;

	 case SOPA:
			fil.end(FALSE);
			if((cas0=gra[0].localitza(p0, x0, y0))==0 ||
				(cas1=gra[0].localitza(p1, x1, y1))==0) break;
			dx=x1-x0; dy=y1-y0;
			if((dx==0 && dy==0) ||
				(dx!=0 && dy!=0 && abs(dx)!=abs(dy))) break;
			if(dx) {ltxt=abs(dx)+1; dx=(dx<0 ? -1:1);}
			if(dy) {ltxt=abs(dy)+1; dy=(dy<0 ? -1:1);}
			if((txt=newChar(ltxt+1))==0) break;
			x1=x0; y1=y0;
			for(i=0; i<ltxt; i++, x0+=dx, y0+=dy)
			txt[i]=*txtB[0][x0+nCw[0]*y0];
			txt[i]=0;
			for(i=0; i<ntags[0]; i++)
			{ if(stricmp(txt, tags[0][i])==0) break; }
			delete txt;
			if(i<ntags[0] && cTags[i]==FALSE)
			{ cTags[i]=TRUE;
			  if(bar[0])
			  { (cas0=gra[1].getCas(i))->hide(FALSE);
				 cas0->play();
			  }
			  for(i=0; i<ltxt; i++, x1+=dx, y1+=dy)
				 gra[0].getCas(x1, y1)->marked(TRUE);
			}
			intent=TRUE;
			break;

	 case ASSOCIA:
			switch(puzMode)
			{ case INFO:
					 return TRUE;

			  case EXPLORA:
					 beeped=TRUE;
					 //Només per a AutoPass
					 PostMessage(hOwn, WM_PLAYW, PLAYW_INTENT, 1L);

					 if((cas0=gra[0].localitza(p0))==0)
					 { avisSonor(1); break;}
					 i=cas0->getId();
					 if(mAss) i=ass[i];
					 cas0=casZ;
					 if(txtB[1])
					 { cas0->getData(i>=0 && i<nLines[1] ? txtB[1][i]:nullStr);
						cas0->play();
					 }
					 else
					 { RECT r;
						int y=i/nctxw;
						r.top=y*rh[1];
						r.left=(i-(y*nctxw))*rw[1];
						r.right=r.left+rw[1];
						r.bottom=r.top+rh[1];
						cas0->getData(hBmBX[1], &r);
					 }
					 break;

			  case IDENTIFICA:
					 if((cas0=gra[0].localitza(p0))==0) break;
					 intent=TRUE;
					 if(ass[cas0->getId()]!=1)
					 { avisSonor(1); beeped=TRUE; break;}
					 cas0->hide(TRUE);
					 if(sol) gra[0].setAltData(cas0);
					 cas0->play();
					 break;


			  case ESCRIU:
					 if(casX)
					 { beeped=TRUE;
						i=casX->getId();
						txt=casZ->getTxt();
						if(*txt!=0)
						{ intent=TRUE;
						  if(cmpString(txt, i<nLines[1] ? txtB[1][i]:nullStr))
						  { casX->marked(FALSE);
							 casX->hide(TRUE);
							 if(sol)
							 { gra[0].setAltData(casX);
								casX->play();
							 }
							 if(c==0xFF)
							 { gra[0].setCasX(casX);
								cas0=casX;
								for(i=0; i<(nCw[0]*nCh[0]); i++)
								  if((casX=gra[0].despl(VK_RIGHT))==0 ||
									  casX==cas0 ||
									  !casX->isHide()) break;
								if(casX==cas0) casX=0;
								if(casX) {casX->marked(TRUE); casX->play();}
							 }
						  }
						  else {/*avisSonor(1);*/ beeped=TRUE;}
						}
						casZ->getData(nullStr);
						if(casX) casZ->gainCaret();
						break;
					 }
					 break;

			  default:
					 fil.end(FALSE);
					 i=-1;
					 if((cas0=gra[0].localitza(p0))!=0) i=1;
					 else if((cas0=gra[1].localitza(p0))!=0) i=0;
					 if(cas0==0) break;
					 if((cas1=gra[i].localitza(p1))==0) break;
					 if(mAss)
					 { if(!cas0->isHide() && !cas1->isHide() &&
					      ass[(i ? cas0 : cas1)->getId()]==(i ? cas1 : cas0)->getId())
						{ (i ? cas0:cas1)->hide(TRUE);
						  if(invAss) (i ? cas1:cas0)->hide(TRUE);
						}
						else {avisSonor(1); beeped=TRUE;}
					 }
					 else
					 { if(*cas0==*cas1 ||
							*cas1>=*gra[i].localitza(cas0->getId()) ||
							*cas0>=*gra[!i].localitza(cas1->getId())
							)
						{cas0->hide(TRUE); cas1->hide(TRUE);}
						else { avisSonor(1); beeped=TRUE;}
					 }
					 if(!beeped && sol)
					 { gra[0].setAltData(i ? cas0 : cas1);
						(i ? cas0 : cas1)->play();
					 }
					 intent=TRUE;
					 break;
			}
			break;

	 case CREUATS:
			if(!casX) break;
			*txtbf=c;
			AnsiUpper(txtbf);
			casX->getData(txtbf);
			intent=TRUE;
			break;

	 default:
			fil.end(FALSE);
			break;
  }

  if(!intent)
  { if(!beeped) avisSonor(0);
	 return FALSE;
  }

  return nouIntent();
}

BOOL PlayWindow::nouIntent()
{
  BOOL acabat;
  int i, j;

  acabat=testEndGame(i);
  if(txc!=NULL && txc->btCorregir!=NULL)
  { j=movToSolv-encertsNow;
	 if(j<1) j=1;
	 PostMessage(hOwn, WM_PLAYW, PLAYW_INTENT, (long)j);
  }
  else
  { PostMessage(hOwn, WM_PLAYW, PLAYW_INTENT, 1L);
  }
  PostMessage(hOwn, WM_PLAYW, PLAYW_ENCERTS, (long)i-iniSolved);

  if(acabat)
  { PostMessage(hOwn, WM_PLAYW, PLAYW_SETMESSPLAY, (long)endMess);
	 PostMessage(hOwn, WM_PLAYW, PLAYW_ENDGAME, 0L);
	 if(txc) txc->endGame();
	 playing=FALSE;
  }
  return TRUE;
}

BOOL PlayWindow::resetGame()
{
  static int itera=0;
  int i, j, k;
  char c;

  if(actMode!=TEXTACT &&
	  (!nGra || !gra[0].isCreated() || (nGra==2 && !gra[1].isCreated())))
	 return FALSE;

  fil.end();
  filOn=FALSE; keybOn=FALSE;
  curCurCas=0; curCurGra=0;

  SetFocus(hOwn);

  silenci();
  UpdateWindow(hOwn);
  switch(actMode)
  { case PUZZLE:
			filOn=TRUE;
			switch(puzMode)
			{ case DOBLE:
					 gra[0].hide(FALSE);
					 gra[1].hide(TRUE);

			  case INTERC:
					 for(i=0; i<passos; i++)
					 { gra[0].barreja(1);
						if(!quick) UpdateWindow(hWnd);
						if(itera && !testEndGame(k)) break;
					 }
					 if(quick) UpdateWindow(hWnd);
					 curAltMode=(puzMode==DOBLE ? AUTOAB:AUTOA);
					 break;

			  case FORAT:
					 if(casX) casX->hide(FALSE);
					 if((casX=gra[0].getRndCas())==0) return FALSE;
					 *casZ=*casX;
					 casX->hide(TRUE);

					 for(i=0; i<passos; i++)
					 { if((casX=gra[0].barrejaVeiBuit(1))==0) return FALSE;
						if(!quick) UpdateWindow(hWnd);
						if(itera && !testEndGame(k)) break;
					 }
					 if(quick) UpdateWindow(hWnd);
					 curAltMode=AUTOA;
					 break;

			  case MEMORY:
					 gra[0].hide(TRUE);
					 for(i=0; i<passos; i++)
					 { gra[0].barreja(1);
						if(itera && !testEndGame(k)) break;
					 }
					 UpdateWindow(hWnd);
					 curAltMode=AUTOA;
					 break;

			  default: break;
			}
			break;

	 case CREUATS:
			keybOn=TRUE;
			gra[0].getData(txtB[0]);
			gra[0].hideSelective(NEGRE);
			(casX=gra[0].getCasX())->marked(FALSE);
			changeKeybDir(1);
			resituaCursor(0, 0);
			curAltMode=AUTOA;
			break;

	 case SOPA:
			filOn=TRUE;
			gra[0].marked(FALSE);
			if(bar[0]) gra[1].hide(TRUE);
			for(i=0; i<ntags[0]; i++) cTags[i]=FALSE;
			for(i=0, k=0; i<nCh[0]; i++)
			  for(j=0; j<nCw[0]; j++, k++)
			  { if((c=graTxt[i][j])==COMODI) c=65+random(90-64);
				 *txtB[0][k]=c;
				 gra[0].getCas(j, i)->getData(txtB[0][k]);
			  }
			curAltMode=AUTOA;
			break;

	 case ASSOCIA:
			switch(puzMode)
			{ case INFO:
					 UpdateWindow(hWnd);
					 break;

			  case ESCRIU:
					 keybOn=TRUE;
					 gra[0].hide(FALSE);
					 gra[0].marked(FALSE);
					 gra[0].resetData();

					 casZ->getData(nullStr);
					 casX=0;
					 if(bar[0])
						for(i=0; i<passos; i++)
						{ gra[0].barreja(1);
						  if(!quick) UpdateWindow(hWnd);
						}
					 if(quick) UpdateWindow(hWnd);

					 casX=gra[0].getCas(0);
					 if(casX)
					 { casX->marked(TRUE);
						casX->play();
						casZ->gainCaret();
					 }
					 curAltMode=AUTOA;
					 break;

			  case IDENTIFICA:
					 gra[0].hide(FALSE);
					 gra[0].resetData();
					 if(bar[0])
						for(i=0; i<passos; i++)
						{ gra[0].barreja(1);
						  if(!quick) UpdateWindow(hWnd);
						}
					 if(quick) UpdateWindow(hWnd);
					 curAltMode=AUTOA;
					 break;

			  case EXPLORA:
					 casZ->getData(nullStr);
					 UpdateWindow(hWnd);
					 curAltMode=AUTOA;
					 break;

			  default:
					 filOn=TRUE;
					 gra[0].hide(FALSE);
					 gra[1].hide(FALSE);
					 gra[0].resetData();

					 if(mAss)
					 { int nc=nCw[0]*nCh[0];
						Casella *cas;
						for(i=0; i<nc; i++)
						  if(ass[i]==-1)
						  { cas=gra[0].getCas(i);
							 cas->hide(TRUE);
							 if(sol) gra[0].setAltData(cas);
						  }
					 }

					 if(bar[0] || bar[1])
						for(i=0; i<passos; i++)
						{ if(bar[0]) gra[0].barreja(1);
						  if(bar[1]) gra[1].barreja(1);
						  if(!quick) UpdateWindow(hWnd);
						  if(itera && !testEndGame(k)) break;
						}
					 if(quick) UpdateWindow(hWnd);
					 curAltMode=AUTOAB;
					 break;
			}
			break;

	 case TEXTACT:
			if(txc==NULL ||
				txc->getActData(this, avPrevHelp!=0)==FALSE) return FALSE;
			if(avPrevHelp)
			  movToSolv=0;
			else
			{ // MODIFICACIO FETA PER COMPTAR 1 MOVTOSOLV QUAN HI HA BOTO
			  //if(btCorregir) movToSolv=1;
			  //else
				 movToSolv=txc->countIncs();
			  txc->scramble(passos, !quick);
			}
			keybOn=TRUE;
			UpdateWindow(hOwn);
			break;

	 default: return FALSE;
  }

  reported=FALSE; solved=FALSE;
  if(++itera>3) {itera=0; return FALSE;}

  if(avPrevHelp==0)
  { if(testEndGame(i)) return resetGame();}
  else i=0;

  SendMessage(hOwn, WM_PLAYW, PLAYW_SETMESSPLAY, (DWORD)(avPrevHelp ? initMessPrev:initMess));

  PostMessage(hOwn, WM_PLAYW, PLAYW_ENCERTS, (DWORD)i-iniSolved);
  if(avPrevHelp) PostMessage(hOwn, WM_PLAYW, PLAYW_WAITMEIN, (DWORD)avTimePH);
  playing=TRUE;
  itera=0;

  return TRUE;
}

BOOL PlayWindow::itsTime()
{
  if(txc!=NULL && txc->waiting==TRUE)
  { txc->getActData(this, FALSE);
	 movToSolv=txc->countIncs();
	 txc->scramble(passos, !quick);
	 SendMessage(hOwn, WM_PLAYW, PLAYW_SETMESSPLAY, (DWORD)initMess);
	 SendMessage(hOwn, WM_PLAYW, PLAYW_WAITMEIN, (DWORD)0L);
	 UpdateWindow(hOwn);
  }

  return TRUE;
}

BOOL PlayWindow::testEndGame(int &encerts)
{
  int i, j, k;
  char c;
  BOOL bo;
  solved=FALSE;

  switch(actMode)
  { case PUZZLE:
			switch(puzMode)
			{ case DOBLE:
					 solved=gra[0].isHide(encerts);
					 break;

			  case INTERC:
					 solved=gra[0].checkIds(encerts);
					 break;

			  case FORAT:
					 if(gra[0].checkIds(encerts)==TRUE)
					 { casX->hide(FALSE);
						casZ->hide(TRUE);
						solved=TRUE;
					 }
					 break;

			  case MEMORY:
					 bo=gra[0].isNotHide(encerts);
					 encerts/=2;
					 solved=bo;
					 break;
			}
			break;

	 case SOPA:
			for(i=0, encerts=0; i<ntags[0]; i++)
			  if(cTags[i]==TRUE) encerts++;
			solved=(encerts==ntags[0] ? TRUE:FALSE);
			break;

	 case CREUATS:
			encerts=0;
			k=0; bo=TRUE;
			for(i=0; i<nCh[0]; i++)
			  for(j=0; j<nCw[0]; j++, k++)
			  { if((c=graTxt[i][j])==COMODI) continue;
				 if(gra[0].getCas(k)->getChar()==c) encerts++;
				 else bo=FALSE;
			  }
			solved=bo;
			break;

	 case ASSOCIA:
			switch(puzMode)
			{ case INFO:
			  case EXPLORA:
					 break;

			  case IDENTIFICA:
					 gra[0].isHide(encerts);
					 solved=(encerts==movToSolv);
					 break;

			  default:
					 solved=gra[(mAss && invAss) ? 1:0].isHide(encerts);
					 break;
			}
			break;

	 case TEXTACT:
			solved=txc->getEncerts(encerts);
			break;

	 default: break;
  }
  return solved;
}

BOOL PlayWindow::showHelp()
{
  RECT r=grr[0];
  DAjuda *da=0;
  char **txctx=NULL;

  switch(actMode)
  { case PUZZLE:
			if(puzMode==MEMORY) r.bottom-=RECTH(r)/2;
			da=new DAjuda(hWnd, actMode, puzMode, &r, (hBmBX[0] ? hBmBX[0]:hBmB[0]), txtB[0], FALSE);
			break;

	 case SOPA:
			da=new DAjuda(hWnd, actMode, puzMode, &r, 0, tags[0], TRUE);
			break;

	 case ASSOCIA:
			if(puzMode==ESCRIU) return FALSE;
			da=new DAjuda(hWnd, actMode, puzMode, &r, (hBmBX[0] ? hBmBX[0]:hBmB[0]), txtB[0], TRUE);
			break;

	 case TEXTACT:
			if(txc==NULL) return FALSE;
			if((txctx=newArray(2))!=NULL && (txctx[0]=txc->getCleanSource())!=NULL)
			  da=new DAjuda(hWnd, actMode, puzMode, &r, NULL, txctx, FALSE);
			break;

	 default: return FALSE;
  }

  if(da)
  { da->run();
	 delete da;
  }
  else NOMEM;

  if(txctx) delArray(txctx);

  if(casH) {casH->marked(FALSE); casH=0;}
  return TRUE;
}

BOOL PlayWindow::resituaCursor(int vkCode, POINT *p)
{
  int i, x, y;
  char *txt0, *txt1;

  if(actMode!=CREUATS) return FALSE;
  if(p && (casX=gra[0].situa(*p))==0) return FALSE;
  if(!vkCode && (unsigned char)casX->getChar()==NEGRE) vkCode=VK_RIGHT;

  if(vkCode)
	 for(i=0; i<100; i++)
		if((casX=gra[0].despl(i>=nCh[0] ? VK_RIGHT : vkCode))!=0 &&
			(unsigned char)casX->getChar()!=NEGRE) break;

  if((casX=gra[0].getCasX(x, y))==0) return FALSE;
  casX->marked(TRUE);

  txt0=getChainInTag(y, x);
  txt1=gra[1].getCas(1)->getTxt();
  if(txt1==0 || strcmp(txt1, txt0)) gra[1].getCas(1)->getData(txt0);

  i=nCh[0]+x;
  txt0=getChainInTag(i, y);
  txt1=gra[1].getCas(3)->getTxt();
  if(txt1==0 || strcmp(txt1, txt0)) gra[1].getCas(3)->getData(txt0);

  return TRUE;
}

char *PlayWindow::getChainInTag(int it, int pos)
{
  static char result[MAX_MESS_LEN+1];
  static char base[MAXGW+MAXGH+1];

  BOOL s;
  char c;
  int  i, j, k;
  char *txt;

  memset(base, 0, sizeof(base));
  *result=0;

  if(it<nctxh) strncpy(base, graTxt[it], sizeof(base)-1);
  else for(i=it-nctxh, k=0; k<nctxh; k++) base[k]=graTxt[k][i];

  s=(*base==COMODI);
  for(k=0, i=0; (c=base[i])!=0 && i<pos; i++)
	 if(s)
	 { if(c==COMODI) continue; else s=FALSE;}
	 else if(c==COMODI) {s=TRUE; k++;}

  if(i!=pos) return result;

  j=0;
  if(it>=nctxh) {it-=nctxh; j=1;}

  for(txt=tags[j][it], i=0; i<k && (c=*txt)!=0; txt++)
	if(c==SEPARADOR) i++;

  if(!*txt) return result;
  for(i=0; (c=txt[i])!=0 && c!=SEPARADOR && i<MAX_MESS_LEN; i++)
	 result[i]=c;
  result[i]=0;

  return result;
}

BOOL PlayWindow::changeKeybDir(int newDir)
{
  keybDir=newDir;
  gra[1].getCas(0)->getData(getStringTbl(newDir==1 ? IDS_HORITZ2:IDS_HORITZ));
  gra[1].getCas(2)->getData(getStringTbl(newDir==2 ? IDS_VERT2:IDS_VERT));
  return TRUE;
}



