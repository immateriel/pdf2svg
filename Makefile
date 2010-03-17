# Compile flags for FreeBSD 6
#CXXFLAGS= -O2 -Wall -I. -I/usr/local/include/poppler -I/usr/local/include
#CFLAGS= $(CXXFLAGS)
#LIBDIRS= -L/usr/local/lib

# Compile flags for Ubuntu Linux
CXXFLAGS= -O2 -Wall -I. -I/usr/local/include/poppler -I/opt/local/include
CFLAGS= $(CXXFLAGS)
LIBDIRS=

pdftosvg_OBJ = pdftosvg.o parseargs.o SVGOutputDev.o
pdftosvg_SRC = SVGOutputDev.cc SVGOutputDev.h pdftosvg.cc parseargs.cc


pdftosvg: $(pdftosvg_OBJ)
	c++ $(DEBUG) -Wall -Wno-write-strings -fno-strict-aliasing -pipe -L/usr/local/lib -lpoppler -L/opt/local/lib -lpng \
	-o pdftosvg $(pdftosvg_OBJ) $(LIBDIRS)

clean:
	rm -rf $(pdftosvg_OBJ) pdftosvg
