CFLAGS = -c -I include -o $@
LDFLAGS := -lstdc++

CFLAGS += $(if $(filter-out $(OS),Windows_NT),-fPIC,)

include ../globalVars.make
LIBRARY_FILENAME := $(BUILD)/$(LIBRARY_NAME).$(LIB_EXT)

include srcs.make
OBJS := $(subst $(SRC),$(BIN), $(SRCS))
OBJS := $(subst .cpp,.o, $(OBJS))

$(LIBRARY_FILENAME): $(OBJS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

$(shell $(CXX) -c -I $(INCLUDE) -MM $(SRCS) | $(ADD_DEPEND_PATH) $(BIN)/ > $(DEPENDS_FILE))
include $(DEPENDS_FILE)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS) $<

.PHONY: all
all: engine windows webgl android

.PHONY: engine
engine: $(LIBRARY_FILENAME)

.PHONY: windows
windows:
	x86_64-w64-mingw32-gcc -shared -I include -o $(BUILD)/$(LIBRARY_NAME).dll $(SRCS) $(LDFLAGS)

.PHONY: webgl
webgl:
	emcc -I $(INCLUDE) -o $(BUILD)/$(LIBRARY_NAME).bc -std=c++11 -s WASM=1 -s EXPORTED_RUNTIME_METHODS='["cwrap"]' $(SRCS)

.PHONY: android
android:
	ndk-build -C $(ANDROID)
	cp $(ANDROID)/libs/armeabi-v7a/lib$(LIBRARY_NAME).so $(BUILD)

.PHONY: clean
clean:
	rm -f $(BIN)/*
	rm -f $(BUILD)/*
	rm -rf $(ANDROID)/libs
	rm -rf $(ANDROID)/obj
	rm -f $(DEPENDS_FILE)