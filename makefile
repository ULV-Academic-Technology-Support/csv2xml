# Commands
CC  := gcc
CXX := g++
LD  := g++
RM  := rm

# Files
OBJECTS := parg.o tinyxml2.o main.o convert.o

# Options
C_FLAGS   := -g -Wall
CXX_FLAGS := $(C_FLAGS) -std=c++11

EXE := csv2xml

# Targets
all: $(OBJECTS)
	$(LD) $(OBJECTS) -o $(EXE)

%.o: %.c
	$(CC) $(C_FLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	-$(RM) *.o