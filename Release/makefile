.PHONY: all clean run

CC=g++
SRCDIR=../src
OBJDIR=obj

CFLAGS=

CFLAGS += -O3 -msse -msse2 -msse3

CXXFLAGS=
LDFLAGS=-lSDL

OBJS=$(patsubst $(SRCDIR)/%.cpp, obj/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: rt

rt: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $? -o $@

clean:
	-rm -f $(OBJDIR)/*.o
	-rm rt

run: rt
	./rt
