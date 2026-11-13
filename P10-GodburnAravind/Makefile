# Makefile for Can't Stop game
# Authors: Ryan Godburn & Arjun Aravind
# Date: 4/24/25
# Description: This makefile compiles the Can't Stop game source files

# Compiler and flags
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

# Source files
SOURCES = main.cpp \
          game.cpp \
          board.cpp \
          column.cpp \
          player.cpp \
          dice.cpp \
          clist.cpp \
          enums.cpp \
          tools.cpp \
          BadPlayer.cpp

# Object files derived from source files
OBJECTS = $(SOURCES:.cpp=.o)

# Final executable name
EXEC = CantStop

# Phony targets (targets that don't represent files)
.PHONY: all clean

# Default target: build the executable
all: $(EXEC)

# Link object files to create executable
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJECTS) $(EXEC)