// Copyright 2010 immatériel.fr

#ifdef __GNUC__
#pragma implementation
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>
#include "goo/gtypes.h"
#include "goo/GooVector.h"
#include "goo/GooString.h"
#include "GfxState.h"
#include "GfxFont.h"
#include "UnicodeMap.h"
#include "UTF8.h"
#include "OutputDev.h"

#include "SVGOutputDev.h"

int SVGOutputDev::imgNum=1;


double pixel(double pt)
{
	return pt;
//	double x=pt/0.72;
//	if(x>=0.5){return ceil(x);}else{return floor(x);}
//	return pt/0.72;
}

/* FONT */
SVGFont::SVGFont(GooString *nm, double a, double d, GooString *s, GooString *w)
{
	name=nm;
	ascent=a;
	descent=d;
	style=s;
	weight=w;

}
SVGFont::SVGFont()
{
	name=NULL;
}

GooString *SVGFont::dump()
{
	GooString *output=new GooString("");

//	output->appendf("<font>");
/*
	output->appendf("<font-face font-family=\"{0:s}\" ascent=\"{1:.2f}\" descent=\"{2:.2f}\" font-style=\"{3:s}\" font-weight=\"{4:s}\">\n",name,ascent,descent,style->getCString(),weight->getCString());
	output->appendf("<font-face-src>");
	output->appendf("<font-face-uri xlink:href=\"{0:s}.ttf\"/>",name->getCString());
	output->appendf("</font-face-src>\n");
	output->appendf("</font-face>\n");
*/

//	output->appendf("</font>\n");
/*
	output->append("@font-face { \n");
	output->appendf(" font-family:\"{0:s}\";\n",name->getCString());
	output->appendf(" font-weight:{0:s};\n",weight->getCString());
	output->appendf(" font-style:{0:s};\n",style->getCString());

	output->appendf(" src: url(\"{0:s}.ttf\");\n",name->getCString());

	output->appendf("}\n");
	*/

	return output;
	
}


GBool SVGFont::isEqual(const SVGFont& x) const{
	return (name->cmp(x.name)==0);
}


SVGFont::~SVGFont()
{
//  if(name) delete name;
//  name=NULL;
}

SVGFontAccu::SVGFontAccu()
{
  accu=new GooVector<SVGFont>();
}

SVGFontAccu::~SVGFontAccu()
{
  if (accu) delete accu;
}


GBool SVGFontAccu::isFont(const SVGFont& font){
	 GooVector<SVGFont>::iterator i; 
	 for (i=accu->begin();i!=accu->end();i++)
	 {
		if (font.isEqual(*i)) 
		{
	     return gTrue;
		}
	 }

	 return gFalse;
	
}

int SVGFontAccu::AddFont(const SVGFont& font){
 GooVector<SVGFont>::iterator i; 
 for (i=accu->begin();i!=accu->end();i++)
 {
	if (font.isEqual(*i)) 
	{
     return (int)(i-(accu->begin()));
	}
 }



 accu->push_back(font);
 return (accu->size()-1);
}

GooString *SVGFontAccu::dump(){
	GooString *output=new GooString("");
 GooVector<SVGFont>::iterator i; 
 for (i=accu->begin();i!=accu->end();i++)
 {
	output->append(i->dump());
 }



return output;
}

GooString *SVGNode::dump()
{
	return (new GooString("NEVER HAPPEN\n"));
}



/* TEXT */
SVGText::SVGText(double cx, double cy, double fsize, GooString *ffamily, double wspace, double cspace, int rot, int ir, int ig, int ib )
{
	str=new GooString("");
//	str->append(s->getCString());
	fontSize=fsize;
	fontFamily=ffamily;
	wordSpace=wspace;
	charSpace=cspace;
	spaceSize=0;
//	textWidth=tWidth;
	textW=0;
	x=cx;
	y=cy;
	rotation=rot;
	r=ir;
	g=ig;
	b=ib;
	
}

SVGText::SVGText()
{
	str=new GooString("");
}

SVGText::~SVGText()
{
	if(str) delete str;
	if(fontFamily) delete fontFamily;

}

void SVGText::setTextWidth(double tWidth) 
{
	textW=tWidth;
}

void SVGText::addTextWidth(double tWidth) 
{
	textW=textW + tWidth;
}

void SVGText::addStr(GooString *s)
{
//	printf("%s\n",s->getCString());
//	if(str)
//	{
		str->append(s);
//	}
}

GooString *SVGText::dump()
{
	GooString *output=new GooString("");
//	output->appendf("<!-- {0:.2f} {1:.2f} {2:.2f} {3:.2f} ({4:.2f} {5:.2f}) : {6:.2f} -->\n",xMin,yMin,xMax,yMax,(xMax-xMin),(yMax-yMin),(spaceSize*fontSize)*1.5);
	output->appendf("<text x=\"{0:.2f}\" y=\"{1:.2f}\"",pixel(x),pixel(y));
//	if(str->cmp(" ")!=0)
//	{
		output->appendf(" font-family=\"{0:s}\"",fontFamily);
//	}
		output->appendf(" textLength=\"{0:.2f}\"",textW);
		

	if(wordSpace !=0)
		output->appendf(" word-spacing=\"{0:.2f}\"",pixel(fontSize * wordSpace));
	if(charSpace !=0)
		output->appendf(" letter-spacing=\"{0:.2f}\"",pixel(fontSize * charSpace));

	if(rotation != 0)
	{
		output->appendf(" transform=\"rotate({0:d},{1:.2f},{2:.2f})\"",rotation*90,pixel(x),pixel(y));
	}


	if(r==0 && g==0 && b==0)
	{
		output->append(" fill=\"black\"");
	} 
	else
	{
			output->appendf(" fill=\"#{0:x}{1:x}{2:x}\"",r,g,b);
	}
//	output->appendf(" font-size=\"{0:.2f}\" stroke=\"none\">",pixel(fontSize));
	output->appendf(" font-size=\"{0:.2f}\" stroke=\"none\">",fontSize);

//		output->appendf("{0:s}",str);
		output->append(str);

		output->append("</text>\n");
	return output;
}

SVGImage::SVGImage()
{
	
}

SVGImage::SVGImage(double cx, double cy, double w, double h, GooString *hr)
{
	x=cx;
	y=cy;
	width=w;
	height=h;
	href=hr;
}

SVGImage::~SVGImage()
{
	
}

GooString *SVGImage::dump()
{
	GooString *output=new GooString("");
	output->appendf("<image x=\"{0:d}\" y=\"{1:d}\" width=\"{2:d}\" height=\"{3:d}\" xlink:href=\"{4:s}\"/>\n",(int)x,(int)y,(int)width,(int)height,href);
	
	return output;
}


// StartNode
SVGStartNode::SVGStartNode()
{
	
}

SVGStartNode::SVGStartNode(GooString *t)
{
	tagName=t;
}

SVGStartNode::~SVGStartNode()
{
	
}

GooString *SVGStartNode::dump()
{
	GooString *output=new GooString("");
	output->appendf("<{0:s}>\n",tagName);	
	return output;
}


