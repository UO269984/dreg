CXX := gcc

SRC := src
INCLUDE := include
BIN := bin
WIN_BIN := bin-win
BUILD := build
ANDROID := android
DEPENDS_FILE := depends

LIB_EXT := $(if $(filter-out $(OS),Windows_NT),so,dll)
STATIC_LIB_EXT := $(if $(filter-out $(OS),Windows_NT),a,lib)
EXEC_EXT = $(if $(filter-out $(OS),Windows_NT),,.exe)

LIBRARY_NAME := dreg
DREG_LIBRARY := $(LIBRARY_NAME).$(LIB_EXT)
DREG_LIBRARY_STATIC := $(LIBRARY_NAME).$(STATIC_LIB_EXT)
DREG_LIBRARY_WIN := $(LIBRARY_NAME).dll
DREG_LIBRARY_WIN_STATIC := $(LIBRARY_NAME).lib
DREG_LIBRARY_WASM := $(LIBRARY_NAME).bc
DREG_LIBRARY_ANDROID := lib$(LIBRARY_NAME).so