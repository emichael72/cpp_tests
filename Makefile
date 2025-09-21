# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++20 -g -Iinc

# Source and output directories
SRC_DIR := src
BIN_DIR := bin

# Detect OS: Windows_NT or (default) Linux/Unix
ifeq ($(OS),Windows_NT)
  EXE_EXT := .exe
  RM := rmdir /s /q
  NULL := >nul 2>&1
  MKDIR := if not exist $(BIN_DIR) mkdir $(BIN_DIR)
else
  EXE_EXT :=
  RM := rm -rf
  NULL := >/dev/null 2>&1
  MKDIR := mkdir -p $(BIN_DIR)
endif

# Collect all .cc files from src/
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(SRCS))

# Target executable
TARGET := $(BIN_DIR)/main$(EXE_EXT)

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
	$(MKDIR)

# Clean build artifacts
clean:
	-$(RM) $(BIN_DIR) $(NULL)

.PHONY: all clean
