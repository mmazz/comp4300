CC = gcc
CXX = g++
SFML_DIR := .

CFLAGS= -C -Wall
CXXFLAGS = -g -O3 -Wall -Wextra -std=c++20
CXXFLAGS += -Ivendor/glad/include
LDLIBS = -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL -L$(SFML_DIR)/lib

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

