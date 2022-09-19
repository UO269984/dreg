ROOT_PATH := ..
include $(ROOT_PATH)/globalVars.make

CFLAGS = -Wall -std=c++17 -c -I $(INCLUDE) -o $@
CFLAGS += $(if $(filter-out $(OS),Windows_NT),-fPIC,)
LDFLAGS := -lstdc++

include srcs.make
OBJS := $(subst $(SRC),$(BIN), $(SRCS))
OBJS := $(subst .cpp,.o, $(OBJS))
WIN_OBJS := $(subst $(BIN),$(WIN_BIN), $(OBJS))

$(BUILD)/$(DREG_LIBRARY): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_STATIC): $(OBJS)
	ar rcs $@ $^

include $(ROOT_PATH)/addDepends.make

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: all
all: engine static windows static-win wasm android

.PHONY: engine
engine: $(BUILD)/$(DREG_LIBRARY)

.PHONY: static
static: $(BUILD)/$(DREG_LIBRARY_STATIC)

ifneq ($(OS),Windows_NT)

$(BUILD)/$(DREG_LIBRARY_WIN): $(WIN_OBJS)
	x86_64-w64-mingw32-gcc -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_WIN_STATIC): $(WIN_OBJS)
	x86_64-w64-mingw32-ar rcs $@ $^

$(WIN_BIN)/%.o: $(SRC)/%.cpp
	x86_64-w64-mingw32-gcc $(CFLAGS) $<

.PHONY: windows
windows: $(BUILD)/$(DREG_LIBRARY_WIN)

.PHONY: static-win
static-win: $(BUILD)/$(DREG_LIBRARY_WIN_STATIC)
endif

.PHONY: wasm
wasm:
	emcc -I $(INCLUDE) -o $(BUILD)/$(DREG_LIBRARY_WASM) -std=c++17 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' $(SRCS)

.PHONY: android
android:
	ndk-build -C $(ANDROID)
	cp $(ANDROID)/libs/armeabi-v7a/$(DREG_LIBRARY_ANDROID) $(BUILD)

.PHONY: clean
clean:
	rm -f $(BIN)/*
	rm -f $(WIN_BIN)/*
	rm -f $(BUILD)/*
	rm -rf $(ANDROID)/libs
	rm -rf $(ANDROID)/obj
	rm -f $(DEPENDS_FILE)