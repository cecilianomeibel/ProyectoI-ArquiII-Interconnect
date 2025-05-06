# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++20 -Wall -Wextra

# Directorios
INCLUDES = -I.

# Archivos fuente
SRCS = main.cpp interconnect/FIFO.cpp interconnect/Priority.cpp

# Archivos objeto
OBJS = $(SRCS:.cpp=.o)

# Ejecutable
TARGET = main

# Regla principal
all: $(TARGET)

# Compilación del ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Cómo compilar .cpp en .o con inclusión de headers
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)
