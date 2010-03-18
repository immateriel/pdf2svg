// Copyright 2010 immatériel.fr

#ifndef SVGOUTPUTDEV_H
#define SVGOUTPUTDEV_H

#ifdef __GNUC__
#pragma interface
#endif

#include <stdio.h>
#include "goo/gtypes.h"
#include "goo/GooVector.h"
#include "goo/GooString.h"
#include "GfxState.h"
#include "Page.h"
#include "OutputDev.h"

//#include "fofi/FoFiTrueType.h"
//#include "fofi/FoFiType1C.h"

#define xoutRound(x) ((int)(x + 0.5))


class GfxState;

class SVGFont{
private:
	GooString *name;
	
	double ascent;
	double descent;
	GooString *style;
	GooString *weight;
	GooString *stretch;
	
public:
	SVGFont(GooString *name, double a,double d,GooString *s, GooString *w, GooString *st);
	SVGFont();
	
	GooString *dump();

	~SVGFont();
	GBool isEqual(const SVGFont& x) const;
};


class SVGFontAccu{
private:
  GooVector<SVGFont> *accu;
  
public:
  SVGFontAccu();
  ~SVGFontAccu();
  GBool isFont(const SVGFont& font);
  int AddFont(const SVGFont& font);
  SVGFont* Get(int i){
    GooVector<SVGFont>::iterator g=accu->begin();
    g+=i;  
    return g;
  } 
	GooString *dump();

  double size() const {return accu->size();}
  
};

class SVGNode{
public:
	SVGNode() {};
	virtual ~SVGNode() {};
	virtual GooString *dump()=0;
};

class SVGStartNode : public SVGNode{
private:
	GooString *tagName;
public:
	SVGStartNode(GooString *t);
	SVGStartNode();
	~SVGStartNode();
	GooString *dump();
};

class SVGEndNode : public SVGNode{
private:
	GooString *tagName;
public:
	SVGEndNode(GooString *t);
	SVGEndNode();
	~SVGEndNode();
	GooString *dump();
};


class SVGImage : public SVGNode{
private:
	double x,y,width,height;
	GooString *href;
public:
	SVGImage(double cx, double cy, double w, double h, GooString *hr);
	SVGImage();
	~SVGImage();
	GooString *dump();
};

class SVGText : public SVGNode{
private:
	GooString *str;
	double fontSize;
	GooString *fontFamily;
	double wordSpace;
	double charSpace;
	double x,y;
	double textW;
	int rotation;
	int r,g,b;
public:
	double xMin,yMin,xMax,yMax;
	double spaceSize;

	SVGText(double cx, double cy, double fsize, GooString *ffamily, double wspace, double cspace, int rot, int ir, int ig, int ib );
	SVGText();
	~SVGText();
	
	void setTextWidth(double tWidth);
	void addTextWidth(double tWidth);

	void addStr(GooString *s);
	GooString *getStr() {return str;};
	
	GooString *dump();
	
};

class SVGLine : public SVGNode{
private:
	int strokeR,strokeG,strokeB;
	double strokeWidth;
	double fillOpacity;
	double strokeOpacity;
	double x1,y1,x2,y2;

public:
	SVGLine(double ix1, double iy1, double ix2, double iy2, double stWidth,double stOp, int stR,int stG,int stB);
	SVGLine();
	~SVGLine();
	GooString *dump();

};

class SVGRect : public SVGNode{
private:
	int fillR,fillG,fillB;
	int strokeR,strokeG,strokeB;
	GooString *fillRule;
	double x,y,width,height;
	double strokeWidth;
	double fillOpacity;
	double strokeOpacity;

public:
	SVGRect(double ix, double iy,double iw, double ih,double stWidth,double stOp, int stR,int stG,int stB, double fiOp, GooString *fiRu, int fiR,int fiG,int fiB);
	SVGRect();
	~SVGRect();
	GooString *dump();

};

class SVGPath : public SVGNode{
private:
	GooString *path;
	int fillR,fillG,fillB;
	int strokeR,strokeG,strokeB;
	GooString *fillRule;
	
	double strokeWidth;
	double matrix[6];
	double fillOpacity;
	double strokeOpacity;

public:
	SVGPath(double stWidth,double stOp, int stR,int stG,int stB, double fiOp, GooString *fiRu, int fiR,int fiG,int fiB, GooString *p, double *mat);
	SVGPath();
	~SVGPath();
	GooString *dump();

};

