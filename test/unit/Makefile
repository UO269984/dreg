ROOT_PATH := ../..
include $(ROOT_PATH)/globalVars.make

CUTEST_LIBRARY := cuTestCPP.$(STATIC_LIB_EXT)
TARGET := testDreg$(EXEC_EXT)

override CFLAGS += -Wall -std=$(CPP_STD) -c -I $(INCLUDE) -o $@
LDFLAGS := $(DREG_LIBRARY_DEBUG) $(CUTEST_LIBRARY) -lstdc++

ifneq ($(OS),Windows_NT)
	LDFLAGS := -Wl,-rpath=./ $(LDFLAGS)
endif

include srcs.make
OBJS := $(subst .cpp,.o, $(SRCS))
OBJS := $(subst $(SRC),$(BIN), $(OBJS))
BIN_FOLDERS := $(BIN)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

include $(ROOT_PATH)/addDepends.make

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: test
test: $(TARGET)
	./$(TARGET)

.PHONY: clean
clean:
	rm -f $(BIN)/*.o
	rm -f $(TARGET)
	rm -f $(DEPENDS_FILE)