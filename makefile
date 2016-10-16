# Compiler
CXX = g++
CXXFLAGS = -MMD -D _DEBUG -c -g -Wall -std=c++11
ALLEG =`pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5`

CORESRCS = $(wildcard src/hc/*.cc)
COREOBJS = $(patsubst src/hc/%.cc,obj/hc/%.o,$(CORESRCS))
COREDEPS = $(patsubst src/hc/%.cc,obj/hc/%.d,$(CORESRCS))

UTILSRCS = $(wildcard src/util/*.cc)
UTILOBJS = $(patsubst src/util/%.cc,obj/util/%.o,$(UTILSRCS))
UTILDEPS = $(patsubst src/util/%.cc,obj/util/%.d,$(UTILSRCS))

GAMESRCS = $(wildcard src/game/*.cc)
GAMEOBJS = $(patsubst src/game/%.cc,obj/game/%.o,$(GAMESRCS))
GAMEDEPS = $(patsubst src/game/%.cc,obj/game/%.d,$(GAMESRCS))

all: hc.o

hc.o: $(COREOBJS) $(GAMEOBJS) $(UTILOBJS)
	@echo $@
	@$(CXX) -o $@ $^ $(ALLEG)

obj/util/%.o: src/util/%.cc
	@echo $<
	@mkdir -p obj/util
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	
obj/hc/%.o: src/hc/%.cc
	@echo $<
	@mkdir -p obj/hc
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I src/util/

obj/game/%.o: src/game/%.cc
	@echo $<
	@mkdir -p obj/game
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I src/hc/ -I src/util/

clean:
	@rm -rf obj/* hc.o

-include $(COREDEPS) $(UTILDEPS) $(GAMEDEPS)