// -----------------------
// FLOATCNT.H
// Declaracio de FloatCnt
// -----------------------

#ifndef __FLOATCNTH_H
#define __FLOATCNTH_H

#ifndef __WCLH_H
#include "wcl.h"
#endif

#ifndef __ZONESH_H
#include "zones.h"
#endif

class FloatCnt : public Window
{
  public:
	 FloatCnt();

	 BOOL getData(char *txt, RECT pos, int fl, int tempsAp=0, int tempsMax=0);
	 BOOL isVisible() {return visible;}
	 BOOL show(BOOL status, BOOL immediate=FALSE);

  protected:
	 LRESULT dispatch(UINT, WPARAM, LPARAM, int);
	 BOOL registerClass();
	 BOOL createWindow();

  private:
	 RECT wRect, clRect;
	 Contenidor c;
	 static BOOL isRegistered;
	 BOOL visible;
	 UINT idTimer;
	 int countDown;
	 int tMax;
	 int tAp;
	 int flags;
	 BOOL actiu;
};

#endif
