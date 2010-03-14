.PHONY: all clean

CC=g++
OBJDIR=./obj

CFLAGS= -Wall -g -DDEBUG -I../

CXXFLAGS=
LDFLAGS=-lSDL

SRCDIR=./

OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $? -o $@
