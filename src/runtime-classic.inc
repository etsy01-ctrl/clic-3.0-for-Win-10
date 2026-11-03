// JClic-inspired classic skin. All artwork is drawn with native GDI.
#ifdef RUNTIME
static COLORREF classicFace=RGB(221,230,249);
static COLORREF classicCanvas=RGB(243,246,255);

static void classicBevel(HDC dc, RECT r, BOOL sunken)
{
  HPEN light=CreatePen(PS_SOLID, 1, RGB(255,255,255));
  HPEN dark=CreatePen(PS_SOLID, 1, RGB(143,161,208));
  HGDIOBJ old=SelectObject(dc, sunken ? dark:light);
  MoveToEx(dc, r.left, r.bottom-1, NULL); LineTo(dc, r.left, r.top); LineTo(dc, r.right-1, r.top);
  SelectObject(dc, sunken ? light:dark);
  LineTo(dc, r.right-1, r.bottom-1); LineTo(dc, r.left, r.bottom-1);
  SelectObject(dc, old); DeleteObject(light); DeleteObject(dark);
}

void MainWindow::applyRuntimeSkin()
{
  LOGFONT font;
  Colors colors={classicSkin ? classicFace:RGB(255,255,255), RGB(30,41,59), RGB(255,255,255), 0};
  memset(&font, 0, sizeof(font));
  font.lfHeight=classicSkin ? -15:-18; font.lfWeight=400;
  font.lfCharSet=DEFAULT_CHARSET; strcpy(font.lfFaceName, classicSkin ? "Tahoma":"Segoe UI");
  Zona::setBaseCont(&font, &colors, MSB); messBox.setBase(MSB);
  if(hMenu)
  {
    CheckMenuItem(hMenu, IDM_SKIN_CLASSIC, MF_BYCOMMAND|(classicSkin ? MF_CHECKED:MF_UNCHECKED));
    CheckMenuItem(hMenu, IDM_SKIN_MODERN, MF_BYCOMMAND|(classicSkin ? MF_UNCHECKED:MF_CHECKED));
  }
  adjustRuntimeSize();
  for(int i=0; i<NBOTONS; i++) if(bwcb[i].hWnd) InvalidateRect(bwcb[i].hWnd, NULL, TRUE);
}

void MainWindow::createRuntimeTooltips()
{
  static HINSTANCE common=LoadLibrary("comctl32.dll");
  if(uiTooltip) {DestroyWindow(uiTooltip); uiTooltip=NULL;}
  if(!common) return;
  typedef void (WINAPI *InitControls)();
  InitControls init=(InitControls)GetProcAddress(common, "InitCommonControls");
  if(init) init();
  uiTooltip=CreateWindowEx(WS_EX_TOPMOST, "tooltips_class32", NULL, WS_POPUP|1|2,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           hWnd, NULL, _hInstance, NULL);
  if(!uiTooltip) return;
  struct ToolV1 {UINT size, flags; HWND parent; UINT id; RECT rect; HINSTANCE instance; LPSTR text;};
  const PlayerText labels[NBOTONS]={PT_HINT_EXIT,PT_HINT_OPEN,PT_HINT_PRINT,PT_HINT_RESTART,PT_HINT_HELP,PT_HINT_PUZZLE,PT_HINT_INFO,PT_HINT_NEXT,PT_HINT_PREVIOUS};
  for(int i=0; i<NBOTONS; i++)
  {
    ToolV1 tool; memset(&tool, 0, sizeof(tool));
    tool.size=sizeof(tool); tool.flags=0x0001|0x0010;
    tool.parent=hWnd; tool.id=(UINT)bwcb[i].hWnd; tool.text=(char*)tr(labels[i]);
    SendMessage(uiTooltip, WM_USER+4, 0, (LPARAM)&tool);
  }
}

