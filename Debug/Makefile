.PHONY: all clean run compile link
SRCDIR=../src

SDL_CXXFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CXXFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib
CXXFLAGS= -O0 -Wall -g -DDEBUG -I$(SRCDIR) -DRT_OMP_THREADS=1 -fopenmp -msse
LDFLAGS= -lboost_regex

CXXFLAGS += $(SDL_CXXFLAGS) $(WIN_CXXFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(WIN_LDFlAGS)

SRC = $(shell find $(SRCDIR) -name '*.cpp')
OBJS=$(SRC:.cpp=.o)

all: rt

deps: $(SRC)
	-rm deps;
	for f in $(SRC); do\
		cpp -MM $(CXXFLAGS) $$f >> deps;\
	done

include deps

rt: $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

clean:
	-rm $(shell find $(SRCDIR) -name '*.o')
	-rm deps
	-rm rt

run: rt
	./rt
