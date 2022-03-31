SRC := src
BIN := bin
DEPENDS_FILE := depends
ENGINE_INCLUDE := ../engine/include

LIBRARY_NAME := driving-engine
LIB_EXT := $(if $(filter-out $(OS),Windows_NT),so,dll)
LIBRARY_FILENAME := $(LIBRARY_NAME).$(LIB_EXT)

CXX := gcc
CFLAGS = -c -I $(ENGINE_INCLUDE) -o $@
LDFLAGS := $(LIBRARY_FILENAME) -lstdc++

SRCS := $(SRC)/main.cpp
OBJS := $(subst $(SRC),$(BIN), $(SRCS))
OBJS := $(subst .cpp,.o, $(OBJS))

.PHONY: all
all: $(LIBRARY_FILENAME) test
	
test: $(OBJS)
	$(CXX) -Wl,-rpath=./ -o $@ $^ $(LDFLAGS)

$(shell $(CXX) -c -I $(ENGINE_INCLUDE) -MM $(SRCS) | ../tools/addDependPath $(BIN)/ > $(DEPENDS_FILE))
include $(DEPENDS_FILE)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: $(LIBRARY_FILENAME)
$(LIBRARY_FILENAME):
	@$(MAKE) --no-print-directory -C ../engine

.PHONY: clean
clean:
	rm -f $(BIN)/*.o
	rm -f test
	rm -f $(DEPENDS_FILE)
	@$(MAKE) --no-print-directory -C ../engine clean