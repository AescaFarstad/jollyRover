CXX=../emsdk/emscripten/1.38.24/em++
CXXFLAGS=-g
LDFLAGS=-g
OBJDIR=bc
TARGET=debugBuild
HTML_NAME=out/output

SDL_LIB = -Wl,-rpath=/usr/lib/
CXXFLAGS += -Wall -c -std=c++14 -D_REENTRANT -s USE_SDL=2 -s USE_SDL_NET=2
LDFLAGS += $(SDL_LIB)
LDFLAGS += -lutil


INC=client
INC += client/view
INC += shared
INC += shared/input
INC += shared/model
INC += shared/model/entity
INC += shared/model/prototypes
INC += shared/network
INC += shared/network/bindings
INC += shared/network/messages
INC += shared/thirdParty
INC += shared/utils

INC_PARAMS=$(foreach d, $(INC), -I$d)
OBJ_DIR=$(foreach d, $(INC), bc/$d)
VPATH=$(INC)

CXXFLAGS += $(INC_PARAMS)
CXXFLAGS += -MMD -MF $(subst .bc,.d,$@)

SRCS = $(wildcard client/*.cpp) 
SRCS += $(wildcard client/view/*.cpp)
SRCS += $(wildcard shared/*.cpp)
SRCS += $(wildcard shared/input/*.cpp)
SRCS += $(wildcard shared/model/*.cpp)
SRCS += $(wildcard shared/model/entity/*.cpp)
SRCS += $(wildcard shared/model/prototypes/*.cpp)
SRCS += $(wildcard shared/network/*.cpp)
SRCS += $(wildcard shared/network/bindings/*.cpp)
SRCS += $(wildcard shared/network/messages/*.cpp)
SRCS += $(wildcard shared/thirdParty/*.cpp)
SRCS += $(wildcard shared/utils/*.cpp)


OBJS=$(subst .cpp,.bc,$(SRCS))
SUBOBJ=$(foreach d, $(OBJS), $(OBJDIR)/$d)


$(TARGET): $(SUBOBJ)
	$(CXX) -s USE_SDL=2 -s USE_SDL_NET=2 -o $(HTML_NAME).html $(SUBOBJ)

bc/%.bc : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_PARAMS) -c -o $@ $<	

-include $(SUBOBJ:.bc=.d)
	
clean:
	rm -rf $(OBJDIR)/
	rm -f $(TARGET)
	rm -f $(HTML_NAME).html
	rm -f $(HTML_NAME).html.map
	rm -f $(HTML_NAME).js
	rm -f $(HTML_NAME).wasm
	