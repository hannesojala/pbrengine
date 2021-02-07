CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb3 -O3

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
LIBRARIES   := -lSDL2 -ldl -lSDL2_image -lassimp
EXECUTABLE  := pbrengine

all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*.c
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES)

.PHONY: clean
clean:
	-rm $(BIN)/$(EXECUTABLE)
