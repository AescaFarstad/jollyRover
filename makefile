EMSCRIPTEN ?= ../../emsdk/emscripten/1.38.24
WEB_COMPILER := $(EMSCRIPTEN)/em++
LOCAL_COMPILER := $(CXX)

OUT := out
WEB_TARGET := $(OUT)/web
LOCAL_TARGET := $(OUT)/local
SERVER_TARGET := $(OUT)/server

AUTODEPS = -MMD -MF $(subst .bc,.d,$@)
COMPILE_FLAGS := -g -Wall -c -std=c++14 -D_REENTRANT $(CXXFLAGS) 
LINK_FLAGS := -std=c++14  $(LDFLAGS)

LOCAL_LIBS := -lSDL2_gpu -lSDL2 -lSDL2_net -lSDL2_image -lutil
SERVER_LIBS := -lSDL2 -lSDL2_net -lutil

DIR_CLIENT =client
DIR_CLIENT +=client/view

DIR_SERVER =server
DIR_SERVER +=server/network
DIR_SERVER +=server/network/clients
DIR_SERVER +=server/thirdParty
DIR_SERVER +=server/thirdParty/libwshandshake

DIR_SHARED +=shared
DIR_SHARED +=shared/input
DIR_SHARED +=shared/model
DIR_SHARED +=shared/model/entity
DIR_SHARED +=shared/model/logic
DIR_SHARED +=shared/model/prototypes
DIR_SHARED +=shared/network
DIR_SHARED +=shared/network/bindings
DIR_SHARED +=shared/network/messages
DIR_SHARED +=shared/thirdParty
DIR_SHARED +=shared/utils
DIR_SHARED +=shared/model/ref


INC_PARAMS_CLIENT=$(foreach d, $(DIR_CLIENT), -I$d) $(foreach d, $(DIR_SHARED), -I$d) -I/usr/include/SDL2/ -I/usr/local/include/SDL_gpu
INC_PARAMS_SERVER=$(foreach d, $(DIR_SERVER), -I$d) $(foreach d, $(DIR_SHARED), -I$d)

SOURCES_CLIENT = $(foreach d, $(DIR_CLIENT), $(wildcard $d/*.cpp)) $(foreach d, $(DIR_SHARED), $(wildcard $d/*.cpp))
SOURCES_SERVER = $(foreach d, $(DIR_SERVER), $(wildcard $d/*.cpp)) $(foreach d, $(DIR_SHARED), $(wildcard $d/*.cpp))

OBJS_CLIENT=$(subst .cpp,.bc,$(SOURCES_CLIENT))
OBJS_SERVER=$(subst .cpp,.bc,$(SOURCES_SERVER))

OBJDIR_CLIENT_LOCAL=bc/local
OBJDIR_CLIENT_WEB=bc/web
OBJDIR_CLIENT_SERVER=bc/server

SUBOBJ_LOCAL=$(foreach d, $(OBJS_CLIENT), $(OBJDIR_CLIENT_LOCAL)/$d)
SUBOBJ_WEB=$(foreach d, $(OBJS_CLIENT), $(OBJDIR_CLIENT_WEB)/$d)
SUBOBJ_SERVER=$(foreach d, $(OBJS_SERVER), $(OBJDIR_CLIENT_SERVER)/$d)


local: $(SUBOBJ_LOCAL)
	$(LOCAL_COMPILER) $(LINK_FLAGS) -o $(LOCAL_TARGET) $(SUBOBJ_LOCAL) $(LOCAL_LIBS)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets
	if which spd-say; then spd-say 'i' --volume -92; fi
	
web: $(SUBOBJ_WEB)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets/
	$(WEB_COMPILER) \
		-O2 -s USE_SDL=2 -s USE_SDL_NET=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3 -s USE_WEBGL2=1 \
		-s WASM=0 -s TRACE_WEBGL_CALLS=1 -s TOTAL_MEMORY=67108864 -s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=0 -s ASSERTIONS=1 -s SAFE_HEAP=1 \
		--use-preload-plugins -v -o $(WEB_TARGET).html lib/SDL_gpu.bc $(SUBOBJ_WEB) \
		--embed-file out/prototypes.json --embed-file out/config.json --preload-file out/assets --embed-file out/assets/sheet_tanks.png
	if which spd-say; then spd-say 'i' --volume -92; fi
	
server: $(SUBOBJ_SERVER)
	$(LOCAL_COMPILER) $(LINK_FLAGS) -o $(SERVER_TARGET) $(SUBOBJ_SERVER) $(SERVER_LIBS)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets
	if which spd-say; then spd-say 'i' --volume -92; fi
	
all: local server web

$(OBJDIR_CLIENT_LOCAL)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_CLIENT) $(LOCAL_FLAGS) -c -o $@ $<
	
$(OBJDIR_CLIENT_WEB)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(WEB_COMPILER) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_CLIENT) -s USE_SDL=2 -s USE_SDL_NET=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3 -c -o $@ $<
	
$(OBJDIR_CLIENT_SERVER)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_SERVER) $(LOCAL_FLAGS) -c -o $@ $<

-include $(SUBOBJ_LOCAL:.bc=.d)
-include $(SUBOBJ_WEB:.bc=.d)
-include $(SUBOBJ_SERVER:.bc=.d)
	
clean: cleanlc cleanServer cleanWeb
	
cleanlc:
	rm -rf $(OBJDIR_CLIENT_LOCAL)/
	rm -f $(LOCAL_TARGET)	

cleanServer:
	rm -rf $(OBJDIR_CLIENT_SERVER)/
	rm -f $(SERVER_TARGET)
	
cleanWeb:
	rm -rf $(OBJDIR_CLIENT_WEB)/
	rm -f $(WEB_TARGET).html
	rm -f $(WEB_TARGET).html.map
	rm -f $(WEB_TARGET).js
	rm -f $(WEB_TARGET).wasm
	