// EndNode
SVGEndNode::SVGEndNode()
{
	
}

SVGEndNode::SVGEndNode(GooString *t)
{
	tagName=t;
}

SVGEndNode::~SVGEndNode()
{
	
}

GooString *SVGEndNode::dump()
{
	GooString *output=new GooString("");
	output->appendf("</{0:s}>\n",tagName);	
	return output;
}

// Line

SVGLine::SVGLine()
{
}

SVGLine::SVGLine(double ix1, double iy1, double ix2, double iy2,double stWidth,double stOp, int stR,int stG,int stB)
{
	x1=ix1;
	x2=ix2;
	y1=iy1;
	y2=iy2;
	strokeWidth=stWidth;
	strokeOpacity=stOp;
	strokeR=stR;
	strokeG=stG;
	strokeB=stB;
	
}

SVGLine::~SVGLine()
{
}


GooString *SVGLine::dump()
{
	GooString *output=new GooString("");
	output->appendf("<line x1=\"{0:.2f}\" y1=\"{1:.2f}\" x2=\"{2:.2f}\" y2=\"{3:.2f}\"",pixel(x1),pixel(y1),pixel(x2),pixel(y2));
	
	if(strokeR > -1 && strokeG > -1 and strokeB > -1) 
	{
		if(strokeR==0 && strokeG==0 && strokeB==0)
		{
			output->append(" stroke=\"black\"");
		} 
		else
		{
			output->appendf(" stroke=\"#{0:x}{1:x}{2:x}\"",strokeR,strokeG,strokeB);
		}
	}
	else
	{
		output->append(" stroke=\"none\"");
	}
	
	if (strokeWidth)
		output->appendf(" stroke-width=\"{0:.2f}\"",pixel(strokeWidth));

	if(strokeOpacity)
		output->appendf(" stroke-opacity=\"{0:.2f}\"",strokeOpacity);
	


	output->append("/>\n");

	
	return output;
}

// Path

SVGPath::SVGPath()
{
}


SVGPath::SVGPath(double stWidth,double stOp, int stR,int stG,int stB, double fiOp, GooString *fiRu, int fiR,int fiG,int fiB, GooString *p, double *mat)
{
	
	strokeWidth=stWidth;
	strokeOpacity=stOp;
	strokeR=stR;
	strokeG=stG;
	strokeB=stB;
	fillOpacity=fiOp;
	fillRule=fiRu;
	fillR=fiR;
	fillG=fiG;
	fillB=fiB;
	path=p;
 for (int i = 0; i<6; i++)
  {
		matrix[i]=mat[i];
	}
}

SVGPath::~SVGPath()
{
	if(fillRule) delete fillRule;
	if(path) delete path;
}

GooString *SVGPath::dump()
{
	GooString *output=new GooString("");
	
//	if(matrix[0]!=1.0 || matrix[1]!=0.0 || matrix[2]!=0.0 || matrix[3]!=-1.0)
//	{
//	output->appendf("<path transform=\"matrix({0:d} {1:d} {2:d} {3:d} {4:.2f} {5:.2f})\"",(int)matrix[0],(int)matrix[1],(int)matrix[2],(int)matrix[3],pixel(matrix[4]),pixel(matrix[5]));
//}
//else
//{
	output->appendf("<path ");	
//}
		if(fillR > -1 && fillG > -1 and fillB > -1) 
		{
			if(fillR==0 && fillG==0 && fillB==0)
			{
				output->append(" fill=\"black\"");
			} 
			else
			{
				output->appendf(" fill=\"#{0:x}{1:x}{2:x}\"",fillR,fillG,fillB);
			}
		}
		else
		{
			output->append(" fill=\"none\"");
		}

			if(strokeR > -1 && strokeG > -1 and strokeB > -1) 
			{
				if(strokeR==0 && strokeG==0 && strokeB==0)
				{
					output->append(" stroke=\"black\"");
				} 
				else
				{
					output->appendf(" stroke=\"#{0:x}{1:x}{2:x}\"",strokeR,strokeG,strokeB);
				}
			}
			else
			{
				output->append(" stroke=\"none\"");
			}
			
		if(fillRule)
			output->append(" fill-rule=\"nonzero\"");

		if(fillOpacity)
			output->appendf(" fill-opacity=\"{0:.2f}\"",fillOpacity);

		if (strokeWidth)
			output->appendf(" stroke-width=\"{0:.2f}\"",pixel(strokeWidth));

		if(strokeOpacity)
			output->appendf(" stroke-opacity=\"{0:.2f}\"",strokeOpacity);
		
		output->appendf(" d=\"{0:s}\"",path->getCString());


		output->append("/>\n");
		return output;
	
}


// Path

SVGRect::SVGRect()
{
}


SVGRect::SVGRect(double ix, double iy,double iw, double ih, double stWidth,double stOp, int stR,int stG,int stB, double fiOp, GooString *fiRu, int fiR,int fiG,int fiB)
{
	x=ix;
	y=iy;
	width=iw;
	height=ih;
	strokeWidth=stWidth;
	strokeOpacity=stOp;
	strokeR=stR;
	strokeG=stG;
	strokeB=stB;
	fillOpacity=fiOp;
	fillRule=fiRu;
	fillR=fiR;
	fillG=fiG;
	fillB=fiB;
}

SVGRect::~SVGRect()
{
	if(fillRule) delete fillRule;
}

GooString *SVGRect::dump()
{
	GooString *output=new GooString("");
	
//	if(matrix[0]!=1.0 || matrix[1]!=0.0 || matrix[2]!=0.0 || matrix[3]!=-1.0)
//	{
//	output->appendf("<path transform=\"matrix({0:d} {1:d} {2:d} {3:d} {4:.2f} {5:.2f})\"",(int)matrix[0],(int)matrix[1],(int)matrix[2],(int)matrix[3],pixel(matrix[4]),pixel(matrix[5]));
//}
//else
//{
	output->appendf("<rect ");	
		output->appendf("x=\"{0:.2f}\" y=\"{1:.2f}\" width=\"{2:.2f}\" height=\"{3:.2f}\"",x,y,width,height);
//}
		if(fillR > -1 && fillG > -1 and fillB > -1) 
		{
			if(fillR==0 && fillG==0 && fillB==0)
			{
				output->append(" fill=\"black\"");
			} 
			else
			{
				output->appendf(" fill=\"#{0:x}{1:x}{2:x}\"",fillR,fillG,fillB);
			}
		}
		else
		{
			output->append(" fill=\"none\"");
		}

			if(strokeR > -1 && strokeG > -1 and strokeB > -1) 
			{
				if(strokeR==0 && strokeG==0 && strokeB==0)
				{
					output->append(" stroke=\"black\"");
				} 
				else
				{
					output->appendf(" stroke=\"#{0:x}{1:x}{2:x}\"",strokeR,strokeG,strokeB);
				}
			}
			else
			{
				output->append(" stroke=\"none\"");
			}
			
		if(fillRule)
			output->append(" fill-rule=\"nonzero\"");

		if(fillOpacity)
			output->appendf(" fill-opacity=\"{0:.2f}\"",fillOpacity);

		if (strokeWidth)
			output->appendf(" stroke-width=\"{0:.2f}\"",pixel(strokeWidth));

		if(strokeOpacity)
			output->appendf(" stroke-opacity=\"{0:.2f}\"",strokeOpacity);
		
	

		output->append("/>\n");
		return output;
	
}


