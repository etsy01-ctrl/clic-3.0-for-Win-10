// -----------------------------------------------------
// DEDITACT.H
// Prototipus de la classe DEditAct, DAssign i DEdPaquet
// -----------------------------------------------------

#ifndef __DEDITACTH_H
#define __DEDITACTH_H

#ifndef NOEDIT

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

#ifndef __MISCFUNCH_H
#include "miscfunc.h"
#endif

#ifndef __CTEXTH_H
#include "ctext.h"
#endif

#ifndef __WCLH_H
#include "wcl.h"
#endif

class DEditAct : public ModalDialog
{ public:
	 DEditAct(HWND hOwner, ActFile *actf);
	 ~DEditAct();
	 static BOOL registerTxClass();

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();
	 void end();

  private:
	 HWND hEditW[3];
	 int  detectExtern;
	 ActFile af;
	 ActFile *destActf;
	 BOOL filesBox[3];

	 void resetDlg();
	 void resetFileBox(int c);
	 void initDlg();
	 void initGra();
	 BOOL getGra();
	 BOOL initTagsBox(int c);
	 BOOL getTagsBox(int c);
	 BOOL endEditCont(int c);
	 BOOL getFileName(int c);
	 BOOL editCont(int c);
	 BOOL checkOk();
	 BOOL setFuncHandles(BOOL connect);
	 BOOL processCommand(WPARAM wParam, LPARAM lParam);
	 BOOL enableSopIt();
	 int caretX0, caretY0, caretW0, caretH0;
	 void changeAssMode();
	 virtual LPCSTR getDialogName();

	 static WNDPROC lpfnEditGra;
	 static WNDPROC lpfnOldEditGra;

	 static LRESULT CALLBACK _export HandleEditGra(HWND, UINT, WPARAM, LPARAM);
	 static LRESULT CALLBACK _export HandleEditText(HWND, UINT, WPARAM, LPARAM);

	 static int lEditLin;
	 static TxContenidor *txc;
	 static ActFile *afp;
};

class DEditAssign : public ModalDialog
{ public:
	 DEditAssign(HWND hOwner, ActFile *actf);
    ~DEditAssign();

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
    virtual int getHelpItem();

  private:
    virtual LPCSTR getDialogName();
    BOOL initDlg();
    BOOL resetDestBox();
    BOOL resetAss(BOOL canvia);
    BOOL processCommand(WPARAM wParam, LPARAM lParam);
	 BOOL fillList(int l);
    BOOL drawBox(int i, DRAWITEMSTRUCT &dris);

    ActFile af;
    ActFile *destActf;
    int nco, ncd, io, id;
	 char **txtC[2];
    HBITMAP hBm[2], hBmX[2];
    RECT    rBm[2];
    int bmlx[2], bmly[2], nlx[2], nly[2], lcx[2], lcy[2];
    BOOL bmAdapted[2];
};

class DEdPaquet : public ModalDialog
{ public:
	 DEdPaquet(HWND hOwner, Paquet *iPac);

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();

	 BOOL checkOk();
	 BOOL initDlg();
	 BOOL resetDlg();
	 BOOL addItem();
	 BOOL deleteItem();

	 Paquet pac;
	 Paquet *destPac;
};

class DChain : public ModalDialog
{ public:
	 DChain(HWND hOwner, Paquet *iPac);

  protected:
	 virtual BOOL dispatch(UINT, WPARAM, LPARAM);
	 virtual int getHelpItem();

  private:
	 virtual LPCSTR getDialogName();
	 BOOL checkOk();
	 BOOL initDlg();
	 BOOL resetDlg();

	 Paquet pac;
	 Paquet *destPac;
};


void searchIter(char *pathToExplore, HWND hWnd, int nItems, UINT *listItems,
					 int numExts, Exts *validFiles, BOOL isList);

#endif

#endif
