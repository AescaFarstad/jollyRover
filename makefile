WEB_COMPILER =../emsdk/emscripten/1.38.24/em++
LOCAL_COMPILER = g++
OUT=out
WEB_TARGET=$(OUT)/web
LOCAL_TARGET=$(OUT)/local
SERVER_TARGET=$(OUT)/server
WEB_FLAGS= -s USE_SDL=2 -s USE_SDL_NET=2
LOCAL_FLAGS= 
CXXFLAGS = -g -Wall -c -std=c++14 -D_REENTRANT
LDFLAGS=-g -Wl,-rpath=/usr/lib/ 

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
DIR_SHARED +=shared/model/prototypes
DIR_SHARED +=shared/network
DIR_SHARED +=shared/network/bindings
DIR_SHARED +=shared/network/messages
DIR_SHARED +=shared/thirdParty
DIR_SHARED +=shared/utils

OBJDIR_CLIENT_LOCAL=bc/local
OBJDIR_CLIENT_WEB=bc/web
OBJDIR_CLIENT_SERVER=bc/server

INC_PARAMS=$(foreach d, $(DIR_CLIENT), -I$d) $(foreach d, $(DIR_SHARED), -I$d)

CXXFLAGS += $(INC_PARAMS)
CXXFLAGS += -MMD -MF $(subst .bc,.d,$@)

SOURCES_CLIENT = $(foreach d, $(DIR_CLIENT), $(wildcard $d/*.cpp)) $(foreach d, $(DIR_SHARED), $(wildcard $d/*.cpp))
SOURCES_SERVER = $(foreach d, $(DIR_SERVER), $(wildcard $d/*.cpp)) $(foreach d, $(DIR_SHARED), $(wildcard $d/*.cpp))

OBJS_CLIENT=$(subst .cpp,.bc,$(SOURCES_CLIENT))
OBJS_SERVER=$(subst .cpp,.bc,$(SOURCES_SERVER))

SUBOBJ_LOCAL=$(foreach d, $(OBJS_CLIENT), $(OBJDIR_CLIENT_LOCAL)/$d)
SUBOBJ_WEB=$(foreach d, $(OBJS_CLIENT), $(OBJDIR_CLIENT_WEB)/$d)
SUBOBJ_SERVER=$(foreach d, $(OBJS_SERVER), $(OBJDIR_CLIENT_SERVER)/$d)


local: $(SUBOBJ_LOCAL)
	$(LOCAL_COMPILER) -std=c++14 -Wl,-rpath=/usr/lib/ -o $(LOCAL_TARGET) $(INC_PARAMS) $(SUBOBJ_LOCAL) -lSDL2 -lSDL2_net -lutil 
	ls
	cp shared/prototypes.json $(OUT)/prototypes.json
	spd-say 'i' --volume -90
	
web: $(SUBOBJ_WEB)
	$(WEB_COMPILER) -s USE_SDL=2 -s USE_SDL_NET=2 -o $(WEB_TARGET).html $(SUBOBJ_WEB)
	cp shared/prototypes.json $(OUT)/prototypes.json
	spd-say 'i' --volume -90
	
server: $(SUBOBJ_SERVER)
	$(LOCAL_COMPILER) -o $(SERVER_TARGET) $(SUBOBJ_SERVER) -lSDL2 -lSDL2_net
	cp shared/prototypes.json $(OUT)/prototypes.json
	spd-say 'i' --volume -90

$(OBJDIR_CLIENT_LOCAL)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(CXXFLAGS) $(LOCAL_FLAGS) -c -o $@ $<	
	
$(OBJDIR_CLIENT_WEB)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(WEB_COMPILER) $(CXXFLAGS) $(WEB_FLAGS) -c -o $@ $<	
	
$(OBJDIR_CLIENT_SERVER)/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(LOCAL_COMPILER) $(CXXFLAGS) $(LOCAL_FLAGS) -c -o $@ $<	

-include $(SUBOBJ_LOCAL:.bc=.d)
-include $(SUBOBJ_WEB:.bc=.d)
-include $(SUBOBJ_SERVER:.bc=.d)
	
clean:
	rm -rf $(OBJDIR_CLIENT_LOCAL)/
	rm -rf $(OBJDIR_CLIENT_WEB)/
	rm -rf $(OBJDIR_CLIENT_SERVER)/
	rm -f $(LOCAL_TARGET)
	rm -f $(SERVER_TARGET)
	rm -f $(WEB_TARGET).html
	rm -f $(WEB_TARGET).html.map
	rm -f $(WEB_TARGET).js
	rm -f $(WEB_TARGET).wasm
	