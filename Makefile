# === Compiler Settings ===
CXX := g++

# AddressSanitizer build (default)
CXXFLAGS_ASAN = -std=c++17 -Wall -g -Iplayer/roles -Igame_logic -IGUI \
                -fsanitize=address -fno-omit-frame-pointer
LDFLAGS_ASAN  = -fsanitize=address

# Valgrind build 
CXXFLAGS_VALGRIND = -std=c++17 -Wall -g -Iplayer/roles -Igame_logic -IGUI
LDFLAGS_VALGRIND  =

# SFML configuration
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics)
SFML_LIBS   := $(shell pkg-config --libs sfml-graphics)

# === Sources ===
SRC = main.cpp \
      $(wildcard GUI/*.cpp) \
      $(wildcard player/*.cpp) \
      $(wildcard player/roles/*.cpp) \
      $(wildcard game_logic/*.cpp)

DEMO_SRC = demo.cpp  $(wildcard GUI/*.cpp) \
      $(wildcard player/*.cpp) \
      $(wildcard player/roles/*.cpp) \
      $(wildcard game_logic/*.cpp)

TESTSRC = test/test.cpp \
          $(wildcard player/*.cpp) \
          $(wildcard player/roles/*.cpp) \
          $(wildcard game_logic/*.cpp)

# === Executables ===
EXEC           := coup_demo
VALGRIND_EXEC  := coup_demo_valgrind
TEST_EXEC      := coup_test
DEMO_EXEC      := demo_valgrind

# === Default target ===
all: $(EXEC)

# === Build with AddressSanitizer ===
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS_ASAN) $(SFML_CFLAGS) -o $@ $^ $(SFML_LIBS) $(LDFLAGS_ASAN)

# === Build without AddressSanitizer (Valgrind) ===
$(VALGRIND_EXEC): $(SRC)
	$(CXX) $(CXXFLAGS_VALGRIND) $(SFML_CFLAGS) -o $@ $^ $(SFML_LIBS) $(LDFLAGS_VALGRIND)

# === Build demo with Valgrind flags ===
$(DEMO_EXEC): $(DEMO_SRC)
	$(CXX) $(CXXFLAGS_VALGRIND) $(SFML_CFLAGS) -o $@ $(filter-out demo.cpp,$^) $(SFML_LIBS) $(LDFLAGS_VALGRIND) demo.cpp

# === Run main program ===
run: $(EXEC)
	./$(EXEC)

# === Valgrind run on main program ===
valgrind: $(VALGRIND_EXEC)
	valgrind --leak-check=full --show-leak-kinds=definite ./$(VALGRIND_EXEC)

# === Build and run demo under Valgrind ===
demo: $(DEMO_EXEC)
	valgrind --leak-check=full --show-leak-kinds=definite ./$(DEMO_EXEC)

# === Build and run tests (no SFML needed) ===
$(TEST_EXEC): $(TESTSRC)
	$(CXX) $(CXXFLAGS_ASAN) -o $@ $^ $(LDFLAGS_ASAN)

test: $(TEST_EXEC)
	./$(TEST_EXEC)

# === Clean ===
clean:
	rm -f $(EXEC) $(VALGRIND_EXEC) $(DEMO_EXEC) $(TEST_EXEC)
