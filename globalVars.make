CXX := gcc

SRC := src
INCLUDE := include
BIN := bin
BUILD := build
ANDROID := android
DEPENDS_FILE := depends

LIBRARY_NAME := driving-engine

LIB_EXT := $(if $(filter-out $(OS),Windows_NT),so,dll)
EXEC_EXT = $(if $(filter-out $(OS),Windows_NT),,.exe)
ADD_DEPEND_PATH := $(if $(filter-out $(OS),Windows_NT),../tools/addDependPath,..\\tools\\addDependPath)

SRCS := $(SRC)/engine.cpp $(SRC)/Vehicle.cpp