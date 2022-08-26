include ../globalVars.make

LIBRARY_FILENAME := $(LIBRARY_NAME).$(LIB_EXT)
TARGET := test$(EXEC_EXT)

CFLAGS = -Wall -std=c++17 -c -I $(INCLUDE) -o $@
LDFLAGS := $(LIBRARY_FILENAME) -lstdc++

ifneq ($(OS),Windows_NT)
	LDFLAGS := -Wl,-rpath=./ $(LDFLAGS)
endif

SRCS := $(SRC)/main.cpp
OBJS := $(subst $(SRC),$(BIN), $(SRCS))
OBJS := $(subst .cpp,.o, $(OBJS))

.PHONY: all
all: $(LIBRARY_FILENAME) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(shell $(CXX) -c -I $(INCLUDE) -MM $(SRCS) | $(ADD_DEPEND_PATH) $(BIN)/ > $(DEPENDS_FILE))
include $(DEPENDS_FILE)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: $(LIBRARY_FILENAME)
$(LIBRARY_FILENAME):
	@$(MAKE) --no-print-directory -C ../engine

.PHONY: clean
clean:
	rm -f $(BIN)/*.o
	rm -f $(TARGET)
	rm -f $(DEPENDS_FILE)
	@$(MAKE) --no-print-directory -C ../engine clean