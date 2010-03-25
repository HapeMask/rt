.PHONY: all clean run compile link

CC=g++

CFLAGS= -O0 -Wall -g -DDEBUG -I../ `sdl-config --cflags`

CXXFLAGS=
LDFLAGS=`sdl-config --libs`

SRC=../src
DIRS=$(SRC)/acceleration $(SRC)/camera $(SRC)/color $(SRC)/framebuffer $(SRC)/geometry $(SRC)/mathlib $(SRC)/scene $(SRC)

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
