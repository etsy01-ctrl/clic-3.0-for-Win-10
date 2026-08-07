// ------------------------------------
// DACTOPT.H
// Dialeg d'opcions de les activitats
// ------------------------------------

#ifndef NOEDIT

#ifndef __DACTOPTH_H
#define __DACTOPTH_H

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

#ifndef __WCLH_H
#include "wcl.h"
#endif

class DActOpt : public ModalDialog
{ public:
	 DActOpt(HWND hOwner, ActFile *iactf);

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 void resetDlg();
	 void initDlg();
	 void resetHlpf();
	 void browseFn();
	 void browseCopy();
	 BOOL checkOk();
	 BOOL runDllConf(BOOL onlyTest);

  private:
	 BOOL dllAvail;
    BOOL bmpf;
	 ActFile af;
	 ActFile *destActf;

	 virtual LPCSTR getDialogName();
};

#endif

#endif