# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++20 -g -Iinc

# Source and output directories
SRC_DIR := src
BIN_DIR := bin

# Collect all .cc files from src/
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(SRCS))

# Target executable
TARGET := $(BIN_DIR)/main.exe

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Compile step for each .cc
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build artifacts
clean:
	@if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR) >nul 2>&1

.PHONY: all clean