/* OutputDev */

SVGOutputDev::SVGOutputDev(XRef *xrefA)
{
     fonts=new SVGFontAccu();
	
	curFont=NULL;
	xref=xrefA;
	ok = gTrue;
	svg=new GooString();
	curPageWidth=0;
	curPageHeight=0;
	lineY=0;
	lastX=0;
	startedLine=gFalse;
	fontSize=0;
	fontFamily=new GooString("");
	wordSpace=0;
	charSpace=0;
	textWidth=0;
	textWidthDiff=0;
	textWidthTmp=0;

	curText=new GooString("");
	curTxt=NULL;
	Docname=new GooString ("img");
  
	startX=0;
	texts=new GooVector<SVGText>(); 
	paths=new GooVector<SVGPath>(); 
//	nodes=new GooVector<SVGNode>(); 

	curNode=0;
}

SVGOutputDev::~SVGOutputDev()
{	
	  if (fonts) delete fonts;
		if (texts) delete texts;
		if (paths) delete paths;
//		if (nodes) delete nodes;

}


int SVGOutputDev::AddNode(SVGNode* node){
 nodes[curNode]=node;
	curNode=curNode+1;
	return curNode;

}


void SVGOutputDev::drawImageMask(GfxState *state, Object *ref,
                             Stream *str,
                             int width, int height, GBool invert,
                             GBool interpolate, GBool inlineImg) {

  FILE *f1;
  int c;
  
  int x0, y0;			// top left corner of image
  int w0, h0, w1, h1;		// size of image
  double xt, yt, wt, ht;
  GBool rotate, xFlip, yFlip;
 
  // get image position and size
  state->transform(0, 0, &xt, &yt);
  state->transformDelta(1, 1, &wt, &ht);
  if (wt > 0) {
    x0 = xoutRound(xt);
    w0 = xoutRound(wt);
  } else {
    x0 = xoutRound(xt + wt);
    w0 = xoutRound(-wt);
  }
  if (ht > 0) {
    y0 = xoutRound(yt);
    h0 = xoutRound(ht);
  } else {
    y0 = xoutRound(yt + ht);
    h0 = xoutRound(-ht);
  }
  state->transformDelta(1, 0, &xt, &yt);
  rotate = fabs(xt) < fabs(yt);
  if (rotate) {
    w1 = h0;
    h1 = w0;
    xFlip = ht < 0;
    yFlip = wt > 0;
  } else {
    w1 = w0;
    h1 = h0;
    xFlip = wt < 0;
    yFlip = ht > 0;
  }

  // dump JPEG file
  if (str->getKind() == strDCT) {
    GooString *fName=new GooString(Docname);
    fName->append("-");
    GooString *pgNum=GooString::fromInt(pageNum);
    GooString *imgnum=GooString::fromInt(imgNum);
    // open the image file
    fName->append(pgNum)->append("_")->append(imgnum)->append(".jpg");
    ++imgNum;
    if (!(f1 = fopen(fName->getCString(), "wb"))) {
      error(-1, "Couldn't open image file '%s'", fName->getCString());
      return;
    }

    // initialize stream
    str = ((DCTStream *)str)->getRawStream();
    str->reset();

    // copy the stream
    while ((c = str->getChar()) != EOF)
      fputc(c, f1);

    fclose(f1);
   
//  if (pgNum) delete pgNum;
  if (imgnum) delete imgnum;
  if (fName) {
/*	imgList->append(fName);
	HtmlImage img(state,x0,y0,w0,h0,fName);
	pages->AddImage(img);
	*/
//	svg->appendf("<image xlink:href=\"{0:s}\"/>\n",fName);
	
	
	}
	
  }
  else {
   OutputDev::drawImageMask(state, ref, str, width, height, invert,interpolate, inlineImg);
  }
}

void SVGOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
                       int width, int height, GfxImageColorMap *colorMap,
                       GBool interpolate, int *maskColors, GBool inlineImg) 
{

  FILE *f1;
  int c;
  
  int x0, y0;			// top left corner of image
  int w0, h0, w1, h1;		// size of image
  double xt, yt, wt, ht;
  GBool rotate, xFlip, yFlip;
 
  // get image position and size
  state->transform(0, 0, &xt, &yt);
  state->transformDelta(1, 1, &wt, &ht);
  if (wt > 0) {
    x0 = xoutRound(xt);
    w0 = xoutRound(wt);
  } else {
    x0 = xoutRound(xt + wt);
    w0 = xoutRound(-wt);
  }
  if (ht > 0) {
    y0 = xoutRound(yt);
    h0 = xoutRound(ht);
  } else {
    y0 = xoutRound(yt + ht);
    h0 = xoutRound(-ht);
  }
  state->transformDelta(1, 0, &xt, &yt);
  rotate = fabs(xt) < fabs(yt);
  if (rotate) {
    w1 = h0;
    h1 = w0;
    xFlip = ht < 0;
    yFlip = wt > 0;
  } else {
    w1 = w0;
    h1 = h0;
    xFlip = wt < 0;
    yFlip = ht > 0;
  }

   
  /*if( !globalParams->getErrQuiet() ) */
//    printf("image stream of kind %d\n", str->getKind());
/**/
  // dump JPEG file
  if (str->getKind() == strDCT) {
    GooString *fName=new GooString(Docname);
    fName->append("-");
    GooString *pgNum= GooString::fromInt(pageNum);
    GooString *imgnum= GooString::fromInt(imgNum);  
    
    // open the image file
    fName->append(pgNum)->append("_")->append(imgnum)->append(".jpg");
    ++imgNum;
    
    if (!(f1 = fopen(fName->getCString(), "wb"))) {
      error(-1, "Couldn't open image file '%s'", fName->getCString());
      return;
    }

//	printf("%ix%i\n",x0,y0);
    // initialize stream
    str = ((DCTStream *)str)->getRawStream();
    str->reset();

    // copy the stream
    while ((c = str->getChar()) != EOF)
      fputc(c, f1);
    
    fclose(f1);
  
    if (fName) {
//			svg->appendf("<image x=\"{0:d}\" y=\"{1:d}\" width=\"{2:d}\" height=\"{3:d}\" xlink:href=\"{4:s}\"/>\n",(int)x0,(int)y0,(int)w0,(int)h0,fName);
			SVGImage *img=new SVGImage(x0,y0,w0,h0,fName);
			this->AddNode(img);
/*		HtmlImage img(state,x0,y0,w0,h0,fName);
		pages->AddImage(img);
		imgList->append(fName);
		double *matrix=state->getCTM();
	*/	
		
	}
    delete pgNum;
    delete imgnum;
  }
  else {
#ifdef ENABLE_LIBPNG
    // Dump the image as a PNG file. Much of the PNG code
    // comes from an example by Guillaume Cottenceau.
    Guchar *p;
    GfxRGB rgb;
    png_structp png_ptr;
    png_infop info_ptr;
    png_byte color_type= PNG_COLOR_TYPE_RGB;
    png_byte bit_depth= 8;
    png_byte *row = (png_byte *) malloc(3 * width);   // 3 bytes/pixel: RGB
    png_bytep *row_pointer= &row;

    // Create the image filename
    GooString *fName=new GooString(Docname);
    fName->append("-");
    GooString *pgNum= GooString::fromInt(pageNum);
    GooString *imgnum= GooString::fromInt(imgNum);  
    fName->append(pgNum)->append("_")->append(imgnum)->append(".png");

    // Open the image file
    if (!(f1 = fopen(fName->getCString(), "wb"))) {
      error(-1, "Couldn't open image file '%s'", fName->getCString());
      return;
    }

    // Initialize the PNG stuff
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
      error(-1, "png_create_write_struct failed");
      return;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
      error(-1, "png_create_info_struct failed");
      return;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
      error(-1, "error during init_io");
      return;
    }

    // Write the PNG header
    png_init_io(png_ptr, f1);
    if (setjmp(png_jmpbuf(png_ptr))) {
      error(-1, "error during writing png header");
      return;
    }

    // Set up the type of PNG image and the compression level
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Write the image info bytes
    png_write_info(png_ptr, info_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
      error(-1, "error during writing png info bytes");
      return;
    }

    // Initialize the image stream
    ImageStream *imgStr = new ImageStream(str, width,
                        colorMap->getNumPixelComps(), colorMap->getBits());
    imgStr->reset();

    // For each line...
    for (int y = 0; y < height; y++) {

      // Convert into a PNG row
      p = imgStr->getLine();
      for (int x = 0; x < width; x++) {
        colorMap->getRGB(p, &rgb);
	// Write the RGB pixels into the row
	row[3*x]= colToByte(rgb.r);
	row[3*x+1]= colToByte(rgb.g);
	row[3*x+2]= colToByte(rgb.b);
         p += colorMap->getNumPixelComps();
      }

      // Write the row to the file
      png_write_rows(png_ptr, row_pointer, 1);
      if (setjmp(png_jmpbuf(png_ptr))) {
        error(-1, "error during png row write");
        return;
      }
    }

    // Finish off the PNG file
    png_write_end(png_ptr, info_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
      error(-1, "error during png end of write");
      return;
    }

    fclose(f1);
    free(row);
    imgList->append(fName);
	HtmlImage img(state,x0,y0,w0,h0,fName);
	pages->AddImage(img);

//	pages->beginString(state, fName);
//	pages->endString();

    ++imgNum;
    delete pgNum;
    delete imgnum;
    delete imgStr;
#else
    OutputDev::drawImage(state, ref, str, width, height, colorMap,interpolate,
                       maskColors, inlineImg);
#endif


  }

}

