// ------------------------------------------
// ACTFILE.H
// Prototipus de les classes ActFile i Paquet
// ------------------------------------------

#ifndef __ACTFILEH_H
#define __ACTFILEH_H

#ifndef __CLICH_H
#include "clic.h"
#endif

#ifndef __REPORTH_H
#include "report.h"
#endif

#ifndef __AUTOCLICH_H
#include "autoclic.h"
#endif

#ifndef __ZONESH_H
#include "zones.h"
#endif

class ActFile
{
  public:
	 ActFile();
	 ~ActFile();

	 ActFile& operator = (const ActFile& af);

	 void clear();
	 BOOL load(char *name);
	 BOOL newAct(int tipus);
	 BOOL save(BOOL promptForName);
	 char *getFnCont(int i);
	 BOOL prepareActData();
	 BOOL callRMKDll();
	 BOOL ajAllowed();
	 void copyStyle(const ActFile& af);
	 BOOL noEvaluable();

	 static BOOL createActdFiles();
	 static void clearActdFiles();

	 char fileName[13];
	 char fileNameCont[3][13];
	 int  actMode, puzMode, graPos;
	 int  ncw, nch;
	 int  nctxw, nctxh;
	 int  txtCW, txtCH, txtCW2, txtCH2;
	 int  ntags[2], cont[3];
	 BOOL bar[2], delim[2], stretch;
	 char initMess[MAX_MESS_LEN+1];
	 char endMess[MAX_MESS_LEN+1];
	 char fileDesc[MAX_DESC+3];
	 char custHelpFile[MAX_FULL_FILENAME+1];
	 char hlpTopic[MAX_FULL_FILENAME+1];
	 char **tags[2], **graTxt;
	 int  nLines[3];
	 int  ass[MAXCW*MAXCH];
	 BOOL loaded, modified, mAss, invAss, shHelp, shPuz, sol;
	 BOOL okToNext, btCorregir;
	 BOOL avCont, avMaj, avAcc, avDblSpc, avPunt, avLletra, brPar, avNoSalta;
	 BOOL noBV, shDisk, shPrint, shPorta, barraAmunt, noAv;
	 BOOL reported, solved, custHlp, tileBmp, pwTransp, pwrp;
	 int pwrx, pwry;
	 int avPrevHelp, avTimePH, tabSpc;
	 int avScope, avMaxScope;
	 int  movToSolv, iniSolved;
	 LOGFONT logF[4];
	 Colors colors[4];
	 COLORREF colorFons[2];
	 COLORREF colorUsuari[2];
	 COLORREF colorError[2];
	 int tramaFons[2];
	 BOOL useDLL;
	 char rgDLL[13];
	 char dllOptions[DLL_OPTIONS_SIZE];
	 BOOL comptadors[3];
	 BOOL marcs;
	 int  btTipus;
	 char *txBase;
	 char *txBtCorregir;
	 char *txPrev;
	 char *bmpFons;
	 char *initMessPrev;

	 static ACTDATA actd;
};

class Paquet
{
  public:
	 Paquet();
	 ~Paquet();

	 void setBaseRep(Report *r) {baseRep=r;}
	 Paquet& operator = (const Paquet& pq);
	 void clear();
	 BOOL load(char *name);
	 BOOL save(BOOL promptForName);
	 BOOL isEmpty() {return acts==NULL;}
	 BOOL firstAct();
	 BOOL lastAct();
	 char *getAct();
	 void nextAct();
	 void prevAct();

	 char fileName[13];
	 char fileDesc[MAX_DESC+3];

	 char **acts;
	 int  nActs;
	 int  act;
	 BOOL loaded, modified;
	 BOOL autoPass, noPassButtons, noDiskButton, noExitButton;
    BOOL botoSep, av800, av256;
	 int  lapToPass;
	 BOOL def, sup, inf, supTFlag, infTFlag;
	 char *chDef, *chSup, *chInf;
	 int  infP, infT, supP, supT;

  protected:
	 static Report *baseRep;
	 static int lastElRep;
};

#endif