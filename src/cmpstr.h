// ---------------------------------
// CMPSTR.H
// Rutines de comparacio de cadenes
// ---------------------------------


#ifndef __CMPSTRH_H
#define __CMPSTRH_H

#ifndef __WINDOWS_H
 #include "w.h"
#endif

#ifndef __ACTFILEH_H
#include "actfile.h"
#endif

BOOL cmpString(char *txt1, char *txt2, BOOL maj=FALSE, BOOL acc=TRUE);

BOOL checkSubString(int &coinci, char far *src, int ls, char far *ok, int lok,
				 ActFile *af, char marcar, char far *atr, BOOL itera=FALSE);

BOOL checkString(char far *src, char far *ok,
                     ActFile *af, char marcar=0, char far *atr=NULL);

char normChar(char ch, ActFile *af);


#endif