/*
void SVGOutputDev::drawMaskedImage(
  GfxState *state, Object *ref, Stream *str,
  int width, int height, GfxImageColorMap *colorMap,
  Stream *maskStr, int maskWidth, int maskHeight, GBool maskInvert) {
  drawImage(state, ref, str, width, height, colorMap, NULL, gFalse);
  drawImageMask(state, ref, maskStr, maskWidth, maskHeight,
		maskInvert, gFalse);
}

void SVGOutputDev::drawSoftMaskedImage(
  GfxState *state, Object *ref, Stream *str,
  int width, int height, GfxImageColorMap *colorMap,
  Stream *maskStr, int maskWidth, int maskHeight,
  GfxImageColorMap *maskColorMap) {
  drawImage(state, ref, str, width, height, colorMap, NULL, gFalse);
  drawImage(state, ref, maskStr, maskWidth, maskHeight,
	    maskColorMap, NULL, gFalse);
//  drawImageMask(state, ref, maskStr, maskWidth, maskHeight,
//	    gFalse, gFalse);


}
*/

 void SVGOutputDev::dumpContent()
{
	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
//	printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\" \n\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n");
//	printf("<?xml-stylesheet href=\"style.css\" type=\"text/css\"?>\n");
	printf("<svg version=\"1.2\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" xml:id=\"svg-root\" width=\"%d\" height=\"%d\" viewBox=\"%d %d %d %d\" xmlns=\"http://www.w3.org/2000/svg\">\n",
	(int)pixel(curPageWidth),(int)pixel(curPageHeight),(int)pixel(this->cropBox->x1),(int)pixel(this->mediaBox->y2 - this->cropBox->y2),(int)pixel(this->cropBox->x2 - this->cropBox->x1),(int)pixel(this->cropBox->y2 - this->cropBox->y1));
	printf("<defs>\n");
//	printf("<style>\n");

	printf("%s",fonts->dump()->getCString());
	
//	printf("</style>\n");
	printf("</defs>\n");
/*	printf("<pageSet>\n");
	
	printf("%s",svg->getCString());
	printf("</pageSet>\n");
*/
	printf("%s",svg->getCString());

	printf("</svg>\n");
}

void SVGOutputDev::endPage()
{
	int i;
/*	for(GooVector<SVGText>::iterator i=texts->begin();i!=texts->end();i++){
		svg->append(i->dump()); 
	}
	int j;
	for(GooVector<SVGPath>::iterator j=paths->begin();j!=paths->end();j++){
		svg->append(j->dump()); 
	}


			if (texts) delete texts;
			if (paths) delete paths;

			texts=new GooVector<SVGText>(); 
			paths=new GooVector<SVGPath>(); 
			*/

/*			
			for(GooVector<SVGNode>::iterator i=nodes->begin();i!=nodes->end();i++){
					svg->append(i->dump()); 
				}
			
				if (nodes) delete nodes;

				nodes=new GooVector<SVGNode>(); 
	*/
//				printf("bla\n");
				for(i=0;i<curNode;i++)
				{
//					printf("%x\n",node);
//					printf("%d\n",i);
					svg->append(nodes[i]->dump());
//					delete nodes[i];
//					nodes[i]=NULL;
				}
//				delete nodes;
//				delete curTxt;
				curNode=0;
//	closeText();
	svg->append("</g>\n"); 

	
}

void SVGOutputDev::startPage(int pageNum, GfxState *state)
{
//	printf("Page %d\n",pageNum);
	
/*	
	cropX1=this->cropBox->x1;
	cropY1=this->mediaBox->y2 - this->cropBox->y2;
	cropW=this->cropBox->x2 - this->cropBox->x1;
	cropH=this->cropBox->y2 - this->cropBox->y1;

*/	
	this->pageNum=pageNum;
	curPageWidth=state->getPageWidth();
	curPageHeight=state->getPageHeight();
	double *matrix=state->getCTM();
	svg->appendf("<g id=\"{0:d}\">\n",pageNum);
//	svg->appendf("<g transform=\"matrix({0:d} {1:d} {2:d} {3:d} {4:.2f} {5:.2f})\">\n",(int)matrix[0],(int)matrix[1],(int)matrix[2],(int)matrix[3],matrix[4],matrix[5]);

//	svg->appendf("<svg width=\"{0:d}pt\" height=\"{1:d}pt\">\n",(int)state->getPageWidth(),(int)state->getPageHeight()); 
		fontSize=state->getTransformedFontSize();
			curFont=state->getFont();
//		fontFamily=state->getFont()->getName();
}

