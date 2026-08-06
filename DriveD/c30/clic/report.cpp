// -----------------------------------
// REPORT.CPP
// Implementació de les classes:
//  - ELREP
//  - Report
// -----------------------------------

#include "report.h"
#include <string.h>
#include <stdio.h>

#define REPBLOCKS 30

// --------- Report --------------

Report::Report()
{
  cur=0;
  maxReps=REPBLOCKS;
  hg=GlobalAlloc(GMEM_MOVEABLE, maxReps * sizeof(ELREP));
  nlocks=0;
}

Report::~Report()
{ while(nlocks) unlockPointer();
  GlobalFree(hg);
}


ELREP huge *Report::lockPointer()
{
  nlocks++;
  return (ELREP huge *)GlobalLock(hg);
}

void Report::unlockPointer()
{
  nlocks--;
  GlobalUnlock(hg);
}

BOOL Report::getDataFrom(int from, int &prec, int &time)
{
  long nr, ac;
  ELREP huge *rp;
  int i;

  nr=ac=0L;
  prec=time=0;

  if(from>=cur) return FALSE;

  rp=lockPointer()+from;

  for(i=from; i<cur; i++, rp++)
  { time+=rp->time;
	 if(!rp->onlyTime)
	 { ac+=rp->prec;
		nr++;
	 }
  }

  if(nr)
  { ac=ac/nr;
	 prec=(int)ac;
  }

  unlockPointer();

  return ac>0;
}

void Report::clear()
{
  cur=0;
}

BOOL Report::addReport(char *fName, int time, int moves, int placed,
				 int movToSolv, BOOL solved, BOOL onlyTime)
{
  ELREP r;
  long m, p;

  if(cur>=maxReps)
  { maxReps+=REPBLOCKS;
	 GlobalReAlloc(hg, maxReps * sizeof(ELREP), GMEM_MOVEABLE);
  }

  _fstrncpy(r.actName, fName, 14);
  r.time=time;
  r.moves=moves;
  r.placed=placed;
  r.movToSolv=movToSolv;
  r.solved=solved;
  r.onlyTime=onlyTime;

  if(onlyTime)
	 r.prec=0;
  else
  { if(solved)
	 { p=movToSolv;
		m=moves>p ? moves:p;
	 }
	 else
	 { p=placed*placed;
		m=moves*movToSolv;
	 }

	 if(!p || !m)
		r.prec=0;
	 else r.prec=(int)((p*100)/m);
	 if(r.prec>100) r.prec=100;
  }

  *(lockPointer()+cur)=r;
  cur++;
  unlockPointer();

  return TRUE;
}


char *Report::getGPrec()
{
  long nr, ac, tTime, tSolv, rSolv;
  unsigned  h, m, s;
  int i;
  ELREP huge *rp;

  rp=lockPointer();

  nr=ac=tTime=tSolv=rSolv=0L;
  for(i=0; i<cur; i++, rp++)
  { tTime+=rp->time;
	 if(!rp->onlyTime)
	 { if(rp->solved) tSolv++;
		 ac+=rp->prec;
		 nr++;
	 }
  }

  if(nr)
  { ac=ac/nr;
	 rSolv=(tSolv*100)/nr;
  }

  h=unsigned(tTime/3600);
  m=unsigned((tTime-h*3600)/60);
  s=unsigned(tTime-h*3600-m*60);

  sprintf(fText, "%d%c%ld (%ld%%)%c%02d:%02d:%02d%c%ld%%",
			 cur, 0, tSolv, rSolv, 0, h, m, s, 0, ac);

  unlockPointer();

  return fText;
}

char *Report::getFRep(int rn)
{
  ELREP r;

  *fText=0;
  if(rn==-1) rn=cur-1;
  if(rn<0 || rn>=cur) return fText;

  r=*(lockPointer()+rn);
  if(r.onlyTime)
	 sprintf(fText, "%-12s   %3d   ---   ---    -   ---",
		 r.actName, r.time);
  else
	 sprintf(fText, "%-12s   %3d   %3d   %3d    %s   %3d%%",
		 r.actName, r.time, r.moves, r.placed,
		 r.solved ? "X":"-", r.prec);

  unlockPointer();

  return fText;
}

