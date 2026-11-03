#include "runtime-branding.h"
#include "rcomu/clicres.h"
#include <string.h>

void paintPlayerBranding(HDC dc, RECT area)
{
  static HBITMAP bitmap=NULL;
  static int width=0, height=0;
  if(!bitmap)
  {
    HINSTANCE app=GetModuleHandle(NULL);
    HRSRC resource=FindResource(app,MAKEINTRESOURCE(IDR_COVER_BRANDING),RT_RCDATA);
    if(!resource) return;
    DWORD size=SizeofResource(app,resource);
    HGLOBAL loaded=LoadResource(app,resource);
    const BYTE *data=(const BYTE*)LockResource(loaded);
    if(!data || size<8) return;
    width=*(const int*)data; height=*(const int*)(data+4);
    if(width<1 || height<1 || width>4096 || height>4096 || size<8UL+(DWORD)width*height*4) return;
    BITMAPINFO info; memset(&info,0,sizeof(info));
    info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth=width; info.bmiHeader.biHeight=-height;
    info.bmiHeader.biPlanes=1; info.bmiHeader.biBitCount=32;
    void *pixels=NULL;
    typedef HBITMAP (WINAPI *NewDIB)(HDC,const BITMAPINFO*,UINT,void**,HANDLE,DWORD);
    HINSTANCE gdi=GetModuleHandle("gdi32.dll");
    NewDIB create=(NewDIB)GetProcAddress(gdi,"CreateDIBSection");
    if(!create) return;
    bitmap=create(dc,&info,DIB_RGB_COLORS,&pixels,NULL,0);
    if(!bitmap || !pixels) return;
    memcpy(pixels,data+8,(long)width*height*4);
  }
  struct Blend {BYTE operation, flags, opacity, alphaFormat;};
  typedef BOOL (WINAPI *BlendImage)(HDC,int,int,int,int,HDC,int,int,int,int,Blend);
  static HINSTANCE library=LoadLibrary("msimg32.dll");
  static BlendImage blend=library ? (BlendImage)GetProcAddress(library,"AlphaBlend") : NULL;
  if(!blend || !bitmap) return;
  int availableW=area.right-area.left-40, availableH=area.bottom-area.top-32;
  if(availableW<1 || availableH<1) return;
  int w=availableW<560 ? availableW:560;
  int h=(long)w*height/width;
  if(h>availableH) {h=availableH; w=(long)h*width/height;}
  int x=area.left+(area.right-area.left-w)/2;
  int y=area.top+(area.bottom-area.top-h)/2;
  HDC mem=CreateCompatibleDC(dc); HGDIOBJ old=SelectObject(mem,bitmap);
  Blend options={0,0,255,1};
  blend(dc,x,y,w,h,mem,0,0,width,height,options);
  SelectObject(mem,old); DeleteDC(mem);
}
