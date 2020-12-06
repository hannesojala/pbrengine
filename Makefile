CXX       := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -O2

BIN     := bin
SRC     := src
INCLUDE := include
LIB     := lib
LIBRARIES   := -lSDL2 -ldl
EXECUTABLE  := pbrengine


all: $(BIN)/$(EXECUTABLE)

run: clean all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*.c
	$(CXX) $(CXX_FLAGS) -I $(INCLUDE) -L $(LIB) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/$(EXECUTABLE)
