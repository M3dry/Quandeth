# TARGET ?= bin/quandeth
# SRC_DIRS ?= ./src
# CFLAGS := -g -Wall -std=c++20
# LDFLAGS := -lm -lraylib

# SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
# OBJS := $(addsuffix .o,$(basename $(SRCS)))
# DEPS := $(OBJS:.o=.d)

# INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# $(TARGET): $(OBJS)
# 	g++ $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

# .PHONY: clean
# clean:
# 	$(RM) $(TARGET) $(OBJS) $(DEPS)

# -include $(DEPS)


CC=g++
CFLAGS=-g -Wall
LDFLAGS=-lm -lraylib
CLIBS=
OBJ=obj
SRC=src
SRCS=$(wildcard $(SRC)/*.cpp)
OBJS=$(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
BINDIR=bin
BIN=quandeth

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CLIBS) $(OBJS) -o $(BINDIR)/$@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ)/* $(BINDIR)/$(BIN)
