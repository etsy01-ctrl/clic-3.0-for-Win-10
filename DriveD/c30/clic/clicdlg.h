// ------------------------------------
// CLICDLG.H
// Prototipus de les classes de dialegs
// ------------------------------------

#ifndef __CLICDLGH_H
#define __CLICDLGH_H

#ifndef __ZONESH_H
#include "zones.h"
#endif

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

#ifndef __REPORTH_H
#include "report.h"
#endif

#ifndef __CTEXTH_H
#include "ctext.h"
#endif

#ifndef __WCLH_H
#include "wcl.h"
#endif

#ifndef RUNTIME
class DInfo : public ModalDialog
{ public:
	 DInfo(HWND hOwner):ModalDialog(hOwner) {};

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();
};

class DReport : public ModalDialog
{ public:
	 DReport(HWND hOwner, Report *ir);

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();
	 void initDlg();

	 Report *r;
};
#endif

class DAjuda : public ModalDialog
{ public:
	 DAjuda(HWND hOwner, int tipusAct, int modeAct,
			  RECT *ibmrect, HBITMAP ihBm, char **itxtp=0, BOOL ilist=FALSE);
	 ~DAjuda();

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();
	 void initDlg();

	 Contenidor cont;
	 ModalControl llista, txwin;
	 RECT wRect, cRect, bmRect;
	 HBITMAP hBm;
	 char *text;
	 char **txtp;
	 BOOL list;
	 int  maxlen, tipus, mode;
};

#ifndef RUNTIME
class DFitx : public ModalDialog
{ public:
	 DFitx(HWND hOwner,
			 char *caption, char *iniPath, char *iniName, char *iniDesc,
	  int extensio, BOOL mustExist);
	 char *getFileName() {return path;}
	 char *getDesc() {return desc;}
	 BOOL exist() {return isHere;}
	 virtual UINT run();

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();

	 char *capt;
	 char *iPath;
	 char *iName;
	 int  ext;
	 BOOL me, isHere;

	 char path[MAX_FULL_FILENAME+1];
	 char desc[MAX_DESC+3];

	 BOOL initDlg();
	 BOOL resetDlg();
	 BOOL processList(WPARAM wParam, LPARAM lParam);
	 BOOL checkOk();
	 BOOL updateFName(char *fName);
};


class DConfirmOps : public ModalDialog
{ public:
	 DConfirmOps(HWND hOwner);
	 BOOL noMes;

  protected:
	 virtual LPCSTR getDialogName();
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
};


class DWorkDir : public ModalDialog
{ public:
	 DWorkDir(HWND hOwner, char *iniPath, BOOL iNoMes, BOOL iHideNoMes=FALSE);
	 virtual UINT run();
	 BOOL noMes;
	 char path[MAX_FULL_FILENAME+1];

  protected:
	 virtual LPCSTR getDialogName();
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 BOOL resetDlg();
	 BOOL hideNoMes;
};
#endif

class DPuzzle : public ModalDialog
{ public:
	 DPuzzle(HWND hOwner, ActFile *actf);

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 ActFile *af;
	 int pmod, ncw, nch, stretch;
	 void resetDlg();
	 void initDlg();
	 virtual LPCSTR getDialogName();
};

#ifndef RUNTIME
class DConfigura : public ModalDialog
{ public:
	 DConfigura(HWND hOwner);
	 BOOL dbChanged;

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 void resetDlg();
	 void browseFn(int field, int ext, int msg);

  private:
	 int p;
	 BOOL q, a, s, rep, ac, acd, gfil, cg, hoh, mn, ed, nu, ng, iu;
	 char workChangedPath[100];

	 virtual LPCSTR getDialogName();
};

class DFileName : public ModalDialog
{ public:
	 DFileName(HWND hOwner, BOOL isText0);
	 BOOL isText;
	 char txBuffer[15];

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 static char bmpHeader[];

  private:
	 virtual LPCSTR getDialogName();

};

class DZones : public ModalDialog
{ public:
	 DZones(HWND hOwner, int w0, int h0, int b0, BOOL la, char *txt0);
	 int w;
	 int h;
	 int b;
	 BOOL la;
	 char *txt;

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();

};

class DIncognita : public ModalDialog
{ public:
	 DIncognita(HWND hOwner, struct Incognita *iniInc);
	 struct Incognita inc;
	 struct Incognita *inc0;

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 BOOL resetDlg();

  private:
	 virtual LPCSTR getDialogName();
};

class DEvalOp : public ModalDialog
{ public:
	 DEvalOp(HWND hOwner, ActFile *af0);

	 BOOL okToNext;
	 BOOL btCorregir;
	 BOOL avCont;
	 BOOL avMaj;
	 BOOL avAcc;
	 BOOL avDblSpc;
	 BOOL avPunt;
	 BOOL avLletra;
	 BOOL avPrevHelp;
	 BOOL avNoSalta;
	 int avTimePH;
	 int avScope;
	 int avMaxScope;

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 ActFile *af;
	 BOOL resetDlg();

  private:
	 virtual LPCSTR getDialogName();

};

class DAutoInc : public ModalDialog
{ public:
	 DAutoInc(HWND hOwner);
	 ~DAutoInc();

	 BOOL expr;
	 BOOL onlyWords;
	 char *txInc;

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 BOOL resetDlg();

  private:
	 virtual LPCSTR getDialogName();
};
#endif

#endif