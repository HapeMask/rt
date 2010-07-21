.PHONY: all clean run parser parserclean
SRCDIR=../src

SDL_CXXFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CXXFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib

PC_CXXFLAGS= -g -I$(SRCDIR) -march=native -O3 -mmmx -msse -msse2 -msse3 -mssse3 \
	-msse4 -msse4.1 -msse4.2 -ffast-math \
	-fno-pic -mfpmath=sse -mrecip -fomit-frame-pointer -ftree-vectorize -pipe \
	-fexpensive-optimizations -funsafe-loop-optimizations -fno-finite-math-only \
	-fno-exceptions -DRT_NO_EXCEPTIONS -DRT_MULTITHREADED -fopenmp -DHAVE_SSE2 -DMEXP=19937 \
	-fgcse-sm -fgcse-las

MAC_CXXFLAGS= -I$(SRCDIR) -march=prescott -O3 -mmmx -msse -msse2 -ffast-math \
	-mfpmath=sse -fomit-frame-pointer -ftree-vectorize -pipe -fno-finite-math-only \
	-fexpensive-optimizations -funsafe-math-optimizations -funsafe-loop-optimizations \
	-fno-exceptions -DRT_NO_EXCEPTIONS -DRT_MULTITHREADED -fopenmp -DHAVE_SSE2 -DMEXP=19937 \
	-fgcse-sm -fgcse-las

#CXX=/opt/local/bin/g++-mp-4.5
#CC=/opt/local/bin/gcc-mp-4.5

LDFLAGS= -fwhole-program

#CXXFLAGS = $(MAC_CXXFLAGS)
CXXFLAGS = $(PC_CXXFLAGS)

CXXFLAGS += $(SDL_CXXFLAGS) $(WIN_CXXFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(WIN_LDFlAGS)

SRC = $(shell find $(SRCDIR) -name '*.cpp')
OBJS=$(SRC:.cpp=.o)
OBJS+=$(SRCDIR)/scene/parser.o $(SRCDIR)/scene/scanner.o

all : parser rt

parser: $(SRCDIR)/scene/parser.o $(SRCDIR)/scene/scanner.o

$(SRCDIR)/scene/sceneparser.tab.h : $(SRCDIR)/scene/sceneparser.tab.c

$(SRCDIR)/scene/sceneparser.tab.c : $(SRCDIR)/scene/sceneparser.y
	bison $(SRCDIR)/scene/sceneparser.y
	mv *.hh $(SRCDIR)/scene/
	mv sceneparser.tab.* $(SRCDIR)/scene/

$(SRCDIR)/scene/parser.o : $(SRCDIR)/scene/sceneparser.tab.c $(SRCDIR)/scene/sceneparser.tab.h
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $(SRCDIR)/scene/parser.o $(SRCDIR)/scene/sceneparser.tab.c 

$(SRCDIR)/scene/lex.yy.cc : $(SRCDIR)/scene/sceneparser.y $(SRCDIR)/scene/sceneparser.l $(SRCDIR)/scene/scanner.hpp
	flex $(SRCDIR)/scene/sceneparser.l
	mv lex.yy.cc $(SRCDIR)/scene/

$(SRCDIR)/scene/scanner.o : $(SRCDIR)/scene/lex.yy.cc
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c -o $(SRCDIR)/scene/scanner.o $(SRCDIR)/scene/lex.yy.cc

rt: $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

clean:
	-rm $(shell find $(SRCDIR) -name '*.o')
	-rm rt

parserclean:
	-rm $(SRCDIR)/scene/*.hh
	-rm $(SRCDIR)/scene/lex.yy.cc $(SRCDIR)/scene/sceneparser.tab.*

run: rt
	./rt
