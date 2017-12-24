CXX     = g++
CFLAGS  = -std=c++1y
INCDIR  = -I$(HOME)/.luaver/lua/5.3.2/include
TARGET  = lhades
SRCDIR  = src
SRCS    = src/opcode.cpp src/lhades.cpp src/main.cpp
OBJS    = $(SRCS:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(INCDIR) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) $(INCDIR) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)
