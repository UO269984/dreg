ROOT_PATH := ..
include $(ROOT_PATH)/globalVars.make

override CFLAGS += -Wall -std=c++17 -c -I $(INCLUDE) -o $@
override CFLAGS += $(if $(filter-out $(OS),Windows_NT),-fPIC,)
LDFLAGS := -lstdc++

include srcs.make
OBJS := $(subst .cpp,.o, $(SRCS))
OBJS_RELEASE := $(subst $(SRC),$(BIN_RELEASE), $(OBJS))
OBJS_DEBUG := $(subst $(SRC),$(BIN_DEBUG), $(OBJS))
WIN_OBJS_RELEASE := $(subst $(SRC),$(WIN_BIN_RELEASE), $(OBJS))
WIN_OBJS_DEBUG := $(subst $(SRC),$(WIN_BIN_DEBUG), $(OBJS))

BIN_FOLDERS := $(BIN_RELEASE) $(BIN_DEBUG) $(WIN_BIN_RELEASE) $(WIN_BIN_DEBUG)


.PHONY: debug
debug: $(BUILD)/$(DREG_LIBRARY_DEBUG)

.PHONY: release
release: $(BUILD)/$(DREG_LIBRARY_RELEASE)

.PHONY: static
static: $(BUILD)/$(DREG_LIBRARY_STATIC)

.PHONY: all
all: debug release static win-debug win-release win-static wasm android


$(BUILD)/$(DREG_LIBRARY_RELEASE): $(OBJS_RELEASE)
	$(CXX) -shared $(RELEASE_OPTIMIZE) -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_DEBUG): $(OBJS_DEBUG)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_STATIC): $(OBJS_RELEASE)
	ar rcs $@ $^

include $(ROOT_PATH)/addDepends.make

$(BIN_RELEASE)/%.o: $(SRC)/%.cpp
	$(CXX) $(RELEASE_OPTIMIZE) $(CFLAGS) $<

$(BIN_DEBUG)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<


ifneq ($(OS),Windows_NT)
.PHONY: win-debug
win-debug: $(BUILD)/$(DREG_LIBRARY_WIN_DEBUG)

.PHONY: win-release
win-release: $(BUILD)/$(DREG_LIBRARY_WIN_RELEASE)

.PHONY: win-static
win-static: $(BUILD)/$(DREG_LIBRARY_WIN_STATIC)


$(BUILD)/$(DREG_LIBRARY_WIN_RELEASE): $(WIN_OBJS_RELEASE)
	x86_64-w64-mingw32-gcc $(RELEASE_OPTIMIZE) -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_WIN_DEBUG): $(WIN_OBJS_DEBUG)
	x86_64-w64-mingw32-gcc -shared -o $@ $^ $(LDFLAGS)

$(BUILD)/$(DREG_LIBRARY_WIN_STATIC): $(WIN_OBJS_RELEASE)
	x86_64-w64-mingw32-ar rcs $@ $^

$(WIN_BIN_RELEASE)/%.o: $(SRC)/%.cpp
	x86_64-w64-mingw32-gcc $(RELEASE_OPTIMIZE) $(CFLAGS) $<

$(WIN_BIN_DEBUG)/%.o: $(SRC)/%.cpp
	x86_64-w64-mingw32-gcc $(CFLAGS) $<
endif


.PHONY: wasm
wasm:
	emcc -I $(INCLUDE) $(RELEASE_OPTIMIZE) -o $(BUILD)/$(DREG_LIBRARY_WASM) \
	-std=c++17 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' $(SRCS)

.PHONY: android
android:
	ndk-build -C $(ANDROID)
	cp $(ANDROID)/libs/armeabi-v7a/$(DREG_LIBRARY_ANDROID) $(BUILD)

.PHONY: clean
clean:
	rm -f $(BIN_RELEASE)/* $(BIN_DEBUG)/*
	rm -f $(WIN_BIN_RELEASE)/* $(WIN_BIN_DEBUG)/*
	rm -rf $(ANDROID)/libs $(ANDROID)/obj
	rm -f $(BUILD)/*
	rm -f $(DEPENDS_FILE)