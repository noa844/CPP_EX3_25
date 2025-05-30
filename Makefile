# === Compiler Settings ===
CXX        := g++
CXXFLAGS = -std=c++17 -Wall -g -Iplayer/roles -Igame_logic -IGUI \
           -fsanitize=address -fno-omit-frame-pointer
LDFLAGS  = -fsanitize=address

# SFML configuration (via pkg-config)
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics)
SFML_LIBS   := $(shell pkg-config --libs   sfml-graphics)

# === Sources ===
SRC = main.cpp $(wildcard GUI/*.cpp) $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp) $(wildcard game_logic/*.cpp)
TESTSRC = test/test.cpp $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp) $(wildcard game_logic/*.cpp)

# === Executables ===
EXEC       := coup_demo
TEST_EXEC  := coup_test

# === Default target: build GUI app ===
all: $(EXEC)

# === Build main program with SFML ===
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SFML_CFLAGS) -o $@ $^ $(SFML_LIBS)

# === Run main program ===
run: $(EXEC)
	./$(EXEC)

# === Build and run tests (no SFML needed) ===
$(TEST_EXEC): $(TESTSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: $(TEST_EXEC)
	./$(TEST_EXEC)

# === Valgrind on demo ===
valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC)

# === Clean ===
clean:
	rm -f $(EXEC) $(TEST_EXEC)
