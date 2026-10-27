CXX = g++
CPPFLAGS += -Iinclude -Iinclude/models -Iinclude/manager
CXXFLAGS ?= -std=c++17 -Wall -Wextra

TARGET := qlnt.exe
SOURCES := $(wildcard src/*.cpp src/manager/*.cpp src/models/*.cpp)
HEADERS := $(wildcard include/*.h include/manager/*.h include/models/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SOURCES) -o $@

ifeq ($(OS),Windows_NT)
clean:
	cmd /c if exist $(TARGET) del /Q $(TARGET)
else
clean:
	rm -f $(TARGET)
endif
