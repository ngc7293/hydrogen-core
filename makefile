# Compiler
CXX = g++
CXXFLAGS = -D _DEBUG -c -g -Wall -std=c++11
ALLEG =`pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5`

CORESRCS = $(wildcard src/hc/*.cc)
COREOBJS = $(patsubst src/hc/%.cc,obj/hc/%.o,$(CORESRCS))

GAMESRCS = $(wildcard src/game/*.cc)
GAMEOBJS = $(patsubst src/game/%.cc,obj/game/%.o,$(GAMESRCS))

all: hc.o

hc.o: $(COREOBJS) $(GAMEOBJS) $(POLYOBJS)
	@echo $@
	@$(CXX) -o $@ $^ $(ALLEG)

obj/hc/%.o: src/hc/%.cc
	@echo $<
	@mkdir -p obj/hc
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/game/%.o: src/game/%.cc
	@echo $<
	@mkdir -p obj/game
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I src/hc/

clean:
	@rm -rf obj/hc/* obj/game/* hc.o