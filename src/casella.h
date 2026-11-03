// -----------------------------------
// CASELLA.H
// Prototipus de les classes:
//   - Casella
//   - Graella
// -----------------------------------

#ifndef __CASELLAH_H
#define __CASELLAH_H

#ifndef __ZONESH_H
#include "zones.h"
#endif

#ifndef __CLICH_H
#include "clic.h"
#endif

enum FLAGS_CASELLA {DL=0x2000};

class Casella : public Contenidor
{
  public:
	BOOL create(int ident, HWND hOwner, RECT *rpos,
			 BOOL iDl=TRUE, BOOL iHid=FALSE, BOOL iMark=FALSE, BASE_ZONES Base=GENERICA);

	virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0);

	Casella& operator=(Casella& c1);
	BOOL operator ==(const Casella& c1);
	BOOL operator >= (const Casella& c1);
	Casella& operator >>(Casella& c1);

	int  getId() {return id;}
	void setId(int newid) {id=newid;}

	BOOL getDl() {return getFlag(DL);}
	void setDl(BOOL newDl) {setFlag(DL, newDl);}

  protected:
	int id;
};


class Graella : public Zona
{
  public:
	 Graella();
	 ~Graella() {destroy();}

	 BOOL create(int numCasW, int numCasH, HWND hOwner, RECT *rpos,
		BOOL iDl=TRUE, BOOL iHid=FALSE, BOOL iMark=FALSE, BASE_ZONES Base=GENERICA);

	 void destroy();

	 virtual void update(HDC hDC, RECT *updRect=0, HDC hMemDC=0);
	 void move(int dx, int dy);
	 void move(RECT *rpos) {move(rpos->left-pos.left, rpos->top-pos.top);}
	 BOOL locateCursorHere(int &rp, int skipHides);

	 BOOL getData(HBITMAP hBitmap, RECT *iRect=0,
		 int firstCas=0, int firstId=0, int gdCw=0, int gdCh=0);
	 BOOL getData(char **txt, int firstCas=0, int firstId=0, int gdNc=0);
	 BOOL resetData();
	 void getAltData(HBITMAP hBitmap) {hBmpAlt=hBitmap;}
	 void getAltData(char **txt) {txtAlt=txt;}

	 void hide(BOOL mode);
	 void marked(BOOL mode);
	 void altCont(BOOL mode);
	 BOOL isHide(int &encerts=stp);
	 BOOL isNotHide(int &encerts=stp);
	 BOOL isMarked(int &encerts=stp);
	 BOOL checkIds(int &encerts=stp);
	 BOOL hideSelective(char c);
	 BOOL setAltData(Casella *c);

	 Casella *barreja(int vegades=DEF_CICLES);
	 Casella *barrejaVeiBuit(int vegades=DEF_CICLES);
	 Casella *veiBuit(int idpos);
	 Casella *localitza(POINT p, int &idpos=stp);
	 Casella *localitza(POINT p, int &x, int &y);
	 Casella *localitza(int id, int &idpos=stp);
	 Casella *getCas(int idpos);
	 Casella *getCas(int x, int y);
	 Casella *getRndCas();
	 Casella *despl(int vkCode);
	 Casella *situa(POINT p);
	 Casella *getCasX(int &x=stp, int &y=stp) {x=casXx; y=casXy; return casX;};
	 int getVirtualId(int dx, int dy) {return dx/casw+ncw*(dy/cash);}
	 BOOL setCasX(Casella *newCasX);

  protected:

	 int ncw, nch, ncas;
	 int casw, cash;
	 int casXx, casXy;
	 Casella *cas;
	 Casella *casX;
	 HBITMAP hBmp0, hBmpAlt;
	 char **txt0, **txtAlt;
	 RECT *iRect0;
	 int firstCas0, firstId0, gdCw0, gdCh0, gdNc0;
};

#endif

