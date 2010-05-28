.PHONY: all clean run compile link
SRCDIR=../src

SDL_CXXFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CXXFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib
CXXFLAGS= -g -I$(SRCDIR) -march=native -O3 -mmmx -msse -msse2 -msse3 -mssse3 -ffast-math \
	-fno-pic -mfpmath=sse -mrecip -fomit-frame-pointer -ftree-vectorize -pipe \
	-fexpensive-optimizations -funsafe-loop-optimizations -funsafe-math-optimizations \
	-fno-exceptions -DRT_NO_EXCEPTIONS -DRT_MULTITHREADED -fopenmp -DHAVE_SSE2 -DMEXP=19937
LDFLAGS= -lboost_regex -fwhole-program

CXXFLAGS += $(SDL_CXXFLAGS) $(WIN_CXXFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(WIN_LDFlAGS)

SRC = $(shell find $(SRCDIR) -name '*.cpp')
OBJS=$(SRC:.cpp=.o)

all: rt

#include  deps

rt: $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

deps:
	-rm deps;
	for f in $(SRC); do\
		cpp -MM $(CXXFLAGS) $$f >> deps;\
	done

clean:
	-rm $(shell find $(SRCDIR) -name '*.o')
	-rm deps
	-rm rt

run: rt
	./rt