BOOL MainWindow::adjustRuntimeSize()
{
  if(!classicSkin) return adjustModernSize();
  int pwW=RECTW(pw.wRect), pwH=RECTH(pw.wRect), width, height;
  if(hWnd) GetClientRect(hWnd, &clRect);
  else
  {
    SetRect(&clRect, 0, 0, uiMin(1024,screenX-64), uiMin(550,screenY-100));
    wRect=clRect; AdjustWindowRect(&wRect, MWSTYLE, TRUE);
  }
  width=RECTW(clRect); height=RECTH(clRect);
  minSizeX=uiMax(640, pwW+30); minSizeY=uiMax(400, pwH+120);
  pw.wRect.left=uiMax(12,(width-pwW)/2);
  pw.wRect.top=uiMax(12,(height-66-pwH)/2);
  pw.wRect.right=pw.wRect.left+pwW; pw.wRect.bottom=pw.wRect.top+pwH;
  pw.bmpfX=pw.wRect.left; pw.bmpfY=pw.wRect.top;
  SetRect(&messRect, 103, height-52, width-130, height-23);
  if(!hWnd) return TRUE;
  if(pw.hWnd)
  {
    MoveWindow(pw.hWnd, pw.wRect.left, pw.wRect.top, pwW, pwH, TRUE);
    ShowWindow(pw.hWnd, *pw.fileName && pw.actMode!=NONE ? SW_SHOW:SW_HIDE);
  }
  bwcb[BENRERE].move(7,height-55,26,18);
  bwcb[BAVANT].move(35,height-55,26,18);
  bwcb[BINICI].move(63,height-55,26,18);
  bwcb[BAJUDA].move(7,height-35,26,17);
  bwcb[BINFO].move(35,height-35,26,17);
  bwcb[BDISC].move(63,height-35,26,17);
  for(int i=0; i<NBOTONS; i++)
    bwcb[i].show(i==BENRERE || i==BAVANT || i==BINICI || i==BAJUDA || i==BINFO || i==BDISC);
  EnableWindow(bwcb[BINFO].hWnd, TRUE);
  messBox.move(&messRect);
  for(i=0; i<3; i++) digit[i].hide(TRUE);
  EMI(BOTO_PRINTER,bVis[BPRINT]); EMI(BOTO_PUZZLE,bVis[BPUZZLE]); EMI(BOTO_INFO,pw.custHlp);
  EMI(IDFA_AJUDA,IsWindowEnabled(bwcb[BAJUDA].hWnd));
  EMI(IDFA_INICI,IsWindowEnabled(bwcb[BINICI].hWnd));
  InvalidateRect(hWnd,NULL,FALSE);
  return TRUE;
}

BOOL MainWindow::drawRuntimeButton(LPARAM param)
{
  if(!classicSkin) return drawModernButton(param);
  LPDRAWITEMSTRUCT d=(LPDRAWITEMSTRUCT)param;
  if(!d || d->CtlType!=ODT_BUTTON) return FALSE;
  RECT r=d->rcItem; uiFill(d->hDC,&r,classicFace);
  BOOL disabled=(d->itemState & ODS_DISABLED)!=0;
  COLORREF ink=disabled ? RGB(159,172,204):RGB(49,75,165);
  if(d->itemState & ODS_SELECTED) classicBevel(d->hDC,r,TRUE);
  int saved=SaveDC(d->hDC), x=(r.right-r.left)/2, y=(r.bottom-r.top)/2;
  HPEN pen=CreatePen(PS_SOLID,1,ink); HBRUSH brush=CreateSolidBrush(ink);
  SelectObject(d->hDC,pen); SelectObject(d->hDC,brush);
  if(d->CtlID==BOTO_AVANT || d->CtlID==BOTO_ENRERE)
  {
    int direction=d->CtlID==BOTO_AVANT ? 1:-1;
    POINT arrow[7]={{x-5*direction,y-2},{x,y-2},{x,y-6},{x+5*direction,y},
                    {x,y+6},{x,y+2},{x-5*direction,y+2}};
    Polygon(d->hDC,arrow,7);
  }
  else if(d->CtlID==BOTO_INICI)
  {
    MoveToEx(d->hDC,x-5,y+6,NULL); LineTo(d->hDC,x-5,y-6);
    HBRUSH flag=CreateSolidBrush(disabled ? RGB(159,172,204):RGB(59,91,201));
    SelectObject(d->hDC,flag);
    POINT points[6]={{x-4,y-5},{x+1,y-4},{x+6,y-6},{x+6,y+2},{x+1,y+4},{x-4,y+2}};
    Polygon(d->hDC,points,6); SelectObject(d->hDC,brush); DeleteObject(flag);
  }
  else if(d->CtlID==BOTO_INFO)
  {
    Ellipse(d->hDC,x-6,y-6,x+7,y+7);
    uiText(d->hDC,uiClassicFont,classicFace,r,"i",DT_CENTER);
  }
  else if(d->CtlID==BOTO_DISC)
    uiText(d->hDC,uiClassicFont,RGB(35,63,196),r,"Clic",DT_CENTER);
  else uiText(d->hDC,uiClassicValueFont,ink,r,"?",DT_CENTER);
  RestoreDC(d->hDC,saved); DeleteObject(pen); DeleteObject(brush);
  if(d->itemState & ODS_FOCUS) {InflateRect(&r,-1,-1); DrawFocusRect(d->hDC,&r);}
  return TRUE;
}

