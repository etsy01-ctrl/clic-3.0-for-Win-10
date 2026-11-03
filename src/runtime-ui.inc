// Native Win32 player chrome. Activity rendering remains in PlayWindow.
#ifdef RUNTIME
#ifndef DT_END_ELLIPSIS
#define DT_END_ELLIPSIS 0x8000
#endif
static void uiFill(HDC dc, const RECT *r, COLORREF color)
{
  HBRUSH b=CreateSolidBrush(color);
  FillRect(dc, r, b);
  DeleteObject(b);
}

static void uiCard(HDC dc, RECT r, COLORREF fill, COLORREF border, int radius)
{
  HBRUSH b=CreateSolidBrush(fill);
  HPEN p=CreatePen(PS_SOLID, 1, border);
  HGDIOBJ oldB=SelectObject(dc, b), oldP=SelectObject(dc, p);
  RoundRect(dc, r.left, r.top, r.right, r.bottom, radius, radius);
  SelectObject(dc, oldB); SelectObject(dc, oldP);
  DeleteObject(b); DeleteObject(p);
}

static void uiText(HDC dc, HFONT font, COLORREF color, RECT r,
                   const char *text, UINT align=DT_LEFT)
{
  HGDIOBJ old=SelectObject(dc, font);
  SetBkMode(dc, TRANSPARENT);
  SetTextColor(dc, color);
  DrawText(dc, text, -1, &r, align|DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX|DT_END_ELLIPSIS);
  SelectObject(dc, old);
}

BOOL MainWindow::adjustModernSize()
{
  int width, height, pwW=RECTW(pw.wRect), pwH=RECTH(pw.wRect), y;
  RECT area;
  if(hWnd) GetClientRect(hWnd, &clRect);
  else
  {
    SetRect(&clRect, 0, 0, uiMin(1120, screenX-64), uiMin(760, screenY-100));
    wRect=clRect;
    AdjustWindowRect(&wRect, MWSTYLE, TRUE);
  }
  width=RECTW(clRect); height=RECTH(clRect);
  minSizeX=uiMax(920, pwW+64); minSizeY=uiMax(640, pwH+280);
  SetRect(&area, 32, 104, width-32, height-164);
  pw.wRect.left=uiMax(area.left, (width-pwW)/2);
  pw.wRect.top=uiMax(area.top, area.top+(RECTH(area)-pwH)/2);
  pw.wRect.right=pw.wRect.left+pwW;
  pw.wRect.bottom=pw.wRect.top+pwH;
  pw.bmpfX=pw.wRect.left; pw.bmpfY=pw.wRect.top;
  SetRect(&messRect, 40, height-136, uiMax(220, width-370), height-96);
  if(!hWnd) return TRUE;
  if(pw.hWnd) { MoveWindow(pw.hWnd, pw.wRect.left, pw.wRect.top, pwW, pwH, TRUE); ShowWindow(pw.hWnd, *pw.fileName && pw.actMode!=NONE ? SW_SHOW:SW_HIDE); }
  y=height-58;
  bwcb[BDISC].move(24, y, 116, 40);
  bwcb[BINICI].move(152, y, 108, 40);
  bwcb[BAJUDA].move(272, y, 84, 40);
  bwcb[BENRERE].move(width-472, y, 116, 40);
  bwcb[BAVANT].move(width-344, y, 128, 40);
  bwcb[BSURT].move(width-104, y, 80, 40);
  bwcb[BDISC].show(TRUE); bwcb[BSURT].show(TRUE);
  bwcb[BINICI].show(TRUE); bwcb[BAJUDA].show(TRUE);
  bwcb[BENRERE].show(TRUE); bwcb[BAVANT].show(TRUE);
  bwcb[BPRINT].show(FALSE); bwcb[BPUZZLE].show(FALSE); bwcb[BINFO].show(FALSE);
  messBox.move(&messRect);
  for(int i=0; i<3; i++) digit[i].hide(TRUE);
  EMI(BOTO_PRINTER, bVis[BPRINT]); EMI(BOTO_PUZZLE, bVis[BPUZZLE]);
  EMI(BOTO_INFO, bVis[BINFO]);
  EMI(IDFA_AJUDA, IsWindowEnabled(bwcb[BAJUDA].hWnd));
  EMI(IDFA_INICI, IsWindowEnabled(bwcb[BINICI].hWnd));
  InvalidateRect(hWnd, NULL, FALSE);
  return TRUE;
}

BOOL MainWindow::drawModernButton(LPARAM param)
{
  LPDRAWITEMSTRUCT d=(LPDRAWITEMSTRUCT)param;
  if(!d || d->CtlType!=ODT_BUTTON) return FALSE;
  BOOL primary=d->CtlID==BOTO_AVANT;
  BOOL dark=d->CtlID==BOTO_DISC;
  BOOL disabled=(d->itemState & ODS_DISABLED)!=0;
  BOOL pressed=(d->itemState & ODS_SELECTED)!=0;
  COLORREF fill=RGB(255,255,255), border=RGB(215,224,234), ink=RGB(51,65,85);
  if(primary) {fill=RGB(35,65,200); border=fill; ink=RGB(255,255,255);}
  if(dark) {fill=RGB(30,41,59); border=fill; ink=RGB(255,255,255);}
  if(pressed) {fill=primary ? RGB(27,49,153):RGB(226,232,240); if(!primary) ink=RGB(15,23,42);}
  if(disabled) {fill=RGB(241,245,249); border=RGB(232,237,243); ink=RGB(148,163,184);}
  uiFill(d->hDC, &d->rcItem, RGB(255,255,255));
  RECT r=d->rcItem; r.right--; r.bottom--;
  uiCard(d->hDC, r, fill, border, 10);
  char text[80]; GetWindowText(d->hwndItem, text, sizeof(text));
  uiText(d->hDC, uiFont, ink, r, text, DT_CENTER);
  if(d->itemState & ODS_FOCUS) {InflateRect(&r, -4, -4); DrawFocusRect(d->hDC, &r);}
  return TRUE;
}

