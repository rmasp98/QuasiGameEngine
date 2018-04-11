CXX       := g++
LD        := g++
FLAGS	  := -std=c++17 -Wall -ggdb -rdynamic -fstack-check

MODULES   := main resource utils renderer physics interface interface/glfw utils/jsonFileManager utils/logging
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cpp))
OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
EXE       := quasi

INCLUDES   = -I include
LIBS +=  -lGL -lglfw -lGLEW -lfreeimage -lassimp -pthread

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

cppcheck:
	cppcheck $(INCLUDES) --enable=all --cppcheck-build-dir=$(BUILD_DIR) --std=c++11 src/


clean:
	@rm -rf build
	@rm -f $(EXE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