GooString* uconv(Unicode* u, int uLen) {
  GooString *tmp = new GooString();
  UnicodeMap *uMap=new UnicodeMap("UTF-8", gTrue, &mapUTF8);
  char buf[8];
  int n;

  // get the output encoding
//  if (!(uMap = globalParams->getTextEncoding())) {
 //   return tmp;
 // }
//uMap="UTF-8";

  for (int i = 0; i < uLen; ++i) {
    switch (u[i])
      { 
	case '"': tmp->append("&quot;");  break;
	case '&': tmp->append("&amp;");  break;
	case '<': tmp->append("&lt;");  break;
	case '>': tmp->append("&gt;");  break;
	case ' ': tmp->append(" " );
	          break;
	default:  
	  {
	    // convert unicode to string
	    if ((n = uMap->mapUnicode(u[i], buf, sizeof(buf))) > 0) {
	      tmp->append(buf, n); 
	  }
      }
    }
  }

  uMap->decRefCnt();
  return tmp;
}


void SVGOutputDev::closeText()
{
	if(startedLine)
	{
//		svg->appendf(" textLength=\"{0:.2f}\"",textWidth);
//		svg->append(">");
//		curTxt->setTextWidth(textWidth);
//		textWidth=0;
//		svg->append(curText);
		delete curText;
		curText=new GooString("");

//		svg->append("</text>\n");
		startedLine=gFalse;	
	}
}

void SVGOutputDev::drawChar(GfxState *state, double x, double y,
	      double dx, double dy,
	      double originX, double originY,
	      CharCode code, int /*nBytes*/, Unicode *u, int uLen) 
{
	GfxRGB rgb;	
	double cx = state->getCurX(),cy=state->getCurY();
	double *matrix=state->getCTM();
	GooString *fntFamily=state->getFont()->getName()->copy();
	state->getFillRGB(&rgb);
	int r=0,g=0,b=0;
	r=(int)(rgb.r/65535.0*255.0);
	g=(int)(rgb.g/65535.0*255.0);
	b=(int)(rgb.b/65535.0*255.0);
/*	GfxPath *path=state->getPath();
	GooString *p=convertPath(matrix, path);
	printf("%s : %s\n",u,p->getCString());
*/
	int rotate;
	
	curFont=state->getFont();
	
	
	int i;
	for(i=0;i<fntFamily->getLength();i++)
	{
		if(fntFamily->getChar(i)=='+')
		{
			break;
		}
	}
	if(i < fntFamily->getLength()-1)
	{
		fntFamily->del(0,i+1);
	}
	
	state->textTransform( x, y, &x, &y );
	state->transform( x, y, &x, &y );

	state->transform( cx, cy, &cx, &cy );
//	state->transform( dx, dy, &dx, &dy );
	
//	if(curTxt)
//		printf("%s %.2f %.2f %.2f %.2f %.2f %.2f\n",curTxt->getStr()->getCString(),state->getCharSpace(),state->getWordSpace(),curTxt->spaceSize,curTxt->xMax,cx,curTxt->xMax + (curTxt->spaceSize)*1.5);
	
	if(curTxt
		&& lineY==y 
		&& fontSize==state->getTransformedFontSize() 
		&& fontFamily->cmp(fntFamily) == 0  
//		&& charSpace==state->getCharSpace() 
//		&& wordSpace==state->getWordSpace()
		&& cx < curTxt->xMax + (curTxt->spaceSize)*1.5 
		)
	{
		curTxt->addStr(uconv(u,uLen));
		curTxt->xMax=curTxt->xMax+dx;
		// - (state->getCharSpace() * state->getHorizScaling());
//		curTxt->yMax=curTxt->yMax+dy;
		curTxt->setTextWidth(curTxt->xMax - curTxt->xMin);
//		startX=cx+curTxt->xMax;
		
	}
	else
	{
//		startX=cx;
		
		 double *fontm;
		 double m[4], m2[4];
		 int rot;
		 state->getFontTransMat(&m[0], &m[1], &m[2], &m[3]);

		  if (curFont && curFont->getType() == fontType3) {
		    fontm = state->getFont()->getFontMatrix();
		    m2[0] = fontm[0] * m[0] + fontm[1] * m[2];
		    m2[1] = fontm[0] * m[1] + fontm[1] * m[3];
		    m2[2] = fontm[2] * m[0] + fontm[3] * m[2];
		    m2[3] = fontm[2] * m[1] + fontm[3] * m[3];
		    m[0] = m2[0];
		    m[1] = m2[1];
		    m[2] = m2[2];
		    m[3] = m2[3];
		  }
		  if (fabs(m[0] * m[3]) > fabs(m[1] * m[2])) {
		    rot = (m[3] < 0) ? 0 : 2;
		  } else {
		    rot = (m[2] > 0) ? 1 : 3;
		  }
		
			fontSize=state->getTransformedFontSize();
			fontFamily=fntFamily;
		
		
		SVGText *txt=new SVGText(cx,cy,state->getTransformedFontSize(),fntFamily,0,0,rot,r,g,b);
		this->AddNode(txt);
		curTxt=txt;
		curTxt->addStr(uconv(u,uLen));
		
		curTxt->xMin=cx;
		curTxt->yMin=cy;
		curTxt->xMax=cx+dx;
		// - (state->getCharSpace() * state->getHorizScaling());
		curTxt->yMax=cy+dy;
		curTxt->setTextWidth(curTxt->xMax - curTxt->xMin);
		
		startX=cx + (curTxt->xMax - curTxt->xMin);
		curTxt->spaceSize=(getSpaceSize(curFont) - state->getCharSpace())*fontSize;		

		if(curTxt->spaceSize==0)
		{
			curTxt->spaceSize=2.0;
		}
		
		double *bbox=curFont->getFontBBox();
		double tmpx,tmpy;
//		printf("%.2f %.2f %.2f %.2f\n",bbox[0],bbox[1],bbox[2],bbox[3]);
//		state->transform(bbox[0],bbox[1],&tmpx,&tmpy);
		curTxt->yMin=cy;
//		(bbox[1]*state->getTransformedFontSize()
//		state->transform(bbox[2],bbox[3],&tmpx,&tmpy);
		curTxt->yMax=cy+(bbox[3]*state->getTransformedFontSize());

		/*
		GooString *style=new GooString("normal");
		GooString *weight=new GooString("");
		
		if(curFont->isItalic())
		{
			style=new GooString("italic");
		}
		
		
		switch(curFont->getWeight())
		{
			case GfxFont::W100:
			weight=new GooString("100");
			break;
			case GfxFont::W200:
			weight=new GooString("200");
			break;
			case GfxFont::W300:
			weight=new GooString("300");
			break;
			case GfxFont::W400:
			weight=new GooString("normal");
			break;
			case GfxFont::W500:
			weight=new GooString("500");
			break;
			case GfxFont::W600:
			weight=new GooString("600");
			break;
			case GfxFont::W700:
			weight=new GooString("bold");
			break;
			case GfxFont::W800:
			weight=new GooString("800");
			break;
			case GfxFont::W900:
			weight=new GooString("900");
			break;


			default: 
			weight=new GooString("normal");
			break;
		}
		
		SVGFont fnt=SVGFont(fntFamily,curFont->getAscent(),curFont->getDescent(),style,weight);
		fonts->AddFont(fnt);
		*/
	}
	lineY=y;	
	
}

