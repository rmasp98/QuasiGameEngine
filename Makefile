CXX       := g++
LD        := g++
FLAGS	  := -std=c++17 -Wall -ggdb

MODULES   := main resource utils renderer physics interface interface/glfw interface/ui interface/ui/imgui utils/jsonFileManager utils/logging
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.cc))
OBJ       := $(patsubst src/%.cc,build/%.o,$(SRC))
EXE       := quasi

INCLUDES   = -I include -I include/interface/ui/imgui
LIBS +=  -lGL -lglfw -lGLEW -lfreeimage -lassimp -pthread
TEST_LIBS  = -lgtest

vpath %.cc $(SRC_DIR)

define make-goal
$1/%.o: %.cc
	$(CXX) $(FLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs clean test

all: checkdirs $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(FLAGS) $^ -o $@ $(LIBS)


checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@



test:
	$(CXX) $(FLAGS) $(INCLUDES) test/interface/deviceInterfaceTest.cc -o test/interface/deviceInterface.test $(LIBS) $(TEST_LIBS)



cppcheck:
	cppcheck $(INCLUDES) --enable=all --cppcheck-build-dir=$(BUILD_DIR) --std=c++11 src/


clean:
	@rm -rf build
	@rm -f $(EXE)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
