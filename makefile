NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
MAKEFLAGS += -j$(NPROCS)

EMSCRIPTEN ?= ../../emsdk/upstream/emscripten
WEB_COMPILER := $(EMSCRIPTEN)/em++
LOCAL_COMPILER := $(CXX)

OUT := out
WEB_TARGET := $(OUT)/web
LOCAL_TARGET := $(OUT)/local
SERVER_TARGET_NAME := JRserver
SERVER_TARGET := $(OUT)/$(SERVER_TARGET_NAME)
CONFIG := $(OUT)/config.json
PROTOTYPES := $(OUT)/prototypes.json
SSH_CONFIG := JollyRover
REMOTE_DEPLOY_PATH := ~/JollyRover


AUTODEPS = -MMD -MF $(subst .bc,.d,$@)

SANITIZATION= -fsanitize=address
OPTIMIZATION_SERVER= -g -O0
OPTIMIZATION_LOCAL= -g -O0
OPTIMIZATION_WEB= -g -O0

DEBUG_MODE :=1

COMPILE_FLAGS := -Wall -c -std=c++17 -D_REENTRANT -DSDL_DISABLE_IMMINTRIN_H -DFC_USE_SDL_GPU $(CXXFLAGS)
LINK_FLAGS := -std=c++17 $(LDFLAGS)

LOCAL_LIBS := -lSDL2_gpu -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_ttf -lutil
SERVER_LIBS := -lSDL2 -lSDL2_net -lutil

DIR_CLIENT =client
DIR_CLIENT +=client/view
DIR_CLIENT +=client/input
DIR_CLIENT +=client/resources
DIR_CLIENT +=client/thirdParty/NFont
DIR_CLIENT +=client/thirdParty/SDL_FontCache

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
DIR_SHARED +=shared/thirdParty/hash
DIR_SHARED +=shared/utils
DIR_SHARED +=shared/utils/serialization
DIR_SHARED +=shared/model/ref


INC_PARAMS_CLIENT=$(foreach d, $(DIR_CLIENT), -I$d) $(foreach d, $(DIR_SHARED), -I$d) -I/usr/include/SDL2/ -I/usr/local/include/SDL_gpu
INC_PARAMS_SERVER=$(foreach d, $(DIR_SERVER), -I$d) $(foreach d, $(DIR_SHARED), -I$d) -I/usr/include/SDL2/ -I/usr/local/include/SDL_gpu

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


SDL_FontCachePATH = client/thirdParty/SDL_FontCache/SDL_FontCache
SDL_FontCache_LOCAL_OBJECT = $(OBJDIR_CLIENT_LOCAL)/$(SDL_FontCachePATH).bc
SDL_FontCache_WEB_OBJECT = $(OBJDIR_CLIENT_WEB)/$(SDL_FontCachePATH).bc

local: $(SUBOBJ_LOCAL) $(SDL_FontCache_LOCAL_OBJECT)
	$(LOCAL_COMPILER) $(OPTIMIZATION_LOCAL) $(SANITIZATION) $(LINK_FLAGS) -o $(LOCAL_TARGET) $(SUBOBJ_LOCAL) $(SDL_FontCache_LOCAL_OBJECT) $(LOCAL_LIBS)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets
	if which spd-say; then spd-say 'i' --volume -92; fi
	
web: $(SUBOBJ_WEB) $(SDL_FontCache_WEB_OBJECT)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets/
	rsync -r web/ out/
	$(WEB_COMPILER) \
		$(OPTIMIZATION_WEB) -s USE_SDL=2 -s USE_SDL_NET=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL_TTF=2\
		-s WASM=1 -s TOTAL_MEMORY=268435456 -s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=1 -s SAFE_HEAP=1 \
		--use-preload-plugins -v -o $(WEB_TARGET).html \
		lib/renderer_GLES_2.o lib/SDL_gpu_matrix.o lib/SDL_gpu_renderer.o lib/SDL_gpu_shapes.o lib/SDL_gpu.o lib/stb_image_write.o lib/stb_image.o \
		$(SUBOBJ_WEB) $(SDL_FontCache_WEB_OBJECT) \
		--embed-file out/prototypes.json --embed-file out/config.json --preload-file out/assets \
		--memory-init-file 1 --shell-file out/index.html -lidbfs.js -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" 
	sed -i 's/antialias\:false/antialias\:true/g' $(WEB_TARGET).js
	if which spd-say; then spd-say 'i' --volume -92; fi
	
server: $(SUBOBJ_SERVER)
	$(LOCAL_COMPILER) $(OPTIMIZATION_SERVER) $(SANITIZATION) $(LINK_FLAGS) -o $(SERVER_TARGET) $(SUBOBJ_SERVER) $(SERVER_LIBS)
	cp shared/prototypes.json out/prototypes.json
	rsync -r assets/ out/assets
	if which spd-say; then spd-say 'i' --volume -92; fi
	
all: local server web

$(SDL_FontCache_LOCAL_OBJECT):
	@mkdir -p $(dir $@)
	gcc -c $(SDL_FontCachePATH).c -o $(SDL_FontCache_LOCAL_OBJECT) -DFC_USE_SDL_GPU -DSDL_DISABLE_IMMINTRIN_H -I/usr/include/SDL2/ -I/usr/local/include/SDL_gpu $(LOCAL_LIBS)
	
$(SDL_FontCache_WEB_OBJECT):
	@mkdir -p $(dir $@)
	$(EMSCRIPTEN)/emcc -c $(SDL_FontCachePATH).c -o $(SDL_FontCache_WEB_OBJECT) -DFC_USE_SDL_GPU -DSDL_DISABLE_IMMINTRIN_H -I/usr/include/SDL2/ -I/usr/local/include/SDL_gpu $(LOCAL_LIBS)

deploy:
	rsync -Pav $(SERVER_TARGET) $(CONFIG) $(PROTOTYPES) '$(SSH_CONFIG):$(REMOTE_DEPLOY_PATH)/$(OUT)'
	ssh $(SSH_CONFIG) pkill -x $(SERVER_TARGET_NAME) || true
	ssh $(SSH_CONFIG) 'cd $(REMOTE_DEPLOY_PATH); $(SERVER_TARGET)' & exit

$(OBJDIR_CLIENT_LOCAL)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(OPTIMIZATION_LOCAL) $(SANITIZATION) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_CLIENT) $(LOCAL_FLAGS) -DIS_SERVER=0 -DDEBUG_MODE=$(DEBUG_MODE) -c -o $@ $<
	
$(OBJDIR_CLIENT_WEB)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(WEB_COMPILER) $(OPTIMIZATION_WEB) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_CLIENT) -DIS_SERVER=0 -DDEBUG_MODE=$(DEBUG_MODE) \
	-s USE_SDL=2 -s USE_SDL_NET=2 -s USE_SDL_IMAGE=2 -s USE_GLFW=3  -c -o $@ $<
	
$(OBJDIR_CLIENT_SERVER)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(OPTIMIZATION_SERVER) $(SANITIZATION) $(COMPILE_FLAGS) $(AUTODEPS) $(INC_PARAMS_SERVER) $(LOCAL_FLAGS) -DIS_SERVER=1 -DDEBUG_MODE=$(DEBUG_MODE) -c -o $@ $<

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
	
