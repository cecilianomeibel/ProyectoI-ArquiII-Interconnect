# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++20 -Wall -Wextra

SRCS = interconnect.cpp Priority.cpp FIFO.cpp 
OBJDIR = ../bin
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(OBJS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o