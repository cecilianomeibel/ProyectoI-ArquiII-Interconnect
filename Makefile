# Makefile

CXX = g++
CXXFLAGS = -Wall -std=c++20 -Iinterconnect

TARGET = main
OBJDIR = bin
MAIN = main.cpp
MAIN_OBJ = $(OBJDIR)/main.o

INTERCONNECT_OBJS = $(OBJDIR)/interconnect.o \
                    $(OBJDIR)/FIFO.o \
                    $(OBJDIR)/Priority.o

all: $(OBJDIR) $(INTERCONNECT_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJ) $(INTERCONNECT_OBJS)

$(MAIN_OBJ): $(MAIN)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(INTERCONNECT_OBJS):
	$(MAKE) -C interconnect

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o 
	$(MAKE) -C interconnect clean
