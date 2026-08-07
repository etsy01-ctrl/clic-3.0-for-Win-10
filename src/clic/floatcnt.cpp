// -------------------------
// FLOATCNT.CPP
// Implementacio de FloatCnt
// -------------------------

#include "ctext.h"
#include "clic.h"
#include "miscfunc.h"

BOOL FloatCnt::isRegistered=FALSE;


FloatCnt::FloatCnt()
{ wRect.left=wRect.top=0;
  wRect.right=wRect.bottom=20;
  visible=FALSE;
  idTimer=0;
  countDown=0;
  tAp=0;
  tMax=0;
  actiu=FALSE;
}


char *fcClassname="FLOATCNT";

BOOL FloatCnt::registerClass()
{
  if(isRegistered) return TRUE;

  WNDCLASS wc;

  wc.style =         CS_HREDRAW|CS_VREDRAW;
  wc.lpfnWndProc =   Window::wndProc;
  wc.cbClsExtra =    0;
  wc.cbWndExtra =    0;
  wc.hInstance =     hInst;
  wc.hIcon =         NULL;
  wc.hCursor =       hGenCursor;
  wc.hbrBackground = NULL;
  wc.lpszMenuName =  NULL;
  wc.lpszClassName = fcClassname;

  isRegistered=TRUE;
  return RegisterClass(&wc);
}

BOOL FloatCnt::createWindow()
{
  hMenu=NULL;
  if((hWnd = CreateWindow(fcClassname,
								  NULL,
								  WS_CHILD,
								  wRect.left,
								  wRect.top,
								  RECTW(wRect),
								  RECTH(wRect),
								  hOwn,
								  (HMENU)3333,
								  hInst,
								  NULL))==0) return FALSE;

  GetClientRect(hWnd, &clRect);
  c.create(hWnd, &clRect, FALSE, FALSE, POP);
  c.setFlag(MARC, TRUE);
  c.setFlag(LALIGN, TRUE);
  idTimer=SetTimer(hWnd, 1, 1000, NULL);
  return TRUE;
}

LRESULT FloatCnt::dispatch(UINT msg, WPARAM wParam, LPARAM lParam, int)
{
  switch(msg)
  { case WM_CLOSE:
			DestroyWindow(hWnd);
			return 0;

	 case WM_DESTROY:
			if(idTimer) KillTimer(hWnd, idTimer);
			break;

	 case WM_QUERYENDSESSION:
			return TRUE;

	 case WM_MOVE:
			GetClientRect(hWnd, &clRect);
			c.move(&clRect);
			break;

	 case WM_ERASEBKGND:
			c.update((HDC)wParam);
			return TRUE;

	 case WM_LBUTTONDOWN:
			c.play();
			return TRUE;

	 case WM_PAINT:
			{ HDC hDC, hMemDC;
			  PAINTSTRUCT ps;

			  hDC=BeginPaint(hWnd, &ps);
			  hMemDC=CreateCompatibleDC(hDC);
			  c.update(hDC, &ps.rcPaint, hMemDC);
			  DeleteDC(hMemDC);
			  EndPaint(hWnd, &ps);
			}
			return TRUE;

	 case WM_TIMER:
			if(!visible || countDown<=0) break;
			countDown--;
			if(countDown==0) show(actiu==FALSE);
			break;

	 default: break;
  }
  return Window::dispatch(msg, wParam, lParam);
}


BOOL FloatCnt::getData(char *txt, RECT pos, int fl, int tempsAp, int tempsMax)
{
  flags=fl;
  MoveWindow(hWnd, pos.left, pos.top, RECTW(pos), RECTH(pos), TRUE);
  c.getData(txt);
  c.setFlag(LALIGN, (flags & INFO_LA)!=0);
  tMax=tempsMax;
  tAp=tempsAp;
  actiu=FALSE;
  countDown=tAp;
  return TRUE;
}

BOOL FloatCnt::show(BOOL status, BOOL immediate)
{

  visible=status;
  actiu=(visible==TRUE && (idTimer==0 || countDown==0 || immediate==TRUE));

  if(hWnd)
  { if(actiu && (flags & INFO_ONLYPLAY)!=0)
		c.play();
	 else
	 {	ShowWindow(hWnd, actiu ? SW_SHOW : SW_HIDE);
		if(actiu) c.play();
		else silenci();
	 }
  }

  if(actiu) countDown=tMax;

  return visible;
}

