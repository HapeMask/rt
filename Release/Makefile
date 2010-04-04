.PHONY: all clean run compile link

CC=g++

SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

CFLAGS= -I../ -O3 -msse -msse2 -msse3
CXXFLAGS=
LDFLAGS=

CFLAGS += $(SDL_CFLAGS)
LDFLAGS += $(SDL_LDFLAGS)

SRC=../src
DIRS=$(SRC)/acceleration $(SRC)/camera $(SRC)/color $(SRC)/framebuffer \
$(SRC)/geometry $(SRC)/mathlib $(SRC)/materials $(SRC)/tracer $(SRC)/scene $(SRC)

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
