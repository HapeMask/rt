.PHONY: all clean

SRCDIR=./
OBJDIR=../obj
CFLAGS += -I./

OBJS=$(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(wildcard $(SRCDIR)/*.cpp))

all: $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $? -o $@
