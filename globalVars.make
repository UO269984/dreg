CXX := gcc

SRC := src
INCLUDE := include
BIN := bin
WIN_BIN := bin-win
BUILD := build
ANDROID := android
DEPENDS_FILE := depends

LIBRARY_NAME := dreg

LIB_EXT := $(if $(filter-out $(OS),Windows_NT),so,dll)
STATIC_LIB_EXT := $(if $(filter-out $(OS),Windows_NT),a,lib)
EXEC_EXT = $(if $(filter-out $(OS),Windows_NT),,.exe)
ADD_DEPEND_PATH := $(if $(filter-out $(OS),Windows_NT),../tools/addDependPath,..\\tools\\addDependPath)