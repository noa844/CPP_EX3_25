# === Compiler Settings ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iplayer/roles -Itest

# === Sources ===
SRC = main.cpp Game.cpp $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp)
TESTSRC = test/test.cpp Game.cpp $(wildcard player/*.cpp) $(wildcard player/roles/*.cpp)

# === Executables ===
EXEC = coup_demo
TEST_EXEC = coup_test

# === Default target ===
all: $(EXEC)

# === Build main program ===
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Run main program ===
run: $(EXEC)
	./$(EXEC)

# === Build and run tests ===
test: $(TEST_EXEC)
	./$(TEST_EXEC)

$(TEST_EXEC): $(TESTSRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Valgrind memory check ===
valgrind: $(EXEC)
	valgrind --leak-check=full ./$(EXEC)

# === Clean compiled files ===
clean:
	rm -f $(EXEC) $(TEST_EXEC)
