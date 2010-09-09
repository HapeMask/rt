.PHONY: all clean run compile link
SRCDIR=../src

QT_CXXFLAGS=-D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED \
	-I/usr/share/qt/mkspecs/linux-g++ -I/usr/include/QtCore \
	-I/usr/include/QtGui

QT_LDFLAGS= -lQtGui -lQtCore -lpthread --hash-style=gnu --as-needed

SDL_CXXFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs)

WIN_CXXFLAGS = -IC:\MinGW\include\SDL
WIN_LDFLAGS = -LC:\MinGW\lib
CXXFLAGS= -pg -O0 -Wall -g -DDEBUG -I$(SRCDIR) -msse
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
