ROOT_PATH := ..
include $(ROOT_PATH)/globalVars.make

TARGET := test$(EXEC_EXT)

override CFLAGS += -Wall -std=$(CPP_STD) -c -I $(INCLUDE) -o $@
LDFLAGS := $(DREG_LIBRARY_DEBUG) -lstdc++

ifneq ($(OS),Windows_NT)
	LDFLAGS := -Wl,-rpath=./ $(LDFLAGS)
endif

SRCS := $(SRC)/main.cpp
OBJS := $(subst .cpp,.o, $(SRCS))
OBJS := $(subst $(SRC),$(BIN), $(OBJS))
BIN_FOLDERS := $(BIN)

.PHONY: all
all: $(DREG_LIBRARY_DEBUG) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

include $(ROOT_PATH)/addDepends.make

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: $(DREG_LIBRARY_DEBUG)
$(DREG_LIBRARY_DEBUG):
	@$(MAKE) --no-print-directory -C ../engine debug

.PHONY: clean
clean:
	rm -f $(BIN)/*.o
	rm -f $(TARGET)
	rm -f $(DEPENDS_FILE)
	@$(MAKE) --no-print-directory -C ../engine clean