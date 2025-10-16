# Compiler and flags
CXX := clang++
CXX_STD := 20
CXXFLAGS := -std=c++$(CXX_STD) -fpermissive -ftemplate-backtrace-limit=0 -O0 -g3 -Iinc
CXXFLAGS += -fdiagnostics-absolute-paths

# g++ -std=c++11 -fpermissive -ftemplate-backtrace-limit=0 primes.cpp

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
	@echo "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure bin directory exists
$(BIN_DIR):
	@echo
	@-$(MKDIR)

.PHONY: all clean run postbuild prebuild

# Default top-level target
all: clean $(TARGET)

# Pre-build hook (runs once before any compilation)
prebuild:
	@echo "Compiling with C++ $(CXX_STD)"

# Make every object depend on the prebuild step
$(OBJS): | prebuild

# Post-build hook (runs after link)
postbuild:
	@echo "Done."
	@echo

# Clean build artifacts
clean:
	@-$(RM) $(BIN_DIR) $(NULL)

# Run target: clean, build, run executable
ARGS ?=

run: all
	@clear
	@echo "Running $(TARGET) $(if $(ARGS),$(ARGS),-a)..."
	@$(TARGET) $(if $(ARGS),$(ARGS),-a)