class SVGClipPath : public SVGNode{
	SVGPath *path;
public:
	SVGClipPath(double stWidth,double stOp, int stR,int stG,int stB, double fiOp, GooString *fiRu, int fiR,int fiG,int fiB, GooString *p, double *mat);
	SVGClipPath();
	~SVGClipPath();
	GooString *dump();
};

class SVGOutputDev : public OutputDev {
public:
  // Constructor.                                                                                                                                                                                          
  SVGOutputDev(XRef *xrefA);
  virtual ~SVGOutputDev();

 // Check if file was successfully created.
  virtual GBool isOk() { return ok; }

 //---- get info about output device

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown() { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar() { return gTrue; }

/*
  virtual void drawImageMask(GfxState *state, Object *ref, 
			     Stream *str,
			     int width, int height, GBool invert,
			     GBool inlineImg); */
				virtual void drawImageMask(GfxState *state, Object *ref,
				                             Stream *str,
				                             int width, int height, GBool invert,
				                             GBool interpolate, GBool inlineImg);
/*  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
			  int width, int height, GfxImageColorMap *colorMap,
			 int *maskColors, GBool inlineImg);
	*/
	  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
	                         int width, int height, GfxImageColorMap *colorMap,
	                         GBool interpolate, int *maskColors, GBool inlineImg);

			
/*			
  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
			       int width, int height,
			       GfxImageColorMap *colorMap,
			       Stream *maskStr, int maskWidth, int maskHeight,
			       GBool maskInvert);
  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
				   int width, int height,
				   GfxImageColorMap *colorMap,
				   Stream *maskStr,
				   int maskWidth, int maskHeight,
				   GfxImageColorMap *maskColorMap);
*/

	virtual void startPage( int pageNum, GfxState *state );
  virtual void endPage();
  virtual void drawString( GfxState * state, GooString * s );
  virtual void drawChar(GfxState *state, double x, double y,
			double dx, double dy,
			double originX, double originY,
			CharCode code, int nBytes, Unicode *u, int uLen);

  virtual void stroke(GfxState *state);
  virtual void fill(GfxState *state);
  virtual void eofill(GfxState *state);


  virtual void endMarkedContent(GfxState *state);
  virtual void beginMarkedContent(char *name, Dict *properties);

// TODO
/*
	virtual void clip(GfxState *state);
  virtual void eoClip(GfxState *state);
  virtual void clipToStrokePath(GfxState *state);
*/

GooString *convertPath( double *matrix, GfxPath *path );
bool detectLine(GfxState *state);
bool detectRect(GfxState *state, int t);

void transform(double *matrix,
                                   double xi, double yi,
																	double *xo, double *yo);
 void dumpContent();

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars() { return gFalse; }

  // Does this device need non-text content?
  virtual GBool needNonText() { return gTrue; }

  //----- initialization and control

  virtual GBool checkPageSlice(Page *page, double hDPI, double vDPI,
                               int rotate, GBool useMediaBox, GBool crop,
                               int sliceX, int sliceY, int sliceW, int sliceH,
                               GBool printing, Catalog * catalogA,
                               GBool (* abortCheckCbk)(void *data) = NULL,
                               void * abortCheckCbkData = NULL)
  {
	cropBox=page->getCropBox();
	mediaBox=page->getMediaBox();
   
   return gTrue;
  }

	int AddNode(SVGNode* node);

  void generateFont(GfxState *state);
		double getSpaceSize(GfxFont *font);
private:
	void closeText();
	
	 GBool ok;			// set up ok?
	XRef *xref;
	GfxFont *curFont;
	SVGFontAccu *fonts;
	GooString *svg;
	
	GooVector<SVGText> *texts;
	GooVector<SVGPath> *paths;
	SVGNode *nodes[10000];
  

	double curPageWidth;
	double curPageHeight;
	double lineY;
	double lastX;
	double startX;
	GBool startedLine;
	double fontSize;
	GooString *fontFamily;
	double wordSpace;
	double charSpace;
	double textWidth;
	double textWidthTmp;

	double textWidthDiff;

	int curNode;

  int pageNum;
  static int imgNum;
  GooString *Docname;

	SVGText *curTxt;

	GooString *curText;
	PDFRectangle *cropBox;
	PDFRectangle *mediaBox;

};

#endif