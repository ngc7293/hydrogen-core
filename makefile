# Compiler
CXX = g++
CXXFLAGS = -D _DEBUG -c -g -Wall -std=c++11
ALLEG =`pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_image-5 allegro_font-5 allegro_ttf-5`

SRC_DIR := src/
BIN_DIR := obj/

SUBFOLDERS := $(sort $(dir $(wildcard $(SRC_DIR)*/)))

SRCS := $(foreach sdir,$(SUBFOLDERS),$(wildcard $(sdir)*.cc))
BINS := $(patsubst $(SRC_DIR)%.cc,$(BIN_DIR)%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)%.cc,$(BIN_DIR)%.d,$(SRCS))

POLYSRCS = $(wildcard src/poly/*.cc)
POLYOBJS = $(patsubst src/poly/%.cc,obj/poly/%.o,$(POLYSRCS))
POLYDEPS = $(patsubst src/poly/%.cc,obj/poly/%.d,$(POLYSRCS))

hc.o: $(BINS)
	@echo $@
	@$(CXX) -o $@ $^ $(ALLEG)

$(BIN_DIR)%.o: $(SRC_DIR)%.cc
	@echo $(subst src/,"",$<)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -Isrc/hc/ -Isrc/util/

clean:
	@rm -rf $(BIN_DIR) hc.o

run:
	@./hc.o

debug:
	@gdb -d $(SRC_DIR) -ex run -ex quit -q ./hc.o

-include $(DEPS)