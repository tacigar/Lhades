CXX     = g++
CFLAGS  = -std=c++1y
TARGET  = lhades
SRCDIR  = src
SRCS    = $(wildcard $(SRCDIR)/*.cpp)
OBJS    = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -I$(LUA_INCDIR) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(LUA_INCDIR) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

install:
	cp lhades $(INST_BINDIR)
