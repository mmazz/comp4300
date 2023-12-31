CC = gcc
CXX = g++

CFLAGS= -C -Wall
CXXFLAGS = -g -Wall -Wextra -std=c++2a
CXXFLAGS += -Ivendor/glad/include
LDLIBS = -lsfml-graphics -lsfml-window -lsfml-system

OBJDIR = obj
BINDIR = bin
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(SRC))

APP = app
all: $(APP)

app: dir $(OBJ)
	$(CXX) $(CXXFLAGS)  $(LDLIBS) $(OBJ) -o $(BINDIR)/$(APP)

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dir:
	mkdir -p $(OBJDIR)

.PHONY: clean run

clean:
	find . -type f | xargs touch
	rm -rf obj/*
	rm ./$(APP)