BOOL MainWindow::paintRuntime()
{
  if(!classicSkin) return paintModern();
  PAINTSTRUCT ps; HDC dc=BeginPaint(hWnd,&ps);
  int width=RECTW(clRect), height=RECTH(clRect), i;
  RECT r, canvas; char text[600];
  uiFill(dc,&clRect,classicFace);
  SetRect(&canvas,7,6,width-7,height-61);
  uiFill(dc,&canvas,classicCanvas); classicBevel(dc,canvas,TRUE);
  if(*pw.fileName && pw.actMode!=NONE)
  {
    if(pw.hBmpFons)
    {
      int saved=SaveDC(dc); IntersectClipRect(dc,canvas.left+1,canvas.top+1,canvas.right-1,canvas.bottom-1);
      HDC mem=CreateCompatibleDC(dc);
      if(pw.tileBmp) TileBmp(dc,mem,pw.hBmpFons,pw.bmpfW,pw.bmpfH,NullPoint,canvas);
      else displayBitmap(dc,mem,pw.hBmpFons,pw.bmpfX,pw.bmpfY,pw.bmpfW,pw.bmpfH);
      DeleteDC(mem); RestoreDC(dc,saved);
    }
  }
  else paintPlayerBranding(dc,canvas);
  SetRect(&r,99,height-55,width-126,height-20); classicBevel(dc,r,TRUE);
  if(!messBox.isEmpty()) messBox.update(dc);
  const char *labels[3]={tr(PT_CLASSIC_SCORE),tr(PT_CLASSIC_TRIES),tr(PT_CLASSIC_TIME)};
  for(i=0;i<3;i++)
  {
    int x=width-120+39*i;
    SetRect(&r,x,height-58,x+38,height-45);
    uiText(dc,uiClassicFont,RGB(42,57,100),r,labels[i],DT_CENTER);
    SetRect(&r,x,height-44,x+38,height-20); classicBevel(dc,r,TRUE);
    if(!*pw.fileName) *text=0;
    else if(!pw.comptadors[i] || (i<2 && pw.noEvaluable())) strcpy(text,"--");
    else sprintf(text,"%d",digit[i].getVal());
    uiText(dc,uiClassicValueFont,RGB(32,47,100),r,text,DT_CENTER);
  }
  SetRect(&r,1,height-17,104,height-1); classicBevel(dc,r,TRUE);
  r.left+=4; uiText(dc,uiClassicFont,RGB(32,47,100),r,tr(PT_READY));
  SetRect(&r,106,height-17,width-36,height-1); classicBevel(dc,r,TRUE); r.left+=5;
  if(!pac.isEmpty()) {char progress[100]; sprintf(progress,tr(PT_ACTIVITY_FMT),pac.act+1,pac.nActs); sprintf(text,"%s  |  %s",runtimePackage,progress);}
  else strcpy(text,runtimePackage);
  uiText(dc,uiClassicFont,RGB(42,57,100),r,text);
  SetRect(&r,width-34,height-17,width-1,height-1); classicBevel(dc,r,TRUE);
  int x=width-20,y=height-9;
  HPEN p=CreatePen(PS_SOLID,1,RGB(65,86,146)); HGDIOBJ old=SelectObject(dc,p);
  MoveToEx(dc,x-6,y-2,NULL); LineTo(dc,x-3,y-2); LineTo(dc,x+1,y-5);
  LineTo(dc,x+1,y+5); LineTo(dc,x-3,y+2); LineTo(dc,x-6,y+2); LineTo(dc,x-6,y-2);
  Arc(dc,x-3,y-5,x+7,y+6,x+4,y-4,x+4,y+4);
  SelectObject(dc,old); DeleteObject(p);
  EndPaint(hWnd,&ps); return TRUE;
}
#endif
