// ---------------------------------
// CMPSTR.CPP
// Rutines de comparacio de cadenes
// ---------------------------------



#include "cmpstr.h"
#include "miscfunc.h"
#include "wcl.h"
#include <string.h>


#define FIRSTANSIACC 169
char ansiAcc[]={99, 97, 171, 172, 173, 114, 175, 111, 177, 50, 51, 145,
181, 182, 46, 184, 49, 111, 187, 188, 189, 190, 191, 65, 65, 65, 65, 65,
65, 198, 199, 69, 69, 69, 69, 73, 73, 73, 73, 68, 209, 79, 79, 79, 79, 79,
120, 79, 85, 85, 85, 85, 89, 222, 223, 97, 97, 97, 97, 97, 97, 230, 231,
101, 101, 101, 101, 105, 105, 105, 105, 240, 241, 111, 111, 111, 111, 111,
247, 111, 117, 117, 117, 117, 121, 254, 121};

#define FIRSTANSIUPP 97
char ansiUpp[]={65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80,
81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125, 126, 127, 128, 129, 130,
131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146,
147, 148, 149, 150, 151, 152, 153, 138, 155, 140, 157, 158, 159, 160, 161, 162,
163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178,
179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 192, 193, 194,
195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210,
211, 212, 213, 214, 247, 216, 217, 218, 219, 220, 221, 222, 159};

BOOL cmpString(char *txt1, char *txt2, BOOL maj, BOOL acc)
{
  int i, j, l;
  char *txt, *txb, *str[2], ch;
  int r=0;

  if(txt1==NULL || txt2==NULL || *txt1==0 || *txt2==0) return FALSE;

  for(i=0; i<2; i++)
  { txt= (i ? txt2:txt1);
	 for(;*txt!=0; txt++) if(*txt!=' ') break;
	 str[i]=dupStr(txt);
	 if(*str[i])
		for(j=strlen(str[i])-1; j>=0; j--)
		  if(str[i][j]==' ') str[i][j]=0;
		  else break;
  }


  if((l=strlen(str[0]))>0)
  for(txt=strtok(str[1], "|"); txt!=NULL; txt=strtok(NULL, "|"))
  {
	 if(strlen(txt)!=l) continue;

	 for(i=0; i<2; i++)
	 { txb= i ? str[0]:txt;
		if(!maj) AnsiUpper(txb);
		for(j=0; j<l; j++)
		{ ch=txb[j];
		  switch(ch)
		  { case '×':
			 case '*':
					txb[j]='X';
					break;

			 default:
					if(acc==FALSE && ch>=FIRSTANSIACC)
					  txb[j]=ansiAcc[ch-FIRSTANSIACC];
					break;
		  }
		}
	 }

	 if(strcmp(str[0], txt)==0) {r=1; break;}

  }

  delete str[0];
  delete str[1];
  return r==1;
}


