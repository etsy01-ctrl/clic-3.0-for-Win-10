// -----------------------------------
// REPORT.H
// Prototipus de les classes:
//  - ELREP
//  - Report
// -----------------------------------

#ifndef __REPORT_H
#define __REPORT_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif


struct ELREP
{
  char actName[14];
  int  time;
  int  moves;
  int  placed;
  int  movToSolv;
  int  prec;
  BOOL solved;
  BOOL onlyTime;
};

class Report
{
 public:
  Report();
  ~Report();

  int getCur() {return cur;}

  void clear();
  BOOL addReport(char *fName,
		 int time, int moves, int placed, int movToSolv,
		 BOOL solved, BOOL iOnlyTime=FALSE);

  void rewind() {cur=0;};
  char *getFRep(int rn=-1);
  char *getGPrec();
  BOOL getDataFrom(int from, int &prec, int &time);
  int getPrec(int r);

 protected:
  HGLOBAL hg;
  int cur;
  int maxReps;
  int  nlocks;
  ELREP huge *lockPointer();
  void unlockPointer();

  char fText[80];
};


#endif
