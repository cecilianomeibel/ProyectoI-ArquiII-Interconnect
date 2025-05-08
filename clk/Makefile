# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++20 -Wall -Wextra

SRCS = clk.cpp shared.cpp 
OBJDIR = ../bin
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

all: $(OBJS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ -pthread

clean:
	rm -f $(OBJDIR)/*.o