BOOL checkSubString(int &coinci, char far *src, int ls, char far *ok, int lok,
				 ActFile *af, char marcar, char far *atr, BOOL itera)
{
  int is, iok, lastIs;
  BOOL lastiok=TRUE;
  BOOL result=TRUE;
  char chs, chok;
  BOOL marcaLletra;

  coinci=0;
  if(ls==0 || lok==0 || src==0 || ok==0) return FALSE;

  marcaLletra=af->avLletra;
  chs=chok=itera ? 0:' ';
  lastIs=0;
  for(iok=0, is=0; is<ls; is++, iok++)
  {
	 while(is<ls &&
			 (src[is]==0x0A ||
			  (!af->avDblSpc && chs==' ' && src[is]==' ') ||
			  (!af->avPunt && _fstrchr(".,;:", src[is])!=NULL)
			 )
			) is++;
	 if(is<ls) chs=normChar(src[is], af); else break;
	 lastIs=is;

	 if(iok>=0)
		while(iok<lok &&
				(ok[iok]==0x0A ||
				 (!af->avDblSpc && chok==' ' && ok[iok]==' ') ||
				 (!af->avPunt && _fstrchr(".,;:", ok[iok])!=NULL)
				)
			  ) iok++;
	 if(iok>=0 && iok<lok) chok=normChar(ok[iok], af); else chok=0;

	 if(chs==chok)
	 {	coinci++;
		if(marcar && marcaLletra) atr[is]&=(~marcar);
		lastiok=TRUE;
	 }
	 else
	 { result=FALSE;
		if(marcar && marcaLletra) atr[is]|=marcar;
		if(!itera && marcaLletra && lastiok && chok!=0 && af->avScope>0 && af->avMaxScope>0)
		{ int *itcoinc, lbloc;
		  int i, j, is2, iok2, ls2, lok2, jmax;
		  lbloc=2*af->avScope+1;
		  itcoinc=new int[lbloc];

		  for(j=0; j<lbloc; j++)
		  { itcoinc[j]=0;
			 i=iok+((j+1)/2)*(j&1 ? 1 : -1);
			 if(i>=lok) continue;
			 is2=(i<0 ? is-i:is);
			 if(is2>=ls) continue;
			 ls2=(ls2=ls-is2)>af->avMaxScope ? af->avMaxScope : ls2;
			 iok2=(i<0 ? 0:i);
			 lok2=(lok2=lok-iok2)>af->avMaxScope ? af->avMaxScope : lok2;
			 if(checkSubString(itcoinc[j], src+is2, ls2, ok+iok2, lok2, af, 0, NULL, TRUE)) break;
		  }
		  if(j==lbloc)
		  { jmax=itcoinc[af->avScope];
			 for(j=0; j<lbloc; j++)
				if(itcoinc[j]>itcoinc[jmax]) jmax=j;
			 i=iok+((jmax+1)/2)*(jmax&1 ? 1 : -1);
		  }
		  else if(itcoinc[j]>0) coinci++;

		  iok=i;
		  lastiok=FALSE;
		  delete itcoinc;
		}
	 }
  }

  if(iok>=0)
	 while(iok<lok &&
			 (ok[iok]==0x0A ||
			  (!af->avDblSpc && chok==' ' && ok[iok]==' ') ||
			  (!af->avPunt && _fstrchr(".,;:", ok[iok])!=NULL)
			 )
			) iok++;

  if(iok!=lok)
  { result=FALSE;
	 if(marcar && marcaLletra) atr[lastIs]|=marcar;
  }

  if(marcar && !marcaLletra)
	 for(is=0; is<ls; is++)
		if(result) atr[is]&=(~marcar); else atr[is]|=marcar;

  return result;
}


char normChar(char ch, ActFile *af)
{
  if(af->avAcc==FALSE && ch>=FIRSTANSIACC)
  ch=ansiAcc[ch-FIRSTANSIACC];

  if(af->avMaj==FALSE && ch>=FIRSTANSIUPP)
  ch=ansiUpp[ch-FIRSTANSIUPP];

  return ch;
}


BOOL checkString(char far *src, char far *ok, ActFile *af, char marcar, char far *atr)
{ BOOL result;
  int i, t, p0, p1, nTags;
  int *coinci;
  int ls, lok;

  if(src==NULL || ok==NULL) return FALSE;
  ls=_fstrlen(src);
  lok=_fstrlen(ok);

  result=FALSE;
  if(marcar)
	 for(i=0; i<ls; i++)
		atr[i]&=(~marcar);

  for(nTags=1, i=0; i<lok; i++) if(ok[i]=='|') nTags++;
  if((coinci=new int[3*nTags])==NULL) {NOMEM; return FALSE;}
  for(p0=0, t=0; t<nTags; t++)
  { for(p1=0;(p0+p1)<lok; p1++) if(ok[p0+p1]=='|') break;
	 coinci[3*t+1]=p0;
	 coinci[3*t+2]=p1;
	 if(checkSubString(coinci[3*t], src, ls, ok+p0, p1, af, 0, NULL)) break;
	 p0+=p1+1;
  }
  if(t==nTags)
  { if(marcar)
	 { for(i=1, t=0; i<nTags; i++) if(coinci[3*i]>coinci[3*t]) t=i;
		checkSubString(coinci[3*t], src, ls, ok+coinci[3*t+1], coinci[3*t+2], af, marcar, atr);
	 }
  }
  else
	 result=TRUE;

  delete coinci;

  return result;
}
