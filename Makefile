CC = gcc
CXX = g++

CFLAGS = -C -Wall
CXXFLAGS = -g -O3 -Wall -Wextra -std=c++20
LDLIBS = -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL  -limgui


OBJDIR = include/obj
SRC = $(wildcard include/*.cpp)
OBJ = $(patsubst include/%.cpp, $(OBJDIR)/%.o, $(SRC))

APP = app
all: dir $(OBJ)


$(OBJDIR)/%.o: include/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dir:
	mkdir -p $(OBJDIR)

.PHONY: clean run

clean:
	find . -type f | xargs touch
	rm -rf obj/*
	rm ./$(APP)

