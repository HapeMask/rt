.PHONY: all clean run tests

CC=g++
SRCDIR=src
OBJDIR=obj

CFLAGS=

ifeq '$(RELEASE)' 'y'
	CFLAGS += -O3 -msse -msse2 -msse3
else
	CFLAGS += -Wall -g -DDEBUG
endif

CXXFLAGS=
LDFLAGS=-lSDL

OBJS=$(patsubst src/%.cpp, obj/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: rt

rt: $(OBJS)
	$(CC) $(patsubst obj/tests.o, , $^) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $? -o $@

clean:
	-rm -f $(OBJDIR)/*.o
	-rm rt
	-rm tests

run: rt
	./rt

tests: $(OBJS)
	$(CC) $(patsubst obj/main.o, , $^) $(LDFLAGS) -o $@
