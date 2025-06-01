# === Compiler Settings ===
CXX := g++

# AddressSanitizer build (default)
CXXFLAGS_ASAN = -std=c++17 -Wall -g -Iplayer/roles -Igame_logic -IGUI \
                -fsanitize=address -fno-omit-frame-pointer
LDFLAGS_ASAN  = -fsanitize=address

# Valgrind build (sans asan)
CXXFLAGS_VALGRIND = -std=c++17 -Wall -g -Iplayer/roles -Igame_logic -IGUI
LDFLAGS_VALGRIND  =

# SFML configuration
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics)
SFML_LIBS   := $(shell pkg-config --libs sfml-graphics)

# === Sources ===
SRC = main.cpp $(wildcard GUI/*.cpp) $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp) $(wildcard game_logic/*.cpp)
TESTSRC = test/test.cpp $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp) $(wildcard game_logic/*.cpp)

# === Executables ===
EXEC         := coup_demo
VALGRIND_EXEC := coup_demo_valgrind
TEST_EXEC    := coup_test

# === Default target ===
all: $(EXEC)

# === Build with AddressSanitizer ===
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS_ASAN) $(SFML_CFLAGS) -o $@ $^ $(SFML_LIBS) $(LDFLAGS_ASAN)

# === Build without AddressSanitizer (Valgrind) ===
$(VALGRIND_EXEC): $(SRC)
	$(CXX) $(CXXFLAGS_VALGRIND) $(SFML_CFLAGS) -o $@ $^ $(SFML_LIBS) $(LDFLAGS_VALGRIND)

# === Run main program ===
run: $(EXEC)
	./$(EXEC)

# === Valgrind run ===
valgrind: $(VALGRIND_EXEC)
	valgrind --leak-check=full --show-leak-kinds=definite ./coup_demo_valgrind ./$(VALGRIND_EXEC)

# === Build and run tests (no SFML needed) ===
$(TEST_EXEC): $(TESTSRC)
	$(CXX) $(CXXFLAGS_ASAN) -o $@ $^ $(LDFLAGS_ASAN)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

# === Clean ===
clean:
	rm -f $(EXEC) $(VALGRIND_EXEC) $(TEST_EXEC)
