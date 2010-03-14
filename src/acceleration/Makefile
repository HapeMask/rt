.PHONY: all clean

SRCDIR=./
OBJDIR=../../obj

all: $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $? -o $@
