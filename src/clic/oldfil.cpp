// -----------------------------------
// Implementaci¢ de Fil
// -----------------------------------

Fil::Fil()
{ active=FALSE;
  visible=FALSE;
  hWnd=0;
  hOldCursor=0;
  cont=0;
  dxCont=dyCont=0;
  oldContRect=NullRect;
  currRect=0;
  hBmCont=0;
}

void Fil::display(HDC hOrgDC, HDC hMemDC)
{
  HDC hDC;

  if(!active) return;

  hDC=(hOrgDC ? hOrgDC : GetDC(hWnd));
  if(cont)
  { if(visible)
    { if(hBmCont)
      { displayBitmap(hDC, hBmCont,
		      curPosCont.x, curPosCont.y, lxCont, lyCont,
		      0, 0, 0, 0, SRCCOPY, hMemDC);
	DeleteObject(hBmCont);
	hBmCont=0;
      }
    }
    else
    { if(hBmCont) DeleteObject(hBmCont);
      hBmCont=getBmRect(hDC, currRect, 0, hMemDC);
      cont->update(hDC, 0, hMemDC);
    }
  }
  else
  { SetROP2(hDC, R2_NOT);
    MoveTo(hDC, origen.x, origen.y);
    LineTo(hDC, desti.x, desti.y);
  }

  if(!hOrgDC) ReleaseDC(hWnd, hDC);
  visible^=1;
}

void Fil::init(POINT ini, HWND hParent, HCURSOR hDispCursor, Contenidor *c)
{
  hCursor=(hDispCursor ? hDispCursor:LoadCursor(NULL, IDC_CROSS));

  if(active) end();
  hWnd=hParent;
  origen=desti=ini;
  if((cont=c)!=0)
  { currRect=cont->getRect();
    oldContRect=*currRect;
    lxCont=RECTW(oldContRect);
    lyCont=RECTH(oldContRect);
    curPosCont.x=oldContRect.left;
    curPosCont.y=oldContRect.top;
    dxCont=ini.x-oldContRect.left;
    dyCont=ini.y-oldContRect.top;

    if(hBmCont) {DeleteObject(hBmCont); hBmCont=0;}
    cont->capture(TRUE);
    InvalidateRect(hWnd, currRect, TRUE);
    UpdateWindow(hWnd);
  }
  active=TRUE;
  visible=FALSE;
  show();
//  setCursor(hWnd, cont ? CAP_CURSOR : hCursor);
  setCursor(hWnd, hCursor);
}

#define LPASSOS 15
void Fil::end(BOOL walk)
{
  if(!active) return;
  if(cont && walk)
  { int i, np;
    float x, y, dx, dy;
    HDC hDC;
    POINT oldDest, tempDest;

    oldDest=desti;
    dx=desti.x-origen.x;
    dy=desti.y-origen.y;
    np=int((abs(dx)>abs(dy) ? abs(dx):abs(dy))/LPASSOS);
    if(np)
    { dx/=np; dy/=np;
      tempDest=oldDest=desti;
      hDC=GetDC(hWnd);
      for(x=tempDest.x, y=tempDest.y, i=0; i<np; i++)
      { move(tempDest, hDC);
	tempDest.x=int(x-=dx);
	tempDest.y=int(y-=dy);
      }
      hide(hDC);
      ReleaseDC(hWnd, hDC);
      desti=oldDest;
    }
  }
  else hide();
  active=FALSE;
  resetCursor();
  if(cont)
  { cont->move(&oldContRect);
    cont->capture(FALSE);
    cont=0;
    if(hBmCont) {DeleteObject(hBmCont); hBmCont=0;}
    currRect=0;
  }
}

void Fil::show(HDC hDC, HDC hMemDC)
{
  if(visible) return;
  display(hDC, hMemDC);
}

void Fil::hide(HDC hDC, HDC hMemDC)
{
  if(!visible) return;
  display(hDC, hMemDC);
}

void Fil::move(POINT nou, HDC hMainDC, HDC hMemDC)
{
   HDC hDC;

   if(!active) return;

   hDC=hMainDC ? hMainDC : GetDC(hWnd);

   if(!cont)
   { hide(hDC, hMemDC);
     desti=nou;
     show(hDC, hMemDC);
   }
   else
   { HDC hTheMemDC;
     HBITMAP hMemBitmap, hBuffBitmap, hOldBitmap;
     RECT newRect, memRect;
     POINT p;
     int dxMem, dyMem;

     SetRect(&newRect, p.x=nou.x-dxCont, p.y=nou.y-dyCont,
		       p.x+lxCont, p.y+lyCont);

     UnionRect(&memRect, &newRect, currRect);

     dxMem=RECTW(memRect);
     dyMem=RECTH(memRect);

     hMemBitmap=CreateCompatibleBitmap(hDC, dxMem, dyMem);
      if(!(hTheMemDC=hMemDC)) hTheMemDC=CreateCompatibleDC(hDC);
       hOldBitmap=SelectObject(hTheMemDC, hMemBitmap);
	hBuffBitmap=getBmRect(hDC, &memRect);
	 displayBitmap(hTheMemDC, hBuffBitmap, 0, 0, dxMem, dyMem);
	DeleteObject(hBuffBitmap);

	curPosCont.x-=memRect.left;
	curPosCont.y-=memRect.top;

	hide(hTheMemDC);
	desti=nou;
	curPosCont=p;
	*currRect=newRect;

	OffsetRect(currRect, -memRect.left, -memRect.top);
	show(hTheMemDC);
	OffsetRect(currRect, memRect.left, memRect.top);

	SetRect(&newRect, 0, 0, dxMem, dyMem);
	hBuffBitmap=getBmRect(hTheMemDC, &newRect);
	 displayBitmap(hDC, hBuffBitmap,
			    memRect.left, memRect.top, dxMem, dyMem);
	DeleteObject(hBuffBitmap);

       SelectObject(hTheMemDC, hOldBitmap);
      if(!hMemDC) DeleteDC(hTheMemDC);
     DeleteObject(hMemBitmap);
   }
   if(!hMainDC) ReleaseDC(hWnd, hDC);
}
