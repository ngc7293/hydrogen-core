# Compiler
CXX = g++
CXXFLAGS = -D _DEBUG -c -g -Wall -std=c++11
ALLEG =`pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5`

SRCS = $(wildcard src/*.cc)
OBJS = $(patsubst src/%.cc,obj/%.o,$(SRCS))

all: hc

hc: $(OBJS)
	@echo $@
	@$(CXX) -o $@ $^ $(ALLEG)

obj/%.o: src/%.cc
	@echo $<
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
