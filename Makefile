CXX = g++
CPPFLAGS += -Iinclude
CXXFLAGS ?= -std=c++17 -Wall -Wextra

SOURCES := $(wildcard src.cpp)
OBJECTS := $(patsubst src/%.cpp,build/%.o,$(SOURCES))

.PHONY: all clean

all: $(OBJECTS)

build:
	mkdir build

build/%.o: src/%.cpp | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

ifeq ($(OS),Windows_NT)
clean:
	if exist build\*.o del /Q build\*.o
else
clean:
	$(RM) $(OBJECTS)
endif
