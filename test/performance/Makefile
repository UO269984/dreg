ROOT_PATH := ../..
include $(ROOT_PATH)/globalVars.make

TARGET := testPerformance$(EXEC_EXT)

override CFLAGS += -Wall -std=c++17 -c -I $(INCLUDE) -o $@
LDFLAGS := $(DREG_LIBRARY_RELEASE) -lstdc++

ifneq ($(OS),Windows_NT)
	LDFLAGS := -Wl,-rpath=./ $(LDFLAGS)
endif

SRCS := $(SRC)/main.cpp $(SRC)/testPerformance.cpp
OBJS := $(subst .cpp,.o, $(SRCS))
OBJS := $(subst $(SRC),$(BIN), $(OBJS))
WIN_OBJS := $(subst $(BIN),$(WIN_BIN), $(OBJS))
BIN_FOLDERS := $(BIN) $(WIN_BIN)

$(TARGET): $(OBJS)
	$(CXX) $(RELEASE_OPTIMIZE) -o $@ $^ $(LDFLAGS)

include $(ROOT_PATH)/addDepends.make

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(RELEASE_OPTIMIZE) $(CFLAGS) $<

.PHONY: test
test: $(TARGET)
	./$(TARGET)

.PHONY: all
all: $(TARGET) windows wasm

.PHONY: windows
windows: $(WIN_TARGET)

ifneq ($(OS),Windows_NT)
WIN_TARGET := testPerformance.exe

.PHONY: windows
windows: $(WIN_TARGET)

$(WIN_TARGET): $(WIN_OBJS)
	x86_64-w64-mingw32-gcc $(RELEASE_OPTIMIZE) -o $@ $^ $(DREG_LIBRARY_WIN_RELEASE) -lstdc++

$(WIN_BIN)/%.o: $(SRC)/%.cpp
	x86_64-w64-mingw32-gcc $(RELEASE_OPTIMIZE) $(CFLAGS) $<
endif

.PHONY: wasm
wasm:
	emcc $(RELEASE_OPTIMIZE) -I include/ -o wasm/web/js/dreg/testPerformance.js \
	-std=c++17 -s WASM=1 -s EXPORT_ES6=1 -s MODULARIZE=1 \
	../../engine/build/$(DREG_LIBRARY_WASM) src/testPerformance.cpp --bind wasm/wasmApi.cpp

.PHONY: clean
clean:
	rm -f $(BIN)/*.o
	rm -f $(WIN_BIN)/*.o
	rm -f $(TARGET)
	rm -f $(WIN_TARGET)
	rm -f wasm/web/js/dreg/*
	rm -f $(DEPENDS_FILE)