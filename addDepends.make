ifneq ($(MAKECMDGOALS),clean)

ADD_DEPEND_PATH := $(ROOT_PATH)/tools/addDependPath
ifeq ($(OS),Windows_NT)
	ADD_DEPEND_PATH := $(subst /,\\, $(ADD_DEPEND_PATH))
endif

$(shell $(CXX) -c -I $(INCLUDE) -MM $(SRCS) | $(ADD_DEPEND_PATH) $(BIN)/ > $(DEPENDS_FILE))
include $(DEPENDS_FILE)
endif