BOOL MainWindow::paintModern()
{
  PAINTSTRUCT ps;
  HDC dc=BeginPaint(hWnd, &ps);
  int width=RECTW(clRect), height=RECTH(clRect), i;
  RECT r, shadow, canvas;
  char text[200];
  uiFill(dc, &clRect, RGB(243,246,255));
  SetRect(&r, 0, 0, width, 82); uiFill(dc, &r, RGB(255,255,255));
  SetRect(&r, 0, 81, width, 82); uiFill(dc, &r, RGB(223,231,239));
  SetRect(&r, 24, 23, 60, 59); uiCard(dc, r, RGB(35,65,200), RGB(35,65,200), 12);
  uiText(dc, uiValueFont, RGB(255,255,255), r, "C", DT_CENTER);
  SetRect(&r, 76, 14, width-310, 42);
  uiText(dc, uiTitleFont, RGB(15,23,42), r, "Clic Player");
  SetRect(&r, 77, 43, width-310, 65);
  uiText(dc, uiSmallFont, RGB(100,116,139), r,
    *runtimePackage ? runtimePackage : tr(PT_INTERACTIVE));
  if(!pac.isEmpty()) sprintf(text, tr(PT_ACTIVITY_FMT), pac.act+1, pac.nActs);
  else strcpy(text, *pw.fileName ? tr(PT_SINGLE_ACTIVITY) : tr(PT_OPEN_TO_START));
  SetRect(&r, width-286, 22, width-24, 45);
  uiText(dc, uiFont, RGB(51,65,85), r, text, DT_RIGHT);
  SetRect(&r, width-204, 54, width-24, 58); uiFill(dc, &r, RGB(226,232,240));
  if(!pac.isEmpty()) {r.right=r.left+180*(pac.act+1)/uiMax(1,pac.nActs); uiFill(dc, &r, RGB(35,65,200));}

  SetRect(&canvas, 0, 83, width, height-156);
  if(pw.hBmpFons)
  {
    int saved=SaveDC(dc);
    IntersectClipRect(dc, canvas.left, canvas.top, canvas.right, canvas.bottom);
    HDC mem=CreateCompatibleDC(dc);
    if(pw.tileBmp) TileBmp(dc, mem, pw.hBmpFons, pw.bmpfW, pw.bmpfH, NullPoint, canvas);
    else displayBitmap(dc, mem, pw.hBmpFons, pw.bmpfX, pw.bmpfY, pw.bmpfW, pw.bmpfH);
    DeleteDC(mem); RestoreDC(dc, saved);
  }
  if(*pw.fileName)
  {
    r=pw.wRect; InflateRect(&r, 14, 14);
    shadow=r; OffsetRect(&shadow, 0, 3);
    uiCard(dc, shadow, RGB(229,235,242), RGB(229,235,242), 14);
    uiCard(dc, r, RGB(255,255,255), RGB(220,228,237), 14);
  }
  if(!*pw.fileName) paintPlayerBranding(dc,canvas);
  SetRect(&r, 24, height-144, width-24, height-88);
  uiCard(dc, r, RGB(255,255,255), RGB(223,231,239), 12);
  if(!messBox.isEmpty()) messBox.update(dc);
  else
  {
    r=messRect;
    uiText(dc, uiFont, RGB(100,116,139), r, tr(PT_SELECT_TO_START));
  }
  SetRect(&r, width-344, height-132, width-343, height-100);
  uiFill(dc, &r, RGB(226,232,240));
  const char *labels[3]={tr(PT_SCORE),tr(PT_TRIES),tr(PT_TIME)};
  for(i=0; i<3; i++)
  {
    int x=width-328+98*i;
    SetRect(&r, x, height-139, x+86, height-120);
    uiText(dc, uiSmallFont, RGB(100,116,139), r, labels[i]);
    if(!pw.comptadors[i] || (i<2 && pw.noEvaluable())) strcpy(text, "--");
    else if(i==2) sprintf(text, "%02d:%02d", digit[i].getVal()/60, digit[i].getVal()%60);
    else sprintf(text, "%d", digit[i].getVal());
    SetRect(&r, x, height-121, x+86, height-94);
    uiText(dc, uiValueFont, RGB(30,41,59), r, text);
  }
  SetRect(&r, 0, height-76, width, height); uiFill(dc, &r, RGB(255,255,255));
  SetRect(&r, 0, height-76, width, height-75); uiFill(dc, &r, RGB(223,231,239));
  EndPaint(hWnd, &ps);
  return TRUE;
}
#endif