double SVGOutputDev::getSpaceSize(GfxFont *font)
{
	GooString *s=new GooString(" ");
	char * p = s->getCString();
	int len = s->getLength();
	Unicode *u = NULL;
	Unicode *str=NULL;
	int slen=0;
	int uLen;
	int size=0;
	double dx, dy, originX, originY;
	CharCode code;
	
	int n = font->getNextChar(p, len, &code, &u, &uLen, &dx, &dy, &originX, &originY);
//	printf("%.2f\n",dx*font->getSize());
	
//	if(dx==0)
//		return 2.0;
//	else
		return dx;
}

void SVGOutputDev::drawString( GfxState * state, GooString * s )
{
	GfxRGB rgb;
	double x = state->getLineX(), y = state->getLineY();
	double cx = state->getCurX(),cy=state->getCurY();
	double *matrix=state->getCTM();
//	double x = state->getX1(), y = state->getY1();
//	double x=0,y=0;
	char * p = s->getCString();
	int len = s->getLength();
	Unicode *u = NULL;
	Unicode *str=NULL;
	int slen=0;
	int uLen;
	int size=0;
	double dx, dy, originX, originY;
	CharCode code;
	curFont=state->getFont();
	double width=0;
		
	GooString *fntFamily=curFont->getName()->copy();
	int i;
	
	for(i=0;i<fntFamily->getLength();i++)
	{
		if(fntFamily->getChar(i)=='+')
		{
			break;
		}
	}
	if(i < fntFamily->getLength()-1)
	{
		fntFamily->del(0,i+1);
	}

	double totw=0;
	double toth=0;

	state->textTransform( x, y, &x, &y );
	state->transform( x, y, &x, &y );

	state->transform( cx, cy, &cx, &cy );

	
		if(lineY==y 
		&& fontSize==state->getTransformedFontSize() 
		&& fontFamily->cmp(fntFamily) == 0  
		&& charSpace==state->getCharSpace() 
		&& wordSpace==state->getWordSpace()
		&& cx < startX + 3.0
		
		)
	//	if(false)
		{
				textWidthTmp+=cx - lastX;
				textWidthDiff=textWidthTmp - textWidth;
		}
		else
		{
			textWidth=0;
			textWidthTmp=0;	
		}
	
	int sps=0;


	while( len > 0 )
	{
		size+=16;

		int n = curFont->getNextChar(p, len, &code, &u, &uLen, &dx, &dy, &originX, &originY);
		p += n;
		len -= n;
		str = (Unicode *)grealloc(str, (size) * sizeof(Unicode));
		str[slen]=*u;

		double ws=0;
		double cs=state->getCharSpace();

		if ((*u==0x0020 || *u==0x00A0 || *u==0x3000) && len > 0)
		{
			sps+=1;
			ws=state->getWordSpace();
		}


		totw+=(dx + cs + ws)*state->getTransformedFontSize();
		totw-=(state->getCharSpace() * state->getHorizScaling());
		toth+=dy;
		
//		state->transformDelta(totw,toth,&totw,&toth);
//		tw=state->transformWidth(totw);

		textWidth+=(dx + cs + ws)*state->getTransformedFontSize();
//					printf("DEBUG: %d %s %.2f %.2f %.2f %.2f = %.2f\n",slen,u,dx,dy,originX,originY,textWidth);
//		printf("DEBUG: %s %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f = %.2f\n",u,uLen,dx,dy,originX,originY,state->getTransformedFontSize(),state->getHorizScaling(),totw,textWidth);
		width+=originX;
		slen+=1;
	}	

//	double xMin, yMin, xMax, yMax;
//	state->textTransform(xMin,yMin,xMax,yMax);
//	printf("DEBUG: %.2f %.2f %.2f %.2f\n",xMin,yMin,xMax,yMax);

//	printf("%.2f\n",state->getLineWidth());
	double *textMat=state->getTextMat();
//		printf("DEBUG: %s %.2f %.2f %.2f %.2f %.2f %.2f\n",s->getCString(),textMat[0],textMat[1],textMat[2],textMat[3],textMat[4],textMat[5]);
//	printf("DEBUG: %.2f %.2f %.2f %.2f\n",x,y,cx,cy);
//	printf("DEBUG: %.2f %.2f %.2f\n",state->getLeading(),state->getRise(),state->getHorizScaling());

	
//	int rotate=state->getRotate();
	state->getFillRGB(&rgb);
	int r=0,g=0,b=0;
	r=(int)(rgb.r/65535.0*255.0);
	g=(int)(rgb.g/65535.0*255.0);
	b=(int)(rgb.b/65535.0*255.0);

	int rotate;
//	printf("%f\n",y);
	

//	double *tmpm=state->getTextMat();
//	printf("DEBUG: %s\n",str);
//	printf("DEBUG: %d\n",state->getFlatness());
//	printf("DEBUG: %.2f %.2f %.2f %.2f %.2f\n",tmpm[0],tmpm[1],tmpm[2],tmpm[3],tmpm[4],tmpm[5]);
//	printf("DEBUG: %.2f\n",state->getTransformedFontSize());
//	printf("DEBUG: %.2f %.2f\n",state->getFont()->getAscent(),state->getFont()->getDescent());
//	printf("DEBUG tot %.2f\n",totw);
//	printf("%s\n",str);

	if(lineY==y 
	&& fontSize==state->getTransformedFontSize() 
	&& fontFamily->cmp(fntFamily) == 0  
	&& charSpace==state->getCharSpace() 
	&& wordSpace==state->getWordSpace()
	&& cx < startX + 3.0
	)
//	if(false)
	{	
			curTxt->xMax=curTxt->xMax+totw;
			curTxt->yMax=curTxt->yMax+toth;
				
				curTxt->addStr(uconv(str,slen));
				curTxt->setTextWidth(textWidth);

			fontSize=state->getTransformedFontSize();
			fontFamily=fntFamily;
			wordSpace=state->getWordSpace();
			charSpace=state->getCharSpace();
			
			
	}
	else
	{
			startX=cx;


			 double *fontm;
			 double m[4], m2[4];
			 int rot;
			 state->getFontTransMat(&m[0], &m[1], &m[2], &m[3]);

			  if (curFont && curFont->getType() == fontType3) {
			    fontm = state->getFont()->getFontMatrix();
			    m2[0] = fontm[0] * m[0] + fontm[1] * m[2];
			    m2[1] = fontm[0] * m[1] + fontm[1] * m[3];
			    m2[2] = fontm[2] * m[0] + fontm[3] * m[2];
			    m2[3] = fontm[2] * m[1] + fontm[3] * m[3];
			    m[0] = m2[0];
			    m[1] = m2[1];
			    m[2] = m2[2];
			    m[3] = m2[3];
			  }
			  if (fabs(m[0] * m[3]) > fabs(m[1] * m[2])) {
			    rot = (m[3] < 0) ? 0 : 2;
			  } else {
			    rot = (m[2] > 0) ? 1 : 3;
			  }


		startedLine=gTrue;
		fontSize=state->getTransformedFontSize();
		fontFamily=fntFamily;
		wordSpace=state->getWordSpace();
		charSpace=state->getCharSpace();

//		printf("DEBUG: %s : %.2f %.2f\n",s->getCString(),state->getCurX(),state->getLineX());

		int i;
		SVGText *txt=new SVGText(cx,cy,state->getTransformedFontSize(),fntFamily,wordSpace,charSpace,rot,r,g,b);


		GooString *style=new GooString("normal");
		GooString *weight=new GooString("");
		
		if(curFont->isItalic())
		{
			style=new GooString("italic");
		}
		
		
		switch(curFont->getWeight())
		{
			case GfxFont::W100:
			weight=new GooString("100");
			break;
			case GfxFont::W200:
			weight=new GooString("200");
			break;
			case GfxFont::W300:
			weight=new GooString("300");
			break;
			case GfxFont::W400:
			weight=new GooString("normal");
			break;
			case GfxFont::W500:
			weight=new GooString("500");
			break;
			case GfxFont::W600:
			weight=new GooString("600");
			break;
			case GfxFont::W700:
			weight=new GooString("bold");
			break;
			case GfxFont::W800:
			weight=new GooString("800");
			break;
			case GfxFont::W900:
			weight=new GooString("900");
			break;


			default: 
			weight=new GooString("normal");
			break;
		}
		
		SVGFont fnt=SVGFont(fntFamily,curFont->getAscent(),curFont->getDescent(),style,weight);
		fonts->AddFont(fnt);
//		updateFont(state);
		i=this->AddNode(txt);
		curTxt=txt;
		curTxt->xMin=cx;
		curTxt->yMin=cy;
		curTxt->xMax=cx+totw;
		curTxt->yMax=cy+toth;

		curTxt->addStr(uconv(str,slen));
		curTxt->setTextWidth(textWidth);
		startX=cx+textWidth;
	}
	
	lineY=y;
	lastX=cx;	

}

