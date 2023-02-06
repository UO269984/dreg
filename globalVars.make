CXX := gcc
CXX_WIN := x86_64-w64-mingw32-gcc-posix
CPP_STD := c++17
RELEASE_OPTIMIZE := -O3

RELEASE_DEFINE := DREG_RELEASE
DEBUG_DEFINE := DREG_DEBUG

SRC := src
INCLUDE := include
BIN := bin
BIN_RELEASE := $(BIN)/release
BIN_DEBUG := $(BIN)/debug

WIN_BIN := $(BIN)/win
WIN_BIN_RELEASE := $(BIN)/win-release
WIN_BIN_DEBUG := $(BIN)/win-debug
BUILD := build
ANDROID := android
DEPENDS_FILE := depends

LIB_EXT := $(if $(filter-out $(OS),Windows_NT),so,dll)
STATIC_LIB_EXT := $(if $(filter-out $(OS),Windows_NT),a,lib)
EXEC_EXT = $(if $(filter-out $(OS),Windows_NT),,.exe)

LIBRARY_NAME := dreg
DREG_LIBRARY_RELEASE := $(LIBRARY_NAME).$(LIB_EXT)
DREG_LIBRARY_DEBUG := $(LIBRARY_NAME)-debug.$(LIB_EXT)
DREG_LIBRARY_STATIC := $(LIBRARY_NAME).$(STATIC_LIB_EXT)
DREG_LIBRARY_WIN_RELEASE := $(LIBRARY_NAME).dll
DREG_LIBRARY_WIN_DEBUG := $(LIBRARY_NAME)-debug.dll
DREG_LIBRARY_WIN_STATIC := $(LIBRARY_NAME).lib
DREG_LIBRARY_WASM := $(LIBRARY_NAME).bc
DREG_LIBRARY_ANDROID := lib$(LIBRARY_NAME).so