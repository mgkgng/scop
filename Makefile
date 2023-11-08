# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := include
GLAD_DIR := glad

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -I$(INCLUDE_DIR) -I$(GLAD_DIR) -std=c++17 -Wall -Wextra -Werror

# Libraries
LIBS := -lglfw -lGL -ldl

# Source files
SRC := $(wildcard $(SRC_DIR)/*.cpp) $(GLAD_DIR)/glad.c

# Object files
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ := $(OBJ:$(GLAD_DIR)/%.c=$(OBJ_DIR)/%.o)

# Target
TARGET := $(BIN_DIR)/scop

# Compile and Link
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(GLAD_DIR)/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Clean
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

re: clean all

.PHONY: all clean re