void SVGOutputDev::stroke(GfxState *state) {
//	closeText();
	
	double *matrix=state->getCTM();
	GfxPath *path=state->getPath();
	GfxRGB rgb;
	state->getStrokeRGB(&rgb);
	int r=(int)(rgb.r/65535.0*255.0);
	int g=(int)(rgb.g/65535.0*255.0);
	int b=(int)(rgb.b/65535.0*255.0);
	GooString *p=convertPath(matrix, path);
	
	if(!detectLine(state) && !detectRect(state,0)) {
 		SVGPath *pt=new SVGPath(state->getLineWidth(),state->getStrokeOpacity(),r,g,b,state->getFillOpacity(),NULL,-1,-1,-1,p,matrix);
		this->AddNode(pt);
	}
}

void SVGOutputDev::fill(GfxState *state) {
//	closeText();

		
	double *matrix=state->getCTM();
	GfxPath *path=state->getPath();
	GfxRGB rgb;
	int r=0,g=0,b=0;
	state->getFillRGB(&rgb);
	r=(int)(rgb.r/65535.0*255.0);
	g=(int)(rgb.g/65535.0*255.0);
	b=(int)(rgb.b/65535.0*255.0);
	GooString *p=convertPath(matrix, path);
	
	if(!detectLine(state) && !detectRect(state,1)) {
	
	SVGPath *pt=new SVGPath(NULL,NULL,-1,-1,-1,state->getFillOpacity(),new GooString("nonzero"),r,g,b,p,matrix);
	this->AddNode(pt);
	}
}

void SVGOutputDev::eofill(GfxState *state) {
//	closeText();

	double *matrix=state->getCTM();
	GfxPath *path=state->getPath();
	GfxRGB rgb;
	int r=0,g=0,b=0;
	state->getFillRGB(&rgb);
	r=(int)(rgb.r/65535.0*255.0);
	g=(int)(rgb.g/65535.0*255.0);
	b=(int)(rgb.b/65535.0*255.0);
	GooString *p=convertPath(matrix, path);
		
		if(!detectLine(state) && !detectRect(state,2)) {
		
	SVGPath *pt=new SVGPath(NULL,NULL,-1,-1,-1,state->getFillOpacity(),new GooString("evenodd"),r,g,b,p,matrix);
	this->AddNode(pt);
	}
}

void SVGOutputDev::endMarkedContent(GfxState *state)
{
//	printf("<!-- endMarked -->\n");
//	SVGEndNode *nd=new SVGEndNode(new GooString("g"));
//	this->AddNode(nd);
}
void SVGOutputDev::beginMarkedContent(char *name, Dict *properties)
{
//	printf("<!-- beginMarked -->\n");

//	SVGStartNode *nd=new SVGStartNode(new GooString("g"));
//	this->AddNode(nd);	
}


void SVGOutputDev::transform(double *matrix,
                                   double xi, double yi,
                                   double *xo, double *yo) {
  //                          [ m[0] m[1] 0 ]                                                                                                                                        
  // [xo yo 1] = [xi yi 1] *  [ m[2] m[3] 0 ]                                                                                                                                        
  //                          [ m[4] m[5] 1 ]                                                                                                                                        
  *xo = xi * matrix[0] + yi * matrix[2] + matrix[4];
  *yo = xi * matrix[1] + yi * matrix[3] + matrix[5];
}

bool SVGOutputDev::detectLine(GfxState *state)
{
	
	double *matrix=state->getCTM();
	GfxPath *path=state->getPath();
	GfxRGB rgb;
	int r=0,g=0,b=0;
	state->getFillRGB(&rgb);
	r=(int)(rgb.r/65535.0*255.0);
	g=(int)(rgb.g/65535.0*255.0);
	b=(int)(rgb.b/65535.0*255.0);
	
	
		if(path->getNumSubpaths()==1)
		{
		GfxSubpath * subpath = path->getSubpath( 0 );
	
	if(subpath->getNumPoints() == 2)
	{
		double x1,y1;
		transform(matrix,subpath->getX(0),subpath->getY(0),&x1,&y1);
		
		if(!subpath->getCurve(1))
		{
			double x2,y2;
			transform(matrix,subpath->getX(1),subpath->getY(1),&x2,&y2);
			SVGLine *ln=new SVGLine(x1,y1,x2,y2,state->getLineWidth(),state->getStrokeOpacity(),r,g,b);
			this->AddNode(ln);
			
			return true;
		}
	}
}
	return false;
	
}


