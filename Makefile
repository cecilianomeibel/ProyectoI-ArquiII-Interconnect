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

CLK_OBJS = $(OBJDIR)/shared.o \
           $(OBJDIR)/clk.o

PE_OBJ = $(OBJDIR)/PE.o

INTERPRETE_OBJ = $(OBJDIR)/interprete.o

all: $(OBJDIR) $(INTERCONNECT_OBJS) $(PE_OBJ) $(INTERPRETE_OBJ) $(CLK_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJ) $(INTERCONNECT_OBJS) $(PE_OBJ) $(CLK_OBJS) $(INTERPRETE_OBJ) -pthread

$(MAIN_OBJ): $(MAIN)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PE_OBJ): Cache.cpp instruction.hpp interconnect/interconnect.hpp clk/shared.hpp
	$(CXX) $(CXXFLAGS) -c PE.cpp -o $(PE_OBJ) -pthread

$(INTERPRETE_OBJ): instruction.hpp
	$(CXX) $(CXXFLAGS) -c interprete.cpp -o $(INTERPRETE_OBJ)

$(INTERCONNECT_OBJS):
	$(MAKE) -C interconnect

$(CLK_OBJS):
	$(MAKE) -C clk

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/*.o 
	$(MAKE) -C interconnect clean
