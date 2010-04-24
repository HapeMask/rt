.PHONY: all clean run compile link

CC=g++

SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib
CFLAGS= -I../ -O3 -msse -msse2 -msse3 -ffast-math -fno-pic -fno-stack-check
CXXFLAGS=
LDFLAGS= -lboost_regex

CFLAGS += $(SDL_CFLAGS) $(WIN_CFLAGS)
LDFLAGS += $(SDL_LDFLAGS) $(WIN_LDFlAGS)

SRC=../src
DIRS=$(SRC)/acceleration $(SRC)/camera $(SRC)/color $(SRC)/framebuffer \
$(SRC)/light $(SRC)/geometry $(SRC)/mathlib $(SRC)/scene $(SRC)/tracer \
$(SRC)/materials $(SRC)/samplers $(SRC)

OBJS=$(foreach DIR,$(DIRS), $(patsubst $(DIR)/%.cpp, ../obj/%.o, $(wildcard $(DIR)/*.cpp)))

all: compile rt

rt: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

compile:
	for DIR in $(DIRS); do \
		export CC='$(CC)'; \
		export CFLAGS='$(CFLAGS)'; \
		export CXXFLAGS='$(CXXFLAGS)'; \
		export LDFLAGS='$(LDFLAGS)'; \
		$(MAKE) -C $${DIR} all; \
		done;

clean:
	for DIR in $(DIRS); do \
		$(MAKE) -C $${DIR} clean; \
		done
	-rm ../obj/*.o
	-rm rt

run: rt
	./rt
