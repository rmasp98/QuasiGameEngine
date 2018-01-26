CXX       := g++
LD        := g++
FLAGS	  := -std=c++11 -Wall -ggdb

MODULES   := main resource utils renderer physics input
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
EXE       := xxx

INCLUDES   = -I include
LIBS += -lfreeimage -lGL -lglfw -lGLEW

vpath %.cpp $(SRC_DIR)

define make-goal
$1/%.o: %.cpp
	$(CXX) $(FLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

all: checkdirs $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(FLAGS) $^ -o $@ $(LIBS)


checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf build
	@rm -f $(EXE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
