CC := g++
CXXFLAGS := -std=c++17 -Ofast

SRC_DIR := src
OBJ_DIR := obj
SRC := $(wildcard $(SRC_DIR)/*.cc)
OBJ := $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)
EXE := main

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(EXE) $(OBJ_DIR)