bool SVGOutputDev::detectRect(GfxState *state, int t)
{
	double *matrix=state->getCTM();
	GfxPath *path=state->getPath();
	GfxRGB frgb;
	GfxRGB srgb;

	int fr=0,fg=0,fb=0,sr=0,sg=0,sb=0;
	state->getFillRGB(&frgb);
	fr=(int)(frgb.r/65535.0*255.0);
	fg=(int)(frgb.g/65535.0*255.0);
	fb=(int)(frgb.b/65535.0*255.0);
	state->getStrokeRGB(&srgb);
	sr=(int)(srgb.r/65535.0*255.0);
	sg=(int)(srgb.g/65535.0*255.0);
	sb=(int)(srgb.b/65535.0*255.0);
	
	
		if(path->getNumSubpaths()==1)
		{
		GfxSubpath * subpath = path->getSubpath( 0 );
	
	if(subpath->getNumPoints() == 5)
	{
		double x0,y0;
		transform(matrix,subpath->getX(0),subpath->getY(0),&x0,&y0);
		
		if(!subpath->getCurve(1) && !subpath->getCurve(2) && !subpath->getCurve(3) && !subpath->getCurve(4))
		{
			double x1,y1,x2,y2,x3,y3,x4,y4;
			transform(matrix,subpath->getX(1),subpath->getY(1),&x1,&y1);
			transform(matrix,subpath->getX(2),subpath->getY(2),&x2,&y2);
			transform(matrix,subpath->getX(3),subpath->getY(3),&x3,&y3);
			transform(matrix,subpath->getX(4),subpath->getY(4),&x4,&y4);

			if((x0==x1 || y0==y1) && (x1==x2 || y1==y2) && (x2==x3 || y2==y3) && (x3==x4 || y3==y4))
			{
				double width=0, height=0;
				if(x0<x1)
				{
					width=abs(x1-x0);
				}	else			
				{
					height=abs(y1-y0);					
				}
				if(x1<x2)
				{
					width=abs(x2-x1);
				} else
				{
					height=abs(y2-y1);					
				}
				
				if (x1 < x0)
					x0=x1;
				if (x2 < x0)
					x0=x2;
				if (x3 < x0)
					x0=x3;
				if (x4 < x0)
					x0=x4;
				if (y1 < y0)
					y0=y1;
				if (y2 < y0)
					y0=y2;
				if (y3 < y0)
					y0=y3;
				if (y4 < y0)
					y0=y4;
					
				
				SVGRect *pt;
				switch(t) {
				case 0:
					pt=new SVGRect(x0,y0,width,height,state->getLineWidth(),state->getStrokeOpacity(),sr,sg,sb,NULL,NULL,-1,-1,-1);
					break;
					case 1:
					pt=new SVGRect(x0,y0,width,height,NULL,NULL,-1,-1,-1,state->getFillOpacity(),new GooString("nonzero"),fr,fg,fb);
					
//									pt=new SVGRect(x0,y0,width,height,state->getLineWidth(),state->getStrokeOpacity(),sr,sg,sb,state->getFillOpacity(),new GooString("nonzero"),fr,fg,fb);
					break;
					case 2:
					pt=new SVGRect(x0,y0,width,height,NULL,NULL,-1,-1,-1,state->getFillOpacity(),new GooString("evenodd"),fr,fg,fb);
					break;

				}
				this->AddNode(pt);
				
				return true;
			}

			
			return false;
		}
	}
}
	return false;
	
}

GooString *SVGOutputDev::convertPath( double *matrix,GfxPath *path )
{
	if( ! path )
		return new GooString();

	GooString *output=new GooString("");

	for( int i = 0; i < path->getNumSubpaths(); ++i )
	{
		GfxSubpath * subpath = path->getSubpath( i );
		if( subpath->getNumPoints() > 0 )
		{
			double tx,ty;
			transform(matrix,subpath->getX(0),subpath->getY(0),&tx,&ty);
			output->appendf(" M {0:.2f},{1:.2f}",pixel(tx),pixel(ty));
			int j = 1;
			while( j < subpath->getNumPoints() )
			{
				if( subpath->getCurve( j ) )
				{
					output->appendf(" C");
					transform(matrix,subpath->getX(j),subpath->getY(j),&tx,&ty);
					output->appendf(" {0:.2f},{1:.2f}",tx,ty);
					transform(matrix,subpath->getX(j+1),subpath->getY(j+1),&tx,&ty);
					output->appendf(" {0:.2f},{1:.2f}",tx,ty);
					transform(matrix,subpath->getX(j+2),subpath->getY(j+2),&tx,&ty);					
					output->appendf(" {0:.2f},{1:.2f}",tx,ty);						
					j += 3;
				}
				else
				{
					transform(matrix,subpath->getX(j),subpath->getY(j),&tx,&ty);					
					output->appendf(" L {0:.2f},{1:.2f}",tx,ty);
					++j;
				}
			}
			if( subpath->isClosed() )
			{
//				printf("YES IT HAPPENS\n");
				output->append(" Z");
			}
		}
	}
//   output->append(" Z");

	return output;
}


void SVGOutputDev::generateFont(GfxState *state) {
//	curFont=state->getFont();

	Ref embRef;
	char *tmpBuf;
	int tmpBufLen;

	if(curFont && curFont->getEmbeddedFontName())
	{
		GooString *fntname=curFont->getName();
//		GooString *fntname=curFont->getFamily();
//		if (!fntname)
//		{
//			fntname=curFont->getName();
//		}

		int i;
		for(i=0;i<fntname->getLength();i++)
		{
			if(fntname->getChar(i)=='+')
			{
				break;
			}
		}
		if(i < fntname->getLength()-1)
		{
			fntname->del(0,i+1);
		}

//		printf("%x\n",curFont->getType());
//		SVGFont *tmpfnt=new SVGFont(fntname->copy());
			
//		if(!fonts->isFont(*tmpfnt)) {
		if (curFont->getEmbeddedFontID(&embRef)) {
			tmpBuf = curFont->readEmbFontFile(xref, &tmpBufLen);
			FILE *f;
			int cur_char;
		
			GooString *filename=fntname->copy();
			filename->append(".pfa");

/*
			filename->append(".ttf");
			printf("%s\n",filename->getCString());
			FILE *outputStream;
			outputStream=fopen (filename->getCString(), "wb");
			FoFiType1C *ttf = FoFiType1C::make(tmpBuf, tmpBufLen);
			ttf->writeTTF(NULL,outputStream,fntname->getCString(),NULL);
			fclose(outputStream);
*/
			//FoFiOutputFunc outputFunc, void *outputStream,
			  //              char *name = NULL, Gushort *codeToGID = NULL);
			

			f=fopen (filename->getCString(), "wb");
			for (cur_char = 0; cur_char < tmpBufLen; ++cur_char) {
				fputc(tmpBuf[cur_char], f);
			}
			fclose (f);
	
//			printf("%s\n",filename);
			
//			fonts->AddFont(*tmpfnt);
//		}
	  }
	 else
	{
//		delete tmpfnt;
	}
//		printf("Update font %s/%s/%s\n",curFont->getName(),curFont->getEmbeddedFontName(),curFont->getExtFontFile());
	}
//		printf("Update font\n");

}
