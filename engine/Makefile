CFLAGS = -Wall -std=c++17 -c -I include -o $@
LDFLAGS := -lstdc++

CFLAGS += $(if $(filter-out $(OS),Windows_NT),-fPIC,)

include ../globalVars.make
LIBRARY_FILENAME := $(BUILD)/$(LIBRARY_NAME).$(LIB_EXT)
STATIC_LIBRARY_FILENAME := $(BUILD)/$(LIBRARY_NAME).$(STATIC_LIB_EXT)

include srcs.make
OBJS := $(subst $(SRC),$(BIN), $(SRCS))
OBJS := $(subst .cpp,.o, $(OBJS))
WIN_OBJS := $(subst $(BIN),$(WIN_BIN), $(OBJS))

$(LIBRARY_FILENAME): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(STATIC_LIBRARY_FILENAME): $(OBJS)
	ar rcs $@ $^

$(shell $(CXX) -c -I $(INCLUDE) -MM $(SRCS) | $(ADD_DEPEND_PATH) $(BIN)/ > $(DEPENDS_FILE))
include $(DEPENDS_FILE)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: all
all: engine static windows static-win wasm android

.PHONY: engine
engine: $(LIBRARY_FILENAME)

.PHONY: static
static: $(STATIC_LIBRARY_FILENAME)

ifneq ($(OS),Windows_NT)

$(BUILD)/$(LIBRARY_NAME).dll: $(WIN_OBJS)
	x86_64-w64-mingw32-gcc -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(LIBRARY_NAME).lib: $(WIN_OBJS)
	x86_64-w64-mingw32-ar rcs $@ $^

$(WIN_BIN)/%.o: $(SRC)/%.cpp
	x86_64-w64-mingw32-gcc $(CFLAGS) $<

.PHONY: windows
windows: $(BUILD)/$(LIBRARY_NAME).dll

.PHONY: static-win
static-win: $(BUILD)/$(LIBRARY_NAME).lib
endif

.PHONY: wasm
wasm:
	emcc -I $(INCLUDE) -o $(BUILD)/$(LIBRARY_NAME).bc -std=c++17 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' $(SRCS)

.PHONY: android
android:
	ndk-build -C $(ANDROID)
	cp $(ANDROID)/libs/armeabi-v7a/lib$(LIBRARY_NAME).so $(BUILD)

.PHONY: clean
clean:
	rm -f $(BIN)/*
	rm -f $(WIN_BIN)/*
	rm -f $(BUILD)/*
	rm -rf $(ANDROID)/libs
	rm -rf $(ANDROID)/obj
	rm -f $(DEPENDS_FILE)