#ifdef RUNTIME
void MainWindow::applyRuntimeLanguage(BOOL rebuildMenu)
{
  if(rebuildMenu)
  {
    HMENU old=hMenu;
    hMenu=createPlayerMenu(); SetMenu(hWnd,hMenu);
    if(old) DestroyMenu(old);
    DrawMenuBar(hWnd);
  }
  const PlayerText labels[NBOTONS]={PT_EXIT,PT_OPEN,PT_PRINT,PT_RESTART,PT_HELP,
                                   PT_PUZZLE,PT_INFO,PT_NEXT,PT_PREVIOUS};
  for(int i=0;i<NBOTONS;i++) SetWindowText(bwcb[i].hWnd,tr(labels[i]));
  createRuntimeTooltips();
  applyRuntimeSkin();
  setCaption(*pw.fileName!=0);
}
#endif
