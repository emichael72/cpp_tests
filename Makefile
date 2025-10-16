# Compiler and flags
CXX := clang++
CXX_STD := 20
CXXFLAGS := -std=c++$(CXX_STD) -fpermissive -ftemplate-backtrace-limit=0 -O0 -g3 -Iinc
CXXFLAGS += -fdiagnostics-absolute-paths 

# Default build target - can be overridden
BUILD_TARGET ?= BUILD_ALL_TESTS

# Add the build target flag
CXXFLAGS += -D$(BUILD_TARGET)


# CXXFLAGS += -DBUILD_ALL_TESTS
# CXXFLAGS += -DBUILD_BUFFER_TESTS
# CXXFLAGS += -DBUILD_TEMPLATES_TESTS
# CXXFLAGS += -DBUILD_ADVANCE_TESTS

# Source and output directories
SRC_DIR := src
BIN_DIR := bin
BIN_NAME := cpp_tests

# Detect OS: Windows_NT or (default) Linux/Unix
ifeq ($(OS),Windows_NT)
  EXE_EXT := .exe
  RM_DIR := rmdir /s /q
  RM := del /f /q
  NULL := >nul 2>&1
  MKDIR := if not exist $(BIN_DIR) mkdir $(BIN_DIR)
  MKDIR_VSCODE := if not exist .vscode mkdir .vscode
else
  EXE_EXT :=
  RM_DIR := rm -rf
  RM := rm -f
  NULL := >/dev/null 2>&1
  MKDIR := mkdir -p $(BIN_DIR) $(NULL)
  MKDIR_VSCODE := mkdir -p .vscode $(NULL)
endif

# Collect all .cc files from src/
SRCS := $(wildcard $(SRC_DIR)/*.cc)
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(SRCS))

# Target executable
TARGET := $(BIN_DIR)/$(BIN_NAME)$(EXE_EXT)

# Default rule - now generates compile_commands.json
all: clean compile-commands

# Internal build target (used by bear)
build-only: $(TARGET)

# Link step
$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "Linking $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Compile step for each .cc
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc | $(BIN_DIR) prebuild
	@echo "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
	@-$(MKDIR)

# Ensure .vscode directory exists
.vscode:
	@-$(MKDIR_VSCODE)

.PHONY: all clean run postbuild prebuild build-only compile-commands

# Pre-build hook (runs once before any compilation)
prebuild:
	@echo "Compiling with C++ $(CXX_STD)"

# Generate compile_commands.json using bear
compile-commands: clean | .vscode
	@echo
	@echo "Generating compile_commands.json with bear..."
	@bear --output .vscode/compile_commands.json -- $(MAKE) --no-print-directory build-only
	@echo "Done."
	@echo

# Clean build artifacts
clean:
	@-$(RM_DIR) $(BIN_DIR) $(NULL)
	@-$(RM) .vscode/compile_commands.json $(NULL)

# Run target: clean, build, run executable
ARGS ?=

run: all
	@clear
	@echo "Running $(TARGET) $(if $(ARGS),$(ARGS),-a)..."
	@$(TARGET) $(if $(ARGS),$(ARGS),-a)

