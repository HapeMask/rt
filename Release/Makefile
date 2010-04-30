.PHONY: all clean run compile link
SRCDIR=../src

SDL_CXXFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CXXFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib
CXXFLAGS= -I$(SRCDIR) -march=native -O3 -mmmx -msse -msse2 -msse3 -mssse3 -ffast-math \
	-fno-pic -mfpmath=sse -mrecip -fomit-frame-pointer -ftree-vectorize -pipe \
	-fexpensive-optimizations -funsafe-loop-optimizations -funsafe-math-optimizations \
	-fno-exceptions -DRT_NO_EXCEPTIONS -fopenmp
LDFLAGS= -lboost_regex -fwhole-program

CXXFLAGS += $(SDL_CXXFLAGS) $(WIN_CXXFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(WIN_LDFlAGS)

SRC = $(shell find ../src/ -name '*.cpp')
OBJS=$(SRC:.cpp=.o)

all: rt

deps: $(SRC)
	for f in $(SRC); do\
		cpp -MM -I../src/ $$f >> deps;\
	done

rt: $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

clean:
	for DIR in $(DIRS); do \
		$(MAKE) -C $${DIR} clean; \
		done
	-rm ../obj/*.o
	-rm rt

run: rt
	./rt

include deps
