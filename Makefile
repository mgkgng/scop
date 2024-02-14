# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := include

# Compiler
CXX := g++

# Compiler flags
CXXFLAGS := -I$(INCLUDE_DIR) -std=c++17 -Wall -Wextra -Werror #-fsanitize=address

# Libraries
LIBS := -L./lib -lGLEW -lglfw 
LIBS_LINUX := -lGLEW -lglfw -lGL

OPENGL := -framework OpenGL

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET := $(BIN_DIR)/scop

# Compile and Link
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS_LINUX)

mac: $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LIBS) $(OPENGL) -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# Clean
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

re: clean all

.PHONY: all clean re