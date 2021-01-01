CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -O2

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
LIBRARIES   := -lSDL2 -ldl -lSDL2_image
EXECUTABLE  := pbrengine

all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*.c
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES)

.PHONY: clean
clean:
	-rm $(BIN)/$(EXECUTABLE)
