# Compiler and flags
CXX := clang++
CXXFLAGS := -std=c++20 -O0 -g3 -Iinc

# Source and output directories
SRC_DIR := src
BIN_DIR := bin
BIN_NAME := cpp_tests

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
  MKDIR := mkdir -p $(BIN_DIR) $(NULL)
endif

# Collect all .cc files from src/
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(SRCS))

# Target executable
TARGET := $(BIN_DIR)/$(BIN_NAME)$(EXE_EXT)

# Default rule
all: clean $(TARGET)
ARGS ?=
# Link step
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@$(MAKE) -s postbuild

# Compile step for each .cc
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc | $(BIN_DIR)
	@echo "Compiling $(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
	@echo
	@-$(MKDIR)

# Clean build artifacts
clean:
	@-$(RM) $(BIN_DIR) $(NULL)

.PHONY: all clean run postbuild

# Post build
postbuild:
	@echo "Done."

# Run target: clean, build, run executable
ARGS ?=

run: all
	@clear
	@echo "Running $(TARGET) $(if $(ARGS),$(ARGS),-a)..."
	@$(TARGET) $(if $(ARGS),$(ARGS),-a)

